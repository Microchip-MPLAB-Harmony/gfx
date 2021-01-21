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
    drv_gfx_intf_spi4.c

  Summary:
    Implements the parallel display interface driver over SMC

  Description:
    Implements the parallel display interface driver over SMC

    Created with MPLAB Harmony Version 3.0
 *******************************************************************************/

#include "definitions.h"

#include "gfx/hal/inc/gfx_common.h"
#include "gfx/hal/inc/gfx_context.h"
#include "drv_gfx_disp_intf.h"

// This is the base address of the SMC0 peripheral on V71.
// Update to appropriate base address for other MCUs.

#define EBI_CS_INDEX  0

#define EBI_BASE_ADDR  EBI_CS0_ADDR

// Use Address bit 12 as DCX
// This lets set set DCX = 1 by writing the data/params to ILI9488_DBIB_DATA_ADDR
#define GFX_DISP_INTF_SMC_DATA_ADDR  (EBI_BASE_ADDR | (1 << 12))
#define GFX_DISP_INTF_SMC_CMD_ADDR EBI_BASE_ADDR

// Data width for 16-bit SMC
typedef uint16_t DBUS_WIDTH_T;




/** GFX_DISP_INTF_SMC

  Summary:
    Structure contains status and handles for SPI interface.
    
 */
typedef struct
{
    /* The GFX HAL context */
    GFX_Context * gfx;
    
    /* Address to write commands */
    volatile DBUS_WIDTH_T * cmdAddr;
    
    /* Address to write data/parameters */
    volatile DBUS_WIDTH_T * dataAddr;
} GFX_DISP_INTF_SMC;

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

static inline void GFX_Disp_Intf_DelayNOP()
{
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
}

GFX_Disp_Intf GFX_Disp_Intf_Open(GFX_Context * gfx, unsigned int index)
{   
    GFX_DISP_INTF_SMC * intf = NULL;
    
    if (gfx == NULL)
        return 0;
    
    intf = (GFX_DISP_INTF_SMC *) gfx->memory.calloc(1, sizeof (GFX_DISP_INTF_SMC));
    
    if (intf == NULL)
        return 0;
    
    intf->dataAddr = (DBUS_WIDTH_T *) GFX_DISP_INTF_SMC_DATA_ADDR;
    intf->cmdAddr = (DBUS_WIDTH_T *) GFX_DISP_INTF_SMC_CMD_ADDR;
    
    intf->gfx = gfx;

    return (GFX_Disp_Intf) intf;
}

void GFX_Disp_Intf_Close(GFX_Disp_Intf intf)
{
    if (((GFX_DISP_INTF_SMC *) intf) == NULL)
        return;

    ((GFX_DISP_INTF_SMC *) intf)->gfx->memory.free(((GFX_DISP_INTF_SMC *) intf));
}

GFX_Result GFX_Disp_Intf_PinControl(GFX_Disp_Intf intf, GFX_DISP_INTF_PIN pin, GFX_DISP_INTF_PIN_VALUE value)
{
    GFX_Result res = GFX_FAILURE;
    
    if (((GFX_DISP_INTF_SMC *) intf) == NULL)
        return GFX_FAILURE;
    
    switch(pin)
    {
        case GFX_DISP_INTF_PIN_CS:
#ifdef GFX_DISP_INTF_PIN_CS_Set
            if (value == GFX_DISP_INTF_PIN_CLEAR)
                GFX_DISP_INTF_PIN_CS_Clear();
            else
                GFX_DISP_INTF_PIN_CS_Set();
            
            res = GFX_SUCCESS;
#endif
            break;
        case GFX_DISP_INTF_PIN_WR:
#ifdef GFX_DISP_INTF_PIN_WR_Set
            if (value == GFX_DISP_INTF_PIN_CLEAR)
                GFX_DISP_INTF_PIN_WR_Clear();
            else
                GFX_DISP_INTF_PIN_WR_Set();

            res = GFX_SUCCESS;            
#endif
            break;
        case GFX_DISP_INTF_PIN_RD:
#ifdef GFX_DISP_INTF_PIN_RD_Set
            if (value == GFX_DISP_INTF_PIN_CLEAR)
                GFX_DISP_INTF_PIN_RD_Clear();
            else
                GFX_DISP_INTF_PIN_RD_Set();
            
            res = GFX_SUCCESS;
#endif
            break;
        case GFX_DISP_INTF_PIN_RSDC:
#ifdef GFX_DISP_INTF_PIN_RSDC_Set
            if (value == GFX_DISP_INTF_PIN_CLEAR)
                GFX_DISP_INTF_PIN_RSDC_Clear();
            else
                GFX_DISP_INTF_PIN_RSDC_Set();
            
            res = GFX_SUCCESS;
#endif            
            break;
        default:
            break;
    }
    
    return res;
}

GFX_Result GFX_Disp_Intf_WriteCommand(GFX_Disp_Intf intf, uint8_t cmd)
{
    GFX_DISP_INTF_SMC * smcIntf = (GFX_DISP_INTF_SMC *) intf;
    
    if (smcIntf == NULL)
        return GFX_FAILURE;

        
    GFX_Disp_Intf_Sync();  
    *(smcIntf->cmdAddr) = cmd ;
    GFX_Disp_Intf_Sync();
	

    return GFX_SUCCESS;
}

GFX_Result GFX_Disp_Intf_WriteData(GFX_Disp_Intf intf, uint8_t * data, int bytes)
{
    GFX_DISP_INTF_SMC * smcIntf = (GFX_DISP_INTF_SMC *) intf;
    unsigned int i;
    
    if (smcIntf == NULL ||
        data == NULL ||
        bytes == 0)
        return GFX_FAILURE;
    
    GFX_Disp_Intf_Sync();    

    
    for (i = 0; i < bytes; i++)
    {
        *(smcIntf->dataAddr) = *(data);
        GFX_Disp_Intf_Sync();
        data++;
    }
    
    return GFX_SUCCESS;
}

GFX_Result GFX_Disp_Intf_WriteData16(GFX_Disp_Intf intf, uint16_t * data, int num)
{
    GFX_DISP_INTF_SMC * smcIntf = (GFX_DISP_INTF_SMC *) intf;
    unsigned int i;
    
    if ((smcIntf) == NULL ||
        data == NULL ||
        num == 0)
        return GFX_FAILURE;

    GFX_Disp_Intf_Sync();


    for (i = 0; i < num; i++)
    {
        *(smcIntf->dataAddr) = *(data);
        GFX_Disp_Intf_Sync();
        data++;
    }
    
    return GFX_SUCCESS;
}

GFX_Result GFX_Disp_Intf_ReadData16(GFX_Disp_Intf intf, uint16_t * data, int num)
{
    GFX_DISP_INTF_SMC * smcIntf = (GFX_DISP_INTF_SMC *) intf;
    unsigned int i;
    
    if (smcIntf == NULL ||
        num == 0 ||
        data == NULL)
        return GFX_FAILURE;
    
    GFX_Disp_Intf_Sync();
    
    for (i = 0; i < num; i++)
    {
        *data = *(smcIntf->dataAddr);
        data++;
    }
    
    return GFX_SUCCESS;
}

GFX_Result GFX_Disp_Intf_ReadData(GFX_Disp_Intf intf, uint8_t * data, int bytes)
{
    GFX_DISP_INTF_SMC * smcIntf = (GFX_DISP_INTF_SMC *) intf;
    unsigned int i;
    
    if (smcIntf == NULL ||
        bytes == 0 ||
        data == NULL)
        return GFX_FAILURE;

    GFX_Disp_Intf_Sync();
    
    for (i = 0; i < bytes; i++)
    {
        *data = (uint8_t) *(smcIntf->dataAddr);
        data++;
    }
    
    return GFX_SUCCESS;
}

GFX_Result GFX_Disp_Intf_ReadCommandData(GFX_Disp_Intf intf, uint8_t cmd, uint8_t * data, int num_data)
{
    GFX_DISP_INTF_SMC * smcIntf = (GFX_DISP_INTF_SMC *) intf;
    GFX_Result retval;
    unsigned int i;
    
    if (smcIntf == NULL ||
        num_data == 0 ||
        data == NULL)
        return GFX_FAILURE;

    retval = GFX_Disp_Intf_WriteCommand(intf, cmd);
    if (retval != GFX_SUCCESS)
        return GFX_FAILURE;  

    GFX_Disp_Intf_Sync();
    
    for (i = 0; i < num_data; i++)
    {
        *data = (uint8_t) *(smcIntf->dataAddr);
        data++;
    }
    
    return retval;
}

GFX_Result GFX_Disp_Intf_WriteCommandParm(GFX_Disp_Intf intf, uint8_t cmd, uint8_t * parm, int num_parms)
{
    GFX_Result retval;
    GFX_DISP_INTF_SMC * smcIntf;
    unsigned int i;
    
    retval = GFX_Disp_Intf_WriteCommand(intf, cmd);
    if (retval != GFX_SUCCESS)
        return GFX_FAILURE;


    if (num_parms > 0 && parm != NULL)
    {
        smcIntf = (GFX_DISP_INTF_SMC *) intf;
        
        for (i = 0; i < num_parms; i++)
        {
            *(smcIntf->dataAddr) = *(parm);
            GFX_Disp_Intf_Sync();
            parm++;
        }
    }
    
    return retval;
}

GFX_Result GFX_Disp_Intf_Write(GFX_Disp_Intf intf, uint8_t * data, int bytes)
{
    GFX_DISP_INTF_SMC * smcIntf;
    unsigned int i;
    
    if (((GFX_DISP_INTF_SMC *) intf) == NULL ||
        data == NULL ||
        bytes == 0)
        return GFX_FAILURE;

    smcIntf = (GFX_DISP_INTF_SMC *) intf;
    
    GFX_Disp_Intf_Sync();
    
    for (i = 0; i < bytes; i++)
    {
        *(smcIntf->cmdAddr) = *(data);
        data++;
        GFX_Disp_Intf_Sync();
    }
    
    return GFX_SUCCESS;
}

GFX_Result GFX_Disp_Intf_WriteDataByte(GFX_Disp_Intf intf, uint8_t data)
{
    GFX_DISP_INTF_SMC * smcIntf;
    
    if (((GFX_DISP_INTF_SMC *) intf) == NULL)
        return GFX_FAILURE;

    smcIntf = (GFX_DISP_INTF_SMC *) intf;


    
    GFX_Disp_Intf_Sync();
    *(smcIntf->cmdAddr) = data;
    GFX_Disp_Intf_Sync();
    
    return GFX_SUCCESS;
}

GFX_Result GFX_Disp_Intf_Read(GFX_Disp_Intf intf, uint8_t * data, int bytes)
{
    GFX_DISP_INTF_SMC * smcIntf = (GFX_DISP_INTF_SMC *) intf;
    unsigned int i;
    
    if (smcIntf == NULL ||
        bytes == 0 ||
        data == NULL)
        return GFX_FAILURE;
    
    GFX_Disp_Intf_Sync();
    
    for (i = 0; i < bytes; i++)
    {
        *(data + i) = *(smcIntf->cmdAddr);
    }
    
    return GFX_SUCCESS;
}


/* *****************************************************************************
 End of File
 */
