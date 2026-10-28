/*
 * UART.h
 *
 *  Created on: 26-Oct-2026
 *      Author: HP
 */

#ifndef INC_UART_H_
#define INC_UART_H_

void UART2_SendChar(char c);
void UART2_Print(const char *str);
int UART2_ReceiveChar(uint8_t *data, uint32_t timeout_loops);

#endif /* INC_UART_H_ */
