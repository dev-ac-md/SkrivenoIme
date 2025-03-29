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
#include <time.h>
#include "Nucl.h"
#include "Megapolis.h"
#include "Fonts.h"
#include "recorder.h"
int HintTime=0;

//Duration in ms for various messages and hints
extern const int kChatMessageDisplayTime = 10000;
extern const int kImportantMessageDisplayTime = 5000;
extern const int kSystemMessageDisplayTime = 3000;
extern const int kMinorMessageDisplayTime = 1500;


extern bool RecordMode;
extern byte PlayGameMode;
extern byte PlayGameMode;
extern byte CHOPT;
extern char LASTCHATSTR[512];

//Ensures that ProcessHints() does not run more frequently than every ... ms
const unsigned int kMinimalProcessingInterval = 100;

unsigned long last_hint_processing_time = 0;
//Primary UI hint field
char primary_hint[256];
char primary_hint_lo[256];
static unsigned int primary_hint_time;

//Maximum number of simoultaneous shown chat messages or hints
const int kMaxHintCount = 25;
//Additional secondary hints and chat messages
char additional_hints[kMaxHintCount][256];
static unsigned int  additional_hints_times[kMaxHintCount];

//Hint or chat messages design
//0: normal; 16-23: national color border; 32: red
byte HintOpt[kMaxHintCount];

//static int HintTime1;
void ResFon(int x,int y,int Lx,int Ly);
void ScrCopy(int x,int y,int Lx,int Ly);
#define NHINT 4
char HintStr[256];
char HintStrLo[256];
char HintStr1[NHINT][256];
//byte HintOpt[NHINT];//(0)-usual,(16+x)-in national bar,32-red 
static int  HintTime1[NHINT];
int HintX;
int HintY;
int HintLx;
int HintLy;
void SetupHint(){
	HintY=smapy+smaply*32-20;
	HintX=smapx;
	HintLx=720;
	HintLy=16;
	HintTime=0;
};
extern byte PlayGameMode;
void ShowHint(){
	if(PlayGameMode==1)return;
	//ResFon(HintX,HintY,HintLx,HintLy);
	if(HintTime){
		ShowString(HintX+2,HintY+2,HintStr,&BlackFont);
		ShowString(HintX,HintY,HintStr,&WhiteFont);

		ShowString(HintX+2,HintY+20+2,HintStrLo,&BlackFont);
		ShowString(HintX,HintY+20,HintStrLo,&WhiteFont);
	};
	int yy=HintY-20;
	for(int i=0;i<NHINT;i++){
		if(HintTime1[i]){
			ShowString(HintX+2,yy+2,HintStr1[i],&BlackFont);
			byte opt=HintOpt[i];
			byte opp=opt>>4;
			if(opp==2){
				int tt=(GetTickCount()/300)&1;
				if(tt)ShowString(HintX,yy,HintStr1[i],&WhiteFont);
				else ShowString(HintX,yy,HintStr1[i],&RedFont);
			}else
			if(opp==1){
				ShowString(HintX,yy,HintStr1[i],&WhiteFont);
				Xbar(HintX-2,yy,GetRLCStrWidth(HintStr1[i],&WhiteFont)+5,GetRLCHeight(WhiteFont.RLC,'y')+1,0xD0+((opt&7)<<2));
			}else{
				ShowString(HintX,yy,HintStr1[i],&WhiteFont);
			};
		};
		yy-=20;
	};
	//ScrCopy(HintX,HintY,HintLx,HintLy);
};
bool WasLoHint=0;
void ClearHints(){
	for(int i=0;i<NHINT;i++)HintStr1[i][0]=0;
	WasLoHint=HintStrLo[0]!=0;
	HintStrLo[0]=0;
};
void HideHint(){
	//ResFon(HintX,HintY,HintLx,HintLy);
	//ScrCopy(HintX,HintY,HintLx,HintLy);
};
CEXPORT
void AssignHint(char* s,int time){
	strcpy(HintStr,s);
	//ShowHint();
	HintTime=time;
	WasLoHint=HintStrLo[0]!=0;
	HintStrLo[0]=0;
};
void AssignHintLo(char* s,int time){
	strcpy(HintStrLo,s);
	//ShowHint();
	HintTime=time;
};

void CreateTimedHint(char* s, int time)
{
    if (!strcmp(s, additional_hints[0]))
    {//Same text already on display, reset timer and exit
        additional_hints_times[0] = time;
        HintOpt[0] = 0;
        return;
    }

    for (int i = kMaxHintCount - 1; i > 0; i--)
    {//Shift existing hints and make room for one more
        strcpy(additional_hints[i], additional_hints[i - 1]);
        additional_hints_times[i] = additional_hints_times[i - 1];
        HintOpt[i] = HintOpt[i - 1];
    }

    strcpy(additional_hints[0], s);
    additional_hints_times[0] = time;
    HintOpt[0] = 0;
}

//Adds secondary hint or chat message with special design option for [time] ms
void CreateTimedHintEx(char* s, int time, byte opt)
{
    if (opt >= 16)
    {//National color masking requested
        if (RecordMode && !PlayGameMode)
        {//Do not assign option bit when watching a replay
            strcpy(LASTCHATSTR, s);
            CHOPT = opt;
        }
    }

    if (!strcmp(s, additional_hints[0]))
    {//Same text already on display, reset timer and exit
        additional_hints_times[0] = time;
        HintOpt[0] = opt;
        return;
    }

    for (int i = kMaxHintCount - 1; i > 0; i--)
    {//Shift existing hints and make room for one more
        strcpy(additional_hints[i], additional_hints[i - 1]);
        additional_hints_times[i] = additional_hints_times[i - 1];
        HintOpt[i] = HintOpt[i - 1];

    }

    strcpy(additional_hints[0], s);
    additional_hints_times[0] = time;
    HintOpt[0] = opt;
}
CEXPORT
void AssignHint1(char* s,int time){
	if(!strcmp(s,HintStr1[0])){
		HintTime1[0]=time;
		HintOpt[0]=0;
		return;
	};
	for(int i=NHINT-1;i>0;i--){
		strcpy(HintStr1[i],HintStr1[i-1]);
		HintTime1[i]=HintTime1[i-1];
		HintOpt[i]=HintOpt[i-1];

	};
	strcpy(HintStr1[0],s);
	HintTime1[0]=time;
	HintOpt[0]=0;
};
extern bool RecordMode;
extern byte PlayGameMode;
extern char LASTCHATSTR[512];
extern byte CHOPT;
CEXPORT
void AssignHint1(char* s,int time,byte opt){
	if(opt>=16){
		if(RecordMode&&!PlayGameMode){
			strcpy(LASTCHATSTR,s);
			CHOPT=opt;
		};
	};
	if(!strcmp(s,HintStr1[0])){
		HintTime1[0]=time;
		HintOpt[0]=opt;
		return;
	};
	for(int i=NHINT-1;i>0;i--){
		strcpy(HintStr1[i],HintStr1[i-1]);
		HintTime1[i]=HintTime1[i-1];
		HintOpt[i]=HintOpt[i-1];

	};
	strcpy(HintStr1[0],s);
	HintTime1[0]=time;
	HintOpt[0]=opt;
};
void GetChar(GeneralObject* GO,char* s){
	Visuals* VS=(Visuals*)GO;
	strcpy(s,VS->Message);
};
void GetMonsterCapsString(char* st);
void ProcessHint(){
	ShowHint();
	if(HintTime){	
		HintTime--;
	};
	for(int i=0;i<NHINT;i++){
		if(HintTime1[i]){
			HintTime1[i]--;
		};
	};
};
