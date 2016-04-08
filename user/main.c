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
int frontWallThresholdL = 30;		// to detect presence of a wall
int frontWallThresholdR = 30;
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

	// Speed profile 12v 512 1
	/*
	maxPwm = 900;
	alignPwm = 300;
	moveSpeed = 60*2;			// speed is in cm/s, double of actual speed
	maxSpeed = 100*2;			// call speed_to_counts(maxSpeed)
	turnSpeed = 40*2;		
	searchSpeed = 60*2;
	stopSpeed = 30*2;
	
	turnLeft90 = -70;
	turnRight90 = 70;
	turnLeft180 = -160;
	turnRight180 = 160;
	*/
	
	/*
	// Speed/turn/sensor profile 12v 512 2
	maxPwm = 800;
	alignPwm = 100;
	moveSpeed = 200*2;			// speed is in cm/s, double of actual speed
	maxSpeed = 500*2;			// call speed_to_counts(maxSpeed)
	turnSpeed = 40*2;
	searchSpeed = 60*2;
	stopSpeed = 20*2;
	alignTime = 200;
	turnDelay = 100;
	
	turnLeft90 = -70;
	turnRight90 = 70;
	turnLeft180 = -158;
	turnRight180 = 158;
	*/
	
	maxPwm = 800;
	alignPwm = 100;
	moveSpeed = 100*2;			// speed is in mm/ms, double of actual speed
	maxSpeed = 200*2;			
	turnSpeed = 40*2;
	searchSpeed = 60*2;
	stopSpeed = 10*2;
	alignTime = 200;
	turnDelay = 500;
	
	turnLeft90 = -71;
	turnRight90 = 71;
	turnLeft180 = -170;
	turnRight180 = 170;

	
	// Sensor Thresholds
	LFvalue1 = 1880;		// for front wall alignment, when mouse is at the center
	RFvalue1 = 1970;
	
	LFvalue2 = 500;
	RFvalue2 = 500;
	
	while(1) {		
		if (isWaiting) {
			int mode = getLeftEncCount()/2048 % 4;
			if (mode < 0)
				mode = -mode;
			switch(mode) {
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
			delay_ms(10);
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
	speedRun();
	
	/*
	moveForward(1);
	delay_ms(1000);
	resetSpeedProfile();
	isSpeedRunning = 1;
	useSpeedProfile = 1;
	pivotTurn(turnRight90);
	delay_ms(1000);
	moveForward(1);
	*/
}



void button1_interrupt(void) {
	shortBeep(200, 500);
	delay_ms(1000);	
	printf("Button 1 pressed\n\r");

	/*
	while(1) {
		readSensor();
		ledTest();					// No delay
		printInfo();				
		delay_ms(2);
	}
	*/
	
	
	moveForward(3);
	moveW();
	moveS();
	moveForward(3);
	moveE();
	moveN();

}



void button2_interrupt(void) {
	shortBeep(200, 500);
	delay_ms(1000);
	printf("Button 2 pressed\n\r");
	
	initializeGrid();
	printGrid();
	delay_ms(100);
	floodCenter();
}



void button3_interrupt(void) {
	shortBeep(200, 500);
	delay_ms(1000);
	printf("Button 3 pressed\n\r");

	/*
	int totalTime = millis();
	times++;
	moveForward(times);
	totalTime = totalTime - millis();
	printf("totalTime = %d", totalTime);
	*/
	
	
	while(1) {
		readSensor();
		ledTest();					// No delay
		printInfo();				
		delay_ms(2);
	}
	


	/*
	resetLeftEncCount();
	resetRightEncCount();
	randomMovement();
	*/
}



void printInfo(void) {
	printf("LF %4d|LD %4d|LS %4d|RS %4d|RD %4d|RF %4d|LENC %9d|RENC %9d|voltage %4d|angle %4d|curSpeedX %4f|distanceLeft %d|encCount %d|sensorFeedback %d\r\n",
					LFSensor, LDSensor, LSSensor, RSSensor, RDSensor, RFSensor, getLeftEncCount(), getRightEncCount(), voltage, angle, curSpeedX, distanceLeft, encCount, sensorError);
}

