#include <windows.h>
int FindAlphaBounds(HBITMAP hBitmap, int *x1, int *x2, int *y1, int *y2)
{
    DIBSECTION ds;
    if (!GetObject(hBitmap, sizeof(ds), &ds))
        return 0;

    int width = ds.dsBmih.biWidth;
    int height = ds.dsBmih.biHeight;
    int absH = height < 0 ? -height : height;
    int topDown = height < 0;
    int stride = ds.dsBm.bmWidthBytes < 0 ? -ds.dsBm.bmWidthBytes : ds.dsBm.bmWidthBytes;

    BYTE *bits = (BYTE *)ds.dsBm.bmBits;
    if (!bits || ds.dsBmih.biBitCount != 32)
        return 0;

#define Pixel(x, y) (*(DWORD *)(bits + (topDown ? (y) : (absH - 1 - (y))) * stride + (x) * 4))

    int col, row;
    int found;
    int cx1 = -1, cx2 = -1, cy1 = -1, cy2 = -1;

    // ── x1: walk right from left edge until a column with data is found ──
    for (col = 0; col < width; col++)
    {
        found = 0;
        for (row = 0; row < absH; row++)
        {
            if (Pixel(col, row))
            {
                found = 1;
                break;
            }
        }
        if (found)
        {
            cx1 = col;
            break;
        }
    }

    // no data found at all
    if (cx1 == -1)
    {
        return 0;
    }

    // ── x2: walk left from right edge until a column with data is found ──
    for (col = width - 1; col >= cx1; col--)
    {
        found = 0;
        for (row = 0; row < absH; row++)
        {
            if (Pixel(col, row))
            {
                found = 1;
                break;
            }
        }
        if (found)
        {
            cx2 = col;
            break;
        }
    }

    // ── y1: walk down from top edge until a row with data is found ───────
    for (row = 0; row < absH; row++)
    {
        found = 0;
        for (col = cx1; col <= cx2; col++)
        {
            if (Pixel(col, row))
            {
                found = 1;
                break;
            }
        }
        if (found)
        {
            cy1 = row;
            break;
        }
    }

    // ── y2: walk up from bottom edge until a row with data is found ──────
    for (row = absH - 1; row >= cy1; row--)
    {
        found = 0;
        for (col = cx1; col <= cx2; col++)
        {
            if (Pixel(col, row))
            {
                found = 1;
                break;
            }
        }
        if (found)
        {
            cy2 = row;
            break;
        }
    }

    *x1 = cx1;
    *x2 = cx2;
    *y1 = cy1;
    *y2 = cy2;

#undef Pixel
    return 1;
}