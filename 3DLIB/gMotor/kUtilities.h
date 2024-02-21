/*****************************************************************
/*  File:   Utilities.h                                          *
/*  Desc:   Helper routines                                      *
/*  Author: Silver, Copyright (C) GSC Game World                 *
/*  Date:   December 2001                                        *
/*****************************************************************/
#ifndef __UTILITIES_H__
#define __UTILITIES_H__

#pragma	once

/*****************************************************************
/*  Class:  util                                                 *
/*  Desc:   Different helper stuff                               *
/*****************************************************************/
class util{
public:
	util();

	static BYTE			powSidesByte[8];
	static float		powSidesFloat[8];

	static float		byteAngleCos[256];
	static float		byteAngleSin[256];
	static int			rnd[1024];

	static float		PI;	
	static float		DoublePI;	
	static float		PIdiv16;
	static float		PIdiv32;
	static float		PImul2;	
	static float		sin30;

	static WORD			quad8x8Idx[384];

	static void InitValueArrays();

	static inline float RndValuef()
    {
        float rndval = static_cast<float>( rand() );
        rndval /= (float)RAND_MAX;
        return rndval;
    }

    static inline int RndValuei( int min, int max )
    {
        return min + static_cast<int>( static_cast<float>( max - min ) * RndValuef() ); 
    }

	static inline float rndValuef( float min, float max )
    {
        return min + static_cast<float>( max - min ) * RndValuef(); 
    }

	static _inl void MemcpyRect(	BYTE* dst,			int dstPitch,
									const BYTE* src,	int srcPitch, 
									int nLines, 
									int bytesInLine );

	static _inl void QuadShrink2xW( BYTE* dst,			int dstPitch,
									const BYTE* src,	int srcPitch, 
									int srcQuadSide );

	static _inl void QuadShrink4xW( BYTE* dst,			int dstPitch,
									const BYTE* src,	int srcPitch, 
									int srcQuadSide );


	static inline void memcpy( void* dest, const void* src, int nBytes )
	{
		::memcpy( dest, src, nBytes );
	}

	static inline void memAndRect(	BYTE* src,  int srcPitch, 
									int nLines, int lineBytes, BYTE andB )
	{
		for (int i = 0; i < nLines; i++)
		{
			for (int j = 0; j < lineBytes; j++) src[j] &= andB;
			src  += srcPitch;
		}
	}

	static inline float minf( float a, float b, float c, float d )
	{
		return minf( minf( a, b ), minf( c, d ) );
	}

	static inline float minf( float a, float b )
	{
		return a < b ? a : b;
	}

	static inline float maxf( float a, float b, float c, float d )
	{
		return maxf( maxf( a, b ), maxf( c, d ) );
	}

	static inline float maxf( float a, float b )
	{
		return a > b ? a : b;
	}

private:
	static util	dummy;
}; // class util

#ifdef _INLINES
#include "kUtilities.inl"
#endif // _INLINES

#endif // __UTILITIES_H__