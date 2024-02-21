/*****************************************************************/
/*  File:   Statistics.h
/*  Desc:   Statistics manager
/*  Author: Silver, Copyright (C) GSC Game World
/*  Date:   Feb 2002
/*****************************************************************/
#ifndef __STATS_H__
#define __STATS_H__

const int c_MaxEvents				= 128;
const int c_MaxCounterNameSymbols	= 512;
/*****************************************************************/
/*  Class:	Stats
/*  Desc:	Statistics collector
/*****************************************************************/
class Stats
{
public:
						Stats();

	static float		GetFPS();
	static float		GetAverageFPS();
	static void			OnFrame();

	static float		GetCounterValue	( int eventID			);	
	static float		GetCounterValue	( const char* eventName	);	
	static void			IncrementCounter( int valID, float amount );
	static int			RegisterCounter	( const char* name		);

protected:
private:

	static Stats					inst;

	static float					FPS;
	static int						frames;
	static int						checkTime;

	static int						totalSeconds;
	static int						totalFrames;

	static float					counter		[c_MaxEvents];
	static char*					counterName	[c_MaxEvents];
	static int						nCounters;

	static char						nameBuf[c_MaxCounterNameSymbols];
	static char*					pFreeNameBuf;	
};  // class Stats

#ifndef _NOSTAT

#define INC_COUNTER(A,B)	{static int ___counterID = Stats::RegisterCounter(#A); \
							Stats::IncrementCounter( ___counterID, (B) ); }
#define GET_COUNTER(A)		Stats::GetCounterValue(#A)
#define GET_COUNTER_STR(A)	Stats::GetCounterValue(A)

#else // _NOSTAT

#define INC_COUNTER(A,B)	;
#define GET_COUNTER(A)		0
#define GET_COUNTER_STR(A)	0

#endif // _NOSTAT



#endif // __STATS_H__