#include "stm32f401xe.h"
#include <stdio.h>
#include "Task.h"
#include "Blinky.h"
#include "Timer.h"
#include "UART.h"

volatile uint32_t counter1 = 0;
volatile uint32_t counter2 = 0;

void Task2_Init()
{
	RCC->AHB1ENR |= (1U << 2);
	GPIOC->MODER &= ~(3U<<26);
}

void Task1()
{
	static uint32_t led_count = 0;

	while(1)
	{
		LED_toggle();
		led_count++;

		if (led_count >= 3)
		{
			led_count = 0;
			UART2_SendString("[Task1] LED task running\r\n");
		}

		for (volatile uint32_t i = 0; i < 500000U; i++)
		{
			__NOP();
		}
	}

}

void Task2(void)
{
	static unsigned int previous_state = 0;
	unsigned int current_state;
	char buffer1[50];

	UART2_SendString("[Task2] Started\r\n");



	while(1)
	{
		current_state = (GPIOC->IDR & (1U<<13)) ? 1 : 0;

		if ((current_state == 1) && (previous_state == 0))
		{
			counter1++;
			UART2_SendString("[Task2] Button pressed\r\n");
			sprintf(buffer1, "Value of counter1 = %lu\r\n", (unsigned long)counter1);
			UART2_SendString(buffer1);
		}

		previous_state = current_state;


	}

}

void Task3()
{
	static uint32_t last_increment = 0;
	char buffer2[50];

	while(1)
	{
		if ((uint32_t)(elapsed_seconds - last_increment) >= 5)
			{
				counter2++;
				last_increment = elapsed_seconds;

				UART2_SendString("[Task3] 5 second event\r\n");
				sprintf(buffer2, "Value of counter2 = %lu\r\n", (unsigned long)counter2);
				UART2_SendString(buffer2);

			}
	}



}


//int main(void)
//{
//	UART2_Init(115200);
//
//	while(1)
//	{
//		UART2_SendString("UART is working!\r\n");
//
//		for(volatile uint32_t i=0;i<3000000;i++)
//		{
//			__NOP();
//		}
//	}
//}
