// BrigadeAI.h: interface for the BrigadeAI class.
//
//////////////////////////////////////////////////////////////////////
#ifdef COSSACKS2
#if !defined(AFX_BRIGADEAI_H__1A25F3B9_A008_46AD_A880_B90797A576B7__INCLUDED_)
#define AFX_BRIGADEAI_H__1A25F3B9_A008_46AD_A880_B90797A576B7__INCLUDED_

#define DIALOGS_API __declspec(dllexport)

//====================================================================//
int GetDamage(word ShooterID,word VictimID,int AttType);
word GetDir(int dx,int dy);
void MakeReformation(byte NI,word BrigadeID,byte FormType);
class DString;

//====================================================================//
class Atom
{
public:
	Atom();
	Atom(Brigade* Br);
	virtual ~Atom();
	int x;
	int y;
	int Force;
	byte Dir;
	int CheckTime;
	int CheckTimeInterval;
	Brigade* BSelf;
	int UnitsAmo;
	int FireReady;
	int FireTimeDelayLeft;
	int Type;
	int Speed;
	int Tired;
	byte NI;
	void virtual UpdateParams();
	bool IsStand;
};
//====================================================================//
class AutoForm;
struct sector
{
	int dest;
	Atom* A;
	AutoForm* AF;
};
struct Direction
{
	sector Sectors[32];
	int SectorsAmo;
	int SumDang;
	int Dang;
	int FireDang;
	int SumFireDang;
	byte Dir;
};
//====================================================================//
class Rules;
class Rule;
//
class AutoForm : public Atom
{
public:
	AutoForm();
	AutoForm(Brigade* B);
	virtual ~AutoForm();

	int Index;
	int RegimentIndex;
	int NextProcesedTime;
	int TimeInterval;
	int FormationTypeIndex;
	void Process();
	void MoveTo(int x, int y);
	void MoveTo(int x, int y, byte dir);

	void SimpleMoveTo(int x, int y);
	void SimpleMoveTo(int x, int y, byte dir);

	
	Direction EnmAng[16];
	Direction FriendAng[16];
	void ClearEnmAng();
	void AddToEnmAng(Atom* A);
	void ClearFriendAng();
	void AddToFriendAng(AutoForm* AF);
	
	int LastTimeCheckXY;
	int AimX;
	int AimY;
	byte AimDir;
	int mtX;
	int mtY;
	byte mtDir;
	int mtXr;
	int mtYr;
	int mtXl;
	int mtYl;

	int GetDistToAim();
	void MoveToAim();
	
	//Actions
	void StepForward();
	void StepBack();
	void MoveBack();
	void TurnLeft();
	void TurnRight();
	void EnableFire();
	void FireNow();
	void DisableFire();
	void SetReadyToFire();
	void SetNotReadyToFire();
	void FormateToLine();
	void FormateToSquare();
	void FormateToKare();
	void Stop();
	void Castle();
	// MainTarget Actions
	void MoveToMT();
	void TurnToMT();
	void FireToMT();
	
	int GetMainTargetAngl();
	Atom* GetMaxDanderAtom();
	Atom* GetNearestAtom();
	//
	void FormateTo012(int Type);
	//Situation
	int IsFireReady(Brigade* Br);
	int GetDistTo(Brigade* Br);
	int GetMoral(Brigade* Br);
	int GetNTo30Limit(Brigade* Br,int* VictimAmo);
	int GetDieAmount(Brigade* Br,int* ReadyShuters);
	int CanIKill(Brigade* Br);
	//
	int GetSituationByIndex(int I);
	//
	byte GetMaxDangDirections();
	void TurnTo(byte dir);
	//
	//Self
	int Moral();
	int GetFireReady();
	int UnitsAmount();
	int GetTired();
	//(MT)//
	Atom* MainTarget;
	//
	int DistToMT();
	int CanIKillMT();
	int MoralMT();
	int MoralMTAfterFire();
	int KillAmoMT();
	int FireFeadyMT();
	int UnitsAmountMT();
	int TiredMT();
	int AngToMT();
	//int DistToSecondaryTarget();
	int SumDangAndForce();
	bool HaveReserv();
	//
	//
  
	bool MessageArr[16];
	//bool OrderArr[16];
	int ActionsArr[32];
	int GetMaxActions();
	void ExecuteActions(int ActionsID);
	int TimeToDoActions[16];
		/*
		0 - MoveToMT
		1 - TurnToMT
		2 - MoveBack
		3 - SetReadyToFire
		4 - EnableFire
		5 - DisableFire
		6 - Stop
		7 - FormateToLine
		8 - FormateToSquare
		9 - FormateToKare
		10- FindPlaceInFront();
		11- TurnToMaxDang();
		12-MoveToAim
		*/
	bool StateArr[32];
	bool static NonStateArr[32][32];
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
		15 - EnableFire
		16 - DisableFire
		*/
	bool GetState(int Index);
	void SetState(int Index, bool Vle);

	int RulesID;
	Rules* RulesList;
	void ProcessRules();
	int NextTimeProcessRules;

	int GetSizeFront();
	int GetSizeBack();
	int GetSizeLeft();
	int GetSizeRight();

	void MoveToFrontOf(AutoForm* A);
	void MoveToBackOf(AutoForm* A);
	void MoveToLeftOf(AutoForm* A);
	void MoveToRightOf(AutoForm* A);

	void FindPlaceInFront();
	void FindPlaceInReserv();
	void TurnToMaxDang();
	
	//TEMP
	AutoForm* GetFriendInFront();
	AutoForm* GetFriendInBack();
	AutoForm* GetFriendInLeft();
	AutoForm* GetFriendInRight();
	int GetMaxDangDirectionMinus();
	int GetNearestFriendDirection();
	bool IsInBatleFormation();
	bool IsInFront();
	bool IsNearAim();
	int ForceDifference();
	int FireForceDifference();
	int Encircle();

	//UpLevel
	//int GeneralCommand();
	int DistToMainTarget;

	int DistInFront();
	void KeepDistInFront();
	
	int DistInReserv();
	void KeepDistInReserv();

	void Fire();
	int GetStand();
	bool IsEnemOnWay();
	int GetNGrenades();
	void ThrowGrenade();
	
	void ShowInfo();//Debug
private:
	void SetNoSearchVictim(bool vle);
	void AttackBrigade(Brigade* B);
	void SetFireState(Brigade* B, bool State);
	void GetCoordToMTWay(int &x, int &y, AutoForm* FR);
	AutoForm* FindNearestFriendInMTDirection();
	bool CorrectDestPoint(int x, int y, byte dir);
	bool Movement;
	
	
};
//====================================================================//
struct Situation
{
	byte CompareID;
	int FunctionID1;
	int Value1;
	int FunctionID2;
	int Value2;
};
struct Action
{
	int ActionID;
	int Weight;
	int Value;
};
//====================================================================//
class Rule
{
public:
	Rule();
	virtual ~Rule();
	Situation Situations[16];
	Action Actions[8];
	int Weight;
	int Type;
	Rule* Next;
	bool CheckRule(AutoForm *A);
	bool CheckSituation(AutoForm *A, int i);
	void FillSituations(DString *s);
	void FillActions(DString *s);
private:
	void SetSituation(char* p1,char *iff, char *p2, Situation *sit);
	int GetFunctionID(char* p);
	int GetCompareID(char* iff);
	int GetActionID(char* aa);
	int GetStateID(char* aa);
};
//====================================================================//
class Rules
{
public:
	Rules();
	virtual ~Rules();
	int UnitType;
	Rule* First;
	Rule* Last;
	void VerifySituation(AutoForm *A);
	////bool Compare(Situation* s1, Situation* s2);
	void LoadRules(DString *d);
private:
	Rule* Pointer;
};
//====================================================================//
class DIALOGS_API Polk
{
public:
	Polk();
	virtual ~Polk();
	void virtual MovePolkTo(int x, int y, int Dir=0);
	int  virtual GetEnemyForce();
	int  virtual GetPolkIndex();
	bool virtual ReSetFormation(int FormIndex);
	void virtual AddSelectedToPolk(byte Nat);
	int  virtual GetFormationIndex(int UnitType);
};
//====================================================================//
class Regiment : public Polk
{
public:
	Regiment();
	Regiment(int I, AutoForm** AFL, int* MaxAFL);
	~Regiment();
	void MovePolkTo(int x, int y, int Dir=0);
	int EnemyForce;
	int GetEnemyForce();
	int GetPolkIndex();

	int Index;
	int DistX;
	int DistY;
	int DistDir;
	int Radius;
	int SearchEnemyRadius;
	bool AddBrigade(Brigade* B);
	void AddSelectedToPolk(byte Nat);
	bool ReSetFormation(int FromIndex);
	AutoForm* GetAutoForm(int UnitType);
	int GetFormationIndex(int UnitType);
	void Process();
	AutoForm** AFList;
	int* MaxAFList;
	int AddFormation(Brigade* B);
	void FillTable();
	void AnalizeTable();
	int TableFillTime;
	AutoForm* P[64];
	Atom* E[64];
	int MaxP;
	int MaxE;
	int TDist[64][64];
	bool TVisible[64][64];
	bool TNearestP[64][64];
	bool TNearestE[64][64];
	bool Chase;
	bool IsChase();
	void SetChaseOn();
	void SetChaseOff();
private:
	int AddE(Atom* A);
	void AddFillEn(AutoForm* AF);
	int GetActualDist();
	int GetActualDistOnWay();
	int GetActualDistInAim();
	void SetActualDist();
	void SetDistIfEnmAlone();
	void FillDestPoints();
};
//====================================================================//
class BrigadeAI  
{
public:
	BrigadeAI();
	virtual ~BrigadeAI();
	void Process();
	int static AddAutoBrigade(Brigade* B);
	bool FirstStart;
	static AutoForm* AF[256];
	static int MaxAF;
	static Regiment* Reg[128];
	static int MaxReg;
	static int CreateRegiment();
	Polk* GetPolkByIndex(int PIndex);
	
	static Rules AllRules[32];
	void LoadAllRules(char* p);

	static Atom* AllForm[8][256];
	static int AllFormAmo[8];
	static int AllFormFillTime;
private:
	//void FillNeighbors();
	void FillEnmAng();
	void FillFriendAng();
	int FillNeighborsTime;
	void FillAllForm();
};
//====================================================================//
#endif // !defined(AFX_BRIGADEAI_H__1A25F3B9_A008_46AD_A880_B90797A576B7__INCLUDED_)
#endif //COSSACKS2
