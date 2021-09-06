/*
 * EXT_Interrupt.c
 *
 *  Created on: Sep 5, 2021
 *      Author: Ahmed Azazy
 */

#include <stm32f446xx.h>
#include "usart.h"
#include "EXT_Interrupt.h"

uint8_t Enter_BootLoader = 0;

void EXT_INT13_Init(edgeSelection_t edge)
{
	RCC->AHB1ENR|=(1<<2);
	RCC->APB2ENR|=(1<<14);

	SYSCFG->EXTICR[3]|=(2<<4);


	EXTI->IMR |=(1<<13);

	if(edge == FALLING_EDGE)
	EXTI->FTSR |= (1<<13);

	else if(edge == RISING_EDGE)
		EXTI->RTSR |= (1<<13);

	else if(edge == BOTH)
	{
		EXTI->RTSR |= (1<<13);
		EXTI->FTSR |= (1<<13);
	}

	NVIC_EnableIRQ(EXTI15_10_IRQn);
}



void EXTI15_10_IRQHandler (void)
{
	EXTI->PR |=(1<<13);
	EXTI->IMR &=~(1<<13);
	Enter_BootLoader = 1;
}
