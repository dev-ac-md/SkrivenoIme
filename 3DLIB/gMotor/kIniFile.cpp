/*****************************************************************/
/*  File:   IniFile.cpp
/*  Desc:   Ini file manipulator
/*  Author: Silver, Copyright (C) GSC Game World
/*  Date:   Feb 2002
/*****************************************************************/
#include "stdafx.h"
#include "kIniFile.h"
#include <fstream>
#include <string>
using namespace std;


string trimLR( const string& orig );
string withoutPar( const string& orig );
bool toFloat( const string& str, float& res );
bool toInt( const string& str, int& res );

/*****************************************************************/
/*  IniFile implementation
/*****************************************************************/
void IniFile::ToUppercase( string& str )
{
	for (int i = 0; i < str.size(); i++)
		if (islower( str[i] )) str[i] = _toupper( str[i] );
}

IniFile::IniFile( const char* fname ) : lastError(ifeNoError)
{
	secIter = iniMap.end();
	
	if (!fname) 
	{
		lastError = ifeNoIniFile;
		return;
	}

	FILE* fp = fopen( fname, "rt" );
	if (!fp) 
	{
		lastError = ifeNoIniFile;
		return;
	}

	const int c_MaxLineSz = 256;
	char line[c_MaxLineSz];
	char *res;

	IniSection* curSec = 0;
	IniRecord curRec;

	while (!feof( fp ))
	{
		res = fgets( line, c_MaxLineSz, fp );
		//  parse line
		
		//  skip comment or empty line
		if (*line == ';' || *line == '\0') continue;
		//  new section
		if (*line == '[')
		{
			char* iend = strstr( line, "]" );
			if (!iend)
			{
				lastError = ifeWrongSectionName;
				fclose( fp );
				return;
			}
			
			IniSection sec;
			sec.name = string( line + 1, iend - line - 1 );
			ToUppercase( sec.name );
			iniMap.insert( map<string, IniSection>::value_type( sec.name, sec ) );
			curSec = &(iniMap[sec.name]);
			continue;
		}

		//  trying to read record
		char* iend = strstr( line, "=" );
		if (iend != 0 && curSec != 0)
		{
			curRec.recName	= string( line, iend - line );
			curRec.recVal	= string( iend + 1, strlen( iend ) - 1 );
			
			curRec.recName = trimLR( curRec.recName );
			ToUppercase( curRec.recName );
			curRec.recVal  = trimLR( curRec.recVal );

			curSec->recMap.insert( map<string, IniRecord>::value_type( 
												curRec.recName, curRec ) );
		}
		
	}
	fclose( fp );
	
	FirstSection();
}

IniFile::~IniFile()
{
}

const bool IniFile::Error()
{
	return lastError != ifeNoError;
}

IniFileError IniFile::GetErrorCode() const
{
	return lastError;
}

string IniFile::GetCurSecName()
{
	if (secIter == iniMap.end()) return "";
	return (*secIter).second.name;
}

string IniFile::GetCurRecName()
{
	if (recIter == (*secIter).second.recMap.end()) return "";
	return (*recIter).second.recName;
}

bool IniFile::NextSection()
{
	if (secIter == iniMap.end()) return false;
	secIter++;
	if (secIter == iniMap.end()) return false;
	FirstRecord();
	return true;
}

bool IniFile::NextRecord()
{
	recIter++;
	if (recIter == (*secIter).second.recMap.end()) return false; 
	return true;
}

bool IniFile::FirstSection()
{
	secIter = iniMap.begin();
	if (secIter == iniMap.end()) return false;
	FirstRecord();
	return true;
}

bool IniFile::FirstRecord()
{
	if (secIter == iniMap.end()) return false;
	
	recIter = (*secIter).second.recMap.begin();
	if (recIter == (*secIter).second.recMap.end()) return false; 
	return true;
}

int		IniFile::GetValInt( const char* recName, 
						  const char* secName )
{
	string recs, secs;
	if (recName) recs = string( recName );
	if (secName) secs = string( secName );

	ToUppercase( recs );
	ToUppercase( secs );

	IniRecord* rec = FindRecord( recs.c_str(), secs.c_str() );
	if (rec == 0) 
	{
		lastError = ifeNoRecord;
		return 0;
	}
	int res = 0;
	if (!toInt( rec->recVal, res ))
	{
		lastError = ifeWrongRecordFmt;
		return 0;
	}
	return res;
}
	
float	IniFile::GetValFloat( const char* recName, 
						    const char* secName )
{
	string recs, secs;
	if (recName) recs = string( recName );
	if (secName) secs = string( secName );

	ToUppercase( recs );
	ToUppercase( secs );

	IniRecord* rec = FindRecord( recs.c_str(), secs.c_str() );
	if (rec == 0) 
	{
		lastError = ifeNoRecord;
		return 0;
	}
	float res = 0;
	if(!toFloat( rec->recVal, res ))
	{
		lastError = ifeWrongRecordFmt;
		return 0.0f;
	}
	return res;
}
	
string	IniFile::GetValString( const char* recName, 
					const char* secName ) 
{
	string recs, secs;
	if (recName) recs = string( recName );
	if (secName) secs = string( secName );

	ToUppercase( recs );
	ToUppercase( secs );

	IniRecord* rec = FindRecord( recs.c_str(), secs.c_str() );
	if (rec == 0) 
	{
		lastError = ifeNoRecord;
		return "";
	}
	const string& st = rec->recVal;
	if (st.size() < 2 || st[0] != '"' || 
		st[st.size() - 1] != '"') 
	{
		lastError = ifeWrongRecord;
		return ""; 
	}
	return withoutPar( rec->recVal );	
}

bool IniFile::GetValBool( const char* recName, 
					const char* secName ) 
{
	string recs, secs;
	if (recName) recs = string( recName );
	if (secName) secs = string( secName );

	ToUppercase( recs );
	ToUppercase( secs );

	IniRecord* rec = FindRecord( recs.c_str(), secs.c_str() );
	if (rec == 0) 
	{
		lastError = ifeNoRecord;
		return false;
	}
	string st = rec->recVal;
	
	ToUppercase( st );
	if (!st.compare( "1" ) || !st.compare( "TRUE" )) return true;
	else if (!st.compare( "0" ) || !st.compare( "FALSE" )) return false;
	
	lastError = ifeWrongRecord;
	return false;	
}

string	IniFile::GetValSymbol( const char* recName, 
					const char* secName ) 
{
	string recs, secs;
	if (recName) recs = string( recName );
	if (secName) secs = string( secName );

	ToUppercase( recs );
	ToUppercase( secs );

	IniRecord* rec = FindRecord( recs.c_str(), secs.c_str() );
	if (rec == 0) 
	{
		lastError = ifeNoRecord;
		return "";
	}
	const string& st = rec->recVal;
	return rec->recVal;	
}

int	IniFile::GetNumSections() const
{
	return iniMap.size();
}

IniFile::IniRecord* IniFile::FindRecord( const char* recName, 
										 const char* secName )
{
	IniMapIter it;
	if (secName != 0) 
	{
		it = iniMap.find( secName ); 
	}
	else 
	{
		it = secIter;
	}

	if (it == iniMap.end())
	{
		lastError = ifeNoSection;
		return 0;
	}

	IniSection& sec = (*it).second;
	RecMapIter rit;
	if (recName != 0) 
	{
		rit = sec.recMap.find( recName );
	}
	else 
	{
		rit = recIter;
	}

	if (rit == sec.recMap.end())
	{
		lastError = ifeNoRecord;
		return 0;
	}

	return &((*rit).second);
}  // IniFile::FindRecord

