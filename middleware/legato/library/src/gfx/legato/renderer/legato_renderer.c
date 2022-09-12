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

#if LE_DEBUG_RENDERER == 0

#include <string.h>

#include "gfx/legato/common/legato_rect.h"
#include "gfx/legato/common/legato_pixelbuffer.h"
#include "gfx/legato/common/legato_utils.h"
#include "gfx/legato/core/legato_state.h"
#include "gfx/legato/datastructure/legato_rectarray.h"
#include "gfx/legato/memory/legato_memory.h"
#include "gfx/legato/renderer/legato_gpu.h"
#include "gfx/driver/gfx_driver.h"

static leState* _state;

/**
 * @brief This enum represents state of the renderer.
 * @details This enum type describes the state at which a
 * widget is being drawn.
 */
typedef enum leFrameState
{
    LE_FRAME_READY = 0,
    LE_FRAME_PREFRAME,
    LE_FRAME_PRELAYER,
    LE_FRAME_PRERECT,
    LE_FRAME_PREWIDGET,
    LE_FRAME_DRAWING,
    LE_FRAME_POSTWIDGET,
    LE_FRAME_POSTRECT,
    LE_FRAME_WAITFORBUFFER,
    LE_FRAME_POSTLAYER,
    LE_FRAME_POSTFRAME,
} leFrameState;

typedef struct leRenderLayerState
{
    leRectArray prevDamageRects;    // previous damaged rectangle list
    leRectArray currentDamageRects; // queued damaged rectangle list
    leRectArray pendingDamageRects; // pending damaged rectangle list
                                    // these are rectangles added during
                                    // a frame in progress

    leRectArray scratchRectList; // used for rectangle culling phase
    leRectArray frameRectList;   // rects to draw for a frame

    leBool drawingPrev;           // indicates if the layer is currently
                                  // drawing from its previous rectangle
                                  // array
} leRenderLayerState;

/**
 * @brief This structs represents global state of the renderer.
 * @details This struct type describes the state at which a
 * widget is being drawn.
 */
typedef struct leRenderState
{
    const gfxDisplayDriver* dispDriver;  // the display driver pointer
    const gfxGraphicsProcessor* gpuDriver; // the gpu driver pointer

    uint32_t layerIdx;           // the current layer index

    //leRect displayRect;          // the driver physical display rectangle
    uint32_t bufferCount;        // the number of scratch buffers the library supports

    uint32_t frameRectIdx;       // the current frame draw rectangle index
    leWidget* currentWidget;     // the widget that is currently drawing

    leRect drawRect;              // the current damage rectangle clipped
                                  // to the currently rendering widget

    leLayerState* currentLayerState; // the current layer state
    leRenderLayerState* currentRenderLayer; // the layer that is being rendered

    leFrameState frameState;      // the current frame render state

    size_t drawCount;             // the number of times the screen has drawn

    uint32_t frameDrawCount;      // the number of widgets that have rendered
                                  // on the screen

    uint32_t deltaTime;           // stores delta time for updates that happen
                                  // during rendering

#if LE_ALPHA_BLENDING_ENABLED == 1
    leBool alphaEnable;           // the global alpha enabled flag
    uint8_t alpha;                // the current global alpha value
#endif

    lePalette* globalPalette;     // the pointer to the global palette

    int32_t currentScratchBuffer; // the index of the current scratch buffer

    gfxIOCTLArg_Value val;
} leRenderState;

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

leBool leRenderer_IsIdle(void)
{
    return _rendererState.frameState == LE_FRAME_READY;
}

size_t leRenderer_GetDrawCount(void)
{
    return _rendererState.drawCount;
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

void leRenderer_DisplaySize(leSize* sz)
{
    gfxIOCTLArg_DisplaySize val;

    _rendererState.dispDriver->ioctl(GFX_IOCTL_GET_DISPLAY_SIZE, &val);

    sz->width = val.width;
    sz->height = val.height;
}

void leRenderer_GetClipRect(leRect* rct)
{
    *rct = _rendererState.drawRect;
}

void leRenderer_GetFrameRect(leRect* rct)
{
    *rct = _rendererState.currentRenderLayer->frameRectList.rects[_rendererState.frameRectIdx];
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
    _rendererState.frameState = LE_FRAME_READY;
    
    return LE_SUCCESS;
}

void leRenderer_Shutdown()
{
}

const gfxDisplayDriver* leRenderer_DisplayInterface(void)
{
    return (gfxDisplayDriver*)_rendererState.dispDriver;
}

const gfxGraphicsProcessor* leRenderer_GPUInterface(void)
{
    return _rendererState.gpuDriver;
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

    if(_rendererState.currentRenderLayer->frameRectList.size > 0)
    {
        for(i = 0; i < _rendererState.currentRenderLayer->frameRectList.size; i++)
        {
            // area is already covered by an existing rect
            if(leRectContainsRect(&_rendererState.currentRenderLayer->frameRectList.rects[i], rect) == LE_TRUE)
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
    
    leRectArray_PushBack(&_rendererState.currentRenderLayer->frameRectList, rect);
}

leResult leRenderer_DamageArea(const leRect* rect,
                               uint32_t layerIdx)
{
    leRect clipRect;
    leLayerState* layerState;
    leRenderLayerState* renderLayerState;

    if(layerIdx >= _state->layerList.size)
        return LE_FAILURE;

    layerState = leGetLayerState(layerIdx);
    
    // make sure rect is inside the layer
    if(leRectIntersects(&layerState->root.rect, rect) == LE_FALSE)
        return LE_FAILURE;
    
    // clip the incoming rectangle
    leRectClip(&layerState->root.rect, rect, &clipRect);

    renderLayerState = (leRenderLayerState*)layerState->rendererData;

    // initiate a new render frame if necessary
    if(_rendererState.frameState <= LE_FRAME_PREFRAME)
    {
        _rendererState.frameState = LE_FRAME_PREFRAME;

        // drawing not in progress, add the rectangle to the current list
        addDamageRectToList(&renderLayerState->currentDamageRects, &clipRect);
    }
    else
    {
        //printf("added pending rect\n");
        
        // there is a frame in progress, add to the list that will be processed
        // next time around
        addDamageRectToList(&renderLayerState->pendingDamageRects, &clipRect);
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

    _rendererState.currentLayerState = leGetLayerState(_rendererState.layerIdx);
    _rendererState.currentRenderLayer = (leRenderLayerState*)_rendererState.currentLayerState->rendererData;

    leRectArray_Clear(&_rendererState.currentRenderLayer->scratchRectList);
    leRectArray_Clear(&_rendererState.currentRenderLayer->frameRectList);

    maxScratchPixels = SCRATCH_BUFFER_SZ / leColorInfoTable[leGetLayerColorMode(_rendererState.layerIdx)].size;

    // merge rectangle lists
    if(_rendererState.bufferCount > 1)
    {
        for(i = 0; i < _rendererState.currentRenderLayer->prevDamageRects.size; i++)
        {
            leRectArray_PushBack(&_rendererState.currentRenderLayer->scratchRectList,
                                 &_rendererState.currentRenderLayer->prevDamageRects.rects[i]);
        }
    }

    for(i = 0; i < _rendererState.currentRenderLayer->currentDamageRects.size; i++)
    {
        leRectArray_PushBack(&_rendererState.currentRenderLayer->scratchRectList,
                             &_rendererState.currentRenderLayer->currentDamageRects.rects[i]);
    }

    if(_rendererState.currentRenderLayer->scratchRectList.size == 0)
    {
        _rendererState.frameState = LE_FRAME_POSTLAYER;

        return;
    }

    // remove duplicate rectangles from combined list
    leRectArray_RemoveDuplicates(&_rendererState.currentRenderLayer->scratchRectList);

    // combine any adjacent rectangles
    leRectArray_MergeSimilar(&_rendererState.currentRenderLayer->scratchRectList);

    // remove overlapping space
    leRectArray_RemoveOverlapping(&_rendererState.currentRenderLayer->scratchRectList);

    // crop to scratch buffer size limit
    if(leGetLayerRenderHorizontal(_rendererState.layerIdx) == LE_TRUE)
    {
        // sort frame rects by X
        leRectArray_CropToSizeX(&_rendererState.currentRenderLayer->scratchRectList, maxScratchPixels);
    }
    else
    {
        // sort frame rects by Y
        leRectArray_CropToSizeY(&_rendererState.currentRenderLayer->scratchRectList, maxScratchPixels);
    }

#if LE_SCRATCH_BUFFER_PADDING == 1
    padSize = 4;

    if(leGetLayerColorMode(_rendererState.layerIdx) == LE_COLOR_MODE_RGB_565)
    {
        padSize = 8;
    }

#if LE_RENDER_ORIENTATION == 90 || LE_RENDER_ORIENTATION == 270
    leRectArray_PadRectangleHeight(&_rendererState.currentRenderLayer->scratchRectList,
                                   padSize);
#else
    leRectArray_PadRectangleWidth(&_rendererState.currentRenderLayer->scratchRectList,
                                  padSize);
#endif
#endif

    while(_rendererState.currentRenderLayer->scratchRectList.size != 0)
    {
        rect = _rendererState.currentRenderLayer->scratchRectList.rects[0];

        leRectArray_RemoveAt(&_rendererState.currentRenderLayer->scratchRectList, 0);

        addRectToFrameList(&rect);
    }

    if(leGetLayerRenderHorizontal(_rendererState.layerIdx) == LE_TRUE)
    {
        // sort frame rects by X
        leRectArray_SortByX(&_rendererState.currentRenderLayer->frameRectList);
    }
    else
    {
        // sort frame rects by Y
        leRectArray_SortByY(&_rendererState.currentRenderLayer->frameRectList);
    }

    _rendererState.frameRectIdx = 0;
    _rendererState.frameDrawCount = 0;

    if(_rendererState.currentRenderLayer->frameRectList.size == 0)
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
    leLayerState* layerState;
    leBool zeroize = LE_FALSE;

    if(_rendererState.frameRectIdx == _rendererState.currentRenderLayer->frameRectList.size)
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

    layerState = leGetLayerState(_rendererState.layerIdx);

    invalidateWidget(&layerState->root,
                     &_rendererState.currentRenderLayer->frameRectList.rects[_rendererState.frameRectIdx]);

    // set up render buffer to match damaged rectangle size
#if LE_RENDER_ORIENTATION == 0 || LE_RENDER_ORIENTATION == 180
    width = _rendererState.currentRenderLayer->frameRectList.rects[_rendererState.frameRectIdx].width;
    height = _rendererState.currentRenderLayer->frameRectList.rects[_rendererState.frameRectIdx].height;
#else
    width = _rendererState.currentRenderLayer->frameRectList.rects[_rendererState.frameRectIdx].height;
    height = _rendererState.currentRenderLayer->frameRectList.rects[_rendererState.frameRectIdx].width;
#endif

    lePixelBufferCreate(width,
                        height,
                        leGetLayerColorMode(_rendererState.layerIdx),
                        &_dataBuffers[idx],
                        &buf->renderBuffer);

    switch(layerState->clearMode)
    {
        case LE_LAYERCLEARMODE_FORCE:
        {
            zeroize = LE_TRUE;

            break;
        }
        case LE_LAYERCLEARMODE_DEFAULT:
        {
            if(layerState->colorMode == LE_COLOR_MODE_RGBA_8888)
            {
                zeroize = LE_TRUE;
            }

            break;
        }
        default:
        { }
    }

    if(zeroize == LE_TRUE)
    {
        if(leGPU_ClearBuffer(&buf->renderBuffer) == LE_FAILURE)
        {
            memset(buf->renderBuffer.pixels,
                   0,
                   buf->renderBuffer.buffer_length);
        }
    }

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
    leLayerState* layerState;

    layerState = leGetLayerState(_rendererState.layerIdx);

    // find a dirty widget to render
    _rendererState.currentWidget = findDirtyWidget(&layerState->root.children);
    
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

static leBool paintWidget(leWidget* widget)
{
    leRect clipRect;
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
            leUtils_ClipRectToAncestors(widget, &clipRect);
            
            // child does not intersect parent at all, do not draw
            if(clipRect.width <= 0 ||
               clipRect.height <= 0)
            {
                widget->fn->_validateChildren(widget);
        
                return LE_TRUE;
            }
            
            // widget visible area does not intersect dirty area at all
            // do not draw
            if(leRectIntersects(&clipRect,
                                &_rendererState.currentRenderLayer->frameRectList.rects[_rendererState.frameRectIdx]) == LE_FALSE)
            {
                widget->fn->_validateChildren(widget);
        
                return LE_TRUE;
            }
            
            // get the delta area between the dirty area and the child/parent
            // delta area
            leRectClip(&_rendererState.currentRenderLayer->frameRectList.rects[_rendererState.frameRectIdx],
                       &clipRect,
                       &_rendererState.drawRect);
        }
        else
        {
            _rendererState.drawRect = _rendererState.currentRenderLayer->frameRectList.rects[_rendererState.frameRectIdx];
        }

        //printf("rendering frame %i, widget %i, type %i, rect %i %i %i %i\n", _rendererState.drawCount, widget->id, widget->type, widget->rect.x, widget->rect.y, widget->rect.width, widget->rect.height);

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
    
#if LE_PREEMPTION_LEVEL >= 1
    // preempt if necessary
    if(painted == LE_TRUE)
        return LE_FAILURE;
#endif
        
    if(widget->children.size == 0)
        return LE_TRUE;
    
    return LE_TRUE;
}

static void _nextRect(void)
{
    _rendererState.frameRectIdx += 1;

    if(_rendererState.frameRectIdx < _rendererState.currentRenderLayer->frameRectList.size)
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

    leRect frameRect = _rendererState.currentRenderLayer->frameRectList.rects[_rendererState.frameRectIdx];

#if LE_RENDER_ORIENTATION == 0
    rotX = frameRect.x;
    rotY = frameRect.y;
#elif LE_RENDER_ORIENTATION == 90
    rotY = frameRect.x;
    rotX = _rendererState.currentLayerState->root.rect.height - frameRect.y - frameRect.height;
#elif LE_RENDER_ORIENTATION == 180
    rotX = _rendererState.currentLayerState->root.rect.width - frameRect.x - frameRect.width;
    rotY = _rendererState.currentLayerState->root.rect.height - frameRect.y - frameRect.height;
#elif LE_RENDER_ORIENTATION == 270
    rotX = frameRect.y;
    rotY = _rendererState.currentLayerState->root.rect.width - frameRect.x - frameRect.width;
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
    
    if(_rendererState.layerIdx < _state->layerList.size)
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
    leLayerState* layerState;
    leRenderLayerState* renderLayerState;

    _rendererState.frameDrawCount = 0;

    _rendererState.drawCount++;
    
    // manage the layer's rectangle arrays
    for(itr = 0; itr < _state->layerList.size; ++itr)
    {
        layerState = leGetLayerState(itr);

        renderLayerState = (leRenderLayerState*)layerState->rendererData;

        // double buffering support - move current rects to previous list
        if(_rendererState.bufferCount > 1)
        {
            leRectArray_Copy(&renderLayerState->currentDamageRects,
                             &renderLayerState->prevDamageRects);

            if(renderLayerState->prevDamageRects.size > MAX_RECTARRAYS_SZ)
            {
                leRectArray_Destroy(&renderLayerState->prevDamageRects);
            }
            else
            {
                leRectArray_Clear(&renderLayerState->prevDamageRects);
            }
        }

        if(renderLayerState->pendingDamageRects.size > 0)
        {
            leRectArray_Copy(&renderLayerState->pendingDamageRects,
                             &renderLayerState->currentDamageRects);

            if(renderLayerState->pendingDamageRects.size > MAX_RECTARRAYS_SZ)
            {
                leRectArray_Destroy(&renderLayerState->pendingDamageRects);
            }
            else
            {
                leRectArray_Clear(&renderLayerState->pendingDamageRects);
            }

            _rendererState.frameState = LE_FRAME_PREFRAME;
        }
        else
        {
            if(renderLayerState->currentDamageRects.size > MAX_RECTARRAYS_SZ)
            {
                leRectArray_Destroy(&renderLayerState->currentDamageRects);
            }
            else
            {
                leRectArray_Clear(&renderLayerState->currentDamageRects);
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

leResult _leRenderer_CreateLayerState(struct leLayerState* st)
{
    leRenderLayerState* renderLayerState;

    renderLayerState = LE_MALLOC(sizeof(leRenderLayerState));

    leRectArray_Create(&renderLayerState->prevDamageRects);
    leRectArray_Create(&renderLayerState->currentDamageRects);
    leRectArray_Create(&renderLayerState->pendingDamageRects);
    leRectArray_Create(&renderLayerState->scratchRectList);
    leRectArray_Create(&renderLayerState->frameRectList);

    renderLayerState->drawingPrev = LE_FALSE;

    st->rendererData = renderLayerState;

    return LE_SUCCESS;
}

void _leRenderer_DestroyLayerState(struct leLayerState* st)
{
    leRenderLayerState* renderLayerState = (leRenderLayerState*)st->rendererData;

    leRectArray_Destroy(&renderLayerState->prevDamageRects);
    leRectArray_Destroy(&renderLayerState->currentDamageRects);
    leRectArray_Destroy(&renderLayerState->pendingDamageRects);
    leRectArray_Destroy(&renderLayerState->frameRectList);
    leRectArray_Destroy(&renderLayerState->scratchRectList);

    LE_FREE(st->rendererData);
}

#endif // LE_DEBUG_RENDERER