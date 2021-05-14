#include "stm32f4xx_hal.h"
#include "stm32f429i_discovery.h"
#include "os.h"

#define UART_TASK_STK_SIZE 128u
#define MAX_SIZE 65535u

extern OS_TCB UartTransmitTaskTCB;
extern CPU_STK UartTransmitTaskStk[UART_TASK_STK_SIZE];

void UartTransmitTask(void *p_arg);
void MX_USART1_UART_Init(void);
void USART1_IRQHandler(void);
