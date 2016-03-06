/* Alignment */
#include "align.h"
#include "pwm.h"
#include "global.h"
#include "delay.h"
#include "encoder.h"

void alignFrontWall(int LSensorVal, int RSensorVal){
	int tempPwm = maxPwm;
	maxPwm = alignPwm;
	useIRSensors = 1;
	useSpeedProfile = 0;
	int timeAllotted = millis();
	while (millis() - timeAllotted < 300) {
		setLeftPwm(LSensorVal - LFSensor);
		setRightPwm(RSensorVal - RFSensor);
	}
	// Ignore encoder count changes
	setLeftEncCount(leftEncCount);
	setRightEncCount(rightEncCount);
	maxPwm = tempPwm;
	useIRSensors = 0;
	useSpeedProfile = 1;
}
