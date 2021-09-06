/*
 * main.c
 *
 *  Created on: Sep 5, 2021
 *      Author: Ahmed Azazy
 */

#include "main.h"
#include "usart.h"
#include <stm32F446xx.h>
#include "EXT_Interrupt.h"
#include "timer.h"
#include "Boot.h"

#define BOOTLOADER_VERSION "1.0.0\r\n"
uint8_t command;
uint32_t address;

int main(void)
{

  usart_Init();
  EXT_INT13_Init(FALLING_EDGE);
  Config_flash();
  TIM9_Init();

  while (1)
  {
	  if(Enter_BootLoader == 1)
	  {

		  //Stop TIM9
		  RCC->APB2ENR|=(1<<16);
		  TIM9->CR1 &=~(1<<0);

		  BootHelp();


		  while(1){

			  while(!(USART1->SR & (1<<5)));

			  command = USART1->DR;

			  switch(command)
			  {

			  case 0x51 :
			  	USART1_SendString("Boot Loader Version is ");
			  	USART1_SendString(BOOTLOADER_VERSION);
			  	break;


			  case 0x52 :
				  USART1_SendString("Please Send start address consists of 4 bytes\r\nIn little Endian \r\n");
				  USART1_Receive(8 , USART1_Buffer);
				  DecodeAddressIntoHex(&address , USART1_Buffer);
				  USART1_SendString("Send Image\r\n");

				  uint32_t counter = 0;
				  uint32_t retry = 0;
				  while(!(USART1->SR & (1<<5)));
				  while(1)
				  {
					  while(!(USART1->SR & (1<<5)) && retry++ < 8000000){}
					  if(retry >= 8000000)
					    break;
					  retry = 0;
					  while(FLASH->SR & (1<<16));
					  *((uint8_t *)(address + counter++)) = USART1->DR;
				  }

				  USART1_SendString("WE ARE DONE\r\n");
				  break;


			  case 0x53 :
				  JumpToUserCode();
				  break;

			  case 0x54 :
				  USART1_SendString("Please Select a sector to Erase\r\n");
				  USART1_SendString("available Sectors to Erase are 2 , 3 , 4 , 5 , 6 , 7\r\n");
				  USART1_Receive(1 , USART1_Buffer);
				  uint8_t sectorNumber = DecodeSector(USART1_Buffer);
				  USART1_SendString("Do you want To Erase Sector ");
				  USART1_SendString(USART1_Buffer);
				  USART1_SendString("\r\n1 for Yes 					2 for No\r\n");
				  USART1_Receive(1 , USART1_Buffer);

				  if(USART1_Buffer[0] == '2')
					  break;
				  else if(USART1_Buffer[0] == '1')
				  {}

				  else
				  {
					  USART1_SendString("Invalid Option\r\n");
					  break;
				  }


				  if(sectorNumber >=2 && sectorNumber <= 7)
				  {
					  EraseSector(sectorNumber);
					  USART1_SendString("Done\r\n");
				  }

				  else
					  USART1_SendString("Sorry Invalid Sector number \r\n");

				  break;

			  case 0x55 :
				  BootHelp();
				  break;




			  case 13 : //Ignore Enter Button
				  command = 0;
				  break;

			  default :
				  command = 0;
				  USART1_SendString("No Valid command\r\n");

		  }
		  }

	  }
  }
}

