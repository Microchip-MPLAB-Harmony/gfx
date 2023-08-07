/*******************************************************************************
  CSI2DC Peripheral Library Source File

  Company:
    Microchip Technology Inc.

  File Name:
    plib_csi2dc.c

  Summary:
    CSI2DC peripheral library source file.

  Description:
    The CSI2DC PLIB provides a simple interface to manage the CSI2 Demux controller.

*******************************************************************************/

//DOM-IGNORE-BEGIN
/*******************************************************************************
* Copyright (C) 2019 Microchip Technology Inc. and its subsidiaries.
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
// Included Files
// *****************************************************************************
// *****************************************************************************
#include "configuration.h"
#include "plib_csi2dc.h"


// *****************************************************************************
// *****************************************************************************
// Global Data
// *****************************************************************************
// *****************************************************************************


void CSI2DC_Power_Up ( bool pu )
{
    if(pu == true)
        // Powering up, de-assert reset line
        //CSI2DC_REGS->CSI2DC_GCTLR = CSI2DC_GCTLR_SWRST_1;
        CSI2DC_REGS->CSI2DC_GCTLR = CSI2DC_GCTLR_SWRST_0;
    else
        // Powering down, assert reset line
        CSI2DC_REGS->CSI2DC_GCTLR = CSI2DC_GCTLR_SWRST_0;
        CSI2DC_REGS->CSI2DC_GCTLR = CSI2DC_GCTLR_SWRST_1;
}

static void CSI2DC_Enable_Video_Pipe ( bool en )
{
    if(en == true)
        CSI2DC_REGS->CSI2DC_VPER = CSI2DC_VPER_ENABLE_1;
    else
        CSI2DC_REGS->CSI2DC_VPER = CSI2DC_VPER_ENABLE_0;
}

static uint8_t CSI2DC_Set_Image_DataType ( CSI2DC_DATA_TYPE dt )
{
//    uint8_t status =0;
    if ((RAW6 <= dt)||(dt <= RAW14)){
//        status = 1;
        CSI2DC_REGS->CSI2DC_VPCFGR |= dt;
    }
    //else invalid datatype
        
      return 0;      
}

static void CSI2DC_Set_Image_Post_Adjustment ( bool pa )
{
    if(pa == true)
        CSI2DC_REGS->CSI2DC_VPCFGR |= CSI2DC_VPCFGR_PA_Msk;
    else
        CSI2DC_REGS->CSI2DC_VPCFGR &= ~CSI2DC_VPCFGR_PA_Msk;
}


static void CSI2DC_Set_MIPI_FRN_Clock ( bool frn )
{
    if(frn == true)
        CSI2DC_REGS->CSI2DC_GCFGR &= ~CSI2DC_GCFGR_MIPIFRN_Msk;
    else
        CSI2DC_REGS->CSI2DC_GCFGR |= CSI2DC_GCFGR_MIPIFRN_Msk;
}

static void CSI2DC_Set_Pipe_Update(CSI2DC_PUR_ATTRIBUTE attr )
{
    CSI2DC_REGS->CSI2DC_PUR = attr;
}

// *****************************************************************************
// *****************************************************************************
// CSI2 DC PLib Interface Routine
// *****************************************************************************
// *****************************************************************************
// *****************************************************************************
/* Function:
    void CSI2DC_Initialize(void)

   Summary:
    Initializes the CSI2DC peripheral.

   Precondition:
    None.

   Parameters:
    None.

   Returns:
    None
*/
void CSI2DC_Initialize ( void )
{
    //CSI2DC_Power_Up(true);
    CSI2DC_Set_MIPI_FRN_Clock(false);  
    CSI2DC_Set_Image_Post_Adjustment(true);
    CSI2DC_Set_Image_DataType(RAW10);
    CSI2DC_Enable_Video_Pipe(true);
    CSI2DC_Set_Pipe_Update(VP);    
}