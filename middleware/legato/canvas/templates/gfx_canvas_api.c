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
  GFX GLCD Driver Interface Declarations for Static Single Instance Driver

  Company:
    Microchip Technology Inc.

  File Name:
    gfx_canvas_api.c

  Summary:
    GFX Canvas Virtual Display Component private header file

  Description:
    The GFX Canvas provides a virtual display driver interface. 

  Remarks:
    None
*******************************************************************************/

#include <stdlib.h>

#include "gfx/canvas/gfx_canvas_api.h"
#include "gfx/canvas/gfx_canvas.h"

//Application Interface functions
int gfxcCreate(void)
{
    unsigned int idx = CANVAS_ID_INVALID;
    unsigned int i = CANVAS_ID_INVALID;
    
    //Try to allocate from static canvas objects
    for (i = 0; i < CONFIG_NUM_CANVAS_OBJ; i++)
    {
        if (canvas[i].id == CANVAS_ID_INVALID)
        {
            idx = i;
            canvas[i].id = i;
            
            //Set default properties
            canvas[idx].layer.alpha = 0xff;
            canvas[idx].layer.id = LAYER_ID_INVALID;
            canvas[idx].layer.pos.xpos = 0;
            canvas[idx].layer.pos.ypos = 0;
            
            break;
        }
    }
    
    return idx;
}

GFXC_RESULT gfxcDestroy(unsigned int canvasID)
{
    if (canvasID < CONFIG_NUM_CANVAS_OBJ)
    {
        canvas[canvasID].id = CANVAS_ID_INVALID;
        canvas[canvasID].layer.id = LAYER_ID_INVALID;
        
        return GFX_SUCCESS;
    }
    
    return GFX_FAILURE;
}

int gfxcSetPixelBuffer(unsigned int canvasID,
                       const unsigned int width,
                       const unsigned int height,
                       const GFXC_COLOR_FORMAT format,
                       void * buf)
{
    if (canvasID < CONFIG_NUM_CANVAS_OBJ)
    {
            gfxPixelBufferCreate(width, height, format, (const void *) buf, &canvas[canvasID].pixelBuffer);
            
            canvas[canvasID].layer.size.width = width;
            canvas[canvasID].layer.size.height = height;
            
        return GFX_SUCCESS;
    }
    
    return GFX_FAILURE;
}

GFXC_RESULT gfxcSetLayer(unsigned int canvasID, unsigned int layerID)
{
    if (canvasID < CONFIG_NUM_CANVAS_OBJ &&
        canvas[canvasID].active == GFX_FALSE &&
        layerID < numLayers)
    {
        canvas[canvasID].layer.id = layerID;
            
        return GFX_SUCCESS;
    }
    
    return GFX_FAILURE;  
}

int gfxcGetlayer(unsigned int canvasID)
{
    if (canvasID < CONFIG_NUM_CANVAS_OBJ)
    {
        return canvas[canvasID].layer.id;
    }
    
    return GFX_FAILURE;
}

GFXC_RESULT gfxcClearLayer(unsigned int canvasID)
{
    if (canvasID < CONFIG_NUM_CANVAS_OBJ)
    {
        if (canvas[canvasID].active == GFX_FALSE)
        {        
            canvas[canvasID].layer.id = LAYER_ID_INVALID;
            
            return GFX_SUCCESS;
        }
    }
    
    return GFX_FAILURE;
}

GFXC_RESULT gfxcSetWindowAlpha(unsigned int canvasID, uint8_t alpha)
{
    if (canvasID < CONFIG_NUM_CANVAS_OBJ)
    {
        canvas[canvasID].layer.alpha = alpha;

        return GFX_SUCCESS;
    }
        
    return GFX_FAILURE;
}

GFXC_RESULT gfxcSetWindowPosition(unsigned int canvasID, int xpos, int ypos)
{
    if (canvasID < CONFIG_NUM_CANVAS_OBJ)
    {
        canvas[canvasID].layer.pos.xpos = xpos;
        canvas[canvasID].layer.pos.ypos = ypos;

        return GFX_SUCCESS;
    }
        
    return GFX_FAILURE;
}

GFXC_RESULT gfxcSetWindowSize(unsigned int canvasID, unsigned int width, unsigned int height)
{
    if (canvasID < CONFIG_NUM_CANVAS_OBJ)
    {
        canvas[canvasID].layer.size.width = width;
        canvas[canvasID].layer.size.height = height;

        return GFX_SUCCESS;
    }
        
    return GFX_FAILURE;
}

GFXC_RESULT gfxcShowCanvas(unsigned int canvasID)
{
    if (canvasID < CONFIG_NUM_CANVAS_OBJ)
    {
        canvas[canvasID].active = GFX_TRUE;
        return GFX_SUCCESS;
    }
        
    return GFX_FAILURE;    
}

GFXC_RESULT gfxcHideCanvas(unsigned int canvasID)
{
    if (canvasID < CONFIG_NUM_CANVAS_OBJ)
    {
        canvas[canvasID].active = GFX_FALSE;
        return GFX_SUCCESS;        
    }
        
    return GFX_FAILURE;    
}

GFXC_RESULT gfxcCanvasUpdate(unsigned int canvasID)
{
    if (canvasID < CONFIG_NUM_CANVAS_OBJ)
    {
        _gfxcCanvasUpdate(canvasID);
        
        return GFX_SUCCESS;        
    }
        
    return GFX_FAILURE;    
}

GFXC_RESULT gfxcSetEffectsInterval(unsigned int ms)
{
    return _gfxcSetEffectsIntervalMS(ms);
}

GFXC_RESULT gfxcSetEffectsCallback(unsigned int canvasID, gfxcCallback cb, void * parm)
{
    if (canvasID < CONFIG_NUM_CANVAS_OBJ)
    {
        canvas[canvasID].effects.cb = cb;
        canvas[canvasID].effects.parm = parm;
        
        return GFX_SUCCESS;        
    }
        
    return GFX_FAILURE; 
}

GFXC_RESULT gfxcStartEffectFade(unsigned int canvasID, uint8_t startAlpha, uint8_t endAlpha, uint8_t delta)
{
    if (canvasID < CONFIG_NUM_CANVAS_OBJ &&
        canvas[canvasID].effects.fade.status == GFXC_FX_IDLE &&
        canvas[canvasID].active == GFX_TRUE)
    {
        canvas[canvasID].effects.fade.startAlpha = startAlpha;
        canvas[canvasID].effects.fade.endAlpha = endAlpha;
        canvas[canvasID].effects.fade.delta = delta;
        
        canvas[canvasID].effects.fade.status = GFXC_FX_START;
        
        gfxcSetWindowAlpha(canvasID, startAlpha);
        _gfxcStartEffects();
        
        if (canvas[canvasID].effects.cb != NULL)
            canvas[canvasID].effects.cb(canvasID,
                                        GFXC_FX_FADE,
                                        GFXC_FX_START,
                                        canvas[canvasID].effects.parm);
        
        return GFX_SUCCESS;        
    }
        
    return GFX_FAILURE; 
}

GFXC_RESULT gfxcStartEffectMove(unsigned int canvasID,
                                GFXC_FX_MOVE_TYPE type,
                                int startX,
                                int startY,
                                int endX,
                                int endY,
                                unsigned int delta)
{
    if (canvasID < CONFIG_NUM_CANVAS_OBJ &&
        canvas[canvasID].effects.move.status == GFXC_FX_IDLE &&
        canvas[canvasID].active == GFX_TRUE)
    {
        canvas[canvasID].effects.move.startX = startX;
        canvas[canvasID].effects.move.startY = startY;
        canvas[canvasID].effects.move.endX = endX;
        canvas[canvasID].effects.move.endY = endY;
        canvas[canvasID].effects.move.type = type;
        canvas[canvasID].effects.move.delta = delta;
        canvas[canvasID].effects.move.status = GFXC_FX_START;
        
        gfxcSetWindowPosition(canvasID, startX, startY);
        _gfxcStartEffects();
        
        if (canvas[canvasID].effects.cb != NULL)
            canvas[canvasID].effects.cb(canvasID,
                                        GFXC_FX_MOVE,
                                        GFXC_FX_START,
                                        canvas[canvasID].effects.parm);
        
        return GFX_SUCCESS;        
    }
        
    return GFX_FAILURE; 
}

GFXC_RESULT gfxcStopEffect(unsigned int canvasID, GFXC_FX_TYPE effect)
{
    if (canvasID < CONFIG_NUM_CANVAS_OBJ)
    {
        switch(effect)
        {
            case GFXC_FX_FADE:
                canvas[canvasID].effects.fade.status = GFXC_FX_IDLE;
                break;
            case GFXC_FX_MOVE:
                canvas[canvasID].effects.move.status = GFXC_FX_IDLE;
                break;
            default:
                break;
        }
              
        
        return GFX_SUCCESS;        
    }
        
    return GFX_FAILURE; 
}


/* *****************************************************************************
 End of File
 */
