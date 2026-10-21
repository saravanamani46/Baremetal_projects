#include "stm32f303xe.h"
#include <stdint.h>
#include <stdio.h>

void Clock_config()
{
    /*Enabling clock for GPIOB and GPIOA*/
    RCC->AHBENR |= (1U << 18); // GPIOB
    RCC->AHBENR |= (1U << 17); // GPIOA

	/*Enabling clock for TIM3, TIM2 and USART2*/
	RCC->APB1ENR |= (1U<<1); //TIM3 clock
	RCC->APB1ENR |= (1U<<0); //TIM2 clock
	RCC->APB1ENR |= (1U<<17); //USART2 clock
}

void Output_PWM_config()
{
    /*Configure PB4 for PWM (TIM3_CH1)*/
    GPIOB->MODER &= ~(3U << 8); // Alternate function mode
    GPIOB->MODER |=  (2U << 8);

	GPIOB->AFR[0] &= ~(0xFU << 16);
	GPIOB->AFR[0] |= (2U<<16); //AF2 = TIM3_CH1
}

void TIM3_Config()
{
    /*Configure TIM3 for 20 kHz PWM mode (ARR = 399, PSC = 0)*/
    TIM3->CR1 = 0;
    TIM3->PSC = 0;   // No prescaler (8 MHz timer clock)
    TIM3->ARR = 399; // 400 total counts = 20 kHz



	TIM3->CCMR1 &= ~(7U<<4);
	TIM3->CCMR1 |= (6U<<4); //PWM mode 1
	TIM3->CCMR1 |= (1U<<3); //CCR1 preload module

	TIM3->CCER |= (1U<<0); //Enable channel 1 output

    TIM3->EGR   |=  (1U << 0); // Generate update event
    TIM3->CR1   |=  (1U << 7); // Enable ARR preload
    TIM3->CR1   |=  (1U << 0); // Start TIM3

    TIM3->CCR1   =  100;       // Initial duty cycle: 100/400 = 25%
}

void Input_PWM_Config()
{
    /*Configure PA0 for input capture (TIM2_CH1)*/
    GPIOA->MODER &= ~(3U << 0);
    GPIOA->MODER |=  (2U << 0); // Alternate function mode

	GPIOA->AFR[0] &= ~(0xFU<<0);
	GPIOA->AFR[0] |= (1U<<0); //AF1 = TIM2_CH1
}

void TIM2_Config()
{
    /*Configure TIM2 to capture signal edges*/
    TIM2->CR1 = 0;
    TIM2->PSC = 0;           // Match TIM3 (8 MHz tick for high resolution)
    TIM2->ARR = 0xFFFFFFFFU; // 32-bit counter range

	TIM2->CCMR1 = (1U<<0) | (2U<<8);

	TIM2->CCER = (1U << 0) | (1U << 4) | (1U << 5);


	TIM2->SMCR = (5U<<4) | (4U << 0);

	TIM2->EGR = (1U<<0);  //Initialize prescaler/counter
	TIM2->SR = 0;         //Clear status flags
	TIM2->CR1 |= (1U<<0); //Start TIM2
}

void USART2_configure()
{
	/*Configure USART2*/
	GPIOA->MODER &= ~(3U<<4);
	GPIOA->MODER |= (2U<<4); //PA2 alternate function

	GPIOA->AFR[0] &= ~(0xFu<<8);
	GPIOA->AFR[0] |= (7U<<8);  //AF7 = USART2_TX

	USART2->CR1 = 0;
	USART2->BRR = 0x45;     //PCLK1 = 8 Mhz, 115200 baudrate
	USART2->CR1 |= (1U<<3); //Transmitter enable
	USART2->CR1 |= (1U<<0); //USART enable
}


void UART2_sendChar(char c)
{
	while((USART2->ISR & (1U<<7)) == 0)
	{

	}

	USART2->TDR = (uint8_t)c;
}

void UART2_sendString(const char *str)
{
	while (*str != '\0')
	{
		UART2_sendChar(*str++);
	}
}


int main()
{
    char message[128];
    // Corrected step values mapped to ARR = 399 (25%, 50%, 75%, 100%)
    uint32_t duty_steps[] = {100, 200, 300, 399};
    int step_index = 0;
    uint32_t capture_count = 0;

	Clock_config();
	Output_PWM_config();
	TIM3_Config();
	Input_PWM_Config();
	TIM2_Config();
	USART2_configure();

    while(1)
    {
        /* Wait until Channel 1 captures a full cycle */
        if ((TIM2->SR & (1U << 1)) != 0)
        {
            /* Read hardware-captured values */
            uint32_t period_ticks = TIM2->CCR1;
            uint32_t high_ticks   = TIM2->CCR2;

					/* Clear capture flags for both CC1 and CC2 */
					TIM2->SR &= ~((1U << 1) | (1U << 2));

            if (period_ticks != 0U)
            {
                // With PSC = 0 (8 MHz timer clock), frequency calculation uses 8000000 ticks/sec
                uint32_t frequency = 8000000U / period_ticks;
                uint32_t duty = ((high_ticks * 100U) + (period_ticks / 2)) / period_ticks;

                snprintf(message, sizeof(message),
                         "Frequency: %lu Hz, "
                         "Period: %lu ticks, "
                         "High: %lu ticks, "
                         "Duty: %lu %%\r\n",
                         (unsigned long)frequency,
                         (unsigned long)period_ticks,
                         (unsigned long)high_ticks,
                         (unsigned long)duty);

                UART2_sendString(message);
            }

            // Change duty cycle only after a few successful captures
            // so each step has time to print and stabilize on the terminal
            capture_count++;
            if (capture_count >= 100)
            {
                capture_count = 0;
                step_index = (step_index + 1) % 4;
                TIM3->CCR1 = duty_steps[step_index];
            }
        }
    }
}




