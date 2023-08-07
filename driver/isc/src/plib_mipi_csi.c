/*******************************************************************************
  MIPI CSI Peripheral Library Interface Source File

  Company:
    Microchip Technology Inc.

  File Name:
    plib_mipi_csi.c

  Summary:
    MIPI CSI peripheral library source file.

  Description:
    The MIPI CSI PLIB provides a simple interface to manage the Camera Serial Interface.

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
#include "plib_mipi_csi.h"
#include "plib_csi2dc.h"
#include "system/time/sys_time.h"
// *****************************************************************************
// *****************************************************************************
// Global Data
// *****************************************************************************
// *****************************************************************************

#if 0
void CSI_Handler(void)
{
    uint32_t global_csi_int = 0;
    uint32_t int_stat=0;
    global_csi_int = CSI_REGS->MIPICSI2PHY_CSI_INT_ST_MAIN;
    if(global_csi_int & MIPICSI2PHY_CSI_INT_ST_MAIN_STATUS_INT_PHY_FATAL_Msk)
        int_stat = CSI_REGS->MIPICSI2PHY_CSI_INT_ST_PHY_FATAL;
    if(global_csi_int & MIPICSI2PHY_CSI_INT_ST_MAIN_STATUS_INT_PKT_FATAL_Msk)
        int_stat = CSI_REGS->MIPICSI2PHY_CSI_INT_ST_PKT_FATAL;
    if(global_csi_int & MIPICSI2PHY_CSI_INT_ST_MAIN_STATUS_INT_FRAME_FATAL_Msk)
        int_stat = CSI_REGS->MIPICSI2PHY_CSI_INT_ST_FRAME_FATAL;
    if(global_csi_int & MIPICSI2PHY_CSI_INT_ST_MAIN_STATUS_INT_PHY_Msk)
        int_stat = CSI_REGS->MIPICSI2PHY_CSI_INT_ST_PHY;
    if(global_csi_int & MIPICSI2PHY_CSI_INT_ST_MAIN_STATUS_INT_PKT_Msk)
        int_stat = CSI_REGS->MIPICSI2PHY_CSI_INT_ST_PKT;
    if(global_csi_int & MIPICSI2PHY_CSI_INT_ST_MAIN_STATUS_INT_LINE_Msk)
        int_stat = CSI_REGS->MIPICSI2PHY_CSI_INT_ST_LINE;
   
}
#endif

static inline void _delayus(int us)
{
    SYS_TIME_HANDLE timer = SYS_TIME_HANDLE_INVALID;

    if (SYS_TIME_DelayUS(us, &timer) != SYS_TIME_SUCCESS)
        return;
    
    while (SYS_TIME_DelayIsComplete(timer) == false);
}

static void MIPI_CSI_Reset( void )
{
    CSI_REGS->MIPICSI2PHY_CSI_CSI2_RESETN = 0;
    _delayus(200);
    CSI_REGS->MIPICSI2PHY_CSI_CSI2_RESETN = 1;
    CSI_REGS->MIPICSI2PHY_CSI_N_LANES = 0;
}

static void MIPI_CSI_Set_Timings ( void )
{
    CSI_REGS->MIPICSI2PHY_CSI_IPI_HSA_TIME = 0;
    CSI_REGS->MIPICSI2PHY_CSI_IPI_HBP_TIME = 0;
    CSI_REGS->MIPICSI2PHY_CSI_IPI_HSD_TIME = 0;
    CSI_REGS->MIPICSI2PHY_CSI_IPI_HLINE_TIME = 640;
    CSI_REGS->MIPICSI2PHY_CSI_IPI_VSA_LINES = 0;       
    CSI_REGS->MIPICSI2PHY_CSI_IPI_VBP_LINES = 0;
    CSI_REGS->MIPICSI2PHY_CSI_IPI_VFP_LINES = 0;
    CSI_REGS->MIPICSI2PHY_CSI_IPI_VACTIVE_LINES = 480;
            
}

static void MIPI_CSI_Start ( void )
{
    CSI_REGS->MIPICSI2PHY_CSI_N_LANES = 1;
    CSI_REGS->MIPICSI2PHY_CSI_IPI_MODE = 0;
    CSI_REGS->MIPICSI2PHY_CSI_IPI_VCID = 0;
    CSI_REGS->MIPICSI2PHY_CSI_IPI_MEM_FLUSH = 0x100;
    /*Setting MIPI CSI Timings*/
    MIPI_CSI_Set_Timings();
    
    
}
// *****************************************************************************
// *****************************************************************************
// MIPI CSI PLib Interface Routine
// *****************************************************************************
// *****************************************************************************
// *****************************************************************************

/*******************************************************************************
  Function:
    MIPI_CSI_Init ( void )

  Summary:
    Initialize the Camera Serial Interface.

  Remarks:
    See plib_mipi_csi.h for more details.
*/
void MIPI_CSI_Init ( void )
{
    /* Program the data type */
    CSI_REGS->MIPICSI2PHY_CSI_DATA_IDS_1 = RAW10;
    CSI_REGS->MIPICSI2PHY_CSI_IPI_DATA_TYPE = RAW10;
    MIPI_CSI_Reset();
#if 0
    /*Enable all interrupts sources for FRAME_FATAL*/
    CSI_REGS->MIPICSI2PHY_CSI_INT_MSK_FRAME_FATAL = 0xFFFFFFFF;
    /*Enable all interrupts sources for PHY_FATAL*/
    CSI_REGS->MIPICSI2PHY_CSI_INT_MSK_PHY_FATAL = 0x1FF   ; //0x1FF           These are available in the datasheet
    /*Enable all interrupts sources for PKT_FATAL*/
    CSI_REGS->MIPICSI2PHY_CSI_INT_MSK_PKT_FATAL = 0x3; //3
    /*Enable all interrupts sources for PHY*/
    CSI_REGS->MIPICSI2PHY_CSI_INT_MSK_PHY = 0xFFFFFF; //0xFFFFFF
    /*Enable all interrupts sources for LINE*/
    CSI_REGS->MIPICSI2PHY_CSI_INT_MSK_LINE = 0xFFFFFF; //0xFFFFFF
    /*Enable all interrupts sources for IPI*/
    CSI_REGS->MIPICSI2PHY_CSI_INT_MSK_IPI = 0x3F; //0x3F
#endif   
    MIPI_CSI_Start();
}

