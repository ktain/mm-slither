/* Turning functions */
#include "turn.h"
#include "pwm.h"
#include "global.h"
#include "delay.h"
#include "stm32f4xx.h"
#include "encoder.h"
#include "speedProfile.h"

void pivotTurn(float degrees) {
	int startLeftEncCount = leftEncCount;
	int startRightEncCount = rightEncCount;
	targetSpeedX = 0;
	int curt = millis();
	while(millis() - curt < 200);
	while( abs(leftEncCount - startLeftEncCount) + abs(rightEncCount - startRightEncCount) < dist_mm_to_counts(3.14*wheelBase*abs(degrees)/180) ) {
		if (degrees < 0)
			targetSpeedW = turnSpeed;
		else
			targetSpeedW = -turnSpeed;
	}
	targetSpeedW = 0;
  curt = millis();
	while(millis() - curt < 200);

}
