/*****************************************************************
/*  File:   kDebug.cpp                                        *
/*  Desc:   Implementation of the debugging kernel               *
/*  Author: Silver, Copyright (C) GSC Game World                 *
/*  Date:   November 2001                                        *
/*****************************************************************/
#include "stdafx.h"
#include "kDebug.h"

using namespace std;

/*****************************************************************
/*  BugSlayer implementation                                     *
/*****************************************************************/
BugSlayer::BugSlayer()
{
}

BugSlayer::~BugSlayer()
{
}

/*---------------------------------------------------------------*
/*  Func:	BugSlayer::InvokeBugDialog
/*	Desc:	Shows bug dialog
/*	Ret:	0 if report wasn't created
/*---------------------------------------------------------------*/
int	BugSlayer::InvokeBugDialog( HRESULT hresult )
{/*
	if (Instance().errType == etDirectX) 
	{
		const char* errm = GetD3DErrorDesc( hresult );
		if (!errm) errm = DXGetErrorString8( hresult );
		Instance().SetMessage( errm );
	}

	if (Instance().errType == etWinAPI) 
	{
		char msgBuf[512];
		*msgBuf = 0;

		FormatMessage(  
			FORMAT_MESSAGE_FROM_SYSTEM | 
			FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL,
			GetLastError(),
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), 
			(LPTSTR) msgBuf,
			0,
			NULL 
		);
		Instance().SetMessage( msgBuf );
	}

	if (Instance().errType == etUnknown)
		Instance().SetMessage( "Error is of unknown nature. Medicine is helpless." );

	HINSTANCE hInst = GetModuleHandle( NULL ); 

	return DialogBox( hInst,
                      MAKEINTRESOURCE(IDD_BUGSLAYER), 
					  NULL,
					  BugDlgProc );*/
	return 0;
}//  BugSlayer::InvokeBugDialog

/*---------------------------------------------------------------*
/*  Func:	BugSlayer::BugDlgProc
/*	Desc:	Bug dialog window procedure
/*---------------------------------------------------------------*/
INT CALLBACK	BugSlayer::BugDlgProc( HWND hWnd, 
										    UINT msg, 
										    WPARAM wParam, 
										    LPARAM lParam )
{	/*
    HWND hErrMessage;   
    HWND hFile;			
    HWND hLine;			
	HWND hType;
    
    if (msg == WM_INITDIALOG) 
    {
        hErrMessage     = GetDlgItem( hWnd, ED_MESSAGE );
        hFile			= GetDlgItem( hWnd, ED_FILE  );
        hLine			= GetDlgItem( hWnd, ED_LINE );
	    hType			= GetDlgItem( hWnd, ED_TYPE );

        Instance().hBugWnd = hWnd;
		
        // Setting text fields
		
		//  error message
        SetWindowText( hErrMessage, Instance().errMessage );
		//  file name
		SetWindowText( hFile, Instance().errFile );
		//  line
		SetWindowText( hLine, Instance().errLine );
		//  error type
		SetWindowText( hType,	
					   GetErrTypeString( Instance().errType )
					 );

        return TRUE;
    }

    if (msg == WM_COMMAND)
    {
		switch (LOWORD( wParam ))
		{
			case IDABORT:  
                SendMessage( hWnd, WM_CLOSE, 0, 0 );
				exit( 1 );					
				EndDialog( hWnd, IDCANCEL );
				break;
			case IDDEBUG:  
				_asm int 3;
				EndDialog( hWnd, IDCANCEL );
				break;
			case IDIGNORE:							
				EndDialog( hWnd, IDCANCEL );
				break;
		}
        return TRUE;
    }*/
    return FALSE;
}

/*---------------------------------------------------------------*
/*  Func:	BugSlayer::GetErrTypeString
/*	Parm:	et - error type to describe
/*	Ret:	string description of error type
/*---------------------------------------------------------------*/
const char* BugSlayer::GetErrTypeString( ErrorTypes et )
{
	switch (et)
	{
		case etUnknown: return "Unknown Error Type";
		case etWinAPI : return "Win32 API Error";
		case etDirectX: return "DirectX Error";
		case etAssert : return "Assertion Failure";
		case etCustom : return "Custom Error Type";
		
		default		  :	return "Invalid Error Type";
	}
}//  BugSlayer::GetErrTypeString

/*---------------------------------------------------------------*
/*  Func:	BugSlayer::ProcessWinError
/*	Desc:	Shows message box with last Windows error description
/*---------------------------------------------------------------*/
void BugSlayer::ProcessWinError()
{
	LPVOID lpMsgBuf;
	FormatMessage( 
		FORMAT_MESSAGE_ALLOCATE_BUFFER | 
		FORMAT_MESSAGE_FROM_SYSTEM | 
		FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		GetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), 
		(LPTSTR) &lpMsgBuf,
		0,
		NULL 
	);
	MessageBox( NULL, (LPCTSTR)lpMsgBuf, "Error", MB_OK | MB_ICONINFORMATION );
	LocalFree( lpMsgBuf );
}//  BugSlayer::ProcessWinError

void BugSlayer::SetFILE( const char* strFile )
{
	strcpy( errFile, strFile );
}

void BugSlayer::SetLINE( UINT nLine )
{
	char buf[32];
	sprintf( buf, "%d", nLine );
	strcpy( errLine, buf );
}

void BugSlayer::SetType( ErrorTypes etType )
{
	errType = etType;
}

void BugSlayer::SetMessage( const char* mes )
{
	strcpy( errMessage, mes );
}





