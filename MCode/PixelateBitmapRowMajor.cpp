#include <stdint.h>
/*
   Row-major pixelate for 32bpp ARGB bitmaps (works for x64 and x86).

   For each row of blocks:
     1. Stream every scanline in that block row left to right, summing pixels
        into a small per-block-column accumulator on the stack.
     2. Divide each block's sums by its area to get one color per block.
     3. Stream the output scanlines left to right, filling each block span.

   Every read and write is sequential, so the hardware prefetcher stays busy,
   and there are no memcpy/memset calls, so the blob needs no imports.

   Block columns are processed in chunks of CHUNK so the stack frame stays
   well under 4 KB (larger frames make GCC emit a ___chkstk_ms call).

   Build (x64):
     gcc -m64 -O3 -fno-tree-loop-distribute-patterns -fno-stack-protector
         -fno-asynchronous-unwind-tables -fno-pic -c PixelateBitmapRowMajor.cpp
   -fno-tree-loop-distribute-patterns stops GCC from turning loops into
   memset/memcpy calls. Check `objdump -r` shows no relocations.

   Returns 0 on success, -1 on bad arguments.
*/
#define CHUNK 96

/*
   Fixed-point reciprocal so each block average is a multiply + shift instead
   of a divide (at Size 2 there is one block per 4 pixels, so divides dominate).

   m = floor(2^56 / area) + 1. For a channel sum s <= 255*area:
     (s*m) >> 56 == s / area   exactly, provided 255*area^2 < 2^56
   and s*m < 255*2^56 + 255*area < 2^64, so it never overflows.
   255*area^2 < 2^56 holds for area <= 16,000,000 (blocks up to ~4000x4000);
   anything larger falls back to a real divide.
*/
#define RECIP_SHIFT    56
#define RECIP_MAX_AREA 16000000u
static inline uint64_t Recip(uint32_t area)
{
   return ((uint64_t)1 << RECIP_SHIFT) / area + 1;
}

extern "C" int __stdcall PixelateBitmap(unsigned char *sBitmap, unsigned char *dBitmap,
                                        int w, int h, int Stride, int Size)
{
   if (!sBitmap || !dBitmap || w <= 0 || h <= 0 || Size <= 0)
      return -1;

   uint32_t accB[CHUNK], accG[CHUNK], accR[CHUNK], accA[CHUNK];
   uint32_t color[CHUNK];
   const intptr_t stride = Stride;

   for (int y0 = 0; y0 < h; y0 += Size)
   {
      const int bh = (h - y0 < Size) ? h - y0 : Size;

      // Walk the block columns of this block row in chunks of CHUNK
      for (int x0 = 0; x0 < w; )
      {
         int nCols = 0;
         int xEnd  = x0;
         while (nCols < CHUNK && xEnd < w)
         {
            xEnd += Size;
            nCols++;
         }
         if (xEnd > w)
            xEnd = w;
         // Width of the last block in this chunk (others are Size wide)
         const int lastW = xEnd - (x0 + (nCols - 1) * Size);

         // 1. Sum: read scanlines y0..y0+bh-1, columns x0..xEnd-1
         for (int c = 0; c < nCols; c++)
            accB[c] = accG[c] = accR[c] = accA[c] = 0;
         for (int y = 0; y < bh; y++)
         {
            const uint8_t *src = sBitmap + (y0 + y) * stride + (intptr_t)x0 * 4;
            for (int c = 0; c < nCols; c++)
            {
               const int bw = (c == nCols - 1) ? lastW : Size;
               uint32_t b = 0, g = 0, r = 0, a = 0;
               for (int x = 0; x < bw; x++, src += 4)
               {
                  b += src[0];
                  g += src[1];
                  r += src[2];
                  a += src[3];
               }
               accB[c] += b; accG[c] += g; accR[c] += r; accA[c] += a;
            }
         }

         // 2. Average each block. All blocks but the last share one area, so
         //    divide once per chunk and multiply per block (see Recip below).
         const uint32_t areaFull = (uint32_t)(Size * bh);
         const uint32_t areaLast = (uint32_t)(lastW * bh);
         if (areaFull <= RECIP_MAX_AREA)
         {
            const uint64_t mFull = Recip(areaFull), mLast = Recip(areaLast);
            for (int c = 0; c < nCols; c++)
            {
               const uint64_t m = (c == nCols - 1) ? mLast : mFull;
               color[c] = (uint32_t)((accA[c] * m) >> RECIP_SHIFT) << 24
                        | (uint32_t)((accR[c] * m) >> RECIP_SHIFT) << 16
                        | (uint32_t)((accG[c] * m) >> RECIP_SHIFT) << 8
                        | (uint32_t)((accB[c] * m) >> RECIP_SHIFT);
            }
         }
         else // huge blocks: few of them, plain divide is fine
         {
            for (int c = 0; c < nCols; c++)
            {
               const uint32_t area = (c == nCols - 1) ? areaLast : areaFull;
               color[c] = ((accA[c] / area) << 24) | ((accR[c] / area) << 16)
                        | ((accG[c] / area) << 8) | (accB[c] / area);
            }
         }

         // 3. Fill the output scanlines
         for (int y = 0; y < bh; y++)
         {
            uint32_t *dst = (uint32_t *)(dBitmap + (y0 + y) * stride) + x0;
            for (int c = 0; c < nCols; c++)
            {
               const int bw = (c == nCols - 1) ? lastW : Size;
               const uint32_t px = color[c];
               for (int x = 0; x < bw; x++)
                  *dst++ = px;
            }
         }

         x0 = xEnd;
      }
   }
   return 0;
}
