/*****************************************************************
/*  File:   rsRenderSystemDX8.cpp                                *
/*  Desc:   Direct3D rendering system implementation             *
/*  Author: Silver, Copyright (C) GSC Game World                 *
/*  Date:   Feb 2002                                             *
/*****************************************************************/
#include "stdafx.h"

#ifndef _DX9

#include "BugSlayer.h"
#include "direct.h"
#include "Log.h" 
#include "IRenderSystem.h"

#include "rsRenderSystemDX.h"

#include "Statistics.h"

#include "IniFile.h"
#include "StrUtil.h"

ResKeeper<IDirect3DTexture8>			RenderSystemD3D::texKeeper;
map<string, int>						RenderSystemD3D::texReg;

IDirect3D8*								RenderSystemD3D::pD3D;
IDirect3DDevice8*						RenderSystemD3D::pDevice;

ScreenProp								RenderSystemD3D::screenProp;

//  O== PUBLIC INTERFACE ========================================O
RenderSystemD3D::RenderSystemD3D() 
{
	renderTarget	= 0;
	curShader		= 0;

	curWorldMatr.setIdentity();

	defaultTD.setValues( 256, 256, cfARGB8888, mpVRAM, 0 ); 
	sBack=NULL;
}

RenderSystemD3D::~RenderSystemD3D()
{
}

void __stdcall 	
RenderSystemD3D::SetViewMatrix( const Matrix4D& vmatr )
{
	DX_CHK( pDevice->SetTransform( D3DTS_VIEW, (D3DMATRIX*)&vmatr ) );
}
	
void __stdcall 	
RenderSystemD3D::SetProjectionMatrix( const Matrix4D& pmatr )
{
	DX_CHK( pDevice->SetTransform( D3DTS_PROJECTION, (D3DMATRIX*)&pmatr ) );
}
	
void __stdcall 		
RenderSystemD3D::SetWorldMatrix( const Matrix4D& wmatr )
{
	curWorldMatr = wmatr;
	DBG( Stats::beginEvent( ST_WMATR ) );
	DX_CHK( pDevice->SetTransform( D3DTS_WORLD, (D3DMATRIX*)&wmatr ) );
	DBG( Stats::endEvent( ST_WMATR ) );
}

void __stdcall 
RenderSystemD3D::SetTextureMatrix( const Matrix4D& tmatr, int stage )
{
	assert( stage >= 0 && stage < 8 );
	DX_CHK( pDevice->SetTransform( (D3DTRANSFORMSTATETYPE)(D3DTS_TEXTURE0 + stage), 
									(D3DMATRIX*)&tmatr ) );
}

const Matrix4D&
RenderSystemD3D::GetWorldMatrix()
{
	return curWorldMatr;
}
	
void __stdcall 		
RenderSystemD3D::ClearDeviceTarget( DWORD color )
{
	DX_CHK( pDevice->Clear( 0, 
							NULL, 
							D3DCLEAR_TARGET, 
							color, 
							1.0, 
							0 ) );
}

void __stdcall 		
RenderSystemD3D::ClearDeviceZBuffer()
{
	DX_CHK( pDevice->Clear( 0, 
							NULL, 
							D3DCLEAR_ZBUFFER, 
							0, 
							1.0, 
							0 ) );
}

void __stdcall 		
RenderSystemD3D::ClearDeviceStencil()
{
	DX_CHK( pDevice->Clear( 0, 
							NULL, 
							D3DCLEAR_STENCIL, 
							0, 
							1.0, 
							0 ) );
}
	
void __stdcall 		
RenderSystemD3D::SwapBuffers()
{
	DX_CHK( pDevice->Present( NULL, NULL, NULL, NULL ) );
}
	
void __stdcall 		
RenderSystemD3D::BeginScene()
{
	DX_CHK( pDevice->BeginScene() );
}

void __stdcall 		
RenderSystemD3D::EndScene()
{

	DX_CHK( pDevice->EndScene() );
}

void __stdcall 		
RenderSystemD3D::OnFrame()
{
	SwapBuffers();
	Stats::onFrame();
}

int __stdcall RenderSystemD3D::GetVBufferID()
{
	if (vbufID != -1) return vbufID;
	//  create vbuffer texture
	TextureDescr vbTD;
	vbTD.setValues( screenProp.width,screenProp.width/* screenProp.height*/, screenProp.colorFormat, 
					mpVRAM, 1, tuRenderTarget );
	vbufID = CreateTexture( vbTD );
	assert( vbufID != -1 );
	return vbufID;
} // RenderSystemD3D::GetVBufferID

void __stdcall 
RenderSystemD3D::DrawPrim( BaseMesh& bm, int firstPoly, int numPoly, 
							int firstVert, int numVert )
{
	BeginScene();
	priBufPool.Draw( bm, firstPoly, numPoly, firstVert, numVert );
	EndScene();
} // RenderSystemD3D::DrawPrim

bool __stdcall RenderSystemD3D::HasCapability( DeviceCapability cap )
{
	D3DCAPS8 d3dCaps;
    pDevice->GetDeviceCaps( &d3dCaps );

	switch (cap)
	{
	case dcIndexedVertexBlending:
		return (d3dCaps.MaxVertexBlendMatrixIndex >= 255);
	case dcBumpEnvMap:
		return ((d3dCaps.TextureOpCaps & D3DTEXOPCAPS_BUMPENVMAP) != 0);
	case dcBumpEnvMapLuminance:
		return ((d3dCaps.TextureOpCaps & D3DTEXOPCAPS_BUMPENVMAPLUMINANCE) != 0);
	default:
		return false;
	}
} // RenderSystemD3D::HasCapability

void __stdcall 
RenderSystemD3D::DrawPrim( BaseMesh& bm )
{
	BeginScene();
	priBufPool.Draw( bm );
	EndScene();
}

/*---------------------------------------------------------------------------*/
/*	Func:	RenderSystemD3D::Draw
/*	Desc:	Draws mesh part
/*---------------------------------------------------------------------------*/
void __stdcall 
RenderSystemD3D::Draw(	BaseMesh& bm, 
						int firstPoly, int numPoly,
						int firstVert, int numVert )
{
	BeginScene();
	SetTexture( bm.getTexture( 0 ), 0 );
	SetTexture( bm.getTexture( 1 ), 1 );
	SetCurrentShader( bm.getShader() );

	priBufPool.Draw( bm, firstPoly, numPoly, firstVert, numVert );

	EndScene();
} // RenderSystemD3D::Draw
	
void __stdcall 		
RenderSystemD3D::Draw( BaseMesh& bm )
{
	BeginScene();
	SetTexture( bm.getTexture( 0 ), 0 );
	SetTexture( bm.getTexture( 1 ), 1 );
	SetCurrentShader( bm.getShader() );

	priBufPool.Draw( bm );
	EndScene();
}

void __stdcall 
RenderSystemD3D::CopyTexture( int destID, int srcID )
{
	DBG( Stats::beginEvent( ST_TEX ) );

	TextureCacheItem* tci = texCache.GetItem( destID );
	int realID = 0; 
	if (!tci) Log.Error( "No texture with ID: %d", destID );
	IDirect3DTexture8* dest = texKeeper.GetEl( tci->resID );
	tci = texCache.GetItem( srcID );
	if (!tci) Log.Error( "No texture with ID: %d", srcID );
	IDirect3DTexture8* src = texKeeper.GetEl( tci->resID );
	
	DX_CHK( pDevice->UpdateTexture( src, dest ) );

	DBG( Stats::endEvent( ST_TEX ) );
}  // RenderSystemD3D::CopyTexture

void __stdcall 
RenderSystemD3D::CopyTexture( int destID, int srcID, 
							 RECT* rectList, int nRect, POINT* ptList )
{
	DBG( Stats::beginEvent( ST_TEX ) );

	int realID = 0; 
	IDirect3DTexture8* dest;
	TextureCacheItem* tci;
	if (destID != 0) 
	{
		tci = texCache.GetItem( destID );	
		if (!tci) Log.Error( "No texture with ID: %d", destID );
		dest = texKeeper.GetEl( tci->resID );
	}

	tci = texCache.GetItem( srcID );
	if (!tci) Log.Error( "No texture with ID: %d", srcID );
	IDirect3DTexture8* src = texKeeper.GetEl( tci->resID );
	
	IDirect3DSurface8 *sDest, *sSrc;
	if (destID != 0) 
	{
		DX_CHK( dest->GetSurfaceLevel( 0, &sDest ) );
	}
	else sDest = sBack;
	DX_CHK( src->GetSurfaceLevel( 0, &sSrc ) );
	DX_CHK( pDevice->CopyRects( sSrc, rectList, nRect, sDest, ptList ) );
	DBG( Stats::endEvent( ST_TEX ) );
} // RenderSystemD3D::CopyTexture

int __stdcall 
RenderSystemD3D::GetTexture( int stage )
{
	assert( stage >= 0 && stage < c_MaxTextureStages );
	return curTex[stage];
}  // RenderSystemD3D::GetTexture


void __stdcall 	
RenderSystemD3D::SetTexture( int texID, int stage )
{
	if (curTex[stage] == texID || texID == -1) return;

	DBG( Stats::beginEvent( ST_TEX ) );

	TextureCacheItem* tci = texCache.GetItem( texID );
	int realID = 0; 
	if (tci) 
	{
		tci->LRU = GetTickCount();
	}
	else 
	{
		DX_CHK( pDevice->SetTexture( stage, 0 ) );
		curTex[stage] = texID;
		return;
	}

	
	IDirect3DTexture8* texd3d = texKeeper.GetEl( tci->resID );
	DX_CHK( pDevice->SetTexture( stage, texd3d ) );
	curTex[stage] = texID;

	DBG( Stats::endEvent( ST_TEX ) );
} // RenderSystemD3D::SetTexture
	
int __stdcall 	
RenderSystemD3D::LoadTexture( const TextureDescr& td, const char* texFileName )
{
	char fname[_MAX_PATH];
	strcpy( fname, rootDirectory );
	strcat( fname, "\\textures\\" );
	strcat( fname, texFileName );

	IDirect3DTexture8 *texLoad = 0, *texd3d = 0;
	D3DXCreateTextureFromFileEx(	pDevice, 
									fname,				//  file name
									D3DX_DEFAULT,		//  width
									D3DX_DEFAULT,		//  height
									td.getNMips(),		//  number of mip levels
									_InterpretTexUsage( td.getTexUsage() ),  
														//  texture usage
									_InterpretColFmt( td.getColFmt() ),		 
														//  color format
									_InterpretMemPool( td.getMemPool() ),	 
														//  memory pool
									D3DX_DEFAULT ,		//  image filter 
									D3DX_DEFAULT ,		//  mip filter
									0,					//  disable color key
									NULL,				//  don't care about image info
									NULL,				//  no palette
									&texLoad			//  put here texture, please
								);

	if (!texLoad) 
	{
		Log.Warning( "Cannot load texture %s", texFileName );
		return 0;
	}

	D3DSURFACE_DESC desc;
	texLoad->GetLevelDesc( 0, &desc );

	TextureCacheItem& tci = texCache.AddTextureItem( td );
	
	tci.resID = texKeeper.CreateID();
	texKeeper.SetEl( tci.resID, texFileName, texLoad );
	texReg[texFileName] = tci.resID;
	return tci.itemID;
} // RenderSystemD3D::LoadTexture

void __stdcall 	 
RenderSystemD3D::SaveTexture( int texID, const char* fname )
{
	IDirect3DTexture8* texd3d = texKeeper.GetEl( texID );
	if (texd3d == 0) return;

	char ext[64];
	ParseExtension( fname, ext );

	D3DXIMAGE_FILEFORMAT type = D3DXIFF_DDS;
	if (!strcmp( ext, "bmp" )) type = D3DXIFF_BMP;
	if (!strcmp( ext, "tga" )) type = D3DXIFF_TGA;
	if (!strcmp( ext, "dds" )) type = D3DXIFF_DDS;
	
	D3DXSaveTextureToFile( fname, type, texd3d, 0 );
}

void __stdcall 	
RenderSystemD3D::UnloadTexture( int texID )
{
	IDirect3DTexture8* texd3d = texKeeper.GetEl( texID );	
	texd3d->Release();
	texKeeper.FreeID( texID );
} // RenderSystemD3D::UnloadTexture

int
RenderSystemD3D::_CreateTexture( const TextureDescr& td )
{	
	IDirect3DTexture8* texd3d = 0;
	if (td.getColFmt() == cfBackBufferCompatible)
	{
		pDevice->CreateTexture( td.getSideX(), td.getSideY(), 
										td.getNMips(),
										_InterpretTexUsage	( td.getTexUsage()	),			
										_InterpretColFmt	( screenProp.colorFormat	), 
										_InterpretMemPool	( td.getMemPool()	), 
										&texd3d );
	}
	else
	{
		pDevice->CreateTexture( td.getSideX(), td.getSideY(), 
										td.getNMips(),
										_InterpretTexUsage	( td.getTexUsage()	),			
										_InterpretColFmt	( td.getColFmt()	), 
										_InterpretMemPool	( td.getMemPool()	), 
										&texd3d );
	}

	if (!texd3d) 
	{
		Log.Warning( "Cannot create texture: " );
		//td.dump();
		return 0;
	}

	int texID = texKeeper.CreateID();
	texKeeper.SetEl( texID, "created", texd3d );

	return texID;

} // RenderSystemD3D::_CreateTexture

int __stdcall 		
RenderSystemD3D::GetTextureSizeBytes( int texID )
{
	TextureCacheItem* tci = texCache.GetItem( texID );
	if (!tci) return 0;

	IDirect3DTexture8* texd3d = texKeeper.GetEl( tci->resID );
	if (!texd3d)
	{
		Log.Warning( "No texture in the cache.", texID );
		return 0; 
	}

	D3DSURFACE_DESC desc;
	texd3d->GetLevelDesc( 0, &desc );
	return desc.Size;
}  // RenderSystemD3D::GetTextureSizeBytes


BYTE*  __stdcall 	
RenderSystemD3D::LockTexBits( int texID, int& pitch )
{
	D3DLOCKED_RECT rct;
	TextureCacheItem* tci = texCache.GetItem( texID );
	if (!tci) return 0;

	IDirect3DTexture8* texd3d = texKeeper.GetEl( tci->resID );
	if (!texd3d)
	{
		Log.Warning( "Couldn't lock texture with ID %d. No such texture.", texID );
		return 0; 
	}
	DX_CHK( texd3d->LockRect( 0, &rct, 0, 0 ) );
	pitch = rct.Pitch;
	return (BYTE*)(rct.pBits);
}  // RenderSystemD3D::LockTexBits

void __stdcall 	
RenderSystemD3D::UnlockTexBits( int texID )
{
	TextureCacheItem* tci = texCache.GetItem( texID );
	if (!tci) return;

	IDirect3DTexture8* texd3d = texKeeper.GetEl( tci->resID );
	DX_CHK( texd3d->UnlockRect( 0 ) );
} // RenderSystemD3D::UnlockTexBits


void __stdcall 	
RenderSystemD3D::ReserveTextureSpace( const TextureDescr& td, int numTex )
{
	for (int i = 0; i < numTex; i++)
	{
		TextureCacheItem& tci = texCache.AddTextureItem( td );
		tci.resID = _CreateTexture( td );
	}
} // RenderSystemD3D::ReserveTextureSpace


int __stdcall 		 
RenderSystemD3D::ObtainTexture( const TextureDescr& td )
{
	int tid = texCache.AllocTextureItem( td );
	if (tid == -1)
	{
		TextureCacheItem& tci = texCache.AddTextureItem( td );
		tci.resID = _CreateTexture( td );
		tid = tci.itemID;
	}
	return tid;
} // RenderSystemD3D::ObtainTexture

int __stdcall 		 
RenderSystemD3D::CreateTexture( const TextureDescr& td )
{
	TextureCacheItem& tci = texCache.AddTextureItem( td, false );
	tci.resID = _CreateTexture( td );
	return tci.itemID;
}  // RenderSystemD3D::CreateTexture

void __stdcall 	 
RenderSystemD3D::GetClientSize( int& width, int& height )
{
	width = wndWidth;
	height = wndHeight;
}  // RenderSystemD3D::GetClientSize

void __stdcall 	
RenderSystemD3D::SetViewPort( const ViewPort& vp )
{
	pDevice->SetViewport( (D3DVIEWPORT8*)&vp );	
	curViewPort = vp;
}

void __stdcall 	
RenderSystemD3D::SetRenderTarget( int texID )
{
	renderTarget = texID;
	
	if (renderTarget == 0)
	{
		DX_CHK( pDevice->SetRenderTarget( sBack, sDepth ) );
		return;
	}

	IDirect3DTexture8* texd3d = texKeeper.GetEl( renderTarget );
	if (texd3d == 0) return;
	texd3d->GetSurfaceLevel( 0, &texSurf );
	DX_CHK( pDevice->SetRenderTarget( texSurf, 0 ) );
	
}

void __stdcall 	  
RenderSystemD3D::SetDebugParam( void* p1, void* p2 )
{
	pD3D = (IDirect3D8*) p1;
	pDevice = (IDirect3DDevice8*) p2;
}

void* __stdcall 
RenderSystemD3D::DbgGetDevice()
{
	return (void*)pDevice;
}
	

void __stdcall 		
RenderSystemD3D::Init( HWND hWnd, bool subclassWnd )
{
	IniFile inif( "settings.cfg" );
	if (!inif.Error()) settings.FillFromIniFile( inif );

	getcwd( rootDirectory, _MAX_PATH );

	wndHandle = hWnd;

	Log.Message( "Initializing D3D Render System..." );
	CHECKM( hWnd != 0, "D3D Render System - No window handle." );

	// Create the D3D object.
    pD3D = Direct3DCreate8( D3D_SDK_VERSION );
	CHECKM( pD3D != NULL, "Failed to create DirectX device" );

	D3DCAPS8 d3dCaps;
	pD3D->GetDeviceCaps( 0, D3DDEVTYPE_HAL, &d3dCaps );  
	
	_FillDevCaps( d3dCaps, deviceCaps );
	_LogDevCaps( deviceCaps );

    // Get the current desktop display mode, so we can set up a back
    // buffer of the same format
    D3DDISPLAYMODE d3ddm;
    DX_CHK( pD3D->GetAdapterDisplayMode( D3DADAPTER_DEFAULT, &d3ddm ) );

    D3DPRESENT_PARAMETERS d3dpp;
    ZeroMemory( &d3dpp, sizeof(d3dpp) );
	
	BOOL bWindowed = (settings.fullScreen == false);

    d3dpp.Windowed				 = bWindowed;
    d3dpp.SwapEffect			 = D3DSWAPEFFECT_COPY;
	d3dpp.EnableAutoDepthStencil = TRUE;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
    d3dpp.BackBufferFormat		 = d3ddm.Format;
    d3dpp.BackBufferCount        = 1;
    d3dpp.hDeviceWindow          = hWnd;
    
    RECT rctClient;
    GetClientRect( hWnd, &rctClient );

    if (bWindowed)
    {
        wndWidth	= rctClient.right  - rctClient.left;
        wndHeight	= rctClient.bottom - rctClient.top;
    }
    else
    {
        wndWidth	= settings.screenWidth;
        wndHeight	= settings.screenHeight;

		//  set display fullscreen mode
		DEVMODE devMode;
		ZeroMemory( &devMode, sizeof( devMode ) );
		devMode.dmSize          = sizeof( DEVMODE );
		devMode.dmPelsWidth     = wndWidth;
		devMode.dmPelsHeight    = wndHeight;
		devMode.dmBitsPerPel    = 24;
		devMode.dmFields        = DM_PELSWIDTH | DM_PELSHEIGHT;
		int devModeRes = ChangeDisplaySettings( &devMode, CDS_FULLSCREEN );
	
    }

	screenProp.width		= wndWidth;
	screenProp.height		= wndHeight;
	screenProp.colorFormat	= _InterpretColFmt( d3dpp.BackBufferFormat );

	d3dpp.BackBufferWidth  = wndWidth;
    d3dpp.BackBufferHeight = wndHeight;
	//d3dpp.Flags|=D3DPRESENTFLAG_LOCKABLE_BACKBUFFER;//by Drew

	DWORD flags = 0;

	if (settings.hardwareTnL) flags |= D3DCREATE_HARDWARE_VERTEXPROCESSING;
		else flags |= D3DCREATE_SOFTWARE_VERTEXPROCESSING;

	if (settings.pureDevice) flags |= D3DCREATE_PUREDEVICE;
	
	D3DDEVTYPE devType = settings.softwareRendering ? D3DDEVTYPE_REF : D3DDEVTYPE_HAL;

    // Create the D3DDevice
    DX_CHK( pD3D->CreateDevice( settings.adapterOrdinal, 
								devType, 
								hWnd,
                                flags,
                                &d3dpp, 
								&pDevice ) );

	DX_CHK( pDevice->ShowCursor( TRUE ) );


	_Init();
	DX_CHK(pDevice->GetBackBuffer(0,D3DBACKBUFFER_TYPE_MONO,&sBack));
	DX_CHK(pDevice->GetDepthStencilSurface(&sDepth));

	shaderCache.Init( pDevice );
	priBufPool.Init( pDevice );

	vbufID = -1;
} // RenderSystemD3D::Init

void __stdcall 
RenderSystemD3D::SetRootDir( const char* rootDir )
{
	strcpy( rootDirectory, rootDir );
} // RenderSystemD3D::SetRootDir

void 	
RenderSystemD3D::_Init()
{
	if (!pDevice || !pD3D)
	{
		Log.Error( "Render system init failed - no render device." );
		return;
	}
	
	defShader = GetShaderID( "Default" );
	SetCurrentShader( defShader );

	for (int i = 0; i < 8; i++)
	{
		curTex[i] = 0;
	}

	defaultTD.setNMips( 0 );
} // RenderSystemD3D::_Init

void __stdcall 		
RenderSystemD3D::ResizeWindow( int newW, int newH )
{
	Log.Warning( "RenderSystemD3D::ResizeWindow - still TODO..." );
}

void __stdcall 	 RenderSystemD3D::ShutDown()
{
	sBack->Release();
	sDepth->Release();

	SetTexture( 0 );
	SetCurrentShader( 0 );

	priBufPool.Shut();

	for (int i = 1; i < c_MaxPoolSize; i++)
	{
		IDirect3DTexture8* tex = texKeeper.GetEl( i );
		if (tex) tex->Release();
	}

	if (pDevice)
	{
		DX_CHK( pDevice->Release() );
		pDevice = 0;
	}

    if (pD3D)
	{
		DX_CHK( pD3D->Release() );
		pD3D = 0;
	}

}//  RenderSystemD3D::_ShutDown

void fpmatr( FILE* fp, D3DMATRIX& matr )
{
	fprintf( fp, "\n%8.5f %8.5f %8.5f %8.5f", 
					matr._11, matr._12, matr._13, matr._14 );	
	fprintf( fp, "\n%8.5f %8.5f %8.5f %8.5f", 
					matr._21, matr._22, matr._23, matr._24 );	
	fprintf( fp, "\n%8.5f %8.5f %8.5f %8.5f", 
					matr._31, matr._32, matr._33, matr._34 );	
	fprintf( fp, "\n%8.5f %8.5f %8.5f %8.5f", 
					matr._41, matr._42, matr._43, matr._44 );	
}

void __stdcall 	
RenderSystemD3D::Dump( const char* fname )
{
	FILE* fp = fname ? fopen( fname, "wt" ) : fopen( "c:\\devicedump.txt", "wt" );
	if (!fp) return;

	shaderCache.Dump( fname );
	//return;

	Shader sha;
	sha.CreateFromCurrentDeviceState( pDevice );
	sha.Dump( fp );

	fprintf( fp, "\nTransforms:\n" );
	D3DMATRIX matr;
	pDevice->GetTransform( D3DTS_VIEW, &matr );
	fprintf( fp, "\n\nView, Det=%f:\n", D3DXMatrixfDeterminant( (D3DXMATRIX*)&matr ) );
	fpmatr( fp, matr );
	pDevice->GetTransform( D3DTS_PROJECTION, &matr );
	fprintf( fp, "\n\nProjection, Det=%f:", D3DXMatrixfDeterminant( (D3DXMATRIX*)&matr ) );
	fpmatr( fp, matr );
	fprintf( fp, "\n\nWorld: Det=%f\n", D3DXMatrixfDeterminant( (D3DXMATRIX*)&matr ) );
	pDevice->GetTransform( D3DTS_WORLD, &matr );
	fpmatr( fp, matr );

	// texture matrices
	fprintf( fp, "\n\nTexture0:\n" );
	pDevice->GetTransform( D3DTS_TEXTURE0, &matr );
	fpmatr( fp, matr );
	fprintf( fp, "\n\nTexture1:\n" );
	pDevice->GetTransform( D3DTS_TEXTURE1, &matr );
	fpmatr( fp, matr );
	fprintf( fp, "\n\nTexture2:\n" );
	pDevice->GetTransform( D3DTS_TEXTURE2, &matr );
	fpmatr( fp, matr );
	fprintf( fp, "\n\nTexture3:\n" );
	pDevice->GetTransform( D3DTS_TEXTURE3, &matr );
	fpmatr( fp, matr );
	fprintf( fp, "\n\nTexture4:\n" );
	pDevice->GetTransform( D3DTS_TEXTURE4, &matr );
	fpmatr( fp, matr );
	fprintf( fp, "\n\nTexture5:\n" );
	pDevice->GetTransform( D3DTS_TEXTURE5, &matr );
	fpmatr( fp, matr );
	fprintf( fp, "\n\nTexture6:\n" );
	pDevice->GetTransform( D3DTS_TEXTURE6, &matr );
	fpmatr( fp, matr );
	fprintf( fp, "\n\nTexture7:\n" );
	pDevice->GetTransform( D3DTS_TEXTURE7, &matr );
	fpmatr( fp, matr );

	D3DVIEWPORT8 vp;
	pDevice->GetViewport( &vp );
	fprintf( fp, "\nViewport: x=%f; y=%f; w=%f; h =%f; minz=%f; maxz=%f\n",
				vp.X, vp.Y, vp.Width, vp.Height, vp.MinZ, vp.MaxZ );
	IDirect3DSurface8* rt;
	pDevice->GetRenderTarget( &rt );
	fprintf( fp, "\nRender target: %X\n", rt );

	fclose( fp );
}

void __stdcall 	 
RenderSystemD3D::SetCurrentShader( int shaderID )
{
	bool res = shaderCache.SetShader( shaderID, pDevice ); 
	
	if (res)
	{
		DBG( Stats::incValue( ST_SHADER, 1 ) );
	}

	return;
} // RenderSystemD3D::SetCurrentShader

int __stdcall 		 
RenderSystemD3D::GetTextureID( const char* texName )
{
	map<string, int>::iterator it = texReg.find( texName );
	if (it != texReg.end())
	{
		//  texture is already loaded
		return (*it).second;
	}
	else
	{
		//  no such texture - so load it
		int texID = LoadTexture( defaultTD, texName );
		texReg[texName] = texID;
		return texID;
	}
}

int	 __stdcall 
RenderSystemD3D::LoadTexture( const char* texName, const TextureDescr& texDescr )
{
	map<string, int>::iterator it = texReg.find( texName );
	if (it != texReg.end())
	{
		//  texture is already loaded
		return (*it).second;
	}
	else
	{
		//  no such texture - so load it
		int texID = LoadTexture( texDescr, texName );
		texReg[texName] = texID;
		return texID;
	}
} // RenderSystemD3D::LoadTexture

bool __stdcall 
RenderSystemD3D::IsShaderValid( const char* shaderName )
{
	//  create file name
	char fname[_MAX_PATH];
	sprintf( fname, "%s\\shaders\\%s.sha", rootDirectory, shaderName );

	ID3DXEffect* eff;
	ID3DXBuffer* compileErr;
	HRESULT hres = D3DXCreateEffectFromFile( pDevice, fname, &eff, &compileErr );
	if (hres != S_OK) 
	{
		if (!compileErr)
		{
			Log.Warning( "Could not load shader file %s", fname );
			return false;
		}
		else
		{
			char* errbuf = (char*)compileErr->GetBufferPointer();
			Log.Warning( "Could not compile shader %s. Error message: %s", 
							shaderName, errbuf );
			compileErr->Release();
			return false;
		}
	}
	hres = eff->SetTechnique( "T0" );
	bool valid = true;
	if (hres != S_OK) valid = false;
	//  Checking if device is capable to set this shader
	if (eff->Validate() != S_OK) valid = false;
	eff->Release();
	return valid;
} // RenderSystemD3D::IsShaderValid

int __stdcall 		 
RenderSystemD3D::GetShaderID( const char* shaderName )
{
	return shaderCache.GetShaderID( shaderName, pDevice );
} // RenderSystemD3D::GetShaderID

bool __stdcall 
RenderSystemD3D::RecompileAllShaders()
{	
	//shaderCache.Dump( "c:\\dumps\\shaderCacheDump.txt" );
	shaderCache.ReloadShaders( pDevice );
	return true;
} // RenderSystemD3D::RecompileAllShaders

bool __stdcall 
RenderSystemD3D::ReloadAllTextures()
{
	bool allOK = true;
	map<string, int>::iterator it;
	for (it = texReg.begin(); it != texReg.end(); it++)
	{
		const char* texName = (*it).first.c_str();
		char fname[_MAX_PATH];
		strcpy( fname, rootDirectory );
		strcat( fname, "\\textures\\" );
		strcat( fname, texName );
			
		int texID = (*it).second;
		TextureCacheItem* tci = texCache.GetItem( texID );
		if (!tci) continue; 
		int resID = (*it).second; 
		IDirect3DTexture8 *tex = texKeeper.GetEl( tci->resID );
		if (!tex) continue;
		//tex->Release();
		
	}
	curTex[0] = 0;
	curTex[1] = 0;
	return allOK;
}

bool __stdcall 
RenderSystemD3D::RecompileShader( int shID )
{
	assert( false );
	return false;
}

bool __stdcall 
RenderSystemD3D::ReloadTexture( int texID )
{
	assert( false );
	return false;
}

//  O== INTERNAL METHODS ========================================O
DWORD 
RenderSystemD3D::_CreateFVF( VertexFormat	vertFmt )
{
	switch (vertFmt)
	{
	case vf2Tex:	return D3DFVF_XYZ		| D3DFVF_DIFFUSE | D3DFVF_TEX2; 
	case vfTnL:		return D3DFVF_XYZRHW	| D3DFVF_DIFFUSE | D3DFVF_SPECULAR | D3DFVF_TEX1;
	case vfTnL2:	return D3DFVF_XYZRHW	| D3DFVF_DIFFUSE | D3DFVF_TEX2;
	case vfN:		return D3DFVF_XYZ		| D3DFVF_NORMAL	 | D3DFVF_TEX1;
	case vfTnL2S:	return D3DFVF_XYZRHW	| D3DFVF_DIFFUSE |D3DFVF_SPECULAR | D3DFVF_TEX2;
	case vfT:		return D3DFVF_XYZ		| D3DFVF_TEX1;		
	case vfMP1:		return D3DFVF_XYZB1		| D3DFVF_LASTBETA_UBYTE4 | D3DFVF_TEX2;	
	case vfNMP1:	return D3DFVF_XYZB1 | D3DFVF_LASTBETA_UBYTE4 | D3DFVF_NORMAL | D3DFVF_TEX2;		
	case vfNMP2:	return D3DFVF_XYZB2 | D3DFVF_LASTBETA_UBYTE4 | D3DFVF_NORMAL | D3DFVF_TEX2;		
	case vfNMP3:	return D3DFVF_XYZB3 | D3DFVF_LASTBETA_UBYTE4 | D3DFVF_NORMAL | D3DFVF_TEX2;		
	case vfNMP4:	return D3DFVF_XYZB4 | D3DFVF_LASTBETA_UBYTE4 | D3DFVF_NORMAL | D3DFVF_TEX2;		
	default: return D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX2; 
	
	}
	return D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX2;
} // RenderSystemD3D::_CreateFVF

D3DFORMAT
RenderSystemD3D::_InterpretColFmt( ColorFormat colFmt )
{
	D3DFORMAT d3dcf = D3DFMT_A4R4G4B4;
	switch (colFmt)
	{
	case cfARGB4444: d3dcf = D3DFMT_A4R4G4B4;	break;
	case cfXRGB1555: d3dcf = D3DFMT_X1R5G5B5;	break;
	case cfARGB8888: d3dcf = D3DFMT_A8R8G8B8;	break;
	case cfXRGB8888: d3dcf = D3DFMT_X8R8G8B8;	break;
	case cfRGB565:   d3dcf = D3DFMT_R5G6B5;	    break;
	case cfV8U8:     d3dcf = D3DFMT_V8U8;	    break;
	}
	return d3dcf;
} // RenderSystemD3D::_InterpretColFmt

ColorFormat 
RenderSystemD3D::_InterpretColFmt( D3DFORMAT	colFmt )
{
	ColorFormat res = cfARGB4444;
	switch (colFmt)
	{
	case D3DFMT_A4R4G4B4: res = cfARGB4444;	break;
	case D3DFMT_X1R5G5B5: res = cfXRGB1555;	break;
	case D3DFMT_A8R8G8B8: res = cfARGB8888;	break;
	case D3DFMT_X8R8G8B8: res = cfXRGB8888;	break;
	case D3DFMT_R5G6B5:   res = cfRGB565;	break;
	case D3DFMT_V8U8:     res = cfV8U8;	    break;
	};
	return res;
} // RenderSystemD3D::_InterpretColFmt

D3DPOOL
RenderSystemD3D::_InterpretMemPool( MemoryPool memPool )
{
	D3DPOOL d3dpool = D3DPOOL_DEFAULT;
	switch (memPool)
	{
	case mpVRAM:	d3dpool = D3DPOOL_DEFAULT;		break;
	case mpSysMem:	d3dpool = D3DPOOL_SYSTEMMEM;	break;
	case mpDual:	d3dpool = D3DPOOL_MANAGED;		break;
	}
	return d3dpool;
}  // RenderSystemD3D::_InterpretMemPool

DWORD
RenderSystemD3D::_InterpretTexUsage( TextureUsage texUsage )
{
	switch (texUsage)
	{
	case tuRenderTarget: return D3DUSAGE_RENDERTARGET;
	}
	return 0;
}  // RenderSystemD3D::_InterpretMemPool

D3DPRIMITIVETYPE 
RenderSystemD3D::_InterpretPriType( PrimitiveType priType )
{
	D3DPRIMITIVETYPE d3dPri = D3DPT_POINTLIST;
	switch (priType)
	{
		case ptTriangleListPlain:
		case ptTriangleList:	d3dPri = D3DPT_TRIANGLELIST;	break;
		case ptTriangleStripPlain:
		case ptTriangleStrip:	d3dPri = D3DPT_TRIANGLESTRIP;	break;
		case ptTriangleFan:
		case ptTriangleFanPlain:
								d3dPri = D3DPT_TRIANGLEFAN;		break;
		case ptLineStrip:		d3dPri = D3DPT_LINESTRIP;		break;
		case ptLineList:		
		case ptLineListPlain:
		case ptLineStripPlain:
								d3dPri = D3DPT_LINELIST;		break;
		case ptPointList:		d3dPri = D3DPT_POINTLIST;		break;
	}
	return d3dPri;
}  // RenderSystemD3D::_InterpretMemPool


string BoolToStr( bool arg )
{
	if (arg) return "Yes";
		else return "None";
}

string IntToStr( int arg )
{
	char buf[64];
	sprintf( buf, "%d", arg );
	return string( buf );
}

void
RenderSystemD3D::_LogDevCaps( const DevCaps& caps )
{
	string str;
	str = "DevType: ";
	if (caps.devType == dtHAL) str += "HAL.";
		else str += "REF.";
	Log.Message( str.c_str() );
	
	str = "Adapter ordinal: ";
	str += IntToStr( caps.adapterOrdinal );
	Log.Message( str.c_str() );

	str = "Hardware Rasterization: ";
	str += BoolToStr( caps.hwRasterization );

	str = "Hardware T&L: ";
	str += BoolToStr( caps.hwTnL );

	if (caps.hwPure) Log.Message( "Pure device here. Wow." );
	else Log.Message( "No pure device support" );

	str = "Higher-order primitives. NPatches: ";
	str += BoolToStr( caps.hwNPatches );
	str += ". Bezier patches: ";
	str += BoolToStr( caps.hwBezier );
	str += ". RTPatches: ";
	str += BoolToStr( caps.hwRTPatches );
	str += ".";

	Log.Message( str.c_str() );

	str = "Multitexturing capacity: ";
	str += IntToStr( caps.texInSinglePass );
	Log.Message( str.c_str() );

	str = "Max textures in single pass: ";
	str += IntToStr( caps.texInSinglePass );

	Log.Message( "Available depth/stencil formats:" );
	if (caps.dsfD16)			Log.Message( " - depth16 " );
	if (caps.dsfD15S1)			Log.Message( " - depth15 stencil 1" );
	if (caps.dsfD24S8)			Log.Message( " - depth24 stencil 8" );
	if (caps.dsfD24X8)			Log.Message( " - depth24" );
	if (caps.dsfD32)			Log.Message( " - depth32" );
	if (caps.dsfD24X4S4)		Log.Message( " - depth24 stencil 4" );
	if (caps.dsfD16Lockable)	Log.Message( " - depth16 lockable" );

	Log.Message( "Available render-to-texture formats:" );
	if (caps.rttARGB4444)		Log.Message( " - argb4444 " );
	if (caps.rttRGB565)			Log.Message( " - rgb565" );
	if (caps.rttRGB888)			Log.Message( " - rgb888" );
	if (caps.rttA8)				Log.Message( " - a8" );
	if (caps.rttARGB1555)		Log.Message( " - argb1555" );
}

void 
RenderSystemD3D::_FillDevCaps( const D3DCAPS8& d3dCaps, DevCaps& caps )
{
	if (pD3D)
	{
		D3DADAPTER_IDENTIFIER8 id;
		pD3D->GetAdapterIdentifier( d3dCaps.AdapterOrdinal, 0, &id );
		strncpy( caps.devDescr,	id.Description, c_MaxDevCapsStr - 1 );
		strncpy( caps.devDriver, id.Driver, c_MaxDevCapsStr - 1 );

	}

	if (d3dCaps.DeviceType == D3DDEVTYPE_HAL) caps.devType = dtHAL;
		else caps.devType = dtREF;

	caps.adapterOrdinal = d3dCaps.AdapterOrdinal;

	if (d3dCaps.DevCaps & D3DDEVCAPS_NPATCHES) 
			caps.hwNPatches = true; else caps.hwNPatches = false;
	if (d3dCaps.DevCaps & D3DDEVCAPS_HWRASTERIZATION  ) 
			caps.hwRasterization = true; else caps.hwRasterization = false;
	if (d3dCaps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT ) 
			caps.hwTnL = true; else caps.hwTnL = false;
	if (d3dCaps.DevCaps & D3DDEVCAPS_PUREDEVICE) 
			caps.hwPure  = true; else caps.hwPure = false;
	if (d3dCaps.DevCaps & D3DDEVCAPS_QUINTICRTPATCHES) 
			caps.hwBezier = true; else caps.hwBezier = false;
	if (d3dCaps.DevCaps & D3DDEVCAPS_RTPATCHES ) 
			caps.hwRTPatches = true; else caps.hwRTPatches = false;

	caps.texBlendStages		= d3dCaps.MaxTextureBlendStages;
	caps.texInSinglePass		= d3dCaps.MaxSimultaneousTextures;

	caps.dsfD16			= _CheckDSAvail( D3DFMT_D16,			d3dCaps );
	caps.dsfD15S1		= _CheckDSAvail( D3DFMT_D15S1,			d3dCaps );
	caps.dsfD24X8		= _CheckDSAvail( D3DFMT_D24X8,			d3dCaps );
	caps.dsfD24S8		= _CheckDSAvail( D3DFMT_D24S8,			d3dCaps );
	caps.dsfD24X4S4		= _CheckDSAvail( D3DFMT_D24X4S4,		d3dCaps );
	caps.dsfD32			= _CheckDSAvail( D3DFMT_D32,			d3dCaps );
	caps.dsfD16Lockable	= _CheckDSAvail( D3DFMT_D16_LOCKABLE,	d3dCaps );

	caps.rttARGB4444	= _CheckRenderToTexAvail( D3DFMT_A4R4G4B4,	d3dCaps );
	caps.rttRGB565		= _CheckRenderToTexAvail( D3DFMT_R5G6B5,	d3dCaps );
	caps.rttRGB888		= _CheckRenderToTexAvail( D3DFMT_R8G8B8,	d3dCaps );
	caps.rttA8			= _CheckRenderToTexAvail( D3DFMT_A8,		d3dCaps );
	caps.rttARGB1555	= _CheckRenderToTexAvail( D3DFMT_A1R5G5B5,	d3dCaps );

} // RenderSystemD3D::_FillDevCaps

//  check if current device supports given depth stencil format
bool RenderSystemD3D::_CheckDSAvail( D3DFORMAT fmt, const D3DCAPS8& d3dCaps )
{
	if (!pD3D) return false;
	D3DDISPLAYMODE mode;
	pD3D->GetAdapterDisplayMode( d3dCaps.AdapterOrdinal, &mode );
	return (pD3D->CheckDeviceFormat( d3dCaps.AdapterOrdinal, 
									 D3DDEVTYPE_HAL, 
									 mode.Format, 
									 D3DUSAGE_DEPTHSTENCIL,
									 D3DRTYPE_SURFACE,
									 fmt
									 ) == D3D_OK);
} // RenderSystemD3D::_CheckDSAvail

bool RenderSystemD3D::_CheckRenderToTexAvail( D3DFORMAT fmt, const D3DCAPS8& d3dCaps )
{
	if (!pD3D) return false;
	D3DDISPLAYMODE mode;
	pD3D->GetAdapterDisplayMode( d3dCaps.AdapterOrdinal, &mode );
	return (pD3D->CheckDeviceFormat( d3dCaps.AdapterOrdinal, 
									 D3DDEVTYPE_HAL, 
									 mode.Format, 
									 D3DUSAGE_RENDERTARGET,
									 D3DRTYPE_TEXTURE,
									 fmt
									 ) == D3D_OK);
} // RenderSystemD3D::_CheckRenderToTexAvail

/*****************************************************************
/*	DevCaps implementation
/*****************************************************************/
DevCaps::DevCaps()
{
	strcpy( devDriver, "Unknown" );
	devDriver[c_MaxDevCapsStr - 1] = 0;
	strcpy( devDescr, "Unknown" );
	devDescr[c_MaxDevCapsStr - 1] = 0;
}

#endif // !_DX9
