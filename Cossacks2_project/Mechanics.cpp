#ifdef COSSACKS2

#include "ddini.h"
#include <stdlib.h>
#include "ResFile.h"
#include "Fastdraw.h"
#include "MapDiscr.h"
#include "mouse.h"
#include "mode.h"
#include "fog.h"
#include "walls.h"
#include "Nature.h"
#include <crtdbg.h>
#include <math.h>
#include "Megapolis.h"
#include "fonts.h"
#include "WeaponID.h"
#include "3DSurf.h"
#include "GSound.h"
#include <assert.h>
#include "3DMapEd.h"
#include "MapSprites.h"
#include "GP_Draw.h"
#include "RealWater.h"
#include "ZBuffer.h"
#include "NewAI.h"
#include "TopoGraf.h"
#include "StrategyResearch.h"
#include "Curve.h"
#include "NewMon.h"
#include "ActiveScenary.h"
#include "ConstStr.h"
#include "DrawForm.h"
#include "IconTool.h"
#include "Dialogs.h"
#include "Path.h"
#include "Sort.h"
#include "MPlayer.h"
#include "Graphs.h"
#include "Safety.h"
#include "EinfoClass.h"

//        
//  =F===X===F=
//        
class MechanicsQuant{
public:
	float xc0,yc0,zc0;
	float Fi0;
	float xc,yc,zc;
	float Fi;

	//int fpx[2];
	//int fpy[2];
	//int fpz[2];

	int ForcePointR[2];
	MechanicsQuant(){
		memset(&xc0,0,int(&ForcePointR)-int(&xc0)+8);
	};
	virtual void MoveQuant(int fpIndex,int dx,int dy);
};
//     =====
//       |
//  =F===X===F=
//       |
//     =====
//
class SingleAxeQuant: public MechanicsQuant{
public:
	int AxeL;
	int LeftAngle;
	int RightAngle;
	SingleAxeQuant(){
		AxeL=0;
		LeftAngle=0;
		int RightAngle=0;
	};
	~SingleAxeQuant(){

	};
	virtual void MoveQuant(int fpIndex,float dx,float dy);
};
//        /
//      /|
//  =F===X===F=
//       |/
//      /
//
class ComplexAxeQuant: public SingleAxeQuant{
public:
	int AdditionalWheelAngle;
	int MaxAdditionalWheelAngle;
	ComplexAxeQuant(){
		AdditionalWheelAngle=0;
		MaxAdditionalWheelAngle=0;
	};
	~ComplexAxeQuant();
	virtual void MoveQuant(int fpIndex,int dx,int dy);
};
void MechanicsQuant::MoveQuant(int fpIndex,int dx,int dy){
	
};
void SingleAxeQuant::MoveQuant(int fpIndex,float dx,float dy){
	float fi=Fi*3.1415/128.0;
	float DX=cos(fi);
	float DY=sin(fi);
	float DL=DX*dx+DY*dy;
	float DT=-DY*dx+DX*dy;
	xc0=xc;
	yc0=yc;
	zc0=zc;
	Fi0=Fi;
	xc+=float(DX*DL);
	yc+=float(DY*DL);
	Fi+=float(DT*407.0)/ForcePointR[fpIndex]/10.0/16.0;
	int DA=DT*AxeL/ForcePointR[fpIndex];
	LeftAngle+=DL+DA;
	RightAngle+=DL-DA;
};
void ComplexAxeQuant::MoveQuant(int fpIndex,int dx,int dy){

};
class ObjectPartAnimation{
public:
	char ID[32];
	word GPID;
	int GP_dx;
	int GP_dy;
	int NOcts;
	int StartFrame;
	int NFrames;
	int DistPerFrame;
	int SortX1;
	int SortX2;
	DWORD ObjectMask;
	__forceinline void DrawAt(int x,int y,byte Dir,int time,OneObject* OB,byte NI){
		int dx,dy;
		int SP=GetSprite(Dir,time,dx,dy);
		int SIN=TSin[Dir];
		int COS=TCos[Dir];
		int XL1=(SortX1*COS)>>4;
		int YL1=(SortX1*SIN)>>5;
		int XL2=(SortX2*COS)>>4;
		int YL2=(SortX2*SIN)>>5;
		AddLine((x+XL1)>>4,(y+YL1)>>4,(x+XL2)>>4,(y+YL2)>>4,(x>>4)+dx,(y>>4)+dy,OB,GPID,SP,0,0);
	};
	int ReadFromString(char* s){  //0 - OK, 1 - Not enough params, 2- unknown GP-name
		char cc[256];
		int z=sscanf(s,"%s%d%d%d%d%d%d%d%d",cc,&GP_dx,&GP_dy,&NOcts,&NFrames,&StartFrame,&DistPerFrame,&SortX1,&SortX2);
		if(z!=9)return 1;
		GPID=GPS.PreLoadGPImage(cc);
		if(GPID==0xFFFF)return 2;
		return 0;
	};
	__forceinline int GetSprite(byte Dir,int Time,int& dx,int& dy){
		bool INV=0;
		int FID;
		if(NOcts&1){
			int NO=(NOcts-1)*2;
			int ANG=256/NO;
			int A2=ANG>>1;
			FID=((Dir+256-64+A2)/ANG)%NO;
			if(FID>=NOcts){
				INV=1;
				FID=NO-FID;
			};
		}else{
			int NO=NOcts;
			int ANG=256/NO;
			int A2=ANG>>1;
			FID=((Dir+256-64+A2)/ANG)%NO;
		};
		int csp;
		int V=DistPerFrame;
		if(!V)V=1;
		if(V<0)csp=StartFrame+NFrames-1-(DWORD(Time)/(-V))%NFrames;
		else csp=StartFrame+(DWORD(Time)/V)%NFrames;
		if(INV){
			dx=GP_dx;
			dy=-GP_dy;
			return csp*NOcts+FID+4096;
		}else{
			dx=-GP_dx;
			dy=-GP_dy;
			return csp*NOcts+FID;
		};
	};
};
struct QuantSingleElement{
	ObjectPartAnimation* ANM;
	int dx,dy,dfi;
	byte AnmDir:4;
	byte RotDir:4;
};
class QuantPartState{
public:
	int NElm;
	QuantSingleElement* Elm;
	QuantPartState(){
		memset(this,0,sizeof *this);
	};
	~QuantPartState(){
		if(Elm)delete[](Elm);
		memset(this,0,sizeof *this);
	};
};

class QuantPartTransformElement{
public:
	byte OpType;
	int StartTime;
	int TimeAmount;
	ObjectPartAnimation* ANM;
	int x0,y0,Fi0,x1,y1,Fi1;
};
class QuantPartTransform{
public:
	int MaxTransfTime;
	byte QP_Type;
	int  NParts;
	QuantPartTransformElement** QPTE;
	int* NInQPTE;
	QuantPartTransform(){
		memset(this,NULL,sizeof *this);
	};
	~QuantPartTransform(){
		for(int i=0;i<NParts;i++)if(QPTE[i])delete[](QPTE[i]);
		if(NInQPTE)delete[](NInQPTE);
		if(QPTE)delete[](QPTE);
		memset(this,NULL,sizeof *this);
	};

};
#define NStates 16
class OneMQuantDesc{
public:
	char ID[32];
	QuantPartState* States[NStates];
	int X1;
	int X2;
	int R;
	//          ALONE  HEAD   TALE HEAD&TALE
	//STAND       0      1      2      3
	//MOTION      4      5      6      7
	//ATTACK      8      9     10     11
	//RATTACK    12     13     14     15
	QuantPartTransform* TRANS[NStates*NStates];
	OneMQuantDesc(){
		memset(this,0,sizeof *this);
	};	
};
#define MaxChains 16
class OneComplexUnitDesc{
public:
	char ID[32];
	int NQuants;
	OneMQuantDesc* Chain[MaxChains];
	bool CanBeTale :1;
	bool CanBeHead :1;
	bool CanBeAlone:1;
	bool InverseTaleMotion:1;
	OneComplexUnitDesc(){
		memset(this,0,sizeof *this);
	};
};

#define MaxOPA 256
#define MaxQD 256
#define MaxU 256
class ComplexObjDesc{
public:
	ObjectPartAnimation* OPANM[MaxOPA];
	int N_OPANM;
	OneMQuantDesc* OMQD[MaxQD];
	int N_OMQD;
	OneComplexUnitDesc* UnitDesc[MaxU];
	int NUnits;

	//--------------------------
	word GetUnitID(char* Name){
		for(int i=0;i<NUnits;i++)if(!strcmp(Name,UnitDesc[i]->ID))return i;
		return 0xFFFF;
	};
	void ReadChainObjectsDesc();
	int GetAnmIndex(char* ID){
		for(int i=0;i<N_OPANM;i++)if(!strcmp(ID,OPANM[i]->ID))return i;
		return -1;
	};
	int GetStageIndex(char* S1){
		int IDX=-1;
		if(strstr(S1,"$STAND_"))IDX=0;
		if(strstr(S1,"$MOTION_"))IDX=4;
		if(strstr(S1,"$ATTACK_"))IDX=8;
		if(strstr(S1,"$RATTACK_"))IDX=12;
		if(IDX==-1)return -1;
		if(strstr(S1,"_HEAD"))IDX++;
		else if(strstr(S1,"_TALE"))IDX+=2;
		else if(strstr(S1,"_HEAD&TALE"))IDX+=3;
		else if(!strstr(S1,"_ALONE"))return -1;
		return IDX;
	};
	ComplexObjDesc(){
		memset(this,0,sizeof *this);
	};
	~ComplexObjDesc(){
		for(int i=0;i<N_OPANM;i++)delete(OPANM[i]);
		for(int i=0;i<N_OMQD;i++)delete(OMQD[i]);
		for(int i=0;i<NUnits;i++)delete(UnitDesc[i]);
		memset(this,0,sizeof *this);
	};
};
bool ReadWinString(GFILE* F,char* STR,int Max);
void LoadErr(char* sz,...);
void ComplexObjDesc::ReadChainObjectsDesc(){
	GFILE* F=Gopen("ComplexObjects\\Objects.dat","r");
	if(F){
		int Line=0;
		char LINE[256];
		char S1[64];
		char S2[64];
		char S3[64];
		int a,b,c,z;
		int v;
		do{
			v=ReadWinString(F,LINE,255);
			Line++;
			if(v){
				char C0=LINE[0];
				char C1=LINE[1];
				if(LINE[0]!='/'){
					if(C0=='#'){
						if(C1=='A'){//#ANIM
							z=sscanf(LINE,"%s%s",S1,S2);
							if(z==2){
								if(N_OPANM<MaxOPA){
									OPANM[N_OPANM]=new ObjectPartAnimation;
									ObjectPartAnimation* OP=OPANM[N_OPANM];
									N_OPANM++;
									int v=ReadWinString(F,LINE,255);
									if(!v)LoadErr("Objects.dat, line %d : Unexpected end of file.",Line);
									Line++;
									OP->ReadFromString(LINE);
									strcpy(OP->ID,S2);
								}else LoadErr("Objects.dat, line %d : Too many animations.",Line);
							}else LoadErr("Objects.dat, line %d : incorrect #ANIM directive params.",Line);
						}else
						if(C1=='O'){//#OBJECT
							z=sscanf(LINE,"%s%s",S1,S2);
							if(z==2){
								if(N_OPANM<MaxOPA){
									OPANM[N_OPANM]=new ObjectPartAnimation;
									ObjectPartAnimation* OP=OPANM[N_OPANM];
									N_OPANM++;
									int v=ReadWinString(F,LINE,255);
									if(!v)LoadErr("Objects.dat, line %d : Unexpected end of file.",Line);
									Line++;
									OP->ReadFromString(LINE);
									strcpy(OP->ID,S2);
									OP->ObjectMask=1;
								}else LoadErr("Objects.dat, line %d : Too many animations.",Line);
							}else LoadErr("Objects.dat, line %d : incorrect #ANIM directive params.",Line);
						}else
						if(C1=='M'){//#MQUANT
							z=sscanf(LINE,"%s%s%d%d%d",S1,S2,&a,&b,&c);
							if(z==5){
								if(N_OMQD<MaxQD){
									OMQD[N_OMQD]=new OneMQuantDesc;
									OneMQuantDesc* OM=OMQD[N_OMQD];
									N_OMQD++;
									strcpy(OM->ID,S2);
									OM->X1=a;
									OM->X2=b;
									//OM->R=r;
									for(int i=0;i<c;i++){
										int v=ReadWinString(F,LINE,255);
										if(!v)LoadErr("Objects.dat, line %d : Unexpected end of file.",Line);
										Line++;
										z=sscanf(LINE,"%s%d",S1,&a);
										if(z&&S1[0]=='@'){
											if(S1[1]=='T'){//@TRANSFORM
												z=sscanf(LINE,"%s%s%s",S1,S2,S3);
												if(z==3){
													int IDX1=GetStageIndex(S2);
													int IDX2=GetStageIndex(S3);
													if(IDX1==-1)LoadErr("Objects.dat, line %d : Unknown stage: %s.",Line,S2);
													if(IDX2==-1)LoadErr("Objects.dat, line %d : Unknown stage: %s.",Line,S3);
													int N0=OM->States[IDX1]->NElm;
													int IDX=IDX1+IDX2*NStates;
													QuantPartTransform* QPT=OM->TRANS[IDX]=new QuantPartTransform;
													QPT->QPTE=new QuantPartTransformElement* [N0];
													QPT->NInQPTE=new int[N0];
													QPT->NParts=N0;
													QPT->QP_Type=0;
													int TMax=0;
													for(int q=0;q<N0;q++){
														int TM=0;
														QuantPartTransformElement** pQC=QPT->QPTE;
														int v=ReadWinString(F,LINE,255);
														if(!v)LoadErr("Objects.dat, line %d : Unexpected end of file.",Line);
														Line++;
														int n1;
														z=sscanf(LINE,"%s%d",S1,&n1);
														if(z){
															if(z==1){
																pQC[q]=NULL;
																QPT->NInQPTE[q]=0;
															}else{
																pQC[q]=new QuantPartTransformElement[n1];
																QPT->NInQPTE[q]=n1;
																for(int p=0;p<n1;p++){
																	QuantPartTransformElement* QC=pQC[q]+p;
																	int v=ReadWinString(F,LINE,255);
																	if(!v)LoadErr("Objects.dat, line %d : Unexpected end of file.",Line);
																	Line++;
																	z=sscanf(LINE,"%d%s%d%d%d%d%d%d",&QC->TimeAmount,S1,
																		&QC->x0,&QC->y0,&QC->Fi0,&QC->x1,&QC->y1,&QC->Fi1);
																	if(z==8){
																		int w=GetAnmIndex(S1);
																		if(w==-1)LoadErr("Objects.dat, line %d : Unknown animation: %s.",Line,S2);
																		QC->ANM=OPANM[w];
																		QC->StartTime=TM;
																		TM+=QC->TimeAmount;
																	}else LoadErr("Objects.dat, line %d : Unrecognized string.",Line);
																};
																if(TM>TMax)TMax=TM;
															};
														}else LoadErr("Objects.dat, line %d : Unrecognized string.",Line);
													};
													QPT->MaxTransfTime=TMax;
												}else LoadErr("Objects.dat, line %d : Unexpected end of file.",Line);
											}else
											if(S1[1]=='D'){//@DIRECT
												z=sscanf(LINE,"%s%s%s",S1,S2,S3);
												if(z==3){
													int IDX1=GetStageIndex(S2);
													int IDX2=GetStageIndex(S3);
													if(IDX1==-1)LoadErr("Objects.dat, line %d : Unknown stage: %s.",Line,S2);
													if(IDX2==-1)LoadErr("Objects.dat, line %d : Unknown stage: %s.",Line,S3);
													int IDX=IDX1+IDX2*NStates;
													QuantPartTransform* QPT=OM->TRANS[IDX]=new QuantPartTransform;
													QPT->QPTE=NULL;
													QPT->NParts=0;
													QPT->QP_Type=255;											
												}else LoadErr("Objects.dat, line %d : Incorrect params for @DIRECT directive.",Line);
											}else LoadErr("Objects.dat, line %d : Unknown transformation directive.",Line);
										}else{
											if(z==2){
												int IDX=GetStageIndex(S1);
												if(IDX==-1)LoadErr("Objects.dat, line %d : Unknown animation type %s.",Line,S1);
												OM->States[IDX]=new QuantPartState;
												QuantPartState* QPS=OM->States[IDX];
												QPS->NElm=a;
												QPS->Elm=new QuantSingleElement[a];
												for(int j=0;j<a;j++){
													int v=ReadWinString(F,LINE,255);
													if(!v)LoadErr("Objects.dat, line %d : Unexpected end of file.",Line);
													Line++;
													int q,w,e;
													e=0;
													z=sscanf(LINE,"%s%d%d%s%d",S1,&q,&w,&S2,&e);
													if(z>=4){
														int aid=GetAnmIndex(S1);
														if(aid==-1)LoadErr("Objects.dat, line %d : Unknown animation: %s.",Line,S1);
														QPS->Elm[j].ANM=OPANM[aid];
														QPS->Elm[j].dx=q;
														QPS->Elm[j].dy=w;
														QPS->Elm[j].dfi=e;
														switch(S2[0]){
														case 'L':
															QPS->Elm[j].AnmDir=0;
															break;
														case 'R':
															QPS->Elm[j].AnmDir=2;
															break;
														default:
															QPS->Elm[j].AnmDir=1;
															break;
														};
														switch(S2[1]){
														case 'F':
															QPS->Elm[j].RotDir=0;
															break;
														case 'B':
															QPS->Elm[j].RotDir=1;
															break;
														default:
															QPS->Elm[j].RotDir=0;
															break;
														};
													};
												};
											}else{
												z=sscanf(LINE,"%s%s",S1,S2);
												if(z==2){
													int IDX1=GetStageIndex(S1);
													int IDX2=GetStageIndex(S2);
													if(IDX1==-1)LoadErr("Objects.dat, line %d : Unknown animation type %s.",Line,S1);
													if(IDX2==-1)LoadErr("Objects.dat, line %d : Unknown animation type %s.",Line,S2);
													OM->States[IDX1]=new QuantPartState;
													QuantPartState* QPS1=OM->States[IDX1];
													QuantPartState* QPS2=OM->States[IDX2];
													QPS1->NElm=QPS2->NElm;
													QPS1->Elm=new QuantSingleElement[QPS2->NElm];
													memcpy(QPS1->Elm,QPS2->Elm,QPS2->NElm*sizeof QuantSingleElement);
												}else LoadErr("Objects.dat, line %d : incorrect #MQUANT directive params.",Line);
											};
										};
									};
								}else LoadErr("Objects.dat, line %d : Too many quants.",Line);
							}else LoadErr("Objects.dat, line %d : incorrect #MQUANT directive params.",Line);
						}else
						if(C1=='U'){//UNIT
							int N;
							z=sscanf(LINE,"%s%s%s",S1,S2,S2);
							if(z==3){
								if(NUnits<MaxU){
									OneComplexUnitDesc* OCUD=UnitDesc[NUnits]=new OneComplexUnitDesc;
									strcpy(OCUD->ID,S2);
									char CC[MaxChains][32];
									z=sscanf(LINE,"%s %s %s%s%s%s%s%s5s%s%s%s%s%s%s%s%s",S1,S2,
										CC[0],CC[1],CC[2],CC[3],CC[4],CC[5],CC[6],CC[7],
										CC[8],CC[9],CC[10],CC[11],CC[12],CC[13],CC[14],CC[15]);
									N=z-2;
									OCUD->NQuants=N;
									for(int p=0;p<N;p++){
										OCUD->Chain[p]=NULL;
										for(int w=0;w<N_OMQD;w++)if(!strcmp(CC[p],OMQD[w]->ID)){
											OCUD->Chain[p]=OMQD[w];
										};
										if(!OCUD->Chain[p])LoadErr("Objects.dat, line %d : Unknown MQUANT: %s.",Line,CC[p]);
										else{
											if(p==0&&OCUD->Chain[p]->States[1])OCUD->CanBeHead=1;
											if(p==N-1&&OCUD->Chain[p]->States[2])OCUD->CanBeTale=1;
											OCUD->CanBeAlone=1;
										};
									};
									NUnits++;
								}else LoadErr("Objects.dat, line %d : Too many units.",Line);
							}else LoadErr("Objects.dat, line %d : Invalid params for #UNIT directive.",Line);
						}else LoadErr("Objects.dat, line %d : incorrect #MQUANT directive params.",Line);
					};
				};
			};
			}while(v);
		Gclose(F);
	};	
};

class OneComplexObject{
public:
	SingleAxeQuant* MQ[8];
	byte  NQuants;
	byte  NI;
	word  CDIndex;
	word  ObjIndex;
	byte Lx;
	int StartState;
	int FinalState;
	int TransTime;
	int GroundMotionState;
	int GroundStandState;


	word LeaderID;
	word LeaderSN;
	word TaleID;
	word TaleSN;

	void CheckConnection(){
		if(LeaderID!=0xFFFF){
			OneObject* OB=Group[LeaderID];
			if((!OB)||OB->Sdoxlo||OB->Serial!=LeaderSN)LeaderID=0xFFFF;
		};
		if(TaleID!=0xFFFF){
			OneObject* OB=Group[TaleID];
			if((!OB)||OB->Sdoxlo||OB->Serial!=TaleSN)TaleID=0xFFFF;
		};
		if(LeaderID==0xFFFF&&TaleID==0xFFFF){
			GroundStandState=0;
			GroundMotionState=4;
		}else
		if(LeaderID!=0xFFFF&&TaleID!=0xFFFF){
			GroundStandState=3;
			GroundMotionState=7;			
		}else
		if(LeaderID!=0xFFFF){
			GroundStandState=2;
			GroundMotionState=6;
		}else{
			GroundStandState=1;
			GroundMotionState=5;
		};

	};
	void PerformShift(int dx,int dy);
	void PerformBackShift(int dx,int dy);
	void PerformRotation(char Final);
	OneComplexObject(){
		memset(this,0,sizeof *this);
	};
	~OneComplexObject(){
		for(int i=0;i<4;i++)if(MQ[i])delete(MQ[i]);
		memset(this,0,sizeof *this);
	};
	void TryToTransform(int FinState);
	void TryToTransformToStandState();
	void TryToTransformToMotionState();
	void GetHeadPoint(int& x,int& y){
		x=MQ[0]->xc+((MQ[0]->ForcePointR[0]*TCos[byte(MQ[0]->Fi)])>>4);
		y=MQ[0]->yc+((MQ[0]->ForcePointR[0]*TSin[byte(MQ[0]->Fi)])>>4);
	};
	void GetTalePoint(int& x,int& y){
		x=MQ[NQuants-1]->xc+((MQ[NQuants-1]->ForcePointR[1]*TCos[byte(MQ[NQuants-1]->Fi)])>>4);
		y=MQ[NQuants-1]->yc+((MQ[NQuants-1]->ForcePointR[1]*TSin[byte(MQ[NQuants-1]->Fi)])>>4);
	};
};

ComplexObjDesc CODS;
OneComplexObject* CGroup[1024];
int MaxCObject=0;
void OneComplexObject::PerformRotation(char Final){
	char FI=MQ[0]->Fi;
	char DFI=FI-Final;
	if(abs(DFI)>20){
		TryToTransformToMotionState();
		if(StartState==GroundMotionState){
			int dx=-TSin[byte(FI)]>>5;
			int dy= TCos[byte(FI)]>>5;
			if(DFI<0){
				dx=-dx;
				dy=-dy;
			};
			PerformShift(dx,dy);
		};
	}else{
		TryToTransformToStandState();
	};
};
void XorBlockComplexUnit(OneComplexObject* COB);
void OneComplexObject::PerformShift(int dx,int dy){
	XorBlockComplexUnit(this);
	for(int j=0;j<NQuants;j++){
		SingleAxeQuant* SA=MQ[j];
		if(j==0){
			SA->MoveQuant(0,dx,dy);
		}else{
			SingleAxeQuant* SP=MQ[j-1];
			float fi=SP->Fi*3.1415/128.0;
			float XC0=SP->xc+(SP->ForcePointR[1]*cos(fi)*16.0);
			float YC0=SP->yc+(SP->ForcePointR[1]*sin(fi)*16.0);

			fi=SA->Fi*3.1415/128.0;
			float XC1=SA->xc+(SA->ForcePointR[0]*cos(fi)*16.0);
			float YC1=SA->yc+(SA->ForcePointR[0]*sin(fi)*16.0);
			float DX=XC0-XC1;
			float DY=YC0-YC1;
			SA->MoveQuant(0,DX,DY);
		};
	};
	if(TaleID!=0xFFFF){
		OneObject* OB=Group[TaleID];
		if(OB&&!OB->Sdoxlo){
			if(OB->CObjIndex!=0xFFFF){
				OneComplexObject* COB=CGroup[OB->CObjIndex];
				if(COB){
					OneComplexUnitDesc* OCUD=CODS.UnitDesc[CGroup[OB->CObjIndex]->CDIndex];
					int xc,yc;
					if(CODS.UnitDesc[CDIndex]->InverseTaleMotion){
						GetHeadPoint(xc,yc);
					}else{
						GetTalePoint(xc,yc);
					};
					if(OCUD->InverseTaleMotion){
						int x,y;
						COB->GetTalePoint(x,y);
						COB->PerformBackShift(xc-x,yc-y);
					}else{
						int x,y;
						COB->GetHeadPoint(x,y);
						COB->PerformShift(xc-x,yc-y);
					};
				};
			};
		};
	};
	XorBlockComplexUnit(this);
};
void OneComplexObject::PerformBackShift(int dx,int dy){
	XorBlockComplexUnit(this);
	for(int j=NQuants-1;j>=0;j--){
		SingleAxeQuant* SA=MQ[j];
		if(j==NQuants-1){
			SA->MoveQuant(1,dx,dy);
		}else{
			SingleAxeQuant* SP=MQ[j+1];
			int XC0=SP->xc+((SP->ForcePointR[0]*TCos[byte(SP->Fi)])>>4);
			int YC0=SP->yc+((SP->ForcePointR[0]*TSin[byte(SP->Fi)])>>4);

			int XC1=SA->xc+((SA->ForcePointR[1]*TCos[byte(SA->Fi)])>>4);
			int YC1=SA->yc+((SA->ForcePointR[1]*TSin[byte(SA->Fi)])>>4);
			SA->MoveQuant(1,XC0-XC1,YC0-YC1);
		};
	};
	if(TaleID!=0xFFFF){
		OneObject* OB=Group[TaleID];
		if(OB&&!OB->Sdoxlo){
			if(OB->CObjIndex!=0xFFFF){
				OneComplexObject* COB=CGroup[OB->CObjIndex];
				if(COB){
					OneComplexUnitDesc* OCUD=CODS.UnitDesc[OB->CObjIndex];
					int xc,yc;
					if(CODS.UnitDesc[CDIndex]->InverseTaleMotion){
						GetTalePoint(xc,yc);
					}else{
						GetHeadPoint(xc,yc);
					};
					if(OCUD->InverseTaleMotion){
						int x,y;
						GetTalePoint(x,y);
						COB->PerformBackShift(xc-x,yc-y);
					}else{
						int x,y;
						GetHeadPoint(x,y);
						COB->PerformBackShift(xc-x,yc-y);
					};
				};
			};
		};
	};
	XorBlockComplexUnit(this);
};


void ReadChainObjectsDesc(){
	CODS.ReadChainObjectsDesc();
};
word GetComplexObjectIndex(char* Name){
	return CODS.GetUnitID(Name);
};
extern int RealLx;
extern int RealLy;
extern int tmtmt;

void DrawComplexObject(OneComplexObject* COB,int sdx,int sdy){
	int N=COB->NQuants;
	OneComplexUnitDesc* COD=CODS.UnitDesc[COB->CDIndex];
	sdx<<=4;
	sdy<<=4;
	int RLX=(RealLx<<4)+128*16;
	int RLY=(RealLy<<4)+128*16;
	OneObject* OB=NULL;
	if(COB->ObjIndex!=0xFFFF){
		OB=Group[COB->ObjIndex];
	};
	for(int i=0;i<N;i++){
		SingleAxeQuant* SA=COB->MQ[i];
		int COS=TCos[byte(SA->Fi)];
		int SIN=TSin[byte(SA->Fi)];
		int gx=SA->xc-sdx;
		int gy=(SA->yc/2)-sdy;
		if(gx>-128*16&&gy>-128*16&&gx<RLX&&gy<RLY){
			OneMQuantDesc* OCD=COD->Chain[i];
			QuantPartState* QPS=OCD->States[COB->FinalState];
			if(COB->StartState==COB->FinalState){
DR1:
				if(QPS){
					for(int p=0;p<QPS->NElm;p++){
						QuantSingleElement* QSE=QPS->Elm+p;
						int T=0;
						int A0=(SA->LeftAngle+SA->RightAngle)/2;
						int DA=(SA->RightAngle-SA->LeftAngle)*abs(QSE->dy)/128;
						switch(QSE->AnmDir){
						case 0:
							T=A0-DA;
							break;
						case 1:
							T=A0;
							break;
						case 2:
							T=A0+DA;
							break;
						};
						if(QSE->RotDir)T=-T;
						int dx=gx+((QSE->dx*COS-QSE->dy*SIN)>>4);
						int dy=gy+((QSE->dy*COS+QSE->dx*SIN)>>5);
						if(QSE->ANM)QSE->ANM->DrawAt(dx,dy,SA->Fi+QSE->dfi,T,OB,OB?OB->NNUM:0);
					};
				};
			}else{
				//drawing intermediate states
				OneMQuantDesc* OMQD=CODS.UnitDesc[COB->CDIndex]->Chain[i];
				QuantPartTransform* QPT=OMQD->TRANS[COB->StartState+COB->FinalState*NStates];
				if(QPT&&QPT->QP_Type==0&&QPS){
					for(int i=0;i<QPT->NParts;i++){
						if(QPT->NInQPTE[i]==0||QPT->MaxTransfTime<=COB->TransTime){
							QuantSingleElement* QSE=QPS->Elm+i;
							int T=0;
							int A0=(SA->LeftAngle+SA->RightAngle)/2;
							int DA=(SA->RightAngle-SA->LeftAngle)*QSE->dy/128;
							switch(QSE->AnmDir){
							case 0:
								T=A0-DA;
								break;
							case 1:
								T=A0;
								break;
							case 2:
								T=A0+DA;
								break;
							};
							int dx=gx+((QSE->dx*COS-QSE->dy*SIN)>>4);
							int dy=gy+((QSE->dy*COS+QSE->dx*SIN)>>5);
							if(QSE->ANM)QSE->ANM->DrawAt(dx,dy,SA->Fi+QSE->dfi,T,OB,OB?OB->NNUM:0);
						}else{
							QuantPartTransformElement* QPTE=QPT->QPTE[i];
							int N=QPT->NInQPTE[i];
							int T=COB->TransTime;
							for(int v=0;v<N;v++){
								if(T>=QPTE->StartTime&&T<QPTE->StartTime+QPTE->TimeAmount){
									int D=QPTE->TimeAmount;
									int DT=COB->TransTime-QPTE->StartTime;
									int x=QPTE->x0+(QPTE->x1-QPTE->x0)*DT/D;
									int y=QPTE->y0+(QPTE->y1-QPTE->y0)*DT/D;
									int Fi=QPTE->Fi0+(QPTE->Fi1-QPTE->Fi0)*DT/D;
									int dx=gx+((x*COS-y*SIN)>>4);
									int dy=gy+((y*COS+x*SIN)>>5);
									if(QPTE->ANM)QPTE->ANM->DrawAt(dx,dy,Fi,DT,OB,OB?OB->NNUM:0);
									break;
								};
								QPTE++;
							};
						};
					};
				}else goto DR1;
			};
		};
	};
};
void DrawAllComplexObjects(){
	for(int i=0;i<MaxCObject;i++){
		OneComplexObject* OCO=CGroup[i];
		if(OCO){
			DrawComplexObject(OCO,mapx<<5,mapy<<4);
		};
	};
};
word GetDir(int dx,int dy);
word getDir(int dx,int dy);
void GetForceActOnBar(int x,int y,int L,int& fx,int& fy,int Media=0);
void XorBlockComplexUnit(OneObject* OB);
void MakeSeveralStepsBack(OneObject* OB,char Dir,int N);
bool CheckStopRuleWhenLocked(OneObject* OB);
int PreCheckMotionRules(OneObject* OB);
void MotionHandlerForComplexObjects(OneObject* OB){
	if(OB->CObjIndex!=0xFFFF){
		OB->PathDelay=0;
		OneComplexObject* COB=CGroup[OB->CObjIndex];
		if(COB){
			COB->CheckConnection();
			if(COB->LeaderID!=0xFFFF)return;

			bool AllReady=1;
			OneComplexObject* CURCOB=COB;
			do{
				if(CURCOB->StartState!=CURCOB->FinalState){
					bool fail=0;
					for(int i=0;i<CURCOB->NQuants;i++){
						OneMQuantDesc* OMQD=CODS.UnitDesc[CURCOB->CDIndex]->Chain[i];
						QuantPartTransform* QPT=OMQD->TRANS[CURCOB->StartState+CURCOB->FinalState*NStates];
						if(QPT){
							if(QPT->QP_Type!=255&&CURCOB->TransTime<QPT->MaxTransfTime)fail=1;
						}else fail=1;
					};
					if(!fail){
						CURCOB->StartState=CURCOB->FinalState;
						assert(!(COB->StartState==1&&COB->FinalState==2));
						assert(!(CURCOB->StartState==1&&CURCOB->FinalState==2));
						CURCOB->TransTime=0;
					}else{
						CURCOB->TransTime++;
					};
					if(fail)AllReady=0;
				};
				if(CURCOB->TaleID!=0xFFFF){
					OneObject* NOB=Group[CURCOB->TaleID];
					if(NOB){
						if(NOB->CObjIndex!=0xFFFF){
							CURCOB=CGroup[NOB->CObjIndex];
							if(!CURCOB)break;
						}else break;
					}else break;
				}else break;
			}while(1);
			if(AllReady){
				OB->RealX=COB->MQ[0]->xc;
				OB->RealY=COB->MQ[0]->yc;
				OB->x=(OB->RealX-((OB->Lx)<<7))>>8;
				OB->y=(OB->RealY-((OB->Lx)<<7))>>8;
				int RULE=OB->DestX>=0?PreCheckMotionRules(OB):0;
				if(OB->DestX<0||RULE==1){

					//COB->PerformShift(0,0);
					//COB->TryToTransform(COB->GroundStandState);
					COB->TryToTransformToStandState();
					return;
				};
				int RX,RY;
				bool Back=OB->BackMotion;
				if(Back){
					COB->GetTalePoint(RX,RY);
				}else{
					RX=OB->RealX;
					RY=OB->RealY;
				};
				int dx=OB->DestX-RX;
				int dy=OB->DestY-RY;
				int LX=OB->Lx;
				int fx=0;
				int fy=0;
				XorBlockComplexUnit(COB);
				if(!OB->UnlimitedMotion)GetForceActOnBar(OB->RealX>>4,OB->RealY>>4,LX,fx,fy,OB->Media);
				XorBlockComplexUnit(COB);
				int N=Norma(dx,dy);
				if(RULE==2){
					fx-=(dy<<7)/N;
					fy+=(dx<<7)/N;
				};
				int MaxD=16;
				if(COB->NQuants==1)MaxD=127;
				if(fx||fy){
					int FN=Norma(fx,fy);
					if(FN>300){
						if(CheckStopRuleWhenLocked(OB)){
							OB->DestX=-1;
							OB->DestY=-1;
							COB->TryToTransformToStandState();
							return;
						};
						if(MaxD<32){
							MaxD=32;
							//OB->NIPoints=0;
							int SC=(dx*fx+dy*fy)*256/Norma(dx,dy)/Norma(fx,fy);
							if(SC>100){
								MakeSeveralStepsBack(OB,GetDir(fx,fy)+128,64);
								//return;
							};
						};
					};
					if(N>128){
						dx=(dx<<8)/N;
						dy=(dy<<8)/N;
					};
					dx+=fx;
					dy+=fy;
					
					int N=Norma(dx,dy);
				};
				if(COB->NQuants==1)MaxD=127;
				int N0=Back?32:64;
				if(N>N0){
					if(COB->StartState==COB->GroundMotionState){
						char Ang=char(GetDir(dx,dy));
						if(Back){
							char A0=char(int(COB->MQ[COB->NQuants-1]->Fi));
							A0=128+A0;
							char D=Ang-A0;
							if(abs(D)>16){
								if(D<0)D=-16;
								else D=16;
							};
							Ang=A0+D;
							dx=TCos[byte(Ang)];
							dy=TSin[byte(Ang)];
							int N0=Norma(dx,dy);
							//if(N0>N){
								dx=dx*N/N0;
								dy=dy*N/N0;
							//};
						}else{
							char A0=char(int(COB->MQ[0]->Fi));
							char D=Ang-A0;
							if(abs(D)>MaxD){
								if(D<0)D=-MaxD;
								else D=MaxD;
							};
							Ang=A0+D;
							dx=TCos[byte(Ang)];
							dy=TSin[byte(Ang)];
							int N0=Norma(dx,dy);
							dx=dx*N/N0;
							dy=dy*N/N0;
						};
						int V=(N-N0+12)>>1;
						if(V>OB->newMons->MotionDist)V=OB->newMons->MotionDist;
						dx=dx*V/N;
						dy=dy*V/N;
						if(Back)COB->PerformBackShift(dx,dy);
						else COB->PerformShift(dx,dy);
					}else{
						//COB->TryToTransform(COB->GroundMotionState);
						COB->TryToTransformToMotionState();
					};
				}else{
ONSTOP:
					OB->DestX=-1;
					OB->DestY=-1;
					//COB->TryToTransform(COB->GroundStandState);
					COB->TryToTransformToStandState();
				};
			};
		};
	};
};
#define CONFI(x) ((byte(x)+2)&252)
void SetStartConditionsForComplexObject(OneComplexObject* COB,OneComplexUnitDesc* COD,int x,int y,byte Dir){
	for(int j=0;j<COB->NQuants;j++){
		SingleAxeQuant* SA=COB->MQ[j];
		SA->Fi=Dir;
		SA->zc=0;
		SA->ForcePointR[0]=COD->Chain[j]->X1;
		SA->ForcePointR[1]=COD->Chain[j]->X2;
		if(j==0){
			SA->xc=x<<4;
			SA->yc=y<<4;
		}else{
			SingleAxeQuant* SP=COB->MQ[j-1];
			SA->xc=SP->xc+((SP->ForcePointR[1]*TCos[CONFI(SP->Fi)])>>4)-((SA->ForcePointR[0]*TCos[CONFI(SA->Fi)])>>4);
			SA->yc=SP->yc+((SP->ForcePointR[1]*TSin[CONFI(SP->Fi)])>>4)-((SA->ForcePointR[0]*TSin[CONFI(SA->Fi)])>>4);
		};
		SA->xc0=SA->xc;
		SA->yc0=SA->yc;
		SA->zc0=SA->zc;
		SA->AxeL=64;//COD->Chain[j]->R;//AxeDistance!!!  //COD->OCD[j].AxeDist;
		SA->LeftAngle=0;
		SA->RightAngle=0;
	};
};
void OneComplexObject::TryToTransform(int FinState){
	if(StartState==FinalState){
		int T01=FinalState&3;
		int T02=FinalState>>2;
		int T11=FinState&3;
		int T12=FinState>>2;
		if(T01==T11)FinalState=FinState;
		else if(T02==0)FinalState=T11;
		else FinalState=T01;
		/*
		ComplexUnitDesc* OCUD=CODS.UnitDesc[CDIndex];
		QuantPartTransform* QPT=OMQD->TRANS[StartState+FinalState*NStates];
		bool ready
		if(QPT&&QPT->QP_Type==255)StartState=FinalState;
		*/
	};
	/*
	if(TaleID!=0xFFFF){
		OneObject* TOB=Group[TaleID];
		if(TOB&&TOB->Serial==TaleSN&&TOB->CObjIndex!=0xFFFF){
			OneComplexObject* COB=CGroup[TOB->CObjIndex];
			if(COB)COB->TryToTransform(FinState);
		};
	};
	*/
};
void OneComplexObject::TryToTransformToStandState(){
	TryToTransform(GroundStandState);
	if(TaleID!=0xFFFF){
		OneObject* TOB=Group[TaleID];
		if(TOB&&TOB->Serial==TaleSN&&TOB->CObjIndex!=0xFFFF){
			OneComplexObject* COB=CGroup[TOB->CObjIndex];
			if(COB)COB->TryToTransformToStandState();
		};
	};
};
void OneComplexObject::TryToTransformToMotionState(){
	TryToTransform(GroundMotionState);
	if(TaleID!=0xFFFF){
		OneObject* TOB=Group[TaleID];
		if(TOB&&TOB->Serial==TaleSN&&TOB->CObjIndex!=0xFFFF){
			OneComplexObject* COB=CGroup[TOB->CObjIndex];
			if(COB)COB->TryToTransformToMotionState();
		};
	};
};
int CreateComplexObjectAt(int x,int y,word Type,byte NI,word Owner,byte StartDir){
	int ID=-1;
	for(int i=0;i<MaxCObject;i++)if(!CGroup[i]){
		ID=i;
		break;
	};
	if(ID==-1){
		if(MaxCObject<1023){
			ID=MaxCObject;
			MaxCObject++;
		}else return -1;		
	};
	OneComplexObject* COB=CGroup[ID]=new OneComplexObject;
	OneComplexUnitDesc* COD=CODS.UnitDesc[Type];
	COB->CDIndex=Type;
	COB->NI=NI;
	COB->ObjIndex=Owner;
	for(int i=0;i<COD->NQuants;i++)COB->MQ[i]=new SingleAxeQuant;
	COB->NQuants=COD->NQuants;
	SetStartConditionsForComplexObject(COB,COD,x,y,StartDir);
	COB->Lx=Group[Owner]->Lx;
	COB->StartState=0;
	COB->FinalState=0;
	COB->LeaderID=0xFFFF;
	COB->TaleID=0xFFFF;
	XorBlockComplexUnit(COB);
	return ID;
};
bool CheckObjectLikeHead(word MID){
	OneObject* OB=Group[MID];
	if(OB&&!OB->Sdoxlo){
		if(OB->CObjIndex==0xFFFF)return false;
		OneComplexObject* COB=CGroup[OB->CObjIndex];
		if(COB->TaleID!=0xFFFF)return false;
		OneComplexUnitDesc* OCUD=CODS.UnitDesc[COB->CDIndex];
		return OCUD->CanBeHead;
	};
	return false;
};
bool CheckObjectLikeTale(word MID){
	OneObject* OB=Group[MID];
	if(OB&&!OB->Sdoxlo){
		if(OB->CObjIndex==0xFFFF)return false;
		OneComplexObject* COB=CGroup[OB->CObjIndex];
		if(COB->TaleID!=0xFFFF)return false;
		OneComplexUnitDesc* OCUD=CODS.UnitDesc[COB->CDIndex];
		return OCUD->CanBeTale;
	};
	return false;
};
void ConnectToObjectLink(OneObject* OBJ){
	word Head=OBJ->LocalOrder->info.BuildObj.ObjIndex;
	word HSN= OBJ->LocalOrder->info.BuildObj.SN;
	byte STAGE=OBJ->LocalOrder->info.BuildObj.AttMethod;
	OneObject* HOB=Group[Head];
	if(HOB&&HOB->Serial==HSN&&!HOB->Sdoxlo){
		OneComplexObject*TCOB=CGroup[OBJ->CObjIndex];
		OneComplexObject* HCOB=CGroup[HOB->CObjIndex];
		OneComplexUnitDesc* TOCUD=CODS.UnitDesc[HCOB->CDIndex];
		OneComplexUnitDesc* HOCUD=CODS.UnitDesc[HCOB->CDIndex];
		int xhh,yhh,xht,yht;
		HCOB->GetHeadPoint(xhh,yhh);
		HCOB->GetTalePoint(xht,yht);
		int dx=xht-xhh;
		int dy=yht-yhh;
		int N=Norma(dx,dy);
		dx=dx*160*16/N;
		dy=dy*160*16/N;
		
		int xdst=xht+dx;
		int ydst=yht+dy;
		int xth,yth;
		TCOB->GetHeadPoint(xth,yth);
		int R=Norma(xth-xdst,yth-ydst);
		
		if(R>16*300){
			OBJ->NewMonsterSmartSendTo(xdst>>4,ydst>>4,0,0,128+16,1);
			//xdst+=dx;
			//ydst+=dx;
			//OBJ->NewMonsterSmartSendTo(xdst>>4,ydst>>4,0,0,128+16,1);
			return;
		}else{
			OBJ->DestX=xht;
			OBJ->DestY=yht;
			OBJ->UnlimitedMotion=1;
			if(Norma(xth-xht,yth-yht)<64){
				OBJ->BackMotion=0;
				OBJ->DestX=-1;
				OBJ->DestY=-1;
				OBJ->DeleteLastOrder();
				HCOB->TaleID=OBJ->Index;
				HCOB->TaleSN=OBJ->Serial;
				TCOB->LeaderID=HOB->Index;
				TCOB->LeaderSN=HOB->Serial;
				HCOB->GroundMotionState=5;
				HCOB->GroundStandState=1;
				TCOB->GroundMotionState=6;
				TCOB->GroundStandState=2;
				HCOB->TryToTransformToStandState();
				TCOB->TryToTransformToStandState();
				return;

			};
		};
		/*
		int xhh,yhh,xht,yht,xtc,ytc;
		HCOB->GetHeadPoint(xhh,yhh);
		HCOB->GetTalePoint(xht,yht);
		if(TOCUD->InverseTaleMotion){
			TCOB->GetTalePoint(xtc,ytc);
		}else{
			TCOB->GetHeadPoint(xtc,ytc);
		};
		int R=Norma(xhh-xtc,yhh-ytc)>>4;
		if(R<400&&STAGE==0){
			STAGE=1;
			OBJ->LocalOrder->info.BuildObj.AttMethod=1;
		};
		if(STAGE==1){
			if(Norma(xht-xtc,yht-ytc)<64){
				OBJ->BackMotion=0;
				OBJ->DestX=-1;
				OBJ->DestY=-1;
				OBJ->DeleteLastOrder();
				HCOB->TaleID=Tale;
				HCOB->TaleSN=TSN;
				TCOB->LeaderID=OBJ->Index;
				TCOB->LeaderSN=OBJ->Serial;
				HCOB->GroundMotionState=5;
				HCOB->GroundStandState=1;
				TCOB->GroundMotionState=6;
				TCOB->GroundStandState=2;
				HCOB->TryToTransformToStandState();
				TCOB->TryToTransformToStandState();
				return;
			}else{
				OBJ->BackMotion=1;
				OBJ->DestX=xtc;
				OBJ->DestY=ytc;
				//TCOB->PerformRotation(GetDir(xtc-OBJ->RealX,ytc-OBJ->RealY));
				//TOB->BackMotion=1;
				TOB->DestX=xht;
				TOB->DestY=yht;
			};
		}else{
			OBJ->CreatePath(xtc>>8,ytc>>8);
		};
		*/
	}else{
		OBJ->DeleteLastOrder();
		OBJ->BackMotion=0;
	};
};
void ConnectToObject(word Head,word Tail){
	OneObject* HOB=Group[Head];
	if(HOB&&HOB->CObjIndex!=0xFFFF&&!HOB->Sdoxlo){
		OneComplexObject*HCOB=CGroup[HOB->CObjIndex];
		if(HCOB){
			OneComplexUnitDesc* HOCUD=CODS.UnitDesc[HCOB->CDIndex];
			if(HOCUD->CanBeTale){
				OneObject* TOB=Group[Tail];
				if(TOB&&!TOB->Sdoxlo){
					OneComplexObject*TCOB=CGroup[TOB->CObjIndex];
					if(TCOB){
						OneComplexUnitDesc* TOCUD=CODS.UnitDesc[TCOB->CDIndex];
						if(TOCUD->CanBeHead){
							Order1* OR1=HOB->CreateOrder(0);
							OR1->info.BuildObj.ObjIndex=Tail;
							OR1->info.BuildObj.SN=TOB->Serial;
							OR1->info.BuildObj.AttMethod=0;
							OR1->DoLink=&ConnectToObjectLink;
							HOB->BackMotion=0;
						};
					};
				};
			};
		};
	};
};
int CONNECTPTR=1;
void SetCurPtr(int v);
void CmdConnectObjs(byte NI,word ID,word CurObj);
void TestConnector(word CurObj,byte NI){
	if(ImNSL[NI]==1&&ImSelm[NI]){
		word ID=ImSelm[NI][0];
		if(ID!=0xFFFF){
			if(CheckObjectLikeTale(ID)&&CheckObjectLikeHead(CurObj)){
				SetCurPtr(CONNECTPTR);
				if(Rpressed){
					Rpressed=0;
					CmdConnectObjs(NI,ID,CurObj);
				};
			};
		};
	};
};
bool CheckDirectConnectionToDestPoint(int xc,int yc,int x,int y){
	if(Norma(xc-x,yc-y)>30)return false;
	int sx=xc>x?-1:1;
	int sy=yc>y?-1:1;
	int ax=abs(xc-x);
	int ay=abs(yc-y);
	int xp=xc;
	int yp=yc;
	if(ax>ay){
		int cum=ax>>1;
		while(xc!=x){
			xc+=sx;
			cum+=ay;
			if(cum>=ax){
				cum-=ax;
				yc+=sy;
			};
			if(CheckBar(xc,yc,3,3))return false;			
		};
	}else{
		int cum=ay>>1;
		while(yc!=y){
			yc+=sy;
			cum+=ax;
			if(cum>=ay){
				cum-=ay;
				xc+=sx;
			};
			if(CheckBar(xc,yc,3,3))return false;
		};
	};
	return true;
};
void GetForceActOnBar(int x,int y,int L,int& fx,int& fy,int Media){
	MotionField* MF=MFIELDS+Media;
	fx=0;
	fy=0;
	int cx=(x-(L<<3))>>4;
	int cy=(y-(L<<3))>>4;
	for(int dx=0;dx<L;dx++){
		for(int dy=0;dy<L;dy++){
			if(MF->CheckPt(cx+dx,cy+dy)){
				int DX=dx+dx-L+1;
				int DY=dy+dy-L+1;
				//int DX=x-((cx+dx)<<4)-8;
				//int DY=y-((cy+dy)<<4)-8;
				int d=Norma(DX,DY);
				if(d<L){
					if(d<1)d=1;
					fx-=DX*256/d/d;
					fy-=DY*256/d/d;
				};
			};
		};
	};
	int N=Norma(fx,fy);
	if(N>512){
		fx=(fx<<9)/N;
		fy=(fy<<9)/N;
	};
};
void xLine(int x,int y,int x1,int y1,byte c);
void CBar(int x,int y,int Lx,int Ly,byte c);
void DrawForceOnUnit(OneObject* OB){
	return;
	if(LMode)return;
	int dx=mapx<<5;
	int dy=mapy<<4;
	if(OB){
		int LX=OB->Lx;
		int fx,fy;
		XorBlockComplexUnit(OB);
		GetForceActOnBar(OB->RealX>>4,OB->RealY>>4,LX,fx,fy,OB->Media);
		XorBlockComplexUnit(OB);
		int X0=(OB->RealX>>4)-dx;
		int Y0=(OB->RealY>>5)-dy-OB->RZ;
		xLine(X0,Y0,X0+fx,Y0+fy,255);
		CBar(X0-1,Y0-1,3,3,255);
		Xbar((int(OB->x)<<4)-dx,(int(OB->y)<<3)-dy,LX<<4,LX<<3,clrRed);
	};
};
void BXorBar(int x,int y,int Lx);
void XorBlockComplexUnit(OneComplexObject* COB){
	int L=COB->Lx;
	int L3=L<<7;
	int N=COB->NQuants;
	for(int i=0;i<N;i++){
		int x=(int(COB->MQ[i]->xc)-L3)>>8;
		int y=(int(COB->MQ[i]->yc)-L3)>>8;
		BXorBar(x,y,L);
	};
};
void XorBlockComplexUnit(OneObject* OB){
	word CID=OB->CObjIndex;
	if(CID!=0xFFFF){
		OneComplexObject* COB=CGroup[CID];
		if(COB){
			XorBlockComplexUnit(COB);
		};
	};
};
void MakeSeveralStepsBackLink(OneObject* OBJ){
	if(!OBJ->LocalOrder->info.MoveToXY.PrevDist--){
		OBJ->BackMotion=0;
		OBJ->DeleteLastOrder();
		return;
	};
	OBJ->BackMotion=1;
	OneComplexObject* COB=CGroup[OBJ->CObjIndex];
	int x,y;
	COB->GetTalePoint(x,y);
	byte DIR=OBJ->LocalOrder->info.MoveToXY.Times;
	OBJ->DestX=(x<<4)+(int(TCos[DIR])<<2);
	OBJ->DestY=(y<<4)+(int(TSin[DIR])<<2);
};
void MakeSeveralStepsBack(OneObject* OB,char Dir,int N){
	if(rando()<16384)Dir+=128;
	if(OB->CObjIndex==0xFFFF)return;
	if(OB->LocalOrder&&OB->LocalOrder->DoLink==&MakeSeveralStepsBackLink)return;
	Order1* OR1=OB->CreateOrder(1);
	if(OR1){
		OR1->info.MoveToXY.PrevDist=N;
		OR1->info.MoveToXY.Times=Dir;
		OR1->DoLink=&MakeSeveralStepsBackLink;
	};
};
bool CheckSqIntersection(int x0,int y0,int L0,int x1,int y1,int L1){
	int d=x0+x0+L0-x1-x1-L1;
	if(abs(d)>=L0+L1)return false;
	d=y0+y0+L0-y1-y1-L1;
	if(abs(d)>=L0+L1)return false;
	return true;
};
bool CheckCObjIntersection(OneComplexObject* COB,int x,int y,int L){
	int Ln=COB->Lx;
	int L3n=Ln<<7;
	for(int i=0;i<COB->NQuants;i++){
		int xn=(int(COB->MQ[i]->xc)-L3n)>>8;
		int yn=(int(COB->MQ[i]->yc)-L3n)>>8;
		if(CheckSqIntersection(xn,yn,Ln,x,y,L))return true;
	};
	return false;
};
OneObject* GetLockOwner(word MyID,int x,int y,int L){
	int cx=(x>>3)+1;
	int cy=(y>>3)+1;
	for(int r=0;r<3;r++){
		char* dx=Rarr[r].xi;
		char* dy=Rarr[r].yi;
		int N=Rarr[r].N;
		for(int i=0;i<N;i++){
			int xx=cx+dx[i];
			int yy=cy+dy[i];
			if(xx>=0&&xx<VAL_MAXCX&&yy>=0&&yy<VAL_MAXCX){
				int cell=xx+(yy<<VAL_SHFCX);
				int NMon=MCount[cell];
				if(NMon){
					int ofs1=cell<<SHFCELL;
					word MID;
					for(int i=0;i<NMon;i++){
						MID=GetNMSL(ofs1+i);
						if(MID!=0xFFFF&&MID!=MyID){
							OneObject* OB=Group[MID];
							if(OB&&OB->Index!=MyID&&OB->CObjIndex!=0xFFFF&&!OB->Sdoxlo){
								OneComplexObject* COB=CGroup[OB->CObjIndex];
								if(CheckCObjIntersection(COB,x,y,L))return OB;
							};
						};
					};
				};
			};
		};
	};
	return NULL;
};
bool CheckStopRuleWhenLocked(OneObject* OB){
	OneObject* OPP=GetLockOwner(OB->Index,OB->x,OB->y,OB->Lx);
	if(OPP&&OPP->DestX>=0){
		int MyDx=OB->DestX-OB->RealX;
		int MyDy=OB->DestY-OB->RealY;
		int OpDx=OPP->DestX-OPP->RealX;
		int OpDy=OPP->DestY-OPP->RealY;
		return (OpDx*MyDy-OpDy*MyDx)>0;
	};
	return false;
};
void MoveAllFoolsAwayFromTheWay(OneObject* OB);
int PreCheckMotionRules(OneObject* OB){
	if(OB->CObjIndex==0xFFFF)return false;

	MoveAllFoolsAwayFromTheWay(OB);

	OneComplexObject* MCOB=CGroup[OB->CObjIndex];
	int XP=MCOB->MQ[0]->xc;
	int YP=MCOB->MQ[0]->yc;
	byte FI=MCOB->MQ[0]->Fi;
	XP+=int(TCos[FI])*12;
	YP+=int(TSin[FI])*12;
	int Ln=MCOB->Lx;
	int L3n=Ln<<7;
	int xn=(XP-L3n)>>8;
	int yn=(YP-L3n)>>8;
	OneObject* OPP=GetLockOwner(OB->Index,xn,yn,OB->Lx);
	if(OPP&&OPP->DestX>=0){
		int MyDx=OB->DestX-OB->RealX;
		int MyDy=OB->DestY-OB->RealY;
		int OpDx=OPP->DestX-OPP->RealX;
		int OpDy=OPP->DestY-OPP->RealY;
		int NM=Norma(MyDx,MyDy);
		int NO=Norma(OpDx,OpDy);
		MyDx=(MyDx<<8)/NM;
		MyDy=(MyDy<<8)/NM;
		OpDx=(OpDx<<8)/NO;
		OpDy=(OpDy<<8)/NO;
		int SP=(MyDx*OpDx+MyDy*OpDy)>>8;
		if(SP<-120)return 2;
		int VP=(OpDx*MyDy-OpDy*MyDx)>>8;
		return VP>0&&abs(SP)<128?1:0;
	};
	
	return false;
};

typedef bool EnumUnitsCallback(OneObject* OB,int x,int y,int r,DWORD param);

int EnumAllUnitsInCircle(int x,int y,int R,EnumUnitsCallback* fn,DWORD param){
	int cx=(x>>7)+1;
	int cy=(y>>7)+1;
	int D=(R>>7)+2;
	int N=0;
	for(int r=0;r<D;r++){
		char* dx=Rarr[r].xi;
		char* dy=Rarr[r].yi;
		int N=Rarr[r].N;
		for(int i=0;i<N;i++){
			int xx=cx+dx[i];
			int yy=cy+dy[i];
			if(xx>=0&&xx<VAL_MAXCX&&yy>=0&&yy<VAL_MAXCX){
				int cell=xx+(yy<<VAL_SHFCX);
				int NMon=MCount[cell];
				if(NMon){
					int ofs1=cell<<SHFCELL;
					word MID;
					for(int i=0;i<NMon;i++){
						MID=GetNMSL(ofs1+i);
						if(MID!=0xFFFF){
							OneObject* OB=Group[MID];
							if(OB&&(!OB->Sdoxlo)&&Norma((OB->RealX>>4)-x,(OB->RealY>>4)-y)<R){
								if(fn(OB,x,y,R,param))N++;
							};
						};
					};
				};
			};
		};
	};
	return N;
};
void NewMonsterSendToLink(OneObject* OB);
bool MoveFools(OneObject* OB,int x,int y,int R,DWORD param){
	if(OB->CObjIndex!=0xFFFF)return 0;
	if(OB->LocalOrder&&OB->LocalOrder->DoLink==&NewMonsterSendToLink&&OB->PrioryLevel==2)return 0;
	byte dir=byte(param);
	int dx=TCos[dir];
	int dy=TSin[dir];
	int DX=(OB->RealX>>4)-x;
	int DY=(OB->RealY>>4)-y;
	if(DX*dy-DY*dx<0){
		dir+=64;
	}else{
		dir-=64;
	};
	OB->NewMonsterSendTo(OB->RealX+(TCos[dir]<<2),OB->RealY+(TSin[dir]<<2),128+2,1);
	return 1;
};
void MoveAllFoolsAwayFromTheWay(OneObject* OB){
	if(OB->DestX>=0){
		if(OB->CObjIndex==0xFFFF)return;
		OneComplexObject* MCOB=CGroup[OB->CObjIndex];
		int XP=MCOB->MQ[0]->xc;
		int YP=MCOB->MQ[0]->yc;
		byte FI=MCOB->MQ[0]->Fi;
		XP+=int(TCos[FI])*2;
		YP+=int(TSin[FI])*2;
		EnumAllUnitsInCircle(XP>>4,YP>>4,110,&MoveFools,FI);
		XP+=int(TCos[FI])*8;
		YP+=int(TSin[FI])*8;
		EnumAllUnitsInCircle(XP>>4,YP>>4,80,&MoveFools,FI);
	};
};
//void AttackPointByComplexObject(OneObject* OB,int x,int y,
#endif //COSSACKS2