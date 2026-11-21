#include <stdint.h>
/*
   Make a captured window bitmap match the Windows 11 rounded window shape:
   pixels outside the rounded corners become transparent, and the corner edge
   is anti-aliased.

   Output is premultiplied ARGB (what GDI+ Format32bppPArgb / UpdateLayeredWindow
   expect):
     - every pixel gets alpha 255 (PrintWindow's alpha channel is not reliable)
     - pixels fully outside a corner become 0x00000000
     - edge pixels are scaled by their coverage (color and alpha)

   The radius comes from the caller (Windows 11 uses 8 px at 96 DPI, 4 px for
   DWMWCP_ROUNDSMALL, 0 when maximized), so no edge detection is needed. All
   four corners use the same mask, so top-down and bottom-up DIBs both work.

   Coverage: each corner pixel is sampled on a 16x16 grid (256 samples), so it
   is exact integer math with no floats, sqrt, or constant tables.

   Returns the number of corner pixels changed, or -1 on bad arguments.

   No imports. Build (x64):
     gcc -m64 -O3 -fno-toplevel-reorder -fno-tree-loop-distribute-patterns
         -fno-stack-protector -fno-asynchronous-unwind-tables -fno-pic
         -c RoundWindowCorners.cpp
*/
#define SS 16   // subsamples per axis per pixel

typedef uint32_t __attribute__((may_alias)) u32a;

static inline void ApplyCoverage(uint8_t *p, uint32_t cov)   // cov 0..255 (256 = untouched)
{
   if (cov == 0)
   {
      *(u32a *)p = 0;
      return;
   }
   p[0] = (uint8_t)((p[0] * cov + 128) >> 8);
   p[1] = (uint8_t)((p[1] * cov + 128) >> 8);
   p[2] = (uint8_t)((p[2] * cov + 128) >> 8);
   p[3] = (uint8_t)((255 * cov + 128) >> 8);
}

extern "C" int __stdcall RoundWindowCorners(uint8_t *bits, int w, int h, int stride, int radius)
{
   if (!bits || w <= 0 || h <= 0 || radius < 0 || radius > 4096)
      return -1;
   if (radius > w / 2)
      radius = w / 2;
   if (radius > h / 2)
      radius = h / 2;

   const intptr_t st = stride;

   // 1. Make every pixel opaque
   for (int y = 0; y < h; y++)
   {
      u32a *row = (u32a *)(bits + y * st);
      for (int x = 0; x < w; x++)
         row[x] |= 0xFF000000u;
   }
   if (radius == 0)
      return 0;

   // 2. Corners. Work in half-subsample units so sample centres are integers:
   //    sample (i, j) of pixel (cx, cy) sits at (2*(cx*SS+i)+1, 2*(cy*SS+j)+1),
   //    the circle centre at (2*radius*SS, 2*radius*SS), radius 2*radius*SS.
   //    (cx, cy) is measured inward from the outer corner.
   const int64_t C  = 2 * (int64_t)radius * SS;
   const int64_t R2 = C * C;
   int changed = 0;

   for (int cy = 0; cy < radius; cy++)
   {
      for (int cx = 0; cx < radius; cx++)
      {
         // Pixel fully inside if its outermost corner (cx, cy) is within the circle
         const int64_t ox = C - 2 * (int64_t)cx * SS, oy = C - 2 * (int64_t)cy * SS;
         if (ox * ox + oy * oy <= R2)
            break;   // everything further right on this row is inside too

         uint32_t inside = 0;
         for (int j = 0; j < SS; j++)
         {
            const int64_t dy  = C - (2 * ((int64_t)cy * SS + j) + 1);
            const int64_t dy2 = dy * dy;
            for (int i = 0; i < SS; i++)
            {
               const int64_t dx = C - (2 * ((int64_t)cx * SS + i) + 1);
               inside += (dx * dx + dy2 <= R2);
            }
         }
         if (inside >= SS * SS)
            continue;

         // Same mask at all four corners
         uint8_t *top = bits + cy * st;
         uint8_t *bot = bits + (h - 1 - cy) * st;
         ApplyCoverage(top + cx * 4, inside);
         ApplyCoverage(top + (w - 1 - cx) * 4, inside);
         ApplyCoverage(bot + cx * 4, inside);
         ApplyCoverage(bot + (w - 1 - cx) * 4, inside);
         changed += 4;
      }
   }
   return changed;
}
