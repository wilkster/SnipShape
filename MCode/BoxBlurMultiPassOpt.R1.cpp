#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static inline uint8_t avgQ64(uint32_t sum, uint64_t invDiameterQ64)
{
    // Multiply-only __int128 use — native 64x64->128 hardware multiply,
    // no libgcc dependency (only the one-time reciprocal below uses a
    // plain 64-bit divide).
    return (uint8_t)(uint64_t)(((unsigned __int128)sum * invDiameterQ64) >> 64);
}

void __stdcall BoxBlurMultiPass(int passes, const uint8_t *src, uint8_t *dst,
                                 int stride, int w, int h, int radius, int horz_only)
{
    if (!src || !dst || w <= 0 || h <= 0 || radius <= 0 || passes <= 0)
        return;
    if (radius >= w / 2) radius = w / 2 - 1;
    if (!horz_only && radius >= h / 2) radius = h / 2 - 1;
    if (radius <= 0) return;

    const int diameter = 2 * radius + 1;

    // One reciprocal for the whole call — every division in this function
    // divides by the same constant `diameter`, so no edge-case fallback
    // is needed (unlike PixelateBitmap's variable block area).
    // Safe: diameter >= 3 always here (radius <= 0 already returned above),
    // so this never hits the d==1 overflow case.
    uint64_t invDiameterQ64 = ((uint64_t)-1 / (uint64_t)diameter) + 1;

    uint8_t *tmp = (uint8_t *)malloc((size_t)h * stride);
    if (!tmp) return;

    const uint8_t *cur = src;
    uint8_t *nxt = (passes % 2 == 1) ? dst : tmp;

    for (int pass = 0; pass < passes; pass++)
    {
        // ---- Horizontal pass: cur → nxt ----
        for (int y = 0; y < h; y++)
        {
            const uint8_t *row = cur + (size_t)y * stride;
            uint8_t       *out = nxt + (size_t)y * stride;

            uint32_t sb = 0, sg = 0, sr = 0, sa = 0;

            for (int i = -radius; i <= radius; i++) {
                int xi = i < 0 ? 0 : i;
                const uint8_t *p = row + xi * 4;
                sb += p[0]; sg += p[1]; sr += p[2]; sa += p[3];
            }

            int x_end_left  = radius < w ? radius : w;
            int x_start_right = w - radius - 1;
            if (x_start_right < 0) x_start_right = 0;

            for (int x = 0; x < x_end_left; x++) {
                uint8_t *q = out + x * 4;
                q[0] = avgQ64(sb, invDiameterQ64);
                q[1] = avgQ64(sg, invDiameterQ64);
                q[2] = avgQ64(sr, invDiameterQ64);
                q[3] = avgQ64(sa, invDiameterQ64);

                int x_add = x + radius + 1;
                if (x_add >= w) x_add = w - 1;
                const uint8_t *p_add = row + x_add * 4;
                const uint8_t *p_sub = row;
                sb += p_add[0] - p_sub[0];
                sg += p_add[1] - p_sub[1];
                sr += p_add[2] - p_sub[2];
                sa += p_add[3] - p_sub[3];
            }

            for (int x = x_end_left; x < x_start_right; x++) {
                uint8_t *q = out + x * 4;
                q[0] = avgQ64(sb, invDiameterQ64);
                q[1] = avgQ64(sg, invDiameterQ64);
                q[2] = avgQ64(sr, invDiameterQ64);
                q[3] = avgQ64(sa, invDiameterQ64);

                const uint8_t *p_add = row + (x + radius + 1) * 4;
                const uint8_t *p_sub = row + (x - radius)     * 4;
                sb += p_add[0] - p_sub[0];
                sg += p_add[1] - p_sub[1];
                sr += p_add[2] - p_sub[2];
                sa += p_add[3] - p_sub[3];
            }

            const uint8_t *row_last = row + (w - 1) * 4;
            for (int x = x_start_right; x < w; x++) {
                uint8_t *q = out + x * 4;
                q[0] = avgQ64(sb, invDiameterQ64);
                q[1] = avgQ64(sg, invDiameterQ64);
                q[2] = avgQ64(sr, invDiameterQ64);
                q[3] = avgQ64(sa, invDiameterQ64);

                int x_sub = x - radius;
                if (x_sub < 0) x_sub = 0;
                const uint8_t *p_add = row_last;
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
            int y_end_top    = radius < h ? radius : h;
            int y_start_bot  = h - radius - 1;
            if (y_start_bot < 0) y_start_bot = 0;

            for (int x = 0; x < w; x++)
            {
                uint8_t *col = nxt + x * 4;
                uint32_t sb = 0, sg = 0, sr = 0, sa = 0;

                for (int i = -radius; i <= radius; i++) {
                    int yi = i < 0 ? 0 : i;
                    const uint8_t *p = col + (size_t)yi * stride;
                    sb += p[0]; sg += p[1]; sr += p[2]; sa += p[3];
                }

                for (int y = 0; y < y_end_top; y++) {
                    uint8_t *q = col + (size_t)y * stride;
                    q[0] = avgQ64(sb, invDiameterQ64);
                    q[1] = avgQ64(sg, invDiameterQ64);
                    q[2] = avgQ64(sr, invDiameterQ64);
                    q[3] = avgQ64(sa, invDiameterQ64);

                    int y_add = y + radius + 1;
                    if (y_add >= h) y_add = h - 1;
                    const uint8_t *p_add = col + (size_t)y_add * stride;
                    const uint8_t *p_sub = col;
                    sb += p_add[0] - p_sub[0];
                    sg += p_add[1] - p_sub[1];
                    sr += p_add[2] - p_sub[2];
                    sa += p_add[3] - p_sub[3];
                }

                for (int y = y_end_top; y < y_start_bot; y++) {
                    uint8_t *q = col + (size_t)y * stride;
                    q[0] = avgQ64(sb, invDiameterQ64);
                    q[1] = avgQ64(sg, invDiameterQ64);
                    q[2] = avgQ64(sr, invDiameterQ64);
                    q[3] = avgQ64(sa, invDiameterQ64);

                    const uint8_t *p_add = col + (size_t)(y + radius + 1) * stride;
                    const uint8_t *p_sub = col + (size_t)(y - radius)     * stride;
                    sb += p_add[0] - p_sub[0];
                    sg += p_add[1] - p_sub[1];
                    sr += p_add[2] - p_sub[2];
                    sa += p_add[3] - p_sub[3];
                }

                const uint8_t *col_last = col + (size_t)(h - 1) * stride;
                for (int y = y_start_bot; y < h; y++) {
                    uint8_t *q = col + (size_t)y * stride;
                    q[0] = avgQ64(sb, invDiameterQ64);
                    q[1] = avgQ64(sg, invDiameterQ64);
                    q[2] = avgQ64(sr, invDiameterQ64);
                    q[3] = avgQ64(sa, invDiameterQ64);

                    int y_sub = y - radius;
                    if (y_sub < 0) y_sub = 0;
                    const uint8_t *p_add = col_last;
                    const uint8_t *p_sub = col + (size_t)y_sub * stride;
                    sb += p_add[0] - p_sub[0];
                    sg += p_add[1] - p_sub[1];
                    sr += p_add[2] - p_sub[2];
                    sa += p_add[3] - p_sub[3];
                }
            }
        }

        if (pass < passes - 1) {
            cur = nxt;
            nxt = (nxt == dst) ? tmp : dst;
        }
    }

    free(tmp);
}