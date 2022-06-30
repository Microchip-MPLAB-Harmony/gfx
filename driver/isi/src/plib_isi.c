// DOM-IGNORE-BEGIN
/*******************************************************************************
* Copyright (C) 2021 Microchip Technology Inc. and its subsidiaries.
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

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

/*  This section lists the other files that are included in this file. */
#include "device.h"
#include "plib_isi.h"

// *****************************************************************************
// *****************************************************************************
// Section: Global Data
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
// *****************************************************************************
// Section: ISI PLIB Functions
// *****************************************************************************
// *****************************************************************************

uint32_t PLIB_ISI_CFG1_HSYNC_POL_Get(void)
{
    uint32_t regVal;

    regVal = (ISI_REGS->ISI_CFG1 & ISI_CFG1_HSYNC_POL_Msk) >> ISI_CFG1_HSYNC_POL_Pos;

    return regVal;
}

void PLIB_ISI_CFG1_HSYNC_POL_Set(uint32_t val)
{
    uint32_t regVal;

    regVal = ISI_REGS->ISI_CFG1;
    regVal &= ~(ISI_CFG1_HSYNC_POL_Msk);
    regVal |= ISI_CFG1_HSYNC_POL(val);

    ISI_REGS->ISI_CFG1 = regVal;
}

uint32_t PLIB_ISI_CFG1_VSYNC_POL_Get(void)
{
    uint32_t regVal;

    regVal = (ISI_REGS->ISI_CFG1 & ISI_CFG1_VSYNC_POL_Msk) >> ISI_CFG1_VSYNC_POL_Pos;

    return regVal;
}

void PLIB_ISI_CFG1_VSYNC_POL_Set(uint32_t val)
{
    uint32_t regVal;

    regVal = ISI_REGS->ISI_CFG1;
    regVal &= ~(ISI_CFG1_VSYNC_POL_Msk);
    regVal |= ISI_CFG1_VSYNC_POL(val);

    ISI_REGS->ISI_CFG1 = regVal;
}

uint32_t PLIB_ISI_CFG1_PIXCLK_POL_Get(void)
{
    uint32_t regVal;

    regVal = (ISI_REGS->ISI_CFG1 & ISI_CFG1_PIXCLK_POL_Msk) >> ISI_CFG1_PIXCLK_POL_Pos;

    return regVal;
}

void PLIB_ISI_CFG1_PIXCLK_POL_Set(uint32_t val)
{
    uint32_t regVal;

    regVal = ISI_REGS->ISI_CFG1;
    regVal &= ~(ISI_CFG1_PIXCLK_POL_Msk);
    regVal |= ISI_CFG1_PIXCLK_POL(val);

    ISI_REGS->ISI_CFG1 = regVal;
}

uint32_t PLIB_ISI_CFG1_EMB_SYNC_Get(void)
{
    uint32_t regVal;

    regVal = (ISI_REGS->ISI_CFG1 & ISI_CFG1_EMB_SYNC_Msk) >> ISI_CFG1_EMB_SYNC_Pos;

    return regVal;
}

void PLIB_ISI_CFG1_EMB_SYNC_Set(uint32_t val)
{
    uint32_t regVal;

    regVal = ISI_REGS->ISI_CFG1;
    regVal &= ~(ISI_CFG1_EMB_SYNC_Msk);
    regVal |= ISI_CFG1_EMB_SYNC(val);

    ISI_REGS->ISI_CFG1 = regVal;
}

uint32_t PLIB_ISI_CFG1_CRC_SYNC_Get(void)
{
    uint32_t regVal;

    regVal = (ISI_REGS->ISI_CFG1 & ISI_CFG1_CRC_SYNC_Msk) >> ISI_CFG1_CRC_SYNC_Pos;

    return regVal;
}

void PLIB_ISI_CFG1_CRC_SYNC_Set(uint32_t val)
{
    uint32_t regVal;

    regVal = ISI_REGS->ISI_CFG1;
    regVal &= ~(ISI_CFG1_CRC_SYNC_Msk);
    regVal |= ISI_CFG1_CRC_SYNC(val);

    ISI_REGS->ISI_CFG1 = regVal;
}

uint32_t PLIB_ISI_CFG1_FRATE_Get(void)
{
    uint32_t regVal;

    regVal = (ISI_REGS->ISI_CFG1 & ISI_CFG1_FRATE_Msk) >> ISI_CFG1_FRATE_Pos;

    return regVal;
}

void PLIB_ISI_CFG1_FRATE_Set(uint32_t val)
{
    uint32_t regVal;

    regVal = ISI_REGS->ISI_CFG1;
    regVal &= ~(ISI_CFG1_FRATE_Msk);
    regVal |= ISI_CFG1_FRATE(val);

    ISI_REGS->ISI_CFG1 = regVal;
}

uint32_t PLIB_ISI_CFG1_DISCR_Get(void)
{
    uint32_t regVal;

    regVal = (ISI_REGS->ISI_CFG1 & ISI_CFG1_DISCR_Msk) >> ISI_CFG1_DISCR_Pos;

    return regVal;
}

void PLIB_ISI_CFG1_DISCR_Set(uint32_t val)
{
    uint32_t regVal;

    regVal = ISI_REGS->ISI_CFG1;
    regVal &= ~(ISI_CFG1_DISCR_Msk);
    regVal |= ISI_CFG1_DISCR(val);

    ISI_REGS->ISI_CFG1 = regVal;
}

uint32_t PLIB_ISI_CFG1_FULL_Get(void)
{
    uint32_t regVal;

    regVal = (ISI_REGS->ISI_CFG1 & ISI_CFG1_FULL_Msk) >> ISI_CFG1_FULL_Pos;

    return regVal;
}

void PLIB_ISI_CFG1_FULL_Set(uint32_t val)
{
    uint32_t regVal;

    regVal = ISI_REGS->ISI_CFG1;
    regVal &= ~(ISI_CFG1_FULL_Msk);
    regVal |= ISI_CFG1_FULL(val);

    ISI_REGS->ISI_CFG1 = regVal;
}

ISI_THMASK_Value PLIB_ISI_CFG1_THMASK_Get(void)
{
    uint32_t regVal;

    regVal = (ISI_REGS->ISI_CFG1 & ISI_CFG1_THMASK_Msk) >> ISI_CFG1_THMASK_Pos;

    return regVal;
}

void PLIB_ISI_CFG1_THMASK_Set(ISI_THMASK_Value val)
{
    uint32_t regVal;

    regVal = ISI_REGS->ISI_CFG1;
    regVal &= ~(ISI_CFG1_THMASK_Msk);
    regVal |= ISI_CFG1_THMASK(val);

    ISI_REGS->ISI_CFG1 = regVal;
}

uint32_t PLIB_ISI_CFG1_SLD_Get(void)
{
    uint32_t regVal;

    regVal = (ISI_REGS->ISI_CFG1 & ISI_CFG1_SLD_Msk) >> ISI_CFG1_SLD_Pos;

    return regVal;
}

void PLIB_ISI_CFG1_SLD_Set(uint32_t val)
{
    uint32_t regVal;

    regVal = ISI_REGS->ISI_CFG1;
    regVal &= ~(ISI_CFG1_SLD_Msk);
    regVal |= ISI_CFG1_SLD(val);

    ISI_REGS->ISI_CFG1 = regVal;
}

uint32_t PLIB_ISI_CFG1_SFD_Get(void)
{
    uint32_t regVal;

    regVal = (ISI_REGS->ISI_CFG1 & ISI_CFG1_SFD_Msk) >> ISI_CFG1_SFD_Pos;

    return regVal;
}

void PLIB_ISI_CFG1_SFD_Set(uint32_t val)
{
    uint32_t regVal;

    regVal = ISI_REGS->ISI_CFG1;
    regVal &= ~(ISI_CFG1_SFD_Msk);
    regVal |= ISI_CFG1_SFD(val);

    ISI_REGS->ISI_CFG1 = regVal;
}

uint32_t PLIB_ISI_CFG2_IM_VSIZE_Get(void)
{
    uint32_t regVal;

    regVal = (ISI_REGS->ISI_CFG2 & ISI_CFG2_IM_VSIZE_Msk) >> ISI_CFG2_IM_VSIZE_Pos;

    return regVal;
}

void PLIB_ISI_CFG2_IM_VSIZE_Set(uint32_t val)
{
    uint32_t regVal;

    regVal = ISI_REGS->ISI_CFG2;
    regVal &= ~(ISI_CFG2_IM_VSIZE_Msk);
    regVal |= ISI_CFG2_IM_VSIZE(val);

    ISI_REGS->ISI_CFG2 = regVal;
}

uint32_t PLIB_ISI_CFG2_GS_MODE_Get(void)
{
    uint32_t regVal;

    regVal = (ISI_REGS->ISI_CFG2 & ISI_CFG2_GS_MODE_Msk) >> ISI_CFG2_GS_MODE_Pos;

    return regVal;
}

void PLIB_ISI_CFG2_GS_MODE_Set(uint32_t val)
{
    uint32_t regVal;

    regVal = ISI_REGS->ISI_CFG2;
    regVal &= ~(ISI_CFG2_GS_MODE_Msk);
    regVal |= ISI_CFG2_GS_MODE(val);

    ISI_REGS->ISI_CFG2 = regVal;
}

uint32_t PLIB_ISI_CFG2_RGB_MODE_Get(void)
{
    uint32_t regVal;

    regVal = (ISI_REGS->ISI_CFG2 & ISI_CFG2_RGB_MODE_Msk) >> ISI_CFG2_RGB_MODE_Pos;

    return regVal;
}

void PLIB_ISI_CFG2_RGB_MODE_Set(uint32_t val)
{
    uint32_t regVal;

    regVal = ISI_REGS->ISI_CFG2;
    regVal &= ~(ISI_CFG2_RGB_MODE_Msk);
    regVal |= ISI_CFG2_RGB_MODE(val);

    ISI_REGS->ISI_CFG2 = regVal;
}

uint32_t PLIB_ISI_CFG2_GRAYSCALE_Get(void)
{
    uint32_t regVal;

    regVal = (ISI_REGS->ISI_CFG2 & ISI_CFG2_GRAYSCALE_Msk) >> ISI_CFG2_GRAYSCALE_Pos;

    return regVal;
}

void PLIB_ISI_CFG2_GRAYSCALE_Set(uint32_t val)
{
    uint32_t regVal;

    regVal = ISI_REGS->ISI_CFG2;
    regVal &= ~(ISI_CFG2_GRAYSCALE_Msk);
    regVal |= ISI_CFG2_GRAYSCALE(val);

    ISI_REGS->ISI_CFG2 = regVal;
}

uint32_t PLIB_ISI_CFG2_RGB_SWAP_Get(void)
{
    uint32_t regVal;

    regVal = (ISI_REGS->ISI_CFG2 & ISI_CFG2_RGB_SWAP_Msk) >> ISI_CFG2_RGB_SWAP_Pos;

    return regVal;
}

void PLIB_ISI_CFG2_RGB_SWAP_Set(uint32_t val)
{
    uint32_t regVal;

    regVal = ISI_REGS->ISI_CFG2;
    regVal &= ~(ISI_CFG2_RGB_SWAP_Msk);
    regVal |= ISI_CFG2_RGB_SWAP(val);

    ISI_REGS->ISI_CFG2 = regVal;
}

uint32_t PLIB_ISI_CFG2_COL_SPACE_Get(void)
{
    uint32_t regVal;

    regVal = (ISI_REGS->ISI_CFG2 & ISI_CFG2_COL_SPACE_Msk) >> ISI_CFG2_COL_SPACE_Pos;

    return regVal;
}

void PLIB_ISI_CFG2_COL_SPACE_Set(uint32_t val)
{
    uint32_t regVal;

    regVal = ISI_REGS->ISI_CFG2;
    regVal &= ~(ISI_CFG2_COL_SPACE_Msk);
    regVal |= ISI_CFG2_COL_SPACE(val);

    ISI_REGS->ISI_CFG2 = regVal;
}

uint32_t PLIB_ISI_CFG2_IM_HSIZE_Get(void)
{
    uint32_t regVal;

    regVal = (ISI_REGS->ISI_CFG2 & ISI_CFG2_IM_HSIZE_Msk) >> ISI_CFG2_IM_HSIZE_Pos;

    return regVal;
}

void PLIB_ISI_CFG2_IM_HSIZE_Set(uint32_t val)
{
    uint32_t regVal;

    regVal = ISI_REGS->ISI_CFG2;
    regVal &= ~(ISI_CFG2_IM_HSIZE_Msk);
    regVal |= ISI_CFG2_IM_HSIZE(val);

    ISI_REGS->ISI_CFG2 = regVal;
}

ISI_YCC_SWAP_Value PLIB_ISI_CFG2_YCC_SWAP_Get(void)
{
    uint32_t regVal;

    regVal = (ISI_REGS->ISI_CFG2 & ISI_CFG2_YCC_SWAP_Msk) >> ISI_CFG2_YCC_SWAP_Pos;

    return regVal;
}

void PLIB_ISI_CFG2_YCC_SWAP_Set(ISI_YCC_SWAP_Value val)
{
    uint32_t regVal;

    regVal = ISI_REGS->ISI_CFG2;
    regVal &= ~(ISI_CFG2_YCC_SWAP_Msk);
    regVal |= ISI_CFG2_YCC_SWAP(val);

    ISI_REGS->ISI_CFG2 = regVal;
}

ISI_RGB_CFG_Value PLIB_ISI_CFG2_RGB_CFG_Get(void)
{
    uint32_t regVal;

    regVal = (ISI_REGS->ISI_CFG2 & ISI_CFG2_RGB_CFG_Msk) >> ISI_CFG2_RGB_CFG_Pos;

    return regVal;
}

void PLIB_ISI_CFG2_RGB_CFG_Set(ISI_RGB_CFG_Value val)
{
    uint32_t regVal;

    regVal = ISI_REGS->ISI_CFG2;
    regVal &= ~(ISI_CFG2_RGB_CFG_Msk);
    regVal |= ISI_CFG2_RGB_CFG(val);

    ISI_REGS->ISI_CFG2 = regVal;
}

uint32_t PLIB_ISI_PSIZE_PREV_VSIZE_Get(void)
{
    uint32_t regVal;

    regVal = (ISI_REGS->ISI_PSIZE & ISI_PSIZE_PREV_VSIZE_Msk) >> ISI_PSIZE_PREV_VSIZE_Pos;

    return regVal;
}

void PLIB_ISI_PSIZE_PREV_VSIZE_Set(uint32_t val)
{
    uint32_t regVal;

    regVal = ISI_REGS->ISI_PSIZE;
    regVal &= ~(ISI_PSIZE_PREV_VSIZE_Msk);
    regVal |= ISI_PSIZE_PREV_VSIZE(val);

    ISI_REGS->ISI_PSIZE = regVal;
}

uint32_t PLIB_ISI_PSIZE_PREV_HSIZE_Get(void)
{
    uint32_t regVal;

    regVal = (ISI_REGS->ISI_PSIZE & ISI_PSIZE_PREV_HSIZE_Msk) >> ISI_PSIZE_PREV_HSIZE_Pos;

    return regVal;
}

void PLIB_ISI_PSIZE_PREV_HSIZE_Set(uint32_t val)
{
    uint32_t regVal;

    regVal = ISI_REGS->ISI_PSIZE;
    regVal &= ~(ISI_PSIZE_PREV_HSIZE_Msk);
    regVal |= ISI_PSIZE_PREV_HSIZE(val);

    ISI_REGS->ISI_PSIZE = regVal;
}

uint32_t PLIB_ISI_PDECF_DEC_FACTOR_Get(void)
{
    uint32_t regVal;

    regVal = (ISI_REGS->ISI_PDECF & ISI_PDECF_DEC_FACTOR_Msk) >> ISI_PDECF_DEC_FACTOR_Pos;

    return regVal;
}

void PLIB_ISI_PDECF_DEC_FACTOR_Set(uint32_t val)
{
    uint32_t regVal;

    regVal = ISI_REGS->ISI_PDECF;
    regVal &= ~(ISI_PDECF_DEC_FACTOR_Msk);
    regVal |= ISI_PDECF_DEC_FACTOR(val);

    ISI_REGS->ISI_PDECF = regVal;
}

uint32_t PLIB_ISI_Y2R_SET0_C0_Get(void)
{
    uint32_t regVal;

    regVal = (ISI_REGS->ISI_Y2R_SET0 & ISI_Y2R_SET0_C0_Msk) >> ISI_Y2R_SET0_C0_Pos;

    return regVal;
}

void PLIB_ISI_Y2R_SET0_C0_Set(uint32_t val)
{
    uint32_t regVal;

    regVal = ISI_REGS->ISI_Y2R_SET0;
    regVal &= ~(ISI_Y2R_SET0_C0_Msk);
    regVal |= ISI_Y2R_SET0_C0(val);

    ISI_REGS->ISI_Y2R_SET0 = regVal;
}

uint32_t PLIB_ISI_Y2R_SET0_C1_Get(void)
{
    uint32_t regVal;

    regVal = (ISI_REGS->ISI_Y2R_SET0 & ISI_Y2R_SET0_C1_Msk) >> ISI_Y2R_SET0_C1_Pos;

    return regVal;
}

void PLIB_ISI_Y2R_SET0_C1_Set(uint32_t val)
{
    uint32_t regVal;

    regVal = ISI_REGS->ISI_Y2R_SET0;
    regVal &= ~(ISI_Y2R_SET0_C1_Msk);
    regVal |= ISI_Y2R_SET0_C1(val);

    ISI_REGS->ISI_Y2R_SET0 = regVal;
}

uint32_t PLIB_ISI_Y2R_SET0_C2_Get(void)
{
    uint32_t regVal;

    regVal = (ISI_REGS->ISI_Y2R_SET0 & ISI_Y2R_SET0_C2_Msk) >> ISI_Y2R_SET0_C2_Pos;

    return regVal;
}

void PLIB_ISI_Y2R_SET0_C2_Set(uint32_t val)
{
    uint32_t regVal;

    regVal = ISI_REGS->ISI_Y2R_SET0;
    regVal &= ~(ISI_Y2R_SET0_C2_Msk);
    regVal |= ISI_Y2R_SET0_C2(val);

    ISI_REGS->ISI_Y2R_SET0 = regVal;
}

uint32_t PLIB_ISI_Y2R_SET0_C3_Get(void)
{
    uint32_t regVal;

    regVal = (ISI_REGS->ISI_Y2R_SET0 & ISI_Y2R_SET0_C3_Msk) >> ISI_Y2R_SET0_C3_Pos;

    return regVal;
}

void PLIB_ISI_Y2R_SET0_C3_Set(uint32_t val)
{
    uint32_t regVal;

    regVal = ISI_REGS->ISI_Y2R_SET0;
    regVal &= ~(ISI_Y2R_SET0_C3_Msk);
    regVal |= ISI_Y2R_SET0_C3(val);

    ISI_REGS->ISI_Y2R_SET0 = regVal;
}

uint32_t PLIB_ISI_Y2R_SET1_C4_Get(void)
{
    uint32_t regVal;

    regVal = (ISI_REGS->ISI_Y2R_SET1 & ISI_Y2R_SET1_C4_Msk) >> ISI_Y2R_SET1_C4_Pos;

    return regVal;
}

void PLIB_ISI_Y2R_SET1_C4_Set(uint32_t val)
{
    uint32_t regVal;

    regVal = ISI_REGS->ISI_Y2R_SET1;
    regVal &= ~(ISI_Y2R_SET1_C4_Msk);
    regVal |= ISI_Y2R_SET1_C4(val);

    ISI_REGS->ISI_Y2R_SET1 = regVal;
}

uint32_t PLIB_ISI_Y2R_SET1_Yoff_Get(void)
{
    uint32_t regVal;

    regVal = (ISI_REGS->ISI_Y2R_SET1 & ISI_Y2R_SET1_Yoff_Msk) >> ISI_Y2R_SET1_Yoff_Pos;

    return regVal;
}

void PLIB_ISI_Y2R_SET1_Yoff_Set(uint32_t val)
{
    uint32_t regVal;

    regVal = ISI_REGS->ISI_Y2R_SET1;
    regVal &= ~(ISI_Y2R_SET1_Yoff_Msk);
    regVal |= ISI_Y2R_SET1_Yoff(val);

    ISI_REGS->ISI_Y2R_SET1 = regVal;
}

uint32_t PLIB_ISI_Y2R_SET1_Croff_Get(void)
{
    uint32_t regVal;

    regVal = (ISI_REGS->ISI_Y2R_SET1 & ISI_Y2R_SET1_Croff_Msk) >> ISI_Y2R_SET1_Croff_Pos;

    return regVal;
}

void PLIB_ISI_Y2R_SET1_Croff_Set(uint32_t val)
{
    uint32_t regVal;

    regVal = ISI_REGS->ISI_Y2R_SET1;
    regVal &= ~(ISI_Y2R_SET1_Croff_Msk);
    regVal |= ISI_Y2R_SET1_Croff(val);

    ISI_REGS->ISI_Y2R_SET1 = regVal;
}

uint32_t PLIB_ISI_Y2R_SET1_Cboff_Get(void)
{
    uint32_t regVal;

    regVal = (ISI_REGS->ISI_Y2R_SET1 & ISI_Y2R_SET1_Cboff_Msk) >> ISI_Y2R_SET1_Cboff_Pos;

    return regVal;
}

void PLIB_ISI_Y2R_SET1_Cboff_Set(uint32_t val)
{
    uint32_t regVal;

    regVal = ISI_REGS->ISI_Y2R_SET1;
    regVal &= ~(ISI_Y2R_SET1_Cboff_Msk);
    regVal |= ISI_Y2R_SET1_Cboff(val);

    ISI_REGS->ISI_Y2R_SET1 = regVal;
}

uint32_t PLIB_ISI_R2Y_SET0_C0_Get(void)
{
    uint32_t regVal;

    regVal = (ISI_REGS->ISI_R2Y_SET0 & ISI_R2Y_SET0_C0_Msk) >> ISI_R2Y_SET0_C0_Pos;

    return regVal;
}

void PLIB_ISI_R2Y_SET0_C0_Set(uint32_t val)
{
    uint32_t regVal;

    regVal = ISI_REGS->ISI_R2Y_SET0;
    regVal &= ~(ISI_R2Y_SET0_C0_Msk);
    regVal |= ISI_R2Y_SET0_C0(val);

    ISI_REGS->ISI_R2Y_SET0 = regVal;
}

uint32_t PLIB_ISI_R2Y_SET0_C1_Get(void)
{
    uint32_t regVal;

    regVal = (ISI_REGS->ISI_R2Y_SET0 & ISI_R2Y_SET0_C1_Msk) >> ISI_R2Y_SET0_C1_Pos;

    return regVal;
}

void PLIB_ISI_R2Y_SET0_C1_Set(uint32_t val)
{
    uint32_t regVal;

    regVal = ISI_REGS->ISI_R2Y_SET0;
    regVal &= ~(ISI_R2Y_SET0_C1_Msk);
    regVal |= ISI_R2Y_SET0_C1(val);

    ISI_REGS->ISI_R2Y_SET0 = regVal;
}

uint32_t PLIB_ISI_R2Y_SET0_C2_Get(void)
{
    uint32_t regVal;

    regVal = (ISI_REGS->ISI_R2Y_SET0 & ISI_R2Y_SET0_C2_Msk) >> ISI_R2Y_SET0_C2_Pos;

    return regVal;
}

void PLIB_ISI_R2Y_SET0_C2_Set(uint32_t val)
{
    uint32_t regVal;

    regVal = ISI_REGS->ISI_R2Y_SET0;
    regVal &= ~(ISI_R2Y_SET0_C2_Msk);
    regVal |= ISI_R2Y_SET0_C2(val);

    ISI_REGS->ISI_R2Y_SET0 = regVal;
}

uint32_t PLIB_ISI_R2Y_SET0_Roff_Get(void)
{
    uint32_t regVal;

    regVal = (ISI_REGS->ISI_R2Y_SET0 & ISI_R2Y_SET0_Roff_Msk) >> ISI_R2Y_SET0_Roff_Pos;

    return regVal;
}

void PLIB_ISI_R2Y_SET0_Roff_Set(uint32_t val)
{
    uint32_t regVal;

    regVal = ISI_REGS->ISI_R2Y_SET0;
    regVal &= ~(ISI_R2Y_SET0_Roff_Msk);
    regVal |= ISI_R2Y_SET0_Roff(val);

    ISI_REGS->ISI_R2Y_SET0 = regVal;
}

uint32_t PLIB_ISI_R2Y_SET1_C3_Get(void)
{
    uint32_t regVal;

    regVal = (ISI_REGS->ISI_R2Y_SET1 & ISI_R2Y_SET1_C3_Msk) >> ISI_R2Y_SET1_C3_Pos;

    return regVal;
}

void PLIB_ISI_R2Y_SET1_C3_Set(uint32_t val)
{
    uint32_t regVal;

    regVal = ISI_REGS->ISI_R2Y_SET1;
    regVal &= ~(ISI_R2Y_SET1_C3_Msk);
    regVal |= ISI_R2Y_SET1_C3(val);

    ISI_REGS->ISI_R2Y_SET1 = regVal;
}

uint32_t PLIB_ISI_R2Y_SET1_C4_Get(void)
{
    uint32_t regVal;

    regVal = (ISI_REGS->ISI_R2Y_SET1 & ISI_R2Y_SET1_C4_Msk) >> ISI_R2Y_SET1_C4_Pos;

    return regVal;
}

void PLIB_ISI_R2Y_SET1_C4_Set(uint32_t val)
{
    uint32_t regVal;

    regVal = ISI_REGS->ISI_R2Y_SET1;
    regVal &= ~(ISI_R2Y_SET1_C4_Msk);
    regVal |= ISI_R2Y_SET1_C4(val);

    ISI_REGS->ISI_R2Y_SET1 = regVal;
}

uint32_t PLIB_ISI_R2Y_SET1_C5_Get(void)
{
    uint32_t regVal;

    regVal = (ISI_REGS->ISI_R2Y_SET1 & ISI_R2Y_SET1_C5_Msk) >> ISI_R2Y_SET1_C5_Pos;

    return regVal;
}

void PLIB_ISI_R2Y_SET1_C5_Set(uint32_t val)
{
    uint32_t regVal;

    regVal = ISI_REGS->ISI_R2Y_SET1;
    regVal &= ~(ISI_R2Y_SET1_C5_Msk);
    regVal |= ISI_R2Y_SET1_C5(val);

    ISI_REGS->ISI_R2Y_SET1 = regVal;
}

uint32_t PLIB_ISI_R2Y_SET1_Goff_Get(void)
{
    uint32_t regVal;

    regVal = (ISI_REGS->ISI_R2Y_SET1 & ISI_R2Y_SET1_Goff_Msk) >> ISI_R2Y_SET1_Goff_Pos;

    return regVal;
}

void PLIB_ISI_R2Y_SET1_Goff_Set(uint32_t val)
{
    uint32_t regVal;

    regVal = ISI_REGS->ISI_R2Y_SET1;
    regVal &= ~(ISI_R2Y_SET1_Goff_Msk);
    regVal |= ISI_R2Y_SET1_Goff(val);

    ISI_REGS->ISI_R2Y_SET1 = regVal;
}

uint32_t PLIB_ISI_R2Y_SET2_C6_Get(void)
{
    uint32_t regVal;

    regVal = (ISI_REGS->ISI_R2Y_SET2 & ISI_R2Y_SET2_C6_Msk) >> ISI_R2Y_SET2_C6_Pos;

    return regVal;
}

void PLIB_ISI_R2Y_SET2_C6_Set(uint32_t val)
{
    uint32_t regVal;

    regVal = ISI_REGS->ISI_R2Y_SET2;
    regVal &= ~(ISI_R2Y_SET2_C6_Msk);
    regVal |= ISI_R2Y_SET2_C6(val);

    ISI_REGS->ISI_R2Y_SET2 = regVal;
}

uint32_t PLIB_ISI_R2Y_SET2_C7_Get(void)
{
    uint32_t regVal;

    regVal = (ISI_REGS->ISI_R2Y_SET2 & ISI_R2Y_SET2_C7_Msk) >> ISI_R2Y_SET2_C7_Pos;

    return regVal;
}

void PLIB_ISI_R2Y_SET2_C7_Set(uint32_t val)
{
    uint32_t regVal;

    regVal = ISI_REGS->ISI_R2Y_SET2;
    regVal &= ~(ISI_R2Y_SET2_C7_Msk);
    regVal |= ISI_R2Y_SET2_C7(val);

    ISI_REGS->ISI_R2Y_SET2 = regVal;
}

uint32_t PLIB_ISI_R2Y_SET2_C8_Get(void)
{
    uint32_t regVal;

    regVal = (ISI_REGS->ISI_R2Y_SET2 & ISI_R2Y_SET2_C8_Msk) >> ISI_R2Y_SET2_C8_Pos;

    return regVal;
}

void PLIB_ISI_R2Y_SET2_C8_Set(uint32_t val)
{
    uint32_t regVal;

    regVal = ISI_REGS->ISI_R2Y_SET2;
    regVal &= ~(ISI_R2Y_SET2_C8_Msk);
    regVal |= ISI_R2Y_SET2_C8(val);

    ISI_REGS->ISI_R2Y_SET2 = regVal;
}

uint32_t PLIB_ISI_R2Y_SET2_Boff_Get(void)
{
    uint32_t regVal;

    regVal = (ISI_REGS->ISI_R2Y_SET2 & ISI_R2Y_SET2_Boff_Msk) >> ISI_R2Y_SET2_Boff_Pos;

    return regVal;
}

void PLIB_ISI_R2Y_SET2_Boff_Set(uint32_t val)
{
    uint32_t regVal;

    regVal = ISI_REGS->ISI_R2Y_SET2;
    regVal &= ~(ISI_R2Y_SET2_Boff_Msk);
    regVal |= ISI_R2Y_SET2_Boff(val);

    ISI_REGS->ISI_R2Y_SET2 = regVal;
}

void PLIB_ISI_CR_ISI_EN_Set(uint32_t val)
{
    volatile uint32_t regVal;

    regVal = ISI_REGS->ISI_CR;
    regVal &= ~(ISI_CR_ISI_EN_Msk);
    regVal |= ISI_CR_ISI_EN(val);

    ISI_REGS->ISI_CR = regVal;
}

void PLIB_ISI_CR_ISI_DIS_Set(uint32_t val)
{
    uint32_t regVal;

    regVal = ISI_REGS->ISI_CR;
    regVal &= ~(ISI_CR_ISI_DIS_Msk);
    regVal |= ISI_CR_ISI_DIS(val);

    ISI_REGS->ISI_CR = regVal;
}

void PLIB_ISI_CR_ISI_SRST_Set(uint32_t val)
{
    uint32_t regVal;

    regVal = ISI_REGS->ISI_CR;
    regVal &= ~(ISI_CR_ISI_SRST_Msk);
    regVal |= ISI_CR_ISI_SRST(val);

    ISI_REGS->ISI_CR = regVal;
}

void PLIB_ISI_CR_ISI_CDC_Set(uint32_t val)
{
    uint32_t regVal;

    regVal = ISI_REGS->ISI_CR;
    regVal &= ~(ISI_CR_ISI_CDC_Msk);
    regVal |= ISI_CR_ISI_CDC(val);

    ISI_REGS->ISI_CR = regVal;
}

uint32_t PLIB_ISI_SR_ENABLE_Get(void)
{
    uint32_t regVal;

    regVal = (ISI_REGS->ISI_SR & ISI_SR_ENABLE_Msk) >> ISI_SR_ENABLE_Pos;

    return regVal;
}

uint32_t PLIB_ISI_SR_DIS_DONE_Get(void)
{
    uint32_t regVal;

    regVal = (ISI_REGS->ISI_SR & ISI_SR_DIS_DONE_Msk) >> ISI_SR_DIS_DONE_Pos;

    return regVal;
}

uint32_t PLIB_ISI_SR_SRST_Get(void)
{
    uint32_t regVal;

    regVal = (ISI_REGS->ISI_SR & ISI_SR_SRST_Msk) >> ISI_SR_SRST_Pos;

    return regVal;
}

uint32_t PLIB_ISI_SR_CDC_PND_Get(void)
{
    uint32_t regVal;

    regVal = (ISI_REGS->ISI_SR & ISI_SR_CDC_PND_Msk) >> ISI_SR_CDC_PND_Pos;

    return regVal;
}

uint32_t PLIB_ISI_SR_VSYNC_Get(void)
{
    uint32_t regVal;

    regVal = (ISI_REGS->ISI_SR & ISI_SR_VSYNC_Msk) >> ISI_SR_VSYNC_Pos;

    return regVal;
}

uint32_t PLIB_ISI_SR_PXFR_DONE_Get(void)
{
    uint32_t regVal;

    regVal = (ISI_REGS->ISI_SR & ISI_SR_PXFR_DONE_Msk) >> ISI_SR_PXFR_DONE_Pos;

    return regVal;
}

uint32_t PLIB_ISI_SR_CXFR_DONE_Get(void)
{
    uint32_t regVal;

    regVal = (ISI_REGS->ISI_SR & ISI_SR_CXFR_DONE_Msk) >> ISI_SR_CXFR_DONE_Pos;

    return regVal;
}

uint32_t PLIB_ISI_SR_SIP_Get(void)
{
    uint32_t regVal;

    regVal = (ISI_REGS->ISI_SR & ISI_SR_SIP_Msk) >> ISI_SR_SIP_Pos;

    return regVal;
}

uint32_t PLIB_ISI_SR_P_OVR_Get(void)
{
    uint32_t regVal;

    regVal = (ISI_REGS->ISI_SR & ISI_SR_P_OVR_Msk) >> ISI_SR_P_OVR_Pos;

    return regVal;
}

uint32_t PLIB_ISI_SR_C_OVR_Get(void)
{
    uint32_t regVal;

    regVal = (ISI_REGS->ISI_SR & ISI_SR_C_OVR_Msk) >> ISI_SR_C_OVR_Pos;

    return regVal;
}

uint32_t PLIB_ISI_SR_CRC_ERR_Get(void)
{
    uint32_t regVal;

    regVal = (ISI_REGS->ISI_SR & ISI_SR_CRC_ERR_Msk) >> ISI_SR_CRC_ERR_Pos;

    return regVal;
}

uint32_t PLIB_ISI_SR_FR_OVR_Get(void)
{
    uint32_t regVal;

    regVal = (ISI_REGS->ISI_SR & ISI_SR_FR_OVR_Msk) >> ISI_SR_FR_OVR_Pos;

    return regVal;
}

void PLIB_ISI_IER_DIS_DONE_Set(uint32_t val)
{
    uint32_t regVal;

    regVal = ISI_REGS->ISI_IER;
    regVal &= ~(ISI_IER_DIS_DONE_Msk);
    regVal |= ISI_IER_DIS_DONE(val);

    ISI_REGS->ISI_IER = regVal;
}

void PLIB_ISI_IER_SRST_Set(uint32_t val)
{
    uint32_t regVal;

    regVal = ISI_REGS->ISI_IER;
    regVal &= ~(ISI_IER_SRST_Msk);
    regVal |= ISI_IER_SRST(val);

    ISI_REGS->ISI_IER = regVal;
}

void PLIB_ISI_IER_VSYNC_Set(uint32_t val)
{
    uint32_t regVal;

    regVal = ISI_REGS->ISI_IER;
    regVal &= ~(ISI_IER_VSYNC_Msk);
    regVal |= ISI_IER_VSYNC(val);

    ISI_REGS->ISI_IER = regVal;
}

void PLIB_ISI_IER_PXFR_DONE_Set(uint32_t val)
{
    uint32_t regVal;

    regVal = ISI_REGS->ISI_IER;
    regVal &= ~(ISI_IER_PXFR_DONE_Msk);
    regVal |= ISI_IER_PXFR_DONE(val);

    ISI_REGS->ISI_IER = regVal;
}

void PLIB_ISI_IER_CXFR_DONE_Set(uint32_t val)
{
    uint32_t regVal;

    regVal = ISI_REGS->ISI_IER;
    regVal &= ~(ISI_IER_CXFR_DONE_Msk);
    regVal |= ISI_IER_CXFR_DONE(val);

    ISI_REGS->ISI_IER = regVal;
}

void PLIB_ISI_IER_P_OVR_Set(uint32_t val)
{
    uint32_t regVal;

    regVal = ISI_REGS->ISI_IER;
    regVal &= ~(ISI_IER_P_OVR_Msk);
    regVal |= ISI_IER_P_OVR(val);

    ISI_REGS->ISI_IER = regVal;
}

void PLIB_ISI_IER_C_OVR_Set(uint32_t val)
{
    uint32_t regVal;

    regVal = ISI_REGS->ISI_IER;
    regVal &= ~(ISI_IER_C_OVR_Msk);
    regVal |= ISI_IER_C_OVR(val);

    ISI_REGS->ISI_IER = regVal;
}

void PLIB_ISI_IER_CRC_ERR_Set(uint32_t val)
{
    uint32_t regVal;

    regVal = ISI_REGS->ISI_IER;
    regVal &= ~(ISI_IER_CRC_ERR_Msk);
    regVal |= ISI_IER_CRC_ERR(val);

    ISI_REGS->ISI_IER = regVal;
}

void PLIB_ISI_IER_FR_OVR_Set(uint32_t val)
{
    uint32_t regVal;

    regVal = ISI_REGS->ISI_IER;
    regVal &= ~(ISI_IER_FR_OVR_Msk);
    regVal |= ISI_IER_FR_OVR(val);

    ISI_REGS->ISI_IER = regVal;
}

void PLIB_ISI_IDR_DIS_DONE_Set(uint32_t val)
{
    uint32_t regVal;

    regVal = ISI_REGS->ISI_IDR;
    regVal &= ~(ISI_IDR_DIS_DONE_Msk);
    regVal |= ISI_IDR_DIS_DONE(val);

    ISI_REGS->ISI_IDR = regVal;
}

void PLIB_ISI_IDR_SRST_Set(uint32_t val)
{
    uint32_t regVal;

    regVal = ISI_REGS->ISI_IDR;
    regVal &= ~(ISI_IDR_SRST_Msk);
    regVal |= ISI_IDR_SRST(val);

    ISI_REGS->ISI_IDR = regVal;
}

void PLIB_ISI_IDR_VSYNC_Set(uint32_t val)
{
    uint32_t regVal;

    regVal = ISI_REGS->ISI_IDR;
    regVal &= ~(ISI_IDR_VSYNC_Msk);
    regVal |= ISI_IDR_VSYNC(val);

    ISI_REGS->ISI_IDR = regVal;
}

void PLIB_ISI_IDR_PXFR_DONE_Set(uint32_t val)
{
    uint32_t regVal;

    regVal = ISI_REGS->ISI_IDR;
    regVal &= ~(ISI_IDR_PXFR_DONE_Msk);
    regVal |= ISI_IDR_PXFR_DONE(val);

    ISI_REGS->ISI_IDR = regVal;
}

void PLIB_ISI_IDR_CXFR_DONE_Set(uint32_t val)
{
    uint32_t regVal;

    regVal = ISI_REGS->ISI_IDR;
    regVal &= ~(ISI_IDR_CXFR_DONE_Msk);
    regVal |= ISI_IDR_CXFR_DONE(val);

    ISI_REGS->ISI_IDR = regVal;
}

void PLIB_ISI_IDR_P_OVR_Set(uint32_t val)
{
    uint32_t regVal;

    regVal = ISI_REGS->ISI_IDR;
    regVal &= ~(ISI_IDR_P_OVR_Msk);
    regVal |= ISI_IDR_P_OVR(val);

    ISI_REGS->ISI_IDR = regVal;
}

void PLIB_ISI_IDR_C_OVR_Set(uint32_t val)
{
    uint32_t regVal;

    regVal = ISI_REGS->ISI_IDR;
    regVal &= ~(ISI_IDR_C_OVR_Msk);
    regVal |= ISI_IDR_C_OVR(val);

    ISI_REGS->ISI_IDR = regVal;
}

void PLIB_ISI_IDR_CRC_ERR_Set(uint32_t val)
{
    uint32_t regVal;

    regVal = ISI_REGS->ISI_IDR;
    regVal &= ~(ISI_IDR_CRC_ERR_Msk);
    regVal |= ISI_IDR_CRC_ERR(val);

    ISI_REGS->ISI_IDR = regVal;
}

void PLIB_ISI_IDR_FR_OVR_Set(uint32_t val)
{
    uint32_t regVal;

    regVal = ISI_REGS->ISI_IDR;
    regVal &= ~(ISI_IDR_FR_OVR_Msk);
    regVal |= ISI_IDR_FR_OVR(val);

    ISI_REGS->ISI_IDR = regVal;
}

uint32_t PLIB_ISI_IMR_DIS_DONE_Get(void)
{
    uint32_t regVal;

    regVal = (ISI_REGS->ISI_IMR & ISI_IMR_DIS_DONE_Msk) >> ISI_IMR_DIS_DONE_Pos;

    return regVal;
}

uint32_t PLIB_ISI_IMR_SRST_Get(void)
{
    uint32_t regVal;

    regVal = (ISI_REGS->ISI_IMR & ISI_IMR_SRST_Msk) >> ISI_IMR_SRST_Pos;

    return regVal;
}

uint32_t PLIB_ISI_IMR_VSYNC_Get(void)
{
    uint32_t regVal;

    regVal = (ISI_REGS->ISI_IMR & ISI_IMR_VSYNC_Msk) >> ISI_IMR_VSYNC_Pos;

    return regVal;
}

uint32_t PLIB_ISI_IMR_PXFR_DONE_Get(void)
{
    uint32_t regVal;

    regVal = (ISI_REGS->ISI_IMR & ISI_IMR_PXFR_DONE_Msk) >> ISI_IMR_PXFR_DONE_Pos;

    return regVal;
}

uint32_t PLIB_ISI_IMR_CXFR_DONE_Get(void)
{
    uint32_t regVal;

    regVal = (ISI_REGS->ISI_IMR & ISI_IMR_CXFR_DONE_Msk) >> ISI_IMR_CXFR_DONE_Pos;

    return regVal;
}

uint32_t PLIB_ISI_IMR_P_OVR_Get(void)
{
    uint32_t regVal;

    regVal = (ISI_REGS->ISI_IMR & ISI_IMR_P_OVR_Msk) >> ISI_IMR_P_OVR_Pos;

    return regVal;
}

uint32_t PLIB_ISI_IMR_C_OVR_Get(void)
{
    uint32_t regVal;

    regVal = (ISI_REGS->ISI_IMR & ISI_IMR_C_OVR_Msk) >> ISI_IMR_C_OVR_Pos;

    return regVal;
}

uint32_t PLIB_ISI_IMR_CRC_ERR_Get(void)
{
    uint32_t regVal;

    regVal = (ISI_REGS->ISI_IMR & ISI_IMR_CRC_ERR_Msk) >> ISI_IMR_CRC_ERR_Pos;

    return regVal;
}

uint32_t PLIB_ISI_IMR_FR_OVR_Get(void)
{
    uint32_t regVal;

    regVal = (ISI_REGS->ISI_IMR & ISI_IMR_FR_OVR_Msk) >> ISI_IMR_FR_OVR_Pos;

    return regVal;
}

void PLIB_ISI_DMA_CHER_P_CH_EN_Set(uint32_t val)
{
    uint32_t regVal;

    regVal = ISI_REGS->ISI_DMA_CHER;
    regVal &= ~(ISI_DMA_CHER_P_CH_EN_Msk);
    regVal |= ISI_DMA_CHER_P_CH_EN(val);

    ISI_REGS->ISI_DMA_CHER = regVal;
}

void PLIB_ISI_DMA_CHER_C_CH_EN_Set(uint32_t val)
{
    uint32_t regVal;

    regVal = ISI_REGS->ISI_DMA_CHER;
    regVal &= ~(ISI_DMA_CHER_C_CH_EN_Msk);
    regVal |= ISI_DMA_CHER_C_CH_EN(val);

    ISI_REGS->ISI_DMA_CHER = regVal;
}

void PLIB_ISI_DMA_CHDR_P_CH_DIS_Set(uint32_t val)
{
    uint32_t regVal;

    regVal = ISI_REGS->ISI_DMA_CHDR;
    regVal &= ~(ISI_DMA_CHDR_P_CH_DIS_Msk);
    regVal |= ISI_DMA_CHDR_P_CH_DIS(val);

    ISI_REGS->ISI_DMA_CHDR = regVal;
}

void PLIB_ISI_DMA_CHDR_C_CH_DIS_Set(uint32_t val)
{
    uint32_t regVal;

    regVal = ISI_REGS->ISI_DMA_CHDR;
    regVal &= ~(ISI_DMA_CHDR_C_CH_DIS_Msk);
    regVal |= ISI_DMA_CHDR_C_CH_DIS(val);

    ISI_REGS->ISI_DMA_CHDR = regVal;
}

uint32_t PLIB_ISI_DMA_CHSR_P_CH_S_Get(void)
{
    uint32_t regVal;

    regVal = (ISI_REGS->ISI_DMA_CHSR & ISI_DMA_CHSR_P_CH_S_Msk) >> ISI_DMA_CHSR_P_CH_S_Pos;

    return regVal;
}

uint32_t PLIB_ISI_DMA_CHSR_C_CH_S_Get(void)
{
    uint32_t regVal;

    regVal = (ISI_REGS->ISI_DMA_CHSR & ISI_DMA_CHSR_C_CH_S_Msk) >> ISI_DMA_CHSR_C_CH_S_Pos;

    return regVal;
}

uint32_t PLIB_ISI_DMA_P_ADDR_Get(void)
{
    return ISI_REGS->ISI_DMA_P_ADDR;
}

void PLIB_ISI_DMA_P_ADDR_Set(uint32_t val)
{
    ISI_REGS->ISI_DMA_P_ADDR = (val & 0xFFFFFFFC);
}

uint32_t PLIB_ISI_DMA_P_CTRL_P_FETCH_Get(void)
{
    uint32_t regVal;

    regVal = (ISI_REGS->ISI_DMA_P_CTRL & ISI_DMA_P_CTRL_P_FETCH_Msk) >> ISI_DMA_P_CTRL_P_FETCH_Pos;

    return regVal;
}

void PLIB_ISI_DMA_P_CTRL_P_FETCH_Set(uint32_t val)
{
    uint32_t regVal;

    regVal = ISI_REGS->ISI_DMA_P_CTRL;
    regVal &= ~(ISI_DMA_P_CTRL_P_FETCH_Msk);
    regVal |= ISI_DMA_P_CTRL_P_FETCH(val);

    ISI_REGS->ISI_DMA_P_CTRL = regVal;
}

uint32_t PLIB_ISI_DMA_P_CTRL_P_WB_Get(void)
{
    uint32_t regVal;

    regVal = (ISI_REGS->ISI_DMA_P_CTRL & ISI_DMA_P_CTRL_P_WB_Msk) >> ISI_DMA_P_CTRL_P_WB_Pos;

    return regVal;
}

void PLIB_ISI_DMA_P_CTRL_P_WB_Set(uint32_t val)
{
    uint32_t regVal;

    regVal = ISI_REGS->ISI_DMA_P_CTRL;
    regVal &= ~(ISI_DMA_P_CTRL_P_WB_Msk);
    regVal |= ISI_DMA_P_CTRL_P_WB(val);

    ISI_REGS->ISI_DMA_P_CTRL = regVal;
}

uint32_t PLIB_ISI_DMA_P_CTRL_P_IEN_Get(void)
{
    uint32_t regVal;

    regVal = (ISI_REGS->ISI_DMA_P_CTRL & ISI_DMA_P_CTRL_P_IEN_Msk) >> ISI_DMA_P_CTRL_P_IEN_Pos;

    return regVal;
}

void PLIB_ISI_DMA_P_CTRL_P_IEN_Set(uint32_t val)
{
    uint32_t regVal;

    regVal = ISI_REGS->ISI_DMA_P_CTRL;
    regVal &= ~(ISI_DMA_P_CTRL_P_IEN_Msk);
    regVal |= ISI_DMA_P_CTRL_P_IEN(val);

    ISI_REGS->ISI_DMA_P_CTRL = regVal;
}

uint32_t PLIB_ISI_DMA_P_CTRL_P_DONE_Get(void)
{
    uint32_t regVal;

    regVal = (ISI_REGS->ISI_DMA_P_CTRL & ISI_DMA_P_CTRL_P_DONE_Msk) >> ISI_DMA_P_CTRL_P_DONE_Pos;

    return regVal;
}

void PLIB_ISI_DMA_P_CTRL_P_DONE_Set(uint32_t val)
{
    uint32_t regVal;

    regVal = ISI_REGS->ISI_DMA_P_CTRL;
    regVal &= ~(ISI_DMA_P_CTRL_P_DONE_Msk);
    regVal |= ISI_DMA_P_CTRL_P_DONE(val);

    ISI_REGS->ISI_DMA_P_CTRL = regVal;
}

uint32_t PLIB_ISI_DMA_P_DSCR_Get(void)
{
    return ISI_REGS->ISI_DMA_P_DSCR;
}

void PLIB_ISI_DMA_P_DSCR_Set(uint32_t val)
{
    ISI_REGS->ISI_DMA_P_DSCR = (val & 0xFFFFFFFC);
}

uint32_t PLIB_ISI_DMA_C_ADDR_Get(void)
{
    return ISI_REGS->ISI_DMA_C_ADDR;
}

void PLIB_ISI_DMA_C_ADDR_Set(uint32_t val)
{
    ISI_REGS->ISI_DMA_C_ADDR = (val & 0xFFFFFFFC);
}

uint32_t PLIB_ISI_DMA_C_CTRL_C_FETCH_Get(void)
{
    uint32_t regVal;

    regVal = (ISI_REGS->ISI_DMA_C_CTRL & ISI_DMA_C_CTRL_C_FETCH_Msk) >> ISI_DMA_C_CTRL_C_FETCH_Pos;

    return regVal;
}

void PLIB_ISI_DMA_C_CTRL_C_FETCH_Set(uint32_t val)
{
    uint32_t regVal;

    regVal = ISI_REGS->ISI_DMA_C_CTRL;
    regVal &= ~(ISI_DMA_C_CTRL_C_FETCH_Msk);
    regVal |= ISI_DMA_C_CTRL_C_FETCH(val);

    ISI_REGS->ISI_DMA_C_CTRL = regVal;
}

uint32_t PLIB_ISI_DMA_C_CTRL_C_WB_Get(void)
{
    uint32_t regVal;

    regVal = (ISI_REGS->ISI_DMA_C_CTRL & ISI_DMA_C_CTRL_C_WB_Msk) >> ISI_DMA_C_CTRL_C_WB_Pos;

    return regVal;
}

void PLIB_ISI_DMA_C_CTRL_C_WB_Set(uint32_t val)
{
    uint32_t regVal;

    regVal = ISI_REGS->ISI_DMA_C_CTRL;
    regVal &= ~(ISI_DMA_C_CTRL_C_WB_Msk);
    regVal |= ISI_DMA_C_CTRL_C_WB(val);

    ISI_REGS->ISI_DMA_C_CTRL = regVal;
}

uint32_t PLIB_ISI_DMA_C_CTRL_C_IEN_Get(void)
{
    uint32_t regVal;

    regVal = (ISI_REGS->ISI_DMA_C_CTRL & ISI_DMA_C_CTRL_C_IEN_Msk) >> ISI_DMA_C_CTRL_C_IEN_Pos;

    return regVal;
}

void PLIB_ISI_DMA_C_CTRL_C_IEN_Set(uint32_t val)
{
    uint32_t regVal;

    regVal = ISI_REGS->ISI_DMA_C_CTRL;
    regVal &= ~(ISI_DMA_C_CTRL_C_IEN_Msk);
    regVal |= ISI_DMA_C_CTRL_C_IEN(val);

    ISI_REGS->ISI_DMA_C_CTRL = regVal;
}

uint32_t PLIB_ISI_DMA_C_CTRL_C_DONE_Get(void)
{
    uint32_t regVal;

    regVal = (ISI_REGS->ISI_DMA_C_CTRL & ISI_DMA_C_CTRL_C_DONE_Msk) >> ISI_DMA_C_CTRL_C_DONE_Pos;

    return regVal;
}

void PLIB_ISI_DMA_C_CTRL_C_DONE_Set(uint32_t val)
{
    uint32_t regVal;

    regVal = ISI_REGS->ISI_DMA_C_CTRL;
    regVal &= ~(ISI_DMA_C_CTRL_C_DONE_Msk);
    regVal |= ISI_DMA_C_CTRL_C_DONE(val);

    ISI_REGS->ISI_DMA_C_CTRL = regVal;
}

uint32_t PLIB_ISI_DMA_C_DSCR_Get(void)
{
    return ISI_REGS->ISI_DMA_C_DSCR;
}

void PLIB_ISI_DMA_C_DSCR_Set(uint32_t val)
{
    ISI_REGS->ISI_DMA_C_DSCR = (val & 0xFFFFFFFC);
}

uint32_t PLIB_ISI_WPMR_WPEN_Get(void)
{
    uint32_t regVal;

    regVal = (ISI_REGS->ISI_WPMR & ISI_WPMR_WPEN_Msk) >> ISI_WPMR_WPEN_Pos;

    return regVal;
}

void PLIB_ISI_WPMR_WPEN_Set(uint32_t val)
{
    uint32_t regVal;

    regVal = ISI_REGS->ISI_WPMR;
    regVal &= ~(ISI_WPMR_WPEN_Msk);
    regVal |= ISI_WPMR_WPEN(val);

    ISI_REGS->ISI_WPMR = regVal;
}

ISI_WPKEY_Value PLIB_ISI_WPMR_WPKEY_Get(void)
{
    uint32_t regVal;

    regVal = (ISI_REGS->ISI_WPMR & ISI_WPMR_WPKEY_Msk) >> ISI_WPMR_WPKEY_Pos;

    return regVal;
}

void PLIB_ISI_WPMR_WPKEY_Set(ISI_WPKEY_Value val)
{
    uint32_t regVal;

    regVal = ISI_REGS->ISI_WPMR;
    regVal &= ~(ISI_WPMR_WPKEY_Msk);
    regVal |= ISI_WPMR_WPKEY(val);

    ISI_REGS->ISI_WPMR = regVal;
}

uint32_t PLIB_ISI_WPSR_WPVS_Get(void)
{
    uint32_t regVal;

    regVal = (ISI_REGS->ISI_WPSR & ISI_WPSR_WPVS_Msk) >> ISI_WPSR_WPVS_Pos;

    return regVal;
}

uint32_t PLIB_ISI_WPSR_WPVSRC_Get(void)
{
    uint32_t regVal;

    regVal = (ISI_REGS->ISI_WPSR & ISI_WPSR_WPVSRC_Msk) >> ISI_WPSR_WPVSRC_Pos;

    return regVal;
}

