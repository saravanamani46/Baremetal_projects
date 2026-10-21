#include "stm32f401xe.h"
int main()
{

	/*Enable clock access to the port PA5*/
	RCC->AHB1ENR |= (1U<<0);
	/*Enable clock access to the Timer2*/
	RCC->APB1ENR |= (1U<<0);

	/*Configure GPIOA modes as output*/
	GPIOA->MODER &= ~(1U<<10);
	GPIOA->MODER |= (1U<<10);

	/*Configure Timer pre-scaler*/
	TIM2->PSC = 15;
	/*Configure Auto-reload register*/
	TIM2->ARR = 2000000 - 1;
	/*Reset the counter*/
	TIM2->CNT = 0;
	/*Enable the counter*/
	TIM2->CR1 |= (1U<<0);

	while(1)
	{
		while((TIM2->SR) & (1U<<0))
		{
			TIM2->SR &= ~(1U<<0);
			GPIOA->ODR ^= (1U<<5);
		}
	}
}
