/*****************************************************************/
/*  File:   Camera.cpp
/*  Desc:   
/*  Author: Silver, Copyright (C) GSC Game World
/*  Date:   Feb 2002
/*****************************************************************/
#include "stdafx.h"
#include "mMath3D.h"
#include "mCamera.h"

#ifndef _INLINES
#include "mCamera.inl"
#endif //!_INLINES

/*****************************************************************/
/*  Camera implementation
/*****************************************************************/
Camera::Camera()
{
	reset();
}

Camera::Camera( const char* camName )
{
	reset();
}

void Camera::reset()
{
	pos.set( 0.0f, 0.0f, 0.0f );
    dir.set( 0.0f, 0.0f, 1.0f );
    up.set( 0.0f, 1.0f, 0.0f );

    Zn      = 1.0f;
    Zf      = 100.0f;
    
	scrW	= 1024.0f;
	scrH	= 768.0f;
	scrX	= 0.0f;
	scrY	= 0.0f;
    
	setHFOV( c_HalfPI );
    setOrtho( false );
	setViewVolumeWidth( 1.0f );

    at. set( 0.0f, 0.0f, 1.0f );

	calcProjM();
	calcViewM();
	frustum.calculate( viewM, projM );
}

void Camera::setPos( float x, float y, float z )
{
	pos.set( x, y, z );
	calcViewM();
}

void Camera::setPos( const Vector3D& pt )
{
	pos.copy( pt );
	calcViewM();
}

/*---------------------------------------------------------------*
/*  Func:	Camera::FocusOnAABB
/*	Desc:	positions camera to fit the object in the viewport
/*			current look direction is preserved
/*	Parm:	aabb - axis-aligned bounding box of the object
/*---------------------------------------------------------------*/
void Camera::FocusOnAABB( AABoundBox& aabb )
{
	float maxSize = aabb.getDiagonal();
	aabb.getCenter( at );

	pos.copy( at );
	dir.normalize();
	dir *= maxSize * 2 / (2 * tan( getHFOV() / 2.0f ) );
	pos -= dir;
    dir.normalize();
    calcViewM();
}//  Camera::FocusOnAABB

/*---------------------------------------------------------------------------*/
/*	Func:	Camera::RotateAroundLookAtPoint	
/*	Desc:	
/*---------------------------------------------------------------------------*/
void Camera::RotateAroundLookAtPoint( float ang )
{
	Matrix4D rotm;
	rotm.rotation( getUp(), ang );
	Vector4D ndir( getDir() );
	ndir.w = 0.0f;
	ndir.mul( rotm );
	float dist = getPos().distance( getAt() );
	ndir.normalize();
	setDir( ndir );
	Vector3D npos( ndir );
	npos.reverse();
	npos *= dist;
	npos.add( getAt() );
	setPos( npos );
} // Camera::RotateAroundLookAtPoint

void Camera::calcProjM()
{
	float w, h;

	projM.setNull();

    if (bOrtho)
	{
		w = viewVolWidth;
		h = viewVolWidth * scrH / scrW;

		projM.e00  = 2.0f / w;
		projM.e11  = 2.0f / h;
		projM.e22  = 1 / (Zf - Zn);
		projM.e32  = -Zn / (Zf - Zn);
		projM.e33  = 1.0f;
	}
	else
    {
		float fovd2 = hFov * 0.5f;
		w = 1.0f / tan( fovd2 );
		h = w * scrW / scrH;
		projM.e00  = w;
		projM.e11  = h;
		projM.e22  = Zf / (Zf - Zn);
		projM.e23  = 1.0f;
		projM.e32  = Zn * Zf / (Zn - Zf);
    }
} // Camera::calcProjM

void Camera::calcViewM()
{
	float* el = viewM.getBuf();
	Vector3D xax, yax, zax;

	zax.copy( dir );
	xax.cross( up, zax );
	xax.normalize();
	yax.cross( zax, xax );
	
	el[0] = xax.x;
	el[1] = yax.x;
	el[2] = zax.x;
	el[3] = 0.0f;
	el[4] = xax.y;
	el[5] = yax.y;
	el[6] = zax.y;
	el[7] = 0.0f;
	el[8] = xax.z;
	el[9] = yax.z;
	el[10]= zax.z;
	el[11]= 0.0f;
	el[12]= -xax.dot( pos );
	el[13]= -yax.dot( pos );
	el[14]= -zax.dot( pos );
	el[15]= 1.0f;

	fullM.mul( viewM, projM );
	//frustum.calculate( viewM, projM );
} // Camera::calcViewM

void Camera::UnviewOnXOY( Vector4D& vec )
{
	float Det = viewM.e00 * viewM.e11 - viewM.e01 * viewM.e10;
	if (fabs( Det ) < c_SmallEpsilon) return;
	float a = vec.x - viewM.e30;
	float b = vec.y - viewM.e31;
	
	vec.x = (a * viewM.e11 + b * viewM.e10) / Det;
	vec.y = (a * viewM.e01 + b * viewM.e00) / Det;
	vec.z = 0.0f;

} // Camera::UnviewOnXOY

void Camera::getPickRay( float curX, float curY, Vector3D& rayOrig, Vector3D& rayDir )
{
    if (bOrtho)
    //  pick ray for the orthogonal projection
    {
        rayDir.set( viewM.e02, viewM.e12, viewM.e22 ); 
        curX -= scrW * 0.5f;
        curX /= scrW * 0.5f;
        curY = scrH * 0.5f - curY;
        curY /= scrH * 0.5f;
        Matrix4D  m;
		//  for orthogonal matrix inverse == transpose
	    m.transpose( viewM );
        Vector4D pt( curX, curY, -10.0f ); // HACK!!! Woo-ha-ha-ha!!!
        pt.mul( m );
        rayOrig.set( pt.x, pt.y, pt.z );
        return;
    }

	Matrix4D  m;
	m.inverse( viewM );

    Vector4D v;
	float hscrW = scrW * 0.5f;
	float hscrH = scrH * 0.5f;

    v.x = (curX - hscrW)/hscrW;
    v.y = (hscrH - curY)/hscrH;
    v.z = 1.0f;
	v.w = 1.0f;

	v.x /= projM.e00;
	v.y /= projM.e11;

	Vector4D vec;
    vec.mul( v, m );
	rayOrig.copy( getPos() );

	rayDir.set( vec.x, vec.y, vec.z );
	rayDir.sub( rayOrig );
	rayDir.normalize();
}

void Camera::txtSave( FILE* fp )
{
	if (isOrtho()) fprintf( fp, "(ortho)\n" ); else fprintf( fp, "(persp)\n" );
	fprintf( fp, "pos(%-2.4f %-2.4f %-2.4f);\n dir(%-2.4f %-2.4f %-2.4f);\n ",
					pos.x, pos.y, pos.z, dir.x, dir.y, dir.z );
	fprintf( fp, "up(%-2.4f %-2.4f %-2.4f);\n",
					up.x, up.y, up.z );
	fprintf( fp, "Zn=%-2.4f; Zf=%-2.4f; vFov=%-2.4f; hFov=%-2.4f;",
					Zn, Zf, hFov, getHFOV() );
}

/*---------------------------------------------------------------------------*/
/*	Func:	Camera::MoveLocal
/*	Desc:	Performs moving of camera position by amount in direction mdir
/*				(mdir is in local camera space)
/*---------------------------------------------------------------------------*/
void Camera::MoveLocal( const Vector3D& mdir, float amount )
{
	Vector3D nPos( mdir );
	//  transform direction from local camera space to the world space
	Matrix3D tr;
	tr.copy( getViewM() );
	tr.inverse();
	nPos *= tr;
	nPos *= amount;
	nPos += getPos();
	setPos( nPos );
} // Camera::MoveLocal

void Camera::Zoom( float scale )
{
	if (isOrtho())
	{
		setViewVolumeWidth( getViewVolumeWidth() / (1.0f + scale) );
		return;
	}

	MoveLocal( Vector3D::oZ, scale * getViewVolumeWidth() );
} // Camera::Zoom

/*****************************************************************
/*	Path implementation
/*****************************************************************/
void Path::AddPt( const Vector3D& vPos, const Vector3D& vAt, 
							float dt )
{
	if (numPt >= c_MaxPathPt)
	{
		Log.Warning( "Too much points for the path." );
		return;
	}
	pos[numPt]		= vPos;
	at[numPt]		= vAt;
	dtime[numPt]	= dt;
}

void Path::getPosDir( Vector3D& vPos, Vector3D& vAt, float dt )
{
	if (numPt < 4)
	{
		Log.Warning( "Too little points in path to build spline." );
		return;
	}
	
	Vector3D vec;
	int idx[4];
	//  find beg point
	dt += lastPtTime;
	while (dt >= dtime[curPt])
	{
		dt -= dtime[curPt];
		curPt++;
		if (curPt > numPt) curPt = 0;
	}

	lastPtTime = dt;
	idx[0] = curPt;
	for (int i = 1; i < 4; i++)
	{
		idx[i] = idx[i - 1] + 1;
		if (idx[i] == numPt) idx[i] = 0;
	}

	float s = lastPtTime / dtime[curPt];

	vPos.splineCatmullRom( s, pos[idx[0]], pos[idx[1]], pos[idx[2]], pos[idx[3]] );
	vAt.splineCatmullRom( s, at[idx[0]], at[idx[1]], at[idx[2]], at[idx[3]] );
}

void Path::InitFromFile( const char* fname )
{
	/*
	char fullName[_MAX_PATH];
	sprintf( fullName, "cameras\\%s.cam", fname );

	IniFile inif( fullName );
	if (inif.Error())
	{
		Log.Warning( "Couldn't init camera path: %s", fname );
		return;
	}

	numPt = 0;

	bool secPresent = inif.FirstSection();
	while (secPresent)
	{
		dtime[numPt] = inif.GetValFloat( "dtime" );

		pos[numPt].x = inif.GetValFloat( "posX" );
		pos[numPt].y = inif.GetValFloat( "posY" );
		pos[numPt].z = inif.GetValFloat( "posZ" );
	
		at[numPt].x = inif.GetValFloat( "atX" );
		at[numPt].y = inif.GetValFloat( "atY" );
		at[numPt].z = inif.GetValFloat( "atZ" );

		numPt++;
		secPresent = inif.NextSection();
	}

	curPt = 0;
	lastPtTime = 0.0f;
	*/
}

void Path::Reset()
{
	numPt = 0;
}



