/*****************************************************************
/*  File:   mMath3D.cpp                                          *
/*  Author: Silver, Copyright (C) GSC Game World                 *
/*  Date:   January 2002                                         *
/*****************************************************************/
#include "stdafx.h"
#include "mMath3D.h"

#ifndef _INLINES
#include "mMath3D.inl"
#endif // !_INLINES


Plane					Plane::xOz = { 0.0f, 1.0f, 0.0f, 0.0f };
Plane					Plane::yOz = { 1.0f, 0.0f, 0.0f, 0.0f };
Plane					Plane::xOy = { 0.0f, 0.0f, 1.0f, 0.0f };

WORD					Quad3D::idx[6]		= { 0, 1, 2, 2, 1, 3 };
WORD					Quad3D::idxccw[6]	= { 0, 2, 1, 1, 2, 3 };

const Quaternion		Quaternion::identity( 0.0f, 0.0f, 0.0f, 1.0f );

inline float rand( int min, int max )
{
    float rndval = static_cast<float>( rand() );
    rndval /= (float)RAND_MAX;
	rndval *= max - min;
	rndval += min;
	return rndval;
}

/*****************************************************************
/*	Vector3D implementation
/*****************************************************************/
const	Vector3D Vector3D::oX( 1.0f, 0.0f, 0.0f );
const	Vector3D Vector3D::oY( 0.0f, 1.0f, 0.0f );
const	Vector3D Vector3D::oZ( 0.0f, 0.0f, 1.0f );

void Vector3D ::Dump()
{
	Log.Message( "x: %f. y: %f. z: %f.", x, y, z );
}

void Vector3D::txtSave( FILE* fp, const char* name )
{
	if (name)
	{
		fprintf( fp, "%s={ %-8.8f, %-8.8f, %-8.8f }\n", name, x, y, z );
	}
	else
	{
		fprintf( fp, "={ %-8.8f, %-8.8f, %-8.8f }\n", x, y, z );
	}
} // Vector3D::txtSave

/*---------------------------------------------------------------*
/*  Func:	Vector3D::splineCatmullRom
/*	Desc:	Calculates point on the Catmull-Rom spline
/*	Parm:	t - interp parameter
/*			p0, p1, p2, p3 - interp points
/*---------------------------------------------------------------*/
void Vector3D::splineCatmullRom( float t,
									const Vector3D& p0,
									const Vector3D& p1,
									const Vector3D& p2,
									const Vector3D& p3 )
{
	float t2 = t * t;
	float t3 = t2 * t;

	x = 0.5f * (	2 * p1.x									+ 
					(-p0.x + p2.x)						* t		+
					(2*p0.x - 5*p1.x + 4*p2.x - p3.x)	* t2	+
					(-p0.x + 3*p1.x - 3*p2.x + p3.x)	* t3 );
	
	y = 0.5f * (	2 * p1.y									+ 
					(-p0.y + p2.y)						* t		+
					(2*p0.y - 5*p1.y + 4*p2.y - p3.y)	* t2	+
					(-p0.y + 3*p1.y - 3*p2.y + p3.y)	* t3 );
	
	z = 0.5f * (	2 * p1.z									+ 
					(-p0.z + p2.z)						* t		+
					(2*p0.z - 5*p1.z + 4*p2.z - p3.z)	* t2	+
					(-p0.z + 3*p1.z - 3*p2.z + p3.z)	* t3 );
	
}//  Vector3D::splineCatmullRom


/*****************************************************************
/*	Vector4D implementation
/*****************************************************************/
const	Vector4D Vector4D::oX( 1.0f, 0.0f, 0.0f );
const	Vector4D Vector4D::oY( 0.0f, 1.0f, 0.0f );
const	Vector4D Vector4D::oZ( 0.0f, 0.0f, 1.0f );

Vector4D::Vector4D() : x(0.0f), y(0.0f), z(0.0f), w(1.0f)
{}

Vector4D::Vector4D(  const float _x,
					 const float _y, 
					 const float _z )
{
	x = _x; y = _y; z = _z; w = 1.0f;
}

Vector4D::Vector4D(  const float _x,
					 const float _y, 
					 const float _z,
					 const float _w )
{
	x = _x; y = _y; z = _z; w = _w;
}

Vector4D::Vector4D( const Vector4D& orig )
{
    x = orig.x;
    y = orig.y;
    z = orig.z;
    w = orig.w;
}

Vector4D::Vector4D( const Vector3D& orig )
{
    x = orig.x;
    y = orig.y;
    z = orig.z;
    w = 1.0f;
}

void Vector4D::txtSave( FILE* fp )
{
	fprintf( fp, "={ %-8.8f, %-8.8f, %-8.8f, %-8.8f }\n", x, y, z, w );
}

/*****************************************************************
/*	Matrix4D implementation
/*****************************************************************/
const Matrix4D	Matrix4D::identity(  1.0f, 0.0f, 0.0f, 0.0f,
									 0.0f, 1.0f, 0.0f, 0.0f,
									 0.0f, 0.0f, 1.0f, 0.0f,
									 0.0f, 0.0f, 0.0f, 1.0f );

Matrix4D::Matrix4D(	float _e00, float _e01, float _e02, float _e03,
					float _e10, float _e11, float _e12, float _e13,
					float _e20, float _e21, float _e22, float _e23,
					float _e30, float _e31, float _e32, float _e33 )
{
	e00 = _e00; e01 = _e01; e02 = _e02; e03 = _e03;
	e10 = _e10; e11 = _e11; e12 = _e12; e13 = _e13;
	e20 = _e20; e21 = _e21; e22 = _e22; e23 = _e23;
	e30 = _e30; e31 = _e31; e32 = _e32; e33 = _e33;
}

Matrix4D::Matrix4D()
{
	setNull();
}

void Matrix4D::txtSave( FILE* fp, const char* name ) const
{
	if (name)
	{
		fprintf( fp,	"%s={\n{%-8.8f, %-8.8f, %-8.8f, %-8.8f}, \n"
						"{%-8.8f, %-8.8f, %-8.8f, %-8.8f}, \n"
						"{%-8.8f, %-8.8f, %-8.8f, %-8.8f}, \n"
						"{%-8.8f, %-8.8f, %-8.8f, %-8.8f} \n}\n",
						name,
						e00, e01, e02, e03, 
						e10, e11, e12, e13, 
						e20, e21, e22, e23,
						e30, e31, e32, e33 );
	}
	else
	{
		fprintf( fp,	"%s={\n{%-8.8f, %-8.8f, %-8.8f, %-8.8f}, \n"
						"{%-8.8f, %-8.8f, %-8.8f, %-8.8f}, \n"
						"{%-8.8f, %-8.8f, %-8.8f, %-8.8f}, \n"
						"{%-8.8f, %-8.8f, %-8.8f, %-8.8f} \n}\n",
						name,
						e00, e01, e02, e03, 
						e10, e11, e12, e13, 
						e20, e21, e22, e23,
						e30, e31, e32, e33 );
	}
} // Matrix4D::txtSave

void Matrix4D::random( float minV, float maxV )
{
	float* buf = getBuf();
	for (int i = 0; i < 16; i++)
	{
		buf[i] = rndValuef( minV, maxV );
	}
}

/*---------------------------------------------------------------*
/*  Func:	Matrix4D::inverse
/*	Desc:	Finds 4x4 matrix inverse using Cramer's rule
/*	Parm:	orig - original matrix
/*	Ret:	determinant value
/*---------------------------------------------------------------*/
float Matrix4D::inverse( const Matrix4D& orig )
{
	float tmp[12]; 
	float src[16]; 
	float* mat = orig.getBuf();
	float* dst = getBuf();
	float det = 0.0f;

	for ( int i = 0; i < 4; i++) 
	{
		src[i		]	= mat[i*4		];
		src[i + 4	]	= mat[i*4 + 1	];
		src[i + 8	]	= mat[i*4 + 2	];
		src[i + 12	]	= mat[i*4 + 3	];
	}
	
	//  co-factors
	tmp[0] = src[10] * src[15];
	tmp[1] = src[11] * src[14];
	tmp[2] = src[9]	 * src[15];
	tmp[3] = src[11] * src[13];
	tmp[4] = src[9]  * src[14];
	tmp[5] = src[10] * src[13];
	tmp[6] = src[8]  * src[15];
	tmp[7] = src[11] * src[12];
	tmp[8] = src[8]  * src[14];
	tmp[9] = src[10] * src[12];
	tmp[10] = src[8] * src[13];
	tmp[11] = src[9] * src[12];
	
	dst[0] =	tmp[0] * src[5] + tmp[3] * src[6] + tmp[4]  * src[7];
	dst[0] -=	tmp[1] * src[5] + tmp[2] * src[6] + tmp[5]  * src[7];
	dst[1] =	tmp[1] * src[4] + tmp[6] * src[6] + tmp[9]  * src[7];
	dst[1] -=	tmp[0] * src[4] + tmp[7] * src[6] + tmp[8]  * src[7];
	dst[2] =	tmp[2] * src[4] + tmp[7] * src[5] + tmp[10] * src[7];
	dst[2] -=	tmp[3] * src[4] + tmp[6] * src[5] + tmp[11] * src[7];
	dst[3] =	tmp[5] * src[4] + tmp[8] * src[5] + tmp[11] * src[6];
	dst[3] -=	tmp[4] * src[4] + tmp[9] * src[5] + tmp[10] * src[6];
	dst[4] =	tmp[1] * src[1] + tmp[2] * src[2] + tmp[5]  * src[3];
	dst[4] -=	tmp[0] * src[1] + tmp[3] * src[2] + tmp[4]  * src[3];
	dst[5] =	tmp[0] * src[0] + tmp[7] * src[2] + tmp[8]  * src[3];
	dst[5] -=	tmp[1] * src[0] + tmp[6] * src[2] + tmp[9]  * src[3];
	dst[6] =	tmp[3] * src[0] + tmp[6] * src[1] + tmp[11] * src[3];
	dst[6] -=	tmp[2] * src[0] + tmp[7] * src[1] + tmp[10] * src[3];
	dst[7] =	tmp[4] * src[0] + tmp[9] * src[1] + tmp[10] * src[2];
	dst[7] -=	tmp[5] * src[0] + tmp[8] * src[1] + tmp[11] * src[2];
	
	tmp[0]	= src[2] * src[7];
	tmp[1]	= src[3] * src[6];
	tmp[2]	= src[1] * src[7];
	tmp[3]	= src[3] * src[5];
	tmp[4]	= src[1] * src[6];
	tmp[5]	= src[2] * src[5];
	tmp[6]	= src[0] * src[7];
	tmp[7]	= src[3] * src[4];
	tmp[8]	= src[0] * src[6];
	tmp[9]	= src[2] * src[4];
	tmp[10] = src[0] * src[5];
	tmp[11] = src[1] * src[4];
	
	dst[8] =	tmp[0]  * src[13] + tmp[3] * src[14] + tmp[4] * src[15];
	dst[8] -=	tmp[1]  * src[13] + tmp[2] * src[14] + tmp[5] * src[15];
	dst[9] =	tmp[1]  * src[12] + tmp[6] * src[14] + tmp[9] * src[15];
	dst[9] -=	tmp[0]  * src[12] + tmp[7] * src[14] + tmp[8] * src[15];
	dst[10] =	tmp[2]  * src[12] + tmp[7] * src[13] + tmp[10] * src[15];
	dst[10]-=	tmp[3]  * src[12] + tmp[6] * src[13] + tmp[11] * src[15];
	dst[11] =	tmp[5]  * src[12] + tmp[8] * src[13] + tmp[11] * src[14];
	dst[11]-=	tmp[4]  * src[12] + tmp[9] * src[13] + tmp[10] * src[14];
	dst[12] =	tmp[2]  * src[10] + tmp[5] * src[11] + tmp[1] * src[9];
	dst[12]-=	tmp[4]  * src[11] + tmp[0] * src[9] + tmp[3] * src[10];
	dst[13] =	tmp[8]  * src[11] + tmp[0] * src[8] + tmp[7] * src[10];
	dst[13]-=	tmp[6]  * src[10] + tmp[9] * src[11] + tmp[1] * src[8];
	dst[14] =	tmp[6]  * src[9]  + tmp[11] * src[11] + tmp[3] * src[8];
	dst[14]-=	tmp[10] * src[11] + tmp[2]  * src[8] + tmp[7] * src[9];
	dst[15] =	tmp[10] * src[10] + tmp[4]  * src[8] + tmp[9] * src[9];
	dst[15]-=	tmp[8]  * src[9]  + tmp[11] * src[10] + tmp[5] * src[8];
	
	//  determinant
	det=src[0] * dst[0]+src[1] * dst[1]+src[2] * dst[2]+src[3] * dst[3];
	//  matrix inverse 
	float idet = 1.0f;
	if (det >= c_SmallEpsilon) idet = 1.0f / det;
	for ( int j = 0; j < 16; j++) dst[j] *= idet;

    return det;
}

/*****************************************************************************/
/*	Matrix3D implementation
/*****************************************************************************/
void Matrix3D::random( float minV, float maxV )
{
	float* buf = getBuf();
	for (int i = 0; i < 9; i++)
	{
		buf[i] = rndValuef( minV, maxV );
	}
} // Matrix3D::random

/*****************************************************************
/*	Frustum implementation
/*****************************************************************/
Frustum::Frustum()
{
}

/*---------------------------------------------------------------*
/*  Func:	Frustum::Calculate
/*	Desc:	updates view frustum
/*	Parm:	pMatView - pointer to view matrix
/*			pMatProj - pointer to projection matrix
/*	FIXME:	Fix me totally
/*---------------------------------------------------------------*/
void Frustum::calculate(	const Matrix4D& pMatView,
							const Matrix4D& pMatProj )
{
	//  find projection space to world transformation
    Matrix4D prToWorld, M;
	M.mul( pMatView, pMatProj );
    prToWorld.inverse( M );

    vector[0].set( -1.0f, -1.0f,  0.0f ); // xyz
    vector[1].set(  1.0f, -1.0f,  0.0f ); // Xyz
    vector[2].set( -1.0f,  1.0f,  0.0f ); // xYz
    vector[3].set(  1.0f,  1.0f,  0.0f ); // XYz
    vector[4].set( -1.0f, -1.0f,  1.0f ); // xyZ
    vector[5].set(  1.0f, -1.0f,  1.0f ); // XyZ
    vector[6].set( -1.0f,  1.0f,  1.0f ); // xYZ
    vector[7].set(  1.0f,  1.0f,  1.0f ); // XYZ

    Vector4D tmp;
	for (int i = 0; i < 8; i++ )
	{
        tmp.mul( vector[i], prToWorld );
		tmp.normW();
		vector[i].copy( tmp );
	}

	plane[0].from3Points( vector[0], vector[1], vector[2] ); // Near
	plane[1].from3Points( vector[6], vector[7], vector[5] ); // Far
	plane[2].from3Points( vector[2], vector[6], vector[4] ); // Left
	plane[3].from3Points( vector[7], vector[3], vector[5] ); // Right
	plane[4].from3Points( vector[2], vector[3], vector[6] ); // Top
	plane[5].from3Points( vector[1], vector[0], vector[4] ); // Bottom

	//  determine aabb p- and n- points coord indices
	for (int i = 0; i < 6; i++)
		aabbMask[i].fromPlaneNormal( plane[i].a, plane[i].b, plane[i].c );

} //  Frustum::Calculate

/*---------------------------------------------------------------*
/*  Func:	FloodQuad
/*	Desc:	2D quad flood filling 
/*	Parm:	nl, nr, fl, fr - near left, near right, far left & 
/*				far right quad vertices 
/*			res - array to store points coords
/*	Ret:	number of points set
/*---------------------------------------------------------------*/
int FloodQuad(  const Vector2Di& nl, const Vector2Di& nr,
				const Vector2Di& fl, const Vector2Di& fr,
				Vector2Di* res )
{
	Vector2Di beg, end;

	Line2Di flr( fr, fl );
	Line2Di nlr( nr, nl );
	Line2Di lnf( fl, nl );
	Line2Di rnf( fr, nr );

	beg.x = mini( nl.x, nr.x, fl.x, fr.x );
	beg.y = mini( nl.y, nr.y, fl.y, fr.y );

	end.x = maxi( nl.x, nr.x, fl.x, fr.x );
	end.y = maxi( nl.y, nr.y, fl.y, fr.y );

	Vector2Di cur;
	int idx = 0;
	for (int i = beg.x; i <= end.x; i++)
	{
		for (int j = beg.y; j <= end.y; j++)
		{
			cur.x = i;
			cur.y = j; 
			if (flr.oneSide( cur, nl ) &&
				nlr.oneSide( cur, fl ) &&
				lnf.oneSide( cur, nr ) &&
				rnf.oneSide( cur, nl ))
			//  if point is inside quad
			{
				res[idx].x = i;
				res[idx].y = j;
				idx++;
			}
		}
	}

	return idx;
}//  FloodQuad


/*****************************************************************
/*  AABoundBox implementation                                    *
/*****************************************************************/
AABoundBox::AABoundBox()
{
}

AABoundBox::AABoundBox( const Vector3D& _minv, const Vector3D& _maxv )
{
	minv.copy( _minv );
	maxv.copy( _maxv );
}

void AABoundBox::txtSave( FILE* fp )
{
	fprintf( fp, "minv(%-2.4f %-2.4f %-2.4f); maxv(%-2.4f %-2.4f %-2.4f); ", 
				minv.x, minv.y, minv.z, maxv.x, maxv.y, maxv.z );
}

void AABoundBox::random(	const Vector3D& minpt,
							const Vector3D& maxpt,
							const Vector3D& maxdim )
{
	minv.x = rand( minpt.x, maxpt.x );
	minv.y = rand( minpt.y, maxpt.y );
	minv.z = rand( minpt.z, maxpt.z );

	maxv.copy( minv );

	maxv.x += rand( 0.0f, maxdim.x );
	maxv.y += rand( 0.0f, maxdim.y );
	maxv.z += rand( 0.0f, maxdim.z );
} // AABoundBox::random

/*---------------------------------------------------------------------------*/
/*	Func:	AABoundBox::transform
/*	Desc:	Finds axis-aligned bounding box after object's transformation
/*	Parm:	matr - transform matrix
/*---------------------------------------------------------------------------*/
void AABoundBox::transform( const Matrix4D& matr )
{
	Vector3D nmin, nmax;
	Vector3D vc[8];
	getCorners( vc );
	vc[0] *= matr;
	minv.copy( vc[0] );
	maxv.copy( minv );
	for (int i = 1; i < 8; i++)
	{
		//  transform corner
		vc[i] *= matr;
		
		if (minv.x > vc[i].x) minv.x = vc[i].x;
		if (maxv.x < vc[i].x) maxv.x = vc[i].x;

		if (minv.y > vc[i].y) minv.y = vc[i].y;
		if (maxv.y < vc[i].y) maxv.y = vc[i].y;

		if (minv.z > vc[i].z) minv.z = vc[i].z;
		if (maxv.z < vc[i].z) maxv.z = vc[i].z;
	}
} // AABoundBox::transform

/*---------------------------------------------------------------------------*/
/*	Func:	AABoundBox::getCorners	
/*	Desc:	returns coordinates of all AABB corners
/*	Parm:	c - reference to array of 8 vectors
/*---------------------------------------------------------------------------*/
void AABoundBox::getCorners( Vector3D (&c)[8] ) const
{
	c[0].set( minv.x, minv.y, minv.z );
    c[1].set( maxv.x, minv.y, minv.z );
    c[2].set( minv.x, maxv.y, minv.z );
    c[3].set( maxv.x, maxv.y, minv.z );
    c[4].set( minv.x, minv.y, maxv.z );
    c[5].set( maxv.x, minv.y, maxv.z );
    c[6].set( minv.x, maxv.y, maxv.z );
    c[7].set( maxv.x, maxv.y, maxv.z );
} // AABoundBox::getCorners

/*****************************************************************
/*  Cylinder implementation                                      *
/*****************************************************************/
Cylinder::Cylinder()
{

}

Cylinder::Cylinder(	const Vector3D& bottom,
				    const Vector3D& top,
                    float R2 )
{
    create( bottom, top, R2 );
}

Cylinder::Cylinder( const Cylinder& orig )
{
    r2 = orig.r2;
    base.copy( orig.base );
    core.copy( orig.core );
}

/*****************************************************************************/
/*	Line3D implementation
/*****************************************************************************/
Line3D::Line3D( const Vector3D& base, const Vector3D& direction )
{
	orig.copy( base );
	dir.copy( direction );
}






