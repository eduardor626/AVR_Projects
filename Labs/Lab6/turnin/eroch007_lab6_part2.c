/*	Author: Eduardo Rocha
 *  Partner(s) Name: Arturo Alvarado
 *	Lab Section:
 *	Assignment: Lab #6  Exercise # 2
 *	Exercise Description: Create a simple light game that requires pressing a button on PA0 while the middle of three LEDs on PB0, PB1, and PB2 is lit.The LEDs light for 300 ms each in sequence. When the button is pressed, the currently lit LED stays lit. Pressing the button again restarts the game. 

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

enum STATES{INIT, L1, L2, L3, PRESSED, WAIT, BACK} STATE;
unsigned char temp = 0xFF;
unsigned char buttonPressed = 0x00;

void state_machine(){

	switch(STATE)
	{
		case INIT:
			STATE = L1;
			break;
		case L1:
			if(buttonPressed == 0x01){
				STATE = PRESSED;
			}else{
				STATE = L2;
			}
			break;
		case L2:
			if(buttonPressed == 0x01){
				STATE = PRESSED;
			}else{
				STATE = L3;
			}
			break;
		case L3:
			if(buttonPressed == 0x01){
				STATE = PRESSED;
			}else{
				STATE = BACK;
			}
			break;

		case PRESSED:
			if(buttonPressed == 0x01){
				STATE = PRESSED;
			}
			else if(buttonPressed == 0x00){
				STATE = WAIT;
			}
			else{
				STATE = PRESSED;
			}
			break;
		case WAIT:
			if(buttonPressed == 0x00){
				STATE = WAIT;
			}else if(buttonPressed == 0x01){
				STATE = L1;
			}else{
				STATE = WAIT;
			}
			break;
		case BACK:
			if(buttonPressed == 0x01){
				STATE = PRESSED;
			}else{
				STATE = L1;
			}
			break;
		default:break;
	
	}

	switch(STATE)
	{
		case INIT:
			break;
		case L1:
			temp = 0x01;
			break;
		case L2:
			temp = 0x02;
			break;
		case L3:
			temp = 0x04;
			break;
		case PRESSED:
			temp = temp;
			break;
		case WAIT:
			temp = temp;
			break;
		case BACK:
			temp = 0x02;	
		default:break;

	}
	PORTC = temp;

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
    /* Insert DDR and PORT initializations */
	DDRC = 0xFF;//set port C to output
	PORTC = 0x00; //INIT port C to 0s
	
	//initializing the button press port
	DDRA = 0x00; //set port A to input
	PORTA = 0xFF; //INIT port A to be 1s

	TimerSet(300);
	TimerOn();
    while (1) {

	buttonPressed = ~PINA;
	state_machine();
	while(!TimerFlag);//wait 1 sec
	TimerFlag = 0;
    }
	//Note: For the above a better style would use a synchSM with TickSM()
	//This example just illustrates the use of the ISR and flag  
}
