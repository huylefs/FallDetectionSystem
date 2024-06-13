#ifndef __SLCD_H__
#define __SLCD_H__

#define _LCDRVEN          (1)  
#define _LCDRVTRIM        (8)
#define _LCDCPSEL         (1)
#define _LCDLOADADJUST    (3)
#define _LCDALTDIV        (0)
#define _LCDALRCLKSOURCE  (0)
#define _LCDSUPPLY        (1)
#define _LCDCLKSOURCE     (1)
#define _LCDLCK           (1)
#define _LCDBLINKRATE     (3)
#define _LCDFRONTPLANES   (8)
#define _LCDBACKPLANES    (4)
#define _LCDUSEDPINS      (_LCDFRONTPLANES + _LCDBACKPLANES)
#define _LCDDUTY          (_LCDBACKPLANES-1)
//#define _CHARNUM          (4)
//#define _CHAR_SIZE        (2)
//#define _LCDTYPE          (2)


#define CHAR1a      37
#define CHAR1b      17
#define CHAR2a      7
#define CHAR2b      8
#define CHAR3a      53
#define CHAR3b      38
#define CHAR4a      10
#define CHAR4b      11
#define CHARCOM0    40
#define CHARCOM1    52
#define CHARCOM2    19
#define CHARCOM3    18

void SLCD_Init(void);
void SLCD_EnablePins(void);
void SLCD_WriteChar(unsigned char lbValue);

#endif
