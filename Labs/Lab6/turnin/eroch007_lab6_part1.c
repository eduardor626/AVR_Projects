/*	Author: aalva058
 *	Partner(s) Name: Eduardo Rocha
 *	Lab Section:022
 *	Assignment: Lab #6  Exercise #1
 * 	Exercise Description:
 *     
 *   	Create a synchSM to blink three LEDs connected to PB0, PB1, and PB2 in 
 *	sequence, 1 second each. Implement that synchSM in C using the method 
 *	defined in class. In addition to demoing your program, you will need 
 *      to show that your code adheres entirely to the method with no variations. 
 *	          
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 *	      
 */
#include <avr/io.h>
#include <avr/interrupt.h>

volatile unsigned char TimerFlag = 0;
unsigned char tempB = 0x00;

unsigned long _avr_timer_M = 1;
unsigned long _avr_timer_cntcurr = 0;

void TimerOn()
{
	TCCR1B = 0x0B;
	
	OCR1A = 125;
	
	TIMSK1 = 0x02;
	
	TCNT1 = 0;
	
	_avr_timer_cntcurr = _avr_timer_M;
	
	SREG |= 0x80;
	
}

void TimerOff(){
	TCCR1B = 0x00;
}

void TimerISR(){
	TimerFlag = 1;
}
ISR(TIMER1_COMPA_vect){
	_avr_timer_cntcurr--;
	if(_avr_timer_cntcurr == 0){
		TimerISR();
		_avr_timer_cntcurr = _avr_timer_M;
			
	}
}

void TimerSet(unsigned long M){
	_avr_timer_M = M;
	_avr_timer_cntcurr = _avr_timer_M;
}

enum STATES{START, B0, B1, B2}STATE;

void state_machine(){
	switch (STATE) 
	{// transitions	
		case START:// Initial state
			STATE = B0; 
			break;
		case B0:
			STATE = B1;
			break;
		case B1: 
			STATE = B2;
			break;
		case B2: 
			STATE = B0;
			break;
		default:
			STATE = START;
			break;
	}

	switch (STATE) 
	{ // actions
		case B0:
			tempB = 0x01; // First LED position
			break;
		case B1:
			tempB = 0x02; // Second LED position
			break;
		case B2:
			tempB = 0x04; // Third LED position
			break;
		default:
			break;
	}
	PORTB = tempB; // Sets PORTB to current LED
}


int main(void)
{
        DDRB = 0xFF;
	PORTB = 0X00;
	TimerSet(100);
	TimerOn();

	STATE = START;
	
    while (1) 
    {
	state_machine();
	while(!TimerFlag);
	TimerFlag = 0;
    }
}
