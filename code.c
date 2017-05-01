/* ###*B*###
 * ERIKA Enterprise - a tiny RTOS for small microcontrollers
 *
 * Copyright (C) 2002-2013  Evidence Srl
 *
 * This file is part of ERIKA Enterprise.
 *
 * ERIKA Enterprise is free software; you can redistribute it
 * and/or modify it under the terms of the GNU General Public License
 * version 2 as published by the Free Software Foundation,
 * (with a special exception described below).
 *
 * Linking this code statically or dynamically with other modules is
 * making a combined work based on this code.  Thus, the terms and
 * conditions of the GNU General Public License cover the whole
 * combination.
 *
 * As a special exception, the copyright holders of this library give you
 * permission to link this code with independent modules to produce an
 * executable, regardless of the license terms of these independent
 * modules, and to copy and distribute the resulting executable under
 * terms of your choice, provided that you also meet, for each linked
 * independent module, the terms and conditions of the license of that
 * module.  An independent module is a module which is not derived from
 * or based on this library.  If you modify this code, you may extend
 * this exception to your version of the code, but you are not
 * obligated to do so.  If you do not wish to do so, delete this
 * exception statement from your version.
 *
 * ERIKA Enterprise is distributed in the hope that it will be
 * useful, but WITHOUT ANY WARRANTY; without even the implied warranty
 * of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License version 2 for more details.
 *
 * You should have received a copy of the GNU General Public License
 * version 2 along with ERIKA Enterprise; if not, write to the
 * Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301 USA.
 * ###*E*### */

#include "ee.h"
#include "ee_irq.h"
#include <stdio.h>
#include "stm32f4xx_conf.h"
#include "stm32f4_discovery.h"
#include "stm32f4_discovery_lcd.h"
#include "stm32f4xx.h"
//#include "NumbersView.h"
#include "NumbersView.h"
#include "StopWatchFSM.h"

#include "STMPE811QTR.h"
#include "pictures.h"
#include "Widget.h"
#include "WidgetConfig.h"
#include "Touch.h"
#include "Event.h"
#include "SW_wrapper.h"
#include "lcd_add.h"
#include "fonts.h"
#include "debug.h"


TimeStructType mywatch;
TimeStructType Currentwatch;
FSMWatch MyFSM;

D_Work_SW_wrapper Dstate;
PrevZCSigStates_SW_wrapper Sigstate;
boolean_T tick, button[4], buzzer;
uint8_T hours=0, minutes=0, seconds=0, tenths=0, mode;
Digit Hour,Min,Sec;
Display A;
boolean_T Psecond=TRUE;

void Delay (){
	 int c = 1, d = 1;

	   for ( c = 1 ; c <= 50 ; c++ )
	       for ( d = 1 ; d <= 10 ; d++ )
	       {}

}

/*
 * SysTick ISR2
 */
ISR2(systick_handler)
{
	/* count the interrupts, waking up expired alarms */
	CounterTick(myCounter);
}

/*
 * TASK LCD
 */
TASK(TaskLCD)
{
	unsigned int px, py;
	TPoint p;
	if (GetTouch_SC_Async(&px, &py)) {
		p.x = px;
		p.y = py;
		OnTouch(MyWatchScr, &p);
	}
}

void setTimeString(char *watchstr, uint8_T hours, uint8_T minutes, uint8_T seconds, uint8_T tenths, uint8_T mode)
{
	sprintf(watchstr, "%2d:%2d:%2d", hours, minutes, seconds);
}
/*
 * TASK Clock
 */
unsigned char IsUpdateTime()
{
	unsigned char res;
	static unsigned char oh=0, om=0, os=0;
	if (hours!=oh || minutes!=om || seconds!= os)
		res = 1;
	else
		res = 0;
	oh = hours;
	om = minutes;
	os = seconds;
	return res;
}

void UpdateTime()
{
	unsigned char watchstr[20];
	setTimeString(watchstr, hours, minutes, seconds, tenths, mode);

	LCD_SetTextColor(Black);
	LCD_SetBackColor(Black);
	LCD_DrawFullRect(30, 76, 160, 32);
	LCD_DrawFullRect(98, 188, 39, 43);

/*	WPrint(&MyWatchScr[TIMESTR], watchstr); */
}


void strencode2digit(char *str, int digit)
{
	str[2]=0;
	str[1]=(digit/10);//digit/10+'0';////
	str[0]=(digit%10);//digit%10+'0';////
}

void TickCrono(FSMWatch * MyFSM){
	if ((*MyFSM).ChronoStart==1){
		TickTenth((*MyFSM).Stopwatch);
	}

}

TASK(TaskClock)
{


	unsigned char i;
	static int oldmode=8;
	static unsigned char oh=99, om=99, os=99;
	char tstr[3];
	Display * S = &A;
	if (tick==FALSE){
	tick=TRUE;
	}
		else {tick=FALSE;
		}
	TickTenth(&mywatch);//For contunusly Clock Tick
	TickCrono(&MyFSM);//for continusly Clock Chrono
	SWatchTestAlarm(&MyFSM,&mywatch,&MyWatchScr);//In case of alarm bling

	if (IsEvent(SELECTMODE)||IsEvent(PLUS)||IsEvent(MINUS)||IsEvent(WLEFT)||IsEvent(WRIGTH)||IsEvent(SWCHRONOPAUSEPLAY)||IsEvent(SWCHRONOSTOP)){
		SWatchFSMDispatch(&MyFSM,&MyWatchScr,&mywatch);
		ClearEvents();
	}
	SWatchFSMUpdateTime(&MyFSM,&mywatch,&Currentwatch);//visualize the current state clock.

	hours=(Currentwatch.hours)%24;
	minutes=(Currentwatch.minutes)%60;
	seconds=(Currentwatch.seconds)%60;



	/* seconds = seconds+1; */


	for (i=0;i<4;i++) button[i]=0;

	if (hours!=oh) {
		strencode2digit(tstr, hours);
		DigitUpdate(&(S->_Hour),tstr);
		LCD_SetTextColor(Black);
		LCD_SetBackColor(Black);
		LCD_DrawFullRect(99, 70, 62, 48);
		DigitDraw(&A._Hour);
		DigitDraw(&A._Min);
		DigitDraw(&A._Sec);
		LCD_DrawPicture(78,70,16,48,names[10]);
		LCD_DrawPicture(139,70,16,48,names[10]);
		oh=hours;
	}

	if (minutes!=om) {
		strencode2digit(tstr, minutes);
		DigitUpdate(&(S->_Min),tstr);
		LCD_SetTextColor(Black);
		LCD_SetBackColor(Black);
		LCD_DrawFullRect(99, 70, 62, 48);
		DigitDraw(&A._Hour);
		DigitDraw(&A._Min);
		DigitDraw(&A._Sec);
		LCD_DrawPicture(78,70,16,48,names[10]);
		LCD_DrawPicture(139,70,16,48,names[10]);
		om=minutes;
	}

	if (seconds!= os) {
		if (Psecond==FALSE){
			Psecond=TRUE;
			LCD_DrawPicture(78,70,16,48,names[10]);
			LCD_DrawPicture(139,70,16,48,names[10]);}

		else {
			Psecond=FALSE;
			LCD_SetBackColor(Black);
			LCD_DrawFullRect(78, 70, 16, 48);
			LCD_DrawFullRect(139, 70, 16, 48);
			}

		strencode2digit(tstr, seconds);
		DigitUpdate(&(S->_Sec),tstr);
		LCD_SetTextColor(Black);
		LCD_SetBackColor(Black);
		LCD_DrawFullRect(168, 70, 62, 48);
		DigitDraw(&A._Hour);
		DigitDraw(&A._Min);
		DigitDraw(&A._Sec);
		os=seconds;
	}

	/**/
}

/**
  * @brief  Inserts a delay time.
  * @param  nCount: specifies the delay time length.
  * @retval None
  */



/*
 * MAIN TASK
 */
int main(void)
{
	SystemInit();
  /*Initializes Erika related stuffs*/
	EE_system_init();

  /* init state machine */

	SWatchFSMInit(&MyFSM,CLOCK);


	/*Initialize systick */
	EE_systick_set_period(MILLISECONDS_TO_TICKS(1, SystemCoreClock));
	EE_systick_enable_int();
	EE_systick_start();

	/* Initializes LCD and touchscreen */
	IOE_Config();
	/* Initialize the LCD */
	STM32f4_Discovery_LCD_Init();



	/* Set the LCD Text size */
	InitTouch(-0.102, 0.0656, -335, 10);


	/* Draw the background */
	DrawInit(MyWatchScr);
	LCD_SetTextColor(Black);
	LCD_SetBackColor(Black);
	LCD_DrawFullRect(28, 62, 200, 56);
	//LCD_DrawFullRect(10, 130,30, 30);//UP
	//LCD_DrawFullRect(63, 126, 30, 30);//Down


	//Initialize LCD Control;
	Display *S=&A;
	Digit Hour={38, 70, 58, 70, 16, 48,names[0],names[0]};
	Digit Min ={99, 70, 119, 70, 16, 48,names[0],names[0]};
	Digit Sec ={168, 70, 188, 70, 16, 48,names[0],names[0]};
	S->_Hour=Hour;
	S->_Min=Min;
	S->_Sec=Sec;


	/* Program cyclic alarms which will fire after an initial offset,
	 * and after that periodically
	 * */
	SetRelAlarm(AlarmTaskLCD, 10, 50);//for cature the display events
	SetRelAlarm(AlarmTaskClock, 10, 100);//for clock FSM behavior

  /* Forever loop: background activities (if any) should go here */
	for (;;) {
	}

}


