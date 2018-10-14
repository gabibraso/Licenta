/*
* buttons.c
*
* Created: 14-Oct-18 01:30:14 PM
*  Author: gabib
*/
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdbool.h>
#include "max7219_driver.h"
#include "segment_driver.h"
#include "buttons.h"

volatile bool buttonpressed = false;		// Used to verify if the button was pressed (ISR timer + debouncing)
volatile bool floorReached = true;			// Used to verify if the floor was reached

volatile bool internalButton0 = false;		//---|
volatile bool internalButton1 = false;		//---|
volatile bool internalButton2 = false;		//---|--- these variables are for the four internal buttons
volatile bool internalButton3 = false;		//---|

volatile bool externalButton0 = false;		//---|
volatile bool externalButton1 = false;		//---|
volatile bool externalButton2 = false;		//---|--- these variables are for the four external buttons
volatile bool externalButton3 = false;		//---|

volatile uint8_t currentFloor = 0x00;		// Used to retain the current floor


void buttonsInit()
{
	// Make the all buttons as input
	DDRD &= ~((1<<PORTD0) | (1<<PORTD1) | (1<<PORTD2) | (1<<PORTD3));
	DDRE &= ~((1<<PORTE4) | (1<<PORTE5) | (1<<PORTE6) | (1<<PORTE7));
	
	// Set pull-up resistors
	PORTE |= (1<<PORTE4) | (1<<PORTE5) | (1<<PORTE6) | (1<<PORTE7);
	PORTD |= (1<<PORTD0) | (1<<PORTD1) | (1<<PORTD2) | (1<<PORTD3);
}

void setButtonsInterrupts()
{
	// Generate interrupt at falling-edge
	EICRA |= (1<<ISC01) | (0<<ISC00) | (1<<ISC11) | (0<<ISC10) | (1<<ISC21) | (0<<ISC20) | (1<<ISC31) | (0<<ISC30);
	EICRB |= (1<<ISC71) | (0<<ISC70) | (1<<ISC61) | (0<<ISC60) | (1<<ISC51) | (0<<ISC50) | (1<<ISC41) | (0<<ISC40);
	
	// Activate external interrupt
	EIMSK = 0xFF;
}

void setDebouncingTimer()
{
	// Set CTC mode
	TCCR1A = (0<<WGM11) | (0<<WGM10);
	
	// Stop the timer
	TCCR1B = (0<<WGM13) | (1<<WGM12) | (0<<CS12) | (0<<CS11) | (0<<CS10);
	
	// Set the start value of the counter
	TCNT1 = 0;
	
	// After 1s generate the interrupt
	OCR1A = 2880;			//1s
	
	// Activate the timer and external interrupts
	TIMSK = (1<<OCIE1A);
	sei();
}

void internalButtonsSequence()
{	// IF	
	// (buttonpressed = true (timer interrupt => 1s)) AND																					        -----|
	// (internalButton0 = true (external interrupt => button was pressed) AND																			 |---- these are for debouncing
	// (button is still pressed),  (pull-up resistor ON => PINE = 0xff AND PINE4 = 1 => will be 0 when button is pressed. when is not pressed is 1) -----|
	 
	if((buttonpressed) && (internalButton0) && ((PINE & (1<<PINE4)) == 0))			
	{
		floorReached = false;					// "floorReached" is now false because the floor is not reached yet
		buttonpressed = false;					// "buttonPressed" is now false because he was pressed
		internalButton0 = false;                // "internalButton0" is now false because he was pressed
		writeNumberOn7Segments(clear);			// Clear 7 segment display
		writeNumberOn7Segments(groundFloor);	// Write "P" on 7 segment display
		
		if(currentFloor == 0x00)				// If current floor is the same with floor
		{										// number that we have pressed
			floorError();						// Display on matrix "X"
		}else									// Else display on matrix "arrow down" and put a smile (floor reached)
		{
			arrowDown();
			smile();
		}
		currentFloor = 0x00;					// Now the currentFloor will be 0 because we are now at the ground floor ("P" button was pressed)
		floorReached = true;					// "floorReached" is now true because he was reached
	}
	
	if((buttonpressed) && (internalButton1) && ((PINE & (1<<PINE5)) == 0))
	{
		floorReached = false;									// "floorReached" is now false because the floor is not reached yet
		buttonpressed = false;									// "buttonPressed" is now false because he was pressed
		internalButton1 = false;								// "internalButton1" is now false because he was pressed
		writeNumberOn7Segments(clear);							// Clear 7 segment display
		writeNumberOn7Segments(one);							// Write "1" on 7 segment display
		
		if((currentFloor == 0x02) || (currentFloor == 0x03))	// If current floor is 2 or 3 because we want to go at 1
		{														// display on led matrix "arrow down" and put a smile (floor reached)
			arrowDown();
			smile();
		}else if(currentFloor == 0x01)							// Else if current floor is 1 because we're already here display "floorError"
		{
			floorError();
		}else													// Else, we are supposed to want to go from P to 1 floor
		{														// and display on matrix led an "arrowUP" and a smile (floor reached)
			arrowUp();
			smile();
		}
		currentFloor = 0x01;									// Now the current floor will be 1 because we are now at the first floor ("1" button was pressed)
		floorReached = true;									// "floorReached" is now true because he was reached
	}
	
	if((buttonpressed) && (internalButton2) && ((PINE & (1<<PINE6)) == 0))
	{
		floorReached = false;									// "floorReached" is now false because the floor is not reached yet
		buttonpressed = false;									// "buttonPressed" is now false because he was pressed
		internalButton2 = false;								// "internalButton2" is now false because he was pressed
		writeNumberOn7Segments(clear);							// Clear 7 segment display
		writeNumberOn7Segments(two);							// Write "2" on 7 segment display
		
		if((currentFloor == 0x00) || (currentFloor == 0x01))	// If current floor is 0 or 1 because we want to go at 2
		{														// display on led matrix "arrow up" and put a smile (floor reached)
			arrowUp();
			smile();
		}else if(currentFloor == 0x02)							// Else if current floor is 2 because we're already here display "floorError"
		{
			floorError();
		}else													// Else, we are supposed to want to go from 3 to 2 floor
		{														// and display on matrix led an "arrow down" and a smile (floor reached)
			arrowDown();
			smile();
		}
		currentFloor = 0x02;									// Now the current floor will be 2 because we are now at the second floor ("2" button was pressed)
		floorReached = true;									// "floorReached" is now true because he was reached
	}
	
	if((buttonpressed) && (internalButton3) && ((PINE & (1<<PINE7)) == 0))
	{
		floorReached = false;									// "floorReached" is now false because the floor is not reached yet
		buttonpressed = false;									// "buttonPressed" is now false because he was pressed
		internalButton3 = false;								// "internalButton3" is now false because he was pressed
		writeNumberOn7Segments(clear);							// Clear 7 segment display
		writeNumberOn7Segments(three);							// Write "3" on 7 segment display
		
		if(currentFloor == 0x03)								// If current floor is the same with floor
		{														// number that we have pressed
			floorError();										// Display on matrix "X"
		}														// Else display on matrix "arrow up" and put a smile (floor reached)
		else
		{
			arrowUp();
			smile();
		}														
		currentFloor = 0x03;									// Now the currentFloor will be 3 because we are now at the third floor ("3" button was pressed)
		floorReached = true;									// "floorReached" is now true because he was reached
	}
}



ISR(INT4_vect)																		// External ISR => the button was pressed
{
	if(floorReached)																// if "floorReached" is true => the cabin can be called
	{
		TCNT1 = 0;							
		TCCR1B =  (0<<WGM13) | (1<<WGM12) | (1<<CS12) | (0<<CS11) | (0<<CS10);		// reset TCNT to 0 and start the timer
		internalButton0 = true;														// "internalButton0" will be true because the button was pressed
	}
}

ISR(INT5_vect)
{
	if(floorReached)																
	{
		TCNT1 = 0;
		TCCR1B =  (0<<WGM13) | (1<<WGM12) | (1<<CS12) | (0<<CS11) | (0<<CS10);
		internalButton1 = true;
	}
}

ISR(INT6_vect)
{
	if(floorReached)
	{
		TCNT1 = 0;
		TCCR1B =  (0<<WGM13) | (1<<WGM12) | (1<<CS12) | (0<<CS11) | (0<<CS10);
		internalButton2 = true;
	}
}

ISR(INT7_vect)
{
	if(floorReached)
	{
		TCNT1 = 0;
		TCCR1B =  (0<<WGM13) | (1<<WGM12) | (1<<CS12) | (0<<CS11) | (0<<CS10);
		internalButton3 = true;
	}
	
}

ISR(TIMER1_COMPA_vect)																// The ISR Timer that is generated after one second
{																					
		buttonpressed = true;														// Put "buttonpressed" on true
		TCCR1B = (0<<WGM13) | (1<<WGM12) | (0<<CS12) | (0<<CS11) | (0<<CS10);		// Stop the timer

}