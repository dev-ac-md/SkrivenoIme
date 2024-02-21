/*****************************************************************
/*  File:   rsVertex.cpp                                          
/*  Desc:                                                        
/*  Author: Silver, Copyright (C) GSC Game World                 
/*  Date:   January 2002                                         
/*****************************************************************/
#include "stdafx.h"
#include "rsVertex.h"

int
Vertex::CalcStride( VertexFormat vf )

{
	return c_vfStride[vf];
}

void* Vertex::CreateVBuf( VertexFormat vf, int numVert )
{
	return new BYTE[numVert * CalcStride( vf )];
}

void Vertex::Dump( void* buf, int numVert, VertexFormat vf )
{
	switch (vf)
	{
	case vf2Tex: 
		{
			Vertex2t* vert = (Vertex2t*)buf;
			for (int i = 0; i < numVert; i++)
			{
				vert[i].Dump();
			}
		}break;
	case vfTnL:
		{
			VertexTnL* vert = (VertexTnL*)buf;
			for (int i = 0; i < numVert; i++)
			{
				vert[i].Dump();
			}
		}break;
	case vfTnL2:
		{
			VertexTnL2* vert = (VertexTnL2*)buf;
			for (int i = 0; i < numVert; i++)
			{
				vert[i].Dump();
			}
		}break;
	case vfTnL2S:
		{
			VertexTnL2S* vert = (VertexTnL2S*)buf;
			for (int i = 0; i < numVert; i++)
			{
				vert[i].Dump();
			}
		}break;
	case vfN:
		{
			VertexN* vert = (VertexN*)buf;
			for (int i = 0; i < numVert; i++)
			{
				vert[i].Dump();
			}
		}break;
	}
}

Vertex2t::Vertex2t()
{
	x		= 0.0f;
	y		= 0.0f;
	z		= 0.0f;
	diffuse	= 0xFF584E40;	

	u = u2 = v = v2 = 0.0f;
}

/*****************************************************************
/*  Vertex2t implementation                                      *
/*****************************************************************/
void Vertex2t::Dump( bool full )
{
	if (full)
	{
		Log.Message( "x:%f, y:%f, z:%f,  dcolor:%d, u1:%f,"
					" v1:%f, u2:%f, v2:%f",
					x, y, z, diffuse, u, v, u2, v2 );
	}
	else
	{
		Log.Message( "x:%f, y:%f, z:%f,  diffuse:%X",
					x, y, z, diffuse );
	}
}

const Vertex2t& Vertex2t::operator =( const Vector3D& vec )
{
	x = vec.x; y = vec.y; z = vec.z;
	return *this;
}

/*****************************************************************
/*  VertexTnL implementation                                     *
/*****************************************************************/
VertexTnL::VertexTnL()
{
	x			= 0.0f;
	y			= 0.0f;
	z			= 0.01f;
	w			= 1.0f;

	diffuse		= 0xFFFFFFFF;
	specular	= 0xFFFFFFFF;

	u = v = 0.0f;
}

void VertexTnL::Dump( bool full )
{
	if (full)
	{
		Log.Message( "x:%f, y:%f, z:%f,  diffuse:%X, u:%f,"
					" v:%f",
					x, y, z, diffuse, u, v );
	}
	else
	{
		Log.Message( "x:%f, y:%f, z:%f,  diffuse:%X",
					x, y, z, diffuse );
	}
}

VertexTnL& VertexTnL::operator =( const Vector3D& vec )
{
	x = vec.x;
	y = vec.y;
	z = vec.z;
	return *this;
}

void VertexTnL2::Dump( bool full )
{
	if (full)
	{
		Log.Message( "x:%f, y:%f, z:%f,  diffuse:%X, u:%f,"
					" v:%f u2:%f v2:%f",
					x, y, z, diffuse, u, v, u2, v2 );
	}
	else
	{
		Log.Message( "x:%f, y:%f, z:%f,  diffuse:%X",
					x, y, z, diffuse );
	}
}
void VertexTnL2S::Dump( bool full )
{
	if (full)
	{
		Log.Message( "x:%f, y:%f, z:%f,  diffuse:%X, u:%f,"
					" v:%f u2:%f v2:%f",
					x, y, z, diffuse, u, v, u2, v2 );
	}
	else
	{
		Log.Message( "x:%f, y:%f, z:%f,  diffuse:%X",
					x, y, z, diffuse );
	}
}
/*****************************************************************
/*  VertexN implementation                                       *
/*****************************************************************/
VertexN::VertexN()
{
	x			= 0.0f;
	y			= 0.0f;
	z			= 0.0f;
	
	nx			= 0.0f;
	ny			= 0.0f;
	nz			= 0.0f;

	u = v = 0.0f;
}

VertexN& VertexN::operator =( const Vector3D& vec )
{
	x = vec.x;
	y = vec.y;
	z = vec.z;
	return *this;
} 


void VertexN::Dump( bool full )
{
	if (full)
	{
		Log.Message( "x:%f, y:%f, z:%f, nx:%f, ny:%f, nz:%f, u:%f,"
					" v:%f",
					x, y, z, nx, ny, nz, u, v );
	}
	else
	{
		Log.Message( "x:%f, y:%f, z:%f",
					x, y, z );
	}
}


