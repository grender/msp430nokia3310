/*
 * nokiaLcd.h
 *
 *  Created on: Jan 5, 2013
 *      Author: grender
 */

#ifndef NOKIALCD_H_
#define NOKIALCD_H_

#define SEND_CMD                   0
#define SEND_CHR                   1

#define LCD_X_RES                  84
#define LCD_Y_RES                  48

// defines for 4250 pin connections to Nokia 3310
#define SCEPORT P1OUT
#define SDINPORT P1OUT
#define DCPORT P1OUT
#define SCKPORT P1OUT
#define RESPORT P1OUT
#define SCE  BIT1
#define SDIN BIT4
#define DC   BIT7
#define SCK  BIT5
#define RES  BIT2

void LCDSend(unsigned char,unsigned char);
void LCDClear(void);
void LCDInit(void);
void LCDBlack(void);
void LCDChar(unsigned char);
void LCDCurs(unsigned char,unsigned char);
void LCDString(unsigned char x, unsigned char row, unsigned char *dataPtr );
void LCDDot(void);
void LCDHour(void);

#endif /* NOKIALCD_H_ */
