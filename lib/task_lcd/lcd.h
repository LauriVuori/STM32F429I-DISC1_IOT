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

/* Global variables */

/* Global structures */

/*-------------------------------------------------------------------*
*    FUNCTION PROTOTYPES                                             *
*--------------------------------------------------------------------*/
void LCD_Init(void);
void LCD_DEBUG(uint8_t * str);
#endif /* LCD_H */

/*** end of file ***/

