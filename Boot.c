/*
 * Boot.c
 *
 *  Created on: Sep 6, 2021
 *      Author: Ahmed Azazy
 */

#include "Boot.h"
#include <stdint.h>
#include "usart.h"
#include <stm32f446xx.h>

void DecodeAddressIntoHex(uint32_t * address, char * buffer)
{
	*address = 0;
	for(uint8_t i = 0 ; i < 8 ; i ++)
	{

		if(buffer[i] >= 0x30 && buffer[i] <= 0x39 )
		{
			buffer[i] -= 0x30;
		}

		else if(buffer[i] >= 0x41 && buffer[i] <= 0x46)
		{
			buffer[i]-=0x37;
		}

		*address|= (buffer[i] << (32-(4*(i+1))));
	}
}



void Config_flash(void)
	{

		FLASH->KEYR=0x45670123;		//unlock sequence step 1
		FLASH->KEYR=0xCDEF89AB;		//unlock sequence step 2
		FLASH->CR&=~(3<<8);   //8 bit access
		FLASH->CR|=(1<<0);    //enable PG
	}


void BootHelp(void )
{
	  USART1_SendString("Inside BootLoader\r\n");

	  USART1_SendString("----------------------\r\n");

	  USART1_SendString("Boot Loader Version -> 0x51 ->Q\r\n");

	  USART1_SendString("Burn an Image -> 0x52 ->R\r\n");

	  USART1_SendString("Jump to User Code -> 0x53 ->S\r\n");

	  USART1_SendString("Erase Sector -> 0x54 ->T\r\n");

	  USART1_SendString("BootLoader Helper -> 0x55 ->U\r\n");
}


void JumpToUserCode(void)
{
	    /****USART RESER*****/
	 	RCC->AHB1ENR&=~(1<<0);
		GPIOA->MODER&=~(3<<18);
		GPIOA->MODER&=~(3<<20);

		GPIOA->AFR[1] &=~(15<<4);
		GPIOA->AFR[1] &=~(15<<8);
		RCC->APB2ENR&=~(1<<4);

		USART1->CR1 = 0;
		USART1->BRR = 0;
		NVIC_DisableIRQ(USART1_IRQn);


		/******EXTI13 Reset******/
		RCC->AHB1ENR&=~(1<<2);
		RCC->APB2ENR&=~(1<<14);
		SYSCFG->EXTICR[3]&=~(15<<4);
		EXTI->IMR &=~(1<<13);
		EXTI->FTSR &=~ (1<<13);
		EXTI->RTSR &= ~(1<<13);
		NVIC_DisableIRQ(EXTI15_10_IRQn);

		/******TIM9 Reset********/
		RCC->APB2ENR&=~(1<<16);
		TIM9->CR1= 0;
		TIM9->DIER=0;
		TIM9->CNT = 0;
		TIM9->PSC = 0;
		TIM9->ARR = 0;
		NVIC_DisableIRQ(TIM1_BRK_TIM9_IRQn);


		/******FLASH Reset*******/
		//Lock Flash Control Register
		FLASH->CR|=(1<<31);


	  //User Application starts at sector2
	  funPtr User_ResetHandler = *((funPtr *)0x08008004);

	  User_ResetHandler();
}


uint8_t DecodeSector(char *buffer)
{
	buffer[1]=0;
	return buffer[0] - 0x30;
}


void EraseSector(uint8_t sectorNum)
{
	while(FLASH->SR & (1<<16));
	//Enable Sector Erase
	FLASH->CR |= (1<<1);

	//chose the sector
	FLASH->CR &=~(15 << 3);
	FLASH->CR |= (sectorNum <<3);

	//Start Erasing
	FLASH->CR |= (1<<16);

	//Wait until erasing is finished
	while(FLASH->SR & (1<<16));
}
