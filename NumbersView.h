#ifndef NVIEW_H_
#define NVIEW_H_

#include "Event.h"
#include "Touch.h"
#include "fonts.h"
#include "stm32f4_discovery_lcd.h"
#include "pictures.h"
char number[3];

extern const unsigned char *names[];

typedef struct
		{
unsigned short int Msxpos;
unsigned short int Msypos;
unsigned short int Lsxpos;
unsigned short int Lsypos;
unsigned short int Width;
unsigned short int Height;
const unsigned char *MSnumbervar;
const unsigned char *LSnumbervar;
}Digit;

typedef struct{
	Digit  _Hour;
	Digit  _Min;
	Digit  _Sec;
}Display;



typedef struct
		{
unsigned short int Msxpos;
unsigned short int Msypos;
unsigned short int Width;
unsigned short int Height;
const unsigned char *Segment;
}Segment;



void DigitDraw(Digit* _digitdef);
void  DigitDraw_Segment(Segment * _digitseg);
void DigitUpdate(Digit * _digitset, char *number);
Digit  getDigit (Display *_Cdigit, short _select);
void SetCurrentDigits (Display *_Cdigit,Digit * _setHour,Digit * _setMin, Digit * _setSec);
#endif


