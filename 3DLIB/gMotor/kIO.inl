/*****************************************************************************/
/*	File:	kIO.inl
/*	Desc:	
/*	Author:	Ruslan Shestopalyuk
/*	Date:	20.02.2003
/*****************************************************************************/

/*****************************************************************************/
/*	MediaChunk implementation
/*****************************************************************************/
_inl MediaChunk::MediaChunk()
{
	memset( strMagic, 0, sizeof( strMagic ) );
} // MediaChunk::ReadHeader

_inl bool MediaChunk::ReadHeader( ResFile rf )
{
	assert( rf );
	//  magic word
	if (RBlockRead( rf, strMagic,	4 ) == 0) return false; 
	if (RBlockRead( rf, &size,		4 ) == 0) return false; 
	curPosInBlock = 0;
	resFile = rf;
	
	return true;
} // MediaChunk::ReadHeader
	
_inl DWORD MediaChunk::Skip()
{
	assert( resFile );
	int skipSize = GetBytesLeft();
	curPosInBlock = GetDataSize();
	RSeekFromCurrent( resFile, skipSize );
	return skipSize;
} // MediaChunk::Skip

_inl DWORD MediaChunk::Skip( int nBytes )
{
	assert( resFile && GetBytesLeft() >= nBytes);
	curPosInBlock += nBytes;
	RSeekFromCurrent( resFile, nBytes );
	return nBytes;
} // MediaChunk::Skip

_inl const char* MediaChunk::GetMagic() const
{
	return strMagic;
} // MediaChunk::GetMagic

_inl DWORD MediaChunk::GetMagicDW() const
{
	return *((DWORD*)strMagic);
} // MediaChunk::GetMagicDW

_inl int MediaChunk::GetFullSize() const
{
	return size;
} // MediaChunk::GetFullSize

_inl int MediaChunk::GetDataSize() const
{
	return size - 8;
} // MediaChunk::GetSize

_inl int MediaChunk::GetBytesLeft() const
{
	return size - 8 - curPosInBlock;
} // MediaChunk::GetSize

_inl DWORD MediaChunk::Read( void* buf, int sizeBytes )
{
	assert( buf && sizeBytes <= GetDataSize() - curPosInBlock );
	
	curPosInBlock += sizeBytes;
	return RBlockRead( resFile, buf, sizeBytes );
} // MediaChunk::Read

/*****************************************************************************/
/*	MediaFileLoader implementaiton
/*****************************************************************************/
_inl MediaFileLoader::MediaFileLoader( ResFile* rf )
{
	assert( rf );
	resFile			= rf;
	errorStatus		= mfleNoError;
	fileSize		= RFileSize( resFile );
	curFilePos		= 0;
} // MediaFileLoader::MediaFileLoader

_inl MediaFileLoader::MediaFileLoader( const char* fName )
{
	resFile		= RReset( fName );
	errorStatus	= mfleNoError;
	if (resFile == INVALID_HANDLE_VALUE)
	{
		errorStatus	= mfleFileOpenError;
		return; 
	}
	fileSize		= RFileSize( resFile );
	curFilePos		= 0;
} // MediaFileLoader::MediaFileLoader

_inl MediaFileLoader::~MediaFileLoader()
{
	if (resFile != INVALID_HANDLE_VALUE) RClose( resFile );
}

_inl MFLError MediaFileLoader::Error()
{
	return errorStatus;
} // MediaFileLoader::Error

_inl bool	MediaFileLoader::NextChunk()
{
	if (curFilePos >= fileSize) return false;
	assert( resFile );
	int nReadBytes = curChunk.ReadHeader( resFile );
	if (nReadBytes == 0)
	{
		errorStatus = mfleBadChunk;
		return false;
	}	
	curFilePos += nReadBytes;
	return true;
} // MediaFileLoader::NextChunk

_inl DWORD MediaFileLoader::Read( void* buf, int sizeBytes )
{
	curFilePos += sizeBytes;
	return curChunk.Read( buf, sizeBytes );
}

_inl DWORD MediaFileLoader::CurChunkSkip()
{
	curFilePos += curChunk.Skip();
	return curFilePos;
} // MediaFileLoader::CurChunkSkip

_inl DWORD MediaFileLoader::CurChunkSkip( DWORD nBytes )
{
	curFilePos += curChunk.Skip( nBytes );
	return curFilePos;
} // MediaFileLoader::CurChunkSkip

