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
typedef void (*vtableFn)();

void _leString_GenerateVTable();
void _leDynamicString_GenerateVTable();
void _leFixedString_GenerateVTable();
void _leTableString_GenerateVTable();

void _leWidget_GenerateVTable();
void _leEditWidget_GenerateVTable();

void _leArcWidget_GenerateVTable();
void _leBarGraphWidget_GenerateVTable();
void _leButtonWidget_GenerateVTable();
void _leCheckBoxWidget_GenerateVTable();
void _leCircleWidget_GenerateVTable();
void _leCircularGaugeWidget_GenerateVTable();
void _leCircularSliderWidget_GenerateVTable();
void _leDrawSurfaceWidget_GenerateVTable();
void _leGradientWidget_GenerateVTable();
void _leGroupBoxWidget_GenerateVTable();
void _leImageWidget_GenerateVTable();
void _leImageRotateWidget_GenerateVTable();
void _leImageScaleWidget_GenerateVTable();
void _leImageSequenceWidget_GenerateVTable();
void _leKeyPadWidget_GenerateVTable();
void _leLabelWidget_GenerateVTable();
void _leLineWidget_GenerateVTable();
void _leLineGraphWidget_GenerateVTable();
void _leListWidget_GenerateVTable();
void _leListWheelWidget_GenerateVTable();
void _lePieChartWidget_GenerateVTable();
void _leProgressBarWidget_GenerateVTable();
void _leRadialMenuWidget_GenerateVTable();
void _leRadioButtonWidget_GenerateVTable();
void _leRectangleWidget_GenerateVTable();
void _leScrollBarWidget_GenerateVTable();
void _leSliderWidget_GenerateVTable();
void _leTextFieldWidget_GenerateVTable();
void _leTouchTestWidget_GenerateVTable();
void _leWindowWidget_GenerateVTable();

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

leResult leInitialize(const gfxDisplayDriver* dispDriver)
{
    uint32_t idx;
    leWidget* root;
    
    if(_initialized == LE_TRUE)
        return LE_FAILURE;
        
    memset(&_state, 0, sizeof(leState));
        
    if(leMemory_Init() == LE_FAILURE ||
       leEvent_Init() == LE_FAILURE ||
       leInput_Init() == LE_FAILURE ||
       leRenderer_Initialize(dispDriver) == LE_FAILURE)
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
    leScheme_Initialize(&_state.defaultScheme, LE_GLOBAL_COLOR_MODE);

    for(idx = 0; idx < LE_LAYER_COUNT; idx++)
    {
        root = &_state.rootWidget[idx];
        
        leWidget_Constructor(root);
        
        root->fn->setPosition(root, 0, 0);
        
        root->fn->setSize(root,
                          dispDriver->getDisplayWidth(),
                          dispDriver->getDisplayHeight());
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
    leEvent_ProcessEvents();

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

leColorMode leGetColorMode()
{
    return LE_GLOBAL_COLOR_MODE;
}

leRect leGetDisplayRect()
{
    return leGetRenderState()->displayRect;
}

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
        
    leRenderer_DamageArea(&wgt->rect);
        
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
        leRenderer_DamageArea(&rect);
        
        return LE_SUCCESS;
    }
    
    return LE_FAILURE;
}

leBool leWidgetIsInScene(const leWidget* wgt)
{
    uint32_t layer;
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
            return LE_TRUE;
    }
    
    return LE_FALSE;
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
