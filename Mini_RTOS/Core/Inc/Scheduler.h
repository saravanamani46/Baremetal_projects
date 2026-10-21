/*
 * Scheduler.h
 *
 *  Created on: 19-Oct-2026
 *      Author: HP
 */

#ifndef INC_SCHEDULER_H_
#define INC_SCHEDULER_H_

#include "TCB.h"

/*Pointer to the currently running task*/
extern TCB_t * volatile currentTCB;

/*Initialize the scheduler*/
void Scheduler_Init(void);

/*Select the next READY task*/
TCB_t *Scheduler_GetNextTask(void);

/*Scheduler entry point*/
void Scheduler_Run(void);

#endif /* INC_SCHEDULER_H_ */
