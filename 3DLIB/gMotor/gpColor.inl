/*****************************************************************
/*  File:   GPColor.inl                                           
/*  Desc:   Color operations inlines
/*  Author: Silver, Copyright (C) GSC Game World                 
/*  Date:   January 2002                                         
/*****************************************************************/
/*****************************************************************
/*	GPColor implementation
/*****************************************************************/
_inl DWORD GPColor::ToARGB( BYTE a, BYTE r, BYTE g, BYTE b )
{
		return  (
				((a & 0xFF) << 24) | 
				((r & 0xFF) << 16) |
				((g & 0xFF) << 8)  |
				(b & 0xFF)
				);
}

_inl DWORD GPColor::ToARGB( float a, float r, float g, float b )
{
	BYTE ba, br, bg, bb;
	ba = (BYTE)(a * 255.0f);
	br = (BYTE)(r * 255.0f);
	bg = (BYTE)(g * 255.0f);
	bb = (BYTE)(b * 255.0f);
	return ToARGB( ba, br, bg, bb );
}	

_inl int GPColor::GetBytesPerPixel( ColorFormat cf )
{
	switch (cf)
	{
	case cfUnknown:		return 0;
	case cfARGB4444:	return 2;
	case cfXRGB1555:	return 2;
	case cfARGB8888:	return 4;
	case cfRGB565:		return 2;
	case cfA8:			return 1;
	case cfRGB888:		return 3;
	case cfXRGB8888:	return 4;
	case cfV8U8:		return 2;
	}
	return 0;
}
