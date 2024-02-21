/*****************************************************************/
/*  File:   gpMesh.cpp
/*  Author: Silver, Copyright (C) GSC Game World
/*  Date:   Feb 2002
/*****************************************************************/
#include "stdafx.h"
#include "mMath3D.h"
#include "gpMesh.h"

#ifndef _INLINES
#include "gpMesh.inl"
#endif // !_INLINES

/*****************************************************************************/
/*	BaseMesh implementation
/*****************************************************************************/
BaseMesh::BaseMesh()
{
	texID[0]	= c_AnyTexture;
	texID[1]	= c_AnyTexture;
	texID[2]	= c_AnyTexture;
	shaderID	= 0;

	indNum		= 0;
	vertNum		= 0;
	
	maxVertNum	= 0;
	maxIndNum	= 0;

	vbuf		= 0;
	ibuf		= 0;

	setVertexFormat( vf2Tex );
	setPriType( ptTriangleList );

	flags		= 0;
	devHandle	= 0;

	vbPos		= -1;
	ibPos		= -1;
}

BaseMesh::BaseMesh( const BaseMesh& bm )
{
	indNum		= 0;
	vertNum		= 0;
	
	maxVertNum	= 0;
	maxIndNum	= 0;

	vbuf		= 0;
	ibuf		= 0;

	copy( bm );
}
	

BaseMesh::~BaseMesh()
{
	delete []ibuf;
	delete []vbuf;
	ibuf = 0;
	vbuf = 0;
}


int					
BaseMesh::getSizeBytes() const
{
	int res = sizeof( *this );
	res += maxIndNum * 2;
	res += getVBufSizeBytes();
	return res;
}

void
BaseMesh::create( int numVert, int numInd,
				  VertexFormat vformat,   
				  PrimitiveType typePri, 
				  bool sharedBuffer )
{
    indNum		= 0;
	vertNum		= 0;

    if (numVert == maxVertNum && maxIndNum == numInd &&
        vformat == getVertexFormat() && typePri == getPriType()) return;

	maxVertNum	= numVert;
	maxIndNum	= numInd;

	setPriType( typePri );
	numPri  = calcNumPri( typePri, numInd ); 

	if (!sharedBuffer)
	{
		delete []ibuf;
		delete []vbuf;

		if (maxIndNum > 0) ibuf = new WORD[maxIndNum];
		vbuf = Vertex::CreateVBuf( vformat, numVert );
	}
	setVertexFormat( vformat );
}

bool
BaseMesh::SaveToFile( FILE* fp )
{
	assert( false );
	return true;
}

bool
BaseMesh::LoadFromFile( FILE* fp )
{
	assert( false );
	return true;
}

void BaseMesh::getAABB( AABoundBox& aabb ) const
{
	int vstride = Vertex::CalcStride( (VertexFormat)_vertFormat );
	float *pbuf = (float*) vbuf;
	BYTE* buf = (BYTE*) vbuf;
	if (vertNum == 0) return;
	aabb.maxv.x = pbuf[0];
	aabb.maxv.y = pbuf[1];
	aabb.maxv.z = pbuf[2];
	aabb.minv.copy( aabb.maxv );
	buf += vstride;

	for (int i = 1; i < vertNum; i++)
	{
		pbuf = (float*)buf;
		if (pbuf[0] < aabb.minv.x) aabb.minv.x = pbuf[0];
		if (pbuf[1] < aabb.minv.y) aabb.minv.y = pbuf[1];
		if (pbuf[2] < aabb.minv.z) aabb.minv.z = pbuf[2];

		if (pbuf[0] > aabb.maxv.x) aabb.maxv.x = pbuf[0];
		if (pbuf[1] > aabb.maxv.y) aabb.maxv.y = pbuf[1];
		if (pbuf[2] > aabb.maxv.z) aabb.maxv.z = pbuf[2];
		buf += vstride;
	}
}

/*---------------------------------------------------------------------------*/
/*	Func:	BaseMesh::getBoundCyl
/*	Desc:	Finds bounding cylinder of the mesh, aligned to the given axis
/*	Parm:	axis - axis of the cylinder
/*			bcyl - cylinder to place result to
/*---------------------------------------------------------------------------*/
void BaseMesh::getBoundCyl( Vector3D& axis, Cylinder& bcyl ) const
{
    Vector3D centroid;
    getCentroid( centroid );
    Line3D line( axis, centroid );

    BYTE* pv = (BYTE*)vbuf;
    int stride = Vertex::CalcStride(  getVertexFormat() );
    
	if (vertNum < 1) return;
	float cdist, maxp, minp;
	Vector3D* v = reinterpret_cast<Vector3D*>( pv );
    
    cdist = line.dist2ToOrig( *v );
	maxp  = line.project( *v );
	minp  = maxp;

	pv += stride;

	for (int i = 1; i < vertNum; i++)
	{
		v = reinterpret_cast<Vector3D*>( pv );
		float dist	= line.dist2ToOrig( *v );
		float pr	= line.project( *v );
		if (dist > cdist) cdist = dist;
		if (pr < minp) minp = pr;
		if( pr > maxp) maxp = pr;

        pv += stride;
	}

	Vector3D bottom, top;
	float r2 = cdist;
	line.getPoint( minp, bottom );
	line.getPoint( maxp, top );
	bcyl.create( bottom, top, r2 );
}

/*---------------------------------------------------------------------------*/
/*	Func:	BaseMesh::getCentroid
/*	Desc:	Finds mass center of the mesh
/*---------------------------------------------------------------------------*/
void BaseMesh::getCentroid( Vector3D& cen ) const
{
    cen.zero();
    BYTE* pv = (BYTE*)vbuf;
    int stride = Vertex::CalcStride(  getVertexFormat() );
    for (int i = 0; i < vertNum; i++, pv += stride)
        cen.add( *((Vector3D*)pv) );
    cen /= vertNum;
}

bool BaseMesh::calcNormals()
{
	if (getVertexFormat() != vfN) return false;
	VertexN* vert = (VertexN*) vbuf;
	Vector3D faceNormal;

	//  set zero normals
    int i;
	for (i = 0; i < vertNum; i++)
	{
		Vector3D* n = (Vector3D*)(&vert[i].nx);
		n->zero();
	}

	//  accumulate vertex normals from face normals
	for (i = 0; i < indNum; i += 3)
	{
		Vector3D* v1 = (Vector3D*)&vert[ibuf[i    ]];
		Vector3D* v2 = (Vector3D*)&vert[ibuf[i + 1]];
		Vector3D* v3 = (Vector3D*)&vert[ibuf[i + 2]];
		
		faceNormal.planeNormal( *v1, *v2, *v3 );
			
		Vector3D* n1 = (Vector3D*)&vert[ibuf[i    ]].nx;
		Vector3D* n2 = (Vector3D*)&vert[ibuf[i + 1]].nx;
		Vector3D* n3 = (Vector3D*)&vert[ibuf[i + 2]].nx;
		
		n1->add( faceNormal );
		n2->add( faceNormal );
		n3->add( faceNormal );
	}

	//  normalize normals :)
	for (i = 0; i < vertNum; i++)
	{
		Vector3D* n = (Vector3D*)(&vert[i].nx);
		n->normalize();
	}
	return true;
}

void BaseMesh::copy( const BaseMesh& bm )
{
	delete []ibuf;
	delete []vbuf;

	_priType	= bm._priType;
	_vertFormat = bm._vertFormat;
	flags		= bm.flags;
	ibPos		= bm.ibPos;
	vbPos		= bm.vbPos;
	indNum		= bm.indNum;
	maxIndNum	= bm.maxIndNum;
	vertNum		= bm.vertNum;
	maxVertNum	= bm.maxVertNum;
	numPri		= bm.numPri;
	shaderID	= bm.shaderID;
	texID[0]	= bm.texID[0];
	texID[1]	= bm.texID[1];
	texID[2]	= bm.texID[2];
	ibuf		= new WORD[indNum];
	vbuf		= Vertex::CreateVBuf( getVertexFormat(), vertNum );
	memcpy( ibuf, bm.ibuf, indNum * 2 );
	memcpy( vbuf, bm.vbuf, getVBufSizeBytes() );
}

bool BaseMesh::convertVF( VertexFormat to )
{
	VertexFormat myVF = getVertexFormat();
	if (myVF == to) return true;
	
	//  TODO - something's wrong with this piece of code...
	if (myVF == vf2Tex && to == vfN)
	{
		Vertex2t*	p2t = (Vertex2t*)	vbuf;
		VertexN*	pN	= (VertexN*)	vbuf;
		float u, v;
		DWORD diffuse;
		
		for (int i = 0; i < vertNum; i++)
		{
			u			= p2t[i].u;
			v			= p2t[i].v;
			diffuse		= p2t[i].diffuse;

			pN[i].u			= u;
			pN[i].v			= v;
			pN[i].nx		= 0.0f;
			pN[i].ny		= 0.0f;
			pN[i].nz		= 0.0f;
		}
		setVertexFormat( vfN );
		calcNormals();
		return true;
	}

	int oldStride = Vertex::CalcStride( myVF );
	int newStride = Vertex::CalcStride( to );

	setVertexFormat( to );
	if (oldStride != newStride)
	{
		setMaxVert( (getMaxVert() * oldStride) / newStride );
	}

	setNVert( 0 );
	setNInd ( 0 );
	setNPri ( 0 );

	return true;
}

void 
BaseMesh::dump()
{
	Log.Message( "numPri %d, indNum %d, vertNum %d, texID %d",
					numPri, indNum, vertNum, texID[0] );
	for (int i = 0; i < indNum; i++) Log.Message( "%d", ibuf[i] );
	Vertex::Dump( vbuf, vertNum, getVertexFormat() );
}

/*---------------------------------------------------------------*
/*  Func:	BaseMesh::rotate
/*	Desc:	rotates object around axis on the given angle
/*	Parm:	axis - rotation axis
/*			angle - rotation angle, in radians
/*---------------------------------------------------------------*/
void BaseMesh::rotate( const Vector3D& axis, float angle )
{
	BYTE* bVbuf = (BYTE*) vbuf;
	int stride = Vertex::CalcStride( getVertexFormat() );
	Matrix3D rot;
	rot.rotation( axis, angle );
	for (int i = 0; i < vertNum; i++)
	{
		Vector3D& v = *((Vector3D*)bVbuf);
		v *= rot; 
		bVbuf += stride;
	}
}//  BaseMesh::rotate

/*---------------------------------------------------------------*
/*  Func:	BaseMesh::transform
/*	Desc:	applies transformation to the mesh
/*	Parm:	transf - 3D transformation matrix
/*---------------------------------------------------------------*/
void BaseMesh::transform( const Matrix3D& transf )
{
	BYTE* bVbuf = (BYTE*) vbuf;
	int stride = Vertex::CalcStride( getVertexFormat() );
	for (int i = 0; i < vertNum; i++)
	{
		Vector3D& v = *((Vector3D*)bVbuf);
		v *= transf; 
		bVbuf += stride;
	}	
}//  BaseMesh::transform

void BaseMesh::transform( const Matrix4D& transf, int firstVert, int numVert )
{
	int stride = Vertex::CalcStride( getVertexFormat() );
	BYTE* bVbuf = (BYTE*) vbuf + stride * firstVert;
	assert( firstVert >= 0 && numVert >= 0 && numVert + firstVert <= vertNum );
	for (int i = 0; i < numVert; i++)
	{
		Vector3D& v = *((Vector3D*)bVbuf);
		v *= transf; 
		bVbuf += stride;
	}
}//  BaseMesh::transform
	
/*---------------------------------------------------------------*
/*  Func:	BaseMesh::transform
/*	Desc:	applies transformation to the mesh
/*	Parm:	transf - 4D transformation matrix
/*---------------------------------------------------------------*/
void BaseMesh::transform( const Matrix4D& transf )
{
	BYTE* bVbuf = (BYTE*) vbuf;
	int stride = Vertex::CalcStride( getVertexFormat() );
	Vector4D v;
	for (int i = 0; i < vertNum; i++)
	{
		v = *((Vector3D*)bVbuf);
		v.mul( transf ); 
		*((Vector3D*)bVbuf) = v;
		bVbuf += stride;
	}	
}//  BaseMesh::transform

void BaseMesh::setDiffuseColor( DWORD color )
{
	int i;

	switch (getVertexFormat())
	{
	case vf2Tex:
		{
			Vertex2t* vert = (Vertex2t*) vbuf;
			for (i = 0; i < vertNum; i++) vert[i].diffuse = color;
			return;
		}
	case vfTnL:
		{
			VertexTnL* vert = (VertexTnL*) vbuf;
			for (i = 0; i < vertNum; i++) vert[i].diffuse = color;
			return;
		}
	
	}
}

float	BaseMesh::pickPoly(	const Vector3D& orig,
                        const Vector3D& dir,
                        int& triIdx )
{
	if (getPriType() != ptTriangleList) return -1;
	int pIdx = -1;
	float cDist = -1.0f, u, v, t;
	float *pV1, *pV2, *pV3;
	BYTE* bVbuf = (BYTE*) vbuf;
	int stride = Vertex::CalcStride( getVertexFormat() );

	for (int i = 0; i < indNum; i += 3)
	{
		Vector3D v1, v2, v3;
		pV1 = (float*)(bVbuf + ibuf[i  	 ] * stride);
		pV2 = (float*)(bVbuf + ibuf[i + 1] * stride);
		pV3 = (float*)(bVbuf + ibuf[i + 2] * stride);

		v1.set( pV1[0], pV1[1], pV1[2] );
		v2.set( pV2[0], pV2[1], pV2[2] );
		v3.set( pV3[0], pV3[1], pV3[2] );
		
		if (RayTriangleX( orig, dir, v1, v2, v3, u, v, t ))
		{
			if (cDist == -1.0f || t < cDist)
			{
				triIdx	= i;
				cDist	= t;
			}
		}
	}

	return cDist;
}

float BaseMesh::pickVertex(	const Cylinder& cyl,
                            int&            vertIdx )
{
	vertIdx = -1;
	float cDist = -1.0f;
	BYTE* bVbuf = (BYTE*) vbuf;
    Vector3D& orig = cyl.getBase();
	int stride = Vertex::CalcStride( getVertexFormat() );

	for (int i = 0; i < vertNum; i++)
	{
		Vector3D* v = (Vector3D*)bVbuf;
		if (cyl.isPointInside( *v ))
		{
           float t = v->distance2( orig );
			if (cDist == -1.0f || t < cDist)
			{
				vertIdx = i;
				cDist	= t;
			}
		}
        bVbuf += stride;
	}
	return cDist;
}

void BaseMesh::createLine( const Vector3D& orig, const Vector3D& dest )
{
	if (getMaxVert() < 2 || getMaxInd() < 2) return;
	BYTE* pvb = (BYTE*) vbuf;
	float *fv = (float*)pvb;
	int stride = Vertex::CalcStride( getVertexFormat() );

	fv[0] = orig.x; fv[1] = orig.y; fv[2] = orig.z;
	pvb += stride; fv = (float*)pvb;

	fv[0] = dest.x; fv[1] = dest.y; fv[2] = dest.z;
	WORD idx[] = { 0, 1 };
	setIndices( idx, 2 );
	setNVert( 2 );
}

void BaseMesh::createQuadArray( int nQuads )
{
	if (getMaxVert() < nQuads * 4 || getMaxInd() < nQuads * 6) return;
	int cV = 0;
	int cI = 0;
	for (int i = 0; i < nQuads; i++)
	{
		ibuf[cI++]	= cV;
		ibuf[cI++]	= cV + 1;
		ibuf[cI++]	= cV + 2;

		ibuf[cI++]	= cV + 2;
		ibuf[cI++]	= cV + 1;
		ibuf[cI++]	= cV + 3;

		cV += 4;
	}
}

void BaseMesh::createTriangle()
{
    create( 3, 3 );
    static WORD idx[] = {0, 1, 2};
    setIndices( idx, 3 );
	setNVert( 3 );
}

void BaseMesh::createPrQuad( float x, float y,
                             float w, float h, float z )
{
	create( 4, 6, vfTnL );
	setIndices( Quad3D::idx, 6 );
	VertexTnL* vert = (VertexTnL*) vbuf;

	vert[0].x = x;
	vert[0].y = y;
	vert[0].z = z;

	vert[1].x = x + w;
	vert[1].y = y;
	vert[1].z = z;

	vert[2].x = x;
	vert[2].y = y + h;
	vert[2].z = z;

	vert[3].x = x + w;
	vert[3].y = y + h;
	vert[3].z = z;

	setNVert( 4 );
}

void BaseMesh::createEnclosingFrame( float cx, float cy, float cz, float hw, float hh )
{
	if (hh == 0.0f) hw = hh;

     create( 4, 6, vfTnL, ptLineStrip );
     static WORD idx[] = { 0, 1, 2, 3, 0 };
	 setIndices( idx, 5 );
	 VertexTnL* vert = (VertexTnL*) vbuf;
     vert[0].x = cx - hw;
     vert[0].y = cy - hh;
     vert[0].z = cz;

     vert[1].x = cx + hw;
     vert[1].y = cy - hh;
     vert[1].z = cz;

     vert[2].x = cx + hw;
     vert[2].y = cy + hh;
     vert[2].z = cz;

     vert[3].x = cx - hw;
     vert[3].y = cy + hh;
     vert[3].z = cz;

     setNVert( 4 );
}

void BaseMesh::createFrame( float x, float y, float z, float w, float h )
{
	if (h == 0.0f) w = h;

     create( 4, 5, vfTnL, ptLineStrip );
     static WORD idx[] = { 0, 1, 2, 3, 0 };
	 setIndices( idx, 5 );
	 VertexTnL* vert = (VertexTnL*) vbuf;
     vert[0].x = x;
     vert[0].y = y;
     vert[0].z = z;

     vert[1].x = x + w;
     vert[1].y = y;
     vert[1].z = z;

     vert[2].x = x + w;
     vert[2].y = y + h;
     vert[2].z = z;

     vert[3].x = x;
     vert[3].y = y + h;
     vert[3].z = z;

     setNVert( 4 );
}

void BaseMesh::createQuad( Vector3D& lt, Vector3D& rt,
							Vector3D& lb, Vector3D& rb )
{
	create( 4, 6 );
	setIndices( Quad3D::idx, 6 );
	Vertex2t* vert = (Vertex2t*) vbuf;
	vert[0] = lt;
	vert[0].u = 0.0f;
	vert[0].v = 0.0f;

	vert[1] = rt;
	vert[1].u = 1.0f;
	vert[1].v = 0.0f;

	vert[2] = lb;
	vert[2].u = 0.0f;
	vert[2].v = 1.0f;

	vert[3] = rb;
	vert[3].u = 1.0f;
	vert[3].v = 1.0f;

	setNVert( 4 );
}

void BaseMesh::createUnitCube( float side )
{
	static float cubevert[][3] = {
		{ 1.0f, 1.0f, 1.0f },
		{ 0.0f, 1.0f, 1.0f },
		{ 1.0f, 0.0f, 1.0f },
		{ 0.0f, 0.0f, 1.0f },
		{ 1.0f, 1.0f, 0.0f },
		{ 0.0f, 1.0f, 0.0f },
		{ 1.0f, 0.0f, 0.0f },
		{ 0.0f, 0.0f, 0.0f }
	};

	static WORD cubeidx[] = {
		0, 2, 1, 1, 2, 3, 
		3, 7, 5, 5, 1, 3, 
		1, 0, 5, 5, 0, 4, 
		0, 6, 4, 6, 0, 2, 
		3, 7, 2, 2, 7, 6, 
		6, 4, 0, 6, 0, 1
	}; 
	create( 8, sizeof( cubeidx ) / 2, vf2Tex, ptTriangleList );
	setIndices( cubeidx, sizeof( cubeidx ) / 2 );
    setNVert( 8 );
	Vertex2t* vb = (Vertex2t*) vbuf;
	for (int i = 0; i < 8; i++)
	{
		vb[i].x = cubevert[i][0] * side;
		vb[i].y = cubevert[i][1] * side;
		vb[i].z = cubevert[i][2] * side;
	}
} // BaseMesh::createUnitCube

BaseMesh& BaseMesh::operator +=( const BaseMesh& bm )
{
	assert( this != &bm );
	assert( _priType == bm._priType && _vertFormat == bm._vertFormat );

	if (maxIndNum < indNum + bm.indNum)
	//  recreate index buffer
	{
		maxIndNum = indNum + bm.indNum;
		WORD* newIB = new WORD[maxIndNum];
		memcpy( newIB, ibuf, indNum * sizeof( *ibuf ) );
		delete []ibuf;
		ibuf = newIB;
	}

	if (maxVertNum < vertNum + bm.vertNum)
	//  recreate index buffer
	{
		maxVertNum = vertNum + bm.vertNum;
		BYTE* newVB = (BYTE*)Vertex::CreateVBuf( getVertexFormat(), maxVertNum );
		memcpy( newVB, vbuf, vertNum * getVertexStride() );
		delete []vbuf;
		vbuf = newVB;
	}

	for (int i = 0; i < bm.indNum; i++)
	{
		ibuf[indNum + i] = vertNum + bm.ibuf[i];
	}
	memcpy(		((BYTE*)vbuf) + vertNum * getVertexStride(), 
				bm.vbuf, 
				bm.vertNum * bm.getVertexStride() );

	vertNum += bm.vertNum;
	indNum	+= bm.indNum;
	numPri  += bm.numPri;

	devHandle	= 0;
	vbPos		= -1;
	ibPos		= -1;
	return (*this);
} // BaseMesh::operator +=

void BaseMesh::createAABB( const AABoundBox& aabb )
{
	static WORD boxidx[] = {
		0, 1, 1, 2, 2, 3, 
		3, 0, 0, 4, 4, 7, 
		7, 6, 6, 5, 5, 4,
		1, 5, 2, 6, 3, 7
	};

	create( 8, sizeof( boxidx ) / 2, vf2Tex, ptLineList );
	setIndices( boxidx, sizeof( boxidx ) / 2 );
    setNVert( 8 );
	
	const Vector3D& vmin = aabb.minv;
	const Vector3D& vmax = aabb.maxv;

	Vertex2t* vb = (Vertex2t*) vbuf;
	vb[0].x = vmin.x; vb[0].y = vmin.y; vb[0].z = vmin.z;
	vb[1].x = vmin.x; vb[1].y = vmin.y; vb[1].z = vmax.z;
	vb[2].x = vmax.x; vb[2].y = vmin.y; vb[2].z = vmax.z;
	vb[3].x = vmax.x; vb[3].y = vmin.y; vb[3].z = vmin.z;
	vb[4].x = vmin.x; vb[4].y = vmax.y; vb[4].z = vmin.z;
	vb[5].x = vmin.x; vb[5].y = vmax.y; vb[5].z = vmax.z;
	vb[6].x = vmax.x; vb[6].y = vmax.y; vb[6].z = vmax.z;
	vb[7].x = vmax.x; vb[7].y = vmax.y; vb[7].z = vmin.z;
} // Kit3D::CreateBox

void BaseMesh::createGrid( Vector3D& lt, Vector3D& rt, 
							Vector3D& lb, Vector3D& rb,
							int cellsX, int cellsY )
{
	int numV = (cellsX + cellsY) * 2;
	int numI = numV + 4;
	create( numV, numI, vf2Tex, ptLineList ); 
	Vertex2t* vert = (Vertex2t*)vbuf;
	Vector3D v1, dv1, v2, dv2;

	v1 = lt;
	v2 = lb;
	dv1.copy( rt );
	dv1.sub( lt );
	dv1 /= cellsX;

	dv2.copy( rb );
	dv2.sub( lb );
	dv2 /= cellsX;

	int cV = 0;
    int i;
	for (i = 0; i <= cellsX; i++)
	{
		ibuf[cV] = cV;
		vert[cV].x = v1.x;
		vert[cV].y = v1.y;
		vert[cV].z = v1.z;
		cV++;

		ibuf[cV] = cV;
		vert[cV].x = v2.x;
		vert[cV].y = v2.y;
		vert[cV].z = v2.z;
		cV++;
		
		v1.add( dv1 );
		v2.add( dv2 );
	}

	v1 = lt;
	v2 = rt;
	dv1.copy( lb );
	dv1.sub( lt );
	dv1 /= cellsY;

	dv2.copy( rb );
	dv2.sub( rt );
	dv2 /= cellsY;

	v1.add( dv1 );
	v2.add( dv2 );

	for (i = 1; i < cellsY; i++)
	{
		ibuf[cV] = cV;
		vert[cV].x = v1.x;
		vert[cV].y = v1.y;
		vert[cV].z = v1.z;
		cV++;

		ibuf[cV] = cV;
		vert[cV].x = v2.x;
		vert[cV].y = v2.y;
		vert[cV].z = v2.z;
		cV++;
		
		v1.add( dv1 );
		v2.add( dv2 );
	}

	// top horz line
	ibuf[cV++] = 0;
	ibuf[cV++] = cellsX * 2;
	// bottom horz line
	ibuf[cV++] = 1;
	ibuf[cV++] = cellsX * 2 + 1;

	vertNum = numV;
	indNum	= numI;
}

void BaseMesh::createDiamond( const Vector3D& orig, float side )
{
	if (getMaxVert() < 6 || getMaxInd() < 24) return;
	BYTE* pvb = (BYTE*) vbuf;
	float *fv = (float*)pvb;
	int stride = Vertex::CalcStride( getVertexFormat() );
	float hs = side * 0.5f;

	fv[0] = orig.x; fv[1] = orig.y + hs; fv[2] = orig.z;
	pvb += stride; fv = (float*)pvb;

	fv[0] = orig.x - hs; fv[1] = orig.y; fv[2] = orig.z - hs;
	pvb += stride; fv = (float*)pvb;

	fv[0] = orig.x - hs; fv[1] = orig.y; fv[2] = orig.z + hs;
	pvb += stride; fv = (float*)pvb;

	fv[0] = orig.x + hs; fv[1] = orig.y; fv[2] = orig.z - hs;
	pvb += stride; fv = (float*)pvb;

	fv[0] = orig.x + hs; fv[1] = orig.y; fv[2] = orig.z + hs;
	pvb += stride; fv = (float*)pvb;

	fv[0] = orig.x; fv[1] = orig.y - hs; fv[2] = orig.z;
	pvb += stride; fv = (float*)pvb;

	WORD idx[] = {  0, 3, 1,   0, 1, 2,   0, 2, 3,   0, 4, 3, 
					5, 3, 4,   5, 4, 2,   5, 2, 1,   5, 1, 3  };
	
	setIndices( idx, 24 );
	setNVert( 6 );
}

/*---------------------------------------------------------------------------*/
/*	Func:	BaseMesh::createFrustum
/*	Desc:	Creates frustum mesh
/*---------------------------------------------------------------------------*/
void BaseMesh::createFrustum( const Frustum& fru )
{
	static WORD fridx[] = {
		0, 2, 2, 3, 3, 1, 1, 0, 
		6, 7, 7, 5, 5, 4, 4, 6, 
		2, 6, 3, 7, 1, 5, 0, 4
	}; 
	create( 8, sizeof( fridx ) / 2, vf2Tex, ptLineList );
	setIndices( fridx, sizeof( fridx ) / 2 );
    setNVert( 8 );
	Vertex2t* vert = (Vertex2t*) vbuf;
	
	vert[0] = fru.getVertex( 0 );
	vert[1] = fru.getVertex( 1 );
	vert[2] = fru.getVertex( 2 );
	vert[3] = fru.getVertex( 3 );
	vert[4] = fru.getVertex( 4 );
	vert[5] = fru.getVertex( 5 );
	vert[6] = fru.getVertex( 6 );
	vert[7] = fru.getVertex( 7 );
}

/*---------------------------------------------------------------------------*/
/*	Func:	BaseMesh::createCylinder
/*	Desc:	Creates cylinder mesh
/*	Parm:	cyl - cylinder which mesh to create
/*			nSeg - number of segments 
/*---------------------------------------------------------------------------*/
void BaseMesh::createCylinder( const Cylinder& cyl, int nSeg )
{
	assert( !cyl.isInfinite() );
	int nVert = 2 * nSeg + 2;
	int nInd = nSeg * 24;

	create( nVert, nInd, vfN );

	VertexN* vert = (VertexN*) getVertexData();
	WORD* ind = getIndices();
	setNInd( nInd );
	setNVert( nVert );

	Vector3D base = cyl.getBase();
	Vector3D top = cyl.getTop();

	vert[0] = base;
	vert[1] = top;
	vert += 2;

	float phi = 0.0f;
	float dphi = c_PI * 2 / nSeg;

	int cV = 2;
	for (int i = 0; i < nSeg; i++)
	{
		*(ind++) = i * 2;
		cyl.getVertex( phi, 0.0f, (Vector3D&)(*vert) );
        vert++;
		cyl.getVertex( phi, cyl.getHeight(), (Vector3D&)(*vert) );
        vert++;
        
		cV += 2;

		phi += dphi;
	}
	calcNormals();
} // BaseMesh::createCylinder


