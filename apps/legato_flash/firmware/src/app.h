/*******************************************************************************
  MPLAB Harmony Application Header File

  Company:
    Microchip Technology Inc.

  File Name:
    app.h

  Summary:
    This header file provides prototypes and definitions for the application.

  Description:
    This header file provides function prototypes and data type definitions for
    the application.  Some of these are required by the system (such as the
    "APP_Initialize" and "APP_Tasks" prototypes) and some of them are only used
    internally by the application (such as the "APP_STATES" definition).  Both
    are defined here for convenience.
*******************************************************************************/

#ifndef _APP_H
#define _APP_H

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
#include "hexdecoder.h"
#include "driver/driver_common.h"
#include "driver/sst26/drv_sst26.h"

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

/* Erase-Write-Read One Sector of Data 4096 Bytes*/
#define BUFFER_SIZE     HEXDECODER_MAX_RECORD_SIZE
#define MEM_ADDRESS     0x0

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
	APP_STATE_INIT = 0,
    APP_INIT_WRITE_MEDIA,
    APP_INIT_GEOMETRY,
    APP_STATE_BOOT_ERASE_WAIT,
    APP_STATE_BUS_ENABLE,
    APP_STATE_WAIT_FOR_BUS_ENABLE_COMPLETE,

    APP_OPEN_FILE,
	APP_FILE_NOT_FOUND,
	APP_VALIDATE_FILE,
	APP_READ_RECORD_COUNT,
	APP_START_DECODING,
	APP_PRE_DECODE,
	APP_DECODE_RECORD,
            
    APP_STATE_WRITE_TO_SQI,
    APP_STATE_WRITE_WAIT,
    APP_STATE_VERIFY_READ,
    APP_STATE_VERIFY_WAIT,
    APP_STATE_VERIFY_DATA,
    APP_STATE_ERASE_WAIT,
	APP_STATE_ERROR,
	APP_STATE_DONE

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
    APP_STATES  state;

    bool screenShown;
    
    /* Driver Handle */
    DRV_HANDLE handle;

    /* SST26 Device Geometry */
    DRV_SST26_GEOMETRY geometry;

    /* Jedec-ID*/
    uint32_t jedec_id;

    /* Read Buffer */
    uint8_t readBuffer[BUFFER_SIZE];

    /* Write Buffer*/
    uint8_t writeBuffer[BUFFER_SIZE];
    
    /* read address */
    uint32_t address;

    /* read size */
    uint32_t size;
    
    /* tracks write index */
    uint32_t write_index;
} APP_DATA;

// *****************************************************************************
// *****************************************************************************
// Section: Application Callback Routines
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

  Summary:
     MPLAB Harmony application initialization routine.

  Description:
    This function initializes the Harmony application.  It places the
    application in its initial state and prepares it to run so that its
    APP_Tasks function can be called.

  Precondition:
    All other system initialization routines should be called before calling
    this routine (in "SYS_Initialize").

  Parameters:
    None.

  Returns:
    None.

  Example:
    <code>
    APP_Initialize();
    </code>

  Remarks:
    This routine must be called from the SYS_Initialize function.
*/

void APP_Initialize ( void );


/*******************************************************************************
  Function:
    void APP_Tasks ( void )

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
    APP_Tasks();
    </code>

  Remarks:
    This routine must be called from SYS_Tasks() routine.
 */

void APP_Tasks( void );



#endif /* _APP_H */

//DOM-IGNORE-BEGIN
#ifdef __cplusplus
}
#endif
//DOM-IGNORE-END

/*******************************************************************************
 End of File
 */

