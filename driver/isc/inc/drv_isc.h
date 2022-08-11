/*******************************************************************************
  ISC Driver Interface Header File

  Company:
    Microchip Technology Inc.

  File Name:
    drv_isc.h

  Summary:
    ISC Driver Interface Header File

  Description:
    The I2C device driver provides a simple interface to manage the ISC peripheral
    on Microchip ARM microprocessors.  This file provides the interface
    definition for the ISC driver.
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

#ifndef DRV_ISC_H
#define DRV_ISC_H

// *****************************************************************************
// *****************************************************************************
// Section: File includes
// *****************************************************************************
// *****************************************************************************

#include "driver/isc/drv_isc.h"
// DOM-IGNORE-BEGIN
#ifdef __cplusplus  // Provide C++ Compatibility

    extern "C" {

#endif
// DOM-IGNORE-END

#define ISCD_MAX_DMA_DESC (2)
// *****************************************************************************
// *****************************************************************************
// Section: Data Types
// *****************************************************************************
// *****************************************************************************

        
        
// *****************************************************************************
// *****************************************************************************
// Section: ISC Driver System Interface Routines
// *****************************************************************************
// *****************************************************************************
// *****************************************************************************
/* Function:
    void DRV_ISC_Capture(void)
    

  Description:
    Send Capture Input Stream Command to start a capture.

  Precondition:
    DRV_ISC_Configure() must be called before calling DRV_ISC_Capture().

  Parameters:
    None.

  Returns:
    None.

  Example:
    <code>
    DRV_ISC_Capture();
    </code>

 */

void DRV_ISC_Capture( void );

// *****************************************************************************
/* Function:
    void DRV_ISC_Configure(void)
    

  Description:
    Configures ISC in continues capture mode.

  Precondition:
    None.

  Parameters:
    None.

  Returns:
    None.

  Example:
    <code>
    DRV_ISC_Configure();
    </code>

 */

void DRV_ISC_Configure( uint32_t cam_bufferp );


       
//DOM-IGNORE-BEGIN
#ifdef __cplusplus
}
#endif
//DOM-IGNORE-END


#endif // #ifndef DRV_ISC_H        