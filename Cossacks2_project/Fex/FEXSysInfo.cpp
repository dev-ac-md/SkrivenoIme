//#include "stdafx.h"
#include <windows.h>
#include <windowsx.h>
#include <ddraw.h>
#include <dinput.h>
#include <stdio.h>
//#include "Include\\dmusici.h"
#define COMPILE_MULTIMON_STUBS
#include "Include\\multimon.h"
#include "Include\\basetsd.h"
#include "FEXModule.h"

extern char FEX_SystemVersionString[1024];

typedef HRESULT(WINAPI * DIRECTDRAWCREATE)( GUID*, LPDIRECTDRAW*, IUnknown* );
typedef HRESULT(WINAPI * DIRECTDRAWCREATEEX)( GUID*, VOID**, REFIID, IUnknown* );
typedef HRESULT(WINAPI * DIRECTINPUTCREATE)( HINSTANCE, DWORD, LPDIRECTINPUT*,
                                             IUnknown* );
//-----------------------------------------------------------------------------
// Name: FEX_GetDXVersion()
// Desc: This function returns the DirectX version number as follows:
//          0x0000 = No DirectX installed
//          0x0100 = DirectX version 1 installed
//          0x0200 = DirectX 2 installed
//          0x0300 = DirectX 3 installed
//          0x0500 = At least DirectX 5 installed.
//          0x0600 = At least DirectX 6 installed.
//          0x0601 = At least DirectX 6.1 installed.
//          0x0700 = At least DirectX 7 installed.
//          0x0800 = At least DirectX 8 installed.
// 
//       Please note that this code is intended as a general guideline. Your
//       app will probably be able to simply query for functionality (via
//       QueryInterface) for one or two components.
//
//       Please also note:
//          "if( dwDXVersion != 0x500 ) return FALSE;" is VERY BAD. 
//          "if( dwDXVersion <  0x500 ) return FALSE;" is MUCH BETTER.
//       to ensure your app will run on future releases of DirectX.
//-----------------------------------------------------------------------------
DWORD FEX_GetDXVersion()
{    
   return 0;
}
//-----------------------------------------------------------------------------
bool FEX_GetFreeSpace(int DiskNum, __int64& i64TotalBytes, __int64& i64FreeBytes)
{
	char _disk[8];
	char* disk = _disk;
	disk[0] = 'a';
	disk[1] = ':';
	disk[2] = '\\';
	disk[3] = 0;

	if(DiskNum == -1)
		disk = NULL;
	else
		disk[0] += DiskNum;
	
	__int64 i64FreeBytesToCaller;
	GetDiskFreeSpaceEx(disk, (PULARGE_INTEGER)&i64FreeBytesToCaller,
									 (PULARGE_INTEGER)&i64TotalBytes,
									 (PULARGE_INTEGER)&i64FreeBytes);
	return true;
}
//-----------------------------------------------------------------------------
void FEX_ADD_STR(char* str, ...) 
{
	static char tmp[256];
	if(!str)	return;

	va_list args;
	va_start(args, str);
	vsprintf(tmp, str, args);
	va_end(args);
	strcat(FEX_SystemVersionString, tmp);
}
//-----------------------------------------------------------------------------
char* FEX_GetSystemVersion(void)
{
	#define BUFSIZE 80

	struct myOSVERSIONINFOEX {
		DWORD dwOSVersionInfoSize;
		DWORD dwMajorVersion;
		DWORD dwMinorVersion;
		DWORD dwBuildNumber;
		DWORD dwPlatformId;
		TCHAR szCSDVersion[ 128 ];
		WORD wServicePackMajor;
		WORD wServicePackMinor;
		WORD wSuiteMask;
		BYTE wProductType;
		BYTE wReserved;
	};
   OSVERSIONINFOEX osvi;

   BOOL bOsVersionInfoEx;

	FEX_SystemVersionString[0] = 0;
   // Try calling GetVersionEx using the OSVERSIONINFOEX structure.
   // If that fails, try using the OSVERSIONINFO structure.

   ZeroMemory(&osvi, sizeof(OSVERSIONINFOEX));
   osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);

   if(!(bOsVersionInfoEx = GetVersionEx((OSVERSIONINFO*)&osvi)))
   {
      // If OSVERSIONINFOEX doesn't work, try OSVERSIONINFO.
      osvi.dwOSVersionInfoSize = sizeof (OSVERSIONINFO);
      if(!GetVersionEx((OSVERSIONINFO*)&osvi)) 
			return NULL;
   }

   switch(osvi.dwPlatformId)
   {
      // Tests for Windows NT product family.
      case VER_PLATFORM_WIN32_NT:

			// Test for the product.
         if(osvi.dwMajorVersion <= 4)
            FEX_ADD_STR("Microsoft Windows NT ");
			
         if(osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 0)
            FEX_ADD_STR("Microsoft Windows 2000 ");

         if(bOsVersionInfoEx)  // Use information from GetVersionEx.
         { 
				// Test for the workstation type.
            if((*(myOSVERSIONINFOEX*)&osvi).wProductType == 0x0000001)//VER_NT_WORKSTATION)
            {
               if(osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 1)
                  FEX_ADD_STR("Microsoft Windows XP ");

               if((*(myOSVERSIONINFOEX*)&osvi).wSuiteMask & 0x00000200)//VER_SUITE_PERSONAL)
                  FEX_ADD_STR("Home Edition ");
               else
                  FEX_ADD_STR("Professional ");
            }
	         // Test for the server type.
            else 
				if((*(myOSVERSIONINFOEX*)&osvi).wProductType == 0x0000003)//VER_NT_SERVER)
            {
               if(osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 2)
                  FEX_ADD_STR("Microsoft Windows .NET ");

               if((*(myOSVERSIONINFOEX*)&osvi).wSuiteMask & 0x00000080)//VER_SUITE_DATACENTER)
                  FEX_ADD_STR("DataCenter Server ");
               else 
					{
						if((*(myOSVERSIONINFOEX*)&osvi).wSuiteMask & 0x00000002)//VER_SUITE_ENTERPRISE)
						{
							if(osvi.dwMajorVersion == 4)
								FEX_ADD_STR("Advanced Server ");
							else
								FEX_ADD_STR("Enterprise Server ");
						}
	               else 
						{
							if((*(myOSVERSIONINFOEX*)&osvi).wSuiteMask == 0x00000400)//VER_SUITE_BLADE)
								FEX_ADD_STR("Web Server ");
							else
								FEX_ADD_STR("Server ");
						}
					}
            }
         }
         else   // Use the registry on early versions of Windows NT.
         {
            HKEY hKey;
            char szProductType[BUFSIZE];
            DWORD dwBufLen=BUFSIZE;
            LONG lRet;

            lRet = RegOpenKeyEx(HKEY_LOCAL_MACHINE,
               "SYSTEM\\CurrentControlSet\\Control\\ProductOptions",
               0, KEY_QUERY_VALUE, &hKey);
            if(lRet != ERROR_SUCCESS)
                return NULL;

            lRet = RegQueryValueEx(hKey, "ProductType", NULL, NULL,
               (LPBYTE) szProductType, &dwBufLen);
            if((lRet != ERROR_SUCCESS) || (dwBufLen > BUFSIZE))
                return NULL;

            RegCloseKey(hKey);

            if(lstrcmpi("WINNT", szProductType) == 0)
               FEX_ADD_STR("Professional ");
            if(lstrcmpi("LANMANNT", szProductType) == 0)
               FEX_ADD_STR("Server ");
            if(lstrcmpi("SERVERNT", szProductType) == 0)
               FEX_ADD_STR("Advanced Server ");
         }

			// Display version, service pack (if any), and build number.
         if(osvi.dwMajorVersion <= 4)
         {
            FEX_ADD_STR("version %d.%d %s (Build %d)",
               osvi.dwMajorVersion,
               osvi.dwMinorVersion,
               osvi.szCSDVersion,
               osvi.dwBuildNumber & 0xFFFF);
         }
         else
         { 
            FEX_ADD_STR("%s (Build %d)",
               osvi.szCSDVersion,
               osvi.dwBuildNumber & 0xFFFF);
         }
         break;
      // Test for the Windows 95 product family.
      case VER_PLATFORM_WIN32_WINDOWS:
         if(osvi.dwMajorVersion == 4 && osvi.dwMinorVersion == 0)
         {
             FEX_ADD_STR("Microsoft Windows 95 ");
             if(osvi.szCSDVersion[1] == 'C' || osvi.szCSDVersion[1] == 'B')
                FEX_ADD_STR("OSR2 ");
         } 

         if(osvi.dwMajorVersion == 4 && osvi.dwMinorVersion == 10)
         {
             FEX_ADD_STR("Microsoft Windows 98 ");
             if(osvi.szCSDVersion[1] == 'A')
                FEX_ADD_STR("SE ");
         } 

         if(osvi.dwMajorVersion == 4 && osvi.dwMinorVersion == 90)
         {
             FEX_ADD_STR("Microsoft Windows Millennium Edition ");
         } 
         break;
   }
   return FEX_SystemVersionString;
}
//-----------------------------------------------------------------------------
char* FEX_GetDisplayName(void)
{
	struct DISPLAY_DEVICE_FULL
	{
		DWORD  cb;
		TCHAR  DeviceName[32];
		TCHAR  DeviceString[128];
		DWORD  StateFlags;
		TCHAR  DeviceID[128];
		TCHAR  DeviceKey[128];
	};
	DISPLAY_DEVICE_FULL dd;
   ZeroMemory(&dd, sizeof(dd));
   dd.cb = sizeof(dd);
	static char str[1024];
	str[0] = 0;

   HINSTANCE hUser32 = LoadLibrary("user32.dll");
   if(hUser32 == NULL)
   {
      OutputDebugString("Couldn't LoadLibrary user32.dll\r\n");
      return str;
   }

	typedef BOOL (WINAPI *EnumDisplayDevicesFn)(
		LPCTSTR lpDevice,               
		DWORD iDevNum,                  
		PDISPLAY_DEVICE lpDisplayDevice,
		DWORD dwFlags);

	EnumDisplayDevicesFn g_pfnEnumDisplayDevices;

	g_pfnEnumDisplayDevices = (EnumDisplayDevicesFn)GetProcAddress(hUser32, 
																					"EnumDisplayDevicesA");

	if(!g_pfnEnumDisplayDevices)
   {
      OutputDebugString("Couldn't GetProcAddress of EnumDisplayDevicesA\r\n");
      return str;
   }

   for(int i = 0; g_pfnEnumDisplayDevices(NULL, i, (DISPLAY_DEVICE*)&dd, 0); i++)
   {
		// Ignore NetMeeting's mirrored displays
      if((dd.StateFlags & DISPLAY_DEVICE_MIRRORING_DRIVER) == 0)
      {
			strcat(str, dd.DeviceString);
			strcat(str, "\n");
		}
   }
   FreeLibrary(hUser32);
	return str;
}
//-----------------------------------------------------------------------------
// Winmm.lib is required!
//-----------------------------------------------------------------------------
char* FEX_GetAudioName(void)
{
	WAVEINCAPS wic;
	static char str[(MAXPNAMELEN+1)*16+1] = "";
	for(int i = 0; i < 16; i++)
	{
		ZeroMemory(&wic, sizeof(WAVEINCAPS));
		if(waveInGetDevCaps(i, &wic, sizeof(WAVEINCAPS)) != MMSYSERR_NOERROR) break;
		strcat(str, wic.szPname);
		strcat(str, "\n");
	}
	return str;
}
//-----------------------------------------------------------------------------
