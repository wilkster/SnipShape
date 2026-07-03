#include <stdint.h>
#include <stdlib.h>
#include <string.h>
__stdcall int ReplaceColor(uint8_t *pixels, int width, int height, int stride,
                            uint32_t targetARGB, uint32_t replaceARGB)
{
    int replaced = 0;
    if (width <= 0 || height <= 0)
        return replaced;

    uint32_t ta = (targetARGB >> 24) & 0xFF;
    uint32_t tr = (targetARGB >> 16) & 0xFF;
    uint32_t tg = (targetARGB >> 8) & 0xFF;
    uint32_t tb = targetARGB & 0xFF;

    uint32_t ra = (replaceARGB >> 24) & 0xFF;
    uint32_t rr = (replaceARGB >> 16) & 0xFF;
    uint32_t rg = (replaceARGB >> 8) & 0xFF;
    uint32_t rb = replaceARGB & 0xFF;
    uint8_t *row = pixels;
    for (int y = 0; y < height; y++)
    {
        uint8_t *p = row;
        for (int x = 0; x < width; x++)
        {
            // Ignore alpha channel
            if (p[0] == tb && p[1] == tg && p[2] == tr)
            {
                p[0] = (uint8_t)rb;
                p[1] = (uint8_t)rg;
                p[2] = (uint8_t)rr;
                p[3] = (uint8_t)ta; // Note, Make sure replacement has a non-zero alpha channel
                replaced++;
            }
            p += 4;
        }
        row += stride;
    }
    return replaced;
}