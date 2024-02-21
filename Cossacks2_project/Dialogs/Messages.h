int ProcessMultilineQuestion(int Nx,char* Bmp1,byte or1,char* Text1,char* Quest){
	int Lx=Nx<<6;
	DialogsSystem GMM((RealLx-Lx)>>1,80);
	SQPicture SPU;
	SPU.SetPixelData(NULL);
	LocalGP CPIC1;
	int pid1=-1;
	int pid2=-1;
	char cc1[100];
	if(or1>2)or1=0;
	if(strcmp(Bmp1,"NONE")){
		sscanf(Bmp1,"%s%d",cc1,&pid1);
		if(pid1!=-1){
			CPIC1.Load(cc1);
		};
	};

	ResFile RF=RRewrite("tmp1.txt");
	RBlockWrite(RF,Text1,strlen(Text1));
	RClose(RF);

	// Fonts
	RLCFont* FWindow = &fon16y5;
	RLCFont* FButton = &fon18y5;
	RLCFont* FWhite = &fon18w;
	RLCFont* FYellow = &fon18y3;
	RLCFont* FText = &fon18y3;
	RLCFont* FHint = &fon13y2;

	/*
	LocalGP LGP;
	LocalGP FONT("rom11");
	LocalGP FONT1("rom11r");
	LocalGP FONTB("vinBig");
	LocalGP FONTRB("vinBig");

	RLCFont fntb(FONTB.GPID);
	RLCFont fntrb(FONTRB.GPID);
	RLCFont f16x16w(FONT.GPID);
	RLCFont f16x16g(FONT.GPID);
	f16x16w.SetRedColor();
	fntrb.SetRedColor();
	*/

	int y0=16;
	int LX1=0;
	int LY1=0;
	int x0=16;
	int plx=0;
	if(pid1!=-1){
		LX1=GPS.GetGPWidth(CPIC1.GPID,pid1);
		LY1=GPS.GetGPHeight(CPIC1.GPID,pid1)+16;
		if(or1==1){
			GMM.addGPPicture(NULL,x0,y0,CPIC1.GPID,pid1);
			x0+=LX1;
			plx=LX1;
		}else if(or1==2){
			GMM.addGPPicture(NULL,Lx-16-LX1,y0,CPIC1.GPID,pid1);
			plx=LX1;
		}else{
			GMM.addGPPicture(NULL,(Lx-LX1)>>1,y0,CPIC1.GPID,pid1);
			y0+=LY1+8;
		};
	};
	TextViewer* TV1;
	
	TV1=GMM.addTextViewer(NULL,x0,y0,Lx-32-plx,1000,"tmp1.txt",FWhite);
	TV1->Enabled=0;
	int dy=TV1->NLines*TV1->SymSize;
	if(or1)y0+=dy>LY1?dy:LY1;
	else y0+=dy;
	
	int NTB=1;
	int LL=strlen(Quest);
	for(int j=0;j<LL;j++)if(Quest[j]=='|')NTB++;
	x0=0;//Lx/2;
	//int DX=Lx/NTB;
	//int Ny=((y0+38)>>6)+1;
	int dst=40;
	//int Ny1=Ny+((NTB*dst)>>6);
	int qy0=y0+32;//Ny1*64-dst*NTB+10;
	char cc[128];
	int pos;
	TextViewer* TVQ[16];
	for(int q=0;q<NTB;q++){
		pos=0;
		char c;
		do{
			c=Quest[0];
			if(c!='|'){
				cc[pos]=c;
				pos++;
				cc[pos]=0;
			};
			Quest++;
		}while(c!=0&&c!='|');
		char fc[100];
		sprintf(fc,"tmpf%d.txt",q);
		GFILE* f=Gopen(fc,"w");
		Gprintf(f,"%s",GetTextByID(cc));
		Gclose(f);
		TVQ[q]=GMM.addTextViewer(NULL,x0+32,qy0,Lx-64,300,fc,FYellow);
		TVQ[q]->y1=TVQ[q]->y+TVQ[q]->NLines*TVQ[q]->SymSize+4;
		qy0+=TVQ[q]->NLines*TVQ[q]->SymSize+16;
		//TextButton* TB=GMM.addTextButton(NULL,x0,qy0+q*dst,GetTextByID(cc),&fntrb,&fntb,&fntb,1);
		TVQ[q]->OnUserClick=&MMItemChoose;
		TVQ[q]->UserParam=q;
		if(PlayGameMode){
			TVQ[q]->Enabled=0;
		};
	};
	qy0+=32;
	int DDY=(RealLy-qy0)/2-80;//(RealLy-64*Ny1)/2-80;
	GMM.BaseY+=DDY;
	for(int p=0;p<MAXDLG;p++){
		if(GMM.DSS[p]&&GMM.DSS[p]->Visible){
			GMM.DSS[p]->y+=DDY;
			GMM.DSS[p]->y1+=DDY;
		};
	};
	ItemChoose=-1;
	//DrawPaperPanelShadow((RealLx-Lx)>>1,80+DDY-32,((RealLx-Lx)>>1)+Lx,80+DDY+Ny1*64+20);
	DrawPaperPanel((RealLx-Lx)>>1,80+DDY-32,((RealLx-Lx)>>1)+Lx,80+DDY+qy0/*Ny1*64+20*/);
	int T0=GetTickCount();
	do{
		if(PlayGameMode){
			for(int i=0;i<NTB;i++){
				if(i==CurrentAnswer)TVQ[i]->Font=FWhite;
				else TVQ[i]->Font=FButton;
			};
		}else{
			for(int i=0;i<NTB;i++){
				if(TVQ[i]->MouseOver)TVQ[i]->Font=FWhite;
				else TVQ[i]->Font=FButton;
			};
		};
		ProcessMessages();
		//if(KeyPressed&&(LastKey==13||LastKey==27)){
		//	KeyPressed=false;
		//	ItemChoose=mcmOk;
		//};
		DrawPaperPanel((RealLx-Lx)>>1,80+DDY-32,((RealLx-Lx)>>1)+Lx,80+DDY+qy0/*Ny1*64+20*/);
		GMM.MarkToDraw();
		GMM.ProcessDialogs();
		GMM.RefreshView();
		if(PlayGameMode&&GetTickCount()-T0>5000)ItemChoose=CurrentAnswer;
	}while(ItemChoose==-1);
	ContinueGame=true;
	UnPress();
	return ItemChoose;
}

// ----

void ProcessHelp(){
	if(!SCENINF.hLib)return;

	ResFile F=RReset("miss.txt");
	int sz=0;
	if(F!=INVALID_HANDLE_VALUE){
		sz=RFileSize(F);
		RClose(F);
	};
	if(!sz)return;

	int MPPLX=GETV("MPPLX");
	int MPPLY=GETV("MPPLY");
	int MPLDX=GETV("MPLDX");
	int MPRDX=GETV("MPRDX");
	int MPDDY=GETV("MPDDY");
	int MPUDY=GETV("MPDDY");	
	MPUDY+=40;

	// Fonts
	RLCFont* FWindow = &fon16y5;
	RLCFont* FButton = &fon18y5;
	RLCFont* FWhite = &fon18w;
	RLCFont* FYellow = &fon18y3;
	RLCFont* FText = &fon18y3;
	RLCFont* FHint = &fon13y2;

	LocalGP MSG("interf2\\messages");
	LocalGP BTN("interf2\\button2");
	LocalGP SCROL("Interf2\\VScroll2");
	LocalGP LN("interf2\\line2");

	int WinID=0;

	int W=GPS.GetGPWidth(MSG.GPID,WinID*2);
	int H=GPS.GetGPHeight(MSG.GPID,WinID*2);
	int x0=(RealLx-W)/2;//smapx+(nsmaplx<<4)-(MPPLX>>1);
	int y0=(RealLy-H)/2;//smapy+(mul3(nsmaply)<<2)-(MPPLY>>1);
	
	int TitleY=34;

	int BX0=191;
	int BY=520;

	int PY=75;

	int TX0=55;
	int TX1=520;
	int TY1=495;

	DialogsSystem GMM(x0,y0);

	GPS.ShowGP(x0,y0,MSG.GPID,WinID*2+1,0);
	GMM.addGPPicture(NULL,0,0,MSG.GPID,WinID*2);

	// title text
	GMM.addTextButton(NULL,W>>1,TitleY,GetTextByID("MISSOBJ"),FWindow,FWindow,FWindow,1);

	// message text	
	int SBW=GPS.GetGPWidth(SCROL.GPID,0);
	int TY0=PY;
	VScrollBar* SB=GMM.addNewGP_VScrollBar(NULL,TX1-SBW,TY0,TY1-TY0,1,0,SCROL.GPID,0);
	TextViewer* TV;
	if(SCENINF.hLib){
		TV=GMM.addTextViewer(NULL,TX0,TY0,TX1-SBW-5-TX0,TY1-TY0,"miss.txt",FText);
	}else{
		TV=GMM.addTextViewer(NULL,TX0,TY0,TX1-SBW-5-TX0,TY1-TY0,"help.txt",FText);
	};
	TV->AssignScroll(SB);
	TV->SymSize=21;
	TV->PageSize=div(TY1-TY0,TV->SymSize).quot;
	//TV->

	// butons
	GP_TextButton* YesBtn=GMM.addGP_TextButton(NULL,BX0,BY,GetTextByID("@MSOK"),BTN.GPID,3,FButton,FButton);

	YesBtn->UserParam=mcmOk;
	YesBtn->OnUserClick=&MMItemChoose;

	/*
	//char* BmpName=GETS("@MISSPANEL");
	DialogsSystem GMM(smapx+(nsmaplx<<4)-(MPPLX>>1),smapy+(mul3(nsmaply)<<2)-(MPPLY>>1));
	//SQPicture SP(BmpName);

	LocalGP FONTB("vinBig");
	RLCFont fntb(FONTB.GPID);
	RLCFont fntrb(FONTB.GPID);
	RLCFont fntw(FONTB.GPID);
	fntrb.SetRedColor();
	//LoadOptionalTable(5,"2\\orange.grd");
	//fntw.SetColorTable(5);
	
	int LX=0;
	int LY=0;
	TextViewer* TV;
	GMM.addTextButton(NULL,(MPPLX>>1)+1,-20,GetTextByID("MISSOBJ"),&BigBlackFont,&BigBlackFont,&BigBlackFont,1);
	GMM.addTextButton(NULL,MPPLX>>1,-21,GetTextByID("MISSOBJ"),&BigYellowFont,&BigYellowFont,&BigYellowFont,1);
	if(SCENINF.hLib){
		TV=GMM.addTextViewer(NULL,MPLDX,MPUDY+LY,MPPLX-MPLDX-MPRDX,MPPLY-MPDDY-MPUDY-LY-40,"miss.txt",&BlackFont);
	}else{
		TV=GMM.addTextViewer(NULL,MPLDX,MPUDY+LY,MPPLX-MPLDX-MPRDX,MPPLY-MPDDY-MPUDY-LY-40,"help.txt",&BlackFont);
	};
	TV->SymSize+=3;
	TV->PageSize=div(TV->y1-TV->y+1,TV->SymSize).quot;

	TextButton* YesBtn=GMM.addTextButton(NULL,MPPLX>>1,MPPLY-60,GetTextByID("@MSOK"),&fntrb,&fntb,&fntb,1);
	YesBtn->UserParam=mcmOk;
	YesBtn->OnUserClick=&MMItemChoose;
	*/

	ItemChoose=-1;
	//DrawPaperPanelShadow(GMM.BaseX,GMM.BaseY,GMM.BaseX+MPPLX,GMM.BaseY+MPPLY);
	do{
		//DrawPaperPanel(GMM.BaseX,GMM.BaseY,GMM.BaseX+MPPLX,GMM.BaseY+MPPLY);
		ProcessMessages();
		if(KeyPressed&&(LastKey==13||LastKey==27)){
			KeyPressed=false;
			ItemChoose=mcmOk;
		};
		GMM.MarkToDraw();
		GMM.ProcessDialogs();
		GMM.RefreshView();
	}while(ItemChoose==-1);
	ContinueGame=true;

	KeyPressed=0;
	UnPress();
};

void ProcessMissionText(char* Bmp,char* Text){
	int MPPLX=GETV("MPPLX");
	int MPPLY=GETV("MPPLY");
	int MPLDX=GETV("MPLDX");
	int MPRDX=GETV("MPRDX");
	int MPDDY=GETV("MPDDY");
	int MPUDY=GETV("MPDDY");

	// Font
	RLCFont* FWindow = &fon16y5;
	RLCFont* FButton = &fon18y5;
	RLCFont* FWhite = &fon18w;
	RLCFont* FYellow = &fon18y3;
	RLCFont* FText = &fon18y3;
	RLCFont* FHint = &fon13y2;

	LocalGP MSG("interf2\\messages");
	LocalGP BTN("interf2\\button2");
	LocalGP SCROL("Interf2\\VScroll2");
	LocalGP LN("interf2\\line2");

	int WinID=0;

	int W=GPS.GetGPWidth(MSG.GPID,WinID*2);
	int H=GPS.GetGPHeight(MSG.GPID,WinID*2);
	int x0=(RealLx-W)/2;
	int y0=(RealLy-H)/2;
	
	int TitleY=34;

	int BX0=191;
	int BY=520;

	int PY=75;

	int TX0=55;
	int TX1=520;
	int TY1=495;

	DialogsSystem GMM(x0,y0);

	GPS.ShowGP(x0,y0,MSG.GPID,WinID*2+1,0);
	GMM.addGPPicture(NULL,0,0,MSG.GPID,WinID*2);

	SQPicture SPU;
	SPU.SetPixelData(NULL);
	LocalGP CPIC;
	int pid=-1;
	char cc1[100];
	if(strcmp(Bmp,"NONE")){
		sscanf(Bmp,"%s%d",cc1,&pid);
		if(pid!=-1){
			CPIC.Load(cc1);
		};
	};

	ResFile RF=RRewrite("tmp.txt");
	RBlockWrite(RF,Text,strlen(Text));
	RClose(RF);
	LocalGP LGP;

	// title text
	GMM.addTextButton(NULL,W>>1,TitleY,GetTextByID("INTF_MESSAGE"),FWindow,FWindow,FWindow,1);

	// picture
	int LX=0;
	int LY=0;
	if(pid!=-1){
		LX=GPS.GetGPWidth(CPIC.GPID,pid);
		LY=GPS.GetGPHeight(CPIC.GPID,pid)+16;
		GMM.addGPPicture(NULL,(W-LX)>>1,PY,CPIC.GPID,pid);
	};
	
	// line delimiter
	int DW=GPS.GetGPWidth(LN.GPID,0);
	int DH=GPS.GetGPHeight(LN.GPID,0);
	GP_Button* Line=GMM.addGP_Button(NULL,(W-DW)>>1,PY+LY,LN.GPID,0,0);

	// message text	
	int SBW=GPS.GetGPWidth(SCROL.GPID,0);
	int TY0=PY+LY+10+DH;
	VScrollBar* SB=GMM.addNewGP_VScrollBar(NULL,TX1-SBW,TY0,TY1-TY0,1,0,SCROL.GPID,0);
	TextViewer* TV=GMM.addTextViewer(NULL,TX0,TY0,TX1-SBW-5-TX0,TY1-TY0,"tmp.txt",FText);
	TV->AssignScroll(SB);
	TV->SymSize=21;
	TV->PageSize=div(TY1-TY0,TV->SymSize).quot;
	//TV->

	// butons
	//GP_TextButton* YesBtn=GMM.addGP_TextButton(NULL,(MPPLX-100)>>1,MPPLY-40,GetTextByID("@MSOK"),DialogHead.GPID,1,FButton,FButton);
	GP_TextButton* YesBtn=GMM.addGP_TextButton(NULL,BX0,BY,GetTextByID("@MSOK"),BTN.GPID,3,FButton,FButton);

	YesBtn->UserParam=mcmOk;
	YesBtn->OnUserClick=&MMItemChoose;
	if(PlayGameMode){
		YesBtn->Enabled=0;
	};

	ItemChoose=-1;
	//DrawPaperPanelShadow(GMM.BaseX,GMM.BaseY,GMM.BaseX+MPPLX,GMM.BaseY+MPPLY);
	int T0=GetTickCount();
	do{
		//DrawPaperPanel(GMM.BaseX,GMM.BaseY,GMM.BaseX+MPPLX,GMM.BaseY+MPPLY);
		ProcessMessages();
		if(KeyPressed&&(LastKey==13||LastKey==27)){
			KeyPressed=false;
			ItemChoose=mcmOk;
		};
		GMM.MarkToDraw();
		GMM.ProcessDialogs();
		GMM.RefreshView();
		if(PlayGameMode&&GetTickCount()-T0>5000)ItemChoose=mcmOk;
	}while(ItemChoose==-1);
	ContinueGame=true;
	KeyPressed=0;
	UnPress();
};
void ProcessScreen();
int ShowHistryItem(char* Bmp,char* Text,bool Shadow){

	byte* Back=(byte*)malloc(RealLx*RealLy);
	MemScreenPart(0,0,RealLx,RealLy,Back);	

	ItemChoose=-1;
	int MPPLX=GETV("MPPLX");
	int MPPLY=GETV("MPPLY");
	int MPLDX=GETV("MPLDX");
	int MPRDX=GETV("MPRDX");
	int MPDDY=GETV("MPDDY");
	int MPUDY=GETV("MPDDY");
	//char* BmpName=GETS("@MISSPANEL");
	
	// Fonts
	RLCFont* FWindow = &fon16y5;
	RLCFont* FButton = &fon18y5;
	RLCFont* FWhite = &fon18w;
	RLCFont* FYellow = &fon18y3;
	RLCFont* FText = &fon18y3;
	RLCFont* FHint = &fon13y2;

	LocalGP MSG("interf2\\messages");
	LocalGP BTN("interf2\\button2");
	LocalGP SCROL("Interf2\\VScroll2");
	LocalGP LN("interf2\\line2");

	int WinID=2;

	int W=GPS.GetGPWidth(MSG.GPID,WinID*2);
	int H=GPS.GetGPHeight(MSG.GPID,WinID*2);
	int x0=(RealLx-W)/2;//smapx+(nsmaplx<<4)-(MPPLX>>1);
	int y0=(RealLy-H)/2;//smapy+(mul3(nsmaply)<<2)-(MPPLY>>1);
	
	int TitleY=34;

	int BX0=50;
	int BX2=217;
	int BX1=384;
	int BY=520;

	int PY=75;

	int TX0=55;
	int TX1=520;
	int TY1=495;

	DialogsSystem GMM(x0,y0);

	GPS.ShowGP(x0,y0,MSG.GPID,WinID*2+1,0);
	GMM.addGPPicture(NULL,0,0,MSG.GPID,WinID*2);

	SQPicture SPU;
	SPU.SetPixelData(NULL);
	//if(strcmp(Bmp,"NONE")){
	//	SPU.LoadPicture(Bmp);
	//};

	ResFile RF=RRewrite("tmp.txt");
	RBlockWrite(RF,Text,strlen(Text));
	RClose(RF);

	LocalGP LGP;
	
	LocalGP CPIC;
	int pid=-1;
	char cc1[100];
	if(strcmp(Bmp,"NONE")){
		sscanf(Bmp,"%s%d",cc1,&pid);
		if(pid!=-1){
			CPIC.Load(cc1);
		};
	};
	
	/*
	if(strstr(BmpName,".bmp")){
		//GMM.addPicture(NULL,0,0,&SP,&SP,&SP);
	}else{
		LGP.Load(BmpName);
		if(Shadow)GPS.ShowGP(GMM.BaseX,GMM.BaseY,LGP.GPID,0,0);
		GMM.addGPPicture(NULL,0,0,LGP.GPID,1);
	};
	*/
	
	// title text
	GMM.addTextButton(NULL,W>>1,TitleY,GetTextByID("MHISTORY"),FWindow,FWindow,FWindow,1);

	// picture
	int LX=0;
	int LY=0;
	if(pid!=-1){
		LX=GPS.GetGPWidth(CPIC.GPID,pid);
		LY=GPS.GetGPHeight(CPIC.GPID,pid)+16;
		GMM.addGPPicture(NULL,(W-LX)>>1,PY,CPIC.GPID,pid);
	};
	
	// line delimiter
	int DW=GPS.GetGPWidth(LN.GPID,0);
	int DH=GPS.GetGPHeight(LN.GPID,0);
	GP_Button* Line=GMM.addGP_Button(NULL,(W-DW)>>1,PY+LY,LN.GPID,0,0);

	// message text	
	int SBW=GPS.GetGPWidth(SCROL.GPID,0);
	int TY0=PY+LY+10+DH;
	VScrollBar* SB=GMM.addNewGP_VScrollBar(NULL,TX1-SBW,TY0,TY1-TY0,1,0,SCROL.GPID,0);
	TextViewer* TV=GMM.addTextViewer(NULL,TX0,TY0,TX1-SBW-5-TX0,TY1-TY0,"tmp.txt",FText);
	TV->AssignScroll(SB);
	TV->SymSize=21;
	TV->PageSize=div(TY1-TY0,TV->SymSize).quot;

	// butons
	GP_TextButton* YesBtn=GMM.addGP_TextButton(NULL,BX0,BY,GetTextByID("@MSPREV"),BTN.GPID,1,FButton,FButton);
	YesBtn->UserParam=1;
	YesBtn->OnUserClick=&MMItemChoose;

	GP_TextButton* NoBtn=GMM.addGP_TextButton(NULL,BX1,BY,GetTextByID("@MSNEXT"),BTN.GPID,1,FButton,FButton);
	NoBtn->UserParam=2;
	NoBtn->OnUserClick=&MMItemChoose;

	GP_TextButton* CBtn=GMM.addGP_TextButton(NULL,BX2,BY,GetTextByID("@MSCANCEL"),BTN.GPID,1,FButton,FButton);
	CBtn->UserParam=3;
	CBtn->OnUserClick=&MMItemChoose;

	/*
	YesBtn->FontDx+=5;
	YesBtn->FontDy+=5;
	NoBtn->FontDx+=5;
	NoBtn->FontDy+=5;
	CBtn->FontDx+=5;
	CBtn->FontDy+=5;
	*/

	//DrawPaperPanelShadow(GMM.BaseX,GMM.BaseY,GMM.BaseX+MPPLX,GMM.BaseY+MPPLY);
	//DrawPaperPanelShadow(x0,y0,x0+MPPLX,y0+MPPLY);
	ItemChoose=-1;
	do{
		//DrawPaperPanel(GMM.BaseX,GMM.BaseY,GMM.BaseX+MPPLX,GMM.BaseY+MPPLY);
		ProcessMessages();
		if(KeyPressed){
			if(LastKey==13)ItemChoose=3;
			else if(LastKey==27)ItemChoose=3;
			KeyPressed=false;
		};
		GMM.MarkToDraw();
		GMM.ProcessDialogs();
		GMM.RefreshView();
	}while(ItemChoose==-1);
	
	ProcessScreen();
	GMM.addBPXView(NULL,-x0,-y0,RealLx,RealLy,1,1,1,Back,NULL);	

	ContinueGame=true;
	KeyPressed=0;
	Lpressed=0;
	UnPress();

	return ItemChoose;
};

