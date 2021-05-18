/*********************************************************************

 1.  NAME
     $usart2.c$ 

 2.  DESCRIPTION

 3.  VERSIONS
       Original:
         $Date$ / $Lauri Vuori$

       Version history:

**********************************************************************/

/*-------------------------------------------------------------------*
*    HEADER FILES                                                    *
*--------------------------------------------------------------------*/
#include "uart5.h"

/*-------------------------------------------------------------------*
*    GLOBAL VARIABLES AND CONSTANTS                                  *
*--------------------------------------------------------------------*/
/* Control flags */

/* Global constants */

/* Global variables */
OS_TCB Uart5ReceiveTaskTCB;
CPU_STK Uart5ReceiveTaskStk[UART_TASK_STK_SIZE];
CPU_INT08U rxData5[MAX_SIZE];

CPU_INT16U rxLen5;
/* Global structures */
UART_HandleTypeDef huart5;
/*-------------------------------------------------------------------*
*    FUNCTION PROTOTYPES                                             *
*--------------------------------------------------------------------*/

/*********************************************************************
*    MAIN TASK                                                       *
**********************************************************************/
/*********************************************************************
	T A S K    D E S C R I P T I O N
----------------------------------------------------------------------*/
/**
 * @fn 
 * @brief 
 * @param 
 * @return 
 */
/*********************************************************************/
void Uart5ReceiveTask(void *p_arg) {
    // OS_ERR err;

    // while (DEF_TRUE) {
    //     BSP_LED_Toggle(LED4);
    //     HAL_Delay(500);
    // }
    __HAL_UART_ENABLE_IT(&huart5, UART_IT_IDLE); //Enable UART IDLE Interrupt
    HAL_UART_Receive_IT(&huart5, rxData5, MAX_SIZE);
    OS_ERR err;
    CPU_INT08U *txData;
    OS_MSG_SIZE msg_size;
    CPU_TS ts;

    while (DEF_TRUE) {
        txData = OSTaskQPend((OS_TICK)0, //Wait for messages from UART interrupt
                             (OS_OPT)OS_OPT_PEND_BLOCKING,
                             (OS_MSG_SIZE *)&msg_size,
                             (CPU_TS *)&ts,
                             (OS_ERR *)&err);

        HAL_UART_Transmit_IT(&huart5, txData, msg_size); //Transmit back
        HAL_UART_Receive_IT(&huart5, rxData5, MAX_SIZE);  //Make ready for next interrupt


        // OSSemPost((OS_SEM *)&LcdUpdateSem,
        //           (OS_OPT)OS_OPT_POST_NONE,
        //           (OS_ERR *)&err);
        // OSTaskQPost((OS_TCB *)&LcdDebugTaskTCB,
        //             (void *)txData,
        //             (OS_MSG_SIZE)rxLen5,
        //             (OS_OPT)OS_OPT_POST_FIFO,
        //             (OS_ERR *)&err);
        LCD_DEBUG(txData, (uint8_t *)"UART5");
        HAL_Delay(1);
    }

}

/*********************************************************************
*    NON-TASK FUNCTIONS                                              *
**********************************************************************/

/*********************************************************************
	F U N C T I O N    D E S C R I P T I O N
----------------------------------------------------------------------*/
/**
 * @fn 
 * @brief 
 * @param 
 * @return 
 */
/*********************************************************************/
void MX_USART5_UART_Init(void) {
    huart5.Instance = UART5;
    huart5.Init.BaudRate = 115200;
    huart5.Init.WordLength = UART_WORDLENGTH_8B;
    huart5.Init.StopBits = UART_STOPBITS_1;
    huart5.Init.Parity = UART_PARITY_NONE;
    huart5.Init.Mode = UART_MODE_TX_RX;
    huart5.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    huart5.Init.OverSampling = UART_OVERSAMPLING_16;
    if (HAL_UART_Init(&huart5) == HAL_OK) {
        BSP_LCD_DisplayStringAtLine(4, (uint8_t *)"UART5 Done");
    }
}

void UART5_IRQHandler(void) {
    OS_ERR err;

    uint32_t isrflags = READ_REG(huart5.Instance->SR);
    uint32_t cr1its = READ_REG(huart5.Instance->CR1);

    /* IDLE interrupt detection and handler to achieve unknown size transmission */
    if (((isrflags & USART_SR_IDLE) != RESET) && ((cr1its & USART_CR1_IDLEIE))) {
        __HAL_UART_CLEAR_IDLEFLAG(&huart5);
        huart5.RxState = HAL_UART_STATE_READY;

        rxLen5 = MAX_SIZE - huart5.RxXferCount;
        OSTaskQPost((OS_TCB *)&Uart5ReceiveTaskTCB,
                    (void *)rxData5,
                    (OS_MSG_SIZE)rxLen5,
                    (OS_OPT)OS_OPT_POST_FIFO,
                    (OS_ERR *)&err);
    }

    HAL_UART_IRQHandler(&huart5);
}
