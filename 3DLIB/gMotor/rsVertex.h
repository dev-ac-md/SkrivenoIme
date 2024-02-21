/*****************************************************************
/*  File:   Vertex.h                                             
/*	Desc:	Set of vertex formats
/*  Date:   November 2001                                        
/*  Modify: Feb 2002, Silver
/*****************************************************************/
#ifndef __S_CUSTOMVERTEX_H__
#define __S_CUSTOMVERTEX_H__
#pragma	once
#include "gmDefines.h"

enum VertexFormat
{
	vfUnknown		= 0,
	vfTnL			= 1,
	vf2Tex			= 2,
	vfN				= 3,
	vfTnL2			= 4,
	vfT				= 5,
	vfMP1			= 6,
	vfNMP1			= 7,
	vfTnL2S			= 8,
	vfNMP2			= 9,
	vfNMP3			= 10,
	vfNMP4			= 11
};  // enum VertexFormat

const int c_NumVertexTypes = 11;

const int c_vfStride[] = 
{
	0,	// vfUnknown
	32, // vfTnL
	32, // vf2Tex
	32, // vfN
	36, // vfTnL2
	20, // vfT
	36, // vfMP1			
	44, // vfNMP1
	40, // vfTnL2S
	48, // vfNMP2
	52, // vfNMP3
	56  // vfNMP4
};
	

class AABoundBox;

/*****************************************************************
/*	Class:	Vertex
/*  Desc:	Generic vertex class - serves as different vertex
/*				types factory.                                              
/*****************************************************************/
class Vertex{
public:
	static int		CalcStride( VertexFormat vf );
	static void*	CreateVBuf( VertexFormat vf, int numVert );
	static void		Dump( void* buf, int numVert, VertexFormat vf );
};

class Vector3D;

/*****************************************************************************/
/*	Class:	Vertex32
/*	Desc:	This class is a hack for working with 32-bytes vertices
/*				in vertex coordinates mode only
/*	TODO:	overload casting operators to make it not a hack
/*****************************************************************************/
class Vertex32
{
public:
	float	x, y, z;

private:
	DWORD	dummy[5];
}; // class Vertex32

/*****************************************************************
/*  Class:  Vertex2t											 
/*  Desc:	Vertex with diffuse color and 2 pairs of texture coors
/*****************************************************************/
class Vertex2t : public Vertex
{
public:
							Vertex2t();

	float					x;
	float					y;
	float					z;

	DWORD					diffuse;	//  diffuse vertex color
	float					u, v;		//  1st texture coordinates
	float					u2, v2;		//  2nd texture coordinates

	const Vertex2t& operator =(const Vector3D& vec);

	void Dump( bool full = true );

	void setXYZ( float _x, float _y, float _z )
	{
		x = _x; y = _y; z = _z; 
	}

	void setXYZUV( float _x, float _y, float _z, float _u, float _v )
	{
		x = _x; y = _y; z = _z; 
		u = _u;
		v = _v;
	}

	void setDiffuse( DWORD color )
	{
		diffuse = color;
	}

	static int				getStride() { return 32;		}
	static VertexFormat		format()	{ return vf2Tex;	}
};  // class Vertex2t

/*****************************************************************
/*  Class:  VertexMP1											 
/*  Desc:	Vertex used in matrix palette blending, vertex is
/*				bound to single bone
/*****************************************************************/
class VertexMP1 : public Vertex
{
public:
							VertexMP1();

	float					x;
	float					y;
	float					z;

	DWORD					matrIdx;	//  index of the corresponding transform matrix
										//  in palette
	DWORD					diffuse;	//  diffuse vertex color
	float					u, v;		//  1st texture coordinates
	float					u2, v2;		//  2nd texture coordinates
	

	const Vertex2t& operator =(const Vector3D& vec);

	void Dump( bool full = true );

	void setXYZ( float _x, float _y, float _z )
	{
		x = _x; y = _y; z = _z; 
	}

	void setXYZUV( float _x, float _y, float _z, float _u, float _v )
	{
		x = _x; y = _y; z = _z; 
		u = _u;
		v = _v;
	}

	void setDiffuse( DWORD color )
	{
		diffuse = color;
	}

	static int				getStride() { return 36;		}
	static VertexFormat		format()	{ return vfMP1;		}
};  // class VertexMP1

/*****************************************************************
/*  Class:  VertexNMP1											 
/*  Desc:	Vertex used in matrix palette blending, vertex is
/*				bound to single bone, also normal is included
/*****************************************************************/
class VertexNMP1 : public Vertex
{
public:
							VertexNMP1();

	float					x;
	float					y;
	float					z;

	DWORD					matrIdx;	//  index of the corresponding transform matrix
										//  in palette
	float					nx;
	float					ny;
	float					nz;

	DWORD					diffuse;	//  diffuse vertex color
	float					u, v;		//  1st texture coordinates
	float					u2, v2;		//  2nd texture coordinates
	

	const Vertex2t& operator =(const Vector3D& vec);

	void Dump( bool full = true );

	void setXYZ( float _x, float _y, float _z )
	{
		x = _x; y = _y; z = _z; 
	}

	void setXYZUV( float _x, float _y, float _z, float _u, float _v )
	{
		x = _x; y = _y; z = _z; 
		u = _u;
		v = _v;
	}

	void setDiffuse( DWORD color )
	{
		diffuse = color;
	}

	static int				getStride() { return c_vfStride[(int)vfNMP1]; }
	static VertexFormat		format()	{ return vfNMP1;	}
};  // class VertexNMP1

/*****************************************************************
/*  Class:  VertexNMP2											 
/*  Desc:	Vertex used in matrix palette blending, vertex is
/*				bound to 2 bones, also normal is included
/*****************************************************************/
class VertexNMP2 : public Vertex
{
public:
							VertexNMP2();

	float					x;
	float					y;
	float					z;

	float					weight[1];
	DWORD					matrIdx;	//  index of the corresponding transform matrix
										//  in palette
	float					nx;
	float					ny;
	float					nz;

	DWORD					diffuse;	//  diffuse vertex color
	float					u, v;		//  1st texture coordinates
	float					u2, v2;		//  2nd texture coordinates
	

	void Dump( bool full = true );

	void setXYZ( float _x, float _y, float _z )
	{
		x = _x; y = _y; z = _z; 
	}

	void setXYZUV( float _x, float _y, float _z, float _u, float _v )
	{
		x = _x; y = _y; z = _z; 
		u = _u;
		v = _v;
	}

	void setDiffuse( DWORD color )
	{
		diffuse = color;
	}

	static int				getStride() { return c_vfStride[(int)vfNMP2]; }
	static VertexFormat		format()	{ return vfNMP2;	}
};  // class VertexNMP2

/*****************************************************************
/*  Class:  VertexNMP3											 
/*  Desc:	Vertex used in matrix palette blending, vertex is
/*				bound to 3 bones, also normal is included
/*****************************************************************/
class VertexNMP3 : public Vertex
{
public:
							VertexNMP3();

	float					x;
	float					y;
	float					z;

	float					weight[2];
	DWORD					matrIdx;	//  index of the corresponding transform matrix
										//  in palette
	float					nx;
	float					ny;
	float					nz;

	DWORD					diffuse;	//  diffuse vertex color
	float					u, v;		//  1st texture coordinates
	float					u2, v2;		//  2nd texture coordinates
	

	const Vertex2t& operator =(const Vector3D& vec);

	void Dump( bool full = true );

	void setXYZ( float _x, float _y, float _z )
	{
		x = _x; y = _y; z = _z; 
	}

	void setXYZUV( float _x, float _y, float _z, float _u, float _v )
	{
		x = _x; y = _y; z = _z; 
		u = _u;
		v = _v;
	}

	void setDiffuse( DWORD color )
	{
		diffuse = color;
	}

	static int				getStride() { return c_vfStride[(int)vfNMP3]; }
	static VertexFormat		format()	{ return vfNMP3;	}
};  // class VertexNMP3

/*****************************************************************
/*  Class:  VertexNMP4										 
/*  Desc:	Vertex used in matrix palette blending, vertex is
/*				bound to 4 bones, also normal is included
/*****************************************************************/
class VertexNMP4 : public Vertex
{
public:
							VertexNMP4();

	float					x;
	float					y;
	float					z;

	float					weight[3];
	DWORD					matrIdx;	//  index of the corresponding transform matrix
										//  in palette
	float					nx;
	float					ny;
	float					nz;

	DWORD					diffuse;	//  diffuse vertex color
	DWORD					specular;	//  specular vertex color
	float					u, v;		//  1st texture coordinates
	float					u2, v2;		//  2nd texture coordinates
	

	const VertexNMP4& operator =(const Vector3D& vec);

	void Dump( bool full = true );

	void setXYZ( float _x, float _y, float _z )
	{
		x = _x; y = _y; z = _z; 
	}

	void setXYZUV( float _x, float _y, float _z, float _u, float _v )
	{
		x = _x; y = _y; z = _z; 
		u = _u;
		v = _v;
	}

	void setDiffuse( DWORD color )
	{
		diffuse = color;
	}

	static int				getStride() { return c_vfStride[(int)vfNMP4]; }
	static VertexFormat		format()	{ return vfNMP4;	}
};  // class VertexNMP4

/*****************************************************************
/*  Class:  VertexN											 
/*  Desc:	Vertex with normal and pair of tex coords
/*****************************************************************/
class VertexN : public Vertex
{
public:
							VertexN();

	float					x;
	float					y;
	float					z;

	float					nx;
	float					ny;
	float					nz;

	float					u, v;		//  1st texture coordinates
	void Dump( bool full = true );


	VertexN&				operator =( const Vector3D& vec ); 

	void setXYZ( float _x, float _y, float _z )
	{
		x = _x; y = _y; z = _z; 
	}

	void setXYZUV( float _x, float _y, float _z, float _u, float _v )
	{
		x = _x; y = _y; z = _z; 
		u = _u;
		v = _v;
	}

	static int				getStride() { return 32;	}
	static VertexFormat		format()	{ return vfN;	}
};  // class VertexN

/*****************************************************************
/*  Class:  VertexT											 
/*  Desc:	XYZUV
/*****************************************************************/
class VertexT : public Vertex
{
public:
							VertexT();

	float					x;
	float					y;
	float					z;

	float					u;
	float					v;

	void Dump( bool full = true );

	void setXYZ( float _x, float _y, float _z )
	{
		x = _x; y = _y; z = _z; 
	}

	void setXYZUV( float _x, float _y, float _z, float _u, float _v )
	{
		x = _x; y = _y; z = _z; 
		u = _u;
		v = _v;
	}

	static int				getStride() { return 20;	}
	static VertexFormat		format()	{ return vfT;	}
};  // class VertexT

/*****************************************************************
/*  Class:  VertexTnL		                                     *
/*  Desc:   Already transformed and lit vertex   				 *
/*****************************************************************/
class VertexTnL : public Vertex
{
public:
							VertexTnL();

    float					x, y;		//  x and y coords in screen space
	float					z;			//  depth coordinate
	float					w;			//  reciprocal homogeneous w

    DWORD					diffuse;	//  diffuse color
	DWORD					specular;	//  specular color (actually, the main purpose here
										//    for using specular component is matching 32-byte
										//    custom vertex size for efficient GPU caching)

	float					u, v;		//  texture coordinates

	void Dump( bool full = true );

	VertexTnL&				operator =( const Vector3D& vec ); 

	void setXYZ( float _x, float _y, float _z )
	{
		x = _x; y = _y; z = _z; 
	}

	void setXYZW( float _x, float _y, float _z, float _w )
	{
		x = _x; y = _y; z = _z; w = _w; 
	}

	void setXYZUV( float _x, float _y, float _z, float _u, float _v )
	{
		x = _x; y = _y; z = _z; 
		u = _u;
		v = _v;
	}

	void setDiffuse( DWORD color )
	{
		diffuse = color;
	}

	static int				getStride() { return 32;	}
	static VertexFormat		format()	{ return vfTnL;	}
}; // class VertexTnL

/*****************************************************************
/*  Class:  VertexTnL2		                                     *
/*  Desc:   Already transformed and lit vertex   				 *
/*****************************************************************/
class VertexTnL2 : public Vertex
{
public:
    float					x, y;		//  x and y coords in screen space
	float					z;			//  depth coordinate
	float					w;			//  reciprocal homogeneous w

    DWORD					diffuse;	//  diffuse color
	float					u, v;		//  texture coordinates
	float					u2, v2;		//  texture coordinates

	void setXYZ( float _x, float _y, float _z )
	{
		x = _x; y = _y; z = _z; 
	}

	void setXYZUV( float _x, float _y, float _z, float _u, float _v )
	{
		x = _x; y = _y; z = _z; 
		u = _u;
		v = _v;
	}

	void setXYZUV( float _x, float _y, float _z, float _u, float _v, float _u2, float _v2 )
	{
		x = _x; y = _y; z = _z; 
		u = _u; v = _v;
		u2 = _u2; v2 = _v2;
	}

	void setDiffuse( DWORD color )
	{
		diffuse = color;
	}
	
	void Dump( bool full = true );

	static int				getStride() { return 36;	}
	static VertexFormat		format()	{ return vfTnL2;	}
}; // class VertexTnL
/*****************************************************************
/*  Class:  VertexTnL2S		                                     *
/*  Desc:   Already transformed and lit vertex   				 *
/*****************************************************************/
class VertexTnL2S : public Vertex
{
public:
    float					x, y;		//  x and y coords in screen space
	float					z;			//  depth coordinate
	float					w;			//  reciprocal homogeneous w

    DWORD					diffuse;	//  diffuse color
	DWORD					specular;	//  diffuse color
	float					u, v;		//  texture coordinates
	float					u2, v2;		//  texture coordinates

	void setXYZ( float _x, float _y, float _z )
	{
		x = _x; y = _y; z = _z; 
	}

	void setXYZUV( float _x, float _y, float _z, float _u, float _v )
	{
		x = _x; y = _y; z = _z; 
		u = _u;
		v = _v;
	}

	void setXYZUV( float _x, float _y, float _z, float _u, float _v, float _u2, float _v2 )
	{
		x = _x; y = _y; z = _z; 
		u = _u; v = _v;
		u2 = _u2; v2 = _v2;
	}

	void setDiffuse( DWORD color )
	{
		diffuse = color;
	}
	
	void Dump( bool full = true );

	static int				getStride() { return 40;	}
	static VertexFormat		format()	{ return vfTnL2S;	}
}; // class VertexTnL
#endif // __S_CUSTOMVERTEX_H__