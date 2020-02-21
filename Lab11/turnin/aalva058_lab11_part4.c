#include <avr/io.h>
#include "keypad.h"
#include "timer.h"
#include "scheduler.h"
#include "io.h"



unsigned char character = 0x00; //used between LCD and Keypad
unsigned char position_i = 0x00;
unsigned char pause_state=0;

enum DAIL_STATES {INIT} dail_state;
int dail_tick(int dail_states){
	unsigned char x =  GetKeypadKey();
	switch(dail_states){
		case INIT:
		dail_states = INIT;
		switch(x) {
			case '\0': 
				character = character; 
				pause_state=0; 
				break;
			case '0':
				character = 0x00;
				pause_state=1;
				break;
			case '1': 
				character = 0x01; 
				pause_state=1; 
				break;
			case '2': 
				character = 0x02; 
				pause_state=1; 
				break;
			case '3': 
				character = 0x03; 
				pause_state=1; 
				break;
			case '4': 
				character = 0x04; 
				pause_state=1; 
				break;
			case '5': 
				character = 0x05; 
				pause_state=1; 
				break;
			case '6': 
				character = 0x06; 
				pause_state=1; 
				break;
			case '7': 
				character = 0x07; 
				pause_state=1; 
				break;
			case '8': 
				character = 0x08; 
				pause_state=1; 
				break;
			case '9': 
				character = 0x09; 
				pause_state=1; 
				break;
			case '*': 
				character = 0x2A - '0'; 
				pause_state=1; 
				break;
			case 'A': 
				character = (0x41 - '0'); 
				pause_state=1; 
				break;
			case 'B': 
				character = (0x42 - '0'); 
				pause_state=1; 
				break;
			case 'C':
				character = (0x43 - '0');
				pause_state=1;
				break;
			case 'D':
				character = (0x44 - '0');
				pause_state=1;
				break;
			case '#': 
				character = 0x23 - '0'; 
				pause_state=1; 
				break;
			default: 
				character = 0x1B; 
				break;
		}
		break;
		
		default:
		dail_states = INIT;
		break;
	}
	return dail_states;
}
enum DISPLAY_STATES {DISPLAY, WAIT} DISPLAY_STATE;


int Display_Function(DISPLAY_STATE)
{
	switch(DISPLAY_STATE)
	{
		case DISPLAY:

		if (position_i>32){
			position_i=0;
			DISPLAY_STATE=DISPLAY;
		}else{
			LCD_Cursor(position_i);
			LCD_WriteData(character+'0');
			++position_i;
			pause_state=0;
			DISPLAY_STATE=WAIT;
		}
		break;
		
		case WAIT:	
			DISPLAY_STATE = (pause_state==0) ? WAIT : DISPLAY;
				
		default:
			break;

	}
	return DISPLAY_STATE;
}

//setting up global task variables
task task1, task2;

int main(void)
{
	DDRA = 0xF0; PORTA = 0x0F; //keypad 
	DDRB = 0xFF; PORTB = 0x00; //test port 
	DDRC = 0xFF; PORTC = 0x00; //lcd 
	DDRD = 0xFF; PORTD = 0x00;
	
	task *tasks[] = {&task1, &task2};
	const unsigned short numTasks = sizeof(tasks)/sizeof(task*);

	
	//Task 1
	task1.state = INIT;
	task1.period = 50;
	task1.elapsedTime = task1.period;
	task1.TickFct = &dail_tick;

	//Task2
	task2.state = DISPLAY;
	task2.period = 50;
	task2.elapsedTime = task2.period;
	task2.TickFct = &Display_Function;
	
	TimerSet(200);
	TimerOn();
	LCD_init();
	LCD_DisplayString(1, "Our final demo!!!!!!!!!!!!!");
	LCD_Cursor(1);

	
	unsigned short i;
	while (1)
	{
		for(i = 0; i < numTasks; i++){
			if(tasks[i]->elapsedTime == tasks[i]->period){
				tasks[i]->state = tasks[i]->TickFct(tasks[i]->state);
				tasks[i]->elapsedTime = 0;
			}
			tasks[i]->elapsedTime += tasks[i]->period;
		}
		while(!TimerFlag);
		TimerFlag = 0;
	}
	return 0;
}

