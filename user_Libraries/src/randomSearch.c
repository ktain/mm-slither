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
#include "align.h"
#include "maze.h"
#include <stdio.h>

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
			
			// Decide next movement (search algorithm)
			while (1) {
				nextMove = (millis() % 4) + 1;
				if ((nextMove == GOFORWARD) && (!hasFrontWall))
					break;	
				if ((nextMove == TURNLEFT) && (!hasLeftWall))
					break;
				if ((nextMove == TURNRIGHT) && (!hasRightWall))
					break;
				if ((nextMove == TURNBACK) && (hasFrontWall && hasLeftWall && hasRightWall))
					break;
			}

		}
		
		// Reached three quarter cell
		if (!threeQuarterCellFlag && (remainingDist <= cellDistance*1/4)) {	// run once
			threeQuarterCellFlag = 1;
		}
		
		
		if (threeQuarterCellFlag) {
			// Check for front wall to turn off for the remaining distance
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
		if ((!fullCellFlag && (remainingDist <= 0)) || (LFSensor > 2000) || (RFSensor > 2000)) {
			fullCellFlag = 1;
			cellCount++;
			shortBeep(200, 1000);
			
			// If has front wall, align with front wall
			if (hasFrontWall) {
				alignFrontWall(1360, 1330, 100);	// left, right, duration
			}
			
			
			// Reached full cell, perform next move
			if (nextMove == TURNLEFT) {
				pivotTurn(turnLeft90);
				turnCount++;
			}
			else if (nextMove == TURNRIGHT) {
				pivotTurn(turnRight90);
				turnCount++;
			}
			else if (nextMove == TURNBACK) {
				pivotTurn(turnLeft180);
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

