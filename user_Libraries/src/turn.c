/* Turning functions */
#include "turn.h"
#include "pwm.h"
#include "global.h"
#include "delay.h"
#include "stm32f4xx.h"
#include "encoder.h"
#include "speedProfile.h"

void pivotTurn(float degrees) {
	resetSpeedProfile();
	useGyro = 0;
	
	while( abs(leftEncCount) + abs(rightEncCount) < dist_mm_to_counts(3.14*wheelBase*abs(degrees)/180) ) {
		if (degrees < 0)
			targetSpeedW = turnSpeed;
		else
			targetSpeedW = -turnSpeed;
	}
	targetSpeedW = 0;
	oldEncCount = encCount;
	resetSpeedProfile();
}
