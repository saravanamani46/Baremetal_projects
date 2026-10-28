#include "stm32f4xx.h"
#include "UART.h"
// Function prototypes for your initialization and menu
void UART2_Init(void);
void Bootloader_Menu(void);

int main(void) {
    // 1. Initialize the system clocks and UART peripheral
    UART2_Init();

    // 2. Launch the interactive bootloader menu
    Bootloader_Menu();

    // 3. Fallback loop (should not be reached)
    while (1)
    {

    }
}
