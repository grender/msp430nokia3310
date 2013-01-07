/*
 * nokiaLcd.c
 *
 *  Created on: Jan 5, 2013
 *      Author: grender
 */
#include  <msp430.h>
#include  <legacymsp430.h>

#include "nokiaLcd.h"
#include "font_6x8.h"

#define AmountChars 14
unsigned char PlaceArray[] = {0,6,12,18,24,30,36,42,48,54,60,66,72,78};

void LCDChrXY(unsigned char x, unsigned char y, unsigned char ch)
{
unsigned int PointerFont = (ch<<2) + (ch<<1);
unsigned char xpos = PlaceArray[x];

    LCDSend((0x80|xpos), SEND_CMD);
    LCDSend((0x40|y), SEND_CMD);

    unsigned char i;
    for(i = 0; i<=5; i++){
    ch =  Font_6x8_Data[PointerFont];
    LCDSend(ch, SEND_CHR);
    PointerFont++;
  }
}

void LCDString(unsigned char x, unsigned char row, unsigned char *dataPtr ) {

  // loop to the and of string
  while ((*dataPtr)&&(x<=(AmountChars-1))) {
    LCDChrXY(x, row, (*dataPtr));
    x++;
    dataPtr++;
  }
}

void LCDSend(unsigned char data, unsigned char cd) {

  volatile unsigned char bits;
  unsigned short cnt=8;
  // assume clk is hi
  // Enable display controller (active low).
  SCEPORT &= ~SCE;  //RESET SCE

  // command or data
  if(cd == SEND_CHR) {
    DCPORT |= DC;  //set to send data
  }
  else {  // reset to send command
    DCPORT &= ~DC;
  }

  ///// SEND SPI /////
  bits=0x80; // bits is mask to select bit to send. select bit msb first

  //send data
  while (0<cnt--)
  {
    // put bit on line
    // cycle clock
    SCKPORT &= ~SCK;
    if ((data & bits)>0) SDINPORT |= SDIN; else SDINPORT &= ~SDIN;
    //Delay(1);
    SCKPORT |= SCK;
    //Delay(2);
    // SHIFT BIT MASK 1 right
    bits >>= 1;
  }

  // Disable display controller.
  SCEPORT |= SCE;

}

void LCDClear(void) {
  int i,j;

  LCDSend(0x80, SEND_CMD );
  LCDSend(0x40, SEND_CMD );

  for (i=0;i<6;i++)  // number of rows
    for (j=0;j<LCD_X_RES;j++)  // number of columns
      LCDSend(0x00, SEND_CHR);
}

void LCDInit(void)
{ // assume ports set up and initialized to output

  // Reset LCD
  SCEPORT &= ~SCE;          // RESET SCE to enable
  // toggle RES
  RESPORT |= RES;           // Set RES
  char l;
  for(l=0;l<10;l++)
    l=l;
  RESPORT &= ~RES;          // reset RES
  for(l=0;l<100;l++)
    l=l;
  RESPORT |= RES;           // Set RES

  // Cycle Clock
  SCKPORT &= ~SCK;
  SCKPORT |= SCK;

 // Disable display controller.
  SCEPORT |= SCE;           // bring high to disable

  for(l=0;l<100;l++)
    l=l;

  // Send sequence of command
  LCDSend( 0x21, SEND_CMD );  // LCD Extended Commands.
  LCDSend( 0xBf, SEND_CMD );  // Set LCD Vop (Contrast).
  LCDSend( 0x06, SEND_CMD );  // Set Temp coefficent to 2.
  LCDSend( 0x13, SEND_CMD );  // LCD bias mode 1:100.
  LCDSend( 0x20, SEND_CMD );  // LCD Standard Commands, Horizontal addressing mode.
//  LCDSend( 0b00100010, SEND_CMD );  // LCD Standard Commands, Vertical addressing mode.
  LCDSend( 0x08, SEND_CMD );  // LCD blank
  LCDSend( 0x0C, SEND_CMD );  // LCD in inverse mode.

  LCDClear();


}

void LCDCurs(unsigned char x, unsigned char y)
{
	LCDSend(0x40|(y&0x07),SEND_CMD);
	LCDSend(0x80|(x&0x7f),SEND_CMD);
}

void LCDDot()
{
  int lm;
  LCDSend(0x00,SEND_CHR);
  LCDSend(0x00,SEND_CHR);
  LCDSend(0x00,SEND_CHR);
  for(lm=0;lm<3;lm++)
    LCDSend(0x3c,SEND_CHR);
}
