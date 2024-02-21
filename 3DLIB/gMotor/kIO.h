/*****************************************************************************/
/*	File:	kIO.h
/*	Desc:	Input-output routines	
/*	Author:	Ruslan Shestopalyuk
/*	Date:	20.02.2003
/*****************************************************************************/
#ifndef __KIO_H__
#define __KIO_H__

/*****************************************************************************/
/*	Class:	MediaChunk
/*	Desc:	chunk loading wrapper
/*****************************************************************************/
class MediaChunk
{
	DWORD		size;
	DWORD		curPosInBlock;
	char		strMagic[5];
	ResFile		resFile;
public:
	_inl				MediaChunk();	
	_inl bool			ReadHeader( ResFile rf );
	_inl DWORD			Skip();
	_inl DWORD			Skip( int nBytes );
	_inl DWORD			Read( void* buf, int sizeBytes );

	_inl const char*	GetMagic()		const;
	_inl DWORD			GetMagicDW()	const;
	_inl int			GetFullSize()	const;
	_inl int			GetBytesLeft()	const;
	_inl int			GetDataSize()	const;
	
}; // class MediaChunk

/*****************************************************************************/
/*	Enum:	MFLError
/*	Desc:	media file loading error codes	
/*****************************************************************************/
enum MFLError
{
	mfleNoError			= 0,
	mfleBadChunk		= 1,
	mfleEndFile			= 2,
	mfleFileOpenError	= 3
}; // enum MFLError

/*****************************************************************************/
/*	Class:	MediaFileLoader
/*	Desc:	Loads chunked media file block-by-block
/*****************************************************************************/
class MediaFileLoader
{
	ResFile				resFile;
	int					fileSize;
	int					curFilePos;

	MFLError			errorStatus;
	MediaChunk			curChunk;

public:
	_inl				MediaFileLoader( ResFile* rf );
	_inl				MediaFileLoader( const char* fName );
	_inl				~MediaFileLoader();
	_inl DWORD			Read( void* buf, int sizeBytes );
	
	_inl MFLError		Error();
	_inl bool			NextChunk();
	_inl DWORD			CurChunkSkip();
	_inl DWORD			CurChunkSkip( DWORD nBytes );

	_inl const MediaChunk& GetCurChunk() const { return curChunk; }

protected:
	_inl ResFile		GetFileHandle() const { return resFile; }
}; // class MediaFileLoader

#define MAGIC_FOURCC(A) ((A << 24) | ((A & 0x0000FF00) << 8) | ((A & 0x00FF0000) >> 8) | ((A & 0xFF000000) >> 24)) 

#ifdef _INLINES
#include "kIO.inl"
#endif // _INLINES

#endif // __KIO_H__