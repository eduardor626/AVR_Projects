/*	Author: Eduardo Rocha
 *  Partner(s) Name: Arturo Alvarado
 *	Lab Section:
 *	Assignment: Lab #11  Exercise #2
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#include "../header/keypad.h"
#include "../header/timer.h"
#include "../header/scheduler.h"
#include "../header/io.h"

enum keypad_STATES {start} keypad_state;
	
int keypad_tick(int keypad_state){
	unsigned char x = GetKeypadKey();
	switch(keypad_state){
		case start: 
			keypad_state = start;
			switch(x) {
				case '\0': PORTB = 0x00; break;
				case '1': PORTB = 0x01; break;
				case '2': PORTB = 0x02; break;
				case '3': PORTB = 0x03; break;
				case 'A': PORTB = 0x0A; break;
				case '4': PORTB = 0x04; break;
				case '5': PORTB = 0x05; break;
				case '6': PORTB = 0x06; break;
				case 'B': PORTB = 0x0B; break;
				case '7': PORTB = 0x07; break;
				case '8': PORTB = 0x08; break;
				case '9': PORTB = 0x09; break;
				case 'C': PORTB = 0x0C; break;
				case '*': PORTB = 0x0E; break;
				case '0': PORTB = 0x00; break;
				case '#': PORTB = 0x0F; break;
				case 'D': PORTB = 0x0D; break;
				default: PORTB = 0x1B; break;
			}
			break;
		
		default:
			keypad_state = start;
			break;
	}
	return keypad_state;
}

char *name = "CS120B is Legend... wait for it DARY!";
//there are 38 characters in the string
unsigned char count = 0;
unsigned char output[17]; //character array that will hold the output
enum Lcd_States{START, PRINT} LCD_STATE;
int LCD_tick(int LCD_STATE)
{
	switch(LCD_STATE)
	{
		case START:
			LCD_STATE = PRINT;
			break;
		case PRINT:
			LCD_STATE = PRINT;
			for (int j = 0; j < 16; ++j)
  			{
    			output[j] = name[(count+j) % 38];
  			}
  			count = (count + 1);
  			LCD_DisplayString(1, output);
			break;
	}
	return LCD_STATE;


}

task task2;

int main(void)
{
	DDRB = 0xFF; PORTB = 0x00;
	DDRC = 0xFF; PORTC = 0x00;
	DDRD = 0xFF; PORTD = 0x00;
	
	task *tasks[] = {&task2};
	const unsigned short numTasks = sizeof(tasks)/sizeof(task*);

	
	//Task 1
	//task1.state = start;
	//task1.period = 50;
	//task1.elapsedTime = task1.period;
	//task1.TickFct = &keypad_tick;

	//Task2
	task2.state = START;
	task2.period = 1000;
	task2.elapsedTime = task2.period;
	task2.TickFct = &LCD_tick;
	
	TimerSet(1000);
	TimerOn();
	LCD_init();



	
	unsigned short i;
    /* Replace with your application code */
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

