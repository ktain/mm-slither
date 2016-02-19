/* Test functions */
#include "stm32f4xx.h"
#include "test.h"
#include "delay.h"
#include "sensor_Function.h"
#include "pwm.h"
#include "led.h"
#include "encoder.h"
#include "global.h"

void hugFrontWall(int sensorVal) {
	int curt = micros(); //start to track time in order to make one adjust every 1000us
	readSensor();
	setLeftPwm(sensorVal - LFSensor);
	setRightPwm(sensorVal - RFSensor);
	elapseMicros(1000, curt); //elapse 1000 micro seconds
}

void stopAtFrontWall(int sensorVal) {
	if ((LFSensor < sensorVal) && (RFSensor < sensorVal)) {
		setLeftPwm(leftBaseSpeed - totalError);
		setRightPwm(rightBaseSpeed + totalError); 
	}
	else {
		turnMotorOff;
	}
}


void ledTest (void) {
	if (LFSensor > frontWallThresholdL)
		LED2_ON;
	else
		LED2_OFF;
	
	if (LDSensor > leftWallThreshold)
		LED1_ON;
	else
		LED1_OFF;
	
	if (RDSensor > rightWallThreshold)
		LED4_ON;
	else
		LED4_OFF;
	
	if (RFSensor > frontWallThresholdR)
		LED3_ON;
	else
		LED3_OFF;
}

/*	
		Function: wheelOffsetTest()
		Parameters: motor speed, ontime
		Return: right - left encoder count
 */
int wheelOffsetTest(int speed, int ontime) {
	resetLeftEncCount();
	resetRightEncCount();

	setRightPwm(speed);    
	setLeftPwm(speed);
	delay_ms(ontime);
	turnMotorOff; 
	delay_ms(100);
	
	return getRightEncCount() - getLeftEncCount();
}
