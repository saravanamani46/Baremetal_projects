#include "stm32f401xe.h"
#include "system_stm32f4xx.h"
#include "UART.h"

void UART2_Init(uint32_t baudrate)
{
	uint32_t pclk1;
	uint32_t prescaler_bits;

	/*Enable GPIOA clock*/
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;

	/*Enable USART2 clock*/
	RCC->APB1ENR |= RCC_APB1ENR_USART2EN;

	/*Configure PA2 and PA3 for alternate function*/
	GPIOA->MODER &= ~((3U<<4) | (3U<<6));
	GPIOA->MODER |= ((2U<<4U) | (2U<<6));

	/*Select AF7 for PA2 and PA3*/
	GPIOA->AFR[0] &= ~((0xFU << 8) | (0xFU<<12));
	GPIOA->AFR[0] |= ((7U<<8) | (7U<<12));

	/*Calculate APB1 peripheral clock*/
	pclk1 = SystemCoreClock;
	prescaler_bits = (RCC->CFGR >> 10U) & 0x7;

	if (prescaler_bits >= 4)
	{
		pclk1 >>= (prescaler_bits - 3);
	}

	/*Disable USART before configuration*/
	USART2->CR1 = 0;
	USART2->CR2 = 0;
	USART2->CR3 = 0;

	/*Configure baud rate*/
	USART2->BRR = (pclk1 + (baudrate / 2)) / baudrate;

	/*Enable transmitter, receiver and USART*/
	USART2->CR1 |= USART_CR1_TE |
			       USART_CR1_RE |
				   USART_CR1_UE;


}

void UART2_SendString(const char *str)
{
	uint32_t primask = __get_PRIMASK();

	__disable_irq();

	while (*str != '\0')
	{
		while ((USART2->SR & USART_SR_TXE) == 0)
		{

		}

		USART2->DR = (uint8_t)*str;
		str++;
	}

	while ((USART2->SR & USART_SR_TC) == 0)
	{

	}

	/*Restore the previous interrupt state*/
	if (primask == 0)
	{
		__enable_irq();
	}
}
