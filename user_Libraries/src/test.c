/* Test functions */
#include "stm32f4xx.h"
#include "test.h"
#include "delay.h"
#include "sensor_Function.h"
#include "pwm.h"
#include "led.h"
#include "encoder.h"
#include "global.h"
#include "speedProfile.h"
#include "config.h"
#include "turn.h"
#include "buzzer.h"


/**
 *	Hug Front Wall
 */
void hugFrontWall(int LSensorVal, int RSensorVal) {
	while (1) {
		int curt = micros(); //start to track time in order to make one adjust every 1000us
		readSensor();
		setLeftPwm(LSensorVal - LFSensor);
		setRightPwm(RSensorVal - RFSensor);
		elapseMicros(1000, curt); //elapse 1000 micro seconds
	}
}


void ledTest (void) {
	if (LFSensor > frontWallThresholdL)
		LED2_ON;
	else
		LED2_OFF;
	
	if (LDSensor > leftWallThreshold)
		LED1_ON;
	else
		LED1_OFF;
	
	if (RDSensor > rightWallThreshold)
		LED4_ON;
	else
		LED4_OFF;
	
	if (RFSensor > frontWallThresholdR)
		LED3_ON;
	else
		LED3_OFF;
}

/*	
		Function: wheelOffsetTest()
		Parameters: motor speed, ontime
		Return: right - left encoder count
 */
int wheelOffsetTest(int speed, int ontime) {
	resetLeftEncCount();
	resetRightEncCount();

	setLeftPwm(speed);
	setRightPwm(speed);    
	delay_ms(ontime);
	turnMotorOff; 
	delay_ms(100);
	
	return getRightEncCount() - getLeftEncCount();
}



/*
 * Random search using pivot turns
 */
void randomSearch(void) {
	isWaiting = 0;
	isSearching = 1;
	isSpeedRunning = 0;
	
	int cellCount = 1;						// number of explored cells
	int turnCount = 0;
	int remainingDist = 0;				// positional distance
	bool quarterCellFlag = 0;
	bool halfCellFlag = 0;
	bool threeQuarterCellFlag = 0;
	bool fullCellFlag = 0;
	bool hasFrontWall = 0;
	bool hasLeftWall = 0;
	bool hasRightWall = 0;
	int nextMove = 0;
	
	targetSpeedX = searchSpeed;
	
	while(1) {	// run forever
		useIRSensors = 0;
		useGyro = 0;
		useSpeedProfile = 1;
		
		remainingDist = cellCount*cellDistance - encCount;
		// Add check for front wall to turn off diag sensors briefly
		
		
		// Reached quarter cell
		if (!quarterCellFlag && (remainingDist <= cellDistance*3/4))	{
			quarterCellFlag = 1;
		}
		
		if (quarterCellFlag && !threeQuarterCellFlag)
			useIRSensors = 1;
		
		
		// Reached half cell
		if (!halfCellFlag && (remainingDist <= cellDistance/2)) {		// Run once
			halfCellFlag = 1;
			// Read wall and set wall flags
			if ((LFSensor > frontWallThresholdL) && (RFSensor > frontWallThresholdR))
				hasFrontWall = 1;
			if (LDSensor > leftWallThreshold)
				hasLeftWall = 1;
			if (RDSensor > rightWallThreshold)
				hasRightWall = 1;
			
			// Store destination cell's wall data
			
			// Get next movement (search algorithm)
			if (!hasFrontWall)
				nextMove = GOFORWARD;
			else if (!hasLeftWall)
				nextMove = TURNLEFT;
			else if (!hasRightWall)
				nextMove = TURNRIGHT;
			else
				nextMove = TURNBACK;
		}
		
		// Reached three quarter cell
		if (!threeQuarterCellFlag && (remainingDist <= cellDistance*1/4)) {
			threeQuarterCellFlag = 1;
		}
		
		if (threeQuarterCellFlag) {
			// Turn IRSensors off for the remaining distance
			if (hasFrontWall)
				useIRSensors = 0;
		}
		
		
		// If has front wall or needs to turn, decelerate to 0 within half a cell distance
		if (hasFrontWall || nextMove == TURNLEFT || nextMove == TURNRIGHT || nextMove == TURNBACK) {
			if(needToDecelerate(remainingDist, (int)speed_to_counts(curSpeedX), (int)speed_to_counts(stopSpeed)) < decX)
				targetSpeedX = searchSpeed;
			else
				targetSpeedX = stopSpeed;
		}
		
		// Reached full cell
		if (!fullCellFlag && (remainingDist <= 0)) {
			fullCellFlag = 1;
			cellCount++;
			shortBeep(200, 1000);
			
			// If has front wall, align with front wall
			
			// Reached full cell, perform next move
			if (nextMove == TURNLEFT) {
				pivotTurn(TURNLEFT90);
				turnCount++;
			}
			else if (nextMove == TURNRIGHT) {
				pivotTurn(TURNRIGHT90);
				turnCount++;
			}
			else if (nextMove == TURNBACK) {
				pivotTurn(TURNLEFT180);
				turnCount++;
			}
			else if (nextMove == GOFORWARD) {
				// Continue moving forward
			}
			
			quarterCellFlag = 0;
			halfCellFlag = 0;
			threeQuarterCellFlag = 0;
			fullCellFlag = 0;
			hasFrontWall = 0;
			hasLeftWall = 0;
			hasRightWall = 0;
			
		}
	}
	
}

