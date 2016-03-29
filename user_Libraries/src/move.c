#include "maze.h"
#include "config.h"
#include "turn.h"
#include "global.h"
#include <stdio.h>

/*
 * Filename: move.c
 * Description: Contains functions that turn to the correct orientation
 */
 
 /* Move north */
void moveN() {

	if (DEBUG) printf("Moving north\n\r");
	
  if ( orientation == 'N' ) {

  }
  else if ( orientation == 'W' ) {
    pivotTurn(turnRight90);
  }
  else if ( orientation == 'E' ) {
    pivotTurn(turnLeft90);
  }
  else if ( orientation == 'S' ) {
    pivotTurn(turnLeft180);
  }

  // Update orientation
  orientation = 'N';

}

/* Move east */
void moveE() {

	if (DEBUG) printf("Moving east\n\r");
	
  if ( orientation == 'N' ) {
    pivotTurn(turnRight90);
  }
  else if ( orientation == 'W' ) {
    pivotTurn(turnLeft180);

  }
  else if ( orientation == 'E' ) {

  }
  else if ( orientation == 'S' ) {
    pivotTurn(turnLeft90);
  }

  // Update orientation
  orientation = 'E';
}

/* Move south */
void moveS() {
	
	if (DEBUG) printf("Moving south\n\r");

  if ( orientation == 'N' ) {
    pivotTurn(turnLeft180);
  }
  else if ( orientation == 'W' ) {
    pivotTurn(turnLeft90);
  }
  else if ( orientation == 'E' ) {
    pivotTurn(turnRight90);
  }
  else if ( orientation == 'S' ) {

  }
	
  // Update orientation
  orientation = 'S';
	
}

/* Move west */
void moveW() {

	if (DEBUG) printf("Moving west\n\r");
	
  if ( orientation == 'N' ) {
    pivotTurn(turnLeft90);
  }
  else if ( orientation == 'W' ) {

  }
  else if ( orientation == 'E' ) {
    pivotTurn(turnLeft180);
  }
  else if ( orientation == 'S' ) {
    pivotTurn(turnRight90);
  }

  // Update orientation
  orientation = 'W';

}


/* Move back */
void moveBack(char prevRelLoc) {
  if (prevRelLoc == 'N')
    moveN();
  else if (prevRelLoc == 'E')
    moveE();
  else if (prevRelLoc == 'S')
    moveS();
  else if (prevRelLoc == 'W')
    moveW();
}
