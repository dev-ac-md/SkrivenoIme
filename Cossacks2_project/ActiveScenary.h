struct GAMEOBJ{
	word Index;
	word Serial;
	int  Type;
};
class UnitsGroup;
struct GrpOrder;
typedef void GrpLink(UnitsGroup* UG);
struct GrpOrder{
	GrpOrder* Next;
	GrpLink* Link;
	void* Data;
	int DataSize;
	DWORD ID;
};
void CheckDynamicalPtr(void* ptr);
class UnitsGroup;
class UnitsGroup{
public:
	word* IDS;
	word* SNS;
	int N;
	int Max;

	byte NI;
	byte NMASK;
	int Index;
	GrpOrder* GOrder;

	//-----------debug---------
	char* file;
	int   Line;
	//-------------------------

	GrpOrder* CreateOrder(byte Type);
	void DeleteLastOrder();
	void ClearOrders();
	void Allocate(int n){
		CheckDynamicalPtr(IDS);
		CheckDynamicalPtr(SNS);
		if(n>Max){
			Max=n+32;
			IDS=(word*)realloc(IDS,Max<<1);
			SNS=(word*)realloc(SNS,Max<<1);
		};
		CheckDynamicalPtr(IDS);
		CheckDynamicalPtr(SNS);
	};
	void AddNewUnit(OneObject* OB);
	void RemoveUnit(OneObject* OB);
	void CopyUnits(UnitsGroup* Dest,int start,int Nc,bool add,bool remove){
		CheckDynamicalPtr(IDS);
		CheckDynamicalPtr(SNS);
		if(start>=N)start=N-1;
		if(start<0)start=0;
		if(start+Nc>N)Nc=N-start;
		if(add){
			Dest->Allocate(Dest->N+Nc);
			memcpy(Dest->IDS+Dest->N,IDS+start,Nc<<1);
			memcpy(Dest->SNS+Dest->N,SNS+start,Nc<<1);
			Dest->N+=Nc;
		}else{
			Dest->Allocate(Nc);
			memcpy(Dest->IDS,IDS+start,Nc<<1);
			memcpy(Dest->SNS,SNS+start,Nc<<1);
			Dest->N=Nc;
		};
		if(remove&&Nc){
			int ncp=(N-start-Nc)<<1;
			memcpy(IDS+start,IDS+start+Nc,ncp);
			memcpy(SNS+start,SNS+start+Nc,ncp);
			N-=Nc;
		};
		CheckDynamicalPtr(IDS);
		CheckDynamicalPtr(SNS);
	};
	UnitsGroup(){
		memset(this,0,sizeof *this);
	};
	~UnitsGroup(){
		ClearOrders();
		if(IDS)free(IDS);
		if(SNS)free(SNS);
		memset(this,0,sizeof *this);
	};
};
struct UnitsPosition{
	word* Type;
	int*  coor;
	int N;
};
struct ZonesGroup{
	word* ZoneID;
	int N;
};
typedef void StdVoid();
struct GTimer{
	int Time;
	bool Used;
	bool First;
};
struct LightSpot{
	int x,y,Type;
};
class ScenaryInterface{
public:
	void** SaveZone;
	int *  SaveSize;
	int    NSaves;
	int    MaxSaves;
	HINSTANCE hLib;
	char*  DLLName;

	UnitsGroup* UGRP;
	int    NUGRP;
	int    MaxUGRP;
	
	UnitsPosition* UPOS;
	int    NUPOS;
	int    MaxUPOS;

	ZonesGroup* ZGRP;
	int    NZGRP;
	int    MaxZGRP;

	char** Messages;
	int    NMess;
	int    MaxMess;

	char** Sounds;
	int    NSnd;
	int    MaxSnds;

	int NErrors;

	//char*  MissText;
	//int    TextSize;

	int NPages;
	int MaxPages;
	char** Page;
	int*   PageSize;
	char** PageID;
	char** PageBMP;

	bool   TextDisable[52];
	
	bool   StandartVictory;
	bool   Victory;
	char*  VictoryText;

	bool   LooseGame;
	char*  LooseText;

	char*  CentralText;
	int CTextTime;

	GTimer TIME[32];
	word   TRIGGER[512];
	LightSpot LSpot[64];
	StdVoid* ScenaryHandler;
	ScenaryInterface();
	~ScenaryInterface();
	void Load(char* Name,char* Text);
	void UnLoading();
};
extern ScenaryInterface SCENINF;
class SingleMission{
public:
	char* ID;
	char* DLLPath;
	char* MapName;
	char* Name;
	char* Description;
	int NIntro;
	char** Intro;
};
class MissPack{
public:
	SingleMission* MISS;
	int NMiss;
	int MaxMiss;
	int CurrentMission;
	int* SingleMS;
	int MSMiss;
	void LoadMissions();
	MissPack();
	~MissPack();
};

class OneBattle{
public:
	char* ID;
	char* Map;
	char* Text;
	char* Brief;
	char* BigMap;
	char* MiniMap;
	char* BigHeader;
	char* SmallHeader;
	char* Date;
	char* RedUnits;
	char* BlueUnits;
	int NHints;
	char** Hints;
	int* Coor;
};
class OneWar{
public:
	char* Name;
	char* Date;
	char* Text;
	int NBatles;
	int* BattleList;
};
class WarPack{
public:
	int NWars;
	OneWar* Wars;
	int NBattles;
	OneBattle* Battles;
	WarPack();
	~WarPack();
};
extern WarPack WARS;
extern MissPack MISSLIST;
struct SingleCampagin{
	char* CampMessage;
	char* CampText;
	char* CampBmp;
	int NMiss;
	int* Miss;
	DWORD* OpenIndex;
};
class CampaginPack{
public:
	int NCamp;
	SingleCampagin* SCamp;
	CampaginPack();
	~CampaginPack();
};
extern CampaginPack CAMPAGINS;
extern byte CurAINation;
extern City* CCIT;
extern Nation* CNAT;
extern bool AiIsRunNow;
extern int CurrentCampagin;
extern int CurrentMission;
CEXPORT void LoadPlayerData();
CEXPORT void SavePlayerData();