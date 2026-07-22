#include <stdint.h>
#include <stdlib.h>
#include <string.h>
// Replace alpha channel for every pixel that doesn't have it already
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
         // Ignore matching alpha channel
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
If using a buffer and no check
void SetAlphaChannel(unsigned int * start, unsigned int * end, unsigned char alpha) {
    while (start < end) {
        *((unsigned char *) start + 3) = alpha;
        start++;
    }
}

*/