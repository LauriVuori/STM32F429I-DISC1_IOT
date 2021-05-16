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
OS_SEM LcdUpdateSem;

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
 NAME:
 DESCRIPTION:
	Input:
	Output:
  Used global variables:
  Used global constants:
 REMARKS when using this function:
*********************************************************************/
void LcdTask(void *p_arg) {
    OS_ERR err;
    CPU_TS ts;

    // for uart1 message
    CPU_INT08U *txData;
    OS_MSG_SIZE msg_size;
    
    // BSP_LCD_Clear(LCD_COLOR_BLACK);
    HAL_Delay(1500);
    BSP_LCD_Clear(LCD_COLOR_WHITE);
    while (DEF_TRUE) {
        // OSSemPend((OS_SEM *)&LcdUpdateSem,
        //           (OS_TICK)0,
        //           (OS_OPT)OS_OPT_PEND_BLOCKING,
        //           (CPU_TS *)&ts,
        //           (OS_ERR *)&err);
        txData = OSTaskQPend((OS_TICK)0, //Wait for messages from UART1 task
                        (OS_OPT)OS_OPT_PEND_BLOCKING,
                        (OS_MSG_SIZE *)&msg_size,
                        (CPU_TS *)&ts,
                        (OS_ERR *)&err);
        BSP_LCD_Clear(LCD_COLOR_WHITE);
        LCD_DEBUG(txData);
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