/*********************************************************************

 1.  NAME
     $LCD_H$ 

 2.  DESCRIPTION

 3.  VERSIONS
       Original:
         $Date$ / $Lauri Vuori$

       Version history:

**********************************************************************/
#ifndef LCD_H
#define LCD_H
/*-------------------------------------------------------------------*
*    HEADER FILES                                                    *
*--------------------------------------------------------------------*/
#include "stm32f429i_discovery_lcd.h"
#include "stm32f4xx_hal.h"
#include "os.h"
/*-------------------------------------------------------------------*
*    GLOBAL VARIABLES AND CONSTANTS                                  *
*--------------------------------------------------------------------*/
/* Control flags */

/* Global constants */
#define MAX_COLUMNS 14u
#define MAX_ROWS 12

#define LCD_TASK_STK_SIZE 128u
#define MAX_SIZE 65535u


/* Global variables */
extern OS_TCB LcdTaskTCB;
extern CPU_STK LcdTaskStk[LCD_TASK_STK_SIZE];
extern OS_TCB LcdDebugTaskTCB;
extern CPU_STK LcdDebugTaskStk[LCD_TASK_STK_SIZE];
extern OS_SEM LcdDebugUpdateSem;


/* Global variables */

/* Global structures */
/*-------------------------------------------------------------------*
*    TASK PROTOTYPES                                                 *
*--------------------------------------------------------------------*/
void LcdTask(void *p_arg); 

void LcdDebugTask(void *p_arg); 
/*-------------------------------------------------------------------*
*    FUNCTION PROTOTYPES                                             *
*--------------------------------------------------------------------*/
void LCD_Init(void);
void LCD_DEBUG(uint8_t *str, uint8_t *from);
#endif /* LCD_H */

/*** end of file ***/

