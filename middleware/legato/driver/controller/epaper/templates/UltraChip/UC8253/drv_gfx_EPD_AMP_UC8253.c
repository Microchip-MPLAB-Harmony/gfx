/**
 * @file drv_gfx_EPD_AMP_UC8253.c
 * @brief Custom controller for AMP EPD displays using UC8253
 * @details
 *    This file contains the source code for using the
 *    Waveshare controller with Legato.
 * 
 * The original data transfer operations, 
 * generic send_cmd() and send_data() functions, are
 * replaced by predefined byte sequence arrays
 * to compress data transfers and provide best speed.
 * The original code sequences are provided for reference.
 * 
 * Original code was provided by the display vendor in August, 2025
 * by multiple email. The code was basically examples for 
 * sending fixed-pattern buffers to demonstrate operation.
 * Sometimes the examples were for a 240x360 version of the display
 * and this code reflects that variation.
 */

 /*******************************************************************************
* Copyright (C) 2026 Microchip Technology Inc. and its subsidiaries.
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


#include "definitions.h"

#include "../../drv_gfx_EPD_config.h"
#include "./drv_gfx_EPD_AMP_UC8253_local.h"

#include "../../drv_gfx_EPD_controller_local.h"
#include "./drv_gfx_EPD_AMP_UC8253_LUT.h"
#include "system/time/sys_time.h"
#define ALENGTH(x)  (sizeof(x)/sizeof(x[0]))

#include <stdint.h>
#include <xc.h>
#define NOP()   _nop()

/* Test options. */
#ifndef AMP_UC8253_setPartial_TEST_DISPLAY
#define AMP_UC8253_setPartial_TEST_DISPLAY   0
#endif

/* TODO: route this through the SPI interface layer. */
#ifdef GFX_DISP_INTF_PIN_RESET_Clear
#define AMP_UC8253_Reset_Assert()      GFX_DISP_INTF_PIN_RESET_Clear()
#define AMP_UC8253_Reset_Deassert()    GFX_DISP_INTF_PIN_RESET_Set()

/* EPD standard is that MOSI and MISO are the same wire,
 * but PIC32 SERCOM does not implicitly support half-duplex operation.
 * Consult the PIC32CM_magic folder for a work-around. */
#define READ_STOP           0
#define READ_REVISION       0
#define READ_STATUS         0
#define nBUSY_Get()     (GFX_DISP_INTF_PIN_nBUSY_Get()? 0:1)
#else
#error "ERROR: GFX_DISP_INTF_PIN_RESET not defined. Please define in Pin Manager."
#define AMP_UC8253_Reset_Assert()
#define AMP_UC8253_Reset_Deassert()
#endif

/* Set or clear CS indirectly through the SPI interface layer. */
#define AMP_UC8253_NCSAssert(intf)   GFX_Disp_Intf_PinControl(intf, \
                                    GFX_DISP_INTF_PIN_CS, \
                                    GFX_DISP_INTF_PIN_CLEAR)
#define AMP_UC8253_NCSDeassert(intf) GFX_Disp_Intf_PinControl(intf, \
                                    GFX_DISP_INTF_PIN_CS, \
                                    GFX_DISP_INTF_PIN_SET)

/* Set or clear DC indirectly through the SPI interface layer. */
#define AMP_UC8253_DCasCommand(intf)   GFX_Disp_Intf_PinControl(intf, \
                                    GFX_DISP_INTF_PIN_RSDC, \
                                    GFX_DISP_INTF_PIN_CLEAR)
#define AMP_UC8253_DCDasData(intf) GFX_Disp_Intf_PinControl(intf, \
                                    GFX_DISP_INTF_PIN_RSDC, \
                                    GFX_DISP_INTF_PIN_SET)

/* ************************************************************************** */

#if defined(DRV_GFX_PROGRAMMABLE_LUT)
const DRV_EPD_lut_t drv_gfx_EPD_LUT[] = 
{
    AMP_UC8253_LUT_GC,    // slow refresh must be #0
    AMP_UC8253_LUT_DU2,   // then the one we like
    AMP_UC8253_LUT_DU,    // others are for experimentation
};
#endif

/* ************************************************************************** */

static void AMP_UC8253_display_update(GFX_Disp_Intf intf) //klk
{
    /* Per vendor 31 July 2025 
     * Vendor's name for this function is "display_update" */
    GFX_Disp_Intf_WriteCommand(intf, 0x04); // POWER_ON
    DRV_EPD_waitBusy(0);
    GFX_Disp_Intf_WriteCommand(intf, 0x12); // DRF Display Refresh
    DRV_EPD_waitBusy(0);
    GFX_Disp_Intf_WriteCommand(intf, 0x02); // POWER_OFF
    DRV_EPD_waitBusy(0);
}

static gfxResult AMP_UC8253_setPartial(const GFX_Disp_Intf intf, const uint8_t pt_scan,
          const int32_t x, const int32_t y, const gfxPixelBuffer* const buf)
{
//	MCU_S8_S9_write_cmd(0x90);
//	MCU_S8_S9_write_data(0);
//	MCU_S8_S9_write_data(0xEF);
//	MCU_S8_S9_write_data(0);
//	MCU_S8_S9_write_data(0);		
//	MCU_S8_S9_write_data(0x01);
//	MCU_S8_S9_write_data(0x67);
//	MCU_S8_S9_write_data(1);
    /* struct window {
            uint8_t  x_start, x_end 
            uint16_t y_start, y_end but big-endian 
            uint8_t  pt_scan:1
       } length = 7 bytes
     */
    register uint32_t y_stop = (y + buf->size.height)-1;
    uint8_t window[7];
    window[0] = 0xff & x;
    window[1] = 0xff & ((x + buf->size.width)-1);
    window[2] = 0xff & (y >> 8);
    window[3] = 0xff & y;
    window[4] = 0xff & (y_stop >> 8);
    window[5] = 0xff & y_stop;
    window[6] = pt_scan;
    
    GFX_Disp_Intf_WriteCommandParm(intf, 0x90, // PTL Partial window
                          (uint8_t *)window, ALENGTH(window));
    return GFX_SUCCESS;
}

#if defined(DRV_GFX_PROGRAMMABLE_LUT)
/** LUT selection algorithm.
 * @param fullPanelUpdate The GC option is only allowed  
 * */
static DRV_EPD_lut_t AMP_UC8253_find_lut(void)
{
    DRV_EPD_lut_t answer = NULL;
    
    if (ALENGTH(drv_gfx_EPD_LUT) > drv.refreshFastControlTable)
    {
        answer = drv_gfx_EPD_LUT[drv.refreshFastControlTable];
    }
    return answer;
}
#endif


gfxResult AMP_UC8253_blit(const GFX_Disp_Intf intf, 
             DRV_transferFunction_t * const transfer, // delegate
                     const avdisplay_load_t fullPartial, 
                   const avdisplay_buffer_t target,
    const int32_t x, const int32_t y, const gfxPixelBuffer * const buffer)
{
    /* This display requires that the given frame consists 
     * of whole bytes-full of pixel bits. The screen can't
     * handler partial bytes.
     * The Y dimension is not part of this decision. */
    if ((0 != (x%8)) || (7 != ((x+buffer->size.width-1)%8)))
    { return GFX_FAILURE; }
    
    AMP_UC8253_NCSAssert(intf);

#if PIXEL_BUFFER_COLOR_MODE == GFX_COLOR_MODE_MONOCHROME
    if (0 != (x%8)) // must be byte-aligned horizontally
    { while(1); }
    NOP(); NOP(); NOP();
#endif
    
    if (AVDISPLAY_LOAD_NONE == fullPartial)
    {
        /* Simply write the image to the display. */
        GFX_Disp_Intf_WriteCommand(intf, target);
    }
    else 
    {
        DRV_EPD_lut_t lut = AMP_UC8253_find_lut();
        if (NULL == lut) 
        { return GFX_FAILURE; }
        
        AMP_UC8253_send_lut(intf,lut);

        if (AVDISPLAY_LOAD_PARTIAL == fullPartial)
        {
            //	MCU_S8_S9_write_cmd(0x91);
            GFX_Disp_Intf_WriteCommand(intf, 0x91); // PTIN Partial In
            AMP_UC8253_setPartial(intf, 0x01, x, y, buffer);

            //	MCU_S8_S9_write_cmd(0x13);
            GFX_Disp_Intf_WriteCommand(intf, 
                (AVDISPLAY_BUFFER_NONE == target)? AVDISPLAY_BUFFER_NEWorRED:target);
        }
        else if (AVDISPLAY_LOAD_FULL_DISPLAY == fullPartial)
        {
            GFX_Disp_Intf_WriteCommand(intf, 0x92); // PTOUT Partial Out
            //	MCU_S8_S9_write_cmd(0x10);
            GFX_Disp_Intf_WriteCommand(intf,
                (AVDISPLAY_BUFFER_NONE == target)? AVDISPLAY_BUFFER_OLDorBW:target);
        }
    }
    
    /* Round-up the pixel count for to send partial bytes as whole. */
    if (buffer->pixels && transfer)
    { (*transfer)(intf, buffer->pixels, (buffer->buffer_length+0)/1); }
    
#if READ_STOP
    uint8_t stopData; (void)stopData;
    GFX_Disp_Intf_WriteCommand(intf, 0x11); // Display stop
    GFX_Disp_Intf_ReadData(intf,&stopData,1);
#endif

    /* Important: update the display while still in Partial mode
     * (if we're there at all), lest you do a full refresh. */
    AMP_UC8253_display_update(intf);
    
    if (AVDISPLAY_LOAD_PARTIAL == fullPartial)
    {
        //	MCU_S8_S9_write_cmd(0x92);
//        GFX_Disp_Intf_WriteCommand(intf, 0x92); // PTOUT Partial Out
    }

    AMP_UC8253_NCSDeassert(intf);
    return GFX_SUCCESS;
}


static int AMP_UC8253_Configure(GFX_Disp_Intf intf, uint8_t useLUTmemory)
{
    /* Just completed a HW reset; hold for a moment. */
    DRV_EPD_waitBusy(10);
    
    /* This initialization sequence is provided by the display vendor
     * in email 1 August 2025. The default values are taken from
     * data sheet rev.1.2, May 30, 2022, except as noted.
     * 
     * Caveat: at this time we cannot fetch data from the display.
     */
    
#if READ_REVISION
    uint8_t revision[4]; (void)revision[0];
    GFX_Disp_Intf_WriteCommand(intf, 0x43);
    GFX_Disp_Intf_WriteCommand(intf, 0xFF);
    DRV_EPD_waitBusy(0);
    GFX_Disp_Intf_ReadData(intf,revision,ALENGTH(revision));
#endif
    
//	MCU_S8_S9_write_cmd(0x00);    //panel setting 
//	MCU_S8_S9_write_data(0xFF); //  ,LUT from,kw mode,scan up,shift right,booster on 
//	MCU_S8_S9_write_data(0x0D); //  ,LUT from,kw mode,scan up,shift right,booster on 	
    /* RES = 11 up to 480
     * REG = 0  LUT from OTP
     * KWR = 1  black/white KW mode (not KWR red)
     * UD  = 1  scan up
     * SHL = 1  shift right 
     * SHD_N = 1    booster on
     * RST_N = 1    no soft reset
     * VCMZ  = 0    VCOM state 0 (no effect)
     * TS_AUTO = 1  activate temperature sensor before boost
     * TIEG = 1     tie VGL to gnd after booster-off
     * NORG = 1     expect refresh display
     * VC_LUTZ = 1  float VCOM after reset 
     * */
//  static const uint8_t panelDefault[] = { 0x0F, 0x0D }; // typo in DS
    static const uint8_t panel240x416otp[] = { 0xDF, 0x0D };
    static const uint8_t panel240x416reg[] = { 0xFF, 0x0D };
    if (useLUTmemory)
    { GFX_Disp_Intf_WriteCommandParm(intf, 0x00, // PANEL_SETTING PSRs
                    (uint8_t *)panel240x416reg, ALENGTH(panel240x416reg)); }
    else
    { GFX_Disp_Intf_WriteCommandParm(intf, 0x00, // PANEL_SETTING PSRs
                    (uint8_t *)panel240x416otp, ALENGTH(panel240x416otp)); }
    DRV_EPD_waitBusy(0);

//	MCU_S8_S9_write_cmd(0x01); 		//power setting
//	MCU_S8_S9_write_data(0x03);		//internal DC/DC function	        
//	MCU_S8_S9_write_data(0x10);		//VCOM voltage level(16V),pannel spec 20V? 05
//	MCU_S8_S9_write_data(0x3F);		//VDH=15v
//	MCU_S8_S9_write_data(0x3F);		//VDL=-15v
//	MCU_S8_S9_write_data(0x00);		//VDHR=2.4v
//  static const uint8_t powerDefault[] = { 0x03, 0x10, 0x3F, 0x3F, 0x0D};
    static const uint8_t power[] = { 0x03, 0x10, 0x3F, 0x3F, 0x00 };
    GFX_Disp_Intf_WriteCommandParm(intf, 0x01, // POWER_SETTING
                              (uint8_t *)power, ALENGTH(power));
	
//	MCU_S8_S9_write_cmd(0x06);		//booster start
//	MCU_S8_S9_write_data(0x17);    //soft start period of phase A(10ms),driving strength of phase A(3),default
//	MCU_S8_S9_write_data(0x17);
//	MCU_S8_S9_write_data(0x26);     //26
//  static const uint8_t boostDefault[] = { 0x17, 0x17, 0x17 };
#if DISPLAY_VENDOR_AMP_240x416
    static const uint8_t boost[] = { 0x17, 0x17, 0x16 };
#elif DISPLAY_VENDOR_AMP_240x360
    static const uint8_t boost[] = { 0x17, 0x17, 0x26 };
#endif
    GFX_Disp_Intf_WriteCommandParm(intf, 0x06, // BOOSTER_SOFT_START
                              (uint8_t *)boost, ALENGTH(boost));
	
//	MCU_S8_S9_write_cmd(0x61);     //resolution setting
//	MCU_S8_S9_write_data(0xF0);    //hres
//	MCU_S8_S9_write_data(0x01);    //vres
//	MCU_S8_S9_write_data(0x68);
//  static const uint8_t tres_240x416[] = { 0xF0, 0x01, 0xA0 };
    static const uint8_t tres[] = 
        { DISPLAY_WIDTH, 0xff & (DISPLAY_HEIGHT >> 8), 0xff & DISPLAY_HEIGHT };
    GFX_Disp_Intf_WriteCommandParm(intf, 0x61, // TRES_RESOLUTION
                              (uint8_t *)tres, ALENGTH(tres));
	
//	MCU_S8_S9_write_cmd(0x65);     
//	MCU_S8_S9_write_data(0x00);    
//	MCU_S8_S9_write_data(0x00);    
//	MCU_S8_S9_write_data(0x00);
    /* The example of 1 Aug 2025 does not load GSST. */
#if DISPLAY_VENDOR_AMP_240x360
    static const uint8_t gsstDefault[] = { 0x00, 0x00, 0x00 };
    GFX_Disp_Intf_WriteCommandParm(intf, 0x65, // Gate/source start setting
                              (uint8_t *)gsstDefault, ALENGTH(gsstDefault));
#endif
    
//	MCU_S8_S9_write_cmd(0x30); 		//PLL control    
//	MCU_S8_S9_write_data(0x13);    //40Hz; 07
//  static const uint8_t pllDefault[] = { 0x09 };
    static const uint8_t pll[] = { 0x13 };
    GFX_Disp_Intf_WriteCommandParm(intf, 0x30, // PLL control
                              (uint8_t *)pll, ALENGTH(pll));
	
//	MCU_S8_S9_write_cmd(0x60); 		//TCON   
//	MCU_S8_S9_write_data(0x22);    //
    static const uint8_t tconDefault[] = { 0x22 };
    GFX_Disp_Intf_WriteCommandParm(intf, 0x60, // TCON setting
                              (uint8_t *)tconDefault, ALENGTH(tconDefault));
    
//	MCU_S8_S9_write_cmd(0x82); 		//VCOM_DC
//	MCU_S8_S9_write_data(VCOM_BUF); //VCOM_BUF  0x06
//  static const uint8_t vdcsDefault[] = { 0 };
//    static const uint8_t vdcs[] = { 0x06 };
//    static const uint8_t vdcs_22082025[] = { 0x0A };
#if 1 // DISPLAY_VENDOR_AMP_UC8253
    static const uint8_t vdcs_09092025[] = { 0x06 };
    GFX_Disp_Intf_WriteCommandParm(intf, 0x82, // VCOM_DC setting
                              (uint8_t *)vdcs_09092025, ALENGTH(vdcs_09092025));
#elif DISPLAY_VENDOR_AMP_240x360
    static const uint8_t vdcs_22082025[] = { 0x05 };
    GFX_Disp_Intf_WriteCommandParm(intf, 0x82, // VCOM_DC setting
                              (uint8_t *)vdcs_22082025, ALENGTH(vdcs_22082025));
#endif

    //	MCU_S8_S9_write_cmd(0x03); 	  
    //	MCU_S8_S9_write_data(0x00); 
    static const uint8_t pfsDefault[] = { 0x00 };
    GFX_Disp_Intf_WriteCommandParm(intf, 0x03, // Power OFF sequence setting
                              (uint8_t *)pfsDefault, ALENGTH(pfsDefault));

//	MCU_S8_S9_write_cmd(0x50);        
//	MCU_S8_S9_write_data(0x67);//B7/47
    typedef union { 
        uint8_t bvalue;
        struct { unsigned cdi:4; unsigned ddx:2; unsigned vbd:2; };
    } cdi_t;
#if 0 // DRV_GFX_DO_NOT_TOGGLE_LUT // 0 // DISPLAY_VENDOR_AMP_UC8253
    static const cdi_t cdi[] = { { .vbd=2, .ddx=3, .cdi=7 } }; // 22082025=0xB7
#elif 1 // DISPLAY_VENDOR_AMP_240x360
    static const cdi_t cdi[] = { { .vbd=0, .ddx=1, .cdi=7 } }; // 0x17
#endif
    GFX_Disp_Intf_WriteCommandParm(intf, 0x50, // VCOM and data interval
                              (uint8_t *)cdi, ALENGTH(cdi));
    /* Per email Lawrence:8Sept2025:
     * If you use [0x50+0x17], you will need to use the toggle flag
     * [when loading the LUT]. 0x50+0xB7 is used without using 
     * the fast refresh mode. If you want to use the fast refresh mode DU, 
     * please use [0x50+0x17].*/

#if READ_STATUS
    uint8_t statusFlag; (void)statusFlag;
    GFX_Disp_Intf_WriteCommand(intf, 0x71); // get status
    GFX_Disp_Intf_ReadData(intf,&statusFlag,1);

    uint8_t lowPower; (void)lowPower;
    GFX_Disp_Intf_WriteCommand(intf, 0x51); // get status
    DRV_EPD_waitBusy(0);
    GFX_Disp_Intf_ReadData(intf,&lowPower,1);

    uint8_t breakCheck; (void)breakCheck;
    GFX_Disp_Intf_WriteCommand(intf, 0x44); // get status
    DRV_EPD_waitBusy(0);
    GFX_Disp_Intf_ReadData(intf,&breakCheck,1);

    uint8_t temperature[2]; (void)temperature[0];
    GFX_Disp_Intf_WriteCommand(intf, 0x43); // get temperature
    DRV_EPD_waitBusy(0);
    GFX_Disp_Intf_ReadData(intf,temperature,ALENGTH(temperature));
#endif
    
//	MCU_S8_S9_write_cmd(0x04); //power on
    GFX_Disp_Intf_WriteCommand(intf, 0x04); // POWER_ON

    /* The display spontaneously goes busy after all data is loaded, 
     * for about 57us. */
    DRV_EPD_waitBusy(0);

    NOP(); NOP(); NOP();
    DRV_EPD_DelayMS(2);
    
    AMP_UC8253_NCSDeassert(intf);
    return 0;
}

__attribute__((unused))
static uint8_t rotate8(uint8_t * data)
{
    uint16_t d = *data << 1;
    if (0x100 & d) d |= 1;
    return (*data = (0xFF & d));
}


/**
  Function:
    static void AMP_UC8253_start_config(void)

  Summary:
    Driver-specific implementation of GFX HAL update function.

  Description:
    On GFX update, this function flushes any pending pixels to the Custom.

  Parameters:
    None.

  Returns:
    * GFX_SUCCESS       - Operation successful
    * GFX_FAILURE       - Operation failed

*/
void AMP_UC8253_start_config(AMP_UC8253_t *ctx)
{
    GFX_Disp_Intf intf = *ctx;
    
    if(ctx)
    {
        DRV_EPD_Reset();
        NOP(); NOP(); NOP();
        
        AMP_UC8253_Configure(intf, 1); // use LUT in RAM
        NOP(); NOP(); NOP();

        /* Load the LUT for a full display, then initialize the display. */
        static const uint8_t initializeByte = 0xFF;
        
        AMP_UC8253_send_lut(intf,AMP_UC8253_LUT_GC);
        NOP(); NOP(); NOP();
        
        /* To start the machine, you need to input a blank image
         * Guarantee that we are in FULL  mode. */
        // GFX_Disp_Intf_WriteCommand(intf, 0x92); // PTOUT Partial Out
        GFX_Disp_Intf_WriteCommand(intf, 0x10); // Display start
        DRV_writeRepeatedData(intf, (uint8_t *)&initializeByte, 
                         (DISPLAY_WIDTH * DISPLAY_HEIGHT)/8 );
        
        /* Added 20250908 to initialize both memory banks. */
        GFX_Disp_Intf_WriteCommand(intf, 0x13); // Display start
        DRV_writeRepeatedData(intf, (uint8_t *)&initializeByte, 
                         (DISPLAY_WIDTH * DISPLAY_HEIGHT)/8 );
        AMP_UC8253_display_update(intf);
        NOP(); NOP(); NOP();
                
        /* Because we updated both memories we are at the
         * baseline condition so we undo the toggle
         * that happened when we loaded the LUT. */
//        AMP_lightDark_toggle = 0;
        
        /* We are complete; allow Legato to send data. */
        drv.state = RUN;
        
    }
}