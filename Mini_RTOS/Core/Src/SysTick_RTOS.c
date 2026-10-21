/*
 * SysTick_RTOS.c
 *
 *  Created on: 20-Oct-2026
 *      Author: HP
 */

#include "stm32f401xe.h"
#include "SysTick_RTOS.h"

/*Number of milliseconds since RTOS tick initialization*/
static volatile uint32_t rtosTickCount = 0;

/*Configure SysTick to generate an interrupt every 1 ms*/
void RTOS_SysTick_Init(void)
{
	/*Configure SysTick using the processsor clock
	 * SystemCoreClock must contain the actual CPU clock frequency
	 */

	SysTick->CTRL = 0;

	rtosTickCount = 0;

	SysTick->LOAD = (SystemCoreClock / 1000) - 1;

	SysTick->VAL = 0;

	/* Configure SysTick for a 1 ms tick */
	 SysTick_Config(SystemCoreClock / 1000U);

	/* SysTick runs above PendSV in priority */
	NVIC_SetPriority(PendSV_IRQn, 15U);
	NVIC_SetPriority(SysTick_IRQn, 14U);

	/*Enable SysTick interrupt and use processor clock*/
	SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk |
					SysTick_CTRL_TICKINT_Msk   |
					SysTick_CTRL_ENABLE_Msk;

	 /* Configure SysTick for a 1 ms interrupt */
	 SysTick_Config(SystemCoreClock / 1000U);

	 /* Set SysTick interrupt priority */
	 NVIC_SetPriority(SysTick_IRQn, 14U);
}

uint32_t RTOS_Get_Tick(void)
{
	return rtosTickCount;
}

void RTOS_Tick_Handler(void)
{
	rtosTickCount++;
}


void SysTick_Handler(void)
{
    RTOS_Tick_Handler();

    /* Request a context switch */
    SCB->ICSR = SCB_ICSR_PENDSVSET_Msk;
}
