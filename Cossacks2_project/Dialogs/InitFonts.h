
RLCFont  BigBlackFont;
RLCFont  BigWhiteFont;
RLCFont  BigYellowFont;
RLCFont  BigRedFont;
RLCFont  BlackFont;
RLCFont  YellowFont;
RLCFont  WhiteFont;
RLCFont  RedFont;
RLCFont  SmallBlackFont;
RLCFont  SmallWhiteFont;
RLCFont  SmallYellowFont;
RLCFont  SmallRedFont;
RLCFont  SmallBlackFont1;
RLCFont  SmallWhiteFont1;
RLCFont  SmallYellowFont1;
RLCFont  SmallRedFont1;
RLCFont  SpecialWhiteFont;
RLCFont  SpecialYellowFont;
RLCFont  SpecialRedFont;
RLCFont  SpecialBlackFont;
RLCFont  fn10;
RLCFont  fn8;

RLCFont  fon30y1;
RLCFont  fon30y3;
RLCFont  fon30y5;
RLCFont  fon40y1;
RLCFont  fon40y5;
RLCFont  fon18w;
RLCFont  fon18y3;
RLCFont  fon18y5;
RLCFont  fon16y1;
RLCFont  fon16y3;
RLCFont  fon16y5;
RLCFont  fon13y2;
RLCFont  fonG60y5;
RLCFont  fonG60w;


void InitFonts(){

	int GPID;
	int GPSh;
	// --- American Conquest ---

	// Main menu
	GPID=GPS.PreLoadGPImage("FontG60");
	GPSh=GPS.PreLoadGPImage("FontG60s");
	fonG60y5.SetGPIndex(GPID);
	fonG60y5.SetColorTable(6);
	//fonG60y5.SetShadowGP(GPSh,0,0);

	fonG60w.SetGPIndex(GPID);
	fonG60w.SetColorTable(2);
	fonG60w.SetShadowGP(GPSh,0,0);

	// Header Big 40
	GPID=GPS.PreLoadGPImage("FontM40");
	GPSh=GPS.PreLoadGPImage("FontM40s");
	fon40y1.SetGPIndex(GPID);
	fon40y1.SetColorTable(3);	
	fon40y1.SetShadowGP(GPSh,0,0);

	fon40y5.SetGPIndex(GPID);
	fon40y5.SetColorTable(6);
	fon40y5.SetShadowGP(GPSh,0,0);

	// Header Big 30
	GPID=GPS.PreLoadGPImage("FontM30");
	GPSh=GPS.PreLoadGPImage("FontM30s");
	fon30y1.SetGPIndex(GPID);
	fon30y1.SetColorTable(2);	
	fon30y1.SetShadowGP(GPSh,0,0);

	fon30y3.SetGPIndex(GPID);
	fon30y3.SetColorTable(3);	
	fon30y3.SetShadowGP(GPSh,0,0);

	fon30y5.SetGPIndex(GPID);
	fon30y5.SetColorTable(5);	

	// Main 18
	GPID=GPS.PreLoadGPImage("FontM18");
	GPSh = GPS.PreLoadGPImage("FontM18s");
	fon18w.SetGPIndex(GPID);
	fon18w.SetColorTable(1);	
	fon18w.SetShadowGP(GPSh,1,1);
	
	fon18y3.SetGPIndex(GPID);
	fon18y3.SetColorTable(3);
	fon18y3.SetShadowGP(GPSh,1,1);

	fon18y5.SetGPIndex(GPID);
	fon18y5.SetColorTable(6);	

	// Med 16
	GPID=GPS.PreLoadGPImage("FontM16");
	GPSh = GPS.PreLoadGPImage("FontM16s");
	fon16y1.SetGPIndex(GPID);
	fon16y3.SetGPIndex(GPID);
	fon16y5.SetGPIndex(GPID);

	fon16y1.SetColorTable(1);
	fon16y3.SetColorTable(3);	
	fon16y5.SetColorTable(6);	

	fon16y1.SetShadowGP(GPSh,1,1);
	fon16y3.SetShadowGP(GPSh,1,1);	
	//fon16y5.SetShadowGP(GPSh,1,1);	

	// Hint 13
	GPID=GPS.PreLoadGPImage("FontM13");
	GPSh=GPS.PreLoadGPImage("FontM13s");
	fon13y2.SetGPIndex(GPID);
	fon13y2.SetColorTable(3);
	fon13y2.SetShadowGP(GPSh,0,0);

	// Old Standart

	GPID=GPS.PreLoadGPImage("FontM40");
	GPSh=GPS.PreLoadGPImage("FontM40s");
	BigWhiteFont.SetGPIndex(GPID);
	BigWhiteFont.SetColorTable(1);	
	BigYellowFont.SetGPIndex(GPID);
	BigYellowFont.SetColorTable(3);
	BigRedFont.SetGPIndex(GPID);	
	
	BigBlackFont.SetGPIndex(GPID);
	BigBlackFont.SetColorTable(1);
	BigBlackFont.SetShadowGP(GPSh,0,0);

	GPID=GPS.PreLoadGPImage("FontM18");
	GPSh=GPS.PreLoadGPImage("FontM18s");

	WhiteFont.SetGPIndex(GPID);	
	YellowFont.SetGPIndex(GPID);
	RedFont.SetGPIndex(GPID);	
	BlackFont.SetGPIndex(GPID);	

	WhiteFont.SetColorTable(1);
	YellowFont.SetColorTable(3);
	RedFont.SetColorTable(2);
	BlackFont.SetColorTable(6);

	WhiteFont.SetShadowGP(GPSh,1,1);
	YellowFont.SetShadowGP(GPSh,1,1);
	RedFont.SetShadowGP(GPSh,1,1);
	//BlackFont.SetShadowGP(GPSh,1,1);

	GPID=GPS.PreLoadGPImage("FontM16");
	GPSh = GPS.PreLoadGPImage("FontM16s");

	SpecialWhiteFont.SetGPIndex(GPID);
	SpecialWhiteFont.SetColorTable(1);
	SpecialYellowFont.SetGPIndex(GPID);
	SpecialYellowFont.SetColorTable(3);
	SpecialRedFont.SetGPIndex(GPID);
	SpecialRedFont.SetColorTable(2);
	SpecialBlackFont.SetGPIndex(GPID);
	SpecialBlackFont.SetColorTable(6);

	SpecialWhiteFont.SetShadowGP(GPSh,1,1);
	SpecialYellowFont.SetShadowGP(GPSh,1,1);
	SpecialRedFont.SetShadowGP(GPSh,1,1);
	//SpecialBlackFont.SetShadowGP(GPSh,1,1);

	GPID=GPS.PreLoadGPImage("FontM13");
	GPSh=GPS.PreLoadGPImage("FontM13s");
	SmallWhiteFont.SetGPIndex(GPID);
	SmallWhiteFont.SetColorTable(1);
	SmallRedFont.SetGPIndex(GPID);
	SmallRedFont.SetColorTable(2);
	SmallYellowFont.SetGPIndex(GPID);
	SmallYellowFont.SetColorTable(3);
	SmallBlackFont.SetGPIndex(GPID);
	SmallBlackFont.SetColorTable(6);

	//SmallWhiteFont.SetShadowGP(GPSh,0,0);
	//SmallRedFont.SetShadowGP(GPSh,0,0);
	//SmallYellowFont.SetShadowGP(GPSh,0,0);

	SmallWhiteFont1.SetGPIndex(GPID);
	SmallWhiteFont1.SetColorTable(1);
	SmallRedFont1.SetGPIndex(GPID);
	SmallRedFont1.SetColorTable(2);
	SmallBlackFont1.SetGPIndex(GPID);
	SmallBlackFont1.SetColorTable(6);
	SmallYellowFont1.SetGPIndex(GPID);
	SmallYellowFont1.SetColorTable(3);

	//SmallWhiteFont1.SetShadowGP(GPSh,0,0);
	//SmallRedFont1.SetShadowGP(GPSh,0,0);
	//SmallYellowFont1.SetShadowGP(GPSh,0,0);

	// very small
	int s1=GPS.PreLoadGPImage("FontM12");
	int s2=GPS.PreLoadGPImage("FontM12");
	fn8.SetGPIndex(s1);
	fn8.SetColorTable(1);
	fn10.SetGPIndex(s2);
	fn10.SetColorTable(1);

#ifdef _USE3D
	static const DWORD c_Black	= 0xFF462814;
	static const DWORD c_Red	= 0xFFFF0000;
	static const DWORD c_Yellow = 0xFFEBD6B2;
	static const DWORD c_White	= 0xFFFFFFFF;

	fonG60y5.		SetColor( c_Black  );
	fonG60w.		SetColor( c_White  );
	fon40y1.		SetColor( c_Yellow );	
	fon40y5.		SetColor( c_Black  );
	fon30y1.		SetColor( c_Red    );	
	fon30y3.		SetColor( c_Yellow );	
	fon30y5.		SetColor( c_Yellow );	
	fon18w.			SetColor( c_White  );	
	fon18y3.		SetColor( c_Yellow );
	fon18y5.		SetColor( c_Black  );	
	fon16y1.		SetColor( c_White  );
	fon16y3.		SetColor( c_Yellow );	
	fon16y5.		SetColor( c_Black  );	
	fon13y2.		SetColor( c_Yellow );
	BigWhiteFont.	SetColor( c_White  );	
	BigYellowFont.	SetColor( c_Yellow );
	BigBlackFont.	SetColor( c_White  );
	WhiteFont.		SetColor( c_White  );
	YellowFont.		SetColor( c_Yellow );
	RedFont.		SetColor( c_Red    );
	BlackFont.		SetColor( c_Black  );
	SpecialRedFont.	SetColor( c_Red    );
	SmallWhiteFont.	SetColor( c_White  );
	SmallRedFont.	SetColor( c_Red    );
	SmallYellowFont.SetColor( c_Yellow );
	SmallBlackFont.	SetColor( c_Black  );
	SmallWhiteFont1.SetColor( c_White  );
	SmallRedFont1.	SetColor( c_Red    );
	SmallBlackFont1.SetColor( c_Black  );
	fn8.			SetColor( c_White  );
	fn10.			SetColor( c_White  );
	SpecialBlackFont.SetColor( c_Black );
	SmallYellowFont1.SetColor( c_Yellow );
	SpecialWhiteFont.SetColor( c_White  );
	SpecialYellowFont.SetColor( c_Yellow);
	
#endif // _USE3D

};
