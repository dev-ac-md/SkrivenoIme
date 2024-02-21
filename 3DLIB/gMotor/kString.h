/*****************************************************************************/
/*	File:	kString.h
/*	Desc:	Generic string routines
/*	Author:	Ruslan Shestopalyuk
/*	Date:	29.01.2003
/*****************************************************************************/
#ifndef __KSTRING_H__
#define __KSTRING_H__

/*****************************************************************************/
/*	Class:	String
/*	Desc:	Generic string class
/*****************************************************************************/
class String
{
	char*				buf;
	int					len;

public:
	_inl				String		();
	_inl				String		( const char* val		); 
	_inl				~String		();
	_inl void			set			( const char* val		);
	_inl void			operator=	( const String& str		);
	_inl void			operator +=	( const char* catStr	);
	_inl char&			operator[]	( int idx				);
	_inl void			operator=	( const char* str		);
	_inl void			resize		( int newSz				);

	_inl operator		const char*	()						const { return buf; }
	_inl bool			operator ==	( const String& str )	const;
	_inl int			size		()						const; 
	_inl const char*	c_str		()						const; 
	_inl unsigned int	hash		()						const;
}; // class kString

#ifdef _INLINES
#include "kString.inl"
#endif // _INLINES

#endif // __KSTRING_H__