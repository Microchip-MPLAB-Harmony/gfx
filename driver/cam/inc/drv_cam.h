/*******************************************************************************
  Camera (Abstract) Driver Interface Header File

  Company:
    Microchip Technology Inc.

  File Name:
    drv_cam.h

  Summary:
    Camera Driver Interface Header File

  Description:
    The camera driver provides an abstraction to the camera sensor driver that
    detects and sets up the camera sensor being used in the application. 
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

#ifndef DRV_CAM_H
#define DRV_CAM_H

// *****************************************************************************
// *****************************************************************************
// Section: File includes
// *****************************************************************************
// *****************************************************************************
#include "driver/driver.h"
#include "driver/driver_common.h"
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

// DOM-IGNORE-BEGIN
#ifdef __cplusplus  // Provide C++ Compatibility
extern "C" {
#endif
// DOM-IGNORE-END


// *****************************************************************************
// *****************************************************************************
// Section: Camera Sensor Driver Implementation Routines
// *****************************************************************************
// *****************************************************************************
// *****************************************************************************
/* Function:
    void drv_imgsensor_setup(void)
    

  Summary:
    Performs the camera sensor setup.

  Description:
    This function is called by DRV_Camera_Setup(). It calls the function specific 
    to the sensor being used to setup the sensor registers.
  
  Precondition:
    Function DRV_Camera_Detect() must be called before calling this function and 
    it must have returned TRUE.

  Parameters:
    None.

  Returns:
    None.

  Example:
    <code>
    drv_imgsensor_setup();
    </code>

 */
void drv_imgsensor_setup(void);

// *****************************************************************************
/* Function:
    bool drv_imgsensor_verifyChipID(void)
    

  Summary:
    Checks if the supported camera can be detected.

  Description:
    This function is called by DRV_Camera_Detect() to check if the supported 
    camera can be detected. 
  
  Precondition:
    None.

  Parameters:
    None.

  Returns:
    If the camera sensor chip ID can be read and if it matches the CHIP ID of the
    supported sensor, this function returns TRUE, else, returns FALSE.

  Example:
    <code>
    bool sensor_found = false;
    sensor_found = drv_imgsensor_verifyChipID();
    </code>

 */
bool drv_imgsensor_verifyChipID(void);

// *****************************************************************************
/* Function:
    void drv_imgsensor_bsp_init(void)
    

  Summary:
    Performs camera specific HW init.

  Description:
    Toggles the camera reset pin and sets the POWER pin to ON state.
  
  Precondition:
    None.

  Parameters:
    None.

  Returns:
    None.

  Example:
    <code>
    drv_imgsensor_bsp_init();
    </code>

 */
void drv_imgsensor_bsp_init(void);  
    
// *****************************************************************************
// *****************************************************************************
// Section: Camera Driver Interface Routines (Abstraction)
// *****************************************************************************
// *****************************************************************************
// *****************************************************************************
/* Function:
    void DRV_Camera_Init(void)
    

  Summary:
    Initialize the camera sensor.

  Description:
    This function is called by SYS_Initialize(). It abstracts the implementation 
    for the actual camera sensor used. The function checks if the supported camera 
    sensor can be detected and if so, its registers are setup with the init values.
    User must implement the driver for the specific camera used.
  
  Precondition:
    Function DRV_Camera_Init must be called AFTER ISC_Initialize() is called in 
    SYS_Initialize().

  Parameters:
    None.

  Returns:
    None.

  Example:
    <code>
    DRV_Camera_Init();
    </code>

 */
void DRV_Camera_Init(void);



    
// DOM-IGNORE-BEGIN
#ifdef __cplusplus
}
#endif
// DOM-IGNORE-END    
    
    
#endif // DRV_CAM