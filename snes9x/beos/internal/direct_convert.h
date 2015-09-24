#ifndef __DIRECT_CONVERT_H__
#define __DIRECT_CONVERT_H__

#include <Bitmap.h>

void conv16(BBitmap &src, BBitmap &dst);

void init_palette();
void draw_line16to8(uint8 *src, uint8 *dst, int w);
void draw_line16to16(uint8 *src, uint8 *dst, int w);
void draw_line16to32(uint8 *src, uint8 *dst, int w);

void draw_line16to8double(uint8 *src, uint8 *dst, int w, int type);
void draw_line16to16double(uint8 *src, uint8 *dst, int w, int type);
void draw_line16to32double(uint8 *src, uint8 *dst, int w, int type);

void draw_line16to8lcd(uint8 *src, uint8 *dst, int w, int type);
void draw_line16to16lcd(uint8 *src, uint8 *dst, int w, int type);
void draw_line16to32lcd(uint8 *src, uint8 *dst, int w, int type);

void clear_ycbr(uint8 *data, int h, int bytes_per_row);
void draw_16_to_ycbr_multi(uint8 *src, uint8 *dst, int sw, int sh, int dw, int dh, int sr, int dr);
void draw_16_to_16_multi(uint8 *src, uint8 *dst, int sy, int dy, int sw, int sh, int dw, int dh, int sbytes_per_row, int bytes_per_row);


#endif
