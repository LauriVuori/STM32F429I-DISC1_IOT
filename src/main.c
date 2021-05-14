/*
*********************************************************************************************************
*                                            LOCAL INCLUDES
*********************************************************************************************************
*/

#include "stm32f4xx_hal.h"
#include "stm32f429i_discovery.h"
#include "os.h"
#include "usart1.h"

/*
*********************************************************************************************************
*                                            LOCAL DEFINES
*********************************************************************************************************
*/

#define APP_TASK_START_STK_SIZE 128u
#define APP_TASK_START_PRIO 1u
#define UART_TRANSMIT_TASK_PRIO 12u


/*
*********************************************************************************************************
*                                           GLOBAL VARIABLES
*********************************************************************************************************
*/

static OS_TCB AppTaskStartTCB;
static CPU_STK AppTaskStartStk[APP_TASK_START_STK_SIZE];


/*
*********************************************************************************************************
*                                         FUNCTION PROTOTYPES
*********************************************************************************************************
*/

static void AppTaskStart(void *p_arg);
// void UartTransmitTask(void *p_arg);



/*
*********************************************************************************************************
*                                                MAIN
*********************************************************************************************************
*/

int main(void) {
    OS_ERR err;

    OSInit(&err);

    OSTaskCreate((OS_TCB *)&AppTaskStartTCB,
                 (CPU_CHAR *)"App Task Start",
                 (OS_TASK_PTR)AppTaskStart,
                 (void *)0,
                 (OS_PRIO)APP_TASK_START_PRIO,
                 (CPU_STK *)&AppTaskStartStk[0],
                 (CPU_STK_SIZE)APP_TASK_START_STK_SIZE / 10,
                 (CPU_STK_SIZE)APP_TASK_START_STK_SIZE,
                 (OS_MSG_QTY)5u,
                 (OS_TICK)0u,
                 (void *)0,
                 (OS_OPT)(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
                 (OS_ERR *)&err);

    OSStart(&err);
}

/*
*********************************************************************************************************
*                                              STARTUP TASK
*********************************************************************************************************
*/

static void AppTaskStart(void *p_arg) {
    OS_ERR err;

    HAL_Init();

    BSP_LED_Init(LED3);
    BSP_LED_Init(LED4);
    MX_USART1_UART_Init();



    OSTaskCreate((OS_TCB *)&UartTransmitTaskTCB,
                 (CPU_CHAR *)"Uart Transmit Task",
                 (OS_TASK_PTR)UartTransmitTask,
                 (void *)0,
                 (OS_PRIO)UART_TRANSMIT_TASK_PRIO,
                 (CPU_STK *)&UartTransmitTaskStk[0],
                 (CPU_STK_SIZE)UART_TASK_STK_SIZE / 10,
                 (CPU_STK_SIZE)UART_TASK_STK_SIZE,
                 (OS_MSG_QTY)5u,
                 (OS_TICK)0u,
                 (void *)0,
                 (OS_OPT)(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
                 (OS_ERR *)&err);
}

/*
*********************************************************************************************************
*                                                  TASKS
*********************************************************************************************************
*/



/*
*********************************************************************************************************
*                                      NON-TASK FUNCTIONS
*********************************************************************************************************
*/




