#include "maze.h"

/*
 * Filename: lookup.c
 * Description: Contains functions to lookup block information.
 */

/* Check for north wall */
unsigned char hasNorth(const unsigned char currentBlock) {
  return ((currentBlock & NWALL) == NWALL)? 1 : 0;
}

/* Check for east wall */
unsigned char hasEast(const unsigned char currentBlock) {
  return ((currentBlock & EWALL) == EWALL)? 1 : 0;
}

/* Check for south wall */
unsigned char hasSouth(const unsigned char currentBlock) {
  return ((currentBlock & SWALL) == SWALL)? 1 : 0;
}

/* Check for west wall */
unsigned char hasWest(const unsigned char currentBlock) {
  return ((currentBlock & WWALL) == WWALL)? 1 : 0;
}

/* Check for Trace */
unsigned char hasTrace(const unsigned char currentBlock) {
  return ((currentBlock & TRACE) == TRACE)? 1 : 0;
}

/* Check for dead end */
unsigned char isDeadEnd(const unsigned char currentBlock) {
  return ((currentBlock & DE) == DE)? 1 : 0;
}

/* 
 * Return the smallest maze->dist from the surrounding maze->walls
 * that are not separated .row a wall
 */
unsigned char getMin(MAZE * maze, COORD coord) 
{
  unsigned char min = MAX_DIST;
  unsigned char distN;
  unsigned char distE;
  unsigned char distS;
  unsigned char distW;

  distN = hasNorth(maze->walls[coord.row][coord.col]) ? MAX_DIST : maze->dist[coord.row + 1][coord.col];
  distE = hasEast(maze->walls[coord.row][coord.col]) ? MAX_DIST : maze->dist[coord.row][coord.col + 1];
  distS = hasSouth(maze->walls[coord.row][coord.col]) ? MAX_DIST : maze->dist[coord.row - 1][coord.col];
  distW = hasWest(maze->walls[coord.row][coord.col]) ? MAX_DIST : maze->dist[coord.row][coord.col - 1];

  min = distN;
  min = (distE < min) ? distE : min;
  min = (distS < min) ? distS : min;
  min = (distW < min) ? distW : min;

  return min;
}

unsigned char getDir(MAZE * maze, COORD coord, MOUSE * mouse)
{
  unsigned char min = MAX_DIST;
  unsigned char distN;
  unsigned char distE;
  unsigned char distS;
  unsigned char distW;

  unsigned char orientation = mouse->orientation;

  distN = hasNorth(maze->walls[coord.row][coord.col]) ? MAX_DIST : maze->dist[coord.row + 1][coord.col];
  distE = hasEast(maze->walls[coord.row][coord.col]) ? MAX_DIST : maze->dist[coord.row][coord.col + 1];
  distS = hasSouth(maze->walls[coord.row][coord.col]) ? MAX_DIST : maze->dist[coord.row - 1][coord.col];
  distW = hasWest(maze->walls[coord.row][coord.col]) ? MAX_DIST : maze->dist[coord.row][coord.col - 1];
  // 1. Pick the shortest route
  if ( (distN < distE) && (distN < distS) && (distN < distW) )
    mouse->orientation = 'N';
  else if ( (distE < distN) && (distE < distS) && (distE < distW) )
    mouse->orientation = 'E';
  else if ( (distS < distE) && (distS < distN) && (distS < distW) )
    mouse->orientation = 'S';
  else if ( (distW < distE) && (distW < distS) && (distW < distN) )
    mouse->orientation = 'W';

  // 2. If multiple equally short routes, go straight if possible
  else if ( orientation == 'N' && !hasNorth(maze->walls[mouse->location.row][mouse->location.col]) )
    mouse->orientation = 'N';
  else if ( orientation == 'E' && !hasEast(maze->walls[mouse->location.row][mouse->location.col]) )
    mouse->orientation = 'E';
  else if ( orientation == 'S' && !hasSouth(maze->walls[mouse->location.row][mouse->location.col]) )
    mouse->orientation = 'S';
  else if ( orientation == 'W' && !hasWest(maze->walls[mouse->location.row][mouse->location.col]) )
    mouse->orientation = 'W';

  // 3. Otherwise prioritize N > E > S > W
  else if (!hasNorth(maze->walls[mouse->location.row][mouse->location.col]))
    mouse->orientation = 'N';
  else if (!hasEast(maze->walls[mouse->location.row][mouse->location.col]))
    mouse->orientation = 'E';
  else if (!hasSouth(maze->walls[mouse->location.row][mouse->location.col]))
    mouse->orientation = 'S';
  else if (!hasWest(maze->walls[mouse->location.row][mouse->location.col]))
    mouse->orientation = 'W';

  else
    printf("Stuck... Can't find center.\n");

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
