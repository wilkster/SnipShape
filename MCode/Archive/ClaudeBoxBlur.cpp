#include <stdint.h>
#include <stdlib.h>
#include <string.h>

// Efficient in-place box blur using sliding window sums.
// One horizontal pass + one vertical pass = O(w*h) regardless of radius.
// pixels: BGRA bitmap (GDI+ PixelFormat32bppARGB in memory)
// stride: bytes per row (may include padding)
// w, h:   region dimensions in pixels
// radius: blur radius in pixels (1 = 3x3 kernel, 2 = 5x5, etc.)
void blur_bitmap(uint8_t *pixels, int stride, int w, int h, int radius)
{
      if (!pixels || w <= 0 || h <= 0 || radius <= 0)
            return;

      // Clamp radius so the kernel never exceeds the image dimensions
      if (radius >= w / 2)
            radius = w / 2 - 1;
      if (radius >= h / 2)
            radius = h / 2 - 1;
      if (radius <= 0)
            return;

      const int diameter = 2 * radius + 1;

      // Intermediate row buffer: one row of BGRA uint32_t sums
      // (uint32_t is wide enough: max sum = 255 * diameter, diameter <= min(w,h),
      //  real screenshots won't exceed ~4000px so max = 255*4000 = 1,020,000 < 2^20)
      uint32_t *row_buf = (uint32_t *)malloc((size_t)w * 4 * sizeof(uint32_t));
      if (!row_buf)
            return;

      // --- Horizontal pass ---
      // For each row, slide a window of width (2*radius+1) across and write
      // the average back. We work in uint32_t to avoid repeated clamping.
      for (int y = 0; y < h; y++)
      {
            uint8_t *src = pixels + (size_t)y * stride;
            uint32_t *dst = row_buf;

            // Prime the window with the first (radius+1) pixels,
            // clamping to the left edge for the out-of-bounds portion.
            uint32_t sb = 0, sg = 0, sr = 0, sa = 0;
            for (int i = -radius; i <= radius; i++)
            {
                  int xi = i < 0 ? 0 : i; // clamp left
                  const uint8_t *p = src + (size_t)xi * 4;
                  sb += p[0];
                  sg += p[1];
                  sr += p[2];
                  sa += p[3];
            }

            for (int x = 0; x < w; x++)
            {
                  dst[x * 4 + 0] = sb / (uint32_t)diameter;
                  dst[x * 4 + 1] = sg / (uint32_t)diameter;
                  dst[x * 4 + 2] = sr / (uint32_t)diameter;
                  dst[x * 4 + 3] = sa / (uint32_t)diameter;

                  // Slide window: add incoming right pixel, remove outgoing left pixel
                  int x_add = x + radius + 1;
                  if (x_add >= w)
                        x_add = w - 1; // clamp right
                  int x_sub = x - radius;
                  if (x_sub < 0)
                        x_sub = 0; // clamp left

                  const uint8_t *p_add = src + (size_t)x_add * 4;
                  const uint8_t *p_sub = src + (size_t)x_sub * 4;
                  sb += p_add[0] - p_sub[0];
                  sg += p_add[1] - p_sub[1];
                  sr += p_add[2] - p_sub[2];
                  sa += p_add[3] - p_sub[3];
            }

            // Write blurred row back
            for (int x = 0; x < w; x++)
            {
                  uint8_t *q = src + (size_t)x * 4;
                  q[0] = (uint8_t)dst[x * 4 + 0];
                  q[1] = (uint8_t)dst[x * 4 + 1];
                  q[2] = (uint8_t)dst[x * 4 + 2];
                  q[3] = (uint8_t)dst[x * 4 + 3];
            }
      }

      // --- Vertical pass ---
      // Same idea but column-by-column. We need a column-sized scratch buffer
      // since stride means we can't alias rows easily.
      uint32_t *col_buf = (uint32_t *)malloc((size_t)h * 4 * sizeof(uint32_t));
      if (!col_buf)
      {
            free(row_buf);
            return;
      }

      for (int x = 0; x < w; x++)
      {
// Helper macro: pointer to pixel (x, y)
#define PX(y_) (pixels + (size_t)(y_) * stride + (size_t)(x) * 4)

            uint32_t sb = 0, sg = 0, sr = 0, sa = 0;
            for (int i = -radius; i <= radius; i++)
            {
                  int yi = i < 0 ? 0 : i; // clamp top
                  const uint8_t *p = PX(yi);
                  sb += p[0];
                  sg += p[1];
                  sr += p[2];
                  sa += p[3];
            }

            for (int y = 0; y < h; y++)
            {
                  col_buf[y * 4 + 0] = sb / (uint32_t)diameter;
                  col_buf[y * 4 + 1] = sg / (uint32_t)diameter;
                  col_buf[y * 4 + 2] = sr / (uint32_t)diameter;
                  col_buf[y * 4 + 3] = sa / (uint32_t)diameter;

                  int y_add = y + radius + 1;
                  if (y_add >= h)
                        y_add = h - 1;
                  int y_sub = y - radius;
                  if (y_sub < 0)
                        y_sub = 0;

                  const uint8_t *p_add = PX(y_add);
                  const uint8_t *p_sub = PX(y_sub);
                  sb += p_add[0] - p_sub[0];
                  sg += p_add[1] - p_sub[1];
                  sr += p_add[2] - p_sub[2];
                  sa += p_add[3] - p_sub[3];
            }

            // Write blurred column back
            for (int y = 0; y < h; y++)
            {
                  uint8_t *q = PX(y);
                  q[0] = (uint8_t)col_buf[y * 4 + 0];
                  q[1] = (uint8_t)col_buf[y * 4 + 1];
                  q[2] = (uint8_t)col_buf[y * 4 + 2];
                  q[3] = (uint8_t)col_buf[y * 4 + 3];
            }

#undef PX
      }

      free(row_buf);
      free(col_buf);
}
