/*********************************************************************

 1.  NAME
     $USART1_H$ 

 2.  DESCRIPTION

 3.  VERSIONS
       Original:
         $Date$ / $Lauri Vuori$

       Version history:

**********************************************************************/
#ifndef USART2_H
#define USART2_H
/*-------------------------------------------------------------------*
*    HEADER FILES                                                    *
*--------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include "stm32f429i_discovery.h"
#include "stm32f429i_discovery_lcd.h"
#include "os.h"
#include "lcd.h"
/*-------------------------------------------------------------------*
*    GLOBAL VARIABLES AND CONSTANTS                                  *
*--------------------------------------------------------------------*/
/* Control flags */

/* Global constants */

#define UART_TASK_STK_SIZE 128u
#define MAX_SIZE 65535u

/* Global variables */
extern OS_TCB Uart5ReceiveTaskTCB;
extern CPU_STK Uart5ReceiveTaskStk[UART_TASK_STK_SIZE];

/* Global structures */
/*-------------------------------------------------------------------*
*    TASK PROTOTYPES                                                 *
*--------------------------------------------------------------------*/
void Uart5ReceiveTask(void *p_arg);
/*-------------------------------------------------------------------*
*    FUNCTION PROTOTYPES                                             *
*--------------------------------------------------------------------*/
void MX_USART5_UART_Init(void);
void USART5_IRQHandler(void);

#endif /* USART2_H */

/*** end of file ***/





