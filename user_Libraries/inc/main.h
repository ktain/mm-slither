#ifndef MAIN_H
#define MAIN_H

#include "stm32f4xx.h"

void systick(void);
void button0_interrupt(void);
void button1_interrupt(void);
void button2_interrupt(void);
void button3_interrupt(void);

void printInfo(void);
void frontWallTest(int ref);
void ledTest(void);

#endif
