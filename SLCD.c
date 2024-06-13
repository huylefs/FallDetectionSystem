#include "MKL46Z4.h"
#include "SLCD.h"

const unsigned long int MASK_BIT[32] =
{
    0x00000001, 0x00000002, 0x00000004, 0x00000008,
    0x00000010, 0x00000020, 0x00000040, 0x00000080,
    0x00000100, 0x00000200, 0x00000400, 0x00000800,
    0x00001000, 0x00002000, 0x00004000, 0x00008000,
    0x00010000, 0x00020000, 0x00040000, 0x00080000,
    0x00100000, 0x00200000, 0x00400000, 0x00800000,
    0x01000000, 0x02000000, 0x04000000, 0x08000000,
    0x10000000, 0x20000000, 0x40000000, 0x80000000,
};

const unsigned char WF_ORDERING_TABLE[] =
{
    CHAR1a, CHAR1b, CHAR2a, CHAR2b,
    CHAR3a, CHAR3b, CHAR4a, CHAR4b,
    CHARCOM0, CHARCOM1, CHARCOM2, CHARCOM3,
};

const char ASCII_TO_WF_CODIFICATION_TABLE[] = {
    0x0B, 0x0E, // Char = 0
    0x00, 0x06, // Char = 1
};

unsigned char bLCD_CharPosition;

void SLCD_Init(void)
{
    SIM->SCGC5 |= SIM_SCGC5_SLCD_MASK | SIM_SCGC5_PORTB_MASK | SIM_SCGC5_PORTC_MASK | SIM_SCGC5_PORTD_MASK | SIM_SCGC5_PORTE_MASK;

    PORTC->PCR[20] = 0x00000000; // VLL2
    PORTC->PCR[21] = 0x00000000; // VLL1
    PORTC->PCR[22] = 0x00000000; // VCAP2
    PORTC->PCR[23] = 0x00000000; // VCAP1

    MCG->C1 = MCG_C1_IRCLKEN_MASK | MCG_C1_IREFSTEN_MASK;
    MCG->C2 &= ~MCG_C2_IRCS_MASK;

    LCD->GCR = (LCD_GCR_RVEN_MASK*_LCDRVEN
              | LCD_GCR_RVTRIM(_LCDRVTRIM)
              | LCD_GCR_CPSEL_MASK*_LCDCPSEL
              | LCD_GCR_LADJ(_LCDLOADADJUST)
              | LCD_GCR_VSUPPLY_MASK*_LCDSUPPLY
              | !LCD_GCR_FDCIEN_MASK
              | LCD_GCR_ALTDIV(_LCDALTDIV)
              | !LCD_GCR_LCDDOZE_MASK
              | !LCD_GCR_LCDSTP_MASK
              | !LCD_GCR_LCDEN_MASK
              | LCD_GCR_SOURCE_MASK*_LCDCLKSOURCE
              | LCD_GCR_ALTSOURCE_MASK*_LCDALRCLKSOURCE
              | LCD_GCR_LCLK(_LCDLCK)
              | LCD_GCR_DUTY(_LCDDUTY)
              );

    SLCD_EnablePins();
    LCD->GCR |= LCD_GCR_LCDEN_MASK;
    LCD->AR = LCD_AR_BRATE(_LCDBLINKRATE);
}

void SLCD_EnablePins(void)
{
    unsigned char i;
    unsigned long int *p_pen;
    unsigned char pen_offset;
    unsigned char pen_bit;

    LCD->PEN[0] = 0x0;
    LCD->PEN[1] = 0x0;
    LCD->BPEN[0] = 0x0;
    LCD->BPEN[1] = 0x0;

    p_pen = (unsigned long int *)&LCD->PEN[0];

    for (i = 0; i < _LCDUSEDPINS; i++)
    {
        pen_offset = WF_ORDERING_TABLE[i] / 32;
        pen_bit = WF_ORDERING_TABLE[i] % 32;
        p_pen[pen_offset] |= MASK_BIT[pen_bit];

        if (i >= _LCDFRONTPLANES)
        {
            p_pen[pen_offset + 2] |= MASK_BIT[pen_bit];
            LCD->WF8B[(unsigned char)WF_ORDERING_TABLE[i]] = MASK_BIT[i - _LCDFRONTPLANES];
        }
    }
}

void SLCD_WriteChar(unsigned char lbValue)
{
    unsigned char char_val;
    unsigned char temp;
    unsigned char *lbpLCDWF;
    unsigned char lbCounter;
    unsigned short int arrayOffset;
    unsigned char position;

    lbpLCDWF = (unsigned char *)&LCD->WF[0];

    if (lbValue < '0' || lbValue > '1') lbValue = ' ';
    lbValue -= '0';

    arrayOffset = (lbValue * 2);

    lbCounter = 0;
    while (lbCounter < 2 && bLCD_CharPosition < 2)
    {
        position = (bLCD_CharPosition) * 2 + lbCounter;
        temp = 0;
        if (lbCounter == 1)
        {
            temp = lbpLCDWF[WF_ORDERING_TABLE[position]] & 0x01;
        }

        char_val = ASCII_TO_WF_CODIFICATION_TABLE[arrayOffset + lbCounter];
        lbpLCDWF[WF_ORDERING_TABLE[position]] = char_val | temp;
        lbCounter++;
    }
}
