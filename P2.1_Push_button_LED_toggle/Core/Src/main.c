#include "stm32f401xe.h"


int main(void)
{
	/*Enable clock access to GPIOA*/
	RCC->AHB1ENR |= (1U<<0);
	RCC->AHB1ENR |= (1U<<2);

	/*Enable LED as output*/

	GPIOA->MODER &= ~(3U<<10);
	GPIOA->MODER |= (1U<<10);

	GPIOA->OTYPER &= ~(1U<<5);
	GPIOA->OSPEEDR  &= ~(3U<<10);
	GPIOA->PUPDR &= ~(3U<<10);

	GPIOC->MODER &= ~(3U<<26);
	GPIOC->MODER |= (1U<<10);

	while(1)
	{
		if ((GPIOC->IDR & (1U<<13)) == 0)
		{
			GPIOA->ODR &= ~(1U<<5);
		}

		else
		{
			GPIOA->ODR |= (1U<<5);
		}
	}

}

