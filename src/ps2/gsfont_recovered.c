/*
 * Recovered Hiryu GSLIB gsFont embedded in SNES Station v0.23.
 * Target corridor: 0x0019b7f0..0x0019bd34.
 *
 * The target object layout, 256-byte kerning table, BFNT pixel offset and
 * gsPipe call graph independently identify this module before historical
 * GSLIB source is used to validate names. Behavioural quirks are preserved.
 */
#include <stdint.h>
#include <string.h>

#include "../../include/gslib_recovered.h"

#define GSFONT_ALIGN_LEFT   1
#define GSFONT_ALIGN_CENTRE 2
#define GSFONT_ALIGN_RIGHT  3

static gsPipeRecovered *font_pipe(const gsFontRecovered *self)
{
    return (gsPipeRecovered *)(uintptr_t)self->m_pFontPipe;
}

/* 0x001b0790 -- older GSLIB power-of-two texture-size helper used by Print. */
uint32_t gsDriver_getTexSizeFromInt_001b0790(int texsize)
{
    int power = 0x400;
    int index;

    if (texsize == 0)
        return 0;

    for (index = 10; index >= 0; --index) {
        if (texsize == power || texsize > (power >> 1))
            return (uint32_t)index;
        power >>= 1;
    }
    return 0;
}

/* 0x0019b7f0 */
void gsFont_uploadFont_0019b7f0(gsFontRecovered *self,
    const gsFontTexRecovered *font, uint32_t tbbase, int tbwidth,
    int tbxpos, int tbypos)
{
    gsPipeRecovered *pipe = font_pipe(self);
    if (pipe == NULL || font == NULL)
        return;

    self->m_TBbase = tbbase;
    self->m_TBwidth = tbwidth;
    self->m_TBxpos = tbxpos;
    self->m_TBypos = tbypos;
    self->m_TexWidth = font->TexWidth;
    self->m_TexHeight = font->TexHeight;
    self->m_CharGridWidth = font->CharGridWidth;
    self->m_CharGridHeight = font->CharGridHeight;
    self->m_NumXChars = font->NumXChars;
    self->m_NumYChars = font->NumYChars;
    self->m_PSM = font->PSM;
    memcpy(self->m_CharWidth, font->CharWidth, sizeof(self->m_CharWidth));

    gsPipe_TextureUpload_00199f88(pipe, self->m_TBbase, self->m_TBwidth,
        self->m_TBxpos, self->m_TBypos, (int)self->m_PSM,
        font->PixelData, (int)self->m_TexWidth, (int)self->m_TexHeight);
}

/* 0x0019bad0 */
void gsFont_GetCurrLineLength_0019bad0(const gsFontRecovered *self,
    const char *text, int max_length, int *pix_length, int *char_length)
{
    *pix_length = 0;
    *char_length = 0;

    for (;;) {
        const int8_t ch = (int8_t)*text;
        switch ((uint8_t)ch) {
        case 0:
            return;
        case '\n':
            ++*char_length;
            return;
        case '\r':
        case '\b':
        case '\a':
            break;
        default: {
            /* Target uses signed-char indexing semantics inherited from this
               early GSLIB revision; ordinary font text is 0..255. */
            const int width = self->m_CharWidth[(uint8_t)ch];
            if (*pix_length + width > max_length)
                return;
            *pix_length += width;
            break;
        }
        }
        ++*char_length;
        ++text;
    }
}

/* 0x0019bb68 */
void gsFont_PrintLine_0019bb68(gsFontRecovered *self, int x, int y, int z,
    uint64_t colour, int length, const char *text)
{
    gsPipeRecovered *pipe = font_pipe(self);
    uint32_t col = (uint32_t)colour;
    int i;

    for (i = 0; i < length; ++i) {
        const uint8_t ch = (uint8_t)text[i];
        switch (ch) {
        case '\n':
        case '\r':
        case '\t':
            break;
        case '\b':
            self->m_Bold ^= 1u;
            col = self->m_Bold ? ((uint32_t)colour | 0xff000000u)
                               : (uint32_t)colour;
            break;
        case '\a':
            self->m_Underline ^= 1u;
            break;
        default: {
            const uint32_t row = ch / self->m_NumXChars;
            const uint32_t column = ch - row * self->m_NumXChars;

            /* Preserve the historical swapped TB offset use visible in the
               target and GSLIB source: Y adds TBxpos; X adds TBypos. */
            const uint32_t char_y = row * self->m_CharGridHeight + (uint32_t)self->m_TBxpos;
            const uint32_t char_x = column * self->m_CharGridWidth + (uint32_t)self->m_TBypos;
            const int width = self->m_CharWidth[ch];

            gsPipe_RectTexture_0019b028(pipe,
                x, y, char_x, char_y,
                x + width - 1, y + (int)self->m_CharGridHeight,
                char_x + (uint32_t)width, char_y + self->m_CharGridHeight,
                (uint32_t)z, col);

            if (self->m_Underline) {
                gsPipe_RectFlat_0019adf8(pipe,
                    x, y + (int)self->m_CharGridHeight,
                    x + width, y + (int)self->m_CharGridHeight + 1,
                    (uint32_t)z, (uint32_t)colour | 0x7f000000u);
            }
            x += width;
            break;
        }
        }
    }
}

/* 0x0019b948 */
void gsFont_Print_0019b948(gsFontRecovered *self, int x, int xend,
    int y, int z, uint64_t colour, int alignment, const char *text)
{
    gsPipeRecovered *pipe = font_pipe(self);
    if (pipe == NULL || text == NULL)
        return;

    const int max_length = xend - x;
    self->m_Bold = 0;
    self->m_Underline = 0;

    gsPipe_TextureSet_0019a588(pipe, self->m_TBbase, self->m_TBwidth,
        gsDriver_getTexSizeFromInt_001b0790((int)self->m_TexWidth),
        gsDriver_getTexSizeFromInt_001b0790((int)self->m_TexHeight),
        self->m_PSM, 0, 0, 0, 0);

    const char *line = text;
    while (*line != '\0') {
        int pix_length = 0;
        int char_length = 0;
        gsFont_GetCurrLineLength_0019bad0(self, line, max_length,
            &pix_length, &char_length);

        int current_x;
        switch (alignment) {
        case GSFONT_ALIGN_RIGHT:
            current_x = xend - pix_length;
            break;
        case GSFONT_ALIGN_CENTRE:
            current_x = x + ((max_length - pix_length) / 2);
            break;
        case GSFONT_ALIGN_LEFT:
        default:
            current_x = x;
            break;
        }

        gsFont_PrintLine_0019bb68(self, current_x, y, z, colour,
            char_length, line);
        y += (int)self->m_CharGridHeight;
        line += char_length;
    }
}
