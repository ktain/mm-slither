#include "main.h"

/* Maze.h settings */
int block[SIZE][SIZE] = {0};  //  ... 0 0 0 0       0 0 0 0
                              //         DE TRACE   W S E N
                              // [row] [col]
                              // [ y ] [ x ]
int input[SIZE][SIZE] = {0};  // to read in custom maze
int distance[SIZE][SIZE] = {0};

int xPos = 0;   // 0-15
int yPos = 0;   // 0-15
int moveCount = 0;
int traceCount = 0;

/* Configure global variables */
int maxPwm;
int alignPwm;
int alignTime;
int turnDelay;
int times;

/* Configure search variables */
bool hasFrontWall = 0;
bool hasLeftWall = 0;
bool hasRightWall = 0;
int nextMove = 0;
char orientation = 'N';

/* Configure encoder settings */
int encResolution = 2048;				// counts/mrev
int gearRatio = 5;							// 5:1
float wheelCircumference = 70.5 ;	// mm
float wheelBase = 68;						// mm
int cellDistance = 25600;				// 12.5 * encResolution = 25600
int cellDistances[16];
float countspermm = 142;

/* Configure speed profile options */
bool useIRSensors = 0;
bool useGyro = 0;
bool usePID = 0;
bool useSpeedProfile = 0;
bool useOnlyGyroFeedback = 0;
bool useOnlyEncoderFeedback = 0;
int moveSpeed;			// speed is in cm/s, double of actual speed
int maxSpeed;			// call speed_to_counts(maxSpeed)
int turnSpeed;		
int searchSpeed;
int stopSpeed;


// Mouse state
bool isWaiting = 0;
bool isSearching = 0;
bool isSpeedRunning = 0;

// Sensor Thresholds
int frontWallThresholdL = 20;		// to detect presence of a wall
int frontWallThresholdR = 20;
int leftWallThreshold = 350;
int rightWallThreshold = 350;
int LDMiddleValue = 650;
int RDMiddleValue = 630;

int LFvalue1;		// for front wall alignment, when mouse is at the center
int RFvalue1;
int LFvalue2;
int RFvalue2;

// Pivot turn profile
int	turnLeft90;
int	turnRight90;
int	turnLeft180;
int	turnRight180;

int distances[100] = {0};

// Interface
int select = 0;

void systick(void) {
	
	// check voltage
	lowBatCheck();	// check if < 7.00V
	
	// Collect data
	if(useIRSensors) {
		readSensor();
	}
	
	// Run speed profile (with PID)
	if(useSpeedProfile) {
		speedProfile();
	}
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
	
	ALL_EM_OFF;
	ALL_LED_OFF;

	delay_ms(1000);
	shortBeep(200, 1000);	// ms, frequency
	
	isWaiting = 1;
	
	
	//Initial Speed Profile
	maxPwm = 800;
	alignPwm = 100;
	moveSpeed = 300*2;
	maxSpeed = 800*2;			
	turnSpeed = 40*2;
	searchSpeed = 70*2;
	stopSpeed = 10*2;
	alignTime = 100;
	turnDelay = 50;
	
	turnLeft90 = -76;
	turnRight90 = 76;
	turnLeft180 = -173;
	turnRight180 = 173;

	
	// Sensor Thresholds
	LFvalue1 = 1880;		// for front wall alignment, when mouse is at the center
	RFvalue1 = 1970;
	
	LFvalue2 = 500;
	RFvalue2 = 500;
	
	while(1) {		
		select = getLeftEncCount()/2048 % 4;
		if (select < 0) {
			select = -select;
		}
		switch(select) {
			case 0:
				LED1_ON;
				LED2_OFF;
				LED3_OFF;
				LED4_OFF;
				break;
			case 1:
				LED1_OFF;
				LED2_ON;
				LED3_OFF;
				LED4_OFF;
				break;
			case 2:
				LED1_OFF;
				LED2_OFF;
				LED3_ON;
				LED4_OFF;
				break;
			case 3:
				LED1_OFF;
				LED2_OFF;
				LED3_OFF;
				LED4_ON;
				break;
			default:
				;
		}

	}
}


void button0_interrupt(void) {
	shortBeep(200, 500);
	delay_ms(1000);
	printf("Button 0 pressed\n\r");
	/*
	while(1) {
		readSensor();
		ledTest();					// No delay
		printInfo();				
		delay_ms(2);
	}
	*/
	
	switch (select) {
		case 0:
			alignPwm = 100;
			moveSpeed = 100*2;
			maxSpeed = 400*2;			
			turnSpeed = 40*2;
			searchSpeed = 70*2;
			stopSpeed = 0*2;
			alignTime = 100;
			turnDelay = 50;
			sensorScale = 100;
			accX = 60;
			decX = 60;
		
			break;
		case 1:
			alignPwm = 100;
			moveSpeed = 200*2;
			maxSpeed = 400*2;			
			turnSpeed = 40*2;
			searchSpeed = 70*2;
			stopSpeed = 0*2;
			alignTime = 100;
			turnDelay = 50;
			sensorScale = 100;
			accX = 60;
			decX = 60;
		
			break;
		case 2:
			alignPwm = 100;
			moveSpeed = 300*2;
			maxSpeed = 400*2;			
			turnSpeed = 40*2;
			searchSpeed = 70*2;
			stopSpeed = 0*2;
			alignTime = 100;
			turnDelay = 50;
			sensorScale = 100;
			accX = 60;
			decX = 60;
		
			break;	
		case 3:
			alignPwm = 100;
			moveSpeed = 400*2;
			maxSpeed = 400*2;			
			turnSpeed = 40*2;
			searchSpeed = 70*2;
			stopSpeed = 0*2;
			alignTime = 100;
			turnDelay = 50;
			sensorScale = 100;
			accX = 60;
			decX = 60;
		
			break;			
		default:
			;
	}
	
	speedRun();
	printf("Finished Button 1 ISR\n\r");
}



void button1_interrupt(void) {
	shortBeep(200, 500);
	delay_ms(1000);	
	//printf("Button 1 pressed\n\r");

	initializeGrid();
	visualizeGrid();
	delay_ms(100);

	alignPwm = 100;
	moveSpeed = 400*2;
	maxSpeed = 500*2;			
	turnSpeed = 40*2;
	searchSpeed = 50*2;
	stopSpeed = 0*2;
	alignTime = 0;
	turnDelay = 100;
	sensorScale = 50;
	accX = 30;
	decX = 30;
	
	LDMiddleValue = 650;
	RDMiddleValue = 630;
	
	floodCenter();
	//printf("Finished Button 1 ISR\n\r");
}



void button2_interrupt(void) {
	shortBeep(200, 500);
	delay_ms(1000);
	printf("Button 2 pressed\n\r");
	
	initializeGrid();
	visualizeGrid();
	delay_ms(100);
	
	switch (select) {
		case 0:
			alignPwm = 100;
			moveSpeed = 400*2;
			maxSpeed = 500*2;			
			turnSpeed = 40*2;
			searchSpeed = 50*2;
			stopSpeed = 0*2;
			alignTime = 200;
			turnDelay = 100;
			sensorScale = 50;
			accX = 30;
			decX = 30;
			break;
		case 1:
			alignPwm = 100;
			moveSpeed = 400*2;
			maxSpeed = 500*2;			
			turnSpeed = 40*2;
			searchSpeed = 50*2;
			stopSpeed = 0*2;
			alignTime = 150;
			turnDelay = 80;
			sensorScale = 50;
			accX = 40;
			decX = 40;		
			break;
		case 2:
			alignPwm = 100;
			moveSpeed = 400*2;
			maxSpeed = 500*2;			
			turnSpeed = 40*2;
			searchSpeed = 60*2;
			stopSpeed = 0*2;
			alignTime = 100;
			turnDelay = 50;
			sensorScale = 50;
			accX = 50;
			decX = 50;
			break;
		case 3:
			alignPwm = 100;
			moveSpeed = 400*2;
			maxSpeed = 500*2;			
			turnSpeed = 40*2;
			searchSpeed = 70*2;
			stopSpeed = 0*2;
			alignTime = 100;
			turnDelay = 50;
			sensorScale = 50;
			accX = 60;
			decX = 60;
			break;
		default:
			;
	}
	
	floodCenter();
	printf("Finished Button 2 ISR\n\r");
}



void button3_interrupt(void) {
	/*
	shortBeep(200, 500);
	delay_ms(1000);
	printf("Button 3 pressed\n\r");

	
	resetLeftEncCount();
	resetRightEncCount();
	randomMovement();
	
	printf("Finished Button 3 ISR\n\r");
	*/
}



void printInfo(void) {
	printf("LF %4d|LD %4d|LS %4d|RS %4d|RD %4d|RF %4d|LENC %9d|RENC %9d|voltage %4d|angle %4d|curSpeedX %4f|distanceLeft %d|encCount %d|sensorFeedback %d\r\n",
					LFSensor, LDSensor, LSSensor, RSSensor, RDSensor, RFSensor, getLeftEncCount(), getRightEncCount(), voltage, angle, curSpeedX, distanceLeft, encCount, sensorError);
}

