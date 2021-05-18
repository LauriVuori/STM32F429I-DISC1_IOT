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
#include "usart5.h"

/*-------------------------------------------------------------------*
*    GLOBAL VARIABLES AND CONSTANTS                                  *
*--------------------------------------------------------------------*/
/* Control flags */

/* Global constants */

/* Global variables */
OS_TCB Uart5ReceiveTaskTCB;
CPU_STK Uart5ReceiveTaskStk[UART_TASK_STK_SIZE];
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
    OS_ERR err;

    while (DEF_TRUE) {
        BSP_LED_Toggle(LED4);
        HAL_Delay(500);
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
        BSP_LCD_DisplayStringAtLine(3, (uint8_t *)"UART5 Done");
    }
}