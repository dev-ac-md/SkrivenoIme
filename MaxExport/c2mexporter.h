/*****************************************************************************/
/*	File:	c2mExporter.h
/*	Desc:	CossacksII generic model export stuff
/*	Author:	Ruslan Shestopalyuk
/*	Date:	07.11.2002
/*****************************************************************************/
#ifndef __C2MEXPORTER_H__
#define __C2MEXPORTER_H__

#include <stdio.h>
#include <stdlib.h>
#include <direct.h>
#include "windows.h"
#include "assert.h"
#include "kHash.hpp"


typedef WORD IDXTYPE;
const int c_MaxNameLen			= 128;
const int c_MaxIndent			= 24;
const int c_MaxBonesPerObject	= 256;
const int c_MaxSceneTextures	= 256;
const int c_MaxSceneMaterials	= 256;
const int c_MaxBonesPerVertex	= 5;

typedef char PATH[_MAX_PATH];

enum GSortAttribute
{
	gsaUnknown		= 0,
	gsaObject		= 1,
	gsaMaterial		= 2,
	gsaIdxProximity = 3
}; // enum GVAttribute

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

/*****************************************************************************/
/*	Class:	VertexConfig
/*	Desc:	Vertex content configuration descriptor
/*****************************************************************************/
class VertexConfig
{
	DWORD			flagz;

public:
	VertexConfig() : flagz(0) {}

	void	Read		( FILE* fp )	{ fread( &flagz, 1, sizeof(DWORD), fp ); }
	int		Write		( FILE* fp )	{ fwrite( &flagz, sizeof(flagz), 1, fp ); 
											return sizeof( flagz ); }


	void	reset()		{ flagz = 0; }
	bool	hasXYZ		() const { return (flagz & 1) != 0; }
	bool	hasXYZW		() const { return (flagz & 2) != 0; }
	bool	hasUV		() const { return (flagz & 4) != 0; }
	bool	hasUV2		() const { return (flagz & 8) != 0; }
	bool	hasDiffuse	() const { return (flagz & 16) != 0;}
	bool	hasSpecular	() const { return (flagz & 32) != 0;}
	bool	hasNormal	() const { return (flagz & 64) != 0;}
	int		numBoneW	() const { return (flagz & 0xF0000000) >> 28;}

	void	setNumBoneW ( int nBW ) { flagz &= 0x0FFFFFFF; flagz |= (nBW << 28); }

	void	setXYZ		( bool has = true ) { if (has) flagz |= 1; else flagz &= ~1; }
	void	setXYZW		( bool has = true ) { if (has) flagz |= 2; else flagz &= ~2; }
	void	setUV		( bool has = true ) { if (has) flagz |= 4; else flagz &= ~4; }
	void	setUV2		( bool has = true ) { if (has) flagz |= 8; else flagz &= ~8; }
	void	setDiffuse	( bool has = true ) { if (has) flagz |= 16; else flagz &= ~16;}
	void	setSpecular	( bool has = true ) { if (has) flagz |= 32; else flagz &= ~32;}
	void	setNormal	( bool has = true ) { if (has) flagz |= 64; else flagz &= ~64;}

	int				getSizeBytes() const
	{
		return c_vfStride[(int)VF()];
	}
	VertexFormat	VF() const
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
	}

}; // class VertexConfig

/*****************************************************************************/
/*	Class:	GVertex
/*	Desc:	Generic vertex description
/*****************************************************************************/
class GVertex
{
public:
	GVertex() : x(0.0f), y(0.0f), z(0.0f), u(0.0f), v(0.0f), color(0xFFFFFFFF),
				matID(0xFFFF), smGroup(0xFFFFFFFF), objID(0xFFFF), nWeights(0) 
				{
					memset( weight, 0, c_MaxBonesPerVertex * sizeof( weight[0] ) ); 
					memset( boneRef, 0, c_MaxBonesPerVertex * sizeof( boneRef[0] ) ); 
				}

	//  hash function ;)
	unsigned int hash() const
	{
		unsigned char* s = (unsigned char*) this;
		unsigned int h = 0;
		int nBytes = sizeof( GVertex ) - sizeof( smGroup );
		for (int i = 0; i < nBytes; i++)
		{
			h = (h * 729 + (*s) * 37) ^ (h >> 1);
			s++;
		}
		return h;
	} // hash

	bool equal( const GVertex& el )
	{
		return (memcmp( this, &el, sizeof( GVertex ) - sizeof( smGroup ) ) == 0)
				&& ((smGroup & el.smGroup) != 0);
	} // equal

	void copy( const GVertex& el )
	{
		memcpy( this, &el, sizeof( GVertex ) );
	} // copy

	float	x, y, z;		//  position
	float	nx, ny, nz;		//  normal
	float	u, v;			//  tex coords
	WORD	matID;			//  material ID
	DWORD	color;			//  vertex color
	WORD	objID;			//  object ID
	float	weight[c_MaxBonesPerVertex];	//  skinning weight
	BYTE	boneRef[c_MaxBonesPerVertex];	//  influence bone index
	BYTE	nWeights;		//  number of skinning weights
	
	DWORD	smGroup;		//  smoothing group

	static int __cdecl cmpObject( const void *e1, const void *e2 )
	{
		return ((GVertex*)e1)->objID - ((GVertex*)e2)->objID;
	}

	static int __cdecl cmpMaterial( const void *e1, const void *e2 )
	{
		return ((GVertex*)e1)->matID - ((GVertex*)e2)->matID;
	}

	void dump( FILE* fp )
	{
		fprintf( fp, "matID: %d; objID: %d; smGroup: %d; nWeights: %d; ", 
					matID, objID, smGroup, nWeights );
		if (nWeights > 0)
		{
			for (int i = 0; i < nWeights; i++)
			{
				fprintf( fp, "w%d=%1.4f; b%d=%d;  ", i, weight[i], i, boneRef[i] );
			}
		}
		fprintf( fp, "\n" );
	}
}; // class GVertex

const int c_MaxTextures = 2;

enum MtlFlag
{
	mfBumpMap		= 1,
	mfEnvMap		= 2,
	mfAnimated		= 3
}; // enum MtlFlags

const int c_MaxBones  = 1024;
const int c_MaxFrames = 1024;

typedef float Matr3x4[12];

/*****************************************************************************/
/*	Class:	GAnimChannel
/*	Desc:	Generic animation
/*****************************************************************************/
class GAnimChannel
{
public:

	int			boneIdx;
	int			animTime;
	int			keyRate;
	int			nFrames;

	Matr3x4		transf[c_MaxFrames];

	GAnimChannel() : nFrames(0), boneIdx(0), animTime(0) {}
	void		addFrame( Matr3x4* tr ) 
				{ 
					assert (nFrames < c_MaxFrames);
					memcpy( transf[nFrames], tr, sizeof( Matr3x4 ) ); 
					nFrames++;
				}
	bool		isConstant()
	{
		if (nFrames <= 1) return true;
		for (int i = 1; i < nFrames; i++)
		{
			if (memcmp( transf[i - 1], transf[i], sizeof( Matr3x4 ) )) return false;
		}
		return true;
	}

}; // class GAnimChannel

/*****************************************************************************/
/*	Class:	GAnimation
/*	Desc:	Generic animation
/*****************************************************************************/
class GAnimation
{
public:

	char			animName[c_MaxNameLen];
	int				animTime;
	int				defKeyRate;
	GAnimChannel	channel[c_MaxBones];
	int				nChannels;
	
					GAnimation()	{ reset(); }
	void			reset()			{ nChannels = 0; animTime = 0; }
	void			construct( const char* name, int time, int keyRate, int nBones = 0 )
	{
		strcpy_s( animName, name );
		animTime = time;
		nChannels = nBones;
		for (int i = 0; i < nChannels; i++)
		{
			channel[i].boneIdx	= i;
			channel[i].animTime = animTime;
			channel[i].keyRate	= keyRate;
			channel[i].nFrames  = 0;
		}
		defKeyRate = keyRate;
	}

	void			addChannel( int boneIdx )
	{
		if (nChannels >= c_MaxBones) 
		{
			assert( false );
			return;
		}
		channel[nChannels].boneIdx	= boneIdx;
		channel[nChannels].animTime = animTime;
		channel[nChannels].keyRate	= defKeyRate;
		channel[nChannels].nFrames  = 0;
		nChannels++;
	}

}; // class GAnimation

struct MeshChunk
{
	WORD	subIdx;
	int		firstPoly;
	int		numPoly;
	int		firstVert;
	int		numVert;

	MeshChunk() : subIdx(-1), firstPoly(0), numPoly(0), 
							firstVert(0), numVert(0) {}
}; // struct MeshChunk

const int c_MaxMeshChunks = 128;

/*****************************************************************************/
/*	Class:	GBone
/*	Desc:	Generic bone
/*****************************************************************************/
class GBone
{
public:
	int				idx;
	int				papaIdx;
	char			name[c_MaxNameLen];
	int				nChunks;
	MeshChunk		chunks[c_MaxMeshChunks];

	//  bounding box
	float			minX, minY, minZ;
	float			maxX, maxY, maxZ;

	GBone() : nChunks(0) { name[0] = 0; papaIdx = -1; }
	void			addChunk( int idx, int firstPoly, int numPoly, int firstVert, int numVert )
	{
		chunks[nChunks].subIdx		= idx;
		chunks[nChunks].firstPoly	= firstPoly;
		chunks[nChunks].numPoly		= numPoly;
		chunks[nChunks].firstVert	= firstVert;
		chunks[nChunks].numVert		= numVert;
		nChunks++;
	}

	int				getNPolys() const
	{
		int nP = 0;
		for (int i = 0; i < nChunks; i++) nP += chunks[i].numPoly;
		return nP;
	}

	void			setBBox(	float x1, float y1, float z1, 
								float x2, float y2, float z2 )
	{
		minX = x1; maxX = x2;
		minY = y1; maxY = y2;
		minZ = z1; maxZ = z2;
	}

private:
	friend class	c2mExporter;
}; // class GBone

const int c_MaxSubmeshes = 128;

/*****************************************************************************/
/*	Class:	GSubMesh
/*	Desc:	Generic submesh - set of geometry rendered with same material
/*****************************************************************************/
class GSubMesh
{
public:
	MeshChunk		chunks[c_MaxMeshChunks];
	int				nChunks;

	GSubMesh() : nChunks(0) {}

	void			addChunk( int idx, int firstPoly, int numPoly, int firstVert, int numVert )
	{
		chunks[nChunks].subIdx		= idx;
		chunks[nChunks].firstPoly	= firstPoly;
		chunks[nChunks].numPoly		= numPoly;
		chunks[nChunks].firstVert	= firstVert;
		chunks[nChunks].numVert		= numVert;
		nChunks++;
	}

	int				getNPolys() const
	{
		int nP = 0;
		for (int i = 0; i < nChunks; i++) nP += chunks[i].numPoly;
		return nP;
	}

	int				getNVert() const
	{
		int nV = 0;
		for (int i = 0; i < nChunks; i++) nV += chunks[i].numVert;
		return nV;
	}

};	// class GSubMesh

/*****************************************************************************/
/*	Class:	GSkeleton
/*	Desc:	Generic skeleton
/*****************************************************************************/
class GSkeleton
{
protected:
	GBone			bones[c_MaxBones];
	int				nBones;

	GSubMesh		subs [c_MaxSubmeshes];
	int				nSubs;

public:
					GSkeleton() : nBones(0) {}
	void			reset() { nBones = 0; nSubs = 0; }
	void			addBone( GBone& bone ) 
					{ 
						if (nBones < c_MaxBones) bones[nBones++] = bone;
						else assert( false );
					}
	void			addSub( GSubMesh& sub ) 
					{ 
						if (nSubs < c_MaxSubmeshes) subs[nSubs++] = sub;
						else assert( false );
					}
	int				getNBones() const	{ return nBones; }
	int				getNSubs() const	{ return nSubs; }
	GBone&			getBone( int bidx ) { return bones[bidx]; }
	GSubMesh&		getSub ( int sidx ) { return subs[sidx]; }
	int				getNPolys() const 
					{
						int nP = 0;
						for (int i = 0; i < nBones; i++) nP += bones[i].getNPolys();
						return nP;
					}
	
private:
	friend class	c2mExporter;
}; // class GSkeleton

/*****************************************************************************/
/*	Class:	GMaterial
/*	Desc:	Generic material description
/*****************************************************************************/
class GMaterial
{
	WORD			tex[c_MaxTextures];
	float			transp;
	DWORD			flagz;
	GAnimChannel	animChannel;
public:
	GMaterial()	
	{
		memset( this, 0, sizeof( *this ) );
	}

	GAnimChannel& getAnimChannel() { return animChannel; }
	void setTexture( WORD texID, int stage = 0 ) 
	{
		tex[stage] = texID;
	}

	void setTransparency( float tr )
	{
		transp = tr;
	}

	WORD getTexture( int stage = 0 ) const
	{
		return tex[stage];
	}

	DWORD flags() const { return flagz; }

	void setFlag( MtlFlag flag, bool state = true )
	{
		if (state)
		{
			switch (flag)
			{
			case mfBumpMap:	flagz |= 0x1; return;
			case mfEnvMap:	flagz |= 0x2; return;
			case mfAnimated:flagz |= 0x4; return;
			}
		}
		else
		{
			switch (flag)
			{
			case mfBumpMap:	flagz &= ~0x1; return;
			case mfEnvMap:	flagz &= ~0x2; return;
			case mfAnimated:flagz &= ~0x4; return;
			}	
		}
	}

	bool getFlag( MtlFlag flag )
	{
		switch (flag)
		{
		case mfBumpMap:	return ((flagz & 0x1) != 0);
		case mfEnvMap:	return ((flagz & 0x2) != 0);
		case mfAnimated:return ((flagz & 0x4) != 0);
		}
		return false;
	}
	unsigned int hash() const
	{
		unsigned char* s = (unsigned char*) this;
		unsigned int h = 0;
		for (int i = 0; i < sizeof( *this ); i++)
		{
			h = (h * 729 + (*s) * 37) ^ (h >> 1);
			s++;
		}
		return h;
	} // hash

	bool equal( const GMaterial& el )
	{
		return (memcmp( this, &el, sizeof( GMaterial ) ) == 0);
	} // equal

	void copy( const GMaterial& el )
	{
		memcpy( this, &el, sizeof( GMaterial ) );
	} // copy
}; // class GMaterial

/*****************************************************************************/
/*	Class:	BlockWriter
/*	Desc:	Writes chunk of information
/*****************************************************************************/
class BlockWriter
{
	FILE*			fp;
	BlockWriter*	papa;
	int				curBlockSize;
	long 			filePos;

public:
	BlockWriter( BlockWriter* _papa, const char* magic );
	BlockWriter( FILE* file, const char* magic );

	void			step( int nBytes );
	void			end();
	int				blockSize() const { return curBlockSize; }
	int				headerSize() const { return 8; }
	int				fullSize() const { return curBlockSize + 8; }

}; // class BlockWriter

typedef Hash<GVertex>				VertHash;
typedef Hash<GMaterial, 256, 256>	MtlHash;

struct Poly
{
	int			v1;
	int			v2;
	int			v3;
}; // struct Poly

enum DialogResult
{
	drUnknown		= 0,
	drYes			= 1,
	drNo			= 2,
	drOK			= 3,
	drCancel		= 4
}; // enum DialogResult

const int c_MinPolyNum = 65536;

#define NO_MATERIAL 0xFFFF

/*****************************************************************************/
/*	Class:	c2mExporter
/*	Desc:	Generic export class, contains stuff independent of 
/*				Max/Maya/Whatever specifics
/*****************************************************************************/
class c2mExporter
{
public:
	c2mExporter();

	virtual void StartExporting() = 0;
	
	void	CreateTextureDir();
	void	CopySceneTextures();
	void	ShowExportDialog();
	void	Msg( const char* format, ... );
	void	SetProgress( double progrVal );
	void	StepProgressNode();

	void	MsgIndent();
	void	MsgUnindent();

	void	ClearTextureTable();
	int		GetTexID( const char* texFile );

	void	InitInternal();
	void	ShutInternal();

	int		WriteC2M();

	//  model-related
	void	CleanModel();
	int		AddPolygon( int v1, int v2, int v3 );

	void	UpOneDirTreeLevel( char* dir );


	static char*	BrowseForFolder();
	static HINSTANCE hInstance;

	VertHash				vertHash;

protected:
	//  exporting options
	bool	bStaticVertexColors;
	bool	bCopyTextures;		
	bool	bKeyframe;			
	bool	bExportAnimation;	
	bool	bExportSkeletal;	
	bool	bExportHeadCam;		
	bool	bExportNormals;		
	bool	bExportColors;		
	bool	bIgnoreSmoothing;	
	bool	bNoTex;
	bool	bHierTransforms;
	bool	bSkinning;
	bool	b2ndTexPair;
	bool	bSuppressLog;
	int		nKPS;
	int		nWeightsPerBone;


	char	modelFileName[_MAX_PATH];
	char	texPath[_MAX_PATH];
	
	double	progress;
	double	progressPerNode;
	int		nSceneNodes;

	VertexConfig	vertConfig;

	int		nIndent;
	double	radius2;

	//  generic window-related 
	bool	IsChecked	( HWND hWnd );
	void	SetCheck	( HWND hWnd, bool checked );
	bool	GetInt		( HWND hWnd, int& val );
	void	EnableAnimGroup( bool enable ); 

	DialogResult AskYesNo( const char* msg );
	

	bool	checkPolySanity( const Poly& p );

	static  c2mExporter*	me;
	static bool				exportDone;
	

	MtlHash					mtlHash;
	GSkeleton				skeleton;
	int						curParentBone;
	bool					haveBlendedVertices;
	bool					abortProcess;

	GAnimation				anim;


	Poly*					poly;
	GVertex*				reordVertArray;

	int						nPoly;
	int						maxPoly;

	int						WriteString		( FILE* fp, const char* str );
	int						WriteInt		( FILE* fp, int val );
	int						WriteFloat		( FILE* fp, float val );
	int						WriteMaterial	( FILE* fp, const GMaterial& mtl );
	int						WriteVertex		( FILE* fp, const GVertex& vert, int nWeights = 0 );
	int						WriteWord		( FILE* fp, WORD val );
	int						WritePoly		( FILE* fp, IDXTYPE v1, IDXTYPE v2, IDXTYPE v3 );
	int						WriteBone		( FILE* fp, const GBone& bone );
	int						WriteSkeleton	( FILE* fp, GSkeleton& skel );
	int						WriteMesh		( FILE* fp );
	int						WriteAnimation	( FILE* fp, GAnimation& anm );
	int						WriteTextureTable( FILE* fp );
	int						WriteAnimChannel( FILE* fp, GAnimChannel& cha );

	void					SortPoly( int first, int num, GSortAttribute attr );
	void					ReorderPolys();
	int						GetPolyClusterSize( int firstPoly, GSortAttribute attr );
	void					SetVertexConfig();

	void					DumpPolys( FILE* fp );
	void					DumpAnimations( FILE* fp );
	void					OnOpenExportDlg();

private:

	static HWND		hPanelWnd;
	static HWND		hLogWnd;	
	static HWND		hProgressWnd;
	static HWND		hGoBtn;
	static HWND		hTexPathWnd;
	static HWND		hChPathWnd;
	static HWND		hCopyTexWnd;
	static HWND		hWeightsPerBoneWnd;
	
	static HWND		hKFWnd;
	static HWND		hSampleWnd;
	static HWND		hExportAnimWnd;
	static HWND		hSrateWnd;
	static HWND		hKPSWnd	;
	static HWND		hSrateCap;
	static HWND		hCopyTCap;
	
	static HWND		hAnGroup;
	static HWND		hObjWnd;
	static HWND		hNormWnd;
	static HWND		hColWnd;
	static HWND		hIgnoreSm;
	static HWND		hSkinning;
	static HWND		h2ndTexPair;
	static HWND		hNoTex;
	static HWND		hSuppressLog;
	
	static HWND		hExportHeadCam;
	static HWND		hExportSkeletal;
	static HWND		hHierTransforms;

	static BOOL CALLBACK 
		OptionsDlgProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam );
	static void		EnableControls();
	static void		DisableControls();

	void			Dialog2Options();
	void			Options2Dialog();

	PATH			texTable[c_MaxSceneTextures];
	int				nTextures;
	static char		folderPath[_MAX_PATH];
}; // class c2mExporter

#endif // __C2MEXPORTER_H__