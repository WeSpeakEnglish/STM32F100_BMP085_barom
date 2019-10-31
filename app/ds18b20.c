#include "ds18b20.h"
//#

//#define PORT  GPIOA   //������� ����, � �������� ���������� �������
//#define TIMER TIM3    //������ ������, ������������ ��� ������������ ��������
TIM_TypeDef * TIMER = TIM3;
GPIO_TypeDef * PORT = GPIOA;
u8 PinNumb = 7;
//*********************************************************************************************
//function  ������ ����������� �������                                                       //
//argument   ����� ���� ����� 0..15                                                          //
//return    1280 - ������, ����� - ��������� ���������                                       //
//*********************************************************************************************
signed int ds_read_temperature()
{
  signed char integer = 0;     
  signed char frac;
  signed int  result;
  uint8_t     buff[8];
 
  //��������� ������ �� ������� 
  if(ds_read_data_single(&buff[0]))  return 1280;
 
  frac    = buff[0] & 0x0f;                            //�������� ������� �����      
  integer = (buff[0]>>4) | ((buff[1] & 0x0f)<<4);      //�������� ����� ����� 
 
  //���� ����������� �������������
  if(integer<0)
  {
    integer = 0 - integer - 1;
    result  = integer *10;                            //��������� ����� �����
    frac = frac | 0xf0;
    frac = 0 - frac ;                                 
  }
  //���� ����������� �������������
  else     result  = integer *10;                    //��������� ����� �����
 
  result = result + ((frac*10)/16);                  //��������� ������� �����
 
  return result;
}

//*********************************************************************************************
//function  ������ ������ �������                                                            //
//argument  ��������� �� ������-�������� ������, ����� ���� ����� 0..15                      //
//return    0 - ������ ���������, 1 - ������ �� ���������, 2 - �.�. 3 - ������ CRC           //
//*********************************************************************************************
uint8_t ds_read_data_single(uint8_t *buff)
{
  uint8_t crc = 0;
  uint8_t data;
  uint8_t i,j;
  uint8_t tmp;
 
  tmp = ds_reset_pulse(1<<PinNumb);          //������� ������� ������
  if(tmp) return tmp;                        //���� ������ - ���������� �� ���
  ds_write_byte(0xCC,1<<PinNumb);            //��������� ������ � ������� �� ��������� �����
 
  ds_write_byte(0xBE,1<<PinNumb);            //������ 9 ���� ������
 
  //��������� 8 ���� � ��������� CRC
  for( i=0; i<8; i++) 
  {
    data = ds_read_byte(1<<PinNumb);         //��������� ��������� ����      
    buff[i] = data;                          //��������� ��� � �������
 
    //���������� CRC - ������������ ������ ��� ��������� �����
    for( j=0; j<8; j++)         
    {
      tmp = (crc ^ data) & 0x01;
      if (tmp==0x01) crc = crc ^ 0x18;
      crc = (crc >> 1) & 0x7F;
      if (tmp==0x01) crc = crc | 0x80;
      data = data >> 1;
    }		
  }
 
  data = ds_read_byte(1<<PinNumb);          //��������� CRC �������
  if(crc==data) return 0;                   //���� CRC ������� - ������ ������ ���
  return 3;                                 //CRC �� �������, ������ �������� ������
}

//*********************************************************************************************
//function  ���� ��������� �����������                                                       //
//argument  ����� ���� ����� 0..15                                                           //
//return    0 - ���� ��������, 1 - ������ �� ���������, 2 - �.�. �� �����                    //
//*********************************************************************************************
uint8_t ds_start_convert_single()
{
  uint8_t result;
  result = ds_reset_pulse(1<<PinNumb);       //������� ������� ������
  if(result) return result;                  //���� ������ - ���������� �� ���
  ds_write_byte(0xCC,1<<PinNumb);            //��������� ������ � ������� �� ��������� �����  
  ds_write_byte(0x44,1<<PinNumb);            //��������� ��������������
  return 0;
}


//*********************************************************************************************
//function  ������� ������                                                                   //
//argument  ����� �����                                                                      //
//return    0 - ���������� ���������, 1 - �� ����������, 2 - �.�. �� �����                   //
//*********************************************************************************************
uint8_t ds_reset_pulse(uint16_t PinMask)
{
   uint16_t result;   
 
   if((PORT->IDR & PinMask)==0)  return 2;         //��������� ����� �� ���������� ���������
   PORT->ODR &= ~PinMask;                          //�������� ���� � �����
   TIMER->CNT=0;
   while(TIMER->CNT<480){};                        //����� 480 �����������
   PORT->ODR |=  PinMask;                          //��������� ����
   while(TIMER->CNT<550){};                        //����� 70 �����������
   result     =  PORT->IDR & PinMask;              //��������� ���� 
   while(TIMER->CNT<960){};                        //��������� ��������� �������������
   if(result) return 1;                            //������ �� ���������
   return 0;                                       //������ ���������      
}

//*********************************************************************************************
//function  �������� ����                                                                    //
//argument  �������� ������������� ����,����� �����                                          //
//return    none                                                                             //
//*********************************************************************************************
void ds_write_bit(uint8_t bit,uint16_t PinMask)
{
   TIMER->CNT=0;
   PORT->ODR &= ~PinMask;                          //�������� ���� � �����
   while(TIMER->CNT<2){};                          //����� 1 ������������
   if(bit) PORT->ODR |=  PinMask;                  //���� �������� 1, �� ��������� ����
   while(TIMER->CNT<60){};                         //�������� 60 ����������� 
   PORT->ODR |=  PinMask;                          //��������� ���� 
}


//function  ������ ����                                                                      //
//argument  ����� �����                                                                      //
//return    ����������� ���                                                                  //

uint16_t ds_read_bit(uint16_t PinMask)
{
   uint16_t result;
 
   TIMER->CNT=0;
   PORT->ODR &= ~PinMask;                          //�������� ���� � �����
   while(TIMER->CNT<2){};
   PORT->ODR |=  PinMask;                          //��������� ����  
   while(TIMER->CNT<15){};                         //�������� 15 �����������
   result     =  PORT->IDR & PinMask;              //��������� ����
   while(TIMER->CNT<60){};                         //���������� ����� 
   return result;                                  //���������� ���������
}


//function  ������ �����                                                                     //
//argument  ������������ ����,����� �����                                                    //
//return    none                                                                             //

void ds_write_byte(uint8_t byte, uint16_t PinMask)
{
   uint8_t i;
   for(i=0;i<8;i++) ds_write_bit(byte&(1<<i), PinMask);
}

//function  ������ �����                                                                     //
//argument  ����� �����                                                                      //
//return    ����������� ����                                                                 //

uint8_t ds_read_byte(uint16_t PinMask)
{
   uint8_t i,result = 0;
   for(i=0;i<8;i++) 
   if(ds_read_bit(PinMask)) result |= 1<<i; 
   return result;
}

void TempPortTimerInit(GPIO_TypeDef * PortName, u8 LineNumber, TIM_TypeDef * Timer){
PORT = PortName;
TIMER = Timer;
PinNumb = LineNumber;
}


