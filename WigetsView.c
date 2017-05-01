#include "WidgetConfig.h"
#include "WigetsView.h"

void UpdateMode(unsigned char om, Widget * _MyWatchScr)
{

	DrawOff(&_MyWatchScr[BALARM]);
	DrawOff(&_MyWatchScr[BTIMESET]);
	DrawOff(&_MyWatchScr[BTIME]);
	DrawOff(&_MyWatchScr[BSWATCH]);
	//DrawOff(&_MyWatchScr[SWCHRONOPAUSE]);
	//DrawOff(&_MyWatchScr[SWCHRONOPLAY]);
	//DrawOff(&_MyWatchScr[SWCHRONOSTOP]);

	switch(om) {
	case WCLOCKMODE:
		//DrawOff(&MyWatchScr[BTIMESET]);
		DrawOn(&_MyWatchScr[BTIME]);
		break;
	case WSETMODE:
		//DrawOff(&MyWatchScr[BTIMESET]);
		DrawOn(&_MyWatchScr[BTIMESET]);
		break;
	case WALARMMODE:
		//DrawOff(&MyWatchScr[BTIMESET]);
		DrawOn(&_MyWatchScr[BALARM]);
		break;
	case WCRONOMODE:
		//DrawOff(&MyWatchScr[BTIMESET]);
		DrawOn(&_MyWatchScr[BSWATCH]);
		break;
	}
}
