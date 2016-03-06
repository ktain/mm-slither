#include "main.h"

/* Configure global variables */
int maxPwm = 400;
int alignPwm = 100;


/* Configure encoder settings */
int encResolution = 1024;				// counts/mrev
int gearRatio = 5;							// 5:1
float wheelCircumference = 70.5 ;	// mm
float wheelBase = 68;						// mm
int cellDistance = 12800;				// 12.5 * encResolution


/* Configure speed profile options */
bool useIRSensors = 0;
bool useGyro = 0;
bool usePID = 0;
bool useSpeedProfile = 0;
bool useOnlyGyroFeedback = 0;
bool useOnlyEncoderFeedback = 0;
int moveSpeed = 30*2;			// speed is in cm/s, double of actual speed
int maxSpeed = 100*2;			// call speed_to_counts(maxSpeed)
int turnSpeed = 10*2;		
int searchSpeed = 40*2;
int stopSpeed = 10*2;

// Mouse state
bool isWaiting = 0;
bool isSearching = 0;
bool isSpeedRunning = 0;

// Sensor Thresholds
int frontWallThresholdL = 30;
int frontWallThresholdR = 30;
int leftWallThreshold = 300;
int rightWallThreshold = 300;
int LDMiddleValue = 850;
int RDMiddleValue = 850;


// Sensor Thresholds
int LFvalue1 = 1000;
int RFvalue1 = 1000;
int LFvalue2 = 500;
int RFvalue2 = 500;

// Pivot turn profile
int	turnLeft90;
int	turnRight90;
int	turnLeft180;
int	turnRight180;


void systick(void) {
	
	// If not moving, check voltage
	if (isWaiting)
		lowBatCheck();	// stall if < 7.00V
	
	else if (isSearching) {
		// Collect data
		if(useIRSensors)
			readSensor();
		
		if(useGyro)
			readGyro();
		
		// Run speed profile (with PID)
		if(useSpeedProfile) {
			speedProfile();
		}
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
	
	/*
	// Speed profile 2
	maxPwm = 400;
	alignPwm = 80;
	moveSpeed = 30*2;			// speed is in cm/s, double of actual speed
	maxSpeed = 100*2;			// call speed_to_counts(maxSpeed)
	turnSpeed = 20*2;		
	searchSpeed = 40*2;
	stopSpeed = 5*2;

	turnLeft90 = -81;
	turnRight90 = 82;
	turnLeft180 = -170;
	turnRight180 = 170;
	*/
	
	/* Speed profile 3
	maxPwm = 400;
	alignPwm = 80;
	moveSpeed = 30*2;			// speed is in cm/s, double of actual speed
	maxSpeed = 100*2;			// call speed_to_counts(maxSpeed)
	turnSpeed = 30*2;		
	searchSpeed = 40*2;
	stopSpeed = 5*2;
	
	turnLeft90 = -70;
	turnRight90 = 70;
	turnLeft180 = -160;
	turnRight180 = 160;
	*/
	
	// Speed profile 4
	maxPwm = 400;
	alignPwm = 80;
	moveSpeed = 30*2;			// speed is in cm/s, double of actual speed
	maxSpeed = 100*2;			// call speed_to_counts(maxSpeed)
	turnSpeed = 40*2;		
	searchSpeed = 40*2;
	stopSpeed = 5*2;
	
	turnLeft90 = -53;
	turnRight90 = 52;
	turnLeft180 = -145;
	turnRight180 = 145;
	
	while(1) {		
		delay_ms(10);
	}
}


void button0_interrupt(void) {
	shortBeep(200, 500);
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
	shortBeep(200, 500);
	delay_ms(1000);	
	
	isWaiting = 0;
	isSearching = 1;
	useSpeedProfile = 1;
	
	while(1) {
		moveForward(1);
		delay_ms(250);
		pivotTurn(turnRight90);
		delay_ms(250);
		moveForward(1);
		delay_ms(250);
		pivotTurn(turnRight90);
		delay_ms(250);
		moveForward(1);
		delay_ms(250);
		pivotTurn(turnRight90);
		delay_ms(250);
		moveForward(1);
		delay_ms(250);
		pivotTurn(turnRight90);
		delay_ms(250);
	}
}



void button2_interrupt(void) {
	shortBeep(200, 500);
	delay_ms(1000);
	
	alignFrontWall(1360, 1360); // left, right
}



void button3_interrupt(void) {
	shortBeep(200, 500);
	delay_ms(1000);
	
	resetLeftEncCount();
	resetRightEncCount();
	randomSearch();
}



void printInfo(void) {
	printf("LF %4d|LD %4d|LS %4d|RS %4d|RD %4d|RF %4d|LENC %9d|RENC %9d|voltage %4d|angle %4d|curSpeedX %4f|distanceLeft %d|encCount %d|sensorFeedback %d\r\n",
					LFSensor, LDSensor, LSSensor, RSSensor, RDSensor, RFSensor, getLeftEncCount(), getRightEncCount(), voltage, angle, curSpeedX, distanceLeft, encCount, sensorError);
}

