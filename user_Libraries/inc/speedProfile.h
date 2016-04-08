#ifndef speed_Profile_H
#define speed_Profile_H

void speedProfile(void);

void getEncoderStatus(void);
void updateCurrentSpeed(void);
void calculateMotorPwm(void);

int needToDecelerate(int32_t dist, int16_t curSpd, int16_t endSpd);
void resetSpeedProfile(void);
float dist_counts_to_mm(float counts);
float dist_mm_to_counts(float mm);
float counts_to_mm(int counts);
float mm_to_counts (float speed);
void getSensorError(void);
float abs(float number);
float getDecNeeded(float Vi, float Vf, float d);

void moveForward(int cells);

#endif
