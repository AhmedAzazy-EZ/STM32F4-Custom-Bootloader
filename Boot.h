/*
 * Boot.h
 *
 *  Created on: Sep 6, 2021
 *      Author: Ahmed Azazy
 */

#ifndef BOOT_H_
#define BOOT_H_


#include <stdint.h>

typedef void (*funPtr)(void );

void DecodeAddressIntoHex(uint32_t * , char *);
void Config_flash(void);
void JumpToUserCode(void);
void BootHelp(void );
uint8_t DecodeSector(char *);
void EraseSector(uint8_t sectorNum);

#endif /* BOOT_H_ */
