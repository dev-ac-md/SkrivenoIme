/*****************************************************************/
/*  File:   gpMesh.h
/*  Desc:   3D visible entities classes
/*  Author: Silver, Copyright (C) GSC Game World
/*  Date:   Feb 2002
/*****************************************************************/
#ifndef __GPMESH_H__
#define __GPMESH_H__

#include "rsVertex.h"
#include "mMath3D.h"

/*****************************************************************************/
/*	Enum:	PrimitiveType
/*	Desc:	Types of the drawn primitives
/*****************************************************************************/
enum PrimitiveType
{
	ptUnknown				= 0,
	ptPointList				= 1,
	ptLineList				= 2,
	ptLineStrip				= 3,
	ptTriangleList			= 4,
	ptTriangleStrip			= 5,
	ptTriangleFan			= 6
};  // enum PrimitiveType

class AABoundBox;

/*****************************************************************
/*	Class:	BaseMesh
/*	Desc:	Base mesh class - all polys are drawn with same 
/*				texture & shader
/*****************************************************************/
class BaseMesh 
{
public:
	
	BaseMesh();
	BaseMesh( const BaseMesh& bm );
	virtual				~BaseMesh();

    void                copy( const BaseMesh& bm );

	//  getters
	_finl BYTE*			getVertexData()			const;
	_finl BYTE*			getVertexData( int vIdx)const;
    _finl BYTE*			getVNormalData( int vIdx)const;

	_finl int			getVBufSizeBytes()		const;
	_finl VertexFormat	getVertexFormat()		const;
	_finl int			getNVert()				const;
	_finl int			getMaxVert()			const;
	_finl int			getMaxInd()				const;

	_finl int			getNInd()				const;
	_finl WORD*			getIndices()			const;
	_finl int			getNPri()				const;
	_finl PrimitiveType	getPriType()			const;
	
	_finl WORD			getShader()				const;	
	_finl WORD			getTexture( int stage = 0 )	const;

	_finl int			getIBPos()				const;
	_finl int			getVBPos()				const;
	_finl void*			getDevHandle()			const;
	_finl int			getVertexStride()		const;
	_finl void			setDevHandle( void* handle );

	_finl Vector3D		getVertexXYZ( int verIdx ) const;

    _finl float* 		vertPos( int verIdx );

	
	void				getAABB( AABoundBox& aabb ) const;
    void                getBoundCyl( Vector3D& axis, Cylinder& bcyl ) const;
    void                getCentroid( Vector3D& cen ) const;

	_finl bool			isTexCached()			const;
	_finl bool			isIBCached()			const;
	_finl bool			isVBCached()			const;
	_finl bool			isStatic()				const;
	_finl bool			isQuadList()			const;

	_finl bool			isIndexed()				const;

	//  setters
	_finl void			setVertexFormat( VertexFormat vf ); 
	_finl void			setPriType( PrimitiveType pt );
	_finl void			setNVert( int n ); 
	_finl void			setNPri( int n ); 
	_finl void			setNInd( int n );
	_finl void			incNVert( int n ); 
	_finl void			incNPri( int n ); 
	_finl void			incNInd( int n );
    _finl void			setMaxVert( int n );
	_finl void			setMaxInd( int n );
	_finl void			setShader( WORD shader );
	_finl void			setTexture( WORD tex, int stage = 0 );

	_finl void			setIndices( WORD* idxBuf, int numIdx );
	_finl void			setVertices( void* vert, int numVert );
	
	_finl void			setTexCached( bool isCached );
	_finl void			setIBCached( bool isCached );
	_finl void			setVBCached( bool isCached );
	_finl void			setIsStatic( bool isStatic );
	_finl void			setIsQuadList( bool isQList );

	_finl void			setIBPos( int pos );
	_finl void			setVBPos( int pos );

	_finl void			setVertexPtr( void* ptr );
	_finl void			setIndexPtr( WORD* ptr );

	void				rotate( const Vector3D& axis, float angle );
	void				transform( const Matrix4D& transf );
	void				transform( const Matrix4D& transf, int firstVert, int numVert );
	void				transform( const Matrix3D& transf );

	float				distance( const Vector3D& pt, const Vector3D& dir );

	int					getSizeBytes() const;

	void				setDiffuseColor( DWORD color );


	void				create( int numVert, 
								int numInd, 
								VertexFormat vformat = vf2Tex, 
		  						PrimitiveType typePri = ptTriangleList,
								bool sharedBuffer = false );

	//  construction tools
	void				createDiamond( const Vector3D& orig, float side );
	void				createLine( const Vector3D& orig, const Vector3D& dest );
	void				createSphere( const Vector3D& center, 
									  float R, 
									  int NSectors );
	void				createQuadArray( int nQuads );
	void				createGrid( Vector3D& lt, 
									Vector3D& rt, 
									Vector3D& lb,
									Vector3D& rb,
									int cellsX, 
									int cellsY );

	void				createQuad( Vector3D& lt, 
									 Vector3D& rt, 
									 Vector3D& lb,
									 Vector3D& rb );

	void				createPrQuad(	float x, float y,
										float w, float h,
                                        float z = 0.01f );

	void				createEnclosingFrame( float cx, float cy, float cz, float hw, float hh );
    void                createFrame( float x, float y, float z, float w, float h = 0.0f );

    void				createTriangle();

	void				createUnitCube( float side );
	void				createAABB( const AABoundBox& aabb );

    void                createCylinder( const Cylinder& cyl, int nSeg );

	void				createFrustum( const Frustum& fru );



	bool				SaveToFile( FILE* fp );
	bool				LoadFromFile( FILE* fp );

	bool				calcNormals();
	bool				convertVF( VertexFormat to );

	static _inl int		calcNumPri( PrimitiveType pType, int indNum );
	static _inl int		calcNumIdx( PrimitiveType pType, int priNum );


	_inl int			numPriToIdx( int priNum );
	_inl int			numIdxToPri( int indNum );


	float				pickPoly(	const Vector3D& orig,
									const Vector3D& dir,
									int&   triIdx );

	float               pickVertex(	const Cylinder& cyl,
                                    int&            vertIdx );

    void				dump();

	BaseMesh&			operator +=( const BaseMesh& bm );

protected:
	WORD						numPri;		//  number of pimitives (triangles, lines)
	

	void*						vbuf;
	WORD						vertNum;
	WORD						maxVertNum;
	
	WORD*						ibuf;		
	int							indNum;
	int							maxIndNum;

	BYTE						flags;		//  cached flags, static/dynamic etc

	int							vbPos;		//  position in the VB
	int							ibPos;		//  position in the IB
	void*						devHandle;						

	int							texID[3];
	int							shaderID;

	unsigned char				_vertFormat;
	unsigned char				_priType;
	
}; // class BaseMesh

#ifdef _INLINES
#include "gpMesh.inl"
#endif // _INLINES

#endif // __GPMESH_H__