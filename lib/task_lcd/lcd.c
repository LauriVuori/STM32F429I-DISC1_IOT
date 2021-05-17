/*********************************************************************

    1.  NAME
        $lcd.c$ 

    2.  DESCRIPTION

    3.  VERSIONS
        Original:
            $Date$ / $Lauri Vuori$

            Version history:

**********************************************************************/

/*-------------------------------------------------------------------*
*    HEADER FILES                                                    *
*--------------------------------------------------------------------*/
#include "lcd.h"
/*-------------------------------------------------------------------*
*    GLOBAL VARIABLES AND CONSTANTS                                  *
*--------------------------------------------------------------------*/
/* Control flags */

/* Global constants */

/* Global variables */
OS_TCB LcdTaskTCB;
CPU_STK LcdTaskStk[LCD_TASK_STK_SIZE];

OS_TCB LcdDebugTaskTCB;
CPU_STK LcdDebugTaskStk[LCD_TASK_STK_SIZE];
OS_SEM LcdDebugUpdateSem;
/* Global structures */

/*-------------------------------------------------------------------*
*    FUNCTION PROTOTYPES                                             *
*--------------------------------------------------------------------*/

/*********************************************************************
*    MAIN TASK                                                       *
**********************************************************************/
/*********************************************************************
	M A I N  T A S K     D E S C R I P T I O N
---------------------------------------------------------------------
 NAME: LcdTask
 DESCRIPTION:
	Input:
	Output:
  Used global variables:
  Used global constants:
 REMARKS when using this function:
*********************************************************************/
void LcdTask(void *p_arg) {
    OS_ERR err;

    CPU_INT08U ii = 0;
    HAL_Delay(1500);
    BSP_LCD_Clear(LCD_COLOR_WHITE);
    while (DEF_TRUE) {
        if (ii % 2 == 0) {
            BSP_LCD_DisplayChar(0, 0, '.');

        }
        else {
            BSP_LCD_ClearStringLine(0);
        }
        ii++;
        OSTimeDlyHMSM((CPU_INT16U)0,
                      (CPU_INT16U)0,
                      (CPU_INT16U)0,
                      (CPU_INT32U)500u,
                      (OS_OPT)OS_OPT_TIME_HMSM_STRICT,
                      (OS_ERR *)&err);
    }
}

/*********************************************************************
	M A I N  T A S K     D E S C R I P T I O N
---------------------------------------------------------------------
 NAME: LcdDebugTask
 DESCRIPTION:
	Input:
	Output:
  Used global variables:
  Used global constants:
 REMARKS when using this function:
*********************************************************************/
void LcdDebugTask(void *p_arg) {
    OS_ERR err;
    CPU_TS ts;

    // for uart1 message
    CPU_INT08U *txData;
    OS_MSG_SIZE msg_size;
    

    while (DEF_TRUE) {
        txData = OSTaskQPend((OS_TICK)2000, //Wait for messages from UART1 task
                             (OS_OPT)OS_OPT_PEND_BLOCKING,
                             (OS_MSG_SIZE *)&msg_size,
                             (CPU_TS *)&ts,
                             (OS_ERR *)&err);
        if (txData == NULL) {
            BSP_LCD_ClearStringLine(MAX_ROWS);
            BSP_LCD_ClearStringLine(MAX_ROWS-1);
        }
        else {
            LCD_DEBUG(txData);
        }
        txData = NULL;
    }
}


/*********************************************************************
*    NON-TASK FUNCTIONS                                              *
**********************************************************************/

/*********************************************************************
	F U N C T I O N    D E S C R I P T I O N
----------------------------------------------------------------------*/
/**
 * @fn void LCD_Init(void)
 * @brief 
 * @param 
 * @return 
 */
/*********************************************************************/

void LCD_Init(void) {
    CPU_INT08U err = 0;
    BSP_LCD_Init();
    BSP_LCD_LayerDefaultInit(LCD_BACKGROUND_LAYER, LCD_FRAME_BUFFER);
    BSP_LCD_LayerDefaultInit(LCD_FOREGROUND_LAYER, LCD_FRAME_BUFFER);
    BSP_LCD_SelectLayer(LCD_FOREGROUND_LAYER);
    BSP_LCD_DisplayOn();
    BSP_LCD_Clear(LCD_COLOR_WHITE);
    BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
    if (err == 0) {
        BSP_LCD_DisplayStringAtLine(1, (uint8_t *)"LCD_Init Done");
    }
}
/*********************************************************************
	F U N C T I O N    D E S C R I P T I O N
----------------------------------------------------------------------*/
/**
 * @fn void LCD_DEBUG(uint8_t *str)
 * @brief Prints text last two lines on lcd screen.
 * @param uint8_t *str
 * @return 
 */
/*********************************************************************/
void LCD_DEBUG(uint8_t *str) {
    BSP_LCD_DisplayStringAtLine(MAX_ROWS-1, (uint8_t *)"Debug:");
    BSP_LCD_DisplayStringAtLine(MAX_ROWS, str);
}