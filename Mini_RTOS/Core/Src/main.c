#include "stm32f401xe.h"
#include "TCB.h"
#include "Timer.h"
#include "Scheduler.h"
#include "SysTick_RTOS.h"
#include "Blinky.h"
#include "UART.h"
#include "Task.h"

int main(void)
{
//	task1_TCB.stackPointer = 0;
//	task1_TCB.state = TASK_READY;
//
//	task2_TCB.stackPointer = 0;
//	task2_TCB.state = TASK_READY;
	Task2_Init();
	TIM2_Init();

	TCB_Init();

	Scheduler_Init();

	LED_Init();
	UART2_Init(115200);

	currentTCB = &task1_TCB;
	task1_TCB.state = TASK_RUNNING;

	//Enable the RTOS scheduler tick
	RTOS_SysTick_Init();


	__asm volatile ("svc 0");


	while(1)
	{
		//Scheduler_Run();
	}
}


