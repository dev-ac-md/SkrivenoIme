/*****************************************************************************/
/*	File:	AnimModel.cpp
/*	Desc:	
/*	Author:	Ruslan Shestopalyuk
/*	Date:	20.11.2002
/*****************************************************************************/
#include "stdafx.h"
#include "IRenderSystem.h"
#include "rsVertex.h"
#include <stdio.h>
#include "mMath3D.h"
#include "mCamera.h"
#include "AnimModel.h"  
#include "gpAuxPrim.h"  

inline void ReadInt( FILE* fp, int& val )                             
{ 
	fread( &val, sizeof( int ), 1, fp );    
} // ReadInt

inline void ReadFloat( FILE* fp, float& val )   
{
	fread( &val, sizeof( float ), 1, fp );
} // ReadFloat

inline void ReadWord( FILE* fp, WORD& val ) 
{
	fread( &val, sizeof( WORD ), 1, fp );
} // ReadWord

inline void ReadDword( FILE* fp, DWORD& val ) 
{
	fread( &val, sizeof( DWORD ), 1, fp ); 
} // ReadDword

inline bool ReadString( FILE* fp, char* str, int nCh ) 
{
	WORD len, rlen;
	ReadWord( fp, len );
	rlen = nCh > len ? len : nCh - 1;
	fread( str, rlen, 1, fp );
	str[rlen] = 0;
	if (nCh <= len) 
	{
		fseek( fp, len - nCh, SEEK_CUR );
		return false;
	}
	return true;
} // ReadString

/*****************************************************************************/
/*	ComplexMesh implementation
/*****************************************************************************/
ComplexMesh::~ComplexMesh()
{
	delete []bm;
}
	
void ComplexMesh::setNMeshes( int nM )
{
	delete []bm;
	bm = new BaseMesh[nM];
	nSub = nM;
}

bool ComplexMesh::LoadMesh( FILE* fp, int nBytes )
{
	int nM = 0;
	ReadInt( fp, nM ); 
	ReadFloat( fp, radius );
	setNMeshes( nM );
	for (int i = 0; i < nM; i++)
	{
		BlockReader suBR( fp );
		
		WORD			texID0, texID1;
		int				nInd;
		int				nVert;
		VertexConfig	cfg;
		
		ReadWord( fp, texID0 );
		ReadWord( fp, texID1 );
		cfg.Read( fp );

		bm[i].setTexture( texID0, 0 );
		bm[i].setTexture( texID1, 1 );
		
		ReadInt	( fp, nInd   );
		ReadInt	( fp, nVert	 );

		VertexFormat vf = cfg.VF();
		assert( vf != vfUnknown );
		bm[i].create( nVert, nInd, vf, ptTriangleList );
		
		fread( bm[i].getVertexData(), 1, nVert * cfg.getSizeBytes(), fp );
		fread( bm[i].getIndices(), sizeof( WORD ) * nInd, 1, fp );
		
		bm[i].setNVert( nVert ); 
		bm[i].setNInd( nInd ); 
		
		suBR.end();
	}
	return true;
} // ComplexMesh::LoadMesh

/*****************************************************************************/
/*	VertexConfig implementation
/*****************************************************************************/
int	VertexConfig::getSizeBytes() const
{
	return Vertex::CalcStride( VF() );
} // VertexConfig::getSizeBytes

VertexFormat VertexConfig::VF() const
{
	if (hasXYZ() && hasUV() && hasNormal())
	{
		if (numBoneW() == 0) return vfN;
		if (numBoneW() == 1) return vfNMP1;
		if (numBoneW() == 2) return vfNMP2;
		if (numBoneW() == 3) return vfNMP3;
		if (numBoneW() == 4) return vfNMP4;
	}
	
	if (hasXYZ() && hasUV2() && hasDiffuse()) 
	{
		return vf2Tex;
	}
	
	return vfUnknown;
} // VertexConfig::VF

void VertexConfig::ConvertToVF( void* dest, void* src, int nVert )
{
	assert( false );
} // VertexConfig::ConvertToVF

/*****************************************************************************/
/*	BlockReader implementation
/*****************************************************************************/
BlockType BlockReader::blockType() const 
{
	char* ctype = (char*) &type;
	if (!strncmp( ctype, "SUBM", 4 )) return btSubmesh;
	if (!strncmp( ctype, "MESH", 4 )) return btMesh;
	if (!strncmp( ctype, "SMTL", 4 )) return btMaterial;
	if (!strncmp( ctype, "IBUF", 4 )) return btIBuf;
	if (!strncmp( ctype, "VBUF", 4 )) return btVBuf;
	if (!strncmp( ctype, "TTBL", 4 )) return btTexTable;
	if (!strncmp( ctype, "SKEL", 4 )) return btSkeleton;
	if (!strncmp( ctype, "ANIM", 4 )) return btAnimation;
	if (!strncmp( ctype, "ANCH", 4 )) return btAnimChannel;
	if (!strncmp( ctype, "C2MD", 4 )) return btC2Model;
	return btUnknown;
} // BlockReader::blockType

/*****************************************************************************/
/*	AnimModel implementation
/*****************************************************************************/
char AnimModel::strBuf[c_StrBufSize];

bool AnimModel::Load( IRenderSystem* irs, const char* fname )
{
	FILE* fp = fopen( fname, "rb" );
	if (!fp) return false;
	
	BlockReader c2mBR( fp );
	if (c2mBR.blockType() != btC2Model) 
	{
		Log.Message( "Not a .c2m file: %s", fname );
		return false;
	}

	int cPos = 0;
	while (!feof( fp ))
	{
		BlockReader br( fp );
		switch (br.blockType())
		{
		case btMesh:			LoadMesh		( fp, br.blockSize() ); break;
		case btAnimation:		LoadAnimation	( fp, br.blockSize() ); break;
		case btSkeleton:		LoadSkeleton	( fp, br.blockSize() ); break;
		case btTexTable:		LoadTexTable	( irs, fp, br.blockSize() ); break;
		default:
			Log.Error( "Unknown block type in file %s.", fname );
			assert( false );
			break;
		}
		br.end();
		cPos += br.fullSize();
		if (cPos > c2mBR.blockSize()) break;
	}
	fclose( fp );

	//  set initial transforms for all bones
	int nB = skeleton.getNBones();
	Matrix4D matr;
	for (int i = 0; i < nB; i++)
	{
		anmBone* bone = skeleton.getBone( i );
		if (bone)
		{
			anim.getChannel( i ).getRestPoseMatrix( matr );
			bone->setTransform( matr );
		}
	}

	int nS = skeleton.getNSub();
	for (i = 0; i < nS; i++)
	{
		anmSubMesh* sub = skeleton.getSub( i );
		if (sub->isAnimated())
		{
			sub->mtlAnimation.getRestPoseMatrix( matr );
			sub->setTransform( matr );
		}
		else
		{
			sub->setTransform( Matrix4D::identity );
		} 
	}

	return false;
} // AnimModel::Load


bool AnimModel::LoadAnimation( FILE* fp, int nBytes )
{
	int nCh = 0;
	ReadInt( fp, nCh );
	anim.setNChannels( nCh );
	for (int i = 0; i < nCh; i++)
	{
		BlockReader chBR( fp );
		int animTime, nFrames, keyRate, boneIdx;
		Matr3x4 tr;
		ReadInt( fp, animTime );
		ReadInt( fp, nFrames  );
		ReadInt( fp, keyRate  );
		ReadInt( fp, boneIdx  );
		anmChannel& cha = anim.getChannel( i );
		cha.setNFrames( nFrames );
		cha.keyRate = keyRate;
		cha.boneIdx = boneIdx;
		cha.animTime = animTime;
		for (int j = 0; j < nFrames; j++)
		{
			fread( tr, sizeof( Matr3x4 ), 1, fp );
			Matrix4D& m = cha.transf[j];
			m.e00 = tr[0]; m.e01 = tr[1]; m.e02 = tr[2]; m.e03 = 0.0f;
			m.e10 = tr[3]; m.e11 = tr[4]; m.e12 = tr[5]; m.e13 = 0.0f;
			m.e20 = tr[6]; m.e21 = tr[7]; m.e22 = tr[8]; m.e23 = 0.0f;
			m.e30 = tr[9]; m.e31 = tr[10]; m.e32 = tr[11]; m.e33 = 1.0f;
		}	
		chBR.end();
	}
	return false;
} // AnimModel::LoadAnimation

void AnimModel::dump()
{
	Log.Message( "nMeshes: %d", nSub );
	for (int i = 0; i < nSub; i++)
	{
		bm[i].dump();
	}
} // AnimModel::dump

bool AnimModel::LoadSkeleton( FILE* fp, int nBytes )
{
	int nB;
	ReadInt( fp, nB );
	WORD skelFlags;
	ReadWord( fp, skelFlags );
	skeleton.setHierarchicalTransf( skelFlags == 1 ); 
	skeleton.setNBones( nB );
	for (int i = 0; i < nB; i++)
	{
		anmBone* bone = skeleton.getBone( i );
		ReadString	( fp, bone->name, c_MaxNameLen );
		ReadInt		( fp, bone->papaIdx );

		//  write bounding box
		ReadFloat( fp, bone->aabb.minv.x );
		ReadFloat( fp, bone->aabb.minv.y );
		ReadFloat( fp, bone->aabb.minv.z );
		ReadFloat( fp, bone->aabb.maxv.x );
		ReadFloat( fp, bone->aabb.maxv.y );
		ReadFloat( fp, bone->aabb.maxv.z );

		WORD nChunks;
		ReadWord	( fp, nChunks );
		bone->setNChunks( nChunks );
		for (int i = 0; i < bone->nChunks; i++)
		{
			ReadWord( fp, bone->chunk[i].subIdx		);
			ReadInt ( fp, bone->chunk[i].firstPoly	);
			ReadInt ( fp, bone->chunk[i].numPoly		); 
			ReadInt ( fp, bone->chunk[i].firstVert	);
			ReadInt ( fp, bone->chunk[i].numVert		); 
		}
	}

	int nSub = 0;
	WORD nChunks = 0;
	ReadInt( fp, nSub );
	skeleton.setNSub( nSub );
	for (i = 0; i < skeleton.getNSub(); i++)
	{
		anmSubMesh* sub = skeleton.getSub( i );
		ReadWord( fp, nChunks );
		DWORD flags;
		ReadDword( fp, flags );
		sub->setNChunks( nChunks );
		sub->flags = flags;
		for (int j = 0; j < nChunks; j++)
		{
			ReadWord( fp, sub->chunks[j].subIdx		);
			ReadInt	( fp, sub->chunks[j].firstPoly	);
			ReadInt	( fp, sub->chunks[j].numPoly		);
			ReadInt	( fp, sub->chunks[j].firstVert	);
			ReadInt	( fp, sub->chunks[j].numVert		);
		}
		if (sub->isAnimated())
		{
			int animTime, nFrames, keyRate, boneIdx;
			Matr3x4 tr;
			ReadInt( fp, animTime );
			ReadInt( fp, nFrames  );
			ReadInt( fp, keyRate  );
			ReadInt( fp, boneIdx  );
			anmChannel& cha = sub->mtlAnimation;
			cha.setNFrames( nFrames );
			cha.keyRate = keyRate;
			cha.boneIdx = boneIdx;
			cha.animTime = animTime;
			for (int j = 0; j < nFrames; j++)
			{
				fread( tr, sizeof( Matr3x4 ), 1, fp );
				Matrix4D& m = cha.transf[j];
				m.e00 = tr[0]; m.e01 = tr[1]; m.e02 = tr[2]; m.e03 = 0.0f;
				m.e10 = tr[3]; m.e11 = tr[4]; m.e12 = tr[5]; m.e13 = 0.0f;
				//  Hack here - need to FIXIT
				m.e20 = -tr[6]; m.e21 = tr[7]; m.e22 = tr[8]; m.e23 = 0.0f;
				m.e30 = tr[9]; m.e31 = tr[10]; m.e32 = tr[11]; m.e33 = 1.0f;
			}	
		}
	}
	return false;
} // AnimModel::LoadSkeleton

bool AnimModel::LoadTexTable( IRenderSystem* irs, FILE* fp, int nBytes )
{
	int nTex = 0;
	fread( &nTex, 4, 1, fp );
	assert( c_StrBufSize > nBytes );
	fread( strBuf, nBytes - 4, 1, fp );
	char* pos = strBuf + 2;
	const int c_MaxTextures = 1024;
	int recodeTex[c_MaxTextures];
	assert( nTex < c_MaxTextures );
	for (int i = 0; i < nTex; i++)
	{
		recodeTex[i] = irs->GetTextureID( pos );
		pos += strlen( pos ) + 3;
	}
	for (i = 0; i < nSub; i++)
	{
		WORD tex0 = bm[i].getTexture(0) == 0xFFFF ? 0 : recodeTex[bm[i].getTexture(0)]; 
		WORD tex1 = bm[i].getTexture(1) == 0xFFFF ? 0 : recodeTex[bm[i].getTexture(1)]; 
		
		bm[i].setTexture( tex0, 0 );
		//bm[i].setTexture( tex0, 1 );
		bm[i].setTexture( irs->GetTextureID( "env.jpg" ), 1 );
		bm[i].setShader( irs->GetShaderID( "smooth" ) );
	}
	return false;
} // AnimModel::LoadTexTable

void AnimModel::ProgressAnim( int frame )
{
	int nB = skeleton.getNBones();
	Matrix4D matr;
	for (int i = 0; i < nB; i++)
	{
		anmBone* bone = skeleton.getBone( i );
		if (bone)
		{
			anim.getChannel( i ).calcMatrByFrame( frame, matr );
			bone->setTransform( matr );
		}
	}
	
	int nS = skeleton.getNSub();
	for (i = 0; i < nS; i++)
	{
		anmSubMesh* sub = skeleton.getSub( i );
		if (sub && sub->isAnimated())
		{
			sub->mtlAnimation.calcMatrByFrame( frame, matr );
			sub->setTransform( matr );
		}
	}

} // AnimModel::ProgressAnim	

/*---------------------------------------------------------------------------*/
/*	Func:	AnimModel::DrawPlain
/*	Desc:	Draws model without materials
/*---------------------------------------------------------------------------*/
void AnimModel::DrawPlain( IRenderSystem* irs )
{
	Matrix4D wm;
	wm = irs->GetWorldMatrix();
	Matrix4D matr;
	int nB = skeleton.getNBones();
	for (int i = 0; i < nB; i++)
	{
		const anmBone* bone = skeleton.getBone( i );
		if (!bone) continue;
		matr = bone->transform;
		matr *= wm;
		irs->SetWorldMatrix( matr );			
		for (int j = 0; j < bone->nChunks; j++)
		{
			const anmMeshChunk& chunk = bone->chunk[j]; 
			anmSubMesh* sub = skeleton.getSub( chunk.subIdx );
			if (sub) irs->SetTextureMatrix( sub->transform );
			irs->DrawPrim( bm[chunk.subIdx], 
							chunk.firstPoly, chunk.numPoly, 
							chunk.firstVert, chunk.numVert );   
		}
	}
	irs->SetWorldMatrix( wm );
} // AnimModel::DrawPlain

BaseMesh AnimModel::swBM;

void AnimModel::DrawSWSkinning( IRenderSystem* irs )
{/*
	if (swBM.getNVert() == 0)
	{
		swBM.create( 65535, 0, vfNMP4 );
		swBM.setShader( irs->GetShaderID( "smooth" ) );
	}

	Matrix4D matr;
	int nCh = skeleton.getNSub();
	for (int i = 0; i < nCh; i++)
	{
		BaseMesh&	cbm		= bm[i];
		anmSubMesh&	sub		= *skeleton.getSub( i );
		BYTE*		pVert	= cbm.getVertexData();
		BYTE*		pVertTo	= swBM.getVertexData();
		int			stride  = Vertex::CalcStride( cbm.getVertexFormat() ); 
		//int			nInfBones = 

		swBM.setVertexFormat( cbm.getVertexFormat() );
		swBM.setIndexPtr	( cbm.getIndices()		);
		swBM.setNVert		( cbm.getNVert()		);
		swBM.setMaxInd		( cbm.getNInd()			);
		swBM.setNInd		( cbm.getNInd()			);
		swBM.setNPri		( cbm.getNPri()			);
		swBM.setTexture		( cbm.getTexture()		);
		swBM.setTexture		( cbm.getTexture( 1 ), 1 );
	
		for (int j = 0; j < sub.nChunks; j++)
		{
			anmMeshChunk&	chunk	= sub.chunks[j];
			switch (cbm.getVertexFormat())
			{
				case vfN:
					VertexTraits<VertexN>::SkinVBuf( pVert, pVertTo, chunk.numVert, 
														skeleton.getBone( chunk.subIdx ),
														skeleton.getNBones() );
					break;
				case vfNMP1:
					VertexTraits<VertexNMP1>::SkinVBuf( pVert, pVertTo, chunk.numVert, 
														skeleton.getBonesArray(),
														skeleton.getNBones() );
					break;
				case vfNMP2:
					VertexTraits<VertexNMP2>::SkinVBuf( pVert, pVertTo, chunk.numVert, 
														skeleton.getBonesArray(),
														skeleton.getNBones() );
					break;
				case vfNMP3:
					VertexTraits<VertexNMP3>::SkinVBuf( pVert, pVertTo, chunk.numVert, 
														skeleton.getBonesArray(),
														skeleton.getNBones() );
					break;
				case vfNMP4:
					VertexTraits<VertexNMP4>::SkinVBuf( pVert, pVertTo, chunk.numVert, 
														skeleton.getBonesArray(),
														skeleton.getNBones() );
					break;
			}
			pVert	+= chunk.numVert * stride;
			pVertTo += chunk.numVert * stride;
		}

		irs->Draw( swBM );
	}*/
} // AnimModel::DrawSWSkinning

void AnimModel::Draw( IRenderSystem* irs ) 
{
	Matrix4D wm;
	wm = irs->GetWorldMatrix();
	Matrix4D matr;
	int nB = skeleton.getNBones(); 
	for (int i = 0; i < nB; i++)
	{
		const anmBone* bone = skeleton.getBone( i ); 
		if (!bone) continue;
		matr = bone->transform;
		matr *= wm;
		irs->SetWorldMatrix( matr );	 		
		for (int j = 0; j < bone->nChunks; j++)
		{
			const anmMeshChunk& chunk = bone->chunk[j]; 
			anmSubMesh* sub = skeleton.getSub( chunk.subIdx ); 
			if (sub) irs->SetTextureMatrix( sub->transform );
			irs->Draw( bm[chunk.subIdx], 
					chunk.firstPoly, chunk.numPoly, 
					chunk.firstVert, chunk.numVert );    
		}
	}
	irs->SetWorldMatrix( wm ); 
} // AnimModel::Draw

void AnimModel::DrawAABB( IRenderSystem* irs, DWORD color )
{
	AABoundBox aabb;
	skeleton.calcAABB( aabb );
	AuxPrim::DrawAABB( irs, aabb, color );
} // AnimModel::DrawAABB

void AnimModel::DrawBoneAABBs( IRenderSystem* irs, DWORD color )
{
	Matrix4D wm;
	wm = irs->GetWorldMatrix();
	Matrix4D matr;
	int nB = skeleton.getNBones();
	for (int i = 0; i < nB; i++)
	{
		const anmBone* bone = skeleton.getBone( i );
		if (!bone) continue;
		matr = bone->transform;
		matr *= wm;
		irs->SetWorldMatrix( matr );			
		AuxPrim::DrawAABB( irs, bone->aabb, color );
	}
} // AnimModel::DrawAABB


AnimModel::AnimModel()
{
}

void anmChannel::calcMatrByTime( int time, Matrix4D& matr )
{
	time %= animTime;
	int frIdx = nFrames * time / animTime; 
	matr = transf[frIdx];
}

void anmChannel::calcMatrByFrame( int frame, Matrix4D& matr )
{
	frame %= nFrames;
	matr = transf[frame];
}

/*****************************************************************************/
/*	anmSkeleton implementation
/*****************************************************************************/
bool anmSkeleton::calcAABB( AABoundBox& aabb )
{
	if (nBones == 0) return false;
	AABoundBox  cur;
	aabb.copy( bone[0].aabb );
	aabb.transform( bone[0].transform ); 
	
	for (int i = 1; i < nBones; i++)
	{
		cur.copy( bone[i].aabb );
		cur.transform( bone[i].transform );
		aabb.Union( cur );
	}
	return true;
} // anmSkeleton::calcAABB

