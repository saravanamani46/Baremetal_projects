#include "stm32f4xx.h"

int main()
{
	/*Enable clock access to GPIOA*/
	RCC->AHB1ENR |= (1U<<0);
	/*Enable clock access to USART2*/
	RCC->APB1ENR |= (1U<<17);

	/*Configure TX pin as alternate function*/
	GPIOA->MODER &= ~(3U<<4);
	GPIOA->MODER |= (1U<<5);

	/*select correct alternate function for TX pin*/
	GPIOA->AFR[0] &= ~(0xFU<<8);
	GPIOA->AFR[0] |= (7U<<8);

	/*Configure USART baudrate*/
	USART2->BRR = 0x0683;
	//USART2->BRR = 0x111B;

	/*Configure USART power format*/
	USART2->CR1 &= ~(1U<<12);
	/*Configure the parity bit*/
	USART2->CR1 &= ~(1U<<10);
    /*Configure the stop bit*/
	USART2->CR2 &= ~(3U<<12);

   /*Enable USART*/
	USART2->CR1 |= (1U<<13);

	/*Enable USART transmitter*/
	USART2->CR1 |= (1U<<3);

	/*Check TXE flag*/
	while(!(USART2->SR & (1U<<7)))
	{

	}

	/*Write data to USART data register*/
	USART2->DR = 'A';

	while(!(USART2->SR & (1U<<6)))
	{

	}

	/*Send next character*/
	USART2->DR = 'B';

}

