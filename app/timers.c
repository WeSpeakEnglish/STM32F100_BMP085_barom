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
   RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;   //��������� ������ �������� ��������� �� TIM2
   TIM2->CNT = 0;
   TIM2->SR = 0;
   TIM2->CR2=0x0;          //�������� �������
   TIM2->CR1=0x0;          //�������� �������

   TIM2->PSC = 1999;          // ������������ ������� 2
   TIM2->ARR = 999;      //

   TIM2->CR1 &= ~TIM_CR1_DIR;      //������ ���� - 0 (�������� - 1) ������
   TIM2->CR1 &= ~TIM_CR1_UDIS;      //����.-1; ��������� ������� �� �������.������� -0
   TIM2->CR1 &= ~TIM_CR1_CKD;      //clock division
   TIM2->CR1 &= ~TIM_CR1_ARPE;      //��������� 0 - ����� �������� �� ���� ������ �������� ARR;���������=1 �����������
   TIM2->CR1 &= ~TIM_CR1_OPM;      //��������� ������ � ������ ���������� ��������
   TIM2->DIER |= TIM_DIER_UIE;      //&UPDATE INTERRPUT - ���������� �� ������������ &&&

   TIM2->CR1 |= TIM_CR1_URS;
   TIM2->EGR &= ~TIM_EGR_UG;      // ���������� 0 ;update enable  ARR � ��������� �������� �� ���������������� � ������� �������
   TIM2->CR1 |= TIM_CR1_CEN;       //��������� ������2
        NVIC_EnableIRQ (TIM2_IRQn);    //��������� ����������
  
}

void TIM3_init(void) // make delays 
{
RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;   //������ ������������ �� TIM3                           /
TIM3->PSC     = 8-1;                  //��������� �������� ��� ������������ 
TIM3->ARR     = 1667;                 //1000 ��� ���� 8 ���
TIM3->CR1     = TIM_CR1_CEN;
}


void TIM2_IRQHandler(void)
{
  static u16 InsideCounter = 0;
  
   if (TIM2->SR & TIM_SR_UIF){
      TIM2->SR &= ~TIM_SR_UIF;        // ������� ���� ���������� 
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

