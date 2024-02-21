/*****************************************************************************/
/*	File:	gpBucket.inl
/*	Desc:	
/*	Author:	Ruslan Shestopalyuk
/*	Date:	19.03.2003
/*****************************************************************************/

const float c_ZMin	= 0.4f;
const float c_ZMax	= 0.9f;

_inl DWORD ZToColor( float z )
{return 0xFFFFFFFF;
	float  fclr = (z - c_ZMin) * ((float)0xFF) / (c_ZMax - c_ZMin);
	if (fclr < 0.0f) fclr = 0.0f;
	if (fclr > (float)0xFF) fclr = (float)0xFF;
	DWORD clr = (DWORD)fclr;
	return 0xFF000000 | (clr << 16) | (clr << 8) | clr;
}

const float c_HalfPixel = 0.0f;
/*****************************************************************************/
/*	GPBucket implementation
/*****************************************************************************/
_inl void GPBucket::AddQuad( const GPChunk& chunk, float cX, float cY, float cZ, DWORD diffuse )
{
	if (getNVert() >= (c_MaxQuadsDrawn - 1)*4) Flush();

	VertexFormat vf = getVertexFormat();
	if (vf == vfTnL)
	{
		VertexTnL* vert = ((VertexTnL*)getVertexData()) + getNVert();
		
		vert[0].x = chunk.x + cX - c_HalfPixel;
		vert[0].y = chunk.y + cY - c_HalfPixel;
		vert[0].z = cZ;
		vert[0].u = chunk.u;
		vert[0].v = chunk.v;
		
		vert[1].x = vert[0].x + chunk.side;
		vert[1].y = vert[0].y;
		vert[1].z = cZ;
		vert[1].u = chunk.u2;
		vert[1].v = chunk.v;
		
		vert[2].x = vert[0].x;
		vert[2].y = vert[0].y + chunk.side;
		vert[2].z = cZ;
		vert[2].u = chunk.u;
		vert[2].v = chunk.v2;
		
		vert[3].x = vert[1].x ;
		vert[3].y = vert[2].y;
		vert[3].z = cZ;
		vert[3].u = chunk.u2;
		vert[3].v = chunk.v2;

		vert[0].w = 1.0f; 
		vert[1].w = 1.0f; 
		vert[2].w = 1.0f; 
		vert[3].w = 1.0f;
		
		vert[0].diffuse = diffuse;
		vert[1].diffuse = diffuse;
		vert[2].diffuse = diffuse;
		vert[3].diffuse = diffuse;
	}
	else if (vf == vf2Tex)
	{
		Vertex2t* vert = ((Vertex2t*)getVertexData()) + getNVert();
		
		vert[0].x = chunk.x + cX;
		vert[0].y = chunk.y*2.0f + cY;
		vert[0].z = cZ;
		vert[0].u = chunk.u;
		vert[0].v = chunk.v;
		
		vert[1].x = vert[0].x + chunk.side;
		vert[1].y = vert[0].y;
		vert[1].z = cZ;
		vert[1].u = chunk.u2;
		vert[1].v = chunk.v;
		
		vert[2].x = vert[0].x;
		vert[2].y = vert[0].y + chunk.side*2.0f;
		vert[2].z = cZ;
		vert[2].u = chunk.u;
		vert[2].v = chunk.v2;
		
		vert[3].x = vert[0].x + chunk.side;
		vert[3].y = vert[2].y;
		vert[3].z = cZ;
		vert[3].u = chunk.u2;
		vert[3].v = chunk.v2;

		vert[0].diffuse = diffuse;
		vert[1].diffuse = diffuse;
		vert[2].diffuse = diffuse;
		vert[3].diffuse = diffuse;
	}
	else
	{
		assert( false );
	}
	setNVert( getNVert() + 4 );
} // GPBucket::AddQuad

_inl void GPBucket::AddQuadMirroredAligned( const GPChunk& chunk, 
											const Plane& plane, 
											float cX, float cY, float cZ, 
											float scale, DWORD diffuse )
{
		if (getNVert() >= (c_MaxQuadsDrawn - 1)*4) Flush();

	VertexFormat vf = getVertexFormat();
	if (vf == vfTnL)
	{
		VertexTnL* vert = ((VertexTnL*)getVertexData()) + getNVert();
		
		float scS = scale * chunk.side;
		float scX = scale * chunk.x;
		float scY = scale * chunk.y;
		
		float scaledSide = scale * chunk.side;
		
		vert[0].x = cX - scale * chunk.x - c_HalfPixel;
		vert[0].y = scale * chunk.y + cY - c_HalfPixel;
		vert[0].z = cZ + plane.getZ( scX, scY );
		vert[0].u = chunk.u;
		vert[0].v = chunk.v;
		
		vert[1].x = vert[0].x - scS;
		vert[1].y = vert[0].y;
		vert[1].z = cZ + plane.getZ( scX + scS, scY );
		vert[1].u = chunk.u2;
		vert[1].v = chunk.v;
		
		vert[2].x = vert[0].x;
		vert[2].y = vert[0].y + scS;
		vert[2].z = cZ + plane.getZ( scX, scY + scS );
		vert[2].u = chunk.u;
		vert[2].v = chunk.v2;
		
		vert[3].x = vert[1].x;
		vert[3].y = vert[2].y;
		vert[3].z = vert[3].z = cZ + plane.getZ( scX + scS, scY + scS );
		vert[3].u = chunk.u2;
		vert[3].v = chunk.v2;

		vert[0].w = 1.0f; 
		vert[1].w = 1.0f; 
		vert[2].w = 1.0f; 
		vert[3].w = 1.0f;
		
		vert[0].diffuse = diffuse;
		vert[1].diffuse = diffuse;
		vert[2].diffuse = diffuse;
		vert[3].diffuse = diffuse;
	}
	else if (vf == vf2Tex)
	{
		Vertex2t* vert = ((Vertex2t*)getVertexData()) + getNVert();

		float scS = scale * chunk.side;
		float scX = scale * chunk.x;
		float scY = scale * chunk.y;
		
		vert[0].x = cX - scale * chunk.x;
		vert[0].y = scale * chunk.y + cY;
		vert[0].z = cZ + plane.getZ( scX, scY );
		vert[0].u = chunk.u;
		vert[0].v = chunk.v;
		
		vert[1].x = vert[0].x - scS;
		vert[1].y = vert[0].y;
		vert[1].z = cZ + plane.getZ( scX + scS, scY );
		vert[1].u = chunk.u2;
		vert[1].v = chunk.v;
		
		vert[2].x = vert[0].x;
		vert[2].y = vert[0].y + scS;
		vert[2].z = cZ + plane.getZ( scX, scY + scS );
		vert[2].u = chunk.u;
		vert[2].v = chunk.v2;
		
		vert[3].x = vert[1].x;
		vert[3].y = vert[2].y;
		vert[3].z = cZ + plane.getZ( scX + scS, scY + scS );
		vert[3].u = chunk.u2;
		vert[3].v = chunk.v2;

		vert[0].diffuse = diffuse;
		vert[1].diffuse = diffuse;
		vert[2].diffuse = diffuse;
		vert[3].diffuse = diffuse;
	}
	else 
	{
		assert( false );
	}
	setNVert( getNVert() + 4 );
} // GPBucket::AddQuadMirroredAligned

_inl void GPBucket::AddQuadAligned( const GPChunk& chunk, 
									const Plane& plane, 
									float cX, float cY, float cZ, 
									float scale, DWORD diffuse  )
{
	if (getNVert() >= (c_MaxQuadsDrawn - 1)*4) Flush();

	VertexFormat vf = getVertexFormat();
	if (vf == vfTnL)
	{
		VertexTnL* vert = ((VertexTnL*)getVertexData()) + getNVert();
		
		float scS = scale * chunk.side;
		float scX = scale * chunk.x;
		float scY = scale * chunk.y;
		
		vert[0].x = scX + cX - c_HalfPixel;
		vert[0].y = scY + cY - c_HalfPixel;
		vert[0].z = cZ + plane.getZ( scX, scY );
		vert[0].u = chunk.u;
		vert[0].v = chunk.v;
		
		vert[1].x = vert[0].x + scS;
		vert[1].y = vert[0].y;
		vert[1].z = cZ + plane.getZ( scX + scS, scY );
		vert[1].u = chunk.u2;
		vert[1].v = chunk.v;
		
		vert[2].x = vert[0].x;
		vert[2].y = vert[0].y + scS;
		vert[2].z = cZ + plane.getZ( scX, scY + scS );
		vert[2].u = chunk.u;
		vert[2].v = chunk.v2;
		
		vert[3].x = vert[1].x;
		vert[3].y = vert[2].y;
		vert[3].z = cZ + plane.getZ( scX + scS, scY + scS );
		vert[3].u = chunk.u2;
		vert[3].v = chunk.v2;

		vert[0].w = 1.0f; 
		vert[1].w = 1.0f; 
		vert[2].w = 1.0f; 
		vert[3].w = 1.0f;
		
		vert[0].diffuse = diffuse;
		vert[1].diffuse = diffuse;
		vert[2].diffuse = diffuse;
		vert[3].diffuse = diffuse;
	}
	else if (vf == vf2Tex)
	{
		Vertex2t* vert = ((Vertex2t*)getVertexData()) + getNVert();

		float scS = scale * chunk.side;
		float scX = scale * chunk.x;
		float scY = scale * chunk.y;
		
		vert[0].x = scX + cX;
		vert[0].y = scY + cY;
		vert[0].z = cZ + plane.getZ( scX, scY );
		vert[0].u = chunk.u;
		vert[0].v = chunk.v;
		
		vert[1].x = vert[0].x + scS;
		vert[1].y = vert[0].y;
		vert[1].z = cZ + plane.getZ( scX + scS, scY );
		vert[1].u = chunk.u2;
		vert[1].v = chunk.v;
		
		vert[2].x = vert[0].x;
		vert[2].y = vert[0].y + scS;
		vert[2].z = cZ + plane.getZ( scX, scY + scS );
		vert[2].u = chunk.u;
		vert[2].v = chunk.v2;
		
		vert[3].x = vert[1].x;
		vert[3].y = vert[2].y;
		vert[3].z = cZ + plane.getZ( scX + scS, scY + scS );
		vert[3].u = chunk.u2;
		vert[3].v = chunk.v2;

		vert[0].diffuse = diffuse;
		vert[1].diffuse = diffuse;
		vert[2].diffuse = diffuse;
		vert[3].diffuse = diffuse;
	}
	else 
	{
		assert( false );
	}
	setNVert( getNVert() + 4 );
} // GPBucket::AddQuadAligned

const float c_UVBorder		= 1.0f / 200.0f;
_inl void GPBucket::AddQuadVS( const GPChunk& chunk, const SoftwareVS& vs, DWORD diffuse  )
{
	if (getNVert() >= (c_MaxQuadsDrawn - 1)*4) Flush();

	VertexFormat vf = getVertexFormat();
	assert( vf == vf2Tex );
	Vertex2t* vert = ((Vertex2t*)getVertexData()) + getNVert();

	vert[0].x = chunk.x;
	vert[0].y = chunk.y;
	vert[0].z = 0.0f;
	vert[0].u = chunk.u + c_UVBorder;
	vert[0].v = chunk.v + c_UVBorder;
	
	vert[1].x = vert[0].x + chunk.side;
	vert[1].y = vert[0].y;
	vert[1].z = 0.0f;
	vert[1].u = chunk.u2 - c_UVBorder;
	vert[1].v = vert[0].v;
	
	vert[2].x = vert[0].x;
	vert[2].y = vert[0].y + chunk.side;
	vert[2].z = 0.0f;
	vert[2].u = vert[0].u;
	vert[2].v = chunk.v2 - c_UVBorder;
	
	vert[3].x = vert[1].x;
	vert[3].y = vert[2].y;
	vert[3].z = 0.0f;
	vert[3].u = vert[1].u;
	vert[3].v = vert[2].v;
	
	vert[0].diffuse = diffuse;
	vert[1].diffuse = diffuse;
	vert[2].diffuse = diffuse;
	vert[3].diffuse = diffuse;

	vs.ProcessVertices( vert, 4 );
	setNVert( getNVert() + 4 );
} // GPBucket::AddQuadVS

_inl void GPBucket::AddQuad( const GPChunk& chunk, const Matrix4D& transf, DWORD diffuse )
{
	if (getNVert() >= (c_MaxQuadsDrawn - 1)*4) Flush();

	VertexFormat vf = getVertexFormat();
	if (vf == vfTnL)
	{
		VertexTnL* vert = ((VertexTnL*)getVertexData()) + getNVert();

		vert[0].x = chunk.x - c_HalfPixel;
		vert[0].y = chunk.y - c_HalfPixel;
		vert[0].z = 0.0f;
		vert[0].u = chunk.u;
		vert[0].v = chunk.v;
		
		vert[1].x = vert[0].x + chunk.side;
		vert[1].y = vert[0].y;
		vert[1].z = 0.0f;
		vert[1].u = chunk.u2;
		vert[1].v = chunk.v;
		
		vert[2].x = vert[0].x;
		vert[2].y = vert[0].y + chunk.side;
		vert[2].z = 0.0f;
		vert[2].u = chunk.u;
		vert[2].v = chunk.v2;
		
		vert[3].x = vert[0].x + chunk.side;
		vert[3].y = vert[0].y + chunk.side;
		vert[3].z = 0.0f;
		vert[3].u = chunk.u2;
		vert[3].v = chunk.v2;
		
		(*(Vector4D*)(vert + 0)).mul( transf );
		(*(Vector4D*)(vert + 1)).mul( transf );
		(*(Vector4D*)(vert + 2)).mul( transf );
		(*(Vector4D*)(vert + 3)).mul( transf );

		vert[0].w = 1.0f; 
		vert[1].w = 1.0f; 
		vert[2].w = 1.0f; 
		vert[3].w = 1.0f;
		
		vert[0].diffuse = diffuse;
		vert[1].diffuse = diffuse;
		vert[2].diffuse = diffuse;
		vert[3].diffuse = diffuse;
	}
	else if (vf == vf2Tex)
	{
		Vertex2t* vert = ((Vertex2t*)getVertexData()) + getNVert();
		
		vert[0].x = chunk.x;
		vert[0].y = chunk.y;
		vert[0].z = 0.0f;
		vert[0].u = chunk.u;
		vert[0].v = chunk.v;
		
		vert[1].x = vert[0].x + chunk.side;
		vert[1].y = vert[0].y;
		vert[1].z = 0.0f;
		vert[1].u = chunk.u2;
		vert[1].v = chunk.v;
		
		vert[2].x = vert[0].x;
		vert[2].y = vert[0].y + chunk.side;
		vert[2].z = 0.0f;
		vert[2].u = chunk.u;
		vert[2].v = chunk.v2;
		
		vert[3].x = vert[0].x + chunk.side;
		vert[3].y = vert[0].y + chunk.side;
		vert[3].z = 0.0f;
		vert[3].u = chunk.u2;
		vert[3].v = chunk.v2;
		
		(*(Vector3D*)(vert + 0)) *= transf;
		(*(Vector3D*)(vert + 1)) *= transf;
		(*(Vector3D*)(vert + 2)) *= transf;
		(*(Vector3D*)(vert + 3)) *= transf;

		vert[0].diffuse = diffuse;
		vert[1].diffuse = diffuse;
		vert[2].diffuse = diffuse;
		vert[3].diffuse = diffuse;
	}	
	else 
	{
		assert( false );
	}
	
	setNVert( getNVert() + 4 );
} // GPBucket::AddQuad

_inl void GPBucket::AddQuadMirrored( const GPChunk& chunk, 
									float cX, float cY, float cZ, DWORD diffuse  )
{
	if (getNVert() >= (c_MaxQuadsDrawn - 1)*4) Flush();

	VertexFormat vf = getVertexFormat();
	if (vf == vfTnL)
	{
		VertexTnL* vert = ((VertexTnL*)getVertexData()) + getNVert();
		
		vert[0].x = cX - chunk.x - c_HalfPixel;
		vert[0].y = chunk.y + cY - c_HalfPixel;
		vert[0].z = cZ;
		vert[0].u = chunk.u;
		vert[0].v = chunk.v;
		
		vert[1].x = vert[0].x - chunk.side;
		vert[1].y = vert[0].y;
		vert[1].z = cZ;
		vert[1].u = chunk.u2;
		vert[1].v = chunk.v;
		
		vert[2].x = vert[0].x;
		vert[2].y = vert[0].y + chunk.side;
		vert[2].z = cZ;
		vert[2].u = chunk.u;
		vert[2].v = chunk.v2;
		
		vert[3].x = vert[1].x;
		vert[3].y = vert[2].y;
		vert[3].z = cZ;
		vert[3].u = chunk.u2;
		vert[3].v = chunk.v2;

		vert[0].w = 1.0f; 
		vert[1].w = 1.0f; 
		vert[2].w = 1.0f; 
		vert[3].w = 1.0f;
		
		vert[0].diffuse = diffuse;
		vert[1].diffuse = diffuse;
		vert[2].diffuse = diffuse;
		vert[3].diffuse = diffuse;
	}
	else if (vf == vf2Tex)
	{
		Vertex2t* vert = ((Vertex2t*)getVertexData()) + getNVert();

		vert[0].x = cX - chunk.x;
		vert[0].y = chunk.y + cY*2.0f;
		vert[0].z = cZ;
		vert[0].u = chunk.u;
		vert[0].v = chunk.v;
		
		vert[1].x = vert[0].x - chunk.side;
		vert[1].y = vert[0].y;
		vert[1].z = cZ;
		vert[1].u = chunk.u2;
		vert[1].v = chunk.v;
		
		vert[2].x = vert[0].x;
		vert[2].y = vert[0].y + chunk.side*2.0f;
		vert[2].z = cZ;
		vert[2].u = chunk.u;
		vert[2].v = chunk.v2;
		
		vert[3].x = vert[1].x;
		vert[3].y = vert[2].y;
		vert[3].z = cZ;
		vert[3].u = chunk.u2;
		vert[3].v = chunk.v2;

		vert[0].diffuse = diffuse;
		vert[1].diffuse = diffuse;
		vert[2].diffuse = diffuse;
		vert[3].diffuse = diffuse;
	}
	else 
	{
		assert( false );
	}

	setNVert( getNVert() + 4 );
} // GPBucket::AddQuadMirrored

_inl void GPBucket::AddQuadMirroredScaled(	const GPChunk& chunk, 
											float cX, float cY, float cZ, 
											float scale, DWORD diffuse  )
{
	if (getNVert() >= (c_MaxQuadsDrawn - 1)*4) Flush();

	VertexFormat vf = getVertexFormat();
	if (vf == vfTnL)
	{
		VertexTnL* vert = ((VertexTnL*)getVertexData()) + getNVert();

		float scaledSide = scale * chunk.side;
		
		vert[0].x = cX - scale * chunk.x - c_HalfPixel;
		vert[0].y = scale * chunk.y + cY - c_HalfPixel;
		vert[0].z = cZ;
		vert[0].u = chunk.u;
		vert[0].v = chunk.v;
		
		vert[1].x = vert[0].x - scaledSide;
		vert[1].y = vert[0].y;
		vert[1].z = cZ;
		vert[1].u = chunk.u2;
		vert[1].v = chunk.v;
		
		vert[2].x = vert[0].x;
		vert[2].y = vert[0].y + scaledSide;
		vert[2].z = cZ;
		vert[2].u = chunk.u;
		vert[2].v = chunk.v2;
		
		vert[3].x = vert[1].x;
		vert[3].y = vert[2].y;
		vert[3].z = cZ;
		vert[3].u = chunk.u2;
		vert[3].v = chunk.v2;

		vert[0].w = 1.0f; 
		vert[1].w = 1.0f; 
		vert[2].w = 1.0f; 
		vert[3].w = 1.0f;
		
		vert[0].diffuse = diffuse;
		vert[1].diffuse = diffuse;
		vert[2].diffuse = diffuse;
		vert[3].diffuse = diffuse;
	}
	else if (vf == vf2Tex)
	{
		Vertex2t* vert = ((Vertex2t*)getVertexData()) + getNVert();
		float scaledSide = scale * chunk.side;
		
		vert[0].x = cX - scale * chunk.x;
		vert[0].y = scale * chunk.y*2.0f + cY;
		vert[0].z = cZ;
		vert[0].u = chunk.u;
		vert[0].v = chunk.v;
		
		vert[1].x = vert[0].x - scaledSide;
		vert[1].y = vert[0].y;
		vert[1].z = cZ;
		vert[1].u = chunk.u2;
		vert[1].v = chunk.v;
		
		vert[2].x = vert[0].x;
		vert[2].y = vert[0].y + scaledSide*2.0f;
		vert[2].z = cZ;
		vert[2].u = chunk.u;
		vert[2].v = chunk.v2;
		
		vert[3].x = vert[1].x;
		vert[3].y = vert[2].y;
		vert[3].z = cZ;
		vert[3].u = chunk.u2;
		vert[3].v = chunk.v2;

		vert[0].x = cX - chunk.x;
		vert[0].y = chunk.y + cY;
		vert[0].z = cZ;
		vert[0].u = chunk.u;
		vert[0].v = chunk.v;
		
		vert[1].x = vert[0].x - chunk.side;
		vert[1].y = vert[0].y;
		vert[1].z = cZ;
		vert[1].u = chunk.u2;
		vert[1].v = chunk.v;
		
		vert[2].x = vert[0].x;
		vert[2].y = vert[0].y + chunk.side;
		vert[2].z = cZ;
		vert[2].u = chunk.u;
		vert[2].v = chunk.v2;
		
		vert[3].x = vert[1].x;
		vert[3].y = vert[2].y;
		vert[3].z = cZ;
		vert[3].u = chunk.u2;
		vert[3].v = chunk.v2;
	}
	else 
	{
		assert( false );
	}

	setNVert( getNVert() + 4 );
} // GPBucket::AddQuadScaledMirrored

_inl void GPBucket::AddQuadScaled(	const GPChunk& chunk, 
									float cX, float cY, float cZ,
									float scale, DWORD diffuse  )
{
	if (getNVert() >= (c_MaxQuadsDrawn - 1)*4) Flush();
	VertexFormat vf = getVertexFormat();
	if (vf == vfTnL)
	{	
		VertexTnL* vert = ((VertexTnL*)getVertexData()) + getNVert();

		float scaledSide = scale * chunk.side;
		
		vert[0].x = scale * chunk.x + cX - c_HalfPixel;
		vert[0].y = scale * chunk.y + cY - c_HalfPixel;
		vert[0].z = cZ;
		vert[0].u = chunk.u;
		vert[0].v = chunk.v;
		
		vert[1].x = vert[0].x + scaledSide;
		vert[1].y = vert[0].y;
		vert[1].z = cZ;
		vert[1].u = chunk.u2;
		vert[1].v = chunk.v;
		
		vert[2].x = vert[0].x;
		vert[2].y = vert[0].y + scaledSide;
		vert[2].z = cZ;
		vert[2].u = chunk.u;
		vert[2].v = chunk.v2;
		
		vert[3].x = vert[1].x;
		vert[3].y = vert[2].y;
		vert[3].z = cZ;
		vert[3].u = chunk.u2;
		vert[3].v = chunk.v2;

		vert[0].w = 1.0f; 
		vert[1].w = 1.0f; 
		vert[2].w = 1.0f; 
		vert[3].w = 1.0f;
		
		vert[0].diffuse = diffuse;
		vert[1].diffuse = diffuse;
		vert[2].diffuse = diffuse;
		vert[3].diffuse = diffuse;
	}
	else if (vf == vf2Tex)
	{
		Vertex2t* vert = ((Vertex2t*)getVertexData()) + getNVert();

		float scaledSide = scale * chunk.side;
		
		vert[0].x = scale * chunk.x + cX;
		vert[0].y = scale * chunk.y*2.0f + cY;
		vert[0].z = cZ;
		vert[0].u = chunk.u;
		vert[0].v = chunk.v;
		
		vert[1].x = vert[0].x + scaledSide;
		vert[1].y = vert[0].y;
		vert[1].z = cZ;
		vert[1].u = chunk.u2;
		vert[1].v = chunk.v;
		
		vert[2].x = vert[0].x;
		vert[2].y = vert[0].y + scaledSide*2.0f;
		vert[2].z = cZ;
		vert[2].u = chunk.u;
		vert[2].v = chunk.v2;
		
		vert[3].x = vert[1].x;
		vert[3].y = vert[2].y;
		vert[3].z = cZ;
		vert[3].u = chunk.u2;
		vert[3].v = chunk.v2;

		vert[0].diffuse = diffuse;
		vert[1].diffuse = diffuse;
		vert[2].diffuse = diffuse;
		vert[3].diffuse = diffuse;
	}
	else 
	{
		assert( false );
	}

	setNVert( getNVert() + 4 );
} // GPBucket::AddQuadScaled

_inl void GPBucket::Flush()
{
	if (getNVert() == 0) return;
	setNPri( getNVert() / 2 );
	setShader( s_CurShader );
	RC::iRS->Draw( *this );
	setNVert( 0 );
} // GPBucket::Flush

_inl int GPBucket::GetCurrentVertexIdx() const
{
	return getNVert(); 
} // GPBucket::GetCurrentVertexIdx

_inl void GPBucket::Reset()
{
	setNVert( 0 );
}

_inl void GPBucket::Init()
{
	static int shSprite2D = RC::iRS->GetShaderID( "hud" );
		
	curVF = vfTnL;
	create		( c_MaxQuadsDrawn * 4, 0, vfTnL, ptTriangleList );
	setMaxInd	( c_MaxQuadsDrawn * 6		);
	setPriType	( ptTriangleList			);
	setShader	( shSprite2D				);
	
	setVertexFormat( vfTnL					);
	setIsQuadList( true );
}; // GPBucket::Init

_inl void GPBucket::SetCurrentVertexFormat( VertexFormat vf )
{
	if (vf != getVertexFormat())
	{
		Flush();
		setVertexFormat( vf );
		setDevHandle( 0 );
	}
} // GPBucket::SetCurrentVertexFormat
