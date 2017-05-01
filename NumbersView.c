#include <stdio.h>
#include "pictures.h"
#include "stm32f4_discovery_lcd.h"
#include "NumbersView.h"

const unsigned char  * names [] = {&Zero, &One, &Two, &Three, &Four, &Five, &Six, &Seven, &Eight, &Nine, &Points};

void DigitDraw(Digit* _digitdef){

	// GRphic The MSBit
	//LCD_DrawPicture(38, 70, 16, 48,names[i]);
	LCD_DrawPicture(_digitdef->Msxpos,_digitdef->Msypos,_digitdef->Width,_digitdef->Height,_digitdef->MSnumbervar);
	LCD_DrawPicture(_digitdef->Lsxpos,_digitdef->Lsypos,_digitdef->Width,_digitdef->Height,_digitdef->LSnumbervar);

};


void DigitDraw_Segment(Segment * _digitseg){

	// GRphic The MSBit
	//LCD_DrawPicture(38, 70, 16, 48,names[i]);
	LCD_DrawPicture(_digitseg->Msxpos,_digitseg->Msypos,_digitseg->Width,_digitseg->Height,_digitseg->Segment);

};



void DigitUpdate(Digit * _digitset, char *number){

	_digitset->MSnumbervar=names [number[1]];
	_digitset->LSnumbervar=names [number[0]];
	// GRphic The MSBit
	//LCD_DrawPicture(38, 70, 16, 48,names[i]);
	//LCD_DrawPicture(_digitdef->Msxpos,_digitdef->Msypos,_digitdef->Width,_digitdef->Height,_digitdef->MSnumbervar);
	//LCD_DrawPicture(_digitdef->Lsxpos,_digitdef->Lsypos,_digitdef->Width,_digitdef->Height,_digitdef->LSnumbervar);
};



Digit  getDigit (Display *_Cdigit, short _select){

	Digit to_Return = _Cdigit->_Sec;
	if (_select==0){
		to_Return = _Cdigit->_Hour;
	}
	if (_select==1){
		to_Return = _Cdigit->_Min;
		}
	return (to_Return);
};

void SetCurrentDigits (Display *_Cdigit, Digit * _setHour,Digit * _setMin, Digit * _setSec){
	_Cdigit->_Hour=*(_setHour);
	_Cdigit->_Min=*(_setMin);
	_Cdigit->_Sec=*(_setSec);

	//*(_Cdigit->_Hour)=(_setHour);
	//*(_Cdigit->_Min)=(_setMin);
	//*(_Cdigit->_Sec)=(_setSec);
};
