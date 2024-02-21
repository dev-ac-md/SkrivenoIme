/*****************************************************************************/
/*	File:	gpRenderSystem.h
/*	Desc:	
/*	Author:	Ruslan Shestopalyuk
/*	Date:	23.01.2003
/*****************************************************************************/
#ifndef __GPRENDERSYSTEM_H__
#define __GPRENDERSYSTEM_H__
#pragma once

/*****************************************************************************/
/*	Enum:	MemoryPool
/*	Desc:	Memory location of resource
/*****************************************************************************/
enum MemoryPool
{
	mpUnknown		= 0,
	mpSysMem		= 1,
	mpVRAM			= 2,
	mpManaged		= 3
};  // enum MemoryPool

/*****************************************************************************/
/*	Enum:	BufferFormat
/*	Desc:	Type of used resource buffer	
/*****************************************************************************/
enum BufferFormat
{
	bfUnknown		= 0,
	bfStatic		= 1,
	bfDynamic		= 2
};	//  enum BufferFormat

/*****************************************************************************/
/*	Enum:	TextureUsage	
/*****************************************************************************/
enum TextureUsage
{
	tuUnknown		= 0,
	tuLoadable		= 1,
	tuProcedural	= 2,
	tuRenderTarget  = 3,
	tuDynamic		= 4
};	// enum TextureUsage

/*****************************************************************************/
/*	Enum:	DeviceCapability
/*****************************************************************************/
enum DeviceCapability
{
	dcUnknown						= 0,
	dcIndexedVertexBlending			= 1,
	dcBumpEnvMap					= 2,
	dcBumpEnvMapLuminance			= 3,
	dcDynamicTextures				= 4,
	dcTnL							= 5,
	dcPure							= 6,
	dcRasterization					= 7,
	dcBezier						= 8,
	dcRTPatches						= 9,
	dcNPatches						= 10,
	dcREF							= 11
};	// enum DeviceCapability

/*****************************************************************************/
/*	Enum: DepthStencilFormat
/*****************************************************************************/
enum DepthStencilFormat
{
	dsfUnknown			= 0
}; // DepthStencilFormat

class TextureDescr;
/*****************************************************************************/
/*	Class:	TextureClient
/*	Desc:	User of the texture
/*****************************************************************************/
class TextureClient
{
public:
	virtual void Invalidate( TextureDescr* texture ) = 0;

}; // class TextureClient

/*****************************************************************************/
/*	Class:	TextureDescr
/*	Desc:	Texture description class
/*****************************************************************************/
class TextureDescr
{
public:
	TextureDescr();
	
	bool equal( const TextureDescr& td );
	bool less( const TextureDescr& td );
	void copy( const TextureDescr& orig );
	
	void			setValues( int sx, int sy, 
								ColorFormat		cf		= cfARGB4444,
								MemoryPool		mp		= mpVRAM, 
								int				nmips	= 1,
								TextureUsage	tu		= tuLoadable );

	int				getSideX()		const { return sideX;					}
	int				getSideY()		const { return sideY;					}
	ColorFormat		getColFmt()		const { return (ColorFormat)colFmt;		}
	MemoryPool		getMemPool()	const { return (MemoryPool)memPool;		}
	TextureUsage	getTexUsage()	const { return (TextureUsage)texUsage;	}
	int				getNMips()		const { return numMips;					}
	TextureClient*	getClient()		const { return client;					}
	const char*		getPoolStr()	const;
	const char*		getColFmtStr()	const;
	const char*		getUsageStr()	const;

	void			setSideX	( int sx			)	{ sideX		= sx;		}
	void			setSideY	( int sy			)	{ sideY		= sy;		}
	void			setColFmt	( ColorFormat cf	)	{ colFmt	= (BYTE)cf; }
	void			setMemPool	( MemoryPool mp		)	{ memPool	= (BYTE)mp;	}
	void			setTexUsage	( TextureUsage tu	)	{ texUsage	= (BYTE)tu; }
	void			setNMips	( int n				)	{ numMips	= n;		}
	void			setClient	( TextureClient* cl )	{ client	= cl;		}
	void			setID		( int _id			)	{ id		= _id;		}
	
	void			invalidate	() 
	{ 
		valid = false; 
		if (client) client->Invalidate( this ); 
	}

protected:
	int				sideX;		//  width
	int				sideY;		//  height

	BYTE			colFmt;		//  color format
	BYTE			memPool;	//  memory pool
	BYTE			texUsage;	//  usage 
	
	int				numMips;	//  number of mip levels

	//  runtime properties
	int				id;
	bool			valid;
	TextureClient*	client;
}; // class TextureDescr

/*****************************************************************************/
/*	Class:	ScreenProp
/*	Desc:	Describes screen properties
/*****************************************************************************/
class ScreenProp
{
public:
	ScreenProp();

	int				width;
	int				height;
	ColorFormat		colorFormat;
	bool			fullScreen;
	bool			wholeScreenWnd;
	int				refreshRate;

	bool equal( const ScreenProp& prop ) const;

}; // class ScreenProp

/*****************************************************************************/
/*	Class:	ViewPort
/*	Desc:	Rendering viewport	
/*****************************************************************************/
class ViewPort
{
public:
    DWORD       x;
	DWORD       y;
    DWORD       width;
    DWORD       height;
    
	float       MinZ;
    float       MaxZ;

	_inl bool	ClipHLine( int& px1, int& px2, int py ) const
	{
		if (py < y || py >= y + height) return false; 
		px1 = px1 < x ? x : px1;
		px2 = px2 < x ? x : px2;
		
		int vpRight  = x + width - 1;

		px1 = px1 > vpRight ? vpRight : px1;
		px2 = px2 > vpRight ? vpRight : px2;
		return (px1 != px2);
	}

	_inl bool	ClipVLine( int px, int& py1, int& py2 ) const
	{
		if (px < x || px >= x + width) return false;
		py1 = py1 < y ? y : py1;
		py2 = py2 < y ? y : py2;
		
		int vpBottom  = y + height - 1;

		py1 = py1 > vpBottom ? vpBottom : py1;
		py2 = py2 > vpBottom ? vpBottom : py2;
		return (py1 != py2);
	}

	_inl bool ClipRect( int& px, int& py, int& pw, int& ph ) const
	{
		if (px < x)
		{
			pw -= x - px;
			px = x;
		}

		if (py < y)
		{
			ph -= y - ph;
			py = y;
		}

		int vpRight  = x + width;
		int vpBottom = y + height;
		if (px + pw > vpRight)  pw = vpRight  - px;
		if (py + ph > vpBottom) ph = vpBottom - py;
		return ((pw != 0) && (ph != 0));
	}

	_inl bool IsPtInside( int px, int py ) const
	{
		return (px >= x && px < x + width && py >= y && py < y + height);
	}

	_inl bool IsRectInside( int px, int py, int pw, int ph ) const
	{
		return (px >= x && px + pw < x + width && 
				py >= y && py + ph < y + height);
	}

	_inl bool ClipY( int py ) const
	{
		return (py < y) || ( py >= y + height ); 
	}

	_inl DWORD GetRight() const
	{
		return x + width - 1;
	}

	_inl DWORD GetBottom() const
	{
		return y + height - 1;
	}
}; // class ViewPort

/*****************************************************************************/
/*	Class:	DeviceDescr
/*	Desc:	3D device capabilities
/*****************************************************************************/
class DeviceDescr
{
public:
	DeviceDescr();

	int				adapterOrdinal;
	String			devDriver;
	String			devDescr;
	
	int				texBlendStages;
	int				texInSinglePass;

	c2::array<DeviceCapability>		capBits;
	c2::array<DepthStencilFormat>	depthStencil;
	c2::array<ColorFormat>			renderTarget;	
}; // class DeviceDescr

#endif // __GPRENDERSYSTEM_H__