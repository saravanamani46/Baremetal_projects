/*
 * TCB.h
 *
 *  Created on: 20-Oct-2026
 *      Author: HP
 */

#ifndef INC_TCB_H_
#define INC_TCB_H_

#include <stdint.h>

/*Task state*/
typedef enum
{
	TASK_READY,
	TASK_RUNNING,
	TASK_BLOCKED
} TaskState_t;

typedef struct
{
	uint32_t *stackPointer;
	TaskState_t state;

}TCB_t;

extern TCB_t task1_TCB;
extern TCB_t task2_TCB;
extern TCB_t task3_TCB;

/*Initialize all task stacks and TCBs*/
void TCB_Init(void);


#endif /* INC_TCB_H_ */
