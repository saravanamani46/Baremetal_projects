/*
 * ContextSwitch.S
 *
 *  Created on: 20-Oct-2026
 *      Author: HP
 */
.syntax unified
.cpu cortex-m4
.thumb

.global PendSV_Handler
.type PendSV_Handler, %function

.extern currentTCB
.extern Scheduler_GetNextTask

PendSV_Handler:

    /* Save software-managed registers of the outgoing task */
    MRS     R0, PSP
    STMDB   R0!, {R4-R11}

    /* Save the outgoing task's stack pointer */
    LDR     R1, =currentTCB
    LDR     R2, [R1]
    CBZ     R2, PendSV_NoOutgoingTask
    STR     R0, [R2]

PendSV_NoOutgoingTask:

    /* Preserve EXC_RETURN across the C function call */
    PUSH    {R3, LR}

    /* Select the next task */
    BL      Scheduler_GetNextTask

    /* Restore EXC_RETURN */
    POP     {R3, LR}

    /* Do not restore a null task */
    CBZ     R0, PendSV_NoReadyTask

    /* Set currentTCB to the selected task */
    LDR     R1, =currentTCB
    STR     R0, [R1]

    /* Load its saved stack pointer */
    LDR     R0, [R0]

    /* Restore software-managed registers */
    LDMIA   R0!, {R4-R11}

    /* Update PSP for the incoming task */
    MSR     PSP, R0
    ISB

PendSV_NoReadyTask:
    BX      LR

.size PendSV_Handler, .-PendSV_Handler
