#include <stdint.h>
#include "stm32f401xe.h"
#include "UART.h"

#define SCB_VTOR (*(volatile uint32_t *)0xE000ED08UL)

void UART2_Init(void);
void UART2_Print(const char *str);

int main(void) {
    // 1. CRITICAL: Relocate Vector Table to match Factory App linker script offset
    SCB_VTOR = 0x08008000UL;

    // 2. Initialize Factory App peripherals
    UART2_Init();

    // 3. Main recovery application loop (matches video output: "FACTORY APP is running...")
    while (1)
    {
        UART2_Print("FACTORY APP is running...\r\n");
        for (volatile uint32_t i = 0; i < 500000; i++) // Simple delay
        {

        }
    }
}
