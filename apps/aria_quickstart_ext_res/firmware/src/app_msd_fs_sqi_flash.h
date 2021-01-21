/*******************************************************************************
  MPLAB Harmony Application Header File

  Company:
    Microchip Technology Inc.

  File Name:
    app_msd_fs_sqi_flash.h

  Summary:
    This header file provides prototypes and definitions for the application.

  Description:
    This header file provides function prototypes and data type definitions for
    the application.  Some of these are required by the system (such as the
    "APP_MSD_FS_SQI_FLASH_Initialize" and "APP_MSD_FS_SQI_FLASH_Tasks" prototypes) and some of them are only used
    internally by the application (such as the "APP_MSD_FS_SQI_FLASH_STATES" definition).  Both
    are defined here for convenience.
*******************************************************************************/

#ifndef _APP_MSD_FS_SQI_FLASH_H
#define _APP_MSD_FS_SQI_FLASH_H

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "configuration.h"
#include "definitions.h"

#include "usb/usb_chapter_9.h"
#include "usb/usb_device.h"
#include "gfx/libaria/libaria.h"

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
/* USB Task states

  Summary:
    USB Task states enumeration

  Description:
    This enumeration defines the valid USB Task states. These states
    determine the behavior of the USB Task at various times.
*/

typedef enum
{
    /* Initial state. */
    APP_USB_STATE_INIT = 0,

    /* Device Attach state */
    APP_USB_STATE_DEVICE_ATTACH,

    /* Running state */
    APP_USB_STATE_RUNNING,

    /* Idle state */
    APP_USB_STATE_IDLE

} APP_USB_STATES;

// *****************************************************************************
/* Application states

  Summary:
    Application states enumeration

  Description:
    This enumeration defines the valid application states.  These states
    determine the behavior of the application at various times.
*/

typedef enum
{
    /* Application's state machine's initial state. */
    APP_STATE_INIT=0,
    APP_STATE_USB_INIT,
    APP_STATE_MOUNT_DISK,
    APP_STATE_READ_FILES,
    APP_STATE_UNMOUNT_DISK,
    APP_STATE_WAIT_TO_RESAMPLE,
    APP_STATE_IDLE,
    APP_STATE_ERROR

} APP_STATES;


// *****************************************************************************
/* Application Data

  Summary:
    Holds application data

  Description:
    This structure holds the application's data.

  Remarks:
    Application strings and buffers are be defined outside this structure.
 */

typedef struct
{
    /* The application's current state */
    APP_STATES state;

    /* USB Sub task's current state */
    APP_USB_STATES usbState;

    /* USB Device Handle */
    USB_DEVICE_HANDLE usbDeviceHandle;

    /* Flag to track the USB Connect Status */
    bool isUsbConnected;
    
    /* Flag to track the File System Connect Status */
    bool isUsbConfigured;

    SYS_TIME_HANDLE timerHandle;
    
    /* SYS_FS File handle */
    SYS_FS_HANDLE fileHandle;
} APP_DATA;

// *****************************************************************************
// *****************************************************************************
// Section: Application Callback Routines
// *****************************************************************************
// *****************************************************************************

void APP_ToggleLanguage( void );

void APP_ToggleImage( void );

// *****************************************************************************
// *****************************************************************************
// Section: Application Initialization and State Machine Functions
// *****************************************************************************
// *****************************************************************************

/*******************************************************************************
  Function:
    void APP_MSD_FS_SQI_FLASH_Initialize ( void )

  Summary:
     MPLAB Harmony application initialization routine.

  Description:
    This function initializes the Harmony application.  It places the
    application in its initial state and prepares it to run so that its
    APP_MSD_FS_SQI_FLASH_Tasks function can be called.

  Precondition:
    All other system initialization routines should be called before calling
    this routine (in "SYS_Initialize").

  Parameters:
    None.

  Returns:
    None.

  Example:
    <code>
    APP_MSD_FS_SQI_FLASH_Initialize();
    </code>

  Remarks:
    This routine must be called from the SYS_Initialize function.
*/

void APP_MSD_FS_SQI_FLASH_Initialize ( void );


/*******************************************************************************
  Function:
    void APP_MSD_FS_SQI_FLASH_Tasks ( void )

  Summary:
    MPLAB Harmony Demo application tasks function

  Description:
    This routine is the Harmony Demo application's tasks function.  It
    defines the application's state machine and core logic.

  Precondition:
    The system and application initialization ("SYS_Initialize") should be
    called before calling this.

  Parameters:
    None.

  Returns:
    None.

  Example:
    <code>
    APP_MSD_FS_SQI_FLASH_Tasks();
    </code>

  Remarks:
    This routine must be called from SYS_Tasks() routine.
 */

void APP_MSD_FS_SQI_FLASH_Tasks( void );



#endif /* _APP_MSD_FS_SQI_FLASH_H */

//DOM-IGNORE-BEGIN
#ifdef __cplusplus
}
#endif
//DOM-IGNORE-END

/*******************************************************************************
 End of File
 */

