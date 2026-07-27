#include <windows.h>
int __stdcall FindExtentInward(HBITMAP hBitmap, int *x1, int *x2, int *y1, int *y2, int topDown)
{
   DIBSECTION ds;
   if (!GetObject(hBitmap, sizeof(ds), &ds))
      return 0;
   int width = ds.dsBmih.biWidth;
   int height = ds.dsBmih.biHeight;
   int absH = height < 0 ? -height : height;
   int stride = ds.dsBm.bmWidthBytes;
   BYTE *bits = (BYTE *)ds.dsBm.bmBits;
   if (!bits || ds.dsBmih.biBitCount != 32)
      return 0;
#define Pixel(x, y) (*(DWORD *)(bits + (topDown ? (y) : (absH - 1 - (y))) * stride + (x) * 4)) & 0xFF000000
   int cx1 = *x1 < 0 ? 0 : (*x1 >= width ? width - 1 : *x1);
   int cx2 = *x2 >= width ? width - 1 : (*x2 < 0 ? 0 : *x2);
   int cy1 = *y1 < 0 ? 0 : (*y1 >= absH ? absH - 1 : *y1);
   int cy2 = *y2 >= absH ? absH - 1 : (*y2 < 0 ? 0 : *y2);
   int col, row, found;
   int prevCx1, prevCx2, prevCy1, prevCy2;
   int passes = 0;
   // Multi-pass in case shrinking one axis exposes that the other axis
   // could shrink further too (mirror of the outward version's corner case)
   do
   {
      passes++;
      prevCx1 = cx1;
      prevCx2 = cx2;
      prevCy1 = cy1;
      prevCy2 = cy2;
      // ── x1: walk right (shrink from the left) ───────────────────────────
      col = cx1;
      while (col < cx2)
      {
         found = 0;
         for (row = cy1; row <= cy2; row++)
         {
            if (Pixel(col, row))
            {
               found = 1;
               break;
            }
         }
         if (found)
            break;
         col++;
      }
      cx1 = col;
      // ── x2: walk left (shrink from the right) ───────────────────────────
      col = cx2;
      while (col > cx1)
      {
         found = 0;
         for (row = cy1; row <= cy2; row++)
         {
            if (Pixel(col, row))
            {
               found = 1;
               break;
            }
         }
         if (found)
            break;
         col--;
      }
      cx2 = col;
      // ── y1: walk down (shrink from the top) ──────────────────────────────
      row = cy1;
      while (row < cy2)
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
            break;
         row++;
      }
      cy1 = row;
      // ── y2: walk up (shrink from the bottom) ─────────────────────────────
      row = cy2;
      while (row > cy1)
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
            break;
         row--;
      }
      cy2 = row;
   } while (cx1 != prevCx1 || cx2 != prevCx2 || cy1 != prevCy1 || cy2 != prevCy2);
   // If we collapsed to a single pixel and even that pixel is transparent,
   // there was no opaque content in the search region at all.
   if (cx1 == cx2 && cy1 == cy2 && !Pixel(cx1, cy1))
   {
      *x1 = 0;
      *x2 = -1;
      *y1 = 0;
      *y2 = -1;
      return 0;
   }
   *x1 = cx1;
   *x2 = cx2;
   *y1 = cy1;
   *y2 = cy2;
#undef Pixel
   return passes;
}

/*
x64 := "
(
QVdBVkFVQVRVV1ZTSIHsmAAAAEiJlCToAAAAi5wkCAEAALpoAAAATImEJPAAAABMjUQkIEyJjCT4AAAA/xUAAAAAQYnHhcB0FUiL
bCQ4RTH/SIXtdAhmg3wkTiB0F0iBxJgAAABEifhbXl9dQVxBXUFeQV/DSIu8JOgAAACLRCRERIsfRYXbD4j+AQAASIu8JPAAAACN
UP9BOcNED03aRIsvQTnFD4z3AQAARI1o/4tEJEhEi0wkSMH4H0ExwUEpwUiLhCT4AAAARIsgRYXkD4imAgAAQY1B/0U5zEQPTeBI
i4QkAAEAAESLAEU5yA+MdgIAAEWNQf9FMf9Ei1QkLEGD6QFEid5Bg8cBRTnrD41gAQAADx9AAEU5xA+P7wEAAI0EtQAAAABEieJI
mEiNTAUA6xAPH0AAg8IBQTnQD4zMAQAARInIKdCF2w9FwkEPr8JImPcEAQAAAP9020E580EPlMZBOfUPjhYCAABCjTytAAAAAESJ
6Uhj/0SJ4kyNXD0A6w8PHwCDwgFBOdAPjFwBAABEicgp0IXbD0XCQQ+vwkiYQfcEAwAAAP902kE5zQ+UwEEhxkU5xA+NagEAAEWJ
40xj7mYuDx+EAAAAAAA58Q+M4AAAAESJykyJ6EQp2oXbQQ9F00EPr9JIY9JIAerrEQ8fRAAASIPAATnBD4y0AAAA9wSCAAAA/3Tr
RInHRTnYfjBEicpMiegp+oXbD0XXQQ+v0khj0kgB6usOZpBIg8ABOcEPjJQAAAD3BIIAAAD/dOtFhPZ0DkU53HUJQTn4D4QKAQAA
QYn4RYncQYnzQYnNQYPHAUSJ3kU56w+MpP7//0SJ6UU5xA+NswAAAEG+AQAAAOk1////SIu8JPAAAABFMdtEiy9BOcUPjQn+//9F
he24AAAAAEQPSOjp/P3//w8fRAAAQYPDAUU52A+FC////0WJxOuUZg8fRAAAg+8BRDnfD4U5////QYn4QYn86Xf///9mDx+EAAAA
AACD6QFIg+8EOfEPhXz+//9FOcQPjVb///9FMfbpsP7//4PGAUE59Q+F/P3//4nxRTH2RTnED4yW/v//RYT2D4Qr////SIuEJOgA
AACJMEiLhCTwAAAAiQhIi4Qk+AAAAESJIEiLhCQAAQAARIkA6fz8//9BifhFidzryUWFwLgAAAAARA9IwOl9/f//RTHk6V39//9E
ienpLP7//5CQkJCQ|Gdi32:GetObjectA:62:4
)"
*/