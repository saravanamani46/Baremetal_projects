#include "stm32f401xe.h"
#include "Task.h"
#include "Scheduler.h"

/*Three tasks in our mini RTOS*/
#define NUMBER_OF_TASKS 3

/*Currently running task*/
TCB_t * volatile currentTCB = 0;

/*Current position in the round-robin list */
static uint32_t current_task = 0;

/*List of task control blocks*/
static TCB_t *taskList[NUMBER_OF_TASKS] =
{
		&task1_TCB,
		&task2_TCB,
		&task3_TCB
};

void Scheduler_Init(void)
{
	current_task = 0;
	currentTCB  = 0;

	task1_TCB.state = TASK_READY;
	task2_TCB.state = TASK_READY;
	task3_TCB.state = TASK_READY;
}

TCB_t *Scheduler_GetNextTask(void)
{
    uint32_t checked_tasks;

    /* Make the outgoing task ready for the next scheduling round */
    if (currentTCB != 0 &&
        currentTCB->state == TASK_RUNNING)
    {
        currentTCB->state = TASK_READY;
    }

    for (checked_tasks = 0;
         checked_tasks < NUMBER_OF_TASKS;
         checked_tasks++)
    {
        if (taskList[current_task]->state == TASK_READY)
        {
            taskList[current_task]->state = TASK_RUNNING;

            TCB_t *next_task = taskList[current_task];

            current_task++;

            if (current_task >= NUMBER_OF_TASKS)
            {
                current_task = 0;
            }

            return next_task;
        }

        current_task++;

        if (current_task >= NUMBER_OF_TASKS)
        {
            current_task = 0;
        }
    }

    return 0;
}

void Scheduler_Run(void)
{
	TCB_t *next_task;

	next_task = Scheduler_GetNextTask();

	if (next_task != 0)
	{

	}


}
