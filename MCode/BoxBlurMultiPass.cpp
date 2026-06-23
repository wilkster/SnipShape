#include <stdint.h>
#include <stdlib.h>
#include <string.h>
void blur_bitmap(int passes, const uint8_t *src, uint8_t *dst, int stride, int w, int h, int radius)
{
   if (!src || !dst || w <= 0 || h <= 0 || radius <= 0 || passes <= 0)
      return;
   if (radius >= w / 2)
      radius = w / 2 - 1;
   if (radius >= h / 2)
      radius = h / 2 - 1;
   if (radius <= 0)
      return;

   const int diameter = 2 * radius + 1;

   // Temp buffer for src→dst pass 1, and ping-pong for subsequent passes
   uint8_t *tmp = (uint8_t *)malloc((size_t)h * stride);
   if (!tmp)
      return;

   uint32_t *col_buf = (uint32_t *)malloc((size_t)h * 4 * sizeof(uint32_t));
   if (!col_buf)
   {
      free(tmp);
      return;
   }

   // On pass 1 read from src, subsequent passes ping-pong between dst and tmp
   const uint8_t *rd = src;
   uint8_t *wr = dst;

   for (int pass = 0; pass < passes; pass++)
   {

      // --- Horizontal pass: rd → wr ---
      for (int y = 0; y < h; y++)
      {
         const uint8_t *row = rd + (size_t)y * stride;
         uint8_t *out = wr + (size_t)y * stride;

         uint32_t sb = 0, sg = 0, sr = 0, sa = 0;
         for (int i = -radius; i <= radius; i++)
         {
            int xi = i < 0 ? 0 : i;
            const uint8_t *p = row + (size_t)xi * 4;
            sb += p[0];
            sg += p[1];
            sr += p[2];
            sa += p[3];
         }

         for (int x = 0; x < w; x++)
         {
            uint8_t *q = out + (size_t)x * 4;
            q[0] = (uint8_t)(sb / (uint32_t)diameter);
            q[1] = (uint8_t)(sg / (uint32_t)diameter);
            q[2] = (uint8_t)(sr / (uint32_t)diameter);
            q[3] = (uint8_t)(sa / (uint32_t)diameter);

            int x_add = x + radius + 1;
            if (x_add >= w)
               x_add = w - 1;
            int x_sub = x - radius;
            if (x_sub < 0)
               x_sub = 0;

            const uint8_t *p_add = row + (size_t)x_add * 4;
            const uint8_t *p_sub = row + (size_t)x_sub * 4;
            sb += p_add[0] - p_sub[0];
            sg += p_add[1] - p_sub[1];
            sr += p_add[2] - p_sub[2];
            sa += p_add[3] - p_sub[3];
         }
      }

// --- Vertical pass: wr → wr in-place ---
#define PX(y_) (wr + (size_t)(y_) * stride + (size_t)(x) * 4)

      for (int x = 0; x < w; x++)
      {
         uint32_t sb = 0, sg = 0, sr = 0, sa = 0;
         for (int i = -radius; i <= radius; i++)
         {
            int yi = i < 0 ? 0 : i;
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

         for (int y = 0; y < h; y++)
         {
            uint8_t *q = PX(y);
            q[0] = (uint8_t)col_buf[y * 4 + 0];
            q[1] = (uint8_t)col_buf[y * 4 + 1];
            q[2] = (uint8_t)col_buf[y * 4 + 2];
            q[3] = (uint8_t)col_buf[y * 4 + 3];
         }
      }

#undef PX

      // Ping-pong: next pass reads from wr, writes to tmp, then swap
      if (pass < passes - 1)
      {
         rd = wr;
         wr = (wr == dst) ? tmp : dst;
      }
   }

   // If the final result landed in tmp, copy it to dst
   if (wr != dst)
      memcpy(dst, tmp, (size_t)h * stride);

   free(col_buf);
   free(tmp);
}