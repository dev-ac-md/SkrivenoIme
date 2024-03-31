#ifndef __GPDRAW_H__
#define __GPDRAW_H__

#ifndef GP_USER
#define GP_API __declspec(dllexport)
#else
#define GP_API __declspec(dllimport)
#endif
#pragma pack(1)
typedef unsigned short word;
typedef char* LPCHAR; 
#define MaxGPIdx 3000

class GP_API GP_Header{
public:
	int NextPict;
	short dx;
	short dy;
	short Lx;
	short Ly;
	byte* Pack;
	char  Options;
	DWORD CData;
	short NLines;
	int GetLx();
	int GetLy();
	int GetDx();
	int GetDy();
};
struct GP_API GP_GlobalHeader{
	int		 Sign;
	short	 NPictures;
	short    Reserved;
	int		 VocOffset;
	short    VocLength;
	//GP_Header* LGPH[256];
	DWORD    LGPH[256];
};
struct OneUnicodeSet{
	int DY;
	int DX;
	byte UseColor;
	int Start;
	int NSymbols;
	int GP_Start;
	int GPID;
};
struct UNICODETABLE{
	int NTables;
	OneUnicodeSet USET[4];
};
struct OneUniFont{
	char FontName[48];
	UNICODETABLE UTBL;
};
class GP_API UNIFONTS{
public:
	OneUniFont* UFONTS;
	int NFonts;
	UNIFONTS();
	~UNIFONTS();
	void LoadFonts();
	UNICODETABLE* FindFont(char* Name);
};
extern GP_API UNIFONTS UFONTS;
#define NO_PACK ((byte*)0xFFFFFFFF)
CEXPORT void ErrM(char* s);
typedef GP_GlobalHeader* lpGP_GlobalHeader;

#ifndef _USE3D
class GP_API GP_System{
private:
	word* _GPNFrames;

public:
	byte* PackCash;
	int CashSize;
	int CashPos;
	int NGP;
	int NGPReady;
	GP_GlobalHeader** GPH;
	DWORD** CASHREF;
	byte* Mapping;
	char** GPNames;
	RLCTable* RLCImage;
	RLCTable* RLCShadow;
	int* GPSize;
	int* GPLastTime;
	byte* ImageType;//0-not exist 1-GP file 2-RLC 3-RLC&Shadow 
	short** ImLx;
	short** ImLy;
	char** ItDX;
	char** ItLX;
	UNICODETABLE** UNITBL;
	GP_System();
	~GP_System();
	byte* GetCash(int Size);
	int PreLoadGPImage(char* Name);
	int PreLoadGPImage(char* Name,bool Shadow);
	bool LoadGP(int i);
	void  UnLoadGP(int i);
	int  GetGPWidth(int i,int n);
	int  GetGPNum() { return NGP; }
	const char* GPName( int FileIndex ) { return GPNames[FileIndex]; }
	int GetGPShift(int i,int n);
	int  GetGPHeight(int i,int n);
	int  GPNFrames( int FileIndex ) { return _GPNFrames[FileIndex]; }
	bool GetGPSize(int i,int n,int* Lx,int* Ly);
	void ShowGP(int x,int y,int FileIndex,int SprIndex,byte Nation);
	void ShowGPLayers(int x,int y,int FileIndex,int SprIndex,byte Nation,int mask);
	void ShowGPTransparent(int x,int y,int FileIndex,int SprIndex,byte Nation);
	void ShowGPTransparentLayers(int x,int y,int FileIndex,int SprIndex,byte Nation,int mask);
	void ShowGPPal(int x,int y,int FileIndex,int SprIndex,byte Nation,byte* Table);
	void ShowGPPalLayers(int x,int y,int FileIndex,int SprIndex,byte Nation,byte* Table,int mask);
	void ShowGPRedN(int x,int y,int FileIndex,int SprIndex,byte Nation,int N);
	void ShowGPDarkN(int x,int y,int FileIndex,int SprIndex,byte Nation,int N);
	void ShowGPDark(int x,int y,int FileIndex,int SprIndex,byte Nation);
	void ShowGPFired(int x,int y,int FileIndex,int SprIndex,byte Nation);
	void ShowGPMutno(int x,int y,int FileIndex,int SprIndex,byte Nation);
	void ShowGPGrad(int x,int y,int FileIndex,int SprIndex,byte Antion,byte* Table);
	void SetWhiteFont(int Gp_File);
	void SetRedFont(int Gp_File);
	void SetBlackFont(int Gp_File);
	void SetOptionalColor(int GP_File,int c);
	void FreeRefs(int i);
};

#endif // !_USE3D

#ifdef _USE3D
#pragma pack (push)
#pragma pack (8)

//-------------------------
#include "../3DLIB/gMotor/gmDefines.h"

#include "../3DLIB/gMotor/kAssert.h"

#include "../3DLIB/gMotor/kLog.h"
#include "../3DLIB/gMotor/kString.h"
#include "../3DLIB/gMotor/kArray.hpp"
#include "../3DLIB/gMotor/kHash.hpp"
#include "../3DLIB/gMotor/kResource.h"

#include "../3DLIB/gMotor/IRenderSystem.h"

#include "../3DLIB/gMotor/mMath3D.h"
#include "../3DLIB/gMotor/mCamera.h"

#include "../3DLIB/gMotor/kResource.h"
#include "../3DLIB/gMotor/kCache.h"
#include "../3DLIB/gMotor/kStatistics.h"

#include "../3DLIB/gMotor/gpBitmap.h"
#include "../3DLIB/gMotor/gpColor.h"
#include "../3DLIB/gMotor/gpMesh.h"
#include "../3DLIB/gMotor/gpBitmap.h"
//-------------------------
#include "../3DLIB/gMotor/gpPackage.h"
#include "../3DLIB/gMotor/gpSystem.h"

class GP_API GP_System : public GPSystem
{
public:
	GP_System();
	~GP_System();

	void	Init( IRenderSystem* irs );

	int		GetGPShift	( int FileIndex, int SprIndex );
	void	ShowGP		( int x, int y, int FileIndex, int SprIndex, byte Nation );
	void	ShowGPLayers( int x, int y, int FileIndex, int SprIndex, byte Nation, int mask);
	void	ShowGPTransparent( int x, int y, int FileIndex, int SprIndex ,byte Nation);
	void	ShowGPTransparentLayers(int x,int y,int FileIndex,int SprIndex,byte Nation,int mask);
	void	ShowGPPal(int x,int y,int FileIndex,int SprIndex,byte Nation,byte* Table);
	void	ShowGPPalLayers(int x,int y,int FileIndex,int SprIndex,byte Nation,byte* Table,int mask);
	void	ShowGPRedN(int x,int y,int FileIndex,int SprIndex,byte Nation,int N);
	void	ShowGPDarkN(int x,int y,int FileIndex,int SprIndex,byte Nation,int N);
	void	ShowGPDark(int x,int y,int FileIndex,int SprIndex,byte Nation);
	void	ShowGPFired(int x,int y,int FileIndex,int SprIndex,byte Nation);
	void	ShowGPMutno(int x,int y,int FileIndex,int SprIndex,byte Nation);
	void	ShowGPGrad(int x,int y,int FileIndex,int SprIndex,byte Antion,byte* Table);
	void	SetWhiteFont(int Gp_File);
	void	SetRedFont(int Gp_File);
	void	SetBlackFont(int Gp_File);
	void	SetOptionalColor(int GP_File,int c);
	const char* GPName( int FileIndex ) { return GetGPName( FileIndex ); }

	void	ShowGPAligned( int x, int y, float z, const Plane& plane, 
							int FileIndex, int SprIndex, byte Nation );

	void	ShowGPTransformed( int FileIndex, int SprIndex, const Matrix4D& transform, byte Nation );

	int		PreLoadGPImage	( char* gpName, bool Shadow );
	int		PreLoadGPImage	( char* gpName );

	bool LoadGP(int i);
	void  UnLoadGP(int i)
	{
		GPSystem::UnLoadGP(i);
	}

	int	 GetGPHeight( int seqID, int frameIdx )
	{
		return GPSystem::GetGPHeight( seqID, frameIdx );
	}

	int	 GPNFrames( int seqID )
	{
		if (seqID < 0 || seqID > GetGPNum() || seqID == -1)
		{
			return 1024;
		}
		return GPSystem::GPNFrames( seqID );	
	}
	int	 GetGPNum()
	{
		return GPSystem::GetGPNum();	
	}
	int	 GetGPWidth( int seqID, int frameIdx );

	void FlushBatch();
	
	UNICODETABLE**			UNITBL;			//  unicode table

	int						NGPReady;		//  number of loaded sequences
	GP_GlobalHeader**		GPH;			
	
	int*					GPLastTime;		//  LRU values ?
	byte*					ImageType;		//  used for storing color of the font
	xRLCTable**				RLCImage;		

	//  italic fonts stuff
	char**					ItDX;			
	char**					ItLX;
};  //  class GP_System

extern GP_API GP_System GPS;
#pragma pack (pop)
#endif // _USE3D


#ifndef _USE3D
#pragma pack(push)
#pragma pack(4)
extern GP_API GP_System GPS;
#pragma pack(pop)
#endif // !_USE3D

class GP_API LocalGP{
public:
	int GPID;
	bool Uniq;
//-----------------//
	LocalGP();
	LocalGP(char* Name);
	~LocalGP();
	void Load(char* Name);
	void UnLoad();
};
extern word GP_L_IDXS[MaxGPIdx];

#ifdef _COSSACKS2
CEXPORT void g_SetWindParam( int x, int y, int x1, int y1 );
CEXPORT void g_GetWindParam( int& x, int& y, int& x1, int& y1 );
#endif // _COSSACKS2

#endif // __GPDRAW_H__
