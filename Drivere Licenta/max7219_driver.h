#ifndef MAX7219_DRIVER_H_
#define MAX7219_DRIVER_H_

#include <avr/io.h>
#define NUMBER_OF_DEVICES 1
#include <avr/pgmspace.h>
#include <avr/delay.h>
#include <string.h>


/* The array for shifting the data to the devices */
uint8_t spidata[16];

/* We keep track of the led-status for all 8 devices in this array */
uint8_t status[64];

uint8_t buffer[8];


/**
 * \brief					Used to initialize the matrix
 * 
 * \param brighness			- set the matrix brighness
 * \param limit				- set the scan limit register
 * \param power				- set the power/shut-down the matrix
 * \param test				- used to check the matrix functionality 
 * 
 * \return void
 */
void initMatrix(uint8_t dMode, uint8_t brightness, uint8_t limit, uint8_t power, uint8_t test);

/**
 * \brief				The decode-mode sets BCD code B (0-9, E, H, L, P, and -) or 
						no-decode operation for each digit. Each bit in the register 
						corresponds to one digit. A logic high selects code B decoding 
						while logic low bypasses the decoder.
						When the code B decode mode is used, the decoder
						looks only at the lower nibble of the data in the digit
						registers (D3–D0), disregarding bits D4–D6. D7, which
						sets the decimal point (SEG DP), is independent of the
						decoder and is positive logic (D7 = 1 turns the decimal
						point on). Table 5 lists the code B font.
						When no-decode is selected, data bits D7–D0 correspond
						to the segment lines of the MAX7219/MAX7221.
						
								    MODE			|	HEX CODE
						----------------------------|-------------		
						No decode for digits 0-7    |	  0x00
						----------------------------|
						Code B decode for digit 0   |
						No decode for digits 7-1    |     0x01
						----------------------------|
						Code B decode for digits 3-0|
						No decode for digits 7-4    |     0x0F
						----------------------------|
						Code B decode for digits 0-7|	  0xFF
						
						
 * 
 * \param mode		  This parameter takes the hex code value for desired mode
 * 
 * \return void
 */
void decodeMode(uint8_t dMode);

/**
 * \brief					Used to set de matrix brightness
 *								DUTY CYCLE	|	HEX CODE
 *								---------------------------
 *								1/32 (min)	|	  0x00
 *								3/32		|	  0x01
 *								5/32		|	  0x02
 *								7/32		|	  0x03
 *								9/32		|	  0x04
 *								11/32		|	  0x05
 *								13/32		|	  0x06
 *								15/32		|	  0x07
 *								17/32		|	  0x08
 *								19/32		|	  0x09
 *								21/32		|	  0x0A
 *								23/32		|	  0x0B
 *								25/32		|	  0x0C
 *								27/32		|	  0x0D
 *								29/32		|	  0x0E
 *								31/32 (max)	|     0x0F
 * 
 * \param brightness			This parameter takes the hex code value for desired brightness
 *	
 * \return void
 */
void displayBrightness(uint8_t brightness);

/**
 * \brief			  scanLimit	sets how many digits(columns) are displayed,
                      from 1 to 8. They are displayed in a multiplexed
                      manner with a typical display scan rate of 800Hz with 8
                      digits displayed. If fewer digits are displayed, the scan
                      rate is 8fOSC/N, where N is the number of digits
                      scanned. Since the number of scanned digits affects
                      the display brightness, the scan-limit register should
                      not be used to blank portions of the display (such as
                      leading zero suppression).
					  
					   SCAN LIMIT   | HEX CODE
				   ----------------------------
	Display digit	0	            |  	 0x00
					0 and 1			|	 0x01
					0,1,2			|	 0x02
					0,1,2,3			|	 0x03
					0,1,2,3,4		|	 0x04
					0,1,2,3,4,5		|	 0x05
					0,1,2,3,4,5,6	|	 0x06
					0,1,2,3,4,5,6,7 |	 0x07
 * 
 * \param limit		This parameter takes the hex code value for desired scan limit
 * 
 * \return void
 */
void scanLimit(uint8_t limit);

/**
 * \brief			This method is used to power or shut down the matrix
 
				    MODE     | HEX CODE
			------------------------------
				   shut-down |   0x00
			 normal operation|   0x01
			      (power)	 |
 * 
 * \param power   This parameter takes the hex code value for desired action (power/shut-down)
 * 
 * \return void
 */
void powerMatrix(uint8_t power);


/**
 * \brief					Display-test mode turns all LEDs on by overriding, 
							but not altering, all controls and digit registers
							(including the shutdown register). In display-test
							mode, 8 digits are scanned and the duty cycle is 31/32
							(15/16 for MAX7221).
							
							MODE    | HEX CODE
						------------------------
				   Normal operation |   0x00
				          Test mode	|   0x01	
 * 
 * \param test		This parameter takes the hex code value for desired action (power/shut-down)
 * 
 * \return void
 */
void displayTest(uint8_t test);

/**
 * \brief			This method clear the matrix (put all leds of matrix on low)
 * 
 * \param			- no parameters
 * 
 * \return void
 */
void clearMatrix(void);

/** 
 * \brief		     This method set the matrix (put all leds of matrix on high)
 * 
 * \param 
 * 
 * \return void
 */

void sendDigit(uint8_t digit, uint8_t value);

void writeWord(uint8_t address, uint8_t data);

void setRow(int addr, int row, uint8_t value) ;


void arrowUp();
void arrowDown();
void floorError();
void errora();










#endif /* MAX7219_DRIVER_H_ */