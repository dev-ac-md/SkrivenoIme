#pragma warning ( disable : 4786 ) 
#pragma warning ( disable : 4530 )

#include "resource.h"
#include <windows.h>
#include <commctrl.h>
#include <shellapi.h>
#include <shlobj.h>

#include "ExportDialog.h"

#include "stdio.h"

#include <string>
using namespace std;

HWND		hPanelWnd		= 0;
HWND		hLogWnd			= 0;	
HWND		hProgressWnd	= 0;
HWND		hGoBtn			= 0;
HWND		hTexPathWnd		= 0;
HWND		hChPathWnd		= 0;
HWND		hCopyTexWnd		= 0;

HWND		hKFWnd			= 0;
HWND		hSampleWnd		= 0;
HWND		hExportAnimWnd	= 0;
HWND		hSrateWnd		= 0;
HWND		hKPSWnd			= 0;
HWND		hSrateCap		= 0;
HWND		hCopyTCap		= 0;

HWND		hAnGroup		= 0;
HWND		hObjWnd			= 0;
HWND		hNormWnd		= 0;
HWND		hColWnd			= 0;
HWND		hIgnoreSm		= 0;

HWND		hExportHeadCam	= 0;
HWND		hExportSkeletal	= 0;


IExporter*		Export = 0;
string			texPath;

extern bool bCopyTextures;
extern bool	bKeyframe;
extern bool	bExportAnimation;
extern bool	bExportColors;
extern bool	bExportNormals;
extern bool	bIgnoreSmoothing;
extern bool	bExportHeadCam;
extern bool	bExportSkeletal;
extern HINSTANCE hInstance;

extern int  nKPS;


void EnableAnimGroup( bool enable );


BOOL CALLBACK dlgProc(  HWND hWnd,
						UINT message,
						WPARAM wParam,
						LPARAM lParam )
{
	switch(message) {
		case WM_INITDIALOG:
			hPanelWnd		= hWnd;
			hLogWnd			= GetDlgItem( hPanelWnd, IDC_LOG		);
			hGoBtn			= GetDlgItem( hPanelWnd, IDEXPORT		);
			hProgressWnd	= GetDlgItem( hPanelWnd, IDC_PROGRESS	);
			hTexPathWnd		= GetDlgItem( hPanelWnd, IDCTEXPATH		);
			hCopyTexWnd		= GetDlgItem( hPanelWnd, IDC_COPYTEX	);
			hChPathWnd		= GetDlgItem( hPanelWnd, IDC_TEXPATH	);

			//  export animation group box
			hKFWnd			= GetDlgItem( hPanelWnd, IDCKF			);
			hSampleWnd		= GetDlgItem( hPanelWnd, IDCSAMPLING	);
			hExportAnimWnd	= GetDlgItem( hPanelWnd, IDCEXPORTANIM	);
			hSrateWnd		= GetDlgItem( hPanelWnd, IDCSRATE		);
			hKPSWnd			= GetDlgItem( hPanelWnd, IDCKPS			);
			hSrateCap		= GetDlgItem( hPanelWnd, IDCSRATECAP	);

			hAnGroup		= GetDlgItem( hPanelWnd, IDCANGROUP		);
			hObjWnd			= GetDlgItem( hPanelWnd, IDCOBJECTS		);
			hNormWnd		= GetDlgItem( hPanelWnd, IDCNORMALS		);
			hColWnd			= GetDlgItem( hPanelWnd, IDCCOLORS		);  			

			hIgnoreSm		= GetDlgItem( hPanelWnd, IDCSMOOTH		);
			hCopyTCap		= GetDlgItem( hPanelWnd, IDCCOPYTCAP	);

			hExportHeadCam	= GetDlgItem( hPanelWnd, IDCAMERAS		);
			hExportSkeletal	= GetDlgItem( hPanelWnd, IDCSKELETAL	);

			SetWindowText( hTexPathWnd, texPath.c_str() );
			SetValues();
			EnableStuff();

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
					DisableStuff(); 
					if (Export) Export->StartExporting(); 
				}
				break;
				
				case IDC_COPYTEX:
				//  pushed checkbox "Do not copy textures"
				{
					GetValues();
					EnableStuff();
				}
				break;

				case IDC_TEXPATH: //  select directory to store textures
				{
					char* BrowseForFolder();
					char* folder = BrowseForFolder();
					if (folder) 
					{
						texPath = string( folder );
						SetWindowText( hTexPathWnd, texPath.c_str() );
					}
				}
				case IDCCOLORS:   //  "Export colors" checkbox
				case IDCNORMALS:  //  "Export normals" checkbox
				case IDCSMOOTH:   //  "Ignore smoothing checkbox
				{
					GetValues();
				}break;
				
				case IDCKF:
				case IDCSAMPLING:
				//  keyframing mode radio clicked
				{
					GetValues();
					EnableAnimGroup( true );
				}

				case IDCEXPORTANIM:
				//  "Export animation" checkbox
				{
					GetValues();
					EnableAnimGroup( bExportAnimation );
				}
				break;
			}
		}return TRUE;
	}
	return FALSE;
}


void Msg( const char* format, ... )
{
	if (!hPanelWnd) return;
	
	char buffer[1024];
	va_list argList;
	va_start( argList, format );
	vsprintf( buffer, format, argList );
	va_end( argList );

	SendMessage( hLogWnd, LB_ADDSTRING, 0, (LPARAM)buffer );
	UpdateWindow( hLogWnd );
}

void ShowExportDialog( IExporter* exp )
{
	Export = exp;
	DialogBoxParam( hInstance, 
					MAKEINTRESOURCE(IDD_EXPORT_PANEL), 
					GetActiveWindow(), 
					dlgProc, 
					0 );
	int res = GetLastError();
}

void SetProgressPos( int pos )
{
	if (!hProgressWnd) return;
	SendMessage( hProgressWnd, PBM_SETPOS, (WPARAM)pos, 0 );
}

void SetProgressRange( int min, int max )
{
	if (!hProgressWnd) return;
	SendMessage( hProgressWnd, PBM_SETRANGE, 0, MAKELPARAM( min, max ) );
}

void EnableAnimGroup( bool enable ) 
{
	if (!enable)
	{
		EnableWindow( hKFWnd,			FALSE );
		EnableWindow( hSampleWnd,		FALSE );
		EnableWindow( hSrateWnd,		FALSE );
		EnableWindow( hKPSWnd,			FALSE );
		EnableWindow( hSrateCap,		FALSE );
		EnableWindow( hExportSkeletal,	FALSE );
	}
	else
	{
		EnableWindow( hKFWnd,			TRUE );
		EnableWindow( hSampleWnd,		TRUE );
		EnableWindow( hExportSkeletal,	TRUE );

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
}

void GetValues()
{
	bExportAnimation = 
		(SendMessage( hExportAnimWnd, BM_GETCHECK, 0, 0 ) == BST_CHECKED);

	bCopyTextures = 
		(SendMessage( hCopyTexWnd, BM_GETCHECK, 0, 0 ) != BST_CHECKED);

	bKeyframe = 
		(SendMessage( hKFWnd, BM_GETCHECK, 0, 0 ) == BST_CHECKED);

	bExportNormals = 
		(SendMessage( hNormWnd, BM_GETCHECK, 0, 0 ) == BST_CHECKED);

	bExportColors = 
		(SendMessage( hColWnd, BM_GETCHECK, 0, 0 ) == BST_CHECKED);

	bIgnoreSmoothing = 
		(SendMessage( hIgnoreSm, BM_GETCHECK, 0, 0 ) == BST_CHECKED);

	bExportHeadCam	 = 
		(SendMessage( hExportHeadCam, BM_GETCHECK, 0, 0 ) == BST_CHECKED);

	bExportSkeletal	 = 
		(SendMessage( hExportSkeletal, BM_GETCHECK, 0, 0 ) == BST_CHECKED);
}

#define CHECK(A) ((A)?(WPARAM)BST_CHECKED:(WPARAM)BST_UNCHECKED)

void SetValues()
{
	SendMessage( hExportAnimWnd,	BM_SETCHECK, CHECK( bExportAnimation ), 0 );
	SendMessage( hCopyTexWnd,		BM_SETCHECK, CHECK( !bCopyTextures	 ), 0 ); 
	SendMessage( hKFWnd,			BM_SETCHECK, CHECK( bKeyframe		 ), 0 ); 
	SendMessage( hNormWnd,			BM_SETCHECK, CHECK( bExportNormals	 ), 0 ); 
	SendMessage( hColWnd,			BM_SETCHECK, CHECK( bExportColors	 ), 0 ); 
	SendMessage( hIgnoreSm,			BM_SETCHECK, CHECK( bIgnoreSmoothing ), 0 ); 
	SendMessage( hIgnoreSm,			BM_SETCHECK, CHECK( bIgnoreSmoothing ), 0 ); 
	SendMessage( hExportHeadCam,	BM_SETCHECK, CHECK( bExportHeadCam	 ), 0 ); 
	SendMessage( hExportSkeletal,	BM_SETCHECK, CHECK( bExportSkeletal	 ), 0 ); 
}

void DisableStuff()
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

	EnableWindow( hExportHeadCam,	FALSE );
	EnableWindow( hExportSkeletal,	FALSE );

	EnableWindow( hExportAnimWnd,	FALSE );
	EnableAnimGroup( false );
}

void EnableStuff()
{
	EnableWindow( hGoBtn,			TRUE );
	EnableWindow( hCopyTCap,		TRUE );

	if (bCopyTextures)
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

	EnableWindow( hExportAnimWnd,	TRUE );

	EnableAnimGroup( bExportAnimation );
}

char g_folderPath[_MAX_PATH];
char* BrowseForFolder()
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
		SHGetPathFromIDList( pIDList, g_folderPath );
		if (strlen( g_folderPath ) == 3) g_folderPath[2] = 0;
		if (SHGetMalloc( &pMalloc ) == NOERROR)
		{
			pMalloc->Free( pIDList );
		}
	}
	else return 0;
	return g_folderPath;
} // InstApp::onBrowsePath