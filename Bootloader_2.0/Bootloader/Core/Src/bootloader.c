#include "stm32f401xe.h"
#include <stdint.h>
#include "UART.h"
#include "bootloader.h"

#define USART2_SR     (*(volatile uint32_t *)(USART2_BASE + 0x00))
#define USART2_DR     (*(volatile uint32_t *)(USART2_BASE + 0x04))


// Application Addresses
#define APP1_ADDRESS        0x08008000U
#define FACTORY_APP_ADDRESS 0x08044000U
#define DEFAULT_APP_ADDRESS APP1_ADDRESS

// Function pointer type for application entry point
typedef void (*pFunction)(void);

void UART2_PrintHex(uint32_t num)
{
    char buf[11];
    buf[0] = '0';
    buf[1] = 'x';

    for (int i = 7; i >= 0; i--)
    {
        uint32_t nibble = (num >> (i * 4)) & 0x0F;
        if (nibble < 10)
            buf[9 - i] = '0' + nibble;
        else
            buf[9 - i] = 'A' + (nibble - 10);
    }
    buf[10] = '\0';

    UART2_Print(buf);
}

void Bootloader_Menu(void)
{
    const char *menu_msg = "\r\n"
                           "*******************************\r\n"
                           "     Bare-Metal Bootloader     \r\n"
                           "*******************************\r\n"
                           "1 -> Run App 1\r\n"
                           "2 -> Run Factory App\r\n"
                           "Any Key -> Run Default App\r\n"
                           "Waiting for selection...\r\n";

    // 1. Print menu over UART
    UART2_Print(menu_msg);

    uint8_t rx_data = 0;

    // 2. Wait for user input with a timeout loop
    // (Adjust the loop counter constant based on your CPU clock speed for desired seconds)
    uint32_t timeout_cycles = 15000000UL;
    int received = UART2_ReceiveChar(&rx_data, timeout_cycles);

    if (received)
    {
        if (rx_data == '1')
        {
            UART2_Print("\r\nExecuting App 1...\r\n");
            UART2_PrintHex(APP1_ADDRESS);
            UART2_Print("\r\n");
            Jump_To_Application(APP1_ADDRESS);
        }
        else if (rx_data == '2')
        {
            UART2_Print("\r\nExecuting Factory App...\r\n");
            UART2_PrintHex(FACTORY_APP_ADDRESS);
            UART2_Print("\r\n");
            Jump_To_Application(FACTORY_APP_ADDRESS);
        }
        else
        {
            UART2_Print("\r\nKey received. Running Default App...\r\n");
            UART2_PrintHex(DEFAULT_APP_ADDRESS);
            UART2_Print("\r\n");
            Jump_To_Application(DEFAULT_APP_ADDRESS);
        }
    }
    else
    {
        // 3. Timeout fallback
        UART2_Print("\r\nTimeout expired. Defaulting to App 1...\r\n");
        Jump_To_Application(DEFAULT_APP_ADDRESS);
    }
}

//void Jump_To_Application(uint32_t app_address)
//{
//    // 1. Check if the stack pointer location is valid (must point to RAM region 0x20000000)
//    uint32_t
//	msp_value = *(volatile uint32_t *)app_address;
//
//    if ((msp_value & 0x2FFE0000) == 0x20000000)
//    {
//
//        // 2. Fetch the Reset Handler address from the application's vector table (offset +4)
//        uint32_t reset_handler_address = *(volatile uint32_t *)(app_address + 4);
//        pFunction app_reset_handler = (pFunction)reset_handler_address;
//
//        // 3. Disable all global interrupts before jumping
//        __asm volatile ("cpsid i" : : : "memory");
//
//        // 4. Set the Main Stack Pointer (MSP) to the application's stack value
//        __asm volatile ("MSR msp, %0" : : "r" (msp_value) : "memory");
//
//        // 5. Optional: Deinitialize peripherals used by bootloader (e.g., UART)
//        // USART2->CR1 &= ~((uint32_t)1 << 13); // Disable USART
//
//        // 6. Jump to the application's Reset Handler
//        app_reset_handler();
//    }
//
//    else
//    {
//        // Handle invalid app image error (e.g., print error and loop back to menu)
//        UART2_Print("\r\nError: Invalid Application Image Stack Pointer!\r\n");
//    }
//}

typedef void (*pFunction)(void);

void Jump_To_Application(uint32_t app_address)
{
    // 1. Fetch the Stack Pointer from the application's vector table
    uint32_t msp_value = *(volatile uint32_t *)app_address;
    UART2_Print("\r\nFound MSP: ");
    UART2_PrintHex(msp_value);

    UART2_Print("\r\nReset vector: ");
    UART2_PrintHex(*(volatile uint32_t *)(app_address + 4));

    // STM32F401RE RAM boundaries: 96KB starting at 0x20000000 to 0x20018000
    #define RAM_START  0x20000000U
    #define RAM_SIZE   (128 * 1024U)
    #define RAM_END    (RAM_START + RAM_SIZE)

    // 2. Validate that the stack pointer points inside valid internal RAM
    if ((msp_value >= RAM_START) && (msp_value < RAM_END))
    {
        // 3. Fetch the Reset Handler address from the application's vector table (offset +4)
        uint32_t reset_handler_address = *(volatile uint32_t *)(app_address + 4);
        pFunction app_reset_handler = (pFunction)reset_handler_address;

        // 4. Disable all global interrupts before jumping
        __disable_irq();

        // 4a. Stop SysTick so it can't fire inside the app
        SysTick->CTRL = 0;
        SysTick->VAL  = 0;


        // 5. Clear all pending interrupts and disable all NVIC channels left by bootloader
        for (int i = 0; i < 8; i++)
        {
            NVIC->ICER[i] = 0xFFFFFFFF; // Disable interrupt sources
            NVIC->ICPR[i] = 0xFFFFFFFF; // Clear pending flags
        }

        // 6. Set the Vector Table Offset Register (VTOR) to the application address
        SCB->VTOR = app_address;

        // 7. Set the Main Stack Pointer (MSP) to the application's stack value
        __set_MSP(msp_value);

        // 8. Jump to the application's Reset Handler
        app_reset_handler();
    }
    else
    {
        // Handle invalid app image error (e.g., flash is blank or address is wrong)
        UART2_Print("\r\nError: Invalid Application Image Stack Pointer!\r\n");
    }
}
