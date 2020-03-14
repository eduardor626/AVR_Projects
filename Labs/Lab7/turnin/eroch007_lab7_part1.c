/*	Author: Eduardo Rocha
 *  	Partner(s) Name: Arturo Alvarado
 *	Lab Section:
 *	Assignment: Lab #7  Exercise #1
 *	Exercise Description: 

		
	Buttons are connected to PA0 and PA1. 
	Output PORTC and PORTD drive the LCD display, initially displaying 0. 
	Pressing PA0 increments the display (stopping at 9). 
	Pressing PA1 decrements the display (stopping at 0). If both buttons 
	are depressed (even if not initially simultaneously), the display resets	to 0. If a button is held, then the display continues to increment 
	(or decrement) at a rate of once per second. 

	Use a synchronous state machine captured in C.

 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#include "io.h" //for LCD functions
#include <avr/interrupt.h> //for TIMER functionality
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

enum STATES {START, RELEASE_STATE, PRESS_PINA0, PRESS_PINA1, RESET} STATE;

unsigned char temp=0;
unsigned char counter =0x00;

void state_machine(){

	unsigned char tempA = ~PINA; //reading from PINA (buttons)

	switch(STATE)
	{
		//starting the initial state
		case START:
			temp = 0;
			LCD_Cursor(1);
			LCD_WriteData(temp+'0');
			STATE  = RELEASE_STATE;
			break;
	
		//the wait for button to be pressed state
		case RELEASE_STATE:
			counter = 0x00;
			if(tempA == 0x00){
				STATE = RELEASE_STATE;

			}else if(tempA == 0x01){
				STATE = PRESS_PINA0;
				if(temp<9){
					++temp;
					LCD_Cursor(1);
					LCD_WriteData(temp+'0');
				}
			}
			else if(tempA == 0x02){
				STATE = PRESS_PINA1;
				if(temp >0){
					--temp;
					LCD_Cursor(1);
					LCD_WriteData(temp+'0');
				}
			}
			else if(tempA == 0x03){
				temp = 0;
				STATE = RESET;
			}else {
				STATE = RELEASE_STATE;
			}
			break;

		//increment is being pressed
		case PRESS_PINA0:
			if(tempA == 0x01){
				STATE = PRESS_PINA0;
				counter++; //counter that checks for 1sec
				if(counter > 10){
					counter = 0;
					++temp;
					LCD_Cursor(1);
					LCD_WriteData(temp+'0');

				}
			}
			else if(tempA == 0x00){
				STATE = RELEASE_STATE;
			}else if(tempA == 0x03){
				STATE = RESET;
				temp = 0;
			}else{
				STATE = PRESS_PINA0;
			}
			break;

		//decrement is being pressed
		case PRESS_PINA1:
			if(tempA == 0x02){
				STATE = PRESS_PINA1;
				counter++; //counter that checks for 1sec
				if(counter > 10){
					counter = 0;
					--temp;
					LCD_Cursor(1);
					LCD_WriteData(temp+'0');
				}
			}
			else if(tempA == 0x00){
				STATE = RELEASE_STATE;
			}
			else if(tempA == 0x03){
				STATE = RESET;
				temp = 0;
			}else{
				STATE = PRESS_PINA1;
			}
			break;

		//the RESET buttons being pressed
		case RESET:
			if(tempA == 0x00){
				STATE = RELEASE_STATE;
				temp = 0;
				LCD_Cursor(1);
				LCD_WriteData(temp+'0');
			}else {
				STATE = RESET;
				temp = 0;
			}
			break;
		default:
			break;
	}
	

}



int main() {

	//INITIALIZING BUTTON INPUTS	
	DDRA = 0x00; PORTA = 0xFF;



	DDRC = 0xFF; PORTC = 0x00; //LCD Data lines
	DDRD = 0xFF; PORTD = 0x00;//LCD control lines

	
	//LCD Initializing
	LCD_init();
	
	//initializing timer to 1/10 sec
	TimerSet(100);
	TimerOn();


	STATE = START;
	

	while(1) {
		state_machine();
		while(!TimerFlag);
		TimerFlag = 0;
	}


	
}
