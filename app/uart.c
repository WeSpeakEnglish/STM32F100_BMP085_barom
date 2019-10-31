#include "uart.h"



char write_UART1(unsigned char * SMS){
  u32 tmp=0;
     while (SMS[tmp]) // ���� �� ����� ������...
     {
          while (!(USART1->SR & USART_SR_TXE)) {} // ����� ������������ ������.
          USART1->DR=SMS[tmp]; tmp++; // ��������� ����.
     }
return 0;
}

char Init_UART1(void){
     RCC->APB2ENR|= RCC_APB2ENR_IOPAEN; // ��������� ������������ ����� A.
     RCC->APB2ENR|= RCC_APB2ENR_AFIOEN; // ������������ �������������� ������� GPIO.
     RCC->APB2ENR|= RCC_APB2ENR_USART1EN; // ��������� ������������ USART1.
     GPIOA->CRH |= GPIO_CRH_MODE9; // ����� TX PA.9 - �� �����.
     GPIOA->CRH &=~GPIO_CRH_CNF9; GPIOA->CRH |=GPIO_CRH_CNF9_1; // �������������� �����.
     USART1->CR1 |=(USART_CR1_RE | USART_CR1_TE); // ��������� ������ RX, TX - PA10, PA9.
     // �������� 115.2 kbps. USARTDIV=FSYS/(16*baud) = 8e6/(16*115200) = 4,34.
     USART1->BRR=(4<<4); // ����� ����� ������������ ������� USART1. 
     USART1->BRR|=5; // ������� �����*16 = 0,34*16 = 5 (���).
     USART1->CR1 |=USART_CR1_UE; // ��������� USART1.
return 0;
}