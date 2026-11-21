/*
   Alpha handling for 32bpp BGRA pixel buffers, in place. Used by
   BitMap.PreMultiplyBitmap (Lib/clsBitmap.ahk2).

   Pixel layout in memory: p[0]=B, p[1]=G, p[2]=R, p[3]=A.

   replaceAlpha selects the mode:

   -1 or 0  Straight (non-premultiplied) ARGB -> premultiplied (PARGB):
               R' = R * A / 255   (same for G, B), A unchanged.
            Pixels with A = 255 are unchanged.

   1..255   Apply a window transparency to PREMULTIPLIED pixels: every
            channel (B, G, R and A) is scaled by replaceAlpha / 255.
            For opaque pixels this equals the old "replace alpha and
            premultiply" behaviour exactly; transparent and partly
            transparent pixels keep their relative transparency (a rounded
            corner with A = 0 stays transparent). 255 changes nothing.

   -2       Normalize a picture as it enters a snip. If no pixel has any
            alpha (all A = 0, typical of plain BitBlt copies and some
            clipboard sources) the alpha is meaningless: make every pixel
            opaque (A = 255, colours unchanged). Otherwise treat the pixels
            as straight ARGB and premultiply them (as for -1).

   Returns the number of pixels changed.

   No imports. Build (x64):
     gcc -m64 -O3 -fno-tree-vectorize -fno-tree-loop-distribute-patterns
         -fno-stack-protector -fno-asynchronous-unwind-tables -fno-pic -c ARGBtoPARGB.cpp
   (-fno-tree-vectorize: vectorized loops load constants from .rdata, which the
    MCode loader can't relocate)
*/
#include <stdint.h>

extern "C" __stdcall int ARGBtoPARGB(uint8_t *pixels, int width, int height, int stride, int replaceAlpha)
{
   int converted = 0;
   if (!pixels || width <= 0 || height <= 0)
      return converted;

   uint8_t *row;

   if (replaceAlpha >= 1 && replaceAlpha <= 255)
   {
      // Scale premultiplied pixels by the window alpha
      if (replaceAlpha == 255)
         return 0;
      const uint32_t r = (uint32_t)replaceAlpha;
      row = pixels;
      for (int y = 0; y < height; y++)
      {
         uint8_t *p = row;
         for (int x = 0; x < width; x++)
         {
            p[0] = (uint8_t)((p[0] * r + 127) / 255);
            p[1] = (uint8_t)((p[1] * r + 127) / 255);
            p[2] = (uint8_t)((p[2] * r + 127) / 255);
            p[3] = (uint8_t)((p[3] * r + 127) / 255);
            converted++;
            p += 4;
         }
         row += stride;
      }
      return converted;
   }

   if (replaceAlpha == -2)
   {
      // Is there any alpha at all?
      int anyAlpha = 0;
      row = pixels;
      for (int y = 0; y < height && !anyAlpha; y++)
      {
         uint8_t *p = row;
         for (int x = 0; x < width; x++, p += 4)
            if (p[3])
            {
               anyAlpha = 1;
               break;
            }
         row += stride;
      }
      if (!anyAlpha)
      {
         // No alpha information: the picture is opaque
         row = pixels;
         for (int y = 0; y < height; y++)
         {
            uint8_t *p = row;
            for (int x = 0; x < width; x++, p += 4)
               p[3] = 255;
            row += stride;
         }
         return width * height;
      }
      // Real alpha: fall through to straight -> premultiplied
   }

   // Straight -> premultiplied by each pixel's own alpha
   row = pixels;
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
