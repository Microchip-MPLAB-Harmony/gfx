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

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include "app.h"
#include "gfx/legato/generated/le_gen_assets.h"
#include "gfx/legato/generated/screen/le_gen_screen_screen1.h"

// *****************************************************************************
// *****************************************************************************
// Section: Global Data Definitions
// *****************************************************************************
// *****************************************************************************

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

static leTableString tableString_DrawRLE;
static leTableString tableString_DrawPaletteCompressed;
static leTableString tableString_DrawRaw;

static leTableString tableString_ImageIsPaletteCompressed;
static leTableString tableString_ImageIsRaw;
static leTableString tableString_ImageIsRLE;

APP_DATA CACHE_ALIGN appData;

// *****************************************************************************
// *****************************************************************************
// Section: Application Callback Functions
// *****************************************************************************
// *****************************************************************************

leResult leApplication_MediaOpenRequest(leStream* stream)
{
    appData.transferStatus = DRV_SST26_TransferStatusGet(appData.handle);
    
    if (appData.handle != DRV_HANDLE_INVALID
            && appData.transferStatus == DRV_SST26_TRANSFER_COMPLETED)
        return LE_SUCCESS;
    
    return LE_FAILURE;
}

leResult leApplication_MediaReadRequest(leStream* stream, // stream reader
                                        uint32_t address,  // address
                                        uint32_t readSize,  // dest size
                                        uint8_t* destBuffer)
{
    if(stream->desc->location == 1)
    {
        appData.mediaStream = stream;
        appData.destBuffer = destBuffer;
        appData.readSize = readSize;
        appData.readAddress = (void*)address;

        if (DRV_SST26_Status(DRV_SST26_INDEX) == SYS_STATUS_READY)
        {
            DRV_SST26_Read(appData.handle, (uint32_t *)(appData.destBuffer), appData.readSize, (uint32_t)appData.readAddress);

            appData.state = APP_STATE_READ_WAIT;

            return GFX_SUCCESS; // success tells the decoder to keep going
        }
    }
    return GFX_FAILURE; // failure tells the decoder to abort and move on
}

void leApplication_MediaCloseRequest(leStream* stream)
{
}

void LogoButton_OnReleased(leButtonWidget* btn)
{
    if (leGetStringLanguage() == language_English)
    {
        leSetStringLanguage(language_Chinese);
    }
    else
    {
        leSetStringLanguage(language_English);
    }
}

void SloganButton_OnReleased(leButtonWidget* btn)
{
    if (LogoButton->releasedImage == &MHGS_logo_small_compressed)
    {
        //Show the next message
        leTableString_Constructor(&tableString_DrawRLE, string_DrawRLE); 
        SloganButton->fn->setString(SloganButton, (leString*)&tableString_DrawRLE);

        //Draw the intended from the previous message
        LogoButton->fn->setPressedImage(LogoButton, &MHGS_logo_small_raw);
        LogoButton->fn->setReleasedImage(LogoButton, &MHGS_logo_small_raw);
        
        leTableString_Constructor(&tableString_ImageIsRaw, string_ImageIsRaw); 
        ImageTypeLabelWidget->fn->setString(ImageTypeLabelWidget, (leString*)&tableString_ImageIsRaw);
    }
    else if (LogoButton->releasedImage == &MHGS_logo_small_raw)
    {
        //Show the next message
        leTableString_Constructor(&tableString_DrawPaletteCompressed, string_DrawPaletteCompressed); 
        SloganButton->fn->setString(SloganButton, (leString*)&tableString_DrawPaletteCompressed);

        //Draw the intended from the previous message
        LogoButton->fn->setPressedImage(LogoButton, &MHGS_logo_small_rle);
        LogoButton->fn->setReleasedImage(LogoButton, &MHGS_logo_small_rle);

        leTableString_Constructor(&tableString_ImageIsRLE, string_ImageIsRLE); 
        ImageTypeLabelWidget->fn->setString(ImageTypeLabelWidget, (leString*)&tableString_ImageIsRLE);
    }
    else if (LogoButton->releasedImage == &MHGS_logo_small_rle)
    {
        //Show the next message
        leTableString_Constructor(&tableString_DrawRaw, string_DrawRaw); 
        SloganButton->fn->setString(SloganButton, (leString*)&tableString_DrawRaw);

        //Draw the intended from the previous message
        LogoButton->fn->setPressedImage(LogoButton, &MHGS_logo_small_compressed);
        LogoButton->fn->setReleasedImage(LogoButton, &MHGS_logo_small_compressed);

        leTableString_Constructor(&tableString_ImageIsPaletteCompressed, string_ImageIsPaletteCompressed); 
        ImageTypeLabelWidget->fn->setString(ImageTypeLabelWidget, (leString*)&tableString_ImageIsPaletteCompressed);
    }
}

// *****************************************************************************
// *****************************************************************************
// Section: Application Local Functions
// *****************************************************************************
// *****************************************************************************



// *****************************************************************************
// *****************************************************************************
// Section: Application Initialization and State Machine Functions
// *****************************************************************************
// *****************************************************************************

/*******************************************************************************
  Function:
    void APP_Initialize ( void )

  Remarks:
    See prototype in app.h.
 */

void APP_Initialize ( void )
{
    /* Place the app state machine in its initial state. */
    appData.state = APP_STATE_INIT;
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
                if (DRV_SST26_Status(DRV_SST26_INDEX) == SYS_STATUS_READY)
                {
                    appData.state = APP_INIT_READ_MEDIA;
                }
            break;
        }

		case APP_INIT_READ_MEDIA:
		{
            appData.handle = DRV_SST26_Open(DRV_SST26_INDEX, DRV_IO_INTENT_READ);

            if (appData.handle != DRV_HANDLE_INVALID)
            {
				appData.state = APP_STATE_IDLE;
            }
            break;
        }

        case APP_STATE_READ_WAIT:
        {
			appData.transferStatus = DRV_SST26_TransferStatusGet(appData.handle);

			if (appData.transferStatus == DRV_SST26_TRANSFER_COMPLETED)
            {
                // indicate that the data buffer is ready
                leStream_DataReady(appData.mediaStream);

				appData.state = APP_STATE_IDLE;
			}

            break;
        }

        case APP_STATE_IDLE:
        default:
        {
            break;
        }
    }
}


/*******************************************************************************
 End of File
 */
