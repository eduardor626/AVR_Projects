/*	Author: Eduardo Rocha
 *  Partner(s) Name: Arturo Alvarado
 *	Lab Section:
 *	Assignment: Lab #9  Exercise #2
 *	Exercise Description: 
	
	Using the ATmega1284’s PWM functionality, design a system where the notes: 
	C4, D, E, F, G, A, B, and C5,  from the table at the top of the lab, can be 
	generated on the speaker by scaling up or down the eight note scale. 

	Three buttons are used to control the system. One button toggles sound on/off. 
	The other two buttons scale up, or down, the eight note scale. Criteria:
	The system should scale up/down one note per button press.
	When scaling down, the system should not scale below a C.
	When scaling up, the system should not scale above a C.
	
	Hints:
	Breaking the system into multiple synchSMs could make this part easier.




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


//Enables the ATmega1284’s PWM functionality. 
void PWM_on(){
	TCCR3A = (1 << COM3A0);
	TCCR3B = (1 << WGM32) | (1 << CS31) | (1 << CS30);
	set_PWM(0);

}
//Disables the ATmega1284’s PWM functionality. 
void PWM_off(){
	TCCR3A = 0x00;
	TCCR3B = 0x00;

}


enum STATES {INIT, SILENT, ON , UP, DOWN} STATE;

unsigned char tempA;
unsigned char i;
double frequency[8] = {261.63,239.66,329.63,349.23,392.00,440.00,493.88,523.25};


void state_machine(){

	tempA = ~PINA & 0x07;

	switch(STATE)
	{
		case INIT:
			STATE = SILENT;
			break;
		case SILENT: 
			i = 0;
			set_PWM(0.00);
			if(tempA == 0x00){
				STATE = SILENT;
			}else if(tempA == 0x01){
				STATE = ON;
			}else{
				STATE = SILENT;
			}
			break;
		case ON:
			set_PWM(frequency[i]);
			if(tempA == 0x01){
				STATE = SILENT;
			}else if(tempA == 0x02){
				STATE = UP;
			}else if(tempA == 0x04){
				STATE = DOWN;
			}
			else{
				STATE = ON;
			}
			break;
		case  UP:
			if(tempA == 0x00){
				STATE = ON;
				if(i < 7){
					i++;
				}

			}else if(tempA == 0x01){
				STATE = SILENT;
			}
			else{
				STATE = UP;
			}
			break;
		case DOWN:
			if(tempA == 0x00){
				STATE = ON;
				if(i > 0){
					i--;
				}
			}else if(tempA == 0x01){
				STATE = SILENT;
			}
			else{
				STATE = DOWN;
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
