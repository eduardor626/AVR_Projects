/*	Author: Eduardo Rocha
 *  Partner(s) Name: Arturo Alvarado
 *	Lab Section: 
 *	Assignment: Lab #9  Exercise #3
 *	Exercise Description: 



 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */

#include <avr/io.h>
#include <avr/interrupt.h>

volatile unsigned char TimerFlag=0;

unsigned long _avr_timer_M = 1;
unsigned long _avr_timer_cntcurr=0;

void TimerOn(){
	TCCR1B=0x0B;
	OCR1A=125;
	TIMSK1=0x02;
	TCNT1=0;
	_avr_timer_cntcurr=_avr_timer_M;
	SREG |= 0x80;
}

void TimerOff(){
	TCCR1B=0x00;
}

void TimerISR(){
	TimerFlag=1;
}

ISR(TIMER1_COMPA_vect){
	_avr_timer_cntcurr --;
	
	if( _avr_timer_cntcurr==0){
		TimerISR();
		_avr_timer_cntcurr= _avr_timer_M;
	}
}

void TimerSet(unsigned long M ){
	_avr_timer_M=M;
	_avr_timer_cntcurr = _avr_timer_M;
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
	
	TCCR3A = ( 1 << COM3A0 );
	
	TCCR3B = ( 1 << WGM32 ) | ( 1 << CS31 ) | ( 1 << CS30 );
	
	set_PWM(0);
}

void PWM_off(){
	TCCR3A=0x00;
	TCCR3B=0x00;
}



enum STATES { start, init, press, release, wait, play } state;

unsigned char button_on=0;
double sound_arr[5]={ 261.63, 329.63, 392.00, 493.88, 523.25 };
unsigned char sound_lasting[5]={3, 1, 2, 1, 6 };
unsigned char sound_waiting[5]={1, 2, 1, 2, 0 };
unsigned char i=0;
unsigned char cnt=0;

void state_machine(){
	
	unsigned char button1=~PINA & 0x01;
	unsigned char button2=~PINA & 0x02;
	unsigned char button3=~PINA & 0x04;
	

	
	
	switch (state){
		
		case start:	
							state = init;
							break;
					
		case init:			
							
							state=(button1) ? press : init;
							break;
					
		case press:		
							state=(button1) ? press : release;
							break;			
					
		case release:				state=wait;
							break;

		case wait:				if( cnt < sound_waiting[i] ){
								cnt=cnt+1;
							}else{
							state=play;
							cnt=0;
							}

							break;

		case play:			
							//play the sound in the current array
							set_PWM( sound_arr[i] );

							if( cnt < sound_lasting[i] ){
								cnt=cnt+1;
							}else{
							state=release;
							cnt=0;
							++i;
							}

							if( i==5 ){
							state=start;
							i=0;
							set_PWM( 0 ); //turn off sound
							}

							break;
					
		default: 
							state=start;
							break;
		}

}

int main(void)
{

	//Initializing our input and output ports
	DDRA=0x00; PORTA=0xFF;
	DDRB=0XFF; PORTB=0x00;

	//Initializing our Speaker
	PWM_on();

	//Initializing our time of 100ms
	TimerSet(100);   
	TimerOn(); //Enabling the timer
	
    while (1) 
    {
		state_machine();
		while(!TimerFlag);
		TimerFlag=0;
    }

    return 0;
}