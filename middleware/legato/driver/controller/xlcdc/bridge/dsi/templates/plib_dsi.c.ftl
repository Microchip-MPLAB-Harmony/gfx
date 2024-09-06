/*******************************************************************************
  DSI Peripheral Library

  Company:
    Microchip Technology Inc.

  File Name:
    plib_dsi.c

  Summary:
    Contains DSI peripheral library function definitions.

  Description:
    This library implements a SUBSET of the register configurations
    for the DSI peripheral.

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
#include "gfx/driver/controller/xlcdc/bridge/dsi/plib_dsi.h"

/* Utility Macros */
/* Command Packet Status Checks */
#define WAIT_CPSR_EQ(_x_)   while((DSI_REGS->DSI_CMD_PKT_STATUS & (_x_) ) == (_x_) ){}
#define WAIT_CPSR_NEQ(_x_)  while((DSI_REGS->DSI_CMD_PKT_STATUS & (_x_) ) != (_x_) ){}
<#if SupportCSI>

/* Utility Macros */
/* PMC */
/* PMC_PCR_GCLKDIV (Bug Fix) */
#undef PMC_PCR_GCLKDIV_Pos
#undef PMC_PCR_GCLKDIV_Msk
#undef PMC_PCR_GCLKDIV
#define PMC_PCR_GCLKDIV_Pos                   _UINT32_(20)
#define PMC_PCR_GCLKDIV_Msk                   (_UINT32_(0xFF) << PMC_PCR_GCLKDIV_Pos)
#define PMC_PCR_GCLKDIV(value)                (PMC_PCR_GCLKDIV_Msk & (_UINT32_(value) << PMC_PCR_GCLKDIV_Pos))

/* Missing DSI definitions */
/* DSI_PCKHDL_CFG_EOTP_TX_LP_EN */
#define DSI_PCKHDL_CFG_EOTP_TX_LP_EN_Pos          _UINT32_(5)
#define DSI_PCKHDL_CFG_EOTP_TX_LP_EN_Msk          (_UINT32_(0x1) << DSI_PCKHDL_CFG_EOTP_TX_LP_EN_Pos)
#define DSI_PCKHDL_CFG_EOTP_TX_LP_EN(value)       (DSI_PCKHDL_CFG_EOTP_TX_LP_EN_Msk & (_UINT32_(value) << DSI_PCKHDL_CFG_EOTP_TX_LP_EN_Pos))
</#if>

/* Configuration Macros */
/* LCD Parameters */
<#if XLCDCConnected>
<#if le_gfx_driver_xlcdc.XClockGCLKOutHint gt 1000>
#define LCD_DCLK_KHZ      ${(le_gfx_driver_xlcdc.XClockGCLKOutHint / 1000)?round?c}
<#else>
#error "Invalid XLCDC GCLK!"
#define LCD_DCLK_KHZ      ${(le_gfx_driver_xlcdc.XClockGCLKOutHint / 1000)?round?c}
</#if>
#define LCD_WIDTH         ${le_gfx_driver_xlcdc.XTResPPL}
#define LCD_HEIGHT        ${le_gfx_driver_xlcdc.XTResRPF}
#define LCD_TIMING_VFP    ${le_gfx_driver_xlcdc.XTVerFP}
#define LCD_TIMING_VBP    ${le_gfx_driver_xlcdc.XTVerBP}
#define LCD_TIMING_VPW    ${le_gfx_driver_xlcdc.XTVerPW}
#define LCD_TIMING_HBP    ${le_gfx_driver_xlcdc.XTHorBP}
#define LCD_TIMING_HPW    ${le_gfx_driver_xlcdc.XTHorPW}
#define LCD_TIMING_HFP    ${le_gfx_driver_xlcdc.XTHorFP}
<#else>
#define LCD_DCLK_KHZ      ${DVTDotClkKiloHz}
#define LCD_WIDTH         ${DVTResPPL}
#define LCD_HEIGHT        ${DVTResRPF}
#define LCD_TIMING_VFP    ${DVTVerFP}
#define LCD_TIMING_VBP    ${DVTVerBP}
#define LCD_TIMING_VPW    ${DVTVerPW}
#define LCD_TIMING_HBP    ${DVTHorBP}
#define LCD_TIMING_HPW    ${DVTHorPW}
#define LCD_TIMING_HFP    ${DVTHorFP}
</#if>

/* D-PHY Test Interface Codes */
#define DPHY_CODE_HSBRC     0x44
#define DPHY_CODE_VCO       0x10
#define DPHY_CODE_ICPCTRL   0x11
#define DPHY_CODE_LPFCTRL   0x12
#define DPHY_CODE_EN_NM     0x19
#define DPHY_CODE_NRATIO    0x17
#define DPHY_CODE_MRATIO    0x18

/* D-PHY Test Interface Data */
#define DPHY_DATA_HSBRC     ${DPhyPllHSBRC}
#define DPHY_DATA_VCORANGE  ${DPhyPllVCORANGE}
#define DPHY_DATA_VCOCAP    0b00
#define DPHY_DATA_ICPCTRL   ${DPhyPllICPCTRL}
#define DPHY_DATA_LPFCTRL   ${DPhyPllLPFCTRL}
#define DPHY_DATA_EN_N      (1 << 4)
#define DPHY_DATA_EN_M      (1 << 5)
#define DPHY_DATA_NRATIO    ${DPhyPllNRatio}
#define DPHY_DATA_MRATIO    ${DPhyPllMRatio}

/* Lane Timings */
#define DPHY_TMR_CLK_LP2HS  ${DPhyTmrClkLpToHs}
#define DPHY_TMR_CLK_HS2LP  ${DPhyTmrClkHsToLp}
#define DPHY_TMR_DAT_LP2HS  ${DPhyTmrDatLpToHs}
#define DPHY_TMR_DAT_HS2LP  ${DPhyTmrDatHsToLp}

/* D-PHY Clocks */
#define DPHY_BIT_CLOCK_KHZ  ${DPhyPllOutClkKiloHz}
#define DPHY_BYTE_CLOCK_KHZ (DPHY_BIT_CLOCK_KHZ >> 3)

/* DSI Clock Management */
#define DSI_CLKMGR_TO_CLK_DIV       10
#define DSI_CLKMGR_TX_ESC_CLK_DIV   (DPHY_BYTE_CLOCK_KHZ / 20000 + 1)

/* DSI Video Mode Lane Byte Clock Cycles */
#define CEIL(_dividend_, _divisor_) (((_dividend_) + (_divisor_) - 1) / (_divisor_))
#define DSI_CYC_TO_LBCYC(_cycles_)  CEIL(((_cycles_) * DPHY_BYTE_CLOCK_KHZ), LCD_DCLK_KHZ)

/* Local Utility Functions */
/* D-PHY Control Operation */
static void DSI_DPHY_GeneralControlOperation(uint8_t test_code, uint8_t test_data) {
    DSI_REGS->DSI_DPHY_TST_CTRL0 = DSI_DPHY_TST_CTRL0_PHY_TESTCLK(1);
    DSI_REGS->DSI_DPHY_TST_CTRL1 = DSI_DPHY_TST_CTRL1_PHY_TESTEN(1) |
                                   DSI_DPHY_TST_CTRL1_PHY_TESTDIN(test_code);
    DSI_REGS->DSI_DPHY_TST_CTRL0 = DSI_DPHY_TST_CTRL0_PHY_TESTCLK(0);
    DSI_REGS->DSI_DPHY_TST_CTRL1 = DSI_DPHY_TST_CTRL1_PHY_TESTDIN(test_data);
    DSI_REGS->DSI_DPHY_TST_CTRL0 = DSI_DPHY_TST_CTRL0_PHY_TESTCLK(1);
    DSI_REGS->DSI_DPHY_TST_CTRL0 = DSI_DPHY_TST_CTRL0_PHY_TESTCLK(0);
}

void DSI_Initialize(void)
{
<#if DSIClocksEn>
    /* Enable clocks */
    /* Enable ${SupportCSI?then('MIPIPHY', 'DSI')} */
    PMC_REGS->PMC_PCR = PMC_PCR_CMD_Msk |
                        PMC_PCR_PID(${SupportCSI?then('ID_MIPIPHY', 'ID_DSI')}) |
                        PMC_PCR_EN_Msk |
                        PMC_PCR_GCLKEN_Msk |
                        PMC_PCR_GCLKDIV(${MIPIPhyGCLKDiv} - 1) |
                        PMC_PCR_GCLKCSS(${MIPIPhyGCLKSource});

<#if SupportCSI>
    /* Enable DSI */
    PMC_REGS->PMC_PCR = PMC_PCR_CMD_Msk |
                        PMC_PCR_PID(ID_DSI) |
                        PMC_PCR_EN_Msk;

</#if>
</#if>
<#if SupportCSI>
    /* Configure and Initialize DSI Mode */
    SFR_REGS->SFR_ISS_CFG = SFR_ISS_CFG_MODE_DSI;

</#if>
    /* Reset DSI */
    DSI_REGS->DSI_PWR_UP = DSI_PWR_UP_SHUTDOWNZ(0);

    /* Reset D-PHY */
    DSI_REGS->DSI_DPHY_RSTZ = DSI_DPHY_RSTZ_DPHY_SHUTDOWNZ(0) |
                              DSI_DPHY_RSTZ_DPHY_RSTZ(0);

    /* Analog Reset */
    DSI_REGS->DSI_DPHY_TST_CTRL0 = DSI_DPHY_TST_CTRL0_PHY_TESTCLR(1);
    DSI_REGS->DSI_DPHY_TST_CTRL0 = DSI_DPHY_TST_CTRL0_PHY_TESTCLR(0);

    /* Configure D-PHY */
    DSI_DPHY_GeneralControlOperation(DPHY_CODE_HSBRC, DPHY_DATA_HSBRC);
    DSI_DPHY_GeneralControlOperation(DPHY_CODE_VCO, (1 << 7) | (DPHY_DATA_VCORANGE << 3) | (DPHY_DATA_VCOCAP << 1));
    DSI_DPHY_GeneralControlOperation(DPHY_CODE_ICPCTRL, DPHY_DATA_ICPCTRL);
    DSI_DPHY_GeneralControlOperation(DPHY_CODE_LPFCTRL, DPHY_DATA_LPFCTRL);
    DSI_DPHY_GeneralControlOperation(DPHY_CODE_EN_NM, DPHY_DATA_EN_M | DPHY_DATA_EN_N);
    DSI_DPHY_GeneralControlOperation(DPHY_CODE_NRATIO, DPHY_DATA_NRATIO - 1);
    DSI_DPHY_GeneralControlOperation(DPHY_CODE_MRATIO, (0 << 7) | (((DPHY_DATA_MRATIO - 1) >> 0) & 0x1F));
    DSI_DPHY_GeneralControlOperation(DPHY_CODE_MRATIO, (1 << 7) | (((DPHY_DATA_MRATIO - 1) >> 5) & 0x1F));

    /* Enable D-PHY */
    DSI_REGS->DSI_DPHY_RSTZ = DSI_DPHY_RSTZ_DPHY_SHUTDOWNZ(1) |
                              DSI_DPHY_RSTZ_DPHY_RSTZ(1) |
                              DSI_DPHY_RSTZ_DPHY_ENABLECLK(1);

    /* Wait D-PHY PLL Lock */
    while((DSI_REGS->DSI_DPHY_STATUS & DSI_DPHY_STATUS_PHY_LOCK_Msk) == 0);

    /* Set number of D-PHY active lanes and stop wait time */
    DSI_REGS->DSI_DPHY_IF_CFG = DSI_DPHY_IF_CFG_N_LANES(${DSIIfActiveLanes} - 1) |
                                DSI_DPHY_IF_CFG_PHY_STOP_WAIT_TIME(0);

    /* Setup Lane Timings */
    DSI_REGS->DSI_DPHY_TMR_LPCLK_CFG = DSI_DPHY_TMR_LPCLK_CFG_PHY_CLKLP2HS_TIME(DPHY_TMR_CLK_LP2HS) |
                                       DSI_DPHY_TMR_LPCLK_CFG_PHY_CLKHS2LP_TIME(DPHY_TMR_CLK_HS2LP);
    DSI_REGS->DSI_DPHY_TMR_CFG = DSI_DPHY_TMR_CFG_PHY_LP2HS_TIME(DPHY_TMR_DAT_LP2HS) |
                                 DSI_DPHY_TMR_CFG_PHY_HS2LP_TIME(DPHY_TMR_DAT_HS2LP);

    /* DSI Clock Management */
    DSI_REGS->DSI_CLKMGR_CFG = DSI_CLKMGR_CFG_TX_ESC_CLK_DIVISION(DSI_CLKMGR_TX_ESC_CLK_DIV) |
                               DSI_CLKMGR_CFG_TO_CLK_DIVISION(DSI_CLKMGR_TO_CLK_DIV);
    DSI_REGS->DSI_LPCLK_CTRL = DSI_LPCLK_CTRL_PHY_TXREQUESTCLKHS(1) |
                               DSI_LPCLK_CTRL_AUTO_CLKLANE_CTRL(0);

    /* DSI Protocol Configuration */
    DSI_REGS->DSI_PCKHDL_CFG = DSI_PCKHDL_CFG_EOTP_TX_EN(0) |
                               DSI_PCKHDL_CFG_EOTP_RX_EN(0) |
                               DSI_PCKHDL_CFG_BTA_EN(1) |
                               DSI_PCKHDL_CFG_ECC_RX_EN(1) |
                               DSI_PCKHDL_CFG_CRC_RX_EN(1) |
                               DSI_PCKHDL_CFG_EOTP_TX_LP_EN(0);

    /* Power-up DSI */
    DSI_REGS->DSI_PWR_UP = DSI_PWR_UP_SHUTDOWNZ(1);
}

void DSI_CommandMode(void)
{
    /* Fill Timeout Counters */
    DSI_REGS->DSI_TO_CNT_CFG = DSI_TO_CNT_CFG_LPRX_TO_CNT(0xFFF) |
                               DSI_TO_CNT_CFG_HSTX_TO_CNT(0xFFF);
    DSI_REGS->DSI_HS_RD_TO_CNT = DSI_HS_RD_TO_CNT_HS_RD_TO_CNT(0xFFF);
    DSI_REGS->DSI_LP_RD_TO_CNT = DSI_LP_RD_TO_CNT_LP_RD_TO_CNT(0xFFF);
    DSI_REGS->DSI_HS_WR_TO_CNT = DSI_HS_WR_TO_CNT_HS_WR_TO_CNT(0xFFF);
    DSI_REGS->DSI_LP_WR_TO_CNT = DSI_LP_WR_TO_CNT_LP_WR_TO_CNT(0xFFF);
    DSI_REGS->DSI_BTA_TO_CNT = DSI_BTA_TO_CNT_BTA_TO_CNT(0xFFF);

    /* Enter DSI Command Mode */
    DSI_REGS->DSI_MODE_CFG = DSI_MODE_CFG_CMD_VIDEO_MODE(1);

    /* Configure DSI Command Mode */
    DSI_REGS->DSI_CMD_MODE_CFG = DSI_CMD_MODE_CFG_TEAR_FX_EN(0) |
                                 DSI_CMD_MODE_CFG_ACK_RQST_EN(0) |
                                 DSI_CMD_MODE_CFG_GEN_SW_0P_TX(1) |
                                 DSI_CMD_MODE_CFG_GEN_SW_1P_TX(1) |
                                 DSI_CMD_MODE_CFG_GEN_SW_2P_TX(1) |
                                 DSI_CMD_MODE_CFG_GEN_SR_0P_TX(1) |
                                 DSI_CMD_MODE_CFG_GEN_SR_1P_TX(1) |
                                 DSI_CMD_MODE_CFG_GEN_SR_2P_TX(1) |
                                 DSI_CMD_MODE_CFG_GEN_LW_TX(1) |
                                 DSI_CMD_MODE_CFG_DCS_SW_0P_TX(1) |
                                 DSI_CMD_MODE_CFG_DCS_SW_1P_TX(1) |
                                 DSI_CMD_MODE_CFG_DCS_SR_0P_TX(1) |
                                 DSI_CMD_MODE_CFG_DCS_LW_TX(1) |
                                 DSI_CMD_MODE_CFG_MAX_RD_PKT_SIZE(1);
}

void DSI_VideoMode(void)
{
    /* Setup DSI Video Mode Parameters */
    DSI_REGS->DSI_VID_PKT_SIZE   = LCD_WIDTH;
    DSI_REGS->DSI_VID_NUM_CHUNKS = 0;
    DSI_REGS->DSI_VID_NULL_SIZE  = 0;
    DSI_REGS->DSI_VID_HSA_TIME   = DSI_CYC_TO_LBCYC(LCD_TIMING_HPW);
    DSI_REGS->DSI_VID_HBP_TIME   = DSI_CYC_TO_LBCYC(LCD_TIMING_HBP);
    DSI_REGS->DSI_VID_HLINE_TIME = DSI_CYC_TO_LBCYC(LCD_WIDTH +
                                                    LCD_TIMING_HPW +
                                                    LCD_TIMING_HBP +
                                                    LCD_TIMING_HFP);
    DSI_REGS->DSI_VID_VSA_LINES  = LCD_TIMING_VPW;
    DSI_REGS->DSI_VID_VBP_LINES  = LCD_TIMING_VBP;
    DSI_REGS->DSI_VID_VFP_LINES  = LCD_TIMING_VFP;
    DSI_REGS->DSI_VID_VACTIVE_LINES = LCD_HEIGHT;

    /* Enter DSI Video Mode */
    DSI_REGS->DSI_MODE_CFG = DSI_MODE_CFG_CMD_VIDEO_MODE(0);

    /* Configure DPI Input */
    DSI_REGS->DSI_DPI_VCID = DSI_DPI_VCID_DPI_VCID(0);
    DSI_REGS->DSI_DPI_COLOR_CODING = DSI_DPI_COLOR_CODING_DPI_COLOR_CODING(${DSIIfDPIMode});
    DSI_REGS->DSI_DPI_LP_CMD_TIM = DSI_DPI_LP_CMD_TIM_INVACT_LPCMD_TIME(0) |
                                   DSI_DPI_LP_CMD_TIM_OUTVACT_LPCMD_TIME(16);

    /* Configure DSI Video Mode */
    DSI_REGS->DSI_VID_MODE_CFG = DSI_VID_MODE_CFG_VID_MODE_TYPE(${VidModeTxType}) |
                                 DSI_VID_MODE_CFG_LP_VSA_EN(1) |
                                 DSI_VID_MODE_CFG_LP_VBP_EN(1) |
                                 DSI_VID_MODE_CFG_LP_VFP_EN(1) |
                                 DSI_VID_MODE_CFG_LP_VACT_EN(1) |
                                 DSI_VID_MODE_CFG_LP_HBP_EN(1) |
                                 DSI_VID_MODE_CFG_LP_HFP_EN(1) |
                                 DSI_VID_MODE_CFG_FRAME_BTA_ACK_EN(0) |
                                 DSI_VID_MODE_CFG_LP_CMD_EN(1) |
                                 DSI_VID_MODE_CFG_VPG_EN(${DVPGEn?then(1,0)}) |
                                 DSI_VID_MODE_CFG_VPG_MODE(${DVPGBerEn?then(1,0)}) |
                                 DSI_VID_MODE_CFG_VPG_ORIENTATION(0);
}

bool DSI_Write(DSI_GENERIC_HEADER * hdr, DSI_GENERIC_PAYLOAD * pld)
{
    /* Wait while command FIFO is full */
    WAIT_CPSR_EQ(DSI_CMD_PKT_STATUS_GEN_CMD_FULL_Msk);

    switch(hdr->longPacket.dataType)
    {
        /* Generic Short Write */
        case 0x03: // No parameter
        case 0x13: // 1 parameter
        case 0x23: // 2 parameters
        /* DCS Short Write */
        case 0x05: // No parameter
        case 0x15: // 1 parameter
        {
            DSI_REGS->DSI_GEN_HDR = hdr->headerU32;

            /* Wait for command to complete */
            WAIT_CPSR_NEQ(DSI_CMD_PKT_STATUS_GEN_CMD_EMPTY_Msk);

            return false;
        }
        /* Generic Long Write */
        case 0x29:
        /* DCS Long Write */
        case 0x39:
        {
            uint32_t pld_size  = 0;

            /* Check size against max FIFO depth (~4128 bytes) */
            if (hdr->longPacket.size > 4096 || hdr->longPacket.size == 0)
            {
                return true;
            }
            else if (hdr->longPacket.size < 4)
            {
                pld_size = 1;
            }
            else if (hdr->longPacket.size % 4)
            {
                pld_size = hdr->longPacket.size + 4 - (hdr->longPacket.size % 4);
                pld_size /= 4;
            }
            else
            {
                pld_size /= 4;
            }

            for (int idx = 0; idx < pld_size; idx++)
            {
                DSI_REGS->DSI_GEN_PLD_DATA = pld[idx].dataU32;
            }

            DSI_REGS->DSI_GEN_HDR = hdr->headerU32;

            /* Wait for command to complete */
            WAIT_CPSR_NEQ(DSI_CMD_PKT_STATUS_GEN_CMD_EMPTY_Msk | DSI_CMD_PKT_STATUS_GEN_PLD_W_EMPTY_Msk);

            return false;
        }
        default:
        {
            return true;
        }
    }
}
