/*******************************************************************************
  MPLAB Harmony Application Source File

  Company:
    Microchip Technology Inc.

  File Name:
    app_msd_host_sdcard_fs.c

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

#include "app_msd_host_sdcard_fs.h"
#include "definitions.h"

// *****************************************************************************
// *****************************************************************************
// Section: Global Data Definitions
// *****************************************************************************
// *****************************************************************************

#define USB_ID      1
#define SDCARD_ID   2

int32_t       usbDeviceConnected;
int32_t       sdcardDeviceConnected;

// *****************************************************************************
/* Application Data

  Summary:
    Holds application data

  Description:
    This structure holds the application's data.

  Remarks:
    This structure should be initialized by the APP_MSD_HOST_SDCARD_FS_Initialize function.

    Application strings and buffers are be defined outside this structure.
*/

APP_DATA appData;

// *****************************************************************************
// *****************************************************************************
// Section: Application Callback Functions
// *****************************************************************************
// *****************************************************************************

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
        laLabelWidget_SetText(LabelWidget2, laString_CreateFromID(string_DrawRLE));

        //Draw the intended from the previous message
        laButtonWidget_SetPressedImage(LogoButton, &MHGS_logo_small_raw);
        laButtonWidget_SetReleasedImage(LogoButton, &MHGS_logo_small_raw);
        laLabelWidget_SetText(ImageTypeLabelWidget, laString_CreateFromID(string_ImageIsRaw));
    }
    else if (LogoButton->releasedImage == &MHGS_logo_small_raw)
    {
        //Show the next message
        laLabelWidget_SetText(LabelWidget2, laString_CreateFromID(string_DrawJpeg));

        //Draw the intended from the previous message
        laButtonWidget_SetPressedImage(LogoButton, &MHGS_logo_small_raw_rle);
        laButtonWidget_SetReleasedImage(LogoButton, &MHGS_logo_small_raw_rle);
        laLabelWidget_SetText(ImageTypeLabelWidget, laString_CreateFromID(string_ImageIsRLE));
    }
    else if (LogoButton->releasedImage == &MHGS_logo_small_raw_rle)
    {
        //Show the next message
        laLabelWidget_SetText(LabelWidget2, laString_CreateFromID(string_DrawDirectBlit));

        //Draw the intended from the previous message
        laButtonWidget_SetPressedImage(LogoButton, &MHGS_logo_small_jpeg);
        laButtonWidget_SetReleasedImage(LogoButton, &MHGS_logo_small_jpeg);
        laLabelWidget_SetText(ImageTypeLabelWidget, laString_CreateFromID(string_ImageIsJPEG));
    }
    else if (LogoButton->releasedImage == &MHGS_logo_small_jpeg)
    {
        //Show the next message
        laLabelWidget_SetText(LabelWidget2, laString_CreateFromID(string_DrawRaw));

        //Draw the intended from the previous message
        laButtonWidget_SetPressedImage(LogoButton, &MHGS_logo_small_raw_direct_blit);
        laButtonWidget_SetReleasedImage(LogoButton, &MHGS_logo_small_raw_direct_blit);
        laLabelWidget_SetText(ImageTypeLabelWidget, laString_CreateFromID(string_ImageIsDirectBlit));
    }
}

USB_HOST_EVENT_RESPONSE APP_USBHostEventHandler (USB_HOST_EVENT event,
                                                 void* eventData,
                                                 uintptr_t context)
{
    switch (event)
    {
        case USB_HOST_EVENT_DEVICE_UNSUPPORTED:
            break;
        default:
            break;
    }
    
    return(USB_HOST_EVENT_RESPONSE_NONE);
}

static void deviceConnectionStateChanged()
{
    bool imageFileExist = false;
    bool font1FileExist = false;
    bool font2FileExist = false;
    
    SYS_FS_HANDLE fileHandle = SYS_FS_HANDLE_INVALID;
    
    if (usbDeviceConnected == 1)
    {
        SYS_FS_CurrentDriveSet(SYS_FS_MEDIA_IDX1_MOUNT_NAME_VOLUME_IDX0);

        fileHandle = SYS_FS_FileOpen(APP_FONT1_FILE_PATHNAME, (SYS_FS_FILE_OPEN_READ));
        if (fileHandle != SYS_FS_HANDLE_INVALID)
        {
            font1FileExist = true;
            appData.font1Location = APP_LOCATION_USB;
        }
        SYS_FS_FileClose(fileHandle);
        
        fileHandle = SYS_FS_FileOpen(APP_FONT2_FILE_PATHNAME, (SYS_FS_FILE_OPEN_READ));
        if (fileHandle != SYS_FS_HANDLE_INVALID)
        {
            font2FileExist = true;
            appData.font2Location = APP_LOCATION_USB;
        }
        SYS_FS_FileClose(fileHandle);
        
        fileHandle = SYS_FS_FileOpen(APP_IMAGE_FILE_PATHNAME, (SYS_FS_FILE_OPEN_READ));
        if (fileHandle != SYS_FS_HANDLE_INVALID)
        {
            imageFileExist = true;
            appData.imageLocation = APP_LOCATION_USB;
        }
        SYS_FS_FileClose(fileHandle);
    }


    if (sdcardDeviceConnected == 1)
    {
        SYS_FS_CurrentDriveSet(SYS_FS_MEDIA_IDX0_MOUNT_NAME_VOLUME_IDX0);

        fileHandle = SYS_FS_FileOpen(APP_FONT1_FILE_PATHNAME, (SYS_FS_FILE_OPEN_READ));
        if (fileHandle != SYS_FS_HANDLE_INVALID)
        {
            font1FileExist = true;
            appData.font1Location = APP_LOCATION_SDCARD;
        }
        SYS_FS_FileClose(fileHandle);
        
        fileHandle = SYS_FS_FileOpen(APP_FONT2_FILE_PATHNAME, (SYS_FS_FILE_OPEN_READ));
        if (fileHandle != SYS_FS_HANDLE_INVALID)
        {
            font2FileExist = true;
            appData.font2Location = APP_LOCATION_SDCARD;
        }
        SYS_FS_FileClose(fileHandle);
        
        fileHandle = SYS_FS_FileOpen(APP_IMAGE_FILE_PATHNAME, (SYS_FS_FILE_OPEN_READ));
        if (fileHandle != SYS_FS_HANDLE_INVALID)
        {
            imageFileExist = true;
            appData.imageLocation = APP_LOCATION_SDCARD;
        }
        SYS_FS_FileClose(fileHandle);
    }

    //if both files exist, boot to the next screen                
    if ( imageFileExist == true
            && font1FileExist == true 
            && font2FileExist == true)
    {
        laWidget_SetVisible((laWidget*)ButtonWidget_MainScreen, LA_TRUE);
    }
    else
    {
        laWidget_SetVisible((laWidget*)ButtonWidget_MainScreen, LA_FALSE);
    }
}

void APP_SYSFSEventHandler(SYS_FS_EVENT event,
                           void* eventData,
                           uintptr_t context)
{
    switch(event)
    {
        case SYS_FS_EVENT_MOUNT:
        {
            if(strcmp((const char *)eventData,SYS_FS_MEDIA_IDX1_MOUNT_NAME_VOLUME_IDX0) == 0)
            {
                usbDeviceConnected = 1;
                LED1_On();
            }
            else if(strcmp((const char *)eventData,SYS_FS_MEDIA_IDX0_MOUNT_NAME_VOLUME_IDX0) == 0)
            {
                sdcardDeviceConnected = 1;
                LED2_On();
            }
            
            if (appData.state != APP_STATE_ERROR)
            {
                deviceConnectionStateChanged();
            }
            
            break;
        }    
        case SYS_FS_EVENT_UNMOUNT:
        {
            if(strcmp((const char *)eventData, SYS_FS_MEDIA_IDX1_MOUNT_NAME_VOLUME_IDX0) == 0)
            {
                usbDeviceConnected = 0;
                LED1_Off();
            }
            else if(strcmp((const char *)eventData, SYS_FS_MEDIA_IDX0_MOUNT_NAME_VOLUME_IDX0) == 0)
            {
                sdcardDeviceConnected = 0;
                LED2_Off();
            }
            
            deviceConnectionStateChanged();
            
            break;
        }
        default:
            break;
    }
}

// *****************************************************************************
// *****************************************************************************
// Section: Application Local Functions
// *****************************************************************************
// *****************************************************************************

SYS_FS_HANDLE APP_GetFileHandle( uint32_t dataLocation )
{
    SYS_FS_HANDLE handle = SYS_FS_HANDLE_INVALID;
    
    switch(dataLocation)
    {
        case GFXU_ASSET_LOCATION_ID_Fonts1:
        {
            if (appData.font1Location == APP_LOCATION_USB)
            {
                SYS_FS_CurrentDriveSet(SYS_FS_MEDIA_IDX1_MOUNT_NAME_VOLUME_IDX0);
            }
            else if (appData.font1Location == APP_LOCATION_SDCARD)
            {
                SYS_FS_CurrentDriveSet(SYS_FS_MEDIA_IDX0_MOUNT_NAME_VOLUME_IDX0);
            }
            else
            {
                return handle;
            }

            handle = SYS_FS_FileOpen(APP_FONT1_FILE_PATHNAME, (SYS_FS_FILE_OPEN_READ));
            break;
        }
        case GFXU_ASSET_LOCATION_ID_Fonts2:
        {
            if (appData.font2Location == APP_LOCATION_USB)
            {
                SYS_FS_CurrentDriveSet(SYS_FS_MEDIA_IDX1_MOUNT_NAME_VOLUME_IDX0);
            }
            else if (appData.font2Location == APP_LOCATION_SDCARD)
            {
                SYS_FS_CurrentDriveSet(SYS_FS_MEDIA_IDX0_MOUNT_NAME_VOLUME_IDX0);
            }
            else
            {
                return handle;
            }

            handle = SYS_FS_FileOpen(APP_FONT2_FILE_PATHNAME, (SYS_FS_FILE_OPEN_READ));
            break;
        }
        case GFXU_ASSET_LOCATION_ID_Images:
        {
            if (appData.imageLocation == APP_LOCATION_USB)
            {
                SYS_FS_CurrentDriveSet(SYS_FS_MEDIA_IDX1_MOUNT_NAME_VOLUME_IDX0);
            }
            else if (appData.imageLocation == APP_LOCATION_SDCARD)
            {
                SYS_FS_CurrentDriveSet(SYS_FS_MEDIA_IDX0_MOUNT_NAME_VOLUME_IDX0);
            }
            else
            {
                return handle;
            }

            handle = SYS_FS_FileOpen(APP_IMAGE_FILE_PATHNAME, (SYS_FS_FILE_OPEN_READ));
            break;
        }
        default:
            break;
    }
    
    return handle;
}

GFX_Result app_externalMediaOpen(GFXU_AssetHeader* ast)
{
    appData.fileHandle = APP_GetFileHandle(ast->dataLocation);

    if (appData.fileHandle == SYS_FS_HANDLE_INVALID)
    {
        return GFX_FAILURE;
    }
    
    return GFX_SUCCESS;
}

GFX_Result app_externalMediaRead(GFXU_ExternalAssetReader* reader,
                                 GFXU_AssetHeader* ast,
                                 void* address,
                                 uint32_t readSize,
                                 uint8_t* destBuffer,
                                 GFXU_MediaReadRequestCallback_FnPtr cb)
{
    if (appData.fileHandle != SYS_FS_HANDLE_INVALID)
    {
        SYS_FS_FileSeek(appData.fileHandle, (uint32_t)address, SYS_FS_SEEK_SET);
        SYS_FS_FileRead(appData.fileHandle, destBuffer, readSize);
        
        if(cb != NULL && reader != NULL)
            cb(reader); // indicate that the data buffer is ready
        
        return GFX_SUCCESS;
    }
    
    return GFX_FAILURE; // failure tells the decoder to abort and move on
}

void app_externalMediaClose(GFXU_AssetHeader* ast)
{
    SYS_FS_FileClose(appData.fileHandle);
    appData.fileHandle = SYS_FS_HANDLE_INVALID;
}

// *****************************************************************************
// *****************************************************************************
// Section: Application Initialization and State Machine Functions
// *****************************************************************************
// *****************************************************************************

/*******************************************************************************
  Function:
    void APP_MSD_HOST_SDCARD_FS_Initialize ( void )

  Remarks:
    See prototype in app_msd_host_sdcard_fs.h.
 */

void APP_MSD_HOST_SDCARD_FS_Initialize ( void )
{
    /* Place the App state machine in its initial state. */
    appData.state = APP_STATE_INIT;

    usbDeviceConnected = 0;
    sdcardDeviceConnected = 0;
    
    SYS_FS_EventHandlerSet(APP_SYSFSEventHandler, USB_ID);
    USB_HOST_EventHandlerSet(&APP_USBHostEventHandler, 0);
    USB_HOST_BusEnable(0);
}


/******************************************************************************
  Function:
    void APP_MSD_HOST_SDCARD_FS_Tasks ( void )

  Remarks:
    See prototype in app_msd_host_sdcard_fs.h.
 */

void APP_MSD_HOST_SDCARD_FS_Tasks ( void )
{
    /* Check the application's current state. */
    switch ( appData.state )
    {
        /* Application's initial state. */
        case APP_STATE_INIT:
        {
            bool appInitialized = true;

            if (appInitialized)
            {
                appData.state = APP_STATE_IDLE;
            }
            break;
        }

        case APP_STATE_IDLE:
        {
            if (laContext_IsDrawing() || laContext_GetActiveScreenIndex() != LoadingScreen_ID)
                break;
            

            break;
        }

        case APP_STATE_ERROR:
        {
#ifdef LED_ON            
            LED_ON();
#else 
#ifdef RGB_LED_R_On
            RGB_LED_R_On();
#endif
#endif
            break;
        }

        /* The default state should never be executed. */
        default:
        {
            break;
        }
    }
}


/*******************************************************************************
 End of File
 */
