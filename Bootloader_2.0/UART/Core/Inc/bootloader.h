/*
 * bootloader.h
 *
 *  Created on: 26-Oct-2026
 *      Author: HP
 */

#ifndef INC_BOOTLOADER_H_
#define INC_BOOTLOADER_H_

void Jump_To_Application(uint32_t app_address);
void Bootloader_Menu(void);

#endif /* INC_BOOTLOADER_H_ */
