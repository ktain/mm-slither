#include "stm32f4xx.h"
#include "delay.h"
#include "led.h"
#include "button.h"
#include <stdio.h>
#include "usart.h"
#include "pwm.h"
#include "encoder.h"
#include "buzzer.h"
#include "main.h"
#include "sensor_Function.h"
#include "adc.h"

void systick(void) {
		//readGyro();
}

void button0_interrupt(void) {
	shortBeep(200, 300);
	delay_ms(1000);
	while(1) {
		readSensor();
		ledTest();					// No delay
		printInfo();				// No delay
		delay_ms(50);
	}
}

void button1_interrupt(void) {
	shortBeep(200, 400);
	delay_ms(1000);
	while(1) {
		frontWallTest(2000);	// has 1ms delay
	}
}

void button2_interrupt(void) {
	shortBeep(200, 500);
	delay_ms(1000);
	while(1) {
		frontWallTest(500);	// has 1ms delay
	}
}

void button3_interrupt(void) {
	shortBeep(200, 600);
	delay_ms(1000);
	setLeftPwm(50);
	setRightPwm(-50);
}


void printInfo(void) {
	
	printf("LEM_F %4d | LEM_D %4d |REM_F %4d | REM_D %4d | LENC %9d | RENC %9d | aSpeed %8d | angle %8d | voltage %4d | Outz %4d | Vref %4d\r\n",
					LFSensor, DLSensor, RFSensor, DRSensor, getLeftEncCount(), getRightEncCount(), aSpeed, angle, voltage, read_Outz, read_Vref);
}

void frontWallTest(int ref) {
	int curt = micros(); //start to track time in order to make one adjust every 1000us
	readSensor();
	setLeftPwm(ref - LFSensor);
	setRightPwm(ref - RFSensor);
	elapseMicros(1000, curt);//elapse 1000 micro seconds
}

void ledTest (void) {
	if (LFSensor > 10)
		LED2_ON;
	else
		LED2_OFF;
	
	if (DLSensor > 10)
		LED1_ON;
	else
		LED1_OFF;
	
	if (DRSensor > 10)
		LED4_ON;
	else
		LED4_OFF;
	
	if (RFSensor > 10)
		LED3_ON;
	else
		LED3_OFF;
}

int main(void) {
	

	Systick_Configuration();
	LED_Configuration();
	button_Configuration();
	usart1_Configuration(9600);
  TIM4_PWM_Init();
	Encoder_Configuration();
	buzzer_Configuration();
	ADC_Config();

	ALL_LED_OFF;
	ALL_EM_OFF;
	delay_ms(1000);
	shortBeep(200, 400);	// ms, frequency
	

	
	while(1) {
		//lowBatCheck();
		//setLeftPwm(50);
		//setRightPwm(50);
		readGyro();
		printf("Angle %d\r\n", angle);
		//delay_ms(1);
	}
}


