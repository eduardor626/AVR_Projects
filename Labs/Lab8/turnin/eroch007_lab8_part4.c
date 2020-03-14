/*	Author: Eduardo Rocha
 *  	Partner(s) Name: Arturo Alvarado
 *	Lab Section:
 *	Assignment: Lab #8  Exercise #4
 *	Exercise Description: 
	
	(Challenge) Design a system, using a bank of eight LEDs, 
	where the number of LEDs illuminated is a representation of 
	how much light is detected. For example, when more light is detected, 
	more LEDs are illuminated. Criteria:	
	
	The LEDs should illuminated in sequence from 0 to 7, based on the 
	amount of light detected by the photoresistor.

 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */


#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif



void ADC_init(){

	ADCSRA |= (1 << ADEN) | (1 << ADSC) | (1 << ADATE); 
}	


int main() {
	/* Insert DDR and PORT initializations */
	DDRA = 0x00; PORTA = 0x00;
	DDRB = 0xFF; PORTB = 0x00;
	DDRB = 0xFF; PORTB = 0x00;
	DDRC = 0xFF; PORTC = 0x00;

	ADC_init();
	
	unsigned short my_short;
	unsigned short MAX = 160;

	/* Insert your solution below */
	while (1) {
		my_short = ADC;
		unsigned char my_char = (char)(my_short);	
		if(my_char >= MAX){
			PORTB = 63;
		}
		else if(my_char > MAX/2){
			PORTB = 31;
		}else if(my_char > MAX/3){
			PORTB = 15;
		}else if(my_char > MAX/4){
			PORTB = 7;
		}else if(my_char > MAX/5){
			PORTB = 3;
		}else if(my_char > MAX/6){
			PORTB = 2;
		}else if(my_char > MAX/7){
			PORTB = 1;
		}
		else{
			PORTB = 0;
		}
	}
	return 0;
}
