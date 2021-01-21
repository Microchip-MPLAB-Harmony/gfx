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

#include "app.h"
#include "system/input/sys_input.h"
#include "gfx/hal/gfx.h"

#define APP_GFX_DRIVER_ID 0
#define APP_GFX_DISPLAY_ID 0
#define APP_GFX_PROCESSOR_ID 0
#define APP_GFX_NUM_LAYERS  3
#define APP_GFX_NUM_BUFFERS_PER_LAYERS  2
#define APP_GFX_COLOR_MODE GFX_COLOR_MODE_RGBA_8888

#define APP_GFX_LAYER_WIDTH_PIXELS  480
#define APP_GFX_LAYER_HEIGHT_PIXELS 272

#define APP_GFX_LAYER_POSITION_X  0
#define APP_GFX_LAYER_POSITION_Y 0


// *****************************************************************************
// *****************************************************************************
// Section: Global Data Definitions
// *****************************************************************************
// *****************************************************************************
extern const uint8_t NewHarmonyLogo_data[41280];
extern const uint8_t StartWithBlankText_data[32000];

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

APP_DATA appData;
static GFX_Handle gfxHandle = NULL;

SYS_INP_InputListener inputListener;

static void touchDownHandler(const SYS_INP_TouchStateEvent* const evt);
static void touchUpHandler(const SYS_INP_TouchStateEvent* const evt);
static void touchMoveHandler(const SYS_INP_TouchMoveEvent* const evt);

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

void APP_Initialize ( void )
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
static void APP_PaintFrameWithBuffer(unsigned int layer_idx, 
                    unsigned int buffer_idx,
                    uint32_t * buff,
                    uint32_t x,
                    uint32_t y,
                    uint32_t width,
                    uint32_t height)
{
    APP_PIXELTYPE * frameBuff;
    uint16_t i, j;

    //Set the active layer
//    GFX_Set(GFXF_LAYER_ACTIVE, layer_idx);
    
    //Set the frame buffer address of the active layer
    GFX_Get(GFXF_LAYER_BUFFER_ADDRESS, buffer_idx, (GFX_Buffer *) & frameBuff);
    
    for (i = 0; i < height; i++) 
    {
        for (j = 0; j < width; j++) 
        {
            *(APP_PIXELTYPE *) (frameBuff + 
                            (y + i) * APP_GFX_LAYER_WIDTH_PIXELS + 
                            (x + j)) = buff[i * width + j];
        }
    }
}

static void APP_PaintFrameWithColor(unsigned int layer_idx, 
                    unsigned int buffer_idx,
                    uint32_t color,
                    uint32_t x,
                    uint32_t y,
                    uint32_t width,
                    uint32_t height)
{
    uint32_t * frameBuff;
    uint32_t i, j;

    //Set the active layer
    GFX_Set(GFXF_LAYER_ACTIVE, layer_idx);
    
    //Set the frame buffer address of the active layer
    GFX_Get(GFXF_LAYER_BUFFER_ADDRESS, buffer_idx, (GFX_Buffer *) & frameBuff);
    
    for (i = 0; i < height; i++) 
    {
        for (j = 0; j < width; j++) 
        {
            *(uint32_t *) (frameBuff + 
                            (y + i) * APP_GFX_LAYER_WIDTH_PIXELS + 
                            (x + j)) = color;
        }
    }
}


/******************************************************************************
  Function:
    void APP_Tasks ( void )

  Remarks:
    See prototype in app.h.
 */

void APP_Tasks ( void )
{

    /* Check the application's current state. */
    switch ( appData.state )
    {
        /* Application's initial state. */
        case APP_STATE_INIT:
        {
            bool appInitialized = true;
            GFX_Result res = GFX_FAILURE;
            unsigned int i = 0;
            
            inputListener.handleTouchDown = &touchDownHandler;
            inputListener.handleTouchUp = &touchUpHandler;
            inputListener.handleTouchMove = &touchMoveHandler;                
            
            //Create a GFX HAL instance
            gfxHandle = GFX_Open(APP_GFX_DRIVER_ID, 
                                 APP_GFX_DISPLAY_ID, 
                                 APP_GFX_PROCESSOR_ID, 
                                 NULL);
            if (gfxHandle == NULL)
            {
                appData.state = APP_STATE_ERROR;
                break;
            }
            
            //Initialize the HAL layers
            for (i = 0; i < APP_GFX_NUM_LAYERS; i++)
            {
                //Set active layer
                res = GFX_Set(GFXF_LAYER_ACTIVE, i);
                if (res == GFX_FAILURE)
                {
                    appData.state = APP_STATE_ERROR;
                    break;
                }

                //Set the layer position to origin
                res = GFX_Set(GFXF_LAYER_POSITION, APP_GFX_LAYER_POSITION_X, APP_GFX_LAYER_POSITION_Y); 
                if (res == GFX_FAILURE)
                {
                    appData.state = APP_STATE_ERROR;
                    break;
                }
                
                //Set the layer size to full screen
                res = GFX_Set(GFXF_LAYER_SIZE, APP_GFX_LAYER_WIDTH_PIXELS, APP_GFX_LAYER_HEIGHT_PIXELS); 
                if (res == GFX_FAILURE)
                {
                    appData.state = APP_STATE_ERROR;
                    break;
                }
                
                //Set the layer color mode
                res = GFX_Set(GFXF_COLOR_MODE, APP_GFX_COLOR_MODE);
                if (res == GFX_FAILURE)
                {
                    appData.state = APP_STATE_ERROR;
                    break;
                }       
                
                //Set the layer buffer count
                res = GFX_Set(GFXF_LAYER_BUFFER_COUNT, APP_GFX_NUM_BUFFERS_PER_LAYERS);
                if (res == GFX_FAILURE)
                {
                    appData.state = APP_STATE_ERROR;
                    break;
                }                

                //Set the layer to visible
                res = GFX_Set(GFXF_LAYER_VISIBLE, GFX_TRUE);
                if (res == GFX_FAILURE)
                {
                    appData.state = APP_STATE_ERROR;
                    break;
                }
                
                //Enable the layer
                res = GFX_Set(GFXF_LAYER_ENABLED, GFX_TRUE);
                if (res == GFX_FAILURE)
                {
                    appData.state = APP_STATE_ERROR;
                    break;
                }
                
            }
            
            SYS_INP_AddListener(&inputListener);
            
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
            APP_PaintFrameWithColor(0, 0,
                    0xffffffff,
                    0,
                    0,
                    APP_GFX_LAYER_WIDTH_PIXELS,
                    APP_GFX_LAYER_HEIGHT_PIXELS);
            
            //Draw Harmony Logo on Layer 1
            APP_PaintFrameWithBuffer(1, 0,
                    (uint32_t *) NewHarmonyLogo_data,
                    90,
                    20,
                    300,
                    180);
            
            //Draw Text on Layer 2
            APP_PaintFrameWithBuffer(2, 0,
                    (uint32_t *) StartWithBlankText_data,
                    140,
                    200,
                    200,
                    40);
            
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
            GFX_Close();
//            BSP_LED_1On();
            //Light the LED for error
            break;
        }
    }
}

void touchDownHandler(const SYS_INP_TouchStateEvent* const evt)
{
//    BSP_LED_D6On();    
}

void touchUpHandler(const SYS_INP_TouchStateEvent* const evt)
{
//    BSP_LED_D7Off();
//    BSP_LED_D6Off();
}

void touchMoveHandler(const SYS_INP_TouchMoveEvent* const evt)
{
//    BSP_LED_D7On();
}


 

/*******************************************************************************
 End of File
 */
