/*******************************************************************************
  Custom Camera Register Header File

  Company:
    Microchip Technology Inc.

  File Name:
    drv_custom_cam_regs.h

  Summary:
    Custom Camera Register Header File

  Description:
    The header file contains the custom camera register mapping.
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
#ifndef DRV_CUSTOM_CAM_REGS_H
#define DRV_CUSTOM_CAM_REGS_H
// *****************************************************************************
// *****************************************************************************
// Section: Data Types
// *****************************************************************************
// *****************************************************************************

typedef struct
{
    uint32_t address;
    uint32_t value;
} RegVal;

/* Custom camera CHIP ID Register Addresses */
/* NOTE: Enter the CHIP ID Register for the camer sensor*/
#define CUSTOM_CAM_CHIP_ID_HIGH_BYTE        (0x01)
#define CUSTOM_CAM_CHIP_ID_LOW_BYTE         (0x02)


/* Enter the Initial value of the custom camera registers */ 
static const RegVal initVals[] = {
};



#endif // DRV_CUSTOM_CAM_REGS_H