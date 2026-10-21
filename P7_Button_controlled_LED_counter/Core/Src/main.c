#include "stm32f4xx.h"

int main()
{
	/*Enable clock access to GPIOA*/
	RCC->AHB1ENR |= (1U<<0);
	/*Enable clock access to GPIOB*/
	RCC->AHB1ENR |= (1U<<1);
	/*Enable clock access to GPIOC*/
	RCC->AHB1ENR |= (1U<<2);

	/*Configure GPIOA as output*/
	GPIOA->MODER &= ~(3U<<20);
	GPIOA->MODER |= (1U<<20);

	/*Configure GPIOB as output*/
	GPIOB->MODER &= ~(3U<<10);
	GPIOB->MODER |= (1U<<10);

	/*Configure GPIOC as input*/
	GPIOC->MODER &= ~(3U<<26);

	/*Configure Output type as push-pull*/
	GPIOA->OTYPER &= ~(1U<<10);
	GPIOB->OTYPER &= ~(1U<<5);

	/*Configure output speed as low*/
	GPIOA->OSPEEDR &= ~(3U<<20);
	GPIOB->OSPEEDR &= ~(3U<<10);

	/*Configure pullup pulldown register*/
	GPIOA->PUPDR &= ~(3U<<20);
	GPIOB->PUPDR &= ~(3U<<10);

	/*Configure pullup pulldown register*/
	GPIOC->PUPDR &= ~(3U<<26);

	int count=0;

	while(1)
	{
		if ((GPIOC->IDR & (1U << 13)) == 0)
		{ /* Increment only once for this press */
			count++;
			/* Reset after 4 */
			if (count > 4)
			{
				count = 1;
			}
			/* Wait until button is released */
			while ((GPIOC->IDR & (1U << 13)) == 0)
			{
				/* Wait */

			}
		}

		if (count == 1)
		{
			GPIOA->ODR |= (1U<<10);
			GPIOB->ODR &= ~(1U<<5);
		}

		else if (count == 2)
		{
			GPIOB->ODR |= (1U<<5);
			GPIOA->ODR &= ~(1U<<10);
		}

		else if (count == 3)
		{
			GPIOA->ODR |= (1U<<10);
			GPIOB->ODR |= (1U<<5);
		}

		else if (count == 4)
		{
			GPIOA->ODR &= ~(1U<<10);
			GPIOB->ODR &= ~(1U<<5);
		}
	}


}
