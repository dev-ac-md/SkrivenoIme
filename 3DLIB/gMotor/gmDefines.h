/*****************************************************************
/*  File:   Defines.h                                            *
/*  Desc:   Constant values and definitions                      *
/*  Author: Silver, Copyright (C) GSC Game World                 *
/*  Date:   January 2002                                         *
/*****************************************************************/
#ifndef __DEFINES_H__
#define __DEFINES_H__
#pragma	 once

typedef unsigned char       BYTE;
typedef unsigned short      WORD;
typedef unsigned int	    UINT;
typedef unsigned long		DWORD;
typedef long				LONG;

#include "string.h"
#include "stdio.h"
#include "stdlib.h"
#include <math.h>

#ifdef _INLINES
#define _inl inline
#define _finl __forceinline
#else
#define _inl 
#define _finl 
#endif // _INLINES

// warning 4018: 'expression' : signed/unsigned mismatch
#pragma warning ( disable : 4018 )
// warning C4244: 'initializing' : 
//  conversion from 'double' to 'float', possible loss of data
#pragma warning ( disable : 4244 )
//  256 symbols name truncation
#pragma warning ( disable : 4786 )


//  wide-used constants
const int	c_NoID			=	0xFFFFFFFF;
const WORD	c_AnyTexture	=	0xFFFF;

#define BEGIN_NAMESPACE(A)	namespace A{
#define END_NAMESPACE(A)	}

#define BEGIN_ONCE		{static ___first__ = true; if (___first__) {___first__ = false;  
#define END_ONCE		}}

#endif //__DEFINES_H__
