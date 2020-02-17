/*	Author: Arturo Alvarado
 *  Partner(s) Name: Eduardo Rocha
 *	Lab Section: 022 
 *	Assignment: Lab #10  Exercise #4
 *	
	Exercise Description: 
	Allow a user to adjust the sound frequency up or down using buttons 
	connected to PA0 (up) and PA1 (down). Using our 1 ms timer abstraction, 
	the fastest you'll be able to pulse is 1 ms on and 1 ms off, meaning 500 Hz.
	
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */



#include <avr/io.h>
#include <avr/interrupt.h>
#include "timer.h"



unsigned char val_3_LEDS = 0x00;

enum STATES_LED { START1, POSITION1, POSITION2, POSITION3 } LED_position;

void Three_LED_Function() {
	
	switch(LED_position){
		case START1:	
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
			LED_position = START1; 
			break;
	}
}


unsigned char alternating_LED = 0x00;

enum Alternating_Pattern { START2, BIT_3_ON, BIT_3_OFF } Alternating_Pattern_State;

void One_LED_Function() {
	
	switch(Alternating_Pattern_State){
		case START2:	
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
			LED_position=START2; 
			break;
	}
	
}

unsigned char sound = 0x00;

enum Sound_Function{ START3, NO_SOUND, ON } SOUND_FUNCTION_STATE;
	unsigned char i;
	double frequency[8] = {261.63,239.66,329.63,349.23,392.00,440.00,493.88,523.25};

void Sound_Generate(){
	
	unsigned char button = ~PINA & 0x07; // Only want the last bit on PINA
	
	switch( SOUND_FUNCTION_STATE){
		case START3: 
			i = 0x00;
			SOUND_FUNCTION_STATE = NO_SOUND;    
			break;
		case NO_SOUND: 
			if(button == 0x00){
				SOUND_FUNCTION_STATE = NO_SOUND;
			} 
			else if(button == 0x01){
				SOUND_FUNCTION_STATE = ON;
			}
			else if(button == 0x02){
				if(i < 0x07){
					i++;
				}
			}
			else if(button == 0x04){
				if(button > 0x00){
					i--;
				}
			}
			break;
		case ON: 
			SOUND_FUNCTION_STATE = NO_SOUND;  
			break;
		default: 
			break;
	}
	
	switch( SOUND_FUNCTION_STATE){
		case START3:  
			break;
		case NO_SOUND:  
			set_PWM(0.00); 
			break;
		case ON:  
			set_PWM(frequency[i]);
			break;
		default: 
			break;
	}

}
void set_PWM(double frequency){
	static double current_frequency;

	if(frequency != current_frequency){
		if(!frequency) {TCCR3B &= 0x08;}
		else{TCCR3B |= 0x03;}

		if(frequency < 0.954){OCR3A = 0xFFFF;}

		else if(frequency > 31250) {OCR3A = 0x0000;}

		else {OCR3A = (short)(8000000 / (128*frequency))-1;}

		TCNT3 = 0;
		current_frequency = frequency;
	}

}
void PWM_on(){
	TCCR3A = (1 << COM3A0);
	TCCR3B = (1 << WGM32) | (1 << CS31) | (1 << CS30);
	set_PWM(0);

}

void PWM_off(){
	TCCR3A = 0x00;
	TCCR3B = 0x00;

}

enum Concurrent_SM { INIT, Start_Sequence } Concurrent_SM_states;

void Concurrent_Machine() {
	
	unsigned char Sequence = 0;
	
	switch(Concurrent_SM_states){
		case INIT:	
			Concurrent_SM_states = Start_Sequence; 
			break;
		case Start_Sequence:	
			Sequence = val_3_LEDS | alternating_LED ;	
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
	PWM_on();
	
	unsigned char i = 0;
	// template from class
	tasks[i].state = START1;
	tasks[i].period = 300;
	tasks[i].elapsedTime = tasks[i].period;
	tasks[i].TickFct = &Three_LED_Function;
	i++;
	//template from class
	tasks[i].state = START2;
	tasks[i].period = 1000;
	tasks[i].elapsedTime = tasks[i].period;
	tasks[i].TickFct = &One_LED_Function;
	i++;
	//template from class
	tasks[i].state = START3;
	tasks[i].period = 2;
	tasks[i].elapsedTime = tasks[i].period;
	tasks[i].TickFct = &Sound_Generate;
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
