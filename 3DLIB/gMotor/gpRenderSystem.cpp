/*****************************************************************************/
/*	File:	gpRenderSystem.cpp
/*	Desc:	
/*	Author:	Ruslan Shestopalyuk
/*	Date:	23.01.2003
/*****************************************************************************/
#include "stdafx.h" 
#include "gpRenderSystem.h"

const char* c_PoolStr[] = 
{
	"Unknown",
	"SysMem",
	"Default",
	"Managed"
};

const char* c_TexUsageStr[] = 
{
	"tuUnknown",
	"tuLoadable",
	"tuProcedural",
	"tuRenderTarget",
	"tuDynamic"
};	// enum TextureUsage

const char* c_ColFmtStr[] = 
{
	"cfUnknown",
	"cfARGB4444",
	"cfXRGB1555",
	"cfARGB8888",
	"cfRGB565",
	"cfA8",
	"cfRGB888",
	"cfXRGB8888",
	"cfV8U8",
	"cfBackBufferCompatible"
}; // enum ColorFormat

IRenderSystem*	RC::iRS			= 0;
DWORD			RC::curFrame	= 0;

/*****************************************************************
/*	TextureDescr inplementation
/*****************************************************************/
TextureDescr::TextureDescr()
{
	sideX		= 256;
	sideY		= 256;
	colFmt		= cfARGB4444;
	memPool		= mpManaged;
	numMips		= 0;
	texUsage	= tuLoadable;
	client		= NULL;
	id			= c_NoID;
}

void TextureDescr::setValues( int sx, int sy, ColorFormat cf,
								MemoryPool mp, 
								int nmips,
								TextureUsage tu 
								)
{
	sideX		= sx;
	sideY		= sy;
	colFmt		= cf;
	memPool		= mp;
	numMips		= nmips;
	texUsage	= tu;
}

void TextureDescr::copy( const TextureDescr& orig )
{
	sideX		= orig.sideX;
	sideY		= orig.sideY;
	colFmt		= orig.colFmt;
	memPool		= orig.memPool;
	numMips		= orig.numMips;
	texUsage	= orig.texUsage;
}

bool TextureDescr::equal( const TextureDescr& td )
{
	return (colFmt == td.colFmt) &&
		   (memPool == td.memPool) &&
		   (numMips == td.numMips) &&
		   (sideX == td.sideX) &&
		   (sideY == td.sideY) &&
		   (texUsage == td.texUsage);
}

bool TextureDescr::less( const TextureDescr& td )
{
	return (colFmt		< td.colFmt		) ||
		   (memPool		< td.memPool	) ||
		   (numMips		< td.numMips	) ||
		   (texUsage	< td.texUsage	) ||
		   (sideX		< td.sideX		) ||
		   (sideY		< td.sideY		); 
}

const char*	TextureDescr::getPoolStr() const 
{ 
	return c_PoolStr[(int)memPool]; 
}

const char* TextureDescr::getColFmtStr()const
{
	return c_ColFmtStr[(int)colFmt]; 
}

const char* TextureDescr::getUsageStr()	const
{
	return c_TexUsageStr[(int)texUsage]; 
}

/*****************************************************************************/
/*	ScreenProp implementation
/*****************************************************************************/
ScreenProp::ScreenProp()
{
	width			= 1024;
	height			= 768;
	colorFormat		= cfRGB565;
	fullScreen		= false;
	wholeScreenWnd	= true;
	refreshRate		= 75;
} // ScreenProp::ScreenProp

bool ScreenProp::equal( const ScreenProp& prop ) const
{
	return	(width			== prop.width		) &&
			(height			== prop.height		) &&
			(colorFormat	== prop.colorFormat ) &&
			(fullScreen		== prop.fullScreen	) &&
			(wholeScreenWnd	== prop.wholeScreenWnd);
}

/*****************************************************************************/
/*	DeviceDescr implementation
/*****************************************************************************/
/*
void DeviceDescr::LogStatus()
{
	String str;
	str = "DevType: ";
	if (devType == dtHAL) str += "HAL.";
		else str += "REF.";
	Log.Message( str.c_str() );
	
	str = "Adapter ordinal: ";
	str += IntToStr( adapterOrdinal );
	Log.Message( str.c_str() );

	str = "Hardware Rasterization: ";
	str += BoolToStr( hwRasterization );

	str = "Hardware T&L: ";
	str += BoolToStr( hwTnL );

	if (hwPure) Log.Message( "Pure device support." );
	else Log.Message( "No pure device support" );

	str = "Higher-order primitives. NPatches: ";
	str += BoolToStr( hwNPatches );
	str += ". Bezier patches: ";
	str += BoolToStr( hwBezier );
	str += ". RTPatches: ";
	str += BoolToStr( hwRTPatches );
	str += ".";

	Log.Message( str.c_str() );

	str = "Multitexturing capacity: ";
	str += IntToStr( texInSinglePass );
	Log.Message( str.c_str() );

	str = "Max textures in single pass: ";
	str += IntToStr( texInSinglePass );

	Log.Message( "Available depth/stencil formats:" );
	if (dsfD16)			Log.Message( " - depth16 " );
	if (dsfD15S1)		Log.Message( " - depth15 stencil 1" );
	if (dsfD24S8)		Log.Message( " - depth24 stencil 8" );
	if (dsfD24X8)		Log.Message( " - depth24" );
	if (dsfD32)			Log.Message( " - depth32" );
	if (dsfD24X4S4)		Log.Message( " - depth24 stencil 4" );
	if (dsfD16Lockable)	Log.Message( " - depth16 lockable" );

	Log.Message( "Available render-to-texture formats:" );
	if (rttARGB4444)		Log.Message( " - argb4444 " );
	if (rttRGB565)			Log.Message( " - rgb565" );
	if (rttRGB888)			Log.Message( " - rgb888" );
	if (rttA8)				Log.Message( " - a8" );
	if (rttARGB1555)		Log.Message( " - argb1555" );
} // DeviceDescr::LogStatus
*/