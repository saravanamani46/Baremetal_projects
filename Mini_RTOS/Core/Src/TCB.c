/*
 * TCB.c
 *
 *  Created on: 20-Oct-2026
 *      Author: HP
 */


#include "TCB.h"
#include<stdint.h>

/*Stack size: 1024 bytes per task*/
#define TASK_STACK_SIZE_WORDS 256

/*Allocate independent stacks in RAM*/
static uint32_t task1Stack[TASK_STACK_SIZE_WORDS];
static uint32_t task2Stack[TASK_STACK_SIZE_WORDS];
static uint32_t task3Stack[TASK_STACK_SIZE_WORDS];

/*Create the three task control block*/
TCB_t task1_TCB;
TCB_t task2_TCB;
TCB_t task3_TCB;

/*Task functions defined in your task source file*/
extern void Task1(void);
extern void Task2(void);
extern void Task3(void);

/*Used if a task function returns*/
static void TaskExitError(void)
{
	while(1)
	{

	}
}

static uint32_t *Task_StackInit(uint32_t *stack, void(*taskFunction)(void))
{
	uint32_t *sp = &stack[TASK_STACK_SIZE_WORDS];

	/* Hardware-stacked frame */
	    *(--sp) = 0x01000000U;              /* xPSR: Thumb state */
	    *(--sp) = (uint32_t)(uintptr_t)taskFunction;   /* PC: task entry */
	    *(--sp) = (uint32_t)(uintptr_t)TaskExitError;  /* LR: task return */
	    *(--sp) = 0x12121212U;              /* R12 */
	    *(--sp) = 0x03030303U;              /* R3 */
	    *(--sp) = 0x02020202U;              /* R2 */
	    *(--sp) = 0x01010101U;              /* R1 */
	    *(--sp) = 0x00000000U;              /* R0 */

	    /* Software-stacked frame: R4 through R11 */
	    *(--sp) = 0x11111111U;              /* R11 */
	    *(--sp) = 0x10101010U;              /* R10 */
	    *(--sp) = 0x09090909U;              /* R9 */
	    *(--sp) = 0x08080808U;              /* R8 */
	    *(--sp) = 0x07070707U;              /* R7 */
	    *(--sp) = 0x06060606U;              /* R6 */
	    *(--sp) = 0x05050505U;              /* R5 */
	    *(--sp) = 0x04040404U;              /* R4 */

	    return sp;
}

void TCB_Init(void)
{
	/*Task 1*/
	task1_TCB.stackPointer = Task_StackInit(task1Stack, Task1);
	task1_TCB.state = TASK_READY;

	/*Task 2*/
	task2_TCB.stackPointer = Task_StackInit(task2Stack, Task2);
	task2_TCB.state = TASK_READY;

	/*Task 3*/
	task3_TCB.stackPointer = Task_StackInit(task3Stack, Task3);
	task3_TCB.state = TASK_READY;
}
