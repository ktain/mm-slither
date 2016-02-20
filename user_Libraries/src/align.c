/* Alignment */
#include "align.h"
#include "pwm.h"
#include "global.h"

// offset
void alignFrontWall(void){
	setLeftPwm(alignLFVal - LFSensor);
	setRightPwm(alignRFVal - RFSensor);	
}
