#include <stdint.h>

/*
    LuminanceContrastStretch()

    Performs a robust luminance-based contrast stretch on a
    32-bit Windows ARGB DIBSection.

    ========================================================================
    PIXEL FORMAT
    ========================================================================

    Logical pixel value:

        0xAARRGGBB

    Actual byte order in a normal Windows 32-bit DIBSection:

        p[0] = Blue
        p[1] = Green
        p[2] = Red
        p[3] = Alpha

    Therefore, for example:

        0x80FF8040

    is stored in memory as:

        40 80 FF 80
        B  G  R  A

    Alpha is never modified.

    ========================================================================
    BITMAP ORIENTATION
    ========================================================================

    The function does not depend on whether the DIB is bottom-up
    or top-down.

    'stride' is the number of bytes between successive rows.

    For a normal bottom-up DIB, stride is normally positive when
    'pixels' points to the first row being processed.

    A negative stride is also supported.

    ========================================================================
    PARAMETERS
    ========================================================================

        pixels
            Pointer to the first BGRA pixel.

        width
            Width of bitmap in pixels.

        height
            Height of bitmap in pixels.

        stride
            Number of bytes between rows.

        lowTrim
            Percentage of pixels to ignore at the dark end.

        highTrim
            Percentage of pixels to ignore at the bright end.

    Example:

        lowTrim  = 0.5
        highTrim = 0.5

    means approximately the darkest 0.5% and brightest 0.5% of
    luminance values are excluded when determining the useful
    contrast range.

    ========================================================================
    ALGORITHM
    ========================================================================

    1. Build a 256-bin Rec.709 luminance histogram.

    2. Find the low and high luminance endpoints after trimming
       the specified percentages from each end.

    3. Linearly stretch that useful luminance range to 0..255.

    4. Apply the luminance change while preserving as much of the
       original color/chroma as possible.

    5. If preserving the original chroma would produce an RGB
       component outside 0..255, reduce the chroma uniformly until
       the color fits in the RGB gamut.

    6. Alpha is left unchanged.

    ========================================================================
    RETURN VALUE
    ========================================================================

        0 = no RGB values were changed
        1 = at least one RGB value was changed

    ========================================================================
    RECOMMENDED TRIM VALUES
    ========================================================================

        0.25, 0.25    conservative
        0.50, 0.50    recommended
        1.00, 1.00    stronger
        2.00, 2.00    aggressive

    The two values can be different if desired.

    ========================================================================
    STACK USAGE
    ========================================================================

    The histogram is only 256 uint32_t values (1024 bytes).

    There is intentionally no second 256-element floating-point
    array. This keeps the stack allocation small and avoids the
    MinGW __chkstk_ms / ___chkstk_ms dependency that can occur
    with large stack frames.

    No malloc(), free(), or other C runtime allocation is used.
*/

__stdcall int LuminanceContrastStretch(
    uint8_t *pixels,
    int width,
    int height,
    int stride,
    float lowTrim,
    float highTrim)
{
   /*
       --------------------------------------------------------------------
       Validate parameters.
       --------------------------------------------------------------------
   */

   if (!pixels || width <= 0 || height <= 0)
      return 0;

   /*
       --------------------------------------------------------------------
       Clamp trim percentages to sensible ranges.

       The two trimmed portions must leave some portion of the
       histogram available for stretching.
       --------------------------------------------------------------------
   */

   if (lowTrim < 0.0f)
      lowTrim = 0.0f;
   else if (lowTrim > 99.0f)
      lowTrim = 99.0f;

   if (highTrim < 0.0f)
      highTrim = 0.0f;
   else if (highTrim > 99.0f)
      highTrim = 99.0f;

   /*
       If the requested trims consume essentially the entire
       histogram, reduce them proportionally so that 1% remains
       available as the usable range.

       This is only a safety measure. Normal values such as
       0.5 / 0.5 never come anywhere near this condition.
   */

   if (lowTrim + highTrim >= 100.0f)
   {
      float factor =
          99.0f / (lowTrim + highTrim);

      lowTrim *= factor;
      highTrim *= factor;
   }

   /*
       --------------------------------------------------------------------
       Calculate total number of pixels.

       uint64_t prevents multiplication overflow for large bitmaps.
       --------------------------------------------------------------------
   */

   uint64_t total =
       (uint64_t)width * (uint64_t)height;

   /*
       --------------------------------------------------------------------
       PASS 1
       Build a 256-bin Rec.709 luminance histogram.

       uint32_t is sufficient for normal Windows bitmaps. A bitmap
       would need more than 4.29 billion pixels before a histogram
       bin could overflow.
       --------------------------------------------------------------------
   */

   uint32_t hist[256] = {0};

   uint8_t *row = pixels;

   for (int y = 0; y < height; y++)
   {
      uint8_t *p = row;

      for (int x = 0; x < width; x++)
      {
         /*
             Windows 32-bit ARGB DIB memory:

                 p[0] = B
                 p[1] = G
                 p[2] = R
                 p[3] = A
         */

         int B = p[0];
         int G = p[1];
         int R = p[2];

         /*
             Rec.709 luminance:

                 Y = 0.2126 R
                   + 0.7152 G
                   + 0.0722 B

             Integer arithmetic is used for the histogram.
         */

         int Y =
             (2126 * R +
              7152 * G +
              722 * B +
              5000) /
             10000;

         /*
             Protect against any unexpected arithmetic result.
         */

         if (Y < 0)
            Y = 0;
         else if (Y > 255)
            Y = 255;

         hist[Y]++;

         p += 4;
      }

      row += stride;
   }

   /*
       --------------------------------------------------------------------
       Determine how many pixels are in each trimmed tail.
       --------------------------------------------------------------------
   */

   double lowCount =
       (double)total * (double)lowTrim / 100.0;

   double highCount =
       (double)total * (double)highTrim / 100.0;

   /*
       --------------------------------------------------------------------
       Find LOW luminance endpoint.

       Starting at zero, accumulate histogram bins until the number
       of pixels exceeds the requested dark-tail population.

       The resulting lowY is the luminance value used as the
       black endpoint of the stretch.
       --------------------------------------------------------------------
   */

   uint64_t cumulative = 0;

   int lowY = 0;

   for (int i = 0; i < 256; i++)
   {
      cumulative += hist[i];

      if ((double)cumulative > lowCount)
      {
         lowY = i;
         break;
      }
   }

   /*
       --------------------------------------------------------------------
       Find HIGH luminance endpoint.

       Starting at 255, accumulate histogram bins until the number
       of pixels exceeds the requested bright-tail population.

       The resulting highY is the luminance value used as the
       white endpoint of the stretch.
       --------------------------------------------------------------------
   */

   cumulative = 0;

   int highY = 255;

   for (int i = 255; i >= 0; i--)
   {
      cumulative += hist[i];

      if ((double)cumulative > highCount)
      {
         highY = i;
         break;
      }
   }

   /*
       --------------------------------------------------------------------
       Already full useful dynamic range.

       If the selected low and high endpoints are already 0 and
       255, there is no further global contrast stretch to perform.

       This also makes the operation effectively idempotent:
       after a successful stretch, a second call with the same
       trim values will normally reach this condition.
       --------------------------------------------------------------------
   */

   if (lowY == 0 && highY == 255)
      return 0;

   /*
       --------------------------------------------------------------------
       No useful luminance range.

       If both endpoints are the same, there is nothing meaningful
       to stretch.
       --------------------------------------------------------------------
   */

   if (highY <= lowY)
      return 0;

   /*
       --------------------------------------------------------------------
       Calculate the linear luminance stretch.

           lowY  -> 0
           highY -> 255

       For a source luminance Y:

           newY = (Y - lowY) * scale

       Values below lowY become 0.
       Values above highY become 255.
       --------------------------------------------------------------------
   */

   double scale =
       255.0 / (double)(highY - lowY);

   /*
       --------------------------------------------------------------------
       PASS 2
       Apply the contrast stretch.
       --------------------------------------------------------------------
   */

   int changed = 0;

   row = pixels;

   for (int y = 0; y < height; y++)
   {
      uint8_t *p = row;

      for (int x = 0; x < width; x++)
      {
         /*
             Read source RGB.

             Alpha is deliberately not changed.
         */

         double B = p[0];
         double G = p[1];
         double R = p[2];

         /*
             ----------------------------------------------------------------
             Calculate actual Rec.709 luminance.
             ----------------------------------------------------------------
         */

         double Y =
             0.0722 * B +
             0.7152 * G +
             0.2126 * R;

         /*
             ----------------------------------------------------------------
             Calculate stretched luminance.

             Clamp to the output range.
             ----------------------------------------------------------------
         */

         double newY;

         if (Y <= (double)lowY)
         {
            newY = 0.0;
         }
         else if (Y >= (double)highY)
         {
            newY = 255.0;
         }
         else
         {
            newY =
                (Y - (double)lowY) * scale;
         }

         /*
             ----------------------------------------------------------------
             Separate luminance from chroma.

             RGB can be represented as:

                 R = Y + dR
                 G = Y + dG
                 B = Y + dB

             where:

                 dR = R - Y
                 dG = G - Y
                 dB = B - Y

             We initially preserve all of the original chroma.
             ----------------------------------------------------------------
         */

         double dR = R - Y;
         double dG = G - Y;
         double dB = B - Y;

         double chromaScale = 1.0;

         /*
             ----------------------------------------------------------------
             RED gamut constraint.

             We require:

                 0 <= newY + dR*k <= 255

             where k is the chroma scale factor.
             ----------------------------------------------------------------
         */

         if (dR > 0.0)
         {
            double limit =
                (255.0 - newY) / dR;

            if (limit < chromaScale)
               chromaScale = limit;
         }
         else if (dR < 0.0)
         {
            double limit =
                -newY / dR;

            if (limit < chromaScale)
               chromaScale = limit;
         }

         /*
             ----------------------------------------------------------------
             GREEN gamut constraint.
             ----------------------------------------------------------------
         */

         if (dG > 0.0)
         {
            double limit =
                (255.0 - newY) / dG;

            if (limit < chromaScale)
               chromaScale = limit;
         }
         else if (dG < 0.0)
         {
            double limit =
                -newY / dG;

            if (limit < chromaScale)
               chromaScale = limit;
         }

         /*
             ----------------------------------------------------------------
             BLUE gamut constraint.
             ----------------------------------------------------------------
         */

         if (dB > 0.0)
         {
            double limit =
                (255.0 - newY) / dB;

            if (limit < chromaScale)
               chromaScale = limit;
         }
         else if (dB < 0.0)
         {
            double limit =
                -newY / dB;

            if (limit < chromaScale)
               chromaScale = limit;
         }

         /*
             ----------------------------------------------------------------
             Numerical protection.

             Mathematically chromaScale should be between 0 and 1.
             ----------------------------------------------------------------
         */

         if (chromaScale < 0.0)
            chromaScale = 0.0;
         else if (chromaScale > 1.0)
            chromaScale = 1.0;

         /*
             ----------------------------------------------------------------
             Reconstruct RGB.

             The desired luminance changes while the original
             chroma is retained as much as the RGB gamut allows.
             ----------------------------------------------------------------
         */

         double newR =
             newY + dR * chromaScale;

         double newG =
             newY + dG * chromaScale;

         double newB =
             newY + dB * chromaScale;

         /*
             ----------------------------------------------------------------
             Final protection against floating-point roundoff.
             ----------------------------------------------------------------
         */

         if (newR < 0.0)
            newR = 0.0;
         else if (newR > 255.0)
            newR = 255.0;

         if (newG < 0.0)
            newG = 0.0;
         else if (newG > 255.0)
            newG = 255.0;

         if (newB < 0.0)
            newB = 0.0;
         else if (newB > 255.0)
            newB = 255.0;

         /*
             ----------------------------------------------------------------
             Convert to 8-bit.

             Alpha p[3] is intentionally left untouched.
             ----------------------------------------------------------------
         */

         uint8_t outB =
             (uint8_t)(newB + 0.5);

         uint8_t outG =
             (uint8_t)(newG + 0.5);

         uint8_t outR =
             (uint8_t)(newR + 0.5);

         /*
             ----------------------------------------------------------------
             Only modify memory when an RGB byte actually changes.

             This makes the return value precise:

                 0 = no RGB bytes changed
                 1 = at least one RGB byte changed
             ----------------------------------------------------------------
         */

         if (outB != p[0] ||
             outG != p[1] ||
             outR != p[2])
         {
            p[0] = outB;
            p[1] = outG;
            p[2] = outR;

            changed = 1;
         }

         /*
             Move to next pixel.
         */

         p += 4;
      }

      /*
          Move to next row.

          'stride' may be positive or negative.
      */

      row += stride;
   }

   /*
       --------------------------------------------------------------------
       Return whether anything actually changed.
       --------------------------------------------------------------------
   */

   return changed;
}