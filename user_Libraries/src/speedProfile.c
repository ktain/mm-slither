/**
 *	Speed profile with PID
 */
 
#include "stdbool.h"
#include "stm32f4xx.h"
#include "speedProfile.h"
#include "global.h"
#include "encoder.h"
#include "pwm.h"
#include "config.h"
#include "maze.h"
#include <stdio.h>

float curSpeedX = 0;
float curSpeedW = 0;
int targetSpeedX = 0;
int targetSpeedW = 0;
int encoderFeedbackX = 0;
int encoderFeedbackW = 0;
float pidInputX = 0;
float pidInputW = 0;
float posErrorX = 0;
float posErrorW = 0;
float oldPosErrorX = 0;
float oldPosErrorW = 0;
int posPwmX = 0;
int posPwmW = 0;
float kpX = 2;
float kdX = 4;
float kpW = 1;
float kdW = 12;						//used in straight, default 12
float kpW1 = 1;						//used for T1 and T3 in curve turn, default 1
float kdW1 = 26;
float kpW2 = 1;						//used for T2 in curve turn
float kdW2 = 36;
float accX = 30;					// acc/dec in mm/ms/ms
float decX = 30; 				 
float accW = 1; 					// cm/s^2
float decW = 1;	

int leftBaseSpeed = 0;
int rightBaseSpeed = 0;
int32_t leftEncCount = 0;
int32_t rightEncCount = 0;
int32_t leftEncOld = 0;
int32_t rightEncOld = 0;
int leftEncChange = 0;
int rightEncChange = 0;
int encChange = 0;
int32_t distanceLeft = 0;
int32_t encCount = 0;
int32_t oldEncCount = 0;
int sensorError = 0;
int sensorScale = 50;

int gyroFeedbackRatio = 5700;



void speedProfile(void)
{	
	getEncoderStatus();
	updateCurrentSpeed();
	calculateMotorPwm();
}


// Updates encoder counts and distance left
void getEncoderStatus(void)
{
	leftEncCount = getLeftEncCount();
	rightEncCount = getRightEncCount();

	leftEncChange = leftEncCount - leftEncOld;
	rightEncChange = rightEncCount - rightEncOld;
	encChange = (leftEncChange + rightEncChange)/2;	 

	leftEncOld = leftEncCount;
	rightEncOld = rightEncCount;

	leftEncCount += leftEncChange;
	rightEncCount += rightEncChange;
	encCount = (leftEncCount + rightEncCount)/2;
	
	distanceLeft -= encChange;		// update distanceLeft
}


// Updates current speed based on target speed
void updateCurrentSpeed(void) {
	if(curSpeedX < targetSpeedX)
	{
		curSpeedX += ((accX*2)/100);
		if(curSpeedX > targetSpeedX)
			curSpeedX = targetSpeedX;
	}
	else if(curSpeedX > targetSpeedX)
	{
		curSpeedX -= ((decX*2)/100);
		if(curSpeedX < targetSpeedX)
			curSpeedX = targetSpeedX;
	}
	if(curSpeedW < targetSpeedW)
	{
		curSpeedW += accW;
		if(curSpeedW > targetSpeedW)
			curSpeedW = targetSpeedW;
	}
	else if(curSpeedW > targetSpeedW)
	{
		curSpeedW -= decW;
		if(curSpeedW < targetSpeedW)
			curSpeedW = targetSpeedW;
	}	
}


void calculateMotorPwm(void) { // encoder PD controller
	int gyroFeedback = 0;
	int rotationalFeedback = 0;
	int sensorFeedback = 0;
	
  /* simple PD loop to generate base speed for both motors */	
	encoderFeedbackX = rightEncChange + leftEncChange;
	encoderFeedbackW = rightEncChange - leftEncChange;
	
	gyroFeedback = aSpeed/gyroFeedbackRatio; 	//gyroFeedbackRatio mentioned in curve turn lecture

	if(useOnlyGyroFeedback)
		rotationalFeedback = gyroFeedback;
	else if(useOnlyEncoderFeedback)
		rotationalFeedback = encoderFeedbackW;
	else
		rotationalFeedback = encoderFeedbackW + gyroFeedback;
	
	// option to include sensor feedback
		
	posErrorX += curSpeedX - encoderFeedbackX;
	posErrorW += curSpeedW - rotationalFeedback;
	
	if (useIRSensors) {
		getSensorError();
		sensorFeedback = sensorError/sensorScale;
		posErrorW += sensorFeedback;
	}
	
	posPwmX = kpX * posErrorX + kdX * (posErrorX - oldPosErrorX);
	posPwmW = kpW * posErrorW + kdW * (posErrorW - oldPosErrorW);
	
	oldPosErrorX = posErrorX;
	oldPosErrorW = posErrorW;
	
	leftBaseSpeed = posPwmX - posPwmW;
	rightBaseSpeed = posPwmX + posPwmW;

	setLeftPwm(leftBaseSpeed);
	setRightPwm(rightBaseSpeed);	
}


/**
 *	Function: needToDecelerate
 *	Parameters: dist - encoder counts
 *				 			curSpd - counts/ms
 *				 			endSpd - counts/ms
 *	Return: positive deceleration in cm/s
 */
int needToDecelerate(int32_t dist, int16_t curSpd, int16_t endSpd) 
{
	if (curSpd<0) 
		curSpd = -curSpd;	// take absolute value
	if (endSpd<0) 
		endSpd = -endSpd;
	if (dist<0) 
		dist = 1;			// prevent negative distance
	if (dist == 0) 
		dist = 1;  // prevent dividing by 0
	
	int estimatedDecX = ((curSpd*curSpd - endSpd*endSpd)*100/counts_to_mm(dist)/4);
	
	return (estimatedDecX < 0)? -estimatedDecX : estimatedDecX;	// cm/s/s
}




void resetSpeedProfile(void)
{
	//resetEverything;
	
	//disable sensor data collecting functions running in 1ms interrupt
 	useIRSensors = 0;
	useSpeedProfile = 0;
	turnMotorOff;
	
	pidInputX = 0;
	pidInputW = 0;
	curSpeedX = 0;
	curSpeedW = 0;
	targetSpeedX = 0;
	targetSpeedW = 0;
	posErrorX = 0;
	posErrorW = 0;
	oldPosErrorX = 0;
	oldPosErrorW = 0;
  leftEncOld = 0;

	rightEncOld = 0;	
	leftEncCount = 0;
	rightEncCount = 0;
	encCount = 0;	
	oldEncCount = 0;
	leftBaseSpeed = 0;
	rightBaseSpeed = 0;
	distanceLeft = 0;

	resetLeftEncCount();
	resetRightEncCount();
}

float getDecNeeded (float d, float Vf, float Vi) {
	if (d <= 0) {
		d = 1;
	}
	
	return abs( (Vf*Vf - Vi*Vi)/d/8 );
}	

// convert counts/ms to speed in mm/ms
float counts_to_mm (int counts) {
	return (counts/countspermm);
}


// convert speed in mm/s to counts/ms,
float mm_to_counts (float speed) {
	return (speed*countspermm);	// truncate from float to int
}


// get absolute value
float abs (float number) {
	return (number<0)? -number : number;
}


/**
 *	Straight movement
 */
void moveForward(int cells) {
	resetSpeedProfile();
	useIRSensors = 1;
	useSpeedProfile = 1;
	
	int remainingDist = cells*cellDistance;

	while( encCount < cells*cellDistance ) {
		//printf("getDecNeeded = %f\n\r", getDecNeeded(counts_to_mm(remainingDist), curSpeedX, 0));
		//printf("speed = %f\n\r", curSpeedX);
		//printf("remainingDist in mm = %f\n\r", counts_to_mm(remainingDist));
		//printf("remaining distance %d\n\r", remainingDist);
		remainingDist = cells*cellDistance - encCount;
		if (remainingDist < cellDistance/2) {
			useIRSensors = 0;
		}
		if (getDecNeeded(counts_to_mm(remainingDist), curSpeedX, 0) < decX) {
			targetSpeedX = moveSpeed;
		}
		else {
			targetSpeedX = 0;
		}
	}
	targetSpeedX = 0;
	//printf("remainingDist = %f\n\r", counts_to_mm(remainingDist));
}



void getSensorError(void)
{
	if (LDSensor > LDMiddleValue && RDSensor > RDMiddleValue)
		sensorError = RDSensor - LDSensor;
	if(LDSensor > LDMiddleValue)
		sensorError = LDMiddleValue - LDSensor;
	else if(RDSensor > RDMiddleValue)
		sensorError = RDSensor - RDMiddleValue;
	else
		sensorError = 0;
}

