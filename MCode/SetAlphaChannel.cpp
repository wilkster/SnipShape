#include <stdint.h>
#include <stdlib.h>
#include <string.h>
/*
   matchAlpha = true, then full targetARGB must match and full replaceARTB is used
              = false, then only the rgb values must match but the target alpha is used for replacement in the
*/
__stdcall int SetAlphaChannel(uint8_t *pixels, int width, int height, int stride, uint32_t replaceAlpha)
{
   int replaced = 0;
   if (width <= 0 || height <= 0)
      return replaced;

   uint8_t ra = replaceAlpha & 0xFF;
   uint8_t *row = pixels;
   // Replacement alpha is matchAlpha dependent
   for (int y = 0; y < height; y++)
   {
      uint8_t *p = row;
      for (int x = 0; x < width; x++)
      {
         // Ignore alpha channel
         if (p[3] != ra)
         {
            p[3] = ra; // Note, Make sure replacement has a non-zero alpha channel
            replaced++;
         }
         p += 4;
      }
      row += stride;
   }
   return replaced;
}
/*
x64 := "
(
hdJ+VFNEicNFhcB+Q0GJ0g+2VCQwTWPBRTHbRTHJZpAxwGYPH0QAADhUgQN0CIhUgQNBg8EBSIPAAUE5wn/pQYPDAUwBwUQ523/V
RInIW8NFMclbRInIw0UxyUSJyMOQ
)"

*/