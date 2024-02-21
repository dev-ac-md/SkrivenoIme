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
#include "RealWater.h"
#include "NewUpgrade.h"
#include "ZBuffer.h"
#include "Path.h"
#include "Transport.h"
#include "3DBars.h"
#include "CDirSnd.h"
#include "NewAI.h"
#include "TopoGraf.h"
#include "HashTop.h"
#include <CrtDbg.h>
#include "fonts.h"
#include "LoadSave.h"

void HashTopTable::AddLink(int N1,int N2){
	Area* AR=TopMap+N1;
	int N=AR->NLinks;
	for(int i=0;i<N;i++){
		if(AR->Link[i+i]==N2)return;
	};
	if(AR->NLinks==AR->MaxLink){
		AR->MaxLink+=4;
		AR->Link=(word*)realloc(AR->Link,AR->MaxLink<<2);
	};
	N=AR->NLinks;
	AR->Link[N+N]=N2;
	Area* A2=TopMap+N2;
	int SC=16;//GetLinkScale(N1,N2);
	AR->Link[N+N+1]=(Norma(AR->x-A2->x,AR->y-A2->y)*SC)>>4;
	AR->NLinks++;
};

bool HashTopTable::AddArea(short x,short y,byte Sliv){
	int MinNorm;
	switch(ADDSH){
	case 1:
		MinNorm=5;
		break;
	case 2:
		MinNorm=8;
		break;
	case 3:
		MinNorm=11;
		break;
	};	

	if(Sliv!=2){		
		for(int i=0;i<NAreas;i++){
			Area* AR=TopMap+i;
			if(Norma(AR->x-x,AR->y-y)<=MinNorm){
				if(Sliv){
					AR->x=(AR->x+x)>>1;
					AR->y=(AR->y+y)>>1;
				};
				return false;
			};
		};
	};
	if(NAreas>=MaxArea){
		MaxArea+=120;
		TopMap=(Area*)realloc(TopMap,MaxArea* sizeof(Area));
	};
	Area* AR=TopMap+NAreas;
	memset(AR,0,sizeof(Area));
	AR->x=x;
	AR->y=y;
	AR->MaxLink=6;
	AR->Link=znew(word,AR->MaxLink<<1);
	NAreas++;
	return true;
}

bool GetTCStatus(int x,int y, byte TopType){
	int xxx=x<<2;
	int yyy=y<<2;
	MotionField* MF=MFIELDS+TopType;
	if(!MF->CheckBar(xxx,yyy,4,4))return true;
	else{
		//accurate check
		if( ( (!MF->CheckPt(xxx+1,yyy))  || (!MF->CheckPt(xxx+2,yyy+1)) || (!MF->CheckPt(xxx+3,yyy+2)) )||
			( (!MF->CheckPt(xxx,yyy+1))  || (!MF->CheckPt(xxx+1,yyy+2)) || (!MF->CheckPt(xxx+2,yyy+3)) )||
			( (!MF->CheckPt(xxx+2,yyy))  || (!MF->CheckPt(xxx+1,yyy+1)) || (!MF->CheckPt(xxx,yyy+2  )) )||
			( (!MF->CheckPt(xxx+3,yyy+1))|| (!MF->CheckPt(xxx+2,yyy+2)) || (!MF->CheckPt(xxx+1,yyy+3)) )
		  )return true;
	};
	return false;
};

void ResearchIslands();
void HashTopTable::CreateAreas(){
	
	EraseAreas();
	MaxArea=200;
	TopMap=znew(Area,MaxArea);
	NAreas=0;

	//creating ref. map
	memset(TopRef,0xFFFF,TopLx*TopLy*2);
	int mmx=msx>>1;
	int mmy=msy>>1;
	for(int x=0;x<mmx;x++)
		for(int y=0;y<mmy;y++){
			if(GetTCStatus(x,y,TopType)) TopRef[x+y*TopLx]=0xFFFE;
		};

	
	//PreCreateTopLinks();
	
	//assert(_CrtCheckMemory());
	
	// создание зон по меткам из редактора карты
	/*
	for(int i=0;i<MAXSPR;i++){
		OneSprite* OS=&Sprites[i];
		if(OS->Enabled){
			SprGroup* SG=OS->SG;
			ObjCharacter* OC=&SG->ObjChar[OS->SGIndex];
			if(SG==&TREES&&OS->SGIndex==SG->NSpr-1)AddArea(OS->x>>6,OS->y>>6,2);
		};
	};
	*/

	// создание сетки топ зон
	int N1=0;
	int N2=0;
	int V0=rando();
	do{
		int VX=(rando()+V0)&32767;
		if(VX<1024)V0+=rando();
		int x=((int(rando()+V0)&32767)*(mmx-4))>>15;
		int y=((int(rando()+V0)&32767)*(mmy-4))>>15;
		bool Empty=true;
		for(int dx=0;dx<=2&&Empty;dx++)
			for(int dy=0;dy<=2&&Empty;dy++){
				if(TopRef[x+dx+(y+dy)*TopLx]!=0xFFFE)Empty=false;
			};
		if(Empty){
			if(AddArea(x+1,y+1,0)){
				N1=0;
				N2=0;
			}else N1++;
		}else{
			N2++;
		};
	}while(N1<500&&N2<5000);

	ProcessMessages();

	//assert(_CrtCheckMemory());
	
	//creating linking between areas	
	for(int i=0;i<NAreas;i++){
		Area* AR=TopMap+i;
		TopRef[AR->x+int(AR->y)*TopLx]=i;
	}

	bool Change=false;
	//word TempTop[TopLx*TopLy];
	Change=true;
	for(int i=1;i<RRad&&Change;i++){
		//memcpy(TempTop,TopRef,sizeof(TopRef));
		Change=false;
		char* xi=Rarr[i].xi;
		char* yi=Rarr[i].yi;
		int N=Rarr[i].N;
		for(int j=0;j<NAreas;j++){
			Area* AR=TopMap+j;
			int xx=AR->x;
			int yy=AR->y;
			for(int k=0;k<N;k++){
				int x=xx+xi[k];
				int y=yy+yi[k];
				if(x>0&&y>0&&x<mmx&&y<mmy){
					int ofst=x+y*TopLx;
					word ZZ=TopRef[ofst	  ];
					word ZU=TopRef[ofst-TopLx];
					word ZD=TopRef[ofst+TopLx];
					word ZL=TopRef[ofst-1    ];
					word ZR=TopRef[ofst+1    ];
					if(ZZ==0xFFFE){
						if(ZU==j||ZD==j||ZL==j||ZR==j||
							TopRef[ofst-TopLx-1]==j||
							TopRef[ofst-TopLx+1]==j||
							TopRef[ofst+TopLx-1]==j||
							TopRef[ofst+TopLx+1]==j){
							TopRef[ofst]=j;
							Change=true;
							ZZ=j;
						};
					};
					if(ZZ<0xFFFE){
						if(ZU!=ZZ&&ZU<0xFFFE){
							AddLink(ZZ,ZU);
							AddLink(ZU,ZZ);
						};
						if(ZD!=ZZ&&ZD<0xFFFE){
							AddLink(ZZ,ZD);
							AddLink(ZD,ZZ);
						};
						if(ZL!=ZZ&&ZL<0xFFFE){
							AddLink(ZZ,ZL);
							AddLink(ZL,ZZ);
						};
						if(ZR!=ZZ&&ZR<0xFFFE){
							AddLink(ZZ,ZR);
							AddLink(ZR,ZZ);
						};
					};
				};
			};
		};
	};

	ProcessMessages();

	//creating resource references
	/*
	for(int i=0;i<MAXSPR;i++){
		OneSprite* OS=&Sprites[i];
		SprGroup* SG=OS->SG;
		ObjCharacter* OC=&SG->ObjChar[OS->SGIndex];
		if(OC->IntResType<8){
			int xx=OS->x>>6;
			int yy=OS->y>>6;
			int ofs=xx+yy*TopLx;
			word TZ=TopRef[ofs];
			if(TZ<4096){
				Area* AR=TopMap+TZ;
				AR->MinesIdx[AR->NMines]=i;
				AR->NMines++;
			};
		};
	};
	*/

	ProcessMessages();
	
	//assert(_CrtCheckMemory());
	
	//if(TopType=0) ResearchIslands();	// вычисление островов, разорваных топологий
	//CreateCostPlaces();				// зоны причаливания парома
}

void HashTopTable::EraseAreas(){	
	if(NAreas){
		for(int i=0;i<NAreas;i++){
			Area* AR1=TopMap+i;
			if(AR1->MaxLink)free(AR1->Link);
			if(AR1->NMines)free(AR1->MinesIdx);
		};
		free(TopMap);
		TopMap=NULL;
		NAreas=0;
		MaxArea=0;
	};
};

extern word FULLWAY[128];
extern int NWPTS;
#define MXZ 16384

bool HashTopTable::CalculateWay(int Ofs, HashTop* HT){
	
	div_t res=div(Ofs,NAreas);
	word Start=res.quot;
	word Fin=res.rem;

	if(Start>=NAreas||Fin>=NAreas) return false;
	//TIME1=GetTickCount();
	NWPTS=0;
	//if(GetZonesDist(Start,Fin)>=0xFFFF)return 0xFFFF;
	word PrecisePointWeight    [MXZ];
	word PrecisePointPrevIndex [MXZ];
	word CandidatPointWeight   [MXZ];
	word CandidatPointPrevIndex[MXZ];
	word CandidatList          [MXZ];

	memset(PrecisePointWeight,0xFF,NAreas<<1);
	memset(CandidatPointWeight,0xFF,NAreas<<1);
	CandidatList[0]=Fin;
	CandidatPointWeight[Fin]=0;
	CandidatPointPrevIndex[Fin]=0xFFFF;
	int NCandidats=1;
	do{
		//best candidat is a last candidat in sorted list
		NCandidats--;
		word TZ=CandidatList[NCandidats];
		PrecisePointWeight[TZ]=CandidatPointWeight[TZ];
		CandidatPointWeight[TZ]=0xFFFF;
		PrecisePointPrevIndex[TZ]=CandidatPointPrevIndex[TZ];
		if(TZ==Start){
			//it finally happen!!!
//			int MaxDang=DangerMap[TZ];
			int T0=TZ;
			FULLWAY[0]=TZ;
			NWPTS=1;
			for(int q=0;/*q<DangSteps&&*/T0!=0xFFFF;q++){
				T0=CandidatPointPrevIndex[T0];
				if(T0!=0xFFFF){
					if(NWPTS<128){
						FULLWAY[NWPTS]=T0;
						NWPTS++;
					};
					//int w=DangerMap[T0];
					//if(w>MaxDang)MaxDang=w;
				};
			};
			//*MaxDanger=MaxDang;
			//TIME1=GetTickCount()-TIME1;
			HT->LD=PrecisePointWeight[TZ];
			HT->ML=PrecisePointPrevIndex[TZ];
			HT->Prio=0;
			return true;//PrecisePointPrevIndex[TZ];
		};
		Area* TAR=TopMap+TZ;
		int NL=TAR->NLinks;
		word* LINK=TAR->Link;
		int w0=PrecisePointWeight[TZ];
		for(int i=0;i<NL;i++){
			int pi=LINK[i+i];
			//adding point to the candidats list
			//1.checking if point is in precise list
			if(PrecisePointWeight[pi]==0xFFFF){
				int wi=LINK[i+i+1]+w0;//+DangerMap[pi];
				//2.checking if point is already in candidats list
				int wc;
				bool add=1;
				if((wc=CandidatPointWeight[pi])!=0xFFFF){
					if(wc>wi){
						//new point is better. Need to delete old point
						for(int j=0;j<NCandidats;j++)if(CandidatList[j]==pi){
							if(j<NCandidats-1)memcpy(CandidatList+j,CandidatList+j+1,(NCandidats-j-1)<<1);
							j=NCandidats;
							NCandidats--;
						};
						CandidatPointWeight[pi]=wi;
					}else{
						add=0;
					};
				};
				if(add){
					//need to add point to candidats list and sort array
					if(NCandidats==0){
						CandidatList[0]=pi;
						CandidatPointWeight[pi]=wi;
						CandidatPointPrevIndex[pi]=TZ;
						NCandidats++;
					}else{
						int IDXmax=0;
						int IDXmin=NCandidats-1;
						int wcmax=CandidatPointWeight[CandidatList[IDXmax]];
						int wcmin=CandidatPointWeight[CandidatList[IDXmin]];
						if(wi<=wcmin){
							CandidatList[NCandidats]=pi;
							CandidatPointWeight[pi]=wi;
							CandidatPointPrevIndex[pi]=TZ;
							NCandidats++;
						}else
						if(wi>wcmax){
							memmove(CandidatList+1,CandidatList,NCandidats<<1);
							CandidatList[0]=pi;
							CandidatPointWeight[pi]=wi;
							CandidatPointPrevIndex[pi]=TZ;
							NCandidats++;
						}else{
							while(IDXmax!=IDXmin-1){
								int IDXmid=(IDXmin+IDXmax)>>1;
								int wm=CandidatPointWeight[CandidatList[IDXmid]];
								if(wm>wi){
									wcmax=wm;
									IDXmax=IDXmid;
								}else{
									wcmin=wm;
									IDXmin=IDXmid;
								};
							};
							memmove(CandidatList+IDXmin+1,CandidatList+IDXmin,(NCandidats-IDXmin+1)<<1);
							CandidatList[IDXmin]=pi;
							CandidatPointWeight[pi]=wi;
							CandidatPointPrevIndex[pi]=TZ;
							NCandidats++;
						};
					};
				};
			};
		};
	}while(NCandidats);
	return false;
	//return 0xFFFF;
}

HashTop* HashTopTable::GetHashTop(int Ofs){
	word key0=Ofs&1023;
	word key1=Ofs>>10;

	HashTop* start=Key1+key0*10;
	HashTop* final=start+10;
	
	HashTop* cur=NULL;

	while(start<final){
		if(start->K1==key1){
			start->Prio++;
			return start;
		}else
		if(!cur&&start->K1==0xFFFF || cur&&start->K1<0xFFFF&&cur->Prio<start->Prio){
			cur=start;
		}
		start++;
	}
	if(cur&&CalculateWay(Ofs,cur)){
		cur->K1=key1;
		return cur;
	}
	return NULL;

}

#ifdef HASH_TOP
HashTopTable HashTable[NMFIELDS];
#endif //HASH_TOP

// --- MF ---

int GetZ(int z1,int z2,int z3,int z4);
void CreateWaterLocking(MotionField* MF,int x,int y,int x1,int y1){
	int ofst=x+(MaxWX)*y;
	int addofs=(MaxWX)+x-x1;
	for(int iy=x;iy<y1;iy++){
		for(int ix=x;ix<x1;ix++){
			int z1=WaterDeep[ofst];
			int z2=WaterDeep[ofst+1];
			int z3=WaterDeep[ofst+(MaxWX)];
			int z4=WaterDeep[ofst+(MaxWX)+1];
			int D1=GetZ(z1,z2,z3,z4);
			int D2=GetZ(z2,z4,z1,z3);
			int D3=GetZ(z3,z4,z1,z2);
			int D4=GetZ(z4,z3,z2,z1);
			/*
			int Z1=WaterBright[ofst];
			int Z2=WaterBright[ofst+1];
			int Z3=WaterBright[ofst+(MaxWX)];
			int Z4=WaterBright[ofst+(MaxWX)+1];
			int B1=GetZ(Z1,Z2,Z3,Z4);
			int B2=GetZ(Z2,Z4,Z1,Z3);
			int B3=GetZ(Z3,Z4,Z1,Z2);
			int B4=GetZ(Z4,Z3,Z2,Z1);
			*/
			int xx=ix+ix;
			int yy=iy+iy;
			//if(D1>130)MFIELDS[1].BClrPt(xx,yy);else MFIELDS[1].BSetPt(xx,yy);
			//if(D2>130)MFIELDS[1].BClrPt(xx+1,yy);else MFIELDS[1].BSetPt(xx+1,yy);
			//if(D3>130)MFIELDS[1].BClrPt(xx,yy+1);else MFIELDS[1].BSetPt(xx,yy+1);
			//if(D4>130)MFIELDS[1].BClrPt(xx+1,yy+1);else MFIELDS[1].BSetPt(xx+1,yy+1);
			//if(D1>128)MFIELDS->BSetPt(xx,yy);
			//if(D2>128)MFIELDS->BSetPt(xx+1,yy);
			//if(D3>128)MFIELDS->BSetPt(xx,yy+1);
			//if(D4>128)MFIELDS->BSetPt(xx+1,yy+1);
			if(D1>128)MF->BSetPt(xx,yy);
			if(D2>128)MF->BSetPt(xx+1,yy);
			if(D3>128)MF->BSetPt(xx,yy+1);
			if(D4>128)MF->BSetPt(xx+1,yy+1);
			ofst++;
		};
		ofst+=addofs;
	};
};

void SetLockMask(MotionField* MF,int x,int y,char* mask){
	int pos=0;
	for(int i=0;i<4;i++)
		for(int j=0;j<4;j++){
			if(mask[pos]=='*')MF->BSetPt(x+j,y+i);
			else MF->BClrPt(x+j,y+i);
			pos++;
		};
};

//void CreateMFieldLocking(byte LockType);
void CreateMFieldLocking(byte LockType){
	if(LockType<2||LockType>=NMFIELDS) return;
	MotionField* MF=MFIELDS+LockType;

	for(int i=0;i<MAXSPR;i++){
		OneSprite* OSP=Sprites+i;
		if(OSP->Enabled){
			ObjCharacter* OC=OSP->OC;
			SprGroup* SG=OSP->SG;
			if(LockType==2){	// not walk in forest
				if(SG==&TREES){
					int id=OSP->SGIndex;
					//AddLockPts(OSP->x-SG->Dx[id],(OSP->y>>1)-SG->Dy[id],OC->NLockPt,OC->LockX,OC->LockY,1);
					MF->BSetBar((OSP->x>>4)-4,(OSP->y>>4)-4,8);
				};
			}
		};
	};
};
void SmoothFields(MotionField* MF){

	MF->ClearMaps();

	int mxx=msx>>1;
	int myy=msy>>1;
	int ix;	

	// mountings locking
	for(ix=0;ix<mxx;ix++){
		int ZLmin=1000000;
		int ZRmin=1000000;	
		for(int iy=myy-1;iy>=0;iy--){
			int x0=ix<<6;
			int y0=iy<<6;
			int ppx=ix<<2;
			int ppy=iy<<2;
			int ZZ=(iy<<5)-iy-iy;
			int z0=GetUnitHeight(x0,y0);
			int z1=GetUnitHeight(x0+64,y0);
			int z2=GetUnitHeight(x0,y0+64);
			int z3=GetUnitHeight(x0+64,y0+64);
			int ZL0=ZZ-z0;
			int ZL1=ZZ-z2+30;
			int ZR0=ZZ-z1;
			int ZR1=ZZ-z3+30;
			if(ZL1<ZL0){
				ZL0+=ZL1;
				ZL1=ZL0-ZL1;
				ZL0-=ZL1;
			};
			if(ZR1<ZR0){
				ZR0+=ZR1;
				ZR1=ZR0-ZR1;
				ZR0-=ZR1;
			};
			//if(ZL1>ZLmin||ZR1>ZRmin){
				//BSetBar(ppx,ppy,4);
			//}else{
				int za=(z0+z1+z2+z3)>>2;
				if(abs(z0-za)>=23||abs(z1-za)>=23||abs(z2-za)>=23||abs(z3-za)>=23){
					MF->BSetBar(ppx,ppy,4);
				}else MF->BClrBar(ppx,ppy,4);
			//};
			if(ZL0<ZLmin)ZLmin=ZL0;
			if(ZR0<ZRmin)ZRmin=ZR0;
				
		};
	};

	// tree locking
	CreateMFieldLocking(2);

	//boloto
	int ofs=0;
	for(int iy=0;iy<MaxTH;iy++){
		for(int ix=0;ix<VertInLine;ix++){
			if(TexFlags[TexMap[ofs]]&(256*64)){
				MF->BSetBar(ix<<1,iy<<1,4);
			};
			ofs++;
		};
	};

	// smoothing blocks
	for(ix=1;ix<mxx;ix++)
		for(int iy=1;iy<myy;iy++){
			int ppx=ix<<2;
			int ppy=iy<<2;
			int N=0;
			if(MF->CheckBar(ppx,ppy,4,4)){
				if(MF->CheckBar(ppx,ppy,2,2))N++;
				if(MF->CheckBar(ppx+2,ppy,2,2))N++;
				if(MF->CheckBar(ppx,ppy+2,2,2))N++;
				if(MF->CheckBar(ppx+2,ppy+2,2,2))N++;
			};
			if(N<3)MF->BClrBar(ppx,ppy,4);
			else MF->BSetBar(ppx,ppy,4);
		};

	MotionField* TMF=new MotionField;
	TMF->Allocate();
	//memcpy(TMF->MapH,MF->MapH,MAPSY*BMSX);
	memcpy(TMF->MapV,MF->MapV,MAPSY*BMSX);
	
	for(ix=1;ix<mxx;ix++)
		for(int iy=1;iy<myy;iy++){
			int ppx=ix<<2;
			int ppy=iy<<2;
			byte c=0;
			if(TMF->CheckBar(ppx+1,ppy+1-4,2,2))c|=1;
			if(TMF->CheckBar(ppx+1+4,ppy+1,2,2))c|=2;
			if(TMF->CheckBar(ppx+1,ppy+1+4,2,2))c|=4;
			if(TMF->CheckBar(ppx+1-4,ppy+1,2,2))c|=8;
			if(!TMF->CheckBar(ppx+1,ppy+1,2,2)){
				switch(c){
				case 3:
					SetLockMask(MF,ppx,ppy,"**** ***  **   *");
					break;
				case 6:
					SetLockMask(MF,ppx,ppy,"   *  ** *******");
					break;
				case 12:
					SetLockMask(MF,ppx,ppy,"*   **  *** ****");
					break;
				case 9:
					SetLockMask(MF,ppx,ppy,"******* **  *   ");
					break;
				case 7:
				case 14:
				case 13:
				case 11:
					MF->BSetBar(ppx,ppy,4);
					break;
				};
			};
		};
	//memcpy(TMF,MF,sizeof MotionField);
	//memcpy(TMF->MapH,MF->MapH,MAPSY*BMSX);
	memcpy(TMF->MapV,MF->MapV,MAPSY*BMSX);
	for(ix=1;ix<mxx;ix++)
		for(int iy=1;iy<myy;iy++){
			int ppx=ix<<2;
			int ppy=iy<<2;
			byte c=0;
			if(TMF->CheckBar(ppx+1,ppy+1-4,2,2))c|=1;
			if(TMF->CheckBar(ppx+1+4,ppy+1,2,2))c|=2;
			if(TMF->CheckBar(ppx+1,ppy+1+4,2,2))c|=4;
			if(TMF->CheckBar(ppx+1-4,ppy+1,2,2))c|=8;
			if(TMF->CheckBar(ppx+1,ppy+1,2,2)){
				switch(c){
				case 0:
					//BSetBar(ppx,ppy,0);
					break;
				case 3:
					SetLockMask(MF,ppx,ppy,"**** ***  **   *");
					break;
				case 6:
					SetLockMask(MF,ppx,ppy,"   *  ** *******");
					break;
				case 12:
					SetLockMask(MF,ppx,ppy,"*   **  *** ****");
					break;
				case 9:
					SetLockMask(MF,ppx,ppy,"******* **  *   ");
					break;
				};
			};
		};
	TMF->FreeAlloc();
	delete(TMF);

	CreateWaterLocking(MF,0,0,(MaxWX)-1,(MaxWX)-1);
}