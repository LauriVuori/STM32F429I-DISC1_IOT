/*********************************************************************

 1.  NAME
     $usart1.c$ 

 2.  DESCRIPTION

 3.  VERSIONS
       Original:
         $Date$ / $Lauri Vuori$

       Version history:

**********************************************************************/

/*-------------------------------------------------------------------*
*    HEADER FILES                                                    *
*--------------------------------------------------------------------*/

#include "usart1.h"
#include "lcd.h"
#include "stm32f429i_discovery_lcd.h"

/*-------------------------------------------------------------------*
*    GLOBAL VARIABLES AND CONSTANTS                                  *
*--------------------------------------------------------------------*/
/* Control flags */

/* Global constants */

/* Global variables */

OS_TCB Uart1TransmitTaskTCB;

CPU_STK Uart1TransmitTaskStk[UART_TASK_STK_SIZE];

// UART handle Structure definition
UART_HandleTypeDef huart1;

CPU_INT08U rxData[MAX_SIZE];

CPU_INT16U rxLen;

/* Global structures */

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
 * @fn UartTransmitTask(void *p_arg)
 * @brief Wait message from UART interrupt,
 * Transmit message back. Post message to LcdDebugTaskTCB
 * @param void* Data to task
 * @return 
 */
/*********************************************************************/
void Uart1TransmitTask(void *p_arg) {
    __HAL_UART_ENABLE_IT(&huart1, UART_IT_IDLE); //Enable UART IDLE Interrupt
    HAL_UART_Receive_IT(&huart1, rxData, MAX_SIZE);
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

        HAL_UART_Transmit_IT(&huart1, txData, msg_size); //Transmit back
        HAL_UART_Receive_IT(&huart1, rxData, MAX_SIZE);  //Make ready for next interrupt


        // OSSemPost((OS_SEM *)&LcdUpdateSem,
        //           (OS_OPT)OS_OPT_POST_NONE,
        //           (OS_ERR *)&err);
        OSTaskQPost((OS_TCB *)&LcdDebugTaskTCB,
                    (void *)txData,
                    (OS_MSG_SIZE)rxLen,
                    (OS_OPT)OS_OPT_POST_FIFO,
                    (OS_ERR *)&err);
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
 * @fn MX_USART1_UART_Init
 * @brief USART1 initialization
 * @param void
 * @return void
 */
/*********************************************************************/
void MX_USART1_UART_Init(void) {
    huart1.Instance = USART1;
    huart1.Init.BaudRate = 115200;
    huart1.Init.WordLength = UART_WORDLENGTH_8B;
    huart1.Init.StopBits = UART_STOPBITS_1;
    huart1.Init.Parity = UART_PARITY_NONE;
    huart1.Init.Mode = UART_MODE_TX_RX;
    huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    huart1.Init.OverSampling = UART_OVERSAMPLING_16;
    if (HAL_UART_Init(&huart1) == HAL_OK) {
        BSP_LCD_DisplayStringAtLine(3, (uint8_t *)"UART1 Done");
    }
}

/*********************************************************************
	F U N C T I O N    D E S C R I P T I O N
----------------------------------------------------------------------*/
/**
 * @fn HAL_UART_MspInit
 * @brief This UART Callback function configures
 *  the hardware resources for UART Initialization
 * @param UART_HandleTypeDef* UART handle Structure definition 
 * @return 
 */
/*********************************************************************/
void HAL_UART_MspInit(UART_HandleTypeDef *huart) {
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    if (huart->Instance == USART1) {
        /* Peripheral clock enable */
        __HAL_RCC_USART1_CLK_ENABLE();

        __HAL_RCC_GPIOA_CLK_ENABLE();
        /**USART1 GPIO Configuration    
        PA9     ------> USART1_RX
        PA10     ------> USART1_TX 
        */
        GPIO_InitStruct.Pin = GPIO_PIN_10 | GPIO_PIN_9;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
        GPIO_InitStruct.Alternate = GPIO_AF7_USART1;
        HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

        /* USART1 interrupt Init */
        HAL_NVIC_SetPriority(USART1_IRQn, 0, 0);
        HAL_NVIC_EnableIRQ(USART1_IRQn);
    }
}

/*********************************************************************
	F U N C T I O N    D E S C R I P T I O N
----------------------------------------------------------------------*/
/**
 * @fn USART1_IRQHandler(void)
 * @brief 
 * @param void
 * @return void
 */
/*********************************************************************/
void USART1_IRQHandler(void) {
    OS_ERR err;

    uint32_t isrflags = READ_REG(huart1.Instance->SR);
    uint32_t cr1its = READ_REG(huart1.Instance->CR1);

    /* IDLE interrupt detection and handler to achieve unknown size transmission */
    if (((isrflags & USART_SR_IDLE) != RESET) && ((cr1its & USART_CR1_IDLEIE))) {
        __HAL_UART_CLEAR_IDLEFLAG(&huart1);
        huart1.RxState = HAL_UART_STATE_READY;

        rxLen = MAX_SIZE - huart1.RxXferCount;
        OSTaskQPost((OS_TCB *)&Uart1TransmitTaskTCB,
                    (void *)rxData,
                    (OS_MSG_SIZE)rxLen,
                    (OS_OPT)OS_OPT_POST_FIFO,
                    (OS_ERR *)&err);
    }

    HAL_UART_IRQHandler(&huart1);
}