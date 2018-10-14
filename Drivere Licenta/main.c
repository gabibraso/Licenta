/*
* main.c
*
* Created: 10-Oct-18 06:04:18 PM
* Author : gabib
*/

#include <avr/io.h>
#include <avr/interrupt.h>
#include "spi_driver.h"
#include "segment_driver.h"
#include "buttons.h"



int main(void)
{
	spi_Init(&PORTB,PORTB0,PORTB1,PORTB2,PORTB3,MSB_FIRST,MASTER_MODE,4);
	initMatrix(0x00,0xff,0x07,0x01,0x00);
	init7Segment(&PORTA,PORTA4,PORTA3,PORTA2);
	buttonsInit();
	setButtonsInterrupts();
	setDebouncingTimer();
	
	
	while (1)
	{
		internalButtonsSequence();
		/*	if(internalButton0)
		{
		counter = 0;
		internalButton0 = false;
		buttonpressed = true;
		
		if(buttonpressed)
		{
		
		
		writeNumberOn7Segments(clear);
		writeNumberOn7Segments(firstFloor);
		
		if (currentFloor == 0x00)
		{
		floorReached();
		}else
		{
		arrowDown();
		smile();
		}
		
		currentFloor = 0x00;
		
		}
		
		}
		
		if(internalButton1)
		{
		counter = 0;
		internalButton1 = false;
		writeNumberOn7Segments(clear);
		writeNumberOn7Segments(one);
		
		if(currentFloor == 0x00)
		{
		arrowUp();
		smile();
		}else if(currentFloor == 0x01)
		{
		floorReached();
		}else
		{
		arrowDown();
		smile();
		}
		currentFloor = 0x01;
		
		}*/
	}
}





