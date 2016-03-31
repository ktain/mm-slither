#ifndef GLOBAL_H
#define GLOBAL_H

#include "stdint.h"
#include "stdbool.h"

/* Global variables */

// Maze data

// Mouse state
extern bool isWaiting;
extern bool isSearching;
extern bool isSpeedRunning;

// Mechanical settings
extern int encResolution;
extern int gearRatio;
extern float wheelCircumference;
extern float wheelBase;
extern int cellDistance;

// Speed settings
extern int leftBaseSpeed;
extern int rightBaseSpeed;
extern int maxPwm;
extern int alignPwm;
extern int alignTime;
extern int turnDelay;

// Speed profile options
extern bool useIRSensors;
extern bool useGyro;
extern bool usePID;
extern bool useSpeedProfile;
extern bool useOnlyGyroFeedback;
extern bool useOnlyEncoderFeedback;

// Speed profile
extern float curSpeedX;
extern float curSpeedW;
extern int targetSpeedX;
extern int targetSpeedW;
extern int encoderFeedbackX;
extern int encoderFeedbackW;
extern float pidInputX;
extern float pidInputW;
extern float posErrorX;
extern float posErrorW;
extern float oldPosErrorX;
extern float oldPosErrorW;
extern int posPwmX;
extern int posPwmW;
extern float kpX, kdX;
extern float kpW, kdW;	//used in straight
extern float kpW1;			//used for T1 and T3 in curve turn
extern float kdW1;
extern float kpW2;			//used for T2 in curve turn
extern float kdW2;
extern float accX;			//6m/s/s  
extern float decX; 
extern float accW; 		//cm/s^2
extern float decW;

extern int moveSpeed;			//speed is in cm/s
extern int maxSpeed;			//call speed_to_counts(maxSpeed);
extern int turnSpeed;
extern int searchSpeed;
extern int stopSpeed;

extern int leftEncCount;
extern int rightEncCount;
extern int leftEncOld;
extern int rightEncOld;
extern int leftEncChange;
extern int rightEncChange;
extern int encChange;
extern int distanceLeft;
extern int sensorError;
extern int sensorScale;
extern int encCount;
extern int oldEncCount;

extern int LFvalue1;
extern int RFvalue1;
extern int LFvalue2;
extern int RFvalue2;

// PID
extern int32_t errorD;
extern int32_t errorP;
extern int32_t oldErrorP;
extern int32_t totalError;

// Sensor
extern int LDMiddleValue;
extern int RDMiddleValue;
extern int leftWallThreshold;
extern int rightWallThreshold;
extern int frontWallThresholdL;
extern int frontWallThresholdR;

// Gyro
extern int reflectionRate;
extern int32_t volMeter;
extern int32_t voltage;
extern int32_t LFSensor;
extern int32_t RFSensor;
extern int32_t LDSensor;
extern int32_t RDSensor;
extern int32_t LSSensor;
extern int32_t RSSensor;
extern int32_t Outz;
extern int32_t aSpeed;
extern int32_t angle; 

// Pivot turn profile;
extern int turnLeft90;
extern int turnRight90;
extern int turnLeft180;
extern int turnRight180;

extern int distances[100];

#endif
