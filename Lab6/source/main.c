/*	Author: Eduardo Rocha
 *  Partner(s) Name: Arturo Alvarado
 *	Lab Section:
 *	Assignment: Lab #  Exercise #
 *	Exercise Description: [optional - include for your own benefit]
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
    /* Insert DDR and PORT initializations */
	DDRB = 0xFF;//set port B to output
	PORTB = 0x00; //INIT port B to 0s
	
	TimerSet(1000);
	TimerOn();
	unsigned char tmpB = 0x00;
    while (1) {
	tmpB = ~tmpB;
	PORTB = tmpB;
	while(!TimerFlag);//wait 1 sec
	TimerFlag = 0;
    }
	//Note: For the above a better style would use a synchSM with TickSM()
	//This example just illustrates the use of the ISR and flag  
}
