#include "stm32f4xx.h"

int main()
{
	/*Enable clock access to D0*/
	RCC->AHB1ENR = (1U<<0);

	/*Enable clock access to UART*/
	RCC->APB1ENR = (1U<<17);

	/*Configure GPIO pins as alternate function*/
	GPIOA->MODER &= ~(3U<<4);
	GPIOA->MODER &= ~(3U<<6);

	GPIOA->MODER |= (2U<<4);
	GPIOA->MODER |= (2U<<6);

	/*Set PA2 and PA3 to alternate function mode*/
	/* Clear AF bits */
	GPIOA->AFR[0] &= ~(0xFU << 8);    // PA2
	GPIOA->AFR[0] &= ~(0xFU << 12);   // PA3

	/* Set AF7 */
	GPIOA->AFR[0] |= (7U << 8);       // PA2 → USART2_TX
	GPIOA->AFR[0] |= (7U << 12);      // PA3 → USART2_RX

	/*Configure the Baud rate*/
	USART2->BRR = 0x0683;

	/*Set USART word length*/
	USART2->CR1 &= ~(1U<<12);
	/*Disable the parity bit*/
	USART2->CR1 &= ~(1U<<10);
	/*Configure the stop bit*/
	USART2->CR2 &= ~(3U<<12);

	/*Enable USART2*/
	USART2->CR1 |= (1U<<13);
	/*Enable USART2 transmitter*/
	USART2->CR1 |= (1U<<3);
	/*Enable USART2 receiver*/
	USART2->CR1 |= (1U<<2);

	while(1)
	{
		/*Check USART RXNE*/
		while(!(USART2->SR & (1U<<5)))
		{

		}

		char data = USART2->DR;

		/*Check USART TXNE*/
		while(!(USART2->SR & (1U<<7)))
		{

		}

		USART2->DR = data;
	}






}

