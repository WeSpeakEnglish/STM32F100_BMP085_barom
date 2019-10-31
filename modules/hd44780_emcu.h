#ifndef hd44780_emcu_H
#define hd44780_emcu_H

    #include "stm32F10x.h" // ����������� ����� CMSIS ��� STM32.
 
    void LCD_CMD (u32 CMD);
    void LCD_DAT (u32 DAT) ;
    void LCD_CHAR (u32 ADR, u32 DAT);
    void LCD_STRING (u32 ADR, u8 *STRING);
    void LCD_INIT (void);
    void LCD_GOTO (u32 ADR);
    void LCD_VIEW_MODE (u32 CURSOR1, u32 CURSOR2, u32 VIEW_ON);
    void CREATE_CHAR (char ADR, char Y1,char Y2,char Y3,char Y4,char Y5,char Y6,char Y7,char Y8);

    #define  SYSCLK_FREQ    24000000 // �������� ������� SYS CLK.
    
    #define  NUM_OF_LINE        2   // ����� ����� LCD.
    #define  CHAR_IN_LINE       16  // ����� �������� � ������ ����� LCD.  
    #define  LCD_C_PORT         PORTC // ���� ����������� �������� CD, RW, EN.
    #define  LCD_CD             8 // ������ CD �������/������.
    #define  LCD_RW             9 // ������ RW ������/������.
    #define  LCD_EN             12 // ������ EN ���������� ������ �� ����.
    #define  LCD_D_PORT         PORTC // ���� ���� ������
    #define  LCD_D_SHIFT     0 // �������� �� ������ ����� �� ���� ������.
    #define  LCD_D_MASK      0x0F<<LCD_D_SHIFT // ����� ���� ������.
    #define  ON                 1 // �������� "��������".
    #define  OFF                0 // �������� "���������".
#endif