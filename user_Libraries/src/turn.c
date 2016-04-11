/* Turning functions */
#include "turn.h"
#include "pwm.h"
#include "global.h"
#include "delay.h"
#include "stm32f4xx.h"
#include "encoder.h"
#include "speedProfile.h"
#include "maze.h"
#include <stdio.h>

void pivotTurn(float degrees) {
	if (DEBUG) printf("Pivot turning %f degrees\n\r", degrees);
	useIRSensors = 0;
	useSpeedProfile = 1;
	targetSpeedX = 0;
	int curt = millis();
	while(millis() - curt < turnDelay);
	int startLeftEncCount = leftEncCount;
	int startRightEncCount = rightEncCount;
	while( abs(leftEncCount - startLeftEncCount) + abs(rightEncCount - startRightEncCount) < mm_to_counts(3.14*wheelBase*abs(degrees)/180) ) {
		if (degrees < 0)
			targetSpeedW = turnSpeed;
		else
			targetSpeedW = -turnSpeed;
	}
	targetSpeedW = 0;
  curt = millis();
	while(millis() - curt < turnDelay);
	useSpeedProfile = 1;
}
