// DOM-IGNORE-BEGIN
/*******************************************************************************
* Copyright (C) 2020 Microchip Technology Inc. and its subsidiaries.
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

/*******************************************************************************
  MPLAB Harmony Generated Driver Implementation File

  File Name:
    drv_gfx_intf_parallel_smc.c

  Summary:
    Implements the parallel display interface driver over SMC

  Description:
    Implements the parallel display interface driver over SMC

    Created with MPLAB Harmony Version 3.0
 *******************************************************************************/

#include <string.h>
#include "definitions.h"

#include "../drv_gfx_disp_intf.h"

// This is the base address of the SMC0 peripheral on V71.
// Update to appropriate base address for other MCUs.

#define EBI_CS_INDEX  ${EBIChipSelectIndex}

<#if EBIChipSelectIndex == 0>
#define EBI_BASE_ADDR  EBI_CS0_ADDR
<#elseif EBIChipSelectIndex == 1>
#define EBI_BASE_ADDR  EBI_CS1_ADDR
<#elseif EBIChipSelectIndex == 2>
#define EBI_BASE_ADDR  EBI_CS2_ADDR
<#elseif EBIChipSelectIndex == 3>
#define EBI_BASE_ADDR  EBI_CS3_ADDR
</#if> 

// Use Address bit ${DCXAddressBit} as DCX
// This lets set set DCX = 1 by writing the data/params to ILI9488_DBIB_DATA_ADDR
#define GFX_DISP_INTF_SMC_DATA_ADDR  (EBI_BASE_ADDR | (1 << ${DCXAddressBit}))
#define GFX_DISP_INTF_SMC_CMD_ADDR EBI_BASE_ADDR

<#if WriteStrobeControl == "GPIO">
#ifdef GFX_DISP_INTF_PIN_WR_Set
#define GFX_DISP_WR_Control(value) GFX_Disp_Intf_PinControl(intf, GFX_DISP_INTF_PIN_WR, value)
#else
#error "GFX_DISP_INTF_PIN_WR pin is not defined. Please define in Pin Manager"
#endif
</#if>

<#if ReadStrobeControl == "GPIO">
#ifdef GFX_DISP_INTF_PIN_RD_Set
#define GFX_DISP_RD_Control(value) GFX_Disp_Intf_PinControl(intf, GFX_DISP_INTF_PIN_RD, value)
#else
#error "GFX_DISP_INTF_PIN_RD pin is not defined. Please define in Pin Manager"
#endif
</#if>

<#if DataCommandSelectControl == "GPIO">
#ifdef GFX_DISP_INTF_PIN_RSDC_Set
#define GFX_DISP_RSDC_Control(value) GFX_Disp_Intf_PinControl(intf, GFX_DISP_INTF_PIN_RSDC, value)
#else
#error "GFX_DISP_INTF_PIN_RSDC DCx pin is not defined. Please define in Pin Manager"
#endif
</#if>

/** GFX_DISP_INTF_SMC

  Summary:
    Structure contains addresses for SMC interface.
    
 */
typedef struct
{   
    /* Address to write commands */
    volatile uint16_t * cmdAddr;
    
    /* Address to write data/parameters */
    volatile uint16_t * dataAddr;
} GFX_DISP_INTF_SMC;

static GFX_DISP_INTF_SMC smcIntf;

/** GFX_Disp_Intf_Sync

  Function:
    static inline void GFX_Disp_Intf_Sync(void)

  Summary:
    Add synchronization for core writes to the SMC

 */
static inline void GFX_Disp_Intf_Sync(void)
{
    __ASM volatile ("dsb");
    __ASM volatile ("dmb");
}

static inline void GFX_Disp_Intf_DelayNOP(void)
{
    <#list 0.. (DelayNOPCount-1) as i>
    asm("nop");
    </#list>
}

GFX_Disp_Intf GFX_Disp_Intf_Open(void)
{   
    smcIntf.dataAddr = (uint16_t *) GFX_DISP_INTF_SMC_DATA_ADDR;
    smcIntf.cmdAddr = (uint16_t *) GFX_DISP_INTF_SMC_CMD_ADDR;
    
    return (GFX_Disp_Intf)&smcIntf;
}

void GFX_Disp_Intf_Close(GFX_Disp_Intf intf)
{
    memset(&smcIntf, 0, sizeof(GFX_DISP_INTF_SMC));
}

int GFX_Disp_Intf_PinControl(GFX_Disp_Intf intf, GFX_DISP_INTF_PIN pin, GFX_DISP_INTF_PIN_VALUE value)
{
    int res = -1;
    
    switch(pin)
    {
        case GFX_DISP_INTF_PIN_CS:
        {
#ifdef GFX_DISP_INTF_PIN_CS_Set
            if (value == GFX_DISP_INTF_PIN_CLEAR)
            {
                GFX_DISP_INTF_PIN_CS_Clear();
            }
            else
            {
                GFX_DISP_INTF_PIN_CS_Set();
            }
            
            res = 0;
#endif
            break;
        }
        case GFX_DISP_INTF_PIN_WR:
        {
#ifdef GFX_DISP_INTF_PIN_WR_Set
            if (value == GFX_DISP_INTF_PIN_CLEAR)
            {
                GFX_DISP_INTF_PIN_WR_Clear();
            }
            else
            {
                GFX_DISP_INTF_PIN_WR_Set();
            }
            
            res = 0;            
#endif
            break;
        }
        case GFX_DISP_INTF_PIN_RD:
        {
#ifdef GFX_DISP_INTF_PIN_RD_Set
            if (value == GFX_DISP_INTF_PIN_CLEAR)
            {
                GFX_DISP_INTF_PIN_RD_Clear();
            }
            else
            {
                GFX_DISP_INTF_PIN_RD_Set();
            }
            
            res = 0;
#endif
            break;
        }
        case GFX_DISP_INTF_PIN_RSDC:
        {
#ifdef GFX_DISP_INTF_PIN_RSDC_Set
            if (value == GFX_DISP_INTF_PIN_CLEAR)
            {
                GFX_DISP_INTF_PIN_RSDC_Clear();
            }
            else
            {
                GFX_DISP_INTF_PIN_RSDC_Set();
            }
            
            res = 0;
#endif            
            break;
        }
        default:
        {
            break;
        }
    }
    
    return res;
}

int GFX_Disp_Intf_WriteCommand(GFX_Disp_Intf intf, uint8_t cmd)
{
<#if DataCommandSelectControl == "GPIO">
    GFX_DISP_RSDC_Control(GFX_DISP_INTF_PIN_CLEAR);
</#if>
<#if WriteStrobeControl == "GPIO">
    GFX_DISP_WR_Control(GFX_DISP_INTF_PIN_SET);
    GFX_Disp_Intf_DelayNOP();
</#if>
        
    GFX_Disp_Intf_Sync();  
    *(smcIntf.cmdAddr) = cmd ;
    GFX_Disp_Intf_Sync();
	
<#if WriteStrobeControl == "GPIO">
    GFX_Disp_Intf_DelayNOP();
    GFX_DISP_WR_Control(GFX_DISP_INTF_PIN_CLEAR);
    GFX_Disp_Intf_DelayNOP();
    GFX_DISP_WR_Control(GFX_DISP_INTF_PIN_SET);
    GFX_Disp_Intf_DelayNOP();
</#if>

    return 0;
}

int GFX_Disp_Intf_WriteData(GFX_Disp_Intf intf, uint8_t* data, int bytes)
{
    unsigned int i;
    
    if (data == NULL || bytes == 0)
        return -1;
    
    GFX_Disp_Intf_Sync();    

<#if DataCommandSelectControl == "GPIO">
    GFX_DISP_RSDC_Control(GFX_DISP_INTF_PIN_SET);
</#if>
<#if WriteStrobeControl == "GPIO">
    GFX_DISP_WR_Control(GFX_DISP_INTF_PIN_SET);
    GFX_Disp_Intf_DelayNOP();
</#if>
    
    for (i = 0; i < bytes; i++)
    {
        *(smcIntf.dataAddr) = *(data);
        GFX_Disp_Intf_Sync();
        data++;
<#if WriteStrobeControl == "GPIO">
        GFX_Disp_Intf_DelayNOP();
        GFX_DISP_WR_Control(GFX_DISP_INTF_PIN_CLEAR);
        GFX_Disp_Intf_DelayNOP();
        GFX_DISP_WR_Control(GFX_DISP_INTF_PIN_SET);
        GFX_Disp_Intf_DelayNOP();
</#if>
    }
    
    return 0;
}

int GFX_Disp_Intf_WriteData16(GFX_Disp_Intf intf, uint16_t* data, int num)
{
    unsigned int i;
    
    if(data == NULL || num == 0)
        return -1;

    GFX_Disp_Intf_Sync();

<#if DataCommandSelectControl == "GPIO">
    GFX_DISP_RSDC_Control(GFX_DISP_INTF_PIN_SET);
</#if>
<#if WriteStrobeControl == "GPIO">
    GFX_DISP_WR_Control(GFX_DISP_INTF_PIN_SET);
    GFX_Disp_Intf_DelayNOP();
</#if>

    for (i = 0; i < num; i++)
    {
        *(smcIntf.dataAddr) = *(data);
        GFX_Disp_Intf_Sync();
        data++;
<#if WriteStrobeControl == "GPIO">
        GFX_Disp_Intf_DelayNOP();
        GFX_DISP_WR_Control(GFX_DISP_INTF_PIN_CLEAR);
        GFX_Disp_Intf_DelayNOP();
        GFX_DISP_WR_Control(GFX_DISP_INTF_PIN_SET);
        GFX_Disp_Intf_DelayNOP();
</#if>
    }
    
    return 0;
}

int GFX_Disp_Intf_ReadData16(GFX_Disp_Intf intf, uint16_t* data, int num)
{
    unsigned int i;
    
    if(num == 0 || data == NULL)
        return -1;
    
    GFX_Disp_Intf_Sync();
<#if DataCommandSelectControl == "GPIO">
    GFX_DISP_RSDC_Control(GFX_DISP_INTF_PIN_SET);
</#if>
<#if ReadStrobeControl == "GPIO">
    GFX_DISP_RD_Control(GFX_DISP_INTF_PIN_SET);
    GFX_Disp_Intf_DelayNOP();
</#if>
    
    for (i = 0; i < num; i++)
    {
<#if ReadStrobeControl == "GPIO">
        GFX_DISP_RD_Control(GFX_DISP_INTF_PIN_CLEAR);
        GFX_Disp_Intf_DelayNOP();
</#if>
        *data = (uint16_t) *(smcIntf.dataAddr);
        data++;
<#if ReadStrobeControl == "GPIO">
        GFX_Disp_Intf_DelayNOP();
        GFX_DISP_RD_Control(GFX_DISP_INTF_PIN_SET);
</#if>
    }
    
    return 0;
}

int GFX_Disp_Intf_ReadData(GFX_Disp_Intf intf, uint8_t* data, int bytes)
{
    unsigned int i;
    
    if(bytes == 0 || data == NULL)
        return -1;

    GFX_Disp_Intf_Sync();
<#if DataCommandSelectControl == "GPIO">
    GFX_DISP_RSDC_Control(GFX_DISP_INTF_PIN_SET);
</#if>
<#if ReadStrobeControl == "GPIO">    
    GFX_DISP_RD_Control(GFX_DISP_INTF_PIN_SET);
    GFX_Disp_Intf_DelayNOP();
</#if>
    
    for (i = 0; i < bytes; i++)
    {
<#if ReadStrobeControl == "GPIO">
        GFX_DISP_RD_Control(GFX_DISP_INTF_PIN_CLEAR);
        GFX_Disp_Intf_DelayNOP();
</#if>
        *data = (uint8_t) *(smcIntf.dataAddr);
        data++;
<#if ReadStrobeControl == "GPIO">
        GFX_Disp_Intf_DelayNOP();
        GFX_DISP_RD_Control(GFX_DISP_INTF_PIN_SET);
</#if>
    }
    
    return 0;
}

DRV_GFX_DEPRECATED int GFX_Disp_Intf_ReadCommandData(GFX_Disp_Intf intf, uint8_t cmd, uint8_t* data, int num_data)
{
    int retval;
    unsigned int i;
    
    if(num_data == 0 || data == NULL)
        return -1;

    retval = GFX_Disp_Intf_WriteCommand(intf, cmd);
    
    if (retval != 0)
        return -1;  

    GFX_Disp_Intf_Sync();
<#if DataCommandSelectControl == "GPIO">
    GFX_DISP_RSDC_Control(GFX_DISP_INTF_PIN_SET);
</#if>
<#if ReadStrobeControl == "GPIO">    
    GFX_DISP_RD_Control(GFX_DISP_INTF_PIN_SET);
    GFX_Disp_Intf_DelayNOP();
</#if>
    
    for (i = 0; i < num_data; i++)
    {
<#if ReadStrobeControl == "GPIO">
        GFX_DISP_RD_Control(GFX_DISP_INTF_PIN_CLEAR);
        GFX_Disp_Intf_DelayNOP();
</#if>
        *data = (uint8_t) *(smcIntf.dataAddr);
        data++;
<#if ReadStrobeControl == "GPIO">
        GFX_Disp_Intf_DelayNOP();
        GFX_DISP_RD_Control(GFX_DISP_INTF_PIN_SET);
</#if>
    }
    
    return retval;
}

DRV_GFX_DEPRECATED int GFX_Disp_Intf_WriteCommandParm(GFX_Disp_Intf intf, uint8_t cmd, uint8_t* parm, int num_parms)
{
    int retval;
    unsigned int i;
    
    retval = GFX_Disp_Intf_WriteCommand(intf, cmd);
    
    if (retval != 0)
        return -1;

<#if DataCommandSelectControl == "GPIO">
    GFX_DISP_RSDC_Control(GFX_DISP_INTF_PIN_SET);
</#if>

    if (num_parms > 0 && parm != NULL)
    {
        for (i = 0; i < num_parms; i++)
        {
            *(smcIntf.dataAddr) = *(parm);

            GFX_Disp_Intf_Sync();

            parm++;
<#if WriteStrobeControl == "GPIO">
            GFX_Disp_Intf_DelayNOP();
            GFX_DISP_WR_Control(GFX_DISP_INTF_PIN_CLEAR);
            GFX_Disp_Intf_DelayNOP();
            GFX_DISP_WR_Control(GFX_DISP_INTF_PIN_SET);
            GFX_Disp_Intf_DelayNOP();
</#if>
        }
    }
    
    return retval;
}

int GFX_Disp_Intf_Write(GFX_Disp_Intf intf, uint8_t* data, int bytes)
{
    unsigned int i;
    
    if (data == NULL || bytes == 0)
        return -1;

    GFX_Disp_Intf_Sync();
<#if WriteStrobeControl == "GPIO">
    GFX_DISP_WR_Control(GFX_DISP_INTF_PIN_SET);
    GFX_Disp_Intf_DelayNOP();
</#if>
    
    for (i = 0; i < bytes; i++)
    {
        *(smcIntf.cmdAddr) = *(data);
        data++;
        GFX_Disp_Intf_Sync();
<#if WriteStrobeControl == "GPIO">
        GFX_Disp_Intf_DelayNOP();
        GFX_DISP_WR_Control(GFX_DISP_INTF_PIN_CLEAR);
        GFX_Disp_Intf_DelayNOP();
        GFX_DISP_WR_Control(GFX_DISP_INTF_PIN_SET);
        GFX_Disp_Intf_DelayNOP();
</#if>
    }
    
    return 0;
}

int GFX_Disp_Intf_WriteDataByte(GFX_Disp_Intf intf, uint8_t data)
{
<#if DataCommandSelectControl == "GPIO">
    GFX_DISP_RSDC_Control(GFX_DISP_INTF_PIN_SET);
</#if>

<#if WriteStrobeControl == "GPIO">
    GFX_DISP_WR_Control(GFX_DISP_INTF_PIN_SET);
    GFX_Disp_Intf_DelayNOP();
</#if>
    
    GFX_Disp_Intf_Sync();
    *(smcIntf.cmdAddr) = data;
    GFX_Disp_Intf_Sync();
<#if WriteStrobeControl == "GPIO">
    GFX_Disp_Intf_DelayNOP();
    GFX_DISP_WR_Control(GFX_DISP_INTF_PIN_CLEAR);
    GFX_Disp_Intf_DelayNOP();
    GFX_DISP_WR_Control(GFX_DISP_INTF_PIN_SET);
    GFX_Disp_Intf_DelayNOP();
</#if>
    
    return 0;
}

int GFX_Disp_Intf_Read(GFX_Disp_Intf intf, uint8_t* data, int bytes)
{
    unsigned int i;
    
    if(bytes == 0 || data == NULL)
        return -1;
    
    GFX_Disp_Intf_Sync();
<#if ReadStrobeControl == "GPIO">
    GFX_DISP_RD_Control(GFX_DISP_INTF_PIN_SET);
    GFX_Disp_Intf_DelayNOP();
</#if>
    
    for (i = 0; i < bytes; i++)
    {
<#if ReadStrobeControl == "GPIO">
        GFX_DISP_RD_Control(GFX_DISP_INTF_PIN_CLEAR);
        GFX_Disp_Intf_DelayNOP();
</#if>
        *(data + i) = (uint8_t) *(smcIntf.cmdAddr);
<#if ReadStrobeControl == "GPIO">
        GFX_Disp_Intf_DelayNOP();
        GFX_DISP_RD_Control(GFX_DISP_INTF_PIN_SET);
</#if>
    }
    
    return 0;
}

int GFX_Disp_Intf_Ready(GFX_Disp_Intf intf)
{
    return 1;
}

int GFX_Disp_Intf_Set_Callback(GFX_Disp_Intf intf, GFX_Disp_Intf_Callback cb, void * parm)
{
    //Not supported
    return -1;
}


/* *****************************************************************************
 End of File
 */
