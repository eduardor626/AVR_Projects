#ifndef _DISPLAY_STOP_CLOCK_H_
#define _DISPLAY_STOP_CLOCK_H_


enum DisplayStopClocks {STOPCLOCK_START, STOPCLOCK_INIT,
						STOPCLOCK_WAIT,STOPCLOCK_DECREMENT, STOPCLOCK_RESET} StopClock;



unsigned char speedLeft;
unsigned char fiveTicks = 0x00;
unsigned char waitForNextGame = 0x00;


void printNokia(){
	unsigned char buffer[5] = {};
	nokia_lcd_clear();
	nokia_lcd_set_cursor(1,5);
    nokia_lcd_write_string("TIME REMAINING",1);
    sprintf(buffer, " %d", speedLeft);
    nokia_lcd_set_cursor(13, 15);
    nokia_lcd_write_string(buffer, 4);
    nokia_lcd_render();
}

void printPressStart(){

	unsigned char buffer[30];
	nokia_lcd_clear();
	nokia_lcd_set_cursor(0,0);
    nokia_lcd_write_string("    Try Again?  ",1);
    nokia_lcd_set_cursor(3,2);
    nokia_lcd_write_string("     press:",1);
    nokia_lcd_set_cursor(6,2);
    nokia_lcd_write_string("START",2);

    //sprintf(buffer, " %d", speedLeft);
    //nokia_lcd_set_cursor(13, 15);
    //nokia_lcd_write_string(buffer, 4);
    nokia_lcd_render();

}


int DisplayStopClockSM (int StopClock){

	unsigned char buttonPress = (~PINA & 0x03);


	switch(StopClock)
	{
		case STOPCLOCK_START:
			StopClock = STOPCLOCK_INIT;
			break;
		case STOPCLOCK_INIT:
			//declare our variables 
			if(countdownFrom == 5 && !GameOver){
				StopClock = STOPCLOCK_WAIT;
				speedLeft = 10;
				fiveTicks = 0;
				printNokia();
			}

			break;
		case STOPCLOCK_WAIT:
			if(buttonPress == 0x00 && countdownFrom == 0){
				StopClock = STOPCLOCK_DECREMENT;
			}
			else if(buttonPress == 0x01 || buttonPress ==0x02){
				StopClock = STOPCLOCK_WAIT;
			}else if(GameOver == 1){
				StopClock = STOPCLOCK_RESET;
			}
			else{
				StopClock = STOPCLOCK_WAIT;
			}
			break;
		case STOPCLOCK_DECREMENT:
			if(buttonPress == 0x00 && !GameOver){
				if(fiveTicks > 5 && speedLeft >0){
					speedLeft--;
					printNokia();
					StopClock = STOPCLOCK_WAIT;
					fiveTicks = 0;
				}
				if(speedLeft == 0){
					StopClockZero = 1;
					StopClock = STOPCLOCK_RESET;
				}
			}else if(speedLeft == 0){
				StopClock = STOPCLOCK_RESET;
				StopClockZero = 1;
				fiveTicks = 0;
			}else if(GameOver == 1 || StopClockZero == 1){
				StopClock = STOPCLOCK_RESET;

			}else{
				StopClock = STOPCLOCK_WAIT;
				fiveTicks = 0;
			}
			fiveTicks++;
			break;
		case STOPCLOCK_RESET:
				if(Start == 1 || Reset == 1){
					StopClockZero = 0;
					speedLeft = 10;
					StopClock = STOPCLOCK_INIT;
				}else{
					printPressStart();
					StopClock = STOPCLOCK_RESET;

				}
			break;
		default:break;
	}

	return StopClock;

}






//DISPLAY STOP CLOCK
#endif