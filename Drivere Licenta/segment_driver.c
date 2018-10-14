/*
 * _7segment_driver.c
 *
 * Created: 12-Sep-18 01:01:54 PM
 *  Author: gabib
 */ 
#include "segment_driver.h"


void init7Segment(volatile uint8_t *Port, unsigned char DATAPin,unsigned char SCKPin,unsigned char SCLPin)
{
	pins.Port = Port;
	pins.SCKPin = SCKPin;
	pins.SCLPin = SCLPin;
	pins.DATAPin = DATAPin;
	
	//DDR - set DDR and make LOAD, SCK, DATA output pin
	*(pins.Port-1) |= (1<<pins.SCLPin)|(1<<pins.SCKPin)|(1<<pins.DATAPin);
	//Put LOAD, SCK, DATA on low
	*(pins.Port) &= ~((1<<pins.SCLPin)|(1<<pins.SCKPin)|(1<<pins.DATAPin));
	writeNumberOn7Segments(groundFloor);
}

void pulseClock()
{
	*(pins.Port) |= (1<<pins.SCKPin);
	*(pins.Port) &= ~(1<<pins.SCKPin);
}

void writeData(uint8_t data)
{
	//set  latch low
	*(pins.Port) &= ~(1<<pins.DATAPin);
	for(uint8_t i = 0; i<8; i++)
	{
		if(data & 0b10000000)
		{
			*(pins.Port) |= (1<<pins.SCLPin);
		}
		else
		{
			*(pins.Port) &= ~(1<<pins.SCLPin);
		}
		pulseClock();
		data = data<<1;
	}
	
	//set  latch high
	*(pins.Port) |= (1<<pins.DATAPin);
}

void writeNumberOn7Segments(uint8_t number)
{
	if(number == groundFloor)
	{    
		writeData(number);
		writeData(0xff);
	}
	else
	{
		writeData(number);
	    writeData(e);
	}
	
}