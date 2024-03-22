#include "ddini.h"
#include "ResFile.h"
#include "FastDraw.h"
#include "mgraph.h"
#include "mouse.h"
#include "menu.h"
#include "MapDiscr.h"
#include "fog.h"
#include "Megapolis.h"
#include <assert.h>
#include "walls.h"
#include "mode.h"
#include "GSound.h"
#include "MapSprites.h"
#include "NewMon.h"
#include "Math.h"
#include "GP_Draw.h"
#include "3DGraph.h"

#ifdef _USE3D
#include "NewMon3D.h" 

#pragma pack (push)
#pragma pack (8)
#include "..\3DLIB\gMotor\mCamera.h" 
extern Camera CAM;
#pragma pack (pop)
int g_CurTree = 0;
#endif // _USE3D

byte CurDrawNation=0;
class ZElement{
public:
	word FileID;
	word SpriteID;
	word FileIDEx;
	word SpriteIDEx;

	int Param1,Param2;
	byte Nat;
	short x,y;

#ifdef _USE3D
	float z;
	float scale;
#endif // _USE3D

	short XL,YL;
	OneObject* OB;
	word NPoints;
	bool IsDrawn;
};
extern bool CINFMOD;
ZElement ZCashe[8192];
int NZElm;
#define NYLines 2000
#define DYZBuf 256
#define ZSHF  6
#define ZBFNX (1<<ZSHF)
byte ZBFCounter[NYLines];
word ZBUF [NYLines*ZBFNX];
word LOLAYER[4096];
int NLOITEMS;
word HILAYER[1024];
int NHIITEMS;
short ZBUFX[NYLines*ZBFNX];
//debug only!
word ZBUF_OLD [NYLines*ZBFNX];
word ZBUFX_OLD [NYLines*ZBFNX];
//-----------
word BRSel[256];
int NBSel;
void ClearZBuffer(){
	memset(ZBFCounter,0,NYLines);
	NZElm=0;
	NLOITEMS=0;
	NHIITEMS=0;
	NBSel=0;
};

#ifdef _USE3D
extern float CamZf, CamZn;
extern int RealLy;
extern int mapy;

int zoomex( int ); 
int unzoomex( int val );
const float c_SpriteZShift = 128.0f;

float CalcWorldZ( int x, int y ) 
{ 
	return 1.0f - (
					(float)(y - mapy * 32) * 0.8660254037844386f + 
					GetHeight( x, y ) * 2.0f + 
					c_SpriteZShift) / (CamZf - CamZn);
}

float GetCurrentScale()
{
	return 1.0 / float(1<<(5-Shifter));
}

#endif // _USE3D

void AddHiPoint(short x,short y,OneObject* OB,word FileID,word SpriteID,int Param1,int Param2){
	if(NHIITEMS>=1024||NZElm>8000)return;
	//assert(SpriteID!=0xFFFF);
	ZElement* ZEL=ZCashe+NZElm;
	ZEL->x=x;
	ZEL->y=y;
	ZEL->FileID=FileID;
	ZEL->FileIDEx=0xFFFF;
	ZEL->SpriteID=SpriteID;
	ZEL->Param1=Param1;
	ZEL->Param2=Param2;
	ZEL->IsDrawn=false;	
	ZEL->Nat=CurDrawNation;
	ZEL->NPoints=0;

#ifdef _USE3D
	if (OB) ZEL->z = CalcWorldZ( OB->RealX / 16, OB->RealY / 16 );
#endif // _USE3D


	HILAYER[NHIITEMS]=NZElm;
	NHIITEMS++;
	NZElm++;
};
void AddSuperLoPoint(short x,short y,OneObject* OB,word FileID,word SpriteID,int Param1,int Param2){
	if(NLOITEMS>=4096||NZElm>8000)return;
	//assert(SpriteID!=0xFFFF);
	ZElement* ZEL=ZCashe+NZElm;
	ZEL->x=x;
	ZEL->y=y;
	ZEL->FileID=FileID;
	ZEL->FileIDEx=0xFFFF;
	ZEL->SpriteID=SpriteID;
	ZEL->Param1=Param1;
	ZEL->Param2=Param2;
	ZEL->OB=OB;
	ZEL->Nat=CurDrawNation;
	ZEL->IsDrawn=true;	
	ZEL->NPoints=0;

#ifdef _USE3D
	if (OB) ZEL->z = CalcWorldZ( OB->RealX / 16, OB->RealY / 16 );
#endif // _USE3D


	LOLAYER[NLOITEMS]=NZElm;
	NLOITEMS++;
	NZElm++;
};
void AddLoPoint(short x,short y,OneObject* OB,word FileID,word SpriteID,int Param1,int Param2){
	if(NLOITEMS>=4096)return;
	//assert(SpriteID!=0xFFFF);
	ZElement* ZEL=ZCashe+NZElm;
	ZEL->x=x;
	ZEL->y=y;
	ZEL->FileID=FileID;
	ZEL->FileIDEx=0xFFFF;
	ZEL->SpriteID=SpriteID;
	ZEL->Param1=Param1;
	ZEL->Param2=Param2;
	ZEL->OB=OB;
	ZEL->Nat=CurDrawNation;
	ZEL->IsDrawn=false;	
	ZEL->NPoints=0;

#ifdef _USE3D
	if (OB) ZEL->z = CalcWorldZ( OB->RealX / 16, OB->RealY / 16 );
#endif // _USE3D


	LOLAYER[NLOITEMS]=NZElm;
	NLOITEMS++;
	NZElm++;
};
int ZPointBias=0;
void SetZBias(int b){
	ZPointBias=b;
};
void AddPoint(short XL,short YL,short x,short y,OneObject* OB,word FileID,word SpriteID,int Param1,int Param2){
	if(NZElm>=8000)return;
	//assert(FileID<1024);
	//assert(SpriteID!=0xFFFF);
	YL+=DYZBuf;
	if(YL<0)YL=0;
	if(YL>=NYLines)YL=NYLines-1;
	int zb=ZBFCounter[YL];
	if(zb>=ZBFNX)return;
	ZElement* ZEL=ZCashe+NZElm;
	ZEL->x=x;
	ZEL->y=y;
	ZEL->XL=XL;
	ZEL->YL=YL;
	ZEL->FileID=FileID;
	ZEL->FileIDEx=0xFFFF;
	ZEL->SpriteID=SpriteID;
	ZEL->Param1=Param1;
	ZEL->Param2=Param2;
	ZEL->IsDrawn=false;
	ZEL->OB=OB;
	ZEL->Nat=CurDrawNation;
	ZEL->NPoints=0;

#ifdef _USE3D
	float rx, ry, rz;
	static const float c_Cos30 = cosf( c_PI / 6.0f );
	
	int sh = 5 - Shifter;
	float zoomScale = 1.0f;
	while (sh > 0) 
	{
		zoomScale /= 2.0f;
		sh >>= 1;
	}

	if (OB) 
	{
		rx = ((float)OB->RealX) / 16.0f;
		ry = ((float)OB->RealY) / 16.0f;
		rz = ((float)OB->RZ);
	}
	else 
	{
		if (Sprites)
		{
			OneSprite& OS = Sprites[g_CurTree];
			rx = OS.x;
			ry = OS.y;
			rz = OS.z;
			
			/*
			if (zoomScale == 1.0f)
			{
				Frame bound;
				GPS.GetGPBoundFrame( ZEL->FileID, ZEL->SpriteID, bound );
				float cX = -(x - XL)*zoomScale;
				float cY = -(y - YL + DYZBuf + rz)*zoomScale;

				Matrix4D trf;
				trf.e00 = 1.0f; trf.e12 = -1.0f; trf.e21 = 1.0f;

				trf.e30 = rx - cX; 
				trf.e31 = ry - (rz + cY)*0.5f; 
				trf.e32 = (rz + cY)*c_Cos30;

				BendVS vs;
				vs.setBase		( cX, cY				);
				vs.setHeight	( bound.h				);
				vs.setTime		( GetTickCount()		);
				vs.setTransform	( trf					);
				vs.setPhase		( (rx + ry) * 10.0f		);
				
				GPS.DrawSpriteVS( ZEL->FileID, ZEL->SpriteID, 0, vs );
				
				rx = -10000.0f;
				ry = -10000.0f;
			}
			*/
		}
		else
		{
			ZEL->z=0.0;
			int zz=(YL<<ZSHF)+zb;
			ZBUF[zz]=NZElm;
			ZBUFX[zz]=XL;
			ZBFCounter[YL]++;
			NZElm++;
			ZEL->scale=zoomScale;
			return;
			rx = 0.0f;
			ry = 0.0f;
		}
	}

	/*
	Vector4D pos( rx, ry, 0.0f, 1.0f );
	CAM.WorldToScreenSpace( pos );

	float gpH = (float)GPS.GetGPHeight( ZEL->FileID, ZEL->SpriteID & 4095 );
	Vector4D upos( 0.0f, 0.0f, gpH, 1.0f );
	upos.mul( CAM.getViewM() );
	upos.mul( CAM.getProjM() );
	upos.normW();
	zoomScale = upos.norm() * 0.5f;
	
	//pos.x *= zoomScale;
	//pos.y *= zoomScale;

	ZEL->x = pos.x + x - XL;
	ZEL->y = pos.y + y - YL + DYZBuf;
	ZEL->z = CalcWorldZ( rx, ry );
	ZEL->scale = zoomScale;
	*/
	
	Vector4D pos( rx, ry - rz*0.5f, rz*c_Cos30, 1.0f );
	CAM.WorldToScreenSpace( pos );

	pos.x *= zoomScale;
	pos.y *= zoomScale;

	/*
	double gpH = (float)GPS.GetGPHeight( FileID, SpriteID & 4095 );
	Vector4D tpos( rx, ry, gpH, 1.0f );
	tpos.mul( CAM.getViewM() );
	tpos.mul( CAM.getProjM() );
	tpos.normW(); 

	CAM.ProjectionToScreenSpace( tpos.x, tpos.y );
	tpos.x *= zoomScale;
	tpos.y *= zoomScale;

	Vector3D len( pos.x - tpos.x, pos.y - tpos.y&, pos.z - tpos.z ); 
	*/
	double scale = 1.0f;//((double)len.norm()) / gpH;

	float pivotX = XL - x;
	float pivotY = (YL - DYZBuf - rz*zoomScale) - y;

	ZEL->x = pos.x - pivotX;
	ZEL->y = pos.y - pivotY;
	ZEL->z = pos.z - (32.0f+ZPointBias)/(CamZf-CamZn);

	ZEL->scale = zoomScale;
	
#endif // _USE3D


	//adding to Z-Buffer
	int zz=(YL<<ZSHF)+zb;
	ZBUF[zz]=NZElm;
	ZBUFX[zz]=XL;
	ZBFCounter[YL]++;
	NZElm++;
	
};
void AddPointEx(short XL,short YL,short x,short y,OneObject* OB,word FileID,word SpriteID,word FileIDex,word SpriteIDex,int Param1,int Param2){
	if(NZElm>=8000)return;
	//assert(FileID<1024);
	//assert(SpriteID!=0xFFFF);
	YL+=DYZBuf;
	if(YL<0)YL=0;
	if(YL>=NYLines)YL=NYLines-1;
	int zb=ZBFCounter[YL];
	if(zb>=ZBFNX)return;
	ZElement* ZEL=ZCashe+NZElm;
	ZEL->x=x;
	ZEL->y=y;
	ZEL->XL=XL;
	ZEL->YL=YL;

	ZEL->FileID=FileID;
	ZEL->SpriteID=SpriteID;
	ZEL->FileIDEx=FileIDex;
	ZEL->SpriteIDEx=SpriteIDex;

	ZEL->Param1=Param1;
	ZEL->Param2=Param2;
	ZEL->IsDrawn=false;
	ZEL->OB=OB;
	ZEL->Nat=CurDrawNation;
	ZEL->NPoints=0;

#ifdef _USE3D
	if (OB) ZEL->z = CalcWorldZ( OB->RealX / 16, OB->RealY / 16 );
#endif // _USE3D


	//adding to Z-Buffer
	int zz=(YL<<ZSHF)+zb;
	ZBUF[zz]=NZElm;
	ZBUFX[zz]=XL;
	ZBFCounter[YL]++;
	NZElm++;
};
//Param1 values:
//bits 0..2-visibility options
//0-simple
//1-pulsing(Param2-pointer to pulsing data)
//2-transparent(Param2-pointer to transparency data)
//3-encoded with palette(Param2-pointer to palette)
//4-shading with mask(Param2-pointer to gradient data)
extern byte fog[8192+1024];
extern byte wfog[8192];
extern byte rfog[8192];
extern byte darkfog[40960];
extern byte yfog[8192];
extern byte trans8[65536];
void AddOptPoint(byte Method,short XL,short YL,short x,short y,OneObject* OB,word FileID,word Sprite,int Options){
#ifdef _USE3D
	AddPoint(XL,YL,x,y,OB,FileID,Sprite,0,0);
	return;
#endif
	//assert(Sprite!=0xFFFF);
    int CType=Options&0x0F;
    int CProp=Options&0xF0;
    int Param1=Options&0xFFFFF0F;
    int Param2=0; 
    switch(CType){
    case AV_PULSING:
        switch(CProp){
        case AV_RED:
            Param2=int(yfog);
            break;
        case AV_WHITE:
            Param2=int(wfog);
            break;
        case AV_DARK:
            Param2=int(fog+1024);
            break;
        default:
            Param1=0;
        };
        break;
    case AV_TRANSPARENT:
        break;
    case AV_PALETTE:
        switch(CProp){
        case AV_RED:
            Param2=int(yfog+2048);
            break;
        case AV_WHITE:
            Param2=int(wfog+2048);
            break;
        case AV_DARK:
            Param2=int(fog+1024+2048);
            break;
        default:
            Param1=0;
        };
        break;
    case AV_GRADIENT:
        switch(CProp){
        case AV_RED:
            Param2=int(yfog);
            break;
        case AV_WHITE:
            Param2=int(wfog);
            break;
        case AV_DARK:
            Param2=int(fog+1024);
            break;
        default:
            Param1=0;
        };
        break;
    };
	switch(Method){
	case 1:
		AddLoPoint(x,y,OB,FileID,Sprite,Param1,Param2);
		break;
	case 2:
		AddHiPoint(x,y,OB,FileID,Sprite,Param1,Param2);
		break;
	default:
		AddPoint(XL,YL,x,y,OB,FileID,Sprite,Param1,Param2);
		break;
	};
};
void AddLine(short X1,short Y1,short X2,short Y2,short x,short y,OneObject* OB,word FileID,word SpriteID,int Param1,int Param2);
void AddOptLine(short X1,short Y1,short X2,short Y2,short x,short y,OneObject* OB,word FileID,word SpriteID,int Options){
	int CType=Options&0x0F;
    int CProp=Options&0xF0;
    int Param1=Options&0xFFFFF0F;
    int Param2=0;
    switch(CType){
    case AV_PULSING:
        switch(CProp){
        case AV_RED:
            Param2=int(yfog);
            break;
        case AV_WHITE:
            Param2=int(wfog);
            break;
        case AV_DARK:
            Param2=int(fog+1024);
            break;
        default:
            Param1=0;
        };
        break;
    case AV_TRANSPARENT:
        break;
    case AV_PALETTE:
        switch(CProp){
        case AV_RED:
            Param2=int(yfog+2048);
            break;
        case AV_WHITE:
            Param2=int(wfog+2048);
            break;
        case AV_DARK:
            Param2=int(fog+1024+2048);
            break;
        default:
            Param1=0;
        };
        break;
    case AV_GRADIENT:
        switch(CProp){
        case AV_RED:
            Param2=int(yfog);
            break;
        case AV_WHITE:
            Param2=int(wfog);
            break;
        case AV_DARK:
            Param2=int(fog+1024);
            break;
        default:
            Param1=0;
        };
        break;
    };
	AddLine(X1,Y1,X2,Y2,x,y,OB,FileID,SpriteID,Param1,Param2);
};
void AddLine(short X1,short Y1,short X2,short Y2,short x,short y,OneObject* OB,word FileID,word SpriteID,int Param1,int Param2){
	if(Y1==Y2){
		AddPoint(X1,Y1,x,y,OB,FileID,SpriteID,Param1,Param2);
		return;
	};
	if(abs(X1-X2)<4){
		AddPoint((X1+X2)>>1,(Y1+Y2)>>1,x,y,OB,FileID,SpriteID,Param1,Param2);
		return;
	};
	if(NZElm>=8000)return;
	Y1+=DYZBuf;
	Y2+=DYZBuf;
	if(Y1<0)Y1=0;
	if(Y1>=NYLines)Y1=NYLines-1;
	if(Y2<0)Y2=0;
	if(Y2>=NYLines)Y2=NYLines-1;
	if(X1>X2){
		X1+=X2;X2=X1-X2;X1-=X2;
		Y1+=Y2;Y2=Y1-Y2;Y1-=Y2;
	};
	ZElement* ZEL=ZCashe+NZElm;
	ZEL->x=x;
	ZEL->y=y;
	ZEL->FileID=FileID;
	ZEL->FileIDEx=0xFFFF;
	ZEL->SpriteID=SpriteID;
	ZEL->Param1=Param1;
	ZEL->Param2=Param2;
	ZEL->IsDrawn=false;
	ZEL->OB=OB;
	ZEL->YL=Y1<Y2?Y1:Y2;
	ZEL->XL=X1;
	ZEL->Nat=CurDrawNation;
	//adding to Z-Buffer
	word Mas=0x8000;
	if(Y1<Y2)Mas|=0x2000;
	else Mas|=0x4000;
	int X0=X1<<16;
	int DD=abs(Y2-Y1+1);
	if(!DD)DD=1;
	int DX=((X2-X1)<<16)/DD;
	int zz=(Y1<<ZSHF);
	if(Y2>Y1){
		int NPT=0;
		for(int YY=Y1;YY<=Y2;YY++){
			int zb=ZBFCounter[YY];
			if(zb<ZBFNX){
				NPT++;
				ZBUF[zz+zb]=NZElm|Mas;
				ZBUFX[zz+zb]=X0>>16;
				X0+=DX;
				zz+=ZBFNX;
				ZBFCounter[YY]++;
			};
		};
		ZEL->NPoints=NPT;
	}else{
		int NPT=0;
		for(int YY=Y1;YY>=Y2;YY--){
			int zb=ZBFCounter[YY];
			if(zb<ZBFNX){
				NPT++;
				ZBUF[zz+zb]=NZElm|Mas;
				ZBUFX[zz+zb]=X0>>16;
				X0+=DX;
				zz-=ZBFNX;
				ZBFCounter[YY]++;
			};
		};
		ZEL->NPoints=NPT;
	};
	NZElm++;
};
void SortWords(int NWords,word* Data,short* Factor){
	if(NWords<2)return;
	__asm{
		push	esi
		push	edi
SW0:	mov		edi,Data
		mov		esi,Factor
		mov		ebx,1
		mov		ecx,NWords;
		dec		ecx
SW1:	mov		ax,[esi+2]
		cmp		ax,[esi]
		jge		SW3
		mov		dx,[esi]
		mov		[esi+2],dx
		mov		[esi],ax
		mov		ax,[edi]
		mov		dx,[edi+2]
		mov		[edi],dx
		mov		[edi+2],ax
		xor		ebx,ebx
SW3:	add		esi,2
		add		edi,2
		dec		ecx
		jnz		SW1
		or		ebx,ebx
		jz		SW0
		pop		edi
		pop		esi
	};
};
void SortZBuffer(){
	int pos=0;
	short TEMPLINE[64];
	int tofs=int(TEMPLINE);
	for(int i=0;i<NYLines;i++){
		int N=ZBFCounter[i];
		if(N>1){
			word* ZPOS=ZBUF+pos;
			SortWords(N,ZBUF+pos,ZBUFX+pos);
		};
		pos+=ZBFNX;
	};
};
extern bool TransMode;
void DrawMarker(OneObject* OB);
void DrawImMarker(OneObject* OB);
void PtLine(int x,int y,int x1,int y1,byte c){
	int DD=int(sqrt((x-x1)*(x-x1)+(y-y1)*(y-y1)));
	if(!DD)return;
	int NP=(DD>>5)+1;
	for(int i=0;i<=NP;i++){
		Xbar(x+div((x1-x)*i,NP).quot-1,y+div((y1-y)*i,NP).quot-1,3,3,c);
	};
};
void DrawColorMarker(OneObject* OB,byte cl);
void DrawBorder(Brigade* BR);
void DrawHealth(OneObject* OB);
extern bool HealthMode;
void DrawGrassNearUnit(int x,int y);
void RegisterVisibleGP(word Index,int FileIndex,int SprIndex,int x,int y);
void ShowZElement(ZElement* ZEL){
#ifdef _USE3D
	extern float g_dbgZ;
	//  second term is added to prevent z-fighting between units standing in line
	g_dbgZ = ZEL->z + 1.0f*float( ZEL->x % 128 )/65536.0f;

	char buf[64];
	sprintf( buf, "%f", ZEL->z );
	//GPS.DrawString( buf, ZEL->x, ZEL->y, ZEL->z, 0xFF00FF00 );
	//GPS.FlushBatches();
	OneObject* ob = ZEL->OB;
	if (ob)
	{
		NewMonster* nm = ob->newMons;
		if (nm)
		{
			NewMon3D* nm3d = nm->Monster3D;
			if (nm3d)
			{
				return;
			}
			if(ob->Sdoxlo>10){
				g_dbgZ+=256.0/65535.0;
				float a=255.0f-float(ob->Sdoxlo-10)/90.0f;
				if(a>128.0f){
					a=255.0f-(255.0f-a)*16.0f;
					if(a<128)a=128;
				};
				int d=a;
				int dc=(d+511)/3;
				if(d<0)d=0;
				DWORD D=(d<<24)+(dc<<16)+(dc<<8)+dc;//0xFFFFFF;
				GPS.SetCurrentDiffuse(D);
				int DY=-ob->NewAnm->Frames[0].dy;
				Plane PL;
				float dz=1.0f/(CamZf-CamZn);
				float z0=float(ob->Index&15)/16384;
				PL.from3Points(Vector3D(0,DY,z0),Vector3D(1,DY,z0),Vector3D(0,DY-1,z0+dz*2));
				GPS.ShowGPAligned(
					smapx+ZEL->x,
					smapy+ZEL->y,
					g_dbgZ,PL,
					ZEL->FileID,
					ZEL->SpriteID,
					ob->NNUM);
				GPS.SetCurrentDiffuse(0xFFFFFFFF);
				return;
			};
		}
	}
#endif // _USE3D

	ZEL->IsDrawn=true;
	if(ZEL->Param1==0xFFFF&&ZEL->Param2==0xFFFF){
		//assert(ZEL->FileID>=0);
#ifndef _USE3D
		DrawTriangleElement(ZEL->FileID,ZEL->SpriteID);
#endif
		return;
	};

#ifdef _USE3D
		GPS.SetScale( ZEL->scale );	
#endif // _USE3D

	if(LMode){
#ifdef _USE3D
		
#else
		ZEL->FileID=GP_L_IDXS[ZEL->FileID];
		if(!ZEL->FileID)return;
#endif
	};
    int par1=ZEL->Param1&0xF;
	int mask=ZEL->Param1>>9;
	if(!(ZEL->Param1&256))mask=0xFFFF;
    int par2=ZEL->Param2;
    byte nat=ZEL->Nat;
	//Shadows
    bool LocTrans=false;
	OneObject* OB=ZEL->OB;
	
    if(OB){
		RegisterVisibleGP(OB->Index,ZEL->FileID,ZEL->SpriteID,ZEL->x,ZEL->y);
        nat=OB->NNUM;
		if(OB->PathDelay){
			//DrawColorMarker(OB,255);
		};
		/*
        if(OB->Selected&&OB->NNUM==MyNation){
			if(OB->BrigadeID!=0xFFFF){
				Brigade* BR=OB->Nat->CITY->Brigs+OB->BrigadeID;
				if(!BR->WarType)DrawMarker(OB);
			}else{
				//if(OB->NewBuilding)par1=7432;
				//else 
				DrawMarker(OB);

			};
        };
		*/
#ifndef _USE3D
		if(OB->ImSelected&GM(MyNation)&&!ZPointBias){
			if(OB->BrigadeID!=0xFFFF&&!LMode){
				Brigade* BR=OB->Nat->CITY->Brigs+OB->BrigadeID;
				if(!BR->WarType)DrawMarker(OB);
			}else{
				if(OB->NewBuilding)par1=7432;
				else if(LMode)par1=7433;
				else DrawMarker(OB);
#ifdef COSSACKS2
				//void DrawForceOnUnit(OneObject* OB);
				//DrawForceOnUnit(OB);
#endif

			};
        };
#endif //_USE3D
		if(CINFMOD&&OB->NewBuilding&&!LMode){
			NewMonster* NM=OB->newMons;
			int CX=OB->RealX>>4;
			int CY=OB->RealY>>4;
			int X0=CX+NM->BuildX0;
			int Y0=CY+NM->BuildY0;
			int X1=CX+NM->BuildX1;
			int Y1=CY+NM->BuildY1;
			int D=(Y1-Y0+X1-X0)>>1;
			int D2=D>>1;
			int mpdx=mapx<<5;
			int mpdy=(mapy<<4)+OB->RZ;
			Y0>>=1;
			Y1>>=1;
			X0-=mpdx;Y0-=mpdy;
			X1-=mpdx;Y1-=mpdy;
			PtLine(X0,Y0,X0+D,Y0+D2,0xB0);
			PtLine(X0,Y0,X1-D,Y1-D2,0xB0);
			PtLine(X1,Y1,X0+D,Y0+D2,0xB0);
			PtLine(X1,Y1,X1-D,Y1-D2,0xB0);
		};
	};
    if(TransMode&&OB&&OB->NewBuilding)LocTrans=true;
	//if(ZEL->FileID==TREES.FileID)par1=AV_ROUGH;
	try{
		do{
			switch(par1){
			case AV_NORMAL:
				if(par2>64){
					if(LocTrans)GPS.ShowGPTransparentLayers(smapx+ZEL->x,smapy+ZEL->y,ZEL->FileID,ZEL->SpriteID,par2-64,mask);
					else GPS.ShowGPLayers(smapx+ZEL->x,smapy+ZEL->y,ZEL->FileID,ZEL->SpriteID,par2-64,mask);
				}else{
					if(LocTrans)GPS.ShowGPTransparentLayers(smapx+ZEL->x,smapy+ZEL->y,ZEL->FileID,ZEL->SpriteID,nat,mask);
					else GPS.ShowGPLayers(smapx+ZEL->x,smapy+ZEL->y,ZEL->FileID,ZEL->SpriteID,nat,mask);
				};
				break;
			case 7432:
				{
					int NNN=8+int(4*(sin(double(GetTickCount())/114)));
					GPS.ShowGPPalLayers(smapx+ZEL->x,smapy+ZEL->y,ZEL->FileID,ZEL->SpriteID,nat,wfog+(NNN<<8),mask+65536);
				};
				break;
			case 7433:
				{
					GPS.ShowGPPalLayers(smapx+ZEL->x,smapy+ZEL->y,ZEL->FileID,ZEL->SpriteID,8,wfog+(8<<8),mask);
				};
				break;
			case AV_PULSING:
				{
					int NNN=5+int(8*(sin(double(GetTickCount())/100)+2));
					GPS.ShowGPPalLayers(smapx+ZEL->x,smapy+ZEL->y,ZEL->FileID,ZEL->SpriteID,nat,(byte*)par2+(NNN<<8),mask);
				};
				break;
			case AV_TRANSPARENT:
				GPS.ShowGPTransparentLayers(smapx+ZEL->x,smapy+ZEL->y,ZEL->FileID,ZEL->SpriteID,nat,mask);
				break;
			case AV_PALETTE:
				GPS.ShowGPPalLayers(smapx+ZEL->x,smapy+ZEL->y,ZEL->FileID,ZEL->SpriteID,nat,(byte*)par2,mask);
				break;
			case AV_GRADIENT:
				GPS.ShowGPGrad(smapx+ZEL->x,smapy+ZEL->y,ZEL->FileID,ZEL->SpriteID,nat,(byte*)par2);
				break;
			case AV_ROUGH:
				GPS.ShowGPLayers(smapx+ZEL->x,smapy+ZEL->y,ZEL->FileID,ZEL->SpriteID,nat,1+512+16+2048);
				break;
			};
			if(ZEL->FileIDEx!=0xFFFF){
				ZEL->FileID=ZEL->FileIDEx;
				ZEL->SpriteID=ZEL->SpriteIDEx;
				ZEL->FileIDEx=0xFFFF;
				ZEL->SpriteIDEx=0xFFFF;
			}else{
				ZEL->FileID=0xFFFF;
			};
		}while(ZEL->FileID!=0xFFFF);
	}catch(...){};
	if(OB&&HealthMode){
		DrawHealth(OB);
	};
	if(OB&&!OB->NewBuilding){
		int xx=(OB->RealX>>4)-(mapx<<5);
		int yy=(OB->RealY>>5)-(mapy<<4)-OB->RZ;
		DrawGrassNearUnit(xx,yy);
	};
#ifdef _USE3D
	GPS.SetCurrentDiffuse(0xFFFFFFFF);
#endif //_USE3D

};
int mul3(int);
void Clean_GP_IMG();
extern int RealLx;
extern int RealLy;
void DrawReflect();
void PROSESS_WAVES();
void DRAW_WAVES();
#define ZMASK 8191
#ifdef _USE3D

void ShowZInternal( bool filterTrees = false )
{
	for (int i = 0; i < NZElm; i++)
	{
		ZCashe[i].IsDrawn = 0;
	}
	for (int i = 0; i < NYLines; i++)
	{
		for (int j = 0; j < ZBFCounter[i]; j++)
		{
			WORD zelIdx = ZBUF[i*ZBFNX + j];
			ZElement& ZEL = ZCashe[zelIdx];
			if (filterTrees && 
					(
					!ZEL.OB //||						//  static object, such as tree
					 //ZEL.OB->BrigadeID == 0xFFFF	//  unit is not in formation, so ignore alpha-artefacts
					 )	
				) continue;

			WORD FileID = ZEL.FileID;
			if (ZEL.FileID < 0xFFFF && !ZEL.IsDrawn) 
			{
				ShowZElement( &ZEL );
				ZEL.IsDrawn = 1;
			}
			ZEL.FileID = FileID;
		}
	}
	GPS.FlushBatches();
	GPS.SetScale(1.0);
} // ShowZInternal

#endif //_USE3D
void ShowZBuffer(){
#ifdef _USE3D
	GPBatchMode oldMode = GPS.SetBatchMode( bmSortByTexture );
	//GPS.SetScale(1.0/float(1<<(5-Shifter)));
#endif // _USE3D
	DrawReflect();
	PROSESS_WAVES();
	DRAW_WAVES();
#ifndef _USE3D
	Clean_GP_IMG();
#endif
	memcpy(ZBUF_OLD,ZBUF,sizeof(ZBUF));
	memcpy(ZBUFX_OLD,ZBUFX,sizeof(ZBUFX));
	SortZBuffer();
#ifdef _USE3D
	ShowZInternal();
#else //_USE3D
	int Lx1=RealLx;
	int Ly1=RealLy;
	SetRLCWindow(smapx,smapy,Lx1,Ly1,SCRSizeX);
	for(int i=0;i<NLOITEMS;i++){
		ZElement* ZEL=ZCashe+LOLAYER[i];
		if(ZEL->IsDrawn)ShowZElement(ZEL);
	};
	for(int i=0;i<NLOITEMS;i++){
		ZElement* ZEL=ZCashe+LOLAYER[i];
		if(!ZEL->IsDrawn)ShowZElement(ZEL);
	};
	int NITR=0;
	bool Done;
	do{
		Done=1;
		NITR++;
		int YMIN=0;
		int YMAX=NYLines-1;
		int XMIN=-100000;
		int XMAX=100000;
		bool FindXMin=false;
		bool FindXMax=false;
		do{
			int nn=ZBFCounter[YMIN];
			if(nn){
				word* OIND=ZBUF+YMIN*ZBFNX;
				short* OINDX=ZBUFX+YMIN*ZBFNX;
				if(FindXMin){
					for(int j=0;j<nn;j++){
						word SOBJ=OIND[j];
						if(SOBJ!=0xFFFF){
							ZElement* ZEL=ZCashe+(SOBJ&ZMASK);
							if(!ZEL->IsDrawn){
								if(OINDX/*ZBUFX*/[j]<XMIN&&(SOBJ&0x6000)==0x2000){
									XMIN=OINDX/*ZBUFX*/[j];
									FindXMin=0;
								};
							}else OIND[j]=0xFFFF;
						};
					};
					if(FindXMin){
						FindXMin=false;
						XMIN=-100000;
					};
				};
				if(FindXMax){
					for(int j=nn-1;j>=0;j--){
						word SOBJ=OIND[j];
						if(SOBJ!=0xFFFF){
							ZElement* ZEL=ZCashe+(SOBJ&ZMASK);
							if(!ZEL->IsDrawn){
								if(OINDX/*ZBUFX*/[j]>XMAX&&(SOBJ&0x6000)==0x4000){
									XMAX=OINDX/*ZBUFX*/[j];
									FindXMax=0;
								};
							}else OIND[j]=0xFFFF;
						};
					};
					if(FindXMax){
						FindXMax=false;
						XMAX=100000;
					};
					FindXMax=false;
				};
				for(int j=0;j<nn;j++){
					word SOBJ=OIND[j];
					if(SOBJ!=0xFFFF){
						ZElement* ZEL=ZCashe+(SOBJ&ZMASK);
						if(!ZEL->IsDrawn){
							int xx=OINDX[j];
							if(XMAX<XMIN){
								XMAX=XMIN+XMAX;
								XMIN=XMAX-XMIN;
								XMAX=XMAX-XMIN;
							}
							if(xx>=XMIN&&xx<=XMAX){
								if(SOBJ&0x6000){
									word w=SOBJ&0x6000;
									if(w==0x2000){
										XMIN=xx;
										if(ZEL->NPoints>1)ZEL->NPoints--;
										else{
											ShowZElement(ZEL);
											ZEL->IsDrawn=1;
											OIND[j]=0xFFFF;
											YMIN=ZEL->YL-1;
											XMIN=ZEL->XL;
											XMAX=ZEL->XL;
											FindXMin=true;
											FindXMax=true;
											OIND[j]=0xFFFF;
										};
									}else
									if(w==0x4000){
										XMAX=xx;
										if(ZEL->NPoints>1)ZEL->NPoints--;
										else{
											ShowZElement(ZEL);
											ZEL->IsDrawn=1;
											OIND[j]=0xFFFF;
											YMIN=ZEL->YL-1;
											XMIN=ZEL->XL;
											XMAX=ZEL->XL;
											FindXMin=true;
											FindXMax=true;
											OIND[j]=0xFFFF;
										};
									};
								}else{
									ShowZElement(ZEL);
									ZEL->IsDrawn=1;
									OIND[j]=0xFFFF;
								};
							};
						}else OIND[j]=0xFFFF;
					};
				};
			}else{
				if(FindXMin){
					XMIN=-100000;
					FindXMin=0;
				};
				if(FindXMax){
					XMAX=100000;
					FindXMax=0;
				};
			};
			YMIN++;
		}while(YMIN<YMAX);
		YMIN=10000;
		ZElement* ZEL=ZCashe;
		int MinIdx=0;
		for(int j=0;j<NZElm;j++){
			if((!ZEL->IsDrawn)&&ZEL->NPoints){
				if(YMIN>ZEL->YL){
					YMIN=ZEL->YL;
					MinIdx=j;
				};
			};
			ZEL++;
		};
		if(YMIN<10000){
			Done=false;
			ZElement* ZEL=ZCashe+MinIdx;
			ShowZElement(ZEL);
			ZEL->NPoints=0;
			ZEL->IsDrawn=1;
		};
	}while(NITR<15&&!Done);
	for(int i=0;i<NHIITEMS;i++)ShowZElement(ZCashe+HILAYER[i]);
#endif //_USE3D
	if(CINFMOD){
		for(int i=DYZBuf;i<1600;i++){
			if(ZBFCounter[i]){
				int n=ZBFCounter[i];
				int pos=i<<ZSHF;
				for(int j=0;j<n;j++){
					word ZT=ZBUF_OLD[pos+j]&0x6000;
					int xx=ZBUFX_OLD[pos+j];
					int yy=i-DYZBuf;
					switch(ZT){
					case 0:
						Xbar(xx-1,yy-1,3,3,0xD0);
						break;
					case 0x2000:
						Xbar(xx-1,yy-1,3,3,0xD0);//clrBlue);
						break;
					case 0x4000:
						Xbar(xx-1,yy-1,3,3,0xD0);
						break;
					};
				};
			};
		};
	};
#ifdef _USE3D
	GPS.SetBatchMode( oldMode );
	GPS.SetScale(1.0);
	void DrawAllGModels();
	DrawAllGModels();
	void TestGModels();
	TestGModels();

	void PiroDraw();
	PiroDraw();

#endif // _USE3D
};

//!!