#include "stm32f401xe.h"
#include "Timer.h"
#include <stdint.h>

volatile uint32_t elapsed_seconds = 0;

void TIM2_Init(void)
{
	/*Enable TIM2 clock*/
	RCC->APB1ENR |= (1U<<0);

	/*Stop TIM2 during configuration*/
	TIM2->CR1 = 0;

	/*Configure counter frequency to 1kHz*/
	TIM2->PSC = 15999;

	/*Configure auto reload register*/
	TIM2->ARR = 999;

	/*Load the prescaler*/
	TIM2->EGR = TIM_EGR_UG;

	/*Clear pending timer flags*/
	TIM2->SR = 0;

	/*Enable update interrupt*/
	TIM2->DIER |= TIM_DIER_UIE;

	/*Enable interrupt in NVIC*/
	NVIC_ClearPendingIRQ(TIM2_IRQn);
	NVIC_EnableIRQ(TIM2_IRQn);

	/*Start timer*/
	TIM2->CR1 |= TIM_CR1_CEN;
}

void TIM2_IRQHandler(void)
{
	if(TIM2->SR & TIM_SR_UIF)
	{
		/*Clear update interrupt flag*/
		TIM2->SR &= ~(TIM_SR_UIF);

		/*Count one elapsed second*/
		elapsed_seconds++;
	}
}
