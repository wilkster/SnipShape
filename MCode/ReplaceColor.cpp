#include <stdint.h>
#include <stdlib.h>
#include <string.h>
/*
   matchAlpha = true, then full targetARGB must match and full replaceARTB is used
              = false, then only the rgb values must match but the target alpha is used for replacement in the
*/
__stdcall int ReplaceColor(uint8_t *pixels, int width, int height, int stride,
                           uint32_t targetARGB, uint32_t replaceARGB, int matchAlpha)
{
   int replaced = 0;
   if (width <= 0 || height <= 0)
      return replaced;

   uint8_t ta = (targetARGB >> 24) & 0xFF;
   uint8_t tr = (targetARGB >> 16) & 0xFF;
   uint8_t tg = (targetARGB >> 8) & 0xFF;
   uint8_t tb = targetARGB & 0xFF;

   uint8_t ra = (replaceARGB >> 24) & 0xFF;
   uint8_t rr = (replaceARGB >> 16) & 0xFF;
   uint8_t rg = (replaceARGB >> 8) & 0xFF;
   uint8_t rb = replaceARGB & 0xFF;
   uint8_t *row = pixels;
   ra = matchAlpha ? ra : ta;
   // Replacement alpha is matchAlpha dependent
   for (int y = 0; y < height; y++)
   {
      uint8_t *p = row;
      for (int x = 0; x < width; x++)
      {
         // Ignore alpha channel
         if (p[0] == tb && p[1] == tg && p[2] == tr && (!matchAlpha || p[3] == ta))
         {
            p[0] = rb;
            p[1] = rg;
            p[2] = rr;
            p[3] = ra; // Note, Make sure replacement has a non-zero alpha channel
            replaced++;
         }
         p += 4;
      }
      row += stride;
   }
   return replaced;
}