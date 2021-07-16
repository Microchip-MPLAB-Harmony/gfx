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


#include "gfx/legato/core/legato_state.h"

#include "gfx/legato/core/legato_input.h"

#include "gfx/legato/image/legato_image.h"
#include "gfx/legato/memory/legato_memory.h"
#include "gfx/legato/renderer/legato_renderer.h"


#include <string.h>

int32_t _initialized = LE_FALSE;

#define LE_MAX_BUFFER_COUNT 2

static leState _state;

leState* leGetState()
{
    return &_state; 
}

#if LE_DYNAMIC_VTABLES == 1
/* vtable generation functions, make sure child classes come after base ones */
typedef void (*vtableFn)(void);

void _leString_GenerateVTable(void);
void _leDynamicString_GenerateVTable(void);
void _leFixedString_GenerateVTable(void);
void _leTableString_GenerateVTable(void);

void _leWidget_GenerateVTable(void);
void _leEditWidget_GenerateVTable(void);

void _leArcWidget_GenerateVTable(void);
void _leBarGraphWidget_GenerateVTable(void);
void _leButtonWidget_GenerateVTable(void);
void _leCheckBoxWidget_GenerateVTable(void);
void _leCircleWidget_GenerateVTable(void);
void _leCircularGaugeWidget_GenerateVTable(void);
void _leCircularSliderWidget_GenerateVTable(void);
void _leDrawSurfaceWidget_GenerateVTable(void);
void _leGradientWidget_GenerateVTable(void);
void _leGroupBoxWidget_GenerateVTable(void);
void _leImageWidget_GenerateVTable(void);
void _leImageRotateWidget_GenerateVTable(void);
void _leImageScaleWidget_GenerateVTable(void);
void _leImageSequenceWidget_GenerateVTable(void);
void _leKeyPadWidget_GenerateVTable(void);
void _leLabelWidget_GenerateVTable(void);
void _leLineWidget_GenerateVTable(void);
void _leLineGraphWidget_GenerateVTable(void);
void _leListWidget_GenerateVTable(void);
void _leListWheelWidget_GenerateVTable(void);
void _lePieChartWidget_GenerateVTable(void);
void _leProgressBarWidget_GenerateVTable(void);
void _leRadialMenuWidget_GenerateVTable(void);
void _leRadioButtonWidget_GenerateVTable(void);
void _leRectangleWidget_GenerateVTable(void);
void _leScrollBarWidget_GenerateVTable(void);
void _leSliderWidget_GenerateVTable(void);
void _leTextFieldWidget_GenerateVTable(void);
void _leTouchTestWidget_GenerateVTable(void);
void _leWindowWidget_GenerateVTable(void);

vtableFn vtableFnTable[] =
{
#if LE_DYNAMIC_VTABLES == 1
    _leString_GenerateVTable,
    _leDynamicString_GenerateVTable,
    _leFixedString_GenerateVTable,
    _leTableString_GenerateVTable,
#endif

    _leWidget_GenerateVTable,
    _leEditWidget_GenerateVTable,

#if LE_ARC_WIDGET_ENABLED == 1
    _leArcWidget_GenerateVTable,
#endif

#if LE_BARGRAPH_WIDGET_ENABLED == 1
    _leBarGraphWidget_GenerateVTable,
#endif

#if LE_BUTTON_WIDGET_ENABLED == 1
    _leButtonWidget_GenerateVTable,
#endif

#if LE_CHECKBOX_WIDGET_ENABLED == 1
    _leCheckBoxWidget_GenerateVTable,
#endif

#if LE_CIRCLE_WIDGET_ENABLED == 1
    _leCircleWidget_GenerateVTable,
#endif

#if LE_CIRCULARGAUGE_WIDGET_ENABLED == 1
    _leCircularGaugeWidget_GenerateVTable,
#endif

#if LE_CIRCULARSLIDER_WIDGET_ENABLED == 1
    _leCircularSliderWidget_GenerateVTable,
#endif

#if LE_DRAWSURFACE_WIDGET_ENABLED == 1
    _leDrawSurfaceWidget_GenerateVTable,
#endif

#if LE_GRADIENT_WIDGET_ENABLED == 1
    _leGradientWidget_GenerateVTable,
#endif

#if LE_GROUPBOX_WIDGET_ENABLED == 1
    _leGroupBoxWidget_GenerateVTable,
#endif

#if LE_IMAGE_WIDGET_ENABLED == 1
    _leImageWidget_GenerateVTable,
#endif

#if LE_IMAGEROTATE_WIDGET_ENABLED == 1
    _leImageRotateWidget_GenerateVTable,
#endif

#if LE_IMAGESCALE_WIDGET_ENABLED == 1
    _leImageScaleWidget_GenerateVTable,
#endif

#if LE_IMAGESEQUENCE_WIDGET_ENABLED == 1
    _leImageSequenceWidget_GenerateVTable,
#endif

#if LE_KEYPAD_WIDGET_ENABLED == 1
    _leKeyPadWidget_GenerateVTable,
#endif

#if LE_LABEL_WIDGET_ENABLED == 1
    _leLabelWidget_GenerateVTable,
#endif

#if LE_LINE_WIDGET_ENABLED == 1
    _leLineWidget_GenerateVTable,
#endif

#if LE_LINEGRAPH_WIDGET_ENABLED == 1
    _leLineGraphWidget_GenerateVTable,
#endif

#if LE_LIST_WIDGET_ENABLED == 1
    _leListWidget_GenerateVTable,
#endif

#if LE_LISTWHEEL_WIDGET_ENABLED == 1
    _leListWheelWidget_GenerateVTable,
#endif

#if LE_PIECHART_WIDGET_ENABLED == 1
    _lePieChartWidget_GenerateVTable,
#endif

#if LE_PROGRESSBAR_WIDGET_ENABLED == 1
    _leProgressBarWidget_GenerateVTable,
#endif

#if LE_RADIALMENU_WIDGET_ENABLED == 1
    _leRadialMenuWidget_GenerateVTable,
#endif

#if LE_RADIOBUTTON_WIDGET_ENABLED == 1
    _leRadioButtonWidget_GenerateVTable,
#endif

#if LE_RECTANGLE_WIDGET_ENABLED == 1
    _leRectangleWidget_GenerateVTable,
#endif

#if LE_SCROLLBAR_WIDGET_ENABLED == 1
    _leScrollBarWidget_GenerateVTable,
#endif

#if LE_SLIDER_WIDGET_ENABLED == 1
    _leSliderWidget_GenerateVTable,
#endif

#if LE_TEXTFIELD_WIDGET_ENABLED == 1
    _leTextFieldWidget_GenerateVTable,
#endif

#if LE_TOUCHTEST_WIDGET_ENABLED == 1
    _leTouchTestWidget_GenerateVTable,
#endif

#if LE_WINDOW_WIDGET_ENABLED == 1
    _leWindowWidget_GenerateVTable,
#endif

    NULL
};
#endif

leResult leInitialize(const gfxDisplayDriver* dispDriver,
                      const gfxGraphicsProcessor* gpuDriver)
{
    uint32_t idx;
    leWidget* root;
    gfxIOCTLArg_DisplaySize disp;
    
    if(_initialized == LE_TRUE)
        return LE_FAILURE;
        
    memset(&_state, 0, sizeof(leState));

#if LE_MEMORY_MANAGER_ENABLE == 1
    if(leMemory_Init() == LE_FAILURE)
        return LE_FAILURE;
#endif

    if(leEvent_Init() == LE_FAILURE ||
       leInput_Init() == LE_FAILURE ||
       leRenderer_Initialize(dispDriver, gpuDriver) == LE_FAILURE)
    {
        return LE_FAILURE;
    }

#if LE_DYNAMIC_VTABLES == 1
    /* initialize virtual function tables */
    idx = 0;
    
    while(vtableFnTable[idx] != NULL)
    {
        vtableFnTable[idx]();
        
        idx += 1;
    }
#endif
    
    leImage_InitDecoders();

    for(idx = 0; idx < LE_LAYER_COUNT; idx++)
    {
        root = &_state.rootWidget[idx];
        
        leWidget_Constructor(root);

        disp.width = 0;
        disp.height = 0;

        dispDriver->ioctl(GFX_IOCTL_SET_ACTIVE_LAYER, &idx);
        dispDriver->ioctl(GFX_IOCTL_GET_DISPLAY_SIZE, &disp);
        
        root->rect.x = 0;
        root->rect.y = 0;

#if LE_RENDER_ORIENTATION == 0 || LE_RENDER_ORIENTATION == 180
        root->rect.width = disp.width;
        root->rect.height = disp.height;
#else
        root->rect.width = disp.height;
        root->rect.height = disp.width;
#endif

        root->fn->invalidate(root);
        root->flags |= LE_WIDGET_ISROOT;
        root->flags |= LE_WIDGET_IGNOREEVENTS;
        root->flags |= LE_WIDGET_IGNOREPICK;

        _state.layerStates[idx].colorMode = LE_DEFAULT_COLOR_MODE;
    }
    
    _initialized = LE_TRUE;

    return LE_SUCCESS;
}

void leShutdown()
{
    uint32_t idx;
    leWidget* root;
    
    if(_initialized == LE_FALSE)
        return;
    
    for(idx = 0; idx < LE_LAYER_COUNT; idx++)
    {
        root = &_state.rootWidget[idx];
        
        root->fn->_destructor(root);
    }
    
    leRenderer_Shutdown();
    leInput_Shutdown();
    leEvent_Shutdown();
    
    memset(&_state, 0, sizeof(leState));

    _initialized = LE_FALSE;
}

static void updateWidget(leWidget* wgt, uint32_t dt)
{
    uint32_t i;
    leWidget* child;

    wgt->fn->update(wgt, dt);

    for(i = 0; i < wgt->children.size; i++)
    {
        child = wgt->children.values[i];

        updateWidget(child, dt);
    }
}

static void updateWidgets(uint32_t dt)
{
    int32_t i;

    if(leIsDrawing() == LE_TRUE)
        return;

    // iterate over all existing layers for update
    for(i = 0; i < LE_LAYER_COUNT; i++)
    {
        updateWidget(&_state.rootWidget[i], dt);
    }
}

leResult leUpdate(uint32_t dt)
{
#if LE_DRIVER_LAYER_MODE == 1
    uint32_t itr;
    gfxIOCTLArg_LayerRect layerRect;
#endif

    leEvent_ProcessEvents();

#if LE_DRIVER_LAYER_MODE == 1
    for(itr = 0; itr < LE_LAYER_COUNT; ++itr)
    {
        layerRect.base.id = itr;
        layerRect.x = 0;
        layerRect.y = 0;
        layerRect.width = 0;
        layerRect.height = 0;

        leGetRenderState()->dispDriver->ioctl(GFX_IOCTL_GET_LAYER_RECT, &layerRect);

        _state.layerStates[itr].driverPosition.x = layerRect.x;
        _state.layerStates[itr].driverPosition.y = layerRect.y;

        _state.rootWidget[itr].fn->setPosition(&_state.rootWidget[itr],
                                               0,
                                               0);

#if LE_RENDER_ORIENTATION == 90 || LE_RENDER_ORIENTATION == 270
        _state.rootWidget[itr].fn->setSize(&_state.rootWidget[itr],
                                           layerRect.height,
                                           layerRect.width);
#else
        _state.rootWidget[itr].fn->setSize(&_state.rootWidget[itr],
                                           layerRect.width,
                                           layerRect.height);
#endif
    }
#endif

#if LE_STREAMING_ENABLED == 1
    // there is an active stream in progress, service that to completion
    // before painting anything else
    if(_state.activeStream != NULL)
    {
        if(_state.activeStream->isDone(_state.activeStream) == LE_TRUE)
        {
            _state.activeStream = NULL;
        }
        else
        {
            _state.activeStream->exec(_state.activeStream);

            return LE_SUCCESS;
        }
    }
#endif

    updateWidgets(dt);

    leRenderer_Paint();

    return LE_SUCCESS;
}

leColorMode leGetLayerColorMode(uint32_t idx)
{
    if(idx >= LE_LAYER_COUNT)
        return LE_COLOR_MODE_GS_8;

    return _state.layerStates[idx].colorMode;
}

leResult leSetLayerColorMode(uint32_t idx,
                             leColorMode mode)
{
    if(idx >= LE_LAYER_COUNT)
        return LE_FAILURE;

    _state.layerStates[idx].colorMode = mode;

    return LE_SUCCESS;
}

leBool leGetLayerRenderHorizontal(uint32_t idx)
{
    if(idx >= LE_LAYER_COUNT)
        return LE_FALSE;

    return _state.layerStates[idx].renderHorizontal;
}

leResult leSetLayerRenderHorizontal(uint32_t idx,
                                    leBool horz)
{
    if(idx >= LE_LAYER_COUNT)
        return LE_FAILURE;

    _state.layerStates[idx].renderHorizontal = horz;

    return LE_SUCCESS;
}

/*leRect leGetDisplayRect()
{
    return leGetRenderState()->displayRect;
}*/

leStringTable* leGetStringTable()
{
    return (leStringTable*)_state.stringTable;
}

void leSetStringTable(const leStringTable* table)
{
    _state.stringTable = table;    
    
    leRedrawAll();
}

uint32_t leGetStringLanguage()
{        
    return _state.languageID;
}

static void updateWidgetLanguage(leWidget* wgt)
{
    uint32_t i;
    leWidget* child;

    wgt->fn->languageChangeEvent(wgt);

    for(i = 0; i < wgt->children.size; i++)
    {
        child = wgt->children.values[i];

        updateWidgetLanguage(child);
    }
}

void leSetStringLanguage(uint32_t id)
{
    uint32_t i;

    if(_state.languageID == id)
        return;

    _state.languageID = id;

    // iterate over all existing layers for update
    for(i = 0; i < LE_LAYER_COUNT; i++)
    {
        updateWidgetLanguage(&_state.rootWidget[i]);
    }
}

leScheme* leGetDefaultScheme()
{
    return &_state.defaultScheme;
}

leWidget* leGetFocusWidget()
{
    return _state.focus;
}

leResult leSetFocusWidget(leWidget* widget)
{
    leEvent evt;
    
    if(_state.focus == widget)
        return LE_SUCCESS;

    if(_state.focus != NULL)
    {
        evt.id = LE_WIDGET_EVENT_FOCUS_LOST;
        
        _state.focus->fn->_handleEvent(_state.focus, &evt);
    } 

    _state.focus = widget;

    if(_state.focus != NULL)
    {
        evt.id = LE_WIDGET_EVENT_FOCUS_GAINED;
        
        _state.focus->fn->_handleEvent(_state.focus, &evt);
    }
    
    return LE_SUCCESS;
}

leEditWidget* leGetEditWidget()
{
    return _state.edit;
}

leResult leSetEditWidget(leEditWidget* widget)
{
    if(_state.edit == (leEditWidget*)widget)
        return LE_SUCCESS;

    if(_state.edit != NULL)
    {
        _state.edit->fn->editEnd(_state.edit);
    }

    _state.edit = widget;

    if(_state.edit != NULL && _state.edit->fn->editStart(_state.edit) == LE_FAILURE)
    {
        _state.edit = NULL;
        
        return LE_FAILURE;
    }

    return LE_SUCCESS;
}

void leRedrawAll()
{
    uint32_t layer;
    leWidget* root;

    for(layer = 0; layer < LE_LAYER_COUNT; layer++)
    {
        root = &_state.rootWidget[layer];

        root->fn->invalidate(root);
    }
}

leBool leIsDrawing()
{
    return leGetRenderState()->frameState > LE_FRAME_PREFRAME;
}

leResult leAddRootWidget(leWidget* wgt,
                         uint32_t layer)
{
    if(wgt == NULL || layer > LE_LAYER_COUNT - 1)
        return LE_FAILURE;
        
    leRenderer_DamageArea(&wgt->rect, layer);
        
    return _state.rootWidget[layer].fn->addChild(&_state.rootWidget[layer], wgt);
}

leResult leRemoveRootWidget(leWidget* wgt, uint32_t layer)
{
    leRect rect;
    
    if(wgt == NULL)
        return LE_FAILURE;
        
    rect = wgt->rect;
        
    if(_state.rootWidget[layer].fn->removeChild(&_state.rootWidget[layer], wgt) == LE_SUCCESS)
    {
        leRenderer_DamageArea(&rect, layer);
        
        return LE_SUCCESS;
    }
    
    return LE_FAILURE;
}

leBool leWidgetIsInScene(const leWidget* wgt)
{
    return leGetWidgetLayer(wgt) >= 0;
}

int32_t leGetWidgetLayer(const leWidget* wgt)
{
    int32_t layer;
    leWidget* root;
    leWidget* wgtRoot;

    if(wgt == NULL)
        return LE_FALSE;

    wgtRoot = wgt->fn->getRootWidget(wgt);

    if(wgtRoot == NULL)
        return LE_FALSE;

    for(layer = 0; layer < LE_LAYER_COUNT; layer++)
    {
        root = &_state.rootWidget[layer];

        if(root == wgtRoot)
            return layer;
    }

    return -1;
}

#if LE_STREAMING_ENABLED == 1
leStreamManager* leGetActiveStream()
{
    return _state.activeStream;
}

leResult leRunActiveStream()
{
    if(_state.activeStream != NULL)
    {
        if(_state.activeStream->exec(_state.activeStream) == LE_FAILURE)
        {
            leAbortActiveStream();
        }
        else
        {
            if(_state.activeStream != NULL &&
               _state.activeStream->isDone(_state.activeStream) == LE_TRUE)
            {
                _state.activeStream->cleanup(_state.activeStream);

                _state.activeStream = NULL;
            }

            return LE_SUCCESS;
        }
    }

    return LE_FAILURE;
}

void leAbortActiveStream()
{
    if(_state.activeStream != NULL)
    {
        _state.activeStream->abort(_state.activeStream);

        if(_state.activeStream != NULL &&
           _state.activeStream->isDone(_state.activeStream) == LE_TRUE)
        {
            _state.activeStream->cleanup(_state.activeStream);

            _state.activeStream = NULL;
        }
    }
}

#endif

leResult leEdit_StartEdit()
{
    leEditWidget* edit = leGetEditWidget();
    
    if(edit == NULL)
        return LE_FAILURE;
        
    return edit->fn->editStart(edit);
}

void leEdit_EndEdit()
{ 
    leEditWidget* edit = leGetEditWidget();
    
    if(edit == NULL)
        return;
        
    edit->fn->editEnd(edit);
}

void leEdit_Clear()
{ 
    leEditWidget* edit = leGetEditWidget();
    
    if(edit == NULL)
        return;
        
    edit->fn->editClear(edit);
}

void leEdit_Accept()
{
    leEditWidget* edit = leGetEditWidget();
    
    if(edit == NULL)
        return;
        
    edit->fn->editAccept(edit);
}

void leEdit_Set(leString* str)
{
    leEditWidget* edit = leGetEditWidget();
    
    if(edit == NULL)
        return;
        
    edit->fn->editSet(edit, str);
}

void leEdit_Append(leString* str)
{
    leEditWidget* edit = leGetEditWidget();
    
    if(edit == NULL)
        return;
        
    edit->fn->editAppend(edit, str);
}

void leEdit_Backspace()
{
    leEditWidget* edit = leGetEditWidget();
    
    if(edit == NULL)
        return;
        
    edit->fn->editBackspace(edit);
}
