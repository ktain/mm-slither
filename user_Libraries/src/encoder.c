#include "stm32f4xx.h"
#include "encoder.h"

//PA0	TIM5_CH1	Encoder_R_CHA
//PA1	TIM5_CH2	Encoder_R_CHB

//PA15	TIM2_CH1	Encoder_L_CHA
//PB3	TIM2_CH2	Encoder_L_CHB


void Encoder_Configuration(void)
{
	
GPIO_InitTypeDef GPIO_InitStructure;
RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5|RCC_APB1Periph_TIM2, ENABLE);

GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_15;
GPIO_Init(GPIOA, &GPIO_InitStructure);
GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
GPIO_Init(GPIOB, &GPIO_InitStructure);

GPIO_PinAFConfig(GPIOA, GPIO_PinSource0, GPIO_AF_TIM5);
GPIO_PinAFConfig(GPIOA, GPIO_PinSource1, GPIO_AF_TIM5);
GPIO_PinAFConfig(GPIOA, GPIO_PinSource15, GPIO_AF_TIM2);
GPIO_PinAFConfig(GPIOB, GPIO_PinSource3, GPIO_AF_TIM2);

TIM_SetAutoreload (TIM5, 0xffffffff);
TIM_SetAutoreload (TIM2, 0xffffffff);
/* Configure the timer */
	// right, left
TIM_EncoderInterfaceConfig(TIM5, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Falling);
TIM_EncoderInterfaceConfig(TIM2, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);
/* TIM2/5 counter enable */
TIM_Cmd(TIM5, ENABLE);
TIM_Cmd(TIM2, ENABLE);
	
}

int32_t getRightEncCount(void) {
	//return TIM_GetCounter(TIM5);
	return TIM5->CNT;
}

int32_t getLeftEncCount(void) {
	//return TIM_GetCounter(TIM2);
	return TIM2->CNT;
}

void resetRightEncCount(void) {
	TIM5->CNT = 0;
}

void resetLeftEncCount(void) {
	TIM2->CNT = 0;
}

void setRightEncCount(int val) {
	TIM5->CNT = val;
}

void setLeftEncCount(int val) {
	TIM2->CNT = val;
}
