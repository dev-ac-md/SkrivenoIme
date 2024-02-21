#pragma once

#ifndef _ogg_vor_h_included_
#define _ogg_vor_h_included_

#include <windows.h>

enum DRIVER_TYPE{
	dtUnknown,
	dtEmulated,
	dtVxD,
	dtWDM
};

void ov_Init(HWND hExtWnd);
void ov_Play(LPCSTR pcszFileName);
void ov_Stop(void);
void ov_Done(void);
void ov_SetVolume(DWORD dwVolume);
DWORD ov_GetStreamLength(void);
DWORD ov_StreamFinished(void);
DRIVER_TYPE ov_DriverType(void);

#endif // _ogg_vor_h_included_