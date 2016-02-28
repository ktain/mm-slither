/* PID Control */
#include "stdbool.h"
#include "stm32f4xx.h"
#include "pid.h"
#include "encoder.h"
#include "global.h"
#include "config.h"
#include "sensor_Function.h"

/* Global variables */
int totalError = 0;
int errorP;
int errorD;
int oldErrorP;



void pid(void) {

	// Use left and right walls
	if((LDSensor > LDMiddleValue) && (RDSensor > RDMiddleValue)) {  
		errorP = RDSensor - LDSensor;
		errorD = errorP - oldErrorP;
	}
	
	// Use left wall only
	else if(LDSensor > leftWallThreshold) {
		errorP = 1 * (LDMiddleValue - LDSensor);
		errorD = errorP - oldErrorP;
	}
	// Use right wall only
	else if(RDSensor > rightWallThreshold)
	{
		errorP = 1 * (RDSensor - RDMiddleValue);
		errorD = errorP - oldErrorP;
	}
	// Use encoders
	else {
		errorP = (getLeftEncCount() - getRightEncCount()) * 1024 / encResolution;
		errorD = 0;
	}

	totalError = KP * errorP + KD * errorD;
	oldErrorP = errorP;
		
}

void resetError(void) {
	errorD = 0;
	errorP = 0;
	oldErrorP = 0;
	totalError = 0;
}
