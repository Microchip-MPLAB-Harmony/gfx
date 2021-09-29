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

static leState* _state;
leRenderState _rendererState;

#define SCRATCH_BUFFER_SZ     (LE_SCRATCH_BUFFER_SIZE_KB * 1024)
#define MAX_RECTARRAYS_SZ    8

#ifndef LE_NO_CACHE_ATTR
#define LE_NO_CACHE_ATTR
#endif

#ifndef __ALIGNED
#define __ALIGNED(val)
#endif

void _leRenderer_InitDrawForMode(leColorMode mode);

static uint8_t LE_COHERENT_ATTR LE_NO_CACHE_ATTR __ALIGNED(64) _dataBuffers[SCRATCH_BUFFER_SZ];

struct leScratchBuffer
{
    lePixelBuffer renderBuffer;
    gfxPixelBuffer gfxBuffer;
};

static uint32_t maxScratchPixels;

static LE_COHERENT_ATTR struct leScratchBuffer _scratchBuffers[LE_SCRATCH_BUFFER_COUNT];

static leEvent paintEvt = { LE_WIDGET_EVENT_PAINT };

static gfxColorMode _convertColorMode(leColorMode mode)
{
    switch(mode)
    {
        case LE_COLOR_MODE_INDEX_1:   return GFX_COLOR_MODE_INDEX_1;
        case LE_COLOR_MODE_INDEX_4:   return GFX_COLOR_MODE_INDEX_4;
        case LE_COLOR_MODE_INDEX_8:   return GFX_COLOR_MODE_INDEX_8;
        case LE_COLOR_MODE_GS_8:      return GFX_COLOR_MODE_GS_8;
        case LE_COLOR_MODE_RGB_332:   return GFX_COLOR_MODE_RGB_332;
        case LE_COLOR_MODE_RGB_565:   return GFX_COLOR_MODE_RGB_565;
        case LE_COLOR_MODE_RGBA_5551: return GFX_COLOR_MODE_RGBA_5551;
        case LE_COLOR_MODE_RGB_888:   return GFX_COLOR_MODE_RGB_888;
        case LE_COLOR_MODE_RGBA_8888: return GFX_COLOR_MODE_RGBA_8888;
        case LE_COLOR_MODE_ARGB_8888: return GFX_COLOR_MODE_ARGB_8888;
        case LE_COLOR_MODE_MONOCHROME: return GFX_COLOR_MODE_MONOCHROME;
    }

    return 0;
}

leRenderState* leGetRenderState()
{
    return &_rendererState; 
}

lePixelBuffer* leGetRenderBuffer(void)
{
    if(_rendererState.currentScratchBuffer == -1)
        return NULL;

    return &_scratchBuffers[_rendererState.currentScratchBuffer].renderBuffer;
}

leColorMode leRenderer_CurrentColorMode()
{
    return _scratchBuffers[_rendererState.currentScratchBuffer].renderBuffer.mode;
}

lePalette* leRenderer_GetGlobalPalette()
{
    return _rendererState.globalPalette;
}

leResult leRenderer_SetGlobalPalette(lePalette* pal)
{
    _rendererState.globalPalette = pal;

    return LE_SUCCESS;
}

leColor leRenderer_GlobalPaletteLookup(uint32_t idx)
{
    return lePalette_GetColor(_rendererState.globalPalette, idx);
}

leColor leRenderer_ConvertColor(leColor inColor, leColorMode inMode)
{
    return leColorConvert(inMode, leRenderer_CurrentColorMode(), inColor);
}

/*leRect leRenderer_GetDisplayRect()
{
    return _rendererState.displayRect;
}*/

void leRenderer_GetDrawRect(leRect* rect)
{
    *rect = _rendererState.drawRect;
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

leResult leRenderer_Initialize(const gfxDisplayDriver* dispDriver,
                               const gfxGraphicsProcessor* gpuDriver)
{
    uint32_t itr;
    gfxIOCTLArg_Value val;

    _state = leGetState();

    memset(&_rendererState, 0, sizeof(leRenderState));
 
    if(dispDriver == NULL ||
       dispDriver->update == NULL ||
       dispDriver->blitBuffer == NULL ||
       dispDriver->ioctl == NULL)
    {
        return LE_FAILURE;
    }

    _rendererState.dispDriver = dispDriver;
    _rendererState.gpuDriver = gpuDriver;

    val.value.v_uint = 0;
    dispDriver->ioctl(GFX_IOCTL_GET_BUFFER_COUNT, &val);

    _rendererState.bufferCount = val.value.v_uint;

    for(itr = 0; itr < LE_LAYER_COUNT; ++itr)
    {
        leRectArray_Create(&_rendererState.layerStates[itr].prevDamageRects);
        leRectArray_Create(&_rendererState.layerStates[itr].currentDamageRects);
        leRectArray_Create(&_rendererState.layerStates[itr].pendingDamageRects);
        leRectArray_Create(&_rendererState.layerStates[itr].frameRectList);
        leRectArray_Create(&_rendererState.layerStates[itr].scratchRectList);
    }

    _rendererState.frameState = LE_FRAME_READY;
    
    return LE_SUCCESS;
}

void leRenderer_Shutdown()
{
    uint32_t itr;

    for(itr = 0; itr < LE_LAYER_COUNT; ++itr)
    {
        leRectArray_Destroy(&_rendererState.layerStates[itr].prevDamageRects);
        leRectArray_Destroy(&_rendererState.layerStates[itr].currentDamageRects);
        leRectArray_Destroy(&_rendererState.layerStates[itr].pendingDamageRects);
        leRectArray_Destroy(&_rendererState.layerStates[itr].frameRectList);
        leRectArray_Destroy(&_rendererState.layerStates[itr].scratchRectList);
    }
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
            leRectCombine(rect,
                          &arr->rects[i],
                          &arr->rects[i]);

            return;
        }
    }

    leRectArray_PushBack(arr, rect);
}

static void addRectToFrameList(leRect* rect)
{
    uint32_t i;
#if 0
#if LE_EFFICIENT_RECT_SLICING == 1
    uint32_t j, cnt;
    leRect rects[4];
#endif
#endif

    if(_rendererState.layerStates[_rendererState.layerIdx].frameRectList.size > 0)
    {
        for(i = 0; i < _rendererState.layerStates[_rendererState.layerIdx].frameRectList.size; i++)
        {
            // area is already covered by an existing rect
            if(leRectContainsRect(&_rendererState.layerStates[_rendererState.layerIdx].frameRectList.rects[i], rect) == LE_TRUE)
            {
                return;
            }
#if 0
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
#endif
        }
    }
    
    leRectArray_PushBack(&_rendererState.layerStates[_rendererState.layerIdx].frameRectList, rect);
}

leResult leRenderer_DamageArea(const leRect* rect,
                               uint32_t layerIdx)
{
    leRect clipRect;

    if(layerIdx >= LE_LAYER_COUNT)
        return LE_FAILURE;
    
    // make sure rect is inside the layer
    if(leRectIntersects(&_state->rootWidget[layerIdx].rect, rect) == LE_FALSE)
        return LE_FAILURE;
    
    // clip the incoming rectangle
    leRectClip(&_state->rootWidget[layerIdx].rect, rect, &clipRect);
        
    // initiate a new render frame if necessary
    if(_rendererState.frameState <= LE_FRAME_PREFRAME)
    {
        _rendererState.frameState = LE_FRAME_PREFRAME;

        // drawing not in progress, add the rectangle to the current list
        addDamageRectToList(&_rendererState.layerStates[layerIdx].currentDamageRects, &clipRect);
    }
    else
    {
        //printf("added pending rect\n");
        
        // there is a frame in progress, add to the list that will be processed
        // next time around
        addDamageRectToList(&_rendererState.layerStates[layerIdx].pendingDamageRects, &clipRect);
    }
    
    return LE_SUCCESS;
}

static leResult preFrame(void)
{
    _rendererState.layerIdx = 0;

    /*if(_rendererState.renderBuffer != NULL)
    {
        _rendererState.renderBuffer->mode = -1;
    }*/

    _rendererState.frameState = LE_FRAME_PRELAYER;
    
    return LE_SUCCESS;
}

static void preLayer(void)
{
    uint32_t i;
    leRect rect;

#if LE_SCRATCH_BUFFER_PADDING == 1
    uint32_t padSize = 0;
#endif

    //printf("dump: %i\n", dump++);

    leRectArray_Clear(&_rendererState.layerStates[_rendererState.layerIdx].scratchRectList);
    leRectArray_Clear(&_rendererState.layerStates[_rendererState.layerIdx].frameRectList);

    maxScratchPixels = SCRATCH_BUFFER_SZ / leColorInfoTable[leGetLayerColorMode(_rendererState.layerIdx)].size;

    // merge rectangle lists
    if(_rendererState.bufferCount > 1)
    {
        for(i = 0; i < _rendererState.layerStates[_rendererState.layerIdx].prevDamageRects.size; i++)
        {
            leRectArray_PushBack(&_rendererState.layerStates[_rendererState.layerIdx].scratchRectList,
                                 &_rendererState.layerStates[_rendererState.layerIdx].prevDamageRects.rects[i]);
        }
    }

    for(i = 0; i < _rendererState.layerStates[_rendererState.layerIdx].currentDamageRects.size; i++)
    {
        leRectArray_PushBack(&_rendererState.layerStates[_rendererState.layerIdx].scratchRectList,
                             &_rendererState.layerStates[_rendererState.layerIdx].currentDamageRects.rects[i]);
    }

    if(_rendererState.layerStates[_rendererState.layerIdx].scratchRectList.size == 0)
    {
        _rendererState.frameState = LE_FRAME_POSTLAYER;

        return;
    }

    // remove duplicate rectangles from combined list
    leRectArray_RemoveDuplicates(&_rendererState.layerStates[_rendererState.layerIdx].scratchRectList);

    // combine any adjacent rectangles
    leRectArray_MergeSimilar(&_rendererState.layerStates[_rendererState.layerIdx].scratchRectList);

    // remove overlapping space
    leRectArray_RemoveOverlapping(&_rendererState.layerStates[_rendererState.layerIdx].scratchRectList);

    // crop to scratch buffer size limit
    if(leGetLayerRenderHorizontal(_rendererState.layerIdx) == LE_TRUE)
    {
        // sort frame rects by X
        leRectArray_CropToSizeX(&_rendererState.layerStates[_rendererState.layerIdx].scratchRectList, maxScratchPixels);
    }
    else
    {
        // sort frame rects by Y
        leRectArray_CropToSizeY(&_rendererState.layerStates[_rendererState.layerIdx].scratchRectList, maxScratchPixels);
    }

#if LE_SCRATCH_BUFFER_PADDING == 1
    padSize = 4;

    if(leGetLayerColorMode(_rendererState.layerIdx) == LE_COLOR_MODE_RGB_565)
    {
        padSize = 8;
    }

#if LE_RENDER_ORIENTATION == 90 || LE_RENDER_ORIENTATION == 270
    leRectArray_PadRectangleHeight(&_rendererState.layerStates[_rendererState.layerIdx].scratchRectList,
                                   padSize);
#else
    leRectArray_PadRectangleWidth(&_rendererState.layerStates[_rendererState.layerIdx].scratchRectList,
                                  padSize);
#endif
#endif

    while(_rendererState.layerStates[_rendererState.layerIdx].scratchRectList.size != 0)
    {
        rect = _rendererState.layerStates[_rendererState.layerIdx].scratchRectList.rects[0];

        leRectArray_RemoveAt(&_rendererState.layerStates[_rendererState.layerIdx].scratchRectList, 0);

        addRectToFrameList(&rect);
    }

    if(leGetLayerRenderHorizontal(_rendererState.layerIdx) == LE_TRUE)
    {
        // sort frame rects by X
        leRectArray_SortByX(&_rendererState.layerStates[_rendererState.layerIdx].frameRectList);
    }
    else
    {
        // sort frame rects by Y
        leRectArray_SortByY(&_rendererState.layerStates[_rendererState.layerIdx].frameRectList);
    }

    _rendererState.frameRectIdx = 0;
    _rendererState.frameDrawCount = 0;

    if(_rendererState.layerStates[_rendererState.layerIdx].frameRectList.size == 0)
    {
        _rendererState.frameState = LE_FRAME_POSTLAYER;
    }
    else
    {
        _leRenderer_InitDrawForMode(leGetLayerColorMode(_rendererState.layerIdx));

        _rendererState.frameState = LE_FRAME_PRERECT;
    }
}

static void invalidateWidget(leWidget* wgt, leRect* rect)
{
    uint32_t idx;
    leRect localRect;
    leRect clipRect;
    
    if(LE_TEST_FLAG(wgt->flags, LE_WIDGET_VISIBLE) == LE_FALSE)
    {
        wgt->fn->_validateChildren(wgt);

        return;
    }

    localRect = wgt->rect;

    if(leRectIntersects(&localRect, rect) == LE_TRUE)
    {
        wgt->fn->_setDirtyState(wgt, LE_WIDGET_DIRTY_STATE_DIRTY);
        
        for(idx = 0; idx < wgt->children.size; idx++)
        {
            leRectClip(&localRect, rect, &clipRect);

            clipRect.x -= wgt->rect.x;
            clipRect.y -= wgt->rect.y;
            
            invalidateWidget(wgt->children.values[idx], &clipRect);
        }
    }
    else
    {
        wgt->fn->_validateChildren(wgt);
    }
}

static void preRect(void)
{
    int32_t idx;
    uint32_t width, height;
    struct leScratchBuffer* buf = NULL;

    if(_rendererState.frameRectIdx == _rendererState.layerStates[_rendererState.layerIdx].frameRectList.size)
    {
        _rendererState.frameState = LE_FRAME_POSTFRAME;
        
        return;
    }

    _rendererState.currentScratchBuffer = -1;

    for(idx = 0; idx < LE_SCRATCH_BUFFER_COUNT; ++idx)
    {
        if(gfxPixelBuffer_IsLocked(&_scratchBuffers[idx].gfxBuffer) == GFX_FALSE)
        {
            _rendererState.currentScratchBuffer = idx;
            buf = &_scratchBuffers[idx];

            break;
        }
    }

    if(_rendererState.currentScratchBuffer == -1)
        return;

    invalidateWidget(&leGetState()->rootWidget[_rendererState.layerIdx],
                     &_rendererState.layerStates[_rendererState.layerIdx].frameRectList.rects[_rendererState.frameRectIdx]);

    // set up render buffer to match damaged rectangle size
#if LE_RENDER_ORIENTATION == 0 || LE_RENDER_ORIENTATION == 180
    width = _rendererState.layerStates[_rendererState.layerIdx].frameRectList.rects[_rendererState.frameRectIdx].width;
    height = _rendererState.layerStates[_rendererState.layerIdx].frameRectList.rects[_rendererState.frameRectIdx].height;
#else
    width = _rendererState.layerStates[_rendererState.layerIdx].frameRectList.rects[_rendererState.frameRectIdx].height;
    height = _rendererState.layerStates[_rendererState.layerIdx].frameRectList.rects[_rendererState.frameRectIdx].width;
#endif

    lePixelBufferCreate(width,
                        height,
                        leGetLayerColorMode(_rendererState.layerIdx),
                        &_dataBuffers[idx],
                        &buf->renderBuffer);

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
        
        if(wgt->status.dirtyState == LE_WIDGET_DIRTY_STATE_DIRTY)
        {
            return wgt;
        }
        else if(wgt->status.dirtyState == LE_WIDGET_DIRTY_STATE_CHILD)
        {
            child = findDirtyWidget(&wgt->children);
            
            if(child != NULL)
            {            
                return child;
            }
            else
            {
                wgt->status.dirtyState = LE_WIDGET_DIRTY_STATE_CLEAN;
            }
        }
    }
    
    return NULL;
}

static void preWidget(void)
{
    // find a dirty widget to render
    _rendererState.currentWidget = findDirtyWidget(&leGetState()->rootWidget[_rendererState.layerIdx].children);
    
    if(_rendererState.currentWidget == NULL)
    {
        _rendererState.frameState = LE_FRAME_POSTRECT;
    }
    else
    {
        _rendererState.currentWidget->status.drawState = LE_WIDGET_DRAW_STATE_READY;
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
            // clear all dirty flags for this child and its descendants
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
    
    // skip any child that isn't dirty or that does not have a dirty descendant
    if(widget->status.dirtyState == LE_WIDGET_DIRTY_STATE_CLEAN)
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
    
    if(widget->status.dirtyState == LE_WIDGET_DIRTY_STATE_DIRTY)
    {
        shouldPaint = LE_TRUE;
    }
    
    // paint this widget and clear dirty flag
    if(shouldPaint == LE_TRUE)
    {   
        // clip the damage rectangle to the child's parent
        if(widget->parent != NULL)
        {
            widget->fn->rectToScreen(widget, &widgetRect);
            widget->parent->fn->rectToScreen(widget->parent, &parentRect);
            
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
                                &_rendererState.layerStates[_rendererState.layerIdx].frameRectList.rects[_rendererState.frameRectIdx]) == LE_FALSE)
            {
                widget->fn->_validateChildren(widget);
        
                return LE_TRUE;
            }
            
            // get the delta area between the dirty area and the child/parent
            // delta area
            leRectClip(&_rendererState.layerStates[_rendererState.layerIdx].frameRectList.rects[_rendererState.frameRectIdx],
                       &clipRect,
                       &_rendererState.drawRect);
        }
        else
        {
            //layer->dmgRectPtr = &layer->currentRect;
            _rendererState.drawRect = _rendererState.layerStates[_rendererState.layerIdx].frameRectList.rects[_rendererState.frameRectIdx];
        }

        widget->fn->_handleEvent(widget, &paintEvt);
        widget->drawCount++;
        
#if LE_PREEMPTION_LEVEL >= 1
        painted = LE_TRUE;
#endif 
        
        //printf("painting widget - %i, %i\n", widget->type, widget->id);
        
        if(widget->status.drawState != LE_WIDGET_DRAW_STATE_DONE)
            return LE_FALSE;
            
        _rendererState.frameDrawCount++;
    }
    
    if(widget->children.size > 0)
    {
        widget->status.dirtyState = LE_WIDGET_DIRTY_STATE_CHILD;
    }
    else
    {
        widget->status.dirtyState = LE_WIDGET_DIRTY_STATE_CLEAN;
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

static void _nextRect(void)
{
    _rendererState.frameRectIdx += 1;

    if(_rendererState.frameRectIdx < _rendererState.layerStates[_rendererState.layerIdx].frameRectList.size)
    {
        _rendererState.frameState = LE_FRAME_PRERECT;
    }
    else
    {
        _rendererState.frameState = LE_FRAME_POSTLAYER;
    }
}

static leResult postRect(void)
{
    int32_t rotX, rotY;

    leRect frameRect = _rendererState.layerStates[_rendererState.layerIdx].frameRectList.rects[_rendererState.frameRectIdx];

#if LE_RENDER_ORIENTATION == 0
    rotX = frameRect.x;
    rotY = frameRect.y;
#elif LE_RENDER_ORIENTATION == 90
    rotY = frameRect.x;
    rotX = _state->rootWidget[_rendererState.layerIdx].rect.height - frameRect.y - frameRect.height;
#elif LE_RENDER_ORIENTATION == 180
    rotX = _state->rootWidget[_rendererState.layerIdx].rect.width - frameRect.x - frameRect.width;
    rotY = _state->rootWidget[_rendererState.layerIdx].rect.height - frameRect.y - frameRect.height;
#elif LE_RENDER_ORIENTATION == 270
    rotX = frameRect.y;
    rotY = _state->rootWidget[_rendererState.layerIdx].rect.width - frameRect.x - frameRect.width;
#endif

    _scratchBuffers[_rendererState.currentScratchBuffer].gfxBuffer.pixel_count = _scratchBuffers[_rendererState.currentScratchBuffer].renderBuffer.pixel_count;
    _scratchBuffers[_rendererState.currentScratchBuffer].gfxBuffer.size.width = _scratchBuffers[_rendererState.currentScratchBuffer].renderBuffer.size.width;
    _scratchBuffers[_rendererState.currentScratchBuffer].gfxBuffer.size.height = _scratchBuffers[_rendererState.currentScratchBuffer].renderBuffer.size.height;
    _scratchBuffers[_rendererState.currentScratchBuffer].gfxBuffer.mode = _convertColorMode(_scratchBuffers[_rendererState.currentScratchBuffer].renderBuffer.mode);
    _scratchBuffers[_rendererState.currentScratchBuffer].gfxBuffer.buffer_length = _scratchBuffers[_rendererState.currentScratchBuffer].renderBuffer.buffer_length;
    _scratchBuffers[_rendererState.currentScratchBuffer].gfxBuffer.flags = 0;
    _scratchBuffers[_rendererState.currentScratchBuffer].gfxBuffer.pixels = (gfxBuffer)_scratchBuffers[_rendererState.currentScratchBuffer].renderBuffer.pixels;

    /* render buffer may be locked by something or display driver may not be ready */
    if(_rendererState.dispDriver->blitBuffer(rotX,
                                             rotY,
                                             &_scratchBuffers[_rendererState.currentScratchBuffer].gfxBuffer) == GFX_FAILURE)
    {
        return LE_FAILURE;
    }

    _rendererState.currentScratchBuffer = -1;

    /*if(lePixelBuffer_IsLocked(&_scratchBuffers[_rendererState.currentScratchBuffer].renderBuffer) == LE_TRUE)
    {
        _rendererState.frameState = LE_FRAME_WAITFORBUFFER;

        return LE_SUCCESS;
    }*/

    _nextRect();
    
    return LE_SUCCESS;
}

static void postLayer(void)
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

static void postFrame(void)
{
    uint32_t itr;

    _rendererState.frameDrawCount = 0;

    _rendererState.drawCount++;
    
    // manage the layer's rectangle arrays
    for(itr = 0; itr < LE_LAYER_COUNT; ++itr)
    {
        // double buffering support - move current rects to previous list
        if(_rendererState.bufferCount > 1)
        {
            leRectArray_Copy(&_rendererState.layerStates[itr].currentDamageRects,
                             &_rendererState.layerStates[itr].prevDamageRects);

            if(_rendererState.layerStates[itr].prevDamageRects.size > MAX_RECTARRAYS_SZ)
            {
                leRectArray_Destroy(&_rendererState.layerStates[itr].prevDamageRects);
            }
            else
            {
                leRectArray_Clear(&_rendererState.layerStates[itr].prevDamageRects);
            }
        }

        if(_rendererState.layerStates[itr].pendingDamageRects.size > 0)
        {
            leRectArray_Copy(&_rendererState.layerStates[itr].pendingDamageRects,
                             &_rendererState.layerStates[itr].currentDamageRects);

            if(_rendererState.layerStates[itr].pendingDamageRects.size > MAX_RECTARRAYS_SZ)
            {
                leRectArray_Destroy(&_rendererState.layerStates[itr].pendingDamageRects);
            }
            else
            {
                leRectArray_Clear(&_rendererState.layerStates[itr].pendingDamageRects);
            }

            _rendererState.frameState = LE_FRAME_PREFRAME;
        }
        else
        {
            if(_rendererState.layerStates[itr].currentDamageRects.size > MAX_RECTARRAYS_SZ)
            {
                leRectArray_Destroy(&_rendererState.layerStates[itr].currentDamageRects);
            }
            else
            {
                leRectArray_Clear(&_rendererState.layerStates[itr].currentDamageRects);
            }

            _rendererState.frameState = LE_FRAME_READY;
        }
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
    {
#endif
        switch(_rendererState.frameState)
        {
            case LE_FRAME_PREFRAME:
            {
                preFrame();

                _rendererState.val.value.v_uint = 0;
                _rendererState.dispDriver->ioctl(GFX_IOCTL_FRAME_START, &_rendererState.val);
                
                break;
            }
            case LE_FRAME_PRELAYER:
            {
                _rendererState.val.value.v_uint = _rendererState.layerIdx;

				if(_rendererState.dispDriver->ioctl(GFX_IOCTL_SET_ACTIVE_LAYER, &_rendererState.val) >= GFX_IOCTL_ERROR_UNKNOWN)
				{
					_rendererState.frameState = LE_FRAME_POSTLAYER;
				}
				else
				{
					preLayer();
				}
                
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
                //if(lePixelBuffer_IsLocked(&renderBuffer) == LE_TRUE)
                //    return;

                _nextRect();

                break;
            }
            case LE_FRAME_POSTLAYER:
            {
                postLayer();
                
                _rendererState.dispDriver->ioctl(GFX_IOCTL_LAYER_SWAP, NULL);
                
                break;
            }
            case LE_FRAME_POSTFRAME:
            {
                _rendererState.dispDriver->ioctl(GFX_IOCTL_FRAME_END, NULL);
                
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
