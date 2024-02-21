/*****************************************************************************/
/*	File:	c2mExporter.cpp
/*	Desc:	
/*	Author:	Ruslan Shestopalyuk
/*	Date:	07.11.2002
/*****************************************************************************/
#pragma warning ( disable : 4786 ) 
#pragma warning ( disable : 4530 )

#include "resource.h"
#include <windows.h>
#include <commctrl.h>
#include <shellapi.h>
#include <shlobj.h>
#include "assert.h"
#include "c2mExporter.h"
#include <math.h>

/*****************************************************************************/
/*	BlockWriter implementation
/*****************************************************************************/
BlockWriter::BlockWriter( BlockWriter* _papa, const char* magic )
{
	if (!_papa->fp) return;
	fwrite( magic, 4, 1, _papa->fp );
	DWORD nul		= 0;
	filePos			= ftell( _papa->fp );
	fwrite( &nul, sizeof( DWORD ), 1, _papa->fp );
	papa			= _papa; 
	fp				= papa->fp;
	curBlockSize	= 0;
	papa->step( 8 );
} // BlockWriter::BlockWriter

BlockWriter::BlockWriter( FILE* file, const char* magic )
{
	if (!file) return;
	fwrite( magic, 4, 1, file );
	DWORD nul		= 0;
	filePos			= ftell( file );
	fwrite( &nul, sizeof( DWORD ), 1, file );
	papa			= 0;
	fp				= file;
	curBlockSize	= 0;
} // BlockWriter::BlockWriter

void BlockWriter::step( int nBytes )
{
	curBlockSize += nBytes;
	if (papa) papa->step( nBytes );
} // BlockWriter::step

void BlockWriter::end()
{
	if (!fp) return;
	long curFPos = ftell( fp );
	fseek( fp, filePos, SEEK_SET );
	DWORD size = curBlockSize;
	fwrite( &size, sizeof( DWORD ), 1, fp ); 
	fseek( fp, curFPos, SEEK_SET );
} // BlockWriter::end

/*****************************************************************************/
/*	c2mExporter implementation
/*****************************************************************************/
HINSTANCE	c2mExporter::hInstance			= 0;
HWND		c2mExporter::hPanelWnd			= 0;
HWND		c2mExporter::hLogWnd			= 0;	
HWND		c2mExporter::hProgressWnd		= 0;
HWND		c2mExporter::hGoBtn				= 0;
HWND		c2mExporter::hTexPathWnd		= 0;
HWND		c2mExporter::hChPathWnd			= 0;
HWND		c2mExporter::hCopyTexWnd		= 0;

HWND		c2mExporter::hKFWnd				= 0;
HWND		c2mExporter::hSampleWnd			= 0;
HWND		c2mExporter::hExportAnimWnd		= 0;
HWND		c2mExporter::hSrateWnd			= 0;
HWND		c2mExporter::hKPSWnd			= 0;
HWND		c2mExporter::hSrateCap			= 0;
HWND		c2mExporter::hCopyTCap			= 0;

HWND		c2mExporter::hAnGroup			= 0;
HWND		c2mExporter::hObjWnd			= 0;
HWND		c2mExporter::hNormWnd			= 0;
HWND		c2mExporter::hColWnd			= 0;
HWND		c2mExporter::hIgnoreSm			= 0;
HWND		c2mExporter::hSkinning			= 0;
HWND		c2mExporter::h2ndTexPair		= 0;
HWND		c2mExporter::hNoTex				= 0;
HWND		c2mExporter::hSuppressLog		= 0;

HWND		c2mExporter::hExportHeadCam		= 0;
HWND		c2mExporter::hExportSkeletal	= 0;
HWND		c2mExporter::hHierTransforms	= 0;

char		c2mExporter::folderPath[_MAX_PATH];
c2mExporter* c2mExporter::me;
bool		c2mExporter::exportDone;


c2mExporter::c2mExporter()
{
	//  default export settings
	bStaticVertexColors = false;
	bCopyTextures		= true;
	bKeyframe			= true;
	bExportAnimation	= true;
	bExportSkeletal		= false;
	bExportHeadCam		= false;
	bExportNormals		= true;
	bExportColors		= false;
	bIgnoreSmoothing	= false;
	bNoTex				= false;
	bSkinning			= false;
	b2ndTexPair			= false;
	bSuppressLog		= false;
	nKPS				= 2;
	nWeightsPerBone		= 0;
	nIndent				= 0;

	me = this;

	poly				= 0;
	reordVertArray		= 0;
} // c2mExporter::c2Exporter

void c2mExporter::OnOpenExportDlg()
{
	char dir	[_MAX_PATH];
	char drive	[_MAX_PATH];
	char file	[_MAX_PATH];
	char ext	[_MAX_PATH];
	
	_splitpath( modelFileName, drive, dir, file, ext );
	UpOneDirTreeLevel( dir );
	strcat( dir, "textures" );
	_makepath( texPath, drive, dir, 0, 0 );
	exportDone = false;
	haveBlendedVertices = false;
} // c2mExporter::OnOpenExportDlg

/*---------------------------------------------------------------------------*/
/*	Func:	c2mExporter::CopySceneTextures
/*	Desc:	Copying scene texture files to the textures directory 
/*---------------------------------------------------------------------------*/
void c2mExporter::CopySceneTextures()
{
	char newPath[_MAX_PATH];
	char drive	[_MAX_DRIVE];
	char dir	[_MAX_DIR];
	char fname	[_MAX_FNAME];
	char ext	[_MAX_EXT];

	if (!bCopyTextures) return;
	if (nTextures == 0)
	{
		Msg( "В сцене не использовались текстуры. Копировать нечего." );
	}

	Msg( "Копирую текстуры: %d шт...", nTextures );
	MsgIndent();
	for (int i = 0; i < nTextures; i++)
	{
		strcpy( newPath, texPath );
		_splitpath( texTable[i], drive, dir, fname, ext );
		strcat( newPath, fname );
		strcat( newPath, ext );
		Msg( "Исходный файл: %s", texTable[i]   );
		Msg( "Скопирован в:  %s", newPath		 );
		CopyFile( texTable[i], newPath, FALSE );
	}
	MsgUnindent();
} // c2mExporter::CopySceneTextures

/*---------------------------------------------------------------------------*/
/*	Func:	c2mExporter::OptionsDlgProc	
/*	Desc:	options dialog window procedure
/*	Parm:	
/*	Ret:	
/*	Remark:	
/*---------------------------------------------------------------------------*/
BOOL CALLBACK 
c2mExporter::OptionsDlgProc(  HWND hWnd, UINT message,
								WPARAM wParam, LPARAM lParam )
{
	switch(message) {
		case WM_INITDIALOG:
			hPanelWnd		= hWnd;
			hLogWnd			= GetDlgItem( hPanelWnd, IDC_LOG		); assert( hLogWnd		);
			hGoBtn			= GetDlgItem( hPanelWnd, IDEXPORT		); assert( hGoBtn		);
			hProgressWnd	= GetDlgItem( hPanelWnd, IDC_PROGRESS	); assert( hProgressWnd );
			hTexPathWnd		= GetDlgItem( hPanelWnd, IDCTEXPATH		); assert( hTexPathWnd	);
			hCopyTexWnd		= GetDlgItem( hPanelWnd, IDC_COPYTEX	); assert( hCopyTexWnd	);
			hChPathWnd		= GetDlgItem( hPanelWnd, IDC_TEXPATH	); assert( hChPathWnd	);
			hSuppressLog	= GetDlgItem( hPanelWnd, IDCSUPPRESSLOG	); assert( hSuppressLog	);
			
			//  export animation group box
			hKFWnd			= GetDlgItem( hPanelWnd, IDCKF			); assert( hKFWnd		);
			hSampleWnd		= GetDlgItem( hPanelWnd, IDCSAMPLING	); assert( hSampleWnd	);
			hExportAnimWnd	= GetDlgItem( hPanelWnd, IDCEXPORTANIM	); assert( hExportAnimWnd );
			hSrateWnd		= GetDlgItem( hPanelWnd, IDCSRATE		); assert( hSrateWnd	);
			hKPSWnd			= GetDlgItem( hPanelWnd, IDCKPS			); assert( hKPSWnd		);
			hSrateCap		= GetDlgItem( hPanelWnd, IDCSRATECAP	); assert( hSrateCap	);

			hAnGroup		= GetDlgItem( hPanelWnd, IDCANGROUP		); assert( hAnGroup		);
			hObjWnd			= GetDlgItem( hPanelWnd, IDCOBJECTS		); assert( hObjWnd		);
			hNormWnd		= GetDlgItem( hPanelWnd, IDCNORMALS		); assert( hNormWnd		);
			hColWnd			= GetDlgItem( hPanelWnd, IDCCOLORS		); assert( hColWnd		);  			

			hIgnoreSm		= GetDlgItem( hPanelWnd, IDCSMOOTH		); assert( hIgnoreSm	);
			hSkinning		= GetDlgItem( hPanelWnd, IDCSKINNING	); assert( hSkinning	);
			h2ndTexPair		= GetDlgItem( hPanelWnd, IDC2TEX		); assert( h2ndTexPair	);
			hNoTex			= GetDlgItem( hPanelWnd, IDCNOTEX		); assert( hNoTex		);
			hCopyTCap		= GetDlgItem( hPanelWnd, IDCCOPYTCAP	); assert( hCopyTCap	);

			hExportHeadCam	= GetDlgItem( hPanelWnd, IDCAMERAS		); assert( hExportHeadCam );
			hExportSkeletal	= GetDlgItem( hPanelWnd, IDCSKELETAL	); assert( hExportSkeletal );
			hHierTransforms	= GetDlgItem( hPanelWnd, IDCHIER		); assert( hHierTransforms );
			
			SendMessage( hProgressWnd, PBM_SETRANGE, 0, MAKELPARAM( 0, 100 ) );

			SetWindowText( hTexPathWnd, me->texPath );
			me->Options2Dialog();
			EnableControls();

			return TRUE;

		case WM_CLOSE:
			EndDialog(hWnd, 0);
			return TRUE;

		case WM_COMMAND:
		{
			switch (LOWORD( wParam ))
			{
				case IDEXPORT:
				//  start exporting process
				{
					if (!exportDone)
					{
						me->Dialog2Options();
						DisableControls(); 
						me->StartExporting(); 
					}
					else
					//  finish export phase
					{
						EndDialog( hWnd, 0 );
						return TRUE;	
					}
				}
				break;
				
				case IDC_COPYTEX:
				//  pushed checkbox "Do not copy textures"
				{
					me->Dialog2Options();
					EnableControls();
				}
				break;

				case IDC_TEXPATH: //  select directory to store textures
				{
					char* folder = BrowseForFolder();
					if (folder) 
					{
						strcpy( me->texPath, folder );
						SetWindowText( hTexPathWnd, me->texPath );
					}
				}
				case IDCCOLORS:   //  "Export colors" checkbox
				case IDCNORMALS:  //  "Export normals" checkbox
				case IDCSMOOTH:   //  "Ignore smoothing checkbox
				{
					me->Dialog2Options();
				}break;
				
				case IDCKF:
				case IDCSAMPLING:
				//  keyframing mode radio clicked
				{
					me->Dialog2Options();
					me->EnableAnimGroup( true );
				}

				case IDCEXPORTANIM:
				//  "Export animation" checkbox
				{
					me->Dialog2Options();
					me->EnableAnimGroup( me->bExportAnimation );
				}
				break;
			}
		}return TRUE;
	}
	return FALSE;
} // c2mExporter::OptionsDlgProc

/*---------------------------------------------------------------------------*/
/*	Func:	c2mExporter::Msg
/*	Desc:	Shows message in the log window
/*---------------------------------------------------------------------------*/
void c2mExporter::Msg( const char* format, ... )
{
	if (!hPanelWnd) return;
	
	char buffer[1024];
	buffer[0] = 0;
	for (int i = 0; i < nIndent; i++)
	{
		strcat( buffer, "    " );
	}

	va_list argList;
	va_start( argList, format );
	vsprintf( buffer + strlen( buffer ), format, argList );
	va_end( argList );

	SendMessage( hLogWnd, LB_INSERTSTRING, 0, (LPARAM)buffer );
	UpdateWindow( hLogWnd );
} // c2mExporter::Msg

void c2mExporter::MsgIndent()
{
	if (nIndent < c_MaxIndent) nIndent++;
} // c2mExporter::MsgIndent

void c2mExporter::MsgUnindent()
{
	if (nIndent > 0) nIndent--;
} // c2mExporter::MsgUnindent

void c2mExporter::DumpPolys( FILE* fp )
{
	fprintf( fp, "NVert = %d\n", vertHash.numElem() );
	fprintf( fp, "Dumping polygon list...\n" );
	for (int i = 0; i < nPoly; i++)
	{
		fprintf( fp, "%d: %d %d %d\n", i, poly[i].v1, poly[i].v2, poly[i].v3 );
		vertHash.elem( poly[i].v1 ).dump( fp );
		vertHash.elem( poly[i].v2 ).dump( fp );
		vertHash.elem( poly[i].v3 ).dump( fp );
	}
} // c2mExporter::DumpPolys

void c2mExporter::DumpAnimations( FILE* fp )
{
	fprintf( fp, "Name: %s; nChannels: %d; animTime: %d ms;\n", 
					anim.animName, anim.nChannels, anim.animTime );	
	for (int i = 0; i < anim.nChannels; i++)
	{
		GAnimChannel& cha = anim.channel[i];
		fprintf( fp, "Channel %d: boneIdx=%d; keyRate=%d KPS; animTime=%d ms; nFrames = %d;\n",
						i, cha.boneIdx, cha.keyRate, cha.animTime, cha.nFrames );
		for (int j = 0; j < cha.nFrames; j++)
		{
			Matr3x4& m = cha.transf[j];
			fprintf( fp, "\n" );
			fprintf( fp, "			%3.6f %3.6f %3.6f\n", m[0], m[1], m[2] );
			fprintf( fp, "			%3.6f %3.6f %3.6f\n", m[3], m[4], m[5] );
			fprintf( fp, "			%3.6f %3.6f %3.6f\n", m[6], m[7], m[8] );
			fprintf( fp, "			%3.6f %3.6f %3.6f\n", m[9], m[10], m[11] );
		}
	}
} // c2mExporter::DumpAnimations

void c2mExporter::SetVertexConfig()
{
	vertConfig.reset();
	vertConfig.setXYZ(); 
	if (bExportColors)	vertConfig.setDiffuse	();
	if (!bNoTex)		vertConfig.setUV		();
	if (bExportNormals)	vertConfig.setNormal	();
	if (nWeightsPerBone > 0) vertConfig.setNumBoneW( nWeightsPerBone );	
} // c2mExporter::SetVertexConfig

/*---------------------------------------------------------------------------*/
/*	Func:	c2mExporter::GetPolyClusterSize
/*	Desc:	Returns number of polygons with same attribute value as first has
/*---------------------------------------------------------------------------*/
int	c2mExporter::GetPolyClusterSize( int firstPoly, GSortAttribute attr )
{
	int num = 0;
	if (attr == gsaObject)
	//  we assume that cluster with the same object ID also 
	//  should have identical matIDs for every polygon
	{
		WORD objID = vertHash.elem( poly[firstPoly].v1 ).objID;
		WORD matID = vertHash.elem( poly[firstPoly].v1 ).matID;
		while (firstPoly < nPoly)
		{
			const GVertex& gv = vertHash.elem( poly[firstPoly].v1 );
			if (gv.objID != objID || gv.matID != matID) break;
			firstPoly++;
			num++;
		}		
		return num;
	}
	else if (attr == gsaMaterial)
	{
		WORD matID = vertHash.elem( poly[firstPoly].v1 ).matID;
		while (	firstPoly < nPoly && 
				vertHash.elem( poly[firstPoly++].v1 ).matID == matID) num++;
		return num;
	}
	else return 0;
} // c2mExporter::GetPolyClusterSize

/*---------------------------------------------------------------------------*/
/*	Func:	ReorderPolys
/*	Desc:	Does all neccessary reordering/sorting of polys and vertices
/*---------------------------------------------------------------------------*/
void c2mExporter::ReorderPolys()
{
	if (haveBlendedVertices && !bSkinning)
	{
		if (AskYesNo( "Некоторые вершины привязаны к костям, сделать им шкурничество?" ) == drYes) 
		{
			Msg( "Окей, шкурничество." );
			bSkinning = true;
			Options2Dialog();
		}
	}
	Msg( "Сортировка геометрии по материалу..." );
	//  sort polys by material
	//  if there are polys with no material, they will be last, with matID==65535 

	SortPoly( 0, nPoly, gsaMaterial );

	Msg( "Сортировка по объектам и индексам в пределах материалов..." );

	int nVert = vertHash.numElem();

	delete []reordVertArray;
	reordVertArray	= new GVertex	[nVert];
	WORD* reindex	= new WORD		[nVert];

	int cPoly = 0;
	int nMat = 0;
	int cVert = 0;
	while (cPoly < nPoly)
	{
		if (vertHash.elem( poly[cPoly].v1 ).matID == NO_MATERIAL)
		//  we reached 'no material' poly group, so finish the meat deal
		{
			break;
		}

		assert( nMat < mtlHash.numElem() );
		int nMtlVert = 0;
		int nMatPoly = GetPolyClusterSize( cPoly, gsaMaterial );
		//  sorting polys by objects inside given material
		SortPoly( cPoly, nMatPoly, gsaObject );
		GSubMesh submesh;
		
		int cObjPoly = 0;
		while (cObjPoly < nMatPoly)
		{
			int nObjPoly = GetPolyClusterSize( cPoly + cObjPoly, gsaObject );
			int objIdx = vertHash.elem( poly[cPoly + cObjPoly].v1 ).objID;
			
			//  pull out all object's vertices
			int nSubObjVert = 0;
			for (int i = 0; i < nVert; i++)
			{
				const GVertex& cv = vertHash.elem( i );
				if (cv.matID == nMat && cv.objID == objIdx)
				{
					reordVertArray[cVert + nSubObjVert] = cv;
					reindex[i] = nMtlVert + nSubObjVert;
					nSubObjVert++;
				}
			}
			//  reindex polys' vertex references to make them local in this material submesh
			int endPoly = cPoly + cObjPoly + nObjPoly;
			for (int i = cPoly + cObjPoly; i < endPoly; i++)
			{
				poly[i].v1 = reindex[poly[i].v1];
				poly[i].v2 = reindex[poly[i].v2];
				poly[i].v3 = reindex[poly[i].v3];
				checkPolySanity( poly[i] );
			}

			//  add meat chunk reference to the bone
			skeleton.bones[objIdx].addChunk( nMat, cObjPoly, nObjPoly, nMtlVert, nSubObjVert );
			//  add bone reference for the submesh part
			submesh.addChunk( objIdx, cObjPoly, nObjPoly, nMtlVert, nSubObjVert );

			nMtlVert	+= nSubObjVert;
			cVert		+= nSubObjVert;  

			//  sort polys with same material and of same object by index proximity
			SortPoly( cPoly + cObjPoly, nObjPoly, gsaIdxProximity ); 
			cObjPoly += nObjPoly;
		}
		skeleton.addSub( submesh );

		cPoly += nMatPoly;
		nMat++;
	}
	
	
	FILE* fd = fopen( "c:\\c2mExportDump.txt", "wt" );
	fprintf( fd, "Poly Dump:\n" ); 
	DumpPolys( fd );
	fprintf( fd, "\n\n\n\n\n\n\n\nAnimDump:\n" );
	DumpAnimations( fd );
	fclose( fd );

	delete []reindex;
} // c2mExporter::MasssagePolys


c2mExporter* g_Exp = 0;
int __cdecl cmpObject( const void *e1, const void *e2 )
{
	return GVertex::cmpObject( &(g_Exp->vertHash.elem( ((Poly*)e1)->v1 )),
								&(g_Exp->vertHash.elem( ((Poly*)e2)->v1 ) ));
}

int __cdecl cmpMaterial( const void *e1, const void *e2 )
{
	return GVertex::cmpMaterial( &(g_Exp->vertHash.elem( ((Poly*)e1)->v1 )),
								&(g_Exp->vertHash.elem( ((Poly*)e2)->v1 )) );
}

//  compare function for sorting by cache index proximity
int __cdecl cmpIdxProximity( const void *e1, const void *e2 )
{
	const Poly& p1 = *((Poly*)e1);
	const Poly& p2 = *((Poly*)e2);
	
	return (p1.v1 + p1.v2 + p1.v3) - (p2.v1 + p2.v2 + p2.v3);

} // cmpIdxProximity

/*---------------------------------------------------------------------------*/
/*	Func:	c2mExporter::SortPoly
/*	Desc:	Sorts poly array in given range, by given attribute:
/*				gsaObject		- by object polygon belongs to
/*				gsaMaterial		- by polygon material
/*---------------------------------------------------------------------------*/
void c2mExporter::SortPoly( int first, int num, GSortAttribute attr )
{
	g_Exp = this;
	if (attr == gsaObject)
	{
		qsort( poly + first, num, sizeof( Poly ), cmpObject );
	}
	else if (attr == gsaMaterial)
	{
		qsort( poly + first, num, sizeof( Poly ), cmpMaterial );	
	}
	else if (attr == gsaIdxProximity)
	{
		qsort( poly + first, num, sizeof( Poly ), cmpIdxProximity );	
	}
} // c2mExporter::SortPoly

/*---------------------------------------------------------------------------*/
/*	Func:	c2mExporter::ShowExportDialog	
/*	Desc:	Brings export options dialog onto the screen
/*---------------------------------------------------------------------------*/
void c2mExporter::ShowExportDialog()
{
	HWND parent = GetActiveWindow();
	DialogBox(	hInstance, 
				MAKEINTRESOURCE(IDD_PAN), 
				parent, 
				OptionsDlgProc );
	int res = GetLastError();
} // c2mExporter::ShowExportDialog

void c2mExporter::SetProgress( double progrVal )
{
	progress = progrVal;
	if (!hProgressWnd) return;
	int pos = (int)(progress * 100.0f);
	SendMessage( hProgressWnd, PBM_SETPOS, (WPARAM)pos, 0 );
} // c2mExporter::SetProgressPos

void c2mExporter::StepProgressNode()
{
	progress += progressPerNode;
	if (!hProgressWnd) return;
	int pos = (int)(progress * 100.0f);
	SendMessage( hProgressWnd, PBM_SETPOS, (WPARAM)pos, 0 );
}

bool c2mExporter::checkPolySanity( const Poly& p )
{
	int nV = vertHash.numElem();
	bool sane = (p.v1 < nV && p.v2 < nV && p.v3 < nV);
	assert( sane );
	return sane;
}

/*---------------------------------------------------------------------------*/
/*	Func:	c2mExporter::EnableAnimGroup	
/*	Desc:	
/*---------------------------------------------------------------------------*/
void c2mExporter::EnableAnimGroup( bool enable ) 
{
	if (!enable)
	{
		EnableWindow( hKFWnd,			FALSE );
		EnableWindow( hSampleWnd,		FALSE );
		EnableWindow( hSrateWnd,		FALSE );
		EnableWindow( hKPSWnd,			FALSE );
		EnableWindow( hSrateCap,		FALSE );
		EnableWindow( hExportSkeletal,	FALSE );
		EnableWindow( hHierTransforms,	FALSE );
		EnableWindow( hExportHeadCam,	FALSE );
	}
	else
	{
		EnableWindow( hKFWnd,			TRUE );
		EnableWindow( hSampleWnd,		TRUE );
		EnableWindow( hExportSkeletal,	TRUE );
		EnableWindow( hHierTransforms,	TRUE );
		EnableWindow( hExportHeadCam,	TRUE );

		if (bKeyframe)
		{
			SendMessage( hKFWnd,		BM_SETCHECK, (WPARAM)BST_CHECKED,	0 ); 
			SendMessage( hSampleWnd,	BM_SETCHECK, (WPARAM)BST_UNCHECKED, 0 ); 
			
			EnableWindow( hSrateWnd,		FALSE );
			EnableWindow( hKPSWnd,			FALSE );
			EnableWindow( hSrateCap,		FALSE );
			
			SetWindowText( hSrateWnd, 0 );
		}
		else
		{
			SendMessage( hKFWnd,		BM_SETCHECK, (WPARAM)BST_UNCHECKED,	0 ); 
			SendMessage( hSampleWnd,	BM_SETCHECK, (WPARAM)BST_CHECKED,   0 ); 
			
			EnableWindow( hSrateWnd,		TRUE );
			EnableWindow( hKPSWnd,			TRUE );
			EnableWindow( hSrateCap,		TRUE );
			
			char sKPS[64];
			sprintf( sKPS, "%d", nKPS );
			SetWindowText( hSrateWnd, sKPS );
		}
	}
} // c2mExporter::EnableAnimGroup

bool c2mExporter::IsChecked( HWND hWnd )
{
	return (SendMessage( hWnd, BM_GETCHECK, 0, 0 ) == BST_CHECKED);
} // c2mExporter::IsChecked

void c2mExporter::SetCheck( HWND hWnd, bool checked )
{
	SendMessage(	hWnd, 
					BM_SETCHECK, 
					checked ? (WPARAM)BST_CHECKED : (WPARAM)BST_UNCHECKED, 
					0 );
} // c2mExporter::SetCheck

bool c2mExporter::GetInt( HWND hWnd, int& val )
{
	const int c_Maxlen = 512;
	char str[c_Maxlen];
	GetWindowText( hWnd, str, c_Maxlen );
	if (sscanf( str, "%d", &val ) < 1) return false;
	return true;
} // c2mExporter::GetInt

/*---------------------------------------------------------------------------*/
/*	Func:	c2mExporter::Dialog2Options	
/*	Desc:	Gets export options from dialog window
/*---------------------------------------------------------------------------*/
void c2mExporter::Dialog2Options()
{
	bExportAnimation	= IsChecked( hExportAnimWnd );
	bCopyTextures		= !IsChecked( hCopyTexWnd	);	
	bKeyframe			= IsChecked( hKFWnd			);
	bExportNormals		= IsChecked( hNormWnd		);
	bExportColors		= IsChecked( hColWnd		);
	bIgnoreSmoothing	= IsChecked( hIgnoreSm		);
	bSkinning			= IsChecked( hSkinning		);
	b2ndTexPair			= IsChecked( h2ndTexPair	);
	bNoTex				= IsChecked( hNoTex			);
	bSuppressLog		= IsChecked( hSuppressLog	);
	bExportHeadCam		= IsChecked( hExportHeadCam );
	bExportSkeletal		= IsChecked( hExportSkeletal );
	bHierTransforms		= IsChecked( hHierTransforms );

	int kps = 0;
	if (GetInt( hSrateWnd, kps )) nKPS = kps;
} // c2mExporter::Dialog2Options

/*---------------------------------------------------------------------------*/
/*	Func:	c2mExporter::Options2Dialog
/*	Desc:	Mirrors current export options into dialog window
/*---------------------------------------------------------------------------*/
void c2mExporter::Options2Dialog()
{
	SetCheck( hExportAnimWnd,	bExportAnimation );
	SetCheck( hCopyTexWnd,		!bCopyTextures	 ); 
	SetCheck( hKFWnd,			bKeyframe		 ); 
	SetCheck( hNormWnd,			bExportNormals	 ); 
	SetCheck( hColWnd,			bExportColors	 ); 
	SetCheck( hIgnoreSm,		bIgnoreSmoothing ); 
	SetCheck( hSkinning,		bSkinning		 ); 
	SetCheck( h2ndTexPair,		b2ndTexPair		 ); 
	SetCheck( hNoTex,			bNoTex			 ); 
	SetCheck( hSuppressLog,		bSuppressLog	 ); 
	SetCheck( hExportHeadCam,	bExportHeadCam	 ); 
	SetCheck( hExportSkeletal,	bExportSkeletal	 ); 
	SetCheck( hHierTransforms,	bHierTransforms	 ); 
} // c2mExporter::Options2Dialog

/*---------------------------------------------------------------------------*/
/*	Func:	c2mExporter::DisableControls	
/*	Desc:	
/*---------------------------------------------------------------------------*/
void c2mExporter::DisableControls()
{
	EnableWindow( hGoBtn,			FALSE );
	EnableWindow( hTexPathWnd,		FALSE );
	EnableWindow( hChPathWnd,		FALSE );
	EnableWindow( hCopyTexWnd,		FALSE );

	EnableWindow( hCopyTCap,		FALSE );

	EnableWindow( hAnGroup,			FALSE );
	EnableWindow( hObjWnd,			FALSE );
	EnableWindow( hNormWnd,			FALSE );
	EnableWindow( hColWnd,			FALSE );
	EnableWindow( hIgnoreSm,		FALSE );
	EnableWindow( hSkinning,		FALSE );
	EnableWindow( h2ndTexPair,		FALSE );
	EnableWindow( hNoTex,			FALSE );
	EnableWindow( hSuppressLog,		FALSE );

	EnableWindow( hExportHeadCam,	FALSE );
	EnableWindow( hExportSkeletal,	FALSE );
	EnableWindow( hHierTransforms,	FALSE );

	EnableWindow( hExportAnimWnd,	FALSE );
	me->EnableAnimGroup( false );
} // c2mExporter::DisableControls

/*---------------------------------------------------------------------------*/
/*	Func:	c2mExporter::EnableControls	
/*	Desc:	
/*---------------------------------------------------------------------------*/
void c2mExporter::EnableControls()
{
	EnableWindow( hGoBtn,			TRUE );
	EnableWindow( hCopyTCap,		TRUE );

	if (me->bCopyTextures)
	{
		EnableWindow( hTexPathWnd,	TRUE );
		EnableWindow( hChPathWnd,	TRUE );
	}
	else
	{
		EnableWindow( hTexPathWnd,	FALSE );
		EnableWindow( hChPathWnd,	FALSE );
	}

	EnableWindow( hCopyTexWnd,		TRUE );	

	EnableWindow( hAnGroup,			TRUE );
	EnableWindow( hObjWnd,			TRUE );
	EnableWindow( hNormWnd,			TRUE );
	EnableWindow( hColWnd,			TRUE );
	EnableWindow( hIgnoreSm,		TRUE );
	EnableWindow( hSkinning,		TRUE );
	EnableWindow( h2ndTexPair,		TRUE );
	EnableWindow( hNoTex,			TRUE );
	EnableWindow( hSuppressLog,		TRUE );

	EnableWindow( hExportAnimWnd,	TRUE );

	me->EnableAnimGroup( me->bExportAnimation );
} // c2mExporter::EnableControls

DialogResult c2mExporter::AskYesNo( const char* msg )
{
	if (MessageBox( hPanelWnd, msg, "c2mExport", MB_YESNO ) == IDYES)
	{
		return drYes;
	}
	else 
	{
		return drNo;
	}
} // c2mExporter::AskYesNo	

/*---------------------------------------------------------------------------*/
/*	Func:	c2mExporter::CreateTextureDir
/*	Desc:	Does stuff related to the texture directory creation
/*---------------------------------------------------------------------------*/
void c2mExporter::CreateTextureDir()
{
	//  check if texture path directory exists
	//  if not - create it
	if (bCopyTextures && _chdir( texPath ) == -1)
	{
		char askStr[512];
		sprintf( askStr, 
				"Директория %s не существует. Создать, что ли?..", 
				texPath );
		int res = MessageBox( 0, askStr, "cllExport", 
								MB_YESNOCANCEL | MB_ICONWARNING | MB_DEFBUTTON1 );
		if (res == IDYES)
		{
			if (_mkdir( texPath ) == 0)
			{
				Msg( "Создал директорию %s", texPath );
			}
			else
			{
				Msg( "Не могу создать директорию для текстур: %s", texPath );
				Msg( "Текстуры не будут скопированы." );
				bCopyTextures = false;	
			}
		}
		if (res == IDNO)
		{
			bCopyTextures = false;
			Msg( "Директория %s не была создана. Текстуры копироваться не будут.", 
					texPath );
			bCopyTextures = false;	
		}
		if (res == IDCANCEL) 
		{
			EnableControls();
			return;
		}
	}
} // c2mExporter::CreateTextureDir

void c2mExporter::ClearTextureTable()
{
	nTextures = 0;
} // c2mExporter::ClearTextureTable

int	c2mExporter::GetTexID( const char* texFile )
{
	for (int i = 0; i < nTextures; i++)
	{
		if (!strcmp( texTable[i], texFile )) return i;
	}
	if (nTextures == c_MaxSceneTextures) return -1;
	strcpy( texTable[nTextures++], texFile );
	return nTextures - 1;
} // c2mExporter::GetTexID

/*---------------------------------------------------------------------------*/
/*	Func:	c2mExporter::AddPolygon	
/*---------------------------------------------------------------------------*/
int	c2mExporter::AddPolygon( int v1, int v2, int v3 )
{
	if (nPoly >= maxPoly)
	//  grow polygon pool
	{
		Poly* npoly = new Poly[maxPoly + maxPoly / 2];
		memcpy( npoly, poly, maxPoly * sizeof( Poly ) );
		maxPoly += maxPoly / 2;
		delete []poly;
		poly = npoly;
	}
	poly[nPoly].v1 = v1;
	poly[nPoly].v2 = v2;
	poly[nPoly].v3 = v3;
	nPoly++;
	return 0;
} // c2mExporter::AddPolygon

/*---------------------------------------------------------------------------*/
/*	Func:	c2mExporter::WriteMesh
/*	Desc:	Saves model mesh sorted in submeshes by material
/*---------------------------------------------------------------------------*/
int c2mExporter::WriteMesh( FILE* fp )
{
	int nMtl  = mtlHash.numElem();
	int cVert = 0;
	int cPoly = 0;

	BlockWriter	meshBW( fp, "MESH" );
	meshBW.step( WriteInt( fp, nMtl ) );
	meshBW.step( WriteFloat( fp, (float)sqrt( radius2 ) ) );
	for (int cMat = 0; cMat < nMtl; cMat++)
	{
		BlockWriter submBW( &meshBW, "SUBM" );
		GMaterial& mtl = mtlHash.elem( cMat );
		submBW.step( WriteMaterial( fp, mtl ) );

		GSubMesh& sub = skeleton.getSub( cMat );
		int nV = sub.getNVert();
		int maxWeights = 0;
		if (bSkinning)
		{
			for (int i = 0; i < nV; i++)
			{
				GVertex& gv = reordVertArray[i];
				if (gv.nWeights > maxWeights) maxWeights = gv.nWeights;
			}
			vertConfig.setNumBoneW( maxWeights );
		}
		
		submBW.step( vertConfig.Write( fp ) );

		int nP = sub.getNPolys();
		submBW.step( WriteInt( fp, nP * 3 ) );			
		submBW.step( WriteInt( fp, nV ) );
		
		//  writing vertex buffer
		for (int i = 0; i < nV; i++)
		{
			GVertex& gv = reordVertArray[cVert];
			submBW.step( WriteVertex( fp, gv, maxWeights ) );
			cVert++;
		}
		
		//  now index buffer
		for (int i = 0; i < nP; i++)
		{
			const Poly& p = poly[cPoly];
			submBW.step( WritePoly( fp, p.v1, p.v2, p.v3 ) );
			cPoly++;
		}

		submBW.end();
	}
	meshBW.end();

	FILE* fdump = fopen( "c:\\dumps\\wdump.txt", "wt" );
	vertHash.dump( fdump );
	fclose( fdump );
	return meshBW.fullSize();
} // c2mExporter::WriteMesh

int	c2mExporter::WriteTextureTable( FILE* fp )
{
	//  texture table
	BlockWriter texBW( fp, "TTBL" );
	texBW.step( WriteInt( fp, nTextures ) );
	char dir[_MAX_PATH];
	char tname[_MAX_PATH];
	char path[_MAX_PATH];
	char ext[_MAX_PATH];

	for (int i = 0; i < nTextures; i++)
	{
		_splitpath( texTable[i], dir, path, tname, ext );
		strcat( tname, ext );
		texBW.step( WriteString( fp, tname ) );
	}
	texBW.end();
	return texBW.fullSize();
} // c2mExporter::WriteTextureTable

/*---------------------------------------------------------------------------*/
/*	Func:	c2mExporter::InitInternal
/*	Desc:	Should be neccesarily called when start exporting
/*---------------------------------------------------------------------------*/
void c2mExporter::InitInternal()
{
	delete []poly;
	poly	= new Poly[c_MinPolyNum];
	maxPoly = c_MinPolyNum;
	nPoly	= 0;
	reordVertArray = 0;

	anim.reset();

	mtlHash.reset();
	vertHash.reset();
	SetVertexConfig();

	SetWindowText( hGoBtn, "Export to c2m format..." );
	exportDone = false;
	abortProcess = false;

	radius2 = 0.0f;

} // c2mExporter::InitInternal

/*---------------------------------------------------------------------------*/
/*	Func:	c2mExporter::ShutInternal
/*	Desc:	Should be neccesarily called when end exporting
/*---------------------------------------------------------------------------*/
void c2mExporter::ShutInternal()
{
	delete []poly;
	poly = 0;
	delete []reordVertArray;
	reordVertArray = 0;
	SetWindowText( hGoBtn, "OK" );
	EnableWindow( hGoBtn, TRUE );
	UpdateWindow( hPanelWnd );
	exportDone = true;
} // c2mExporter::ShutInternal

int	c2mExporter::WriteString( FILE* fp, const char* str )
{
	int len = strlen( str ) + 1;
	WriteWord( fp, len );
	fwrite( str, len, 1,  fp );  
	return len + 2; 
} // c2mExporter::WriteString

int c2mExporter::WriteC2M()
{
	FILE* fp = fopen( modelFileName, "wb" );
	if (!fp)
	{
		Msg( "ERROR: Не могу создать файл %s. Кина не будет.", modelFileName );
		return 0;
	}

	BlockWriter c2mBW( fp, "C2MD" );
	c2mBW.step( WriteSkeleton		( fp, skeleton	) );
	c2mBW.step( WriteMesh			( fp			) );
	c2mBW.step( WriteAnimation		( fp, anim		) );
	c2mBW.step( WriteTextureTable	( fp			) );
	c2mBW.end();
	fclose( fp );
	return c2mBW.fullSize();
} // c2mExporter::WriteC2M

int	c2mExporter::WriteInt( FILE* fp, int val )
{
	fwrite( &val, sizeof( int ), 1, fp );
	return sizeof( int );
} // c2mExporter::WriteInt

int	c2mExporter::WriteFloat( FILE* fp, float val )
{
	fwrite( &val, sizeof( float ), 1, fp );
	return sizeof( float );
} // c2mExporter::WriteInt

int	c2mExporter::WriteWord( FILE* fp, WORD val )
{
	fwrite( &val, sizeof( WORD ), 1, fp );
	return sizeof( WORD );
} // c2mExporter::WriteWord

void c2mExporter::UpOneDirTreeLevel( char* dir )
{
} // c2mExporter::UpOneDirTreeLevel

int	c2mExporter::WriteMaterial( FILE* fp, const GMaterial& mtl )
{
	int nW = 0;
	nW += WriteWord( fp, mtl.getTexture( 0 ) );
	nW += WriteWord( fp, mtl.getTexture( 1 ) );
	return nW; 
} // c2mExporter::WriteMaterial

int	c2mExporter::WriteVertex( FILE* fp, const GVertex& vert, int nWeights )
{
	//  vertex coords are written in any case
	int nW = sizeof( float ) * 3;
	fwrite( &vert.x, nW, 1, fp ); // little lazy hack to write 3 at once
	
	//  blending weights go here
	if (bSkinning)
	{
		for (int i = 0; i < nWeights - 1; i++)
		{
			fwrite( &vert.weight[i], sizeof( vert.weight[i] ), 1, fp );
			nW += sizeof( vert.weight[i] );
		}
		BYTE boneInd[] = { 0, 0, 0, 0 };
		boneInd[0] = vert.boneRef[0];
		boneInd[1] = vert.boneRef[1];
		boneInd[2] = vert.boneRef[2];
		boneInd[3] = vert.boneRef[3];
		fwrite( boneInd, 4, 1, fp );
		nW += 4;
	}

	//  normal
	if (bExportNormals)
	{
		fwrite( &vert.nx, sizeof( float ) * 3, 1, fp );
		nW += sizeof( float ) * 3;
	}

	//  vertex color
	if (bExportColors)
	{
		fwrite( &vert.color, sizeof( DWORD ), 1, fp );
		nW += sizeof( DWORD );
	}

	//  texture coordinates
	if (!bNoTex)
	{	
		fwrite( &vert.u, 2 * sizeof( float ), 1, fp ); 
		nW += 2 * sizeof( float );
		if (b2ndTexPair)
		{
			float uv[2]; uv[0] = 0.0f; uv[1] = 0.0f;
			fwrite( &uv, 2 * sizeof( float ), 1, fp ); 
			nW += 2 * sizeof( float );
		}
	}
	
	assert( nW == vertConfig.getSizeBytes() );
	return nW;	
} // c2mExporter::WriteVertex

int	c2mExporter::WritePoly( FILE* fp, IDXTYPE v1, IDXTYPE v2, IDXTYPE v3 )
{
	fwrite( &v1, sizeof( IDXTYPE ), 1, fp );
	fwrite( &v2, sizeof( IDXTYPE ), 1, fp );
	fwrite( &v3, sizeof( IDXTYPE ), 1, fp );
	return 3 * sizeof( IDXTYPE );
}

int c2mExporter::WriteBone( FILE* fp, const GBone& bone )
{
	int nW = 0;
	nW += WriteString( fp, bone.name );
	nW += WriteInt( fp, bone.papaIdx );
	
	//  write bounding box
	nW += WriteFloat( fp, bone.minX );
	nW += WriteFloat( fp, bone.minY );
	nW += WriteFloat( fp, bone.minZ );
	nW += WriteFloat( fp, bone.maxX );
	nW += WriteFloat( fp, bone.maxY );
	nW += WriteFloat( fp, bone.maxZ );

	nW += WriteWord( fp, bone.nChunks );
	for (int i = 0; i < bone.nChunks; i++)
	{
		nW += WriteWord( fp, bone.chunks[i].subIdx		);
		nW += WriteInt ( fp, bone.chunks[i].firstPoly	);
		nW += WriteInt ( fp, bone.chunks[i].numPoly		);
		nW += WriteInt ( fp, bone.chunks[i].firstVert	);
		nW += WriteInt ( fp, bone.chunks[i].numVert		);
	}
	return nW;
} // c2mExporter::WriteBone

int	c2mExporter::WriteSkeleton( FILE* fp, GSkeleton& skel )
{
	BlockWriter skelBW( fp, "SKEL" );
	int nW = 0;
	nW += WriteInt( fp, skel.nBones );
	WORD skelFlags = bHierTransforms ? 1 : 0;
	nW += WriteWord( fp, skelFlags );
	for (int i = 0; i < skel.nBones; i++)
	{
		nW += WriteBone( fp, skel.bones[i] );
	}
	nW += WriteInt( fp, skel.getNSubs() );
	for (int i = 0; i < skel.getNSubs(); i++)
	{
		GSubMesh& sub = skel.getSub( i );
		nW += WriteWord	( fp, sub.nChunks	);
		nW += WriteInt	( fp, mtlHash.elem( i ).flags() );
		for (int j = 0; j < sub.nChunks; j++)
		{
			nW += WriteWord	( fp, sub.chunks[j].subIdx		);
			nW += WriteInt	( fp, sub.chunks[j].firstPoly	);
			nW += WriteInt	( fp, sub.chunks[j].numPoly		);
			nW += WriteInt	( fp, sub.chunks[j].firstVert	);
			nW += WriteInt	( fp, sub.chunks[j].numVert		);
		}
		if (mtlHash.elem( i ).getFlag( mfAnimated ))
		{
			nW += WriteAnimChannel( fp, mtlHash.elem( i ).getAnimChannel() ); 
		}
	}
	skelBW.step( nW );
	skelBW.end();
	return nW;
} // c2mExporter::WriteSkeleton

int	c2mExporter::WriteAnimation( FILE* fp, GAnimation& anm )
{
	Msg( "Пишу анимацию: %s", anm.animName );
	BlockWriter animBW( fp, "ANIM" );
	animBW.step( WriteInt( fp, anm.nChannels ) );
	for (int i = 0; i < anm.nChannels; i++)
	{
		BlockWriter chBW( &animBW, "ANCH" );
		chBW.step( WriteAnimChannel( fp, anm.channel[i] ) ); 
		chBW.end();
	}
	animBW.end();
	return animBW.fullSize();
} // c2mExporter::WriteAnimation

int	c2mExporter::WriteAnimChannel( FILE* fp, GAnimChannel& cha )
{
	int nW = 0;
	nW += WriteInt( fp, cha.animTime );
	nW += WriteInt( fp, cha.nFrames  );
	nW += WriteInt( fp, cha.keyRate  );
	nW += WriteInt( fp, cha.boneIdx  );
	for (int i = 0; i < cha.nFrames; i++)
	{
		fwrite( cha.transf[i], sizeof( Matr3x4 ), 1, fp );
	}
	nW += cha.nFrames * sizeof( Matr3x4 );
	return nW;
} // c2mExporter::WriteAnimChannel

/*---------------------------------------------------------------------------*/
/*	Func:	c2mExporter::BrowseForFolder	
/*	Desc:	Shows "browse for folder" dialog
/*	Ret:	folder path, NULL if user canceled
/*---------------------------------------------------------------------------*/
char* c2mExporter::BrowseForFolder()
{
	BROWSEINFO				binfo;
	char					dname[_MAX_PATH];
	ITEMIDLIST*				pIDList;
	IMalloc*				pMalloc;
	
	const char* title		= "Select textures directory";

	binfo.hwndOwner			= hPanelWnd;
	binfo.pidlRoot			= NULL;
	binfo.pszDisplayName	= dname;
	binfo.lpszTitle			= title;
	binfo.ulFlags			= BIF_RETURNONLYFSDIRS | BIF_RETURNFSANCESTORS | 0x0040;
	binfo.lpfn				= NULL;
	binfo.lParam			= 0;
	binfo.iImage			= 0;

	pIDList = SHBrowseForFolder(&binfo);

	if (pIDList)
	{
		SHGetPathFromIDList( pIDList, folderPath );
		if (strlen( folderPath ) == 3) folderPath[2] = 0;
		if (SHGetMalloc( &pMalloc ) == NOERROR)
		{
			pMalloc->Free( pIDList );
		}
	}
	else return 0;
	return folderPath;
} // InstApp::onBrowsePath