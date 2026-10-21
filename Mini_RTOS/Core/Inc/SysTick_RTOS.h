/*
 * SysTick_RTOS.h
 *
 *  Created on: 20-Oct-2026
 *      Author: HP
 */

#ifndef INC_SYSTICK_RTOS_H_
#define INC_SYSTICK_RTOS_H_

#include <stdint.h>

/*Initialize SysTick for a 1 ms RTOS tick*/
void RTOS_SysTick_Init(void);

/*Return the current RTOS tick count*/
uint32_t RTOS_Get_Tick(void);

/*Called by the SysTick interrupt handler*/
void RTOS_Tick_Hanlder(void);

#endif /* INC_SYSTICK_RTOS_H_ */
