/*******************************************************************************
  LVDSC Peripheral Library

  Company:
    Microchip Technology Inc.

  File Name:
    plib_lvdsc.c

  Summary:
    Contains LVDSC peripheral library function definitions.

  Description:
    This library implements a SUBSET of the register configurations
    for the LVDSC peripheral.

    Created with MPLAB Harmony Version 3.0
*******************************************************************************/

// DOM-IGNORE-BEGIN
/*******************************************************************************
* Copyright (C) 2023 Microchip Technology Inc. and its subsidiaries.
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

#include "device.h"
#include "gfx/driver/controller/xlcdc/bridge/lvdsc/plib_lvdsc.h"

/*******************************************************************************
  Function:
    void LVDSC_Initialize ( void )

  Summary:
    Initializes the LVDSC peripheral.

  Remarks:
 */

void LVDSC_Initialize(void)
{
<#if LVDSClocksEn>
    /* Initialize LVDSPLL */
    /* Set PLL target to LVDSPLL and configure startup time of 150us */
    PMC_REGS->PMC_PLL_UPDT = PMC_PLL_UPDT_STUPTIM(0x6) |
                             PMC_PLL_UPDT_ID(0x${LVDSPLLIndex});

    /* Set the analog controls to the values recommended in the data sheet */
    PMC_REGS->PMC_PLL_ACR = PMC_PLL_ACR_LOOP_FILTER(0x1B) |
                            PMC_PLL_ACR_LOCK_THR(0x4) |
                            PMC_PLL_ACR_CONTROL(0x10);

    /* Set loop parameters for the fractional PLL */
    PMC_REGS->PMC_PLL_CTRL1 = PMC_PLL_CTRL1_MUL(${LVDSClockMul} - 1) |
                              PMC_PLL_CTRL1_FRACR(${LVDSClockFrac});

    /* Update the PLL target i.e. LVDSPLL, with the configured settings */
    PMC_REGS->PMC_PLL_UPDT |= PMC_PLL_UPDT_UPDATE_Msk;

    /* Enable and lock LVDSPLL clock, enable it for use by PMC */
    PMC_REGS->PMC_PLL_CTRL0 = PMC_PLL_CTRL0_ENLOCK_Msk |
                              PMC_PLL_CTRL0_ENPLL_Msk |
                              PMC_PLL_CTRL0_DIVPMC(${LVDSClockDivPMC} - 1) |
                              PMC_PLL_CTRL0_ENPLLCK_Msk;

    /* Update the PLL target i.e. LVDSPLL, with the configured settings */
    PMC_REGS->PMC_PLL_UPDT |= PMC_PLL_UPDT_UPDATE_Msk;

    /* Wait for PLL lock */
    while ((PMC_REGS->PMC_PLL_ISR0 & ${LVDSPLLISRMask}) != ${LVDSPLLISRMask});

    /* Enable LVDSC Peripheral Clock */
    PMC_REGS->PMC_PCR = PMC_PCR_CMD_Msk |
                        PMC_PCR_PID(ID_LVDSC) |
                        PMC_PCR_EN_Msk;

</#if>
    /* Disable LVDS Serializer */
    LVDSC_REGS->LVDSC_CR = LVDSC_CR_SER_EN(0);
    while ((LVDSC_REGS->LVDSC_SR & LVDSC_SR_CS_Msk) != 0);

    /* Configuration */
    LVDSC_REGS->LVDSC_CFGR = LVDSC_CFGR_LCDC_PIXSIZE(${LVDSCDPIInput}) |
            LVDSC_CFGR_LCDC_DEN_POL(${LVDSCDENPol}) |
<#if LVDSCDCMode == "LVDSC_CFGR_DC_BAL_UNBALANCED_Val">
            LVDSC_CFGR_MAPPING(${LVDSCDCMapping}) |
</#if>
            LVDSC_CFGR_DC_BAL(${LVDSCDCMode});

    LVDSC_REGS->LVDSC_UCBR = LVDSC_UCBR_RESA3(${LVDSCA3Bit});

    /* Analog Control */
    LVDSC_REGS->LVDSC_ACR = LVDSC_ACR_DCBIAS(${LVDSCDCBias}) |
                            LVDSC_ACR_PREEMP_A0(${LVDSCA0Premph}) |
                            LVDSC_ACR_PREEMP_A1(${LVDSCA1Premph}) |
                            LVDSC_ACR_PREEMP_A2(${LVDSCA2Premph}) |
                            LVDSC_ACR_PREEMP_A3(${LVDSCA3Premph}) |
                            LVDSC_ACR_PREEMP_CLK1(${LVDSClkPremph});

    /* Enable LVDS Serializer */
    LVDSC_REGS->LVDSC_CR = LVDSC_CR_SER_EN(1);
}
