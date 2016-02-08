#ifndef SPI_H
#define SPI_H

#define RS_1	GPIO_SetBits(GPIOB, GPIO_Pin_10)   //select control register
#define RS_0	GPIO_ResetBits(GPIOB, GPIO_Pin_10) //select data register
#define CE_1	GPIO_SetBits(GPIOC, GPIO_Pin_13)   //disable data writing, latch on
#define CE_0	GPIO_ResetBits(GPIOC, GPIO_Pin_13) //enable date writing



void SPI_Configuration(void);
void WriteControlRegister(char c);








#endif
