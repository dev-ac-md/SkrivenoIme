/*****************************************************************
/*  File:   kDebug.h											 *
/*  Desc:   Debugging utilities declarations                     *
/*  Author: Silver, Copyright (C) GSC Game World                 *
/*  Date:   November 2001                                        *
/*****************************************************************/
#ifndef __KDEBUG_H__
#define __KDEBUG_H__

#include "stdlib.h"
#include <string>
#include "windows.h"

#pragma once

//  error types
enum ErrorTypes{
		etUnknown	= 0,
		etWinAPI	= 1,
		etDirectX	= 2,
		etAssert	= 3,
		etCustom	= 4
};

const int c_MaxErrMessageLen = 512;

/*****************************************************************
/*  Class:   BugSlayer, singleton                                *
/*  Desc:    Debugging kernel class			                     *
/*****************************************************************/
class BugSlayer  
{
public:
	~BugSlayer();

	static BugSlayer& Instance()
	{ 
		static BugSlayer meLovely;
		return meLovely;
	}

	void		ProcessWinError();

	static INT CALLBACK	BugDlgProc( HWND	hWnd, 
									UINT	msg, 
									WPARAM  wParam, 
									LPARAM  lParam );
	

	int			InvokeBugDialog( HRESULT hresult = S_OK );

	void		SetFILE( const char* strFile );
	void		SetLINE( UINT nLine );
	void		SetType( ErrorTypes etType );
	void		SetMessage( const char* mes );

protected:
	                    BugSlayer();
	static const char*  GetErrTypeString( ErrorTypes et );

	char		errFile[_MAX_PATH];						//	name of the source file
	char		errLine[64];							//	number of source line
	char		errMessage[c_MaxErrMessageLen];			//  error message

	ErrorTypes	errType;		//  error type

    HWND        hBugWnd;
private:
};  // class BugSlayer

#define WA_CHK(A)		{	if((A) == 0){ \
							BugSlayer::Instance().SetFILE(__FILE__); \
							BugSlayer::Instance().SetLINE(__LINE__); \
							BugSlayer::Instance().SetType(etWinAPI);\
							Log.Error("Win32 Error.");\
							BugSlayer::Instance().InvokeBugDialog();} }

#define CHECKM(A,B)		{if (!(A)){ \
							BugSlayer::Instance().SetFILE(__FILE__); \
							BugSlayer::Instance().SetLINE(__LINE__); \
							BugSlayer::Instance().SetType(etAssert);\
							BugSlayer::Instance().SetMessage(Log.Error((B)));\
							BugSlayer::Instance().InvokeBugDialog();} }
/*
#ifdef assert
#undef assert
#endif

#define assert(A)		CHECKM((A),(A))
 */

#endif // __KDEBUG_H__
