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
float accX = 70;					// acc/dec in cm/s/s
float decX = 600; 				// default 600 = 6m/s/s  
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
int sensorScale = 20;

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
		curSpeedX += (float)(speed_to_counts(accX*2)/100);
		if(curSpeedX > targetSpeedX)
			curSpeedX = targetSpeedX;
	}
	else if(curSpeedX > targetSpeedX)
	{
		curSpeedX -= (float)(speed_to_counts(decX*2)/100);
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
	
	int estimatedDecX = ((curSpd*curSpd - endSpd*endSpd)*100/dist_counts_to_mm(dist)/4);
	
	return (estimatedDecX < 0)? -estimatedDecX : estimatedDecX;	// cm/s/s
}


void resetSpeedProfile(void)
{
	//resetEverything;
	
	//disable sensor data collecting functions running in 1ms interrupt
 	useIRSensors = false;
 	useGyro = false;
	
	//no PID calculating, no motor lock
	usePID = false;	

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

	resetLeftEncCount();
	resetRightEncCount();
}


// convert distance in counts to mm
float dist_counts_to_mm(float counts) {
	return (counts*wheelCircumference/encResolution/gearRatio);
}


// convert distance in mm to counts 
float dist_mm_to_counts(float mm) {
	return (mm*encResolution*gearRatio/wheelCircumference);
}

// convert counts/ms to speed in cm/s
float counts_to_speed (int counts) {
	return 0;
}


// convert speed in cm/s to counts/ms,
float speed_to_counts (float speed) {
	return (speed*encResolution*gearRatio/wheelCircumference/100);	// truncate from float to int
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
	useGyro = 0;
	usePID = 1;
	useSpeedProfile = 1;
	isWaiting = 0;
	isSearching = 0;
	isSpeedRunning = 1;
	
	int	remainingDist = cells*cellDistance;
	
	targetSpeedX = maxSpeed;
	
	while( (encCount - oldEncCount) < cells*cellDistance ) {
		remainingDist = cells*cellDistance - encCount;
		if(remainingDist < cellDistance/2)
			useIRSensors = 0;
		if(needToDecelerate(remainingDist, (int)speed_to_counts(curSpeedX), (int)speed_to_counts(moveSpeed)) < decX)
			targetSpeedX = maxSpeed;
		else
			targetSpeedX = moveSpeed;
	}
	targetSpeedX = 0;
	turnMotorOff;
	oldEncCount = encCount;
	
	useSpeedProfile = 0;
	isSpeedRunning = 0;
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

