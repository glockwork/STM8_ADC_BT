/**********************************************
程序名称:ADC0832子程序
作    者:devinzhang91
时    间:2014.10.04
**********************************************/
#ifndef ADC0832_H
#define ADC0832_H
#include "stm8s.h"
#define uchar unsigned char
#define uint  unsigned int
#define first_channel   0x02  //通道1
#define second_channel  0x03  //通道2

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
函数名称:void ioInit(void)
函数作用:初始化GPIO
参数说明:null
********************************************************/
void ioInit(void)
{
  //全为输出模式
  GPIO_Init(CLK_GPIO_PORT, CLK_GPIO_PINS, GPIO_MODE_OUT_PP_LOW_FAST);
  GPIO_Init(DI_GPIO_PORT, DI_GPIO_PINS, GPIO_MODE_OUT_PP_LOW_FAST);
  GPIO_Init(DO_GPIO_PORT, DO_GPIO_PINS, GPIO_MODE_OUT_PP_LOW_FAST);
  GPIO_Init(CS_GPIO_PORT, CS_GPIO_PINS, GPIO_MODE_OUT_PP_LOW_FAST);
}

/********************************************************
函数名称:void ioChange()
函数作用:初始化GPIO
参数说明:i=0,表示输出，i=1，表示输入
********************************************************/
void ioChange(uchar i)
{
  if( i == 0)
    GPIO_Init(DO_GPIO_PORT, (GPIO_Pin_TypeDef)DO_GPIO_PINS, GPIO_MODE_OUT_PP_LOW_FAST);
  if( i == 1)
    GPIO_Init(DI_GPIO_PORT, (GPIO_Pin_TypeDef)DI_GPIO_PINS, GPIO_MODE_IN_PU_NO_IT);
}

/********************************************************
函数名称:void AD_init(uchar i)
函数作用:初始化ADC0832
参数说明:i=0,表示通道0，i=1，表示通道1
********************************************************/
void AD_init(uchar i) 
{
    ioInit();   //初始化io
    ioChange(0);  //作为输出
    GPIO_WriteLow(CLK_GPIO_PORT, (GPIO_Pin_TypeDef)CLK_GPIO_PINS);    //形成下降沿
    asm("nop");
    asm("nop");
    GPIO_WriteHigh(DO_GPIO_PORT, (GPIO_Pin_TypeDef)DO_GPIO_PINS);       /*在第1 个时钟脉冲的下沉之前DI端必须是高电平，表示启始信号*/
    asm("nop");
    asm("nop");
    GPIO_WriteLow(CS_GPIO_PORT, (GPIO_Pin_TypeDef)CS_GPIO_PINS);  //使能ADC0832
    asm("nop");
    asm("nop");
    GPIO_WriteHigh(CLK_GPIO_PORT, (GPIO_Pin_TypeDef)CLK_GPIO_PINS);
    asm("nop");
    asm("nop");
    GPIO_WriteLow(CLK_GPIO_PORT, (GPIO_Pin_TypeDef)CLK_GPIO_PINS);    //形成下降沿1 
    asm("nop");
    asm("nop");         /*在第2、3个脉冲下沉之前DI端应输入2位数据用于选择通道功能*/
    if( i==0 )
      GPIO_WriteLow(DO_GPIO_PORT, (GPIO_Pin_TypeDef)DO_GPIO_PINS);
    if( i==1 )
      GPIO_WriteHigh(DO_GPIO_PORT, (GPIO_Pin_TypeDef)DO_GPIO_PINS);
    asm("nop");
    asm("nop");
    GPIO_WriteHigh(CLK_GPIO_PORT, (GPIO_Pin_TypeDef)CLK_GPIO_PINS);
    asm("nop");
    asm("nop");
    GPIO_WriteLow(CLK_GPIO_PORT, (GPIO_Pin_TypeDef)CLK_GPIO_PINS);    //形成下降沿2 
    asm("nop");
    asm("nop");
    if( i==0 )
      GPIO_WriteLow(DO_GPIO_PORT, (GPIO_Pin_TypeDef)DO_GPIO_PINS);
    if( i==1 )
      GPIO_WriteHigh(DO_GPIO_PORT, (GPIO_Pin_TypeDef)DO_GPIO_PINS);
    GPIO_WriteHigh(CLK_GPIO_PORT, (GPIO_Pin_TypeDef)CLK_GPIO_PINS);
    asm("nop");
    asm("nop");
    GPIO_WriteLow(CLK_GPIO_PORT, (GPIO_Pin_TypeDef)CLK_GPIO_PINS);    //形成下降沿3 
    asm("nop");
    asm("nop");
}
/********************************************************
函数名称:uchar AD_read()
函数作用:读取ADC0832转换的数据
参数说明:无
函数返回:返回8位的数据
********************************************************/
u8 AD_read()
{
    u8 temp1 = 0;
    u8 temp2 = 0;
    uchar i = 0;
    GPIO_WriteHigh(DO_GPIO_PORT, (GPIO_Pin_TypeDef)DO_GPIO_PINS);
    asm("nop");
    asm("nop");
     ioChange(1); //作为输入
    for(i = 0; i < 8; i++)
    {
        GPIO_WriteHigh(CLK_GPIO_PORT, (GPIO_Pin_TypeDef)CLK_GPIO_PINS);
        asm("nop");
        asm("nop");
        GPIO_WriteLow(CLK_GPIO_PORT, (GPIO_Pin_TypeDef)CLK_GPIO_PINS);   //形成下降沿
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
       GPIO_WriteLow(CLK_GPIO_PORT, (GPIO_Pin_TypeDef)CLK_GPIO_PINS);   //形成下降沿 
       asm("nop");
       asm("nop");
    }
    GPIO_WriteHigh(CLK_GPIO_PORT, (GPIO_Pin_TypeDef)CLK_GPIO_PINS);
    asm("nop");
    asm("nop");
    GPIO_WriteHigh(DO_GPIO_PORT, (GPIO_Pin_TypeDef)DO_GPIO_PINS);
    asm("nop");
    asm("nop");
    GPIO_WriteHigh(CS_GPIO_PORT, (GPIO_Pin_TypeDef)CS_GPIO_PINS);  //使能ADC0832
    asm("nop");
    asm("nop");
       
    if(temp1 == temp2)
      return temp1;
    else
      return 0;
}
#endif