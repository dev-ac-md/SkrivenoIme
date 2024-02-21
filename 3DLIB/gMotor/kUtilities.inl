/*****************************************************************************/
/*	File:	kUtilities.inl
/*	Author:	Ruslan Shestopalyuk
/*	Date:	23.04.2003
/*****************************************************************************/

/*****************************************************************************/
/*	util implementation
/*****************************************************************************/
_inl void util::MemcpyRect(	BYTE* dst,			int dstPitch,
							const BYTE* src,	int srcPitch, 
							int nLines, 
							int bytesInLine )
{
	for (int i = 0; i < nLines; i++)
	{
		memcpy( dst, src, bytesInLine );
		dst += dstPitch;
		src += srcPitch;
	}
} // util::MemcpyRect


#define GET_A(W) ((W & 0xF000)>>12)
#define GET_R(W) ((W & 0x0F00)>>8)
#define GET_G(W) ((W & 0x00F0)>>4)
#define GET_B(W) ((W & 0x000F))
#define ARGB(A,R,G,B) (((A)<<12) | ((R)<<8) | ((G)<<4) | (B))

_inl void util::QuadShrink2xW( BYTE* dst,		int dstPitch,
							   const BYTE* src,	int srcPitch, 
							   int srcQuadSide )
{
	int dstQuadSide = srcQuadSide >> 1;
	int srcPitch2 = srcPitch << 1;
	for (int  i = 0; i < dstQuadSide; i++)
	{
		WORD* pDst  = (WORD*)dst;
		WORD* pSrc1 = (WORD*)src;
		WORD* pSrc2 = (WORD*)(src + srcPitch);
		for (int j = 0; j < dstQuadSide; j++)
		{
			DWORD pix00 = pSrc1[j * 2		];
			DWORD pix01 = pSrc1[j * 2 + 1	];
			DWORD pix10 = pSrc2[j * 2		];
			DWORD pix11 = pSrc2[j * 2 + 1	];
			
			DWORD a = GET_A(pix00) + GET_A(pix01) + GET_A(pix10) + GET_A(pix11);
			DWORD r = GET_R(pix00) + GET_R(pix01) + GET_R(pix10) + GET_R(pix11);
			DWORD g = GET_G(pix00) + GET_G(pix01) + GET_G(pix10) + GET_G(pix11);
			DWORD b = GET_B(pix00) + GET_B(pix01) + GET_B(pix10) + GET_B(pix11);
						 
			a >>= 2; r >>= 2; g >>= 2; b >>= 2;

			a=(GET_A(pix00)+a)>>1;
			r=(GET_R(pix00)+r)>>1;
			g=(GET_G(pix00)+g)>>1;
			b=(GET_B(pix00)+b)>>1;
			pDst[j] = ARGB( a, r, g, b );
		}
		dst += dstPitch;
		src += srcPitch2;
	}
} // util::QuadShrink2xW

_inl void util::QuadShrink4xW( BYTE* dst,		int dstPitch,
						   const BYTE* src,	int srcPitch, 
							   int srcQuadSide )
{

} // util::QuadShrink4xW

