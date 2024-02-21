/*****************************************************************/
/*  File:   GPSystem.h
/*  Desc:   Sprite manager
/*  Author: Silver, Copyright (C) GSC Game World
/*  Date:   Feb 2002
/*****************************************************************/
#ifndef __GPSYSTEM_H__
#define __GPSYSTEM_H__

#ifndef GP_USER
#define GP_API __declspec(dllexport)
#else
#define GP_API __declspec(dllimport)
#endif

#pragma warning ( disable : 4786 )
#include "gmDefines.h"

#include "kResFile.h"
#include "kCache.h"
#include "kHash.hpp"

#include "IRenderSystem.h"

#include "gpBucket.h"

const int c_MaxGPSeqs				= 4096;

const DWORD c_InsideChunks			= 0xFFFFFFFE;
const DWORD c_OutsideChunks			= 0xFFFFFFFF;

const int c_GPFrameInstHashTableSize	= 104729;
const int c_GPFrameInstHashPoolSize		= 16384;
const int c_GPHashTableSize			= 7919;

class GPSeq;
class GPChunk;
class GPSystem;

/*****************************************************************************/
/*	Enum:	GPBatchMode	
/*	Desc:	Modes for batch drawing  sprites
/*****************************************************************************/
enum GPBatchMode
{
	bmUnknown			= 0,	
	bmSortByTexture		= 1,	//  batching by texture surface
	bmSortByZ			= 2,	//  drawing in Z-order
	bmBlockBurst		= 3,	//  bursting block chunks into buffer VMEM texture
	bmDrawImmediately	= 4,	//  draw right here, right now
	bmDrawLazy			= 5		//  postpone drawing until need texture switch
}; // enum GPBatchMode

typedef PointerHash<	GPFrameInst, 
						GPFrameInst::Key, 
						c_GPFrameInstHashTableSize, 
						c_GPFrameInstHashPoolSize>		GPFrameHash;
typedef PointerHash<GPSeq, String, c_GPHashTableSize, c_MaxGPSeqs> GPPackageHash;
		
class ComplexBitmap;
class SoftwareVS;
class GPFont;
/*****************************************************************/
/*  Class:	GPSystem
/*  Desc:	Sprite manager
/*****************************************************************/
class GPSystem 
{
public:
						GPSystem();
	virtual				~GPSystem();

	//  initializing system
	virtual void		Init( IRenderSystem* irs );

	//  generic sprite drawing function
	bool		DrawSprite(	int	gpID, int sprID,
							float x, float y, float	z,
							bool mirrored, 
							DWORD color = 0x00000000 );
	
	bool		DrawSpriteTransformed(	int gpID, int sprID, 
										const Matrix4D& transf, 
										DWORD color = 0x00000000 );

	bool		DrawSpriteAligned(	int gpID, int sprID, 
									const Plane& pl, 
									float x, float y, float	z,
									bool mirrored,
									DWORD color = 0x00000000 );


	bool		DrawWSprite(	int	gpID, int sprID,
								float x, float y, float	z,
								bool mirrored, 
								DWORD color = 0x00000000 );

	bool		DrawWSpriteTransformed(	int gpID, int sprID, 
										const Matrix4D& transf, 
										DWORD color = 0x00000000 );

	bool		DrawWSpriteAligned(	int gpID, int sprID, 
									const Plane& pl, 
									float x, float y, float	z,
									bool mirrored,
									DWORD color = 0x00000000 );

	bool		DrawSpriteVS( int gpID, int sprID, DWORD color, const SoftwareVS& vs );
	

	void		DrawLine	( float x1, float y1, float x2, float y2, DWORD color );
	void		DrawLine	( const Vector3D& beg, const Vector3D& end, DWORD color );
	void		DrawFillRect( float x, float y, float w, float h, DWORD color );
	void		DrawRect( float x, float y, float w, float h, DWORD color );
	
	void		DrawFillRect( const Vector3D& lt, const Vector3D& rt, 
								const Vector3D& lb, const Vector3D& rb, 
								DWORD color );

	void		DrawString( const char* str, 
							float x, float y, float z,
							DWORD color		= 0xFFFFFFFF,	//  color of text
							float height	= -1.0f,		//  height of text, in pixels
							Camera* cam		= NULL			//  when NULL text position is taken in screen space
							);

	//  package loading / unloading
	int			PreLoadGPImage	( const char* gpName );

	bool		LoadGP			( int seqID );
	void		UnLoadGP		( int seqID );

	_inl GPBatchMode SetBatchMode	( GPBatchMode	mode );
	_inl int	SetCurShader	( int shID );
	
	_inl float	SetScale		( float scale );
	_inl float	GetScale		() const;	
	_inl bool	SetClipArea		( DWORD x, DWORD y, DWORD w, DWORD h );
	_inl const ViewPort&		GetClipArea() const;
	
	//  sprite attributes getters
	_inl int	GetGPHeight		( int seqID, int frameIdx )					const;
	_inl int	GPNFrames		( int seqID )								const;
	_inl int	GetGPNum		()											const;
	_inl int	GetGPWidth		( int seqID, int frameIdx )					const;
	_inl const char* GetGPName	( int seqID )								const;

	_inl void	SetSpritePivot	(	int gpID, int sprID, 
									int centerX, int centerY )				const;

	_inl void	SetCurrentZ		( float z );
	_inl void	SetCurrentDiffuse( DWORD color );

	_inl float	GetCurrentZ		()											const;
	_inl DWORD	GetCurrentDiffuse()											const;


	_inl bool	GetGPBoundFrame	( int seqID, int frameIdx, Frame& frame );
	
	_inl void	SetCurrentCamera( Camera* cam );
	_inl void	FlushPrimBucket();
	_inl void	FlushSpriteBucket( int bucketIdx );

	void		OnFrame();
	void		FlushBatches();

	void		EnableZBuffer	( bool enable = true );
	DWORD		GetGPAlpha		(	int seqID, int frameIdx, 
									int ptX, int ptY, 
									bool precise = false );

	_inl static int GetNumTextureCacheSurfaces(); 

protected:
	void					DrawBuckets();
	void					CleanBuckets();

	_inl GPSeq*				GetSeq					( int idx ); 
	_inl GPSeq*				c_GetSeq				( int idx ) const; 
	_inl GPFrameInst*		GetFrameInstForDrawing	( int& seqID, int& sprID, DWORD color, WORD lod = 0 );
	_inl GPFrameInst*		GetFrameInst			( int seqID, int sprID, DWORD color, WORD lod = 0 );
	_inl int				UnswizzleFrameIndex		( int seqID, int sprID ) const;
	_inl GPFrame*			GetFrame				( int seqID, int sprID );
	_inl const GPFrame*		c_GetFrame				( int seqID, int sprID ) const;


private:

	void					dbgDrawBucketZs( int bucketIdx );
	GPFrameHash				frameHash;
	GPPackageHash			gpHash;
	float					gpScale;
	WORD					curLOD;

	float					curZ;
	DWORD					curDiffuse;

	
	Camera*					curCamera;
	int						curBucket;
	
	GPBatchMode				batchMode;

	//  sprite drawing buckets
	c2::array<GPBucket>		sprBucket;

	BaseMesh				rectBucket2D;
	BaseMesh				lineBucket2D;
	BaseMesh				rectBucket3D;
	BaseMesh				lineBucket3D;
	
	GPFont*					debugFont;
	
	ViewPort				curViewPort;
	static int				s_GPTexNum;

	char					rootDirectory[_MAX_PATH];

public:
	static int				curCacheFactor;

	friend class			GPPixelCache;
};  // class GPSystem

//#pragma pack ( pop )

#include "GPSystem.hpp"

#ifdef _INLINES
#include "GPSystem.inl"
#endif // _INLINES


#endif // __GPSYSTEM_H__