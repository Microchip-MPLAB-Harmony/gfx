#ifndef PLIB_MIPI_CSI_DPHY_H
#define PLIB_MIPI_CSI_DPHY_H
/*******************************************************************************
  MIPI CSI DPHY PLIB Header

  Company:
    Microchip Technology Inc.

  File Name:
    plib_mipi_csi_dphy.h

  Summary:
    This file provides the public declarations for the CSI DPHY Interface.

  Description:
    None

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
#include "device.h"
#include <stddef.h>
#include <stdbool.h>
#include <device.h>
#include "system/system_module.h"
#include "driver/driver.h"
#include "driver/driver_common.h"

#ifdef __cplusplus  // Provide C++ Compatibility
    extern "C" {
#endif
// DOM-IGNORE-END
        
// *****************************************************************************
/* Function:
    void MIPI_DPHY_Init ( void )

  Summary:
    Initializes the MIPI DPHY Interface.

  Description:
    This function Initializes the MIPI DPHY Interface.

  Precondition:
    None.

  Parameters:
    None.
 
  Returns:
    None.

  Example:
    <code>
    MIPI_DPHY_Init();
    </code>

  Remarks:
    None.
*/
void MIPI_DPHY_Init ( void );      
                
void MIPI_DPHY_Configure ( void );  
// DOM-IGNORE-BEGIN
#ifdef __cplusplus  // Provide C++ Compatibility
    }
#endif
// DOM-IGNORE-END
    
#endif // PLIB_MIPI_CSI_DPHY_H
        