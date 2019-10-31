#include "timers.h"
#include "LCD.h"
#include "hd44780_driver.h"
#include "sht1x.h"
#include "uart.h"
#include "bmp085.h"
//////////////////////////////////////////////
u16 Hum;
u16 Temp;
u8 Crc;

float CalculatedHum;
float CalculatedTemp;
enum{TEMP=1,HUM};
//////////////////////////////////////////////////


void TIM2_init(void)
{
   RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;   //разрешить подачу тактовых импульсов на TIM2
   TIM2->CNT = 0;
   TIM2->SR = 0;
   TIM2->CR2=0x0;          //обнуляем регистр
   TIM2->CR1=0x0;          //обнуляем регистр

   TIM2->PSC = 1999;          // предделитель таймера 2
   TIM2->ARR = 999;      //

   TIM2->CR1 &= ~TIM_CR1_DIR;      //прямой счёт - 0 (обратный - 1) отсчёт
   TIM2->CR1 &= ~TIM_CR1_UDIS;      //запр.-1; разрешаем событие по перепол.таймера -0
   TIM2->CR1 &= ~TIM_CR1_CKD;      //clock division
   TIM2->CR1 &= ~TIM_CR1_ARPE;      //запрещаем 0 - когда прграмно не надо менять значение ARR;разрешаем=1 перзагрузку
   TIM2->CR1 &= ~TIM_CR1_OPM;      //Запрещаем работу в режиме одиночного импульса
   TIM2->DIER |= TIM_DIER_UIE;      //&UPDATE INTERRPUT - прерывание по переполнению &&&

   TIM2->CR1 |= TIM_CR1_URS;
   TIM2->EGR &= ~TIM_EGR_UG;      // сбрасываем 0 ;update enable  ARR и прескалер грузятся из предварительного в теневой регистр
   TIM2->CR1 |= TIM_CR1_CEN;       //запускаем таймер2
        NVIC_EnableIRQ (TIM2_IRQn);    //разрешаем прерывание
  
}

void TIM3_init(void) // make delays 
{
RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;   //подать тактирование на TIM3                           /
TIM3->PSC     = 8-1;                  //настроить делитель для формирования 
TIM3->ARR     = 1667;                 //1000 мкс если 8 Мгц
TIM3->CR1     = TIM_CR1_CEN;
}


void TIM2_IRQHandler(void)
{
  static u16 InsideCounter = 0;
  
   if (TIM2->SR & TIM_SR_UIF){
      TIM2->SR &= ~TIM_SR_UIF;        // очищаем флаг прерывания 
   }
 /*     
   //   if(InsideCounter%2) {
   //                     GPIOA->ODR&=~GPIO_ODR_ODR5; // drive PA5 to low
   //                     GPIOA->ODR|=GPIO_ODR_ODR6; // drive PA6 to high
   //                       }
   //   else                {
   //                     GPIOA->ODR|=GPIO_ODR_ODR5; // drive PA5 to high
   //                     GPIOA->ODR&=~GPIO_ODR_ODR6; // drive PA6 to low
   //                       }
     
  //    lcd_clear(); // clear LCD
      lcd_set_xy(1,0);
      DisplayTemperature(bmp085_read_temperature());
      
    //  lcd_send(num++,1);
      lcd_set_xy(8,0);
      DisplayPressure(0.1);
      
  //    if(!(InsideCounter%8)){
        lcd_set_xy(1,1);
        SHT10_Config();
        SHT10_ConReset();
//     SHT10_Start();  
           
           
      SHT10_Measure(&Temp,&Crc,TEMP);
      SHT10_Measure(&Hum,&Crc,HUM);
      SHT10_Cal(Temp,Hum,&CalculatedTemp,&CalculatedHum);
      DisplayTemperature((int16_t)(CalculatedTemp*10.0));
      lcd_set_xy(8,1);
      DisplayHumidity(&CalculatedHum);
//      }
 //     lcd_out("m");
  //    write_UART1(DisplayTemperature(0.1));
      
      }
  */
  InsideCounter++;
}

