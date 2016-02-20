#include "main.h"

/* Global variables */
// PID
int32_t errorD = 0;
int32_t errorP = 0;
int32_t oldErrorP = 0;
int32_t totalError = 0;

// In-place turning
int turnProfile = 0;
int tempPwm;
int timeAllotted = 0;
float turnKP[4] = {0};
int turnTime[4] = {0};
int turnPwm[4] = {0};
int turnAngle[4] = {0};

// Motor
int32_t leftBaseSpeed = 100;
int32_t rightBaseSpeed  = 100;
int32_t maxPwm = 300;
int32_t forwardPwm = 100;

// Sensor
int leftMiddleValue = 600;
int rightMiddleValue = 600;
int leftWallThreshold = 300;
int rightWallThreshold = 300;
int frontWallThresholdL = 30;
int frontWallThresholdR = 30;
bool hasLeftWall = 0;
bool hasRightWall = 0;
bool hasFrontWall = 0;

// Systick flags
bool isMovingForward = 0;
bool isTurning = 0;
bool isAligning = 0;
bool quarterCellFlag = 0;
bool halfCellFlag = 0;
bool fullCellFlag = 0;
bool threeQuarterCellFlag = 0;
bool stopAtFrontWallFlag = 0;

// Systick events
int ticker = 0;

// States
int currentState = 1;
int lastState = 1;

// Statistics
int leftSpeed = 0;
int rightSpeed = 0;
int prevLeftEncCount = 0;
int prevRightEncCount = 0;

// Alignment
int alignLFVal = 1500;
int alignRFVal = 1500;
int alignTime = 800;
int alignPwm = 50;

void systick(void) {
	
	lowBatCheck();	// stall when < 7.00V
	readGyro();

	
	// Moving forward state
	if (isMovingForward) {
		readSensor();
		pid();
		
		maxPwm = forwardPwm;
		
		// Move forward
		if (quarterCellFlag && hasFrontWall) {	// Align when approaching front wall
			timeAllotted = alignTime;
			maxPwm = alignPwm;
			isAligning = 1;
			isMovingForward = 0;
			
		}
		else if (turnProfile){ // slow down when about to turn
			setLeftPwm(50 - totalError);
			setRightPwm(50 + totalError);
		}
		else {	// Otherwise go at normal speed
			setLeftPwm(leftBaseSpeed - totalError);
			setRightPwm(rightBaseSpeed + totalError);
		}
	}
	
		
	// Quarter cell, run once
	if(!quarterCellFlag && (getLeftEncCount() + getRightEncCount() > CELL_DISTANCE/2)){
		quarterCellFlag = 1;
		
		// Update position...
		
		// Read walls
		if(LDSensor > leftWallThreshold)
			hasLeftWall = 1;
		if(RDSensor > rightWallThreshold)
			hasRightWall = 1;
		
	}
		
	// Half cell, run once
	if(!halfCellFlag && (getLeftEncCount() + getRightEncCount() > CELL_DISTANCE)){
		halfCellFlag = 1;
		
		if ( (LFSensor > frontWallThresholdL) && (RFSensor > frontWallThresholdR) )
			hasFrontWall = 1;
		// if LF or RF only, error
		
		// Random search sets turn profile
		if (!hasRightWall)
			turnProfile = 2;	// Turn right 90
		else if (!hasLeftWall)
			turnProfile = 1;	// Turn left 90
		else if (!hasFrontWall)
			turnProfile = 0;	// Move forward
		else
			turnProfile = 3;
			
	}
		
	// Three quarter cell, stay on
	if((getLeftEncCount() + getRightEncCount() > CELL_DISTANCE*1.5)){
		threeQuarterCellFlag = 1;
		
	}
	
	// Full cell
	if (getLeftEncCount() + getRightEncCount() >= CELL_DISTANCE*2){
		shortBeep(200, 4000);
		
		// Align with front wall
		if (hasFrontWall) {
			timeAllotted = alignTime;
			tempPwm = maxPwm;
			maxPwm = alignPwm;
			isAligning = 1;
			isMovingForward = 0;
		}
		
		// Perform turn
		else if (turnProfile) {
			
			// Reset angle
			angle = 0;
			
			// Perform turn
			timeAllotted = turnTime[turnProfile];
			maxPwm = turnPwm[turnProfile];
			isAligning = 0;
			isMovingForward = 0;
			isTurning = 1;
		}
		
		// Reset encoder counts
		resetLeftEncCount();
		resetRightEncCount();
		
		// Reset Error
		resetError();
		
		// Reset Flags
		hasLeftWall = 0;
		hasRightWall = 0;
		hasFrontWall = 0;
		quarterCellFlag = 0;
		halfCellFlag = 0;
		threeQuarterCellFlag = 0;
		fullCellFlag = 0;
		
	}
	
	// Alignment
	else if (isAligning) {
		readSensor();
		if (timeAllotted > 0) {
			timeAllotted--;
			alignFrontWall();
		}	
		else {
			// Perform turn
			timeAllotted = turnTime[turnProfile];
			maxPwm = turnPwm[turnProfile];
			isAligning = 0;
			isMovingForward = 0;
			isTurning = 1;
		}
	}
	
	// Turning state
	else if (isTurning) {		// Finish turn
		
		if (timeAllotted > 0) {
			timeAllotted--;
			turn(turnProfile);
		}	
		else {	
			// Reset encoder count
			resetLeftEncCount();
			resetRightEncCount();
			
			// Reset error
			resetError();
			
			// Reset Flags
			hasLeftWall = 0;
			hasRightWall = 0;
			hasFrontWall = 0;
			quarterCellFlag = 0;
			halfCellFlag = 0;
			threeQuarterCellFlag = 0;
			fullCellFlag = 0;
			isTurning = 0;
			turnProfile = 0;
			
			// Revert back to moving forward state
			angle = 0;
			isMovingForward = 1;
		}
	}
	
}



void button0_interrupt(void) {
	shortBeep(200, 300);
	delay_ms(1000);
	
	angle = 0;
	while(1) {
		readSensor();
		ledTest();					// No delay
		printInfo();				// No delay
		delay_ms(10);
	}
}



void button1_interrupt(void) {
	shortBeep(200, 400);
	delay_ms(1000);	
	
	resetLeftEncCount();
	resetRightEncCount();
	
	angle = 0;
	
	delay_ms(1000);
	isMovingForward = 1;
}



void button2_interrupt(void) {
	shortBeep(200, 500);
	delay_ms(1000);

}



void button3_interrupt(void) {
	shortBeep(200, 600);
	delay_ms(1000);

}



void printInfo(void) {
	printf("LF %4d|LD %4d|LS %4d|RS %4d|RD %4d|RF %4d|LENC %9d|RENC %9d|voltage %4d|angle %4d|currentState %d|leftSpeed %d\r\n",
					LFSensor, LDSensor, LSSensor, RSSensor, RDSensor, RFSensor, getLeftEncCount(), getRightEncCount(), voltage, angle, currentState, leftSpeed);
}


int main(void) {
	
	Systick_Configuration();
	LED_Configuration();
	button_Configuration();
	usart1_Configuration(9600);
  TIM4_PWM_Init();
	Encoder_Configuration();
	buzzer_Configuration();
	ADC_Config();

	ALL_LED_OFF;
	ALL_EM_OFF;
	
	delay_ms(1000);
	shortBeep(200, 400);	// ms, frequency
	
	// Turn left 90
	turnKP[1] = 0.1;
  turnTime[1] = 500;
  turnPwm[1] = 200;
  turnAngle[1] = -9200;	// left turn is negative
	
	// Turn right 90
	turnKP[2] = 0.1;
  turnTime[2] = 500;
  turnPwm[2] = 200;
  turnAngle[2] = 9400;	// right turn is positive 

	// Turn back 180
	turnKP[3] = 0.1;
  turnTime[3] = 1000;
  turnPwm[3] = 200;
  turnAngle[3] = -19000; // left turn is more reliable
	
	
	while(1) {		
		printInfo();
		delay_ms(100);
	}
}
