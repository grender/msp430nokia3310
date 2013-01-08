#include  <msp430.h>
#include  <legacymsp430.h>

#include "nokiaLcd.h"

#define LED0 BIT0
#define LED1 BIT6

#define BUTTON BIT3

#include "Image_Eye.xbm"
#include "Image_peace.xbm"
#include "rolling_stones_N.xbm"
#include "rolling_stones_P.xbm"

void delay_ms(unsigned int ms ){ 		// Not really a milliseconds delay!
      unsigned int i;
      for (i = 0; i <= ms; i++){ 		// The operator is <=, I've yet to fix this site bug.
        __delay_cycles(500);
      }
}


int main(void) {
	WDTCTL = WDTPW + WDTHOLD; // Stop WDT
	P1DIR |= SCE | SDIN | DC | SCK | RES;

	_BIS_SR(GIE);
	LCDInit();

	unsigned char* currentImage;
	char currentImageNum=0;
	while (1) {
/*
		 LCDString(0,0,"\x99\x9d\x9d\x9d\x9d\x9d\x9d\x9d\x9d\x9d\x9d\x9d\x9d\x8b");
		 LCDString(0,1,"\x8aСаш! Еще раз\x8a");
		 LCDString(0,2,"\x8a  спасибо   \x8a");
		 LCDString(0,3,"\x8aза паяльник!\x8a");
		 LCDString(0,4,"\x8a \x80\x80\x81\x81\xac\xac\x81\x81\x80\x80 \x8a");
		 LCDString(0,5,"\x98\x9d\x9d\x9d\x9d\x9d\x9d\x9d\x9d\x9d\x9d\x9d\x9d\x8c");
*/

		int row = 0;
		int col = 0;
		char bitMov[] = { BIT0, BIT1, BIT2, BIT3, BIT4, BIT5, BIT6, BIT7 };
		int curBit = 0;
		for (row = 0; row < 6; row++) {
			LCDCurs(0, row);
			for (col = 0; col < 11; col++) {
				for (curBit = 0; curBit < 8; curBit++) {
					LCDSend(
							(((currentImage[88 * row + 0 * 11 + col]
									& bitMov[curBit]) ? BIT0 : 0)
									| ((currentImage[88 * row + 1 * 11 + col]
											& bitMov[curBit]) ? BIT1 : 0)
									| ((currentImage[88 * row + 2 * 11 + col]
											& bitMov[curBit]) ? BIT2 : 0)
									| ((currentImage[88 * row + 3 * 11 + col]
											& bitMov[curBit]) ? BIT3 : 0)
									| ((currentImage[88 * row + 4 * 11 + col]
											& bitMov[curBit]) ? BIT4 : 0)
									| ((currentImage[88 * row + 5 * 11 + col]
											& bitMov[curBit]) ? BIT5 : 0)
									| ((currentImage[88 * row + 6 * 11 + col]
											& bitMov[curBit]) ? BIT6 : 0)
									| ((currentImage[88 * row + 7 * 11 + col]
											& bitMov[curBit]) ? BIT7 : 0)),
							SEND_CHR);

				}
			}
		}
		switch(currentImageNum)
		{
			case 0:
				currentImage=Image_peace_bits;
			break;

			case 1:
				currentImage=rolling_stones_N_bits;
			break;

			case 2:
				currentImage=rolling_stones_P_bits;
			break;

			case 3:
				currentImage=Image_Eye_bits;
				currentImageNum=-1;
			break;
		}
		delay_ms(2000);
		currentImageNum++;
	}
}
