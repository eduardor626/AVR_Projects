#ifndef _DISPLAY_SCORE_H_
#define _DISPLAY_SCORE_H_

/**
*
*	This header file takes care of the displaying of scores on the LCD Display.
*  
*
*/

enum DisplayScores {DisplayScore_Start, DisplayScore_Init, Display_Scores, DisplayScore_Wait,DisplayScore_Menu} DisplayScore;

unsigned char currentScore = 0x00;
unsigned char highScore;

//Function to update the score
void updateScore(){

	if(GameOver == 0 && countdownComplete == 1){
		currentScore++;
	}
	if(currentScore >= highScore){
		highScore = currentScore;
		newHighScoreFlag = 1;
		eeprom_update_byte((const char*) 1,highScore);

	}
	return;
}


void printScore(uint8_t hiScore, uint8_t currScore){

		//buffer variables so we can create strings
		unsigned char buffer[16];
		unsigned char buff[16];

		if ((hiScore / 10) > 0) {
				sprintf(buffer, ">Hi Score: %d", hiScore);
		} else {
				sprintf(buffer, ">Hi Score: %d", hiScore);
		}

		LCD_DisplayString(1, buffer);
		LCD_Cursor(17);

		if ((currScore / 10) > 0 ){
			sprintf(buff, " Score: %d", currScore);
		} else {
			sprintf(buff, " Score: %d", currScore);
		}
		LCD_DisplayThis(17,buff);
}

void readValue(){
		if (eeprom_read_byte((const char*) 1) != 0xFF) {
			highScore = eeprom_read_byte((const char*) 1);
		}else{
			highScore = 0;
		}
}


int DisplayScoreSM(int DisplayScore)
{	
	unsigned char readMe = (~PINA & 0x03);

	switch(DisplayScore)
	{
		case DisplayScore_Start:
			DisplayScore = DisplayScore_Init;
			break;
		case DisplayScore_Init:
			if(countdownFrom == 3){
				readValue();
				if(highScore == 0){
					eeprom_update_byte((const char *) 1, highScore);
					highScore = 0;
				}
				DisplayScore = Display_Scores;
				printScore(highScore,currentScore);
				break;
            }else{
            	DisplayScore = DisplayScore_Init;
            }
			break;
		case Display_Scores:
			if(readMe == 0x01 || readMe == 0x02){
				updateScore();
				DisplayScore = DisplayScore_Wait;
			}else{
				DisplayScore = Display_Scores;
			}
			break;
		case DisplayScore_Wait:
			if(readMe == 0x00){
				printScore(highScore,currentScore);
				DisplayScore = Display_Scores;
			}
			if(GameOver ==1 ){
				DisplayScore = DisplayScore_Menu;
			}
			break;
		case DisplayScore_Menu:
			if(readMe == 0x03){
				printScore(highScore,currentScore);
			}
			DisplayScore = DisplayScore_Menu;
		default:break;
	}
    return DisplayScore;
}



//DISPLAY SCORE END
#endif