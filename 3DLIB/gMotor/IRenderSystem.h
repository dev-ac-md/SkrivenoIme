/*****************************************************************
/*  File:   IRenderSystem.h                                      *
/*  Desc:   3D rendering device abstract interface               *
/*  Date:   Feb 2002											 *
/*****************************************************************/
#ifndef __RENDERSYSTEM_H__ 
#define __RENDERSYSTEM_H__
#pragma	once

#include "windows.h"
#include "gmDefines.h"
#include "gpColor.h"
#include "gpRenderSystem.h"

class Matrix4D;
class Drawn;
class BaseMesh;

/*****************************************************************
/*  Class:  IRenderSystem, abstract interface                                  
/*  Desc:   Incapsulates all rendering commands	passed to hardware 
/*				API				 
/*****************************************************************/
class  IRenderSystem  
{
public:
//  O==  PUBLIC INTERFACE  =======================================O
	//  initialize render device 
	virtual void  __stdcall Init( HWND hWnd, bool subclassWnd = true ) = 0;
	//  close & cleanup
	virtual void __stdcall ShutDown() = 0;

	//  -- Rendering----------------------------------------------
	//  sets current observer view matrix
	virtual void __stdcall SetViewMatrix( const Matrix4D& vmatr ) = 0;
	//  sets current camera projection matrix
	virtual void __stdcall SetProjectionMatrix( const Matrix4D& pmatr ) = 0;
	//  sets current model/world transformation
	virtual void __stdcall SetWorldMatrix( const Matrix4D& wmatr ) = 0;
	//  sets current texture transform for given stage
	virtual void __stdcall SetTextureMatrix( const Matrix4D& tmatr, int stage = 0 ) = 0;
	
	virtual const Matrix4D& __stdcall GetWorldMatrix() = 0;
	

	virtual void __stdcall GetClientSize( int& width, int& height ) = 0;

	//  clears backbuffer 
	virtual void __stdcall ClearDeviceTarget( DWORD color = 0x00000000 ) = 0;
	virtual void __stdcall ClearDeviceZBuffer() = 0;
	virtual void __stdcall ClearDeviceStencil() = 0;

	virtual void __stdcall PresentBackBuffer( const RECT* rect = 0 ) = 0;
	
	
	virtual void __stdcall SetCursorBitmap( const char* bmpName ) = 0;
	virtual void __stdcall SetCursorPosition( int x, int y, bool drawNow = false ) = 0;
	
	//  begin/end scene
	virtual void __stdcall BeginScene() = 0;
	virtual void __stdcall EndScene()	= 0;
	virtual void __stdcall OnFrame() = 0;

	virtual void __stdcall Draw( BaseMesh& bm ) = 0;
	virtual void __stdcall Draw( BaseMesh& bm, int firstPoly, int numPoly, 
									int firstVert, int numVert ) = 0;
	virtual void __stdcall DrawPrim( BaseMesh& bm, 
									int firstPoly, int numPoly, 
									int firstVert, int numVert ) = 0;
	virtual void __stdcall DrawPrim( BaseMesh& bm ) = 0;

	virtual bool __stdcall RecompileAllShaders() = 0;
	virtual bool __stdcall ReloadAllTextures() = 0;

	virtual bool __stdcall SetScreenProperties( const ScreenProp& prop ) = 0;	
	virtual ScreenProp __stdcall GetScreenProperties() = 0;	

	
	//  -- Texture management -----------------------------------------
	//  textures at the render system are identified by the
	//  unique integer ID, in GL style.
	//	All operations by the clients are performed through 
	//	this ID.
	
	virtual void __stdcall SetTexture( int texID, int stage = 0 ) = 0;
	virtual int __stdcall GetTexture( int stage = 0 ) = 0;

	//  saves texture to the file
	virtual void __stdcall SaveTexture( int texID, const char* fname ) = 0;
	
	//  copies one texture to another
	virtual void __stdcall CopyTexture( int destID, int srcID ) = 0;

	//  copies one texture to another
	virtual void __stdcall CopyTexture( int destID, int srcID, 
										RECT* rectList, int nRect, POINT* ptList = 0 ) = 0;

	virtual int	 __stdcall CreateTexture( const char* texName, const TextureDescr& td, 
											TextureClient* client = 0 ) = 0;
	virtual int	 __stdcall GetTextureID( const char* texName, const TextureDescr& td ) = 0;
	virtual int	 __stdcall GetTextureID( const char* texName )	= 0;
	
	virtual int	 __stdcall GetShaderID( const char* shaderName ) = 0;

	//  obtain pointer to the texture pixel data
	virtual BYTE* __stdcall LockTexBits( int texID, int& pitch, int level = 0 ) = 0;
	//  unlock texture pixel data 
	virtual void __stdcall UnlockTexBits( int texID, int level = 0 )	= 0;

	virtual int	__stdcall GetTextureSizeBytes( int texID ) = 0;

	virtual void __stdcall SetRenderTarget( int texID ) = 0;
	virtual void __stdcall SetViewPort( const ViewPort& vp ) = 0;
	virtual int	 __stdcall GetVBufferID() = 0;

	virtual void __stdcall SetCurrentShader( int shaderID ) = 0;
	virtual bool __stdcall IsShaderValid( const char* shaderName ) = 0;
	virtual int  __stdcall EstimateTexMemSizeMB() = 0;

	virtual void __stdcall SetTextureFactor( DWORD tfactor ) = 0;

	// -- Debugging ----------------------------------------------------
	virtual void __stdcall Dump( const char* fname = 0 )	= 0;
	
	virtual void* __stdcall DbgGetDevice() = 0;
	virtual void __stdcall dbgDumpShader( int shaderID, FILE* fp ) = 0;
	
}; // class IRenderSystem

class RC
{
public:
	static IRenderSystem*	iRS;
	static DWORD			curFrame;



	static void OnFrame() { curFrame++; }
}; // class RC

#endif // __RENDERSYSTEM_H__