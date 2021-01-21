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
  GFX GLCD Driver Interface Declarations for Static Single Instance Driver

  Company:
    Microchip Technology Inc.

  File Name:
    gfx_canvas_api.h

  Summary:
    GFX Canvas Virtual Display Component private header file

  Description:
    The GFX Canvas provides a virtual display driver interface. 

  Remarks:
    None
*******************************************************************************/

/** \file gfx_canvas_api.h
* @brief  GFX Canvas Virtual Display definitions and functions.
*
* @details The GFX Canvas provides a virtual display driver interface.
*/

#ifndef _GFX_CANVAS_API_H    /* Guard against multiple inclusion */
#define _GFX_CANVAS_API_H

#include "gfx/driver/gfx_driver.h"

// DOM-IGNORE-BEGIN
#ifdef __cplusplus  // Provide C++ Compatibility
extern "C" {
#endif
// DOM-IGNORE-END
    
#define GFXC_COLOR_FORMAT gfxColorMode 
#define GFXC_RESULT gfxResult

// *****************************************************************************
/* Enumeration:
    GFXC_FX_TYPE

  Summary:
    Canvas effects types
*/
typedef enum 
{
    GFXC_FX_FADE,       //Fade effect
    GFXC_FX_MOVE,       //Move effect
} GFXC_FX_TYPE;

// *****************************************************************************
/* Enumeration:
    GFXC_FX_MOVE_TYPE

  Summary:
    Canvas move effects type.
*/
typedef enum 
{
    GFXC_FX_MOVE_FIXED,      //Fixed speed move
    GFXC_FX_MOVE_ACC,       //Accelerating move
    GFXC_FX_MOVE_DEC,       //Decelerating move
} GFXC_FX_MOVE_TYPE;

// *****************************************************************************
/* Enumeration:
    GFXC_FX_STATUS

  Summary:
    Canvas effects status.
*/
typedef enum 
{
    GFXC_FX_IDLE,                   //Done or idle, no active effects
    GFXC_FX_DONE = GFXC_FX_IDLE,
    GFXC_FX_START,                  //Starting effect
    GFXC_FX_RUN,                    //effect in progress
} GFXC_FX_STATUS;

typedef void (*gfxcCallback) (unsigned int canvasID,
                              GFXC_FX_TYPE effect,
                              GFXC_FX_STATUS status,
                              void * parm);

extern const gfxDisplayDriver gfxDriverInterface;

// *****************************************************************************
/* Function:
    int gfxcCreate(void);

  Summary:
    Reserves an instance of the canvas object. An instance must be available
    in the canvas list, otherwise this will fail.

  Parameters:
    None

  Returns:
    int - the index of the canvas object if successful, -1 if failed
*/
int gfxcCreate(void);

// *****************************************************************************
/* Function:
    GFXC_RESULT gfxcDestroy(unsigned int canvasID);

  Summary:
    Releases a reserved canvas object and makes it available    

  Parameters:
    canvasID - the index of the canvas object

  Returns:
    GFX_SUCCESS - success
    GFX_FAILURE - fail
*/
GFXC_RESULT gfxcDestroy(unsigned int canvasID);

// *****************************************************************************
/* Function:
    GFXC_RESULT gfxcSetPixelBuffer(unsigned int canvasID, 
                                   unsigned int width,
                                   unsigned int height,
                                   const GFXC_COLOR_FORMAT format,
                                   void * buf);

  Summary:
    Initializes the pixel/frame buffer of the canvas

  Parameters:
    canvasID - the index of the canvas object
    width - the horizontal resolution of the frame buffer in pixels
    height - the vertical resolution of the frame buffer in pixels
    format - the color format of the frame buffer in pixels
    buf - address of the frame buffer

  Returns:
    GFX_SUCCESS - success
    GFX_FAILURE - fail
*/
GFXC_RESULT gfxcSetPixelBuffer(unsigned int canvasID, 
                               unsigned int width,
                               unsigned int height,
                               const GFXC_COLOR_FORMAT format,
                               void * buf);

// *****************************************************************************
/* Function:
    GFXC_RESULT gfxcSetLayer(unsigned int canvasID, unsigned int layerID);

  Summary:
    Sets the display controller layer that will be used to show the canvas. The
    display controller must support layers for this function to work.

  Parameters:
    canvasID - the index of the canvas object
    layerID - the layer index

  Returns:
    GFX_SUCCESS - success
    GFX_FAILURE - fail
*/
GFXC_RESULT gfxcSetLayer(unsigned int canvasID, unsigned int layerID);

// *****************************************************************************
/* Function:
    int gfxcGetlayer(unsigned int canvasID);

  Summary:
    Returns the index of the layer used to display the canvas objects

  Parameters:
    canvasID - the index of the canvas object

  Returns:
    int - the index of the layer
*/
int gfxcGetlayer(unsigned int canvasID);

// *****************************************************************************
/* Function:
    int gfxcClearLayer(unsigned int canvasID);

  Summary:
    Resets/Clears the layer assigned to the canvas object.

  Parameters:
    canvasID - the index of the canvas object

  Returns:
    GFX_SUCCESS - success
    GFX_FAILURE - fail
*/
GFXC_RESULT gfxcClearLayer(unsigned int canvasID);

// *****************************************************************************
/* Function:
    GFXC_RESULT gfxcSetWindowAlpha(unsigned int canvasID, uint8_t alpha);

  Summary:
    Sets the window alpha value of the canvas. The display controller layer for 
    the canvas must support per-layer, global alpha values

  Parameters:
    canvasID - the index of the canvas object
    alpha - the alpha value

  Returns:
    GFX_SUCCESS - success
    GFX_FAILURE - fail
*/
GFXC_RESULT gfxcSetWindowAlpha(unsigned int canvasID, uint8_t alpha);

// *****************************************************************************
/* Function:
    GFXC_RESULT gfxcSetWindowPosition(unsigned int canvasID, int xpos, int ypos);

  Summary:
    Sets the window alpha value of the canvas. The display controller layer for 
    the canvas must support per-layer, positioning of the frame

  Parameters:
    canvasID - the index of the canvas object
    xpos - the x position
    ypos - the y position

  Returns:
    GFX_SUCCESS - success
    GFX_FAILURE - fail
*/
GFXC_RESULT gfxcSetWindowPosition(unsigned int canvasID, int xpos, int ypos);

// *****************************************************************************
/* Function:
    GFXC_RESULT gfxcSetWindowSize(unsigned int canvasID,
                                  unsigned int width,
                                  unsigned int height);

  Summary:
    Sets the window size for canvas. The display controller layer for 
    the canvas must support per-layer size setting. If the window is smaller
    than the canvas size, the canvas will be cropped

  Parameters:
    canvasID - the index of the canvas object
    width - the horizontal size
    height - the vertical size

  Returns:
    GFX_SUCCESS - success
    GFX_FAILURE - fail
*/
GFXC_RESULT gfxcSetWindowSize(unsigned int canvasID, unsigned int width, unsigned int height);

// *****************************************************************************
/* Function:
    GFXC_RESULT gfxcSetEffectsCallback(unsigned int canvasID, gfxcCallback cb, void * parm);

  Summary:
    Sets the callback function called to notify of effects status

  Parameters:
    canvasID - the index of the canvas object
    cb - the callback function
    parm - the callback function parameter

  Returns:
    GFX_SUCCESS - success
    GFX_FAILURE - fail
*/
GFXC_RESULT gfxcSetEffectsCallback(unsigned int canvasID, gfxcCallback cb, void * parm);

// *****************************************************************************
/* Function:
    GFXC_RESULT gfxcShowCanvas(unsigned int canvasID);

  Summary:
    Displays the canvas on the screen. The layer used to display the canvas must
    be set, otherwise it will not be shown.

  Parameters:
    canvasID - the index of the canvas object

  Returns:
    GFX_SUCCESS - success
    GFX_FAILURE - fail
*/
GFXC_RESULT gfxcShowCanvas(unsigned int canvasID);

// *****************************************************************************
/* Function:
    GFXC_RESULT gfxcHideCanvas(unsigned int canvasID);

  Summary:
    Hides the canvas

  Parameters:
    canvasID - the index of the canvas object

  Returns:
    GFX_SUCCESS - success
    GFX_FAILURE - fail
*/
GFXC_RESULT gfxcHideCanvas(unsigned int canvasID);

// *****************************************************************************
/* Function:
    GFXC_RESULT gfxcCanvasUpdate(unsigned int canvasID);

  Summary:
    Updates the assigned layer with the canvas properties

  Parameters:
    canvasID - the index of the canvas object

  Returns:
    GFX_SUCCESS - success
    GFX_FAILURE - fail
*/
GFXC_RESULT gfxcCanvasUpdate(unsigned int canvasID);

// *****************************************************************************
/* Function:
    GFXC_RESULT gfxcStartEffectFade(unsigned int canvasID,
                                uint8_t startAlpha,
                                uint8_t endAlpha,
                                uint8_t delta);

  Summary:
    Fades in/out the canvas. A valid display controller layer must be assigned 
    to the canvas, and the layer must support per-layer, global alpha blending

  Parameters:
    canvasID - the index of the canvas object
    startAlpha - the starting alpha value
    endAlpha - the ending alpha value
    delta - the step value

  Returns:
    GFX_SUCCESS - success
    GFX_FAILURE - fail
*/
GFXC_RESULT gfxcStartEffectFade(unsigned int canvasID,
                                uint8_t startAlpha,
                                uint8_t endAlpha,
                                uint8_t delta);

// *****************************************************************************
/* Function:
    GFXC_RESULT gfxcStartEffectMove(unsigned int canvasID,
                                    GFXC_FX_MOVE_TYPE type,
                                    int startX, int startY,
                                    int endX,
                                    int endY,
                                    unsigned int delta);

  Summary:
    Moves the canvas from a start position to the end position using the 
    display controller functions. The layer for the canvas must support 
    window positioning.

  Parameters:
    canvasID - the index of the canvas object
    type - the move type
    startX - the starting x position
    startY - the starting y position
    endX - the end x position
    endY - the end y position
    delta - the move delta. For FIXED moves, a small delta will move slower.
            For ACC moves, a small delta will accelerate faster.
            For DEC moves, a small delta will decelerate faster.

  Returns:
    GFX_SUCCESS - success
    GFX_FAILURE - fail
*/
GFXC_RESULT gfxcStartEffectMove(unsigned int canvasID,
                                GFXC_FX_MOVE_TYPE type,
                                int startX,
                                int startY,
                                int endX,
                                int endY,
                                unsigned int delta);

// *****************************************************************************
/* Function:
    GFXC_RESULT gfxcStopEffect(unsigned int canvasID, GFXC_FX_TYPE effect);

  Summary:
    Stops the specified effect, if active

  Parameters:
    canvasID - the index of the canvas object
    effect - the effect

  Returns:
    GFX_SUCCESS - success
    GFX_FAILURE - fail
*/
GFXC_RESULT gfxcStopEffect(unsigned int canvasID, GFXC_FX_TYPE effect);

// *****************************************************************************
/* Function:
    void GFX_CANVAS_Initialize(void);

  Summary:
    Initializes the GFX canvas framework

  Parameters:
    None

  Returns:
    None
*/
void GFX_CANVAS_Initialize(void);

// *****************************************************************************
/* Function:
    void GFX_CANVAS_Task(void);

  Summary:
    The canvas task

  Parameters:
    None

  Returns:
    None
*/
void GFX_CANVAS_Task(void);
    
    /* Provide C++ Compatibility */
#ifdef __cplusplus
}
#endif

#endif /* _GFX_CANVAS_API_H */

/* *****************************************************************************
 End of File
 */
