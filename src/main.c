/**********************************************************************************************************
*                                            LOCAL INCLUDES
**********************************************************************************************************/

#include "stm32f4xx_hal.h"
#include "stm32f429i_discovery.h"
#include "stm32f429i_discovery_lcd.h"
#include "stm32f4xx_hal_gpio.h"
#include "os.h"

/**********************************************************************************************************
*                                            TASK INCLUDES
**********************************************************************************************************/
#include "usart1.h"
#include "lcd.h"
#include "gpio.h"

/**********************************************************************************************************
*                                            LOCAL DEFINES
**********************************************************************************************************/

#define APP_TASK_START_STK_SIZE 128u
#define APP_TASK_START_PRIO 1u
#define LCD_TASK_PRIO 2u
#define LCD_DEBUG_TASK_PRIO 12u
#define UART_TRANSMIT_TASK_PRIO 3u


/**********************************************************************************************************
*                                           GLOBAL VARIABLES
**********************************************************************************************************/

static OS_TCB AppTaskStartTCB;
static CPU_STK AppTaskStartStk[APP_TASK_START_STK_SIZE];


/**********************************************************************************************************
*                                         FUNCTION PROTOTYPES
**********************************************************************************************************/

static void AppTaskStart(void *p_arg);
// void UartTransmitTask(void *p_arg);
static void SystemClock_Config(void);
void LCD_Init(void);
// void MX_GPIO_Init();
// void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin);



/**********************************************************************************************************
*                                                MAIN
**********************************************************************************************************/

int main(void) {
    OS_ERR err;
    OSInit(&err);

    // OSSemCreate((OS_SEM *)&LcdUpdateSem,
    //         (CPU_CHAR *)"Lcd Update Semaphore",
    //         (OS_SEM_CTR)0,
    //         (OS_ERR *)&err);

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

/**********************************************************************************************************
*                                              STARTUP TASK
**********************************************************************************************************/

static void AppTaskStart(void *p_arg) {
    OS_ERR err;

    HAL_Init();
    SystemClock_Config();

    LCD_Init();
    
    MX_USART1_UART_Init();

    MX_GPIO_Init();
    BSP_LED_Init(LED3);
    BSP_LED_Init(LED4);

    OSTaskCreate((OS_TCB *)&Uart1TransmitTaskTCB,
                 (CPU_CHAR *)"Uart1 Transmit Task",
                 (OS_TASK_PTR)Uart1TransmitTask,
                 (void *)0,
                 (OS_PRIO)UART_TRANSMIT_TASK_PRIO,
                 (CPU_STK *)&Uart1TransmitTaskStk[0],
                 (CPU_STK_SIZE)UART_TASK_STK_SIZE / 10,
                 (CPU_STK_SIZE)UART_TASK_STK_SIZE,
                 (OS_MSG_QTY)5u,
                 (OS_TICK)0u,
                 (void *)0,
                 (OS_OPT)(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
                 (OS_ERR *)&err);


    OSTaskCreate((OS_TCB *)&LcdTaskTCB,
            (CPU_CHAR *)"LCD Task",
            (OS_TASK_PTR)LcdTask,
            (void *)0,
            (OS_PRIO)LCD_TASK_PRIO,
            (CPU_STK *)&LcdTaskStk[0],
            (CPU_STK_SIZE)LCD_TASK_STK_SIZE / 10,
            (CPU_STK_SIZE)LCD_TASK_STK_SIZE,
            (OS_MSG_QTY)5u,
            (OS_TICK)0u,
            (void *)0,
            (OS_OPT)(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
            (OS_ERR *)&err);

    OSTaskCreate((OS_TCB *)&LcdDebugTaskTCB,
        (CPU_CHAR *)"LCD Debug Task",
        (OS_TASK_PTR)LcdDebugTask,
        (void *)0,
        (OS_PRIO)LCD_TASK_PRIO,
        (CPU_STK *)&LcdDebugTaskStk[0],
        (CPU_STK_SIZE)LCD_TASK_STK_SIZE / 10,
        (CPU_STK_SIZE)LCD_TASK_STK_SIZE,
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
void EXTI0_IRQHandler(void){
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_0);
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
    if (GPIO_Pin ==  GPIO_PIN_0) {
        BSP_LED_Toggle(LED3);
    }
}


void HAL_Delay(uint32_t Delay) {
    OS_ERR err;
    OSTimeDly((OS_TICK)Delay,
              (OS_OPT)OS_OPT_TIME_DLY,
              (OS_ERR *)&err);
}

void SystemClock_Config(void) {
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
    RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};

    /** Configure the main internal regulator output voltage */
    __HAL_RCC_PWR_CLK_ENABLE();
    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
    /** Initializes the CPU, AHB and APB busses clocks */
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
    RCC_OscInitStruct.HSIState = RCC_HSI_ON;
    RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
    RCC_OscInitStruct.PLL.PLLM = 8;
    RCC_OscInitStruct.PLL.PLLN = 180;
    RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
    RCC_OscInitStruct.PLL.PLLQ = 7;
    HAL_RCC_OscConfig(&RCC_OscInitStruct);

    /** Activate the Over-Drive mode */
    HAL_PWREx_EnableOverDrive();

    /** Initializes the CPU, AHB and APB busses clocks */
    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

    HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5);

    PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_LTDC;
    PeriphClkInitStruct.PLLSAI.PLLSAIN = 216;
    PeriphClkInitStruct.PLLSAI.PLLSAIR = 2;
    PeriphClkInitStruct.PLLSAIDivR = RCC_PLLSAIDIVR_2;
    HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct);
}

/**********************************************************************************************************
*                                              CODE SNIPPETS
**********************************************************************************************************/
/**********************************************************************************************************
Read gpio pin state
if (HAL_GPIO_ReadPin (GPIOA, GPIO_PIN_0)) {
    // Set The LED ON!
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_SET);
}
***********************************************************************************************************/