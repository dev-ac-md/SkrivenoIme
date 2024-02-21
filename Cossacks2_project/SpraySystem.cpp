#ifdef _USE3D

#include "ddini.h"
#include "ResFile.h"
#include "FastDraw.h"
#include "mode.h"
#include "MapDiscr.h"
#include "GP_Draw.h"

#include "SpraySystem.h"

SpraySystem SpraySys;

// --- SpraySystem ---

bool SpraySystem::AddDust(int x, int y, byte r, byte g, byte b, byte a){
	int xx=x>>SquadSH;
	int yy=y>>SquadSH;
	int sid=xx+(yy<<SquadNSH);
	if(sid>=SquadN) return false;
	SpraySquad* SQD=Squads[sid];
	if(!SQD){
		SQD=(SpraySquad*)malloc(sizeof(SpraySquad));
		SQD->Init();
		Squads[sid]=SQD;
	}
	xx=(x-(xx<<SquadSH))>>SpraySH;
	yy=(y-(yy<<SquadSH))>>SpraySH;
	int cid=xx+(yy<<SprayNSH);
	SprayCell* SC=SQD->Cells+cid;	
	if(int(SC->A)+int(a)>255){
		SC->A=255;
	}else{
		SC->A+=a;
	}
	SC->R=r;
	SC->G=g;
	SC->B=b;
	return true;
};

void SpraySystem::Process(){
	SpraySquad** SQD=Squads;
	for(int i=0;i<SquadN;i++){
		if(*SQD){
			if(!(*SQD)->Process()){
				//free(*SQD);
				//*SQD=NULL;
			};
		}
		SQD++;
	}
};

extern RLCFont  YellowFont;
DWORD GetSpraySysDust(int x,int y,void* param){

	int xx=(mapx<<5)+(x<<10-Shifter);
	int yy=(mapy<<4)+(y<<9-Shifter);

	//int xx=x<<5;
	//int yy=y<<5;

	/*
	int S=3<<(5-Shifter);
	if(S>20)S=20;
	int vv=GetInterpFOW(x,y);
	if(vv<FMIN)return 255;
	vv=255-(vv-FMIN)/S;
	//int vv=GETF3D(int(param)+(x)+(y<<7));
	if(vv<0)vv=0;
	if(vv>200)vv=255;
	return vv;
	*/
	
	int vv=SpraySys.GetSprayDust(mapx+(x<<(5-Shifter)),mapy+(y<<(5-Shifter)));
	
	//if(x%4==1&&y%4==1){
		//char buf[256];
		//sprintf(buf,"%d,%d",xx,yy);
		//sprintf(buf,"%d",vv);	
		//sprintf(buf,"%d,%d",mapx<<5,mapy<<5);
		//ShowString(x<<5,y<<5,buf,&YellowFont);
		//ShowString(200,200,buf,&YellowFont);
		//ShowString(x<<5,y<<5,buf,&YellowFont);
	//}	

	return (vv<<24)+0xFF0000;
};

int SpraySystem::GetSprayDust(int x,int y){
	x<<=5;
	y<<=5;
	//x-=50;
	//y-=50;
	//int R=255*8/sqrt(64+x*x+y*y);
	//return R;
	int xx=x>>SquadSH;
	int yy=y>>SquadSH;
	int sid=xx+(yy<<SquadNSH);
	if(sid>=SquadN) return 0;
	SpraySquad* SQD=Squads[sid];
	if(!SQD) return 0;
	xx=(x-(xx<<SquadSH))>>SpraySH;
	yy=(y-(yy<<SquadSH))>>SpraySH;
	int cid=xx+(yy<<SprayNSH);
	SprayCell* SC=SQD->Cells+cid;
	
	//char buf[256];
	//sprintf(buf,"%d",SC->a);	
	//ShowString(x-(mapx<<5),y-(mapy<<4),buf,&YellowFont);

	return SC->a;
};

extern int RealLx;
extern int RealLy;

void SpraySystem::Draw(){
	if(!DXSquare){
		DXSquare=new SquareObject;
	};
	int Nx=RealLx>>8;
	if(Nx<<8!=RealLx)Nx++;
	int Ny=RealLy>>8;
	if(Ny<<8!=RealLy)Ny++;
	bool first=1;
	for(int iy=0;iy<Ny;iy++){
		for(int ix=0;ix<Nx;ix++){

			int ofs=(ix<<3)+(iy<<11);
			if(DXSquare->CreateRGBA(ix<<8,iy<<8,&GetSpraySysDust,ix<<3,iy<<4,qt_32,qt_16,(void*)ofs,0,0,1,1)){
				VertexTnL* VER=(VertexTnL*)DXSquare->BM->getVertexData();
				int NV=DXSquare->BM->getNVert();
				
				
				float T=GetTickCount()/14000.0;
				for(int i=0;i<NV;i++){
					float px=(float(mapx<<(Shifter))+VER->x)/1200.0;
					float py=(float(mapy<<(Shifter-1))+VER->y)/1200.0;
					VER->u=px+px+sin(px+T);
					VER->v=py+py+sin(py*1.1+T*1.213);
					//VER->u2=px+px+cos(px*1.61+T*0.94);
					//VER->v2=py+py+sin(py*1.12+T*1.113);
					VER++;
				};
				

				if(first){
					//if(GetKeyState(VK_MENU)&0x8000)FogOfWar->BM->setShader(IRS->GetShaderID("wfog"));
					//else FogOfWar->BM->setShader(IRS->GetShaderID("fow"));
					DXSquare->BM->setShader(IRS->GetShaderID("dust"));
					DXSquare->BM->setTexture(IRS->GetTextureID("temp\\tx1.tga"));
					//FogOfWar->BM->setTexture(IRS->GetTextureID("temp\\tx1.tga"),1);
					first=0;
				};
				IRS->Draw(*DXSquare->BM);
/*
#ifdef COMLEXFOG
				VER=(VertexTnL*)FogOfWar->BM->getVertexData();
				for(int i=0;i<NV;i++){
					float px=(float(mapx<<(Shifter))+VER->x)/1200.0;
					float py=(float(mapy<<(Shifter-1))+VER->y)/1200.0;
					VER->u=px+px+sin(-T);
					VER->v=py+py+sin(-T*1.213);
					//VER->u2=px+px+cos(px*1.61+T*0.94);
					//VER->v2=py+py+sin(py*1.12+T*1.113);
					VER++;
				};
				IRS->Draw(*FogOfWar->BM);
#endif
*/
			};
		};
	};

	
	/*
	Nx=RealLx>>5;
	if(Nx<<5!=RealLx)Nx++;
	Ny=RealLy>>5;
	if(Ny<<5!=RealLy)Ny++;
	
	for(iy=0;iy<Ny;iy++){
		for(int ix=0;ix<Nx;ix++)if(((ix%1)==0)&&((iy%1)==0)){					
			
			int xx=(mapx<<5)+(ix<<(10-Shifter));
			int yy=(mapy<<5)+(iy<<(10-Shifter));

			int vv=SpraySys.GetSprayDust(ix,iy);
			
			char buf[256];
			//sprintf(buf,"%d,%d",xx,yy);
			sprintf(buf,"%d",vv);	
			//sprintf(buf,"%d,%d",mapx<<5,mapy<<5);
			ShowString(ix<<5,iy<<4,buf,&YellowFont);
			//ShowString(200,200,buf,&YellowFont);
			//ShowString(x<<5,y<<5,buf,&YellowFont);
			
		}
	}
	*/
	
};

// --- SpraySquad ---

void SpraySquad::Init(){
	memset(Cells,0,sizeof(Cells));
};

bool SpraySquad::Process(){
	bool empty=true;
	for(int i=0;i<SprayN;i++){
		if(Cells[i].Process()){
			empty=false;
		};
	}
	return !empty;
};

// --- SprayCell ---

bool SprayCell::Process(){		
	if(!(A||a)) return false;
	int aa=a;
	if(A>a){
		int DA=((A-a)>>3);
		if(!DA)DA=1;
		aa+=DA;
	}else{
		int DA=((a-A)>>3);
		if(!DA)DA=1;
		aa-=DA;
	}
	if(aa>=0)a=aa;
		else a=0;
	int AA=A;
	int DA=(A>>4);
	if(!DA)DA=1;
	AA-=DA;
	if(AA<0)AA=0;
	A=AA;
	//	else return false;
	if(A<1&&a<1) return false;
		else return true;
}

// --- ---

void UnLoadSpraySystem(){
	SpraySys.Free();
};

void InitSpraySystem(){
	SpraySys.Init();
};
int Norma(int,int);
void AddOneDust(int x, int y){
	if(!SpraySys.Squads) SpraySys.Init();
	int xx0=x>>5;
	int yy0=y>>5;
	for(int ix=-3;ix<=3;ix++){
		for(int iy=-3;iy<=3;iy++){
			int x0=(xx0+ix)<<5;
			int y0=(yy0+iy)<<5;
			int R=Norma(x0-x,y0-y);
			int DP=10*600/(600+R*R);
			SpraySys.AddDust(x0,y0,255,255,255,DP);
		};
	};
	/*
	SpraySys.AddDust(x,y,255,255,255,25);
	
	SpraySys.AddDust(x-32,y,255,255,255,15);
	SpraySys.AddDust(x+32,y,255,255,255,15);
	SpraySys.AddDust(x,y-32,255,255,255,15);
	SpraySys.AddDust(x,y+32,255,255,255,15);
	*/

	//SpraySys.AddDust(x-32,y-32,255,255,255,25);
	//SpraySys.AddDust(x+32,y+32,255,255,255,25);
	//SpraySys.AddDust(x+32,y-32,255,255,255,25);
	//SpraySys.AddDust(x-32,y+32,255,255,255,25);
}

void DrawDust(){
	SpraySys.Draw();
}

void ProcessDust(){
	SpraySys.Process();
}

#endif //_USE3D