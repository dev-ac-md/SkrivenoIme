/*****************************************************************************/
/*	File:	MaxExport.cpp
/*	Desc:	Export from max to c2m
/*	Author:	Ruslan Shestopalyuk
/*	Date:	07.11.2002
/*****************************************************************************/
#include <direct.h>
#include <stdio.h>
#include <stdlib.h>

#include "c2mExporter.h"
#include "MaxUtil.h"
#include "MaxExport.h"

#include "meshadj.h"
#include "stdmat.h"
#include "modstack.h"

#pragma warning (disable : 4530)

using namespace std;
static maxExporterClassDesc c2mExporterDesc;
ClassDesc2* Getc2mExporterDesc() { return &c2mExporterDesc; }

/*****************************************************************************/
/*	maxExporter implementation
/*****************************************************************************/
maxExporter::maxExporter() : SceneExport(), c2mExporter()
{
	iPCtxExport = 0;
}

maxExporter::~maxExporter() 
{
}

/*****************************************************************************/
/*	3d Max base exporter class methods implementations
/*****************************************************************************/
int maxExporter::ExtCount()
{
	return 1;
}

const TCHAR *maxExporter::Ext(int n)
{		
	return _T("c2m");
}

const TCHAR *maxExporter::LongDesc()
{
	return _T("Export to the Cossacks2 model format");
}
	
const TCHAR *maxExporter::ShortDesc() 
{			
	return _T("c2m Model");
}

const TCHAR *maxExporter::AuthorName()
{			
	return _T("Silver");
}

const TCHAR *maxExporter::CopyrightMessage() 
{	
	return _T("Copyright(C) 2000-2002 GSC Game World");
}

const TCHAR *maxExporter::OtherMessage1() 
{		
	return _T("");
}

const TCHAR *maxExporter::OtherMessage2() 
{		
	return _T("");
}

unsigned int maxExporter::Version()
{				
	return 100;
}

void maxExporter::ShowAbout(HWND hWnd)
{			
}

BOOL maxExporter::SupportsOptions(int ext, DWORD options)
{
	return TRUE;
}

/*---------------------------------------------------------------------------*/
/*	Func:	maxExporter::DoExport
/*	Desc:	Core max exporting process entry point
/*---------------------------------------------------------------------------*/
int	maxExporter::DoExport(	const TCHAR*	name,
							ExpInterface*	ei,
							Interface*		i, 
							BOOL			suppressPrompts, 
							DWORD			options )
{
	strcpy( modelFileName, name );
	OnOpenExportDlg();
	//  re-assign silly interface names
	iExport = ei;
	iMax	= i;

	if(!suppressPrompts) ShowExportDialog();
	return TRUE;
} // maxExporter::DoExport

void maxExporter::PreTraverseSubTree( INode* node )
{
	ObjectState os = node->EvalWorldState( curTime );
	Object* obj = os.obj;
	if (obj)
	{
		if (obj->ClassID() != Class_ID(TARGET_CLASS_ID, 0))
		{
			//  add node pointer to hash
			HashedNode hnode( node );
			boneHash.add( hnode );
		}
	}
	int numCh = node->NumberOfChildren();
	for (int i = 0; i < numCh; i++) PreTraverseSubTree( node->GetChildNode( i ) );
} // maxExporter::PreTraverseSubTree

/*---------------------------------------------------------------------------*/
/*	Func:	maxExporter::PreprocessScene	
/*	Desc:	Traverse scene counting nodes etc.
/*---------------------------------------------------------------------------*/
void maxExporter::PreprocessScene()
{
	TimeValue dTime	= iMax->GetAnimRange().End() - 
						iMax->GetAnimRange().Start();
	int ms = dTime * 1000 / TIME_TICKSPERSEC; 	
	int rate;
	if (bKeyframe)
	{
		rate = dTime / GetTicksPerFrame();
	}
	else
	{
		rate = nKPS;
	}
	
	anim.construct( "default", ms, rate ); 

	ClearTextureTable();
	nSceneNodes = TotalChildrenNum( iMax->GetRootNode() );
	
	boneHash.reset();
	PreTraverseSubTree( iMax->GetRootNode() );

	if (nSceneNodes > 0) progressPerNode = 0.9f / (double)nSceneNodes;
	if (nSceneNodes > 512) 
		Msg( "Ого. Да я коньки откину с такой сценой. Можете идти за пепси-колой." );
} // maxExporter::PreprocessScene

/*---------------------------------------------------------------------------*/
/*	Func:	maxExporter::ProcessNodeSubTree	
/*	Desc:	Extracts data from the given 3d max scene node (and, recurrently,
/*				from all its subtree)
/*---------------------------------------------------------------------------*/
void maxExporter::ProcessNodeSubTree( INode* node )
{
	if (abortProcess) return;
	if (!node) return;

	//  acnowledge user we are in this node
	TCHAR* nodeName = node->GetName();
	Msg( "Обработка узла <%s>...", nodeName );
	
	MsgIndent();
	StepProgressNode();
	ProcessNode( node );
	//  do stuff recursively for all children
	int numCh = node->NumberOfChildren();
	for (int i = 0; i < numCh; i++) 
	{
		ProcessNodeSubTree( node->GetChildNode( i ) );
		if (abortProcess) return;
	}
	MsgUnindent();
} // maxExporter::ProcessNodeSubTree

/*---------------------------------------------------------------------------*/
/*	Func:	maxExporter::ProcessNodeMtl
/*	Desc:	analyses node material, adds correspondednt texture and shader
/*				to the table
/*	Parm:	node - node which material to process
/*	Ret:	shID - shader ID
/*			texID0 - 1st layer texture ID
/*			texID1 - 2nd layer texture ID
/*---------------------------------------------------------------------------*/
void maxExporter::ProcessNodeMtl( INode* node )
{
	Mtl* mtl = node->GetMtl();
	if (!mtl) 
	{
		Msg( "Узел не имеет материала." );
		nSubMtls = NO_MATERIAL;
		return;
	}

	if (mtl->ClassID() == Class_ID(DMTL_CLASS_ID, 0))
	{
		subMtl[0]	= ProcessStdMtl( mtl );
		nSubMtls	= 0;
		return;
	}
	
	//  material has a bunch of sub-materials
	if (mtl->ClassID() == Class_ID(MULTI_CLASS_ID, 0))
	{
		for (int cmat = 0; cmat < mtl->NumSubMtls(); cmat++)
		{
			subMtl[cmat] = ProcessStdMtl( mtl->GetSubMtl( cmat ) );
		}
		nSubMtls = cmat;
		return ;
	}

	nSubMtls = NO_MATERIAL;
} // maxExporter::ProcessNodeMtl

/*---------------------------------------------------------------------------*/
/*	Func:	maxExporter::ProcessStdMtl	
/*---------------------------------------------------------------------------*/
int maxExporter::ProcessStdMtl( Mtl* mtl )
{
	GMaterial	gmtl;

	//  material colors
	Color ambient		= mtl->GetAmbient();
	Color diffuse		= mtl->GetDiffuse();
	Color specular		= mtl->GetSpecular();
	float specularPower = mtl->GetShinStr();

	if (specularPower > 0.0f)
	//  ok. we've got specular color
	{
	}
	else
	{
	}

	WORD tex0 = 0xFFFF;
	WORD tex1 = 0xFFFF;

	StdMat*		smat		= (StdMat*)mtl;
	bool		twoSided	= (smat->GetTwoSided() != 0);
	StdUVGen*	uvgen		= 0;
	
	//  diffuse map
	int cLay = 0;
	float amt = 1.0;
	Texmap* difMap = mtl->GetSubTexmap( ID_DI );
	if (difMap && smat->MapEnabled( ID_DI ))
	{
		amt = smat->GetTexmapAmt( ID_DI, 0 );
		gmtl.setTransparency( amt );
		if (difMap->ClassID() == Class_ID(BMTEX_CLASS_ID, 0))
		{
			BitmapTex *bmt = (BitmapTex*) difMap;
			tex0 = GetTexID( bmt->GetMapName() );
			uvgen = bmt->GetUVGen();
		}
		cLay++;
	}

	//  bump map
	Texmap* bumpMap = mtl->GetSubTexmap( ID_BU );
	if (bumpMap && smat->MapEnabled( ID_BU ))
	{
		amt = smat->GetTexmapAmt( ID_BU, 0 );
		if (bumpMap->ClassID() == Class_ID(BMTEX_CLASS_ID, 0))
		{
			BitmapTex *bmt = (BitmapTex*) bumpMap;
			tex1 = GetTexID( bmt->GetMapName() );
			gmtl.setFlag( mfBumpMap, true );
		}
		cLay++;
	}
	//  environment map
	Texmap* envMap = mtl->GetSubTexmap( ID_RL );
	if (bumpMap && smat->MapEnabled( ID_RL ))
	{
		amt = smat->GetTexmapAmt( ID_RL, 0 );
		if (envMap->ClassID() == Class_ID(BMTEX_CLASS_ID, 0))
		{
			BitmapTex *bmt = (BitmapTex*) envMap;
			tex1 = GetTexID( bmt->GetMapName() );
			gmtl.setFlag( mfEnvMap, true );
		}
		cLay++;
	}

	gmtl.setTexture( tex0, 0 );
	gmtl.setTexture( tex1, 1 );

	//  extracting material animation
	
	if (bExportAnimation && uvgen)
	{
		GAnimChannel& cha = gmtl.getAnimChannel();
		TimeValue startTime = 0;
		TimeValue endTime	= 0;
		TimeValue timeStep;
		startTime	= iMax->GetAnimRange().Start();
		endTime		= iMax->GetAnimRange().End();
		if (bKeyframe)
		{
			timeStep = GetTicksPerFrame();
		}
		else
		{
			timeStep	= TIME_TICKSPERSEC / nKPS;
		}
		for (TimeValue cTime = startTime; cTime <= endTime; cTime += timeStep)
		{
			Matr3x4		matr;
			matr[0]	= 1.0f; 
			matr[1] = 0.0f; 
			matr[2] = 0.0f;
			
			matr[3]	= 0.0f; 
			matr[4] = 1.0f; 
			matr[5] = 0.0f;
			
			matr[6]	= uvgen->GetUOffs( cTime ); 
			matr[7]= -uvgen->GetVOffs( cTime ); 
			matr[8]= 1.0f;

			matr[9]		= 0.0f;
			matr[10]	= 0.0f;
			matr[11]	= 0.0f;

			cha.addFrame( &matr );
		}

		TimeValue dTime	= iMax->GetAnimRange().End() - iMax->GetAnimRange().Start();
		int ms = dTime * 1000 / TIME_TICKSPERSEC; 	
		int rate;
		if (bKeyframe) rate = dTime / GetTicksPerFrame();
			else rate = nKPS;
		cha.animTime = ms;
		cha.keyRate = rate;

		gmtl.setFlag( mfAnimated, !cha.isConstant() );
	}

	return mtlHash.add( gmtl );
} // maxExport::ProcessStdMtl

bool maxExporter::FetchAnimationTracks( INode* node, int curBoneIdx )
{
	Matrix3 parentTM, nodeTM, localTM;
	
	int cIdx = anim.nChannels;
	anim.addChannel( curBoneIdx );
	//  animation track for that node
	if (bExportAnimation)
	{
		TimeValue startTime = 0;
		TimeValue endTime	= 0;
		TimeValue timeStep;
		startTime	= iMax->GetAnimRange().Start();
		endTime		= iMax->GetAnimRange().End();
		if (bKeyframe)
		{
			timeStep = GetTicksPerFrame();
		}
		else
		{
			timeStep	= TIME_TICKSPERSEC / nKPS;
		}
		for (TimeValue cTime = startTime; cTime <= endTime; cTime += timeStep)
		{
			nodeTM		= node->GetObjTMAfterWSM( cTime );
			parentTM	= node->GetParentNode()->GetObjTMAfterWSM( cTime );
			localTM		= nodeTM * Inverse( parentTM );
		
			Matrix3&	tm = bHierTransforms ? localTM : nodeTM;
			Matr3x4		matr;
			matr[0]	= tm.GetRow(0).x; 
			matr[1] = tm.GetRow(0).y; 
			matr[2] = tm.GetRow(0).z;
			
			matr[3]	= tm.GetRow(1).x; 
			matr[4] = tm.GetRow(1).y; 
			matr[5] = tm.GetRow(1).z;
			
			matr[6]	= tm.GetRow(2).x; 
			matr[7] = tm.GetRow(2).y; 
			matr[8] = tm.GetRow(2).z;

			matr[9]	= tm.GetRow(3).x; 
			matr[10]= tm.GetRow(3).y; 
			matr[11]= tm.GetRow(3).z;// + 250.0f;
			
			anim.channel[cIdx].addFrame( &matr );
		}
	}
	return true;
} // maxExporter::FetchAnimationTracks

/*---------------------------------------------------------------------------*/
/*	Func:	maxExporter::ProcessNode	
/*	Desc:	Extracts data from the given 3d max scene node
/*---------------------------------------------------------------------------*/
void maxExporter::ProcessNode( INode* node )
{
	//  evaluate node
	TCHAR* nodeName = node->GetName(); 
	ObjectState os = node->EvalWorldState( curTime );
	Object* obj = os.obj;
	if (!obj) return;
	StepProgressNode();

	//  skip light and camera target objects
	if (obj->ClassID() == Class_ID(TARGET_CLASS_ID, 0)) return; 
	
	//  add bone to the skeleton
	GBone newBone;
	strcpy( newBone.name, nodeName );
	int curBoneIdx	= skeleton.getNBones();
	newBone.idx		= curBoneIdx;
	newBone.papaIdx	= curParentBone;
	if (skeleton.getNBones() == c_MaxBones)
	{
		Msg(	"Ничего не будет. Сильно много подобъектов. "
				"Максимально поддерживаемое количество == %d", 
				c_MaxBonesPerObject );
		abortProcess = true;
		return;
	}
	
	if (obj->ClassID() == Class_ID(BONE_CLASS_ID, 0)) 
	{
		skeleton.addBone( newBone );
		FetchAnimationTracks( node, curBoneIdx );
		return;
	}

	if (obj->SuperClassID()	!= GEOMOBJECT_CLASS_ID) return; 

	skeleton.addBone( newBone );
	GBone& bone = skeleton.getBone( curBoneIdx );

	//  node submaterials local-to-global material index mapping table
	const int	c_MaxSubMtls = 0;
	ProcessNodeMtl( node );

	//  tesselate object
	int			deleteIt	= FALSE;
	TriObject	*tri		= 0;
	if (obj->CanConvertToType( Class_ID( TRIOBJ_CLASS_ID, 0 ) )) 
	{ 
		tri = (TriObject*) obj->ConvertToType( curTime, Class_ID(TRIOBJ_CLASS_ID, 0));
	}
	if (obj != tri) deleteIt = TRUE;
	if (!tri) 
	{
		Msg( "Не получилось триангулировать объект в узле. Узел пропущен." );
		return;
	}

	//  correct vertex polygon order (we need CW)
	Matrix3 parentTM, nodeTM, localTM;

	if (!node->IsNodeHidden() && node->Renderable())
	{
		nodeTM = node->GetObjTMAfterWSM( 0 );	
		BOOL negScale;
		if (DotProd( CrossProd( nodeTM.GetRow(0), nodeTM.GetRow(1) ), nodeTM.GetRow(2) ) < 0.0)
		{
			negScale = 0;
		}
		else
		{
			negScale = 1;
		}
		
		int vx[3];
		if (negScale) 
		{
			vx[0] = 2; vx[1] = 1; vx[2] = 0; 
		} 
		else 
		{ 
			vx[0] = 0; vx[1] = 1; vx[2] = 2; 
		}
		
		Mesh& mesh	= tri->GetMesh();
		
		bool hasUV  = (mesh.numTVerts != 0);
		bool hasClr = (mesh.numCVerts != 0);
		
		AdjEdgeList		edgeList( mesh );
		AdjFaceList		faceList( mesh, edgeList );
		
		//  some cosmetics
		BOOL res = mesh.RemoveDegenerateFaces();
		if (res) Msg( "Найдены дегенеративные лица - удалено." );
		res = mesh.RemoveIllegalFaces();
		if (res) Msg( "Найдены нелегальные индексы - удалено." );
		
		WORD numPri	= mesh.getNumFaces();
		int numVert = mesh.numVerts;
		
		haveBlendedVertices = false;
		bool haveWeights = InitNodePhysiqueContext( node );
		
		if (haveWeights)
		{
			assert( iPCtxExport && iPCtxExport->GetNumberVertices() == numVert ); 
		}
		
		//Msg( "Полигонов: %d", numPri );
		
		//  loop on polys, 'splitting' vertices, if needed, by tex coords, material, smoothing 
		GVertex gv;
		gv.objID = bone.idx;
		Point3 norm;
		for (int i = 0; i < numPri; i++)
		{
			Face& face	= mesh.faces[i];
		
			gv.matID	= (nSubMtls > 0) ?	subMtl[face.getMatID() % nSubMtls] : //  has submtls 
						((nSubMtls == 0) ?	subMtl[0] :							 //  single mtl	 
											NO_MATERIAL);						 //  no mtl
			gv.smGroup	= -1;
		
			if (bExportNormals)
			{
				norm = mesh.FaceNormal( i, TRUE );
				gv.nx = norm.x;
				gv.ny = norm.y;
				gv.nz = norm.z;
			}
			
			int vidx[3];
			for (int j = 0; j < 3; j++)
			{
				Point3 pt	= mesh.verts[face.v[vx[j]]];
				gv.x		= pt.x;
				gv.y		= pt.y;
				gv.z		= pt.z;
		
				//  check for object radius
				nodeTM.PointTransform( pt );
				double r2 = pt.x * pt.x + pt.y * pt.y + pt.z * pt.z;
				if (r2 > radius2) radius2 = r2;
		
				if (haveWeights) FetchVertexWeights( gv, face.v[vx[j]] );

				//  it is assuming that there is no hard edges in the object 
				//  that is skinned by Physique. Maybe it is incorrect.
				if (!bIgnoreSmoothing && 
					!haveBlendedVertices && !haveWeights) 
				{
					gv.smGroup = face.smGroup;
				}
				else 
				{
					gv.smGroup = 0xFFFFFFFF;
				}
				
				if (bExportColors && hasClr)	
				{
					const VertColor& col = mesh.vertCol[mesh.vcFace[i].t[vx[j]]];
				}
		
				if (!bNoTex && hasUV)	
				{
					const Point3& texCoord = mesh.tVerts[mesh.tvFace[i].t[vx[j]]];
					gv.u = texCoord.x;
					gv.v = 1.0f - texCoord.y;
				}
		
				//  new vertex index
				vidx[j] = vertHash.add( gv );
			}
			AddPolygon( vidx[0], vidx[1], vidx[2] );
		}

		//  bounding box
		mesh.buildBoundingBox();
		Box3 box = mesh.getBoundingBox();
		bone.setBBox(	box.pmin.x, box.pmin.y, box.pmin.z,
						box.pmax.x, box.pmax.y, box.pmax.z );
	}

	FetchAnimationTracks( node, curBoneIdx );	

	if (deleteIt) delete tri;
} // maxExporter::ProcessNode

Modifier* maxExporter::FindModifier( INode* node, Class_ID modID )
{
	Object* obj = node->GetObjectRef();
	if (obj->SuperClassID() == GEN_DERIVOB_CLASS_ID)
	{
		IDerivedObject* dm = (IDerivedObject*)obj;
		int nMod = dm->NumModifiers();
		for(int i = 0; i < nMod; i++)
		{
			Modifier* modifier = dm->GetModifier(i);
			Class_ID id = modifier->ClassID(); 
			if (id == modID)
			{
				return modifier;
			}
		}
	}
	return 0;
} // maxExporter::FindModifier

/*---------------------------------------------------------------------------*/
/*	Func:	maxExporter::FetchVertexWeights 
/*	Desc:	Fills skinning weights values for given vertex
/*	Rmrk:	iPCtxExport should be properly initialized for given node
/*---------------------------------------------------------------------------*/
bool maxExporter::FetchVertexWeights( GVertex& vert, int vertIdx )
{
	IPhyVertexExport* iPVertExport = 
			(IPhyVertexExport *)iPCtxExport->GetVertexInterface( vertIdx ); 
    if (!iPVertExport)
    {
        Msg( "Не могу извлечь интерфейс для экспорта весов вершины." );
		return false;
    }
    
	//  check vertex type (rigid/rigid blended)
    DWORD vtype;
	vtype = iPVertExport->GetVertexType();
    iPCtxExport->ReleaseVertexInterface( iPVertExport );    

    if (vtype == RIGID_TYPE)
    {
		IPhyRigidVertex* iPRigid = 
			(IPhyRigidVertex*) iPCtxExport->GetVertexInterface( vertIdx );
        if (!iPRigid)
        {
            Msg( "Не могу извлечь интерфейс для ригидной фершины." );
			return false;
        }
		
		//  only single bone
        INode* boneNode = iPRigid->GetNode();
		vert.nWeights = 1;
		HashedNode hbone( boneNode );

		int bIdx = boneHash.find( hbone );
		vert.boneRef[0] = bIdx;
		vert.weight[0] = 1.0f;
		assert( bIdx != NO_ELEMENT );

		haveBlendedVertices = true;
        iPCtxExport->ReleaseVertexInterface( iPRigid);
    }
    else if (vtype == RIGID_BLENDED_TYPE)
    {
        IPhyBlendedRigidVertex* iPBRigid = 
				(IPhyBlendedRigidVertex*) iPCtxExport->GetVertexInterface( vertIdx );
        if (!iPCtxExport)
        {
            Msg( "Не могу извлечь интерфейс для RIGID_BLENDED_TYPE вершины." );
			return false;
        }

        // fetch number of bones that influence this vertex
        int nBones = iPBRigid->GetNumberNodes();
		vert.nWeights = nBones;
		
        for (int cB = 0; cB < nBones; cB++)
        {
            INode* boneNode = iPBRigid->GetNode( cB );
			HashedNode hnode( boneNode );
            vert.weight[cB] = iPBRigid->GetWeight( cB );
			int bIdx = boneHash.find( hnode );
			vert.boneRef[cB] = bIdx;
			assert( bIdx != NO_ELEMENT );
        }

		haveBlendedVertices = true;
        iPCtxExport->ReleaseVertexInterface( iPBRigid );
    }
	else
	{
		//  incorrect vertex type
		assert( false );
	}
	return true;
} // maxExporter::FetchVertexWeights

bool maxExporter::InitNodePhysiqueContext( INode* node )
{
	Modifier* pPMod = FindModifier( node, Class_ID(PHYSIQUE_CLASS_ID_A, PHYSIQUE_CLASS_ID_B) );
	if (!pPMod) return false;

	IPhysiqueExport* iPExport = 
		(IPhysiqueExport*)pPMod->GetInterface( I_PHYINTERFACE );
    
	if (!iPExport) return false;
	iPCtxExport = (IPhyContextExport*) iPExport->GetContextInterface( node );
	if (!iPCtxExport) return false;

    // convert to rigid with blending
	iPCtxExport->ConvertToRigid	( TRUE );
	iPCtxExport->AllowBlending	( TRUE );
	return true;
} // maxExporter::InitNodePhyContext

/*---------------------------------------------------------------------------*/
/*	Func:	maxExporter::StartExporting
/*	Desc:	Starts exporting scene process 
/*---------------------------------------------------------------------------*/
void maxExporter::StartExporting()
{
	InitInternal();

	//  texture path stuff
	CreateTextureDir();
	Msg( "Начинаю экспортировать модель %s ...", modelFileName );
	//  count scene nodes etc
	Msg( "Предварительная обработка сцены..." );
	PreprocessScene();
	Msg( "Количество узлов в графе сцены: %d", nSceneNodes );
	
	curTime = 0;
	skeleton.reset();
	curParentBone = -1;

	ProcessNodeSubTree( iMax->GetRootNode() );
	if (abortProcess)
	{
		Msg( "Экспорт не был завершен." );
		return;
	}
	ReorderPolys();

	WriteC2M();

	SetProgress( 0.90 );

	Msg( "Полигонов: %d", nPoly );
	Msg( "Vертексов: %d", vertHash.numElem() );
	Msg( "Mатериалов: %d", mtlHash.numElem() );

	SetProgress( 0.95 );
	CopySceneTextures();
	SetProgress( 1.00 );

	Msg( "Экспортировали." );

	ShutInternal();
} // maxExporter::StartExporting





