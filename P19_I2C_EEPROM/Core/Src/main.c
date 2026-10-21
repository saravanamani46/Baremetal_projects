#include "stm32f401xe.h"

int count=0;

int main()
{
	/*Enable clock access to PA5*/
	RCC->AHB1ENR |= (1U<<0);

	/*Enable clock access to TIM2*/
	RCC->APB1ENR |= (1U<<0);

	/*Configure GPIO as output*/
	GPIOA->MODER &= ~(3U<<10);
	GPIOA->MODER |= (1U<<10);

	/*Configure Pre-scaler timer*/
	TIM2->PSC = 15;

	/*Configure Automatic reload register*/
	TIM2->ARR = 1000000-1;

	/*Enable Time enable interrupt*/
	TIM2->DIER |= (1U<<0);

	//GPIOA->ODR |= (1U<<5);

	/*Call the interrupt handler*/
	NVIC_EnableIRQ(TIM2_IRQn);

	/*Start the counter*/
	TIM2->CR1 |= (1U<<0);

	while(1)
	{

	}

}

void TIM2_IRQHandler(void)
{

	if (TIM2->SR & (1U<<0))
	{
		TIM2->SR &= ~(1U<<0);
		count++;

		if(count ==1)
		{
			GPIOA->ODR |= (1U<<5);
		}

		else if (count==5)
		{
			GPIOA->ODR ^= (1U<<5);
		}

		else if (count==10)
		{
			GPIOA->ODR ^= (1U<<5);
			count=0;
		}
	}

}
