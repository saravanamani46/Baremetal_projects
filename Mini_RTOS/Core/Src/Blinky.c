#include "stm32f401xe.h"


void LED_Init(void)
{
    // 1. Enable the clock for GPIOA
    // The AHB1 peripheral clock enable register (RCC_AHB1ENR) controls port clocks.
    // Bit 0 corresponds to GPIOA.
    RCC->AHB1ENR |= (1U << 0);

    // 2. Configure Pin PA5 as a General Purpose Output
    // Each pin uses 2 bits in the MODER register. PA5 uses bits 10 and 11 (5 * 2).
    // Mode values: 00 = Input, 01 = Output, 10 = Alternate Function, 11 = Analog
    GPIOA->MODER &= ~(3U <<10); // Clear bits 10 and 11
    GPIOA->MODER |=  (1U <<10); // Set bit 10 to 1 (Output mode)

}

void LED_toggle(void)
{

        GPIOA->ODR ^= (1U << 5);

        for (volatile uint32_t i = 0; i < 500000U; i++)
        {

        }
}
