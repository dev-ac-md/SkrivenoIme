/*****************************************************************/
/*  File:   IniFile.h
/*  Desc:   Ini file parser/container
/*  Author: Silver, Copyright (C) GSC Game World
/*  Date:   Feb 2002
/*****************************************************************/

#ifndef __INIFILE_H__
#define __INIFILE_H__

#pragma once 
#pragma warning ( disable : 4786 )

#include <map>
#include <string>

using namespace std;

enum IniFileError{
		ifeNoError			= 0,
		ifeNoSection		= 1,
		ifeNoRecord			= 2,
		ifeWrongRecord		= 3,
		ifeNoIniFile		= 4,
		ifeWrongSectionName = 5,
		ifeIniFileError		= 6,
		ifeWrongRecordFmt	= 7
}; // enum IniFileError

/*****************************************************************/
/*  Class:	IniFile
/*  Desc:	Describes generic .ini file like standard windowz 
/*				format:
/*				- ini file contains set of the sections
/*				- each section begins with [<section name>]
/*					(ex. [VidMode])
/*				- each section contains set of the ini records
/*					in the form 
/*					<record1>=<value1>
/*					<record2>=<value2>
/*					...
/*				- you can use comments in the form 
/*					;this is the comment
/*****************************************************************/
class IniFile
{
//------------------------- PUBLIC INTERFACE ----------------------
public:
							IniFile( const char* fname );
							~IniFile();
	
	
	//  returns true if error occured during last call
	const bool				Error();
	//  returns string error message, if there was error
	IniFileError			GetErrorCode() const;

	//  get integer section/record value
	//  if secName==NULL, current section is used, same for the record
	int						GetValInt( const char* recName = 0, 
										 const char* secName = 0 );
	
	//  get float section/record value
	float					GetValFloat( const char* recName = 0, 
										   const char* secName = 0 );
	
	//  get string( without "") section/record value
	string				GetValString( const char* recName = 0, 
											const char* secName = 0 );

	//  get symbol section/record value (whole string)
	string				GetValSymbol( const char* recName = 0, 
											const char* secName = 0 );

	//  get boolean
	bool					GetValBool( const char* recName = 0, 
											const char* secName = 0 );

	//  returns number of the sections
	int						GetNumSections() const;
	
	//  step to the next section in ini file
	bool	NextSection();
	//  step to the next record in the section
	bool	NextRecord();

	//  jump to the first section
	bool	FirstSection();
	//  jump to the first record in the section
	bool	FirstRecord();

	//  returns name of the current section
	string GetCurSecName();
	string GetCurRecName();

//-----------------------------------------------------------------
protected:

	struct IniRecord
	{
		string		recName;
		string		recVal;
	}; // struct IniRecord

	typedef map<string, IniRecord>::iterator	RecMapIter;
	//  nested class IniSection
	struct IniSection
	{
		string name;
		map<string, IniRecord>	recMap;
	}; // struct IniSection

	typedef map<string, IniSection>::iterator	IniMapIter;

	void			ToUppercase( string& str );
	IniRecord*		FindRecord( const char* secName, 
								const char* recName );
	void			Init( istream& istr );

private:
	IniFile();
	
	map<string, IniSection>	iniMap;		//  sections map
	IniFileError						lastError;	//  error code
	IniMapIter							secIter;	//  sections iterator
	RecMapIter							recIter;	//  records iterator
};  // class IniFile

#endif // __INIFILE_H__