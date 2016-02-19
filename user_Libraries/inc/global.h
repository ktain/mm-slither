#ifndef GLOBAL_H
#define GLOBAL_H

#include "stdint.h"
#include "stdbool.h"

/* Global variables */

// Speed profile
extern int32_t leftBaseSpeed;
extern int32_t rightBaseSpeed;
extern int32_t maxPwm;

// PID
extern int32_t errorD;
extern int32_t errorP;
extern int32_t oldErrorP;
extern int32_t totalError;

// In-place turning
extern int turnProfile;
extern int tempPwm;
extern int timeAllotted;
extern float turnKP[4];
extern int turnTime[4];
extern int turnPwm[4];
extern int turnAngle[4];

// Sensor
extern int leftMiddleValue;
extern int rightMiddleValue;
extern int leftWallThreshold;
extern int rightWallThreshold;
extern int frontWallThresholdL;
extern int frontWallThresholdR;

extern bool hasLeftWall;
extern bool hasRightWall;
extern bool hasFrontWall;

// Gyro
extern int reflectionRate; // which is 1.000 (converted to ingeter)
extern int32_t volMeter;
extern int32_t voltage;
extern int32_t LFSensor;
extern int32_t RFSensor;
extern int32_t LDSensor;
extern int32_t RDSensor;
extern int32_t LSSensor;
extern int32_t RSSensor;
extern int32_t Outz;
extern int32_t aSpeed; //angular velocity
extern int32_t angle; 

// Systick flags
extern bool isMovingForward;
extern bool isTurning;
extern bool isAligning;
extern bool quarterCellFlag;
extern bool halfCellFlag;
extern bool fullCellFlag;
extern bool threeQuarterCellFlag;
extern bool stopAtFrontWallFlag;

// Systick events
extern int ticker;

// Current state
extern int currentState;

// Last state
extern int lastState;

// Statistics
extern int leftSpeed;
extern int rightSpeed;
extern int prevLeftEncCount;
extern int prevRightEncCount;

// Alignment
extern int alignLFVal;
extern int alignRFVal;
extern int alignTime;
extern int alignPwm;

#endif
