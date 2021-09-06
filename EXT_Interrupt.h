/*
 * EXT_Interrupt.h
 *
 *  Created on: Sep 5, 2021
 *      Author: Ahmed Azazy
 */

#ifndef EXT_INTERRUPT_H_
#define EXT_INTERRUPT_H_


extern uint8_t Enter_BootLoader;


typedef enum edge_selection
{
	FALLING_EDGE ,
	RISING_EDGE ,
	BOTH
}edgeSelection_t;

void EXT_INT13_Init(edgeSelection_t );

#endif /* EXT_INTERRUPT_H_ */
