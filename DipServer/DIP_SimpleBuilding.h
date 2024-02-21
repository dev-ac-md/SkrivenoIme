struct DipComParams{
	char MessageID[ 32];
	char HintID   [ 32];
	char ProcessID[ 32];
	int  Cost     [  6];
	char ComID    [ 16];
	char Params   [256];
};
class DIP_SimpleBuilding:public BasicDiploRelation{
public:
	DipComParams* DIPPAR;
	int NDipPar;
	int ProcessID;
	GAMEOBJ DestGroup;
	GAMEOBJ BigZone;
	GAMEOBJ VeryBigZone;
	//dance group
	short   ActorsType;
	short   NStartActors;
	GAMEOBJ Actors;
	GAMEOBJ DanceZone;
	int DanceStage;
	//repair
	GAMEOBJ Repairers;
	GAMEOBJ Defenders;
	GAMEOBJ ComeInside;
#ifdef AC_ADDON
	//addon
	GAMEOBJ Friends;
	bool EasyTalk;		// no need diplomats (need anyone unit)
#endif AC_ADDON
	short RepType;
	short DefType;
	short MaxRep;
	short MaxDef;
	short AddDefAttack;
	short AddDefShield;

	//---------
	char CongratID[32];		// congratulation text
	char FirstMeetID[32];
	char ReSellID[32];
	char TellNo[32];
	char gpPix[32];
	int ReSellCost[6];
	int gpidx;
	static int CurIndex;
	union{
		struct ProdUnits{
			int Stage;
			int StartTime;
			GAMEOBJ UnitType;
			int Amount;
#ifdef AC_ADDON
			byte Speed;
#endif AC_ADDON
		}Prod;
	}info;
	bool DipProposition;
	byte CounsilNI;
	bool UseLikeSkarb;
	bool AlreadyUsedLikeSkarb;
	int StartRelTime;
	virtual void InitThisTribe(char* Preffix,char* FirstGroupName,char* CommonID);
	virtual void InitThisTribe2(char* FileID, char* Dip, char* GDance, char* ZDance);
	virtual int GetAmountOfPropositions();
	virtual DWORD GetPossibility(int Index,char* Message,char* Hint);
	virtual void PerformPossibility(DWORD Code);
	virtual void ProcessTribe();
	virtual void PerformCommand(char* Data,int size);
	virtual int GetSaveData(byte* buf,bool write);
	virtual void SetSaveData(byte* buf,int size);
	virtual void Init();
	virtual void Free();
	void PerformProduceUnitsCommend();
	void PerformRazvedka();
	void PerformFiring();
	void PerformStorms();
	void PerformKillers();
	void PerformTomahawks();
	void CreateAbilityPage();
	void ProcessDance();
	void ShowNoMoney();
};