/*******************************************************************************
  MPLAB Harmony Application Source File
  
  Company:
    Microchip Technology Inc.
  
  File Name:
    app.c

  Summary:
    This file contains the source code for the MPLAB Harmony application.

  Description:
    This file contains the source code for the MPLAB Harmony application.  It 
    implements the logic of the application's state machine and it may call 
    API routines of other MPLAB Harmony modules in the system, such as drivers,
    system services, and middleware.  However, it does not call any of the
    system interfaces (such as the "Initialize" and "Tasks" functions) of any of
    the modules in the system or make any assumptions about when those functions
    are called.  That is the responsibility of the configuration-specific system
    files.
 *******************************************************************************/

// DOM-IGNORE-BEGIN
/*******************************************************************************
Copyright (c) 2017 released Microchip Technology Inc.  All rights reserved.

Microchip licenses to you the right to use, modify, copy and distribute
Software only when embedded on a Microchip microcontroller or digital signal
controller that is integrated into your product or third party product
(pursuant to the sublicense terms in the accompanying license agreement).

You should refer to the license agreement accompanying this Software for
additional information regarding your rights and obligations.

SOFTWARE AND DOCUMENTATION ARE PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND,
EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION, ANY WARRANTY OF
MERCHANTABILITY, TITLE, NON-INFRINGEMENT AND FITNESS FOR A PARTICULAR PURPOSE.
IN NO EVENT SHALL MICROCHIP OR ITS LICENSORS BE LIABLE OR OBLIGATED UNDER
CONTRACT, NEGLIGENCE, STRICT LIABILITY, CONTRIBUTION, BREACH OF WARRANTY, OR
OTHER LEGAL EQUITABLE THEORY ANY DIRECT OR INDIRECT DAMAGES OR EXPENSES
INCLUDING BUT NOT LIMITED TO ANY INCIDENTAL, SPECIAL, INDIRECT, PUNITIVE OR
CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST DATA, COST OF PROCUREMENT OF
SUBSTITUTE GOODS, TECHNOLOGY, SERVICES, OR ANY CLAIMS BY THIRD PARTIES
(INCLUDING BUT NOT LIMITED TO ANY DEFENSE THEREOF), OR OTHER SIMILAR COSTS.
 *******************************************************************************/
// DOM-IGNORE-END


// *****************************************************************************
// *****************************************************************************
// Section: Included Files 
// *****************************************************************************
// *****************************************************************************

#include "app_glcd.h"
#include "definitions.h"
#include "system/input/sys_input.h"
#include "gfx/driver/gfx_driver.h"

#define APP_GFX_LAYER_WIDTH_PIXELS  480
#define APP_GFX_LAYER_HEIGHT_PIXELS 272

// *****************************************************************************
// *****************************************************************************
// Section: Global Data Definitions
// *****************************************************************************
// *****************************************************************************
extern const uint8_t StartWithBlankText_data[32000];
extern const uint8_t NewHarmonyLogo_data[41280];
// *****************************************************************************
/* Application Data

  Summary:
    Holds application data

  Description:
    This structure holds the application's data.

  Remarks:
    This structure should be initialized by the APP_Initialize function.
    
    Application strings and buffers are be defined outside this structure.
*/

APP_GLCD_DATA appData;
gfxPixelBuffer imageBuffer;
gfxPixelBuffer textBuffer;

// *****************************************************************************
// *****************************************************************************
// Section: Application Callback Functions
// *****************************************************************************
// *****************************************************************************

/* TODO:  Add any necessary callback functions.
*/

// *****************************************************************************
// *****************************************************************************
// Section: Application Local Functions
// *****************************************************************************
// *****************************************************************************


/* TODO:  Add any necessary local functions.
*/


// *****************************************************************************
// *****************************************************************************
// Section: Application Initialization and State Machine Functions
// *****************************************************************************
// *****************************************************************************

/*******************************************************************************
  Function:
    void APP_Initialize ( void )

  Summary:
    Maintains state machine of APP.
 
 */

void APP_GLCD_Initialize ( void )
{
    /* Place the App state machine in its initial state. */
    appData.state = APP_STATE_INIT;

    
    /* TODO: Initialize your application's state machine and other
     * parameters.
     */
}

/*******************************************************************************
  Function:
    void APP_Initialize ( void )

 */
static void APP_PaintFrameWithBuffer(
                    gfxPixelBuffer* buf,
                    int x,
                    int y)
{
    gfxPixelBuffer* frameBuff;
    gfxBuffer pixel;
    gfxColor clr;
    gfxPoint pnt;
    int i, j;
    
    frameBuff = gfxDriverInterface.getFrameBuffer(0);

    for (i = 0; i < buf->size.height; i++) 
    {
        for (j = 0; j < buf->size.width; j++) 
        {
            pnt.x = j;
            pnt.y = i;
            
            clr = gfxColorConvert(buf->mode,
                                  frameBuff->mode,
                                  gfxPixelBufferGet(buf, pnt.x, pnt.y));
            
            pixel = gfxPixelBufferOffsetGet(frameBuff, pnt.x + x, pnt.y + y);
            
            memcpy(pixel, &clr, gfxColorInfoTable[frameBuff->mode].size);
        }
    }
}

static void APP_PaintFrameWithColor(
                    uint16_t color,
                    uint32_t x,
                    uint32_t y,
                    uint32_t width,
                    uint32_t height)
{
    gfxPixelBuffer * frameBuff;
    
    frameBuff = gfxDriverInterface.getFrameBuffer(0);
    memset(frameBuff->pixels, color, APP_GFX_LAYER_WIDTH_PIXELS * APP_GFX_LAYER_HEIGHT_PIXELS * 2);
}


/******************************************************************************
  Function:
    void APP_Tasks ( void )

  Remarks:
    See prototype in app.h.
 */

void APP_GLCD_Tasks ( void )
{

    /* Check the application's current state. */
    switch ( appData.state )
    {
        /* Application's initial state. */
        case APP_STATE_INIT:
        {
            bool appInitialized = true;
                       
            gfxPixelBufferCreate(172,
                    120,
                    GFX_COLOR_MODE_RGB_565,
                    NewHarmonyLogo_data,
                    &imageBuffer);
            
            gfxPixelBufferCreate(240,
                    40,
                    GFX_COLOR_MODE_RGB_565,
                    StartWithBlankText_data,
                    &textBuffer);
                
            if (appData.state != APP_STATE_ERROR)
            {
                if (appInitialized)
                {

                    appData.state = APP_STATE_PAINT;
                }
            }
            break;
        }

        case APP_STATE_PAINT:
        {
            //Paint Layer 0 with white
            APP_PaintFrameWithColor(
                    0xFF,
                    0,
                    0,
                    APP_GFX_LAYER_WIDTH_PIXELS,
                    APP_GFX_LAYER_HEIGHT_PIXELS);
            
            //Draw Harmony Logo on Layer 1
            APP_PaintFrameWithBuffer(&imageBuffer, 151, 50);
            
            appData.state = APP_STATE_PROCESS;

            //Draw Text on Layer 2
            APP_PaintFrameWithBuffer(&textBuffer, 115, 200);
            
            appData.state = APP_STATE_PROCESS;
            
            break;
        }

        case APP_STATE_PROCESS:
        {
            //Do application processing
            
            break;
        }
        case APP_STATE_ERROR:
        /* The default state should never be executed. */
        default:
        {
            LED1_On();
            //Light the LED for error
            break;
        }
    }
}

/*******************************************************************************
 End of File
 */
