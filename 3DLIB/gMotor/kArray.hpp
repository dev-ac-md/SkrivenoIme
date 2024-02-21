/*****************************************************************************/
/*	File:	kArray.hpp
/*	Desc:	Array-like templated containers
/*	Author:	Ruslan Shestopalyuk
/*	Date:	30.01.2003
/*	Rmrk:	Style is borrowed from STL to make it less confusing
/*****************************************************************************/
#ifndef __KARRAY_HPP__
#define __KARRAY_HPP__
#pragma once

BEGIN_NAMESPACE( c2 )

/*****************************************************************************/
/*	Class:	array
/*	Desc:	Linear array with self-adjusting
/*****************************************************************************/
template <class TElem>
class array
{
public:
	array() : buf(0), nElem(0), maxElem(0) {}
	array( int nEl ) : buf(0), nElem(0), maxElem(0)
	{
		reserve( nEl );
	}

	array( const array<TElem>& arr )
	{
		resize( arr.nElem );
		memcpy( buf, arr.buf, nElem * sizeof(TElem) );
	}

	~array()
	{
		delete []buf;
		buf		= 0;
		nElem	= 0;
		maxElem = 0;
	}

	void	reserve( int sz )
	{
		if (sz == maxElem) return;
		TElem*	newBuf	= new TElem[sz];
		nElem	= sz < nElem ? sz : nElem; 
		memcpy( newBuf, buf, nElem * sizeof(TElem) );
		maxElem = sz; 
		delete []buf;
		buf = newBuf;
	}

	void	resize( int newSize )
	{
		if (newSize == maxElem)
		{
			nElem = newSize;
			return;
		}
		delete []buf;
		nElem	= 0;
		maxElem = 0;
		buf		= 0;
		reserve( newSize );
		nElem = newSize;
	}

	TElem&	operator[]( int idx )
	{
		assert( buf && idx >= 0 && idx < nElem );
		return buf[idx];
	}

	const TElem&	operator[]( int idx ) const
	{
		assert( buf && idx >= 0 && idx < nElem );
		return buf[idx];
	}

	const TElem& c_elem( int idx ) const
	{
		assert( buf && idx >= 0 && idx < nElem );
		return buf[idx];
	}

	int size_bytes() const
	{
		return maxElem * sizeof( TElem ) + sizeof( *this );
	}

	int	size() const { return nElem; }

	bool push_back( const TElem& el )
	{
		if (nElem >= maxElem ) 
		{
			if (maxElem > 1) 
			{
				reserve( maxElem + maxElem / 2 );
			}
			else 
			{
				reserve( 2 );
			}
		}
		buf[nElem++] = el;
		return true;
	}

	int capacity() const
	{
		return maxElem;
	}

private:
	TElem*		buf;
	int			nElem;
	int			maxElem;

}; // class array

/*****************************************************************************/
/*	Class:	static_array
/*	Desc:	Static linear unordered array 
/*****************************************************************************/
template <class TElem, int MaxSz>
class static_array
{
public:
	static_array() : nElem(0) {}

	static_array( const static_array<TElem, MaxSz>& arr )
	{
		memcpy( buf, arr.buf, nElem * sizeof(TElem) );
	}

	~static_array()
	{
		nElem	= 0;
	}

	TElem&	operator[]( int idx )
	{
		assert( idx >= 0 && idx < nElem );
		return buf[idx];
	}

	const TElem&	operator[]( int idx ) const
	{
		assert( idx >= 0 && idx < nElem );
		return buf[idx];
	}

	const TElem& c_elem( int idx ) const
	{
		assert( idx >= 0 && idx < nElem );
		return buf[idx];
	}

	int size_bytes() const
	{
		return MaxSz * sizeof( TElem ) + sizeof( *this );
	}

	int	size() const { return nElem; }

	void push_back( const TElem& el )
	{
		assert(nElem < MaxSz); 
		buf[nElem++] = el;
	}

	void set_size( int sz )
	{
		assert( sz > 0 && sz < MaxSz );
		nElem = sz;
	}

	void erase( int elIdx ) 
	{
		assert( elIdx >= 0 && elIdx < nElem );
		nElem--;
		buf[elIdx] = buf[nElem];
	}

	int capacity() const
	{
		return MaxSz;
	}

	void clear()
	{
		nElem = 0;
	}

	void dump( FILE* fp )
	{
		if (!fp) return;
		fprintf( fp, "\nDumping array(%X), nElem:%d, MaxSz:%d\n", this, nElem, MaxSz );
		for (int i = 0; i < nElem; i++)
		{
			fprintf( fp, "%d\n", buf[i] );
		}
	}

private:
	TElem		buf[MaxSz];
	int			nElem;
}; // class static_array

/*****************************************************************************/
/*	Class:	static_ptr_array
/*	Desc:	Static linear pointer array 
/*****************************************************************************/
template <class TElem, int MaxSz>
class static_ptr_array
{
public:
	static_ptr_array() : nElem(0) {}

	static_ptr_array( const static_ptr_array<TElem, MaxSz>& arr )
	{
		memcpy( buf, arr.buf, nElem * sizeof(TElem*) );
	}

	~static_ptr_array()
	{
		nElem	= 0;
	}

	TElem*	operator[]( int idx )
	{
		massert( idx >= 0 && idx < nElem, "static_ptr_array index is out of bounds." );
		return buf[idx];
	}

	const TElem*	operator[]( int idx ) const
	{
		massert( idx >= 0 && idx < nElem, "static_ptr_array index is out of bounds." );
		return buf[idx];
	}

	const TElem* c_elem( int idx ) const
	{
		massert( idx >= 0 && idx < nElem, "static_ptr_array index is out of bounds." );
		return buf[idx];
	}

	int size_bytes() const
	{
		return MaxSz * sizeof( TElem* ) + sizeof( *this );
	}

	int	size() const { return nElem; }

	void push_back( TElem* el )
	{
		assert(nElem < MaxSz); 
		buf[nElem++] = el;
	}

	void set_size( int sz )
	{
		assert( sz > 0 && sz < MaxSz );
		nElem = sz;
	}

	void erase( int elIdx ) 
	{
		assert( elIdx >= 0 && elIdx < nElem );
		buf[elIdx] = NULL;
	}

	int capacity() const
	{
		return MaxSz;
	}

	void clear()
	{
		nElem = 0;
	}

	void dump( FILE* fp )
	{
		if (!fp) return;
		fprintf( fp, "\nDumping array(%X), nElem:%d, MaxSz:%d\n", this, nElem, MaxSz );
		for (int i = 0; i < nElem; i++)
		{
			fprintf( fp, "%d\n", buf[i] );
		}
	}

private:
	TElem*		buf[MaxSz];
	int			nElem;
}; // class static_ptr_array

/*****************************************************************************/
/*	Class:	round_robin
/*	Desc:	circular queue 
/*****************************************************************************/
template <class TElem, int MaxSz>
class round_robin
{
public:
				round_robin() : cTail(0), cHead(0) {}

	void		push( const TElem& el )
	{
		if (cHead == cTail) cTail = (cTail + 1) % MaxSz;
		buf[cHead] = el;
		cHead = (cHead + 1) % MaxSz;
	}

	const TElem& bottom() const
	{
		return buf[cTail];
	}

	const TElem& pop_bottom()
	{
		int idx = cTail;
		cTail = (cTail + 1) % MaxSz;
		assert( cTail != cHead );
		return buf[idx];
	}

	const TElem& top() const
	{
		int idx = cHead > 0 ? cHead - 1 : MaxSz - 1;
		return buf[idx];
	}

	void dump( FILE* fp )
	{
		if (!fp) return;
		fprintf( fp, "\nDumping round_robin(%X), cHead:%d, cTail:%d, MaxSz:%d\n", 
					this, cHead, cTail, MaxSz );
		for (int i = 0; i < MaxSz; i++)
		{
			fprintf( fp, "buf[%3d] = %d\n", i, buf[i] );
		}
	}

private:
	TElem		buf[MaxSz];
	int			cTail;
	int			cHead;
}; // class round_robin

END_NAMESPACE( c2 )

#endif // __KARRAY_HPP__