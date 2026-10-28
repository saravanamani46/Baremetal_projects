#include <stdint.h>
#include "stm32f401xe.h"
#include "UART.h"

#define SCB_VTOR (*(volatile uint32_t *)0xE000ED08UL)

void UART2_Init(void);
void UART2_Print(const char *str);

int main(void) {
    // 1. CRITICAL: Relocate Vector Table to match App 1 linker script offset
    SCB_VTOR = 0x08028000UL;

    // 2. Initialize App 1 peripherals
    UART2_Init();

    // 3. Main application loop (matches the video output: "APP 1 is running...")
    while (1)
    {
        UART2_Print("APP 1 is running...\r\n");
        for (volatile uint32_t i = 0; i < 500000; i++) // Simple delay
        {

        }
    }
}
