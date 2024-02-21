/*****************************************************************/
/*  File:   rsTextureDX.h
/*  Desc:   Direct3D texture management 
/*  Author: Silver, Copyright (C) GSC Game World
/*  Date:   Feb 2002
/*****************************************************************/
#ifndef __RSTEXTUREDX_H__
#define __RSTEXTUREDX_H__

#include "kHash.hpp"

/*****************************************************************************/
/*	Class:	HashedTexture
/*	Desc:	Texture	storage element
/*****************************************************************************/
class HashedTexture
{
public:
	_inl unsigned int	hash	() const;
	_inl bool			equal	( const HashedTexture& el );
	_inl void			copy	( const HashedTexture& el );

	void		Reload			( DXDevice* pDevice );
	bool		CreateFromFile	( DXDevice* pDevice, bool forceFormat = false );
	bool		Create			( DXDevice* pDevice, const TextureDescr* td = 0 );
	int			GetSizeBytes	( DXDevice* pDevice ) const;
	void		DumpStatus		( DXDevice* pDevice, FILE* fp );
	bool		InvalidateDeviceObjects	( DXDevice* pDevice );
	bool		RestoreDeviceObjects	( DXDevice* pDevice );

protected:
	_inl HashedTexture( const char* texName );
	_inl HashedTexture();

	DXTexture*					iTexture;	//  D3D texture interface
	DXSurface*					iSurface0;  //  cached surface level 0 interface

	String						name;
	TextureDescr				descr;

private:
	friend class	TextureCache;
}; // class HashedTexture

const int c_TextureCacheHashTableSize	= 2048;
const int c_TextureCacheHashPoolSize	= 512;

/*****************************************************************************/
/*	Class:	TextureCache
/*	Desc:	Texture storage & managament class
/*****************************************************************************/
class TextureCache
{
	DXDevice*					pDevice;
public:
	TextureCache();
	~TextureCache();
	
	void			Init				( DXDevice* pDev );
	void			Shut				();
	void			ReloadTextures		();
	void			ResetDeviceResources();
	void			SaveTexture			( int texID, const char* fname );
	void			DumpStatus			( FILE* fp );
	void			LogStatus			();
	
	_inl DXSurface*	GetTextureSurface	( int texID, int level = 0 );
	_inl void		CopyTexture			( int destID, int srcID );
	_inl void		CopyTexture			(	int destID, int srcID, 
											RECT* rectList, int nRect, 
											POINT* ptList );
	_inl void		CopyTexture			( DXSurface* destSurf, int srcID, 
											RECT* rectList, int nRect, POINT* ptList = 0 );

	_inl void		SetTexture			( int texID, int stage = 0 );
	_inl int		GetTextureSizeBytes	( int texID );
	_inl BYTE*		LockTexture			( int texID, int& pitch, int level = 0 );
	_inl void		UnlockTexture		( int texID, int level = 0 );
	
	int				CreateTexture		( const char* texName, const TextureDescr& td,
										  TextureClient* client = 0 );
	_inl int		GetTextureID		( const char* texName );
	_inl int		GetTextureID		( const char* texName, const TextureDescr& td );

	_inl int		GetCurrentTexture	( int stage = 0 ) const;

	bool			InvalidateDeviceObjects();
	bool			RestoreDeviceObjects();
	void			SetDevice( DXDevice* pDev ) { pDevice = pDev; }
	int				GetUsedTextureMemory();


protected:
	int							curTexture[c_MaxTextureStages];
	Hash<	HashedTexture, 
			c_TextureCacheHashTableSize,
			c_TextureCacheHashPoolSize>			hash;

private:

	_inl	void			resetCurTextures	();
	_finl	DXTexture*		getDXTex			( int id );
	_inl	HashedTexture&	getHashedTexture	( int id );

}; // class TextureCache

#define TC_CHK(A)	{ if ((A) != S_OK) LogStatus(); DX_CHK((A)); }

#ifdef _INLINES
#include "rsTextureDX.inl"
#endif // _INLINES

#endif // __RSTEXTUREDX_H__