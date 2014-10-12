/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __ADC0832_H
#define __ADC0832_H

/* Includes ------------------------------------------------------------------*/
#include "stm8s.h"

void AD_init(unsigned char i);
u8 AD_read();

#endif /* __ADC0832_H */