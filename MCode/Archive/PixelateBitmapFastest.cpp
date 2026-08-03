#include <stdint.h>
#include <string.h>
// 64 bit only
int __stdcall PixelateBitmap(unsigned char *sBitmap, unsigned char *dBitmap, int w, int h, int Stride, int Size)
{
   if (!sBitmap || !dBitmap || w <= 0 || h <= 0 || Size <= 0)
      return -1;

   const int blocksCols = w / Size;
   const int blocksRows = h / Size;
   const int remX = w % Size;
   const int remY = h % Size;
   const int blockArea = Size * Size;

   // Q64 fixed-point reciprocal of blockArea, computed with a PLAIN 64-bit
   // divide only (no __int128 division/modulo — those need __udivti3 /
   // __umodti3 from libgcc, which mcode/raw-machine-code extraction cannot
   // link). Identity: ceil(2^64 / blockArea), or the exact value when
   // blockArea divides 2^64 evenly — verified exhaustively/boundary-tested
   // for block sizes up to 4096x4096.
   //
   // blockArea == 1 (Size == 1) is degenerate for this formula (it would
   // require 2^64 itself, which overflows uint64_t), so it's special-cased
   // below: no division is needed there anyway (dividing by 1 is a no-op).
   uint64_t invBlockAreaQ64 = (blockArea > 1)
       ? ((uint64_t)-1 / (uint64_t)blockArea) + 1
       : 0;

   int totalBlockCols = blocksCols + (remX > 0 ? 1 : 0);
   int totalBlockRows = blocksRows + (remY > 0 ? 1 : 0);

   for (int by = 0; by < totalBlockRows; by++)
   {
      int bh = (by < blocksRows) ? Size : remY;

      for (int bx = 0; bx < totalBlockCols; bx++)
      {
         int bw = (bx < blocksCols) ? Size : remX;
         int area = bw * bh;
         int sA = 0, sR = 0, sG = 0, sB = 0;

         int originX = bx * Size;
         int originY = by * Size;

         for (int y = 0; y < bh; y++)
         {
            const unsigned char *src = sBitmap + (originY + y) * Stride + originX * 4;
            for (int x = 0; x < bw; x++, src += 4)
            {
               sB += src[0];
               sG += src[1];
               sR += src[2];
               sA += src[3];
            }
         }

         unsigned char aB, aG, aR, aA;
         if (area == blockArea && blockArea > 1)
         {
            // Multiply-only use of __int128 — compiles to a native 64x64->128
            // hardware multiply, no libgcc call involved (verified: only
            // memcpy shows up as an undefined symbol in the compiled object).
            aB = (unsigned char)(uint64_t)(((unsigned __int128)(uint64_t)sB * invBlockAreaQ64) >> 64);
            aG = (unsigned char)(uint64_t)(((unsigned __int128)(uint64_t)sG * invBlockAreaQ64) >> 64);
            aR = (unsigned char)(uint64_t)(((unsigned __int128)(uint64_t)sR * invBlockAreaQ64) >> 64);
            aA = (unsigned char)(uint64_t)(((unsigned __int128)(uint64_t)sA * invBlockAreaQ64) >> 64);
         }
         else if (area == 1)
         {
            // Single-pixel block (Size==1, or a 1x1 leftover edge block):
            // dividing by 1 is a no-op.
            aB = (unsigned char)sB;
            aG = (unsigned char)sG;
            aR = (unsigned char)sR;
            aA = (unsigned char)sA;
         }
         else
         {
            // Edge block with area != blockArea (and area != 1): real divide.
            aB = (unsigned char)(sB / area);
            aG = (unsigned char)(sG / area);
            aR = (unsigned char)(sR / area);
            aA = (unsigned char)(sA / area);
         }

         uint32_t packed = ((uint32_t)aA << 24) | ((uint32_t)aR << 16) | ((uint32_t)aG << 8) | aB;

         unsigned char *firstRow = dBitmap + originY * Stride + originX * 4;
         uint32_t *dst32 = (uint32_t *)firstRow;
         for (int x = 0; x < bw; x++)
            dst32[x] = packed;

         for (int y = 1; y < bh; y++)
            memcpy(dBitmap + (originY + y) * Stride + originX * 4, firstRow, (size_t)bw * 4);
      }
   }
   return 0;
}