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
    BSP_LCD_Init();
    BSP_LCD_LayerDefaultInit(LCD_BACKGROUND_LAYER, LCD_FRAME_BUFFER);
    BSP_LCD_LayerDefaultInit(LCD_FOREGROUND_LAYER, LCD_FRAME_BUFFER);
    BSP_LCD_SelectLayer(LCD_FOREGROUND_LAYER);
    BSP_LCD_DisplayOn();
    BSP_LCD_Clear(LCD_COLOR_WHITE);
    BSP_LCD_SetTextColor(LCD_COLOR_BLACK);

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