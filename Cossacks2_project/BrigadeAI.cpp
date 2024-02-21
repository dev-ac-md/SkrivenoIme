// BrigadeAI.cpp: implementation of the BrigadeAI class.
//
//////////////////////////////////////////////////////////////////////
#include "ddini.h"
#include "ResFile.h"
#include "FastDraw.h"
#include "mgraph.h"
#include "mouse.h"
#include "menu.h"
#include "MapDiscr.h"
#include "multipl.h"
#include "fog.h"
#include "walls.h"
#include "Nature.h"
#include "Nucl.h"
#include <assert.h>
#include "Megapolis.h"
#include "MapSprites.h"
#include "NewAI.h"
#include "TopoGraf.h"
#include "Path.h"
#include "NewMon.h"
#include <math.h>
#include "StrategyResearch.h"
#include "Towers.h"
#include "Safety.h"
#include "ActiveScenary.h"
#include "Sort.h"
#include "ConstStr.h"
#include "EinfoClass.h"
#include <crtdbg.h>
#include <math.h>
#include "BrigadeAI.h"
#include "..\IntExplorer\DString.h"

#ifdef COSSACKS2

extern int tmtmt;
extern City CITY[8];
extern int MaxMorale_LostCriticalPercent;
extern int Morale_KillDec;
extern int Morale_DecWhen30Lost;
extern int Morale_DamageDec1;
extern int Morale_FormationShield15;
extern int Morale_FormationShield196;
extern RLCFont YellowFont;
extern int GetHeight(int x, int y);
extern void xLine(int x,int y,int x1,int y1,byte c);
DLLEXPORT int GetTopZone(int x,int y);
DLLEXPORT bool GetTopZoneCoor(int Zone,int* x,int* y);
DLLEXPORT int GetNextZone(int Start,int Fin);
extern void ComThrowGrenade(byte NI,int ID);

//extern OrderDescription ElementaryOrders[128];

//====================================================================//
Atom::Atom()
{
	BSelf=NULL;
	CheckTimeInterval=3;
	Force=100;
	Type=0;
	CheckTime=-1;
}
Atom::Atom(Brigade* Br)
{
	BSelf=Br;
	CheckTimeInterval=3;
	Force=100;
	Type=1;
	CheckTime=-1;
	UpdateParams();
	IsStand=true;
}
Atom::~Atom()
{}
void Atom::UpdateParams()
{
	if(CheckTime!=tmtmt&&tmtmt>(CheckTime+CheckTimeInterval))
	{
		if(BSelf&&BSelf->WarType!=0)
		{
			BSelf->GetCenter(&x,&y,&Dir);
			UnitsAmo=0;
			FireReady=0;
			FireTimeDelayLeft=0;
			int maxdelay=0;
			Tired=0;
			IsStand=true;
			for(int i=3;i<BSelf->NMemb;i++)
			{
				if(BSelf->Memb[i]!=0xFFFF)
				{
					OneObject* OB=Group[BSelf->Memb[i]];
					if(OB)
					{
						UnitsAmo++;
						if(OB->delay==0)
							FireReady++;
						else
						{
							FireTimeDelayLeft+=OB->delay;
							maxdelay+=OB->MaxDelay;
						}
						Tired+=OB->GetTired;
						if(OB->DestX!=-1)
							IsStand=false;
					}
				}
			}
			if(FireTimeDelayLeft)
				FireTimeDelayLeft=(FireTimeDelayLeft*100)/maxdelay;
			
			int com=0;
			if(BSelf->Memb[0]!=0xFFFF)
			{
				OneObject* OB=Group[BSelf->Memb[0]];
				if(OB){Tired+=OB->GetTired;com++;}
			}
			if(BSelf->Memb[1]!=0xFFFF)
			{
				OneObject* OB=Group[BSelf->Memb[1]];
				if(OB){Tired+=OB->GetTired;com++;}
			}
			if(BSelf->Memb[2]!=0xFFFF)
			{
				OneObject* OB=Group[BSelf->Memb[2]];
				if(OB){Tired+=OB->GetTired;com++;}
			}
			if(UnitsAmo)
				Tired=Tired/(UnitsAmo+com);
			else
				Type=0;
			Force=UnitsAmo;
		}
		else
			Type=0;
		CheckTime=tmtmt;
	}
}
//====================================================================//
/*
		
		1-Reserv
		2-MoveBack
		3-Stand
		4-Front
		5-March
		6-NearAim
		7-MoveFront
		8-BattleFormation
		9-Rest
		10-Move
		11-NearAim
		12-LineFormation
		13-SquareFormation
		14-KareFormation
		*/
bool AutoForm::NonStateArr[32][32] = 
{
					   //0 1 2 3 4 5 6 7 8 9 10111213141516
/*0-Attack*/			{0,1,1,1,0,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
/*1-Reserv*/			{0,0,0,1,1,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
/*2-MoveBack*/			{1,0,0,1,1,1,0,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
/*3-Stand*/				{1,0,1,0,0,1,0,1,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
/*4-Front*/				{0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
/*5-March*/				{1,1,1,1,1,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
/*6-NearAim*/			{0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
/*7-MoveFront*/			{0,0,0,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
/*8-BattleFormation*/	{0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
/*9-Rest*/				{1,0,1,0,0,0,0,1,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
/*10-Move*/				{1,0,1,1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
/*11-ReadyToFire*/		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
/*12-LineFormation*/	{0,0,0,0,0,1,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
/*13-SquareFormation*/	{0,0,0,0,0,0,0,0,0,0,0,0,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
/*14-KareFormation*/	{0,0,0,0,0,1,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
/*15-EnableFire*/		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
/*16-DisableFire*/		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
};
AutoForm::AutoForm()
{
	BSelf=NULL;
}
AutoForm::AutoForm(Brigade* Bn)
{
	RegimentIndex=-1;
	NextProcesedTime=0;
	NextTimeProcessRules=0;
	Force=70;

	//Atom::Atom(Bn);
	BSelf=Bn;
	UpdateParams();

	AimX=-1;
	AimY=-1;
	
	if(Bn->WarType)
		FormationTypeIndex=ElementaryOrders[Bn->WarType].GroupID;
	
	for(int i=0;i<16;i++)
	{
		EnmAng[i].SumDang=0;
		EnmAng[i].Dang=0;
		EnmAng[i].Dir=i<<4;
		EnmAng[i].SectorsAmo=0;
		
		FriendAng[i].SumDang=0;
		FriendAng[i].Dang=0;
		FriendAng[i].Dir=i<<4;
		FriendAng[i].SectorsAmo=0;
	}
	SetState(16,false);DisableFire();
	DistToMainTarget=-1;
	SetNoSearchVictim(true);
	Movement=false;
	mtXr=-1;
	mtYr=-1;
	mtXl=-1;
	mtYl=-1;
}
AutoForm::~AutoForm()
{
}
void AutoForm::MoveTo(int xx, int yy)
{
	DisableFire();
	byte dd=(byte)(GetDir(xx-x,yy-y));
	MoveTo(xx,yy,dd);
	//BSelf->HumanLocalSendTo(xx<<4,yy<<4,dd,128,0);
}
void AutoForm::MoveTo(int x, int y, byte dir)
{
	DisableFire();
	Movement=true;
	mtX=x;
	mtY=y;
	bool cor=CorrectDestPoint(mtX,mtY,dir);
	for(int i=0;i<10&&!cor;i++)
		cor=CorrectDestPoint(mtX,mtY,dir);
	//BSelf->HumanLocalSendTo(x<<4,y<<4,dir,128,0);
	byte dd=(dir+8)>>4;dd=dd<<4;
	BSelf->HumanLocalSendTo(mtX<<4,mtY<<4,dd,128+16,0);

	mtXr=mtX-((TSin[dd]*GetSizeRight())>>8);
	mtYr=mtY+((TCos[dd]*GetSizeRight())>>8);

	mtXl=mtX+((TSin[dd]*GetSizeLeft())>>8);
	mtYl=mtY-((TCos[dd]*GetSizeLeft())>>8);
}
void AutoForm::SimpleMoveTo(int xx, int yy)
{
	DisableFire();
	byte dd=(byte)(GetDir(xx-x,yy-y));
	SimpleMoveTo(xx,yy,dd);
	//BSelf->HumanLocalSendTo(xx<<4,yy<<4,dd,128,0);
}
void AutoForm::SimpleMoveTo(int x, int y, byte dir)
{
	DisableFire();
	Movement=true;
	mtX=x;
	mtY=y;
	
	//BSelf->HumanLocalSendTo(x<<4,y<<4,dir,128,0);
	byte dd=(dir+8)>>4;dd=dd<<4;
	mtDir=dd;
	BSelf->HumanLocalSendTo(mtX<<4,mtY<<4,dd,128,0);

	mtXr=mtX-((TSin[dd]*GetSizeRight())>>8);
	mtYr=mtY+((TCos[dd]*GetSizeRight())>>8);

	mtXl=mtX+((TSin[dd]*GetSizeLeft())>>8);
	mtYl=mtY-((TCos[dd]*GetSizeLeft())>>8);
}
void AutoForm::Process()
{
	if(tmtmt>NextProcesedTime)
	{
		if(IsStand)
			Movement=false;
		TimeInterval=7;
		//if(GetState(0))TimeInterval=1;
		Atom* tm=MainTarget;
		MainTarget=GetNearestAtom();
		if(MainTarget!=tm&&MainTarget)
		{
			//DisableFire();
			TurnToMT();
		}
		int a2=GetMainTargetAngl();
		SetState(1,true);//Reserv;
		if(a2!=-1)
		{
			byte dr=a2<<4;
			if((FriendAng[dr].Dang==0)||((FriendAng[dr].SumDang>0)&&(DistToMT()<FriendAng[dr].Sectors[0].dest)))
			{
				SetState(4,true);//Front
			}
		}
		ProcessRules();
		int ACT=GetMaxActions();
		ExecuteActions(ACT);
		
		if(IsStand&&ACT==-1&&GetTired()>90)
		{
			//if((BSelf->Direction>(Dir+17)||BSelf->Direction<(Dir-17)))
			//	MoveTo(x-10,y-10,BSelf->Direction);
			
			bool mm=false;
			for(byte i=192;i!=80&&!mm;i=i+16)
			{
				byte in=i>>4;
				if(FriendAng[in].SumDang)
				{
					AutoForm* A=FriendAng[in].Sectors[0].AF;
					if(A&&A->IsStand&&FriendAng[in].Sectors[0].dest<300)
						mm=true;
				}
			}
			if(mm)
				StepBack();
		}
		//if(!IsStand&&ACT==-1&&Movement)
		//	MoveTo(mtX, mtY); 
		NextProcesedTime=tmtmt+TimeInterval;
	}
}
void AutoForm::StepForward()
{
	//UpdateParams();
	int S=500;
	int dx=(TCos[BSelf->Direction]*S)>>8;
	int dy=(TSin[BSelf->Direction]*S)>>8;
	MoveTo(x+dx,y+dy,BSelf->Direction);
	//BSelf->HumanLocalSendTo((x+dx)<<4,(y+dy)<<4,BSelf->Direction,128,0);
}
void AutoForm::StepBack()
{
	//UpdateParams();
	int S=400;
	byte dr=BSelf->Direction+128;
	int dx=(TCos[dr]*S)>>8;
	int dy=(TSin[dr]*S)>>8;
	MoveTo(x+dx,y+dy,BSelf->Direction);
	//BSelf->HumanLocalSendTo((x+dx)<<4,(y+dy)<<4,BSelf->Direction,128,0);
}
void AutoForm::MoveBack()
{
	//int a=GetMaxDangDirectionMinus();
	int a=GetMainTargetAngl();
	if(a>-1)
	{
		//byte ang=(byte) (a<<4);
		byte ang=a+BSelf->Direction+128;
		int S=400;
		int dx=(TCos[ang]*S)>>8;
		int dy=(TSin[ang]*S)>>8;
		ang+=128;
		MoveTo(x+dx,y+dy,ang);
	}
}
void AutoForm::TurnTo(byte dir)
{
	//BSelf->Direction=dir;
	//DisableFire();
	//Self->HumanLocalSendTo(x<<4,y<<4,dir,128,0);
	MoveTo(x,y,dir);
}
void AutoForm::TurnLeft()
{
	UpdateParams();
	BSelf->Direction+=-16;
	BSelf->HumanLocalSendTo(x<<4,y<<4,BSelf->Direction,128+1,0);
}
void AutoForm::TurnRight()
{
	UpdateParams();
	BSelf->Direction+=16;
	BSelf->HumanLocalSendTo(x<<4,y<<4,BSelf->Direction,128+1,0);
}
void AutoForm::SetNoSearchVictim(bool vle)
{
	if(BSelf->WarType)
	{
		for(int i=0;i<BSelf->NMemb;i++)
		{
			word ID=BSelf->Memb[i];
			if(ID!=0xFFFF)
			{
				OneObject* OB=Group[ID];
				if(OB&&OB->Serial==BSelf->MembSN[i])
				{
					OB->NoSearchVictim=vle;
				}
			}
		}
	}
}
void AutoForm::AttackBrigade(Brigade *Br)
{
	if(Br&&Br->WarType&&BSelf->WarType)
	{
		//EnableFire();
		for(int i=3;i<BSelf->NMemb;i++)
		{
			if(BSelf->Memb[i]!=0xFFFF)
			{
				int rnu=-1;
				int dist=999999;
				OneObject* OBs=Group[BSelf->Memb[i]];
				//OBs->newMons->
				if(Br->NMemb&&OBs&&OBs->delay==0&&!OBs->Attack)
				{
					for(int j=0;j<10;j++)
					{
						int in=-1;
						bool flag=true;	
						for(int iter=0;flag;iter++)
						{	
							in=rando()%Br->NMemb;
							if(Br->Memb[in]!=0xFFFF)
							{
								//if(Group[Br->Memb[in]]->Life>life[in])
									flag=false;
								//if(iter>10)
								//	flag=false;
							}
						}
						OneObject* OBg=Group[Br->Memb[in]];
						if(OBg)
						{
							int dd=Norma((OBs->RealX>>4)-(OBg->RealX>>4),(OBs->RealY>>4)-(OBg->RealY>>4));
							if(dd<dist)
							{
								dist=dd;
								rnu=in;
							}
						}
					}
					if(rnu>-1)
					{
						OBs->NoSearchVictim=true;
						OBs->RifleAttack=true;
						OBs->AttackObj(Br->Memb[rnu],128+16,0,2);
						OBs->RifleAttack=false;
						//OBs->StandGround=false;
					}
					//life[rnu]+=GetDamage(BSelf->Memb[i],Br->Memb[rnu],1);
				}
			}
		}
	}
}
void AutoForm::SetFireState(Brigade* Bn, bool State)
{
	if(Bn->WarType)
	{
		for(int i=0;i<Bn->NMemb;i++)
		{
			word ID=Bn->Memb[i];
			if(ID!=0xFFFF)
			{
				OneObject* OB=Group[ID];
				if(OB&&OB->Serial==Bn->MembSN[i])
				{
					OB->RifleAttack=State;
				}
			}
		}
	}
}
void AutoForm::GetCoordToMTWay(int &xt, int &yt, AutoForm* FR)
{
	if(MainTarget&&DistToMainTarget!=-1)
	{
		byte Dr = GetDir(x-MainTarget->x,y-MainTarget->y);
		int Step=400;
		int MTdst=DistToMT();
		int ndst=DistToMainTarget;
		if(DistToMainTarget>MTdst&&(DistToMainTarget-MTdst)>Step)
			ndst=MTdst-Step;
		int xx=MainTarget->x+((ndst*TCos[Dr])>>8);
		int yy=MainTarget->y+((ndst*TSin[Dr])>>8);
		/*
		if(FR)
		{
			int Shift=Norma(FR->x-xx,FR->y-yy);
			int li=GetSizeLeft()+FR->GetSizeLeft();
			if(Shift<li)
			{
				byte D= GetDir(xx-FR->x,yy-FR->y);
				xx=FR->x+((li*TCos[D])>>8);
				yy=FR->y+((li*TSin[D])>>8);
			}
		}
		*/
		xt=xx;
		yt=yy;
	}
}
AutoForm* AutoForm::FindNearestFriendInMTDirection()
{
	AutoForm* rez=NULL;
	if(MainTarget)
	{
		byte ang = (byte) GetMainTargetAngl();
		byte From = ang - 32;
		byte To = ang + 48;
		int dst=9999999;
		for(byte i=From;i!=To;i=i+16)
		{
			byte ii=i<<4;
			if(ii<16&&FriendAng[ii].SumDang)
				if(FriendAng[ii].Sectors[0].dest<dst&&(dst+300)<DistToMT())
				{
					dst=FriendAng[ii].Sectors[0].dest;
					rez=FriendAng[ii].Sectors[0].AF;
				}
		}
	}
	return rez;
}
bool AutoForm::CorrectDestPoint(int xx,int yy, byte dirr)
{
	
	bool rez=true;
	mtX=xx;
	mtY=yy;
	mtDir=dirr;
	for(int an=0;an<16;an++)
		for(int i=0;i<FriendAng[an].SectorsAmo;i++)
		{
			AutoForm* FR=FriendAng[an].Sectors[i].AF;
			if(FR&&FR->IsStand)
			{
				int dist=Norma(FR->mtX-mtX,FR->mtY-mtY);

				byte drr=GetDir(mtX-FR->mtX,mtX-FR->mtX);
				char drrc=(char)drr;
				char adr=(char)FR->BSelf->Direction;
				byte dim=abs(drrc-adr);
				if(dim>64)dim=dim-64;
				int fdist=(int)sqrt(((FR->GetSizeLeft()*TSin[dim])>>8)*((FR->GetSizeLeft()*TSin[dim])>>8)+((FR->GetSizeFront()*TCos[dim])>>8)*((FR->GetSizeFront()*TCos[dim])>>8));

				byte drr2=drr+128;
				char drrc2=(char)drr2;
				char adr2=(char)dirr;
				byte dim2=abs(drrc2-adr2);
				if(dim2>64)dim2=dim2-64;
				int fdist2=(int)sqrt(((GetSizeLeft()*TSin[dim2])>>8)*((GetSizeLeft()*TSin[dim2])>>8)+((GetSizeFront()*TCos[dim2])>>8)*((GetSizeFront()*TCos[dim2])>>8));

				int ndist=fdist2+fdist;
				if(ndist>dist)
				{
					mtX=FR->mtX+((ndist*TSin[drr])>>8);
					mtY=FR->mtY+((ndist*TCos[drr])>>8);
					rez=false;
				}
			}
		}
	return rez;
}
void AutoForm::ShowInfo()
{
	char st[128];
	if(GetState(1))
		sprintf(st,"F:%d Reserv,TI:%d",BSelf->ID,TimeInterval);
	else
	if(GetState(4))
		sprintf(st,"F:%d Front,TI:%d",BSelf->ID,TimeInterval);
	ShowString(x-(mapx<<5),y/2-GetHeight(x,y)-(mapy<<4),st,&YellowFont);
	if(MainTarget)
		xLine(x-(mapx<<5),y/2-GetHeight(x,y)-(mapy<<4),MainTarget->x-(mapx<<5),MainTarget->y/2-GetHeight(MainTarget->x,MainTarget->y)-(mapy<<4),0xFF);

	ShowString(mtXr-(mapx<<5),mtYr/2-GetHeight(mtXr,mtYr)-(mapy<<4),"R",&YellowFont);
	ShowString(mtXl-(mapx<<5),mtYl/2-GetHeight(mtXl,mtYl)-(mapy<<4),"L",&YellowFont);
	xLine(mtXr-(mapx<<5),mtYr/2-GetHeight(mtXr,mtYr)-(mapy<<4),mtXl-(mapx<<5),mtYl/2-GetHeight(mtXl,mtYl)-(mapy<<4),0xF0);

}
void AutoForm::EnableFire()
{
	//if(!IsStand)
	//	FormateToLine();
	if(!GetState(15))//EnableFire
	{
		SetNoSearchVictim(false);
		//BSelf->ClearBOrders();
		SetFireState(BSelf,true);
		//SetNoSearchVictim(true);
		SetState(15,true);
	}
	//AttackBrigade(MainTarget->BSelf);
}
void AutoForm::FireNow()
{
	if(!GetState(15))//EnableFire
	{
		//BSelf->ClearBOrders();
		SetNoSearchVictim(false);
		SetFireState(BSelf,true);
		SetState(15,true);
	}
	//AttackBrigade(MainTarget->BSelf);
}
void AutoForm::DisableFire()
{
	if(!GetState(16))
	{
		SetFireState(BSelf,false);
		SetNoSearchVictim(true);
		SetState(16,true);
	}
}
int AutoForm::GetMoral(Brigade* Br)
{
	return Br->Morale/10000;
}
int AutoForm::GetDistTo(Brigade* Br)
{
	int xx, yy;
	byte dd;
	Br->GetCenter(&xx,&yy,&dd);
	return Norma(x-xx,y-yy);
}
int AutoForm::GetDieAmount(Brigade* Br,int* ReadyShuters)
{
	int rez=0;
	word* life = (word*)malloc(Br->NMemb<<1);
	memset(life,0,Br->NMemb<<1);
	*ReadyShuters=0;
	if(Br&&Br->WarType&&BSelf->WarType)
	for(int i=3;i<BSelf->NMemb;i++)
	{
		if(BSelf->Memb[i]!=0xFFFF)
		{
			int rnu=-1;
			int dist=999999;
			OneObject* OBs=Group[BSelf->Memb[i]];
			//OBs->newMons->
			if(Br->NMemb&&OBs->delay==0)
			{
				for(int j=0;j<5;j++)
				{
					int in=-1;
					bool flag=true;	
					for(int iter=0;flag;iter++)
					{	
						in=rando()%Br->NMemb;
						if(Br->Memb[in]!=0xFFFF)
						{
							if(Group[Br->Memb[in]]->Life>life[in])
								flag=false;
							if(iter>10)
								flag=false;
						}
					}
					OneObject* OBg=Group[Br->Memb[in]];
					int dd=Norma((OBs->RealX>>4)-(OBg->RealX>>4),(OBs->RealY>>4)-(OBg->RealY>>4));
					if(dd<dist)
					{
						dist=dd;
						rnu=in;
					}
				}
				life[rnu]+=GetDamage(BSelf->Memb[i],Br->Memb[rnu],1);
				(*ReadyShuters)++;
			}
		}
	}
	for(int i=0;i<Br->NMemb;i++)
	{
		if(Br->Memb[i]!=0xFFFF)
			if(Group[Br->Memb[i]]->Life<=life[i])
				rez++;
	}
	return rez;
}
int AutoForm::CanIKill(Brigade *Br)
{
	if(Br)
	{
		int av=0;
		int mmor=0;
		int ReadyShuters;
		for(int i=0;i<10;i++)
			av+=GetDieAmount(Br,&ReadyShuters);
		av=(int)((av*1.1)/10);if(av%5)av++;
		if(av)
		{
			//SetReadyToFire();
			int mor=Br->Morale;
			mmor=av*Morale_KillDec;
			mmor+=ReadyShuters*Morale_DamageDec1;
			int VictimAmo;
			int lim=GetNTo30Limit(Br,&VictimAmo);
			if(lim>0&&lim<=av)
				mmor+=Morale_DecWhen30Lost;
			
			int MoraleShild=Morale_FormationShield15+(VictimAmo-15)*(Morale_FormationShield196-Morale_FormationShield15)/(196-15);	
			mmor=(mmor*MoraleShild)/100;
			char c[32];
			sprintf(c,"MoraleDownTo - %d(%d)",(mor-mmor)/10000,av);
			AssignHint1(c,30);
			//if((mor-mmor)/10000<46)
			return (mor-mmor)/10000;
		}
	}
	return -1;
}
int AutoForm::GetNTo30Limit(Brigade *Br,int* VictimAmo)
{
	int amo=0;
	for(int i=3;i<Br->NMemb;i++)
		if(Br->Memb[i]!=0xFFFF)
			amo++;
	int NU=ElementaryOrders[Br->WarType-1].NUnits;
	*VictimAmo=amo;
	return amo-(NU*(100-MaxMorale_LostCriticalPercent))/100;
}
void AutoForm::SetReadyToFire()
{
	if(!GetState(11))
	{
		BSelf->AttEnm=true;
		for(int i=3;i<BSelf->NMemb;i++)
		{
			int index=BSelf->Memb[i];
			if(index!=0xFFFF)
			{
				OneObject* OB=Group[index];
				if(OB)
				{
					OB->NewState=1;
					//OB->GroundState=1;
				}
			}
		}
		SetState(11,true);
	}
}
void AutoForm::SetNotReadyToFire()
{
	if(GetState(11))
	{
		BSelf->AttEnm=false;
		for(int i=3;i<BSelf->NMemb;i++)
		{
			int index=BSelf->Memb[i];
			if(index!=0xFFFF)
			{
				OneObject* OB=Group[index];
				if(OB)
				{
					OB->NewState=0;
					//OB->GroundState=1;
				}
			}
		}
		SetState(11,false);
	}
}
void AutoForm::FormateToLine()
{
	FormateTo012(0);
}
void AutoForm::FormateToSquare()
{
	FormateTo012(1);
}
void AutoForm::FormateToKare()
{
	FormateTo012(2);
}
void AutoForm::Stop()
{
	BSelf->ClearBOrders();
	SimpleMoveTo(x,y,Dir);
	//BSelf->HumanLocalSendTo(x<<4,y<<4,Dir,128,0);
}
void AutoForm::Castle()
{
	AutoForm* R=GetFriendInBack();
	if(R&&R->GetState(1))
	{
		R->SimpleMoveTo(x,y,BSelf->Direction);
		R->TimeInterval=100;
		R->DisableFire();
		//R->SetState(4,true);//front
		DisableFire();
		StepBack();
		SetState(1,true);//reserv
	}
}
void AutoForm::FormateTo012(int Type)
{
	if(BSelf->WarType!=0&&FormationTypeIndex!=Type&&Type>-1&&Type<3)
	{
		OrderDescription* ODE=ElementaryOrders+BSelf->WarType-1;
		SingleGroup* FGD=FormGrp.Grp+ODE->GroupID;
		MakeReformation(BSelf->CT->NI,BSelf->ID,FGD->IDX[Type]);
		FormationTypeIndex=Type;
		
		mtXr=x-((TSin[BSelf->Direction]*GetSizeRight())>>8);
		mtYr=y+((TCos[BSelf->Direction]*GetSizeRight())>>8);

		mtXl=x+((TSin[BSelf->Direction]*GetSizeLeft())>>8);
		mtYl=y-((TCos[BSelf->Direction]*GetSizeLeft())>>8);
	}
}
void AutoForm::ClearEnmAng()
{
	for(int i=0;i<16;i++)
	{	
		EnmAng[i].SectorsAmo=0;
		EnmAng[i].SumDang=0;
		EnmAng[i].Dang=0;
		EnmAng[i].FireDang=0;
		EnmAng[i].SumFireDang=0;
	}
}
void AutoForm::ClearFriendAng()
{
	for(int i=0;i<16;i++)
	{
		FriendAng[i].SectorsAmo=0;
		FriendAng[i].SumDang=0;
		FriendAng[i].Dang=0;
		FriendAng[i].FireDang=0;
		FriendAng[i].SumFireDang=0;
	}
}
void AutoForm::AddToEnmAng(Atom *A)
{
	int dist=Norma(A->x-x,A->y-y);
	if(dist<5001)
	{
		byte v2=(byte)(GetDir(A->x-x,A->y-y));
		byte alfa=v2-BSelf->Direction+8;
		byte ani=alfa>>4;
		Direction* D=&(EnmAng[ani]);

		if(ani&&GetState(12))//Line
		{
			dist-=abs(TSin[alfa]*(GetSizeLeft()-200))>>8;
			if(dist<1)dist=1;
		}
		
		D->Sectors[D->SectorsAmo].A=A;
		D->Sectors[D->SectorsAmo].dest=dist;
		D->SectorsAmo++;
		int dd=(A->Force*10000)/dist;
		if(dist<600)
		{
			byte lef=(alfa-16);
			byte rat=(alfa+16);
			EnmAng[lef>>4].Dang+=dd/3;
			EnmAng[rat>>4].Dang+=dd/3;
		}
		D->Dang+=dd;
		D->SumDang+=A->Force;
		D->FireDang+=(A->FireReady*900)/dist;
		D->SumFireDang+=A->FireReady;

		for(int i=(D->SectorsAmo-1);i>0;i--)
		{
			if(D->Sectors[i].dest<D->Sectors[i-1].dest)
			{
				Atom* aa=D->Sectors[i].A;
				int dd=D->Sectors[i].dest;
				D->Sectors[i].A=D->Sectors[i-1].A;
				D->Sectors[i].dest=D->Sectors[i-1].dest;
				D->Sectors[i-1].A=aa;
				D->Sectors[i-1].dest=dd;
			}
		}
	}
}
void AutoForm::AddToFriendAng(AutoForm *AF)
{
	int dist=Norma(AF->x-x,AF->y-y);
	if(dist<5001)
	{
		if(!dist)dist=1;
		byte v2=(byte)(GetDir(AF->x-x,AF->y-y));
		byte alfa=v2-BSelf->Direction+8;
		byte ani=alfa>>4;
		Direction* D=&(FriendAng[ani]);
		
		D->Sectors[D->SectorsAmo].AF=AF;
		D->Sectors[D->SectorsAmo].dest=dist;
		D->SectorsAmo++;
		int dd=(AF->Force*10000)/dist;
		if(dist<700)
		{
			byte lef=(alfa-16);
			byte rat=(alfa+16);
			FriendAng[lef>>4].Dang+=dd/3;
			FriendAng[rat>>4].Dang+=dd/3;
		}
		if(dist<300)
		{
			byte lef=(alfa-32);
			byte rat=(alfa+32);
			FriendAng[lef>>4].Dang+=dd/6;
			FriendAng[rat>>4].Dang+=dd/6;
		}
		D->Dang+=dd;
		D->SumDang+=AF->Force;
		D->FireDang+=(AF->FireReady*900)/dist;
		D->SumFireDang+=AF->FireReady;

		for(int i=(D->SectorsAmo-1);i>0;i--)
		{
			if(D->Sectors[i].dest<D->Sectors[i-1].dest)
			{
				AutoForm* aa=D->Sectors[i].AF;
				int dd=D->Sectors[i].dest;
				D->Sectors[i].AF=D->Sectors[i-1].AF;
				D->Sectors[i].dest=D->Sectors[i-1].dest;
				D->Sectors[i-1].AF=aa;
				D->Sectors[i-1].dest=dd;
			}
		}
	}
}
byte AutoForm::GetMaxDangDirections()
{
	int md=0;
	byte rez=0;
	for(int i=0;i<16;i++)
		if(EnmAng[i].Dang>md)
		{
			rez=EnmAng[i].Dir;
			md=EnmAng[i].Dang;
		}
	return rez;
}
int AutoForm::GetSituationByIndex(int I)
{
	int rez=-1;

	switch (I)
	{
		//MT
	case  0: rez=DistToMT(); break;
	case  1: rez=MoralMT(); break;
	case  2: rez=KillAmoMT(); break;
	case  3: rez=FireFeadyMT(); break;
	case  4: rez=UnitsAmountMT(); break;
	case  5: rez=TiredMT(); break;
	case  6: rez=MoralMTAfterFire(); break;
		//Self
	case  7: rez=Moral(); break;
	case  8: rez=GetFireReady(); break;
	case  9: rez=UnitsAmount(); break;
	case 10: rez=GetTired(); break;
	case 11: rez=GetDistToAim(); break;
	case 12: rez=SumDangAndForce(); break;
	case 13: rez=FireForceDifference(); break;
	case 14: rez=Encircle(); break;
	case 15: rez=HaveReserv(); break;
	case 16: rez=AngToMT(); break;
	case 17: rez=DistInFront(); break;
	case 18: rez=DistInReserv(); break;
	case 19: rez=GetStand(); break;
	case 20: rez=IsEnemOnWay(); break;
	case 21: rez=GetNGrenades(); break;
	}
	if(I>99&&I<132)
		rez=GetState(I-100);
	return rez;
}
int AutoForm::DistToMT()
{
	int rez=-1;
	if(MainTarget)
		rez=Norma(x-MainTarget->x,y-MainTarget->y);
	else
		rez=9999999;
	return rez;
}
int AutoForm::CanIKillMT()
{
	int rez=0;
	if(MainTarget)
		rez=CanIKill(MainTarget->BSelf);
	return rez;
}
int AutoForm::MoralMT()
{
	int rez=0;
	if(MainTarget)
		rez=GetMoral(MainTarget->BSelf);
	return rez;
}
int AutoForm::KillAmoMT()
{
	int rez=0;
	int ReadyShuters;
	if(MainTarget)
		rez=GetDieAmount(MainTarget->BSelf,&ReadyShuters);
	return rez;
}
int AutoForm::FireFeadyMT()
{
	int rez=-1;
	if(MainTarget)
		if(MainTarget->UnitsAmo)
			rez=(MainTarget->FireReady*100)/MainTarget->UnitsAmo;
		else
			rez=0;
	return rez;
}
int AutoForm::UnitsAmountMT()
{
	if(MainTarget)
		return MainTarget->UnitsAmo;
	return -1;
}
int AutoForm::TiredMT()
{
	int rez=-1;
	if(MainTarget)
		rez=MainTarget->Tired/1000;
	return rez;
}
int AutoForm::AngToMT()
{
	int rez=-1;
	if(MainTarget)
	{
		byte dd=(byte)(GetDir(MainTarget->x-x,MainTarget->y-y));
		dd=BSelf->Direction-dd;
		char ot=(char)(dd);
		rez=abs(ot);
	}
	return rez;
}
int AutoForm::SumDangAndForce()
{
	int rez=Force;
	for(int i=0;i<16;i++)
	{
		rez+=FriendAng[i].Dang;
		rez-=EnmAng[i].Dang;
	}
	return rez;
}
bool AutoForm::HaveReserv()
{
	bool rez=false;
	AutoForm* R=GetFriendInBack();
	if(R)
	{
		if(R->GetFireReady()>90&&R->GetState(1))
			rez=true;
	}
	return rez;
}
int AutoForm::MoralMTAfterFire()
{
	if(MainTarget)
		return CanIKill(MainTarget->BSelf);
	else
		return -1;
}
int AutoForm::Moral()
{
	return GetMoral(BSelf);
}
int AutoForm::GetFireReady()
{
	if(UnitsAmo)
		return (FireReady*100)/UnitsAmo;
	else
		return -1;
}
int AutoForm::UnitsAmount()
{
	return UnitsAmo;
}
int AutoForm::GetTired()
{
	return Tired/1000;
}
void AutoForm::TurnToMT()
{
	if(MainTarget)
	{
		for(int i=0;i<16;i++)
			if(EnmAng[i].Dang>0&&EnmAng[i].Sectors[0].A==MainTarget)
			{
				byte dd=EnmAng[i].Dir;
				char ot=(char)dd;
				int dist=EnmAng[i].Sectors[0].dest;

				//if((abs(ot)*30)/dist)
				{
					byte nd=BSelf->Direction+EnmAng[i].Dir;
					TurnTo(nd);
				}
				return;
			}
	}
}
void AutoForm::MoveToMT()
{
	if(MainTarget)
	{
		int dist=150;
		byte d1=MainTarget->BSelf->Direction;
		int x1=MainTarget->x+((dist*TCos[d1])>>8);
		int y1=MainTarget->y+((dist*TSin[d1])>>8);

		byte d2=MainTarget->BSelf->Direction+128;
		int x2=MainTarget->x+((dist*TCos[d2])>>8);
		int y2=MainTarget->y+((dist*TSin[d2])>>8);

		if(Norma(x1-x,y1-y)>Norma(x2-x,y2-y))
		{
			MoveTo(x2,y2,d1);
		}
		else
		{
			MoveTo(x1,y1,d2);
		}

		//MoveTo(MainTarget->x,MainTarget->y);
	}
}
int AutoForm::GetMainTargetAngl()
{
	if(MainTarget)
		for(int i=0;i<16;i++)
			if(EnmAng[i].Dang>0&&EnmAng[i].Sectors[0].A==MainTarget)
				return EnmAng[i].Dir;
	return -1;
}
Atom* AutoForm::GetMaxDanderAtom()
{
	int md=0;
	Atom* rez=NULL;
	for(int i=0;i<16;i++)
		if((EnmAng[i].Dang-FriendAng[i].Dang)>md)
		{
			rez=EnmAng[i].Sectors[0].A;
			md=EnmAng[i].Dang-FriendAng[i].Dang;
		}
	return rez;
}
Atom* AutoForm::GetNearestAtom()
{
	int md=99999999;
	Atom* rez=NULL;
	for(int i=0;i<16;i++)
		if(EnmAng[i].SumDang&&EnmAng[i].Sectors[0].dest<md)
		{
			rez=EnmAng[i].Sectors[0].A;
			md=EnmAng[i].Sectors[0].dest;
		}
	if(MainTarget&&MainTarget->BSelf->WarType!=0)
		if((md*100)/DistToMT()>80)
			rez=MainTarget;
	return rez;
}
void AutoForm::ProcessRules()
{
	if(RulesList)
		RulesList->VerifySituation(this);
}
bool AutoForm::GetState(int Index)
{
	if(Index>-1&&Index<32)
		return StateArr[Index];
	else
		return false;
}
void AutoForm::SetState(int Index, bool Vle)
{
	if(Index>-1&&Index<32)
	{
		StateArr[Index]=Vle;
		if(Vle)
			for(int i=0;i<32;i++)
				if(NonStateArr[Index][i])
					StateArr[i]=false;
	}
}
int AutoForm::GetMaxActions()
{
	int rez=-1;
	int max=0;
	for(int i=0;i<32;i++)
		if(max<ActionsArr[i])
		{
			max=ActionsArr[i];
			rez=i;
		}
	return rez;
}
void AutoForm::ExecuteActions(int ActionsID)
{
	/*
	0 - MoveToMT !
	1 - TurnToMT !
	2 - MoveBack 
	3 - SetReadyToFire !
	4 - EnableFire !
	5 - DisableFire !
	6 - Stop
	7 - FormateToLine ! 
	8 - FormateToSquare !
	9 - FormateToKare !
	10- FindPlaceInFront();
	11- TurnToMaxDang();
	12 -MoveToAim
		*/
		/*
		0  - Attack
		1  - Reserv
		2  - MoveBack
		3  - Stand
		4  - Front
		5  - March
		6  - NearAim
		7  - MoveFront
		8  - BattleFormation
		9  - Rest
		10 - Move
		11 - ReadyToFire
		12 - LineFormation
		13 - SquareFormation
		14 - KareFormation
		*/
	if(ActionsID>-1)
	switch (ActionsID)
	{
	case -1: 
		break;
	case 0:
		MoveToMT();
		SetState(0,true);
		TimeInterval=54;
		break;
	case 1:
		TurnToMT();
		SetState(3,true);
		TimeInterval=30;
		break;
	case 2:
		MoveBack();
		SetState(2,true);
		TimeInterval=50;
		break;
	case 3:
		SetReadyToFire();
		TimeInterval=12;
		break;
	case 4:
		EnableFire();
		SetState(15,true);
		TimeInterval=53;
		break;
	case 5:
		DisableFire();
		SetState(16,true);
		TimeInterval=4;
		break;
	case 6:
		Stop();    //Rest
		SetState(9,true);
		TimeInterval=100;
		break;
	case 7:
		FormateToLine();
		SetState(12,true);
		TimeInterval=25;
		break;
	case 8:
		FormateToSquare();
		SetState(13,true);
		TimeInterval=25;
		break;
	case 9:
		FormateToKare();
		SetState(14,true);
		TimeInterval=25;
		break;
	case 10:
		FindPlaceInFront();
		//SetState(8,true);
		TimeInterval=25;
		break;
	case 11:
		TurnToMaxDang();
		TimeInterval=25;
		break;
	case 12:
		MoveToAim();
		SetState(5,true);//March
		TimeInterval=75;
		break;
	case 13:
		Castle();//рокировка
		TimeInterval=99;
		break;
	case 14:
		FireNow();
		TimeInterval=2;
		break;
	case 15:
		KeepDistInFront();
		TimeInterval=26;
		break;
	case 16:
		KeepDistInReserv();
		TimeInterval=31;
		break;
	case 17:
		Fire();
		TimeInterval=33;
		break;
	case 18:
		SetNotReadyToFire();
		TimeInterval=12;
		break;
	case 19:
		ThrowGrenade();
		TimeInterval=34;
		break;
	}
}
int AutoForm::GetDistToAim()
{
	int rez=-1;
	if(AimX>-1&&AimY>-1)
		rez=Norma(AimX-x,AimY-y);
	return rez;
}
void AutoForm::MoveToAim()
{
	if(AimX>-1&&AimY>-1)
		if(AimDir!=-1)
			MoveTo(AimX,AimY,AimDir);
		else
			MoveTo(AimX,AimY);
}
void AutoForm::MoveToFrontOf(AutoForm *A)
{
	if(A)
	{
		int r=A->GetSizeBack()+GetSizeFront();
		byte al=A->BSelf->Direction;
		int xx=A->x+((r*TCos[al])>>8);
		int yy=A->y+((r*TSin[al])>>8);
		MoveTo(xx,yy,A->BSelf->Direction);
	}
}
void AutoForm::MoveToBackOf(AutoForm *A)
{
	if(A)
	{
		int r=A->GetSizeBack()+GetSizeFront();
		byte al=A->BSelf->Direction+128;
		int xx=A->x+((r*TCos[al])>>8);
		int yy=A->y+((r*TSin[al])>>8);
		MoveTo(xx,yy,A->BSelf->Direction);
	}
}
void AutoForm::MoveToLeftOf(AutoForm *A)
{
	if(A)
	{
		int r=A->GetSizeLeft()+GetSizeRight();
		byte al=A->BSelf->Direction-64;
		int xx=A->x+((r*TCos[al])>>8);
		int yy=A->y+((r*TSin[al])>>8);
		MoveTo(xx,yy,A->BSelf->Direction);
	}
}
void AutoForm::MoveToRightOf(AutoForm *A)
{
	if(A)
	{
		int r=A->GetSizeRight()+GetSizeLeft();
		byte al=A->BSelf->Direction+64;
		int xx=A->x+((r*TCos[al])>>8);
		int yy=A->y+((r*TSin[al])>>8);
		MoveTo(xx,yy,A->BSelf->Direction);
	}
}
AutoForm* AutoForm::GetFriendInFront()
{
	AutoForm* rez=NULL;
	byte an=4;
	for(int i=14;i<16;i++)
		if(FriendAng[i].Dang&&(16-i)<an&&FriendAng[i].Sectors[0].dest<1200)
		{
			an=16-i;
			rez=FriendAng[i].Sectors[0].AF;
		}
	for(int i=0;i<3;i++)
		if(FriendAng[i].Dang&&i<an)
		{
			an=i;
			rez=FriendAng[i].Sectors[0].AF;
		}
	return rez;
}
AutoForm* AutoForm::GetFriendInBack()
{
	AutoForm* rez=NULL;
	byte an=3;
	for(int i=6;i<11;i++)
		if(FriendAng[i].Dang&&abs(8-i)<an&&FriendAng[i].Sectors[0].dest<1200)
		{
			an=abs(8-i);
			rez=FriendAng[i].Sectors[0].AF;
		}
	return rez;
}
AutoForm* AutoForm::GetFriendInLeft()
{
	AutoForm* rez=NULL;
	byte an=2;
	for(int i=3;i<6;i++)
		if(FriendAng[i].Dang&&abs(4-i)<an&&FriendAng[i].Sectors[0].dest<1200)
		{
			an=abs(4-i);
			rez=FriendAng[i].Sectors[0].AF;
		}
	return rez;
}
AutoForm* AutoForm::GetFriendInRight()
{
	AutoForm* rez=NULL;
	byte an=2;
	for(int i=11;i<14;i++)
		if(FriendAng[i].Dang&&abs(12-i)<an&&FriendAng[i].Sectors[0].dest<1200)
		{
			an=abs(12-i);
			rez=FriendAng[i].Sectors[0].AF;
		}
	return rez;
}
void AutoForm::FindPlaceInFront()
{
	int a1=GetNearestFriendDirection();
	int a2=GetMaxDangDirectionMinus();
	char fr=(char)(a1<<4);
	char en=(char)(a2<<4);
	if(a1==-1||(abs(fr)>50&&abs(en)<64))
	{
		SetState(4,true);//Front
	}
	else
	{
		SetState(1,true);//Reserv;
	}
	//if(!IsInBatleFormation())
 	if(a1>-1&&a2>-1)
	{
		

		char ang=fr-en;
		if(GetState(1)&&abs(ang)<55)
		{
			if(FriendAng[a1].Sectors[0].AF->GetState(4))
			{
				bool flag=false;
				if(ang>0)
					if(!FriendAng[a1].Sectors[0].AF->GetFriendInLeft())
					{
						MoveToLeftOf(FriendAng[a1].Sectors[0].AF);
						SetState(4,true);
						flag=true;
					}
				else
					if(!FriendAng[a1].Sectors[0].AF->GetFriendInRight())
					{
						MoveToRightOf(FriendAng[a1].Sectors[0].AF);
						SetState(4,true);
						flag=true;
					}
				if(!flag)				
				{
					if(!FriendAng[a1].Sectors[0].AF->GetFriendInBack())
						MoveToBackOf(FriendAng[a1].Sectors[0].AF);
					SetState(1,true);
				}
				SetState(8,true);
			}
			else
			{
				//wait
			}
		}
		/*
		else
		{
			//TurnToMaxDang();
			SetState(8,true);//BattleFormation
			SetState(4,true);//Front
		}
		*/
	}
}
void AutoForm::FindPlaceInReserv()
{
	if(GetState(1))//Reserv
	{
		int dd=99999999;
		AutoForm* Frontovic=NULL;
		for(int i=0;i<16;i++)
			if(FriendAng[i].Dang)
				for(int j=0;j<FriendAng[i].SectorsAmo;j++)
					if(FriendAng[i].Sectors[j].AF->GetState(4))
						if(FriendAng[i].Sectors[j].dest<dd)
							if(!(FriendAng[i].Sectors[j].AF->GetFriendInBack()&&FriendAng[i].Sectors[j].AF->GetFriendInBack()->GetState(1)))
							{
								dd=FriendAng[i].Sectors[j].dest;
								Frontovic=FriendAng[i].Sectors[j].AF;
							}
		if(Frontovic)
			MoveToBackOf(Frontovic);
	}
}
int  AutoForm::GetMaxDangDirectionMinus()
{
	int rez=-1;
	int dd=-99999999;
	for(int i=0;i<16;i++)
		if((EnmAng[i].Dang-FriendAng[i].Dang)>dd)
		{
			rez=i;
			dd=EnmAng[i].Dang-FriendAng[i].Dang;
		}
	return rez;
}
int AutoForm::GetNearestFriendDirection()
{
	int rez=-1;
	int dd=999999;
	for(int i=0;i<16;i++)
		if((FriendAng[i].SumDang>0)&&(FriendAng[i].Sectors[0].dest<dd))
		{
			rez=i;
			dd=FriendAng[i].Sectors[0].dest;
		}
	return rez;
}
bool AutoForm::IsInBatleFormation()
{
	AutoForm* Fa=GetFriendInFront();
	AutoForm* Ba=GetFriendInBack();
	AutoForm* La=GetFriendInLeft();
	AutoForm* Ra=GetFriendInRight();
	if((Fa&&Fa->GetState(8))||(Ba&&Ba->GetState(8))||(La&&La->GetState(8))||(Ra&&Ra->GetState(8)))
		return true;
	return false;
}
bool AutoForm::IsNearAim()
{
	bool rez=true;
	if(AimX!=-1&&AimY!=-1)
		if(Norma(x-AimX,y-AimY)>600)
			rez=false;
	return rez;
}
void AutoForm::TurnToMaxDang()
{
	int d=GetMaxDangDirectionMinus();
	if(d>0)
	{
		byte an=(byte) d;
		an=an<<4;
		byte ra=BSelf->Direction-an;
		//BSelf->HumanLocalSendTo(x<<4,y<<4,ra,128,0);
		MoveTo(x,y,ra);
	}
}
int AutoForm::ForceDifference()
{
	int rez=0;
	for(int i=0;i<16;i++)
		rez+=FriendAng[i].Dang-EnmAng[i].Dang;
	return rez;
}
int AutoForm::FireForceDifference()
{
	int rez=0;
	for(int i=0;i<16;i++)
		rez+=FriendAng[i].FireDang-EnmAng[i].FireDang;
	if(FriendAng[0].FireDang)
		rez+=FriendAng[0].FireDang*2;
	return rez;
}
int AutoForm::Encircle()
{
	int co=0;
	byte razv=0;
	byte vr=0;
	for(int i=0;i<16;i++)
	{
		if(co)
			vr++;
		if(EnmAng[i].SumDang)
			if(EnmAng[i].Sectors[0].dest<930)
			{
				if(co)
				{
					if(vr>8)vr=16-vr;
					razv+=vr;
					vr=0;
				}
				co++;
			}
	}
	return razv<<4;
}
//UpLevel
int AutoForm::DistInFront()
{
	int rez=-1;
	if(DistToMainTarget>0&&GetState(4))
	{
		int d=DistToMT();
		rez=(d*100)/DistToMainTarget;
	}
	return rez;
}
void AutoForm::KeepDistInFront()
{
	if(DistToMainTarget!=-1)
	{
		int xx=0 ,yy=0;
		GetCoordToMTWay(xx, yy, FindNearestFriendInMTDirection());
		byte dr=GetDir(MainTarget->x-xx,MainTarget->y-yy);
		dr=((dr+8)>>4)<<4;
		if(xx&&yy&&(abs((xx-x)*(yy-y))>50))
			MoveTo(xx,yy,dr);
	}
}
int AutoForm::DistInReserv()
{
	int rez=-1;
	if(GetState(1))
	{
		AutoForm* Front = FindNearestFriendInMTDirection();
		if(Front&&Front->GetState(4))
		{
			int dst=Norma(x-Front->x,y-Front->y);
			if(!dst)dst=1;
			rez=(dst*100)/(Front->GetSizeBack()+GetSizeFront());
		}
	}
	return rez;
}
void AutoForm::KeepDistInReserv()
{
	if(GetState(1))
	{
		AutoForm* Front = FindNearestFriendInMTDirection();
		if(Front)
			MoveToBackOf(Front);
		else
			KeepDistInFront();
	}
}
void AutoForm::Fire()
{
	if(MainTarget)
		AttackBrigade(MainTarget->BSelf);
}
int AutoForm::GetStand()
{
	return IsStand;
}
bool AutoForm::IsEnemOnWay()
{
	bool rez=false;
	int ed=DistToMT();
	if(!IsNearAim()&&ed<1200)
	{
		int nx,ny;
		if(GetTopZoneCoor(GetNextZone(GetTopZone(x,y),GetTopZone(AimX,AimY)),&nx,&ny))
		{
			int ned=Norma(nx-MainTarget->x,ny-MainTarget->y);
			if(((ned*100)/ed)<85)
				rez=true;
		}
	}
	return rez;
}
int AutoForm::GetNGrenades()
{
	return BSelf->NGrenades;
}
void AutoForm::ThrowGrenade()
{
	ComThrowGrenade(NI,BSelf->ID);
}
//====================================================================//
//====================================================================//
AutoForm* BrigadeAI::AF[];
int BrigadeAI::MaxAF=0;
Regiment* BrigadeAI::Reg[];
int BrigadeAI::MaxReg=0;
Rules BrigadeAI::AllRules[];
Atom* BrigadeAI::AllForm[8][256];
int BrigadeAI::AllFormAmo[];
int BrigadeAI::AllFormFillTime=0;

BrigadeAI::BrigadeAI()
{
	memset(AF,0,256*4);
	memset(Reg,0,128*4);
	MaxAF=0;
	FillNeighborsTime=0;
	AllFormFillTime=0;
	AllFormAmo[0]=0;
	AllFormAmo[1]=0;
	AllFormAmo[2]=0;
	AllFormAmo[3]=0;
	AllFormAmo[4]=0;
	AllFormAmo[5]=0;
	AllFormAmo[6]=0;
	AllFormAmo[7]=0;
	FirstStart=true;
}
BrigadeAI::~BrigadeAI()
{
	/*for(int i=0;i<MaxAF;i++)
		if(AF[i]!=NULL)
			delete (AF[i]);
	*/

	for(int i=0;i<8;i++)
		for(int j=0;j<AllFormAmo[i];j++)
			if(AllForm[i][j])
				delete (AllForm[i][j]);
	for(int i=0;i<32;i++)
	{
		if(AllRules[i].First)
			delete AllRules[i].First;
	}
	for(int i=0;i<MaxReg;i++)
		if(Reg[i])
			delete (Reg[i]);
}
int BrigadeAI::AddAutoBrigade(Brigade* B)
{
	int rez=-1, i=0;
	for(i=0;i<MaxAF;i++)
		if((AF[i])&&AF[i]->BSelf==B)
			return AF[i]->Index;
	for(i=0;i<(MaxAF+1);i++)
	{
		if(AF[i]==NULL)
		{
			AF[i] = new AutoForm(B);
			AF[i]->RulesID=0;
			AF[i]->Index;
			rez=i;
			//
			//CalculateFormationWight(&(ElementaryOrders[AF[i]->BSelf->WarType-1]));
			//
			AF[i]->RulesList=&(AllRules[AF[i]->RulesID]);
			int NI=AF[i]->BSelf->CT->NI;
			for(int j=0;j<AllFormAmo[NI];j++)
				if(AllForm[NI][j]->BSelf->ID==AF[i]->BSelf->ID)
				{
					delete (AllForm[NI][j]);
					AllForm[NI][j]=AF[i];
				}
			break;
		}
	}
	if(i==MaxAF)MaxAF=i+1;
	return rez;
}
void BrigadeAI::Process()
{
	if(FirstStart)
	{
		LoadAllRules("AI/BrigadeAI/Rules.ai");
		FirstStart=false;
	}
	FillAllForm();
	

	if(tmtmt>FillNeighborsTime)
	{
		for(int i=0;i<8;i++)
			for(int j=0;j<AllFormAmo[i];j++)
				AllForm[i][j]->UpdateParams();
		//FillNeighbors();
		FillEnmAng();
		FillFriendAng();
		FillNeighborsTime=tmtmt+2;
	}
	int  i;
	for(int i=0;i<MaxReg;i++)
	{
		if(Reg[i]!=NULL)
		{
			Reg[i]->Process();
		}
	}
	for(int i=0;i<MaxAF;i++)
	{
		if(AF[i]!=NULL)
		{
			AF[i]->Process();
		}
	}
}

void BrigadeAI::FillEnmAng()
{
	for(int i=0;i<MaxAF;i++)
	{
		AutoForm* Fo=AF[i];
		Fo->ClearEnmAng();
		byte NI=Fo->BSelf->CT->NI;
		byte NMask=Fo->BSelf->CT->Nat->NMask;
		if(Fo&&(Fo->BSelf->WarType!=0))
			for(int j=0;j<8;j++)
				if(j!=NI&&!(NMask&NATIONS[j].NMask))
					for(int k=0;k<AllFormAmo[j];k++)
					{
						Atom* A=AllForm[j][k];
						if(A&&A->BSelf&&A->BSelf->WarType!=0)
							Fo->AddToEnmAng(A);
					}
	}
}
void BrigadeAI::FillFriendAng()
{
	for(int i=0;i<MaxAF;i++)
	{
		AutoForm* Fo=AF[i];
		Fo->ClearFriendAng();
		//byte NI=Fo->BSelf->CT->NI;
		byte NMask=Fo->BSelf->CT->Nat->NMask;
		if(Fo&&(Fo->BSelf->WarType!=0))
			for(int j=0;j<MaxAF;j++)
			{
				AutoForm* AA=AF[j];
				byte NMask2=AA->BSelf->CT->Nat->NMask;
				if(Fo!=AA&&(NMask&NMask2)&&AA->BSelf->WarType!=0)
					Fo->AddToFriendAng(AA);
			}

	}
}
void BrigadeAI::FillAllForm()
{
	if(AllFormFillTime<tmtmt)
	{
		for(int i=0;i<8;i++)
			for(int j=0;j<MaxBrig;j++)
			{
				Brigade* BR=&(CITY[i].Brigs[j]);
				if(BR->Enabled&&BR->WarType!=0)
				{
					AllForm[i][AllFormAmo[i]] = new Atom(BR);
					AllFormAmo[i]++;
				}

			}
		AllFormFillTime=999999999;
	}
}
void BrigadeAI::LoadAllRules(char *p)
{
	DString R, fw;
	R.ReadFromFile(p);
	bool f=true;
	if(R.L)
	for(int i=0;i<32&&f;i++)
	{
		R.ExtractWord(fw);
		if(fw.str[0]=='#')
		{
			//AllRules[i].UnitType=
			AllRules[i].LoadRules(&R);
			f=false;
		}
	}
}
int BrigadeAI::CreateRegiment()
{
	int rez=-1;
	if(MaxReg<127)
	{
		Reg[MaxReg]= new Regiment(MaxReg,AF,&MaxAF);
		rez=MaxReg;
		MaxReg++;
	}
	return rez;
}
Polk* BrigadeAI::GetPolkByIndex(int PIndex)
{
	Polk* rez=NULL;
	if(PIndex<MaxReg)
		rez=(Polk*) Reg[PIndex];
	return rez;
}
//====================================================================//
Rule::Rule()
{
	for(int i=0;i<16;i++)
	{
		Situations[i].CompareID=0;
		Situations[i].FunctionID1=-1;
		Situations[i].FunctionID2=-1;
	}
	for(int i=0;i<16;i++)Actions[i].ActionID=-1;
	Next=NULL;
}
Rule::~Rule()
{
	if(Next)
		delete Next;
}
bool Rule::CheckSituation(AutoForm *A, int i)
{
	bool rez=true;
	int vle1, vle2;

	if(Situations[i].FunctionID1!=-1)
	{
		vle1=A->GetSituationByIndex(Situations[i].FunctionID1);
		if(vle1==-1)return false;
	}
	else
		vle1=Situations[i].Value1;
	if(Situations[i].FunctionID2!=-1)
	{
		vle2=A->GetSituationByIndex(Situations[i].FunctionID2);
		if(vle2==-1)return false;
	}
	else
		vle2=Situations[i].Value2;

	switch (Situations[i].CompareID)
	{
	case 0: 
		break;
	case 1: //NOT
		if(Situations[i].FunctionID1!=-1)
			rez=!A->GetSituationByIndex(Situations[i].FunctionID1);
		break;
	case 2: // ">"		
		rez=(vle1>vle2);
		break;
	case 3: // "<"
		rez=(vle1<vle2);
		break;
	case 4: // "="
		rez=(vle1==vle2);
		break;
	case 5: // ">="
		rez=(vle1>=vle2);
		break;
	case 6: // "<="
		rez=(vle1>=vle2);
		break;
	case 7: // "!="
		rez=(vle1!=vle2);
		break;
	}
	return rez;
}
bool Rule::CheckRule(AutoForm *A)
{
	bool rez=true;
	for(int i=0;i<16&&rez;i++)
		rez=CheckSituation(A, i);
	return rez;
}
void Rule::FillSituations(DString *s)
{
	int poi=0;
	DString p1, iff, p2;
	while(s->L)
	{
		s->ExtractWord(p1);
		s->ExtractWord(iff);
		s->ExtractWord(p2);
		SetSituation(p1.str, iff.str, p2.str, &(Situations[poi]));
		poi++;
	}
}
void Rule::SetSituation(char* p1,char *iff, char *p2, Situation *sit)
{
	//
	if(isdigit(p1[0])||p1[0]=='-')
		sit->Value1=atoi(p1);
	else
		sit->FunctionID1=GetFunctionID(p1);
	
	if(isdigit(p2[0])||p2[0]=='-')
		sit->Value2=atoi(p2);
	else
		sit->FunctionID2=GetFunctionID(p2);
	
	sit->CompareID=GetCompareID(iff);
}
int Rule::GetCompareID(char *iff)
{
	int rez=0;
	if(!strcmp(iff,"!"))  rez=1;
	else
	if(!strcmp(iff,">"))  rez=2;
	else
	if(!strcmp(iff,"<"))  rez=3;
	else
	if(!strcmp(iff,"="))  rez=4;
	else
	if(!strcmp(iff,">=")) rez=5;
	else
	if(!strcmp(iff,"<=")) rez=6;
	else
	if(!strcmp(iff,"!=")) rez=7;

	return rez;
}
int Rule::GetFunctionID(char *fff)
{
	int rez=-1;
	if(!strcmp(fff,"DistToMT"))         rez=0;
	else
	if(!strcmp(fff,"MoralMT"))          rez=1;
	else
	if(!strcmp(fff,"KillAmoMT"))        rez=2;
	else
	if(!strcmp(fff,"FireFeadyMT"))      rez=3;
	else
	if(!strcmp(fff,"UnitsAmountMT"))    rez=4;
	else
	if(!strcmp(fff,"TiredMT"))          rez=5;
	else
	if(!strcmp(fff,"MoralMTAfterFire")) rez=6;
	else
	if(!strcmp(fff,"Moral"))            rez=7;
	else
	if(!strcmp(fff,"GetFireReady"))     rez=8;
	else
	if(!strcmp(fff,"UnitsAmount"))      rez=9;
	else
	if(!strcmp(fff,"GetTired"))         rez=10;
	else
	if(!strcmp(fff,"DistToAim"))      rez=11;
	else
	if(!strcmp(fff,"SumDangAndForce"))      rez=12;
	else
	if(!strcmp(fff,"FireForceDifference"))      rez=13;
	else
	if(!strcmp(fff,"Encircle"))      rez=14;
	else
	if(!strcmp(fff,"HaveReserv"))      rez=15;
	else
	if(!strcmp(fff,"AngToMT"))      rez=16;
	else
	if(!strcmp(fff,"DistInFront"))      rez=17;
	else
	if(!strcmp(fff,"DistInReserv"))      rez=18;
	else
	if(!strcmp(fff,"GetStand"))      rez=19;
	else
	if(!strcmp(fff,"IsEnemOnWay")) rez=20;
	else
	if(!strcmp(fff,"GetNGrenades")) rez=21;
	else
	{
		rez=GetStateID(fff);
		if(rez>-1)
			rez+=100;
	}

	return rez;
}
void Rule::FillActions(DString *s)
{
	int poi=0;
	DString a, w;
	while(s->L)
	{
		s->ExtractWord(a);
		s->ExtractWord(w);
		int aid=GetActionID(a.str);
		if(aid>-1)
		{
			Actions[poi].ActionID=aid;
			Actions[poi].Weight=atoi(w.str);
			poi++;
		}
	}
}
int Rule::GetActionID(char *aa)
{
	int rez=-1;
	if(!strcmp(aa,"AttackMT"))       rez=0;
	else
	if(!strcmp(aa,"TurnToMT"))       rez=1;
	else
	if(!strcmp(aa,"MoveBack"))       rez=2;
	else
	if(!strcmp(aa,"SetReadyToFire")) rez=3;
	else
	if(!strcmp(aa,"EnableFire"))     rez=4;
	else
	if(!strcmp(aa,"DisableFire"))    rez=5;
	else
	if(!strcmp(aa,"Stop"))           rez=6;
	else
	if(!strcmp(aa,"FormateToLine"))  rez=7;
	else
	if(!strcmp(aa,"FormateToSquare"))rez=8;
	else
	if(!strcmp(aa,"FormateToKare"))  rez=9;
	else
	if(!strcmp(aa,"FindPlaceInFront"))  rez=10;
	else
	if(!strcmp(aa,"TurnToMaxDang"))  rez=11;
	else
	if(!strcmp(aa,"MoveToAim"))  rez=12;
	else
	if(!strcmp(aa,"Castle"))  rez=13;
	else
	if(!strcmp(aa,"FireNow"))  rez=14;
	else
	if(!strcmp(aa,"KeepDistInFront"))  rez=15;
	else
	if(!strcmp(aa,"KeepDistInReserv")) 	rez=16;
	else
	if(!strcmp(aa,"Fire")) 	rez=17;
	else
	if(!strcmp(aa,"SetNotReadyToFire")) 	rez=18;
	else
	if(!strcmp(aa,"ThrowGrenade")) rez=19;
	
	return rez;
}
int Rule::GetStateID(char *aa)
{
	int rez=-1;
	if(strstr(aa,"St_")!=NULL)
	{
		char* aaa = aa+3;
		if(!strcmp(aaa,"Attack"))       rez=0;
		else
		if(!strcmp(aaa,"Reserv"))       rez=1;
		else
		if(!strcmp(aaa,"MoveBack"))       rez=2;
		else
		if(!strcmp(aaa,"Stand")) rez=3;
		else
		if(!strcmp(aaa,"Front"))     rez=4;
		else
		if(!strcmp(aaa,"March"))    rez=5;
		else
		if(!strcmp(aaa,"NearAim"))           rez=6;
		else
		if(!strcmp(aaa,"MoveFront"))  rez=7;
		else
		if(!strcmp(aaa,"BattleFormation"))rez=8;
		else
		if(!strcmp(aaa,"Rest"))  rez=9;
		else
		if(!strcmp(aaa,"Move"))  rez=10;
		else
		if(!strcmp(aaa,"ReadyToFire"))  rez=11;
		else
		if(!strcmp(aaa,"LineFormation"))     rez=12;
		else
		if(!strcmp(aaa,"SquareFormation"))    rez=13;
		else
		if(!strcmp(aaa,"KareFormation"))      rez=14;
		else
		if(!strcmp(aaa,"EnableFire"))  rez=15;
		else
		if(!strcmp(aaa,"DisableFire"))rez=16;
	}
	return rez;
}
//====================================================================//
Rules::Rules()
{
	First=NULL;
	Last=NULL;
}
Rules::~Rules()
{}
void Rules::VerifySituation(AutoForm *A)
{
	if(A)
	{
		for(int i=0;i<32;i++)
			A->ActionsArr[i]=0;
		Pointer=First;
		while(Pointer)
		{
			if(Pointer->CheckRule(A))
				for(int i=0;i<8;i++)
					if(Pointer->Actions[i].ActionID>-1)
						A->ActionsArr[Pointer->Actions[i].ActionID]+=Pointer->Actions[i].Weight;
			Pointer=Pointer->Next;
		}
	}
}
void Rules::LoadRules(DString *d)
{
	DString ds;
	d->ExtractWord(ds);
	while(ds.str[0]!='#'&&ds.str[0]!=0)
	{
		if(!strcmp(ds.str,"if"))
		{
			Rule* R = new Rule();

			if(!First)First=R;
			if(Last)Last->Next=R;
			Last=R;

			d->ExtractLine(ds);
			R->FillSituations(&ds);
			d->ExtractWord(ds);
			if(!strcmp(ds.str,"do"))
			{
				d->ExtractLine(ds);
				R->FillActions(&ds);
			}
		}
		d->ExtractWord(ds);
	}
}
int AutoForm::GetSizeFront()
{
	int rez=-1;
	if(BSelf->WarType>0)
		rez=ElementaryOrders[BSelf->WarType-1].Hight/2+140;
	return rez;
}
int AutoForm::GetSizeBack()
{
	return GetSizeFront();
}
int AutoForm::GetSizeLeft()
{
	int rez=-1;
	if(BSelf->WarType>0)
		rez=ElementaryOrders[BSelf->WarType-1].Width/2+50;
	return rez;
}
int AutoForm::GetSizeRight()
{
	return GetSizeLeft();
}
//====================================================================//
Polk::Polk()
{}
Polk::~Polk()
{}
void Polk::MovePolkTo(int x, int y, int Dir)
{}
int Polk::GetEnemyForce()
{
	return 0;
}
int Polk::GetPolkIndex()
{
	return -1;
}
bool Polk::ReSetFormation(int FormIndex)
{
	return false;
}
void Polk::AddSelectedToPolk(byte Nat)
{}
int Polk::GetFormationIndex(int UnitType)
{
	return -1;
}
//====================================================================//
Regiment::Regiment()
{
	Index=-1;
	DistX=-1;
	DistY=-1;
	Radius=1200;
	SearchEnemyRadius=3000;
}
Regiment::~Regiment()
{
 //
}
Regiment::Regiment(int I, AutoForm** AFL, int* MaxAFL)
{
	Index=I;
	DistX=-1;
	DistY=-1;
	DistDir=-1;

	DistX=8000;
	DistY=5000;
	DistDir=128;

	
	AFList=AFL;
	MaxAFList=MaxAFL;
	TableFillTime=0;
	EnemyForce=-1;
	Chase=true;
}
void Regiment::MovePolkTo(int x, int y, int Dir)
{
	DistX=x;
	DistY=y;
	DistDir=Dir;
	FillDestPoints();
}
int Regiment::GetEnemyForce()
{
	return EnemyForce;
}
int Regiment::GetPolkIndex()
{
	return Index;
}
bool Regiment::AddBrigade(Brigade *B)
{
	int in= BrigadeAI::AddAutoBrigade(B);
	if(in>-1)
	{
		AFList[in]->RegimentIndex=Index;
		AFList[in]->AimX=DistX;
		AFList[in]->AimY=DistY;
	}
/*	for(int i=0;i<*MaxAFList;i++)
		if((AFList[i])&&AFList[i]->BSelf==Br)
		{
			AFList[i]->RegimentIndex=Index;
			AFList[i]->AimX=DistX;
			AFList[i]->AimY=DistY;
			return true;
		}
	for(int i=0;i<(*MaxAFList+1);i++)
	{
		if(AFList[i]==NULL)
		{
			AFList[i] = new AutoForm(Br);
			AFList[i]->RegimentIndex=Index;
			AFList[i]->AimX=DistX;
			AFList[i]->AimY=DistY;
			break;
		}
	}
	if(i==*MaxAFList)*MaxAFList=i+1;
	return true;
//////////////////////////////////////////////////////////////////
	for(int i=0;i<*MaxAFList;i++)
		if((AFList[i])&&AFList[i]->BSelf==B)
		{
			AFList[i]->RegimentIndex=Index;
			AFList[i]->AimX=DistX;
			AFList[i]->AimY=DistY;
			return true;
		}
	for(int i=0;i<(MaxAF+1);i++)
	{
		if(AFList[i]==NULL)
		{
			AFList[i] = new AutoForm(B);
			AFList[i]->RegimentIndex=Index;
			AFList[i]->AimX=DistX;
			AFList[i]->AimY=DistY;

			AFList[i]->RulesID=0;
			AFList[i]->RulesList=&(AllRules[AFList[i]->RulesID]);
			int NI=AFList[i]->BSelf->CT->NI;
			for(int j=0;j<AllFormAmo[NI];j++)
				if(AllForm[NI][j]->BSelf->ID==AFList[i]->BSelf->ID)
				{
					delete (AllForm[NI][j]);
					AllForm[NI][j]=AFList[i];
				}
			break;
		}
	}
	if(i==*MaxAFList)*MaxAFList=i+1;
	return true;
	*/
	return true;
}
AutoForm* Regiment::GetAutoForm(int UnitType)
{
	for(int i=0;i<*MaxAFList;i++)
		if(AFList[i]->RegimentIndex==Index&&AFList[i]->BSelf->MembID==UnitType)
			return AFList[i];
	return NULL;
}
int Regiment::GetFormationIndex(int UnitType)
{
   	for(int i=0;i<*MaxAFList;i++)
		if(AFList[i]->RegimentIndex==Index&&AFList[i]->BSelf->MembID==UnitType)
			return i;
	return -1;
}
bool Regiment::ReSetFormation(int FormIndex)
{
	if(FormIndex>-1)
		if(AFList[FormIndex])
		{
			AFList[FormIndex]->RegimentIndex=Index;
			AFList[FormIndex]->AimX=DistX;
			AFList[FormIndex]->AimY=DistY;
			return true;
		}
	return false;
}
void Regiment::Process()
{
	if(tmtmt>(TableFillTime+75))
	{
		TableFillTime=tmtmt;
		FillTable();
		SetActualDist();
	}
}
void Regiment::AddSelectedToPolk(byte Nat)
{
	if(Nat>-1&&Nat<8&&NSL[Nat]>0)
	{
		for(int i=0;i<NSL[Nat];i+=15)
		{
			OneObject* OB=Group[Selm[Nat][i]];
			if(OB&&OB->BrigadeID!=0xFFFF)
				AddBrigade(&(CITY[Nat].Brigs[OB->BrigadeID]));
		}
	}
}
void Regiment::FillTable()
{
	MaxP=0;
	MaxE=0;
	for(int d=0;d<64;d++)
		for(int f=0;f<64;f++)
		{
			TNearestP[d][f]=false;
			TNearestE[d][f]=false;
			TVisible[d][f]=false;
			TDist[d][f]=-1;
		}
	for(int i=0;i<*MaxAFList;i++)
	{
		AutoForm* AF=AFList[i];
		if(AF&&AF->BSelf->WarType!=0)
			if(AF->RegimentIndex==Index)
				AddFillEn(AF);
	}
	for(int i=0;i<MaxP;i++)
	{
		int md=99999999;
		int ind=-1;
		for(int j=0;j<MaxE;j++)
			if(TDist[i][j]<md)
			{
				md=TDist[i][j];
				ind=j;
			}
		TNearestE[i][ind]=true;
	}
	for(int i=0;i<MaxE;i++)
	{
		int md=99999999;
		int ind=-1;
		for(int j=0;j<MaxP;j++)
			if(TDist[j][i]>0&&TDist[j][i]<md)
			{
				md=TDist[j][i];
				ind=j;
			}
		TNearestP[ind][i]=true;
	}
}
void Regiment::AnalizeTable()
{
	EnemyForce = (MaxE*100)/MaxP;
}
int Regiment::AddE(Atom *A)
{
	if(A)
	{
		for(int i=0;i<MaxE;i++)
			if(E[i]==A)
				return i;
		E[MaxE]=A;
		MaxE++;
		return MaxE-1;
	}
	return -1;
}
void Regiment::AddFillEn(AutoForm *AF)
{
	if(AF)
	{
		P[MaxP]=AF;
		int Pi=MaxP;
		MaxP++;
		for(int i=0;i<16;i++)
			if(AF->EnmAng[i].SumDang)
				for(int j=0;j<AF->EnmAng[i].SectorsAmo;j++)
				{
					int Ei=AddE(AF->EnmAng[i].Sectors[j].A);
					TDist[Pi][Ei]=AF->EnmAng[i].Sectors[j].dest;
					if(AF->FriendAng[i].Dang&&j==0)
						TVisible[Pi][Ei]=false;
					else
						TVisible[Pi][Ei]=true;
				}
	}
}
int Regiment::GetActualDist()
{
	int rez=-1;
	int mindist=999999999;
	int round=0;
	for(int i=0;i<MaxP;i++)
	{
		int c=0;
		for(int j=0;j<MaxE;j++)
		{
			if(TNearestP[i][j])
			{
				c++;
				if(TDist[i][j]>0&&TDist[i][j]<mindist)
					mindist=TDist[i][j];
			}
		}
		if(c>1)
			round++;
	}
	if(round)
		rez=mindist-20*round;
	if(MaxE==1)rez=600;
	return rez;
}
void Regiment::SetActualDist()
{
	if(Chase)
	{
		int dd=GetActualDist();
		for(int i=0;i<MaxP;i++)
			if(P[i])
				P[i]->DistToMainTarget=dd;
	}
	else
	{
		for(int i=0;i<MaxP;i++)
		{}
	}
	//SetDistIfEnmAlone();
	FillDestPoints();
}
void Regiment::SetDistIfEnmAlone()
{
	for(int i=0;i<MaxE;i++)
	{
		int co=0;
		bool twis=true;
		for(int j=0;j<MaxP&&twis;j++)
		{
			if(TNearestE[j][i])
			{
				bool flg=true;
				for(int k=0;k<MaxE&&flg;k++)
					if(TNearestP[j][k]&&k!=i)
					{
						flg=false;
						twis=false;
					}
				if(flg)
					co++;
			}
				
		}
		if(co>1&&twis)
		{
			int dst=800-100*co;
			if(dst<400)dst=400;
			for(int j=0;j<MaxP;j++)
			{
				bool flg=false;
				if(TNearestE[j][i])
				{
					flg=true;
					for(int k=0;k<MaxE&&flg;k++)
						if(TNearestP[j][k]&&k!=i)
							flg=false;
				}
				if(flg)
					P[j]->DistToMainTarget=dst;	
			}
		}
	}
}
void Regiment::FillDestPoints()
{
	if(DistX!=-1&&DistY!=-1&&MaxP)
	{
		if(DistDir==-1)DistDir=0;
		for(int i=0;i<MaxP;i++)
			if(P[i])
			{
				P[i]->AimX=-1;
				P[i]->AimY=-1;
				P[i]->AimDir=-1;
			}
		int column = (int)sqrt(MaxP*10000*2);
		if(column%100)
			column=column/100+1;
		else
			column=column/100;
		int line = MaxP/column;
		if(MaxP%column)line++;

		int RangeX = 500;
		int RangeY = 900;
		for(int y=0;y<column;y++)
			for(int x=0;x<line;x++)
			{
				if(((y+1)*(x+1))<(MaxP+1))
				{
					int xx=DistX-(column/2)*RangeX+x*RangeX;
					int yy=DistY-(line/2)*RangeY+y*RangeY;

					int xa=abs(((xx*TCos[DistDir])>>8)-((yy*TSin[DistDir])>>8));
					int ya=abs(((xx*TSin[DistDir])>>8)+((yy*TCos[DistDir])>>8));

					AutoForm* Nr=NULL;
					int dst=9999999;
					for(int i=0;i<MaxP;i++)
						if(P[i])
							if(P[i]->AimX==-1)
							{
								int dd=Norma(P[i]->x-xa,P[i]->y-ya);
								if(dd<dst)
								{
									dst=dd;
									Nr=P[i];
								}
							}
					if(Nr)
					{
						Nr->AimX=xa;
						Nr->AimY=ya;
						if(DistDir!=-1)
							Nr->AimDir=DistDir;
					}
				}
			}
	}
}
bool Regiment::IsChase()
{
	return Chase;
}
void Regiment::SetChaseOff()
{
	Chase=false;
}
void Regiment::SetChaseOn()
{
	Chase=true;
}
//====================================================================//
//====================================================================//
BrigadeAI WiselyFormations;
void WiselyFormationsProcess()
{
	WiselyFormations.Process();
}
void AddBrigadeInWiselyFormations(Brigade* B)
{
	if(B)
	{
		int NI=B->CT->NI;
		int reg=WiselyFormations.CreateRegiment();
		WiselyFormations.Reg[reg]->AddSelectedToPolk(NI);
		/*for(int j=0;j<MaxBrig;j++)
		{
			Brigade* BR=&(CITY[NI].Brigs[j]);
			if(BR->Enabled&&BR->WarType!=0)
				WiselyFormations.AddAutoBrigade(BR);
		}
		*/
	}
}

void ShowAddInfo(int x,int y)
{
	for(int i=0;i<WiselyFormations.MaxAF;i++)
	{
		AutoForm* FF=WiselyFormations.AF[i];
		if(FF&&FF->BSelf->WarType!=0)
			FF->ShowInfo();
	}
}
CEXPORT int CreatePolk()
{
	return WiselyFormations.CreateRegiment();
}
CEXPORT Polk* GetPolkByIndex(int PIndex)
{
	return WiselyFormations.GetPolkByIndex(PIndex);
}

//====================================================================//
#else

void WiselyFormationsProcess()
{}
void AddBrigadeInWiselyFormations(Brigade* B)
{}
#endif COSSACKS2