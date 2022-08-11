/*******************************************************************************
  ISC Driver Implementation.

  Company:
    Microchip Technology Inc.

  File Name:
    drv_isc.c

  Summary:
    Source code for the ISC driver.

  Description:
    This file contains the source code for the implementation of the
    ISC driver.
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
#include "configuration.h"
#include "driver/isc/drv_isc.h"
#include "system/debug/sys_debug.h"
#include "peripheral/isc/plib_isc.h"

// *****************************************************************************
// *****************************************************************************
// Section: Global Data
// *****************************************************************************
// *****************************************************************************
/* ISC color correction components */
static struct ISC_Color_Correction ref_cc = {
	0, 0, 0, 0x100, 0, 0, 0x100, 0, 0, 0, 0, 0x100};

/* DMA pool for ISC */
struct ISC_DMA_View __attribute__ ((section(".region_nocache"), aligned (32))) isc_dma_view_array[2]= { 0 };

// *****************************************************************************
// *****************************************************************************
// Section: File scope functions
// *****************************************************************************
// *****************************************************************************
static void _DRV_ISC_Configure_DMA(uint32_t bufferp)
{
    struct ISC_DMA_View* dma_view;
    struct  ISC_DMA_Control_Config ctrl;
    ctrl.Descriptor_Enable = 1;
    ctrl.Descriptor_View = 0;
    ctrl.Interrupt_Enable = 0;
    ctrl.Writeback_Enable = 0;
    ctrl.Field = 0;
    ctrl.Done = 0;
    
    dma_view = isc_dma_view_array;
    dma_view[0].ctrl = ISC_DCTRL_DVIEW_PACKED | ISC_DCTRL_DE_Msk;
	dma_view[0].next_desc = (uint32_t)&dma_view[0];
	dma_view[0].addr = (uint32_t)bufferp; 
	dma_view[0].stride = 0;
    ISC_DMA_Configure_Desc((uint32_t)&dma_view[0]);
    ISC_DMA_Configure_Register(ISC_DCFG_IMODE_PACKED32_Val, ISC_DCFG_YMBSIZE_BEATS8_Val, ISC_DCFG_CMBSIZE_SINGLE_Val);
    ISC_DMA_Enable(&ctrl);
}


// *****************************************************************************
// *****************************************************************************
// Section: ISC Driver Common Interface Implementation
// *****************************************************************************
// *****************************************************************************

void DRV_ISC_Configure( uint32_t cam_bufferp )
{
    /* Perform software reset of the interface */
    ISC_Software_Reset();
    /* Set Continuous Acquisition mode */
    ISC_PFE_Set_Continuous_Mode(true);
    /* Configure the Parallel Front End module performs data
	 * re-sampling across clock domain boundary. The PFE module 
     * outputs a 12-bit data on the vp_data[11:0] bus */
    ISC_PFE_Set_Video_Mode(ISC_PFE_CFG0_MODE_PROGRESSIVE);
   
    /* ISC_PFE_CFG0.BPS shows the number of bits per sample depends on the bit
	 * width of sensor output */
    ISC_PFE_Set_BPS(ISC_PFE_CFG0_BPS_EIGHT);
    /* valid pixels are sampled when VSYNC is deasserted and HSYNC is asserted */
    ISC_PFE_Set_Sync_Polarity(0, ISC_PFE_CFG0_VPOL_Msk);
    /* Enable Gamma Correction */
    ISC_Gamma_Enable(true, 0, 0, 0);
    /* Enable Color Filter Array Interpolation */
    ISC_CFA_Enable(true);
    ISC_CFA_Configure(ISC_CFA_CFG_BAYCFG_BGBG_Val, 1);
    
    /* The White Balance (WB) module captures the data bus from the PFE module,
	 * each Bayer color component (R,Gr, B, Gb) can be manually adjusted using
	 *  an offset and a gain. */

    ISC_WB_Enable(true);
    ISC_WB_Set_Bayer_Pattern(ISC_WB_CFG_BAYCFG_BGBG);
    
    /* Default value for White balance setting */
    ISC_WB_Adjust_Color_Offset(0, 0, 0, 0);
    ISC_WB_Adjust_Color_Gain(0x200, 0x200, 0x200, 0x200);
    
    
    ISC_CBC_Enable(true);
    //ISC_CBC_Configure(false, 0, 0xF, 0x10);
    ISC_CBC_Configure(false, 0, 0, 0x100);
    
    ISC_CC_Enable(true);
    ISC_CC_Configure(&ref_cc);
    ISC_RLP_Configure(ISC_RLP_CFG_MODE_ARGB32_Val, 0);
    _DRV_ISC_Configure_DMA(cam_bufferp);       
    ISC_Update_Profile();
    ISC_Enable_Interrupt(ISC_INTEN_VD_Msk);
    ISC_Get_Interrupt_Status();

}

void DRV_ISC_Capture( void )
{
    ISC_Start_Capture();
}
/* *****************************************************************************
 End of File
 */
