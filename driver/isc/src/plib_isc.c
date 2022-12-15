/*******************************************************************************
  ISC Peripheral Library Interface Source File

  Company:
    Microchip Technology Inc.

  File Name:
    plib_isc.c

  Summary:
    ISC function implementations for the ISC PLIB.

  Description:
    The ISC PLIB provides a simple interface to manage image sensor controller.

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
#include "plib_isc.h"

// *****************************************************************************
// *****************************************************************************
// Global Data
// *****************************************************************************
// *****************************************************************************
#define ISC_MASTER_CLK_SEL  0
#define ISC_MASTER_CLK_DIV  7
#define ISC_ISP_CLK_SEL     0
#define ISC_ISP_CLK_DIV     2

ISC_IRQ_CALLBACK_OBJECT ISC_IRQ_CallbackObj;
void ISC_Handler(void)
{
    if (ISC_IRQ_CallbackObj.callback_fn != NULL)
    {
        ISC_IRQ_CallbackObj.callback_fn(ISC_IRQ_CallbackObj.context);
    }
}


/******************************************************************************
  Function:
    ISC_Software_Reset ( void )

  Summary:
    Perform software reset of the ISC interface.

  Remarks:
    See plib_isc.h for more details.
*/
void ISC_Software_Reset ( void )
{
    while (ISC_IsSIP_Aserted());
    ISC_REGS->ISC_CTRLDIS = ISC_CTRLDIS_SWRST_Msk;
}

/******************************************************************************
  Function:
    ISC_IsSIP_Aserted( void )

  Summary:
    Checks if ISC_CTRLSR.SIP bit is asserted.

  Remarks:
    See plib_isc.h for more details.
*/
bool ISC_IsSIP_Aserted( void )
{
    return (ISC_REGS->ISC_CTRLSR &ISC_CTRLSR_SIP_Msk) == ISC_CTRLSR_SIP_Msk;
}

/******************************************************************************
  Function:
    ISC_Start_Capture( void )

  Summary:
     Send Capture Input Stream Command to start a single shot capture or multiple frames.

  Remarks:
    See plib_isc.h for more details.
*/
void ISC_Start_Capture( void )
{
    while (ISC_IsSIP_Aserted());
	ISC_REGS->ISC_CTRLEN = ISC_CTRLEN_CAPTURE_Msk;
}

/******************************************************************************
  Function:
    ISC_Stop_Capture( void )

  Summary:
    End the capture at the next Vertical Synchronization Detection.

  Remarks:
    See plib_isc.h for more details.
*/
void ISC_Stop_Capture( void )
{
    while (ISC_IsSIP_Aserted());
	ISC_REGS->ISC_CTRLEN = ISC_CTRLDIS_DISABLE_Msk;
}

/******************************************************************************
  Function:
    ISC_Get_Ctrl_Status( void )

  Summary:
    Returns ISC Control Status.

  Remarks:
    See plib_isc.h for more details.
*/
uint32_t ISC_Get_Ctrl_Status( void )
{
    return ISC_REGS->ISC_CTRLSR;
}

/******************************************************************************
  Function:
    ISC_Update_Profile( void )

  Summary:
     Update the color profile.

  Remarks:
    See plib_isc.h for more details.
*/
void ISC_Update_Profile( void )
{
    while (ISC_IsSIP_Aserted());
	ISC_REGS->ISC_CTRLEN = ISC_CTRLEN_UPPRO_Msk;
	while ((ISC_REGS->ISC_CTRLSR & ISC_CTRLSR_UPPRO_Msk) == ISC_CTRLSR_UPPRO_Msk);
}

/******************************************************************************
  Function:
    ISC_Enable_Interrupt( uint32_t flag )

  Summary:
     Enable ISC Interrupt.

  Remarks:
    See plib_isc.h for more details.
*/
void ISC_Enable_Interrupt( uint32_t flag )
{
    ISC_REGS->ISC_INTEN = flag;	
}

/******************************************************************************
  Function:
    ISC_Disable_Interrupt( uint32_t flag )

  Summary:
    Disable ISC Interrupt.

  Remarks:
    See plib_isc.h for more details.
*/
void ISC_Disable_Interrupt( uint32_t flag )
{
    ISC_REGS->ISC_INTDIS = flag;	
}

/******************************************************************************
  Function:
    ISC_Get_Interrupt_Status( void )

  Summary:
    Returns ISC Interrupt status

  Remarks:
    See plib_isc.h for more details.
*/
uint32_t ISC_Get_Interrupt_Status( void )
{
    return ISC_REGS->ISC_INTSR;	
}

/******************************************************************************
  Function:
    ISC_PFE_Set_Video_Mode( uint32_t vmode )

  Summary:
    configure PFE(Parallel Front End) video mode.

  Remarks:
    See plib_isc.h for more details.
*/
void ISC_PFE_Set_Video_Mode( uint32_t vmode )
{
    ISC_REGS->ISC_PFE_CFG0 &= ~ISC_PFE_CFG0_MODE_Msk;
    ISC_REGS->ISC_PFE_CFG0 |= vmode;
}

/******************************************************************************
  Function:
    ISC_PFE_Set_Sync_Polarity( uint32_t hpol, uint32_t vpol )

  Summary:
    set PFE(Parallel Front End) H/V synchronization polarity.

  Remarks:
    See plib_isc.h for more details.
*/
void ISC_PFE_Set_Sync_Polarity( uint32_t hpol, uint32_t vpol )
{
    ISC_REGS->ISC_PFE_CFG0 &= ~ISC_PFE_CFG0_HPOL_Msk;
    ISC_REGS->ISC_PFE_CFG0 &= ~ISC_PFE_CFG0_VPOL_Msk;
    ISC_REGS->ISC_PFE_CFG0 |= hpol | vpol;
}

/******************************************************************************
  Function:
    SC_PFE_Set_Pixel_Polarity( uint32_t ppol )

  Summary:
    set PFE(Parallel Front End) pixel clock polarity.

  Remarks:
    See plib_isc.h for more details.
*/
void ISC_PFE_Set_Pixel_Polarity( uint32_t ppol )
{
    ISC_REGS->ISC_PFE_CFG0 &= ~ISC_PFE_CFG0_PPOL_Msk;
    ISC_REGS->ISC_PFE_CFG0 |= ppol;
}

/******************************************************************************
  Function:
    SC_PFE_Set_Field_Polarity( uint32_t fpol )

  Summary:
    set PFE(Parallel Front End) field polarity.

  Remarks:
    See plib_isc.h for more details.
*/
void ISC_PFE_Set_Field_Polarity( uint32_t fpol )
{
    ISC_REGS->ISC_PFE_CFG0 &= ~ISC_PFE_CFG0_FPOL_Msk;
    ISC_REGS->ISC_PFE_CFG0 |= fpol;
}

/******************************************************************************
  Function:
    ISC_PFE_Set_BPS( uint32_t bps )

  Summary:
    set PFE(Parallel Front End) bps.

  Remarks:
    See plib_isc.h for more details.
*/
void ISC_PFE_Set_BPS( uint32_t bps )
{
    ISC_REGS->ISC_PFE_CFG0 &= ~ISC_PFE_CFG0_BPS_Msk;
    ISC_REGS->ISC_PFE_CFG0 |= bps;
}

/******************************************************************************
  Function:
    ISC_PFE_Set_Continuous_Mode ( bool enable )

  Summary:
    Set PFE(Parallel Front End)in continuous mode.

  Remarks:
    See plib_isc.h for more details.
*/
void ISC_PFE_Set_Continuous_Mode( bool enable )
{
    if(enable)
        ISC_REGS->ISC_PFE_CFG0 |= ISC_PFE_CFG0_CONT_Msk;
    else
        ISC_REGS->ISC_PFE_CFG0 &= ~ISC_PFE_CFG0_CONT_Msk;
}

/******************************************************************************
  Function:
    void ISC_SUB422_Enable( bool enable )

  Summary:
    Enables/disable 4:4:4 to 4:2:2 Chrominance Horizontal Subsampling Filter.

  Remarks:
    See plib_isc.h for more details.
*/
void ISC_SUB422_Enable( bool enable )
{
    if(enable)
        ISC_REGS->ISC_SUB422_CTRL = ISC_SUB422_CTRL_ENABLE_Msk;
    else
        ISC_REGS->ISC_SUB422_CTRL = 0;
}

/******************************************************************************
  Function:
    void ISC_SUB422_Configure( uint8_t ccir656, uint8_t byte_order, uint8_t lpf )

  Summary:
    Configure Subsampling 4:4:4 to 4:2:2 with given values.

  Remarks:
    See plib_isc.h for more details.
*/
void ISC_SUB422_Configure( uint8_t ccir656, uint8_t byte_order, uint8_t lpf )
{
    if(ccir656)
        ISC_REGS->ISC_SUB422_CFG = ISC_SUB422_CTRL_ENABLE_Msk | byte_order;
    else
        ISC_REGS->ISC_SUB422_CFG = 0;
    ISC_REGS->ISC_SUB422_CFG &= ISC_SUB422_CFG_FILTER_Msk;
    ISC_REGS->ISC_SUB422_CFG |= lpf ;
}

/******************************************************************************
  Function:
    ISC_SUB420_Enable( bool enable, bool filter )

  Summary:
    Enables/disable 4:2:2 to 4:2:0 Chrominance Vertical Subsampling Filter.

  Remarks:
    See plib_isc.h for more details.
*/
void ISC_SUB420_Enable( bool enable, bool filter )
{
	if (enable){
		ISC_REGS->ISC_SUB420_CTRL = ISC_SUB420_CTRL_ENABLE_Msk;
		if (filter)
			ISC_REGS->ISC_SUB420_CTRL |= ISC_SUB420_CTRL_FILTER_Msk;
	} else {
		ISC_REGS->ISC_SUB420_CTRL = 0;
	}
}

/******************************************************************************
  Function:
    void ISC_CSC_Configure( struct ISC_Color_Space* cs )

  Summary:
    Color space convert with color space component.

  Remarks:
    See plib_isc.h for more details.
*/
void ISC_CSC_Configure( struct ISC_Color_Space* cs )
{
    ISC_REGS->ISC_CSC_YR_YG = ISC_CSC_YR_YG_YRGAIN(cs->yr_gain) |
                              ISC_CSC_YR_YG_YGGAIN(cs->yg_gain);
    
    ISC_REGS->ISC_CSC_YB_OY = ISC_CSC_YB_OY_YBGAIN(cs->yb_gain) |
	                          ISC_CSC_YB_OY_YOFST(cs->y_offset);
    
    ISC_REGS->ISC_CSC_CBR_CBG = ISC_CSC_CBR_CBG_CBRGAIN(cs->cbr_gain) |
	                            ISC_CSC_CBR_CBG_CBGGAIN(cs->cbg_gain); 
     
    ISC_REGS->ISC_CSC_CBB_OCB = ISC_CSC_CBB_OCB_CBBGAIN(cs->cbb_gain) |
	                            ISC_CSC_CBB_OCB_CBOFST(cs->cb_offset); 
    
    ISC_REGS->ISC_CSC_CRR_CRG = ISC_CSC_CRR_CRG_CRRGAIN(cs->crr_gain) |
	                            ISC_CSC_CRR_CRG_CRGGAIN(cs->crg_gain);
    
    ISC_REGS->ISC_CSC_CRB_OCR = ISC_CSC_CRB_OCR_CRBGAIN(cs->crb_gain) |
	                            ISC_CSC_CRB_OCR_CROFST(cs->cr_offset);
}

/******************************************************************************
  Function:
    void ISC_CSC_Enable( bool enable )

  Summary:
    Enables/disable Color Space Conversion.

  Remarks:
    See plib_isc.h for more details.
*/
void ISC_CSC_Enable( bool enable )
{
    if(enable)
        ISC_REGS->ISC_CSC_CTRL = ISC_CSC_CTRL_ENABLE_Msk;
    else
        ISC_REGS->ISC_CSC_CTRL = 0;
}

/******************************************************************************
  Function:
    void ISC_CSC_Configure( struct ISC_Color_Space* cs )

  Summary:
    Configure Contrast and brightness with given parameter.

  Remarks:
    See plib_isc.h for more details.
*/
void ISC_CBC_Configure( bool Stream_Enable, uint8_t byte_order,
                        uint16_t brightness, uint16_t contrast)
{
    if(Stream_Enable)
        ISC_REGS->ISC_CBC_CFG = ISC_CBC_CFG_CCIR_Msk | ISC_CBC_CFG_CCIRMODE(byte_order);
    else
        ISC_REGS->ISC_CBC_CFG = 0;
    
    ISC_REGS->ISC_CBC_BRIGHT = ISC_CBC_BRIGHT_BRIGHT(brightness);
    ISC_REGS->ISC_CBC_CONTRAST = ISC_CBC_CONTRAST_CONTRAST(contrast);      
}

/******************************************************************************
  Function:
    void ISC_CBC_Enable( bool enable )

  Summary:
    Enables/disable contrast and brightness control.

  Remarks:
    See plib_isc.h for more details.
*/
void ISC_CBC_Enable( bool enable )
{
    if(enable)
        ISC_REGS->ISC_CBC_CTRL = ISC_CBC_CTRL_ENABLE_Msk;
    else
        ISC_REGS->ISC_CBC_CTRL = 0;
}

/******************************************************************************
  Function:
    void ISC_Gamma_Enable( bool enable,uint8_t renable, uint8_t genable, uint8_t benable )

  Summary:
    Enables/disable Gamma Correction for the given channels.

  Remarks:
    See plib_isc.h for more details.
*/
void ISC_Gamma_Enable( bool enable ,uint8_t renable, uint8_t genable, uint8_t benable )
{
    if(enable)
        ISC_REGS->ISC_GAM_CTRL |= ISC_GAM_CTRL_ENABLE_Msk | ISC_GAM_CTRL_RENABLE(renable)|
                ISC_GAM_CTRL_GENABLE(genable)| ISC_GAM_CTRL_BENABLE(benable);
    else
        ISC_REGS->ISC_GAM_CTRL = 0;
}

/******************************************************************************
  Function:
    void ISC_CSC_Configure( struct ISC_Color_Correction* cc )

  Summary:
    Color correction with color component.

  Remarks:
    See plib_isc.h for more details.
*/
void ISC_CC_Configure( struct ISC_Color_Correction* cc)
{
    ISC_REGS->ISC_CC_RR_RG = ISC_CC_RR_RG_RRGAIN(cc->rr_gain) | ISC_CC_RR_RG_RGGAIN(cc->rg_gain);
    ISC_REGS->ISC_CC_RB_OR = ISC_CC_RB_OR_RBGAIN(cc->rb_gain) | ISC_CC_RB_OR_ROFST(cc->r_offset);   
    ISC_REGS->ISC_CC_GR_GG = ISC_CC_GR_GG_GRGAIN(cc->gr_gain) | ISC_CC_GR_GG_GGGAIN(cc->gg_gain);
    ISC_REGS->ISC_CC_GB_OG = ISC_CC_GB_OG_GBGAIN(cc->gb_gain) | ISC_CC_GB_OG_ROFST(cc->g_offset);
    ISC_REGS->ISC_CC_BR_BG = ISC_CC_BR_BG_BRGAIN(cc->br_gain) | ISC_CC_BR_BG_BGGAIN(cc->bg_gain);
    ISC_REGS->ISC_CC_BB_OB = ISC_CC_BB_OB_BBGAIN(cc->bb_gain) | ISC_CC_BB_OB_BOFST(cc->b_offset);
}

/******************************************************************************
  Function:
    void ISC_CC_Enable( bool enable )

  Summary:
    Enables/disable Color Correction.

  Remarks:
    See plib_isc.h for more details.
*/
void ISC_CC_Enable( bool enable )
{
    if(enable)
        ISC_REGS->ISC_CC_CTRL = ISC_CC_CTRL_ENABLE_Msk;
    else
        ISC_REGS->ISC_CC_CTRL = 0;
}

/******************************************************************************
  Function:
    ISC_WB_Enable ( bool enable )

  Summary:
    Enables/disable White Balance.

  Remarks:
    See plib_isc.h for more details.
*/
void ISC_WB_Enable( bool enable )
{
    if(enable)
        ISC_REGS->ISC_WB_CTRL = ISC_WB_CTRL_ENABLE_Msk;
    else
        ISC_REGS->ISC_WB_CTRL = 0;
}

/******************************************************************************
  Function:
    ISC_WB_Set_Bayer_Pattern ( uint8_t pattern )

  Summary:
    Sets the White Balance Bayer Configuration (Pixel Color Pattern).

  Remarks:
    See plib_isc.h for more details.
*/
void ISC_WB_Set_Bayer_Pattern ( uint8_t pattern )
{
    ISC_REGS->ISC_WB_CFG = pattern;  
}

/******************************************************************************
  Function:
    ISC_WB_Adjust_Color_Offset ( uint32_t b_offset, uint32_t gb_offset,
                                 uint32_t r_offset, uint32_t gr_offset )

  Summary:
    Adjusts the White Balance Bayer Color offset.

  Remarks:
    See plib_isc.h for more details.
*/
void ISC_WB_Adjust_Color_Offset ( uint32_t b_offset, uint32_t gb_offset,
                                 uint32_t r_offset, uint32_t gr_offset )
{
    ISC_REGS->ISC_WB_O_BGB = ISC_WB_O_BGB_BOFST(b_offset)  | 
                            ISC_WB_O_BGB_GBOFST(gb_offset);
    ISC_REGS->ISC_WB_O_RGR = ISC_WB_O_RGR_ROFST (r_offset) |
                            ISC_WB_O_RGR_GROFST(gr_offset);
}

/******************************************************************************
  Function:
    ISC_WB_Adjust_Color_Gain ( uint32_t b_gain, uint32_t gb_gain,
                                 uint32_t r_gain, uint32_t gr_gain )

  Summary:
    Adjusts the White Balance Bayer Color gain.

  Remarks:
    See plib_isc.h for more details.
*/
void ISC_WB_Adjust_Color_Gain ( uint32_t b_gain, uint32_t gb_gain,
                                 uint32_t r_gain, uint32_t gr_gain )
{
    ISC_REGS->ISC_WB_G_BGB = ISC_WB_G_BGB_BGAIN(b_gain)  | 
                            ISC_WB_G_BGB_GBGAIN(gb_gain);
    ISC_REGS->ISC_WB_G_RGR = ISC_WB_G_RGR_RGAIN (r_gain) |
                            ISC_WB_G_RGR_GRGAIN(gr_gain);
}

/******************************************************************************
  Function:
    ISC_CFA_Configure( uint8_t pattern, uint8_t edge )

  Summary:
    Configure color filter array interpolation.

  Remarks:
    See plib_isc.h for more details.
*/
void ISC_CFA_Configure( uint8_t pattern, uint8_t edge )
{
    ISC_REGS->ISC_CFA_CFG = ISC_CFA_CFG_BAYCFG(pattern) | ISC_CFA_CFG_EITPOL(edge);
}

/******************************************************************************
  Function:
    ISC_CFA_Enable ( bool enable )

  Summary:
    Enables/disable Color Filter Array Interpolation.

  Remarks:
    See plib_isc.h for more details.
*/
void ISC_CFA_Enable( bool enable )
{
    if(enable)
        ISC_REGS->ISC_CFA_CTRL = ISC_CFA_CTRL_ENABLE_Msk;
    else
        ISC_REGS->ISC_CFA_CTRL = 0;
}

/******************************************************************************
  Function:
    ISC_Histogram_enable( bool enable )

  Summary:
    Enables/disable Histogram.

  Remarks:
    See plib_isc.h for more details.
*/
void ISC_Histogram_enable( bool enable )
{
    if(enable)
        ISC_REGS->ISC_HIS_CTRL |= ISC_HIS_CTRL_ENABLE_Msk;
    else
        ISC_REGS->ISC_HIS_CTRL = 0;
}

/******************************************************************************
  Function:
    ISC_Histogram_Configure( uint8_t mode, uint8_t bay_sel, uint8_t reset )

  Summary:
    Configure color filter array interpolation.

  Remarks:
    See plib_isc.h for more details.
*/
void ISC_Histogram_Configure( uint8_t mode, uint8_t bay_sel, uint8_t reset )
{
    ISC_REGS->ISC_HIS_CFG = ISC_HIS_CFG_MODE(mode) | ISC_HIS_CFG_BAYSEL(bay_sel)
                            |ISC_HIS_CFG_RAR(reset);
}

/******************************************************************************
  Function:
    ISC_Histogram_Table_Update( void )

  Summary:
    Update the histogram table.

  Remarks:
    See plib_isc.h for more details.
*/
void ISC_Histogram_Table_Update( void )
{
    while((ISC_REGS->ISC_CTRLSR & ISC_CTRLSR_HISREQ_Msk) == ISC_CTRLSR_HISREQ_Msk);
    ISC_REGS->ISC_CTRLEN = ISC_CTRLEN_HISREQ_Msk;
    
}

/******************************************************************************
  Function:
    ISC_Histogram_Table_Clear( void )

  Summary:
    Clear the histogram table.

  Remarks:
    See plib_isc.h for more details.
*/
void ISC_Histogram_Table_Clear( void )
{
    while (ISC_IsSIP_Aserted());
    ISC_REGS->ISC_CTRLEN = ISC_CTRLEN_HISCLR_Msk;
}

/******************************************************************************
  Function:
    ISC_RLP_Configure( uint8_t mode, uint8_t alpha )

  Summary:
    Configure Rounding, Limiting and Packing Mode.

  Remarks:
    See plib_isc.h for more details.
*/
void ISC_RLP_Configure( uint8_t mode, uint8_t alpha )
{
    ISC_REGS->ISC_RLP_CFG = ISC_RLP_CFG_MODE(mode) | ISC_RLP_CFG_ALPHA(alpha);
}

/******************************************************************************
  Function:
    ISC_DMA_Configure_IP_Mode( uint32_t mode )

  Summary:
    Configure ISC DMA input mode.

  Remarks:
    See plib_isc.h for more details.
*/
void ISC_DMA_Configure_Register( uint8_t imode, uint8_t ysize, uint8_t csize )
{
    ISC_REGS->ISC_DCFG = ISC_DCFG_IMODE(imode) | ISC_DCFG_YMBSIZE(ysize)|
                         ISC_DCFG_CMBSIZE(csize);
}

/******************************************************************************
  Function:
    ISC_DMA_Configure_Desc( uint32_t desc_entry )

  Summary:
    Configure the next dma descriptor address.

  Remarks:
    See plib_isc.h for more details.
*/
void ISC_DMA_Configure_Desc( uint32_t desc_entry )
{
    ISC_REGS->ISC_DNDA = desc_entry;
}

/******************************************************************************
  Function:
    ISC_DMA_Enable( uint32_t mode )

  Summary:
    Configure and Enable ISC DMA.

  Remarks:
    See plib_isc.h for more details.
*/
void ISC_DMA_Enable( struct  ISC_DMA_Control_Config* ctrl )
{
    uint32_t regVal;

    regVal = ISC_REGS->ISC_DCTRL;
    regVal |= ISC_DCTRL_DE(ctrl->Descriptor_Enable);
    regVal |= ISC_DCTRL_DVIEW(ctrl->Descriptor_View);
    regVal |= ISC_DCTRL_IE(ctrl->Interrupt_Enable);
    regVal |= ISC_DCTRL_WB(ctrl->Writeback_Enable);
    regVal |= ISC_DCTRL_FIELD(ctrl->Field);
    regVal |= ISC_DCTRL_DONE(ctrl->Done);     
    ISC_REGS->ISC_DCTRL = 1;
  
    //ISC_REGS->ISC_DCTRL = regVal;
}

/******************************************************************************
  Function:
    ISC_DMA_Address( uint8_t channel, uint32_t address, uint32_t stride )

  Summary:
    Configure ISC DMA start address.

  Remarks:
    See plib_isc.h for more details.
*/
void ISC_DMA_Address( uint8_t channel, uint32_t address, uint32_t stride )
{        
    ISC_REGS->ISC_SUB0[channel].ISC_DAD = ISC_DAD_AD0(address);
    ISC_REGS->ISC_SUB0[channel].ISC_DST = ISC_DST_ST0(stride);
}

/******************************************************************************
  Function:
    ISC_Initialize ( void )

  Summary:
    Initialize the Image Sensor Controller.

  Remarks:
    See plib_isc.h for more details.
*/
void ISC_Initialize ( void )
{
    uint32_t clkcfg;
    clkcfg = ISC_REGS->ISC_CLKCFG & ~(ISC_CLKCFG_MCDIV_Msk | ISC_CLKCFG_MCSEL_Msk);
 /* Configure ISC Master Clock */
    ISC_REGS->ISC_CLKCFG = clkcfg |
                          ISC_CLKCFG_MCDIV(ISC_MASTER_CLK_DIV) |
                          ISC_CLKCFG_MCSEL(ISC_MASTER_CLK_SEL);
	
    while((ISC_REGS->ISC_CLKSR & ISC_CLKSR_SIP_Msk) == ISC_CLKSR_SIP_Msk);
    
 /* Enable ISC Master Clock */   
    ISC_REGS->ISC_CLKEN = ISC_CLKEN_MCEN_Msk;
    
 /* Configure ISP Clock */
    clkcfg = ISC_REGS->ISC_CLKCFG & ~(ISC_CLKCFG_ICDIV_Msk | ISC_CLKCFG_ICSEL_Msk);
	ISC_REGS->ISC_CLKCFG = clkcfg |
                          ISC_CLKCFG_ICDIV(ISC_ISP_CLK_DIV) |
                          (ISC_ISP_CLK_SEL << 8);
    while((ISC_REGS->ISC_CLKSR & ISC_CLKSR_SIP_Msk) == ISC_CLKSR_SIP_Msk);
    
 /* Enable ISP Clock */   
    ISC_REGS->ISC_CLKEN = ISC_CLKEN_ICEN_Msk;   
}
