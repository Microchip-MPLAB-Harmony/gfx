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
    drv_gfx_intf_iogroup.c

  Summary:
    Implements the parallel display interface driver over IO group

  Description:
    Implements the parallel display interface driver over IO group

    Created with MPLAB Harmony Version 3.0
 *******************************************************************************/

#include "definitions.h"

#include "gfx/interface/drv_gfx_disp_intf.h"

#define GFX_DISP_INTF_PORT  (PORT_GROUP_2)
#define GFX_DISP_INTF_PORT_HALF (0)
#define GFX_DISP_INTF_PORT_QUADRANT (0)
#define GFX_DISP_INTF_PORT_MASK_SHIFT (GFX_DISP_INTF_PORT_HALF * 16 + GFX_DISP_INTF_PORT_QUADRANT * 8)

#ifdef GFX_DISP_INTF_PIN_WR_Set
#define GFX_DISP_WR_Control(intf, value) GFX_Disp_Intf_PinControl(intf, GFX_DISP_INTF_PIN_WR, value)
#else
#error "GFX_DISP_INTF_PIN_WR pin is not defined. Please define in Pin Manager"
#endif

#ifdef GFX_DISP_INTF_PIN_RD_Set
#define GFX_DISP_RD_Control(intf, value) GFX_Disp_Intf_PinControl(intf, GFX_DISP_INTF_PIN_RD, value)
#else
#error "GFX_DISP_INTF_PIN_RD pin is not defined. Please define in Pin Manager"
#endif

#ifdef GFX_DISP_INTF_PIN_RSDC_Set
#define GFX_DISP_RSDC_Control(intf, value) GFX_Disp_Intf_PinControl(intf, GFX_DISP_INTF_PIN_RSDC, value)
#else
#error "GFX_DISP_INTF_PIN_RSDC DCx pin is not defined. Please define in Pin Manager"
#endif

/** GFX_DISP_INTF_PORTGROUP

  Summary:
    Structure contains status and handles for SPI interface.
    
 */
typedef struct
{
    /* port to use */
    PORT_GROUP port;

    /* interface lock */
    int locked;
} GFX_DISP_INTF_PORTGROUP;

GFX_DISP_INTF_PORTGROUP portGroupIntf = {.locked = 0};

/** GFX_Disp_Intf_Sync

  Function:
    static inline void GFX_Disp_Intf_Sync(void)

  Summary:
    Add synchronization for core writes to the SMC

 */
static inline void GFX_Disp_Intf_Sync(void)
{
//    __ASM volatile ("dsb");
//    __ASM volatile ("dmb");
}

static inline void GFX_Disp_Intf_DelayNOP()
{
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
}

GFX_Disp_Intf GFX_Disp_Intf_Open(void)
{   
    GFX_DISP_INTF_PORTGROUP * intf = &portGroupIntf;

    if (intf == NULL || intf->locked == 1)
        return 0;
    
    intf->port = GFX_DISP_INTF_PORT;

    intf->locked = 1;

    return (GFX_Disp_Intf) intf;
}

void GFX_Disp_Intf_Close(GFX_Disp_Intf intf)
{
    if (((GFX_DISP_INTF_PORTGROUP *) intf) == NULL)
        return;

    ((GFX_DISP_INTF_PORTGROUP *) intf)->locked = 0;
}

int GFX_Disp_Intf_PinControl(GFX_Disp_Intf intf, GFX_DISP_INTF_PIN pin, GFX_DISP_INTF_PIN_VALUE value)
{
    int res = -1;
    
    if (((GFX_DISP_INTF_PORTGROUP *) intf) == NULL)
        return -1;
    
    switch(pin)
    {
        case GFX_DISP_INTF_PIN_CS:
#ifdef GFX_DISP_INTF_PIN_CS_Set
            if (value == GFX_DISP_INTF_PIN_CLEAR)
                GFX_DISP_INTF_PIN_CS_Clear();
            else
                GFX_DISP_INTF_PIN_CS_Set();
            
            res = 0;
#endif
            break;
        case GFX_DISP_INTF_PIN_WR:
#ifdef GFX_DISP_INTF_PIN_WR_Set
            if (value == GFX_DISP_INTF_PIN_CLEAR)
                GFX_DISP_INTF_PIN_WR_Clear();
            else
                GFX_DISP_INTF_PIN_WR_Set();

            res = 0;            
#endif
            break;
        case GFX_DISP_INTF_PIN_RD:
#ifdef GFX_DISP_INTF_PIN_RD_Set
            if (value == GFX_DISP_INTF_PIN_CLEAR)
                GFX_DISP_INTF_PIN_RD_Clear();
            else
                GFX_DISP_INTF_PIN_RD_Set();
            
            res = 0;
#endif
            break;
        case GFX_DISP_INTF_PIN_RSDC:
#ifdef GFX_DISP_INTF_PIN_RSDC_Set
            if (value == GFX_DISP_INTF_PIN_CLEAR)
                GFX_DISP_INTF_PIN_RSDC_Clear();
            else
                GFX_DISP_INTF_PIN_RSDC_Set();
            
            res = 0;
#endif            
            break;
        default:
            break;
    }
    
    return res;
}

int GFX_Disp_Intf_WriteCommand(GFX_Disp_Intf intf, uint8_t cmd)
{
    GFX_DISP_INTF_PORTGROUP * ioGroupIntf = (GFX_DISP_INTF_PORTGROUP *) intf;
    
    if (ioGroupIntf == NULL)
        return -1;

    GFX_DISP_RSDC_Control(intf, GFX_DISP_INTF_PIN_CLEAR);
    GFX_DISP_WR_Control(intf, GFX_DISP_INTF_PIN_SET);
    GFX_Disp_Intf_DelayNOP();
        
    GFX_Disp_Intf_Sync();
    PORT_GroupWrite(ioGroupIntf->port, 
                  0xff << GFX_DISP_INTF_PORT_MASK_SHIFT,
                  cmd << GFX_DISP_INTF_PORT_MASK_SHIFT);
    GFX_Disp_Intf_Sync();
	
    GFX_Disp_Intf_DelayNOP();
    GFX_DISP_WR_Control(intf, GFX_DISP_INTF_PIN_CLEAR);
    GFX_Disp_Intf_DelayNOP();
    GFX_DISP_WR_Control(intf, GFX_DISP_INTF_PIN_SET);
    GFX_Disp_Intf_DelayNOP();

    return 0;
}

int GFX_Disp_Intf_WriteData(GFX_Disp_Intf intf, uint8_t * data, int bytes)
{
    GFX_DISP_INTF_PORTGROUP * ioGroupIntf = (GFX_DISP_INTF_PORTGROUP *) intf;
    unsigned int i;
    
    if (ioGroupIntf == NULL ||
        data == NULL ||
        bytes == 0)
        return -1;
    
    GFX_Disp_Intf_Sync();    

    GFX_DISP_RSDC_Control(intf, GFX_DISP_INTF_PIN_SET);
    GFX_DISP_WR_Control(intf, GFX_DISP_INTF_PIN_SET);
    GFX_Disp_Intf_DelayNOP();
    
    for (i = 0; i < bytes; i++)
    {
        PORT_GroupWrite(ioGroupIntf->port, 
                  0xff << GFX_DISP_INTF_PORT_MASK_SHIFT,
                  (*data) << GFX_DISP_INTF_PORT_MASK_SHIFT);
        GFX_Disp_Intf_Sync();
        data++;
        GFX_Disp_Intf_DelayNOP();
        GFX_DISP_WR_Control(intf, GFX_DISP_INTF_PIN_CLEAR);
        GFX_Disp_Intf_DelayNOP();
        GFX_DISP_WR_Control(intf, GFX_DISP_INTF_PIN_SET);
        GFX_Disp_Intf_DelayNOP();
    }
    
    return 0;
}

int GFX_Disp_Intf_WriteData16(GFX_Disp_Intf intf, uint16_t * data, int num)
{
    GFX_DISP_INTF_PORTGROUP * ioGroupIntf = (GFX_DISP_INTF_PORTGROUP *) intf;
    unsigned int i;
    
    if ((ioGroupIntf) == NULL ||
        data == NULL ||
        num == 0)
        return -1;

    GFX_Disp_Intf_Sync();

    GFX_DISP_RSDC_Control(intf, GFX_DISP_INTF_PIN_SET);
    GFX_DISP_WR_Control(intf, GFX_DISP_INTF_PIN_SET);
    GFX_Disp_Intf_DelayNOP();

    for (i = 0; i < num; i++)
    {
        PORT_GroupWrite(ioGroupIntf->port, 
                  0xffff << GFX_DISP_INTF_PORT_MASK_SHIFT,
                  (*data) << GFX_DISP_INTF_PORT_MASK_SHIFT);
        GFX_Disp_Intf_Sync();
        data++;
        GFX_Disp_Intf_DelayNOP();
        GFX_DISP_WR_Control(intf, GFX_DISP_INTF_PIN_CLEAR);
        GFX_Disp_Intf_DelayNOP();
        GFX_DISP_WR_Control(intf, GFX_DISP_INTF_PIN_SET);
        GFX_Disp_Intf_DelayNOP();
    }
    
    return 0;
}

int GFX_Disp_Intf_ReadData16(GFX_Disp_Intf intf, uint16_t * data, int num)
{
    GFX_DISP_INTF_PORTGROUP * ioGroupIntf = (GFX_DISP_INTF_PORTGROUP *) intf;
    unsigned int i;

    if (ioGroupIntf == NULL ||
        num == 0 ||
        data == NULL)
        return -1;

    GFX_Disp_Intf_Sync();
    GFX_DISP_RSDC_Control(intf, GFX_DISP_INTF_PIN_SET);
    GFX_DISP_RD_Control(intf, GFX_DISP_INTF_PIN_SET);
    GFX_Disp_Intf_DelayNOP();

    for (i = 0; i < num; i++)
    {
        GFX_DISP_RD_Control(intf, GFX_DISP_INTF_PIN_CLEAR);
        GFX_Disp_Intf_DelayNOP();
        *data = (uint16_t) ((PORT_GroupRead(ioGroupIntf->port) >> 
                             GFX_DISP_INTF_PORT_MASK_SHIFT) & 0xffff);
        data++;
        GFX_Disp_Intf_DelayNOP();
        GFX_DISP_RD_Control(intf, GFX_DISP_INTF_PIN_SET);
    }
    
    return 0;
}

int GFX_Disp_Intf_ReadData(GFX_Disp_Intf intf, uint8_t * data, int bytes)
{
    GFX_DISP_INTF_PORTGROUP * ioGroupIntf = (GFX_DISP_INTF_PORTGROUP *) intf;
    unsigned int i;
    
    if (ioGroupIntf == NULL ||
        bytes == 0 ||
        data == NULL)
        return -1;

    GFX_Disp_Intf_Sync();
    GFX_DISP_RSDC_Control(intf, GFX_DISP_INTF_PIN_SET);
    GFX_DISP_RD_Control(intf, GFX_DISP_INTF_PIN_SET);
    GFX_Disp_Intf_DelayNOP();
    
    for (i = 0; i < bytes; i++)
    {
        GFX_DISP_RD_Control(intf, GFX_DISP_INTF_PIN_CLEAR);
        GFX_Disp_Intf_DelayNOP();
        *data = (uint8_t)((PORT_GroupRead(ioGroupIntf->port) >>
                           GFX_DISP_INTF_PORT_MASK_SHIFT) & 0xff);
        data++;
        GFX_Disp_Intf_DelayNOP();
        GFX_DISP_RD_Control(intf, GFX_DISP_INTF_PIN_SET);
    }
    
    return 0;
}

DRV_GFX_DEPRECATED int GFX_Disp_Intf_ReadCommandData(GFX_Disp_Intf intf, uint8_t cmd, uint8_t * data, int num_data)
{
    GFX_DISP_INTF_PORTGROUP * ioGroupIntf = (GFX_DISP_INTF_PORTGROUP *) intf;
    int retval;
    unsigned int i;
    
    if (ioGroupIntf == NULL ||
        num_data == 0 ||
        data == NULL)
        return -1;

    retval = GFX_Disp_Intf_WriteCommand(intf, cmd);
    if (retval != 0)
        return -1;  

    GFX_Disp_Intf_Sync();
    GFX_DISP_RSDC_Control(intf, GFX_DISP_INTF_PIN_SET);
    GFX_DISP_RD_Control(intf, GFX_DISP_INTF_PIN_SET);
    GFX_Disp_Intf_DelayNOP();
    
    for (i = 0; i < num_data; i++)
    {
        GFX_DISP_RD_Control(intf, GFX_DISP_INTF_PIN_CLEAR);
        GFX_Disp_Intf_DelayNOP();
        *data = (uint8_t) ((PORT_GroupRead(ioGroupIntf->port) >>
                            GFX_DISP_INTF_PORT_MASK_SHIFT) & 0xff);
        data++;
        GFX_Disp_Intf_DelayNOP();
        GFX_DISP_RD_Control(intf, GFX_DISP_INTF_PIN_SET);
    }
    
    return retval;
}

DRV_GFX_DEPRECATED int GFX_Disp_Intf_WriteCommandParm(GFX_Disp_Intf intf, uint8_t cmd, uint8_t * parm, int num_parms)
{
    int retval;
    GFX_DISP_INTF_PORTGROUP * ioGroupIntf;
    unsigned int i;
    
    retval = GFX_Disp_Intf_WriteCommand(intf, cmd);
    if (retval != 0)
        return -1;

    GFX_DISP_RSDC_Control(intf, GFX_DISP_INTF_PIN_SET);

    if (num_parms > 0 && parm != NULL)
    {
        ioGroupIntf = (GFX_DISP_INTF_PORTGROUP *) intf;
        
        for (i = 0; i < num_parms; i++)
        {
            PORT_GroupWrite(ioGroupIntf->port, 
                  0xff << GFX_DISP_INTF_PORT_MASK_SHIFT,
                  (*parm) << GFX_DISP_INTF_PORT_MASK_SHIFT);
            GFX_Disp_Intf_Sync();
            parm++;
            GFX_Disp_Intf_DelayNOP();
            GFX_DISP_WR_Control(intf, GFX_DISP_INTF_PIN_CLEAR);
            GFX_Disp_Intf_DelayNOP();
            GFX_DISP_WR_Control(intf, GFX_DISP_INTF_PIN_SET);
            GFX_Disp_Intf_DelayNOP();
        }
    }
    
    return retval;
}

int GFX_Disp_Intf_Write(GFX_Disp_Intf intf, uint8_t * data, int bytes)
{
    GFX_DISP_INTF_PORTGROUP * ioGroupIntf;
    unsigned int i;
    
    if (((GFX_DISP_INTF_PORTGROUP *) intf) == NULL ||
        data == NULL ||
        bytes == 0)
        return -1;

    ioGroupIntf = (GFX_DISP_INTF_PORTGROUP *) intf;
    
    GFX_Disp_Intf_Sync();
    GFX_DISP_WR_Control(intf, GFX_DISP_INTF_PIN_SET);
    GFX_Disp_Intf_DelayNOP();
    
    for (i = 0; i < bytes; i++)
    {
        PORT_GroupWrite(ioGroupIntf->port, 
                      0xff << GFX_DISP_INTF_PORT_MASK_SHIFT,
                      (*data) << GFX_DISP_INTF_PORT_MASK_SHIFT);
        data++;
        GFX_Disp_Intf_Sync();
        GFX_Disp_Intf_DelayNOP();
        GFX_DISP_WR_Control(intf, GFX_DISP_INTF_PIN_CLEAR);
        GFX_Disp_Intf_DelayNOP();
        GFX_DISP_WR_Control(intf, GFX_DISP_INTF_PIN_SET);
        GFX_Disp_Intf_DelayNOP();
    }
    
    return 0;
}

int GFX_Disp_Intf_WriteDataByte(GFX_Disp_Intf intf, uint8_t data)
{
    GFX_DISP_INTF_PORTGROUP * ioGroupIntf;
    
    if (((GFX_DISP_INTF_PORTGROUP *) intf) == NULL)
        return -1;

    ioGroupIntf = (GFX_DISP_INTF_PORTGROUP *) intf;

    GFX_DISP_RSDC_Control(intf, GFX_DISP_INTF_PIN_SET);

    GFX_DISP_WR_Control(intf, GFX_DISP_INTF_PIN_SET);
    GFX_Disp_Intf_DelayNOP();
    
    GFX_Disp_Intf_Sync();
    PORT_GroupWrite(ioGroupIntf->port, 
                  0xff << GFX_DISP_INTF_PORT_MASK_SHIFT,
                  data << GFX_DISP_INTF_PORT_MASK_SHIFT);
    GFX_Disp_Intf_Sync();
    GFX_Disp_Intf_DelayNOP();
    GFX_DISP_WR_Control(intf, GFX_DISP_INTF_PIN_CLEAR);
    GFX_Disp_Intf_DelayNOP();
    GFX_DISP_WR_Control(intf, GFX_DISP_INTF_PIN_SET);
    GFX_Disp_Intf_DelayNOP();
    
    return 0;
}

int GFX_Disp_Intf_Read(GFX_Disp_Intf intf, uint8_t * data, int bytes)
{
    GFX_DISP_INTF_PORTGROUP * ioGroupIntf = (GFX_DISP_INTF_PORTGROUP *) intf;
    unsigned int i;
    uint32_t value;
    
    if (ioGroupIntf == NULL ||
        bytes == 0 ||
        data == NULL)
        return -1;
    
    GFX_Disp_Intf_Sync();
    GFX_DISP_RD_Control(intf, GFX_DISP_INTF_PIN_SET);
    GFX_Disp_Intf_DelayNOP();
    
    for (i = 0; i < bytes; i++)
    {
        GFX_DISP_RD_Control(intf, GFX_DISP_INTF_PIN_CLEAR);
        GFX_Disp_Intf_DelayNOP();
        *data = (uint8_t) ((value = PORT_GroupRead(ioGroupIntf->port) >>
                            GFX_DISP_INTF_PORT_MASK_SHIFT) & 0xff);
        data++;
        GFX_Disp_Intf_DelayNOP();
        GFX_DISP_RD_Control(intf, GFX_DISP_INTF_PIN_SET);
    }
    
    return 0;
}

int GFX_Disp_Intf_Ready(GFX_Disp_Intf intf)
{
    return 1;
}


/* *****************************************************************************
 End of File
 */
