#include "stm32f4xx.h"

int main()
{
	/*Enable clock access to PC13*/
	RCC->AHB1ENR |= (1U<<2);
	/*Enable clock access to PB4*/
	RCC->AHB1ENR |= (1U<<1);
	/*Enable clock access to TIM3*/
	RCC->APB1ENR |= (1U<<1);


	/*Configure GPIO pin as alternate function*/
	GPIOB->MODER &= ~(3U<<8);
	GPIOB->MODER |= (1U<<9);

	/*Configure GPIO pin as input pin*/
	GPIOC->MODER &= ~(3U<<26);

	/*PB4 -> AF2 -> TIM3_CH1*/
	GPIOB->AFR[0] &= ~(0xFU<<16);
	GPIOB->AFR[0] |= (0x2U<<16);

	/*Set prescaler value*/
	TIM3->PSC = 15;

	/*Set auto-reload value*/
	TIM3->ARR = 10000 - 1;

	/*Set CCR1 value*/
	//TIM3->CCR1 = 500;

	/*Clear counter*/
	TIM3->CNT = 0;

	/*Configure CCR1 value as output*/
	TIM3->CCMR1 &= ~(3U<<0);

	/*Enable PWM*/
	TIM3->CCER |= (1U<<0);

	/*Enable PWM mode*/
	TIM3->CCMR1 &= ~(7U<<4);
	TIM3->CCMR1 |= (6U<<4);

	/*Enable Pre-scaler register*/
	TIM3->CCMR1 |= (1U<<3);

	/*Configure the PWM pin to output pin*/
//	TIM3->CCMR1 |= (1U<<0);

	/*Enable the counter*/
	TIM3->CR1 |= (1U<<0);

	int count = 0;

	while(1)
	{
	    /* Wait for button press */
	    while (GPIOC->IDR & (1U << 13))
	    {
	    }

	    /* One press = one count */
	    count++;

	    if (count == 1)
	    {
	        TIM3->CCR1 = 500;
	    }
	    else if (count == 2)
	    {
	        TIM3->CCR1 = 2500;
	    }
	    else if (count == 3)
	    {
	        TIM3->CCR1 = 5000;
	    }
	    else if (count == 4)
	    {
	        TIM3->CCR1 = 7500;
	    }
	    else if (count == 5)
	    {
	        TIM3->CCR1 = 9999;
	        count = 0;
	    }

	    /* Wait for button release */
	    while (!(GPIOC->IDR & (1U << 13)))
	    {
	    }
	}

}

