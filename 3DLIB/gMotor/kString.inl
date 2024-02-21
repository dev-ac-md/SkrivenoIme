/*****************************************************************************/
/*	File:	kString.inl
/*	Author:	Ruslan Shestopalyuk
/*	Date:	29.01.2003
/*****************************************************************************/

/*****************************************************************************/
/*	String implementation
/*****************************************************************************/
_inl String::String() : buf(0), len(0)
{
}

_inl String::String( const char* val ) 
{ 
	buf = 0;
	len = 0;
	set( val ); 
}
	
_inl String::~String() 
{ 
	delete[] buf; 
}

_inl void String::set( const char* val )
{
	int nlen = strlen( val ) + 1;
	if (nlen != len)
	{
		delete []buf;
		buf = new char[nlen];
	}
	memcpy( buf, val, nlen );
	len = nlen;
}

_inl bool String::operator ==( const String& str ) const
{
	if (buf == 0)
	{
		if (str.buf == 0) return true;
		return false;
	}
	return !strcmp( buf, str.buf );
}

_inl char& String::operator[]( int idx )
{
	assert( idx >= 0 && idx < len );
	return buf[idx];
}

_inl void String::operator=( const String& str )
{
	set( str.buf );
}

_inl void String::operator +=( const char* catStr )
{
	int clen = strlen( catStr );
	resize( len + clen );
	memcpy( buf + len - 1, catStr, clen + 1 );
	len += clen;
} // String::operator +=

_inl void String::operator=( const char* str )
{
	set( str );
}

_inl void String::resize( int newSz )
{
	char* newBuf = new char[newSz];
	len = newSz < len ? newSz : len;
	memcpy( newBuf, buf, len );
	delete []buf;
	buf = newBuf;
} // String::resize

_inl int String::size() const 
{ 
	return len; 
}

_inl const char* String::c_str() const 
{
	return buf; 
} 

_inl unsigned int String::hash() const
{
	char* s = buf;
	unsigned int h = 0;
	while (*s)
	{
		h = (h * 729 + (*s) * 37) ^ (h >> 1);
		s++;
	}
	return h;
}
