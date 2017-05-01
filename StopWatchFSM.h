#ifndef SWATCHFSM2_H_
#define SWATCHFSM2_H_

#include "Watch.h"
#include "Event.h"
#include "WigetsView.h"



typedef enum State_ {
	CLOCK, SETUP_HOUR,SETUP_MIN,SETUP_SEC,SETUPCLOCK,SETALARM,SCRONOMODE,CRONOSTARTMODE,CRONOPLAYMODE,CRONOPAUSEMODE,CRONOSTOPMODE,
	DISPLAYTIME, SETTIMEHRS, SETTIMEMINS, SETALARMHRS, SETALARMMINS,  NUM_STATES
} State;


typedef enum Signal_ {
	MODE, SUBMODE, UP, DOWN,  LEFT, RIGTH, NOEVENT, NUM_SIGNALS,CHRONOSTOP, CHRONOTORUN, CHRONOTOPAUSE
} Signal;

extern TimeStructType StartChronoscreen;
/*
 *
 * BK_currentStatemachine def
 typedef struct FSMWatch_ {
	State     curState;
	State	  SubState;
} FSMWatch;

 */


typedef struct FSMWatch_ {
	TimeStructType * Setwatch;
	TimeStructType * Setalarmwatch;
	TimeStructType * Stopwatch;
	char PAlarm;
	char AlarmSetup;
	char ChronoStart;
	State     curState;
	State	  SubState;
} FSMWatch;


TimeStructType *SWatchFSMGetWatch(FSMWatch * ClockFSM);
void WatchFSMTran(FSMWatch * ClockFSM,State nextstate);
void SWatchFSMSubTran(FSMWatch * ClockFSM,State Sstate);
void SWatchFSMInit(FSMWatch * ClockFSM, State _initialState);
void SWatchFSMDispatch(FSMWatch * ClockFSM, Widget * _MyWatchScr,TimeStructType * _watch);
void SWatchFSMUpdateTime(FSMWatch * ClockFSM,TimeStructType * _watch,TimeStructType * c_watch);
void SWatchTestAlarm(FSMWatch * ClockFSM,TimeStructType * _watch, Widget * _MyWatchScr);
//void SWatchChronoControlView(FSMWatch * ClockFSM, Widget * _MyWatchScr);
Signal DecodeEvents();


#endif /* SWATCHFSM2_H_ */
