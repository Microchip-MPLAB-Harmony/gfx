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

#include "gfx/legato/renderer/legato_renderer.h"

#include <string.h>

#include "gfx/legato/common/legato_rect.h"
#include "gfx/legato/common/legato_pixelbuffer.h"
#include "gfx/legato/common/legato_utils.h"
#include "gfx/legato/core/legato_state.h"
#include "gfx/legato/datastructure/legato_rectarray.h"
#include "gfx/driver/gfx_driver.h"

leRenderState _rendererState;

#define SCRACH_BUFFER_SZ     (LE_SCRATCH_BUFFER_SIZE_KB * 1024)
#define MAX_RECTARRAYS_SZ    8

static uint8_t scratchBuffer[SCRACH_BUFFER_SZ];
static uint32_t maxScratchPixels;

static lePixelBuffer renderBuffer;

static leEvent paintEvt = { LE_WIDGET_EVENT_PAINT };

leRenderState* leGetRenderState()
{
    return &_rendererState; 
}

lePalette* leRenderer_GetGlobalPalette()
{
    return _rendererState.globalPalette;
}

leResult leRenderer_SetGlobalPalette(lePalette* pal)
{
    if(LE_GLOBAL_COLOR_MODE != LE_COLOR_MODE_PALETTE)
        return LE_FAILURE;

    _rendererState.globalPalette = pal;

    return LE_SUCCESS;
}

leColor leRenderer_GlobalPaletteLookup(uint32_t idx)
{
    LE_ASSERT(LE_GLOBAL_COLOR_MODE == LE_COLOR_MODE_PALETTE &&
              _rendererState.globalPalette != NULL);

    return lePalette_GetColor(_rendererState.globalPalette, idx);
}

leColor leRenderer_ConvertColor(leColor inColor, leColorMode inMode)
{
    return leColorConvert(inMode, LE_GLOBAL_COLOR_MODE, inColor);
}

leRect leRenderer_GetDisplayRect()
{
    return _rendererState.displayRect;
}

leRect leRenderer_GetDrawRect()
{
    return _rendererState.drawRect;
}

leBool leRenderer_CullDrawRect(const leRect* rect)
{
    return leRectIntersects(&_rendererState.drawRect, rect) == LE_FALSE;
}

leBool leRenderer_CullDrawXY(int32_t x, int32_t y)
{
    lePoint pt;

    pt.x = x;
    pt.y = y;

    return leRectContainsPoint(&_rendererState.drawRect, &pt) == LE_FALSE;
}

leBool leRenderer_CullDrawPoint(const lePoint* pt)
{
    return leRectContainsPoint(&_rendererState.drawRect, pt) == LE_FALSE;
}

void leRenderer_ClipDrawRect(const leRect* rect,
                             leRect* res)
{
    leRectClip(rect, &_rendererState.drawRect, res);
}

leResult leRenderer_Initialize(const gfxDisplayDriver* dispDriver)
{
    memset(&_rendererState, 0, sizeof(leRenderState));
 
    if(dispDriver == NULL ||
       dispDriver->getColorMode == NULL ||
       dispDriver->getBufferCount == NULL ||
       dispDriver->getDisplayWidth == NULL ||
       dispDriver->getDisplayHeight == NULL ||
       dispDriver->update == NULL ||
       dispDriver->blitBuffer == NULL ||
       dispDriver->swap == NULL ||
       dispDriver->getVSYNCCount == NULL)
    {
        return LE_FAILURE;
    }
 
    _rendererState.dispDriver = dispDriver;
    _rendererState.bufferCount = dispDriver->getBufferCount();
    
    _rendererState.displayRect.x = 0;
    _rendererState.displayRect.y = 0;
    _rendererState.displayRect.width = dispDriver->getDisplayWidth();
    _rendererState.displayRect.height = dispDriver->getDisplayHeight();
    
    leRectArray_Create(&_rendererState.prevDamageRects);
    leRectArray_Create(&_rendererState.currentDamageRects);
    leRectArray_Create(&_rendererState.pendingDamageRects);
    leRectArray_Create(&_rendererState.frameRectList);
    leRectArray_Create(&_rendererState.scratchRectList);
    
    _rendererState.frameState = LE_FRAME_READY;
    
    maxScratchPixels = SCRACH_BUFFER_SZ / leColorInfoTable[LE_GLOBAL_COLOR_MODE].size;
    
    //leRenderer_DamageArea(&lyr->widget.rect, LE_FALSE);
    
    return LE_SUCCESS;
}

void leRenderer_Shutdown()
{
    leRectArray_Destroy(&_rendererState.prevDamageRects);
    leRectArray_Destroy(&_rendererState.currentDamageRects);
    leRectArray_Destroy(&_rendererState.pendingDamageRects);
    leRectArray_Destroy(&_rendererState.frameRectList);
    leRectArray_Destroy(&_rendererState.scratchRectList);
}

static void addDamageRectToList(leRectArray* arr, const leRect* rect)
{
    uint32_t i;
    
    for(i = 0; i < arr->size; i++)
    {
        // nothing to do, damaged area is already covered by an existing rect
        if(leRectContainsRect(&arr->rects[i], rect) == LE_TRUE)
        {
            return;
        }
        // new rect completely envelopes old rect, just replace
        else if(leRectContainsRect(rect, &arr->rects[i]) == LE_TRUE)
        {
            arr->rects[i] = *rect;

            return;
        }
        // two rectangles are touching, combine the areas
        else if(leRectIntersects(rect, &arr->rects[i]) == LE_TRUE)
        {
            arr->rects[i] = leRectCombine(rect, &arr->rects[i]);

            return;
        }
    }

    leRectArray_PushBack(arr, rect);
}

static void addRectToFrameList(leRect* rect)
{
    uint32_t i;
#if LE_EFFICIENT_RECT_SLICING == 1
    uint32_t j, cnt;
    leRect rects[4];
#endif

    if(_rendererState.frameRectList.size > 0)
    {
        for(i = 0; i < _rendererState.frameRectList.size; i++)
        {
            // area is already covered by an existing rect
            if(leRectContainsRect(&_rendererState.frameRectList.rects[i], rect) == LE_TRUE)
            {
                return;
            }
#if LE_EFFICIENT_RECT_SLICING == 1
            // two rectangles are touching, split the incoming rectangle and
            // add pieces to list
            else if(leRectIntersects(rect, &_rendererState.frameRectList.rects[i]) == LE_TRUE)
            {
                cnt = leRectSplit(rect, &_rendererState.frameRectList.rects[i], rects);

                for(j = 0; j < cnt; j++)
                {
                    leRectArray_PushBack(&_rendererState.scratchRectList, &rects[j]);
                }
                
                return;
            }
#endif
        }
    }
    
    leRectArray_PushBack(&_rendererState.frameRectList, rect);
}

leResult leRenderer_DamageArea(const leRect* rect)
{
    leRect clipRect;
    
    // make sure rect is inside the layer
    if(leRectIntersects(&_rendererState.displayRect, rect) == LE_FALSE)
        return LE_FAILURE;    
    
    // clip the incoming rectangle
    leRectClip(&_rendererState.displayRect, rect, &clipRect);
        
    // initiate a new render frame if necessary
    if(_rendererState.frameState <= LE_FRAME_PREFRAME)
    {
        _rendererState.frameState = LE_FRAME_PREFRAME;
        
        // drawing not in progress, add the rectangle to the current list
        addDamageRectToList(&_rendererState.currentDamageRects, &clipRect);
    }
    else
    {
        //printf("added pending rect\n");
        
        // there is a frame in progress, add to the list that will be processed
        // next time around
        addDamageRectToList(&_rendererState.pendingDamageRects, &clipRect);
    }
    
    return LE_SUCCESS;
}

static leResult preFrame()
{
    uint32_t i;
    leRect rect;
    
    _rendererState.layerIdx = 0;
    
    leRectArray_Clear(&_rendererState.scratchRectList);
    leRectArray_Clear(&_rendererState.frameRectList);
    
    //printf("dump: %i\n", dump++);
    
    // merge rectangle lists
    if(_rendererState.bufferCount > 1)
    {
        for(i = 0; i < _rendererState.prevDamageRects.size; i++)
        {
            leRectArray_PushBack(&_rendererState.scratchRectList,
                                 &_rendererState.prevDamageRects.rects[i]);
        }
    }
    
    for(i = 0; i < _rendererState.currentDamageRects.size; i++)
    {
        leRectArray_PushBack(&_rendererState.scratchRectList,
                             &_rendererState.currentDamageRects.rects[i]);
    }
    
    // remove duplicate rectangles from combined list
    leRectArray_RemoveDuplicates(&_rendererState.scratchRectList);
    
    // combine any adjacent rectangles
    leRectArray_MergeSimilar(&_rendererState.scratchRectList);
    
    // remove overlapping space
    leRectArray_RemoveOverlapping(&_rendererState.scratchRectList);
    
    // crop to scratch buffer size limit
    leRectArray_CropToSize(&_rendererState.scratchRectList, maxScratchPixels);
    
    while(_rendererState.scratchRectList.size != 0)
    {
        rect = _rendererState.scratchRectList.rects[0];
        
        leRectArray_RemoveAt(&_rendererState.scratchRectList, 0);
        
        addRectToFrameList(&rect);
    }

#if LE_RENDER_LEFTRIGHT == 0
    // sort frame rects by Y
    leRectArray_SortByY(&_rendererState.frameRectList);
#else
    // sort frame rects by X
    leRectArray_SortByX(&_rendererState.frameRectList);
#endif

    _rendererState.frameRectIdx = 0;
    _rendererState.frameDrawCount = 0;
    _rendererState.renderBuffer = &renderBuffer;
    
    if(_rendererState.frameRectList.size == 0)
        return LE_FAILURE;
        
    _rendererState.frameState = LE_FRAME_PRELAYER;
    
    return LE_SUCCESS;
}

static void preLayer()
{
    if(_rendererState.dispDriver->setActiveLayer(_rendererState.layerIdx) != 0)
        return;
        
    _rendererState.frameState = LE_FRAME_PRERECT;
}

static void invalidateWidget(leWidget* wgt, leRect* rect)
{
    uint32_t idx;
    leRect localRect;
    
    if(wgt->visible == LE_FALSE)
    {
        wgt->fn->_validateChildren(wgt);

        return;
    }

    if(leRectIntersects(&wgt->rect, rect) == LE_TRUE)
    {
        wgt->fn->_setDirtyState(wgt, LE_WIDGET_DIRTY_STATE_DIRTY);
        
        for(idx = 0; idx < wgt->children.size; idx++)
        {
            localRect = *rect;
            
            localRect.x -= wgt->rect.x;
            localRect.y -= wgt->rect.y;
            
            invalidateWidget(wgt->children.values[idx], &localRect);
        }
    }
}

static void preRect()
{
    //uint32_t idx;
    
    if(_rendererState.frameRectIdx == _rendererState.frameRectList.size)
    {
        _rendererState.frameState = LE_FRAME_POSTFRAME;
        
        return;
    }
    
    // invalidate all widgets that intersect with the current
    // damaged rectangle
    //for(idx = 0; idx < leGetState()->rootWidgets.size; idx++)
    //{
        invalidateWidget(&leGetState()->rootWidget[_rendererState.layerIdx],
                         &_rendererState.frameRectList.rects[_rendererState.frameRectIdx]);
    //}
    
    // set up render buffer to match damaged rectangle size
    renderBuffer.size.width = _rendererState.frameRectList.rects[_rendererState.frameRectIdx].width;
    renderBuffer.size.height = _rendererState.frameRectList.rects[_rendererState.frameRectIdx].height;
    renderBuffer.pixel_count = renderBuffer.size.width * renderBuffer.size.height;
    renderBuffer.mode = LE_GLOBAL_COLOR_MODE;
    renderBuffer.pixels = &scratchBuffer;
    renderBuffer.buffer_length = renderBuffer.pixel_count * leColorInfoTable[renderBuffer.mode].size;
    renderBuffer.flags &= ~(BF_LOCKED); // clear any lock on the buffer
    
    _rendererState.frameState = LE_FRAME_PREWIDGET;
}

static leWidget* findDirtyWidget(leArray* widgetArray)
{
    uint32_t idx;
    leWidget *wgt, *child;
    
    if(widgetArray->size == 0)
        return NULL;
    
    for(idx = 0; idx < widgetArray->size; idx++)
    {
        wgt = widgetArray->values[idx];
        
        if(wgt->dirtyState == LE_WIDGET_DIRTY_STATE_DIRTY)
        {
            return wgt;
        }
        else if(wgt->dirtyState == LE_WIDGET_DIRTY_STATE_CHILD)
        {
            child = findDirtyWidget(&wgt->children);
            
            if(child != NULL)
            {            
                return child;
            }
            else
            {
                wgt->dirtyState = LE_WIDGET_DIRTY_STATE_CLEAN;
            }
        }
    }
    
    return NULL;
}

static void preWidget()
{
    // find a dirty widget to render
    _rendererState.currentWidget = findDirtyWidget(&leGetState()->rootWidget[_rendererState.layerIdx].children);
    
    if(_rendererState.currentWidget == NULL)
    {
        _rendererState.frameState = LE_FRAME_POSTRECT;
    }
    else
    {
        _rendererState.currentWidget->drawState = LE_WIDGET_DRAW_STATE_READY;
        _rendererState.frameState = LE_FRAME_DRAWING;
    }
}

#if 0
static void updateWidgetDirtyFlags(leWidget* widget)
{
    leWidget* child;
    leRect childRect;
    uint32_t i;

    widget->fn->_clearDirtyState(widget);
    
    for(i = 0; i < widget->children.size; i++)
    {
        child = widget->children.values[i];
        
        childRect = child->rect;
        childRect.x += widget->rect.x;
        childRect.y += widget->rect.y;

        // pre-cull based on visibility and rectangle intersection
        // anything not visible or completely outside parent can be ignored.
        if(child->visible == LE_TRUE &&
           child->dirtyState != LE_WIDGET_DIRTY_STATE_CLEAN &&
           leRectIntersects(&widget->rect, &childRect) == LE_TRUE)
        {
            widget->fn->_increaseDirtyState(widget, LE_WIDGET_DIRTY_STATE_CHILD);
        }
        else
        {            
            // clear all dirty flags for this child and its descendents
            child->fn->_validateChildren(child);
        }
    }
}
#endif

int debug = 0;

static leBool paintWidget(leWidget* widget)
{
    leRect widgetRect, parentRect, clipRect;
    leBool shouldPaint = LE_FALSE;
    
#if LE_PREEMPTION_LEVEL >= 1
    leBool painted = LE_FALSE;
#endif
    
    // skip any child that isn't dirty or that does not have a dirty descendent
    if(widget->dirtyState == LE_WIDGET_DIRTY_STATE_CLEAN)
        return LE_TRUE;
        
    // if widget is completely transparent just mark clean and return
#if LE_ALPHA_BLENDING_ENABLED == 1
    if(widget->fn->getCumulativeAlphaEnabled(widget) == LE_TRUE &&
       widget->fn->getCumulativeAlphaAmount(widget) == 0)
    {
        widget->fn->_validateChildren(widget);
        
        return LE_TRUE;
    }
#endif
    
    if(widget->dirtyState == LE_WIDGET_DIRTY_STATE_DIRTY)
    {
        shouldPaint = LE_TRUE;
    }
    
    // paint this widget and clear dirty flag
    if(shouldPaint == LE_TRUE)
    {   
        // clip the damage rectangle to the child's parent
        if(widget->parent != NULL)
        {
            widgetRect = widget->fn->rectToScreen(widget);
            parentRect = widget->parent->fn->rectToScreen(widget->parent);
            
            // child does not intersect parent at all, do not draw
            if(leRectIntersects(&widgetRect, &parentRect) == LE_FALSE)
            {
                widget->fn->_validateChildren(widget);
        
                return LE_TRUE;
            }
            
            // get the delta area between the parent and child
            leRectClip(&widgetRect, &parentRect, &clipRect);
            
            // widget visible area does not intersect dirty area at all
            // do not draw
            if(leRectIntersects(&clipRect,
                                &_rendererState.frameRectList.rects[_rendererState.frameRectIdx]) == LE_FALSE)
            {
                widget->fn->_validateChildren(widget);
        
                return LE_TRUE;
            }
            
            // get the delta area between the dirty area and the child/parent
            // delta area
            leRectClip(&_rendererState.frameRectList.rects[_rendererState.frameRectIdx],
                       &clipRect,
                       &_rendererState.drawRect);
        }
        else
        {
            //layer->dmgRectPtr = &layer->currentRect;
            _rendererState.drawRect = _rendererState.frameRectList.rects[_rendererState.frameRectIdx];
        }
        
        // turn off clipping by default
        //GFX_Set(GFXF_DRAW_CLIP_ENABLE, GFX_FALSE);
        
        //if(debug < 50)
        //{
        //printf("%u\n", widget->id);
        
        widget->fn->_handleEvent(widget, &paintEvt);
        widget->drawCount++;
        
            debug++;
        //}
#if LE_PREEMPTION_LEVEL >= 1        
        painted = LE_TRUE;
#endif 
        
        //printf("painting widget - %i, %i\n", widget->type, widget->id);
        
        if(widget->drawState != LE_WIDGET_DRAW_STATE_DONE)
            return LE_FALSE;
            
        _rendererState.frameDrawCount++;
    }
    
    if(widget->children.size > 0)
    {
        widget->dirtyState = LE_WIDGET_DIRTY_STATE_CHILD;
    }
    else
    {
        widget->dirtyState = LE_WIDGET_DIRTY_STATE_CLEAN;
    }
    
    //widget->fn->_clearDirtyState(widget);
    
    // update widget dirty flags
    //updateWidgetDirtyFlags(widget);
    
#if LE_PREEMPTION_LEVEL >= 1
    // preempt if necessary
    if(painted == LE_TRUE)
        return LE_FAILURE;
#endif
        
    if(widget->children.size == 0)
        return LE_TRUE;
    
    // draw children
    /*for(i = 0; i < widget->children.size; i++)
    {
        child = widget->children.values[i];

        if(child->dirtyState != LE_WIDGET_DIRTY_STATE_CLEAN)
        {
            if(paintWidget(child) == LE_FAILURE)
                return LE_FALSE;
        }
    }
    
    // update widget dirty flags (should come out clean)
    updateWidgetDirtyFlags(widget);*/
    
    return LE_TRUE;
}

static void _nextRect()
{
    _rendererState.frameRectIdx += 1;

    if(_rendererState.frameRectIdx < _rendererState.frameRectList.size)
    {
        _rendererState.frameState = LE_FRAME_PRERECT;
    }
    else
    {
        _rendererState.frameState = LE_FRAME_POSTLAYER;
    }
}

static leResult postRect()
{
    leRect frameRect = _rendererState.frameRectList.rects[_rendererState.frameRectIdx];

    /* render buffer may be locked by something or display driver may not be ready */
    if(_rendererState.dispDriver->blitBuffer(frameRect.x, frameRect.y, (gfxPixelBuffer*)&renderBuffer, GFX_BLEND_NONE) == GFX_FAILURE)
    {
        return LE_FAILURE;
    }

    if(lePixelBuffer_IsLocked(&renderBuffer) == LE_TRUE)
    {
        _rendererState.frameState = LE_FRAME_WAITFORBUFFER;

        return LE_SUCCESS;
    }

    _nextRect();
    
    return LE_SUCCESS;
}

static void postLayer()
{
    _rendererState.layerIdx += 1;
    
    if(_rendererState.layerIdx < LE_LAYER_COUNT)
    {
        _rendererState.frameState = LE_FRAME_PRELAYER;
    }
    else
    {
        _rendererState.frameState = LE_FRAME_POSTFRAME;
    }
}

static void postFrame()
{
    _rendererState.frameDrawCount = 0;
    
    // move current rects to previous list
    if(_rendererState.bufferCount > 1)
    {
        leRectArray_Copy(&_rendererState.currentDamageRects, &_rendererState.prevDamageRects);
    }
    
    // need to automatically fill the back buffer the first time this layer
    // draws
    //if(_rendererState.drawCount == 0 && _rendererState.bufferCount > 1)
    //{
    //    leRectArray_PushBack(&_rendererState.pendingDamageRects, &layer->widget.rect);
    //}
    
    //After the back buffer has been tagged for fill, clear the prevDamaged 
    //rects. This avoids a full redraw of the frame later
    if (_rendererState.drawCount == 1 && _rendererState.bufferCount > 1)
    {
        if(_rendererState.prevDamageRects.size > MAX_RECTARRAYS_SZ)
        {
            leRectArray_Destroy(&_rendererState.prevDamageRects);
        }
        else
        {
            leRectArray_Clear(&_rendererState.prevDamageRects);
        }
    }
    
    _rendererState.drawCount++;
    
    // manage the layer's rectangle arrays            
    if(_rendererState.pendingDamageRects.size > 0)
    {
        leRectArray_Copy(&_rendererState.pendingDamageRects, 
                         &_rendererState.currentDamageRects);

        if(_rendererState.pendingDamageRects.size > MAX_RECTARRAYS_SZ)
        {
            leRectArray_Destroy(&_rendererState.pendingDamageRects);
        }
        else
        {
            leRectArray_Clear(&_rendererState.pendingDamageRects);
        }

        _rendererState.frameState = LE_FRAME_PREFRAME;
    }
    else
    {
        if(_rendererState.currentDamageRects.size > MAX_RECTARRAYS_SZ)
        {
            leRectArray_Destroy(&_rendererState.currentDamageRects);
        }
        else
        {
            leRectArray_Clear(&_rendererState.currentDamageRects);
        }

        _rendererState.frameState = LE_FRAME_READY;
    }
    
    /*GFX_SetDebugRect(layer->currentDrawingRect.x,
                     layer->currentDrawingRect.y,
                     layer->currentDrawingRect.width,
                     layer->currentDrawingRect.height);
                     
    GFX_SetDebugRectColor(0x0000FF);
    GFX_ShowDebugRect(GFX_TRUE);
    GFX_NextDebugRect();*/
}
						 
extern int debugFlag;

void leRenderer_Paint()
{
#if LE_PREEMPTION_LEVEL == 0

    while(_rendererState.frameState != LE_FRAME_READY)
#endif
    {
        switch(_rendererState.frameState)
        {
            case LE_FRAME_PREFRAME:
            {
                preFrame();
                
                break;
            }
            case LE_FRAME_PRELAYER:
            {
                preLayer();
                
                break;
            }
            case LE_FRAME_PRERECT:
            {
                preRect();
                
                break;
            }
            case LE_FRAME_PREWIDGET:
            {
                preWidget();
                
                break;
            }
            case LE_FRAME_DRAWING:
            {
                if(paintWidget(_rendererState.currentWidget) == LE_TRUE)
                {
                    _rendererState.frameState = LE_FRAME_POSTWIDGET;
                }
                else
                {
                    return;
                }
                
                break;
            }
            case LE_FRAME_POSTWIDGET:
            {
                _rendererState.frameState = LE_FRAME_PREWIDGET;
                
                break;
            }
            case LE_FRAME_POSTRECT:
            {
                /* may need to preempt to give some time to the rest of the system */
                if(postRect() == LE_FAILURE)
                    return;
                
                break;
            }
            case LE_FRAME_WAITFORBUFFER:
            {
                /* may need to preempt to give some time to the rest of the system */
                if(lePixelBuffer_IsLocked(&renderBuffer) == LE_TRUE)
                    return;

                _nextRect();

                break;
            }
            case LE_FRAME_POSTLAYER:
            {
                postLayer();
                
                break;
            }
            case LE_FRAME_POSTFRAME:
            {
                _rendererState.dispDriver->swap();
                
                postFrame();
                
                //if(_rendererState.pendingDamageRects.size > 0)
                //{
                //    _rendererState.frameState = LE_FRAME_PREFRAME;
                //}
                
                break;
            }
            default:
            {
                break;
            }
        }
#if LE_PREEMPTION_LEVEL == 0
    }
#endif    
}
