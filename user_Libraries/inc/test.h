#ifndef TEST_H
#define TEST_H

#include <stdbool.h>

void hugFrontWall(int LSensorVal, int RSensorVal);
void stopAtFrontWall(int);
void ledTest(void);
int wheelOffsetTest(int speed, int ontime);
void randomMovement(void);
void speedRun(void);
int getNextDirection(void);
void speedRunOld(void);
void closeUntracedCells(void);
bool hasFrontWallInMem(void);
#endif


