
#include "stm32f303xe.h"
#include <stdint.h>

void delay(volatile uint32_t count)
{
    while (count--)
    {
    }
}

void IWDG_Init(void)
{
    /* 1. Enable LSI oscillator */
    RCC->CSR |= RCC_CSR_LSION;

    /* 2. Wait until LSI is ready */
    while ((RCC->CSR & RCC_CSR_LSIRDY) == 0U)
    {
    }

    IWDG->KR = 0xCCCCu;

    /* 3. Unlock IWDG configuration registers */
    IWDG->KR = 0x5555U;

    /* 4. Configure prescaler: divide by 32 */
    IWDG->PR = 3U;

    /* 5. Set reload value */
    IWDG->RLR = 2000U;

    /* 6. Wait for register updates */
    while ((IWDG->SR & (IWDG_SR_PVU | IWDG_SR_RVU)) != 0U)
    {
    }

    /* 7. Reload counter */
    IWDG->KR = 0xAAAAU;

    /* 8. Start watchdog */
  //  IWDG->KR = 0xCCCCU;
}

void IWDG_Refresh(void)
{
    /* Refresh watchdog counter */
    IWDG->KR = 0xAAAAU;
}

int main(void)
{
    /* Enable GPIOA clock */
    RCC->AHBENR |= (1U << 17);

    /* Configure PA5 as output */
    GPIOA->MODER &= ~(3U << 10);
    GPIOA->MODER |=  (1U << 10);

    /* Initialize watchdog */
    IWDG_Init();

    while (1)
    {
        /* Normal operation: toggle LED */
        GPIOA->ODR ^= (1U << 5);
        delay(500000U);

        GPIOA->ODR ^= (1U << 5);
        delay(500000U);

        /* Keep watchdog alive */
        IWDG_Refresh();

        /*
         * WATCHDOG TEST:
         * Uncomment the following two lines
         * to simulate a software hang.
         */

         GPIOA->BSRR = (1U << 5);  /* Force LED ON */
         while (1) { }             /* Simulate hang */
    }
}
