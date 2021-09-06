
#include "stm32f4xx.h"


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

int main(void)
{


	RCC->AHB1ENR|=(1<<0);
	GPIOA->MODER|=(1<<10);

	usart_Init();
  while (1)
  {
	for(uint32_t i = 0; i < 800000 ; i++){}
	GPIOA->ODR^=(1<<5);
	USART1_SendString("Inside Application\r\n");
  }
}
