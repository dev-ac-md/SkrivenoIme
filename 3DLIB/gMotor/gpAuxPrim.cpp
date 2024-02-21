/*****************************************************************/
/*  File:   gpAuxPrim.cpp
/*  Desc:   
/*  Author: Silver, Copyright (C) GSC Game World
/*  Date:   Feb 2002
/*****************************************************************/
#include "stdafx.h"
#include "IRenderSystem.h"
#include "gpAuxPrim.h"

namespace AuxPrim
{

/*****************************************************************/
/*  CoorSys implementation
/*****************************************************************/
CoorSys::CoorSys()
{
}

CoorSys::~CoorSys()
{
}

void CoorSys::Init( IRenderSystem* irs, float axisLen )
{
	create( 6, 6, vf2Tex, ptLineList );
	Vertex2t* vbuf = (Vertex2t*)getVertexData();
	WORD idx[] = { 0, 3, 1, 4, 2, 5 };

	vbuf[0].x = 0.0f;
	vbuf[0].y = 0.0f;
	vbuf[0].z = 0.0f;
	vbuf[0].diffuse = 0xFFFFFFFF;

	vbuf[1].x = 0.0f;
	vbuf[1].y = 0.0f;
	vbuf[1].z = 0.0f;
	vbuf[1].diffuse = 0xFFFFFFFF;

	vbuf[2].x = 0.0f;
	vbuf[2].y = 0.0f;
	vbuf[2].z = 0.0f;
	vbuf[2].diffuse = 0xFFFFFFFF;

	vbuf[3].x = axisLen;
	vbuf[3].y = 0.0f;
	vbuf[3].z = 0.0f;
	vbuf[3].diffuse = 0xFFFF0000;

	vbuf[4].x = 0.0f;
	vbuf[4].y = axisLen;
	vbuf[4].z = 0.0f;
	vbuf[4].diffuse = 0xFF00FF00;

	vbuf[5].x = 0.0f;
	vbuf[5].y = 0.0f;
	vbuf[5].z = axisLen;
	vbuf[5].diffuse = 0xFF0000FF;
	setNVert( 6 );
	setIndices( idx, 6 );

	setShader( irs->GetShaderID( "lines3d" ) );
}

BaseMesh	boxMesh;

const WORD c_WireBoxIdx[24] = 
{
	0, 1,   1, 5,   5, 4,   4, 0, 
	6, 7,   7, 3,   3, 2,   2, 6, 
	6, 4,   7, 5,   3, 1,   2, 0
};

void DrawAABB( IRenderSystem* iRS, const AABoundBox& aabb, DWORD color )
{
	if (boxMesh.getNVert() == 0)
	{
		boxMesh.create( 8, 24, vf2Tex, ptLineList );
		boxMesh.setShader( iRS->GetShaderID( "lines3D" ) );
		boxMesh.setNVert( 8 );
		boxMesh.setNInd( 24 );
		memcpy( boxMesh.getIndices(), c_WireBoxIdx, 24 * sizeof( WORD ) );
	}

	Vector3D cr[8];
	aabb.getCorners( cr );
	Vertex2t* vert = (Vertex2t*)boxMesh.getVertexData();
	for (int i = 0; i < 8; i++)
	{
		vert[i].setXYZ( cr[i].x, cr[i].y, cr[i].z );
		vert[i].diffuse = color;
	}

	iRS->Draw( boxMesh );
} // AuxPrim::DrawAABB

BaseMesh planeMesh;

void DrawPlane( IRenderSystem* IRS, const Plane& plane, DWORD color, 
					int sections = 8, float width = 100.0f )
{
	/*
	Vector3D lt(  );
	Vector3D rt(  );
	Vector3D lb(  );
	Vector3D rb(  );

	planeMesh.createGrid( lt, rt, lb, rb, sections, sections );
	IRS->Draw( planeMesh );
	*/
} // AuxPrim::DrawPlane

} // namespace AuxPrim

