/*	Author: eduardorocha
 *  Partner(s) Name: 
 *	Lab Section:
 *	Assignment: Lab #  Exercise #
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#include <stdio.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#include <util/delay.h>
#include "../header/max7219.h"

#endif

unsigned char five[] = { 0x00, 0x7C, 0x60, 0x7E, 0x06, 0x06, 0x66, 0x3C };
unsigned char four[] = { 0x00, 0x0C, 0x1C, 0x2C, 0x4C, 0x7E, 0x0C, 0x0C };
unsigned char three[] = { 0x00, 0x3C, 0x66, 0x06, 0x1C, 0x0C, 0x66, 0x3C };
unsigned char two[] = { 0x00, 0x3C, 0x66, 0x06, 0x0C, 0x30, 0x60, 0x7E };
unsigned char one[] = { 0x00, 0x18, 0x18, 0x38, 0x18, 0x18, 0x18, 0x7E };


int main(void) {

    DDRD = 0xFF;
    PORTD = 0x00;

    max7219_init();
    
    uint8_t ic = 0;

    //init ic
    for(ic=0; ic<MAX7219_ICNUMBER; ic++) {
        max7219_shutdown(ic, 1); //power on
        max7219_test(ic, 0); //test mode off
        max7219_decode(ic, 0); //use led matrix
        max7219_intensity(ic, 3); //intensity
        max7219_scanlimit(ic, 7); //set number of digit to drive
    }

    uint8_t i = 0;
    uint8_t j = 0;

    max7219_clearDisplay(1);

    while(1)
    {
        for (unsigned char j = 0; j < 8; j++) {
                max7219_digit(0, j, five[j]);
        }
        _delay_ms(1000);
        for (unsigned char j = 0; j < 8; j++) {
            max7219_digit(0, j, four[j]);
        }
         _delay_ms(1000);

        for (unsigned char j = 0; j < 8; j++) {
            max7219_digit(0, j, three[j]);
        }
        _delay_ms(1000);

        for (unsigned char j = 0; j < 8; j++) {
            max7219_digit(0, j, two[j]);
        }
        _delay_ms(1000);

        for (unsigned char j = 0; j < 8; j++) {
            max7219_digit(0, j, one[j]);
        }
         _delay_ms(1000);
        
        
    }
    //do test loop for every ic
    //for(;;) {
        //for(ic=0; ic<MAX7219_ICNUMBER; ic++) {
            //for(i=0; i<8; i++) {
                //for(j=0; j<8; j++) {
                    //max7219_digit(ic, i, (1<<j));
                //}
                //max7219_digit(ic, i, 0);
            //}
        //}
    //}
}
