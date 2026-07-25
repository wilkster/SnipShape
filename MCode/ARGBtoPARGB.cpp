#include <stdint.h>
#include <stdlib.h>
#include <string.h>

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
__stdcall int ARGBtoPARGB(uint8_t *pixels, int width, int height, int stride)
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
         uint8_t a = p[3];
         if (a != 255)
         {
            p[0] = (uint8_t)((p[0] * a + 127) / 255);
            p[1] = (uint8_t)((p[1] * a + 127) / 255);
            p[2] = (uint8_t)((p[2] * a + 127) / 255);
            converted++;
         }
         p += 4;
      }
      row += stride;
   }
   return converted;
}

/*
-m64 -O2 -fPIC -fno-stack-protector -fno-plt -fno-asynchronous-unwind-tables
x64 := "
(
hdIPjrAAAABBVFVXVkSJxlNFhcAPjpEAAABJY/lIictBidNFMcBFMdJBuYGAgICQSInaMckPHwAPtkIDPP90SUQPtuAPtgIPtmoB
QYPCAUEPr8RBD6/sg8B/SQ+vwYPFf0kPr+lIwegniAIPtkICSMHtJ0CIagFBD6/Eg8B/SQ+vwUjB6CeIQgKDwQFIg8IEQTnLf6NB
g8ABSAH7RDnGf49bRInQXl9dQVzDRTHSW15EidBfXUFcw0Ux0kSJ0MOQ
)"

-m32 -O2 -fPIC -fno-stack-protector -fno-plt -fno-asynchronous-unwind-tables
x86 := "
(
VVdWU4PsBItUJByF0g+OoAAAAItEJCCFwA+OlAAAAMcEJAAAAAAx7b+BgICAjXYAi0wkGDH2jbQmAAAAAI12AA+2WQOA+/90PQ+2
EYPFAQ+v04PCf4nQ9+fB6geIEQ+2UQEPr9ODwn+J0PfnweoHiFEBD7ZRAg+v2o1Tf4nQ9+fB6geIUQKDwQSDxgE5dCQcf66LRCQk
gwQkAQFEJBiLBCQ5RCQgf4mDxASJ6FteX13CEACDxAQx7VuJ6F5fXcIQAJCQkJCQkJCQkJCQkJCQkA==
)"

*/




