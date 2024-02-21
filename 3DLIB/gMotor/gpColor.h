/*****************************************************************
/*  File:   gpColor.h                                            *
/*  Desc:   Color operations declaration
/*  Author: Silver, Copyright (C) GSC Game World                 *
/*  Date:   January 2002                                         *
/*****************************************************************/
#ifndef __GPCOLOR_H__
#define __GPCOLOR_H__
#include "gmDefines.h"

//  texture/surface/bitmap etc. color formats 
enum ColorFormat
{
	cfUnknown		= 0,
	cfARGB4444		= 1,
	cfXRGB1555		= 2,
	cfARGB8888		= 3,
	cfRGB565		= 4,
	cfA8			= 5,
	cfRGB888		= 6,
	cfXRGB8888		= 7,
	cfV8U8			= 8,

	cfBackBufferCompatible	= 9
}; // enum ColorFormat

/*****************************************************************
/*  Class:  GPColor                                              *
/*  Desc:   represents color valaue
/*****************************************************************/
class GPColor
{
	float					a;
	float					r;
	float					g;
	float					b;

public:
							GPColor();
	_inl void				SetARGB( BYTE a, BYTE r, BYTE g, BYTE b );
	
	static _inl DWORD		ToARGB( BYTE a, BYTE r, BYTE g, BYTE b );
	static _inl DWORD		ToARGB( float a, float r, float g, float b );
	static _inl int			GetBytesPerPixel( ColorFormat cf );
	
	void					Dump();
}; // class GPColor

#ifdef _INLINES
#include "gpColor.inl"
#endif // _INLINES

#endif // __GPCOLOR_H__