/*****************************************************************************/
/*	File:	mUtil.h
/*	Desc:	math utilities declarations
/*	Author:	Ruslan Shestopalyuk
/*	Date:	16.01.2003
/*****************************************************************************/
#ifndef __MUTIL_H__
#define __MUTIL_H__
 
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

_inl bool RaySphereX( const Vector3& orig, const Vector3& dir, 
					  const Vector3& center, float R );

_inl bool RayTriangleX(	const Vector3&	orig, 
						const Vector3&	dir, 
						const Vector3&	v0, 
						const Vector3&	v1, 
						const Vector3&	v2, 
						float& u, float& v, 
						float& t );

_inl float rndValuef();
_inl float rndValuef( float min, float max );


#ifdef _INLINES
#include "mUtil.inl"
#endif //  _INLINES

#endif // __MUTIL_H__