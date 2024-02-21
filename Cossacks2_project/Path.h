class MotionBrush{
public:
	int* Data;
	int Lx,Ly;
	int Dx;
	int Dy;
	MotionBrush();
	~MotionBrush();
	void CreateRoundBrush(int d);
	void CreateSquareBrush(int L);
};
class MotionField{
public:
	//byte* MapH;//(x,y)-bit matrix(1point=16x12 pix)
	byte* MapV;//(y,x)-T bit matrix
	//----Methods-----
	void Allocate();
	void FreeAlloc();
	void ClearMaps();
	void BSetPt(int x,int y);
	void BClrPt(int x,int y);
	void BXorPt(int x,int y);
	void BSetBar(int x,int y,int Lx);
	void BClrBar(int x,int y,int Lx);
	void BXorBar(int x,int y,int Lx);
	void BSetSQ(int x,int y,int Lx,int Ly);
	void BClrSQ(int x,int y,int Lx,int Ly);
	void BXorSQ(int x,int y,int Lx,int Ly);
	int  CheckPt(int x,int y);
	int  CheckHLine(int x,int y,int Lx);
	int  CheckVLine(int x,int y,int Lx);
	bool CheckBar(int x,int y,int Lx,int Ly);
	void FillNewCirc(int x,int y,int Lx,int Ly);
};

#ifdef HASH_TOP
	#define NMFIELDS 3	
#else //HASH_TOP	
	#define NMFIELDS 2
#endif //HASH_TOP

extern MotionField MFIELDS[NMFIELDS];//0-Land,1-Water
