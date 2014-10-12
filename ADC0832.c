/**********************************************
��������:ADC0832�ӳ���
��    ��:devinzhang91
ʱ    ��:2014.10.04
**********************************************/
#ifndef ADC0832_H
#define ADC0832_H
#include "stm8s.h"
#define uchar unsigned char
#define uint  unsigned int
#define first_channel   0x02  //ͨ��1
#define second_channel  0x03  //ͨ��2

#define CLK_GPIO_PORT  (GPIOC)
#define CLK_GPIO_PINS  (GPIO_PIN_3)
#define DI_GPIO_PORT  (GPIOC)
#define DI_GPIO_PINS  (GPIO_PIN_4)
#define DO_GPIO_PORT  (GPIOC)
#define DO_GPIO_PINS  (GPIO_PIN_4)
#define CS_GPIO_PORT  (GPIOC)
#define CS_GPIO_PINS  (GPIO_PIN_1)

uchar buff[] = {0};

/********************************************************
��������:void ioInit(void)
��������:��ʼ��GPIO
����˵��:null
********************************************************/
void ioInit(void)
{
  //ȫΪ���ģʽ
  GPIO_Init(CLK_GPIO_PORT, CLK_GPIO_PINS, GPIO_MODE_OUT_PP_LOW_FAST);
  GPIO_Init(DI_GPIO_PORT, DI_GPIO_PINS, GPIO_MODE_OUT_PP_LOW_FAST);
  GPIO_Init(DO_GPIO_PORT, DO_GPIO_PINS, GPIO_MODE_OUT_PP_LOW_FAST);
  GPIO_Init(CS_GPIO_PORT, CS_GPIO_PINS, GPIO_MODE_OUT_PP_LOW_FAST);
}

/********************************************************
��������:void ioChange()
��������:��ʼ��GPIO
����˵��:i=0,��ʾ�����i=1����ʾ����
********************************************************/
void ioChange(uchar i)
{
  if( i == 0)
    GPIO_Init(DO_GPIO_PORT, (GPIO_Pin_TypeDef)DO_GPIO_PINS, GPIO_MODE_OUT_PP_LOW_FAST);
  if( i == 1)
    GPIO_Init(DI_GPIO_PORT, (GPIO_Pin_TypeDef)DI_GPIO_PINS, GPIO_MODE_IN_PU_NO_IT);
}

/********************************************************
��������:void AD_init(uchar i)
��������:��ʼ��ADC0832
����˵��:i=0,��ʾͨ��0��i=1����ʾͨ��1
********************************************************/
void AD_init(uchar i) 
{
    ioInit();   //��ʼ��io
    ioChange(0);  //��Ϊ���
    GPIO_WriteLow(CLK_GPIO_PORT, (GPIO_Pin_TypeDef)CLK_GPIO_PINS);    //�γ��½���
    asm("nop");
    asm("nop");
    GPIO_WriteHigh(DO_GPIO_PORT, (GPIO_Pin_TypeDef)DO_GPIO_PINS);       /*�ڵ�1 ��ʱ��������³�֮ǰDI�˱����Ǹߵ�ƽ����ʾ��ʼ�ź�*/
    asm("nop");
    asm("nop");
    GPIO_WriteLow(CS_GPIO_PORT, (GPIO_Pin_TypeDef)CS_GPIO_PINS);  //ʹ��ADC0832
    asm("nop");
    asm("nop");
    GPIO_WriteHigh(CLK_GPIO_PORT, (GPIO_Pin_TypeDef)CLK_GPIO_PINS);
    asm("nop");
    asm("nop");
    GPIO_WriteLow(CLK_GPIO_PORT, (GPIO_Pin_TypeDef)CLK_GPIO_PINS);    //�γ��½���1 
    asm("nop");
    asm("nop");         /*�ڵ�2��3�������³�֮ǰDI��Ӧ����2λ��������ѡ��ͨ������*/
    if( i==0 )
      GPIO_WriteLow(DO_GPIO_PORT, (GPIO_Pin_TypeDef)DO_GPIO_PINS);
    if( i==1 )
      GPIO_WriteHigh(DO_GPIO_PORT, (GPIO_Pin_TypeDef)DO_GPIO_PINS);
    asm("nop");
    asm("nop");
    GPIO_WriteHigh(CLK_GPIO_PORT, (GPIO_Pin_TypeDef)CLK_GPIO_PINS);
    asm("nop");
    asm("nop");
    GPIO_WriteLow(CLK_GPIO_PORT, (GPIO_Pin_TypeDef)CLK_GPIO_PINS);    //�γ��½���2 
    asm("nop");
    asm("nop");
    if( i==0 )
      GPIO_WriteLow(DO_GPIO_PORT, (GPIO_Pin_TypeDef)DO_GPIO_PINS);
    if( i==1 )
      GPIO_WriteHigh(DO_GPIO_PORT, (GPIO_Pin_TypeDef)DO_GPIO_PINS);
    GPIO_WriteHigh(CLK_GPIO_PORT, (GPIO_Pin_TypeDef)CLK_GPIO_PINS);
    asm("nop");
    asm("nop");
    GPIO_WriteLow(CLK_GPIO_PORT, (GPIO_Pin_TypeDef)CLK_GPIO_PINS);    //�γ��½���3 
    asm("nop");
    asm("nop");
}
/********************************************************
��������:uchar AD_read()
��������:��ȡADC0832ת��������
����˵��:��
��������:����8λ������
********************************************************/
u8 AD_read()
{
    u8 temp1 = 0;
    u8 temp2 = 0;
    uchar i = 0;
    GPIO_WriteHigh(DO_GPIO_PORT, (GPIO_Pin_TypeDef)DO_GPIO_PINS);
    asm("nop");
    asm("nop");
     ioChange(1); //��Ϊ����
    for(i = 0; i < 8; i++)
    {
        GPIO_WriteHigh(CLK_GPIO_PORT, (GPIO_Pin_TypeDef)CLK_GPIO_PINS);
        asm("nop");
        asm("nop");
        GPIO_WriteLow(CLK_GPIO_PORT, (GPIO_Pin_TypeDef)CLK_GPIO_PINS);   //�γ��½���
        asm("nop");
        asm("nop");
        temp1 = temp1 << 1;
        if(GPIO_ReadInputPin(DI_GPIO_PORT, (GPIO_Pin_TypeDef)DI_GPIO_PINS) !=0)
          temp1 |= 0x01;
        else temp1 |= 0x00;
    }
    
    for(i = 0; i < 8; i++)
   {
       temp2 = temp2>>1;
       if(GPIO_ReadInputPin(DI_GPIO_PORT, (GPIO_Pin_TypeDef)DI_GPIO_PINS) !=0)
            temp2 = temp2|0x80;
       GPIO_WriteHigh(CLK_GPIO_PORT, (GPIO_Pin_TypeDef)CLK_GPIO_PINS);
       asm("nop");
       asm("nop");
       GPIO_WriteLow(CLK_GPIO_PORT, (GPIO_Pin_TypeDef)CLK_GPIO_PINS);   //�γ��½��� 
       asm("nop");
       asm("nop");
    }
    GPIO_WriteHigh(CLK_GPIO_PORT, (GPIO_Pin_TypeDef)CLK_GPIO_PINS);
    asm("nop");
    asm("nop");
    GPIO_WriteHigh(DO_GPIO_PORT, (GPIO_Pin_TypeDef)DO_GPIO_PINS);
    asm("nop");
    asm("nop");
    GPIO_WriteHigh(CS_GPIO_PORT, (GPIO_Pin_TypeDef)CS_GPIO_PINS);  //ʹ��ADC0832
    asm("nop");
    asm("nop");
       
    if(temp1 == temp2)
      return temp1;
    else
      return 0;
}
#endif