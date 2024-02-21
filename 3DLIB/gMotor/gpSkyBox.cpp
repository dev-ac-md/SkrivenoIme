/*****************************************************************************/
/*	File:	gpSkyBox.cpp
/*	Desc:	Skybox drawing routines
/*	Author:	Ruslan Shestopalyuk
/*	Date:	08.07.2002
/*****************************************************************************/
#include "stdafx.h"
#include "gmDefines.h"
#include "gpSkyBox.h"

#include "IRenderSystem.h"

Vector3D SkyBox::vec[8] = {	Vector3D(	-1.0f,	1.0f,		-1.0f	),
							Vector3D(	-1.0f,	1.0f,		1.0f	),
							Vector3D(	1.0f,	1.0f,		1.0f	),
							Vector3D(	1.0f,	1.0f,		-1.0f	),
							Vector3D(	-1.0f,	-1.0f,		-1.0f	),
							Vector3D(	-1.0f,	-1.0f,		1.0f	),
							Vector3D(	1.0f,	-1.0f,		1.0f	),
							Vector3D(	1.0f,	-1.0f,		-1.0f	)
							};

int SkyBox::irt[6] = { 0, 4, 0, 1, 2, 3 };
int SkyBox::ilt[6] = { 3, 5, 1, 2, 3, 0 };
int SkyBox::irb[6] = { 1, 7, 4, 5, 6, 7 };
int SkyBox::ilb[6] = { 2, 6, 5, 6, 7, 4 };

SkyBox::SkyBox()
{
}

void SkyBox::create( IRenderSystem* irs, const char* skyName, float side )
{
	String texNames[6];
	for (int i = 0; i < 6; i++) texNames[i] = skyName;
	
	halfSide = side / 2.0f;

	texNames[0] += "_up";
	texNames[1] += "_dn";
	texNames[2] += "_ft";
	texNames[3] += "_rt";
	texNames[4] += "_bk";
	texNames[5] += "_lf";
	for (int i = 0; i < 6; i++) texNames[i] += ".jpg";

	for (int i = 0; i < 6; i++)
	{
		BaseMesh* bm = &(wall[i]);
		
		bm->createQuad( vec[ilt[i]], vec[irt[i]], vec[ilb[i]], vec[irb[i]] );

		Vertex2t* vert = (Vertex2t*) bm->getVertexData();
		vert[0].u = 0.0f;
		vert[0].v = 0.0f;
		vert[0].x *= halfSide; vert[0].y *= halfSide; vert[0].z *= halfSide;

		vert[1].u = 1.0f;
		vert[1].v = 0.0f;
		vert[1].x *= halfSide; vert[1].y *= halfSide; vert[1].z *= halfSide;


		vert[2].u = 0.0f;
		vert[2].v = 1.0f;
		vert[2].x *= halfSide; vert[2].y *= halfSide; vert[2].z *= halfSide;


		vert[3].u = 1.0f;
		vert[3].v = 1.0f;
		vert[3].x *= halfSide; vert[3].y *= halfSide; vert[3].z *= halfSide;
	
		
		bm->setTexture( irs->GetTextureID( texNames[i].c_str() ) );
		bm->setShader( irs->GetShaderID( "sky" ) );
	}

	Matrix4D rot;
	rot.rotation( Vector3D::oX, c_PI * 0.5f );

	for (int i = 0; i < 6; i++)
	{
		BaseMesh* bm = &(wall[i]);
		bm->transform( rot ); 
	}
}

void SkyBox::render( IRenderSystem* irs, Camera* cam )
{
	Matrix4D transM, oldM;
	oldM = irs->GetWorldMatrix();
	const Vector3D& trans = cam->getPos();
	transM = oldM;
	transM.e30 = trans.x; transM.e31 = trans.y; transM.e32 = trans.z;
	irs->SetWorldMatrix( transM );
	for (int i = 0; i < 6; i++)
	{
		irs->Draw( wall[i] );
	}
	
	irs->SetWorldMatrix( oldM );
}
