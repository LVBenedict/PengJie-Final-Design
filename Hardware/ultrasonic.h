#ifndef __ULTRASONIC_H
#define __ULTRASONIC_H

#include "stm32f1xx.h"
#include "tim.h"
#include "OLED.h"
#include "stdio.h"


void HCSR04_Init(void);
void HCSR04_Read_m(void);
void HCSR04_Read_l(void);
void HCSR04_Read_r(void);
void HCSR04_OLEDShowResult(void);

#endif
