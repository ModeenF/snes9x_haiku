#ifndef __S9X_BE_DEFS_H__
#define __S9X_BE_DEFS_H__

void Init_2xSaI (uint32);
void Super2xSaI (uint8 *srcPtr, uint32 srcPitch,
		 uint8 *deltaPtr, uint8 *dstPtr, uint32 dstPitch,
		 int width, int height);
void SuperEagle (uint8 *srcPtr, uint32 srcPitch, uint8 *deltaPtr, 
		 uint8 *dstPtr, uint32 dstPitch, int width, int height);
void _2xSaI (uint8 *srcPtr, uint32 srcPitch, uint8 *deltaPtr, 
	     uint8 *dstPtr, uint32 dstPitch, int width, int height);
void Scale_2xSaI (uint8 *srcPtr, uint32 srcPitch, uint8 * /* deltaPtr */,
		  uint8 *dstPtr, uint32 dstPitch, 
		  uint32 dstWidth, uint32 dstHeight, int width, int height);
extern "C" {
	void normal_blit(uint8 *src, uint8 *dst, int w);
	void double_blit(uint8 *src, uint8 *dst, int w);
	void lcd_blit_standard(uint8 *src, uint8 *dst, int w);
	void lcd_blit_extend(uint8 *src, uint8 *dst, int w);
};

#endif
