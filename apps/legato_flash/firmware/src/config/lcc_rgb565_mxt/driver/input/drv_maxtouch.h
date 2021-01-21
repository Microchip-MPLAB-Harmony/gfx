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
 Touch Controller MAXTOUCH Driver Interface File

  File Name:
    drv_maxtouch.c

  Summary:
    Touch controller MAXTOUCH Driver interface header file.

  Description:
    This header file describes the macros, data structure and prototypes of the 
    touch controller MAXTOUCH driver interface.
 ******************************************************************************/


#ifndef _DRV_MAXTOUCH_H
#define _DRV_MAXTOUCH_H

#include "system/system_module.h"
#include "driver/driver.h"
#include "driver/driver_common.h"
#include "system/int/sys_int.h"

#ifdef __cplusplus
    extern "C" {
#endif
        
typedef void (*DRV_MAXTOUCH_ConfigProgress_FnPtr)(uint32_t);
typedef void (*DRV_MAXTOUCH_ConfigFileReader_FnPtr)(char * ptr);
typedef bool (*DRV_MAXTOUCH_ConfigFileEof_FnPtr)(void);

// *****************************************************************************
/* MAXTOUCH Driver Module Index Numbers

  Summary:
    MAXTOUCH driver index definitions.

  Description:
    These constants provide the MAXTOUCH driver index definitions.

  Remarks:
    These constants should be used in place of hard-coded numeric literals.
    These values should be passed into the DRV_MAXTOUCH_Initialize and
    DRV_MAXTOUCH_Open functions to identify the driver instance in use.
*/

#define DRV_MAXTOUCH_INDEX_0         0
#define DRV_MAXTOUCH_INDEX_1         1

// *****************************************************************************
/* MAXTOUCH Driver Module Index Count

  Summary:
    Number of valid Touch controller MAXTOUCH driver indices.

  Description:
    This constant identifies the number of valid Touch Controller MAXTOUCH
    driver indices.

  Remarks:
    This constant should be used in place of hard-coded numeric literals.
    This value is derived from device-specific header files defined as part of 
    the peripheral libraries.
*/

#define DRV_MAXTOUCH_INDEX_COUNT     2

// *****************************************************************************
/*Structure
	DRV_MAXTOUCH_INIT

  Summary:
    Defines the data required to initialize or reinitialize the MAXTOUCH driver

  Description:
    This data type defines the data required to initialize or reinitialize the
    MAXTOUCH driver.

  Remarks:
    None.
*/
typedef struct
{
    /* System module initialization */
    SYS_MODULE_INIT         moduleInit;

    /* ID */
    int                     touchId;

    /* initialize function for module (normally called statically */
    SYS_MODULE_OBJ          (*drvInitialize) (const SYS_MODULE_INDEX index,
                                              const SYS_MODULE_INIT* const init);

    /* open function for I2C driver */
    DRV_HANDLE              (*drvOpen) ( const SYS_MODULE_INDEX index, const DRV_IO_INTENT intent );
    
    /* interrupt source for driver instance */
    INT_SOURCE              interruptSource;
    
    /* */
    uint16_t	            orientation;          // Orientation of the display (given in degrees of 0,90,180,270)

    /* */
    uint16_t                horizontalResolution; // Horizontal Resolution of the displayed orientation in Pixels

    /* */
    uint16_t                verticalResolution;
             
} DRV_MAXTOUCH_INIT;

// *****************************************************************************
/* DRV_MAXTOUCH_CONFIG_SOURCE

  Summary:
    Identifies the source of configuration input.

  Description:
    This enumeration identifies the current source from which a configuration file
    is read.

  Remarks:
    This enumeration is the input type for the load config status routine.
*/

typedef enum
{
    // Indicates that the configuration file is stored in program flash.
    DRV_MAXTOUCH_RAW_FLASH   = 0,

    // Indicates that the configuration file is stored in USB thumb drive.
    DRV_MAXTOUCH_RAW_FILE    = 1,

    // Indicates that the configuration file is stored in sdcard drive.
    DRV_MAXTOUCH_XCFG_FILE   = 2,
            
    DRV_MAXTOUCH_UNKNOWN_FILE,

} DRV_MAXTOUCH_CONFIG_TYPE;

typedef struct DRV_MAXTOUCH_Firmware_t
{
    void * data;
    DRV_MAXTOUCH_CONFIG_TYPE type;
    DRV_MAXTOUCH_ConfigFileReader_FnPtr reader;
    DRV_MAXTOUCH_ConfigFileEof_FnPtr eof;
    DRV_MAXTOUCH_ConfigProgress_FnPtr progress;
    void * mem;
    size_t mem_size;
} DRV_MAXTOUCH_Firmware;

// *****************************************************************************
// Section: Interface Routines - System Level
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/* Function:
      SYS_MODULE_OBJ DRV_MAXTOUCH_Initialize(const SYS_MODULE_INDEX index,
                                           const SYS_MODULE_INIT * const init )

  Summary:
    Initializes the MAXTOUCH instance for the specified driver index

  Description:
    This routine initializes the MAXTOUCH driver instance for the specified
    driver index, making it ready for clients to open and use it. The
    initialization data is specified by the 'init' parameter. The initialization
    may fail if the number of driver objects allocated are insufficient or if
    the specified driver instance is already initialized. The driver instance
    index is independent of the MAXTOUCH module ID. For example, driver instance
    0 can be assigned to MAXTOUCH2.  If the driver is built statically, then
    some of the initialization parameters are overridden by configuration
    macros. Refer to the description of the DRV_MAXTOUCH_INIT data
    structure for more details on which members on this data structure are
    overridden.

  Precondition:
    None.

  Parameters:
    index  - Identifier for the instance to be initialized.  Please note this
             is not the MAXTOUCH ID.  The hardware MAXTOUCH ID is set in the
             initialization structure. This is the index of the driver index to
             use.

    init   - Pointer to a data structure containing any data necessary to
             initialize the driver. If this pointer is NULL, the driver
             uses the static initialization override macros for each
             member of the initialization data structure.

  Returns:
    If successful, returns a valid handle to a driver instance object.
    Otherwise, returns SYS_MODULE_OBJ_INVALID.

  Example:
    <code>
    DRV_MAXTOUCH_INIT        init;
    SYS_MODULE_OBJ      objectHandle;

    // Populate the MAXTOUCH initialization structure
    // Touch Module Id
    init.moduleInit                  = {0},
    init.touchId                     = DRV_TOUCH_INDEX_0,
    init.drvInitialize               = NULL,
    init.drvOpen                     = DRV_I2C_Open,
    init.interruptSource             = INT_SOURCE_EXTERNAL_1,
    init.interruptChannel            = PORT_CHANNEL_D,
    init.interruptPin                = PORTS_BIT_POS_1,
    init.resetChannel                = PORT_CHANNEL_A,
    init.resetPin                    = PORTS_BIT_POS_14,

    objectHandle = DRV_MAXTOUCH_Initialize(DRV_TOUCH_INDEX_0,
                                              (SYS_MODULE_INIT*)init);
    if (SYS_MODULE_OBJ_INVALID == objectHandle)
    {
        // Handle error
    }
    </code>

  Remarks:
    This routine must be called before any other MAXTOUCH routine is called.

    This routine should only be called once during system initialization
    unless DRV_MAXTOUCH_Deinitialize is called to deinitialize the driver
    instance. This routine will NEVER block for hardware access.
*/

SYS_MODULE_OBJ DRV_MAXTOUCH_Initialize(const SYS_MODULE_INDEX index,
                                      const SYS_MODULE_INIT* const init);

/*************************************************************************
  Function:
       void DRV_MAXTOUCH_Deinitialize ( SYS_MODULE_OBJ object )

  Summary:
    Deinitializes the specified instance of the MAXTOUCH driver module.
	<p><b>Implementation:</b> Dynamic</p>

  Description:
    Deinitializes the specified instance of the MAXTOUCH driver module,
    disabling its operation (and any hardware) and invalidates all of the
    internal data.

  Preconditions:
    Function DRV_MAXTOUCH_Initialize must have been called before calling 
    this routine and a valid SYS_MODULE_OBJ must have been returned.

  Parameter:
    object -  Driver object handle, returned from DRV_MAXTOUCH_Initialize

  Returns:
    None.

  Example:
    <code>
    SYS_MODULE_OBJ      object;    //Returned from DRV_MAXTOUCH_Initialize
    SYS_STATUS          status;

    DRV_MAXTOUCH_Deinitialize ( object );

    status = DRV_MAXTOUCH_Status( object );
    if( SYS_MODULE_UNINITIALIZED == status )
    {
        // Check again later if you need to know
        // when the driver is deinitialized.
    }
    </code>

  Remarks:
    Once the Initialize operation has been called, the De-initialize
    operation must be called before the Initialize operation can be called
    again.

    This function will NEVER block waiting for hardware. If the operation
    requires time to allow the hardware to complete, this will be reported
    by the DRV_MAXTOUCH_Status operation. The system has to use
    DRV_MAXTOUCH_Status to determine when the module is in the ready state.
*/

void DRV_MAXTOUCH_Deinitialize(SYS_MODULE_OBJ object);


/**************************************************************************
  Function:
       SYS_STATUS DRV_MAXTOUCH_Status ( SYS_MODULE_OBJ object )

  Summary:
    Provides the current status of the MAXTOUCH driver module.
	<p><b>Implementation:</b> Dynamic</p>

  Description:
    This function provides the current status of the MAXTOUCH driver module.

  Precondition:
    The DRV_MAXTOUCH_Initialize function must have been called before 
    calling this function.

  Parameters:
    object -  Driver object handle, returned from DRV_MAXTOUCH_Initialize

  Returns:
    SYS_STATUS_READY - Indicates that the driver is busy with a previous
    system-level operation and cannot start another

  Example:
    <code>
    SYS_MODULE_OBJ      object;  // Returned from DRV_MAXTOUCH_Initialize
    SYS_STATUS          status;

    status = DRV_MAXTOUCH_Status( object );
    if( SYS_STATUS_READY != status )
    {
        // Handle error
    }
    </code>

  Remarks:
    Any value greater than SYS_STATUS_READY is also a normal running state
    in which the driver is ready to accept new operations.

    SYS_MODULE_UNINITIALIZED - Indicates that the driver has been
    deinitialized

    This value is less than SYS_STATUS_ERROR.

    This function can be used to determine when any of the driver's module
    level operations has completed.

    If the status operation returns SYS_STATUS_BUSY, the previous operation
    has not yet completed. Once the status operation returns
    SYS_STATUS_READY, any previous operations have completed.

    The value of SYS_STATUS_ERROR is negative (-1). Any value less than
    that is also an error state.

    This function will NEVER block waiting for hardware.

    If the Status operation returns an error value, the error may be
    cleared by calling the reinitialize operation. If that fails, the
    deinitialize operation will need to be called, followed by the
    initialize operation to return to normal operations.
*/

SYS_STATUS DRV_MAXTOUCH_Status(SYS_MODULE_OBJ object);

// *****************************************************************************
/* Function:
    void DRV_MAXTOUCH_ConfigParse ( SYS_MODULE_OBJ object, DRV_MAXTOUCH_Firmware * firmware );

  Summary:
    Implements the loading of maxTouch configuration to MXT Device RAM.
	<p><b>Implementation:</b> Dynamic</p>

  Description:
	This routine is used to update mxt device ram with a configuration specific 
    by config_data and from a location named source. 

  Precondition:
    The DRV_MAXTOUCH_Initialize routine must have been called for the 
    specified MAXTOUCH driver instance.

  Parameters:
    config - pointer to configuration data structure


  Returns:
    None.

  Example:
    <code>
    DRV_MAXTOUCH_Firmware config;
  
    config.data = Default_RAW_FLASH;
    config.type = DRV_MAXTOUCH_RAW_FLASH;

    DRV_MAXTOUCH_ConfigLoad ( &config );
    </code>

  Remarks:

*/

void DRV_MAXTOUCH_ConfigParse ( SYS_MODULE_OBJ object, DRV_MAXTOUCH_Firmware * firmware );

// *****************************************************************************
/* Function:
    void DRV_MAXTOUCH_ConfigLoad ( uint8_t* config_data, DRV_MAXTOUCH_CONFIG_SOURCE source );

  Summary:
    Implements the loading of maxTouch configuration to MXT Device RAM.
	<p><b>Implementation:</b> Dynamic</p>

  Description:
	This routine is used to update mxt device ram with a configuration specific 
    by config_data and from a location named source. 

  Precondition:
    The DRV_MAXTOUCH_Initialize routine must have been called for the 
    specified MAXTOUCH driver instance.

  Parameters:
    config - pointer to configuration data structure


  Returns:
    None.

  Example:
    <code>
    DRV_MAXTOUCH_Firmware config;
  
    config.data = Default_RAW_FLASH;
    config.type = DRV_MAXTOUCH_RAW_FLASH;

    DRV_MAXTOUCH_ConfigLoad ( &config );
    </code>

  Remarks:

*/
void DRV_MAXTOUCH_ConfigLoad ( SYS_MODULE_OBJ object, DRV_MAXTOUCH_Firmware * firmware );

// *****************************************************************************
/* Function:
    void DRV_MAXTOUCH_ConfigSave ( );

  Summary:
    Implements the saving of maxTouch configuration to MXT Device 
    non-volatile memory.
	<p><b>Implementation:</b> Dynamic</p>

  Description:
	This routine is used to save configuration from RAM to non-volatile memory. 

  Precondition:
    The DRV_MAXTOUCH_ConfigLoad routine must have been called for the 
    specified MAXTOUCH driver instance.

  Parameters:
    None.

  Returns:
    None.

  Example:
    <code>

    DRV_MAXTOUCH_ConfigSave ();
    </code>

  Remarks:

*/
void DRV_MAXTOUCH_ConfigSave (SYS_MODULE_OBJ object, DRV_MAXTOUCH_ConfigProgress_FnPtr ptr );

// *****************************************************************************
/* Function:
    void DRV_MAXTOUCH_Tasks ( SYS_MODULE_OBJ object );

  Summary:
    Maintains the driver's state machine and implements its task queue
    processing.
	<p><b>Implementation:</b> Dynamic</p>

  Description:
	This routine is used to maintain the driver's internal state
	machine and implement its command queue processing. It is always called
        from SYS_Tasks() function. This routine decodes the touch input data
        available in drvI2CReadFrameData.

  Precondition:
    The DRV_MAXTOUCH_Initialize routine must have been called for the 
    specified MAXTOUCH driver instance.

  Parameters:
    object      - Object handle for the specified driver instance (returned from
                  DRV_MAXTOUCH_Initialize)

  Returns:
    None.

  Example:
    <code>
    SYS_MODULE_OBJ      object;   // Returned from DRV_MAXTOUCH_Initialize

    void SYS_Tasks( void )
    {
        DRV_MAXTOUCH_Tasks ( object );

        // Do other tasks
    }
    </code>

  Remarks:
    This function is normally not called directly by an application.  It is
    called by the system's Tasks routine (SYS_Tasks)

*/

void DRV_MAXTOUCH_Tasks (SYS_MODULE_OBJ object);

// *****************************************************************************
/* Function:
    void DRV_MAXTOUCH_ReadRequest( SYS_MODULE_OBJ object )

  Summary:
    Sends a read request to I2C bus driver and adds the read task to queue.
	<p><b>Implementation:</b> Dynamic</p>

  Description:
	This routine is used to send a touch input read request to the I2C bus
        driver and adding the input read decode task to the queue. It is always
        called from MAXTOUCH interrupt ISR routine.

  Precondition:
    The DRV_MAXTOUCH_Initialize routine must have been called for the 
    specified MAXTOUCH driver instance. 

  Parameters:
    object      - Object handle for the specified driver instance (returned from
                  DRV_MAXTOUCH_Initialize)

  Returns:
    None.

  Example:
    <code>
    SYS_MODULE_OBJ      object;   // Returned from DRV_MAXTOUCH_Initialize

    void __ISR(_EXTERNAL_INT_VECTOR, ipl5) _IntHandlerDrvMXT(void)
    {
        DRV_MAXTOUCH_ReadRequest ( object );

        // Do other tasks
    }
    </code>

  Remarks:
    This function is normally not called directly by an application.  It is
    called by the MAXTOUCH ISR routine.

*/
void DRV_MAXTOUCH_ReadRequest(SYS_MODULE_OBJ object);

// *****************************************************************************
// *****************************************************************************
// Section: Interface Routines - Client Level
// *****************************************************************************
// *****************************************************************************

/**************************************************************************
  Function:
       DRV_HANDLE DRV_MAXTOUCH_Open ( const SYS_MODULE_INDEX drvIndex,
                                const DRV_IO_INTENT    intent )

  Summary:
    Opens the specified MAXTOUCH driver instance and returns a handle to it.
	<p><b>Implementation:</b> Dynamic</p>
	
  Description:
    This routine opens the specified MAXTOUCH driver instance and provides a
    handle that must be provided to all other client-level operations to
    identify the caller and the instance of the driver. The ioIntent
    parameter defines how the client interacts with this driver instance.

    The current version of driver does not support the DRV_IO_INTENT feature.
    The driver is by default non-blocking. The driver can perform both read
    and write to the MAXTOUCH device. The driver supports single client only.	
	
  Precondition:
    The DRV_MAXTOUCH_Initialize function must have been called before 
    calling this function.
	
  Parameters:
    drvIndex -  Index of the driver initialized with
                DRV_MAXTOUCH_Initialize().
                
    intent -    Zero or more of the values from the enumeration
                DRV_IO_INTENT ORed together to indicate the intended use of
                the driver. The current version of driver does not support
				the selective IO intent feature.
				
  Returns:
    If successful, the routine returns a valid open-instance handle (a
    number identifying both the caller and the module instance).

    If an error occurs, the return value is DRV_HANDLE_INVALID. An error
    can occur when the following is true:
      * if the number of client objects allocated via
        DRV_MAXTOUCH_CLIENTS_NUMBER is insufficient
      * if the client is trying to open the driver but driver has been
        opened exclusively by another client
      * if the driver hardware instance being opened is not initialized or
        is invalid
		
  Example:
    <code>
    DRV_HANDLE  handle;

    handle = DRV_MAXTOUCH_Open( DRV_MAXTOUCH_INDEX_0,
                                      DRV_IO_INTENT_EXCLUSIVE );

    if( DRV_HANDLE_INVALID == handle )
    {
        // Unable to open the driver
    }
    </code>
	
  Remarks:
    The handle returned is valid until the DRV_MAXTOUCH_Close routine is
    called. This routine will NEVER block waiting for hardware. If the
    requested intent flags are not supported, the routine will return
    DRV_HANDLE_INVALID. This function is thread safe in a RTOS application.
    It should not be called in an ISR.
*/

DRV_HANDLE DRV_MAXTOUCH_Open(const SYS_MODULE_INDEX drvIndex,
                            const DRV_IO_INTENT    intent );

// *****************************************************************************
/* Function:
    void DRV_MAXTOUCH_Close ( DRV_HANDLE handle )

  Summary:
    Closes an opened instance of the MAXTOUCH driver.
	<p><b>Implementation:</b> Dynamic</p>

  Description:
    This function closes an opened instance of the MAXTOUCH driver, invalidating
    the handle.

  Precondition:
    The DRV_MAXTOUCH_Initialize routine must have been called for the 
    specified MAXTOUCH driver instance.

    DRV_MAXTOUCH_Open must have been called to obtain a valid opened
    device handle.

  Parameters:
    handle       - A valid open-instance handle, returned from the driver's
                   open routine

  Returns:
    None

  Example:
    <code>
    DRV_HANDLE handle;  // Returned from DRV_MAXTOUCH_Open

    DRV_MAXTOUCH_Close ( handle );
    </code>

  Remarks:
	After calling this routine, the handle passed in "handle" must not be 
    used with any of the remaining driver routines.  A new handle must be
    obtained by calling DRV_MAXTOUCH_Open before the caller may use the
    driver again. This function is thread safe in a RTOS application.

    Note: Usually, there is no need for the driver client to verify that the 
          Close operation has completed.
*/

void DRV_MAXTOUCH_Close ( DRV_HANDLE handle );



#ifdef __cplusplus
    }
#endif
    
#endif //_DRV_MAXTOUCH_H
