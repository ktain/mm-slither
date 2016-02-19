/* Turning functions */
#include "turn.h"
#include "pwm.h"
#include "global.h"
#include "delay.h"
#include "stm32f4xx.h"
#include "encoder.h"

// Must be called in a loop
void turn(int turnProfile) {
	setLeftPwm(turnKP[turnProfile]*(turnAngle[turnProfile] + angle));
	setRightPwm(-turnKP[turnProfile]*(turnAngle[turnProfile] + angle));
}
