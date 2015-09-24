#include "direct_convert.h"

uint8 y[65536];
uint8 palette8[65536];
uint32 palette_ycbcr[65536];

unsigned long long REDS   __attribute__ ((aligned(8))) = 0x0000f8000000f800ULL;
unsigned long long GREENS __attribute__ ((aligned(8))) = 0x000007e0000007e0ULL;
unsigned long long BLUES  __attribute__ ((aligned(8))) = 0x0000001f0000001fULL;

short Y(uint8 R, uint8 G, uint8 B)
{
	short YY = (short)(float)(0.29900 * R + 0.58700 * G + 0.11400 * B);
	return YY;
}

short Cb(uint8 R, uint8 G, uint8 B)
{
	short CbCb = (short)(float)(-0.16874 * R - 0.33126 * G + 0.50000 * B + 0x80);
	return CbCb;
}

short Cr(uint8 R, uint8 G, uint8 B)
{
	short CrCr = (short)(float)(0.50000 * R - 0.41869 * G - 0.08131 * B + 0x80);
	return CrCr;
}

void init_palette()
{
	const color_map * map = system_colors();
	uint8 y0, cb0, cr0;
	for(int i = 0; i < 65536; i++) {
		unsigned char r, g, b;
		uint16 p16 = i;
		r = (((p16) & 0xf800) >> 11) << 3;
		g = (((p16) & 0x07e0) >> 5) << 2;
		b = (((p16) & 0x001f)) << 3;
		uint16 val = (b>>3)|((g<<2)&0x3e0)|((r<<7)&0x7c00);
		palette8[i] = map->index_map[val];
		y[i] = (uint8)Y(r, g, b);
		y0 = y[i];
		cb0 = (uint8)Cb(r, g, b);
		cr0 = (uint8)Cr(r, g, b);
		palette_ycbcr[i] = cr0 << 24 | 0 << 16 | cb0 << 8 | y0;
	}
}

void draw_line16to8(uint8 *src, uint8 *dst, int w)
{
	uint16 *s = (uint16 *)src;
	while(w--) {
		uint16 p16 = *s++;
		*dst++ = palette8[p16];
	}
}

void draw_line16to8double(uint8 *src, uint8 *dst, int w, int type)
{
	if(type == 4)
	{
		uint32 *s = (uint32 *)src;
		uint16 *d = (uint16 *)dst;
		while(w--)
		{
			uint32 p16 = *s++;
			*d++ = palette8[p16 & 0xffff] | palette8[(p16 >> 16) & 0xffff] << 8;
		}
	}
	else
	{
		uint16 *s = (uint16 *)src;
		uint16 *d = (uint16 *)dst;
		while(w--)
		{
			uint16 p16 = *s++;
			*d++ = palette8[p16] | palette8[p16] << 8;
		}
	}
}

void draw_line16to8lcd(uint8 *src, uint8 *dst, int w, int type)
{
	if(type == 4)
	{
		uint32 *s = (uint32 *)src;
		uint16 *d = (uint16 *)dst;
		while(w--)
		{
			uint32 p16 = *s++;
			*d++ = palette8[p16 & 0xffff];
		}
	}
	else
	{
		uint16 *s = (uint16 *)src;
		uint16 *d = (uint16 *)dst;
		while(w--)
		{
			uint16 p16 = *s++;
			*d++ = palette8[p16];
		}
	}
}

void draw_line16to16(uint8 *src, uint8 *dst, int w)
{
	asm(
	"testb $7, %%cl\n"
	"je 2f\n"
	".p2align 4,,7\n"
	"1:\n"
	"movw (%0), %%ax\n"
	"movw %%ax, (%1)\n"
	"addl $2, %0\n"
	"addl $2, %1\n"
	"decl %2\n"
	"testb $7, %%cl\n"
	"jne 1b\n"
	"2:\n"
	"sarl $2, %2\n"
	"jz 4f\n"
	"3:\n"
	"movq (%0), %%mm0\n"
	"movq %%mm0, (%1)\n"
	"addl $8, %0\n"
	"addl $8, %1\n"
	"decl %2\n"
	"jnz 3b\n"
	"4:\n"
	:
	: "S"(src), "D"(dst), "c"(w));
}

void draw_line16to16double(uint8 *src, uint8 *dst, int w, int type)
{
	if(type == 4)
	{
		asm(
		"testb $7, %%cl\n"
		"je 2f\n"
		".p2align 4,,7\n"
		"1:\n"
		"movl (%0), %%eax\n"
		"movl %%eax, (%1)\n"
		"addl $4, %0\n"
		"addl $4, %1\n"
		"decl %2\n"
		"testb $7, %%cl\n"
		"jne 1b\n"
		"2:\n"
		"sarl $2, %2\n"
		"jz 4f\n"
		".p2align 4,,7\n"
		"3:\n"
		"movq (%0), %%mm0\n"
		"movq 8(%0), %%mm1\n"
		"movq %%mm0, (%1)\n"
		"movq %%mm1, 8(%1)\n"
		"addl $16, %0\n"
		"addl $16, %1\n"
		"decl %2\n"
		"jnz 3b\n"
		".p2align 4,,7\n"
		"4:\n"
		:
		: "S"(src), "D"(dst), "c"(w));
	}
	else
	{
		asm(
		"testb $7, %%cl\n"
		"je 2f\n"
		".p2align 4,,7\n"
		"1:\n"
		"movw (%0), %%ax\n"
		"movw %%ax, (%1)\n"
		"movw %%ax, 2(%1)\n"
		"addl $2, %0\n"
		"addl $4, %1\n"
		"decl %2\n"
		"testb $7, %%cl\n"
		"jne 1b\n"
		"2:\n"
		"sarl $2, %2\n"
		"jz 4f\n"
		".p2align 4,,7\n"
		"3:\n"
		"movq (%0), %%mm0\n"
		"movq %%mm0, %%mm1\n"
		"punpcklwd %%mm0, %%mm0\n"
		"punpckhwd %%mm1, %%mm1\n"
		"movq %%mm0, (%1)\n"
		"movq %%mm1, 8(%1)\n"
		"addl $8, %0\n"
		"addl $16, %1\n"
		"decl %2\n"
		"jnz 3b\n"
		".p2align 4,,7\n"
		"4:\n"
		:
		: "S"(src), "D"(dst), "c"(w));
	}
}

void draw_line16to16lcd(uint8 *src, uint8 *dst, int w, int type)
{
	if(type == 4)
	{
		asm(
		"sarl $2, %2\n"
		"jz 2f\n"
		".p2align 4,,7\n"
		"1:\n"
		"movq (%0), %%mm0\n"
		"movq 8(%0), %%mm1\n"
		"pslld $16, %%mm0\n"
		"psrld $16, %%mm0\n"
		"movq %%mm0, (%1)\n"
		"pslld $16, %%mm1\n"
		"psrld $16, %%mm1\n"
		"movq %%mm1, 8(%1)\n"
		"addl $16, %0\n"
		"addl $16, %1\n"
		"decl %2\n"
		"jnz 1b\n"
		".p2align 4,,7\n"
		"2:\n"
		:
		: "S"(src), "D"(dst), "c"(w));
	}
	else
	{
		asm(
		"sarl $2, %2\n"
		"jz 2f\n"
		".p2align 4,,7\n"
		"1:\n"
		"movq (%0), %%mm0\n"
		"movq %%mm0, %%mm1\n"
		"punpcklwd %%mm0, %%mm0\n"
		"punpckhwd %%mm1, %%mm1\n"
		"pslld $16, %%mm0\n"
		"psrld $16, %%mm0\n"
		"movq %%mm0, (%1)\n"
		"pslld $16, %%mm1\n"
		"psrld $16, %%mm1\n"
		"movq %%mm1, 8(%1)\n"
		"addl $8, %0\n"
		"addl $16, %1\n"
		"decl %2\n"
		"jnz 1b\n"
		".p2align 4,,7\n"
		"2:\n"
		:
		: "S"(src), "D"(dst), "c"(w));
	}
}

void draw_line16to32(uint8 *src, uint8 *dst, int w)
{
	asm (
	"testb $7, %%cl\n"
	"je 2f\n"
	".p2align 4,,7\n"
	"1:\n"
	"movw (%0), %%ax\n"
	"movl %%eax, %%ebx\n"
		
	"andl $0xf800, %%eax\n"
	"shll $8, %%eax\n"
		
	"movl %%ebx, %%edx\n"
	"andl $0x07e0, %%ebx\n"
	"shll $5, %%ebx\n"
	"orl %%ebx, %%eax\n"
		
	"andl $0x001f, %%edx\n"
	"shll $3, %%edx\n"
	"orl %%edx, %%eax\n"
		
	"movl %%eax, (%1)\n"
	"addl $2, %0\n"
	"addl $4, %1\n"
	"decl %2\n"
	"testb $7, %%cl\n"
	"jne 1b\n"
	"2:\n"
	"sarl $1, %2\n"
	"jz 4f\n"
	"3:\n"
	"movd (%0), %%mm0\n"
	"punpcklwd %%mm0, %%mm0\n"
	"movq %%mm0, %%mm1\n"
				 
	"pand REDS, %%mm0\n"
	"pslld $8, %%mm0\n"
				 
	"movq %%mm1, %%mm2\n"
	"pand GREENS, %%mm1\n"
	"pslld $5, %%mm1\n"
	"por %%mm1, %%mm0\n"
				 
	"pand BLUES, %%mm2\n"
	"pslld $3, %%mm2\n"
	"por %%mm2, %%mm0\n"
				 
	"movq %%mm0, (%1)\n"
				 
	"addl $4, %0\n"
	"addl $8, %1\n"
	"decl %%ecx\n"
	"jnz 3b\n"
	"4:\n"
	:
	: "S"(src), "D"(dst), "c"(w)
	: "%ebx");
}

void draw_line16to32double(uint8 *src, uint8 *dst, int w, int type)
{
	if(type == 4)
	{
		asm(
		"testb $7, %%cl
		je 2f
		.p2align 4,,7
		1:
		movw (%0), %%ax
		movl %%eax, %%ebx
		
		andl $0xf800, %%eax
		shll $8, %%eax
		
		movl %%ebx, %%edx
		andl $0x07e0, %%ebx
		shll $5, %%ebx
		orl %%ebx, %%eax
		
		andl $0x001f, %%edx
		shll $3, %%edx
		orl %%edx, %%eax
		
		movl %%eax, (%1)
		
		movw 2(%0), %%ax
		movl %%eax, %%ebx
		
		andl $0xf800, %%eax
		shll $8, %%eax
		
		movl %%ebx, %%edx
		andl $0x07e0, %%ebx
		shll $5, %%ebx
		orl %%ebx, %%eax
		
		andl $0x001f, %%edx
		shll $3, %%edx
		orl %%edx, %%eax
		
		movl %%eax, 4(%1)
		
		addl $4, %0
		addl $8, %1
		decl %2
		testb $7, %%cl
		jne 1b
		2:
		sarl $2, %2
		jz 4f
		.p2align 4,,7
		3:
		movq (%0), %%mm0
		movq 8(%0), %%mm1
		
		movq %%mm0, %%mm2
		punpcklwd %%mm2, %%mm2
		movq %%mm2, %%mm3
		pand REDS, %%mm2
		pslld $8, %%mm2
		
		movq %%mm3, %%mm4
		pand GREENS, %%mm3
		pslld $5, %%mm3
		por %%mm3, %%mm2
		
		pand BLUES, %%mm4
		pslld $3, %%mm4
		por %%mm4, %%mm2
		
		movq %%mm2, (%1)
		
		punpckhdq %%mm0, %%mm0
		movq %%mm0, %%mm2
		punpcklwd %%mm2, %%mm2
		movq %%mm2, %%mm3
		pand REDS, %%mm2
		pslld $8, %%mm2
		
		movq %%mm3, %%mm4
		pand GREENS, %%mm3
		pslld $5, %%mm3
		por %%mm3, %%mm2
		
		pand BLUES, %%mm4
		pslld $3, %%mm4
		por %%mm4, %%mm2
		
		movq %%mm2, 8(%1)
		
		movq %%mm1, %%mm2
		punpcklwd %%mm2, %%mm2
		movq %%mm2, %%mm3
		pand REDS, %%mm2
		pslld $8, %%mm2
		
		movq %%mm3, %%mm4
		pand GREENS, %%mm3
		pslld $5, %%mm3
		por %%mm3, %%mm2
		
		pand BLUES, %%mm4
		pslld $3, %%mm4
		por %%mm4, %%mm2
		
		movq %%mm2, 16(%1)
		
		
		punpckhdq %%mm1, %%mm1
		movq %%mm1, %%mm2
		punpcklwd %%mm2, %%mm2
		movq %%mm2, %%mm3
		pand REDS, %%mm2
		pslld $8, %%mm2
		
		movq %%mm3, %%mm4
		pand GREENS, %%mm3
		pslld $5, %%mm3
		por %%mm3, %%mm2
		
		pand BLUES, %%mm4
		pslld $3, %%mm4
		por %%mm4, %%mm2
		
		movq %%mm2, 24(%1)
		
		addl $16, %0
		addl $32, %1
		decl %2
		jnz 3b
		.p2align 4,,7
		4:"
		:
		: "S"(src), "D"(dst), "c"(w)
		: "%ebx", "%edx");
	}
	else
	{
		asm(
		"testb $7, %%cl
		je 2f
		.p2align 4,,7
		1:
		movw (%0), %%ax
		movl %%eax, %%ebx
		
		andl $0xf800, %%eax
		shl $8, %%eax
		
		movl %%ebx, %%edx
		andl $0x07e0, %%ebx
		shl $5, %%ebx
		orl %%ebx, %%eax
		
		andl $0x001f, %%edx
		shl $3, %%edx
		orl %%edx, %%eax
		
		movl %%eax, (%1)
		movl %%eax, 4(%1)
		addl $2, %0
		addl $8, %1
		decl %2
		testb $7, %%cl
		jne 1b
		2:
		sarl $2, %2
		jz 4f
		.p2align 4,,7
		3:
		movq (%0), %%mm0
		movq %%mm0, %%mm1
		punpcklwd %%mm0, %%mm0
		punpckhwd %%mm1, %%mm1
		
		
		movq %%mm0, %%mm2
		punpcklwd %%mm2, %%mm2
		movq %%mm2, %%mm3
		pand REDS, %%mm2
		pslld $8, %%mm2
		
		movq %%mm3, %%mm4
		pand GREENS, %%mm3
		pslld $5, %%mm3
		por %%mm3, %%mm2
		
		pand BLUES, %%mm4
		pslld $3, %%mm4
		por %%mm4, %%mm2
		
		movq %%mm2, (%1)
		
		punpckhdq %%mm0, %%mm0
		movq %%mm0, %%mm2
		punpcklwd %%mm2, %%mm2
		movq %%mm2, %%mm3
		pand REDS, %%mm2
		pslld $8, %%mm2
		
		movq %%mm3, %%mm4
		pand GREENS, %%mm3
		pslld $5, %%mm3
		por %%mm3, %%mm2
		
		pand BLUES, %%mm4
		pslld $3, %%mm4
		por %%mm4, %%mm2
		
		movq %%mm2, 8(%1)
		
		
		movq %%mm1, %%mm2
		punpcklwd %%mm2, %%mm2
		movq %%mm2, %%mm3
		pand REDS, %%mm2
		pslld $8, %%mm2
		
		movq %%mm3, %%mm4
		pand GREENS, %%mm3
		pslld $5, %%mm3
		por %%mm3, %%mm2
		
		pand BLUES, %%mm4
		pslld $3, %%mm4
		por %%mm4, %%mm2
		
		movq %%mm2, 16(%1)
		
		
		punpckhdq %%mm1, %%mm1
		movq %%mm1, %%mm2
		punpcklwd %%mm2, %%mm2
		movq %%mm2, %%mm3
		pand REDS, %%mm2
		pslld $8, %%mm2
		
		movq %%mm3, %%mm4
		pand GREENS, %%mm3
		pslld $5, %%mm3
		por %%mm3, %%mm2
		
		pand BLUES, %%mm4
		pslld $3, %%mm4
		por %%mm4, %%mm2
		
		movq %%mm2, 24(%1)
		
		addl $8, %0
		addl $32, %1
		decl %2
		jnz 3b
		.p2align 4,,7
		4:"
		:
		: "S"(src), "D"(dst), "c"(w)
		: "%ebx", "%edx");
	}
}


void draw_line16to32lcd(uint8 *src, uint8 *dst, int w, int type)
{
	if(type == 4)
	{
		asm(
		"testb $7, %%cl\n"
		"je 2f\n"
		".p2align 4,,7\n"
		"1:\n"
		"movw (%0), %%ax\n"
		"movl %%eax, %%ebx\n"
		
		"andl $0xf800, %%eax\n"
		"shll $8, %%eax\n"
		
		"movl %%ebx, %%edx\n"
		"andl $0x07e0, %%ebx\n"
		"shll $5, %%ebx\n"
		"orl %%ebx, %%eax\n"
		
		"andl $0x001f, %%edx\n"
		"shll $3, %%edx\n"
		"orl %%edx, %%eax\n"
		
		"movl %%eax, (%1)\n"
		
		"addl $4, %0\n"
		"addl $8, %1\n"
		"decl %2\n"
		"testb $7, %%cl\n"
		"jne 1b\n"
		"2:\n"
		"sarl $2, %2\n"
		"jz 4f\n"
		".p2align 4,,7\n"
		"3:\n"
		"movq (%0), %%mm0\n"
		"movq 8(%0), %%mm1\n"
		
		"movq %%mm0, %%mm2\n"
		"punpcklwd %%mm2, %%mm2\n"
		"movq %%mm2, %%mm3\n"
		"pand REDS, %%mm2\n"
		"pslld $8, %%mm2\n"
		
		"movq %%mm3, %%mm4\n"
		"pand GREENS, %%mm3\n"
		"pslld $5, %%mm3\n"
		"por %%mm3, %%mm2\n"
		
		"pand BLUES, %%mm4\n"
		"pslld $3, %%mm4\n"
		"por %%mm4, %%mm2\n"
		
		"psllq $32, %%mm2\n"
		"psrlq $32, %%mm2\n"
		"movq %%mm2, (%1)\n"
		
		"punpckhdq %%mm0, %%mm0\n"
		"movq %%mm0, %%mm2\n"
		"punpcklwd %%mm2, %%mm2\n"
		"movq %%mm2, %%mm3\n"
		"pand REDS, %%mm2\n"
		"pslld $8, %%mm2\n"
		
		"movq %%mm3, %%mm4\n"
		"pand GREENS, %%mm3\n"
		"pslld $5, %%mm3\n"
		"por %%mm3, %%mm2\n"
		
		"pand BLUES, %%mm4\n"
		"pslld $3, %%mm4\n"
		"por %%mm4, %%mm2\n"
		
		"psllq $32, %%mm2\n"
		"psrlq $32, %%mm2\n"
		"movq %%mm2, 8(%1)\n"
		
		"movq %%mm1, %%mm2\n"
		"punpcklwd %%mm2, %%mm2\n"
		"movq %%mm2, %%mm3\n"
		"pand REDS, %%mm2\n"
		"pslld $8, %%mm2\n"
		
		"movq %%mm3, %%mm4\n"
		"pand GREENS, %%mm3\n"
		"pslld $5, %%mm3\n"
		"por %%mm3, %%mm2\n"
		
		"pand BLUES, %%mm4\n"
		"pslld $3, %%mm4\n"
		"por %%mm4, %%mm2\n"
		
		"psllq $32, %%mm2\n"
		"psrlq $32, %%mm2\n"
		"movq %%mm2, 16(%1)\n"
		
		"punpckhdq %%mm1, %%mm1\n"
		"movq %%mm1, %%mm2\n"
		"punpcklwd %%mm2, %%mm2\n"
		"movq %%mm2, %%mm3\n"
		"pand REDS, %%mm2\n"
		"pslld $8, %%mm2\n"
		
		"movq %%mm3, %%mm4\n"
		"pand GREENS, %%mm3\n"
		"pslld $5, %%mm3\n"
		"por %%mm3, %%mm2\n"
		
		"pand BLUES, %%mm4\n"
		"pslld $3, %%mm4\n"
		"por %%mm4, %%mm2\n"
		
		"psllq $32, %%mm2\n"
		"psrlq $32, %%mm2\n"
		"movq %%mm2, 24(%1)\n"
		
		"addl $16, %0\n"
		"addl $32, %1\n"
		"decl %2\n"
		"jnz 3b\n"
		".p2align 4,,7\n"
		"4:\n"
		:
		: "S"(src), "D"(dst), "c"(w)
		: "%ebx");
	}
	else
	{
		asm(
		"testb $7, %%cl\n"
		"je 2f\n"
		".p2align 4,,7\n"
		"1:\n"
		"movw (%0), %%ax\n"
		"movl %%eax, %%ebx\n"
		
		"andl $0xf800, %%eax\n"
		"shl $8, %%eax\n"
		
		"movl %%ebx, %%edx\n"
		"andl $0x07e0, %%ebx\n"
		"shl $5, %%ebx\n"
		"orl %%ebx, %%eax\n"
		
		"andl $0x001f, %%edx\n"
		"shl $3, %%edx\n"
		"orl %%edx, %%eax\n"
		"xorl %%ebx, %%ebx\n"
		
		"movl %%eax, (%1)\n"
		"movl %%ebx, 4(%1)\n"
		"addl $2, %0\n"
		"addl $8, %1\n"
		"decl %2\n"
		"testb $7, %%cl\n"
		"jne 1b\n"
		"2:\n"
		"sarl $2, %2\n"
		"jz 4f\n"
		".p2align 4,,7\n"
		"3:\n"
		"movq (%0), %%mm0\n"
		"movq %%mm0, %%mm1\n"
		"punpcklwd %%mm0, %%mm0\n"
		"punpckhwd %%mm1, %%mm1\n"
		
		"movq %%mm0, %%mm2\n"
		"punpcklwd %%mm2, %%mm2\n"
		"movq %%mm2, %%mm3\n"
		"pand REDS, %%mm2\n"
		"pslld $8, %%mm2\n"
		
		"movq %%mm3, %%mm4\n"
		"pand GREENS, %%mm3\n"
		"pslld $5, %%mm3\n"
		"por %%mm3, %%mm2\n"
		
		"pand BLUES, %%mm4\n"
		"pslld $3, %%mm4\n"
		"por %%mm4, %%mm2\n"
		
		"psllq $32, %%mm2\n"
		"psrlq $32, %%mm2\n"
		"movq %%mm2, (%1)\n"
		
		"punpckhdq %%mm0, %%mm0\n"
		"movq %%mm0, %%mm2\n"
		"punpcklwd %%mm2, %%mm2\n"
		"movq %%mm2, %%mm3\n"
		"pand REDS, %%mm2\n"
		"pslld $8, %%mm2\n"
		
		"movq %%mm3, %%mm4\n"
		"pand GREENS, %%mm3\n"
		"pslld $5, %%mm3\n"
		"por %%mm3, %%mm2\n"
		
		"pand BLUES, %%mm4\n"
		"pslld $3, %%mm4\n"
		"por %%mm4, %%mm2\n"
		
		"psllq $32, %%mm2\n"
		"psrlq $32, %%mm2\n"
		"movq %%mm2, 8(%1)\n"
		
		"movq %%mm1, %%mm2\n"
		"punpcklwd %%mm2, %%mm2\n"
		"movq %%mm2, %%mm3\n"
		"pand REDS, %%mm2\n"
		"pslld $8, %%mm2\n"
		
		"movq %%mm3, %%mm4\n"
		"pand GREENS, %%mm3\n"
		"pslld $5, %%mm3\n"
		"por %%mm3, %%mm2\n"
		
		"pand BLUES, %%mm4\n"
		"pslld $3, %%mm4\n"
		"por %%mm4, %%mm2\n"
		
		"psllq $32, %%mm2\n"
		"psrlq $32, %%mm2\n"
		"movq %%mm2, 16(%1)\n"
		
		"punpckhdq %%mm1, %%mm1\n"
		"movq %%mm1, %%mm2\n"
		"punpcklwd %%mm2, %%mm2\n"
		"movq %%mm2, %%mm3\n"
		"pand REDS, %%mm2\n"
		"pslld $8, %%mm2\n"
		
		"movq %%mm3, %%mm4\n"
		"pand GREENS, %%mm3\n"
		"pslld $5, %%mm3\n"
		"por %%mm3, %%mm2\n"
		
		"pand BLUES, %%mm4\n"
		"pslld $3, %%mm4\n"
		"por %%mm4, %%mm2\n"
		
		"psllq $32, %%mm2\n"
		"psrlq $32, %%mm2\n"
		"movq %%mm2, 24(%1)\n"
		
		"addl $8, %0\n"
		"addl $32, %1\n"
		"decl %2\n"
		"jnz 3b\n"
		".p2align 4,,7\n"
		"4:\n"
		:
		: "S"(src), "D"(dst), "c"(w)
		: "%ebx");
	}
}

void clear_ycbr(uint8 *src, int h, int bytes_per_row)
{
	uint8 *bits = (uint8 *)src;
	for (int y = 0; y < h; y++) {
	for (int i = 0; i < bytes_per_row; i += 2) {
			bits[i] = 16;
			bits[i + 1] = 128;
		}
		bits += bytes_per_row;
	}
}

static int32 heights[512];
static int32 widths[512];

void calculate_sizes(int32 *data, int s, int d)
{
	int ratio = (s << 16) / d;
	int pval = 0;
	for(int i = 0; i < d; i++) {
		data[i] = pval >> 16;
		pval += ratio;
	}
}

void draw_16_to_ycbr_multi(uint8 *src, uint8 *dst, int sw, int sh, int dw, int dh, int sr, int dr)
{
	static int lastwidth = 0;
	static int lastheight = 0;
	if(sw != lastwidth) calculate_sizes(widths, sw, dw);
	if(sh != lastheight) calculate_sizes(heights, sh, dh);
	sr >>= 1;
	dw >>= 1;
	uint32 *pal = (uint32 *)palette_ycbcr;
	
	for(int yy = 0; yy < dh; yy++) {
		uint16 *ss = (uint16 *)src + heights[yy] * (sr);
		uint32 *dd = (uint32 *)dst;
		int xx = 0;
		for(int x = 0; x < dw; x++) {
			uint16 p0 = ss[widths[xx++]];
			uint32 err = pal[p0];
			p0 = ss[widths[xx++]];
			err |= (y[p0] << 16);
			*dd++ = err;
		}
		dst += dr;
	}
	lastwidth = sw;
	lastheight = sh;
}

void draw_16_to_16_multi(uint8 *src, uint8 *dst, int sy, int dy, int sw, int sh, int dw, int dh, int sbytes_per_row, int bytes_per_row)
{
	int x, y, u, v = 0;
	int xratio = (sw * 0x10000) / (dw);
	int yratio = (sh * 0x10000) / (dh);
	const int mul = sy == 8 ? 2 : 1;
	
	uint16 *ssrc = (uint16 *)src;
	uint16 *ddst = (uint16 *)dst;
	uint16 *sourceRow, *dest;
	sbytes_per_row >>= 1;
	bytes_per_row >>= 1;
	
	for(y = 0; y < dh; y++) {
		u = 0;
		sourceRow = ssrc + (v >> 16) * (sbytes_per_row);
		dest = ddst;
		if(y % 2 == 0) {
			for(x = 0; x < dw; x++) {
				*dest++ = sourceRow[u >> 16];
				u += xratio*mul;
			}
		}
		v += yratio;
		ddst += bytes_per_row;
	}
}
