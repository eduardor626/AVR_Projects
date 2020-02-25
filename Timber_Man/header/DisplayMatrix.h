#ifndef _DISPLAY_MATRIX_H_
#define _DISPLAY_MATRIX_H_

#include "max7219.h"
#include <util/delay.h>


//Countdown Variables that display the countdown
unsigned char five[] = { 0x00, 0x7C, 0x60, 0x7E, 0x06, 0x06, 0x66, 0x3C };
unsigned char four[] = { 0x00, 0x0C, 0x1C, 0x2C, 0x4C, 0x7E, 0x0C, 0x0C };
unsigned char three[] = { 0x00, 0x3C, 0x66, 0x06, 0x1C, 0x0C, 0x66, 0x3C };
unsigned char two[] = { 0x00, 0x3C, 0x66, 0x06, 0x0C, 0x30, 0x60, 0x7E };
unsigned char one[] = { 0x00, 0x18, 0x18, 0x38, 0x18, 0x18, 0x18, 0x7E };

//Tree Trunk Variable
const unsigned char treeTrunk = 0x18;
//Timber Man character position
unsigned char timberMan = 0x40;

//LeftBranch
unsigned char leftBranch = 0x60;
unsigned char rightBranch = 0x06;
unsigned char emptyBranch = treeTrunk;

unsigned char countdownFrom = 6;


//Function that will display a countdown on the LED Matrix 5,4,3,2,1 etc.
void countdown(){
    switch(countdownFrom)
    {
        case 6:
            countdownFrom--;
            break;
        case 5:
            for (unsigned char j = 0; j < 8; j++) {
                max7219_digit(0, j, five[j]); //first LCD at row , equal value at five[row]
            }
            countdownFrom--;

            break;
        case 4:
            for (unsigned char j = 0; j < 8; j++) {
                max7219_digit(0, j, four[j]); //first LCD at row , equal value at five[row]
            }
            countdownFrom--;
            break;
        case 3:
            for (unsigned char j = 0; j < 8; j++) {
                max7219_digit(0, j, three[j]); //first LCD at row , equal value at five[row]
            }
            countdownFrom--;
            break;

        case 2:
            for (unsigned char j = 0; j < 8; j++) {
                max7219_digit(0, j, two[j]); //first LCD at row , equal value at five[row]
            }
            countdownFrom--;
            break;

        case 1:
            for (unsigned char j = 0; j < 8; j++) {
                max7219_digit(0, j, one[j]); //first LCD at row , equal value at five[row]
            }
            countdownFrom = 0;
            break;
        default:
            break;
    }


}


//Initializing the initial start display of the game
void initializeMatrix(){
    max7219_clearDisplay(0);
    max7219_clearDisplay(1);
    for (unsigned char j = 0; j < 8; j++) {
        max7219_digit(0, j,treeTrunk); //first LCD at row , equal value at five[row]
        max7219_digit(1,j,treeTrunk);
        if(j == 6){
            max7219_digit(1,j,(treeTrunk|timberMan));
        }
    }

    unsigned int branch1row = 0, branch2row = 2, branch3row = 4, branch4row = 6;
    unsigned int branch5row = 0, branch6row = 2, branch7row = 4; 
    //initialize branches
    max7219_digit(0,branch1row,leftBranch | treeTrunk);
    max7219_digit(0,branch2row,rightBranch| treeTrunk);
    max7219_digit(0,branch3row,rightBranch| treeTrunk);
    max7219_digit(0,branch4row,leftBranch | treeTrunk);

    max7219_digit(1,branch5row,leftBranch | treeTrunk);
    max7219_digit(1,branch6row,rightBranch | treeTrunk);
    max7219_digit(1,branch7row,leftBranch | treeTrunk);    


}

//DISPLAY BEGIN
enum DisplayStates {Display_Start, Display_Countdown, Display_Print,Wait} DisplayState;

int DisplaySM(int DisplayState)
{

    //transitions
    switch(DisplayState)
    {
        case Display_Start:
            DisplayState = Display_Countdown;
            max7219_clearDisplay(0);
            max7219_clearDisplay(1);
            break;
        case Display_Countdown:
            DisplayState = Display_Countdown;
            break;
        case Display_Print:
            DisplayState = Display_Print;
            break;
        case Wait:
            DisplayState = Wait;
        default:
            DisplayState = Display_Countdown;
            break;
    }

    //actions
    switch(DisplayState)
    {
        case Display_Start:
            break;
        case Display_Countdown:
            countdown();
            if(countdownFrom == 0){
                DisplayState = Display_Print;
            }
            break;
        case Display_Print:
            initializeMatrix();
            if(countdownFrom == 0){
                DisplayState = Wait;
            }
            break;
        case Wait:
            
            break;
        default:
            break;
    }
    return DisplayState;
}


//DISPLAY END
#endif
