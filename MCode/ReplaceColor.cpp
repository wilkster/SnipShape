#include <stdint.h>
#include <stdlib.h>
#include <string.h>
/*
   matchAlpha = true, then full fromARGB must match and full toARTB is used
              = false, then only the rgb values must match but the from alpha is used for toment in the
*/
__stdcall int ToColor(uint8_t *pixels, int width, int height, int stride,
                           uint32_t fromARGB, uint32_t toARGB, int matchAlpha)
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
         // Ignore alpha channel
         if (p[0] == fmB && p[1] == fmG && p[2] == fmR && (!matchAlpha || p[3] == fmA))
         {
            p[0] = toB;
            p[1] = toG;
            p[2] = toR;
            p[3] = toA; // To with either the toment alpha or from alpha
            replaced++;
         }
         p += 4;
      }
      row += stride;
   }
   return replaced;
}