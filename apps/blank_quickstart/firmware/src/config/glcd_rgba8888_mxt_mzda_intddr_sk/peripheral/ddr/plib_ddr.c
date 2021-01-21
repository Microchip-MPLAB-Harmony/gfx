/*******************************************************************************
  Memory System Service Functions for DDR Initialization

  Company:
    Microchip Technology Inc.

  File Name:
    plib_ddr.c

  Summary:
    Memory System Service implementation for the DDR controller.

  Description:
    The Memory System Service initializes the DDR Controller and PHY to 
    provide access to external DDR2 SDRAM.
    
  Remarks:
    Static interfaces incorporate the driver instance number within the names
    of the routines, eliminating the need for an object ID or object handle.
    Static single-open interfaces also eliminate the need for the open handle.
*******************************************************************************/

//DOM-IGNORE-BEGIN
/*******************************************************************************
Copyright (c) 2013 released Microchip Technology Inc.  All rights reserved.

Microchip licenses to you the right to use, modify, copy and distribute
Software only when embedded on a Microchip microcontroller or digital signal
controller that is integrated into your product or third party product
(pursuant to the sublicense terms in the accompanying license agreement).

You should refer to the license agreement accompanying this Software for
additional information regarding your rights and obligations.

SOFTWARE AND DOCUMENTATION ARE PROVIDED AS IS WITHOUT WARRANTY OF ANY KIND,
EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION, ANY WARRANTY OF
MERCHANTABILITY, TITLE, NON-INFRINGEMENT AND FITNESS FOR A PARTICULAR PURPOSE.
IN NO EVENT SHALL MICROCHIP OR ITS LICENSORS BE LIABLE OR OBLIGATED UNDER
CONTRACT, NEGLIGENCE, STRICT LIABILITY, CONTRIBUTION, BREACH OF WARRANTY, OR
OTHER LEGAL EQUITABLE THEORY ANY DIRECT OR INDIRECT DAMAGES OR EXPENSES
INCLUDING BUT NOT LIMITED TO ANY INCIDENTAL, SPECIAL, INDIRECT, PUNITIVE OR
CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST DATA, COST OF PROCUREMENT OF
SUBSTITUTE GOODS, TECHNOLOGY, SERVICES, OR ANY CLAIMS BY THIRD PARTIES
(INCLUDING BUT NOT LIMITED TO ANY DEFENSE THEREOF), OR OTHER SIMILAR COSTS.
*******************************************************************************/
//DOM-IGNORE-END

// *****************************************************************************
// *****************************************************************************
// Header Includes
// *****************************************************************************
// *****************************************************************************
#include "peripheral/ddr/plib_ddr.h"

// *****************************************************************************
// *****************************************************************************
// Section: Memory System Service DDR static functions
// *****************************************************************************
// *****************************************************************************

#define sys_mem_ddr_max(a,b) (((a)>(b))?(a):(b))
#define sys_mem_ddr_round_up(x,y) (((x) + (y) - 1) / (y))
#define sys_mem_ddr_hc_clk_dly(dly) (sys_mem_ddr_max((sys_mem_ddr_round_up((dly),2500)),2)-2)

#ifndef round_up
#define round_up(x,y) (((x) + (y) - 1) / (y))
#endif

static void DDR_Init(void)
{
    uint32_t tmp;
    uint32_t ba_field, ma_field;

    /* Target Arbitration */
	DDRTSELbits.TSEL = (DDR_TARGET_0 * 5);
	DDRMINLIMbits.MINLIMIT = 0x1f;
	DDRTSELbits.TSEL = DDR_TARGET_0 * 8;
	DDRRQPERbits.RQPER = 0xff;
	DDRTSELbits.TSEL = DDR_TARGET_0 * 8;
	DDRMINCMDbits.MINCMD = 0x4;

	DDRTSELbits.TSEL = (DDR_TARGET_1 * 5);
	DDRMINLIMbits.MINLIMIT = 0x1f;
	DDRTSELbits.TSEL = DDR_TARGET_1 * 8;
	DDRRQPERbits.RQPER = 0xff;
	DDRTSELbits.TSEL = DDR_TARGET_1 * 8;
	DDRMINCMDbits.MINCMD = 0x10;

	DDRTSELbits.TSEL = (DDR_TARGET_2* 5);
	DDRMINLIMbits.MINLIMIT = 0x1f;
	DDRTSELbits.TSEL = DDR_TARGET_2 * 8;
	DDRRQPERbits.RQPER = 0xff;
	DDRTSELbits.TSEL = DDR_TARGET_2 * 8;
	DDRMINCMDbits.MINCMD = 0x10;
	
	DDRTSELbits.TSEL = (DDR_TARGET_3 * 5);
	DDRMINLIMbits.MINLIMIT = 0x4;
	DDRTSELbits.TSEL = DDR_TARGET_3 * 8;
	DDRRQPERbits.RQPER = 0xff;
	DDRTSELbits.TSEL = DDR_TARGET_3 * 8;
	DDRMINCMDbits.MINCMD = 0x4;

	DDRTSELbits.TSEL = (DDR_TARGET_4 * 5);
	DDRMINLIMbits.MINLIMIT = 0x4;
	DDRTSELbits.TSEL = DDR_TARGET_4 * 8;
	DDRRQPERbits.RQPER = 0xff;
	DDRTSELbits.TSEL = DDR_TARGET_4 * 8;
	DDRMINCMDbits.MINCMD = 0x4;

    /* Addressing */
	DDRMEMCFG0bits.RWADDR = ROW_ADDR_RSHIFT;
	DDRMEMCFG1bits.RWADDRMSK = ROW_ADDR_MASK;
    DDRMEMCFG0bits.CLHADDR = COL_HI_RSHFT;
	DDRMEMCFG3bits.CLADDRLMSK = COL_LO_MASK;
	DDRMEMCFG2bits.CLADDRHMSK = COL_HI_MASK;
	DDRMEMCFG0bits.BNKADDR = BA_RSHFT;
	DDRMEMCFG4bits.BNKADDRMSK = BANK_ADDR_MASK;
	DDRMEMCFG0bits.CSADDR = CS_ADDR_RSHIFT;
	DDRMEMCFG4bits.CSADDRMSK = CS_ADDR_MASK;

    /* Refresh */
	DDRREFCFGbits.REFCNT = (7800000 + CTRL_CLK_PERIOD - 1) / CTRL_CLK_PERIOD - 2;
	DDRREFCFGbits.REFDLY = (127500 + CTRL_CLK_PERIOD - 1) / CTRL_CLK_PERIOD - 2;
	DDRREFCFGbits.MAXREFS = 7;
	DDRPWRCFGbits.ASLFREFEN = 0;

    /* Power */
	DDRPWRCFGbits.APWRDNEN = 0;
	DDRMEMCFG0bits.APCHRGEN = 0;
	DDRPWRCFGbits.PCHRGPWRDN = 0;

    /* Timing */
	DDRDLYCFG0bits.R2WDLY = 2 + 2u;
	DDRDLYCFG0bits.RMWDLY = 5 - 4 + 3u;

    uint32_t w2rdly, w2rcsdly;

    w2rdly = round_up(7500, CTRL_CLK_PERIOD) + 4 + 2;
    w2rcsdly = ((w2rdly - 1u) > 3u) ? (w2rdly - 1u) : 3u;

	DDRDLYCFG0bits.W2RDLY = w2rdly & 0x0Fu;
	DDRDLYCFG1bits.W2RDLY4 = ((w2rdly & 0x10u) != 0) ? 1 : 0;
	DDRDLYCFG0bits.W2RCSDLY = w2rcsdly & 0x0Fu;
	DDRDLYCFG1bits.W2RCSDLY4 = ((w2rcsdly & 0x10u) != 0) ? 1 : 0;
	DDRDLYCFG0bits.R2RDLY = 2 - 1u;
	DDRDLYCFG0bits.R2RCSDLY = 2;
	DDRDLYCFG0bits.W2WDLY = 2 - 1u;
	DDRDLYCFG0bits.W2WCSDLY = 2 - 1u;
	DDRPWRCFGbits.SLFREFDLY = 17;
	DDRDLYCFG1bits.SLFREFMINDLY = 3 - 1u;
	DDRDLYCFG1bits.SLFREFEXDLY = (round_up(200, 2u) - 2u) & 0xFFu;
	DDRDLYCFG1bits.SLFREFEXDLY8 = ((round_up(200, 2u) & 0x100u) != 0) ? 1u : 0;
	DDRPWRCFGbits.PWRDNDLY = 8;
	DDRDLYCFG1bits.PWRDNMINDLY = 3 - 1;	
	DDRDLYCFG1bits.PWRDNEXDLY = (3 > 2 ? 3 : 2) - 1; 
	DDRDLYCFG2bits.PCHRGALLDLY = round_up(12500, CTRL_CLK_PERIOD); 
	DDRDLYCFG2bits.R2PCHRGDLY = round_up(7500, CTRL_CLK_PERIOD) + 2 - 2; 
	DDRDLYCFG2bits.W2PCHRGDLY = (round_up(15000, CTRL_CLK_PERIOD) + 4 + 2) & 0x0F;
	DDRDLYCFG1bits.W2PCHRGDLY4 = (((round_up(15000, CTRL_CLK_PERIOD) + 4 + 2) & 0x10u) != 0) ? 1 : 0;
	DDRDLYCFG2bits.PCHRG2RASDLY = round_up(12500, CTRL_CLK_PERIOD) - 1; 
	DDRDLYCFG3bits.RAS2PCHRGDLY = round_up(45000, CTRL_CLK_PERIOD) - 1;
	DDRDLYCFG3bits.RAS2RASSBNKDLY = round_up(57500, CTRL_CLK_PERIOD) - 1;
	DDRDLYCFG2bits.RAS2RASDLY = round_up(7500, CTRL_CLK_PERIOD) - 1;
	DDRDLYCFG2bits.RAS2CASDLY = round_up(12500, CTRL_CLK_PERIOD) - 1;
	DDRDLYCFG2bits.RBENDDLY = 5 + 3u;
	DDRXFERCFGbits.NXTDATRQDLY = 2;
	DDRXFERCFGbits.NXTDATAVDLY = 4;
	DDRDLYCFG1bits.NXTDATAVDLY4 = (((5 + 5u) & 0x10u) != 0) ? 1 : 0;
	DDRXFERCFGbits.RDATENDLY = 2;/*5 - 1*/
	DDRDLYCFG3bits.FAWTDLY = round_up(35000, CTRL_CLK_PERIOD) - 1;

    /* On-Die Termination */
	DDRODTCFGbits.ODTCSEN = 0;
	DDRODTENCFGbits.ODTREN = 0;
	DDRODTCFGbits.ODTCSEN = 0;
	DDRODTENCFGbits.ODTWEN = 1;
	DDRODTCFGbits.ODTWLEN = 3;
	DDRODTCFGbits.ODTWDLY = 1;

    /* Controller Settings */
	DDRXFERCFGbits.BIGENDIAN = 0;
	DDRMEMWIDTHbits.HALFRATE = 1;
	DDRXFERCFGbits.MAXBURST = 3;
	DDRCMDISSUEbits.NUMHOSTCMDS = 12;

    /* DRAM Initialization */

    /* bring CKE high after reset and wait 400 nsec */
	*(&DDRCMD10 + DDR_HOST_CMD_REG_10) = DRV_DDR_IDLE_NOP;
	*(&DDRCMD10 + DDR_HOST_CMD_REG_20) = (0x00 | (0x00 << 8) | (sys_mem_ddr_hc_clk_dly(400000) << 11));

    /* issue precharge all command */
	*(&DDRCMD10 + DDR_HOST_CMD_REG_11) = DRV_DDR_PRECH_ALL_CMD;
	*(&DDRCMD10 + DDR_HOST_CMD_REG_21) = (0x04 | (0x00 << 8) | (sys_mem_ddr_hc_clk_dly(12500 + 2500) << 11));

    /* initialize EMR2 */
	*(&DDRCMD10 + DDR_HOST_CMD_REG_12) = DRV_DDR_LOAD_MODE_CMD;
	*(&DDRCMD10 + DDR_HOST_CMD_REG_22) = (0x00 | (0x02 << 8) | (sys_mem_ddr_hc_clk_dly(2 * 2500) << 11));

    /* initialize EMR3 */
	*(&DDRCMD10 + DDR_HOST_CMD_REG_13) = DRV_DDR_LOAD_MODE_CMD;
	*(&DDRCMD10 + DDR_HOST_CMD_REG_23) = (0x00 | (0x03 << 8) | (sys_mem_ddr_hc_clk_dly(2 * 2500) << 11));

    /* RDQS disable, DQSB enable, OCD exit, 150 ohm termination, AL=0, DLL enable */
	*(&DDRCMD10 + DDR_HOST_CMD_REG_14) = (DRV_DDR_LOAD_MODE_CMD | (0x40 << 24));
	*(&DDRCMD10 + DDR_HOST_CMD_REG_24) = (0x00 | (0x01 << 8) | (sys_mem_ddr_hc_clk_dly(2 * 2500) << 11));

    tmp = ((sys_mem_ddr_round_up(15000, 2500) -1 ) << 1) | 1;
    ma_field = tmp & 0xFF;
    ba_field = (tmp >> 8) & 0x03;

    /* PD fast exit, WR REC = tWR in clocks -1, DLL reset, CAS = RL, burst = 4 */
	*(&DDRCMD10 + DDR_HOST_CMD_REG_15) = (DRV_DDR_LOAD_MODE_CMD | (((5 << 4) | 2) << 24));
	*(&DDRCMD10 + DDR_HOST_CMD_REG_25) = (ma_field | (ba_field << 8) | (sys_mem_ddr_hc_clk_dly(2 * 2500) << 11));

    /* issue precharge all command */
	*(&DDRCMD10 + DDR_HOST_CMD_REG_16) = DRV_DDR_PRECH_ALL_CMD;
	*(&DDRCMD10 + DDR_HOST_CMD_REG_26) = (0x04 | (0x00 << 8) | (sys_mem_ddr_hc_clk_dly(12500 + 2500) << 11));

    /* issue refresh command */
	*(&DDRCMD10 + DDR_HOST_CMD_REG_17) = DRV_DDR_REF_CMD;
	*(&DDRCMD10 + DDR_HOST_CMD_REG_27) = (0x00 | (0x00 << 8) | (sys_mem_ddr_hc_clk_dly(127500) << 11));

    /* issue refresh command */
	*(&DDRCMD10 + DDR_HOST_CMD_REG_18) = DRV_DDR_REF_CMD;
	*(&DDRCMD10 + DDR_HOST_CMD_REG_28) = (0x00 | (0x00 << 8) | (sys_mem_ddr_hc_clk_dly(127500) << 11));
	
    tmp = ((sys_mem_ddr_round_up(15000, 2500) -1 ) << 1);
    ma_field = tmp & 0xFF;
    ba_field = (tmp >> 8) & 0x03;

    /* Mode register programming as before without DLL reset */
	*(&DDRCMD10 + DDR_HOST_CMD_REG_19) = (DRV_DDR_LOAD_MODE_CMD | (((5 << 4) | 3) << 24));
	*(&DDRCMD10 + DDR_HOST_CMD_REG_29) = (ma_field | (ba_field << 8) | (sys_mem_ddr_hc_clk_dly(2 * 2500) << 11));
	
    /* extended mode register same as before with OCD default */
	*(&DDRCMD10 + DDR_HOST_CMD_REG_110) = (DRV_DDR_LOAD_MODE_CMD | (0xC0 << 24));
	*(&DDRCMD10 + DDR_HOST_CMD_REG_210) = (0x03 | (0x01 << 8) | (sys_mem_ddr_hc_clk_dly(2 * 2500) << 11));
	
    /* extended mode register same as before with OCD exit */
	*(&DDRCMD10 + DDR_HOST_CMD_REG_111) = (DRV_DDR_LOAD_MODE_CMD | (0x40 << 24));
	*(&DDRCMD10 + DDR_HOST_CMD_REG_211) = (0x00 | (0x01 << 8) | (sys_mem_ddr_hc_clk_dly(140 * 2500) << 11));

	/* Set number of host commands */
	DDRCMDISSUEbits.NUMHOSTCMDS = 0xb;
	DDRCMDISSUEbits.VALID = 1;
	DDRMEMCONbits.STINIT = 1;
	while (DDRCMDISSUEbits.VALID);
	DDRMEMCONbits.INITDN = 1;
}

static void DDR_PHY_Init(void)
{
	DDRPHYPADCONbits.ODTSEL = 1;
	DDRPHYPADCONbits.ODTEN = 1;

	DDRPHYPADCONbits.DATDRVSEL = 0;
	DDRPHYPADCONbits.ADDCDRVSEL = 0;
	DDRPHYPADCONbits.ODTPUCAL = 3;
	DDRPHYPADCONbits.ODTPDCAL = 2;
	DDRPHYPADCONbits.DRVSTRNFET = 14;
	DDRPHYPADCONbits.DRVSTRPFET = 14;
	
	DDRPHYPADCONbits.NOEXTDLL = 1;
	
	DDRPHYPADCONbits.EOENCLKCYC = 0;

	DDRPHYPADCONbits.RCVREN = 1;
	DDRPHYPADCONbits.PREAMBDLY = 2;
	DDRPHYPADCONbits.HALFRATE = 1;
	DDRPHYPADCONbits.WRCMDDLY = 1;
	DDRPHYDLLRbits.RECALIBCNT = 16;
	DDRPHYDLLRbits.DISRECALIB = 0;
	DDRPHYDLLRbits.DLYSTVAL = 3;
	DDRSCLCFG0bits.BURST8 = DDR_PHY_SCL_BURST_MODE_8;
	DDRSCLCFG0bits.DDR2 = !DDR_PHY_DDR_TYPE_DDR2;
	DDRSCLCFG0bits.RCASLAT = 5;
	DDRSCLCFG1bits.WCASLAT = 4;

	DDRSCLCFG0bits.ODTCSW = 1;
	DDRSCLCFG1bits.DBLREFDLY = 0;
	DDRSCLLATbits.DDRCLKDLY = 4;
	DDRSCLLATbits.CAPCLKDLY = 3;

	/* Chip select 0 enabled by hardware. Other chip selects not available. */
	/* DDRSCLCFG1bits.SCLCSEN = 0;*/
}

static void DDR_PHY_Calib(void)
{
	DDRSCLSTARTbits.SCLEN = 1;
	DDRSCLSTARTbits.SCLSTART = 1;

	while (!((DDRSCLSTARTbits.SCLLBPASS & DDRSCLSTARTbits.SCLUBPASS) == 0x01));
}

void DDR_Initialize(void)
{
    DDR_PHY_Init();
    DDR_Init();
	DDR_PHY_Calib();
}

/*******************************************************************************
 End of File
*/
