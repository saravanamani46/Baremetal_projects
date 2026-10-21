#include "stm32f401xe.h"

void delay(void)
{
    for (volatile unsigned int i = 0; i < 1000000; i++);
}

int main(void)
{
    /* 1. Enable GPIOA clock */
    RCC->AHB1ENR |= (1 << 0);

    /* 2. Configure PA10 as General Purpose Output */
    GPIOA->MODER &= ~(3 << (10 * 2));  // Clear bits 20 and 21
    GPIOA->MODER |=  (1 << (10 * 2));  // Set PA10 as output

    /* 3. Push-pull output (default, but explicitly configure it) */
    GPIOA->OTYPER &= ~(1 << 10);

    /* 4. No pull-up / pull-down */
    GPIOA->PUPDR &= ~(3 << (10 * 2));

    while (1)
    {
        /* LED ON */
        GPIOA->ODR |= (1 << 10);

        delay();

        /* LED OFF */
        GPIOA->ODR &= ~(1 << 10);

        delay();
    }
}
