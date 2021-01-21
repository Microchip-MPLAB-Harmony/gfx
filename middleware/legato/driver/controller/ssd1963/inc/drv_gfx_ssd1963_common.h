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
 Module for Microchip Graphics Library - Hardware Abstraction Layer

  Company:
    Microchip Technology Inc.

  File Name:
    drv_gfx_ssd1963_common.h

  Summary:
    Contains SSD1963 driver type definitions and interface prototypes.

  Description:
    Contains SSD1963 driver type definitions and interface prototypes.
*******************************************************************************/


#ifndef DRV_GFX_SSD1963_COMMON_H    /* Guard against multiple inclusion */
#define DRV_GFX_SSD1963_COMMON_H

/** SSD1963_CMD_PARAM

  Summary:
    Structure contains command and parameter information.

*/
typedef struct 
{
    /* Command */
    uint8_t cmd;
    
    /* Number of command parameters */
    uint8_t parmCount;
    
    /* Command parameters, max of 4 */
    uint8_t parms[4];
} SSD1963_CMD_PARAM;

/** SSD1963_DRV_STATE

  Summary:
    Enum of SSD1963 driver states.
    
*/
typedef enum
{
    INIT = 0,
    RUN
} SSD1963_DRV_STATE;

/** SSD1963_DRV

  Summary:
    Structure contains driver-specific data and ops pointers.
    
*/
typedef struct SSD1963_DRV 
{
    /* Driver state */
    SSD1963_DRV_STATE state;
    
    /* Port-specific private data */
    void *port_priv;
} SSD1963_DRV;

/* Provide C++ Compatibility */
#ifdef __cplusplus
}
#endif

#endif // DRV_GFX_SSD1963_COMMON_H

/* *****************************************************************************
 End of File
 */
