/*****************************************************************
/*  File:   kUtilities.cpp                                       *
/*  Desc:   Helper routines implementation                       *
/*  Author: Silver, Copyright (C) GSC Game World                 *
/*  Date:   December 2001                                        *
/*****************************************************************/
#include "stdafx.h"
#include "kUtilities.h"

#ifndef _INLINES
#include "kUtilities.inl"
#endif // _INLINES

#include <string>

using namespace std;

BYTE  util::powSidesByte[8] = { 1, 2, 4, 8, 16, 32, 64, 128 };
float  util::powSidesFloat[8] = { 1.0f, 2.0f, 4.0f, 8.0f, 
									16.0f, 32.0f, 64.0f, 128.0f };
float util::byteAngleCos[256];
float util::byteAngleSin[256];
util  util::dummy;

float util::PI		= 3.1415926f;
float util::DoublePI= 3.1415926f * 2.0f;
float util::PIdiv16 = 0.1963495f;
float util::PIdiv32 = 0.0981747f;
float util::PImul2	= 6.2831853f;
float util::sin30	= 0.5f;
WORD  util::quad8x8Idx[384];
int	  util::rnd[1024];

	
/*---------------------------------------------------------------*
/*  Func:	util::util
/*	Desc:	Fills arrays of const values, e.g, sin, cos values
/*---------------------------------------------------------------*/
util::util()
{
	//  sin and cos
	for (int i = 0; i < 256; i++)
	{
		byteAngleCos[i] = static_cast<float>( cos( i * PI / 128 ) );
		byteAngleSin[i] = static_cast<float>( sin( i * PI / 128 ) );
	}

	//  vertex indices for 8x8 triangle grid
	int idx = 0;
	int cVert = 0;
	for(int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			quad8x8Idx[idx++] = cVert;
			quad8x8Idx[idx++] = cVert + 1;
			quad8x8Idx[idx++] = cVert + 10;
			quad8x8Idx[idx++] = cVert;
			quad8x8Idx[idx++] = cVert + 10;
			quad8x8Idx[idx++] = cVert + 9;
			cVert++;
		}
		cVert++;
	}

	//  init random uniform values
	for (int i = 0; i < 1024; i++)
	{
		rnd[i] = rand();
	}
}//  util::util
