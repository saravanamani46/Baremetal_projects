#include "stm32f4xx.h"
#include <stdint.h>

void UART2_Init(void)
{
    // 1. Enable GPIOA and USART2 clocks
    RCC->AHB1ENR |= (1U << 0);  // Enable GPIOA clock
    RCC->APB1ENR |= (1U << 17); // Enable USART2 clock

    // 2. Configure PA2 (TX) and PA3 (RX) for Alternate Function mode
    GPIOA->MODER &= ~((3U << (2 * 2)) | (3U << (3 * 2))); // Clear mode bits
    GPIOA->MODER |=  ((2U << (2 * 2)) | (2U << (3 * 2))); // Set to AF mode

    // 3. Set Alternate Function type to AF7 (USART2) for PA2 and PA3
    GPIOA->AFR[0] &= ~((0xFU << (4 * 2)) | (0xFU << (4 * 3)));
    GPIOA->AFR[0] |=  ((7U  << (4 * 2)) | (7U  << (4 * 3)));

    // 4. Configure Baud Rate: 115200 bps (assuming 16 MHz internal/external clock)
    // Formula: USARTDIV = F_CK / (16 * BaudRate) -> 16000000 / (16 * 115200) = 8.68 -> Mantissa = 8, Fraction = 11 (0x8B)
    USART2->BRR = 0x8B;

    // 5. Enable USART, Transmitter, and Receiver
    USART2->CR1 = (1U << 13) | (1U << 3) | (1U << 2); // UE = 1, TE = 1, RE = 1
}
// Send a single character via USART2
void UART2_SendChar(char c) {
    // Wait until Transmit Data Register Empty (TXE)
    while (!(USART2->SR & USART_SR_TXE));
    USART2->DR = (uint8_t)c;
}

// Print a full string via USART2
void UART2_Print(const char *str) {
    while (*str) {
        UART2_SendChar(*str++);
    }
}

// Receive a character with a timeout loop
int UART2_ReceiveChar(uint8_t *data, uint32_t timeout_loops) {
    volatile uint32_t timeout = timeout_loops;

    while (timeout > 0) {
        // Check if Read Data Register Not Empty (RXNE)
        if (USART2->SR & USART_SR_RXNE) {
            *data = (uint8_t)(USART2->DR & 0xFF);
            return 1; // Success
        }
        timeout--;
    }
    return 0; // Timeout expired
}
