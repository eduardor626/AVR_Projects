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
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#include "../header/timer.h"
#endif

int main(void) {
    //output of rows
    DDRC = 0xFF;
    PORTC = 0x00;
    
    //output of columns
    DDRA = 0xFF;
    PORTA = 0x00;


    TimerSet(500);
    TimerOn();

    unsigned char row = 0;
    unsigned char column = 0;

    while (1)
    {
        if(row < 7)
        {
            if(column < 7)
            {
                column++;
            }
            else
            {
                row++;
                column = 0;
            }
        }
        else
        {
            if(column < 7)
            {
                column++;
            }
            else
            {
                row = 0;
                column = 0;
            }
        }
       //PORTA = (0x00 | (1 << column));
       //PORTC = (0xFF & ~(1 << row));
        PORTA = 0x10;
        PORTC = 0x80;
        while(!TimerFlag);
        TimerFlag = 0;
    }
}
