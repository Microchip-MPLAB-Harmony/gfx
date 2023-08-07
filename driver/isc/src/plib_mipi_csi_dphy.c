/*******************************************************************************
  MIPI CSI DPHY Peripheral Library Interface Source File

  Company:
    Microchip Technology Inc.

  File Name:
    plib_mipi_csi_dphy.c

  Summary:
    MIPI CSI DPHY peripheral library source file.

  Description:
    The MIPI CSI DPHY PLIB provides a simple interface to manage the Physical interface.

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
#include "plib_mipi_csi_dphy.h"
#include "system/time/sys_time.h"


static inline void _delayus(int us)
{
    SYS_TIME_HANDLE timer = SYS_TIME_HANDLE_INVALID;

    if (SYS_TIME_DelayUS(us, &timer) != SYS_TIME_SUCCESS)
        return;
    
    while (SYS_TIME_DelayIsComplete(timer) == false);
}
static void MIPI_DPHY_PowerUp ( void )
{
    //CSI_REGS->MIPICSI2PHY_CSI_PHY_TEST_CTRL0 = MIPICSI2PHY_CSI_PHY_TEST_CTRL0_PHY_TESTCLK_Msk;
    CSI_REGS->MIPICSI2PHY_CSI_PHY_TEST_CTRL0 = 0x2;
    CSI_REGS->MIPICSI2PHY_CSI_PHY_SHUTDOWNZ = MIPICSI2PHY_CSI_PHY_SHUTDOWNZ_PHY_SHUTDOWNZ_Msk;
    _delayus(500);
    CSI_REGS->MIPICSI2PHY_CSI_DPHY_RSTZ = MIPICSI2PHY_CSI_DPHY_RSTZ_DPHY_RSTZ_Msk;
}

static void MIPI_DPHY_Configure_bitrate ( void )
{
    //CSI_REGS->MIPICSI2PHY_CSI_PHY_TEST_CTRL0 = MIPICSI2PHY_CSI_PHY_TEST_CTRL0_PHY_TESTCLK_Msk;
    CSI_REGS->MIPICSI2PHY_CSI_PHY_TEST_CTRL0 = 0x2;
    CSI_REGS->MIPICSI2PHY_CSI_PHY_TEST_CTRL1 = 0x44; 
    CSI_REGS->MIPICSI2PHY_CSI_PHY_TEST_CTRL1 = 0x10044; 
    CSI_REGS->MIPICSI2PHY_CSI_PHY_TEST_CTRL0 = 0;
    CSI_REGS->MIPICSI2PHY_CSI_PHY_TEST_CTRL1 = 0x44; 
    CSI_REGS->MIPICSI2PHY_CSI_PHY_TEST_CTRL1 = 0x72;  //data 
    //CSI_REGS->MIPICSI2PHY_CSI_PHY_TEST_CTRL0 = MIPICSI2PHY_CSI_PHY_TEST_CTRL0_PHY_TESTCLK_Msk;
    CSI_REGS->MIPICSI2PHY_CSI_PHY_TEST_CTRL0 = 0x2;
    CSI_REGS->MIPICSI2PHY_CSI_PHY_TEST_CTRL0 = 0;
     
}

static void MIPI_DPHY_Analog_Init ( void )
{
    // provide initial active high test clear pulse in TESTCLR
    CSI_REGS->MIPICSI2PHY_CSI_PHY_TEST_CTRL0 = MIPICSI2PHY_CSI_PHY_TEST_CTRL0_PHY_TESTCLR_Msk;
    CSI_REGS->MIPICSI2PHY_CSI_PHY_TEST_CTRL0 = 0;
    /* Configure D_PHY bit rate */
    MIPI_DPHY_Configure_bitrate();
}

static void MIPI_DPHY_PowerDown( void )
{
    CSI_REGS->MIPICSI2PHY_CSI_DPHY_RSTZ = 0;
    CSI_REGS->MIPICSI2PHY_CSI_PHY_TEST_CTRL0 = MIPICSI2PHY_CSI_PHY_TEST_CTRL0_PHY_TESTCLR_Msk;
    CSI_REGS->MIPICSI2PHY_CSI_PHY_SHUTDOWNZ = 0;
}
// *****************************************************************************
// *****************************************************************************
// MIPI CSI DPHY PLib Interface Routine
// *****************************************************************************
// *****************************************************************************
// *****************************************************************************

/*******************************************************************************
  Function:
    MIPI_DPHY_Init ( void )

  Summary:
    Initialize the Physical interface to connect CSI-2 camera sensors.

  Remarks:
    See plib_mipi_csi_dphy.h for more details.
*/
void MIPI_DPHY_Configure ( void )
{
    uint8_t wait=0;
    MIPI_DPHY_PowerDown();
    //CSI_REGS->MIPICSI2PHY_CSI_PHY_TEST_CTRL0 = MIPICSI2PHY_CSI_PHY_TEST_CTRL0_PHY_TESTCLR_Msk;
    //CSI_REGS->MIPICSI2PHY_CSI_PHY_SHUTDOWNZ = 0;
    MIPI_DPHY_Analog_Init();
    MIPI_DPHY_PowerUp();
    /*Wait for DPHY Stop State to become high*/
    while(CSI_REGS->MIPICSI2PHY_CSI_PHY_STOPSTATE != 0x10003)
    {
        _delayus(500);
        wait++;
        if(wait>10)
            break;
        
    }
}

void MIPI_DPHY_Init ( void )
{
    /* Power down DPHY */
    CSI_REGS->MIPICSI2PHY_CSI_DPHY_RSTZ = 0;
    CSI_REGS->MIPICSI2PHY_CSI_PHY_SHUTDOWNZ = 0;
    CSI_REGS->MIPICSI2PHY_CSI_CSI2_RESETN = 0;
}