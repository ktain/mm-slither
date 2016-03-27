#ifndef MAZE_H
#define MAZE_H

#include "stdbool.h"

/*
 * Filename: maze.h
 * Description: Header file containing function declarations,
 *              constants, and global variables
 */
 
/* Debug option */
#define DEBUG 1

/* Graphics */
#define HIDESOUTH 1   // Hide south cell wall when printing
#define HIDEEAST 1    // Hide east cell wall when printing

//Wall info 
#define NWALL 1
#define EWALL 2
#define SWALL 4
#define WWALL 8
#define ALLWALLS 15
#define TRACE 16
#define DE 32
#define VISITED 64

/* Constants */
#define SIZE 16       // size of maze
#define STACKSIZE 512 //max stacksize (512*2 bytes)
#define MAX_DIST 500  // max distance for flood search

/* struct Maze holds all MM maze information
 * - input: if running a virtual mouse, holds maze with all wall info
 * - walls: holds all wall information known to the mouse at a specific time
 * - dist: holds distances to the goal coordinate/tile from each tile in maze
 */
struct Maze {
	//Input maze (all walls known)
	unsigned char input [SIZE][SIZE];
	//Holds all walls currently known to mouse 
	unsigned char walls [SIZE][SIZE];
	//Distance of each tile to goal
	unsigned char dist [SIZE][SIZE];
};

//Struct that holds the coordinates of a tile
struct Coord {
	unsigned char row;
	unsigned char col;
};

//Stack data structure implementation
struct Stack {
	struct Coord stack[STACKSIZE];
	int top; //index of last item pushed on stack (if -1, stack is empty)
};

/* Mouse - representation of all data for the mouse 
 * - orientation: direction Mouse is facing (N W E S or D for done) 
 * - location: coordinates of the Mouse
 * - traceCount: # of tiles traversed 
 */
struct Mouse {
	unsigned char orientation;
	struct Coord location;
	unsigned char traceCount;
  int moveCount;
};

typedef struct Maze MAZE;
typedef struct Coord COORD;
typedef struct Stack STACK; 
typedef struct Mouse MOUSE;

/* Function declarations */

//Floodfill functions -- floodfill.c
//Returns a direction to go in
unsigned char floodfill(MAZE * m, MOUSE * mouse);
unsigned char getMin(MAZE * maze, COORD coord);
unsigned char getDir(MAZE * maze, COORD coord, MOUSE * mouse);
unsigned char updateDist(MAZE * maze, COORD coord, unsigned char detectedWalls);

//Lookup functions 
unsigned char hasNorth(unsigned char c);
unsigned char hasEast(unsigned char c);
unsigned char hasSouth(unsigned char c);
unsigned char hasWest(unsigned char c);
unsigned char hasTrace(unsigned char c);
unsigned char isDeadEnd(unsigned char c);
bool willTurn(void);

//Main startup functions -- maze.c 
void initMaze(MAZE * maze);
void initDist(MAZE * maze);
void printMaze(MAZE * maze);
void readMaze(MAZE * m);

//Movement Functions 
void moveN(void);
void moveE(void);
void moveS(void);
void moveW(void);
void moveBack(char prevRelLoc);

//Stack functions -- stack.c
int push(STACK * s, const COORD c);
int pop(STACK * s);
int empty(STACK * s);
int full(STACK * s);
COORD top(STACK * s);

//Graphic functions -- graphics.c
void printGrid(const MAZE * maze);
void visualizeGrid(const MAZE * maze, const MOUSE mouse);

//Virtual mouse functions -- virtualMouse.c
void mouser(MAZE * maze, const COORD goal, MOUSE * mouse);
void detectWalls(MAZE * maze, const MOUSE mouse);

extern int nextMove;
extern bool hasFrontWall;
extern bool hasLeftWall;
extern bool hasRightWall;

#endif
