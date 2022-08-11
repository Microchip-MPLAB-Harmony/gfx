#ifndef PLIB_ISC_H
#define PLIB_ISC_H
/*******************************************************************************
  ISC PLIB Header

  Company:
    Microchip Technology Inc.

  File Name:
    plib_isc.h

  Summary:
    This file provides the public declarations for the Image Sensor
    Controller.

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
#ifdef __cplusplus  // Provide C++ Compatibility
    extern "C" {
#endif
// DOM-IGNORE-END
        
#include "device.h"
#include <stddef.h>
#include <stdbool.h>
#include <device.h>

        
        
typedef void (*ISC_IRQ_CALLBACK) (uintptr_t context);

// *****************************************************************************
/* LCDC IRQ Callback Object

   Summary:
    Struct for LCDC IRQ handler

   Description:
    This structure defines the LCDC IRQ handler object, used to store the IRQ
    callback function registered from the LCDC driver

   Remarks:
    None.
*/      
typedef struct
{
    ISC_IRQ_CALLBACK callback_fn;
    uintptr_t context;
}ISC_IRQ_CALLBACK_OBJECT;        
// *****************************************************************************
/* ISC color space conversion components structure

  Summary:
    ISC CSC component.

  Description:
    This data structure defines the color space conversion components for ISC.

  Remarks:
    None.
*/
struct ISC_Color_Space{
	/* Red Gain for Luminance (signed 12 bits 1:3:8) */
	uint16_t yr_gain;
	/* Green Gain for Luminance (signed 12 bits 1:3:8)*/
	uint16_t yg_gain;
	/* Blue Gain for Luminance Component (12 bits signed 1:3:8)*/
	uint16_t yb_gain;
	/* Luminance Offset (11 bits signed 1:10:0)*/
	uint16_t y_offset;
	/* Green Gain for Blue Chrominance (signed 12 bits 1:3:8)*/
	uint16_t cbr_gain;
	/* Red Gain for Blue Chrominance (signed 12 bits, 1:3:8)*/
	uint16_t cbg_gain;
	/* Blue Gain for Blue Chrominance (signed 12 bits 1:3:8)*/
	uint16_t cbb_gain;
	/* Blue Chrominance Offset (signed 11 bits 1:10:0)*/
	uint16_t cb_offset;
	/* Red Gain for Red Chrominance (signed 12 bits 1:3:8)*/
	uint16_t crr_gain;
	/* Green Gain for Red Chrominance (signed 12 bits 1:3:8)*/
	uint16_t crg_gain;
	/* Blue Gain for Red Chrominance (signed 12 bits 1:3:8)*/
	uint16_t crb_gain;
	/* Red Chrominance Offset (signed 11 bits 1:10:0)*/
	uint16_t cr_offset;
} ;

// *****************************************************************************
/* ISC color correction components structure

  Summary:
    ISC CC component.

  Description:
    This data structure defines the color correction components for ISC.

  Remarks:
    None.
*/
struct ISC_Color_Correction{
	/* Red Component Offset (signed 13 bits, 1:12:0) */
	uint16_t r_offset;
	/* Green  Component Offset (signed 13 bits, 1:12:0) */
	uint16_t g_offset;
	/* Green Component Offset (signed 13 bits, 1:12:0) */
	uint16_t b_offset;
	/* Red Gain for Red Component (signed 12 bits, 1:3:8) */
	uint16_t rr_gain;
	/* Green Component (Red row) Gain (unsigned 13 bits, 0:4:9) */
	uint16_t rg_gain;
	/* Blue Gain for Red Component (signed 12 bits, 1:3:8) */
	uint16_t rb_gain;
	/* Green Gain for Green Component (signed 12 bits, 1:3:8)*/
	uint16_t gg_gain;
	/* Red Gain for Green Component (signed 12 bits, 1:3:8) */
	uint16_t gr_gain;
	/* Blue Gain for Green Component (signed 12 bits, 1:3:8) */
	uint16_t gb_gain;
	/* Green Gain for Blue Component (signed 12 bits, 1:3:8) */
	uint16_t bg_gain;
	/* Red Gain for Blue Component (signed 12 bits, 1:3:8) */
	uint16_t br_gain;
	/* Blue Gain for Blue Component (signed 12 bits, 1:3:8) */
	uint16_t bb_gain;
} ;


// *****************************************************************************
/* ISC DMA control register configuration

  Summary:
    ISC DMA component.

  Description:
    This data structure defines the configuration for DMA Control Register for ISC.

  Remarks:
    None.
*/
struct ISC_DMA_Control_Config{
    /* Enable DMA descriptor */
    uint8_t Descriptor_Enable : 1;
    /* Descriptor View : Packed, semiplanar, planar */
    uint8_t Descriptor_View : 2; 
    /* DMA Done interrupt enable */
    uint8_t Interrupt_Enable : 1;
    /* Write back operation is enabled */
    uint8_t Writeback_Enable : 1;
    /* Value of Captured Frame Field Signal */
    uint8_t Field : 1;
    /* Descriptor Processing Status */
    uint8_t Done : 1;
};

// *****************************************************************************
/* ISC DMA Descriptor

  Summary:
    ISC DMA descriptor.

  Description:
    Structure for ISC DMA descriptor view0 that can be performed when the pixel 
    or data stream is packed.

  Remarks:
    None.
*/
struct ISC_DMA_View{
    /* ISC DMA Control */
    uint32_t ctrl;
    /* Next ISC DMA Descriptor Address number */
    uint32_t next_desc;
    /* Transfer Address */
    uint32_t addr;
    /* Stride */
    uint32_t stride;
};

// *****************************************************************************
/* Function:
    void ISC_Software_Reset ( void )

  Summary:
    Resets ISC.

  Description:
    Perform software reset of the ISC interface.

  Precondition:
    None.

  Parameters:
    None.
 
  Returns:
    None.

  Example:
    <code>
    ISC_Software_Reset();
    </code>

  Remarks:
    None.
*/
void ISC_Software_Reset ( void );

// *****************************************************************************
/* Function:
    bool ISC_IsSIP_Aserted( void )

  Summary:
    Returns ISC_CTRLSR.SIP bit.

  Description:
    Checks if the double domain synchronization is in progress.

  Precondition:
    None.

  Parameters:
    None.
 
  Returns:
    The double domain synchronization in progress status.

  Example:
    <code>
    bool sip_status;
    sip_status = ISC_IsSIP_Aserted();
    </code>

  Remarks:
    None.
*/
bool ISC_IsSIP_Aserted ( void );

// *****************************************************************************
/* Function:
    void ISC_Start_Capture(void)

  Summary:
    Starts ISC Capture.

  Description:
    Send Capture Input Stream Command to start a single shot capture or multiple frames.

  Precondition:
    Must wait till ISC_CTRLSR.SIP bit is cleared.

  Parameters:
    None.
 
  Returns:
    None.

  Example:
    <code>
    ISC_Start_Capture();
    </code>

  Remarks:
    None.
*/
void ISC_Start_Capture(void);

// *****************************************************************************
/* Function:
    void ISC_Stop_Capture(void)

  Summary:
    Ends ISC Capture.

  Description:
    End the capture at the next Vertical Synchronization Detection.

  Precondition:
    Must wait till ISC_CTRLSR.SIP bit is cleared.

  Parameters:
    None.
 
  Returns:
    None.

  Example:
    <code>
    ISC_Stop_Capture();
    </code>

  Remarks:
    None.
*/
void ISC_Stop_Capture(void);

// *****************************************************************************
/* Function:
    uint32_t ISC_Get_Ctrl_Status( void )

  Description:
    Returns ISC Control Status.

  Precondition:
    None.

  Parameters:
    None.
 
  Returns:
    ISC Control Status.

  Example:
    <code>
    uint32_t isc_ctl_status;
    isc_ctl_status = ISC_Get_Ctrl_Status();
    </code>

  Remarks:
    None.
*/
uint32_t ISC_Get_Ctrl_Status( void );

// *****************************************************************************
/* Function:
    void ISC_Update_Profile( void );

  Description:
    Update the color profiles.

  Precondition:
    None.

  Parameters:
    None.
 
  Returns:
    None.

  Example:
    <code>
    ISC_Update_Profile();
    </code>

  Remarks:
    None.
*/
void ISC_Update_Profile( void );

// *****************************************************************************
/* Function:
    void ISC_Enable_Interrupt( uint32_t flag )

  Description:
    Enable ISC Interrupt.

  Precondition:
    None.

  Parameters:
    flag - All the interrupts that need to be enabled for ISC.
 
  Returns:
    None.

  Example:
    <code>
    ISC_Enable_Interrupt(ISC_INTEN_VD_Msk | ISC_INTEN_HISDONE_Msk);
    </code>

  Remarks:
    None.
*/
void ISC_Enable_Interrupt( uint32_t flag );

// *****************************************************************************
/* Function:
    void ISC_Disable_Interrupt( uint32_t flag )

  Description:
    Disable ISC Interrupt.

  Precondition:
    None.

  Parameters:
    flag - All the ISC interrupts that need to be disabled.
 
  Returns:
    None.

  Example:
    <code>
    ISC_Disable_Interrupt(ISC_INTEN_VD_Msk | ISC_INTEN_HISDONE_Msk);
    </code>

  Remarks:
    None.
*/
void ISC_Disable_Interrupt( uint32_t flag );

// *****************************************************************************
/* Function:
    uint32_t ISC_Get_Interrupt_Status( void )

  Description:
    Returns ISC Interrupt status.

  Precondition:
    None.

  Parameters:
    None.
 
  Returns:
    Interrupt Status.

  Example:
    <code>
    uint32_t isc_intr_status;
    isc_intr_status = ISC_Get_Interrupt_Status();
    </code>

  Remarks:
    None.
*/
uint32_t ISC_Get_Interrupt_Status( void );

// *****************************************************************************
/* Function:
    void ISC_PFE_Set_Video_Mode( uint32_t vmode )

  Description:
    Configure PFE(Parallel Front End) video mode.

  Precondition:
    None.

  Parameters:
    PFE Mode.
 
  Returns:
    None.

  Example:
    <code>
    isc_pfe_set_video_mode(ISC_PFE_CFG0_MODE_PROGRESSIVE);
    </code>

  Remarks:
    None.
*/
void ISC_PFE_Set_Video_Mode( uint32_t vmode );

// *****************************************************************************
/* Function:
    void ISC_PFE_Set_Sync_Polarity( uint32_t hpol, uint32_t vpol )

  Description:
    set PFE(Parallel Front End) H/V synchronization polarity.

  Precondition:
    None.

  Parameters:
    hpol - Horizontal Polarity.
    vpol - Vertical Polarity.
 
  Returns:
    None.

  Example:
    <code>
    ISC_PFE_Set_Sync_Polarity(0, ISC_PFE_CFG0_VPOL_Msk);
    </code>

  Remarks:
    None.
*/
void ISC_PFE_Set_Sync_Polarity( uint32_t hpol, uint32_t vpol );


// *****************************************************************************
/* Function:
    void ISC_PFE_Set_Pixel_Polarity( uint32_t ppol )

  Description:
   Set PFE(Parallel Front End) pixel clock polarity. The pixel stream is sampled on 
   the falling edge of the pixel clock when set.

  Precondition:
    None.

  Parameters:
    ppol - Pixel Polarity.
    
  Returns:
    None.

  Example:
    <code>
    ISC_PFE_Set_Pixel_Polarity(ISC_PFE_CFG0_PPOL_Msk);
    </code>

  Remarks:
    None.
*/
void ISC_PFE_Set_Pixel_Polarity( uint32_t ppol );

// *****************************************************************************
/* Function:
    void ISC_PFE_Set_Field_Polarity( uint32_t fpol )

  Description:
   Set PFE(Parallel Front End) field polarity.

  Precondition:
    None.

  Parameters:
    fpol - Field Polarity.
    
  Returns:
    None.

  Example:
    <code>
    ISC_PFE_Set_Pixel_Polarity(ISC_PFE_CFG0_FPOL_Msk);
    </code>

  Remarks:
    None.
*/
void ISC_PFE_Set_Field_Polarity( uint32_t fpol );

// *****************************************************************************
/* Function:
    void ISC_PFE_Set_BPS( uint32_t bps )

  Description:
   set PFE(Parallel Front End) bps.

  Precondition:
    None.

  Parameters:
    bps - Bits Per Sample.
    
  Returns:
    None.

  Example:
    <code>
    ISC_PFE_Set_BPS(ISC_PFE_CFG0_BPS_EIGHT);
    </code>

  Remarks:
    None.
*/
void ISC_PFE_Set_BPS( uint32_t bps );

// *****************************************************************************
/* Function:
    void ISC_PFE_Set_Continuous_Mode( bool enable )

  Description:
   Set PFE(Parallel Front End)in continuous mode.

  Precondition:
    None.

  Parameters:
    enable/disable flag.
    
  Returns:
    None.

  Example:
    <code>
    ISC_PFE_Set_Continuous_Mode(true);
    </code>

  Remarks:
    None.
*/
void ISC_PFE_Set_Continuous_Mode( bool enable );

// *****************************************************************************
/* Function:
    void ISC_SUB422_Enable( bool enable )

  Description:
   Enables/disable 4:4:4 to 4:2:2 Chrominance Horizontal Subsampling Filter Enable.

  Precondition:
    None.

  Parameters:
    enable/disable flag.
    
  Returns:
    None.

  Example:
    <code>
    ISC_SUB422_Enable(true);
    </code>

  Remarks:
    None.
*/
void ISC_SUB422_Enable( bool enable );

// *****************************************************************************
/* Function:
    void ISC_SUB420_Configure( bool enable, uint8_t filter )

  Description:
   Enables/disable 4:2:2 to 4:2:0 Chrominance Vertical Subsampling Filter.

  Precondition:
    None.

  Parameters:
    enable - enable/disable flag.
    filter - Interlaced or progressive filter.
    0: Progressive filter {0.5, 0.5}
    1: Field-dependent filter, top field filter is {0.75, 0.25},
			bottom field filter is {0.25, 0.75}
    
  Returns:
    None.

  Example:
    <code>
    ISC_SUB422_Enable(true, false);
    </code>

  Remarks:
    None.
*/
void ISC_SUB420_Enable( bool enable, bool filter );

// *****************************************************************************
/* Function:
    void ISC_CSC_Configure( struct ISC_Color_Space* cs )

  Description:
   Color space convert with color space component.

  Precondition:
    None.

  Parameters:
    Pointer to structure ISC_Color_Space
    
  Returns:
    None.

  Example:
    <code>
    
    static struct _color_space ref_cs = {
	0x42, 0x81, 0x19, 0x10, 0xFDA, 0xFB6, 0x70, 0x80, 0x70, 0xFA2, 0xFEE, 0x80};  

    ISC_CSC_Configure(&ref_cs);
    </code>

  Remarks:
    None.
*/
void ISC_CSC_Configure( struct ISC_Color_Space* cs );

// *****************************************************************************
/* Function:
    void ISC_CSC_Enable( bool enable )

  Description:
   Enables/disable Color Space Conversion.

  Precondition:
    None.

  Parameters:
    enable - enable/disable flag.
       
  Returns:
    None.

  Example:
    <code>
    ISC_CSC_Enable(true);
    </code>

  Remarks:
    None.
*/
void ISC_CSC_Enable( bool enable );

// *****************************************************************************
/* Function:
    void ISC_CBC_Configure( bool Stream_Enable, uint8_t byte_order,
                        uint16_t brightness, uint16_t contrast)

  Description:
   Configure Contrast and brightness with given parameter.

  Precondition:
    None.

  Parameters:
    enable - enable/disable flag.
       
  Returns:
    None.

  Example:
    <code>
    ISC_CBC_Configure(false, 0, 0xF, 0x10);
    </code>

  Remarks:
    None.
*/
void ISC_CBC_Configure( bool Stream_Enable, uint8_t byte_order,
                        uint16_t brightness, uint16_t contrast);

// *****************************************************************************
/* Function:
    void ISC_CBC_Enable( bool enable )

  Description:
   Enables/disable contrast and brightness control.

  Precondition:
    None.

  Parameters:
    enable - enable/disable flag.
       
  Returns:
    None.

  Example:
    <code>
    ISC_CBC_Enable(true);
    </code>

  Remarks:
    None.
*/
void ISC_CBC_Enable( bool enable );

// *****************************************************************************
/* Function:
    void ISC_Gamma_Enable( bool enable ,uint8_t renable, uint8_t genable, uint8_t benable )

  Description:
   Enables/disable Gamma Correction for the given channels.

  Precondition:
    None.

  Parameters:
    enable - enable/disable flag.
    benable - Piecewise interpolation is used to perform 12 bits to 10 bits compression 
                for the blue channel
    genable - Piecewise interpolation is used to perform 12 bits to 10 bits compression 
                for the green channel
    renable - Piecewise interpolation is used to perform 12 bits to 10 bits compression 
                for the red channel
       
  Returns:
    None.

  Example:
    <code>
    ISC_Gamma_Enable(true, ISC_GAM_CTRL_BENABLE_Msk, ISC_GAM_CTRL_GENABLE_Msk
                       ISC_GAM_CTRL_RENABLE_Msk);
    </code>

  Remarks:
    None.
*/
void ISC_Gamma_Enable( bool enable ,uint8_t renable, uint8_t genable, uint8_t benable );

// *****************************************************************************
/* Function:
    void ISC_CBC_Configure( struct ISC_Color_Correction* cc )

  Description:
   Color correction with color component.

  Precondition:
    None.

  Parameters:
    enable - enable/disable flag.
       
  Returns:
    None.

  Example:
    <code>
    ISC_CBC_Configure(false, 0, 0xF, 0x10);
    </code>

  Remarks:
    None.
*/
void ISC_CC_Configure( struct ISC_Color_Correction* cc);

// *****************************************************************************
/* Function:
    void ISC_CC_Enable( bool enable )

  Description:
   Enables/disable Color Correction.

  Precondition:
    None.

  Parameters:
    enable - enable/disable flag.
       
  Returns:
    None.

  Example:
    <code>
    ISC_CC_Enable(true);
    </code>

  Remarks:
    None.
*/
void ISC_CC_Enable( bool enable );

// *****************************************************************************
/* Function:
    void ISC_WB_Enable( bool enable )

  Description:
   Enables/disable White Balance.

  Precondition:
    None.

  Parameters:
    enable - enable/disable flag.
       
  Returns:
    None.

  Example:
    <code>
    ISC_WB_Enable(true);
    </code>

  Remarks:
    None.
*/
void ISC_WB_Enable( bool enable );


// *****************************************************************************
/* Function:
    void ISC_WB_Set_Bayer_Pattern ( uint8_t pattern )

  Description:
   Sets the White Balance Bayer Configuration (Pixel Color Pattern).

  Precondition:
    None.

  Parameters:
    pattern - Pixel Color Pattern.
       
  Returns:
    None.

  Example:
    <code>
    ISC_WB_Set_Bayer_Pattern(ISC_WB_CFG_BAYCFG_BGBG);
    </code>

  Remarks:
    None.
*/
void ISC_WB_Set_Bayer_Pattern ( uint8_t pattern );

// *****************************************************************************
/* Function:
    void ISC_WB_Adjust_Color_Offset ( uint32_t b_offset, uint32_t gb_offset,
                                 uint32_t r_offset, uint32_t gr_offset )

  Description:
   Adjusts the White Balance Bayer Color offset.

  Precondition:
    None.

  Parameters:
    b_offset - Offset Blue Component (signed 13 bits, 1:12:0).
    gb_offset - Offset Green Component for Blue Row (signed 13 bits, 1:12:0)
    r_offset - Offset Red Component (signed 13 bits 1:12:0)
    gr_offset - Offset Green Component for Red Row (signed 13 bits 1:12:0)
       
  Returns:
    None.

  Example:
    <code>
    ISC_WB_Adjust_Color_Offset(0, 0, 0, 0);
    </code>

  Remarks:
    None.
*/
void ISC_WB_Adjust_Color_Offset ( uint32_t b_offset, uint32_t gb_offset,
                                 uint32_t r_offset, uint32_t gr_offset );

// *****************************************************************************
/* Function:
    void ISC_WB_Adjust_Color_Gain ( uint32_t b_gain, uint32_t gb_gain,
                                 uint32_t r_gain, uint32_t gr_gain )

  Description:
   Adjusts the White Balance Bayer Color gain.

  Precondition:
    None.

  Parameters:
    b_gain - Blue Component Gain (unsigned 13 bits, 0:4:9)
    gb_gain - Green Component (Blue row) Gain (unsigned 13 bits, 0:4:9)
    r_gain - Red Component Gain (unsigned 13 bits, 0:4:9)
    gr_gain - Green Component (Red row) Gain (unsigned 13 bits, 0:4:9)
       
  Returns:
    None.

  Example:
    <code>
    ISC_WB_Adjust_Color_Gain(0x200, 0x200, 0x200, 0x200);
    </code>

  Remarks:
    None.
*/
void ISC_WB_Adjust_Color_Gain ( uint32_t b_gain, uint32_t gb_gain,
                                 uint32_t r_gain, uint32_t gr_gain );

// *****************************************************************************
/* Function:
    void ISC_CFA_Configure( uint8_t pattern, uint8_t edge )

  Description:
   Configure color filter array interpolation.

  Precondition:
    None.

  Parameters:
    pattern - Pixel Color Pattern.
       
  Returns:
    None.

  Example:
    <code>
    ISC_CFA_Configure(ISC_CFA_CFG_BAYCFG_BGBG_Val, 1);
    </code>

  Remarks:
    None.
*/
void ISC_CFA_Configure( uint8_t pattern, uint8_t edge );

// *****************************************************************************
/* Function:
    void ISC_CFA_Enable( bool enable )

  Description:
   Enables/disable Color Filter Array Interpolation.

  Precondition:
    None.

  Parameters:
    enable - enable/disable flag.
       
  Returns:
    None.

  Example:
    <code>
 v
    </code>

  Remarks:
    None.
*/
void ISC_CFA_Enable( bool enable );

// *****************************************************************************
/* Function:
    void ISC_Histogram_enable( bool enable )

  Description:
   Enables/disable Histogram.

  Precondition:
    None.

  Parameters:
    enable - enable/disable flag.
       
  Returns:
    None.

  Example:
    <code>
    ISC_Histogram_enable(true);
    </code>

  Remarks:
    None.
*/
void ISC_Histogram_enable( bool enable );

// *****************************************************************************
/* Function:
    void ISC_Histogram_Configure( uint8_t mode, uint8_t bay_sel, uint8_t reset )

  Description:
   Configure Histogram.

  Precondition:
    None.

  Parameters:
    mode - Histogram Operating Mode.
    baySel - Bayer Color Component Selection.
    reset - Histogram Reset After Read.
            0: Reset after read mode is disabled
			1: Reset after read mode is enabled.
       
  Returns:
    None.

  Example:
    <code>
    ISC_Histogram_Configure(ISC_HIS_CFG_MODE_RAW, ISC_HIS_CFG_BAYSEL_BGBG_Val, 1);
    </code>

  Remarks:
    None.
*/
void ISC_Histogram_Configure( uint8_t mode, uint8_t bay_sel, uint8_t reset );

// *****************************************************************************
/* Function:
    void ISC_Histogram_Table_Update( void )

  Description:
   Update the histogram table.

  Precondition:
    The Histogram table can't be updated when a histogram request is pending.

  Parameters:
    None.
       
  Returns:
    None.

  Example:
    <code>
    ISC_Histogram_Table_Update();
    </code>

  Remarks:
    None.
*/
void ISC_Histogram_Table_Update( void );

// *****************************************************************************
/* Function:
    void ISC_Histogram_Table_Clear( void )

  Description:
   Clear the histogram table.

  Precondition:
    The Histogram table can't be cleared when SIP flag is set.

  Parameters:
    None.
       
  Returns:
    None.

  Example:
    <code>
    ISC_Histogram_Table_Clear();
    </code>

  Remarks:
    None.
*/
void ISC_Histogram_Table_Clear( void );

// *****************************************************************************
/* Function:
    void ISC_RLP_Configure( uint8_t mode, uint8_t alpha )

  Description:
   Configure Rounding, Limiting and Packing Mode.

  Precondition:
    None.

  Parameters:
    None.
       
  Returns:
    None.

  Example:
    <code>
    ISC_RLP_Configure(ISC_RLP_CFG_MODE_RGB565_Val, 0);
    </code>

  Remarks:
    None.
*/
void ISC_RLP_Configure( uint8_t mode, uint8_t alpha );

// *****************************************************************************
/* Function:
    void ISC_DMA_Configure_Register( uint8_t imode, uint8_t ysize, uint8_t csize )

  Description:
   Configure ISC DMA.

  Precondition:
    None.

  Parameters:
    mode - DMA Input Mode Selection.
    ysize - DMA Memory Burst Size Y channel
    csize - DMA Memory Burst Size C channel
       
  Returns:
    None.

  Example:
    <code>
    ISC_DMA_Configure_Register(ISC_DCFG_IMODE_PACKED16, ISC_DCFG_YMBSIZE_BEATS8_Val,
                               ISC_DCFG_CMBSIZE_SINGLE_Val);
    </code>

  Remarks:
    None.
*/
void ISC_DMA_Configure_Register( uint8_t imode, uint8_t ysize, uint8_t csize );

// *****************************************************************************
/* Function:
    void ISC_DMA_Configure_Desc( uint32_t desc_entry )

  Description:
   Configure the next dma descriptor address.

  Precondition:
    None.

  Parameters:
    descEntry - entry of DMA descriptor VIEW.
       
  Returns:
    None.

  Example:
    <code>
    CACHE_ALIGNED static union {
        struct _isc_dma_view0 view0[ISCD_MAX_DMA_DESC];
        struct _isc_dma_view1 view1[ISCD_MAX_DMA_DESC];
        struct _isc_dma_view2 view2[ISCD_MAX_DMA_DESC];
    } _isc_dma_view_pool;

    struct _isc_dma_view0* dma_view0;

    dma_view0 = _isc_dma_view_pool.view0;
		for (i = 0; i < desc->cfg.multi_bufs; i++) {
			dma_view0[i].ctrl = ISC_DCTRL_DVIEW_PACKED | ISC_DCTRL_DE;
			dma_view0[i].next_desc = (uint32_t)&dma_view0[i + 1];
			dma_view0[i].addr = (uint32_t)desc->dma.address0 + i * desc->dma.size;
			dma_view0[i].stride = 0;
		}
		dma_view0[i - 1].next_desc = (uint32_t)&dma_view0[0];
   
    ISC_DMA_Configure_Desc( (uint32_t)&dma_view0[0] );
    </code>

  Remarks:
    None.
*/
void ISC_DMA_Configure_Desc( uint32_t desc_entry );

// *****************************************************************************
/* Function:
   void ISC_DMA_Enable( struct  ISC_DMA_Control_Config* ctrl )

  Description:
   Configure and Enable ISC DMA.

  Precondition:
    None.

  Parameters:
    ctrl - Pointer to DMA Control Configuration register.
        
  Returns:
    None.

  Example:
    <code>
    struct  ISC_DMA_Control_Config ctrl;
    ctrl.Descriptor_Enable = 1;
    ctrl.Descriptor_View = 0;
    ctrl.Interrupt_Enable = 1;
    ctrl.Writeback_Enable = 0;
    ctrl.Field = 0;
    ctrl.Done = 0;
    ISC_DMA_Enable(&ctrl);
    </code>

  Remarks:
    None.
*/
void ISC_DMA_Enable( struct  ISC_DMA_Control_Config* ctrl );

// *****************************************************************************
/* Function:
   void ISC_DMA_Address( uint8_t channel, uint32_t address, uint32_t stride )

  Description:
   Configure ISC DMA start address.

  Precondition:
    None.

  Parameters:
    channel - channel number.
    address - address for given channel.
    stride - stride for given channel.
        
  Returns:
    None.

  Example:
    <code>
    
    ISC_DMA_Address(channel, address, stride);
    </code>

  Remarks:
    None.
*/
void ISC_DMA_Address( uint8_t channel, uint32_t address, uint32_t stride );

// *****************************************************************************
/* Function:
    void ISC_Initialize(void)

  Summary:
    Initializes ISC.

  Description:
    This function Initializes the ISC by configuring and enabling clocks, etc.

  Precondition:
    None.

  Parameters:
    None.
 
  Returns:
    None.

  Example:
    <code>
    ISC_Initialize();
    </code>

  Remarks:
    None.
*/
void ISC_Initialize(void);

// DOM-IGNORE-BEGIN
#ifdef __cplusplus  // Provide C++ Compatibility
    }
#endif
// DOM-IGNORE-END
    
#endif // PLIB_ISC_H
