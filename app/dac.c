#include "dac.h"
#include "stm32f10x.h"
#include "stm32f10x_dac.h"

 #define  RCC_APB1ENR_DACEN                  ((uint32_t)0x20000000)        /*!< DAC interface clock enable */

void ConfigureDAC1(void){
RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;     // ������������ ����� GPIOA
RCC->APB1ENR |= RCC_APB1ENR_DACEN;      // �������� ������������ ����
//DAC->CR |= DAC_CR_BOFF1;        // ���� �������� �����.
DAC->CR      |= DAC_CR_EN1;             // �������� ����� �1
//DAC->CR      |= DAC_CR_EN2;             // �������� ����� �2
DAC->DHR8R1   = 0x30;                   // �� ������ ������ 1 
}