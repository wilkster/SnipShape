#include <stdint.h>
#include <stdlib.h>
#include <string.h>
/*
   matchAlpha = true, then full fromARGB must match and full toARGB is used
              = false, then only the rgb values must match but the from alpha is used for the toment

   th = per-channel (R,G,B) match tolerance. A pixel channel matches if it is
        within th of the corresponding fromARGB channel. th=0 reproduces the
        original exact-match behavior.

        On a match, the replaced channel is toChannel + (pixelChannel - fromChannel),
        i.e. the offset found during matching is preserved onto the "to" color,
        clamped to 0-255.
*/
static inline uint8_t clampU8(int v);

__stdcall int ToColor(uint8_t *pixels, int width, int height, int stride,
                           uint32_t fromARGB, uint32_t toARGB, int matchAlpha, int th)
{
   int replaced = 0;
   if (width <= 0 || height <= 0)
      return replaced;
   uint8_t fmA = (fromARGB >> 24) & 0xFF;
   uint8_t fmR = (fromARGB >> 16) & 0xFF;
   uint8_t fmG = (fromARGB >> 8) & 0xFF;
   uint8_t fmB = fromARGB & 0xFF;
   uint8_t toA = (toARGB >> 24) & 0xFF;
   uint8_t toR = (toARGB >> 16) & 0xFF;
   uint8_t toG = (toARGB >> 8) & 0xFF;
   uint8_t toB = toARGB & 0xFF;
   uint8_t *row = pixels;
   toA = matchAlpha ? toA : fmA;
   // Toment alpha is matchAlpha dependent
   for (int y = 0; y < height; y++)
   {
      uint8_t *p = row;
      for (int x = 0; x < width; x++)
      {
         int dB = (int)p[0] - (int)fmB;
         int dG = (int)p[1] - (int)fmG;
         int dR = (int)p[2] - (int)fmR;
         if (abs(dB) <= th && abs(dG) <= th && abs(dR) <= th && (!matchAlpha || p[3] == fmA))
         {
            p[0] = clampU8((int)toB + dB);
            p[1] = clampU8((int)toG + dG);
            p[2] = clampU8((int)toR + dR);
            p[3] = toA; // To with either the toment alpha or from alpha
            replaced++;
         }
         p += 4;
      }
      row += stride;
   }
   return replaced;
}
static inline uint8_t clampU8(int v)
{
   if (v < 0) return 0;
   if (v > 255) return 255;
   return (uint8_t)v;
}
