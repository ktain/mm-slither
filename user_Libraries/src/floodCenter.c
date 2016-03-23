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

bool hasFrontWall = 0;
bool hasLeftWall = 0;
bool hasRightWall = 0;
int nextMove = 0;
char orientation = 'N';


/*
 * Random search using pivot turns
 */
void floodCenter(void) {
	isWaiting = 0;
	isSearching = 1;
	isSpeedRunning = 0;
	
	int cellCount = 1;						// number of explored cells
	int remainingDist = 0;				// positional distance
	bool beginCellFlag = 0;
	bool quarterCellFlag = 0;
	bool halfCellFlag = 0;
	bool threeQuarterCellFlag = 0;
	bool fullCellFlag = 0;

	int distN = 0;   // distances around current position
  int distE = 0;
  int distS = 0;
  int distW = 0;
	
	// Place trace at starting position
  if (!hasTrace(block[yPos][xPos])) {
    block[yPos][xPos] |= 16;
    traceCount++;
	}
	
	targetSpeedX = searchSpeed;
	
	while(!atCenter()) {
		useIRSensors = 0;
		useGyro = 0;
		useSpeedProfile = 1;
		
		remainingDist = cellCount*cellDistance - encCount;
		
		// Beginning of cell
		if (!beginCellFlag && (remainingDist <= cellDistance))	{	// run once
			beginCellFlag = 1;
			
			// Update position
			if (orientation == 'N') {
				yPos += 1;
			}
			if (orientation == 'E') {
				xPos += 1;
			}
			if (orientation == 'S') {
				yPos -= 1;
			}
			if (orientation == 'W') {
				xPos -= 1;
			}
		}
		
		// Reached quarter cell
		if (!quarterCellFlag && (remainingDist <= cellDistance*3/4))	{	// run once
			quarterCellFlag = 1;
		}
		
		if (quarterCellFlag && !threeQuarterCellFlag)	// middle half
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
			
		
			// Store next cell's wall data
			if (DEBUG) printf("Detecting walls\n\r");
			detectWalls();
			
			// Update distance for current block
			if (DEBUG) printf("Updating distance for current block\n\r");
			distance[yPos][xPos] = getMin(xPos, yPos) + 1;
			
			// Update distances for every other block
			if (DEBUG) printf("Updating distances\n\r");
			updateDistance();
			
			// Get distances around current block
			distN = hasNorth(block[yPos][xPos])? 500 : distance[yPos + 1][xPos];
			distE = hasEast(block[yPos][xPos])? 500 : distance[yPos][xPos + 1];
			distS = hasSouth(block[yPos][xPos])? 500 : distance[yPos - 1][xPos];
			distW = hasWest(block[yPos][xPos])? 500 : distance[yPos][xPos - 1];
			if (DEBUG) printf("distN %d, distE %d, distS %d, distW %d\n\r", distN, distE, distS, distW);
			
			// Decide next movement
			if (DEBUG) printf("Deciding next movement\n\r");
			// 1. Pick the shortest route
			if ( (distN < distE) && (distN < distS) && (distN < distW) )
				nextMove = MOVEN;
			else if ( (distE < distN) && (distE < distS) && (distE < distW) )
				nextMove = MOVEE;
			else if ( (distS < distE) && (distS < distN) && (distS < distW) )
				nextMove = MOVES;
			else if ( (distW < distE) && (distW < distS) && (distW < distN) )
				nextMove = MOVEW;
			 
			// 2. If multiple equally short routes, go straight if possible
			else if ( orientation == 'N' && !hasNorth(block[yPos][xPos]) )
				nextMove = MOVEN;
			else if ( orientation == 'E' && !hasEast(block[yPos][xPos]) )
				nextMove = MOVEE;
			else if ( orientation == 'S' && !hasSouth(block[yPos][xPos]) )
				nextMove = MOVES;
			else if ( orientation == 'W' && !hasWest(block[yPos][xPos]) )
				nextMove = MOVEW;
			 
			// 3. Otherwise prioritize N > E > S > W
			else if (!hasNorth(block[yPos][xPos]))
				nextMove = MOVEN;
			else if (!hasEast(block[yPos][xPos]))
				nextMove = MOVEE;
			else if (!hasSouth(block[yPos][xPos]))
				nextMove = MOVES;
			else if (!hasWest(block[yPos][xPos]))
				nextMove = MOVEW;
			
			if (DEBUG) printf("nextMove %d\n\r", nextMove);
			
			else {
				if (DEBUG) {
					printf("Stuck... Can't find center.\n\r");
					turnMotorOff;
					useSpeedProfile = 0;
					while(1);
				}
			}
			
			if (DEBUG) printf("Placing pseudo walls\n\r");
			
			// Isolate known dead ends with pseudo walls
			for (int i = SIZE*SIZE; i >= 0; i--) {
				for (int j = 0; j < SIZE; j++) {
					for (int k = 0; k < SIZE; k++) {
						if ( !((j == 0) && (k == 0)))
							// If dead end, isolate block
							if ((hasNorth(
								block[j][k]) + hasEast(block[j][k]) +
									hasSouth(block[j][k]) + hasWest(block[j][k])) >= 3) {
								block[j][k] |= 32;
								block[j][k] |= 15;
								if (j < SIZE - 1)  // Update adjacent wall
									block[j + 1][k] |= 4;
								if (k < SIZE - 1)  // Update adjacent wall
									block[j][k + 1] |= 8;
								if (j > 0)         // Update adjacent wall
									block[j - 1][k]  |= 1;
								if (k > 0)         // Update adjacent wall
									block[j][k - 1] |= 2;
							}
					}
				}
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
		if (hasFrontWall || willTurn()) {
			if(needToDecelerate(remainingDist, (int)speed_to_counts(curSpeedX), (int)speed_to_counts(stopSpeed)) < decX)
				targetSpeedX = searchSpeed;
			else
				targetSpeedX = stopSpeed;
		}
		else 
			targetSpeedX = searchSpeed;
		
		// Reached full cell
		if ((!fullCellFlag && (remainingDist <= 0))) {	// run once
			if (DEBUG) printf("Reached full cell\n\r");
			fullCellFlag = 1;
			cellCount++;
			shortBeep(200, 1000);
			
			// If has front wall, align with front wall
			if (hasFrontWall) {
				alignFrontWall(1360, 1330, alignTime);	// left, right, duration
			}
			
			// Reached full cell, perform next move
			if (nextMove == MOVEN) {
				moveN();
			}
			else if (nextMove == MOVEE) {
				moveE();
			}
			else if (nextMove == MOVES) {
				moveS();
			}
			else if (nextMove == MOVEW) {
				moveW();
			}
			
			beginCellFlag = 0;
			quarterCellFlag = 0;
			halfCellFlag = 0;
			threeQuarterCellFlag = 0;
			fullCellFlag = 0;
			hasFrontWall = 0;
			hasLeftWall = 0;
			hasRightWall = 0;
		}
	}
	
	targetSpeedX = 0;
	
	// Reached center
	// Move one more cell
	alignFrontWall(1360, 1330, 1000);
	
	// Update position
	if (orientation == 'N') {
		yPos += 1;
	}
	if (orientation == 'E') {
		xPos += 1;
	}
	if (orientation == 'S') {
		yPos -= 1;
	}
	if (orientation == 'W') {
		xPos -= 1;
	}
		
	useSpeedProfile = 0;
	turnMotorOff;
	
	if (DEBUG) visualizeGrid();
}



// Update distances for every other block while flooding the center
// slow...
void updateDistance() {
  
  int i, j, k;
  for (i = SIZE*SIZE; i >= 0; i--) {
    for (j = 0; j < SIZE; j++) {
      for (k = 0; k < SIZE; k++) {
          if ((j == (SIZE/2)-1 || (j == SIZE/2)) &&
              ((k == (SIZE/2)-1) || (k == SIZE/2)))
            ;
          else
            distance[j][k] = getMin(k, j) + 1;
      }
    }
  }
}


void detectWalls() {
  
	if (orientation == 'N') {
		if (hasFrontWall) {
			block[yPos][xPos] |= 1;
			if (yPos < SIZE - 1)  // Update adjacent wall
				block[yPos + 1][xPos] |= 4;
		}
		if (hasLeftWall) {
			block[yPos][xPos] |= 8;
			if (xPos > 0)  // Update adjacent wall
				block[yPos][xPos - 1] |= 2;
		}
		if (hasRightWall) {
			block[yPos][xPos] |= 2;
			if (xPos < SIZE - 1)  // Update adjacent wall
				block[yPos][xPos + 1] |= 8;
		}		
	}
	else if (orientation == 'E') {
		if (hasFrontWall) {
			block[yPos][xPos] |= 2;
			if (xPos < SIZE - 1)  // Update adjacent wall
				block[yPos][xPos + 1] |= 8;
		}
		if (hasLeftWall) {
		  block[yPos][xPos] |= 1;
			if (yPos < SIZE - 1)  // Update adjacent wall
				block[yPos + 1][xPos] |= 4;
		}
		if (hasRightWall) {
			block[yPos][xPos] |= 4;
			if (yPos > 0)  // Update adjacent wall
				block[yPos - 1][xPos] |= 1;
		}
	}
	else if (orientation == 'S') {
		if (hasFrontWall) {
			block[yPos][xPos] |= 4;
			if (yPos > 0)  // Update adjacent wall
				block[yPos - 1][xPos] |= 1;
		}
		if (hasLeftWall) {
			block[yPos][xPos] |= 2;
			if (xPos < SIZE - 1)  // Update adjacent wall
				block[yPos][xPos + 1] |= 8;
		}
		if (hasRightWall) {
			block[yPos][xPos] |= 8;
			if (xPos > 0)  // Update adjacent wall
				block[yPos][xPos - 1] |= 2;
		}
	}
	else if (orientation == 'W') {
		if (hasFrontWall) {
			block[yPos][xPos] |= 8;
			if (xPos > 0)  // Update adjacent wall
				block[yPos][xPos - 1] |= 2;
		}
		if (hasLeftWall) {
			block[yPos][xPos] |= 4;
			if (yPos > 0)  // Update adjacent wall
				block[yPos - 1][xPos] |= 1;
		}
		if (hasRightWall) {
			block[yPos][xPos] |= 1;
			if (yPos < SIZE - 1)  // Update adjacent wall
				block[yPos + 1][xPos] |= 4;
		}
	}	
}

bool willTurn(void) {
	if ( (orientation == 'N' && nextMove == MOVEN) || (orientation == 'E' && nextMove == MOVEE) || (orientation == 'S' && nextMove == MOVES) || (orientation == 'W' && nextMove == MOVEW) )
		return 0;
	else return 1;
}
