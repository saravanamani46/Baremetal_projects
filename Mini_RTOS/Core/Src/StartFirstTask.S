/*
 * StartFirstTask.S
 *
 *  Created on: 20-Oct-2026
 *      Author: HP
 */


.syntax unified
.cpu cortex-m4
.thumb

.global SVC_Handler
.type SVC_Handler, %function

.extern currentTCB

SVC_Handler:
    /* Get the currently selected task's TCB */
    LDR     R1, =currentTCB
    LDR     R1, [R1]

    /* Load its saved stack pointer */
    LDR     R0, [R1]

    /* Restore software-saved registers R4-R11 */
    LDMIA   R0!, {R4-R11}

    /* Use the task's stack as PSP */
    MSR     PSP, R0

    /* Switch Thread mode to use PSP */
    MOVS    R0, #2
    MSR     CONTROL, R0
    ISB

    /* Return from SVC into Thread mode using PSP */
    LDR     LR, =0xFFFFFFFD
    BX      LR

.size SVC_Handler, .-SVC_Handler

