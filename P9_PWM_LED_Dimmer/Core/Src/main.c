#include "stm32f4xx.h"

void delay(void)
{
	for(int i=0;i<1000000;i++)
	{

	}
}

int main()
{
	/*Enable clock access to GPIOB*/
	RCC->AHB1ENR |= (1U<<1);
	/*Enable clock access to Timer3*/
	RCC->APB1ENR |= (1U<<1);

	/*Configure GPIOB as alternate function register*/
	GPIOB->MODER &= ~(3U<<8);
	GPIOB->MODER |= (1U<<9);

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

   /*Configure CCMR register as output*/
	TIM3->CCMR1 &= ~(3U<<0);

	/*Enable PWM mode register*/
	TIM3->CCMR1 &= ~(7U << 4);   // Clear OC1M bits [6:4]
	TIM3->CCMR1 |=  (6U << 4);   // Set OC1M = 110

	/*Enable Pre-scaler register*/
	TIM3->CCMR1 |= (1U<<3);

	/*Configure the generated PWM to the required output pin*/
	TIM3->CCER |= (1U<<0);

	/*Start the Timer*/
	TIM3->CR1 |= (1U<<0);


	while(1)
	{
		TIM3->CCR1 = 500;
		delay();

		TIM3->CCR1 = 2500;
		delay();

		TIM3->CCR1 = 5000;
		delay();

		TIM3->CCR1 = 7500;
		delay();

		TIM3->CCR1 = 9999;
		delay();

		TIM3->CCR1 = 0;
		delay();

	}


}
