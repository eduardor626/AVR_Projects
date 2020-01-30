/*	Author: Eduardo Rocha
 *  Partner(s) Name: Arturo Alvarado
 *	Lab Section:
 *	Assignment: Lab #6  Exercise # 2
 *	Exercise Description: 

 Create a simple light game that requires pressing a button on PA0 while 
 the middle of three LEDs on PB0, PB1, and PB2 is lit.The LEDs light for 
 300 ms each in sequence. When the button is pressed, the currently lit 
 LED stays lit. Pressing the button again restarts the game. 

 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#include <avr/interrupt.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

volatile unsigned char TimerFlag = 0;

//Internal variables for mapping AVR's ISR to our cleaner TimerISR model
unsigned long _avr_timer_M = 1;
unsigned long _avr_timer_cntcurr = 0;



enum STATES {START, RELEASE_STATE, PRESS_PINA0, PRESS_PINA1, RESET} STATE;

unsigned char temp = 0xFF;
unsigned char currentState = 0x00;
unsigned char tempState = 0x00;

void state_machine(){

	unsigned char tempA = ~PINA;

	switch(STATE)
	{
		//starting the initial state
		case START:
			PORTC = 0x02;
			STATE  = RELEASE_STATE;
			break;
		case RELEASE_STATE:
			if(tempA == 0x00){
				STATE = RELEASE_STATE;

			}else if(tempA == 0x01){
				STATE = PRESS_PINA0;
			}
			else if(tempA == 0x02){
				STATE = PRESS_PINA1;
			}
			else if(tempA == 0x03){
				PORTC = 0;
				STATE = RESET;
			}else {
				STATE = RELEASE_STATE;
			}
			break;
			
		case PRESS_PINA0:
			if(PORTC < 9){
				++PORTC;
			}
			if(tempA == 0x01){
				STATE = PRESS_PINA0;
			}
			else if(tempA == 0x00){
				STATE = RELEASE_STATE;
			}else if(tempA == 0x03){
				STATE = RESET;
				PORTC = 0;
			}else{
				STATE = PRESS_PINA0;
			}
			break;
		case PRESS_PINA1:
			if(PORTC != 0){
				--PORTC;
			}
			if(tempA == 0x02){
				STATE = PRESS_PINA1;
			}
			else if(tempA == 0x00){
				STATE = RELEASE_STATE;
			}
			else if(tempA == 0x03){
				STATE = RESET;
				PORTC = 0;
			}else{
				STATE = PRESS_PINA1;
			}
			break;

		case RESET:
			if(tempA == 0x00){
				STATE = RELEASE_STATE;
				PORTC = 0x00;
			}else {
				STATE = RESET;
			}
			break;
		default:
			break;
	}

}



void TimerOn(){
	//AVR timer/counter controller register TCCR1
	TCCR1B = 0x0B; //0000 1011 : 0x0B

	//AVR output compare register OCR1A
	OCR1A = 125;

	//AVR timer interrupt mask register
	TIMSK1 = 0x02;

	//Initialize avr counter
	TCNT1 = 0;

	_avr_timer_cntcurr = _avr_timer_M;
	//TimerISR will be called every _avr_timer_cntcurr millisec

	//enable global interrupts
	SREG |= 0x80; //0x80: 1000 0000

}

void TimerOff(){
	TCCR1B = 0x00; //bit3bit1bit0 = 000: timer off
}

void TimerISR(){
	TimerFlag = 1;

}

//In our approach the C programmer does not touch this ISR, only TimerISR
ISR(TIMER1_COMPA_vect){
	_avr_timer_cntcurr--;
	if(_avr_timer_cntcurr == 0){
		TimerISR(); //CALL THE ISR that the user uses
		_avr_timer_cntcurr = _avr_timer_M;
	}
}


//Set TimerISR() to tick every M ms
void TimerSet(unsigned long M){
	_avr_timer_M = M;
	_avr_timer_cntcurr = _avr_timer_M;
}



int main() {
	DDRA = 0X00; PORTA = 0xFF;
	DDRC = 0xFF; PORTC = 0x00;

	TimerSet(300);
	TimerOn();

	STATE = START;
	while(1) {
		state_machine();
		while(!TimerFlag);
		TimerFlag = 0;
	}

	return 0;
}
