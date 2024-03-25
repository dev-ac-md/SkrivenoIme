/*****************************************************************************/
/*	File:	Scape3D.cpp
/*	Desc:	
/*	Author:	Ruslan Shestopalyuk
/*	Date:	14.11.2002
/*****************************************************************************/
#pragma pack( push )  
#pragma pack( 8 ) 

#include "..\3DLIB\gMotor\gmDefines.h"

#include "windows.h"
#include "..\3DLIB\gMotor\kAssert.h"

#include "ResFile.h" 
#include "..\3DLIB\gMotor\kArray.hpp"
#include "..\3DLIB\gMotor\kString.h"
#include "..\3DLIB\gMotor\IRenderSystem.h"   
#include "..\3DLIB\gMotor\kLog.h"
#include "..\3DLIB\gMotor\gpMesh.h"
#include "..\3DLIB\gMotor\mCamera.h"
#include "..\3DLIB\gMotor\gpAnimModel.h"  
#include "..\3DLIB\gMotor\gpSkyBox.h"    


#include "DDini.h" 
#include "FastDraw.h" 
#include "GP_Draw.h"

#include "Scape3D.h"
Camera		CAM;
SkyBox		SB;
#pragma pack( pop )  

int			LOD = 0;
float		CamZn, CamZf;    

/*****************************************************************************/
/*	ShadowPatch implementation
/*****************************************************************************/
void ShadowPatch::InitMesh( IRenderSystem* irs ) 
{
	mesh.create			(	c_ShadowPatchVert,  
							c_ShadowPatchInd, 
							vf2Tex, 
							ptTriangleList		
						);
	mesh.setShader		( irs->GetShaderID( "shadowReceiver" )				);
	mesh.setNVert		( c_ShadowPatchVert									);
	mesh.setNInd		( c_ShadowPatchInd									);
	mesh.setNPri		( BaseMesh::calcNumPri( ptTriangleList, c_ShadowPatchInd ) );
} // ShadowPatch::InitMesh

/*---------------------------------------------------------------------------*/
/*	Func:	ShadowPatch::CreateMesh
/*	Desc:	
/*---------------------------------------------------------------------------*/
void ShadowPatch::CreateMesh( const Camera& lightCam, int qx, int qy, int qside )
{
	if (!THMap) return;
	assert( qside > 0 ); 

	attr.px		= qx; 
	attr.py		= qy;
	attr.pside	= qside; 

	int sideTriNum = qside / c_nMinShadowPatchGridStep;
	if (sideTriNum > c_SPSideMaxTris) sideTriNum = c_SPSideMaxTris;
	if (sideTriNum == 0) sideTriNum = 1;


	Vertex2t*	vert	= (Vertex2t*)mesh.getVertexData();
	int			cVertert	= 0;
	int			vInLine = sideTriNum + 1;
	
	int		cVert	= 0;
	begWorldX		= ((float)qx) * c_HmapToWorldX; 
	begWorldY		= ((float)qy) * c_HmapToWorldY; 
	worldSide		= ((float)qside) * c_HmapToWorldX;
	float texScaleFactor = 1.0f / worldSide; 

	float	worldX	= 0.0f;
	float	worldY	= 0.0f;
	
	int		hmapX	= qx;
	int		hmapY	= qy;

	int		hmapStepX = qside / sideTriNum;
	int		hmapStepY = hmapStepX;
	
	float	worldStepX = hmapStepX * c_HmapToWorldX;
	float	worldStepY = hmapStepY * c_HmapToWorldY;
	float	halfWorldStepY = worldStepY * 0.5f;

	float	cU, cV;

	const Matrix4D&	m = lightCam.getViewM();
	Vector4D	vec;
	
	for (int i = 0; i < vInLine; i++) 
	{
		for (int j = 0; j < vInLine; j++) 
		{
			vec.x = worldX;
			vec.z = ((float)GetHeight( hmapX, hmapY )) / c_Cos30;
			vec.y = worldY + ((j & 1) ? 0.0f : halfWorldStepY);
			vec.y -= vert[cVert].z * 0.1f; 
			vert[cVert].setXYZ( vec.x + begWorldX, vec.y + begWorldY, vec.z );

			//  texture coordinates - project mesh vertex onto 
			//  plane, orthogonal to light direction
			vec.mul( m );
			vert[cVert].u = vec.x * texScaleFactor;
			vert[cVert].v = -vec.y * texScaleFactor;

			cVert++;
			worldX += worldStepX;
			hmapX += hmapStepX;
		}
		hmapX = qx;
		hmapY += hmapStepY;

		worldX = 0.0f;
		worldY += worldStepY;  
	}

	//  indices
	WORD* qIdx = mesh.getIndices(); 
	int cI = 0;
	cVert = 0;
	for (int i = 0; i < sideTriNum; i++)
	{
		for (int j = 0; j < sideTriNum; j++)
		{
			if (j & 1)
			{
				qIdx[cI++] = cVert;
				qIdx[cI++] = cVert + 1;
				qIdx[cI++] = cVert + vInLine; 
				qIdx[cI++] = cVert + vInLine;
				qIdx[cI++] = cVert + 1;
				qIdx[cI++] = cVert + vInLine + 1;
			}
			else
			{
				qIdx[cI++] = cVert;
				qIdx[cI++] = cVert + 1;
				qIdx[cI++] = cVert + 1 + vInLine; 
				qIdx[cI++] = cVert;
				qIdx[cI++] = cVert + 1 + vInLine; 
				qIdx[cI++] = cVert + vInLine;	 
			}
			cVert++;
		}
		cVert++;
	}
	mesh.setNInd( cI );
} // ShadowPatch::CreateMesh

/*****************************************************************************/
/*	ScapeQuad implementation
/*****************************************************************************/
template <class VType>	
WORD ScapeQuad<VType>::qIdx[c_QuadInd];

template <class VType>	
ScapeQuad<VType>::ScapeQuad()
{
	maxH	= 0;
	lod		= 0;
}

/*---------------------------------------------------------------------------*/
/*	Func:	ScapeQuad::IsVisible
/*	Desc:	Determines if given quad is fully or partially visible on screen
/*---------------------------------------------------------------------------*/
template <class VType>	
bool ScapeQuad<VType>::IsVisible()
{
	return true;
} // ScapeQuad::IsVisible

DWORD GetDW_Light( int idx );

int VIdx( int x, int y )
{
	if (x < 0)			x	= 0;
	if (y < 32)			y	= 32;
	extern int ADDSH;
	int MSXH = 240 << ADDSH;
	if (x > MSXH * 32)	x	= MSXH * 32;
	if (y > MSXH * 32)	y	= MSXH * 32;
	extern int VertInLine;
	return x + y * VertInLine;
}

/*---------------------------------------------------------------------------*/
/*	Func:	ScapeQuad::CreateInWorld	
/*	Desc:	creates quad mesh in world space
/*---------------------------------------------------------------------------*/
template <class VType>
void ScapeQuad<VType>::CreateInWorld( int qx, int qy, int qlod )
{
	if (!THMap) return;
	lod = qlod;
	bound.x		= qx;
	bound.y		= qy;
	bound.x1	= qx + c_QuadSide;
	bound.y1	= qy + c_QuadSide;

	VType*	vert	= (VType*)mesh.getVertexData();
	int		cVert	= 0;
	int		vInLine = c_QuadHorzTris + 1;

	int		cV		= 0;
	float	bX		= ((float)qx) * c_WorldGridStepX / c_HmapGridStepX;  
	float	worldX	= bX;
	float	worldY	= ((float)qy) * c_WorldGridStepY / c_HmapGridStepY; 
	
	int		hmapX	= qx;
	int		hmapY	= qy;

	int		hStepX	= c_HmapGridStepX << qlod;
	int		hStepY	= c_HmapGridStepY << qlod;
	int		hStepYHalf = hStepY / 2;	
	float	wStepX	= ((float)hStepX) * c_WorldGridStepX / c_HmapGridStepX;
	float	wStepY	= ((float)hStepY) * c_WorldGridStepY / c_HmapGridStepY;
	float	wStepYHalf = wStepY * 0.5f;

	for (int i = 0; i < vInLine; i++) 
	{
		for (int j = 0; j < vInLine; j++) 
		{
			int ry = (j & 1) ? hmapY : hmapY + hStepYHalf;
			float H = (float)GetHeight( hmapX, ry );
			vert[cV].x = worldX;
			vert[cV].z = H * c_Cos30;
			vert[cV].y = worldY + ((j & 1) ? 0.0f : wStepYHalf) - H*0.5f;
			//vert[cV].y += vert[cV].z * 0.16f; 

			//  texture coordinates
			vert[cV].diffuse = GetDW_Light( VIdx( hmapX / 32, ry / 32 ) );

			cV++;
			worldX += wStepX;
			hmapX += hStepX;
		}
		hmapX = qx;
		hmapY += hStepY;

		worldX = bX;
		worldY += wStepY;
	}

	//  now skew it
	//mesh.transform( skewMatr );
} // ScapeQuad::CreateInWorld

/*---------------------------------------------------------------------------*/
/*	Func:	ScapeQuad::InitMesh
/*---------------------------------------------------------------------------*/
template <class VType>
void ScapeQuad<VType>::InitMesh()
{
	mesh.create			( c_QuadVert, 0, VType::format(), ptTriangleList	);
	mesh.setIndexPtr	( ScapeQuad::qIdx									);
	mesh.setNVert		( c_QuadVert										);
	mesh.setMaxInd		( c_QuadInd											);
	mesh.setNInd		( c_QuadInd											);
	mesh.setNPri		( BaseMesh::calcNumPri( ptTriangleList, c_QuadInd ) );
	
	//mesh.setShader		( IRS->GetShaderID( "zground" )						);
	mesh.setShader		( IRS->GetShaderID( "fground" )						);
	mesh.setTexture		( IRS->GetTextureID( "ground2.bmp" )				);
	mesh.setTexture		( IRS->GetTextureID( "edge.tga" ), 1				);

	//  texture coordinates
	int			vInLine = c_QuadHorzTris + 1;
	int			cVert	= 0;
	float		cV		= 0.0f;
	float		cU		= 0.0f;
	
	//float		stepU	= 1.0f / (float)vInLine;
	//float		stepV	= 1.0f / (float)vInLine;

	float		stepU	= 1.0f / (float)vInLine;
	float		stepV	= 1.0f / (float)vInLine;

	float		halfStepV = stepV * 0.5f;
	VType* vert = (VType*)mesh.getVertexData();
	for (int i = 0; i < vInLine; i++) 
	{
		for (int j = 0; j < vInLine; j++) 
		{
			vert[cVert].u = cU;
			vert[cVert].v = cV + ((j & 1) ? 0.0f : halfStepV);

			cVert++;
			cU += stepU;
		}
		cU = 0.0f;
		cV += stepV;
	}

} // ScapeQuad::InitMesh

/*---------------------------------------------------------------------------*/
/*	Func:	ScapeQuad::InitIndices	
/*---------------------------------------------------------------------------*/
template <class VType>
void ScapeQuad<VType>::InitIndices()
{
	int cI = 0;
	int cV = 0;
	int vInLine = c_QuadHorzTris + 1;
	for (int i = 0; i < c_QuadHorzTris; i++)
	{
		for (int j = 0; j < c_QuadHorzTris; j++)
		{
			if (j & 1)
			{
				qIdx[cI++] = cV;
				qIdx[cI++] = cV + 1;
				qIdx[cI++] = cV + vInLine;
				qIdx[cI++] = cV + vInLine;
				qIdx[cI++] = cV + 1;
				qIdx[cI++] = cV + vInLine + 1;
			}
			else
			{
				qIdx[cI++] = cV;
				qIdx[cI++] = cV + 1;
				qIdx[cI++] = cV + 1 + vInLine;
				qIdx[cI++] = cV;
				qIdx[cI++] = cV + 1 + vInLine;
				qIdx[cI++] = cV + vInLine;	
			}
			cV++;
		}
		cV++;
	}
} // ScapeQuad::InitIndices

/*****************************************************************************/
/*	ScapeMesh implementation
/*****************************************************************************/
/*---------------------------------------------------------------------------*/
/*	Func:	ScapeMesh::Init	
/*	Desc:	
/*---------------------------------------------------------------------------*/
void ScapeMesh::Init( IRenderSystem* iRS )
{
	TScapeQuad::InitIndices();

	int cQuad = 0;
	for (int i = 0; i < quadHache.maxElem(); i++)
	{
		TScapeQuad* q = quadHache.elem( i );
		q->InitMesh();
	}
	
	//  init indices for the scape quad
	float viewVolumeWidth = RealLx;
	
	CAM.setOrtho			();
	CAM.setViewVolumeWidth	( viewVolumeWidth					);
	CAM.setUp				( Vector3D::oZ						);  
	
	CAM.setDir				( 0.0f, -c_Cos30,					-0.5f					);
	CAM.setPos				( 0.0f, c_Cos30 * c_CameraDistance,	0.5f * c_CameraDistance );	 

	CamZn = c_CameraDistance - ((float)RealLy) * c_Cos30 - c_MaxMapHeight / c_Cos30;
	CamZf = c_CameraDistance + ((float)RealLy) * c_Cos30; 
	CAM.setZn				( CamZn );
	CAM.setZf				( CamZf );
	needUpdate = false;

} // ScapeMesh::Init

int unzoomex( int val );
int zoomex( int val );

const int c_ScapeMeshBorder = c_QuadSide;

/*---------------------------------------------------------------------------*/
/*	Func:	ScapeMesh::Show
/*	Desc:	Draws bunch of scape quads
/*---------------------------------------------------------------------------*/
void ScapeMesh::Show( IRenderSystem* iRS )
{
	if (!CAM.isOrtho())
	{
		/*
		const Frustum&  fr = CAM.getFrustum();
		const Vector3D& lt = fr.getNearLTVec();
		const Vector3D& rt = fr.getNearRTVec();
		const Vector3D& p  = CAM.getPos();

		float lx = p.x - lt.x * p.z / lt.z;
		float ly = p.y - lt.y * p.z / lt.z;

		float rx = p.x - rt.x * p.z / rt.z;
		float ry = p.y - rt.y * p.z / rt.z;
		*/
	}

	int		mapX	= mapx * c_HmapGridStepX;
	int		mapY	= mapy * c_HmapGridStepY;
	int		qside	= /*zoomex(*/ c_QuadSide /*)*/;
	int		bx		= (mapX / qside) * qside - c_ScapeMeshBorder;
	int		by		= (mapY / qside) * qside - 16 - c_ScapeMeshBorder;

	int		ex		= mapX + unzoomex( RealLx ) + c_ScapeMeshBorder; 
	int		ey		= mapY + unzoomex( RealLy * 2 + c_BottomFieldExtent ) + c_ScapeMeshBorder;
	DWORD	factor	= GetTickCount();
	int		nQuads	= 0;
	float	quadSide = (float)((int)c_QuadSide << LOD);

	for (int qy = by; qy < ey; qy += quadSide)
	{					
		for (int qx = bx; qx < ex; qx += quadSide)
		{
			TScapeQuad::Attr attr( qx, qy, LOD );
			TScapeQuad* quad = quadHache.getHitAllocEl( attr, factor );
			assert( quad );
			if (!quad->hasAttr( attr ) || needUpdate) 
			{
				quad->CreateInWorld( qx, qy, LOD );
				//quad->CreateTIN( qx, qy, LOD );
			}
			IRS->Draw( quad->mesh ); 
			nQuads++;
		}
	}
	needUpdate = false;
} // ScapeMesh::Show

void ScapeMesh::reset()
{
	setNeedUpdate();
}

ScapeMesh ScapeBM;
ShadowMapper	SHM;

/*****************************************************************************/
/*	ShadowMapper implementation
/*****************************************************************************/
void ShadowMapper::init( IRenderSystem* irs )
{
	mapTD.setValues(	(float)c_MapTexSide, 
						(float)c_MapTexSide, 
						cfBackBufferCompatible, 
						mpVRAM, 1, tuRenderTarget );
	mapTex = irs->CreateTexture( "shadowMapRenderTarget", mapTD );
	lightDir.set( -0.5f, 0.0f, -0.766f );
	
	lightDir.normalize();

	lightCam.setOrtho();
	lightCam.setLookAt( 0.0f, 0.0f, 0.0f );
	lightCam.setDir( lightDir );
	Vector3D pos( lightDir );
	pos.reverse();
	pos *= c_LightDistance;
	lightCam.setPos( pos );

	lightCam.setZn( c_LightDistance - c_DefObjectRadius );  
	lightCam.setZf( c_LightDistance + c_DefObjectRadius );
	lightCam.setViewVolumeWidth( c_DefObjectRadius * 2.0f );

	vport.x			= 1.0f;
	vport.y			= 1.0f;
	vport.width		= c_MapTexSide - 2;
	vport.height	= c_MapTexSide - 2;
	vport.MinZ		= 0.0f;
	vport.MaxZ		= 0.0f;

	shMap = irs->GetShaderID( "shadowMap" );
	shReceiver = irs->GetShaderID( "shadowReceiver" );

	for (int i = 0; i < patchHache.maxElem(); i++)
	{
		ShadowPatch* patch = patchHache.elem( i );
		patch->InitMesh( irs );
	}

} // ShadowMapper::init

/*---------------------------------------------------------------------------*/
/*	Func:	ShadowMapper::reset
/*	Desc:	Resets shadow mapper state
/*---------------------------------------------------------------------------*/
void ShadowMapper::reset()
{
	patchHache.reset();
	for (int i = 0; i < patchHache.numElem(); i++)
	{
		ShadowPatch* patch = patchHache.elem( i );
		patch->resetAttr();
	}
} // ShadowMapper::reset

/*---------------------------------------------------------------------------*/
/*	Func:	ShadowMapper::castShadow
/*	Desc:	Casts shadow for the given model
/*---------------------------------------------------------------------------*/
void ShadowMapper::castShadow( IRenderSystem* irs, AnimModel& mdl, Matrix4D& transf )
{
	//  create shadow map
	irs->SetRenderTarget( mapTex );
	//irs->SetViewPort( vport );
	//  adjust "light camera" parameters to accomodate to this object
	Matrix4D rot( transf );
	rot.e30 = 0.0f; rot.e31 = 0.0f; rot.e32 = 0.0f;

	AABoundBox aabb;
	mdl.GetSkeleton().calcAABB( aabb );
	float vpWidth = aabb.getDiagonal();
	vpWidth += vpWidth;
	float objRadius = vpWidth / 2.0f;
	lightCam.setViewVolumeWidth( vpWidth );

	irs->SetViewMatrix		( lightCam.getViewM()	);
	irs->SetProjectionMatrix( lightCam.getProjM()	);
	//  apply rotation only
	irs->SetWorldMatrix		( rot					); 
	
	irs->ClearDeviceTarget( 0xFFFFFFFF );
	irs->SetCurrentShader( shMap );
	mdl.DrawPlain();
	irs->SetRenderTarget( 0 );

	//  find bounding frame of the shadow on the ground
	Vector4D corner;
	//  left top
	corner.set( -objRadius, objRadius, 0.0f, 1.0f );
	lightCam.UnviewOnXOY( corner );
	corner.mul( lightCam.getViewM() );
	float minX = corner.x; 
	float minY = corner.y;
	float maxX = corner.x;
	float maxY = corner.y;
	
	//  right top
	corner.set( objRadius, objRadius, 0.0f, 1.0f );
	lightCam.UnviewOnXOY( corner );
	if (corner.x > maxX) maxX = corner.x;
	if (corner.x < minX) minX = corner.x;
	if (corner.y > maxY) maxY = corner.y;
	if (corner.y < minY) minY = corner.y;
	
	//  left bottom
	corner.set( -objRadius, -objRadius, 0.0f, 1.0f );
	lightCam.UnviewOnXOY( corner );
	if (corner.x > maxX) maxX = corner.x;
	if (corner.x < minX) minX = corner.x;
	if (corner.y > maxY) maxY = corner.y;
	if (corner.y < minY) minY = corner.y;
	
	//  right bottom
	corner.set( objRadius, -objRadius, 0.0f, 1.0f );
	lightCam.UnviewOnXOY( corner );
	if (corner.x > maxX) maxX = corner.x;
	if (corner.x < minX) minX = corner.x;
	if (corner.y > maxY) maxY = corner.y;
	if (corner.y < minY) minY = corner.y;

	int X1 = (int)minX; 
	int X2 = (int)maxX; 
	int Y1 = (int)minY; 
	int Y2 = (int)maxY;
	
	X1 = X1 + X1 % c_MinShadowPatchSide;
	X2 = X2 - X2 % c_MinShadowPatchSide + c_MinShadowPatchSide;
	Y1 = Y1 + Y1 % c_MinShadowPatchSide;
	Y2 = Y2 - Y2 % c_MinShadowPatchSide + c_MinShadowPatchSide;

	int CX = (int)transf.e30; 
	CX = CX - CX % c_MinShadowPatchSide;
	
	int CY = (int)transf.e31; 
	CY = CY - CY % c_MinShadowPatchSide; 

	int		qx		= CX + X1;
	int		qy		= CY + Y1;
	int		S		= X2 - X1 > Y2 - Y1 ? X2 - X1 : Y2 - Y1;
	int		qside	= S;

	ShadowPatch::Attr attr( qx, qy, qside );
	DWORD factor = GetTickCount();
	ShadowPatch* patch = patchHache.getHitAllocEl( attr, factor );
	assert( patch );
	if (!patch->hasAttr( attr ))
	{
		patch->CreateMesh( lightCam, qx, qy, qside );
	}

	irs->SetViewMatrix		( CAM.getViewM()		); 
	irs->SetProjectionMatrix( CAM.getProjM()		); 
	irs->SetWorldMatrix		( Matrix4D::identity	); 

	Matrix4D texMatr;
	texMatr.setIdentity();
	//  scale texture coords to accomodate shadow map viewport width
	float scale = vpWidth / patch->worldSide;
	texMatr.e00 = 1.0f / scale;
	texMatr.e11 = 1.0f / scale;
	//  shift texture coords to match shadow map projection center
	Vector4D patchV( patch->begWorldX, patch->begWorldY, 0.0f );
	Vector4D objV( transf.e30, transf.e31, transf.e32 ); 
	patchV.mul( lightCam.getViewM() );
	objV.mul( lightCam.getViewM() );
	objV.sub( patchV );

	texMatr.e20 = 0.5 - objV.x / vpWidth; 
	texMatr.e21 = 0.5 + objV.y / vpWidth;   

	irs->SetTextureMatrix( texMatr ); 
	irs->SetCurrentShader( shReceiver ); 
	irs->SetTexture( mapTex, 0 );
	Matrix4D matr;
	
	matr.translation( 0.0f, 8.0f, 16.0f );
	irs->SetWorldMatrix( matr );
	
	irs->DrawPrim( patch->mesh );
	
	//irs->SaveTexture( mapTex, "c:\\dumps\\shadow.dds" );
} // ShadowMapper::castShadow

/*****************************************************************************/
/*	Globals implementation
/*****************************************************************************/
anmBone* turretBone = 0;

void InitGroundZbuffer()
{
	SHM.init( IRS );
	ScapeBM.Init( IRS );
	//SB.create( IRS, "nuke_", 300.0f );
} // InitGroundZbuffer

void GetGroundNormal( int x, int y, Vector3D& vec )
{
	float l = GetHeight( x - 64, y );		
	float r = GetHeight( x + 64, y );		
	float u = GetHeight( x, y - 64 );		
	float d = GetHeight( x, y + 64 );		
	
	vec.x = -(r - l) * 0.015625f * 0.5f;
	vec.y = -(d - u) * 0.015625f * 0.5f;
	vec.z = 1.0f;
	vec.normalize();
} // GetGroundNormal

const float c_1divStep = 0.0078125f;
void GetGroundNormal( int x, int y, Vector4D& vec )
{
	float l = GetHeight( x - 64, y );		
	float r = GetHeight( x + 64, y );		
	float u = GetHeight( x, y - 64 );		
	float d = GetHeight( x, y + 64 );		
	
	vec.x = -(r - l) * c_1divStep;
	vec.y = -(d - u) * c_1divStep; 
	vec.z = 1.0f;
	vec.w = 0.0f;
	vec.normalize(); 
} // GetGroundNormal 

inline float ParabolicSecondDeriv( float dxStep2, 
									float hl, float h0, float hr )
{
	return (2.0f * h0 + hl + hr) / dxStep2;
}

float GetGround_d2x( int x, int y )
{
	return ParabolicSecondDeriv( 1024.0f, 
									GetHeight( x - 64,  y ),
									GetHeight( x,		y ),
									GetHeight( x + 64,  y )
									);
}  

extern Camera MCAM;
extern GP_System GPS;

void ShowNewMonsters();
void MiniShowNewMonsters();
void HandleWater();
void ShowZBuffer();

bool g_bDrawMCam = false;
ShadowMode shadowMode  = shmNoShadow;

void ShowMCam()
{
	if (!g_bDrawMCam) return;

	ViewPort vp;
	vp.width	= MCAM.getScrW();
	vp.height	= MCAM.getScrH();

	vp.x		= (RealLx - vp.width) * 0.5f;
	vp.y		= RealLy - vp.height;
	vp.MinZ		= 0.0f;
	vp.MaxZ		= 1.0f;

	MCAM.setViewPort( vp.x, vp.y, vp.width, vp.height );

	IRS->SetViewPort( vp );
	IRS->ClearDeviceTarget();
	IRS->ClearDeviceZBuffer();

	IRS->SetViewMatrix( MCAM.getViewM() );
	IRS->SetProjectionMatrix( MCAM.getProjM() );
	IRS->SetWorldMatrix( Matrix4D::identity );

	SB.render( IRS, &MCAM );
	ScapeBM.Show( IRS ); 

	shadowMode = shmNoShadow;
	Camera oldCam = CAM;
	CAM = MCAM;

	MiniShowNewMonsters();

	CAM = oldCam; 
	shadowMode = shmShadowPatch;

	GPS.EnableZBuffer( true );
	ShowZBuffer();
	GPS.EnableZBuffer( false );
	GPS.FlushBatches(); 

	Matrix4D tr;
	tr.translation( 0.0f, 0.0f, 0.0f );
	IRS->SetWorldMatrix( tr );
	//HandleWater();
	const Matrix4D& matr = IRS->GetWorldMatrix();

} // ShowMCam

/*---------------------------------------------------------------------------*/
/*	Func:	FillGroundZBuffer	
/*	Desc:	Draws z values of ground pixels into device z buffer
/*---------------------------------------------------------------------------*/
void FillGroundZBuffer()
{ 	
	IRS->SetViewMatrix		( CAM.getViewM()		);   
	IRS->SetProjectionMatrix( CAM.getProjM()		);    
	IRS->SetWorldMatrix		( Matrix4D::identity	);        
     
	extern int Shifter;
	LOD = 5 - Shifter;  

	if (GetKeyState( VK_END ) < 0) shadowMode = shmShadowPatch; 

	CAM.ZBiasDecrease( 0.001f );
	IRS->SetProjectionMatrix( CAM.getProjM() );
	static bool first = true;
	if (!first)
	{
		ScapeBM.Show( IRS );
	}
	else first = false;
	CAM.ZBiasIncrease( 0.001f );
	IRS->SetProjectionMatrix( CAM.getProjM() );
	
} // FillGroundZBuffer

void ResetGroundCache() 
{
	ScapeBM.reset(); 
	SHM.reset();
}

float g_PerspEdge = 100.0f;

void SetPerspCamera()
{
	//return;
	extern int Shifter; 
	float scale		= float(1<<(5-Shifter));
	g_PerspEdge = 100.0f * scale;
	float volWidth	= RealLx * scale; 
	float ScaledLy	= RealLy * scale;
	float camDist	= ScaledLy * c_Cos30 + c_MaxMapHeight / c_Cos30 + 100.0f;
	float fov		= 2.0f * atan( (volWidth - g_PerspEdge) / (2 * camDist) ) - 0.40f/(5-scale * 0.84); 
	
	extern float CamZn, CamZf;
	CamZf = camDist + ScaledLy * c_Cos30; 
	CamZn = camDist - ScaledLy * c_Cos30 - c_MaxMapHeight / c_Cos30;
	
	Vector3D pos(	mapx * c_WorldGridStepX + RealLx*scale / 2,  
					mapy * c_WorldGridStepY + RealLy*scale,     
					0.0f );
	Vector3D dir( CAM.getDir() ); 
	
	CAM.setOrtho( false );
	CAM.setLookAt( pos );
	dir *= -camDist; 
	pos += dir; 

	CAM.setPos		( pos );
	CAM.setHFOV		( fov );
	CAM.setViewPort	( 0, 0, volWidth, ScaledLy ); 
	CAM.setZn		( CamZn	);
	CAM.setZf		( CamZf	); 
}