/*
 * UART.h
 *
 *  Created on: 20-Oct-2026
 *      Author: HP
 */

#ifndef INC_UART_H_
#define INC_UART_H_

#include <stdint.h>

void UART2_Init(uint32_t baudrate);
void UART2_SendString(const char *str);

#endif /* INC_UART_H_ */
