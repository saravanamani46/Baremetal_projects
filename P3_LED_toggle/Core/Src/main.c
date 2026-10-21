// LED is connected to:
// Port A
// Pin 5

#include <stdint.h>

//==================== Base Addresses ====================

#define PERIPH_BASE            (0x40000000UL)
#define AHB1PERIPH_OFFSET      (0x00020000UL)
#define AHB1PERIPH_BASE        (PERIPH_BASE + AHB1PERIPH_OFFSET)

#define GPIOA_OFFSET           (0x0000UL)
#define RCC_OFFSET             (0x3800UL)

#define GPIOA_BASE             (AHB1PERIPH_BASE + GPIOA_OFFSET)
#define RCC_BASE               (AHB1PERIPH_BASE + RCC_OFFSET)

//==================== Register Offsets ====================

#define RCC_AHB1ENR_OFFSET     (0x30UL)
#define GPIOA_MODER_OFFSET     (0x00UL)
#define GPIOA_ODR_OFFSET       (0x14UL)

//==================== Register Definitions ====================

#define RCC_AHB1ENR            (*(volatile uint32_t *)(RCC_BASE + RCC_AHB1ENR_OFFSET))
#define GPIOA_MODER            (*(volatile uint32_t *)(GPIOA_BASE + GPIOA_MODER_OFFSET))
#define GPIOA_ODR              (*(volatile uint32_t *)(GPIOA_BASE + GPIOA_ODR_OFFSET))

//==================== Bit Definitions ====================

#define GPIOAEN                (1U << 0)
#define PIN5                   (1U << 5)


typedef struct
{

}GPIO_TypeDef;

int main(void)
{
    // 1. Enable clock for GPIOA
    RCC_AHB1ENR |= GPIOAEN;

    // 2. Configure PA5 as General Purpose Output
    GPIOA_MODER &= ~(3U << 10);   // Clear bits 11:10
    GPIOA_MODER |=  (1U << 10);   // Set to 01 (Output)

    while (1)
    {
        // 3. Turn ON LED
        GPIOA_ODR ^= PIN5;

        for(int i=0;i<1000000;i++)
        {

        }
    }
}
