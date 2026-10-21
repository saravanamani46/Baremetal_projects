#include "stm32f401xe.h"

void long_delay(void)
{
	for(int i=0;i<10000000;i++)
	{

	}
}

void delay(void)
{
	for(int i=0;i<1000000;i++)
	{

	}
}

int main(void)
{
	/*Enable clock access to GPIOA*/
	RCC->AHB1ENR |= (1U<<0);
	/*Enable clock access to GPIOB*/
	RCC->AHB1ENR |= (1U<<1);

	/*Configure PA10 (D2) as output*/
	GPIOA->MODER &= ~(3U<<20);
	GPIOA->MODER |= (1U<<20);

	/*Configure PB5 (D4) as output*/
	GPIOB->MODER &= ~(3U<<10);
	GPIOB->MODER |= (1U<<10);

	/*Configure Output type register*/
	GPIOA->OTYPER &= ~(1U<<10);
	GPIOB->OTYPER &= ~(1U<<5);

	/*Configure output speed register*/
	GPIOA->OSPEEDR &= ~(3U<<20);
	GPIOB->OSPEEDR &= ~(3U<<10);

	/*Configure pull up and pull down register*/
	GPIOA->PUPDR &= ~(3U<<20);
	GPIOB->PUPDR &= ~(3U<<10);

	while(1)
	{
		GPIOA->ODR |= (1U<<10);
		GPIOB->ODR &= ~(1U<<5);

		long_delay();

		GPIOA->ODR &= ~(1U<<10);
		GPIOB->ODR |= (1U<<5);

		delay();
	}
}

