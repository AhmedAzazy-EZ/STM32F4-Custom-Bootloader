/*
 * usart.h
 *
 *  Created on: Sep 5, 2021
 *      Author: Ahmed Azazy
 */

#ifndef USART_H_
#define USART_H_

extern char USART1_Buffer[100];

void usart_Init(void );
void USART1_SendString(char *);
void USART1_Receive(uint8_t length , char * buffer);

#endif /* USART_H_ */
