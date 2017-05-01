/*
 * WidgetConfig.h
 *
 *  Created on: 22/ott/2015
 *      Author: admim
 */
#include "Widget.h"

#ifndef WIDGETCONFIG_H_
#define WIDGETCONFIG_H_

#define NUMWIDGETS 18

#define BAKCG 0
#define BTIME 1
#define BTIMESET 2
#define BALARM 3
#define BSWATCH 4
#define BMODE 5
#define BPLUS 6
#define BMINUS 7
#define BLEFT 8
#define BRIGHT 9
#define HRSSTR 10
#define MINSTR 11
#define SECSTR 12
#define TTSSTR 13
#define SEP1STR 14
#define SEP2STR 15

#define TIMEMODE 0x00
#define TIMESETMODE 0x01
#define ALARMMODE 0x02
#define SWATCHMODE 0x04
#define SELECTMODE 0x08
#define PLUS 0x10
#define MINUS 0X20//0x020
#define WLEFT 0X40//0x040
#define WRIGTH 0x80
#define SWCHRONOPAUSEPLAY 0x200
//#define SWCHRONOPAUSE 0X400
#define SWCHRONOSTOP 0x400


extern const Widget MyWatchScr[NUMWIDGETS];

#endif /* WIDGETCONFIG_H_ */
