#include "smg.h"

uint8_t disArr[]={0xD7,0x14,0xcd,0x9d,0x1e,0x9b,0xdb,0x15,0xdf,0x9f,0x5f,0xda,0xc3,0xdc,0xcb,0x4b};

void smg_init()
{
  P1SEL &= ~(0x07);//BIT3;           // Function as General Purpose I/O.
    P1 |= 0x07;                 // LED1 on.
    P1DIR |= 0x07;//BIT3;            // Output.
    
     P0SEL &= ~(0xff);//BIT3;           // Function as General Purpose I/O.
    P0 |= 0xff;                 // LED1 on.
    P0DIR |= 0xff;//BIT3;            // Output.
    

}
uint8_t delay_ms(uint8_t delay)
{
while(delay--)
{}
}

void smg_display3333(uint8_t *digData)
{
  static uint8_t tmp;
 
    tmp= disArr[*digData&0x0f];
    if(*digData>0x0f)
    {
      tmp |=0x20;
    }
    P1 |=(0x07);
    P0 =tmp;
    
      delay_ms(100);
 
 
}
void smg_display_num(uint8_t *digData,uint8_t num)
{
  static uint8_t tmp;
  static uint8_t nummm=0;
  nummm++;
  if(nummm>2)
  {
  nummm=0;
  }
  nummm=*(digData+3);
    tmp= disArr[*(digData+nummm)&0x0f];
    if(*(digData+nummm)>0x0f)
    {
      tmp |=0x20;
    }
    P1 &=~(0x07);
    P0 =tmp;
    P1 |=(0x01<<nummm);
 
}

void smg_display_str(uint8_t *digData)
{
  static uint8_t tmp;
  static uint8_t dig=0;
  static uint8_t nummm=0;
  nummm++;
  if(nummm>2)
  {
  nummm=0;
  }
  switch(nummm)
  {
  case 2:
    dig=*digData/100;
    break;
  case 1:
    dig=(*digData%100)/10;
    break;
  case 0:
    dig=*digData%10;
    break;
  
  }
    tmp= disArr[dig&0x0f];
    if(dig>0x0f)
    {
      tmp |=0x20;
    }
    P1 &=~(0x07);
    P0 =tmp;
    P1 |=(0x01<<nummm);
 
}
void smg_display_str_nor(int digData)
{
  static uint8_t tmp;
  static uint8_t dig=0;
  static uint8_t nummm=0;
  nummm++;
  if(nummm>2)
  {
  nummm=0;
  }
  switch(nummm)
  {
  case 2:
    dig=digData/100;
    break;
  case 1:
    dig=(digData%100)/10;
    break;
  case 0:
    dig=digData%10;
    break;
  
  }
    tmp= disArr[dig&0x0f];
    if(dig>0x0f)
    {
      tmp |=0x20;
    }
    P1 &=~(0x07);
    P0 =tmp;
    P1 |=(0x01<<nummm);
 
}
void smg_display_num222(uint8_t digData,uint8_t num)
{
  static uint8_t tmp;
  static uint8_t nummm=0;
  nummm++;
  if(nummm>2)
  {
  nummm=0;
  }
    tmp= disArr[digData&0x0f];
    if(digData>0x0f)
    {
      tmp |=0x20;
    }
    P1 &=~(0x07);
    P0 =tmp;
    P1 |=(0x01<<nummm);
 
}
void smg_display333344(uint8_t *digData)
{
  static uint8_t tmp;
  for(char i=0;i<3;i++,digData++)
  {
    tmp= disArr[*digData&0x0f];
    if(*digData>0x0f)
    {
      tmp |=0x20;
    }
    P1 &=~(0x07);
    P0 =tmp;
    P1 |=(0x01<<i);
      delay_ms(100);
  }
 
}