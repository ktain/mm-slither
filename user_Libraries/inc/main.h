#ifndef MAIN_H
#define MAIN_H

#include "stm32f4xx.h"
#include "delay.h"
#include "led.h"
#include "button.h"
#include <stdio.h>
#include "usart.h"
#include "pwm.h"
#include "encoder.h"
#include "buzzer.h"
#include "sensor_Function.h"
#include "adc.h"
#include "test.h"
#include "config.h"
#include "global.h"
#include "pid.h"
#include "turn.h"
#include "align.h"

void systick(void);
void button0_interrupt(void);
void button1_interrupt(void);
void button2_interrupt(void);
void button3_interrupt(void);

void printInfo(void);

#endif
