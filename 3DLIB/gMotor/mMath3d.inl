/*****************************************************************
/*  File:   Math3D.inl                                           *
/*  Author: Silver, Copyright (C) GSC Game World                 *
/*  Date:   January 2002                                         *
/*****************************************************************/
#include <math.h>

_inl float rndValuef()
{
    float rndval = static_cast<float>( rand() );
    rndval /= (float)RAND_MAX;
    return rndval;
}

_inl float rndValuef( float min, float max )
{
        return min + static_cast<float>( max - min ) * rndValuef(); 
}

/*---------------------------------------------------------------*
/*  Func:	RayTriangleX
/*	Desc:	Determines if ray intersects 3D triangle
/*	Parm:	orig - ray origin
/*			dir  - ray direction (normalized)
/*			v0, v1, v2 - triangle vertices
/*	Ret:	true if ray intersects triangle, false if not
/*			u, v - barycentric coords of intersection point
/*			t distance from orig to intersection point
/*	Rmrk:	From Tomas Moller - "Fast, Minimum Storage 
/*				Ray/Triangle Intersection"
/*---------------------------------------------------------------*/
_inl bool RayTriangleX( const Vector3D&	orig, 
							    const Vector3D&	dir, 
								const Vector3D&	v0, 
								const Vector3D&	v1, 
								const Vector3D&	v2, 
								float& u, float& v, 
								float& t )
{
	Vector3D e1, e2, tv, pv, qv;
	e1.sub( v1, v0 );
	e2.sub( v2, v0 );
	pv.cross( dir, e2 );
	float det = e1.dot( pv );
	if (det > -c_Epsilon && det < c_Epsilon) return false;
	float invDet = 1.0f / det;
	tv.sub( orig, v0 );
	u = tv.dot( pv ) * invDet;
	if (u < 0.0f || u > 1.0f) return false;
	qv.cross( tv, e1 );
	v = dir.dot( qv ) * invDet;
	if (v < 0.0f || u + v > 1.0f) return false;
	t = e2.dot( qv ) * invDet;
	return true;
}  // RayIntersectTriangle

template <typename T>
void SmoothFilter( T* arr, int width, int height )
{
	int w1 = width	- 1;
	int h1 = height - 1;
	int cp = width + 1;
	int val;
	for (int i = 1; i < w1; i++)
	{
		for (int j = 1; j < h1; j++)
		{
			val =
				arr[cp - width - 1] + arr[cp - width] + arr[cp - width + 1]	+
				arr[cp		   - 1]	+ arr[cp		] + arr[cp		   + 1]	+ 
				arr[cp + width - 1]	+ arr[cp + width] + arr[cp + width + 1];
			val /= 9;
			arr[cp] = val;
			cp++;
		}
		cp += 2;
	}
}

_inl float 
minf( float a, float b )
{
	return (a < b) ? a : b;
}
_inl float 
maxf( float a, float b )
{
	return (a > b) ? a : b;
}

_inl float 
minf( float a, float b, float c, float d )
{
	return minf( minf( a, b ), minf( c, d ) );
}

_inl float 
maxf( float a, float b, float c, float d )
{
	return maxf( maxf( a, b ), maxf( c, d ) );
}

_inl float 
minf(	float a, float b, float c, float d, 
		float e, float f, float g, float h )
{
	return minf(	minf( a, b, c, d ),
					minf( e, f, g, h ) );
}

_inl float 
maxf(	float a, float b, float c, float d, 
		float e, float f, float g, float h )
{
	return maxf(	maxf( a, b, c, d ),
					maxf( e, f, g, h ) );
}

_inl float 
mini( float a, float b )
{
	return (a < b) ? a : b;
}
_inl float 
maxi( float a, float b )
{
	return (a > b) ? a : b;
}

_inl float 
mini( float a, float b, float c, float d )
{
	return mini( mini( a, b ), mini( c, d ) );
}

_inl float 
maxi( float a, float b, float c, float d )
{
	return maxi( maxi( a, b ), maxi( c, d ) );
}

/*****************************************************************
/*	Vector3D implementation
/*****************************************************************/
_inl Vector3D::Vector3D( float _x, 
						 float _y, 
						 float _z )
{
	x = _x;
	y = _y;
	z = _z;
}

_inl Vector3D::Vector3D( const Vector3D& orig )
{
    x = orig.x;
    y = orig.y;
    z = orig.z;
}

_inl Vector3D::Vector3D( const Vector4D& orig )
{
    x = orig.x;
    y = orig.y;
    z = orig.z;
}

_inl float 
Vector3D::TriArea( const Vector3D& v ) const 
{
	Vector3D cr( y * v.z - z * v.y, 
				 x * v.z - z * v.x,
				 x * v.y - y * v.x );

	return static_cast<float>( sqrt( cr.x * cr.x + 
									 cr.y * cr.y + 
									 cr.z * cr.z ) / 2 ); 
}

_inl void
Vector3D::cross( const Vector3D& v1, const Vector3D& v2 ) 
{
	x = v1.y * v2.z - v1.z * v2.y; 
	y = v1.z * v2.x - v1.x * v2.z;
	z = v1.x * v2.y - v1.y * v2.x;
}

_inl void	
Vector3D::set( float _x, float _y, float _z )
{
	x = _x;
	y = _y;
	z = _z;
}

_inl void	
Vector3D::sub( const Vector3D& v1, const Vector3D& v2 )
{
	x = v1.x - v2.x;
	y = v1.y - v2.y;
	z = v1.z - v2.z;
}

_inl void	
Vector3D::add( const Vector3D& v1, const Vector3D& v2 )
{
	x = v1.x + v2.x;
	y = v1.y + v2.y;
	z = v1.z + v2.z;
}

_inl void	
Vector3D::sub( const Vector3D& v )
{
	x -= v.x;
	y -= v.y;
	z -= v.z;
}

_inl void	
Vector3D::add( const Vector3D& v )
{
	x += v.x;
	y += v.y;
	z += v.z;
}

_inl void		
Vector3D::addWeighted( const Vector3D& v, float weight )
{
	x += v.x * weight;
	y += v.y * weight;
	z += v.z * weight;
}


_inl void		
Vector3D::zero()
{
	x = 0.0f;
	y = 0.0f;
	z = 0.0f;
}

_inl float 
Vector3D::norm() const
{
	return static_cast<float>( sqrt( x * x + y * y + z * z ) );
}

_inl float 
Vector3D::distance( const Vector3D& orig ) const
{
	Vector3D tmp;
	tmp.copy( *this );
	tmp -= orig;
	return tmp.norm();
}

_inl float
Vector3D::distance2( const Vector3D& orig ) const
{
	Vector3D tmp;
	tmp.copy( *this );
	tmp -= orig;
	return tmp.norm2();
}

_inl void	
Vector3D::reverse( const Vector3D& orig )
{
	x = -orig.x;
	y = -orig.y;
	z = -orig.z;
}

_inl void Vector3D::random( float minX, float maxX, 
							float minY, float maxY, 
							float minZ, float maxZ )
{
	x = rndValuef( minX, maxX );
	y = rndValuef( minY, maxY );
	z = rndValuef( minZ, maxZ );
} // Vector3D::random

_inl void	
Vector3D::reverse()
{
	x = -x;
	y = -y;
	z = -z;
}

_inl float 
Vector3D::norm2() const
{
	return x * x + y * y + z * z;
}

_inl bool		
Vector3D::isEqual( const Vector3D& orig ) const
{
	return (fabs( x - orig.x ) < c_Epsilon && 
			fabs( y - orig.y ) < c_Epsilon && 
			fabs( z - orig.z ) < c_Epsilon);  
}

_inl bool		
Vector3D::isColinear( const Vector3D& orig ) const
{
//  FIXME: lame algebra here
	Vector3D na, nb;
	na = orig;
	na.normalize();
	nb.copy( *this );
	nb.normalize();
	return na.isEqual( nb );
}

_inl void		
Vector3D::planeNormal(	const Vector3D& v1,
						const Vector3D& v2,
						const Vector3D& v3 )
{
	Vector3D t1, t2;
	t1.sub( v1, v2 );
	t2.sub( v3, v2 );
	cross( t2, t1 ); 
}

_inl float		
Vector3D::dot( const Vector3D& v ) const
{
	return x * v.x + y * v.y + z * v.z;
}


_inl const Vector3D& 
Vector3D::operator +=( const Vector3D& vec )
{
	x += vec.x;
	y += vec.y;
	z += vec.z;
	return *this;
}

_inl const Vector3D& 
Vector3D::operator -=( const Vector3D& vec )
{
	x -= vec.x;
	y -= vec.y;
	z -= vec.z;
	return *this;
}

_inl const Vector3D& 
Vector3D::operator *=( const float val )
{
	x *= val;
	y *= val;
	z *= val;
	return *this;
}

_inl const Vector3D& 
Vector3D::operator *=( const Matrix3D& m )
{
	Vector3D tmp;
	tmp.x = x * m.e00 + y * m.e10 + z * m.e20;
	tmp.y = x * m.e01 + y * m.e11 + z * m.e21;
	tmp.z = x * m.e02 + y * m.e12 + z * m.e22;

	(*this) = tmp;
	return *this;
}

_inl const Vector3D& 
Vector3D::operator *=( const Matrix4D& m )
{
	float cx = x * m.e00 + y * m.e10 + z * m.e20 + m.e30;
	float cy = x * m.e01 + y * m.e11 + z * m.e21 + m.e31;
	float cz = x * m.e02 + y * m.e12 + z * m.e22 + m.e32;
	x = cx; y = cy; z = cz;
	return *this;
}

_inl const Vector3D& 
Vector3D::mulPt( const Vector3D& v, const Matrix4D& m )
{
	float cx = v.x * m.e00 + v.y * m.e10 + v.z * m.e20 + m.e30;
	float cy = v.x * m.e01 + v.y * m.e11 + v.z * m.e21 + m.e31;
	float cz = v.x * m.e02 + v.y * m.e12 + v.z * m.e22 + m.e32;
	x = cx; y = cy; z = cz;
	return *this;
}

_inl const Vector3D& 
Vector3D::mulPt( const Matrix4D& m )
{
	Vector3D tmp;
	tmp.x = x * m.e00 + y * m.e10 + z * m.e20 + m.e30;
	tmp.y = x * m.e01 + y * m.e11 + z * m.e21 + m.e31;
	tmp.z = x * m.e02 + y * m.e12 + z * m.e22 + m.e32;
	(*this) = tmp;
	return (*this);
}

_inl const Vector3D& 
Vector3D::mulLeft( const Matrix3D& m )
{
	Vector3D tmp;
	tmp.x = x * m.e00 + y * m.e01 + z * m.e02;
	tmp.y = x * m.e10 + y * m.e11 + z * m.e12;
	tmp.z = x * m.e20 + y * m.e21 + z * m.e22;

	(*this) = tmp;
	return *this;
}

_inl const Vector3D& 
Vector3D::operator /=( const float val )
{
	x /= val;
	y /= val;
	z /= val;
	return *this;
}

_inl void 
Vector3D::normalize()
{
	float len = static_cast<float>( sqrt( x * x + y * y + z * z) );
    if (len < c_Epsilon) return;
	x /= len;
	y /= len;
	z /= len;
}

_inl void	
Vector3D::copy( const Vector3D& orig )
{
	x = orig.x;
	y = orig.y;
	z = orig.z;
}

_inl void
Vector3D::copy( const Vector4D& orig )
{
	x = orig.x;
	y = orig.y;
	z = orig.z;
}

_inl const Vector3D& Vector3D::operator=( const Vector4D& orig )
{
	if (orig.w != 1.0f && orig.w > c_Epsilon)
	{
		x = orig.x / orig.w;
		y = orig.y / orig.w;
		z = orig.z / orig.w;
	}
	else
	{
		x = orig.x;
		y = orig.y;
		z = orig.z;
	}
	return *this; 
}

_inl const Vector3D& Vector3D::operator=( const Vector3D& orig )
{
	x = orig.x;
	y = orig.y;
	z = orig.z;
	return *this;
}

_inl void		
Vector3D::PlaneIntersectLine( Plane& p, 
							  const Vector3D& v1,
							  const Vector3D& v2 )
{
	Vector3D v;
	Vector3D* pn = p.getNormal();
	v.sub( v2, v1 );
	float coef = pn->dot( v1 );
	coef += p.d;
	coef /= pn->dot( v );
	v *= coef;
	sub( v1, v );
}

/*****************************************************************
/*	Vector2D implementation
/*****************************************************************/
template <class T>
Vector2D<T>::Vector2D() : x(0.0f), y(0.0f)
{
}

template <class T> _inl bool
Vector2D<T>::inAAQuad( T _x, T _y, T _side ) const
{
	return (x >= _x) && (x <= _x + _side) && (y >= _y) && (y <= _y + _side);
}

template <class T>
Vector2D<T>::Vector2D( const T _x, const T _y )
{
	x = _x;
	y = _y;
}

template <class T> _inl void 
Vector2D<T>::sub( const Vector2D<T>& v )
{
	x -= v.x;
	y -= v.y;
}

template <class T> _inl float 
Vector2D<T>::dot( const Vector2D<T>& v )
{
	return x * v.x + y * v.y;
}

template <class T> _inl T 
Vector2D<T>::triArea( const Vector2D<T>& v )
{
	return v.x * y - v.y * x;
}


template <class T> void 
Vector2D<T>::Dump()
{
	Log::Message( "x: %f. y: %f.", x, y );
}

template <class T> _inl void 
Vector2D<T>::set( T _x, T _y )
{
	x = _x;
	y = _y;
}

template <class T> _inl void 
Vector2D<T>::sub( const Vector2D& v1, const Vector2D& v2 )
{
	x = v1.x - v2.x;
	y = v1.y - v2.y;
}

template <class T> _inl void 
Vector2D<T>::clamp( T minX, T minY, T maxX, T maxY )
{
	if (x < minX) x = minX;
	if (y < minY) y = minY;

	if (x > maxX) x = maxX;
	if (y > maxY) y = maxY;
}

template <class T> _inl void 
Vector2D<T>::copy( const Vector2D& orig )
{
	x = orig.x;
	y = orig.y;
}

template <class T> _inl const Vector2D<T>& 
Vector2D<T>::operator +=( const Vector2D<T>& vec )
{
	x += vec.x;
	y += vec.y;
	return *this;
}

template <class T> _inl const Vector2D<T>& 
Vector2D<T>::operator -=( const Vector2D<T>& vec )
{
	x -= vec.x;
	y -= vec.y;
	return *this;
}

template <class T> _inl const Vector2D<T>& 
Vector2D<T>::operator *=( const T val )
{
	x *= val;
	y *= val;
	return *this;
}

template <class T> _inl const Vector2D<T>& 
Vector2D<T>::operator /=( const T val )
{
	x /= val;
	y /= val;
	return *this;
}

template <class T> _inl T 
Vector2D<T>::norm() const
{
	return static_cast<T>( sqrt( x * x + y * y ) );	
}

template <class T> _inl T
Vector2D<T>::norm2() const
{
	return x * x + y * y;
}

template <class T> _inl T
Vector2D<T>::dist2( Vector2D<T> r ) const
{
	return (x - r.x) * (x - r.x) + (y - r.y) * (y - r.y);
}

template <class T> _inl void 
Vector2D<T>::normalize()
{
	float len = static_cast<T>( sqrt( x * x + y * y ) );
	x /= len;
	y /= len;
}

/*****************************************************************
/*	Line2D implementation
/*****************************************************************/
template <class T> 
Line2D<T>::Line2D( const Vector2D<T> _a, const Vector2D<T> _b )
{
	a.copy( _a );
	b.copy( _b );
}

template <class T> _inl void
Line2D<T>::Init( const Vector2D<T> _a, const Vector2D<T> _b )
{
	a.copy( _a );
	b.copy( _b );
}

template <class T> _inl bool 
Line2D<T>::oneSide( const Vector2D<T> v1, const Vector2D<T> v2 ) const
{
	return ( ((v1.x - a.x)*(b.y - a.y) - 
			  (v1.y - a.y)*(b.x - a.x)) * 
					((v2.x - a.x)*(b.y - a.y) - 
					 (v2.y - a.y)*(b.x - a.x))
			) >= 0; 
}


template <class T> _inl bool 
Line2D<T>::inLeftHalf( const Vector2D<T> pt ) const
{
	return ((pt.x - a.x)*(b.y - a.y) - (pt.y - a.y)*(b.x - a.x)) <= 0;
}


template <class T> _inl bool 
Line2D<T>::inLeftHalf( T _x, T _y ) const
{
	return ((_x - a.x)*(b.y - a.y) - (_y - a.y)*(b.x - a.x)) <= 0;
}

/*****************************************************************
/*	Matrix3D implementation
/*****************************************************************/
/*---------------------------------------------------------------*
/*  Func:	Matrix3D::scaling
/*	Desc:	Builds scaling matrix 
/*	Parm:	sx, sy, sz - x, y, z, axis scales
/*---------------------------------------------------------------*/
_inl void Matrix3D::scaling( float sx, float sy, float sz )
{
	setIdentity();
	e00 = sx;
	e11 = sy;
	e22 = sz;
}

/*---------------------------------------------------------------*
/*  Func:	Matrix3D::rotation
/*	Desc:	Builds rotation matrix from the quaternion
/*	Parm:	q - rotation quaternion
/*	Rmrk:	rotation matrix is pre-multiply transform matrix,
/*			in Direct3D style
/*---------------------------------------------------------------*/
_inl void Matrix3D::rotation( const Quaternion& q )
{
	float xx, yy, zz, xy, yz, xz, sx, sy, sz;
	xx = 2.0f * q.v.x * q.v.x;
	yy = 2.0f * q.v.y * q.v.y;
	zz = 2.0f * q.v.z * q.v.z;
	xy = 2.0f * q.v.x * q.v.y;
	yz = 2.0f * q.v.y * q.v.z;
	xz = 2.0f * q.v.x * q.v.z;
	sx = 2.0f * q.s   * q.v.x;
	sy = 2.0f * q.s	  * q.v.y;
	sz = 2.0f * q.s   * q.v.z;

	e00 = 1.0f - yy - zz;
	e01 = xy + sz;
	e02 = xz - sy;

	e10 = xy - sz;
	e11 = 1 - xx - zz;
	e12 = yz + sx;

	e20 = xz + sy;
	e21 = yz - sx;
	e22 = 1 - xx - yy;
}//  Matrix3D::rotation

/*---------------------------------------------------------------*
/*  Func:	Matrix3D::rotation
/*	Desc:	Builds rotation matrix from axis of rotation and angle
/*	Parm:	axis  - axis of rotation
/*			angle - rotation angle 
/*---------------------------------------------------------------*/
_inl void Matrix3D::rotation( const Vector3D& axis, float angle )
{
	Quaternion q;
	q.fromAxisAngle( axis, angle );
	rotation( q );
} // Matrix3D::RotationFromAxisAngle

/*---------------------------------------------------------------*
/*  Func:	Matrix3D::rotationFromEuler
/*	Desc:	Builds rotation matrix from Euler angles
/*	Parm:	yaw		- yaw around the y-axis
/*			pitch	- pitch around the x-axis
/*			roll	- roll around the z-axis
/*---------------------------------------------------------------*/
_inl void Matrix3D::rotationFromEuler( float yaw, float pitch, float roll )
{
	float sinYaw	= sin( yaw );
	float cosYaw	= cos( yaw );
	float sinPitch	= sin( pitch );
	float cosPitch	= cos( pitch );
	float sinRoll	= sin( roll );
	float cosRoll	= cos( roll );
	
	assert( false ); // TODO
}


/*---------------------------------------------------------------------------*/
/*	Func:	Matrix3D::det	
/*	Desc:	finds determinant of matrix
/*---------------------------------------------------------------------------*/
_inl float Matrix3D::det() const
{
	
	return	e00*(e11*e22 - e21*e12) + 
			e01*(e20*e12 - e10*e22) + 
			e02*(e10*e21 - e20*e11);
} // Matrix3D::det

_inl void Matrix3D::copy( const Matrix3D& orig )
{
	memcpy( (void*)this, (void*)&orig, sizeof( Matrix3D ) );
}

_inl void Matrix3D::copy( const Matrix4D& orig )
{
	e00 = orig.e00; e01 = orig.e01; e02 = orig.e02;
	e10 = orig.e10; e11 = orig.e11; e12 = orig.e12;
	e20 = orig.e20; e21 = orig.e21; e22 = orig.e22;
}

_inl void Matrix3D::txtSave( FILE* fp, const char* name ) const
{
	if (name)
	{
		fprintf( fp,	"%s={\n{%-8.8f, %-8.8f, %-8.8f}, \n"
						"{%-8.8f, %-8.8f, %-8.8f}, \n"
						"{%-8.8f, %-8.8f, %-8.8f} \n}\n",
						name,
						e00, e01, e02, 
						e10, e11, e12, 
						e20, e21, e22 );
	}
	else
	{
		fprintf( fp,	"={\n{%-8.8f, %-8.8f, %-8.8f}, \n"
						"{%-8.8f, %-8.8f, %-8.8f}, \n"
						"{%-8.8f, %-8.8f, %-8.8f} \n}\n",
						name,
						e00, e01, e02, 
						e10, e11, e12, 
						e20, e21, e22 );
	}
} // Matrix3D::txtSave

/*---------------------------------------------------------------------------*/
/*	Func:	Matrix3D::inverse
/*	Desc:	finds inverse matrix with Kramer's rule 
/*	Ret:	matrix determinant
/*---------------------------------------------------------------------------*/
_inl float Matrix3D::inverse( const Matrix3D& m )
{
	long double d = m.det();
	if (fabs( d ) < c_SmallEpsilon) return d;
	
	float* e  = getBuf();

	/*
    e[0] =    ma[4]*ma[8] - ma[5]*ma[7]   / d;
    e[1] = -( ma[1]*ma[8] - ma[7]*ma[2] ) / d;
    e[2] =    ma[1]*ma[5] - ma[4]*ma[2]   / d;

    e[3] = -( ma[3]*ma[8] - ma[5]*ma[6] ) / d;
    e[4] =    ma[0]*ma[8] - ma[6]*ma[2]   / d;
    e[5] = -( ma[0]*ma[5] - ma[3]*ma[2] ) / d;

    e[6] =    ma[3]*ma[7] - ma[6]*ma[4]   / d;
    e[7] = -( ma[0]*ma[7] - ma[6]*ma[1] ) / d;
    e[8] =    ma[0]*ma[4] - ma[1]*ma[3]   / d;
	*/
	

	e00 = (-m.e12*m.e21 + m.e11*m.e22) / d;
	e01 = ( m.e02*m.e21 - m.e01*m.e22) / d;
	e02 = (-m.e02*m.e11 + m.e01*m.e12) / d;

	e10 = ( m.e12*m.e20 - m.e10*m.e22) / d;
	e11 = (-m.e02*m.e20 + m.e00*m.e22) / d;
	e12 = ( m.e02*m.e10 - m.e00*m.e12) / d;

	e20 = (-m.e11*m.e20 + m.e10*m.e21) / d;
	e21 = ( m.e01*m.e20 - m.e00*m.e21) / d;
	e22 = (-m.e01*m.e10 + m.e00*m.e11) / d;

	return d;
} // Matrix3D::inverse

_inl float Matrix3D::inverse()
{
	Matrix3D tmp( *this );
	return inverse( tmp );
} // Matrix3D::inverse

_inl bool Matrix3D::isOrthonormalBasis() const
{
	return true;
} // Matrix3D::isOrthonormalBasis


_inl void Matrix3D::setNull()
{
	memset( getBuf(), 0, 9 * sizeof( e00 ) );
}

_inl void Matrix3D::setIdentity()
{
	setNull();
	e00 = 1.0f;
	e11 = 1.0f;
	e22 = 1.0f;
}

_inl Matrix3D& Matrix3D::operator *=( const Matrix3D& r )
{
	Matrix3D tmp;
	tmp.mul( *this, r );
	(*this) = tmp;
	return (*this);
}

_inl void Matrix3D::transpose()
{
	register float tmp = e01; e01 = e10; e10 = tmp;
	tmp = e21; e21 = e20; e20 = tmp;
	tmp = e21; e21 = e12; e12 = tmp;
}

_inl void Matrix3D::mul( const Matrix3D& l, const Matrix3D& r )
{
	e00 = l.e00 * r.e00 + l.e01 * r.e10 + l.e02 * r.e20;
	e01 = l.e00 * r.e01 + l.e01 * r.e11 + l.e02 * r.e21;
	e02 = l.e00 * r.e02 + l.e01 * r.e12 + l.e02 * r.e22;

	e10 = l.e10 * r.e00 + l.e11 * r.e10 + l.e12 * r.e20;
	e11 = l.e10 * r.e01 + l.e11 * r.e11 + l.e12 * r.e21;
	e12 = l.e10 * r.e02 + l.e11 * r.e11 + l.e12 * r.e21;

	e20 = l.e20 * r.e00 + l.e21 * r.e10 + l.e22 * r.e20;
	e21 = l.e20 * r.e01 + l.e21 * r.e11 + l.e22 * r.e21;
	e22 = l.e20 * r.e02 + l.e21 * r.e12 + l.e22 * r.e22;
}

_inl Matrix3D& Matrix3D::operator *=( const float f )
{
	e00 *= f;
	e01 *= f;
	e02 *= f;

	e10 *= f;
	e11 *= f;
	e12 *= f;

	e10 *= f;
	e11 *= f;
	e12 *= f;

	return (*this);
}

_inl Matrix3D& Matrix3D::operator +=( const float f )
{
	e00 += f;
	e01 += f;
	e02 += f;

	e10 += f;
	e11 += f;
	e12 += f;

	e10 += f;
	e11 += f;
	e12 += f;

	return (*this);
}

_inl Matrix3D& Matrix3D::operator +=( const Matrix3D& r )
{
	e00 += r.e00;
	e01 += r.e01;
	e02 += r.e02;

	e10 += r.e10;
	e11 += r.e11;
	e12 += r.e12;

	e10 += r.e10;
	e11 += r.e11;
	e12 += r.e12;

	return (*this);
}

/*****************************************************************
/*	Matrix4D implementation
/*****************************************************************/
_inl Matrix4D& 
Matrix4D::mul( const Matrix4D& a, const Matrix4D& b )
{
	getV0().mul( a.getV0(), b );
	getV1().mul( a.getV1(), b );
	getV2().mul( a.getV2(), b );
	getV3().mul( a.getV3(), b );
	return *this;
}

_inl float
Matrix4D::quadraticForm( Vector4D& v )
{
    Vector4D tmp;
    tmp.mul( v, *this );
    return v.dot( tmp );
}

_inl Matrix4D&			
Matrix4D::operator +=( const Matrix4D& r )
{
	e00 += r.e00; e01 += r.e01; e02 += r.e02; e03 += r.e03;
	e10 += r.e10; e11 += r.e11; e12 += r.e12; e13 += r.e13;
	e20 += r.e20; e21 += r.e21; e22 += r.e22; e23 += r.e23;
	e30 += r.e30; e31 += r.e31; e32 += r.e32; e33 += r.e33;
	return *this;
}

_inl Matrix4D&			
Matrix4D::addWeighted( const Matrix4D& r, float w )
{
	e00 += r.e00 * w; e01 += r.e01 * w; e02 += r.e02 * w; e03 += r.e03 * w;
	e10 += r.e10 * w; e11 += r.e11 * w; e12 += r.e12 * w; e13 += r.e13 * w;
	e20 += r.e20 * w; e21 += r.e21 * w; e22 += r.e22 * w; e23 += r.e23 * w;
	e30 += r.e30 * w; e31 += r.e31 * w; e32 += r.e32 * w; e33 += r.e33 * w;
	return *this;
}

_inl Matrix4D&
Matrix4D::mulVV( const Vector4D& v )
{
    e00 = v.x * v.x;
    e11 = v.y * v.y;
    e22 = v.z * v.z;
    e33 = v.w * v.w;

    e01 = e10 = v.x * v.y;
    e02 = e20 = v.x * v.z;
    e03 = e30 = v.x * v.w;
    e12 = e21 = v.y * v.z;
    e13 = e31 = v.y * v.w;
    e23 = e32 = v.z * v.w;

    return *this;
}

_inl Matrix4D& Matrix4D::operator *=( const Matrix4D& r )
{
	Matrix4D tmp;
	tmp.mul( *this, r );
	*this = tmp;
	return *this;
}

_inl Matrix4D& Matrix4D::operator *=( float w )
{
	e00 *= w; e01 *= w; e02 *= w; e03 *= w;
	e10 *= w; e11 *= w; e12 *= w; e13 *= w;
	e20 *= w; e21 *= w; e22 *= w; e23 *= w;
	e30 *= w; e31 *= w; e32 *= w; e33 *= w;
	return *this;
}

_inl const Matrix4D&		
Matrix4D::operator =( const Matrix4D& matr )
{
	memcpy( getBuf(), matr.getBuf(), 16 * sizeof( e00 ) );
	return *this;
}

_inl const Matrix4D&		
Matrix4D::operator =( const Matrix3D& matr )
{
	e00 = matr.e00;
	e01 = matr.e01;
	e02 = matr.e02;
	e03 = 0.0f;

	e10 = matr.e10;
	e11 = matr.e11;
	e12 = matr.e12;
	e13 = 0.0f;

	e20 = matr.e20;
	e21 = matr.e21;
	e22 = matr.e22;
	e23 = 0.0f;

	e30 = 0.0f;
	e31 = 0.0f;
	e32 = 0.0f;
	e33 = 1.0f;

	return *this;
}

_inl void
Matrix4D::setIdentity()
{
	memset( getBuf(), 0, 16 * sizeof( e00 ) );
	e00 = 1.0;
	e11 = 1.0;
	e22 = 1.0;
	e33 = 1.0;
}

_inl void	
Matrix4D::setNull()
{
	memset( getBuf(), 0, 16 * sizeof( e00 ) );
}

/*---------------------------------------------------------------*
/*  Func:	Matrix4D::translation
/*	Desc:	builds translation matrix
/*	Parm:	dx, dy, dz - shifts along the correspondent axes
/*---------------------------------------------------------------*/
_inl void Matrix4D::translation( float dx, float dy, float dz )
{
	setIdentity();
	e30 = dx; e31 = dy; e32 = dz;
}//  Matrix4D::translation

_inl void Matrix4D::translation( const Vector3D& trans )
{
	setIdentity();
	e30 = trans.x; e31 = trans.y; e32 = trans.z;
}//  Matrix4D::translation

_inl void Matrix4D::transpose( const Matrix4D& orig )
{
	e00 = orig.e00; e01 = orig.e10; e02 = orig.e20; e03 = orig.e30;
	e10 = orig.e01; e11 = orig.e11; e12 = orig.e21; e13 = orig.e31;
	e20 = orig.e02; e21 = orig.e12; e22 = orig.e22; e23 = orig.e32;
	e30 = orig.e03; e31 = orig.e13; e32 = orig.e23; e33 = orig.e33;
}

/*---------------------------------------------------------------*
/*  Func:	Matrix4D::scaling
/*	Desc:	builds scaling matrix
/*	Parm:	sx, sy, sz - scales along the correspondent axes
/*---------------------------------------------------------------*/
_inl void Matrix4D::scaling( float sx, float sy, float sz )
{
	setIdentity();
	e00 = sx; e11 = sy; e22 = sz;
}

/*---------------------------------------------------------------*
/*  Func:	Matrix4D::rotation
/*	Desc:	builds 4D rotation matrix
/*	Parm:	axis, angle - axis and angle of rotation
/*---------------------------------------------------------------*/
_inl void Matrix4D::rotation( const Vector3D& axis, float angle )
{
	Matrix3D rot;
	rot.rotation( axis, angle );
	*this = rot;
}//  Matrix4D::rotation

_inl void Matrix4D::srt(	float scale, 
							const Vector3D& axis, float angle, 
							const Vector3D& pos )
{
	Matrix4D tmp;
	scaling( scale, scale, scale );
	tmp.rotation( axis, angle );
	(*this) *= tmp;
	e30 = pos.x;
	e31 = pos.y;
	e32 = pos.z;
} // Matrix4D::srt
	
_inl void Matrix4D::shearing(	float sxy, float sxz, 
								float syx, float syz,
								float szx, float szy )
{
	e00 = 1.0f; e01 = syx;	e02 = szx;	e03 = 0.0f;
	e10 = sxy;  e11 = 1.0f; e12 = szy;  e13 = 0.0f;
	e20 = sxz;  e21 = syz;	e22 = 1;	e23 = 0.0f;
	e30 = 0.0f; e31 = 0.0f; e32 = 0.0f;	e33 = 1.0f;
} // Matrix4D::shearing

/*---------------------------------------------------------------*
/*  Func:	Matrix4D::rotation
/*	Desc:	Builds rotation matrix from the quaternion
/*	Parm:	q - rotation quaternion
/*	Rmrk:	rotation matrix is pre-multiply transform matrix,
/*			in Direct3D style
/*---------------------------------------------------------------*/
_inl void Matrix4D::rotation( const Quaternion& q )
{
	Matrix3D rot;
	rot.rotation( q );
	*this = rot;
}//  Matrix4D::rotation

/*---------------------------------------------------------------*
/*  Func:	Matrix4D::affineInverse
/*	Desc:	Inverse of a matrix that is made up of only scales, 
/*			rotations, and translations
/*	Parm:	orig - original matrix
/*---------------------------------------------------------------*/
_inl void Matrix4D::affineInverse( const Matrix4D& orig )
{
	float Tx, Ty, Tz;

	// rotational part is simply transposed 
	e00 = orig.e00;
	e10 = orig.e01;
	e20 = orig.e02;
	
	e01 = orig.e10;
	e11 = orig.e11;
	e21 = orig.e12;
	
	e02 = orig.e20;
	e12 = orig.e21;
	e22 = orig.e22;
	
	e03 = e13 = e23 = 0.0f;
	e33 = 1.0f;

 	Tx = orig.e30;
	Ty = orig.e31;
	Tz = orig.e32;

	//  calculate translation part of affine inverse matrix
	e30 = -( e00 * Tx + e10 * Ty + e20 * Tz );
	e31 = -( e01 * Tx + e11 * Ty + e21 * Tz );
	e32 = -( e02 * Tx + e12 * Ty + e22 * Tz );
  
}//  Matrix4D::affineInverse


/*****************************************************************
/*	Vector4D implementation
/*****************************************************************/
_inl Vector4D& 
Vector4D::mul( const Vector4D& v, const Matrix4D& m )
{
	x = v.x * m.e00 + v.y * m.e10 + v.z * m.e20 + v.w * m.e30;
	y = v.x * m.e01 + v.y * m.e11 + v.z * m.e21 + v.w * m.e31;
	z = v.x * m.e02 + v.y * m.e12 + v.z * m.e22 + v.w * m.e32;
	w = v.x * m.e03 + v.y * m.e13 + v.z * m.e23 + v.w * m.e33;
	return *this;
}

_inl Vector4D& 
Vector4D::mul( const Matrix4D& m, const Vector4D& v )
{
	x = v.x * m.e00 + v.y * m.e01 + v.z * m.e02 + v.w * m.e03;
	y = v.x * m.e10 + v.y * m.e11 + v.z * m.e12 + v.w * m.e13;
	z = v.x * m.e20 + v.y * m.e21 + v.z * m.e22 + v.w * m.e23;
	w = v.x * m.e30 + v.y * m.e31 + v.z * m.e32 + v.w * m.e33;
	return *this;
}

// v = v * M
_inl Vector4D& 
Vector4D::mul( const Matrix4D& m )
{
	Vector4D tmp; 
	tmp.x = x * m.e00 + y * m.e10 + z * m.e20 + w * m.e30;
	tmp.y = x * m.e01 + y * m.e11 + z * m.e21 + w * m.e31;
	tmp.z = x * m.e02 + y * m.e12 + z * m.e22 + w * m.e32;
	tmp.w = x * m.e03 + y * m.e13 + z * m.e23 + w * m.e33;
	*this = tmp;
	return *this;
}

//  v = M * v
_inl Vector4D& 
Vector4D::transform( const Matrix4D& m )
{
	Vector4D tmp; 
	tmp.x = x * m.e00 + y * m.e01 + z * m.e02 + w * m.e03;
	tmp.y = x * m.e10 + y * m.e11 + z * m.e12 + w * m.e13;
	tmp.z = x * m.e20 + y * m.e21 + z * m.e22 + w * m.e23;
	tmp.w = x * m.e30 + y * m.e31 + z * m.e32 + w * m.e33;
	*this = tmp;
	return *this;
}

_inl void	
Vector4D::set( float _x, float _y, float _z )
{
	x = _x;
	y = _y;
	z = _z;
	w = 1.0f;
}

_inl void	
Vector4D::set( float _x, float _y, float _z, float _w )
{
	x = _x;
	y = _y;
	z = _z;
	w = _w;
}

_inl void	
Vector4D::sub( const Vector4D& v1, const Vector4D& v2 )
{
	x = v1.x - v2.x;
	y = v1.y - v2.y;
	z = v1.z - v2.z;
	w = v1.w - v2.w;
}

_inl void	
Vector4D::add( const Vector4D& v1, const Vector4D& v2 )
{
	x = v1.x + v2.x;
	y = v1.y + v2.y;
	z = v1.z + v2.z;
	w = v1.w + v2.w;
}

_inl void	
Vector4D::sub( const Vector4D& v )
{
	x -= v.x;
	y -= v.y;
	z -= v.z;
	w -= v.w;
}

_inl void	
Vector4D::add( const Vector4D& v )
{
	x += v.x;
	y += v.y;
	z += v.z;
	w += v.w;
}

_inl float 
Vector4D::norm() const
{
	return static_cast<float>( sqrt( x * x + y * y + z * z  + w * w ) );
}

_inl float 
Vector4D::norm2() const
{
	return x * x + y * y + z * z + w * w;
}

_inl float 
Vector4D::distance( const Vector4D& orig ) const
{
	Vector4D tmp;
	tmp.copy( *this );
	tmp -= orig;
	return tmp.norm();
}

_inl float		
Vector4D::dot( const Vector4D& v ) const
{
	return x * v.x + y * v.y + z * v.z + w * v.w;
}


_inl const Vector4D& 
Vector4D::operator +=( const Vector4D& vec )
{
	x += vec.x;
	y += vec.y;
	z += vec.z;
	w += vec.w;
	return *this;
}

_inl const Vector4D& 
Vector4D::operator -=( const Vector4D& vec )
{
	x -= vec.x;
	y -= vec.y;
	z -= vec.z;
	w -= vec.w;
	return *this;
}

_inl const Vector4D& 
Vector4D::operator *=( const float val )
{
	x *= val;
	y *= val;
	z *= val;
	w *= val;
	return *this;
}

_inl const Vector4D& 
Vector4D::operator /=( const float val )
{
	x /= val;
	y /= val;
	z /= val;
	w /= val;
	return *this;
}

_inl void 
Vector4D::normalize()
{
	float len = static_cast<float>( sqrt( x * x + y * y + z * z + w * w) );
	x /= len;
	y /= len;
	z /= len;
	w /= len;
}

_inl void
Vector4D::normW()
{
	if (fabs( w ) > c_SmallEpsilon)
	{
		x /= w;
		y /= w;
		z /= w;
		w = 1.0f;
	}
}

_inl const Vector4D&	
Vector4D::copy( const Vector4D& orig )
{
	x = orig.x;
	y = orig.y;
	z = orig.z;
	w = orig.w;
	return *this;
}

_inl const Vector4D& 
Vector4D::operator =( const Vector3D& orig )
{
	x = orig.x;
	y = orig.y;
	z = orig.z;
	w = 1.0f;
	return *this;
}

_inl const Vector4D& 
Vector4D::operator =( const Vector4D& orig )
{
	x = orig.x;
	y = orig.y;
	z = orig.z;
	w = orig.w;
	return *this;
}


_inl float		
Vector4D::mul( const Vector4D& v )
{
	return v.x * x + v.y * y + v.z * z + v.w * w;
}

/*****************************************************************
/*  Quad2D implementation                                
/*****************************************************************/
template <class T> 
Quad2D<T>::Quad2D( T x, T y, T side )
{
	a.x = x;
	a.y = y;

	b.x = x + side;
	b.y = y;

	c.x = x + side;
	c.y = y + side;

	d.x = x + side;
	d.y = y;
}

template <class T> _inl void 
Quad2D<T>::Init( T ax, T ay, T bx, T by, T cx, T cy, T dx, T dy )
{
	a.x = ax;
	a.y = ay;

	b.x = bx;
	b.y = by;

	c.x = cx;
	c.y = cy;

	d.x = dx;
	d.y = dy;
}

template <class T> _inl bool 
Quad2D<T>::IsIntersecting( const Quad2D<T> r ) const
{
	if (r.contains( a )) return true;
	if (r.contains( b )) return true;
	if (r.contains( c )) return true;
	if (r.contains( d )) return true;
	
	if (contains( r.a )) return true;
	if (contains( r.b )) return true;
	if (contains( r.c )) return true;
	if (contains( r.d )) return true;

	return false;
}

template <class T> _inl bool 
Quad2D<T>::IsIntersecting( T x, T y, T side ) const
{
	return (a.inAAQuad( x, y, side ) || b.inAAQuad( x, y, side ) ||
			c.inAAQuad( x, y, side ) || d.inAAQuad( x, y, side ) );
}

template <class T> _inl bool 
Quad2D<T>::contains( const Vector2D<T> r ) const
{
	Line2D<T> l1( a, b );
	Line2D<T> l2( b, c );
	Line2D<T> l3( c, d );
	Line2D<T> l4( d, a );
	return	l1.inLeftHalf( r ) &&
			l2.inLeftHalf( r ) &&
			l3.inLeftHalf( r ) &&
			l4.inLeftHalf( r );
}

template <class T> _inl bool 
Quad2D<T>::contains( T _x, T _y ) const
{
	Line2D<T> l1( a, b );
	Line2D<T> l2( b, c );
	Line2D<T> l3( c, d );
	Line2D<T> l4( d, a );
	return	l1.inLeftHalf( _x, _y ) &&
			l2.inLeftHalf( _x, _y ) &&
			l3.inLeftHalf( _x, _y ) &&
			l4.inLeftHalf( _x, _y );
}

template <class T> _inl T 
Quad2D<T>::dist2ToPt( const Vector2D<T> pt ) const 
{
	T d1, d2, d3, d4;
	d1 = a.dist2( pt );
	d2 = b.dist2( pt );
	d3 = c.dist2( pt );
	d4 = d.dist2( pt );
	if (d2 < d1) d1 = d2;
	if (d3 < d1) d1 = d3;
	if (d4 < d1) d1 = d4;
	return d1; 
}


template <class T> _inl void 
Quad2D<T>::Init( Vector2D<T>& _a, Vector2D<T>& _b, Vector2D<T>& _c, Vector2D<T>& _d )
{
	a = _a;
	b = _b;
	c = _c;
	d = _d;
}

/*****************************************************************
/*  Quad3D implementation                                
/*****************************************************************/
void _inl 
Quad3D::CreateRect( const Vector3D& topLeft, 
							  const Vector3D& botRight,
							  const Vector3D& botLeft )
{
	a = topLeft;
	c = botRight;
	d = botLeft;

	b = a;
	b.add( c );
	b.sub( d );
} // Quad3D::CreateRect

/*---------------------------------------------------------------*
/*  Func:	Quad3D::getArea
/*	Desc:	Finds the area of the quad with given four vertices
/*				coords. It is assumed that vertices are in CW or
/*				CCW order
/*---------------------------------------------------------------*/
_inl float Quad3D::getArea() const
{	
	Vector3D v1, v2, v3, v4;
	v1.sub( b, a );
	v2.sub( d, a );
	v3.sub( b, c );
	v4.sub( d, c );
	return v1.TriArea( v2 ) + v3.TriArea( v4 );
} // Math3D::QuadArea

/*****************************************************************
/*	AABoundBox implementation
/*****************************************************************/
_inl void AABoundBox::getCenter( Vector3D& dest ) const
{
	dest.copy( minv );
	dest += maxv;
	dest /= 2.0f;
}

_inl float AABoundBox::getDiagonal() const
{
	return minv.distance( maxv );
}

_inl float AABoundBox::distance2( const Vector3D& pt )
{
	Vector3D center;
	getCenter( center );
	return pt.distance2( center );
}

_inl float AABoundBox::distance( const Vector3D& pt )
{
	Vector3D center;
	getCenter( center );
	return pt.distance( center );
}

_inl void AABoundBox::Union( const AABoundBox& orig )
{
	if (minv.x > orig.minv.x) minv.x = orig.minv.x;
	if (minv.y > orig.minv.y) minv.y = orig.minv.y;
	if (minv.z > orig.minv.z) minv.z = orig.minv.z;

	if (maxv.x < orig.maxv.x) maxv.x = orig.maxv.x;
	if (maxv.y < orig.maxv.y) maxv.y = orig.maxv.y;
	if (maxv.z < orig.maxv.z) maxv.z = orig.maxv.z;
} // AABoundBox::Union

_inl void AABoundBox::copy( const AABoundBox& orig )
{
	minv.copy( orig.minv );
	maxv.copy( orig.maxv );
}

/*---------------------------------------------------------------------------*/
/*	Func:	AABoundBox::intersectRay	
/*	Parm:	pt - intersection point to place in
/*			orig, dir - ray
/*	Ret:	true when ray intersects AABB
/*	Rmrk:	Adapted from Pierre Tierdiman code
/*---------------------------------------------------------------------------*/
_inl bool AABoundBox::intersectRay(			Vector3D& pt,
									const	Vector3D& orig, 
									const	Vector3D& dir ) const
{
	//  TODO
	/*
	#define RAYAABB_EPSILON 0.00001f
	bool inside = true;
	Point MinB = aabb.mCenter - aabb.mExtents;
	Point MaxB = aabb.mCenter + aabb.mExtents;
	Point MaxT;
	MaxT.x=MaxT.y=MaxT.z=-1.0f;

	// Find candidate planes.
	for(udword i=0;i<3;i++)
	{
		if(origin.m[i] < MinB.m[i])
		{
			coord.m[i]	= MinB.m[i];
			Inside		= FALSE;

			// Calculate T distances to candidate planes
			if(IR(dir.m[i]))	MaxT.m[i] = (MinB.m[i] - origin.m[i]) / dir.m[i];
		}
		else if(origin.m[i] > MaxB.m[i])
		{
			coord.m[i]	= MaxB.m[i];
			Inside		= FALSE;

			// Calculate T distances to candidate planes
			if(IR(dir.m[i]))	MaxT.m[i] = (MaxB.m[i] - origin.m[i]) / dir.m[i];
		}
	}

	// Ray origin inside bounding box
	if(Inside)
	{
		coord = origin;
		return true;
	}

	// Get largest of the maxT's for final choice of intersection
	udword WhichPlane = 0;
	if(MaxT.m[1] > MaxT.m[WhichPlane])	WhichPlane = 1;
	if(MaxT.m[2] > MaxT.m[WhichPlane])	WhichPlane = 2;

	// Check final candidate actually inside box
	if(IR(MaxT.m[WhichPlane])&0x80000000) return false;

	for(int i=0;i<3;i++)
	{
		if(i!=WhichPlane)
		{
			coord.m[i] = origin.m[i] + MaxT.m[WhichPlane] * dir.m[i];

			if(coord.m[i] < MinB.m[i] - RAYAABB_EPSILON || coord.m[i] > MaxB.m[i] + RAYAABB_EPSILON)	return false;
		}
	}
	return true;	// ray hits box
	*/
	return false;
}

/*****************************************************************
/*	Frustum implementation
/*****************************************************************/
/*---------------------------------------------------------------*
/*  Func:	Frustum::intersect
/*	Desc:	Tests frustum - axis-aligned bounding box intersection
/*	Parm:	aabb - reference to the axis-aligned bounding box
/*	Ret:	isInside	- aabb is totally inside frustum
/*			isOutside	- aabb is totally outside frustum
/*			isIntersect	- aabb is partially inside, partially 
/*					outside, e.g. intersects frustum
/*	FIXME:  clipping for far and near frustum planes
/*---------------------------------------------------------------*/
_inl IntersectStatus	
Frustum::intersect( AABoundBox& aabb ) const
{
	//  check for all frustum planes
	bool intersect = false;
	for (int i = 2; i < 6; i++)
	{
		BYTE mask;
		float resultN = 0.0f, resultP = 0.0f;
		
		float* pCoord;
		float* pPlane;

		//  far negative point
		mask	= aabbMask[i].NMask;
		pCoord	= (float*)(&aabb);
		pPlane	= (float*)&plane[i];

		if (mask & 1)  resultN += pCoord[0] * pPlane[0];
		if (mask & 2)  resultN += pCoord[1] * pPlane[1];
		if (mask & 4)  resultN += pCoord[2] * pPlane[2];
		if (mask & 8)  resultN += pCoord[3] * pPlane[0];
		if (mask & 16) resultN += pCoord[4] * pPlane[1];
		if (mask & 32) resultN += pCoord[5] * pPlane[2];

		resultN += pPlane[3];

		if (resultN < 0) return isOutside;

		//  far positive point
		mask	= aabbMask[i].PMask;
		
		if (mask & 1)  resultP += pCoord[0] * pPlane[0];
		if (mask & 2)  resultP += pCoord[1] * pPlane[1];
		if (mask & 4)  resultP += pCoord[2] * pPlane[2];
		if (mask & 8)  resultP += pCoord[3] * pPlane[0];
		if (mask & 16) resultP += pCoord[4] * pPlane[1];
		if (mask & 32) resultP += pCoord[5] * pPlane[2];

		resultP += pPlane[3];

		if (resultP > 0) intersect = true;
	}
	if (intersect) return isIntersects;
	return isInside;
}//  Frustum::intersect


_inl const Vector3D& Frustum::getVertex( int idx ) const
{
	return vector[idx];
}

_inl const Vector3D& Frustum::getNearLTVec() const
{
	return vector[2];
}

_inl const Vector3D& Frustum::getNearLBVec() const
{
	return vector[0];
}

_inl const Vector3D& Frustum::getNearRTVec() const
{
	return vector[3];
}

_inl const Vector3D& Frustum::getNearRBVec() const
{
	return vector[1];
}

_inl const Vector3D& Frustum::getFarLTVec() const
{
	return vector[6];
}

_inl const Vector3D& Frustum::getFarLBVec() const
{
	return vector[4];
}

_inl const Vector3D& Frustum::getFarRTVec() const
{
	return vector[7];
}

_inl const Vector3D& Frustum::getFarRBVec() const
{
	return vector[5];
}

/*****************************************************************
/*  AABBIntersectMask implementation                             *
/*****************************************************************/
/*---------------------------------------------------------------*
/*  Func:	AABBIntersectMask::fromPlaneNormal
/*	Desc:	Fills intersect mask values
/*	Parm:	a, b, c - plane's normal coords 
/*	Rmrk:	This is tied to the physical struct of the AABoundBox
/*---------------------------------------------------------------*/
_inl void 
AABBIntersectMask::fromPlaneNormal( float a, float b, float c )
{
	NMask = 0;
	PMask = 0;

	if (a < 0) NMask |= 0x01; else NMask |= 0x08;
	if (b < 0) NMask |= 0x02; else NMask |= 0x10;
	if (c < 0) NMask |= 0x04; else NMask |= 0x20;
	PMask = ~NMask;
}//  AABBIntersectMask::fromPlaneNormal


/*****************************************************************
/*	Plane implementation
/*****************************************************************/
_inl void
Plane::from3Points(	const Vector3D& v1, 
					const Vector3D& v2,
					const Vector3D& v3 )
{
	Vector3D av, bv;
	av.sub( v1, v2 );
	bv.sub( v3, v2 );
	Vector3D* normal = reinterpret_cast<Vector3D*>( &a );

	normal->cross( av, bv );
	normal->normalize();
	d = - normal->dot( v2 );
}

_inl void 
Plane::fromPointNormal(	const Vector3D& pt, 
						const Vector3D& norm )
{
	a = norm.x;
	b = norm.y;
	c = norm.z;
	d = - pt.dot( norm );
}

_inl void 
Plane::fromPointNormal(	const Vector4D& pt, 
						const Vector4D& norm )
{
	a = norm.x;
	b = norm.y;
	c = norm.z;
	d = - pt.x * norm.x - pt.y * norm.y - pt.z * norm.z;
}

_inl float 
Plane::getZ( float x, float y ) const
{
	if (c == 0) return 0.0f;
	return (-a*x - b*y - d) / c;
}

_inl float 
Plane::getY( float x, float z ) const
{
	if (b == 0) return 0.0f;
	return (-a*x - c*z - d) / b;
}

_inl float 
Plane::getX( float y, float z ) const
{
	if (a == 0) return 0.0f;
	return (-b*y - c*z - d) / a;
}

_inl bool 
Plane::isPerpendicular( const Plane& p ) const
{
	//  check the dot product of plane normals
	return a * p.a + b * p.b + c * p.c <= c_SmallEpsilon;
}

_inl Vector3D*
Plane::getNormal() 
{
	return reinterpret_cast<Vector3D*>( &a );
}

_inl Vector4D*
Plane::asVector()
{
	return reinterpret_cast<Vector4D*>( &a );
}

_inl void Plane::txtSave( FILE* fp )
{
	asVector()->txtSave( fp );
}

/*---------------------------------------------------------------------------*/
/*	Func:	Plane::intersect
/*	Desc:	finds intersection point of three planes
/*	Parm:	p1 - 2nd plane
/*			p2 - 3rd plane
/*			pt - intersection point to return 
/*	Ret:	true if such point exist
/*---------------------------------------------------------------------------*/
_inl bool 
Plane::intersect( const Plane& p1, const Plane& p2, Vector3D& pt )
{
	Matrix3D A, Ainv;
	A.e00 = a;
	A.e10 = b;
	A.e20 = c;

	A.e01 = p1.a;
	A.e11 = p1.b;
	A.e21 = p1.c;

	A.e02 = p2.a;
	A.e12 = p2.b;
	A.e22 = p2.c;

	float det = Ainv.inverse( A );
	if (det < c_SmallEpsilon) return false;

	pt.x = -d;
	pt.y = -p1.d;
	pt.z = -p2.d;

	pt *= Ainv;
	return true;
}

/*****************************************************************
/*	Quaternion implementation
/*****************************************************************/
/*---------------------------------------------------------------*
/*  Func:	Quaternion::conjugate
/*	Desc:	calculates conjugate to orig quaternion
/*---------------------------------------------------------------*/
_inl void Quaternion::conjugate( const Quaternion& orig )
{
	v.x = -orig.v.x;
	v.y = -orig.v.y;
	v.z = -orig.v.z;
	s =  orig.s;
}//  Quaternion::conjugate

/*---------------------------------------------------------------*
/*  Func:	Quaternion::norm2
/*	Desc:	Squared norm of the quaternion
/*---------------------------------------------------------------*/
_inl float Quaternion::norm2() const
{
	return s * s + v.norm2();
}

_inl void Quaternion::reverse()
{
	s = -s;
	v.reverse();
}

/*---------------------------------------------------------------*
/*  Func:	Quaternion::dot
/*	Desc:	finds dot product of two quaternions
/*---------------------------------------------------------------*/
_inl float Quaternion::dot( const Quaternion& q ) const
{
	return s * s + v.x * q.v.x + v.y * q.v.y + v.z * q.v.z;
}
	

/*---------------------------------------------------------------*
/*  Func:	Quaternion::slerp
/*	Desc:	Spherical linear interpolation between two quaternions
/*				using the shortest arc
/*	Parm:	q1 - source quaternion
/*			q2 - destination quaternion
/*			t  - interpolation parameter, 0<=t<=1	
/*---------------------------------------------------------------*/
_inl void Quaternion::slerp(	const Quaternion& q1, 
								const Quaternion& q2, 
								float t )
{
	Quaternion tmp( q2 );
	float cosTheta = q1.dot( q2 );
	
	//  choose the sign of q2, to keep the angle between q1 & q2 acute
	if (cosTheta < 0.0f)
	{
		cosTheta = -cosTheta;
		tmp.reverse();
	}
    
	float t1, t2;
	if (1.0f - cosTheta > c_SmallEpsilon) 
	{
		float theta = acos( cosTheta );
		float invSinTheta = 1.0f / sin( theta );
		t1 = sin( (1.0f - t) * theta);
		t2 = sin( t * theta );
	
		v.x = t1 * q1.v.x	+ t2 * tmp.v.x;
		v.y = t1 * q1.v.y	+ t2 * tmp.v.y;
		v.z = t1 * q1.v.z	+ t2 * tmp.v.z;
		s	= t1 * q1.s		+ t2 * tmp.s;
		
		v *= invSinTheta;
		s *= invSinTheta;
	} 
	else 
	// do simple linear interpolation (arc is very close to segment,
	//	so why bother?)
	{
		t1 = 1.0f - t;
		t2 = t;

		v.x = t1 * q1.v.x	+ t2 * tmp.v.x;
		v.y = t1 * q1.v.y	+ t2 * tmp.v.y;
		v.z = t1 * q1.v.z	+ t2 * tmp.v.z;
		s	= t1 * q1.s		+ t2 * tmp.s;
	}
}


/*---------------------------------------------------------------*
/*  Func:	Quaternion::setIdentity
/*	Desc:	sets identity quaternion
/*---------------------------------------------------------------*/
_inl void Quaternion::setIdentity()
{
	v.zero();
	s = 1.0f;
}//  Quaternion::setIdentity

/*---------------------------------------------------------------*
/*  Func:	Quaternion::operator *=
/*	Desc:	Quaternion composition
/*---------------------------------------------------------------*/
_inl const Quaternion& Quaternion::operator *=( const Quaternion& q )
{
	Vector3D tmp;
	tmp.cross( v, q.v ); 
	tmp.x += v.x * q.s + q.v.x * s;
	tmp.y += v.y * q.s + q.v.y * s;
	tmp.z += v.z * q.s + q.v.z * s;
	s = s * q.s - v.dot( q.v );
	v.copy( tmp );
	return *this;
}

/*---------------------------------------------------------------*
/*  Func:	Quaternion::axisToAxis
/*	Desc:	Takes two points on unit sphere an angle THETA apart, 
/*			returns quaternion that represents a rotation around 
/*			cross product by theta.
/*---------------------------------------------------------------*/
_inl void Quaternion::axisToAxis( const Vector3D& from, const Vector3D& to )
{
	Vector3D tmp, half;
	tmp.copy( from );
	half.copy( to );
	
	tmp.normalize();
	half.normalize();

	half.add( tmp );
	half.normalize();

	v.cross( tmp, half );
	s = tmp.dot( half );
}//  axisToAxis

/*---------------------------------------------------------------*
/*  Func:	Quaternion::fromAxisAngle
/*	Desc:	Builds unit quaternion from rotational axis and angle
/*---------------------------------------------------------------*/
_inl void Quaternion::fromAxisAngle( const Vector3D& axis, float angle )
{
	float half = angle / 2.0f;
	s = cos ( half );
	v.copy( axis );
	v.normalize();
	v *= sin( half );
}//  fromAxisAngle

/*---------------------------------------------------------------*
/*  Func:	Quaternion::fromEulerAngles
/*	Desc:	Builds unit quaternion from Euler angles
/*	Parm:	yaw		- yaw around the y-axis
/*			pitch	- pitch around the x-axis
/*			roll	- roll around the z-axis
/*---------------------------------------------------------------*/
_inl void Quaternion::fromEulerAngles( float yaw, float pitch, float roll )
{
	float sinYaw	= sin( yaw * 0.5f );
	float cosYaw	= cos( yaw * 0.5f );
	float sinPitch	= sin( pitch * 0.5f );
	float cosPitch	= cos( pitch * 0.5f );
	float sinRoll	= sin( roll * 0.5f );
	float cosRoll	= cos( roll * 0.5f );
	float cPcY		= cosPitch * cosYaw;
	float sPsY		= sinPitch * sinYaw;

	s	= cosRoll * cPcY + sinRoll * sPsY;
	v.x = sinRoll * cPcY - cosRoll * sPsY;
	v.y = cosRoll * sinPitch * cosYaw + sinRoll * cosPitch * sinYaw;
	v.z = cosRoll * cosPitch * sinYaw - sinRoll * sinPitch * cosYaw;
} // Quaternion::fromEulerAngles

/*---------------------------------------------------------------------------*/
/*	Func:	Quaternion::fromMatrix	
/*	Desc:	builds quaternion from rotational matrix
/*---------------------------------------------------------------------------*/
_inl void Quaternion::fromMatrix( const Matrix3D& rotM )
{
	assert( false );
} // Quaternion::fromMatrix

/*****************************************************************
/*	Cylinder implementation
/*****************************************************************/
_inl bool Cylinder::isInfinite() const
{
	return r2 < 0.0f;
}

_inl void Cylinder::setInfinite()
{
	if (r2 < 0.0f) return;
	r2 = -r2;
	core.normalize();
}

_inl Vector3D Cylinder::getBase() const
{
	return base;
}

_inl Vector3D Cylinder::getDir() const
{
	assert( isInfinite() );
	return core;
}

_inl Vector3D Cylinder::getTop() const
{
	assert( !isInfinite() );
	Vector3D vec( base );
	vec.add( core );
	return vec;
}

_inl void Cylinder::getCenter( Vector3D& cen ) const
{
	if (isInfinite()) return;
	cen.copy( core );
	cen *= 0.5f;
	cen.add( base );
}

_inl float Cylinder::getR() const
{
	return sqrt( fabs( r2 ) );
}

_inl float Cylinder::getR2() const
{
	return fabs( r2 );
}

_inl bool Cylinder::isPointInside( const Vector3D& pt ) const
{
	if (isInfinite())
	{
		Vector3D tmp;
		tmp.sub( pt, base );
		float dirPr = tmp.dot( core );
		if (dirPr < 0.0f) return false;
		dirPr *= dirPr;
		return tmp.norm2() - dirPr <= -r2;
	}
	else
	{
		assert( false ); // TODO
		return false;
	}
}

_inl void Cylinder::transform( const Matrix3D& matr )
{
	base *= matr;
    core *= matr;

    if (isInfinite())
    {
        core.normalize();
	    Vector3D vec( 0.0f, 0.0f, sqrt( -r2 ) );
	    vec *= matr;
	    r2 = -vec.norm();
    }
    else
    {
	    Vector3D vec( 0.0f, 0.0f, sqrt( r2 ) );
	    vec *= matr;
	    r2 = vec.norm();
    }
}

_inl void Cylinder::transform( const Matrix4D& matr )
{
	base *= matr;

    Vector4D core4( core );
    core4.w = 0.0f;
    core4.mul( matr );
    core.copy( core4 );

	if (isInfinite())
    {
        core.normalize();
	    Vector4D vec( 0.0f, 0.0f, sqrt( -r2 ) );
        vec.w = 0.0f;
	    vec.mul( matr );
	    r2 = -vec.norm();
    }
    else
    {
	    Vector4D vec( 0.0f, 0.0f, sqrt( r2 ) );
	    vec.mul( matr );
        vec.w = 0.0f;
	    r2 = vec.norm();
    }
}

_inl void Cylinder::getAxis( Vector3D& axis ) const
{
	if (isInfinite()) 
	{
		axis.copy( core );
		return;
	}
	axis.sub( core, base );
	axis.normalize();
}

/*---------------------------------------------------------------------------*/
/*	Func:	Cylinder::getVertex	
/*	Desc:	returns coordinates of the vertex on the cylinder
/*	Parm:	angle - vertex phi ( in cylinder coordinates )
/*			h	  - vertex height from base along cylinder axis
/*	Remark:	
/*---------------------------------------------------------------------------*/
_inl void Cylinder::getVertex( float angle, float h, Vector3D& vert ) const
{
	if (isInfinite()) return;
	Vector4D pt;
	//  first in local cylinder coordinate system
	pt.x = cos( angle );
	pt.y = sin( angle );
	pt.z = h;
	//  convert to the world coordinate system
	
	pt.mul( getLocalToWorld() );
	vert.copy( pt );
}


_inl float Cylinder::getHeight() const
{
	if (isInfinite()) return -1.0f;
	return core.norm();
}

_inl Matrix4D Cylinder::getLocalToWorld() const
{
	assert( false );
	Vector4D vx, vy;

	return Matrix4D(
						vx.x,		vy.x,		core.x,		0.0f, 
						vx.y,		vy.y,		core.y,		0.0f, 
						vx.z,		vy.z,		core.z,		0.0f, 
						-base.x,	-base.y,	-base.z,	1.0f
		
					);
}


_inl void Cylinder::create(	const Vector3D& bottom, 
							const Vector3D& top, 
							float R2 )
{
	base.copy( bottom );
    r2 = R2;
    if (R2 < 0.0f )
    {
        core.copy( top );
        core.normalize();
    }
    else
    {
        core.copy( top );
        core.sub( base );
    }
}

/*---------------------------------------------------------------------------*/
/*	Func:	Cylinder::Union	
/*	Desc:	Finds the union of the two cylinders, places result 
/*			into the current cylinder
/*	Rmrk:	Currently cylinders should have the same axis	
/*---------------------------------------------------------------------------*/
_inl void Cylinder::Union( const Cylinder& cyl )
{
	//  new axis is average between two cylinders' axises
	Vector3D ax1, ax2, nbase, c1, c2;
	
	getAxis( ax1 );
	getCenter( c1 );

	cyl.getAxis( ax2 );
	cyl.getCenter( c2 );

	Vector3D naxis;
	float pr = ax1.dot( ax2 );
	if (pr >= 0.0f) naxis.add( ax1, ax2 );
	else naxis.add( ax1, ax2 );

	naxis.normalize();

	Line3D line( naxis, nbase );

	float d1 = 0.0f, d2 = 0.0f;
	r2 = maxf( d1, d2 );
}

/*****************************************************************
/*	Frame implementation
/*****************************************************************/
_inl float Frame::getAspect() const
{
     return w / h;
}

_inl float Frame::getRight() const
{
	return x + w;
}

_inl float Frame::getBottom() const
{
	return y + h;
}

_inl float Frame::maxSide()	const
{
	return w > h ? w : h;
}

_inl void Frame::copy( const Frame& orig )
{
	x = orig.x;
	y = orig.y;
	h = orig.h;
	w = orig.w;
}

_inl void Frame::set( float _x, float _y, float _w, float _h )
{
	x = _x;
	y = _y;
	w = _w;
	h = _h;
}

//  fits this rectangle into the rct, constraining the proportion
_inl void  Frame::fitInto( const Frame& rct )
{
     float asp = getAspect();
     if (rct.getAspect() > asp)
     {
         h = rct.h;
         w = h * asp;
         x = rct.x + (rct.w - w) / 2.0f;
         y = rct.y;
     }
     else
     {
         w = rct.w;
         h = w / asp;
         x = rct.x;
         y = rct.y + (rct.h - h) / 2.0f;
     }
}

/*****************************************************************************/
/*	Line3D implementation
/*****************************************************************************/
_inl float Line3D::project( const Vector3D& vec ) const
{
	Vector3D pt( vec );
	pt -= orig;
	return pt.dot( dir );
}

_inl float Line3D::dist2ToOrig( const Vector3D& vec ) const
{
	Vector3D pt( vec );
	pt -= orig;
	return pt.norm2();
}

/*---------------------------------------------------------------------------*/
/*	Func:	Line3D::getPoint	
/*	Desc:	Finds point on the line	
/*	Parm:	prLen - distance of the result line point from line origin
/*---------------------------------------------------------------------------*/
_inl void Line3D::getPoint( float prLen, Vector3D& pt ) const
{
	pt.copy( dir );
	pt *= prLen;
	pt += orig;
}


/*****************************************************************
/*	Global functions implementation
/*****************************************************************/
/*---------------------------------------------------------------*
/*  Func:  RaySphereX                                            *
/*  Desc:  Founds if ray intersects sphere                       *
/*  Parm:  orig, dir - ray origin/direction                      *
/*		   center - sphere center								 *
/*		   R2 - sphere radius squared							 *
/*  Ret:   true if ray intersects sphere                         *
/*  Rmrk:  dir should be normalized!!!							 *
/*---------------------------------------------------------------*/
_inl bool RaySphereX( const Vector3D& orig, const Vector3D& dir,
   					  const Vector3D& center, float R2 )
{
	Vector3D tmp;
	tmp.sub( center, orig );
	float dirPr = tmp.dot( dir );
	dirPr *= dirPr;
	return tmp.norm2() - dirPr <= R2;
} // RaySphereX

/*---------------------------------------------------------------------------*/
/*	Func:	sqdistPt2D
/*	Desc:	squared distance between two plane points
/*---------------------------------------------------------------------------*/
_inl float sqdistPt2D( float x1, float y1, float x2, float y2 )
{
	return (x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2);
}

/*---------------------------------------------------------------------------*/
/*	Func:	Seg2Seg2X	
/*	Desc:	finds if two 2d segments are crossing
/*	Parm:	
/*	Ret:	
/*	Remark:	
/*---------------------------------------------------------------------------*/
_inl bool Seg2Seg2X(	float a1x, float a1y, float b1x, float b1y,
						float a2x, float a2y, float b2x, float b2y,
						float& crossX, float& crossY )
{ 
	return false;
}




