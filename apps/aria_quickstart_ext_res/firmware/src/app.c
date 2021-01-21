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
#include "gfx/libaria/inc/libaria_context.h"
#include "gfx/gfx_assets.h"
#include "config/lcc_rgb565_mxt_mzef_sk/gfx/libaria/inc/libaria_widget_button.h"
#include "config/lcc_rgb565_mxt_mzef_sk/gfx/libaria/libaria_init.h"

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

APP_DATA CACHE_ALIGN appData;

// *****************************************************************************
// *****************************************************************************
// Section: Application Callback Functions
// *****************************************************************************
// *****************************************************************************

GFX_Result app_externalMediaOpen(GFXU_AssetHeader* ast)
{
    appData.transferStatus = DRV_SST26_TransferStatusGet(appData.handle);
    
    if (appData.handle != DRV_HANDLE_INVALID
            && appData.transferStatus == DRV_SST26_TRANSFER_COMPLETED)
        return GFX_SUCCESS;
    
    return GFX_FAILURE;
}

GFX_Result app_externalMediaRead(GFXU_ExternalAssetReader* reader,
                                 GFXU_AssetHeader* ast,
                                 void* address,
                                 uint32_t readSize,
                                 uint8_t* destBuffer,
                                 GFXU_MediaReadRequestCallback_FnPtr cb)
{
    if(ast->dataLocation == GFXU_ASSET_LOCATION_ID_SQI)
    {
        // read from cache if available
        if(appData.sqiCacheValid == 1 &&
           appData.sqiCacheAddress <= (uint32_t)address &&
           (uint32_t)address + readSize < appData.sqiCacheAddress + (uint32_t)SQI_CACHE_SIZE)
        {
            memcpy(destBuffer, &appData.sqiCache[(uint32_t)address - appData.sqiCacheAddress], readSize);

            if(reader != NULL && cb != NULL)
                cb(reader); // indicate that the data buffer is ready

            return GFX_SUCCESS; // success tells the decoder to keep going
        }
        else
        {
            appData.destBuffer = destBuffer;
            appData.readSize = readSize;
            appData.readAddress = address;
            appData.reader = reader;
            appData.cb = cb;

            appData.sqiCacheAddress = (uint32_t)appData.readAddress;
            appData.sqiCacheAddress -= appData.sqiCacheAddress % 4;
                       
            if (DRV_SST26_Status(DRV_SST26_INDEX) == SYS_STATUS_READY)
            {
                DRV_SST26_Read(appData.handle, (uint32_t *)&(appData.sqiCache), SQI_CACHE_SIZE, appData.sqiCacheAddress);

                appData.state = APP_STATE_READ_WAIT;

                return GFX_SUCCESS; // success tells the decoder to keep going
            }
        }
    }
    return GFX_FAILURE; // failure tells the decoder to abort and move on
}

void app_externalMediaClose(GFXU_AssetHeader* ast)
{
}

void APP_ToggleLanguage( void )
{
    if (laContext_GetStringLanguage() == language_English)
    {
        laContext_SetStringLanguage(language_Chinese);
    }
    else
    {
        laContext_SetStringLanguage(language_English);
    }
}

void APP_ToggleImage( void )
{
    if (LogoButton->releasedImage == &MHGS_logo_small_raw_direct_blit)
    {
        //Show the next message
        laButtonWidget_SetText(SloganButton, laString_CreateFromID(string_DrawRLE));

        //Draw the intended from the previous message
        laButtonWidget_SetPressedImage(LogoButton, &MHGS_logo_small_raw);
        laButtonWidget_SetReleasedImage(LogoButton, &MHGS_logo_small_raw);
        laLabelWidget_SetText(ImageTypeLabelWidget, laString_CreateFromID(string_ImageIsRaw));
    }
    else if (LogoButton->releasedImage == &MHGS_logo_small_raw)
    {
        //Show the next message
        laButtonWidget_SetText(SloganButton, laString_CreateFromID(string_DrawDirectBlit));

        //Draw the intended from the previous message
        laButtonWidget_SetPressedImage(LogoButton, &MHGS_logo_small_rle);
        laButtonWidget_SetReleasedImage(LogoButton, &MHGS_logo_small_rle);
        laLabelWidget_SetText(ImageTypeLabelWidget, laString_CreateFromID(string_ImageIsRLE));
    }
    else if (LogoButton->releasedImage == &MHGS_logo_small_rle)
    {
        //Show the next message
        laButtonWidget_SetText(SloganButton, laString_CreateFromID(string_DrawJpeg));

        //Draw the intended from the previous message
        laButtonWidget_SetPressedImage(LogoButton, &MHGS_logo_small_raw_direct_blit);
        laButtonWidget_SetReleasedImage(LogoButton, &MHGS_logo_small_raw_direct_blit);
        laLabelWidget_SetText(ImageTypeLabelWidget, laString_CreateFromID(string_ImageIsDirectBlit));
    }
    else if (LogoButton->releasedImage == &MHGS_logo_small_png)
    {
        //Show the next message
        laButtonWidget_SetText(SloganButton, laString_CreateFromID(string_DrawDirectBlit));

        //Draw the intended from the previous message
        laButtonWidget_SetPressedImage(LogoButton, &MHGS_logo_small_jpeg);
        laButtonWidget_SetReleasedImage(LogoButton, &MHGS_logo_small_jpeg);
        laLabelWidget_SetText(ImageTypeLabelWidget, laString_CreateFromID(string_ImageIsJPEG));
    }
    else if (LogoButton->releasedImage == &MHGS_logo_small_jpeg)
    {
        //Show the next message
        laButtonWidget_SetText(SloganButton, laString_CreateFromID(string_DrawRaw));

        //Draw the intended from the previous message
        laButtonWidget_SetPressedImage(LogoButton, &MHGS_logo_small_raw_direct_blit);
        laButtonWidget_SetReleasedImage(LogoButton, &MHGS_logo_small_raw_direct_blit);
        laLabelWidget_SetText(ImageTypeLabelWidget, laString_CreateFromID(string_ImageIsDirectBlit));
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

    appData.sqiCacheValid = 0;
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
                appData.sqiCacheValid = 1;

                memcpy(appData.destBuffer, &appData.sqiCache[(uint32_t)appData.readAddress - appData.sqiCacheAddress], appData.readSize);
                
                // indicate that the data buffer is ready
                if(appData.reader != NULL && appData.cb != NULL)
                    appData.cb(appData.reader); 

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
