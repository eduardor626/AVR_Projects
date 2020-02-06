/*	Author: Eduardo Rocha
 *  Partner(s) Name: Arturo Alvarado
 *	Lab Section:
 *	Assignment: Lab #9  Exercise #1
 *	Exercise Description: 
	
	Using the ATmega1284’s PWM functionality, design a system that uses three buttons 
	to select one of three tones to be generated on the speaker. When a button is pressed, 
	the tone mapped to it is generated on the speaker. 

	Criteria:
	Use the tones C4, D4, and E4 from the table in the introduction section.
	When a button is pressed and held, the tone mapped to it is generated on the speaker.
	When more than one button is pressed simultaneously, the speaker remains silent. 
	When no buttons are pressed, the speaker remains silent.



 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */



#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif


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


enum STATES {INIT, SILENT, C4, D4, E4} STATE;

unsigned char tempA;


void state_machine(){

	tempA = ~PINA & 0x07;

	switch(STATE)
	{
		case INIT:
			STATE = SILENT;
			break;
		case SILENT: 
			set_PWM(0.00);
			if(tempA == 0x00){
				STATE = SILENT;
				set_PWM(0.00);
			}else if(tempA == 0x01){
				STATE = C4;
			}else if(tempA == 0x02){
				STATE = D4;
			}else if(tempA == 0x04){
				STATE = E4;
			}else{
				STATE = SILENT;
			}
			break;
		case C4:
			if(tempA == 0x01){
				STATE = C4;
				set_PWM(261.63);
			}else{
				STATE = SILENT;
			}
			break;
		case D4:
			if(tempA == 0x02){
				STATE = D4;
				set_PWM(293.66);
			}else{
				STATE = SILENT;
			}
			break;
		case E4:
			if(tempA == 0x04){
				STATE = E4;
				set_PWM(329.63);
			}else{
				STATE = SILENT;
			}
			break;
		default: break;
	}

}


int main(void) {
    /* Insert DDR and PORT initializations */

	//INPUTS FROM THE BUTTONS 
	DDRA = 0x00; PORTA = 0xFF; 

	//OUPUT FOR THE SPEAKER
	DDRB = 0xFF; PORTB = 0x00;
  
	PWM_on();
	STATE = INIT;

    while (1) {
    	state_machine();
    }
    return 1;
}
