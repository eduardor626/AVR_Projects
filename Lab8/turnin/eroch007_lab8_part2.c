/*	Author: Eduardo Rocha
 *  	Partner(s) Name: Arturo Alvarado
 *	Lab Section:
 *	Assignment: Lab #8  Exercise #2
 *	Exercise Description: 
	A photoresistor is a resistor whose resistance varies based on how 
	much light the photoresistor detects. 

	An additional resistor needs to be connected in parallel 
	with the photoresistor. Without the additional resistor, 
	results from ADC will be too small to observe. 
	A 330 ohm resistor is chosen because it is common in the lab kits. 
	Connect the photoresistor to the microcontroller according to the 
	diagram below.

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

	/* Insert your solution below */
	while (1) {
		my_short = ADC;
		unsigned char my_char = (char)(my_short);
		PORTB = my_char;
		my_char = (char)(my_short >> 8);
		PORTD = my_char;
		
		
	}
	return 0;
}
