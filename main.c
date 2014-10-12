#include "stm8s.h"
#include "stm8s_it.h"

#define LED_G_GPIO_PORT  (GPIOB)
#define LED_G_GPIO_PINS  (GPIO_PIN_6)
#define LED_R_GPIO_PORT  (GPIOB)
#define LED_R_GPIO_PINS  (GPIO_PIN_7)

uint8_t HexTable[]={'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};   //16进制字符表
uint8_t i=0;

void Init_IO(void)
{
  GPIO_Init(LED_G_GPIO_PORT, LED_G_GPIO_PINS, GPIO_MODE_OUT_PP_LOW_FAST);
  //GPIO_Init(LED_R_GPIO_PORT, LED_R_GPIO_PINS, GPIO_MODE_IN_PU_NO_IT);
  //GPIO_WriteLow(LED_G_GPIO_PORT, LED_G_GPIO_PINS);
  GPIO_Init(GPIOB, GPIO_PIN_4, GPIO_MODE_IN_PU_NO_IT);
  GPIO_Init(GPIOB, GPIO_PIN_5, GPIO_MODE_OUT_PP_LOW_FAST); //板载LEC
}
//串口UART1初始化
void Init_UART(void)
{
  //默认初始化
  UART1_DeInit();       
  //设置波特率9600 8位数据 1位停止位 无校验  外部时钟不可用 模式接收发送
  UART1_Init((u32)9600, UART1_WORDLENGTH_8D, UART1_STOPBITS_1, UART1_PARITY_NO, UART1_SYNCMODE_CLOCK_DISABLE, UART1_MODE_TXRX_ENABLE);
  //设置接收寄存器溢出中断
  UART1_ITConfig(UART1_IT_RXNE_OR, ENABLE);
}
void Init_Timer4(void)
{
  //1ms中断一次
  TIM4_TimeBaseInit(TIM4_PRESCALER_128, 124);
  /* Clear TIM4 update flag */
  TIM4_ClearFlag(TIM4_FLAG_UPDATE);
  /* Enable update interrupt */
  TIM4_ITConfig(TIM4_IT_UPDATE, ENABLE);
  TIM4_Cmd(ENABLE);
}
//发送字节
void Send(uint8_t dat)
{
  //检查并等待发送寄存器是否为空
  while(( UART1_GetFlagStatus(UART1_FLAG_TXE)==RESET));
  //发送字节
  UART1_SendData8(dat);
}
//发送16位16进制
void UART1_mysend16hex(u16 dat)
{
    Send(HexTable[(dat>>12)&0x0f]);
    Send(HexTable[(dat>>8)&0x0f]);
    Send(HexTable[(dat>>4)&0x0f]);
    Send(HexTable[(dat)&0x0f]);
}
//发送8位16进制
void UART1_mysend8hex(uint8_t dat)
{
    Send(HexTable[(dat>>4)&0x0f]);
    Send(HexTable[(dat)&0x0f]);
    Send('U');
}
void main()
{
  //初始化
  Init_IO();
  Init_UART();
  Init_Timer4();
  //中断开启
  enableInterrupts(); 
  while(1)
  {
  }
}

//这个必须加上 不然会报错 估计是库的要求

#ifdef USE_FULL_ASSERT
void assert_failed(u8* file, u32 line)
{ 
  while (1)
  {
  }
}
#endif
