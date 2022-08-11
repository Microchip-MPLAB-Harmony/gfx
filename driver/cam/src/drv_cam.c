/*******************************************************************************
  Camera abstraction layer Driver Implementation.

  Company:
    Microchip Technology Inc.

  File Name:
    drv_cam.c

  Summary:
    Source code for the camera driver for ISC/ISI applications.

  Description:
    This file contains the source code for setup of the camera sensor module. It
    abstracts the camera detection & configuration for the actual camera sensor 
    used. The camera specific driver needs to be implemented. The application 
    calls camera init function from this driver.
*******************************************************************************/

//DOM-IGNORE-BEGIN
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
//DOM-IGNORE-END

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************
#include "driver/cam/drv_cam.h"


// *****************************************************************************
// *****************************************************************************
// Section: File scope functions
// *****************************************************************************
// *****************************************************************************
static bool DRV_Camera_Detect(void)
{
    bool sensor_found = false;
    /* Perform any camera specific initialization */
    drv_imgsensor_bsp_init();
    /* Return TRUE if the supported camera is detected */
    sensor_found = drv_imgsensor_verifyChipID();
    return sensor_found;
}

static void DRV_Camera_Setup(void)
{
    /* Call the implementation specific to the camera used to configure it */
    drv_imgsensor_setup();
}

// *****************************************************************************
// *****************************************************************************
// Section: Camera Driver Common Interface Implementation called by the ap
// *****************************************************************************
// *****************************************************************************

void DRV_Camera_Init(void)
{
    /* If the supported camera is detected, configure it */
    if(DRV_Camera_Detect() == true)
        DRV_Camera_Setup();
    
}
