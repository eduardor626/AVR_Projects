#ifndef _DISPLAY_SCORE_H_
#define _DISPLAY_SCORE_H_


enum DisplayScores {DisplayScore_Start, DisplayScore_Init, Display_Scores, DisplayScore_Wait} DisplayScore;

int DisplayScoreSM(int DisplayScore)
{

	switch(DisplayScore)
	{
		case DisplayScore_Start:
			DisplayScore = DisplayScore_Init;
			break;
		case DisplayScore_Init:
			if(countdownComplete == 1){
				DisplayScore = Display_Scores;
            }
			break;
		case Display_Scores:
			LCD_Cursor(1);
			//LCD_DisplayString(1,"hello world");
			DisplayScore = DisplayScore_Wait;
			break;
		case DisplayScore_Wait:
			DisplayScore = DisplayScore_Wait;
			break;
		default:break;
	}
    return DisplayScore;
}



//DISPLAY SCORE END
#endif