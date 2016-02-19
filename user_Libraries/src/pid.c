/* PID Control */
#include "stdbool.h"
#include "stm32f4xx.h"
#include "pid.h"
#include "encoder.h"
#include "global.h"
#include "config.h"
#include "sensor_Function.h"


void pid(void) {

	if (1) {
		// Use left and right walls
		if((LDSensor > leftWallThreshold) && (RDSensor > rightWallThreshold)) {  
			errorP = RDSensor - LDSensor;
			errorD = errorP - oldErrorP;
		}
		
		// Use left wall only
		else if(LDSensor > leftWallThreshold) {
			errorP = 1 * (leftMiddleValue - LDSensor);
			errorD = errorP - oldErrorP;
		}
		// Use right wall only
		else if(RDSensor > rightWallThreshold)
		{
			errorP = 1 * (RDSensor - rightMiddleValue);
			errorD = errorP - oldErrorP;
		}
		
			// Use encoders
		else {
			errorP = (getLeftEncCount() - getRightEncCount()) * 1024 / ENC_RESOLUTION;
			errorD = 0;
		}
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
