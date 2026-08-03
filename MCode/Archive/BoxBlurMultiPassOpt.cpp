#include <stdint.h>
#include <stdlib.h>
#include <string.h>

void __stdcall BoxBlurMultiPass(int passes, const uint8_t *src, uint8_t *dst,
                                int stride, int w, int h, int radius, int horz_only)
{
   if (!src || !dst || w <= 0 || h <= 0 || radius <= 0 || passes <= 0)
      return;
   if (radius >= w / 2)
      radius = w / 2 - 1;
   if (!horz_only && radius >= h / 2)
      radius = h / 2 - 1;
   if (radius <= 0)
      return;

   const int diameter = 2 * radius + 1;

   uint8_t *tmp = (uint8_t *)malloc((size_t)h * stride);
   if (!tmp)
      return;

   // 4 separate channel arrays — sequential access in writeback
   uint32_t *cb0 = NULL, *cb1 = NULL, *cb2 = NULL, *cb3 = NULL;
   if (!horz_only)
   {
      cb0 = (uint32_t *)malloc((size_t)h * sizeof(uint32_t));
      cb1 = (uint32_t *)malloc((size_t)h * sizeof(uint32_t));
      cb2 = (uint32_t *)malloc((size_t)h * sizeof(uint32_t));
      cb3 = (uint32_t *)malloc((size_t)h * sizeof(uint32_t));
      if (!cb0 || !cb1 || !cb2 || !cb3)
      {
         free(cb0);
         free(cb1);
         free(cb2);
         free(cb3);
         free(tmp);
         return;
      }
   }

   // Ping-pong: cur is read source, nxt is write dest
   // We arrange so final result always lands in dst
   const uint8_t *cur = src;
   uint8_t *nxt = (passes % 2 == 1) ? dst : tmp;

   for (int pass = 0; pass < passes; pass++)
   {
      // ---- Horizontal pass: cur → nxt ----
      for (int y = 0; y < h; y++)
      {
         const uint8_t *row = cur + (size_t)y * stride;
         uint8_t *out = nxt + (size_t)y * stride;

         uint32_t sb = 0, sg = 0, sr = 0, sa = 0;

         // Prime with left-clamped window
         for (int i = -radius; i <= radius; i++)
         {
            int xi = i < 0 ? 0 : i;
            const uint8_t *p = row + xi * 4;
            sb += p[0];
            sg += p[1];
            sr += p[2];
            sa += p[3];
         }

         // Left edge zone: x_sub is clamped to 0
         int x_end_left = radius < w ? radius : w;
         int x_start_right = w - radius - 1;
         if (x_start_right < 0)
            x_start_right = 0;

         for (int x = 0; x < x_end_left; x++)
         {
            uint8_t *q = out + x * 4;
            q[0] = (uint8_t)(sb / (uint32_t)diameter);
            q[1] = (uint8_t)(sg / (uint32_t)diameter);
            q[2] = (uint8_t)(sr / (uint32_t)diameter);
            q[3] = (uint8_t)(sa / (uint32_t)diameter);

            int x_add = x + radius + 1;
            if (x_add >= w)
               x_add = w - 1;
            const uint8_t *p_add = row + x_add * 4;
            const uint8_t *p_sub = row; // x_sub = 0, clamped
            sb += p_add[0] - p_sub[0];
            sg += p_add[1] - p_sub[1];
            sr += p_add[2] - p_sub[2];
            sa += p_add[3] - p_sub[3];
         }

         // Center zone: no clamping needed at all
         for (int x = x_end_left; x < x_start_right; x++)
         {
            uint8_t *q = out + x * 4;
            q[0] = (uint8_t)(sb / (uint32_t)diameter);
            q[1] = (uint8_t)(sg / (uint32_t)diameter);
            q[2] = (uint8_t)(sr / (uint32_t)diameter);
            q[3] = (uint8_t)(sa / (uint32_t)diameter);

            const uint8_t *p_add = row + (x + radius + 1) * 4;
            const uint8_t *p_sub = row + (x - radius) * 4;
            sb += p_add[0] - p_sub[0];
            sg += p_add[1] - p_sub[1];
            sr += p_add[2] - p_sub[2];
            sa += p_add[3] - p_sub[3];
         }

         // Right edge zone: x_add is clamped to w-1
         const uint8_t *row_last = row + (w - 1) * 4;
         for (int x = x_start_right; x < w; x++)
         {
            uint8_t *q = out + x * 4;
            q[0] = (uint8_t)(sb / (uint32_t)diameter);
            q[1] = (uint8_t)(sg / (uint32_t)diameter);
            q[2] = (uint8_t)(sr / (uint32_t)diameter);
            q[3] = (uint8_t)(sa / (uint32_t)diameter);

            int x_sub = x - radius;
            if (x_sub < 0)
               x_sub = 0;
            const uint8_t *p_add = row_last; // clamped
            const uint8_t *p_sub = row + x_sub * 4;
            sb += p_add[0] - p_sub[0];
            sg += p_add[1] - p_sub[1];
            sr += p_add[2] - p_sub[2];
            sa += p_add[3] - p_sub[3];
         }
      }

      // ---- Vertical pass: nxt in-place ----
      if (!horz_only)
      {
         int y_end_top = radius < h ? radius : h;
         int y_start_bot = h - radius - 1;
         if (y_start_bot < 0)
            y_start_bot = 0;

         for (int x = 0; x < w; x++)
         {
            uint8_t *col = nxt + x * 4; // base of this column
            uint32_t sb = 0, sg = 0, sr = 0, sa = 0;

            // Prime
            for (int i = -radius; i <= radius; i++)
            {
               int yi = i < 0 ? 0 : i;
               const uint8_t *p = col + (size_t)yi * stride;
               sb += p[0];
               sg += p[1];
               sr += p[2];
               sa += p[3];
            }

            // Top edge
            for (int y = 0; y < y_end_top; y++)
            {
               cb0[y] = sb / (uint32_t)diameter;
               cb1[y] = sg / (uint32_t)diameter;
               cb2[y] = sr / (uint32_t)diameter;
               cb3[y] = sa / (uint32_t)diameter;

               int y_add = y + radius + 1;
               if (y_add >= h)
                  y_add = h - 1;
               const uint8_t *p_add = col + (size_t)y_add * stride;
               const uint8_t *p_sub = col; // y=0 clamped
               sb += p_add[0] - p_sub[0];
               sg += p_add[1] - p_sub[1];
               sr += p_add[2] - p_sub[2];
               sa += p_add[3] - p_sub[3];
            }

            // Center
            for (int y = y_end_top; y < y_start_bot; y++)
            {
               cb0[y] = sb / (uint32_t)diameter;
               cb1[y] = sg / (uint32_t)diameter;
               cb2[y] = sr / (uint32_t)diameter;
               cb3[y] = sa / (uint32_t)diameter;

               const uint8_t *p_add = col + (size_t)(y + radius + 1) * stride;
               const uint8_t *p_sub = col + (size_t)(y - radius) * stride;
               sb += p_add[0] - p_sub[0];
               sg += p_add[1] - p_sub[1];
               sr += p_add[2] - p_sub[2];
               sa += p_add[3] - p_sub[3];
            }

            // Bottom edge
            const uint8_t *col_last = col + (size_t)(h - 1) * stride;
            for (int y = y_start_bot; y < h; y++)
            {
               cb0[y] = sb / (uint32_t)diameter;
               cb1[y] = sg / (uint32_t)diameter;
               cb2[y] = sr / (uint32_t)diameter;
               cb3[y] = sa / (uint32_t)diameter;

               int y_sub = y - radius;
               if (y_sub < 0)
                  y_sub = 0;
               const uint8_t *p_add = col_last;
               const uint8_t *p_sub = col + (size_t)y_sub * stride;
               sb += p_add[0] - p_sub[0];
               sg += p_add[1] - p_sub[1];
               sr += p_add[2] - p_sub[2];
               sa += p_add[3] - p_sub[3];
            }

            // Writeback — sequential per channel
            for (int y = 0; y < h; y++)
            {
               uint8_t *q = col + (size_t)y * stride;
               q[0] = (uint8_t)cb0[y];
               q[1] = (uint8_t)cb1[y];
               q[2] = (uint8_t)cb2[y];
               q[3] = (uint8_t)cb3[y];
            }
         }
      }

      // Swap ping-pong for next pass
      if (pass < passes - 1)
      {
         cur = nxt;
         nxt = (nxt == dst) ? tmp : dst;
      }
   }

   free(cb0);
   free(cb1);
   free(cb2);
   free(cb3);
   free(tmp);
}