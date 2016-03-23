#include "maze.h"

/*
 * Filename: lookup.c
 * Description: Contains functions to lookup block information.
 */

/* Check for north wall */
int hasNorth(int currentBlock) {
  return ((currentBlock & 1) == 1)? 1 : 0;
}

/* Check for east wall */
int hasEast(int currentBlock) {
  return ((currentBlock & 2) == 2)? 1 : 0;
}

/* Check for south wall */
int hasSouth(int currentBlock) {
  return ((currentBlock & 4) == 4)? 1 : 0;
}

/* Check for west wall */
int hasWest(int currentBlock) {
  return ((currentBlock & 8) == 8)? 1 : 0;
}

/* Check for Trace */
int hasTrace(int currentBlock) {
  return ((currentBlock & 16) == 16)? 1 : 0;
}

/* Check for dead end */
int isDeadEnd(int currentBlock) {
  return ((currentBlock & 32) == 32)? 1: 0;
}

/* Check if object at center, and place pseudo walls accordingly */
int atCenter() {
  if ( (((SIZE - 1)/2 == xPos) || (SIZE/2 == xPos)) &&
       (((SIZE - 1)/2 == yPos) || (SIZE/2 == yPos)) ) {
         
    // not at southwest
    if ( !((yPos == (SIZE-1)/2) && (xPos == (SIZE-1)/2)) ) {
      block[(SIZE-1)/2][(SIZE-1)/2] |= 12;  // Place pseudo walls on all sides
      // Update adjacent walls
      block[(SIZE - 1) / 2 - 1][(SIZE - 1) / 2] |= 1;
      block[(SIZE - 1) / 2][(SIZE - 1) / 2 - 1] |= 2;
    }
    
    // not at northwest
    if ( !((yPos == SIZE/2) && (xPos == (SIZE-1)/2)) ) {
      block[SIZE/2][(SIZE-1)/2] |= 9;     // Place pseudo walls on all sides     
      // Update adjacent walls
      block[SIZE / 2 + 1][(SIZE - 1) / 2] |= 4;
      block[SIZE / 2][(SIZE - 1) / 2 - 1] |= 2;
    }
    
    // not at northeast
    if ( !((yPos == SIZE/2) && (xPos == SIZE/2)) ) {
      block[SIZE/2][SIZE/2] |= 3;        // Place pseudo walls on all sides
      // Update adjacent walls
      block[SIZE / 2 + 1][SIZE / 2] |= 4;
      block[SIZE / 2][SIZE / 2 + 1] |= 8; 
    }
    
    // not at southeast
    if ( !((yPos == (SIZE-1)/2) && (xPos == SIZE/2)) ) {
      block[(SIZE-1)/2][SIZE/2] |= 4;    // Place pseudo walls on all sides     
      // Update adjacent walls
      block[(SIZE - 1) / 2 - 1][SIZE / 2] |= 1;
      block[(SIZE - 1) / 2][SIZE / 2 + 1] |= 8;
    }
    
    return 1;
    
  }
  else {  // not at center
    return 0;
  }
}

  
/* 
 * Return the smallest distance from the surrounding blocks
 * that are not separated by a wall
 */
int getMin(int x, int y) {
   int min;
   int distN;
   int distE;
   int distS;
   int distW;
   
   distN = hasNorth(block[y][x])? MAX_DIST : distance[y + 1][x];
   distE = hasEast(block[y][x])? MAX_DIST : distance[y][x + 1];
   distS = hasSouth(block[y][x])? MAX_DIST : distance[y - 1][x];
   distW = hasWest(block[y][x])? MAX_DIST : distance[y][x - 1];
   
   min = distN;
   min = (distE < min)? distE : min;
   min = (distS < min)? distS : min;
   min = (distW < min)? distW : min;
   
   return min;
}
