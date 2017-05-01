/*
 * WidgetConfig.c
 *
 *  Created on: 21/ott/2015
 *      Author: admim
 */

#include "WidgetConfig.h"
#include "pictures.h"
#include <stdio.h>
#include "stm32f4_discovery_lcd.h"
#include "fonts.h"

ButtonIcon buttontime = {
		icontime_on, icontime_off, TIMEMODE
};

ButtonIcon buttontimeset = {
		icontimeset_on, icontimeset_off, TIMESETMODE
};

ButtonIcon buttonalarm = {
		iconalarm_on, iconalarm_off, ALARMMODE
};

ButtonIcon buttonswatch = {
		iconswatch_on, iconswatch_off, SWATCHMODE
};


ButtonIcon buttonChronoPauseplay = {
		0, 0, PLUS
		//icontime_on, icontime_off, SWCHRONOPLAY
};

//ButtonIcon buttonChronoPause = {
//		0, 0, SWCHRONOPAUSE
//};

ButtonIcon buttonChronoStop = {
		0, 0, MINUS
};


ButtonIcon buttonsmode = {
		0, 0, SELECTMODE
};

ButtonIcon buttonplus = {
		0, 0, PLUS
};

ButtonIcon buttonminus = {
		0, 0, MINUS
};

ButtonIcon buttonleft = {
		0, 0, WLEFT
};

ButtonIcon buttonrigth = {
		0, 0, WRIGTH
};

Text txt = {
		&Font16x24, White
};

Image backg = {
		ImageBuffer2
};

const Widget MyWatchScr[NUMWIDGETS] = {
		{0, 0, 320, 240, BACKGROUND, (void *)&backg},
		{0, 178,62, 62, BUTTONICON, (void *)&buttontime},
		{55, 178, 66, 62, BUTTONICON, (void *)&buttontimeset},
		{110, 178, 66, 62, BUTTONICON, (void *)&buttonalarm},
		{173, 170, 58, 56, BUTTONICON, (void *)&buttonswatch},
		{270, 178, 58, 63, BUTTONICON, (void *)&buttonsmode},
		{10, 130,30, 30, BUTTONICON, (void *)&buttonplus},
		{63, 126, 30, 30, BUTTONICON, (void *)&buttonminus},
		{250, 25, 50, 50, BUTTONICON, (void *)&buttonleft},
		{250, 90, 50, 50, BUTTONICON, (void *)&buttonrigth},
//		Controls Chrono

		//{200, 126, 54, 55, BUTTONICON, (void *)&buttonChronoPlay},
		{142, 125, 58, 45, BUTTONICON, (void *)&buttonChronoPauseplay},
		{202, 120, 58, 53, BUTTONICON, (void *)&buttonChronoStop},

//		{26, 76, 100, 32, RECTANGLE, (void *)&rectangle}
		{29, 70, 40, 40, TEXT, (void *)&txt},
		{99, 70, 40, 40, TEXT, (void *)&txt},
		{168, 70, 40, 40, TEXT, (void *)&txt},
		{243, 70, 40, 40, TEXT, (void *)&txt},
		{80, 70, 40, 40, TEXT, (void *)&txt},
		{149, 70, 40, 40, TEXT, (void *)&txt},
};

