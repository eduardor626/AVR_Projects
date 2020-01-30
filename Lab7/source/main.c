/*	Author: Eduardo Rocha
 *  	Partner(s) Name: Arturo Alvarado
 *	Lab Section:
 *	Assignment: Lab #7  Exercise #1
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#include "io.h"
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

int main(void) {

	DDRC = 0xFF; PORTC = 0x00; //LCS data lines
	DDRD = 0xFF; PORTD = 0x00; //LCD control lines

	//Initialize the LCD display
	LCD_init();

	//Starting at position 1 on the LCD screen, writes Hello World
	LCD_DisplayString(1,"Hello World");
	while (1) {continue;}
	
}
