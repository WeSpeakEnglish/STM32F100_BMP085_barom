#include "stm32F10x.h"
#include "stm32f10x_i2c.h"
#include "uart.h"
#include "delays.h"
#include "timers.h"
#include "ioconfig.h"
#include "i2c.h"
#include "bmp085.h"
#include "hd44780_driver.h"
#include "LCD.h"
#include "dac.h"
#include "sht1x.h"

//GPIO_TypeDef * PORT_Temp;




int main(void)
{ 
 //    RCC->CR|=RCC_CR_HSEON; // Включить генератор HSE.
//     while (!(RCC->CR & RCC_CR_HSERDY)) {} // Ожидание готовности HSE.
 //    RCC->CFGR &=~RCC_CFGR_SW; // Очистить биты SW0, SW1.
//     RCC->CFGR |= RCC_CFGR_SW_HSE; // Выбрать HSE для тактирования SW0=1.
  RCC->CR   |=  RCC_CR_HSION;            //Включить генератор HSI
  while((RCC->CR & RCC_CR_HSIRDY)==0) {} //Ожидание готовности HSI
  RCC->CFGR &= ~RCC_CFGR_SW;             //Очистка битов выбора источника тактового сигнала
  RCC->CFGR |=  RCC_CFGR_SW_HSI;         //Выбрать источником тактового сигнала HSI
  //Настроить делитель для AHB
   RCC->CFGR &= ~RCC_CFGR_HPRE;           //Очистка битов предделителя "AHB Prescaler"
   RCC->CFGR |=  RCC_CFGR_HPRE_DIV4;      //Установить "AHB Prescaler" равным 8
// RCC->CFGR |= (uint32_t)RCC_CFGR_PPRE1_DIV2;
  PortAConf();
  I2c1_Init();
//  bmp085_init();
  
  PortCConf();
  PortBConf();
  ButtonConf(); // button enable/disable signal
  
  lcd_init();
  
     TIM2_init();  // for TEST SIGNAL
     
     Init_UART1(); 
     ConfigureDAC1();
     ConfigureLCD();
     lcd_set_xy(0,0);
     lcd_send(0,DATA); // temp sumbol to LCD
     lcd_out("      ");
     lcd_send(4,DATA); // press sumbol to LCD
     lcd_set_xy(0,1);
     
     lcd_send(0,DATA); // temp sumbol to LCD 
     lcd_out("      ");
     lcd_send(6,DATA); // hum sumbol to LCD

//SHT10_Config();

     while(1){

     }   

}