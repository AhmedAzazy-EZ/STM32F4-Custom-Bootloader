/*
 * timer.c
 *
 *  Created on: Sep 6, 2021
 *      Author: Ahmed Azazy
 */


#include <stm32f446xx.h>
#include "timer.h"
#include "usart.h"
#include "Boot.h"

uint8_t flag = 0;

void TIM9_Init(void )
{
	//Enable TIM9 clock
	RCC->APB2ENR|=(1<<16);

	//Only Counter overflow generates update Interrupt
	TIM9->CR1|=(1<<2);

	//One Pulse Mode
	//TIM9->CR1 |= (1<<3);

	//Enable Update Interrupt
	TIM9->DIER|=(1<<0);

	//Generating 30 seconds timeout
	TIM9->CNT = 0;

	TIM9->PSC = 7328;

	TIM9->ARR = 65500;


	NVIC_EnableIRQ(TIM1_BRK_TIM9_IRQn );

	TIM9->CR1|=(1<<0);
}



void TIM1_BRK_TIM9_IRQHandler(void )
{
	TIM9->SR &=~(1<<0);
	if(flag == 0)
	{
		flag=1;
		return ;
	}


	//Do whatever the hell you want to do
	JumpToUserCode();
}


