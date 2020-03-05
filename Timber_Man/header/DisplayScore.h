#ifndef _DISPLAY_SCORE_H_
#define _DISPLAY_SCORE_H_


enum DisplayScores {DisplayScore_Start, DisplayScore_Init, Display_Scores, DisplayScore_Wait,DisplayScore_Menu} DisplayScore;
unsigned char currentScore = 0x00;
unsigned char highScore;

void updateScore(){
	unsigned char readInput = (~PINA & 0x03);
	if(GameOver == 0 && countdownComplete == 1 && (readInput == 0x01 || readInput == 0x02)){
		currentScore++;
	}
	if(currentScore >= highScore){
		highScore = currentScore;
		eeprom_update_byte((const char*) 1,highScore);

	}

	return;
}

void readValue(){
		if (eeprom_read_byte((const char*) 1) != 0xFF) {
			highScore = eeprom_read_byte((const char*) 1);
		}
}


int DisplayScoreSM(int DisplayScore)
{	
	unsigned char buffer[2];
	unsigned char buffer2[2];

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
					highScore = 2;
				}else if(currentScore > highScore){
					highScore = currentScore;
					eeprom_update_byte((const char *) 1, currentScore);
	
				}else{
					highScore = 3;
				}
				DisplayScore = Display_Scores;
            }else{
            	DisplayScore = DisplayScore_Init;
            }
			break;
		case Display_Scores:
			//LCD_Cursor(1);
			readValue();
			updateScore();
			//sprintf(buffer, "> Hi Score : %d", highScore); 
			LCD_DisplayThis(1,"> Hi Score: ");
			LCD_WriteData(highScore+'0');
			LCD_Cursor(17);
			//sprintf(buffer2," Score: %d",currentScore);
			LCD_DisplayThis(17," Score: ");
			LCD_WriteData(currentScore+'0');
			DisplayScore = DisplayScore_Wait;
			break;
		case DisplayScore_Wait:
			readValue();
			updateScore();
			DisplayScore = Display_Scores;
			if(GameOver ==1 ){
				DisplayScore = DisplayScore_Menu;
			}
			break;
		case DisplayScore_Menu:
			updateScore();
			DisplayScore = DisplayScore_Menu;
		default:break;
	}
    return DisplayScore;
}



//DISPLAY SCORE END
#endif