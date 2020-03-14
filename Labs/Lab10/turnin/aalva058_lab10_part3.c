/*	Author: Arturo Alvarado
 *  Partner(s) Name: Eduardo Rocha
 *	Lab Section: 022 
 *	Assignment: Lab #10  Exercise #3
 *	
	Exercise Description: 
	To the previous exercises implementation, connect your speaker's red wire 
	to PB4 and black wire to ground. Add a third task that toggles PB4 on for 
	2 ms and off for 2 ms as long as a switch on PA2 is in the on position.
	
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */



#include <avr/io.h>
#include <avr/interrupt.h>
#include "timer.h"



unsigned char val_3_LEDS = 0x00;

enum STATES_LED { INIT, POSITION1, POSITION2, POSITION3 } LED_position;

void 3_LED_Function() {
	switch(LED_position){
		case INIT:	
			LED_position = POSITION1; 
			break;
		case POSITION1:	
			LED_position = POSITION2; 
			val_3_LEDS = 0x01; // 1st bit position
			break;
		case POSITION2:	
			LED_position = POSITION3; 
			val_3_LEDS = 0x02; // 2nd bit position
			break;
		case POSITION3:	
			LED_position = POSITION1; 
			val_3_LEDS = 0x04; // 3rd bit position
			break;
		default:	
			LED_position = INIT; 
			break;
	}
}


unsigned char alternating_LED = 0x00;

enum Alternating_Pattern { INIT, BIT_3_ON, BIT_3_OFF } Alternating_Pattern_State;

void 1_LED_Function() {
	switch(Alternating_Pattern_State){
		case INIT:	
			Alternating_Pattern_State = BIT_3_ON; 
			break;
		case BIT_3_ON:	
			Alternating_Pattern_State = BIT_3_OFF; 
			alternating_LED = 0x08; // 4th bit position	
			break;
		case BIT_3_OFF: 
			Alternating_Pattern_State = BIT_3_ON; 
			alternating_LED=0x00; 
			break;
		default:	
			LED_position=INIT; 
			break;
	}
	
}

unsigned char sound = 0x00;

enum Sound_Function{ INIT, NO_SOUND, ON } SOUND_FUNCTION_STATE;

void Sound_Generate(){
	
	unsigned char button = ~PINA & 0x01; // Only want the last bit on PINA
	
	switch( SOUND_FUNCTION_STATE){
		case INIT: 
			SOUND_FUNCTION_STATE = NO_SOUND;    
			break;
		case NO_SOUND: 
			SOUND_FUNCTION_STATE = (button) ? ON : NO_SOUND; 
			break;
		case ON: 
			SOUND_FUNCTION_STATE = NO_SOUND;  
			break;
		default: 
			break;
	}
	
	switch( SOUND_FUNCTION_STATE){
		case INIT:  
			break;
		case NO_SOUND:  
			sound = 0x00;  
			break;
		case ON:  
			sound = 0x10; // Sets B4 to 1  
			break;
		default: 
			break;
	}

}

enum Concurrent_SM { INIT, Start_Sequence } Concurrent_SM_states;

void Concurrent_Machine() {
	
	unsigned char Sequence = 0;
	
	switch(Concurrent_SM_states){
		case INIT:	
			Concurrent_SM_states = Start_Sequence; 
			break;
		case Start_Sequence:	
			Sequence = val_3_LEDS | alternating_LED | sound;	
			break;
		default:	
			Concurrent_SM_states = INIT; 
			break;
	}
	
	PORTB = Sequence;
}


// -----------------------------------------------------------------------------------------------------------------------------------
// Template from class

typedef struct task {
	int state;                  // Task's current state
	unsigned long period;       // Task period
	unsigned long elapsedTime;  // Time elapsed since last task tick
	int (*TickFct)(int);        // Task tick function
} task;

task tasks[4];
const unsigned short tasksNum = 4;


int main(void)
{
	/* Replace with your application code */
	/* Replace with your application code */
	DDRA=0x00; PORTA=0xFF;
	DDRB=0XFF; PORTB=0x00;

	
	TimerSet(2);
	TimerOn();
	
	unsigned char i = 0;
	// template from class
	tasks[i].state = INIT;
	tasks[i].period = 300;
	tasks[i].elapsedTime = tasks[i].period;
	tasks[i].TickFct = &3_LED_Function;
	i++;
	//template from class
	tasks[i].state = INIT;
	tasks[i].period = 1000;
	tasks[i].elapsedTime = tasks[i].period;
	tasks[i].TickFct = &1_LED_Function;
	i++;
	//template from class
	tasks[i].state = INIT;
	tasks[i].period = 2;
	tasks[i].elapsedTime = tasks[i].period;
	tasks[i].TickFct = &Sound_Function;
	i++;
	//template from class
	tasks[i].state = INIT;
	tasks[i].period = 2;
	tasks[i].elapsedTime = tasks[i].period;
	tasks[i].TickFct = &Concurrent_Machine;
	
	
	while (1)
	{
		//template from class
		for (i = 0; i < tasksNum; ++i) { 
			if ( tasks[i].elapsedTime >= tasks[i].period ) { 
				tasks[i].state = tasks[i].TickFct(tasks[i].state);
				tasks[i].elapsedTime = 0;
			}
			tasks[i].elapsedTime += 2;
		}
		
		while(!TimerFlag);
		TimerFlag=0;
	}
}
