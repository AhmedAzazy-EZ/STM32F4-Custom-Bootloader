/*
 * uart.c
 *
 *  Created on: Sep 5, 2021
 *      Author: Ahmed Azazy
 */


#include "main.h"
#include <stm32F446xx.h>

char USART1_Buffer[100];
uint8_t USART1_Counter = 0;

void usart_Init(void )
{
	//Enable PORT A Clock
	RCC->AHB1ENR|=(1<<0);

	GPIOA->MODER&=~(3<<18);
	GPIOA->MODER&=~(3<<20);
	//PA9 Tx
	GPIOA->MODER|=(2<<18);
	//PA10 Rx
	GPIOA->MODER|=(2<<20);

	GPIOA->AFR[1] |=(7<<4);
	GPIOA->AFR[1] |=(7<<8);


	//Enable USART1 Clock
	RCC->APB2ENR|=(1<<4);
	//Enable Receiver Interrupt
	//USART1->CR1|=(1<<5);
	//Enable Transmitter Interrupt
	//USART1->CR|=(1<<7);
	//Enable Transmitter Reciever
	USART1->CR1|=(3<<2);

	//Set Fraction
	USART1->BRR= 3;
	//Set Mantissa
	USART1->BRR |= (104<<4);


	NVIC_EnableIRQ(USART1_IRQn);
	//Enable USART
	USART1->CR1|=(1<<13);
}



void USART1_SendString(char *str)
{

	for(uint32_t i = 0 ;str[i]; i++)
	{
		USART1->DR = str[i];
		while(!((USART1->SR) & (1<<7))){}
	}

}


void USART1_IRQHandler(void )
{
	//Rx Register NOT EMPTY
	if((USART1->SR & (1<<5)))
	{
		USART1_Buffer[USART1_Counter++] = USART1->DR;
	}

	//TX Register Empty
	if(USART1->SR & (1<<7))
	{
		//Fill it
	}
}



void USART1_Receive(uint8_t length , char * buffer)
{

	for(uint8_t i = 0 ; i < length ; i++)
	{
		while(!(USART1->SR & (1<<5)));

		buffer[i] = USART1->DR;
	}

}
