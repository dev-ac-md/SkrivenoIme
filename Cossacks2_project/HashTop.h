
// Ofs=Dst*NAreas+Src Key0=Ofs&1023 Key1=Ofs>>10
struct HashTop{	
	unsigned K1:16;		// Key1 to search
	unsigned ML:13;		// MotionLinks
	unsigned LD:13;		// LinkDist
	unsigned Prio:6;	// priory

	HashTop(){Clear();}
	void Clear(){K1=0xFFFF;}
};

#define NKey0 1024
#define NKey1 10
#define NKey NKey0*NKey1

class HashTopTable{
public:
	byte TopType;
	//HashTop* Key0[NKey0];
	HashTop Key1[NKey];
	//int NKey1;

	void SetUp(byte LockType) { TopType=LockType; TopRef=znew(word,TopLx*TopLx); }
	void Free() { free(TopRef); TopRef=NULL; }

	void Clear(){for(int i=0;i<NKey;i++) Key1[i].Clear();}
	HashTop* GetHashTop(int Ofs);
	bool CalculateWay(int Ofs, HashTop* HT);

	word* TopRef;
	Area* TopMap;
	int   NAreas;
	int MaxArea;
	
	void CreateAreas();
	bool AddArea(short x,short y,byte Sliv);
	void AddLink(int N1,int N2);
	void EraseAreas();
};

extern HashTopTable HashTable[NMFIELDS];

void PreCreateTopLinks();
bool ProcessMessages();
extern int MAXSPR;
