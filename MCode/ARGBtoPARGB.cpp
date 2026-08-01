/*
   Converts a buffer of straight (non-premultiplied) ARGB pixels to
   premultiplied ARGB (PARGB) in place.

   R' = R * A / 255
   G' = G * A / 255
   B' = B * A / 255
   A unchanged

   Pixel layout assumed BGRA in memory (p[0]=B, p[1]=G, p[2]=R, p[3]=A),
   matching ReplaceColor above. Returns count of pixels actually modified
   (alpha != 255, since A=255 pixels are unchanged by premultiply).
*/
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
__stdcall int ARGBtoPARGB(uint8_t *pixels, int width, int height, int stride, int replaceAlpha)
{
   int converted = 0;
   if (width <= 0 || height <= 0)
      return converted;

   uint8_t *row = pixels;
   for (int y = 0; y < height; y++)
   {
      uint8_t *p = row;
      for (int x = 0; x < width; x++)
      {
         uint8_t a = replaceAlpha > 0 ? (uint8_t)replaceAlpha : p[3];
         if (a != 255)
         {
            p[0] = (uint8_t)((p[0] * a + 127) / 255);
            p[1] = (uint8_t)((p[1] * a + 127) / 255);
            p[2] = (uint8_t)((p[2] * a + 127) / 255);
            p[3] = (uint8_t)a;
            converted++;
         }
         p += 4;
      }
      row += stride;
   }
   return converted;
}