/*****************************************************************
/*  File:   gpMesh.inl                                           *
/*  Author: Silver, Copyright (C) GSC Game World                 *
/*  Date:   Feb 2002                                             *
/*****************************************************************/
#include "gpColor.h"

const BYTE						c_TexCached = 0x01;
const BYTE						c_IBCached	= 0x02;
const BYTE						c_VBCached	= 0x04;
const BYTE						c_QuadList	= 0x08;
const BYTE						c_Static	= 0x10;

/*****************************************************************
/*	BaseMesh implementation
/*****************************************************************/
_finl BYTE*	BaseMesh::getVertexData() const
{ 
	return reinterpret_cast<BYTE*>( vbuf ); 
}

_finl BYTE* BaseMesh::getVertexData( int vIdx) const
{
	return reinterpret_cast<BYTE*>( vbuf )
				+ vIdx * Vertex::CalcStride( (VertexFormat)_vertFormat );
}

_finl BYTE* BaseMesh::getVNormalData( int vIdx)const
{
     assert( getVertexFormat() == vfN );
     return reinterpret_cast<BYTE*>( vbuf )
				+ vIdx * Vertex::CalcStride( (VertexFormat)_vertFormat ) +
                sizeof( float ) * 3;
}

_finl int BaseMesh::getVBufSizeBytes() const 
{ 
	return vertNum * Vertex::CalcStride( (VertexFormat)_vertFormat ); 
}

_finl VertexFormat BaseMesh::getVertexFormat() const 
{ 
	return (VertexFormat)_vertFormat; 
}

_finl int BaseMesh::getNVert() const		
{ 
	return vertNum; 
}

_finl int BaseMesh::getMaxVert()	const	
{ 
	return maxVertNum; 
}

_finl int BaseMesh::getMaxInd()	const	
{ 
	return maxIndNum; 
}

_finl int BaseMesh::getNInd() const		
{ 
	return indNum;
}

_finl int BaseMesh::getNPri() const
{
	return numPri;
}

_finl WORD* BaseMesh::getIndices() const
{
	return ibuf;
}

_finl PrimitiveType BaseMesh::getPriType() const
{
	return (PrimitiveType)_priType;
}

_finl WORD BaseMesh::getShader()	 const 
{ 
	return shaderID; 
}
	
_finl WORD BaseMesh::getTexture( int stage ) const 
{ 
	assert( stage >= 0 && stage < 3 );
	return texID[stage]; 
}

_finl int  BaseMesh::getIBPos() const	
{ 
	return ibPos; 
}

_finl int  BaseMesh::getVBPos() const	
{ 
	return vbPos; 
}

_finl void* BaseMesh::getDevHandle()	const
{
	return devHandle;
}

_finl int BaseMesh::getVertexStride() const 
{
	return Vertex::CalcStride( getVertexFormat() );
}

_finl Vector3D BaseMesh::getVertexXYZ( int vertIdx )	const
{
	assert( vertIdx >= 0 && vertIdx < vertNum );

	float* pV = (float*)((BYTE*)vbuf + 
							vertIdx * Vertex::CalcStride( (VertexFormat)_vertFormat ));
	return Vector3D( pV[0], pV[1], pV[2] );
}

_finl float* BaseMesh::vertPos( int verIdx )
{
    assert( verIdx >= 0 && verIdx < vertNum );
    float* pV = (float*)((BYTE*)vbuf + 
							verIdx * Vertex::CalcStride( (VertexFormat)_vertFormat ));
	return pV;
}


_finl bool BaseMesh::isTexCached() const 
{ 
	return (flags & c_TexCached) != 0; 
}

_finl bool BaseMesh::isIBCached() const	
{ 
	return (flags & c_IBCached) != 0; 
}

_finl bool BaseMesh::isVBCached() const	
{
	return (flags & c_VBCached) != 0; 
}

_finl bool BaseMesh::isStatic() const	
{
	return (flags & c_Static) != 0; 
}

_finl bool BaseMesh::isQuadList() const	
{
	return (flags & c_QuadList) != 0; 
}

_finl bool BaseMesh::isIndexed() const
{
	return (ibuf != 0);
}

_finl void BaseMesh::setIBPos( int pos ) 
{ 
	ibPos = pos; 
}

_finl void BaseMesh::setVBPos( int pos ) 
{ 
	vbPos = pos; 
}

_finl void BaseMesh::setDevHandle( void* handle ) 
{ 
	devHandle = handle; 
}

_finl void BaseMesh::setVertexPtr( void* ptr )	
{ 
	vbuf = ptr; 
}

_finl void BaseMesh::setIndexPtr( WORD* ptr )
{
	ibuf = ptr;
}

_finl void BaseMesh::setMaxVert( int n )
{
    maxVertNum = n;
}

_finl void BaseMesh::setMaxInd( int n )
{
    maxIndNum = n;
}

_finl void BaseMesh::setTexCached( bool isCached )
{
	if (isCached) flags |= c_TexCached;
		else flags &= ~c_TexCached;
}

_finl void BaseMesh::setIBCached( bool isCached )
{
	if (isCached) flags |= c_IBCached;
		else flags &= ~c_IBCached;
}

_finl void BaseMesh::setVBCached( bool isCached )
{
	if (isCached) flags |= c_VBCached;
		else flags &= ~c_VBCached;
}	

_finl void BaseMesh::setIsStatic( bool isStatic )
{
	if (isStatic) flags |= c_Static;
		else flags &= ~c_Static;
}

_finl void BaseMesh::setIsQuadList( bool isQList )
{
	if (isQList) flags |= c_QuadList;
		else flags &= ~c_QuadList;
}

_finl void BaseMesh::setNVert( int n ) 
{
	assert( n <= maxVertNum );
	vertNum = n; 
}

_finl void BaseMesh::setNPri( int n )
{
	numPri = n;	
}

_finl void BaseMesh::setNInd( int n ) 
{
	assert( n <= maxIndNum );
	indNum = n; 
	numPri = calcNumPri( (PrimitiveType)_priType, n );
}

_finl void BaseMesh::incNVert( int n )
{
	vertNum += n;
	assert( vertNum <= maxVertNum );
}
 
_finl void BaseMesh::incNPri( int n )
{
	numPri += n;
}

_finl void BaseMesh::incNInd( int n )
{
	indNum += n;
	assert( indNum <= maxIndNum );
}
    
_finl int BaseMesh::numPriToIdx( int priNum )
{
	return calcNumIdx( getPriType(), priNum );
}

_finl int BaseMesh::numIdxToPri( int indNum )
{
	return calcNumPri( getPriType(), indNum );
}

_finl void BaseMesh::setVertexFormat( VertexFormat vf ) 
{
	_vertFormat = (unsigned char)vf;
}

_finl void BaseMesh::setPriType( PrimitiveType pt )
{
	_priType = (unsigned char)pt;
}

_finl void BaseMesh::setIndices( WORD* idxBuf, int numIdx )
{
	if (numIdx > maxIndNum) 
	{
		Log.Error( "Couldn't set indices: ib size %d, needed %d", 
						maxIndNum, numIdx );
		return;
	}

	indNum	= numIdx;
	memcpy( ibuf, idxBuf, numIdx << 1 );
	numPri = calcNumPri( (PrimitiveType)_priType, numIdx );
} // BaseMesh::setIndices 

_finl void BaseMesh::setVertices( void* vert, int numVert )
{
	if (numVert > maxVertNum)
	{
		Log.Error( "Couldn't set vertices: vb size %d, needed %d", 
						maxVertNum, numVert );
		return;
	}

	vertNum = numVert;
	memcpy( vbuf, vert, numVert * Vertex::CalcStride( (VertexFormat)_vertFormat ) );
}

_finl void BaseMesh::setTexture( WORD tex, int stage )
{
	assert( stage >= 0 && stage < 3 );
	texID[stage] = tex;
} 

_finl void BaseMesh::setShader( WORD shader )
{
	shaderID = shader;
}

_inl int BaseMesh::calcNumPri( PrimitiveType pType, int indNum )
{
	switch( pType )
	{
	case ptUnknown:			return 0;
	case ptTriangleList:	return indNum / 3; 	
	case ptTriangleStrip:	return indNum - 2;
	case ptLineList:		return indNum >> 1;
	case ptLineStrip:		return indNum - 1;
	case ptPointList:		return indNum; 
	case ptTriangleFan:		return indNum - 2;
	}	
	return 0;
} // BaseMesh::calcNumPri

_inl int BaseMesh::calcNumIdx( PrimitiveType pType, int priNum )
{
	switch( pType )
	{
	case ptUnknown:			return 0;
	case ptTriangleList:	return priNum * 3; 	
	case ptTriangleStrip:	return priNum + 2;
	case ptLineList:		return priNum << 1;
	case ptLineStrip:		return priNum + 1;
	case ptPointList:		return priNum; 
	case ptTriangleFan:		return priNum + 2;
	}	
	return 0;
} // BaseMesh::calcNumIdx

