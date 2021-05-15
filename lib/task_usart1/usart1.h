/*********************************************************************

 1.  NAME
     $USART1_H$ 

 2.  DESCRIPTION

 3.  VERSIONS
       Original:
         $Date$ / $Lauri Vuori$

       Version history:

**********************************************************************/
#ifndef USART1_H
#define USART1_H
/*-------------------------------------------------------------------*
*    HEADER FILES                                                    *
*--------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include "stm32f429i_discovery.h"
#include "os.h"
/*-------------------------------------------------------------------*
*    GLOBAL VARIABLES AND CONSTANTS                                  *
*--------------------------------------------------------------------*/
/* Control flags */

/* Global constants */

#define UART_TASK_STK_SIZE 128u
#define MAX_SIZE 65535u

/* Global variables */
extern OS_TCB UartTransmitTaskTCB;
extern CPU_STK UartTransmitTaskStk[UART_TASK_STK_SIZE];

/* Global structures */

/*-------------------------------------------------------------------*
*    FUNCTION PROTOTYPES                                             *
*--------------------------------------------------------------------*/

void UartTransmitTask(void *p_arg);
void MX_USART1_UART_Init(void);
void USART1_IRQHandler(void);

#endif /* USART1_H */

/*** end of file ***/





