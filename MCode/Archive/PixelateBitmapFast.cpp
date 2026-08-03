int __stdcall PixelateBitmap(unsigned char *sBitmap, unsigned char *dBitmap, int w, int h, int Stride, int Size)
{
   if (!sBitmap || !dBitmap || w <= 0 || h <= 0 || Size <= 0)
      return -1;

   // Hoist constants — no recomputation inside loops
   const int blocksCols = w / Size;
   const int blocksRows = h / Size;
   const int remX = w % Size;
   const int remY = h % Size;
   const int blockArea = Size * Size;

   // Process every block including partial edge blocks in one unified loop
   int totalBlockCols = blocksCols + (remX > 0 ? 1 : 0);
   int totalBlockRows = blocksRows + (remY > 0 ? 1 : 0);

   for (int by = 0; by < totalBlockRows; by++)
   {
      // Actual pixel height of this block row (last row may be partial)
      int bh = (by < blocksRows) ? Size : remY;

      for (int bx = 0; bx < totalBlockCols; bx++)
      {
         // Actual pixel width of this block (last column may be partial)
         int bw = (bx < blocksCols) ? Size : remX;
         int area = bw * bh;

         int sA = 0, sR = 0, sG = 0, sB = 0;

         // Top-left pixel of this block
         int originX = bx * Size;
         int originY = by * Size;

         // Sum block pixels
         for (int y = 0; y < bh; y++)
         {
            const unsigned char *src = sBitmap + (originY + y) * Stride + originX * 4;
            for (int x = 0; x < bw; x++, src += 4)
            {
               sB += src[0];
               sG += src[1];
               sR += src[2];
               sA += src[3];
            }
         }

         // Average
         unsigned char aB = (unsigned char)(sB / area);
         unsigned char aG = (unsigned char)(sG / area);
         unsigned char aR = (unsigned char)(sR / area);
         unsigned char aA = (unsigned char)(sA / area);

         // Write averaged color to destination block
         for (int y = 0; y < bh; y++)
         {
            unsigned char *dst = dBitmap + (originY + y) * Stride + originX * 4;
            for (int x = 0; x < bw; x++, dst += 4)
            {
               dst[0] = aB;
               dst[1] = aG;
               dst[2] = aR;
               dst[3] = aA;
            }
         }
      }
   }
   return 0;
}