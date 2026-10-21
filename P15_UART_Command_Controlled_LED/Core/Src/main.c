#include "stm32f401xe.h"
int main()
{
	/*Enable clock access to PA5*/
	RCC->AHB1ENR |= (1U<<0);

	/*Configure GPIOA as output*/
	GPIOA->MODER &= ~(3U<<10);
	GPIOA->MODER |= (1U<<10);

	/*Configure output type register*/
	GPIOA->OTYPER &= ~(1U<<5);

	/*Configure output speed register*/
	GPIOA->OSPEEDR &= ~(3U<<10);

	/*Configure pullup and pulldown register*/
	GPIOA->PUPDR &= ~(3U<<10);

	/*Enable clock access to USART*/
	RCC->APB1ENR |= (1U<<17);

	/*Configure USART as alternate function*/
	GPIOA->MODER &= ~(3U<<4);
	GPIOA->MODER |= (1U<<5);

	/*Configure USART as alternate function*/
	GPIOA->MODER &= ~(3U<<6);
	GPIOA->MODER |= (1U<<7);

	/*Set PA2 and PA3 to alternate function mode*/
	/*Clear AF bits*/
	GPIOA->AFR[0] &= ~(0xFU<<8);
	GPIOA->AFR[0] &= ~(0xFU<<12);

	/*Set AF7*/
	GPIOA->AFR[0] |= (7U<<8);
	GPIOA->AFR[0] |= (7U<<12);

	/*Configure baud rate register*/
	USART2->BRR = 0x0683;
	/*Enable USART transmitter register*/
	USART2->CR1 |= (1U<<3);
	/*Enable USART receiver register*/
	USART2->CR1 |= (1U<<2);
	/*Enable USART*/
	USART2->CR1 |= (1U<<13);
	/*Enable USART word*/
	USART2->CR1 &= ~(1U<<12);

	while(1)
	{
		char data;
		while(!(USART2->SR & (1U<<5)))
		{

		}
		data = USART2->DR;

		if (data == 'g')
		{
			GPIOA->ODR |= (1U<<5);
		}

		else if (data == 'o')
		{
			GPIOA->ODR &= ~(1U<<5);
		}
	}
}
