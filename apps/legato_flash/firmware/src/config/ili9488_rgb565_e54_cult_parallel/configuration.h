/*******************************************************************************
  System Configuration Header

  File Name:
    configuration.h

  Summary:
    Build-time configuration header for the system defined by this project.

  Description:
    An MPLAB Project may have multiple configurations.  This file defines the
    build-time options for a single configuration.

  Remarks:
    This configuration header must not define any prototypes or data
    definitions (or include any files that do).  It only provides macro
    definitions for build-time configuration options

*******************************************************************************/

// DOM-IGNORE-BEGIN
/*******************************************************************************
* Copyright (C) 2018 Microchip Technology Inc. and its subsidiaries.
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

#ifndef CONFIGURATION_H
#define CONFIGURATION_H

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************
/*  This section Includes other configuration headers necessary to completely
    define this configuration.
*/

#include "user.h"
#include "toolchain_specifics.h"

// DOM-IGNORE-BEGIN
#ifdef __cplusplus  // Provide C++ Compatibility

extern "C" {

#endif
// DOM-IGNORE-END

// *****************************************************************************
// *****************************************************************************
// Section: System Configuration
// *****************************************************************************
// *****************************************************************************



// *****************************************************************************
// *****************************************************************************
// Section: System Service Configuration
// *****************************************************************************
// *****************************************************************************
/* TIME System Service Configuration Options */
#define SYS_TIME_INDEX_0                     0
#define SYS_TIME_MAX_TIMERS                  5
#define SYS_TIME_HW_COUNTER_WIDTH            16
#define SYS_TIME_HW_COUNTER_PERIOD           0xFFFFU
#define SYS_TIME_HW_COUNTER_HALF_PERIOD	     (SYS_TIME_HW_COUNTER_PERIOD>>1)
#define SYS_TIME_CPU_CLOCK_FREQUENCY         120000000
#define SYS_TIME_COMPARE_UPDATE_EXECUTION_CYCLES      (188)


/* File System Service Configuration */

#define SYS_FS_MEDIA_NUMBER               2

#define SYS_FS_VOLUME_NUMBER              1

#define SYS_FS_AUTOMOUNT_ENABLE           false
#define SYS_FS_MAX_FILES                  2
#define SYS_FS_MAX_FILE_SYSTEM_TYPE       1
#define SYS_FS_MEDIA_MAX_BLOCK_SIZE       512
#define SYS_FS_MEDIA_MANAGER_BUFFER_SIZE  2048
#define SYS_FS_FILE_NAME_LEN              255
#define SYS_FS_CWD_STRING_LEN             1024







// *****************************************************************************
// *****************************************************************************
// Section: Driver Configuration
// *****************************************************************************
// *****************************************************************************
/* I2C Driver Instance 0 Configuration Options */
#define DRV_I2C_INDEX_0                       0
#define DRV_I2C_CLIENTS_NUMBER_IDX0           1
#define DRV_I2C_QUEUE_SIZE_IDX0               2
#define DRV_I2C_CLOCK_SPEED_IDX0              400

/* I2C Driver Common Configuration Options */
#define DRV_I2C_INSTANCES_NUMBER              1



/*** MXT336T Driver Configuration ***/
#define DRV_MAXTOUCH_I2C_MODULE_INDEX   0

/* SDMMC Driver Global Configuration Options */
#define DRV_SDMMC_INSTANCES_NUMBER                       1

/* SST26 Driver Instance Configuration */
#define DRV_SST26_INDEX                 0
#define DRV_SST26_CLIENTS_NUMBER        1
#define DRV_SST26_START_ADDRESS         0x0
#define DRV_SST26_PAGE_SIZE             256
#define DRV_SST26_ERASE_BUFFER_SIZE     4096


/*** SDMMC Driver Instance 0 Configuration ***/
#define DRV_SDMMC_INDEX_0                                0
#define DRV_SDMMC_CLIENTS_NUMBER_IDX0                    1
#define DRV_SDMMC_QUEUE_SIZE_IDX0                        1
#define DRV_SDMMC_PROTOCOL_SUPPORT_IDX0                  DRV_SDMMC_PROTOCOL_SD
#define DRV_SDMMC_CONFIG_SPEED_MODE_IDX0                 DRV_SDMMC_SPEED_MODE_DEFAULT
#define DRV_SDMMC_CONFIG_BUS_WIDTH_IDX0                  DRV_SDMMC_BUS_WIDTH_4_BIT
#define DRV_SDMMC_CARD_DETECTION_METHOD_IDX0             DRV_SDMMC_CD_METHOD_USE_SDCD





// *****************************************************************************
// *****************************************************************************
// Section: Middleware & Other Library Configuration
// *****************************************************************************
// *****************************************************************************
// *****************************************************************************
// *****************************************************************************
// Section: USB Host Layer Configuration
// *****************************************************************************
// **************************************************************************

/* Number of Endpoints used */
#define DRV_USBFSV1_ENDPOINTS_NUMBER                        1

#define USB_HOST_DEVICES_NUMBER                             1

/* Size of Endpoint 0 buffer */
#define USB_DEVICE_EP0_BUFFER_SIZE                          64

/* Target peripheral list entries */
#define  USB_HOST_TPL_ENTRIES                               1 

/* Maximum number of configurations supported per device */
#define USB_HOST_DEVICE_INTERFACES_NUMBER                   5    

#define USB_HOST_CONTROLLERS_NUMBER                         1

#define USB_HOST_TRANSFERS_NUMBER                           10

/* Provides Host pipes number */
#define USB_HOST_PIPES_NUMBER                               10

/* Number of Host Layer Clients */
#define USB_HOST_CLIENTS_NUMBER                             1   

/*** USB Driver Configuration ***/

/* Maximum USB driver instances */
#define DRV_USBFSV1_INSTANCES_NUMBER                        1


/* Disable Device Support */
#define DRV_USBFSV1_DEVICE_SUPPORT                          false
	
/* Enable Host Support */
#define DRV_USBFSV1_HOST_SUPPORT                            true

/* Number of NAKs to wait before returning transfer failure */ 
#define DRV_USBFSV1_HOST_NAK_LIMIT                          2000 

/* Maximum Number of pipes */
#define DRV_USBFSV1_HOST_PIPES_NUMBER                       10 

/* Attach Debounce duration in milli Seconds */ 
#define DRV_USBFSV1_HOST_ATTACH_DEBOUNCE_DURATION           500

/* Reset duration in milli Seconds */ 
#define DRV_USBFSV1_HOST_RESET_DURATION                     100

/* Alignment for buffers that are submitted to USB Driver*/ 
#define USB_ALIGN  CACHE_ALIGN

/* Number of MSD Function driver instances in the application */
#define USB_HOST_MSD_INSTANCES_NUMBER         1

/* Number of Logical Units */
#define USB_HOST_SCSI_INSTANCES_NUMBER        1
#define USB_HOST_MSD_LUN_NUMBERS              1




// *****************************************************************************
// *****************************************************************************
// Section: Application Configuration
// *****************************************************************************
// *****************************************************************************


//DOM-IGNORE-BEGIN
#ifdef __cplusplus
}
#endif
//DOM-IGNORE-END

#endif // CONFIGURATION_H
/*******************************************************************************
 End of File
*/
