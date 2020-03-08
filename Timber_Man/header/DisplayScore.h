#ifndef _DISPLAY_SCORE_H_
#define _DISPLAY_SCORE_H_

/**
*
*	This header file takes care of the displaying of scores on the LCD Display.
*  
*
*/

enum DisplayScores {DisplayScore_Start, DisplayScore_Init, Display_Scores, DisplayScore_Wait,
					DisplayScore_Menu, DisplayScore_wait4Release, DisplayScore_Choice, DisplayScore_SelectPressed ,
					DisplayScore_WaitRight, DisplayScore_CheckDeleteFlag, DisplayScore_WaitForSelect, DisplayScore_WaitForChoice} DisplayScore;

unsigned char currentScore = 0x00;
unsigned char highScore;

//Function to update the score
void updateScore(){

	if(GameOver == 0 && StopClockZero == 0 && countdownComplete == 1){
		currentScore++;
	}
	if(currentScore >= highScore){
		highScore = currentScore;
		newHighScoreFlag = 1;
		eeprom_update_byte((const char*) 1,highScore);

	}else{
		newHighScoreFlag = 0;
	}
	return;
}


void deleteHighScore(){
	if(deleteHighScoreFlag == 1){
		highScore = 0;
		eeprom_update_byte((const char*) 1,highScore);
		deleteHighScoreFlag = 0;
	}
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


void deleteHighMenuNo(){
	LCD_DisplayString(1,"Delete Hi Score?");
	LCD_DisplayThis(17," Yes      >No");
}

void deleteHighMenu(){
	LCD_DisplayString(1,"Delete Hi Score?");
	LCD_DisplayThis(17,">Yes       No");
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
	unsigned char readMe = (~PINA & 0x0F);

	switch(DisplayScore)
	{
		case DisplayScore_Start:
			DisplayScore = DisplayScore_Init;
			break;
		case DisplayScore_Init:
			if(countdownFrom == 5){
				readValue();
				if(highScore == 0){
					eeprom_update_byte((const char *) 1, highScore);
					highScore = 0;
				}
				currentScore = 0;
				DisplayScore = Display_Scores;
				printScore(highScore,currentScore);
				break;
            }else{
            	DisplayScore = DisplayScore_Init;
            	currentScore = 0;
            }
			break;
		case Display_Scores:
			if(readMe == 0x01 || readMe == 0x02){
				updateScore();
				DisplayScore = DisplayScore_Wait;
			}else if(GameOver == 1 || StopClockZero == 1){
				DisplayScore = DisplayScore_Menu;
			}
			else{
				DisplayScore = Display_Scores;
			}
			break;
		case DisplayScore_Wait:
			if(readMe == 0x00){
				printScore(highScore,currentScore);
				DisplayScore = Display_Scores;
			}
			if(GameOver == 1  || StopClockZero == 1){
				DisplayScore = DisplayScore_Menu;
			}
			break;
		case DisplayScore_Menu:
			if(Start == 1 || Reset == 1){
				DisplayScore = DisplayScore_Start;
				currentScore = 0;
			}else if(( GameOver == 1 || StopClockZero == 1) && readMe == 0x08){
				deleteHighMenu();
				deleteHighScoreFlag = 1;
				DisplayScore = DisplayScore_wait4Release;

			}
			else{
				DisplayScore = DisplayScore_Menu;
			}
			break;

		case DisplayScore_wait4Release:
			if(readMe == 0x00){
				DisplayScore = DisplayScore_Choice;
			}else if(readMe == 0x08){
				DisplayScore = DisplayScore_wait4Release;
			}
			else{
				DisplayScore = DisplayScore_wait4Release;
			}
			break;
		case DisplayScore_Choice:
			if(readMe == 0x08){
				DisplayScore = DisplayScore_SelectPressed;
			}else if(readMe == 0x02){
				deleteHighScoreFlag = 1;
				DisplayScore = DisplayScore_Choice;
			}else if(readMe == 0x01){
				DisplayScore = DisplayScore_WaitRight;
				deleteHighScoreFlag = 0;
			}else{
				DisplayScore = DisplayScore_Choice;
			}
			break;
		case DisplayScore_SelectPressed:
			if(readMe == 0x08){
				DisplayScore = DisplayScore_SelectPressed;
			}else if(readMe == 0x00){
				DisplayScore = DisplayScore_CheckDeleteFlag;
			}else{
				DisplayScore = DisplayScore_SelectPressed;
			}
			break;
		case DisplayScore_WaitRight:
			if(readMe == 0x01){
				DisplayScore = DisplayScore_WaitRight;
			}else if(readMe == 0x00){
				deleteHighMenuNo();
				DisplayScore = DisplayScore_WaitForSelect;
			}else{
				DisplayScore = DisplayScore_WaitRight;
			}
			break;
		case DisplayScore_WaitForSelect:
			if(readMe == 0x01){
				DisplayScore = DisplayScore_WaitRight;
			}else if(readMe == 0x02){
				DisplayScore = DisplayScore_Choice;
				deleteHighScoreFlag = 1;
				deleteHighMenu();
			}else if(readMe == 0x08){
				DisplayScore = DisplayScore_SelectPressed;
			}else{
				DisplayScore = DisplayScore_WaitForSelect;
			}
			break;
		case DisplayScore_CheckDeleteFlag:
			if(deleteHighScoreFlag == 0){
				currentScore = 0;
				printScore(highScore, currentScore);
				DisplayScore = DisplayScore_Menu;
			}else if(deleteHighScoreFlag == 1){
				deleteHighScore();
				currentScore = 0;
				printScore(highScore,currentScore);
				DisplayScore = DisplayScore_Menu;
			}else{
				DisplayScore = DisplayScore_CheckDeleteFlag;
			}
			break;
			// if(readMe == 0x01){
			// 	deleteHighMenuNo();
			// 	deleteHighScoreFlag = 0;
			// 	DisplayScore = DisplayScore_WaitForChoice;
			// }else if(readMe == 0x02){
			// 	deleteHighMenu();
			// 	deleteHighScoreFlag = 1;
			// 	DisplayScore = DisplayScore_WaitForChoice;
			// }else if(readMe == 0x08){
			// 	//deleteHighMenu();
			// 	deleteHighScore();
			// 	currentScore = 0;
			// 	printScore(highScore,currentScore);
			// }else if(Start == 1 || Reset == 1){
			// 	currentScore = 0;
			// 	deleteHighScoreFlag = 0;
			// 	DisplayScore = DisplayScore_Start;

			// }else{
			// 	DisplayScore = DisplayScore_Choice;
			// }
			//break;
		default:break;
	}
    return DisplayScore;
}



//DISPLAY SCORE END
#endif