#ifndef SENSOR_FUNCTION_H
#define SENSOR_FUNCTION_H 

#include <stm32f4xx.h>

void readSensor(void);
void readSideSensors(void);
void readGyro(void);
void readVolMeter(void);
void lowBatCheck(void);

#endif
