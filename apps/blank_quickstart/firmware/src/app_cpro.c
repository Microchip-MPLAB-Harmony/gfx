/*******************************************************************************
  MPLAB Harmony Application Source File

  Company:
    Microchip Technology Inc.

  File Name:
    app_cpro.c

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

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include "app_cpro.h"
#include "definitions.h"
#include "gfx/driver/gfx_driver.h"

#define APP_GFX_LAYER_WIDTH_PIXELS  320
#define APP_GFX_LAYER_HEIGHT_PIXELS 480

// *****************************************************************************
// *****************************************************************************
// Section: Global Data Definitions
// *****************************************************************************
// *****************************************************************************
extern const uint8_t StartWithBlankText_data[19000];
extern const uint8_t NewHarmonyLogo_data[57800];

// *****************************************************************************
/* Application Data

  Summary:
    Holds application data

  Description:
    This structure holds the application's data.

  Remarks:
    This structure should be initialized by the APP_CPRO_Initialize function.

    Application strings and buffers are be defined outside this structure.
*/

APP_CPRO_DATA app_cproData;
#define APP_GFX_BYTES_PER_PIXEL 2
static uint8_t ScratchPixels[APP_GFX_LAYER_WIDTH_PIXELS * 50 * APP_GFX_BYTES_PER_PIXEL];
gfxPixelBuffer scratchPixels;
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
    void APP_16_PORTRAIT_Initialize ( void )

  Remarks:
    See prototype in app_cproData.h.
 */

void APP_CPRO_Initialize ( void )
{
    /* Place the App state machine in its initial state. */
    app_cproData.state = APP_CPRO_STATE_INIT;



    /* TODO: Initialize your application's state machine and other
     * parameters.
     */
}

/*******************************************************************************
  Function:
    void APP_Initialize ( void )

 */
static void APP_PaintFrameWithBuffer(
                    uint16_t * buff,
                    int x,
                    int y,
                    int width,
                    int height)
{   
    // set up render buffer to match damaged rectangle size
    scratchPixels.size.width = width;
    scratchPixels.size.height = height;
    scratchPixels.pixel_count = scratchPixels.size.width * scratchPixels.size.height;
    scratchPixels.mode = GFX_COLOR_MODE_RGB_565;
    scratchPixels.pixels = buff;
    scratchPixels.buffer_length = scratchPixels.pixel_count * APP_GFX_BYTES_PER_PIXEL;
    
    ili9488DisplayDriver.blitBuffer(x, y, &scratchPixels, GFX_BLEND_NONE);

}

static void APP_PaintFrameWithColor(
                    uint16_t color,
                    uint32_t x,
                    uint32_t y,
                    uint32_t width,
                    uint32_t height)
{   
     memset(scratchPixels.pixels, color, APP_GFX_LAYER_WIDTH_PIXELS * 50 * APP_GFX_BYTES_PER_PIXEL);
     scratchPixels.size.height = 1;
     
     for (int i = y; i < height; i++)
     {
         ili9488DisplayDriver.blitBuffer(x, i, &scratchPixels, GFX_BLEND_NONE);
     }

}


/******************************************************************************
  Function:
    void APP_CPRO_Tasks ( void )

  Remarks:
    See prototype in app_cproData.h.
 */

void APP_CPRO_Tasks ( void )
{

    /* Check the application's current state. */
    switch ( app_cproData.state )
    {
        /* Application's initial state. */
        case APP_CPRO_STATE_INIT:
        {
            bool appInitialized = true;

            gfxPixelBufferCreate(APP_GFX_LAYER_WIDTH_PIXELS,
                50,
                GFX_COLOR_MODE_RGB_565,
                &ScratchPixels[0],
                &scratchPixels);
            
            if (app_cproData.state != APP_CPRO_STATE_ERROR)
            {
                if (appInitialized)
                {
                    app_cproData.state = APP_CPRO_STATE_PAINT;
                }
            }
            break;
        }

        case APP_CPRO_STATE_PAINT:
        {
            //Paint Layer 0 with white
            APP_PaintFrameWithColor(
                    0xFFFF,
                    0,
                    0,
                    APP_GFX_LAYER_WIDTH_PIXELS,
                    APP_GFX_LAYER_HEIGHT_PIXELS);

            //Draw Harmony Logo on Layer 1
            APP_PaintFrameWithBuffer(
                    (uint16_t *) NewHarmonyLogo_data,
                    76,
                    123,
                    170,
                    170);
                       
            //Draw Text on Layer 2
            APP_PaintFrameWithBuffer(
                    (uint16_t *) StartWithBlankText_data,
                    65,
                    350,
                    190,
                    50);
            
            app_cproData.state = APP_CPRO_STATE_PROCESS;

            break;
        }

        case APP_CPRO_STATE_PROCESS:
        {
            //Do application processing

            break;
        }
        case APP_CPRO_STATE_ERROR:
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
