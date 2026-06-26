#include <windows.h>
int __stdcall FindExtent(HBITMAP hBitmap, int *x1, int *x2, int *y1, int *y2, int topDown)
{
   DIBSECTION ds;
   if (!GetObject(hBitmap, sizeof(ds), &ds))
      return 0;

   int width  = ds.dsBmih.biWidth;
   int height = ds.dsBmih.biHeight;
   int absH   = height < 0 ? -height : height; // Always positive
   int stride = ds.dsBm.bmWidthBytes;

   BYTE *bits = (BYTE *)ds.dsBm.bmBits;
   if (!bits || ds.dsBmih.biBitCount != 32)
      return 0;

// Pixel(x, y) returns the DWORD value at logical (x, y) top-down coordinates
#define Pixel(x, y) (*(DWORD *)(bits + (topDown ? (y) : (absH - 1 - (y))) * stride + (x) * 4))

   int cx1 = *x1 < 0 ? 0 : (*x1 >= width ? width - 1 : *x1);
   int cx2 = *x2 >= width ? width - 1 : (*x2 < 0 ? 0 : *x2);
   int cy1 = *y1 < 0 ? 0 : (*y1 >= absH ? absH - 1 : *y1);
   int cy2 = *y2 >= absH ? absH - 1 : (*y2 < 0 ? 0 : *y2);

   int col, row;
   int found;

   // ── x1: walk left ────────────────────────────────────────────────────
   col = cx1;
   while (col > 0)
   {
      found = 0;
      for (row = cy1; row <= cy2; row++)
      {
         if (Pixel(col - 1, row))
         {
            found = 1;
            break;
         }
      }
      if (!found)
         break;
      col--;
   }
   cx1 = col;

   // ── x2: walk right ───────────────────────────────────────────────────
   col = cx2;
   while (col < width - 1)
   {
      found = 0;
      for (row = cy1; row <= cy2; row++)
      {
         if (Pixel(col + 1, row))
         {
            found = 1;
            break;
         }
      }
      if (!found)
         break;
      col++;
   }
   cx2 = col;

   // ── y1: walk up ──────────────────────────────────────────────────────
   row = cy1;
   while (row > 0)
   {
      found = 0;
      for (col = cx1; col <= cx2; col++)
      {
         if (Pixel(col, row - 1))
         {
            found = 1;
            break;
         }
      }
      if (!found)
         break;
      row--;
   }
   cy1 = row;

   // ── y2: walk down ────────────────────────────────────────────────────
   row = cy2;
   while (row < absH - 1)
   {
      found = 0;
      for (col = cx1; col <= cx2; col++)
      {
         if (Pixel(col, row + 1))
         {
            found = 1;
            break;
         }
      }
      if (!found)
         break;
      row++;
   }
   cy2 = row;

   *x1 = cx1;
   *x2 = cx2;
   *y1 = cy1;
   *y2 = cy2;

#undef Pixel
   return 1;
}