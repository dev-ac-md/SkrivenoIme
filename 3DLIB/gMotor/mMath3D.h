/*****************************************************************
/*  File:   Math3D.h	                                         *
/*	Desc:	cll math routines									 *
/*  Author: Silver, Copyright (C) GSC Game World                 *
/*  Date:   January 2002                                         *
/*****************************************************************/
#ifndef __GPMATH3D_H__
#define __GPMATH3D_H__
#pragma	once

const float c_Epsilon		= 0.000001f;
const float c_SmallEpsilon	= 0.00000000001f;
const float c_PI			= 3.14159265f;
const float c_DoublePI		= 6.28318530f;
const float c_HalfPI		= 1.57079632f;
const float c_QuarterPI		= 0.78539816f;
const float c_CosPId6		= 0.86602540f;
const float c_PId6			= 0.52359877f;

class Matrix4D;
class Matrix3D;
class Vector4D;
class Vector3D;
class Plane;
class Quaternion;

_inl bool Seg2Seg2X(	float a1x, float a1y, float b1x, float b1y,
						float a2x, float a2y, float b2x, float b2y,
						float& crossX, float& crossY );

_inl float sqdistPt2D( float x1, float y1, float x2, float y2 );


_inl float minf( float a, float b );
_inl float maxf( float a, float b );
_inl float minf( float a, float b, float c, float d );
_inl float maxf( float a, float b, float c, float d );
_inl float minf( float a, float b, float c, float d, float e, float f, float g, float h );
_inl float maxf( float a, float b, float c, float d, float e, float f, float g, float h );

_inl float mini( float a, float b );
_inl float maxi( float a, float b );
_inl float mini( float a, float b, float c, float d );
_inl float maxi( float a, float b, float c, float d );

_inl bool RaySphereX( const Vector3D& orig, const Vector3D& dir, 
					  const Vector3D& center, float R );

_inl bool RayTriangleX(	const Vector3D&	orig, 
						const Vector3D&	dir, 
						const Vector3D&	v0, 
						const Vector3D&	v1, 
						const Vector3D&	v2, 
						float& u, float& v, 
						float& t );

//  simple averaging smoothing 2D filter
template <typename T>
void SmoothFilter( T* arr, int width, int height );

_inl float rndValuef();
_inl float rndValuef( float min, float max );



/*****************************************************************
/*  Class:  Vector3D                                             *
/*  Desc:   3-dimensional vector                                 *
/*****************************************************************/
class Vector3D 			
{
public:
	float			x;
	float			y;
	float			z;

    _inl            Vector3D( const Vector3D& orig );
    _inl            Vector3D( const Vector4D& orig );
	_inl			Vector3D(  float _x = 0.0f,
							   float _y = 0.0f, 
							   float _z = 0.0f );

	_inl void		set( float _x, float _y, float _z );
	_inl void		sub( const Vector3D& v1, const Vector3D& v2 );
	_inl void		add( const Vector3D& v1, const Vector3D& v2 );
	_inl void		cross( const Vector3D& v1, const Vector3D& v2 );
	_inl void		sub( const Vector3D& v );
	_inl void		add( const Vector3D& v );
	_inl void		addWeighted( const Vector3D& v, float weight );


	_inl void		reverse( const Vector3D& orig );
	_inl void		reverse();
	_inl void		random( float minX, float maxX, 
							float minY, float maxY, 
							float minZ, float maxZ );

	_inl float		dot( const Vector3D& v ) const;

	_inl const Vector3D& operator +=( const Vector3D& vec );
	_inl const Vector3D& operator -=( const Vector3D& vec );
	_inl const Vector3D& operator *=( const float val );
	_inl const Vector3D& operator *=( const Matrix3D& m );
	_inl const Vector3D& operator *=( const Matrix4D& m );

	_inl const Vector3D& mulLeft( const Matrix3D& m );
	_inl const Vector3D& mulPt( const Vector3D& v, const Matrix4D& m );
	_inl const Vector3D& mulPt( const Matrix4D& m );
	
	_inl const Vector3D& operator /=( const float val );

	_inl const Vector3D& operator =( const Vector3D& orig );
	_inl const Vector3D& operator =( const Vector4D& orig );

	_inl bool		isEqual( const Vector3D& orig ) const;



	_inl float		norm() const;
	_inl float		norm2() const;
	_inl void		normalize();
	_inl float		distance( const Vector3D& orig ) const;
    _inl float		distance2( const Vector3D& orig ) const;

	_inl void		copy( const Vector3D& orig );
	_inl void		copy( const Vector4D& orig );

	_inl float		TriArea( const Vector3D& v ) const; 

	_inl void		PlaneIntersectLine( Plane& p, 
										const Vector3D& v1,
										const Vector3D& v2 );

	_inl void		planeNormal(	const Vector3D& v1,
									const Vector3D& v2,
									const Vector3D& v3 );

	_inl bool		isColinear( const Vector3D& orig ) const;
	_inl void		zero();

	void			Dump();

	void			splineCatmullRom( float t, 
										const Vector3D& p0,
										const Vector3D& p1,
										const Vector3D& p2,
										const Vector3D& p3 );

	void			txtSave( FILE* fp, const char* name = 0 );

	static  const	Vector3D oX;
	static  const	Vector3D oY;
	static  const	Vector3D oZ;

}; // class Vector3D

/*****************************************************************
/*  Class:  Vector4D                                             *
/*  Desc:   4-dimencional vector							     *
/*****************************************************************/
class Vector4D
{
public:
	float x, y, z, w;

					Vector4D();
                    Vector4D( const Vector4D& orig );
                    Vector4D( const Vector3D& orig );

					Vector4D(  const float _x, 
							   const float _y, 
							   const float _z );

					Vector4D(	const float _x,
								const float _y, 
								const float _z,
								const float _w );

	_inl Vector4D&	mul( const Vector4D& v, const Matrix4D& m );
	_inl Vector4D&	mul( const Matrix4D& m, const Vector4D& v );
	_inl Vector4D&	mul( const Matrix4D& m );
	_inl Vector4D&  transform( const Matrix4D& m );

	_inl void		set( float _x, float _y, float _z );
	_inl void		set( float _x, float _y, float _z, float _w );
	_inl void		sub( const Vector4D& v1, const Vector4D& v2 );
	_inl void		add( const Vector4D& v1, const Vector4D& v2 );
	_inl void		sub( const Vector4D& v );
	_inl void		add( const Vector4D& v );

	_inl float		mul( const Vector4D& v );
	_inl float		dot( const Vector4D& v ) const;

	_inl const Vector4D& operator +=( const Vector4D& vec );
	_inl const Vector4D& operator -=( const Vector4D& vec );
	_inl const Vector4D& operator *=( const float val );
	_inl const Vector4D& operator /=( const float val );
	_inl const Vector4D& operator =( const Vector3D& orig );
	_inl const Vector4D& operator =( const Vector4D& orig );

	_inl const Vector4D& copy( const Vector4D& orig );

	_inl float		norm() const;
	_inl float		norm2() const;
	_inl void		normalize();
	_inl void		normW();

	_inl float		distance( const Vector4D& orig ) const;

	void			txtSave( FILE* fp );


	static const	Vector4D oX;
	static const	Vector4D oY;
	static const	Vector4D oZ;
}; // class Vector4D

/*****************************************************************
/*  Class:  Vector2D                                             *
/*  Desc:   2-dimencional vector							     *
/*****************************************************************/
template <class T>
class Vector2D 			
{
public:
	T				x;
	T				y;

	Vector2D();
	Vector2D( const T _x, const T _y );

	_inl void set( T _x, T _y );
	_inl void sub( const Vector2D& v1, const Vector2D& v2 );
	_inl void sub( const Vector2D<T>& v );
	_inl float dot( const Vector2D<T>& v );

	_inl void copy( const Vector2D& orig );

	_inl T norm() const;
	_inl T norm2() const;
	_inl T dist2( Vector2D<T> r ) const;

	_inl bool inAAQuad( T _x, T _y, T _side ) const;
	_inl void normalize();


	_inl const Vector2D& operator +=( const Vector2D& vec );
	_inl const Vector2D& operator -=( const Vector2D& vec );
	_inl const Vector2D& operator *=( const T val );
	_inl const Vector2D& operator /=( const T val );

	_inl T	triArea( const Vector2D<T>& v );

	_inl void clamp( T minX, T minY, T maxX, T maxY );

	void			Dump();
}; // class Vector2D

/*****************************************************************
/*  Class:  Line2D 	                                             *
/*  Desc:   2D line											     *
/*****************************************************************/
template <class T>
class Line2D 	
{
public:
	Line2D( const Vector2D<T> _a, const Vector2D<T> _b );
	Line2D(float ax, float ay, float bx, float by) { a.x = ax; a.y = ay; b.x = bx; }

	Line2D(){};
	
	_inl void Init( const Vector2D<T> _a, const Vector2D<T> _b );
	_inl bool oneSide( const Vector2D<T> v1, const Vector2D<T> v2 ) const;
	_inl bool inLeftHalf( const Vector2D<T> pt ) const;
	_inl bool inLeftHalf( T _x, T _y ) const;

	Vector2D<T> a, b;
}; // class Line2D

typedef Vector2D<float> Vector2Df;
typedef Vector2D<int>	Vector2Di;
typedef Line2D<int>		Line2Di;
typedef Line2D<float>	Line2Df;

/*****************************************************************
/*  Class:  Matrix4D                                             *
/*  Desc:   4-dimencional matrix							     *
/*****************************************************************/
class Matrix4D
{
public:
	float		e00, e01, e02, e03;
	float		e10, e11, e12, e13;
	float		e20, e21, e22, e23;
	float		e30, e31, e32, e33;

	Matrix4D(	float _e00, float _e01, float _e02, float _e03,
				float _e10, float _e11, float _e12, float _e13,
				float _e20, float _e21, float _e22, float _e23,
				float _e30, float _e31, float _e32, float _e33 );

	Matrix4D();

	_inl Matrix4D&	mul( const Matrix4D& a, const Matrix4D& b );
    float			inverse( const Matrix4D& orig );
	_inl void		affineInverse( const Matrix4D& orig );
	
	_inl const Matrix4D&	operator = ( const Matrix3D& matr );
	_inl const Matrix4D&	operator = ( const Matrix4D& matr );
	_inl Matrix4D&			operator *=( const Matrix4D& r );
	_inl Matrix4D&			operator *=( float w );
	_inl Matrix4D&			operator +=( const Matrix4D& r );

	_inl Matrix4D&			addWeighted( const Matrix4D& r, float w );
	
	_inl void		setIdentity();
	_inl void		setNull();

	_inl float*		getBuf()	const { return (float*)&e00; }
	_inl Vector4D&	getV0()		const { return (Vector4D&)e00; }
	_inl Vector4D&	getV1()		const { return (Vector4D&)e10; }
	_inl Vector4D&	getV2()		const { return (Vector4D&)e20; }
	_inl Vector4D&	getV3()		const { return (Vector4D&)e30; }

	_inl void		translation( float dx, float dy, float dz );
	_inl void		translation( const Vector3D& trans );
	
	_inl void		scaling( float sx, float sy, float sz );
	_inl void		rotation( const Vector3D& axis, float angle );
	_inl void		rotation( const Quaternion& q );
	_inl void		shearing(	float sxy, float sxz, 
								float syx, float syz,
								float szx, float szy );

	_inl void		srt( float scale, const Vector3D& axis, float angle, const Vector3D& pos );
	
	_inl float		quadraticForm( Vector4D& v );
	_inl Matrix4D&	mulVV( const Vector4D& v );

	_inl void		transpose( const Matrix4D& orig );

	void			txtSave( FILE* fp, const char* name = 0 ) const;
	void			random( float minV = 0.0f, float maxV = 1.0f );

	static const	Matrix4D identity;

};  // class Matrix4D

/*****************************************************************
/*  Class:  Matrix3D                                             *
/*  Desc:   3-dimensional matrix							     *
/*****************************************************************/
class Matrix3D
{
public:
	float		e00, e01, e02;
	float		e10, e11, e12;
	float		e20, e21, e22;

	_inl void rotation( const Vector3D& axis, float angle );
	_inl void rotation( const Quaternion& q );

	_inl void rotationFromEuler( float yaw, float pitch, float roll );
	_inl void scaling( float sx, float sy, float sz );

	_inl float*		getBuf()	const { return (float*)&e00; }
	_inl Vector4D&	getV0()		const { return (Vector4D&)e00; }
	_inl Vector4D&	getV1()		const { return (Vector4D&)e10; }
	_inl Vector4D&	getV2()		const { return (Vector4D&)e20; }

	_inl Matrix3D& operator *=( const Matrix3D& r );
	_inl Matrix3D& operator +=( const Matrix3D& r );
	_inl Matrix3D& operator *=( const float f );
	_inl Matrix3D& operator +=( const float f );

	_inl void	mul			( const Matrix3D& l, const Matrix3D& r );
	_inl void	setNull		();
	_inl void	setIdentity	();
	_inl float	inverse		( const Matrix3D& m );
	_inl float	inverse		();
	_inl float	det			() const;
	_inl void	txtSave		( FILE* fp, const char* name ) const;
	_inl void	copy		( const Matrix3D& orig );
	_inl void	copy		( const Matrix4D& orig );
	_inl void	transpose	();
	_inl bool	isOrthonormalBasis() const;

	void		random		( float minV = 0.0f, float maxV = 1.0f );

}; // class Matrix3D

/*****************************************************************
/*  Class:  Quad3D                                               *
/*  Desc:   Describes quad in the 3D space (e.g 3D frame)        *
/*****************************************************************/
class Quad3D
{
public:
	Vector3D		a;
	Vector3D		b;
	Vector3D		c;
	Vector3D		d;

	void _inl		CreateRect( const Vector3D& topLeft, 
								const Vector3D& botRight,
								const Vector3D& botLeft );

	_inl float		getArea() const;

	static WORD		idx[6];
	static WORD		idxccw[6];
}; // class Quad3D

/*****************************************************************
/*  Class:  Quad2D                                               
/*  Desc:   Describes quad in the 2D space 
/*****************************************************************/
template<class T>
class Quad2D
{
public:
	Vector2D<T>		a;
	Vector2D<T>		b;
	Vector2D<T>		c;
	Vector2D<T>		d;

	Quad2D( T x, T y, T side );
	Quad2D(){};
	
	_inl void Init( T ax, T ay, T bx, T by, T cx, T cy, T dx, T dy );
	_inl void Init( Vector2D<T>& _a, Vector2D<T>& _b, Vector2D<T>& _c, Vector2D<T>& _d );
	
	_inl T dist2ToPt( const Vector2D<T> pt ) const;
	_inl bool contains( const Vector2D<T> r ) const;
	_inl bool contains( T _x, T _y ) const;

	_inl bool IsIntersecting( const Quad2D<T> r ) const;
	_inl bool IsIntersecting( T x, T y, T side ) const;

}; // class Quad2D
typedef Quad2D<int>		Quad2Di;
typedef Quad2D<float>	Quad2Df;

/*****************************************************************
/*  Class:  AABoundBox                                           *
/*  Desc:   Presents axis-aligned bounding box description       *
/*****************************************************************/
class AABoundBox{
public:
	AABoundBox();
    AABoundBox( const Vector3D& _minv, const Vector3D& _maxv );

	//  getters
	_inl void		getCenter( Vector3D& dest ) const;
	_inl float		getDiagonal()				const;
	_inl void		Union( const AABoundBox& orig );
	_inl void		copy( const AABoundBox& orig );

	_inl float		distance2( const Vector3D& pt );
	_inl float		distance( const Vector3D& pt );

	_inl bool		intersectRay(			Vector3D& pt,
									const	Vector3D& orig, 
									const	Vector3D& dir ) const;

	void			transform( const Matrix4D& matr );
	void			getCorners( Vector3D (&c)[8] ) const;
	void			txtSave( FILE* fp );

	void			random( const Vector3D& minpt,
							const Vector3D& maxpt,
							const Vector3D& maxdim );

	
	Vector3D		minv;		//  "min" vertex of the bbox
	Vector3D		maxv;		//  "max" vertex of the bbox
};  // class AABoundBox

/*****************************************************************************/
/*	Class:	BBox
/*	Desc:	arbitrarly aligned bounding box
/*****************************************************************************/
class BBox
{
public:
	Vector3D		center;			//  world space center of bounding box
	Vector3D		orientation;	//  direction of local oX in world space
	float			wlx, wly, wlz;	//  widthes along local axis

}; // BBox

enum IntersectStatus{
	isUnknown		= 0,
	isInside		= 1,
	isOutside		= 2,
	isIntersects	= 3
};

/*****************************************************************
/*  Class:  AABBIntersectMask                                    *
/*  Desc:   Intersection mask for AABB-plane intersection test   *
/*****************************************************************/
class AABBIntersectMask
{
public:
	_inl void		fromPlaneNormal( float a, float b, float c );

	BYTE			NMask;		//  mask for far negative point 
								//    (in the plane's normal direction)
	BYTE			PMask;		//  mask for far positive point
};  // class AABBIntersectMask

/*****************************************************************
/*  Class:  Plane                                                *
/*  Desc:   Plane in the 3D space                                *
/*****************************************************************/
class Plane
{
public:
	//  plane equation is 'ax + by + cz + d = 0'

	float		a;
	float		b;
	float		c;

	float		d;

	static Plane		xOz;
	static Plane		yOz;
	static Plane		xOy;

	_inl void
	from3Points(	const Vector3D& v1, 
					const Vector3D& v2,
					const Vector3D& v3 );

	_inl void 
	fromPointNormal(	const Vector3D& pt, 
						const Vector3D& norm );
	
	_inl void 
	fromPointNormal(	const Vector4D& pt, 
						const Vector4D& norm );

	_inl bool intersect( const Plane& p1, const Plane& p2, Vector3D& pt );

	_inl float 
	getZ( float x, float y ) const;

	_inl float 
	getY( float x, float z ) const;

	_inl float 
	getX( float y, float z ) const;

	_inl Vector3D*
	getNormal();

	_inl Vector4D*
	asVector();

	_inl bool isPerpendicular( const Plane& p ) const;

	_inl void	txtSave( FILE* fp );

}; // class Plane

/*****************************************************************
/*  Class:  Quaternion                                           *
/*  Desc:   quaternion class	                                 *
/*****************************************************************/
class Quaternion{
public:
	Vector3D	v;
	float		s;

	Quaternion() : s(0.0f) {};
	Quaternion( float _x, float _y, float _z, float _s ) : 
				v(_x, _y, _z), s(_s) {}
	
	_inl void	conjugate( const Quaternion& orig );
	_inl void	setIdentity();

	_inl void	axisToAxis		( const Vector3D& from, const Vector3D& to );
	_inl void	fromAxisAngle	( const Vector3D& axis, float angle );
	_inl void	fromEulerAngles	( float yaw, float pitch, float roll );
	_inl void	fromMatrix		( const Matrix3D& rotM );

	_inl float	norm2() const;
	_inl void	reverse();
	_inl float	dot( const Quaternion& q ) const;
	_inl void	slerp( const Quaternion& q1, const Quaternion& q2, float t );

	_inl const Quaternion& operator *=( const Quaternion& q );

	static const Quaternion identity;
};

/*****************************************************************
/*  Class:  Frustum                                              *
/*  Desc:   Describes visibility frustum                         *
/*****************************************************************/
class Frustum
{
private:
	Vector3D				vector[8];		// corners of the view frustum
											//   xyz Xyz xYz XYz xyZ XyZ xYZ XYZ

    Plane					plane[6];		// planes of the view frustum
											//   Near Far Left Right Top Bottom

	AABBIntersectMask		aabbMask[6];	//  AABB intersect mask for all planes 
	
public:
	Frustum();

	_inl IntersectStatus	intersect( AABoundBox& aabb ) const;
	_inl const Vector3D&	getVertex( int idx ) const;

	_inl const Vector3D&	getNearLTVec() const;
	_inl const Vector3D&	getNearLBVec() const;
	_inl const Vector3D&	getNearRTVec() const;
	_inl const Vector3D&	getNearRBVec() const;

	_inl const Vector3D&	getFarLTVec() const;
	_inl const Vector3D&	getFarLBVec() const;
	_inl const Vector3D&	getFarRTVec() const;
	_inl const Vector3D&	getFarRBVec() const;

	void					calculate(  const Matrix4D& pMatView, 
										const Matrix4D& pMatProj );
};  // class Frustum

/*****************************************************************
/*  Class:  Cylinder                                             *
/*  Desc:   Describes 3D cylinder								 *
/*****************************************************************/
class Cylinder{
private:
	Vector3D		base;		//  base (bottom) point
	Vector3D		core;		//  vector from base to top
	float			r2;			//  squared radius of cylinder
								//  if r2 < 0.0f, cylinder is infinite
public:
	Cylinder();
	Cylinder(	const Vector3D& bottom, 
				const Vector3D& top, 
				float R2 );
	Cylinder( const Cylinder& orig );

	_inl void		create(	const Vector3D& bottom, 
							const Vector3D& top, 
							float R2 );

	_inl bool		isInfinite() const;
	_inl void		setInfinite();

	_inl Vector3D	getBase() const;
	_inl Vector3D	getDir() const;
	_inl Vector3D	getTop() const;

	_inl void		getCenter( Vector3D& cen ) const;

	_inl float		getR() const;
	_inl float		getR2() const;
	_inl void		getAxis( Vector3D& axis ) const;
	_inl void		getVertex( float angle, float h, Vector3D& vert ) const;
	_inl float		getHeight() const;
	_inl Matrix4D	getLocalToWorld() const;

	_inl bool		isPointInside( const Vector3D& pt ) const;

	_inl void		transform( const Matrix3D& matr );
	_inl void		transform( const Matrix4D& matr );
	_inl void		Union( const Cylinder& cyl );
	
};  // class Cylinder

/*****************************************************************
/*  Class:  Frame                                                *
/*  Desc:   2D rectangular area                                  *
/*****************************************************************/
class Frame
{
public:
       Frame() : x(0), y(0), w(0), h(0){}
       Frame( float _x, float _y, float _w, float _h ) :
             x(_x), y(_y), w(_w), h(_h) {}

    _inl float		getAspect()								const;
	_inl float		maxSide()								const;
	_inl void		copy( const Frame& orig );
    _inl void		fitInto( const Frame& rct );
	_inl void		set( float _x, float _y, float _w, float _h );

	_inl float		getRight() const;
	_inl float		getBottom() const;

       float x, y, w, h;
};   // class Frame


/*****************************************************************
/*  Class:  Line3D                                               *
/*  Desc:   3D line 			                                 *
/*****************************************************************/
class Line3D
{
	Vector3D orig;			//  line base point
	Vector3D dir;			//  line directional vector

public:
	Line3D( const Vector3D& base, const Vector3D& direction );

	_inl float		project( const Vector3D& vec )			const;
	_inl float		dist2ToOrig( const Vector3D& vec )		const;
	_inl void		getPoint( float prLen, Vector3D& pt )	const;

};

/*****************************************************************
/*  Class:  Curve2D                                              *
/*  Desc:   parametric curve on the plane 			             *
/*****************************************************************/
class Curve2D
{
	Vector2Df*		nodes;
	int				nNodes;
public:

}; // class Curve2D

#ifdef _INLINES
#include "mMath3D.inl"
#endif // _INLINES

#endif // __GPMATH3D_H__