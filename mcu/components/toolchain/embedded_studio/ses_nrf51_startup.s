/*****************************************************************************
 * Copyright (c) 2013 Rowley Associates Limited.                             *
 *                                                                           *
 * This file may be distributed under the terms of the License Agreement     *
 * provided with this software.                                              *
 *                                                                           *
 * THIS FILE IS PROVIDED AS IS WITH NO WARRANTY OF ANY KIND, INCLUDING THE   *
 * WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE. *
 *****************************************************************************/
.macro ISR_HANDLER name=
  .section .vectors, "ax"
  .word \name
  .section .init, "ax"
  .thumb_func
  .weak \name
\name:
1: b 1b /* endless loop */
.endm

.macro ISR_RESERVED
  .section .vectors, "ax"
  .word 0
.endm

  .syntax unified
  .global reset_handler

  .section .vectors, "ax"
  .code 16
  .global _vectors

_vectors:
  .word __stack_end__
  .word reset_handler
    ISR_HANDLER   NMI_Handler
    ISR_HANDLER   HardFault_Handler
    ISR_RESERVED                           /*Reserved */
    ISR_RESERVED                           /*Reserved */
    ISR_RESERVED                           /*Reserved */
    ISR_RESERVED                           /*Reserved */
    ISR_RESERVED                           /*Reserved */
    ISR_RESERVED                           /*Reserved */
    ISR_RESERVED                           /*Reserved */
    ISR_HANDLER   SVC_Handler
    ISR_RESERVED                           /*Reserved */
    ISR_RESERVED                           /*Reserved */
    ISR_HANDLER   PendSV_Handler
    ISR_HANDLER   SysTick_Handler
  /* External Interrupts */
    ISR_HANDLER   POWER_CLOCK_IRQHandler
    ISR_HANDLER   RADIO_IRQHandler
    ISR_HANDLER   UART0_IRQHandler
    ISR_HANDLER   SPI0_TWI0_IRQHandler
    ISR_HANDLER   SPI1_TWI1_IRQHandler
    ISR_RESERVED                           /*Reserved */
    ISR_HANDLER   GPIOTE_IRQHandler
    ISR_HANDLER   ADC_IRQHandler
    ISR_HANDLER   TIMER0_IRQHandler
    ISR_HANDLER   TIMER1_IRQHandler
    ISR_HANDLER   TIMER2_IRQHandler
    ISR_HANDLER   RTC0_IRQHandler
    ISR_HANDLER   TEMP_IRQHandler
    ISR_HANDLER   RNG_IRQHandler
    ISR_HANDLER   ECB_IRQHandler
    ISR_HANDLER   CCM_AAR_IRQHandler
    ISR_HANDLER   WDT_IRQHandler
    ISR_HANDLER   RTC1_IRQHandler
    ISR_HANDLER   QDEC_IRQHandler
    ISR_HANDLER   LPCOMP_IRQHandler
    ISR_HANDLER   SWI0_IRQHandler
    ISR_HANDLER   SWI1_IRQHandler
    ISR_HANDLER   SWI2_IRQHandler
    ISR_HANDLER   SWI3_IRQHandler
    ISR_HANDLER   SWI4_IRQHandler
    ISR_HANDLER   SWI5_IRQHandler
    ISR_RESERVED                           /*Reserved */
    ISR_RESERVED                           /*Reserved */
    ISR_RESERVED                           /*Reserved */
    ISR_RESERVED                           /*Reserved */
    ISR_RESERVED                           /*Reserved */
    ISR_RESERVED                           /*Reserved */

  .section .init, "ax"
  .thumb_func

  reset_handler:

  // turn on RAM blocks  
  .equ    NRF_POWER_RAMON_ADDRESS,             0x40000524
  .equ    NRF_POWER_RAMONB_ADDRESS,            0x40000554
  .equ    NRF_POWER_RAMONx_RAMxON_ONMODE_Msk,  0x3  
 
  /* Make sure ALL RAM banks are powered on */
  MOVS    R1, #NRF_POWER_RAMONx_RAMxON_ONMODE_Msk
  
  LDR     R0, =NRF_POWER_RAMON_ADDRESS
  LDR     R2, [R0]
  ORRS    R2, R1
  STR     R2, [R0]

  LDR     R0, =NRF_POWER_RAMONB_ADDRESS
  LDR     R2, [R0]
  ORRS    R2, R1
  STR     R2, [R0]


#ifndef __NO_SYSTEM_INIT
  ldr r0, =__SRAM_segment_end__
  mov sp, r0
  bl SystemInit
#endif

  b _start

#ifndef __NO_SYSTEM_INIT
  .thumb_func
  .weak SystemInit
SystemInit:
  bx lr
#endif

  
