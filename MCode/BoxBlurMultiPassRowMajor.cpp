#include <stdint.h>
#include <stdlib.h>
#include <emmintrin.h>   // SSE2 (baseline on every x64 CPU)
/*
   Multi-pass separable box blur for 32bpp ARGB bitmaps, clamp-to-edge.
   Drop-in replacement for BoxBlurMultiPassOpt.R1.cpp (same signature).

   Each pass:
     Horizontal: cur -> tmp  (running sum along each row)
     Vertical:   tmp -> dst  (row-major: one running sum per column/channel,
                              updated a whole row at a time)
   Both passes read and write memory sequentially, and the vertical pass
   never reads the buffer it writes. (The R1 version blurred vertically in
   place, column by column: it subtracted pixels it had already overwritten,
   and every access hit a new cache line.)

   With horz_only the passes ping-pong between dst and tmp so the last pass
   lands in dst.

   Averages use a 32x32->64 multiply and shift (no __int128), see MakeRecip.

   Imports: msvcrt malloc/free.
   Build (x64):
     gcc -m64 -O3 -fno-toplevel-reorder -fno-tree-loop-distribute-patterns
         -fno-stack-protector -fno-asynchronous-unwind-tables -fno-pic
         -c BoxBlurMultiPassRowMajor.cpp
   Check BoxBlurMultiPass is at offset 0 (objdump -t) and the only
   relocations are malloc/free (objdump -r).
   -fno-toplevel-reorder flag is required
*/

struct Recip
{
   uint32_t m;
   uint32_t shift;
};

/*
   m = floor(2^L / d) + 1 with L = 31 + floor(log2 d), so m < 2^32.
   For a sum s <= 255*d:  (s*m) >> L == s / d  exactly, provided 255*d^2 < 2^L,
   which holds for d < 2^30/255 (~4.2 million, far beyond any real radius),
   and s*m < 2^64 so the product never overflows.
*/
static inline Recip MakeRecip(uint32_t d)
{
   uint32_t lg = 0;
   while ((d >> (lg + 1)) != 0)
      lg++;
   Recip rc;
   rc.shift = 31 + lg;
   rc.m     = (uint32_t)((((uint64_t)1) << rc.shift) / d + 1);
   return rc;
}

static inline uint8_t Avg(uint32_t sum, Recip rc)
{
   return (uint8_t)(((uint64_t)sum * rc.m) >> rc.shift);
}

static void HPass(const uint8_t *in, uint8_t *out, intptr_t stride, int w, int h, int r, Recip rc);
static void VPass(const uint8_t *in, uint8_t *out, intptr_t stride, int w, int h, int r,
                  Recip rc, uint32_t *sum);

// Entry point is defined first so it lands at offset 0 of .text for MCode
// (needs -fno-toplevel-reorder, otherwise GCC may emit the helpers first).
extern "C" void __stdcall BoxBlurMultiPass(int passes, const uint8_t *src, uint8_t *dst,
                                           int stride, int w, int h, int radius, int horz_only)
{
   if (!src || !dst || w <= 0 || h <= 0 || radius <= 0 || passes <= 0)
      return;
   if (radius >= w / 2)
      radius = w / 2 - 1;
   if (!horz_only && radius >= h / 2)
      radius = h / 2 - 1;
   if (radius <= 0)
      return;

   const Recip rc = MakeRecip((uint32_t)(2 * radius + 1));
   const intptr_t st = stride;
   const size_t imgBytes = (size_t)h * (size_t)stride;

   // One allocation: temp image, then the column sums for the vertical pass
   uint8_t *tmp = (uint8_t *)malloc(imgBytes + (horz_only ? 0 : (size_t)w * 4 * sizeof(uint32_t)));
   if (!tmp)
      return;

   if (horz_only)
   {
      // Ping-pong so the final pass writes dst
      const uint8_t *cur = src;
      uint8_t *nxt = (passes & 1) ? dst : tmp;
      for (int pass = 0; pass < passes; pass++)
      {
         HPass(cur, nxt, st, w, h, radius, rc);
         cur = nxt;
         nxt = (nxt == dst) ? tmp : dst;
      }
   }
   else
   {
      uint32_t *colSum = (uint32_t *)(tmp + imgBytes);
      const uint8_t *cur = src;
      for (int pass = 0; pass < passes; pass++)
      {
         HPass(cur, tmp, st, w, h, radius, rc);
         VPass(tmp, dst, st, w, h, radius, rc, colSum);
         cur = dst;
      }
   }
   free(tmp);
}

/*
   Horizontal pass, SSE2. The four channel sums (B,G,R,A) live in one
   register as 4 x uint32, so each pixel costs one add + one subtract.
   Averages: pmuludq multiplies lanes 0,2 (B,R) and, after a 32-bit shift,
   lanes 1,3 (G,A) by m as 64-bit products; both are shifted right by L
   (count held in a register, so L can vary) and recombined.
*/
typedef uint32_t __attribute__((may_alias)) u32a;   // unaligned/aliased pixel access

static inline __m128i LoadPx(const uint8_t *p, __m128i z)
{
   __m128i v = _mm_cvtsi32_si128((int)*(const u32a *)p);
   v = _mm_unpacklo_epi8(v, z);            // 4 x u16
   return _mm_unpacklo_epi16(v, z);        // 4 x u32
}

static inline uint32_t AvgPx(__m128i s, __m128i m, __m128i sh)
{
   __m128i lo = _mm_srl_epi64(_mm_mul_epu32(s, m), sh);                        // B, R
   __m128i hi = _mm_srl_epi64(_mm_mul_epu32(_mm_srli_epi64(s, 32), m), sh);    // G, A
   __m128i v  = _mm_or_si128(lo, _mm_slli_epi64(hi, 32));                      // B,G,R,A
   v = _mm_packs_epi32(v, v);              // values <= 255, no saturation
   v = _mm_packus_epi16(v, v);
   return (uint32_t)_mm_cvtsi128_si32(v);
}

// Horizontal running-sum blur of every row: in -> out (must differ)
static void HPass(const uint8_t *in, uint8_t *out, intptr_t stride, int w, int h, int r, Recip rc)
{
   const int xEndLeft    = r;           // r < w/2, so these never overlap
   const int xStartRight = w - r - 1;
   const __m128i z  = _mm_setzero_si128();
   const __m128i m  = _mm_set1_epi32((int)rc.m);
   const __m128i sh = _mm_cvtsi32_si128((int)rc.shift);

   for (int y = 0; y < h; y++)
   {
      const uint8_t *row = in + y * stride;
      u32a *q = (u32a *)(out + y * stride);
      const __m128i first = LoadPx(row, z);
      const __m128i last  = LoadPx(row + (w - 1) * 4, z);

      // Window for x = 0: pixel 0 repeated r+1 times, plus pixels 1..r
      __m128i s = first;
      for (int i = 1; i <= r; i++)
         s = _mm_add_epi32(_mm_add_epi32(s, first), LoadPx(row + i * 4, z));

      int x = 0;
      // Left edge: outgoing pixel clamps to column 0
      for (; x < xEndLeft; x++)
      {
         *q++ = AvgPx(s, m, sh);
         s = _mm_sub_epi32(_mm_add_epi32(s, LoadPx(row + (x + r + 1) * 4, z)), first);
      }
      // Middle: no clamping
      const uint8_t *pa = row + (x + r + 1) * 4;
      const uint8_t *ps = row + (x - r) * 4;
      for (; x < xStartRight; x++, pa += 4, ps += 4)
      {
         *q++ = AvgPx(s, m, sh);
         s = _mm_sub_epi32(_mm_add_epi32(s, LoadPx(pa, z)), LoadPx(ps, z));
      }
      // Right edge: incoming pixel clamps to column w-1
      for (; x < w; x++)
      {
         *q++ = AvgPx(s, m, sh);
         s = _mm_sub_epi32(_mm_add_epi32(s, last), LoadPx(row + (x - r) * 4, z));
      }
   }
}

// Vertical blur, row-major: in -> out (must differ). sum holds w*4 uint32.
static void VPass(const uint8_t *in, uint8_t *out, intptr_t stride, int w, int h, int r,
                  Recip rc, uint32_t *sum)
{
   const int n = w * 4;

   // Window for y = 0: row 0 repeated r+1 times, plus rows 1..r
   for (int i = 0; i < n; i++)
      sum[i] = (uint32_t)(r + 1) * in[i];
   for (int y = 1; y <= r; y++)
   {
      const uint8_t *p = in + y * stride;
      for (int i = 0; i < n; i++)
         sum[i] += p[i];
   }

   for (int y = 0; y < h; y++)
   {
      const int ya = (y + r + 1 < h) ? y + r + 1 : h - 1;
      const int ys = (y - r > 0) ? y - r : 0;
      const uint8_t *pa = in + ya * stride;
      const uint8_t *ps = in + ys * stride;
      uint8_t *o = out + y * stride;
      for (int i = 0; i < n; i++)
      {
         const uint32_t s = sum[i];
         o[i]   = Avg(s, rc);
         sum[i] = s + pa[i] - ps[i];   // true value is never negative
      }
   }
}
