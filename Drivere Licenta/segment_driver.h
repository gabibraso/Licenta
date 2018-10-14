#ifndef SEGMENT_DRIVER_H_
#define SEGMENT_DRIVER_H_

#include <avr/io.h>

					 //TRUTH TABLE        |   0 = segment on
					  //ABCDEFGH          |   1 = segment off
#define empty         0b11111111	 //   |        H
#define zero          0b11000000	 //   |      -----
#define one           0b11111001	 //   |   C |     | G
#define two           0b10100100	 //   |     |  B  |
#define three         0b10110000	 //   |      -----
#define four		  0b10011001	 //   |   D |     | F
#define five		  0b10010010	 //   |     |     |
#define groundFloor	  0b10001100	 //   |      -----
#define e			  0b00000110	 //   |        E
#define clear		  0b11111111

typedef struct{
	volatile uint8_t *Port;
	unsigned char DATAPin;
	unsigned char SCKPin;
	unsigned char SCLPin;
}displayPins;

displayPins pins;

void init7Segment(volatile uint8_t *Port, unsigned char DATAPin,unsigned char SCKPin,unsigned char SCLPin);

void pulseClock();

void writeData(uint8_t data);

void writeNumberOn7Segments(uint8_t number);
#endif /* SEGMENT_DRIVER_H_ */