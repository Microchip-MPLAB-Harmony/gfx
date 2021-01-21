/*******************************************************************************
  LCDC PLIB Implementation

  Company:
    Microchip Technology Inc.

  File Name:
    plib_lcdc.c

  Summary:
    Function implementations for the LCDC (LCD Controller) PLIB.

  Description:
    This PLIB provides the interfaces to configure the LCD Controller (LCDC).
    This peripheral library implements a SUBSET of the register configurations
    for the LCDC. 

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

#include "definitions.h"

LCDC_IRQ_CALLBACK_OBJECT LCDC_IRQ_CallbackObj;

void LCDC_SetPixelClockPolarity(LCDC_SIGNAL_POLARITY polarity)
{
    LCDC_REGS->LCDC_LCDCFG0 = (LCDC_REGS->LCDC_LCDCFG0 & ~LCDC_LCDCFG0_CLKPOL_Msk) | 
                                LCDC_LCDCFG0_CLKPOL(polarity == LCDC_POLARITY_NEGATIVE);
}

void LCDC_SetPWMClockSourceSelection(LCDC_PWM_CLOCK_SOURCE source)
{
    LCDC_REGS->LCDC_LCDCFG0 = (LCDC_REGS->LCDC_LCDCFG0 & ~LCDC_LCDCFG0_CLKPWMSEL_Msk) | 
                                LCDC_LCDCFG0_CLKPWMSEL(source == LCDC_PWM_SOURCE_SYSTEM);
}

void LCDC_SetLayerClockGatingDisable(LCDC_LAYER_ID layer, bool disable)
{
    switch(layer)
    {
        case LCDC_LAYER_BASE:
            LCDC_REGS->LCDC_LCDCFG0 = (LCDC_REGS->LCDC_LCDCFG0 & ~LCDC_LCDCFG0_CGDISBASE_Msk) | 
                                LCDC_LCDCFG0_CGDISBASE(disable == false);
            break;
        case LCDC_LAYER_OVR1:
            LCDC_REGS->LCDC_LCDCFG0 = (LCDC_REGS->LCDC_LCDCFG0 & ~LCDC_LCDCFG0_CGDISOVR1_Msk) | 
                                LCDC_LCDCFG0_CGDISOVR1(disable == false);
            break;
        case LCDC_LAYER_OVR2:
            LCDC_REGS->LCDC_LCDCFG0 = (LCDC_REGS->LCDC_LCDCFG0 & ~LCDC_LCDCFG0_CGDISOVR2_Msk) | 
                                LCDC_LCDCFG0_CGDISOVR2(disable == false);
            break;
        case LCDC_LAYER_HEO:
            LCDC_REGS->LCDC_LCDCFG0 = (LCDC_REGS->LCDC_LCDCFG0 & ~LCDC_LCDCFG0_CGDISHEO_Msk) | 
                                LCDC_LCDCFG0_CGDISHEO(disable == false);
            break;
        case LCDC_LAYER_PP:
            //Unsupported
        default:
            break;
    }
}

void LCDC_SetClockDivider(uint8_t value)
{
    LCDC_REGS->LCDC_LCDCFG0 = (LCDC_REGS->LCDC_LCDCFG0 & ~LCDC_LCDCFG0_CLKDIV_Msk) | 
                                  LCDC_LCDCFG0_CLKDIV(value - 2);
}

void LCDC_SetHSYNCPulseWidth(uint16_t value)
{
    LCDC_REGS->LCDC_LCDCFG1 = (LCDC_REGS->LCDC_LCDCFG1 & ~LCDC_LCDCFG1_HSPW_Msk) | 
                                  LCDC_LCDCFG1_HSPW(value);
}

void LCDC_SetVSYNCPulseWidth(uint16_t value)
{
    LCDC_REGS->LCDC_LCDCFG1 = (LCDC_REGS->LCDC_LCDCFG1 & ~LCDC_LCDCFG1_VSPW_Msk) | 
                                  LCDC_LCDCFG1_VSPW(value);
}

void LCDC_SetVerticalFrontPorchWidth(uint16_t value)
{
    LCDC_REGS->LCDC_LCDCFG2 = (LCDC_REGS->LCDC_LCDCFG2 & ~LCDC_LCDCFG2_VFPW_Msk) | 
                                  LCDC_LCDCFG2_VFPW(value - 1);
}

void LCDC_SetVerticalBackPorchWidth(uint16_t value)
{
    LCDC_REGS->LCDC_LCDCFG2 = (LCDC_REGS->LCDC_LCDCFG2 & ~LCDC_LCDCFG2_VBPW_Msk) | 
                                  LCDC_LCDCFG2_VBPW(value - 1);
}

void LCDC_SetHorizontalFrontPorchWidth(uint16_t value)
{
    LCDC_REGS->LCDC_LCDCFG3 = (LCDC_REGS->LCDC_LCDCFG3 & ~LCDC_LCDCFG3_HFPW_Msk) | 
                                  LCDC_LCDCFG3_HFPW(value - 1);
}

void LCDC_SetHorizontalBackPorchWidth(uint16_t value)
{
    LCDC_REGS->LCDC_LCDCFG4 = (LCDC_REGS->LCDC_LCDCFG4 & ~LCDC_LCDCFG3_HBPW_Msk) | 
                                  LCDC_LCDCFG3_HBPW(value - 1);
}

void LCDC_SetNumActiveRows(uint16_t value)
{
    LCDC_REGS->LCDC_LCDCFG4 = (LCDC_REGS->LCDC_LCDCFG4 & ~LCDC_LCDCFG4_RPF_Msk) | 
                                  LCDC_LCDCFG4_RPF(value - 1);
}

void LCDC_SetNumPixelsPerLine(uint16_t value)
{
    LCDC_REGS->LCDC_LCDCFG4 = (LCDC_REGS->LCDC_LCDCFG4 & ~LCDC_LCDCFG4_PPL_Msk) | 
                                  LCDC_LCDCFG4_PPL(value - 1);
}

void LCDC_SetHSYNCPolarity(LCDC_SIGNAL_POLARITY polarity)
{
    LCDC_REGS->LCDC_LCDCFG5 = (LCDC_REGS->LCDC_LCDCFG5 & ~LCDC_LCDCFG5_HSPOL_Msk) | 
                                  LCDC_LCDCFG5_HSPOL(polarity == LCDC_POLARITY_NEGATIVE);
}

void LCDC_SetVSYNCPolarity(LCDC_SIGNAL_POLARITY polarity)
{
    LCDC_REGS->LCDC_LCDCFG5 = (LCDC_REGS->LCDC_LCDCFG5 & ~LCDC_LCDCFG5_VSPOL_Msk) | 
                                  LCDC_LCDCFG5_VSPOL(polarity == LCDC_POLARITY_NEGATIVE);
}

void LCDC_SetVSYNCPulseStart(LCDC_VSYNC_SYNC_EDGE edge)
{
    LCDC_REGS->LCDC_LCDCFG5 = (LCDC_REGS->LCDC_LCDCFG5 & ~LCDC_LCDCFG5_VSPDLYS_Msk) | 
                                  LCDC_LCDCFG5_VSPDLYS(edge == LCDC_SYNC_EDGE_FIRST);
}

void LCDC_SetVSYNCPulseEnd(LCDC_VSYNC_SYNC_EDGE edge)
{
    LCDC_REGS->LCDC_LCDCFG5 = (LCDC_REGS->LCDC_LCDCFG5 & ~LCDC_LCDCFG5_VSPDLYE_Msk) | 
                                  LCDC_LCDCFG5_VSPDLYE(edge == LCDC_SYNC_EDGE_FIRST);
}

void LCDC_SetDisplaySignalPolarity(LCDC_SIGNAL_POLARITY polarity)
{
    LCDC_REGS->LCDC_LCDCFG5 = (LCDC_REGS->LCDC_LCDCFG5 & ~LCDC_LCDCFG5_DISPPOL_Msk) | 
                                  LCDC_LCDCFG5_DISPPOL(polarity == LCDC_POLARITY_NEGATIVE);
}

void LCDC_SetDitheringEnable(bool enable)
{
    LCDC_REGS->LCDC_LCDCFG5 = (LCDC_REGS->LCDC_LCDCFG5 & ~LCDC_LCDCFG5_DITHER_Msk) | 
                                  LCDC_LCDCFG5_DITHER(enable == true);
}

void LCDC_SetDisplaySignalSynchronization(bool synchronous)
{
    LCDC_REGS->LCDC_LCDCFG5 = (LCDC_REGS->LCDC_LCDCFG5 & ~LCDC_LCDCFG5_DISPDLY_Msk) | 
                                  LCDC_LCDCFG5_DISPDLY(synchronous == false);
}

void LCDC_SetOutputMode(LCDC_OUTPUT_COLOR_MODE mode)
{
    LCDC_REGS->LCDC_LCDCFG5 = (LCDC_REGS->LCDC_LCDCFG5 & ~LCDC_LCDCFG5_MODE_Msk) | 
                                  LCDC_LCDCFG5_MODE(mode);
}

void LCDC_SetVSYNCPulseSetupConfig(bool synchronous)
{
    LCDC_REGS->LCDC_LCDCFG5 = (LCDC_REGS->LCDC_LCDCFG5 & ~LCDC_LCDCFG5_VSPSU_Msk) | 
                                  LCDC_LCDCFG5_VSPSU(synchronous == false);
}

void LCDC_SetVSYNCPulseHoldConfig(bool synchronous)
{
    LCDC_REGS->LCDC_LCDCFG5 = (LCDC_REGS->LCDC_LCDCFG5 & ~LCDC_LCDCFG5_VSPHO_Msk) | 
                                  LCDC_LCDCFG5_VSPHO(synchronous == false);
}

void LCDC_SetDisplayGuardTime(uint16_t frames)
{
    LCDC_REGS->LCDC_LCDCFG5 = (LCDC_REGS->LCDC_LCDCFG5 & ~LCDC_LCDCFG5_GUARDTIME_Msk) | 
                                  LCDC_LCDCFG5_GUARDTIME(frames);
}

void LCDC_SetPWMPrescaler(uint8_t div)
{
    LCDC_REGS->LCDC_LCDCFG6 = (LCDC_REGS->LCDC_LCDCFG6 & ~LCDC_LCDCFG6_PWMPS_Msk) | 
                                  LCDC_LCDCFG6_PWMPS(div);
}

void LCDC_SetPWMSignalPolarity(LCDC_SIGNAL_POLARITY polarity)
{
    LCDC_REGS->LCDC_LCDCFG6 = (LCDC_REGS->LCDC_LCDCFG6 & ~LCDC_LCDCFG6_PWMPOL_Msk) | 
                                  LCDC_LCDCFG6_PWMPOL(polarity == LCDC_POLARITY_POSITIVE);
}

void LCDC_SetPWMCompareValue(uint32_t value)
{
    LCDC_REGS->LCDC_LCDCFG6 = (LCDC_REGS->LCDC_LCDCFG6 & ~LCDC_LCDCFG6_PWMCVAL_Msk) | 
                                  LCDC_LCDCFG6_PWMCVAL(value);
}

void LCDC_SetPixelClockEnable(bool enable)
{
    if (enable == true)
        LCDC_REGS->LCDC_LCDEN = (LCDC_REGS->LCDC_LCDEN & ~LCDC_LCDEN_CLKEN_Msk) | 
                                      LCDC_LCDEN_CLKEN(1);
    else
        LCDC_REGS->LCDC_LCDDIS = (LCDC_REGS->LCDC_LCDDIS & ~LCDC_LCDDIS_CLKDIS_Msk) | 
                                      LCDC_LCDDIS_CLKDIS(1);
}

void LCDC_SetSYNCEnable(bool enable)
{
    if (enable == true)
        LCDC_REGS->LCDC_LCDEN = (LCDC_REGS->LCDC_LCDEN & ~LCDC_LCDEN_SYNCEN_Msk) | 
                                  LCDC_LCDEN_SYNCEN(1);
    else
        LCDC_REGS->LCDC_LCDDIS = (LCDC_REGS->LCDC_LCDDIS & ~LCDC_LCDDIS_SYNCDIS_Msk) | 
                                      LCDC_LCDDIS_SYNCDIS(1);
}

void LCDC_SetDISPSignalEnable(bool enable)
{
    if (enable == true)
        LCDC_REGS->LCDC_LCDEN = (LCDC_REGS->LCDC_LCDEN & ~LCDC_LCDEN_DISPEN_Msk) | 
                                  LCDC_LCDEN_DISPEN(1);
    else
        LCDC_REGS->LCDC_LCDDIS = (LCDC_REGS->LCDC_LCDDIS & ~LCDC_LCDDIS_DISPDIS_Msk) | 
                                      LCDC_LCDDIS_DISPDIS(1);  
}

void LCDC_SetPWMEnable(bool enable)
{
    if (enable == true)
        LCDC_REGS->LCDC_LCDEN = (LCDC_REGS->LCDC_LCDEN & ~LCDC_LCDEN_PWMEN_Msk) | 
                                 LCDC_LCDEN_PWMEN(1);
    else
        LCDC_REGS->LCDC_LCDDIS = (LCDC_REGS->LCDC_LCDDIS & ~LCDC_LCDDIS_PWMDIS_Msk) | 
                                      LCDC_LCDDIS_PWMDIS(1);    
}

void LCDC_ClockReset( void )
{
    LCDC_REGS->LCDC_LCDDIS = (LCDC_REGS->LCDC_LCDDIS & ~LCDC_LCDDIS_CLKRST_Msk) | 
                              LCDC_LCDDIS_CLKRST(1);    
}

void LCDC_SYNCReset( void )
{
    LCDC_REGS->LCDC_LCDDIS = (LCDC_REGS->LCDC_LCDDIS & ~LCDC_LCDDIS_SYNCRST_Msk) | 
                              LCDC_LCDDIS_SYNCRST(1);    
}

void LCDC_DISPSignalReset( void )
{
    LCDC_REGS->LCDC_LCDDIS = (LCDC_REGS->LCDC_LCDDIS & ~LCDC_LCDDIS_DISPRST_Msk) | 
                              LCDC_LCDDIS_DISPRST(1);    
}

void LCDC_PWMReset( void )
{
    LCDC_REGS->LCDC_LCDDIS = (LCDC_REGS->LCDC_LCDDIS & ~LCDC_LCDDIS_PWMRST_Msk) | 
                              LCDC_LCDDIS_PWMRST(1); 
}

bool LCDC_GetPixelClockStatusRunning( void )
{
    return ((LCDC_REGS->LCDC_LCDSR & LCDC_LCDSR_CLKSTS_Msk) == LCDC_LCDSR_CLKSTS_Msk);
}

bool LCDC_GetTimingEngineStatusRunning( void )
{
    return ((LCDC_REGS->LCDC_LCDSR & LCDC_LCDSR_LCDSTS_Msk) == LCDC_LCDSR_LCDSTS_Msk);
}

bool LCDC_GetDisplaySignalStatusActivated( void )
{
    return ((LCDC_REGS->LCDC_LCDSR & LCDC_LCDSR_DISPSTS_Msk) == LCDC_LCDSR_DISPSTS_Msk);
}

bool LCDC_GetPWMSignalStatusActivated( void )
{
    return ((LCDC_REGS->LCDC_LCDSR & LCDC_LCDSR_PWMSTS_Msk) == LCDC_LCDSR_PWMSTS_Msk);
}

bool LCDC_GetSynchronizationStatusInProgress( void )
{
    return ((LCDC_REGS->LCDC_LCDSR & LCDC_LCDSR_SIPSTS_Msk) == LCDC_LCDSR_SIPSTS_Msk);
}

void LCDC_SetSOFInterruptEnable(bool enable)
{
    if (enable == true)
        LCDC_REGS->LCDC_LCDIER = (LCDC_REGS->LCDC_LCDIER & ~LCDC_LCDIER_SOFIE_Msk) | 
                                  LCDC_LCDIER_SOFIE(1);
    else
        LCDC_REGS->LCDC_LCDIDR = (LCDC_REGS->LCDC_LCDIDR & ~LCDC_LCDIDR_SOFID_Msk) | 
                                  LCDC_LCDIDR_SOFID(1);    
}

void LCDC_SetLCDDisableInterruptEnable(bool enable)
{
    if (enable == true)
        LCDC_REGS->LCDC_LCDIER = (LCDC_REGS->LCDC_LCDIER & ~LCDC_LCDIER_DISIE_Msk) | 
                                  LCDC_LCDIER_DISIE(1);
    else
        LCDC_REGS->LCDC_LCDIDR = (LCDC_REGS->LCDC_LCDIDR & ~LCDC_LCDIDR_DISID_Msk) | 
                                  LCDC_LCDIDR_DISID(1);    
}

void LCDC_WaitForSyncInProgress( void )
{
    while ((LCDC_REGS->LCDC_LCDSR & LCDC_LCDSR_SIPSTS_Msk));
}

void LCDC_WaitForClockRunning( void )
{
    while (!(LCDC_REGS->LCDC_LCDSR & LCDC_LCDSR_CLKSTS_Msk));
}

void LCDC_WaitForSynchronization( void )
{
    while (!(LCDC_REGS->LCDC_LCDSR & LCDC_LCDSR_LCDSTS_Msk));
}

void LCDC_WaitForDISPSignal( void )
{
    while (!(LCDC_REGS->LCDC_LCDSR & LCDC_LCDSR_DISPSTS_Msk));
}

void LCDC_SetRGBModeInput(LCDC_LAYER_ID layer, LCDC_INPUT_COLOR_MODE mode)
{
    switch(layer)
    {
        case LCDC_LAYER_BASE:
            LCDC_REGS->LCDC_BASECFG1 = (LCDC_REGS->LCDC_BASECFG1 & ~LCDC_BASECFG1_RGBMODE_Msk) | 
                                        LCDC_BASECFG1_RGBMODE(mode);
          break;
        case LCDC_LAYER_OVR1:
            LCDC_REGS->LCDC_OVR1CFG1 = (LCDC_REGS->LCDC_OVR1CFG1 & ~LCDC_OVR1CFG1_RGBMODE_Msk) | 
                                        LCDC_OVR1CFG1_RGBMODE(mode);
          break;
        case LCDC_LAYER_OVR2:
            LCDC_REGS->LCDC_OVR2CFG1 = (LCDC_REGS->LCDC_OVR2CFG1 & ~LCDC_OVR2CFG1_RGBMODE_Msk) | 
                                        LCDC_OVR2CFG1_RGBMODE(mode);
            break;
        case LCDC_LAYER_HEO:
            LCDC_REGS->LCDC_HEOCFG1 = (LCDC_REGS->LCDC_HEOCFG1 & ~LCDC_HEOCFG1_RGBMODE_Msk) | 
                                       LCDC_HEOCFG1_RGBMODE(mode);
            break;            
        case LCDC_LAYER_PP:
        default:
          break;
    }
}

void LCDC_SetDMAHeadPointer(LCDC_LAYER_ID layer, uint32_t head)
{
    switch(layer)
    {
        case LCDC_LAYER_BASE:
            LCDC_REGS->LCDC_BASEHEAD = (LCDC_REGS->LCDC_BASEHEAD & ~LCDC_BASEHEAD_HEAD_Msk) | 
                                        LCDC_BASEHEAD_HEAD(head);
          break;
        case LCDC_LAYER_OVR1:
            LCDC_REGS->LCDC_OVR1HEAD = (LCDC_REGS->LCDC_OVR1HEAD & ~LCDC_OVR1HEAD_HEAD_Msk) | 
                                        LCDC_OVR1HEAD_HEAD(head);
          break;
        case LCDC_LAYER_OVR2:
            LCDC_REGS->LCDC_OVR2HEAD = (LCDC_REGS->LCDC_OVR2HEAD & ~LCDC_OVR2HEAD_HEAD_Msk) | 
                                        LCDC_OVR2HEAD_HEAD(head);
            break;
        case LCDC_LAYER_HEO:
            LCDC_REGS->LCDC_HEOHEAD = (LCDC_REGS->LCDC_HEOHEAD & ~LCDC_HEOHEAD_HEAD_Msk) | 
                                       LCDC_HEOHEAD_HEAD(head);
            break;
        case LCDC_LAYER_PP:
        default:
          break;
    }
}

void LCDC_UpdateAddToQueue(LCDC_LAYER_ID layer)
{
    switch(layer)
    {
        case LCDC_LAYER_BASE:
            LCDC_REGS->LCDC_ATTR = (LCDC_REGS->LCDC_ATTR & ~LCDC_ATTR_BASEA2Q_Msk) | 
                                    LCDC_ATTR_BASEA2Q(1); 
            break;
        case LCDC_LAYER_OVR1:
            LCDC_REGS->LCDC_ATTR = (LCDC_REGS->LCDC_ATTR & ~LCDC_ATTR_OVR1A2Q_Msk) | 
                                    LCDC_ATTR_OVR1A2Q(1); 
            break;
        case LCDC_LAYER_OVR2:
            LCDC_REGS->LCDC_ATTR = (LCDC_REGS->LCDC_ATTR & ~LCDC_ATTR_OVR2A2Q_Msk) | 
                                    LCDC_ATTR_OVR2A2Q(1);
            break;
        case LCDC_LAYER_HEO:
            LCDC_REGS->LCDC_ATTR = (LCDC_REGS->LCDC_ATTR & ~LCDC_ATTR_HEOA2Q_Msk) | 
                              LCDC_ATTR_HEOA2Q(1);
            break;
        case LCDC_LAYER_PP:
            //Unsupported
        default:
          break;
    }
}

void LCDC_SetWindowPosition(LCDC_LAYER_ID layer, uint16_t xpos, uint16_t ypos)
{
    switch(layer)
    {
        case LCDC_LAYER_OVR1:
            LCDC_REGS->LCDC_OVR1CFG2 = LCDC_OVR1CFG2_YPOS(ypos) | LCDC_OVR1CFG2_XPOS(xpos); 
            break;
        case LCDC_LAYER_OVR2:
            LCDC_REGS->LCDC_OVR2CFG2 = LCDC_OVR1CFG2_YPOS(ypos) | LCDC_OVR1CFG2_XPOS(xpos); 
            break;
        case LCDC_LAYER_HEO:
            LCDC_REGS->LCDC_HEOCFG2 = LCDC_HEOCFG2_YPOS(ypos) | LCDC_HEOCFG2_XPOS(xpos); 
            break;
        case LCDC_LAYER_PP:
        case LCDC_LAYER_BASE:
            //unsupported
        default:
          break;
    }
}

void LCDC_SetWindowSize(LCDC_LAYER_ID layer, uint16_t width, uint16_t height)
{
    switch(layer)
    {
        case LCDC_LAYER_OVR1:
            LCDC_REGS->LCDC_OVR1CFG3 = LCDC_OVR1CFG3_YSIZE(height - 1) | LCDC_OVR1CFG3_XSIZE(width - 1); 
            break;
        case LCDC_LAYER_OVR2:
            LCDC_REGS->LCDC_OVR2CFG3 = LCDC_OVR2CFG3_YSIZE(height - 1) | LCDC_OVR2CFG3_XSIZE(width - 1); 
            break;
        case LCDC_LAYER_HEO:
            LCDC_REGS->LCDC_HEOCFG3 = LCDC_HEOCFG3_YSIZE(height - 1) | LCDC_HEOCFG3_XSIZE(width - 1); 
            break;
        case LCDC_LAYER_PP:
        case LCDC_LAYER_BASE:
            //unsupported
        default:
          break;
    }
}

void LCDC_SetHorizStride(LCDC_LAYER_ID layer, uint32_t xstride)
{
    switch(layer)
    {
        case LCDC_LAYER_OVR1:
            LCDC_REGS->LCDC_OVR1CFG4 = LCDC_OVR1CFG4_XSTRIDE(xstride);
            break;
        case LCDC_LAYER_OVR2:
            LCDC_REGS->LCDC_OVR2CFG4 = LCDC_OVR2CFG4_XSTRIDE(xstride);
            break;
        case LCDC_LAYER_BASE:
            LCDC_REGS->LCDC_BASECFG2 = LCDC_BASECFG2_XSTRIDE(xstride);
            break;
        case LCDC_LAYER_HEO:
            LCDC_REGS->LCDC_HEOCFG5 = LCDC_HEOCFG5_XSTRIDE(xstride);
            break;
        case LCDC_LAYER_PP:
            //Unsupported
        default:
          break;
    }
}

void LCDC_SetUseDMAPathEnable(LCDC_LAYER_ID layer, bool enable)
{
    switch(layer)
    {
        case LCDC_LAYER_BASE:
            LCDC_REGS->LCDC_BASECFG4 = (LCDC_REGS->LCDC_BASECFG4 & ~LCDC_BASECFG4_DMA_Msk) | 
                                LCDC_BASECFG4_DMA(enable == true);
            break;
        case LCDC_LAYER_OVR1:
            LCDC_REGS->LCDC_OVR1CFG9 = (LCDC_REGS->LCDC_OVR1CFG9 & ~LCDC_OVR1CFG9_DMA_Msk) | 
                                LCDC_OVR1CFG9_DMA(enable == true);
            break;
        case LCDC_LAYER_OVR2:
            LCDC_REGS->LCDC_OVR2CFG9 = (LCDC_REGS->LCDC_OVR2CFG9 & ~LCDC_OVR2CFG9_DMA_Msk) | 
                                LCDC_OVR2CFG9_DMA(enable == true);
            break;
        case LCDC_LAYER_HEO:
            LCDC_REGS->LCDC_HEOCFG12 = (LCDC_REGS->LCDC_HEOCFG12 & ~LCDC_HEOCFG12_DMA_Msk) | 
                                LCDC_HEOCFG12_DMA(enable == true);
            break;
        case LCDC_LAYER_PP:
        default:
          break;
    }
}

void LCDC_SetDMAAddressRegister(LCDC_LAYER_ID layer, uint32_t addr)
{
    switch(layer)
    {
        case LCDC_LAYER_BASE:
            LCDC_REGS->LCDC_BASEADDR = LCDC_BASEADDR_ADDR(addr);
            break;
        case LCDC_LAYER_OVR1:
            LCDC_REGS->LCDC_OVR1ADDR = LCDC_OVR1ADDR_ADDR(addr);
            break;
        case LCDC_LAYER_OVR2:
            LCDC_REGS->LCDC_OVR2ADDR = LCDC_OVR2ADDR_ADDR(addr);
            break;
        case LCDC_LAYER_HEO:
            LCDC_REGS->LCDC_HEOADDR = LCDC_HEOADDR_ADDR(addr);
            break;
        case LCDC_LAYER_PP:
            //Unsupported
        default:
          break;
    }
}

void LCDC_SetDMADescriptorNextAddress(LCDC_LAYER_ID layer, uint32_t next)
{
    switch(layer)
    {
        case LCDC_LAYER_BASE:
            LCDC_REGS->LCDC_BASENEXT = LCDC_BASENEXT_NEXT(next);  
            break;
        case LCDC_LAYER_OVR1:
            LCDC_REGS->LCDC_OVR1NEXT = LCDC_OVR1NEXT_NEXT(next);
            break;
        case LCDC_LAYER_OVR2:
            LCDC_REGS->LCDC_OVR2NEXT = LCDC_OVR2NEXT_NEXT(next);  
            break;
        case LCDC_LAYER_HEO:
            LCDC_REGS->LCDC_HEONEXT = LCDC_HEONEXT_NEXT(next);  
            break;
        case LCDC_LAYER_PP:
            //Unsupported
        default:
          break;
    }  
}

void LCDC_SetTransferDescriptorFetchEnable(LCDC_LAYER_ID layer, bool enable)
{
    switch(layer)
    {
        case LCDC_LAYER_BASE:
            LCDC_REGS->LCDC_BASECTRL = (LCDC_REGS->LCDC_BASECTRL & ~LCDC_BASECTRL_DFETCH_Msk) | 
                                        LCDC_BASECTRL_DFETCH(enable == true);
            break;
        case LCDC_LAYER_OVR1:
            LCDC_REGS->LCDC_OVR1CTRL = (LCDC_REGS->LCDC_OVR1CTRL & ~LCDC_OVR1CTRL_DFETCH_Msk) | 
                                        LCDC_OVR1CTRL_DFETCH(enable == true);
            break;
        case LCDC_LAYER_OVR2:
            LCDC_REGS->LCDC_OVR2CTRL = (LCDC_REGS->LCDC_OVR2CTRL & ~LCDC_OVR2CTRL_DFETCH_Msk) | 
                                        LCDC_OVR2CTRL_DFETCH(enable == true);
            break;
        case LCDC_LAYER_HEO:
            LCDC_REGS->LCDC_HEOCTRL = (LCDC_REGS->LCDC_HEOCTRL & ~LCDC_HEOCTRL_DFETCH_Msk) | 
                                        LCDC_HEOCTRL_DFETCH(enable == true);
            break;
        case LCDC_LAYER_PP:
            //Unsupported
        default:
          break;
    }
}

void LCDC_UpdateOverlayAttributesEnable(LCDC_LAYER_ID layer)
{
    switch(layer)
    {
        case LCDC_LAYER_BASE:
            LCDC_REGS->LCDC_BASECHER = (LCDC_REGS->LCDC_BASECHER & ~LCDC_BASECHER_UPDATEEN_Msk) | 
                                        LCDC_BASECHER_UPDATEEN(1);
            break;
        case LCDC_LAYER_OVR1:
            LCDC_REGS->LCDC_OVR1CHER = (LCDC_REGS->LCDC_OVR1CHER & ~LCDC_OVR1CHER_UPDATEEN_Msk) | 
                                        LCDC_OVR1CHER_UPDATEEN(1);
            break;
        case LCDC_LAYER_OVR2:
            LCDC_REGS->LCDC_OVR2CHER = (LCDC_REGS->LCDC_OVR2CHER & ~LCDC_OVR2CHER_UPDATEEN_Msk) | 
                                        LCDC_OVR2CHER_UPDATEEN(1);
            break;
        case LCDC_LAYER_HEO:
            LCDC_REGS->LCDC_HEOCHER = (LCDC_REGS->LCDC_HEOCHER & ~LCDC_HEOCHER_UPDATEEN_Msk) | 
                                        LCDC_HEOCHER_UPDATEEN(1);
            break;
        case LCDC_LAYER_PP:
            //Unsupported
        default:
          break;
    }  
}

void LCDC_SetChannelEnable(LCDC_LAYER_ID layer, bool enable)
{
    switch(layer)
    {
        case LCDC_LAYER_BASE:
            if (enable == true)
                LCDC_REGS->LCDC_BASECHER = (LCDC_REGS->LCDC_BASECHER & ~LCDC_BASECHER_CHEN_Msk) | 
                                            LCDC_BASECHER_CHEN(1);
            else
                LCDC_REGS->LCDC_BASECHDR = (LCDC_REGS->LCDC_BASECHDR & ~LCDC_BASECHDR_CHDIS_Msk) | 
                                            LCDC_BASECHDR_CHDIS(1);    
            break;
        case LCDC_LAYER_OVR1:
            if (enable == true)
                LCDC_REGS->LCDC_OVR1CHER = (LCDC_REGS->LCDC_OVR1CHER & ~LCDC_OVR1CHER_CHEN_Msk) | 
                                            LCDC_OVR1CHER_CHEN(1);
            else
                LCDC_REGS->LCDC_OVR1CHDR = (LCDC_REGS->LCDC_OVR1CHDR & ~LCDC_OVR1CHDR_CHDIS_Msk) | 
                                            LCDC_OVR1CHDR_CHDIS(1);    
            break;
        case LCDC_LAYER_OVR2:
            if (enable == true)
                LCDC_REGS->LCDC_OVR2CHER = (LCDC_REGS->LCDC_OVR2CHER & ~LCDC_OVR2CHER_CHEN_Msk) | 
                                            LCDC_OVR2CHER_CHEN(1);
            else
                LCDC_REGS->LCDC_OVR2CHDR = (LCDC_REGS->LCDC_OVR2CHDR & ~LCDC_OVR2CHDR_CHDIS_Msk) | 
                                            LCDC_OVR2CHDR_CHDIS(1);    
            break;
        case LCDC_LAYER_HEO:
            if (enable == true)
                LCDC_REGS->LCDC_HEOCHER = (LCDC_REGS->LCDC_HEOCHER & ~LCDC_HEOCHER_CHEN_Msk) | 
                                            LCDC_HEOCHER_CHEN(1);
            else
                LCDC_REGS->LCDC_HEOCHDR = (LCDC_REGS->LCDC_HEOCHDR & ~LCDC_HEOCHDR_CHDIS_Msk) | 
                                            LCDC_HEOCHDR_CHDIS(1);    
            break;
        case LCDC_LAYER_PP:
            //Unsupported
        default:
          break;
    }  
}

void LCDC_AddToQueueEnable(LCDC_LAYER_ID layer)
{
    switch(layer)
    {
        case LCDC_LAYER_BASE:
            LCDC_REGS->LCDC_BASECHER = (LCDC_REGS->LCDC_BASECHER & ~LCDC_BASECHSR_A2QSR_Msk) | 
                                        LCDC_BASECHER_A2QEN(1);
            break;
        case LCDC_LAYER_OVR1:
            LCDC_REGS->LCDC_OVR1CHER = (LCDC_REGS->LCDC_OVR1CHER & ~LCDC_OVR1CHSR_A2QSR_Msk) | 
                                        LCDC_OVR1CHER_A2QEN(1);
            break;
        case LCDC_LAYER_OVR2:
            LCDC_REGS->LCDC_OVR2CHER = (LCDC_REGS->LCDC_OVR2CHER & ~LCDC_OVR2CHSR_A2QSR_Msk) | 
                                        LCDC_OVR2CHER_A2QEN(1);
            break;
        case LCDC_LAYER_HEO:
            LCDC_REGS->LCDC_HEOCHER = (LCDC_REGS->LCDC_HEOCHER & ~LCDC_HEOCHSR_A2QSR_Msk) | 
                                        LCDC_HEOCHER_A2QEN(1);
            break;
        case LCDC_LAYER_PP:
            //Unsupported
        default:
          break;
    }  
}

void LCDC_SetBlenderOverlayLayerEnable(LCDC_LAYER_ID layer, bool enable)
{
    switch(layer)
    {
        case LCDC_LAYER_OVR1:
            LCDC_REGS->LCDC_OVR1CFG9 = (LCDC_REGS->LCDC_OVR1CFG9 & ~LCDC_OVR1CFG9_OVR_Msk) | 
                                        LCDC_OVR1CFG9_OVR(enable == true);
            break;
        case LCDC_LAYER_OVR2:
            LCDC_REGS->LCDC_OVR2CFG9 = (LCDC_REGS->LCDC_OVR2CFG9 & ~LCDC_OVR2CFG9_OVR_Msk) | 
                                        LCDC_OVR2CFG9_OVR(enable == true);
            break;
        case LCDC_LAYER_HEO:
            LCDC_REGS->LCDC_HEOCFG12 = (LCDC_REGS->LCDC_HEOCFG12 & ~LCDC_HEOCFG12_OVR_Msk) | 
                                        LCDC_HEOCFG12_OVR(enable == true);
            break;
        //Not supported            
        case LCDC_LAYER_BASE: 
        case LCDC_LAYER_PP:
        default:
          break;
    }  
}

void LCDC_SetBlenderDMALayerEnable(LCDC_LAYER_ID layer, bool enable)
{
    switch(layer)
    {
        case LCDC_LAYER_OVR1:
            LCDC_REGS->LCDC_OVR1CFG9 = (LCDC_REGS->LCDC_OVR1CFG9 & ~LCDC_OVR1CFG9_DMA_Msk) | 
                                        LCDC_OVR1CFG9_DMA(enable == true);
            break;
        case LCDC_LAYER_OVR2:
            LCDC_REGS->LCDC_OVR2CFG9 = (LCDC_REGS->LCDC_OVR2CFG9 & ~LCDC_OVR2CFG9_DMA_Msk) | 
                                        LCDC_OVR2CFG9_DMA(enable == true);
            break;
        case LCDC_LAYER_HEO:
            LCDC_REGS->LCDC_HEOCFG12 = (LCDC_REGS->LCDC_HEOCFG12 & ~LCDC_HEOCFG12_DMA_Msk) | 
                                        LCDC_HEOCFG12_DMA(enable == true);
            break;
        //Not supported            
        case LCDC_LAYER_BASE: 
        case LCDC_LAYER_PP:
        default:
          break;
    }  
}

void LCDC_SetBlenderLocalAlphaEnable(LCDC_LAYER_ID layer, bool enable)
{
    switch(layer)
    {
        case LCDC_LAYER_OVR1:
            LCDC_REGS->LCDC_OVR1CFG9 = (LCDC_REGS->LCDC_OVR1CFG9 & ~LCDC_OVR1CFG9_LAEN_Msk) | 
                                        LCDC_OVR1CFG9_LAEN(enable == true);
            break;
        case LCDC_LAYER_OVR2:
            LCDC_REGS->LCDC_OVR2CFG9 = (LCDC_REGS->LCDC_OVR2CFG9 & ~LCDC_OVR2CFG9_LAEN_Msk) | 
                                        LCDC_OVR2CFG9_LAEN(enable == true);
            break;
        case LCDC_LAYER_HEO:
            LCDC_REGS->LCDC_HEOCFG12 = (LCDC_REGS->LCDC_HEOCFG12 & ~LCDC_HEOCFG12_LAEN_Msk) | 
                                        LCDC_HEOCFG12_LAEN(enable == true);
            break;
        //Not supported            
        case LCDC_LAYER_BASE: 
        case LCDC_LAYER_PP:
        default:
          break;
    }  
}

void LCDC_SetBlenderIteratedColorEnable(LCDC_LAYER_ID layer, bool enable)
{
    switch(layer)
    {
        case LCDC_LAYER_OVR1:
            LCDC_REGS->LCDC_OVR1CFG9 = (LCDC_REGS->LCDC_OVR1CFG9 & ~LCDC_OVR1CFG9_ITER2BL_Msk) | 
                                        LCDC_OVR1CFG9_ITER2BL(enable == true);
            break;
        case LCDC_LAYER_OVR2:
            LCDC_REGS->LCDC_OVR2CFG9 = (LCDC_REGS->LCDC_OVR2CFG9 & ~LCDC_OVR2CFG9_ITER2BL_Msk) | 
                                        LCDC_OVR2CFG9_ITER2BL(enable == true);
            break;
        case LCDC_LAYER_HEO:
            LCDC_REGS->LCDC_HEOCFG12 = (LCDC_REGS->LCDC_HEOCFG12 & ~LCDC_HEOCFG12_ITER2BL_Msk) | 
                                        LCDC_HEOCFG12_ITER2BL(enable == true);
            break;
        //Not supported            
        case LCDC_LAYER_BASE: 
        case LCDC_LAYER_PP:
        default:
          break;
    }  
}

void LCDC_SetBlenderUseIteratedColor(LCDC_LAYER_ID layer, bool use)
{
    switch(layer)
    {
        case LCDC_LAYER_OVR1:
            LCDC_REGS->LCDC_OVR1CFG9 = (LCDC_REGS->LCDC_OVR1CFG9 & ~LCDC_OVR1CFG9_ITER_Msk) | 
                                        LCDC_OVR1CFG9_ITER(use == true);
            break;
        case LCDC_LAYER_OVR2:
            LCDC_REGS->LCDC_OVR2CFG9 = (LCDC_REGS->LCDC_OVR2CFG9 & ~LCDC_OVR2CFG9_ITER_Msk) | 
                                        LCDC_OVR2CFG9_ITER(use == true);
            break;
        case LCDC_LAYER_HEO:
            LCDC_REGS->LCDC_HEOCFG12 = (LCDC_REGS->LCDC_HEOCFG12 & ~LCDC_HEOCFG12_ITER_Msk) | 
                                        LCDC_HEOCFG12_ITER(use == true);
            break;
        //Not supported            
        case LCDC_LAYER_BASE: 
        case LCDC_LAYER_PP:
        default:
          break;
    }  
}

void LCDC_SetBlenderGlobalAlphaEnable(LCDC_LAYER_ID layer, bool enable)
{
    switch(layer)
    {
        case LCDC_LAYER_OVR1:
            LCDC_REGS->LCDC_OVR1CFG9 = (LCDC_REGS->LCDC_OVR1CFG9 & ~LCDC_OVR1CFG9_GAEN_Msk) | 
                                        LCDC_OVR1CFG9_GAEN(enable == true);
            break;
        case LCDC_LAYER_OVR2:
            LCDC_REGS->LCDC_OVR2CFG9 = (LCDC_REGS->LCDC_OVR2CFG9 & ~LCDC_OVR2CFG9_GAEN_Msk) | 
                                        LCDC_OVR2CFG9_GAEN(enable == true);
            break;
        case LCDC_LAYER_HEO:
            LCDC_REGS->LCDC_HEOCFG12 = (LCDC_REGS->LCDC_HEOCFG12 & ~LCDC_HEOCFG12_GAEN_Msk) | 
                                        LCDC_HEOCFG12_GAEN(enable == true);
            break;
        //Not supported            
        case LCDC_LAYER_BASE: 
        case LCDC_LAYER_PP:
        default:
          break;
    }  
}

void LCDC_SetBlenderGlobalAlpha(LCDC_LAYER_ID layer, uint8_t alpha)
{
    switch(layer)
    {
        case LCDC_LAYER_OVR1:
            LCDC_REGS->LCDC_OVR1CFG9 = (LCDC_REGS->LCDC_OVR1CFG9 & ~LCDC_OVR1CFG9_GA_Msk) | 
                                        LCDC_OVR1CFG9_GA(alpha);
            break;
        case LCDC_LAYER_OVR2:
            LCDC_REGS->LCDC_OVR2CFG9 = (LCDC_REGS->LCDC_OVR2CFG9 & ~LCDC_OVR2CFG9_GA_Msk) | 
                                        LCDC_OVR2CFG9_GA(alpha);
            break;
        case LCDC_LAYER_HEO:
            LCDC_REGS->LCDC_HEOCFG12 = (LCDC_REGS->LCDC_HEOCFG12 & ~LCDC_HEOCFG12_GA_Msk) | 
                                        LCDC_HEOCFG12_GA(alpha);
            break;
        //Not supported            
        case LCDC_LAYER_BASE: 
        case LCDC_LAYER_PP:
        default:
          break;
    }  
}


void LCDC_UpdateAttribute(LCDC_LAYER_ID layer)
{
    switch(layer)
    {
        case LCDC_LAYER_OVR1:
            LCDC_REGS->LCDC_ATTR = (LCDC_REGS->LCDC_ATTR & ~LCDC_ATTR_OVR1_Msk) | 
                                    LCDC_ATTR_OVR1(1);             
            break;
        case LCDC_LAYER_OVR2:
            LCDC_REGS->LCDC_ATTR = (LCDC_REGS->LCDC_ATTR & ~LCDC_ATTR_OVR2_Msk) | 
                                    LCDC_ATTR_OVR2(1);
            break;
        case LCDC_LAYER_HEO:
            LCDC_REGS->LCDC_ATTR = (LCDC_REGS->LCDC_ATTR & ~LCDC_ATTR_HEO_Msk) | 
                                    LCDC_ATTR_HEO(1);
            break;
        case LCDC_LAYER_BASE:
            LCDC_REGS->LCDC_ATTR = (LCDC_REGS->LCDC_ATTR & ~LCDC_ATTR_BASE_Msk) | 
                                    LCDC_ATTR_BASE(1);
            break;          
        case LCDC_LAYER_PP:
            //Unsupported
        default:
          break;
    }   

}

/* Register callback for period interrupt */
void LCDC_IRQ_CallbackRegister(LCDC_IRQ_CALLBACK callback, uintptr_t context)
{
    LCDC_IRQ_CallbackObj.callback_fn = callback;
    LCDC_IRQ_CallbackObj.context = context;
}

void LCDC_IRQ_Enable(LCDC_INTERRUPT interrupt)
{
    switch(interrupt)
    {
        case LCDC_INTERRUPT_SOF:
            LCDC_REGS->LCDC_LCDIER = (LCDC_REGS->LCDC_LCDIER & ~LCDC_LCDIER_SOFIE_Msk) |
                                      LCDC_LCDIER_SOFIE(1);
            break;
        case LCDC_INTERRUPT_DIS:
            LCDC_REGS->LCDC_LCDIER = (LCDC_REGS->LCDC_LCDIER & ~LCDC_LCDIER_DISIE_Msk) |
                                      LCDC_LCDIER_DISIE(1);
            break;
        case LCDC_INTERRUPT_DISP:
            LCDC_REGS->LCDC_LCDIER = (LCDC_REGS->LCDC_LCDIER & ~LCDC_LCDIER_DISPIE_Msk) |
                                      LCDC_LCDIER_DISPIE(1);
            break;
        case LCDC_INTERRUPT_FIFOERR:
            LCDC_REGS->LCDC_LCDIER = (LCDC_REGS->LCDC_LCDIER & ~LCDC_LCDIER_FIFOERRIE_Msk) |
                                      LCDC_LCDIER_FIFOERRIE(1);
            break;
        case LCDC_INTERRUPT_BASE:
            LCDC_REGS->LCDC_LCDIER = (LCDC_REGS->LCDC_LCDIER & ~LCDC_LCDIER_BASEIE_Msk) |
                                      LCDC_LCDIER_BASEIE(1);          
            break;
        case LCDC_INTERRUPT_OVR1:
            LCDC_REGS->LCDC_LCDIER = (LCDC_REGS->LCDC_LCDIER & ~LCDC_LCDIER_OVR1IE_Msk) |
                                      LCDC_LCDIER_OVR1IE(1);          
            break;
        case LCDC_INTERRUPT_OVR2:
            LCDC_REGS->LCDC_LCDIER = (LCDC_REGS->LCDC_LCDIER & ~LCDC_LCDIER_OVR2IE_Msk) |
                                      LCDC_LCDIER_OVR2IE(1);          
            break;
        case LCDC_INTERRUPT_HEO:
            LCDC_REGS->LCDC_LCDIER = (LCDC_REGS->LCDC_LCDIER & ~LCDC_LCDIER_HEOIE_Msk) |
                                      LCDC_LCDIER_HEOIE(1);                    
            break;
        case LCDC_INTERRUPT_PP:
            //Unsupported
        default:
          break;
    }
}

void LCDC_IRQ_Disable(LCDC_INTERRUPT interrupt)
{
    switch(interrupt)
    {
        case LCDC_INTERRUPT_SOF:
            LCDC_REGS->LCDC_LCDIDR = (LCDC_REGS->LCDC_LCDIDR & ~LCDC_LCDIDR_SOFID_Msk) |
                                      LCDC_LCDIDR_SOFID(1);
            break;
        case LCDC_INTERRUPT_DIS:
            LCDC_REGS->LCDC_LCDIDR = (LCDC_REGS->LCDC_LCDIDR & ~LCDC_LCDIDR_DISID_Msk) |
                                      LCDC_LCDIDR_DISID(1);
            break;
        case LCDC_INTERRUPT_DISP:
            LCDC_REGS->LCDC_LCDIDR = (LCDC_REGS->LCDC_LCDIDR & ~LCDC_LCDIDR_DISPID_Msk) |
                                      LCDC_LCDIDR_DISPID(1);
            break;
        case LCDC_INTERRUPT_FIFOERR:
            LCDC_REGS->LCDC_LCDIDR = (LCDC_REGS->LCDC_LCDIDR & ~LCDC_LCDIDR_FIFOERRID_Msk) |
                                      LCDC_LCDIDR_FIFOERRID(1);
            break;
        case LCDC_INTERRUPT_BASE:
            LCDC_REGS->LCDC_LCDIDR = (LCDC_REGS->LCDC_LCDIDR & ~LCDC_LCDIDR_BASEID_Msk) |
                                      LCDC_LCDIDR_BASEID(1);          
            break;
        case LCDC_INTERRUPT_OVR1:
            LCDC_REGS->LCDC_LCDIDR = (LCDC_REGS->LCDC_LCDIDR & ~LCDC_LCDIDR_OVR1ID_Msk) |
                                      LCDC_LCDIDR_OVR1ID(1);          
            break;
        case LCDC_INTERRUPT_OVR2:
            LCDC_REGS->LCDC_LCDIDR = (LCDC_REGS->LCDC_LCDIDR & ~LCDC_LCDIDR_OVR2ID_Msk) |
                                      LCDC_LCDIDR_OVR2ID(1);          
            break;
        case LCDC_INTERRUPT_HEO:
            LCDC_REGS->LCDC_LCDIDR = (LCDC_REGS->LCDC_LCDIDR & ~LCDC_LCDIDR_HEOID_Msk) |
                                      LCDC_LCDIDR_HEOID(1);                    
            break;
        case LCDC_INTERRUPT_PP:
            //Unsupported
        default:
          break;
    }
}

uint32_t LCDC_IRQ_Status(void)
{
    return LCDC_REGS->LCDC_LCDISR;
}
    
void LCDC_LAYER_IRQ_Enable(LCDC_LAYER_ID layer, LCDC_LAYER_INTERRUPT interrupt)
{
    uint32_t volatile * reg;
    switch(layer)
    {
        case LCDC_LAYER_BASE:
            reg = &LCDC_REGS->LCDC_BASEIER;
            break;
        case LCDC_LAYER_OVR1:
            reg = &LCDC_REGS->LCDC_OVR1IER;
            break;
        case LCDC_LAYER_OVR2:
            reg = &LCDC_REGS->LCDC_OVR2IER;
            break;
        case LCDC_LAYER_HEO:
            reg = &LCDC_REGS->LCDC_HEOIER;
            break;
        case LCDC_LAYER_PP:
        default:
            return;
    }
    
    switch(interrupt)
    {
        case LCDC_LAYER_INTERRUPT_DMA:
            *reg = (*reg & ~LCDC_BASEIER_DMA_Msk) | LCDC_BASEIER_DMA(1);
            break;
        case LCDC_LAYER_INTERRUPT_DSCR:
            *reg = (*reg & ~LCDC_BASEIER_DSCR_Msk) | LCDC_BASEIER_DSCR(1);
            break;
        case LCDC_LAYER_INTERRUPT_ADD:
            *reg = (*reg & ~LCDC_BASEIER_ADD_Msk) | LCDC_BASEIER_ADD(1);
            break;
        case LCDC_LAYER_INTERRUPT_DONE:
            *reg = (*reg & ~LCDC_BASEIER_DONE_Msk) | LCDC_BASEIER_DONE(1);
            break;
        case LCDC_LAYER_INTERRUPT_OVR:
            *reg = (*reg & ~LCDC_BASEIER_OVR_Msk) | LCDC_BASEIER_OVR(1);
            break;
        default:
            break;
    }
}

void LCDC_LAYER_IRQ_Disable(LCDC_LAYER_ID layer, LCDC_LAYER_INTERRUPT interrupt)
{
    uint32_t volatile * reg;
    switch(layer)
    {
        case LCDC_LAYER_BASE:
            reg = &LCDC_REGS->LCDC_BASEIDR;
            break;
        case LCDC_LAYER_OVR1:
            reg = &LCDC_REGS->LCDC_OVR1IDR;
            break;
        case LCDC_LAYER_OVR2:
            reg = &LCDC_REGS->LCDC_OVR2IDR;
            break;
        case LCDC_LAYER_HEO:
            reg = &LCDC_REGS->LCDC_HEOIDR;
            break;
        case LCDC_LAYER_PP:
        default:
            return;
    }
    
    switch(interrupt)
    {
        case LCDC_LAYER_INTERRUPT_DMA:
            *reg = (*reg & ~LCDC_BASEIDR_DMA_Msk) | LCDC_BASEIDR_DMA(1);
            break;
        case LCDC_LAYER_INTERRUPT_DSCR:
            *reg = (*reg & ~LCDC_BASEIDR_DSCR_Msk) | LCDC_BASEIDR_DSCR(1);
            break;
        case LCDC_LAYER_INTERRUPT_ADD:
            *reg = (*reg & ~LCDC_BASEIDR_ADD_Msk) | LCDC_BASEIDR_ADD(1);
            break;
        case LCDC_LAYER_INTERRUPT_DONE:
            *reg = (*reg & ~LCDC_BASEIDR_DONE_Msk) | LCDC_BASEIDR_DONE(1);
            break;
        case LCDC_LAYER_INTERRUPT_OVR:
            *reg = (*reg & ~LCDC_BASEIDR_OVR_Msk) | LCDC_BASEIDR_OVR(1);
            break;
        default:
            break;
    }
}

uint32_t LCDC_LAYER_IRQ_Status(LCDC_LAYER_ID layer)
{
    switch(layer)
    {
        case LCDC_LAYER_BASE:
            return LCDC_REGS->LCDC_BASEISR;
        case LCDC_LAYER_OVR1:
            return LCDC_REGS->LCDC_OVR1ISR;
        case LCDC_LAYER_OVR2:
            return LCDC_REGS->LCDC_OVR2ISR;
        case LCDC_LAYER_HEO:
            return LCDC_REGS->LCDC_HEOISR;
        case LCDC_LAYER_PP:
        default:
            return 0;
    }
}

void LCDC_Interrupt_Handler(void)
{
    if (LCDC_IRQ_CallbackObj.callback_fn != NULL)
    {
        LCDC_IRQ_CallbackObj.callback_fn(LCDC_IRQ_CallbackObj.context);
    }
}

void LCDC_SetHEOImageMemSize(uint16_t width, uint16_t height)
{
    LCDC_REGS->LCDC_HEOCFG4 = LCDC_HEOCFG4_YMEMSIZE(height - 1) | 
                            LCDC_HEOCFG4_XMEMSIZE(width - 1); 
}

void LCDC_SetHEOVideoPriority(bool top)
{
    LCDC_REGS->LCDC_HEOCFG12 = (LCDC_REGS->LCDC_HEOCFG12 & ~LCDC_HEOCFG12_VIDPRI_Msk) | 
        LCDC_HEOCFG12_VIDPRI(top == true); 
}

void LCDC_SetHEOScaler(uint16_t yfactor, uint16_t xfactor, bool enable)
{
    LCDC_REGS->LCDC_HEOCFG13 = LCDC_HEOCFG13_SCALEN(enable == true) |
                               LCDC_HEOCFG13_YFACTOR(yfactor) |
                               LCDC_HEOCFG13_XFACTOR(xfactor);
}

void LCDC_SetHEOFilterPhaseOffset(uint8_t yphidef, uint8_t xphidef)
{
    LCDC_REGS->LCDC_HEOCFG41 = LCDC_HEOCFG41_YPHIDEF(yphidef) | 
                              LCDC_HEOCFG41_XPHIDEF(xphidef); 
}
