/*****************************************************************************/
/*	File:	kString.hpp
/*	Desc:	Generic string routines
/*	Author:	Ruslan Shestopalyuk
/*	Date:	29.01.2003
/*****************************************************************************/
#ifndef __KSTRING_HPP__
#define __KSTRING_HPP__

/*****************************************************************************/
/*	Class:	String
/*	Desc:	Generic string class
/*****************************************************************************/
class String
{
	char*		buf;
	int			len;
public:
	String() : buf(0), len(0) {}
	~String() { delete[] buf; }

}; // class kString

#endif // __KSTRING_HPP__