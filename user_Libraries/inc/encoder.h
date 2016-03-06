#ifndef ENCODER_H
#define ENCODER_H 

void Encoder_Configuration(void);
int32_t getLeftEncCount(void);
int32_t getRightEncCount(void);
void resetRightEncCount(void);
void resetLeftEncCount(void);
void setLeftEncCount(int val);
void setRightEncCount(int val);

#endif



