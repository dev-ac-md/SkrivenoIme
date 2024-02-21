/*****************************************************************************/
/*	File:	MaxExport.h
/*	Desc:	
/*	Author:	Ruslan Shestopalyuk
/*	Date:	07.11.2002
/*****************************************************************************/
#ifndef __MAXEXPORT__H
#define __MAXEXPORT__H

#include "Max.h"
#include "resource.h"
#include "istdplug.h"
#include "iparamb2.h"
#include "iparamm2.h"
#include "phyexp.h"

extern TCHAR *GetString(int id);

#define C2MEXPORTER_CLASS_ID	Class_ID(0x2a71a045, 0x340d6436)
const int c_ProgressRange	= 100;
const int c_MaxNodeSubMtls	= 128;

/*****************************************************************************/
/*	Class:	HashedNode
/*	Desc:	hashed INode pointer 
/*****************************************************************************/
class HashedNode
{
	INode*		node;
public:
	HashedNode() : node(0) {}
	HashedNode( INode* val ) : node(val) {}
	
	unsigned int hash() const
	{
		unsigned char* s = (unsigned char*) this;
		unsigned int h = 0;
		int nBytes = sizeof( *this );
		for (int i = 0; i < nBytes; i++)
		{
			h = (h * 729 + (*s) * 37) ^ (h >> 1);
			s++;
		}
		return h;
	} // hash

	bool equal( const HashedNode& el )
	{
		return node == el.node;
	} // equal

	void copy( const HashedNode& el )
	{
		node = el.node;
	} // copy
}; // class HashedNode

/*****************************************************************************/
/*	Class:	maxExporter
/*	Desc:	Main exporter class	
/*****************************************************************************/
class maxExporter : public SceneExport, public c2mExporter 
{
public:
	static HWND hParams;

	int				ExtCount();					// Number of extensions supported
	const TCHAR *	Ext(int n);					// Extension #n (i.e. "3DS")
	const TCHAR *	LongDesc();					// Long ASCII description (i.e. "Autodesk 3D Studio File")
	const TCHAR *	ShortDesc();				// Short ASCII description (i.e. "3D Studio")
	const TCHAR *	AuthorName();				// ASCII Author name
	const TCHAR *	CopyrightMessage();			// ASCII Copyright message
	const TCHAR *	OtherMessage1();			// Other message #1
	const TCHAR *	OtherMessage2();			// Other message #2
	unsigned int	Version();					// Version number * 100 (i.e. v3.01 = 301)
	void			ShowAbout(HWND hWnd);		// Show DLL's "About..." box

	BOOL SupportsOptions(int ext, DWORD options);
	int				DoExport(const TCHAR *name,ExpInterface *ei,Interface *i, BOOL suppressPrompts=FALSE, DWORD options=0);

	maxExporter();
	~maxExporter();		

	void StartExporting();

protected:
	
	Interface*				iMax;
	ExpInterface*			iExport;
	TimeValue				curTime;

	int						subMtl[c_MaxNodeSubMtls];
	int						nSubMtls;
	bool					abortProcess;

	void ProcessNode		( INode* node );
	void ProcessNodeSubTree	( INode* node );
	void ProcessNodeMtl		( INode* node );
	int  ProcessStdMtl		( Mtl* mtl );
	
	//  skinning
	bool FetchVertexWeights ( GVertex& vert, int vertIdx );
	bool InitNodePhysiqueContext( INode* node );
	bool FetchAnimationTracks( INode* node, int curBoneIdx );

	IPhyContextExport*		iPCtxExport;

	void					PreprocessScene	();
	void					PreTraverseSubTree( INode* node );
	Modifier*				FindModifier( INode* node, Class_ID modID );

	Hash<HashedNode>		boneHash;
}; // class maxExporter

/*****************************************************************************/
/*	Class:	maxExporterClassDesc
/*	Desc:	3ds Max exporter class glue
/*****************************************************************************/
class maxExporterClassDesc : public ClassDesc2 
{
public:
	int 			IsPublic() { return TRUE; }
	void *			Create(BOOL loading = FALSE) { return new maxExporter(); }
	const TCHAR *	ClassName() { return GetString(IDS_CLASS_NAME); }
	SClass_ID		SuperClassID() { return SCENE_EXPORT_CLASS_ID; }
	Class_ID		ClassID() { return C2MEXPORTER_CLASS_ID; }
	const TCHAR* 	Category() { return GetString(IDS_CATEGORY); }

	const TCHAR*	InternalName() { return _T("maxExporter"); }	// returns fixed parsable name (scripter-visible name)
	HINSTANCE		HInstance() { return c2mExporter::hInstance; }	// returns owning module handle
}; // class maxExporterClassDesc 

#endif // __MAXEXPORT__H
