/*
 * SWatchFSM2.c
 *
 *  Created on: 10/nov/2015
 *      Author: Diego
 */
#include "WidgetConfig.h"
#include "StopWatchFSM.h"
#include "WigetsView.h"
#include "pictures.h"
//FSMWatch ClockFSM;
TimeStructType StartClockscreen={0,0,0,0};
TimeStructType StartSetScreen={0,0,0,0};
TimeStructType StartAlarmScreen={0,0,0,0};
TimeStructType StartChronoscreen={0,0,0,0};
TimeStructType CurrentSetup;
Signal cEvent;
//char PAlarm=1;
//char AlarmSetup=0;
//char ChronoStart=0;

Signal DecodeEvents(FSMWatch * ClockFSM)
{
	if(IsEvent(SELECTMODE))return MODE;
	if(IsEvent(PLUS)&&((*ClockFSM).curState!=SCRONOMODE)) return UP;
	if(IsEvent(MINUS)&&((*ClockFSM).curState!=SCRONOMODE)) return DOWN;
	if(IsEvent(WLEFT)) return LEFT;
	if(IsEvent(WRIGTH)) return RIGTH;
	if(IsEvent(PLUS)&&((*ClockFSM).ChronoStart==0)) return CHRONOTORUN;
	if(IsEvent(PLUS)&&((*ClockFSM).ChronoStart==1)) return CHRONOTOPAUSE;
	if(IsEvent(MINUS)) return CHRONOSTOP;
	return NOEVENT;
}


void SWatchFSMUpdateTime(FSMWatch * ClockFSM,TimeStructType * _watch,TimeStructType * c_watch){
	if((*ClockFSM).curState==SETUPCLOCK){
		//Setwatch
		setTime(c_watch,(*ClockFSM).Setwatch->hours,(*ClockFSM).Setwatch->minutes,(*ClockFSM).Setwatch->seconds,(*ClockFSM).Setwatch->tenths);
	}

	if((*ClockFSM).curState==SETALARM){
		//Setalarmwatch
		setTime(c_watch,(*ClockFSM).Setalarmwatch->hours,(*ClockFSM).Setalarmwatch->minutes,(*ClockFSM).Setalarmwatch->seconds,(*ClockFSM).Setalarmwatch->tenths);
		}

	if((*ClockFSM).curState==SCRONOMODE){
		//Stopwatch
		setTime(c_watch,(*ClockFSM).Stopwatch->hours,(*ClockFSM).Stopwatch->minutes,(*ClockFSM).Stopwatch->seconds,(*ClockFSM).Stopwatch->tenths);
		}
	if((*ClockFSM).curState==CLOCK){
		//Clock
		setTime(c_watch,_watch->hours,_watch->minutes,_watch->seconds,_watch->tenths);
	}
	//setTime(c_watch,_watch->hours,_watch->minutes,_watch->seconds,_watch->tenths);
}



void SWatchFSMInit(FSMWatch * ClockFSM, State _initialState)
{
	(*ClockFSM).curState = _initialState;
	(*ClockFSM).PAlarm=1;
	(*ClockFSM).AlarmSetup=0;
	(*ClockFSM).ChronoStart=0;
	((*ClockFSM).Setwatch) = &StartSetScreen;
	((*ClockFSM).Stopwatch) = &StartChronoscreen;
	((*ClockFSM).Setalarmwatch) = &StartAlarmScreen;
}

TimeStructType *SWatchFSMGetWatch(FSMWatch * ClockFSM){
	if((*ClockFSM).curState==SETUPCLOCK) return (*ClockFSM).Setwatch;
	if((*ClockFSM).curState==SETALARM) return (*ClockFSM).Setalarmwatch;
	return (*ClockFSM).Stopwatch;
};

void SWatchFSMTran(FSMWatch * ClockFSM,State nextstate)
{
	(*ClockFSM).curState = nextstate;
}

void SWatchFSMSubTran(FSMWatch * ClockFSM,State Sstate)
{
	(*ClockFSM).SubState = Sstate;
}

void SWatchTestAlarm(FSMWatch * ClockFSM,TimeStructType * _watch, Widget * _MyWatchScr)
{
	if(((*ClockFSM).Setalarmwatch->hours==(*_watch).hours)&&((*ClockFSM).Setalarmwatch->minutes==(*_watch).minutes) && ((ClockFSM)->AlarmSetup==1)){
		if((*ClockFSM).PAlarm==1){
			(*ClockFSM).PAlarm=0;
			DrawOff(&_MyWatchScr[BALARM]);
		}
		else{
			(*ClockFSM).PAlarm=1;
			DrawOn(&_MyWatchScr[BALARM]);
		}
	}
}

void SWatchChronoControlView(FSMWatch * ClockFSM, Widget * _MyWatchScr){
	LCD_DrawPicture(202, 120, 58, 53,&CrhonoStopOn);
		if((*ClockFSM).SubState==CRONOPLAYMODE){
			LCD_DrawPicture(142, 125, 58, 45,&ChronoPlayOn);
		}
		else if ((*ClockFSM).SubState==CRONOPAUSEMODE){
			LCD_DrawPicture(140, 122, 58, 50,&ChronoPauseOn);
		}
}

void SWatchChronoControlOff(FSMWatch * ClockFSM, Widget * _MyWatchScr){
	LCD_DrawPicture(202, 120, 58, 53,&CrhonoStopOff);
		if((*ClockFSM).SubState==CRONOPLAYMODE){
			LCD_DrawPicture(142, 125, 58, 45,&ChronoPlayOff);
		}
		else if ((*ClockFSM).SubState==CRONOPAUSEMODE){
			LCD_DrawPicture(140, 122, 58, 50,&ChronoPauseOff);
		}
}


void SWatchFSMDispatch(FSMWatch * ClockFSM, Widget * _MyWatchScr, TimeStructType * _watch){

	switch((*ClockFSM).curState){
	case CLOCK:
		UpdateMode(WCLOCKMODE,_MyWatchScr);//Update The widgets WCLOCKMODE: specific Widget ; _MyWatchScr all widget Conf
		switch (DecodeEvents(ClockFSM)){
		case MODE:
			SWatchFSMTran(ClockFSM,SETUPCLOCK);
			SWatchFSMSubTran(ClockFSM,SETUP_HOUR);
			UpdateMode(WSETMODE,_MyWatchScr);
			break;
		default:
			SWatchFSMTran(ClockFSM,CLOCK);
			UpdateMode(WCLOCKMODE,_MyWatchScr);
			break;
		}
		break;
	case SETUPCLOCK:
		CurrentSetup=(*(*ClockFSM).Setwatch);//My current setup value for evaluate hour modification.
		switch ((*ClockFSM).SubState){
			case SETUP_HOUR:
				cEvent=DecodeEvents(ClockFSM);
				switch (DecodeEvents(ClockFSM)){
					case MODE:
						SWatchFSMTran(ClockFSM,SETALARM);
						SWatchFSMSubTran(ClockFSM,SETTIMEHRS);
						UpdateMode(WALARMMODE,_MyWatchScr);
					break;
					case UP:
						(*ClockFSM).Setwatch->hours=((abs((*ClockFSM).Setwatch->hours+1))%24);
						(*ClockFSM).Setwatch->minutes=0;
						(*ClockFSM).Setwatch->seconds=0;
					break;
					case DOWN:
						(*ClockFSM).Setwatch->hours=((abs((*ClockFSM).Setwatch->hours-1))%24);
						(*ClockFSM).Setwatch->minutes=0;
						(*ClockFSM).Setwatch->seconds=0;
					break;
					case RIGTH:
						SWatchFSMSubTran(ClockFSM,SETUP_MIN);
					break;
					default:
						SWatchFSMSubTran(ClockFSM,SETUP_HOUR);
					break;
				}
			break;
		case SETUP_MIN:
				switch (DecodeEvents(ClockFSM)){
					case MODE:
						SWatchFSMTran(ClockFSM,SETALARM);
						SWatchFSMSubTran(ClockFSM,SETTIMEHRS);
						UpdateMode(WALARMMODE,_MyWatchScr);
					break;
					case UP:
						(*ClockFSM).Setwatch->minutes=abs(((*ClockFSM).Setwatch->minutes+1))%60;
						(*ClockFSM).Setwatch->seconds=0;
						//(*_watch).seconds=0;
					break;
					case DOWN:
						(*ClockFSM).Setwatch->minutes=abs(((*ClockFSM).Setwatch->minutes-1))%60;
						(*ClockFSM).Setwatch->seconds=0;
					break;
					case RIGTH:
						SWatchFSMSubTran(ClockFSM,SETUP_SEC);
					break;
					case LEFT:
						SWatchFSMSubTran(ClockFSM,SETUP_HOUR);
					break;
					default:
						SWatchFSMSubTran(ClockFSM,SETUP_MIN);
				break;
		}

		break;
		case SETUP_SEC:
				switch (DecodeEvents(ClockFSM)){
					case MODE:
						SWatchFSMTran(ClockFSM,SETALARM);
						SWatchFSMSubTran(ClockFSM,SETTIMEHRS);
						UpdateMode(WALARMMODE,_MyWatchScr);
					break;
					case UP:
						(*ClockFSM).Setwatch->seconds=abs(((*ClockFSM).Setwatch->seconds+1))%60;
						//ClockFSM->watch.seconds=ClockFSM->watch.seconds+1;
					break;
					case DOWN:
						(*ClockFSM).Setwatch->seconds=abs(((*ClockFSM).Setwatch->seconds-1))%60;;
						//ClockFSM->watch.seconds=ClockFSM->watch.seconds-1;
					break;
					case LEFT:
						SWatchFSMSubTran(ClockFSM,SETUP_MIN);
					break;
					default:
						SWatchFSMSubTran(ClockFSM,SETUP_SEC);
					break;
		}
		break;
	}
	if((CurrentSetup.hours!=(*ClockFSM).Setwatch->hours)||(CurrentSetup.minutes!=(*ClockFSM).Setwatch->minutes)||(CurrentSetup.seconds!=(*ClockFSM).Setwatch->seconds))
		copyTime(_watch,((*ClockFSM).Setwatch));
	break;

	case SETALARM:
		switch ((*ClockFSM).SubState){
			case SETTIMEHRS:
				switch (DecodeEvents(ClockFSM)){
					case MODE:
						SWatchFSMTran(ClockFSM,SCRONOMODE);
						if(((*ClockFSM).ChronoStart==0))SWatchFSMSubTran(ClockFSM,CRONOPAUSEMODE);
						if(((*ClockFSM).ChronoStart==1))SWatchFSMSubTran(ClockFSM,CRONOPLAYMODE);
						UpdateMode(WCRONOMODE,_MyWatchScr);
						SWatchChronoControlView(ClockFSM,_MyWatchScr);
					break;
					case UP:
						(*ClockFSM).AlarmSetup=1;
						(*ClockFSM).Setalarmwatch->hours=abs(((*ClockFSM).Setalarmwatch->hours+1))%24;
						(*ClockFSM).Setalarmwatch->seconds=0;

					break;
					case DOWN:
						(*ClockFSM).AlarmSetup=1;
						(*ClockFSM).Setalarmwatch->hours=abs(((*ClockFSM).Setalarmwatch->hours-1))%24;
						(*ClockFSM).Setalarmwatch->seconds=0;

					break;
					case RIGTH:
						SWatchFSMSubTran(ClockFSM,SETALARMMINS);
					break;
					default:
						SWatchFSMSubTran(ClockFSM,SETTIMEHRS);
					break;
				}
			break;
			case SETALARMMINS:
				switch (DecodeEvents(ClockFSM)){
					case MODE:
						if(((*ClockFSM).ChronoStart==0))SWatchFSMSubTran(ClockFSM,CRONOPAUSEMODE);
						if(((*ClockFSM).ChronoStart==1))SWatchFSMSubTran(ClockFSM,CRONOPLAYMODE);
						SWatchFSMTran(ClockFSM,SCRONOMODE);
						UpdateMode(WCRONOMODE,_MyWatchScr);
						SWatchChronoControlView(ClockFSM,_MyWatchScr);
					break;
					case UP:
						(*ClockFSM).AlarmSetup=1;
						(*ClockFSM).Setalarmwatch->minutes=abs(((*ClockFSM).Setalarmwatch->minutes+1))%60;
						(*ClockFSM).Setalarmwatch->seconds=0;

					break;
					case DOWN:
						(*ClockFSM).AlarmSetup=1;
						(*ClockFSM).Setalarmwatch->minutes=abs(((*ClockFSM).Setalarmwatch->minutes-1))%60;
						(*ClockFSM).Setalarmwatch->seconds=0;
					break;
					case RIGTH:
						SWatchFSMSubTran(ClockFSM,SETALARMMINS);
					break;
					case LEFT:
						SWatchFSMSubTran(ClockFSM,SETTIMEHRS);
					break;
					default:
					SWatchFSMSubTran(ClockFSM,SETALARMMINS);
					break;
				}
			break;
		}
	break;
	case SCRONOMODE:
		switch ((*ClockFSM).SubState){
		case CRONOSTARTMODE:
			switch (DecodeEvents(ClockFSM)){
				case MODE:
					SWatchFSMTran(ClockFSM,CLOCK);
					UpdateMode(WCLOCKMODE,_MyWatchScr);
					SWatchChronoControlOff(ClockFSM, _MyWatchScr);
					break;
				case CHRONOTORUN:
					(*ClockFSM).ChronoStart=1;
					SWatchFSMSubTran(ClockFSM,CRONOPLAYMODE);
					SWatchChronoControlView(ClockFSM, _MyWatchScr);
					break;
				case CHRONOTOPAUSE:
					(*ClockFSM).ChronoStart=0;
					SWatchFSMSubTran(ClockFSM,CRONOPAUSEMODE);
					SWatchChronoControlView(ClockFSM, _MyWatchScr);
					break;
				case CHRONOSTOP:
					(*ClockFSM).ChronoStart=0;
					SWatchFSMSubTran(ClockFSM,CRONOPLAYMODE);
					initTime((ClockFSM)->Stopwatch);
					SWatchChronoControlView(ClockFSM, _MyWatchScr);
					break;
			}
		//End	CRONOSTARTMODE
			break;
			case CRONOPLAYMODE:
				(*ClockFSM).ChronoStart=1;
				switch (DecodeEvents(ClockFSM)){
					case MODE:
					SWatchFSMTran(ClockFSM,CLOCK);
					UpdateMode(WCLOCKMODE,_MyWatchScr);
					SWatchChronoControlOff(ClockFSM, _MyWatchScr);
					break;
					case CHRONOTOPAUSE:
					(*ClockFSM).ChronoStart=0;
					SWatchFSMSubTran(ClockFSM,CRONOPAUSEMODE);
					SWatchChronoControlView(ClockFSM, _MyWatchScr);
					break;
					case CHRONOSTOP:
					SWatchChronoControlView(ClockFSM, _MyWatchScr);
					initTime((ClockFSM)->Stopwatch);
					SWatchFSMSubTran(ClockFSM,CRONOPAUSEMODE);
					break;
				}
			break;
			case CRONOPAUSEMODE:
				(*ClockFSM).ChronoStart=0;
				switch (DecodeEvents(ClockFSM)){
					case MODE:
					SWatchFSMTran(ClockFSM,CLOCK);
					UpdateMode(WCLOCKMODE,_MyWatchScr);
					SWatchChronoControlOff(ClockFSM, _MyWatchScr);
					break;
					case CHRONOTORUN:
						(*ClockFSM).ChronoStart=1;
						SWatchFSMSubTran(ClockFSM,CRONOPLAYMODE);
						SWatchChronoControlView(ClockFSM, _MyWatchScr);
						break;
					case CHRONOSTOP:
						SWatchFSMSubTran(ClockFSM,CRONOPAUSEMODE);
						initTime((ClockFSM)->Stopwatch);
						SWatchChronoControlView(ClockFSM, _MyWatchScr);
						break;
				}
			break;
		}

	break;
	}
}


