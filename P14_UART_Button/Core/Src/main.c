#include "stm32f401xe.h"

void delay();

void UART_transmit();

int main()
{

	/*Enable clock access for Port C*/
	RCC->AHB1ENR |= (1U<<2);

	/*Enable clock access for USART*/
	RCC->APB1ENR |= (1U<<17);

	/*set GPIOC as input function*/
	GPIOC->MODER &= ~(3U<<26);

	/*Enable clock access to Port A*/
	RCC->AHB1ENR |= (1U<<0);

	/*Set PA2 as alternate function*/
	GPIOA->MODER &= ~(3U<<4);
	GPIOA->MODER |= (1U<<5);

	/*Set PA3 as alternate function*/
	GPIOA->MODER &= ~(3U<<6);
	GPIOA->MODER |= (1U<<7);

	/*Set PA2 and PA3 to alternate function mode*/
	/*Clear AF bits*/
	GPIOA->AFR[0] &= ~(0xFU<<8);
	GPIOA->AFR[0] &= ~(0xFU<<12);

	/*Set AF7*/
	GPIOA->AFR[0] |= (7U<<8);
	GPIOA->AFR[0] |= (7U<<12);

	/*Configure baudrate*/
	USART2->BRR = 0x0683;
	/*Set USART word length*/
	USART2->CR1 &= ~(1U<<12);
	/*Enable USART*/
	USART2->CR1 |= (1U<<13);
	/*Enable USART transmitter*/
	USART2->CR1 |= (1U<<3);

	while(1)
	{
		/*If button is released transmit the data*/
		if (!(GPIOC->IDR & (1U<<13)))
		{
			UART_transmit();
		}

		/*wait for transmission*/
		while(!(GPIOC->IDR & (1U<<13)))
		{
			delay();
		}
	}
}

void UART_transmit()
{
	char data[3] = "On";
	for(int i=0;i<3;i++)
	{
		/*check for USART status register for setting the interrupt bit in receiver*/
		while(!(USART2->SR &(1U<<7)))
		{

		}
		/*Transmit character via USART*/
		USART2->DR = data[i];
	}
}

void delay()
{
	for(int i=0;i<5000000;i++)
	{

	}
}
