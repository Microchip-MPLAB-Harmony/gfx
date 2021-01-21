/*******************************************************************************
  MPLAB Harmony Application Source File

  Company:
    Microchip Technology Inc.

  File Name:
    app_msd_fs_sqi_flash.c

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

#include "app_msd_fs_sqi_flash.h"
#include "gfx/hal/gfx.h"
#include "gfx/gfx_assets.h"
#include "gfx/libaria/libaria_init.h"
#include "gfx/libaria/inc/libaria_utils.h"

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
    This structure should be initialized by the APP_MSD_FS_SQI_FLASH_Initialize function.

    Application strings and buffers are be defined outside this structure.
*/

#define APP_MOUNT_NAME          "/mnt/qspi"
#define APP_DEVICE_NAME         "/dev/mtda1"
#define APP_FS_TYPE             FAT

#define APP_IMAGE_FILE_PATHNAME  APP_MOUNT_NAME"/""Images.bin"
#define APP_FONT1_FILE_PATHNAME  APP_MOUNT_NAME"/""Fonts1.bin"
#define APP_FONT2_FILE_PATHNAME  APP_MOUNT_NAME"/""Fonts2.bin"

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

/******************************************************************************
  Function:
    void APP_USBDeviceEventHandler
    (
        USB_DEVICE_EVENTS event
        void * pEventData,
        uintptr_t context
    );

  Remarks:
    Handles the Device Layers Events.
*/

void APP_USBDeviceEventHandler ( USB_DEVICE_EVENT event, void * pEventData, uintptr_t context )
{
    APP_DATA * appData = (APP_DATA*)context;

    switch (event)
    {
        case USB_DEVICE_EVENT_RESET:
        case USB_DEVICE_EVENT_SUSPENDED:
        case USB_DEVICE_EVENT_DECONFIGURED:
        {
            appData->isUsbConnected = false;

            /* Device was reset or deconfigured. */
            LED1_Off(); 
            break;
        }

        case USB_DEVICE_EVENT_CONFIGURED:
        {
            /* Endpoint is configured. */
            LED1_On();

            appData->isUsbConfigured = true;
            break;
        }

        case USB_DEVICE_EVENT_POWER_DETECTED:
        {
            /* Indicate Device is attached to Host */ 
            appData->isUsbConnected = true;

            if (appData->isUsbConfigured == true)
            {
                /* VBUS is detected. Since FS is running defer the event
                 * handling to the Task Routine. */
            }
            else
            {
                /* VBUS is detected. Attach the device. */
                USB_DEVICE_Attach(appData->usbDeviceHandle);
            }
            break;
        }

        case USB_DEVICE_EVENT_POWER_REMOVED:
        {
            appData->isUsbConnected = false;
            
            LED1_Off(); 
            /* VBUS is removed. Detach the device */
            USB_DEVICE_Detach(appData->usbDeviceHandle);
            break;
        }

            /* These events are not used in this demo */
        case USB_DEVICE_EVENT_RESUMED:
        case USB_DEVICE_EVENT_ERROR:
        case USB_DEVICE_EVENT_SOF:
        default:
        {
            break;
        }
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
            handle = SYS_FS_FileOpen(APP_FONT1_FILE_PATHNAME, (SYS_FS_FILE_OPEN_READ));
            break;
        case GFXU_ASSET_LOCATION_ID_Fonts2:
            handle = SYS_FS_FileOpen(APP_FONT2_FILE_PATHNAME, (SYS_FS_FILE_OPEN_READ));
            break;
        case GFXU_ASSET_LOCATION_ID_Images:
            handle = SYS_FS_FileOpen(APP_IMAGE_FILE_PATHNAME, (SYS_FS_FILE_OPEN_READ));
            break;
        default:
            break;
    }
    
    return handle;
}

void APP_USBInitialize ( void )
{
    /* Place the App state machine in its initial state. */
    appData.usbState = APP_USB_STATE_INIT;
    
    /* Set device layer handle as invalid */
    appData.usbDeviceHandle = USB_DEVICE_HANDLE_INVALID;
    
    /* USB is not connected */
    appData.isUsbConnected = false;
    appData.isUsbConfigured = false;
}

/******************************************************************************
  Function:
    void APP_USBTasks ( void )

  Remarks:
    See prototype in app.h.
 */
bool APP_USBTasks (void)
{
    bool done = false;
    
    /* Check the application's current state. */
    switch (appData.usbState)
    {
        /* Application's initial state. */
        case APP_USB_STATE_INIT:
        {
            appData.usbDeviceHandle = USB_DEVICE_Open(USB_DEVICE_INDEX_0, DRV_IO_INTENT_READWRITE);

            if(appData.usbDeviceHandle != USB_DEVICE_HANDLE_INVALID)
            {
                /* Set the Event Handler. We will start receiving events after
                 * the handler is set */
                USB_DEVICE_EventHandlerSet(appData.usbDeviceHandle, APP_USBDeviceEventHandler, (uintptr_t)&appData);

                /* Move the application to the next state */
                appData.usbState = APP_USB_STATE_RUNNING;
            }

            break;
        }

        case APP_USB_STATE_DEVICE_ATTACH:
        {
            /* VBUS detect event handling was deferred to the task routine.
             * Attach the device. */
            if (appData.isUsbConfigured == false)
            {
                USB_DEVICE_Attach(appData.usbDeviceHandle);
                appData.usbState = APP_USB_STATE_RUNNING;
            }
            break;
        }

        case APP_USB_STATE_RUNNING:
        {
            /* The MSD Device is maintained completely by the MSD function
             * driver and does not require application intervention. So there
             * is nothing related to MSD Device to do here. */
            done = true;
            break;
        }

        /* The default state should never be executed. */
        default:
        {
            break;
        }
    }
    
    return done;
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

void APP_SysTimerCallback ( uintptr_t context)
{
    //After 5 seconds retry opening the files
    appData.state = APP_STATE_READ_FILES;
    
    if (appData.timerHandle != SYS_TIME_HANDLE_INVALID)
    {
        SYS_TIME_TimerDestroy(appData.timerHandle);        
        appData.timerHandle = SYS_TIME_HANDLE_INVALID;
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
    void APP_MSD_FS_SQI_FLASH_Initialize ( void )

  Remarks:
    See prototype in app_msd_fs_sqi_flash.h.
 */

void APP_MSD_FS_SQI_FLASH_Initialize ( void )
{
    appData.state = APP_STATE_INIT;
    appData.timerHandle = SYS_TIME_HANDLE_INVALID;

    APP_USBInitialize();
}


/******************************************************************************
  Function:
    void APP_MSD_FS_SQI_FLASH_Tasks ( void )

  Remarks:
    See prototype in app_msd_fs_sqi_flash.h.
 */

void APP_MSD_FS_SQI_FLASH_Tasks ( void )
{
    SYS_FS_HANDLE fonts1Handle = SYS_FS_HANDLE_INVALID;
    SYS_FS_HANDLE fonts2Handle = SYS_FS_HANDLE_INVALID;
    SYS_FS_HANDLE imagesHandle = SYS_FS_HANDLE_INVALID;
    
    /* Check the application's current state. */
    switch ( appData.state )
    {
        /* Application's initial state. */
        case APP_STATE_INIT:
        {
            if (laContext_IsDrawing() || laContext_GetActiveScreenIndex() != LoadingScreen_ID)
                break;

            laWidget_SetVisible((laWidget*)ButtonWidget_MainScreen, LA_FALSE);

            appData.state = APP_STATE_USB_INIT;
            break;
        }
        
        case APP_STATE_USB_INIT:
        {
		    if (APP_USBTasks () == true)
            {
                appData.state = APP_STATE_MOUNT_DISK;
            }
            break;            
        }

        case APP_STATE_MOUNT_DISK:
        {
            //Do not access the SQI if USB is connected, user may be copying files
            if (appData.isUsbConnected == true)
                break;
            
            /* Mount the disk */
            if(SYS_FS_Mount(APP_DEVICE_NAME, APP_MOUNT_NAME, APP_FS_TYPE, 0, NULL) != SYS_FS_RES_SUCCESS)
            {
                /* The disk could not be mounted. Try mounting again until
                 * the operation succeeds. */
                appData.state = APP_STATE_MOUNT_DISK;
            }
            else
            {
                /* Mount was successful. Proceed to access file system */
                appData.state = APP_STATE_READ_FILES;
            }

            break;
        }

        case APP_STATE_READ_FILES:
        {
            //Do not access the SQI if USB is connected, user may be copying files
            if (appData.isUsbConnected == true)
                break;

            if (laContext_IsDrawing() || laContext_GetActiveScreenIndex() != LoadingScreen_ID)
                break;
            
            if(SYS_FS_CurrentDriveSet(APP_MOUNT_NAME) != SYS_FS_RES_SUCCESS)
                break;

            fonts1Handle = SYS_FS_FileOpen(APP_FONT1_FILE_PATHNAME, (SYS_FS_FILE_OPEN_READ));
            fonts2Handle = SYS_FS_FileOpen(APP_FONT2_FILE_PATHNAME, (SYS_FS_FILE_OPEN_READ));
            imagesHandle = SYS_FS_FileOpen(APP_IMAGE_FILE_PATHNAME, (SYS_FS_FILE_OPEN_READ));

            //if both files exist, boot to the next screen                
            if (fonts1Handle != SYS_FS_HANDLE_INVALID
                    && fonts2Handle != SYS_FS_HANDLE_INVALID 
                    && imagesHandle != SYS_FS_HANDLE_INVALID)
            {
                laWidget_SetVisible((laWidget*)ButtonWidget_MainScreen, LA_TRUE);
            }
            else
            {
                laWidget_SetVisible((laWidget*)ButtonWidget_MainScreen, LA_FALSE);
            }
            
            SYS_FS_FileClose(fonts1Handle);
            SYS_FS_FileClose(fonts2Handle);
            SYS_FS_FileClose(imagesHandle);

            appData.state = APP_STATE_WAIT_TO_RESAMPLE;
            break;
        }
        
        case APP_STATE_WAIT_TO_RESAMPLE:
        {
            if (appData.timerHandle == SYS_TIME_HANDLE_INVALID)
            {
                appData.timerHandle = SYS_TIME_CallbackRegisterMS(APP_SysTimerCallback, 0, 10000, SYS_TIME_SINGLE );
                appData.state = APP_STATE_IDLE;
            }
            break;
        }
            
        case APP_STATE_UNMOUNT_DISK:
        {
            if (SYS_FS_Unmount(APP_MOUNT_NAME) != SYS_FS_RES_SUCCESS)
            {
                appData.state = APP_STATE_UNMOUNT_DISK;
            }
            else
            {
                appData.state = APP_STATE_MOUNT_DISK;
            }
            break;
        }

        case APP_STATE_IDLE:
        {
            if (!laContext_IsDrawing() && laContext_GetActiveScreenIndex() == LoadingScreen_ID)
            {
                if (appData.isUsbConnected == true)
                {
                    laWidget_SetVisible((laWidget*)ButtonWidget_MainScreen, LA_FALSE);
                }
            }

            break;
        }

        case APP_STATE_ERROR:
        {
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
