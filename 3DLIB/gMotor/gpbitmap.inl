/*****************************************************************************/
/*	File:	gpBitmap.inl
/*	Author:	Ruslan Shestopalyuk
/*	Date:	27.02.2003
/*****************************************************************************/

/*****************************************************************
/*	Bitmap implementation
/*****************************************************************/
_inl int Bitmap::GetW() const
{
	return lx;
}
 
_inl int Bitmap::GetH() const
{
	return ly;
}

_inl int Bitmap::GetLineSize() const
{
	return lx * GPColor::GetBytesPerPixel( cf );	
}

_inl int Bitmap::GetBytesPerPixel() const
{
	return GPColor::GetBytesPerPixel( cf );
}

_inl int Bitmap::GetRealWidth() const
{
	return lx;
}
_inl int Bitmap::GetRealHeight() const
{
	return ly;
}


_inl const BYTE* Bitmap::GetPixelData() const
{
	return pix;
}

_inl WORD Bitmap::GetPixel( int x, int y ) const
{
	return ((WORD*)pix)[x + lx * y];	
}

_inl int Bitmap::GetDataSize() const
{
	return GPColor::GetBytesPerPixel( cf ) * lx * ly;
}

_inl ColorFormat Bitmap::GetColFmt() const
{
	return cf;
}

/*****************************************************************************/
/*	GPBitmapChunk implementation
/*****************************************************************************/
_inl void GPBitmapChunk::Set( GPBitmap* _papa, int _relX, int _relY, int _width, int _height )
{
	papa	= _papa;
	width	= _width;
	height	= _height;
	relX	= _relX;
	relY	= _relY;
} // GPBitmapChunk::Set
