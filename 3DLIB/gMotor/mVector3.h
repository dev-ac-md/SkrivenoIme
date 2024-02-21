/*****************************************************************
/*  File:   mVector3.h	                                         *
/*	Desc:	3D Vector declaration								 *
/*  Author: Silver, Copyright (C) GSC Game World                 *
/*  Date:   January 2002                                         *
/*****************************************************************/
#ifndef __MVECTOR3_H__
#define __MVECTOR3_H__
#pragma	once

/*****************************************************************
/*  Class:  Vector3                                              *
/*  Desc:   3-dimensional vector                                 *
/*****************************************************************/
class Vector3			
{
public:
	float			x;
	float			y;
	float			z;

					Vector3( const Vector3& orig );
					Vector3( const Vector4& orig );
					Vector3(  float _x = 0.0f,
							   float _y = 0.0f, 
							   float _z = 0.0f );

	_inl void		set( float _x, float _y, float _z );
	_inl void		sub( const Vector3& v1, const Vector3& v2 );
	_inl void		add( const Vector3& v1, const Vector3& v2 );
	_inl void		cross( const Vector3& v1, const Vector3& v2 );
	_inl void		sub( const Vector3& v );
	_inl void		add( const Vector3& v );

	_inl void		reverse( const Vector3& orig );
	_inl void		reverse();

	_inl float		dot( const Vector3& v ) const;

	_inl const Vector3& operator +=( const Vector3& vec );
	_inl const Vector3& operator -=( const Vector3& vec );
	_inl const Vector3& operator *=( const float val );
	_inl const Vector3& operator *=( const Matrix3& m );
	_inl const Vector3& operator *=( const Matrix4& m );

	_inl const Vector3& mulLeft( const Matrix3& m );
	_inl const Vector3& mulPt( const Vector3& v, const Matrix4& m );
	_inl const Vector3& mulPt( const Matrix4& m );
	
	_inl const Vector3& operator /=( const float val );

	_inl const Vector3& operator =( const Vector3& orig );
	_inl const Vector3& operator =( const Vector4& orig );

	_inl bool		isEqual( const Vector3& orig ) const;



	_inl float		norm() const;
	_inl float		norm2() const;
	_inl void		normalize();
	_inl float		distance( const Vector3& orig ) const;
    _inl float		distance2( const Vector3& orig ) const;

	_inl void		copy( const Vector3& orig );
	_inl void		copy( const Vector4& orig );

	_inl float		TriArea( const Vector3& v ) const; 

	_inl void		PlaneIntersectLine( Plane& p, 
										const Vector3& v1,
										const Vector3& v2 );

	_inl void		planeNormal(	const Vector3& v1,
									const Vector3& v2,
									const Vector3& v3 );

	_inl bool		isColinear( const Vector3& orig ) const;
	_inl void		zero();

	void			Dump();

	void			splineCatmullRom( float t, 
										const Vector3& p0,
										const Vector3& p1,
										const Vector3& p2,
										const Vector3& p3 );

	void			txtSave( FILE* fp );

	static  const	Vector3 oX;
	static  const	Vector3 oY;
	static  const	Vector3 oZ;

}; // class Vector3

#ifdef _INLINES
#include "mVector3.inl"
#endif // _INLINES

#endif // __MVECTOR3_H__