 #define __MSP430G2553__
#include  <msp430.h>
#include  <legacymsp430.h>

#include "nokiaLcd.h"

#define FALSE 0
#define TRUE 1

#define LED0 BIT0
#define LED1 BIT6

#define BUTTON BIT3

#define SCREEN_WIDTH 48
#define SCREEN_HEIGHT (84-16)
#define SCREEN_BUFFER_SIZE (SCREEN_HEIGHT*SCREEN_WIDTH/8)
unsigned char* currentBuffer;

void delay_ms(unsigned int ms) { // Not really a milliseconds delay!
	unsigned int i;
	for (i = 0; i <= ms; i++) { // The operator is <=, I've yet to fix this site bug.
		__delay_cycles(500);
	}
}

void LCD_pixel(unsigned char x, unsigned char y, unsigned char mode) { // вывод пиксела

	if ((x < SCREEN_WIDTH) && (y < SCREEN_HEIGHT)) {
		unsigned char xNum = x / 8;
		unsigned char bit = x % 8;
		if (mode != 0)
			currentBuffer[SCREEN_WIDTH * y / 8 + xNum] |= (1 << bit);
		else
			currentBuffer[SCREEN_WIDTH * y / 8 + xNum] &= (~(1 << bit));
	}
}

void LCD_line(unsigned char x1, unsigned char y1, unsigned char x2,
		unsigned char y2, char mode) { // процедура рисования линии
	signed char dx, dy, sx, sy;
	unsigned char x, y, mdx, mdy, l;

	dx = x2 - x1;
	dy = y2 - y1;

	if (dx >= 0) {
		mdx = dx;
		sx = 1;
	} else {
		mdx = x1 - x2;
		sx = -1;
	}
	if (dy >= 0) {
		mdy = dy;
		sy = 1;
	} else {
		mdy = y1 - y2;
		sy = -1;
	}

	x = x1;
	y = y1;

	if (mdx >= mdy) {
		l = mdx;
		while (l > 0) {
			if (dy > 0) {
				y = y1 + mdy * (x - x1) / mdx;
			} else {
				y = y1 - mdy * (x - x1) / mdx;
			}
			LCD_pixel(x, y, mode);
			x = x + sx;
			l--;
		}
	} else {
		l = mdy;
		while (l > 0) {
			if (dy > 0) {
				x = x1 + ((mdx * (y - y1)) / mdy);
			} else {
				x = x1 + ((mdx * (y1 - y)) / mdy);
			}
			LCD_pixel(x, y, mode);
			y = y + sy;
			l--;
		}
	}
}

void LCDShowImageFullScreenHorizontal(unsigned char* currentImage) {
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
}

#ifdef TIMER0_A0_VECTOR
interrupt(TIMER0_A0_VECTOR) TIMERA0_ISR(void) {
#else
interrupt(TIMERA0_VECTOR) TIMERA0_ISR(void) {
#endif
	LCDCurs(0, 0);
	int col = 0;
	for (col = 0; col < 6 * 8; col++)
		LCDSend(0xFF, SEND_CHR);
	for (col = 0; col < SCREEN_BUFFER_SIZE; col++)
		LCDSend(currentBuffer[col], SEND_CHR);
	for (col = 0; col < 6 * 8; col++)
		LCDSend(0xFF, SEND_CHR);

}

int main(void) {
	WDTCTL = WDTPW + WDTHOLD; // Stop WDT
	P1DIR |= SCE | SDIN | DC | SCK | RES;

	//Set ACLK to use internal VLO (12 kHz clock)
	BCSCTL3 |= LFXT1S_2;

	//Set TimerA to use auxiliary clock in UP mode
	TACTL = TASSEL_1 | MC_1;
	//Enable the interrupt for TACCR0 match
	TACCTL0 = CCIE;

	// Set TACCR0 which also starts the timer. At 12 kHz, counting to 12000
	// should output an LED change every 1 second. Try this out and see how
	// inaccurate the VLO can be
	TACCR0 = 4000;

	LCDInit(TRUE, 0x1F);

	//Enable global interrupts
	eint();

	unsigned char screenBuffer[SCREEN_BUFFER_SIZE];
	currentBuffer = screenBuffer;

	while (1) {

		int i = 0;
		for (i = 0; i < SCREEN_BUFFER_SIZE; i++)
			screenBuffer[i] = 0;

		for (i = 0; i <= 23; i++) {
			LCD_line(0 + i, 0, 23, 33, 1);
			LCD_line(24, 33, SCREEN_WIDTH - 1 - i, 0, 1);
			LCD_line(0 + i, SCREEN_HEIGHT - 1, 23, 34, 1);
			LCD_line(24, 34, SCREEN_WIDTH - 1 - i, SCREEN_HEIGHT - 1, 1);
		}

		//delay_ms(15);

		/*
		 LCDString(0,0,"\x99\x9d\x9d\x9d\x9d\x9d\x9d\x9d\x9d\x9d\x9d\x9d\x9d\x8b");
		 LCDString(0,1,"\x8aСаш! Еще раз\x8a");
		 LCDString(0,2,"\x8a  спасибо   \x8a");
		 LCDString(0,3,"\x8aза паяльник!\x8a");
		 LCDString(0,4,"\x8a \x80\x80\x81\x81\xac\xac\x81\x81\x80\x80 \x8a");
		 LCDString(0,5,"\x98\x9d\x9d\x9d\x9d\x9d\x9d\x9d\x9d\x9d\x9d\x9d\x9d\x8c");
		 */
		/*
		 LCDCurs(0, 0);
		 int col=0;
		 for (col = 0; col < 504; col++) {
		 LCDSend(rolling_stones_Rotated_bits[col],SEND_CHR);
		 }
		 */
		/*
		 LCDShowImageFullScreenHorizontal(currentImage);
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
		 */
	}

}
