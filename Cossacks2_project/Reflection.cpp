#include "ddini.h"
#include "ResFile.h"
#include "FastDraw.h"
#include "mgraph.h"
#include "mouse.h"
#include "menu.h"
#include "MapDiscr.h"
#include "ActiveScenary.h"
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
#include "RealWater.h"
#include "NewUpgrade.h"
#include "ZBuffer.h"
#include "Path.h"
#include "Transport.h"
#include "3DBars.h"
#include "CDirSnd.h"
#include "NewAI.h"
#include "3DMapEd.h"
#include "TopoGraf.h"
#include "Towers.h"
#include "Fonts.h"
#include "Safety.h"
#include "3DGraph.h"
#include "Nature.h"
#include "ConstStr.h"
#include "StrategyResearch.h"
#include "EinfoClass.h"
#include "Sort.h"
#include "drawform.h"
#include "VirtScreen.h"
#include "bmptool.h"
struct OneReflection{
	bool Shown:1;
	bool Added:1;
	word HASH;
	int Nx,Ny;
	int x,y;
	byte Data[128];
};
class ReflectEngine{
public:
	OneReflection** RefGrp;
	//search hash
	byte REF_HASH[4096];
	int NRefGrp;
	int MaxRefGrp;
	void Process();
	void AddGP_Reflection(int xg,int yg,int xc,int yc,int GP_File,int Sprite);
	void Clear();
	void ShowAll();
	ReflectEngine();
	~ReflectEngine();
};
ReflectEngine::ReflectEngine(){
	memset(this,0,sizeof ReflectEngine);
	memset(REF_HASH,0xFF,sizeof REF_HASH);
};
void ReflectEngine::Clear(){
};
ReflectEngine::~ReflectEngine(){
	Clear();
	memset(this,0,sizeof ReflectEngine);
};
void SETWINDX(int x,int y,int lx,int ly);
int GetDeep(int x,int y);
bool PutWaterReflectionMask(byte* buf,int z1,int z2,int z3,int z4,int width){
	//z1-=19;
	//z2-=19;
	//z3-=19;
	//z4-=19;
	if(z1>0&&z2>0&&z3>0&&z4>0)return false;
	if(z1<0&&z2<0&&z3<0&&z4<0)return true;
	else{
		z1<<=9;
		z2<<=9;
		z3<<=9;
		z4<<=9;
		int sdx=(z2-z1)>>5;
		int sdy=(z3-z1)>>4;
		int ddx=(z1+z4-z2-z3)>>9;
		__asm{
			push edi
			mov  edi,buf
			cld
			mov  edx,z1
			mov  ebx,sdx
			mov  ecx,0x1020
LPP3:		cmp  edx,0
			jge  LPP4
			mov  [edi],0
LPP4:		inc  edi
			add  edx,ebx
			dec  cl
			jnz  LPP3
			mov  ebx,sdx
			add  ebx,ddx
			mov  sdx,ebx
			mov  edx,z1
			add  edx,sdy
			mov  z1,edx
			mov  cl,32
			add  edi,width
			sub  edi,32
			dec  ch
			jnz  LPP3
			pop  edi
		};
	};
	return false;
};
int PackReflDataTo(byte* Buf,byte* Src,int Width){
	Src-=Width;
	int pos=0;
	bool ppprs=0;
	byte* bstart=Buf+1;
	for(int iy=0;iy<16;iy++){
		byte* src=Src-iy*Width;
		byte* btmp=bstart+1;
		bstart[0]=0;
		pos=0;
		int L=pos;
		while(pos<32&&!src[pos])pos++;
		if(pos<32){
			btmp[0]=pos-L;
			btmp++;
			L=pos;
			while(pos<32&&src[pos])pos++;
			btmp[0]=pos-L;
			btmp++;
			if(pos!=L){
				memcpy(btmp,src+L,pos-L);
				btmp+=pos-L;
				ppprs=1;
				bstart[0]=1;
			};
			while(pos<32){
				L=pos;
				while(pos<32&&!src[pos])pos++;
				if(pos<32){
					btmp[0]=pos-L;
					btmp++;
					L=pos;
					while(pos<32&&src[pos])pos++;
					btmp[0]=pos-L;
					btmp++;
					if(pos!=L){
						memcpy(btmp,src+L,pos-L);
						btmp+=pos-L;
						ppprs=1;
						bstart[0]++;
					};
				};
			};
			bstart=btmp;
		}else{
			bstart[0]=0;
			bstart++;
		};
	};
	if(ppprs){
		Buf[0]=16;
		return bstart-Buf;
	}else{
		Buf[0]=0;
		return 1;
	};
};
void GetRealGPSize(int File,int spr,int* dx,int* dy);
void SaveBMP8(char* Name,int lx,int ly,byte* Data);

extern int XShift[1024];
extern byte trans4[65536];
extern byte trans8[65536];
int START_EBX=0;
void ShowOneReflSquare_8(int x,int y,byte* Buf){
	int xs0=START_EBX;
	if(!Buf[0])return;
	int sofs=int(ScreenPtr)+x+y*ScrWidth;
	int tmp;
	__asm{
		push esi
		push edi
		mov  edx,xs0
		shl  edx,2
		mov  esi,Buf
		inc  esi
		mov  edi,sofs
		mov  bl,16
		xor  eax,eax
		xor  ecx,ecx
		cld
LPP1:	mov  tmp,edi
		add  edx,4
		add  edi,[XShift+edx]
		mov  bh,[esi]
		inc  esi
		or   bh,bh
		jz   LPP3
LPP2:	mov  cl,[esi]
		add  edi,ecx
		mov  cl,[esi+1]
		add  esi,2
		jcxz LPP8 
LPP7:	mov  ah,[edi]
		mov  al,[esi]
		inc  esi
		mov  al,[trans4+eax]
		mov  [edi],al
		inc  edi
		dec  cl
		jnz  LPP7
LPP8:
		dec  bh
		jnz  LPP2
		mov  edi,tmp
		add  edi,ScrWidth
		dec  bl
		jnz  LPP1
		jmp  LPP4
LPP3:	mov  edi,tmp
		add  edi,ScrWidth
		dec  bl
		jnz  LPP1
LPP4:
		pop  edi
		pop  esi
	};
};
void ShowOneReflSquare_4(int x,int y,byte* Buf){
	int xs0=START_EBX;
	if(!Buf[0])return;
	int sofs=int(ScreenPtr)+x+y*ScrWidth;
	int tmp;
	__asm{
		push esi
		push edi
		mov  edx,xs0
		shl  edx,2
		mov  esi,Buf
		inc  esi
		mov  edi,sofs
		mov  bl,16
		xor  eax,eax
		xor  ecx,ecx
		cld
LPP1:	mov  tmp,edi
		add  edx,4
		add  edi,[XShift+edx]
		mov  bh,[esi]
		inc  esi
		or   bh,bh
		jz   LPP3
LPP2:	mov  cl,[esi]
		add  edi,ecx
		mov  cl,[esi+1]
		add  esi,2
		jcxz LPP8 
LPP7:	mov  ah,[edi]
		mov  al,[esi]
		inc  esi
		mov  al,[trans8+eax]
		mov  [edi],al
		inc  edi
		dec  cl
		jnz  LPP7
LPP8:
		dec  bh
		jnz  LPP2
		mov  edi,tmp
		add  edi,ScrWidth
		dec  bl
		jnz  LPP1
		jmp  LPP4
LPP3:	mov  edi,tmp
		add  edi,ScrWidth
		dec  bl
		jnz  LPP1
LPP4:
		pop  edi
		pop  esi
	};
};
void ShowOneReflSquare_6(int x,int y,byte* Buf){
	int xs0=START_EBX;
	if(!Buf[0])return;
	int sofs=int(ScreenPtr)+x+y*ScrWidth;
	int tmp;
	__asm{
		push esi
		push edi
		mov  edx,xs0
		shl  edx,2
		mov  esi,Buf
		inc  esi
		mov  edi,sofs
		mov  bl,16
		xor  eax,eax
		xor  ecx,ecx
		cld
LPP1:	mov  tmp,edi
		add  edx,4
		add  edi,[XShift+edx]
		mov  bh,[esi]
		inc  esi
		or   bh,bh
		jz   LPP3
LPP2:	mov  cl,[esi]
		add  edi,ecx
		mov  cl,[esi+1]
		add  esi,2
		jcxz LPP8 
LPP7:	mov  ah,[edi]
		mov  al,[esi]
		inc  esi
		mov  al,[trans4+eax]
		mov  [edi],al
		inc  edi
		dec  cl
		jz  LPP8
		mov  ah,[edi]
		mov  al,[esi]
		inc  esi
		mov  al,[trans8+eax]
		mov  [edi],al
		inc  edi
		dec  cl
		jnz  LPP7
LPP8:
		dec  bh
		jnz  LPP2
		mov  edi,tmp
		add  edi,ScrWidth
		dec  bl
		jnz  LPP1
		jmp  LPP4
LPP3:	mov  edi,tmp
		add  edi,ScrWidth
		dec  bl
		jnz  LPP1
LPP4:
		pop  edi
		pop  esi
	};
};
void ShowOneReflSquare_7(int x,int y,byte* Buf){
	int xs0=START_EBX;
	if(!Buf[0])return;
	int sofs=int(ScreenPtr)+x+y*ScrWidth;
	int tmp;
	__asm{
		push esi
		push edi
		mov  edx,xs0
		shl  edx,2
		mov  esi,Buf
		inc  esi
		mov  edi,sofs
		mov  bl,16
		xor  eax,eax
		xor  ecx,ecx
		cld
LPP1:	mov  tmp,edi
		add  edx,4
		add  edi,[XShift+edx]
		mov  bh,[esi]
		inc  esi
		or   bh,bh
		jz   LPP3
LPP2:	mov  cl,[esi]
		add  edi,ecx
		mov  cl,[esi+1]
		add  esi,2
		jcxz LPP8 
LPP7:	mov  ah,[edi]
		mov  al,[esi]
		inc  esi
		mov  al,[trans4+eax]
		mov  [edi],al
		inc  edi
		dec  cl
		jz  LPP8
		mov  ah,[edi]
		mov  al,[esi]
		inc  esi
		mov  al,[trans8+eax]
		mov  [edi],al
		inc  edi
		dec  cl
		jz  LPP8
		mov  ah,[edi]
		mov  al,[esi]
		inc  esi
		mov  al,[trans4+eax]
		mov  [edi],al
		inc  edi
		dec  cl
		jnz  LPP7
LPP8:
		dec  bh
		jnz  LPP2
		mov  edi,tmp
		add  edi,ScrWidth
		dec  bl
		jnz  LPP1
		jmp  LPP4
LPP3:	mov  edi,tmp
		add  edi,ScrWidth
		dec  bl
		jnz  LPP1
LPP4:
		pop  edi
		pop  esi
	};
};
void ShowOneReflSquare_5(int x,int y,byte* Buf){
	int xs0=START_EBX;
	if(!Buf[0])return;
	int sofs=int(ScreenPtr)+x+y*ScrWidth;
	int tmp;
	__asm{
		push esi
		push edi
		mov  edx,xs0
		shl  edx,2
		mov  esi,Buf
		inc  esi
		mov  edi,sofs
		mov  bl,16
		xor  eax,eax
		xor  ecx,ecx
		cld
LPP1:	mov  tmp,edi
		add  edx,4
		add  edi,[XShift+edx]
		mov  bh,[esi]
		inc  esi
		or   bh,bh
		jz   LPP3
LPP2:	mov  cl,[esi]
		add  edi,ecx
		mov  cl,[esi+1]
		add  esi,2
		jcxz LPP8 
LPP7:	mov  ah,[edi]
		mov  al,[esi]
		inc  esi
		mov  al,[trans8+eax]
		mov  [edi],al
		inc  edi
		dec  cl
		jz  LPP8
		mov  ah,[edi]
		mov  al,[esi]
		inc  esi
		mov  al,[trans4+eax]
		mov  [edi],al
		inc  edi
		dec  cl
		jz  LPP8
		mov  ah,[edi]
		mov  al,[esi]
		inc  esi
		mov  al,[trans8+eax]
		mov  [edi],al
		inc  edi
		dec  cl
		jnz  LPP7
LPP8:
		dec  bh
		jnz  LPP2
		mov  edi,tmp
		add  edi,ScrWidth
		dec  bl
		jnz  LPP1
		jmp  LPP4
LPP3:	mov  edi,tmp
		add  edi,ScrWidth
		dec  bl
		jnz  LPP1
LPP4:
		pop  edi
		pop  esi
	};
};
void ShowOneReflSquare_9(int x,int y,byte* Buf){
	int xs0=START_EBX;
	if(!Buf[0])return;
	int sofs=int(ScreenPtr)+x+y*ScrWidth;
	int tmp;
	__asm{
		push esi
		push edi
		mov  edx,xs0
		shl  edx,2
		mov  esi,Buf
		inc  esi
		mov  edi,sofs
		mov  bl,16
		xor  eax,eax
		xor  ecx,ecx
		cld
LPP1:	mov  tmp,edi
		add  edx,4
		add  edi,[XShift+edx]
		mov  bh,[esi]
		inc  esi
		or   bh,bh
		jz   LPP3
LPP2:	mov  cl,[esi]
		add  edi,ecx
		mov  cl,[esi+1]
		add  esi,2
		jcxz LPP8 
LPP7:	mov  ah,[edi]
		mov  al,[esi]
		inc  esi
		mov  al,[trans4+eax]
		mov  [edi],al
		inc  edi
		dec  cl
		jz  LPP8
		//mov  ah,[edi]
		//mov  al,[esi]
		inc  esi
		//mov  al,[trans4+eax]
		//mov  [edi],al
		inc  edi
		dec  cl
		jz  LPP8
		mov  ah,[edi]
		mov  al,[esi]
		inc  esi
		mov  al,[trans4+eax]
		mov  [edi],al
		inc  edi
		dec  cl
		jnz  LPP7
LPP8:
		dec  bh
		jnz  LPP2
		mov  edi,tmp
		add  edi,ScrWidth
		dec  bl
		jnz  LPP1
		jmp  LPP4
LPP3:	mov  edi,tmp
		add  edi,ScrWidth
		dec  bl
		jnz  LPP1
LPP4:
		pop  edi
		pop  esi
	};
};
void ShowOneReflSquare_10(int x,int y,byte* Buf){
	int xs0=START_EBX;
	if(!Buf[0])return;
	int sofs=int(ScreenPtr)+x+y*ScrWidth;
	int tmp;
	__asm{
		push esi
		push edi
		mov  edx,xs0
		shl  edx,2
		mov  esi,Buf
		inc  esi
		mov  edi,sofs
		mov  bl,16
		xor  eax,eax
		xor  ecx,ecx
		cld
LPP1:	mov  tmp,edi
		add  edx,4
		add  edi,[XShift+edx]
		mov  bh,[esi]
		inc  esi
		or   bh,bh
		jz   LPP3
LPP2:	mov  cl,[esi]
		add  edi,ecx
		mov  cl,[esi+1]
		add  esi,2
		jcxz LPP8 
LPP7:	mov  ah,[edi]
		mov  al,[esi]
		inc  esi
		mov  al,[trans4+eax]
		mov  [edi],al
		inc  edi
		dec  cl
		jz  LPP8
		//mov  ah,[edi]
		//mov  al,[esi]
		inc  esi
		//mov  al,[trans8+eax]
		//mov  [edi],al
		inc  edi
		dec  cl
		jnz  LPP7
LPP8:
		dec  bh
		jnz  LPP2
		mov  edi,tmp
		add  edi,ScrWidth
		dec  bl
		jnz  LPP1
		jmp  LPP4
LPP3:	mov  edi,tmp
		add  edi,ScrWidth
		dec  bl
		jnz  LPP1
LPP4:
		pop  edi
		pop  esi
	};
};
void ShowOneReflSquare_(int x,int y,byte* Buf){
	int xs0=START_EBX;
	if(!Buf[0])return;
	int sofs=int(ScreenPtr)+x+y*ScrWidth;
	int tmp;
	__asm{
		push esi
		push edi
		mov  edx,xs0
		shl  edx,2
		mov  esi,Buf
		inc  esi
		mov  edi,sofs
		mov  bl,16
		xor  eax,eax
		xor  ecx,ecx
		cld
LPP1:	add  edx,4
		add  edi,[XShift+edx]
		mov  tmp,edi
		mov  bh,[esi]
		inc  esi
		or   bh,bh
		jz   LPP3
LPP2:	mov  cl,[esi]
		add  edi,ecx
		mov  cl,[esi+1]
		add  esi,2
		rep  movsb
		dec  bh
		jnz  LPP2
		mov  edi,tmp
		add  edi,ScrWidth
		dec  bl
		jnz  LPP1
		jmp  LPP4
LPP3:	mov  edi,tmp
		add  edi,ScrWidth
		dec  bl
		jnz  LPP1
LPP4:
		pop  edi
		pop  esi
	};
};
void ShowOneReflSquare_B0(int x,int y,byte* Buf);
void ShowOneReflSquare_B1(int x,int y,byte* Buf);
void ShowOneReflSquare_B2(int x,int y,byte* Buf);
void ShowOneReflSquare_B3(int x,int y,byte* Buf);
void ShowOneReflSquare_XY(int x,int y,byte* Buf);
void ReflectEngine::ShowAll(){
	if(LMode)return;
	for(int i=0;i<NRefGrp;i++){
		REF_HASH[RefGrp[i]->HASH]=i;
		if(!RefGrp[i]->Added){
			REF_HASH[RefGrp[i]->HASH]=-1;
			free(RefGrp[i]);
			if(i<NRefGrp-1){
				memcpy(RefGrp+i,RefGrp+i+1,(NRefGrp-i-1)<<2);
			};
			NRefGrp--;
			i--;
		};

	};
	int xmin=mapx;
	int xmax=mapx+smaplx;
	int ymin=mapy;
	int ymax=ymin+smaply;
	for(int i=0;i<NRefGrp;i++){
		RefGrp[i]->Shown=0;
	};
	for(int i=0;i<NRefGrp;i++){
		OneReflection* ORF=RefGrp[i];
		int x0=ORF->x>>5;
		int y0=ORF->y>>4;
		int Nx=ORF->Nx;
		int Ny=ORF->Ny;
		int FLINE=-1;
		if(x0<xmax&&x0+Nx>xmin&&y0<ymax&&y0+Ny>ymin){
			for(int iy=0;iy<Ny;iy++){
				//FLINE=-1;
				for(int ix=0;ix<Nx;ix++){
					int xx=x0-xmin+ix;
					int yy=y0-ymin+iy;
					if(xx>=0&&xx<=smaplx&&yy>=0&&yy<=smaply){
						int* refs=(int*)ORF->Data;
						byte* Data=ORF->Data+refs[ix+iy*Nx];
						if(Data[0]){
							if(FLINE==-1)FLINE=iy;
							START_EBX=(iy-FLINE)<<4;
							switch((iy-FLINE)>>8){
							case 0:
							case 1:
							case 2:
							case 3:
								ShowOneReflSquare_XY(xx<<5,yy<<4,Data);
								ORF->Shown=1;
								break;
							//case 2:
							//case 3:
								//ShowOneReflSquare_B1(xx<<5,yy<<4,Data);
								ORF->Shown=1;
								break;
							case 4:
							case 5:
								ShowOneReflSquare_B2(xx<<5,yy<<4,Data);
								ORF->Shown=1;
								break;
							case 6:
							case 7:
							case 8:
								ShowOneReflSquare_B2(xx<<5,yy<<4,Data);
								ORF->Shown=1;
								break;
								/*
							case 0:
								//ShowOneReflSquare_4(xx<<5,yy<<4,Data);
								//ORF->Shown=1;
								//break;
							case 1:
								ShowOneReflSquare_5(xx<<5,yy<<4,Data);
								ORF->Shown=1;
								break;
							case 2:
								ShowOneReflSquare_6(xx<<5,yy<<4,Data);
								ORF->Shown=1;
								break;
							case 3:
								ShowOneReflSquare_7(xx<<5,yy<<4,Data);
								ORF->Shown=1;
								break;
							case 4:
								ShowOneReflSquare_8(xx<<5,yy<<4,Data);
								ORF->Shown=1;
								break;
							case 6:
							case 5:
								ShowOneReflSquare_9(xx<<5,yy<<4,Data);
								ORF->Shown=1;
								break;
							//case 6:
							//	ShowOneReflSquare_10(xx<<5,yy<<4,Data);
							//	ORF->Shown=1;
							//	break;
							*/
							};
						};
					};
				};
			};
		};
	};
	for(int i=0;i<NRefGrp;i++){
		RefGrp[i]->Added=0;
		REF_HASH[RefGrp[i]->HASH]=i;
		if(!RefGrp[i]->Shown){
			REF_HASH[RefGrp[i]->HASH]=-1;
			free(RefGrp[i]);
			if(i<NRefGrp-1){
				memcpy(RefGrp+i,RefGrp+i+1,(NRefGrp-i-1)<<2);
			};
			NRefGrp--;
			i--;
		};
	};
};
ReflectEngine RENG;
void DrawReflect(){
	RENG.ShowAll();
};
bool AddSprRefl(int xg,int yg,int xc,int yc,int GP_File,int Sprite,int dy1,int dy2){
	int cx=xg>>5;
	bool success=0;
	if(dy1>=0){
		int cy=(yg+dy1)>>4;
		if(cx>0&&cx<MaxWX&&cy>0&&cy<MaxWX){
			if(WaterDeep[cx+(cy<<(TopSH+1))]>120)success=1;
		};
	};
	if(dy2>=0&&!success){
		int cy=(yg+dy2)>>4;
		if(cx>0&&cx<MaxWX&&cy>0&&cy<MaxWX){
			if(WaterDeep[cx+(cy<<(TopSH+1))]>120)success=1;
		};
	};
	if(success||(dy1==-1&&dy2==-1))RENG.AddGP_Reflection(xg,yg,xc,yc,GP_File,Sprite);
	return success;
};
void ReflectEngine::AddGP_Reflection(int xg,int yg,int xc,int yc,int GP_File,int Sprite){
	if(LMode)return;
	if(NRefGrp>=255)return;
	bool Back=Sprite>=4096;
	Sprite&=4095;
	if(!GPS.GPH[GP_File]){
		GPS.LoadGP(GP_File);
		if(!GPS.GPH[GP_File])return;
	};
	DWORD HASH=(xg+yg+yg+(xc<<4)-yc-yc-yc+GP_File*73+Sprite*91)&4095;
	if(REF_HASH[HASH]!=255){
		RefGrp[REF_HASH[HASH]]->Added=1;
		return;
	};
	int dx,dy, lx, ly;
	GetRealGPSize(GP_File,Sprite,&dx,&dy);
	yg-=dy;
	lx=GPS.GetGPWidth(GP_File,Sprite);
	ly=GPS.GetGPHeight(GP_File,Sprite);
	
	int x0=(xg-xc+dx-1-32)&(0xFFFFFFFF-31);
	int y0=yg&(0xFFFFFFFF-15);
	int x1=(xg-xc+lx+32)&(0xFFFFFFFF-31);
	int y1=(yg+yc+16)&(0xFFFFFFFF-15);
	int sz=(x1-x0)*(y1-y0);
	byte* Buf=znew(byte,sz);
	memset(Buf,0,sz);
	TempWindow TW;
	PushWindow(&TW);
	int ww=ScrWidth;
	void* ptr=ScreenPtr;
	ScreenPtr=Buf;
	ScrWidth=x1-x0;
	SETWINDX(0,0,x1-x0,y1-y0);
	int nx=(x1-x0)>>5;;
	int ny=(y1-y0)>>4;
	memset(Buf,0,(nx*ny)<<9);
	if(Back)GPS.ShowGPLayers(xg-x0+xc,y1-yg-yc-dy,GP_File,Sprite+4096,0,1|2|512);
	else GPS.ShowGPLayers(xg-xc-x0,y1-yg-yc-dy,GP_File,Sprite,0,1|2|512);
	//apply water mask
	byte PREVIEW[1024];
	int ppos=0;
	for(int ix=0;ix<nx;ix++){
		for(int iy=0;iy<ny;iy++){
			int xx=x0+(ix<<5);
			int yy=y1-(iy<<4);

			int z1=GetDeep(xx,yy);
			int z2=GetDeep(xx+32,yy);
			int z3=GetDeep(xx,yy-16);
			int z4=GetDeep(xx+32,yy-16);
			ppos++;
			PREVIEW[ix+(ny-iy-1)*nx]=PutWaterReflectionMask(Buf+(ix<<5)+(iy<<9)*nx,z1,z2,z3,z4,nx<<5);
		};
	};
	//BMPformat BM;
	//SaveBMP8("2.bmp",x1-x0,y1-y0,Buf);
	int REFS[1024];
	int rpos=0;
	byte PackBuf[65536*4];
	int cppos=nx*ny*4;;
	int pbpos=0;
	for(int iy=0;iy<ny;iy++){
		for(int ix=0;ix<nx;ix++){
			REFS[rpos]=pbpos+cppos;
			if(PREVIEW[rpos]){
				PackBuf[pbpos]=0;
				pbpos++;
			}else pbpos+=PackReflDataTo(PackBuf+pbpos,Buf+(ix<<5)+((ny-iy)<<9)*nx,nx<<5);
			rpos++;
		};
	};
	if(NRefGrp>=MaxRefGrp){
		MaxRefGrp+=32;
		RefGrp=(OneReflection**)realloc(RefGrp,MaxRefGrp<<2);
	};
	RefGrp[NRefGrp]=(OneReflection*)malloc(sizeof(OneReflection)-128+pbpos+cppos);
	memcpy(RefGrp[NRefGrp]->Data,REFS,cppos);
	memcpy(RefGrp[NRefGrp]->Data+cppos,PackBuf,pbpos);
	RefGrp[NRefGrp]->Nx=nx;
	RefGrp[NRefGrp]->Ny=ny;
	RefGrp[NRefGrp]->x=x0;
	RefGrp[NRefGrp]->y=y0;
	RefGrp[NRefGrp]->HASH=HASH;
	RefGrp[NRefGrp]->Added=1;
	REF_HASH[HASH]=NRefGrp;
	NRefGrp++;
	free(Buf);
	PopWindow(&TW);
	ScreenPtr=ptr;
};
void ShowOneReflSquare_B0(int x,int y,byte* Buf){
	int xs0=START_EBX;
	if(!Buf[0])return;
	int sofs=int(ScreenPtr)+x+y*ScrWidth;
	int tmp;
	__asm{
		push esi
		push edi
		mov  edx,xs0
		shl  edx,2
		mov  esi,Buf
		inc  esi
		mov  edi,sofs
		mov  bl,16
		xor  eax,eax
		xor  ecx,ecx
		cld
LPP1:	mov  tmp,edi
		add  edx,4
		add  edi,[XShift+edx]
		mov  bh,[esi]
		inc  esi
		or   bh,bh
		jz   LPP3
LPP2:	mov  cl,[esi]
		add  edi,ecx
		mov  cl,[esi+1]
		add  esi,2
		jcxz LPP8 
LPP7:	mov  ah,[edi]
		mov  al,[esi]
		inc  esi
		mov  al,[trans8+eax]
		mov  [edi],al
		inc  edi
		dec  cl
		jnz  LPP7
LPP8:
		dec  bh
		jnz  LPP2
		mov  edi,tmp
		add  edi,ScrWidth
		dec  bl
		jnz  LPP1
		jmp  LPP4
LPP3:	mov  edi,tmp
		add  edi,ScrWidth
		dec  bl
		jnz  LPP1
LPP4:
		pop  edi
		pop  esi
	};
};
void ShowOneReflSquare_B1(int x,int y,byte* Buf){
	int xs0=START_EBX;
	if(!Buf[0])return;
	int sofs=int(ScreenPtr)+x+y*ScrWidth;
	int tmp;
	__asm{
		push esi
		push edi
		mov  edx,xs0
		shl  edx,2
		mov  esi,Buf
		inc  esi
		mov  edi,sofs
		mov  bl,16
		xor  eax,eax
		xor  ecx,ecx
		cld
LPP1:	mov  tmp,edi
		add  edx,4
		add  edi,[XShift+edx]
		mov  bh,[esi]
		inc  esi
		or   bh,bh
		jz   LPP3
LPP2:	mov  cl,[esi]
		add  edi,ecx
		mov  cl,[esi+1]
		add  esi,2
		jcxz LPP8 
		test bl,1
		jz   LPP7
LPP7A:	mov  ah,[edi]
		mov  al,[esi]
		inc  esi
		mov  al,[trans8+eax]
		mov  [edi],al
		inc  edi
		dec  cl
		jnz  LPP7A
		jmp  LPP8
LPP7:	mov  ah,[edi]
		mov  al,[esi]
		inc  esi
		mov  al,[trans4+eax]
		mov  [edi],al
		inc  edi
		dec  cl
		jnz  LPP7
LPP8:
		dec  bh
		jnz  LPP2
		mov  edi,tmp
		add  edi,ScrWidth
		dec  bl
		jnz  LPP1
		jmp  LPP4
LPP3:	mov  edi,tmp
		add  edi,ScrWidth
		dec  bl
		jnz  LPP1
LPP4:
		pop  edi
		pop  esi
	};
};
extern int YShift[2048];
void ShowOneReflSquare_XY(int x,int y,byte* Buf){
#ifndef _USE3D
	if(y<16||y>=WindY1)return;
	int MAX=WindY1-y;
#else
	const ViewPort& vp = GPS.GetClipArea();
	if (y < 16 || !vp.ClipY( y ))return;
	int MAX= vp.GetBottom() - y;
#endif
	if(MAX>16)MAX=16;
	int xs0=START_EBX;
	if(!Buf[0])return;
	int sofs=int(ScreenPtr)+x+y*ScrWidth;
	int tmp;
	__asm{
		push esi
		push edi
		mov  edx,xs0
		shl  edx,2
		mov  esi,Buf
		inc  esi
		mov  edi,sofs
		mov  bl,byte ptr MAX
		xor  eax,eax
		xor  ecx,ecx
		cld
LPP1:	mov  tmp,edi
		add  edx,4
		add  edi,[XShift+edx]
		mov  bh,[esi]
		inc  esi
		or   bh,bh
		jz   LPP3
LPP2:	mov  cl,[esi]
		add  edi,ecx
		mov  cl,[esi+1]
		add  esi,2
		jcxz LPP8 
LPP7:	add  edi,[YShift+ecx*4]
		mov  ah,[edi]
		mov  al,[esi]
		inc  esi
		mov  al,[trans4+eax]
		mov  [edi],al
		sub  edi,[YShift+ecx*4]
		inc  edi
		dec  cl
		jnz  LPP7
LPP8:
		dec  bh
		jnz  LPP2
		mov  edi,tmp
		add  edi,ScrWidth
		dec  bl
		jnz  LPP1
		jmp  LPP4
LPP3:	mov  edi,tmp
		add  edi,ScrWidth
		dec  bl
		jnz  LPP1
LPP4:
		pop  edi
		pop  esi
	};
};
void ShowOneReflSquare_B2(int x,int y,byte* Buf){
	int xs0=START_EBX;
	if(!Buf[0])return;
	int sofs=int(ScreenPtr)+x+y*ScrWidth;
	int tmp;
	__asm{
		push esi
		push edi
		mov  edx,xs0
		shl  edx,2
		mov  esi,Buf
		inc  esi
		mov  edi,sofs
		mov  bl,16
		xor  eax,eax
		xor  ecx,ecx
		cld
LPP1:	mov  tmp,edi
		add  edx,4
		add  edi,[XShift+edx]
		mov  bh,[esi]
		inc  esi
		or   bh,bh
		jz   LPP3
LPP2:	mov  cl,[esi]
		add  edi,ecx
		mov  cl,[esi+1]
		add  esi,2
		jcxz LPP8 
LPP7:	mov  ah,[edi]
		mov  al,[esi]
		inc  esi
		mov  al,[trans4+eax]
		mov  [edi],al
		inc  edi
		dec  cl
		jnz  LPP7
LPP8:
		dec  bh
		jnz  LPP2
		mov  edi,tmp
		add  edi,ScrWidth
		dec  bl
		jnz  LPP1
		jmp  LPP4
LPP3:	mov  edi,tmp
		add  edi,ScrWidth
		dec  bl
		jnz  LPP1
LPP4:
		pop  edi
		pop  esi
	};
};
void ShowOneReflSquare_B3(int x,int y,byte* Buf){
	int xs0=START_EBX;
	if(!Buf[0])return;
	int sofs=int(ScreenPtr)+x+y*ScrWidth;
	int tmp;
	__asm{
		push esi
		push edi
		mov  edx,xs0
		shl  edx,2
		mov  esi,Buf
		inc  esi
		mov  edi,sofs
		mov  bl,16
		xor  eax,eax
		xor  ecx,ecx
		cld
LPP1:	mov  tmp,edi
		add  edx,4
		add  edi,[XShift+edx]
		mov  bh,[esi]
		inc  esi
		or   bh,bh
		jz   LPP3
LPP2:	mov  cl,[esi]
		add  edi,ecx
		mov  cl,[esi+1]
		add  esi,2
		jcxz LPP8 
		test bl,1
		jz   LPP7
		add esi,ecx
		add edi,ecx
		jmp  LPP8
LPP7:	mov  ah,[edi]
		mov  al,[esi]
		inc  esi
		mov  al,[trans4+eax]
		mov  [edi],al
		inc  edi
		dec  cl
		jnz  LPP7
LPP8:
		dec  bh
		jnz  LPP2
		mov  edi,tmp
		add  edi,ScrWidth
		dec  bl
		jnz  LPP1
		jmp  LPP4
LPP3:	mov  edi,tmp
		add  edi,ScrWidth
		dec  bl
		jnz  LPP1
LPP4:
		pop  edi
		pop  esi
	};
};