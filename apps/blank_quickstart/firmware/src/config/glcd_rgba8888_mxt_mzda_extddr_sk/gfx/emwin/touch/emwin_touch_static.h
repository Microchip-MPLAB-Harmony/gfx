// DOM-IGNORE-BEGIN
/*******************************************************************************
* Copyright (C) 2020 Microchip Technology Inc. and its subsidiaries.
*
* Subject to your compliance with these terms, you may use Microchip software
* and any derivatives exclusively with Microchip products. It is your
* responsibility to comply with third party license terms applicable to your
* use of third party software (including open source software) that may
* accompany Microchip software.
*
* THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
* EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
* WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
* PARTICULAR PURPOSE.
*
* IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
* INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
* WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
* BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
* FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
* ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
* THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
*******************************************************************************/
// DOM-IGNORE-END

/*******************************************************************************
  emWin Touch Wrapper Header File

  Company:
    Microchip Technology Inc.

  File Name:
    emwin_touch_static.h

  Summary:
    This header file provides prototypes and definitions for emWin Touch Wrapper.

  Description:

*******************************************************************************/


#ifndef _EMWIN_TOUCH_STATIC_H_
#define _EMWIN_TOUCH_STATIC_H_

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include "definitions.h"
#include "gfx/emwin/gui.h"

// DOM-IGNORE-BEGIN
#ifdef __cplusplus  // Provide C++ Compatibility

extern "C" {

#endif
// DOM-IGNORE-END 

// *****************************************************************************
// *****************************************************************************
// Section: Type Definitions
// *****************************************************************************
// ***************************************************************************** 

// *****************************************************************************
/* 

  Summary:

  Description:

  Remarks:

 */
typedef enum
{
    /* 0 degree */
    EMWIN_TOUCH_ORIENTATION_0_DEGREE = 0,

    /* 90 degree */
    EMWIN_TOUCH_ORIENTATION_90_DEGREE,

    /* 180 degree */
    EMWIN_TOUCH_ORIENTATION_180_DEGREE,

    /* 270 degree */
    EMWIN_TOUCH_ORIENTATION_270_DEGREE,
    
} EMWIN_TOUCH_ORIENTATION;

// *****************************************************************************
/* 

  Summary:

  Description:

  Remarks:

 */

typedef struct
{


} EMWIN_TOUCH_INIT;

// *****************************************************************************
// *****************************************************************************
// Section: Application Initialization and State Machine Functions
// *****************************************************************************
// *****************************************************************************

/*******************************************************************************
  Function:

  Summary:

  Description:

  Precondition:

  Parameters:

  Returns:

  Example:
    <code>
    </code>

  Remarks:

 */
   void emWin_TouchInitialize( );
/*******************************************************************************
  Function:

  Summary:

  Description:

  Precondition:

  Parameters:

  Returns:

  Example:
    <code>
    </code>

  Remarks:

 */

void emWin_TouchLayerIndexSet( uint32_t layerIndex );

/*******************************************************************************
  Function:

  Summary:

  Description:

  Precondition:

  Parameters:

  Returns:

  Example:
    <code>
    </code>

  Remarks:

 */

void emWin_TouchOrientationSet( EMWIN_TOUCH_ORIENTATION orientation );

/*******************************************************************************
  Function:

  Summary:

  Description:

  Precondition:

  Parameters:

  Returns:

  Example:
    <code>
    </code>

  Remarks:

 */

void emWin_TouchResolutionSet( uint32_t displayWidth, uint32_t displayHeight );



/*******************************************************************************
  Function:

  Summary:

  Description:

  Precondition:

  Parameters:

  Returns:

  Example:
    <code>
    </code>

  Remarks:

 */
void emWin_TouchTasks(void);

//DOM-IGNORE-BEGIN
#ifdef __cplusplus
}
#endif
//DOM-IGNORE-END

#endif // _EMWIN_TOUCH_STATIC_H_

/*******************************************************************************
 End of File
 */

