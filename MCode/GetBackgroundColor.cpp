#include <stdint.h>
#include <stdlib.h>
#include <string.h>

uint32_t __stdcall GetBackgroundColor(const uint8_t *pixels, int width, int height, int stride)
{
   if (width <= 0 || height <= 0)
      return 0xFF000000;

   // Scale step to image size — larger images need less density
   const int step = (width > 512 || height > 512) ? 4 : (width > 64 || height > 64) ? 2
                                                                                    : 1;

   // Estimate sampled pixel count and pick smallest table that keeps load < 25%
   // (4x slots per sampled pixel avoids collisions without over-allocating)
   const uint32_t sampled = (uint32_t)((width / step) * (height / step));
   uint32_t TABLE_BITS = 14; // minimum: 16K slots, 256KB
   while (TABLE_BITS < 20 && (1u << TABLE_BITS) < sampled * 4)
      TABLE_BITS++; // maximum: 1M slots, 16MB

   const uint32_t TABLE_SIZE = 1u << TABLE_BITS;
   const uint32_t TABLE_MASK = TABLE_SIZE - 1;

   uint32_t *table = (uint32_t *)calloc(TABLE_SIZE * 2, sizeof(uint32_t));
   if (!table)
      return 0xFF000000;

   const uint8_t *row = pixels;
   for (int y = 0; y < height; y += step)
   {
      const uint8_t *p = row;
      for (int x = 0; x < width; x += step)
      {
         uint32_t b = p[0];
         uint32_t g = p[1];
         uint32_t r = p[2];
         uint32_t a = p[3];
         p += 4 * step;
         // ignore if alpha channel is 0
         if (a == 0)
            continue;
         uint32_t rgb = (r << 16) | (g << 8) | b;
         uint32_t h = (rgb * 2654435761u) >> (32 - TABLE_BITS);
         uint32_t probe_dist = 0;
         for (;;)
         {
            uint32_t idx = (h & TABLE_MASK) * 2;
            uint32_t count = table[idx + 1];
            if (count == 0)
            {
               table[idx + 0] = rgb;
               table[idx + 1] = 1;
               break;
            }
            if (table[idx + 0] == rgb)
            {
               table[idx + 1] = count + 1;
               break;
            }
            uint32_t existing_home = (table[idx + 0] * 2654435761u) >> (32 - TABLE_BITS);
            uint32_t existing_dist = (h - existing_home) & TABLE_MASK;
            if (existing_dist < probe_dist)
            {
               uint32_t tmp_rgb = table[idx + 0];
               uint32_t tmp_count = table[idx + 1];
               table[idx + 0] = rgb;
               table[idx + 1] = 1;
               rgb = tmp_rgb;
               count = tmp_count;
               probe_dist = existing_dist;
            }
            h++;
            probe_dist++;
            if (probe_dist >= TABLE_SIZE)
            {
               uint32_t min_idx = 0;
               uint32_t min_count = UINT32_MAX;
               for (uint32_t j = 0; j < TABLE_SIZE; j++)
               {
                  if (table[j * 2 + 1] < min_count)
                  {
                     min_count = table[j * 2 + 1];
                     min_idx = j;
                  }
               }
               table[min_idx * 2 + 0] = rgb;
               table[min_idx * 2 + 1] = 1;
               break;
            }
         }
      }
      row += stride * step;
   }

   uint32_t best_color = 0;
   uint32_t best_count = 0;
   for (uint32_t i = 0; i < TABLE_SIZE; i++)
   {
      uint32_t count = table[i * 2 + 1];
      if (count > best_count)
      {
         best_count = count;
         best_color = table[i * 2 + 0];
      }
   }

   free(table);
   return best_color;
}