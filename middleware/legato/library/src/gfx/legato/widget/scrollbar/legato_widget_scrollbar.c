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


#include <gfx/legato/legato.h>
#include "gfx/legato/widget/scrollbar/legato_widget_scrollbar.h"

#if LE_SCROLLBAR_WIDGET_ENABLED

#include "gfx/legato/common/legato_error.h"
#include "gfx/legato/common/legato_math.h"
#include "gfx/legato/common/legato_utils.h"
#include "gfx/legato/memory/legato_memory.h"
#include "gfx/legato/widget/legato_widget.h"

#if LE_DEBUG == 1
#include "gfx/legato/core/legato_debug.h"
#endif

#define DEFAULT_WIDTH           20
#define DEFAULT_HEIGHT          100

#define DEFAULT_MIN             0
#define DEFAULT_MAX             100
#define DEFAULT_EXTENT          10
#define DEFAULT_VALUE           0

#define DEFAULT_STEP            1

#define MIN_HANDLE_HEIGHT       16

static
#if LE_DYNAMIC_VTABLES == 0
const
#endif
leScrollBarWidgetVTable scrollBarWidgetVTable;

void _leScrollBar_GetUpLeftButtonRect(const leScrollBarWidget* bar, leRect* rect);
void _leScrollBar_GetDownRightButtonRect(const leScrollBarWidget* bar, leRect* rect);
void _leScrollBar_GetScrollAreaRect(const leScrollBarWidget* bar, leRect* rect);
void _leScrollBar_GetHandleRect(const leScrollBarWidget* bar, leRect* rect);
void _leScrollBarWidget_InvalidateBorderAreas(const leScrollBarWidget* bar);

static void _invalidateUpLeftButton(const leScrollBarWidget* _this)
{
    leRect rect;
    
    _leScrollBar_GetUpLeftButtonRect(_this, &rect);
    
    _this->fn->_damageArea(_this, &rect);
}

static void _invalidateDownRightButton(const leScrollBarWidget* _this)
{
    leRect rect;
    
    _leScrollBar_GetDownRightButtonRect(_this, &rect);
    
    _this->fn->_damageArea(_this, &rect);
}

static void _invalidateScrollArea(const leScrollBarWidget* _this)
{
    leRect rect;
    
    _leScrollBar_GetScrollAreaRect(_this, &rect);
    
    _this->fn->_damageArea(_this, &rect);
}

static void _invalidateHandleRect(const leScrollBarWidget* _this)
{
    leRect rect;
    
    _leScrollBar_GetHandleRect(_this, &rect);
    
    _this->fn->_damageArea(_this, &rect);
}

uint32_t _leScrollWidget_GetExtent(const leScrollBarWidget* _this)
{
    uint32_t extent = _this->extent;
    
    if(extent >= _this->max)
        extent = _this->max - 1;
        
    return extent;
}

static uint32_t _getPercentFromPoint(const leScrollBarWidget* _this,
                                     const lePoint* pnt)
{
    leRect scrollRect, handleRect;
    int32_t val;
    int32_t size;
    
    _leScrollBar_GetScrollAreaRect(_this, &scrollRect);
    _leScrollBar_GetHandleRect(_this, &handleRect);
    
    if(_this->alignment == LE_ORIENTATION_VERTICAL)
    {
        size = scrollRect.height - handleRect.height;
                
        // translate rect and point in to rect space
        val = pnt->y - scrollRect.y;
        scrollRect.y = 0;
        
        if(val <= 0)
        {
            return 0;
        }
        else if(val >= size)
        {
            return 100;
        }
        
        return lePercentWholeRounded(val, size);
    }
    else
    {
        size = scrollRect.width - handleRect.width;
        
        // translate rect and point in to rect space
        val = pnt->x - scrollRect.x;
        scrollRect.x = 0;
        
        //scrollRect.width -= _leScrollWidget_GetExtent(_this);
        
        if(val <= 0)
        {
            return 0;
        }
        else if(val >= size)
        {
            return 100;
        }
        
        return lePercentWholeRounded(val, size);
    }
}

static uint32_t _getValueFromPercent(const leScrollBarWidget* _this,
                                     uint32_t per)
{
    int32_t max;
    
    if(per == 0)
    {
        return DEFAULT_MIN;
    }
    else if(per == 100)
    {
        return _this->max;
    }
    
    // zero based range
    max = _this->max;
    
    return lePercentOf(max, per);
}

void leScrollBarWidget_Constructor(leScrollBarWidget* _this)
{
    leWidget_Constructor((leWidget*)_this);

    _this->widget.fn = (void*)&scrollBarWidgetVTable;
    _this->fn = &scrollBarWidgetVTable;
    
    _this->widget.type = LE_WIDGET_SCROLLBAR;

    _this->widget.rect.width = DEFAULT_WIDTH;
    _this->widget.rect.height = DEFAULT_HEIGHT;

    _this->state = LE_SCROLLBAR_STATE_NONE;

    _this->widget.style.borderType = LE_WIDGET_BORDER_BEVEL;

    _this->alignment = LE_ORIENTATION_VERTICAL;
    
    //_this->min = DEFAULT_MIN;
    _this->max = DEFAULT_MAX;
    _this->extent = DEFAULT_EXTENT;
    _this->value = DEFAULT_VALUE;
    _this->step = DEFAULT_STEP;
    _this->handleDownOffset = lePoint_Zero;

    //_this->widget.invalidateBorderAreas = (leWidget_InvalidateBorderAreas_FnPtr)&_leScrollBarWidget_InvalidateBorderAreas;

    _this->valueChangedEvent = NULL;
}

void _leWidget_Destructor(leWidget* wgt);

static void destructor(leScrollBarWidget* _this)
{
    _leWidget_Destructor((leWidget*)_this);
}

leScrollBarWidget* leScrollBarWidget_New(void)
{
    leScrollBarWidget* bar = NULL;

    bar = LE_MALLOC(sizeof(leScrollBarWidget));
    
    leScrollBarWidget_Constructor(bar);

    return bar;
}

static leOrientation getOrientation(const leScrollBarWidget* _this)
{
    LE_ASSERT_THIS();

    return _this->alignment;
}

static leResult setOrientation(leScrollBarWidget* _this,
                               leOrientation align,
                               leBool swapDimensions)
{
    uint32_t t;
    
    LE_ASSERT_THIS();
    
    if(_this->alignment == align)
        return LE_SUCCESS;

    _this->alignment = align;
    
    // reverse dimensions
    if(swapDimensions == LE_TRUE)
    {
        t = _this->widget.rect.width;
        _this->widget.rect.width = _this->widget.rect.height;
        _this->widget.rect.height = t;
    }
    
    _this->fn->invalidate(_this);

#if LE_DEBUG == 1
    _leDebugNotify_WidgetPropertyChanged((leWidget*)_this);
#endif

    return LE_SUCCESS;
}

static uint32_t getMaximumValue(const leScrollBarWidget* _this)
{
    LE_ASSERT_THIS();
        
    return _this->max;
}

static leResult setMaximumValue(leScrollBarWidget* _this,
                                uint32_t val)
{
    LE_ASSERT_THIS();
        
    if(_this->max == val)
        return LE_SUCCESS;
        
    _this->max = val;

    if(_this->value > _this->max)
    {
        _this->value = _this->max;
    }
    
    _this->fn->invalidate(_this);

#if LE_DEBUG == 1
    _leDebugNotify_WidgetPropertyChanged((leWidget*)_this);
#endif

    return LE_SUCCESS;
}                                                 

static uint32_t getExtentValue(const leScrollBarWidget* _this)
{
    LE_ASSERT_THIS();
        
    return _this->extent;
}

static leResult setExtentValue(leScrollBarWidget* _this,
                               uint32_t val)
{
    LE_ASSERT_THIS();
        
    if(_this->extent == val)
        return LE_SUCCESS;
        
    _this->extent = val;
    
    _this->fn->invalidate(_this);

#if LE_DEBUG == 1
    _leDebugNotify_WidgetPropertyChanged((leWidget*)_this);
#endif

    return LE_SUCCESS;
}

static uint32_t getScrollPercentage(const leScrollBarWidget* _this)
{
    LE_ASSERT_THIS();
        
    return lePercentWholeRounded(_this->value, _this->max);
}

static leResult setScrollPercentage(leScrollBarWidget* _this,
                                    uint32_t val)
{
    LE_ASSERT_THIS();
        
    if(_this->value == val)
        return LE_SUCCESS;
        
    _this->value = val;
    
    if(_this->valueChangedEvent != NULL)
    {
        _this->valueChangedEvent(_this);
    }
    
    _this->fn->invalidate(_this);

#if LE_DEBUG == 1
    _leDebugNotify_WidgetPropertyChanged((leWidget*)_this);
#endif

    return LE_SUCCESS;
} 
                                                                                                               
static uint32_t getScrollValue(const leScrollBarWidget* _this)
{
    LE_ASSERT_THIS();
        
    return _this->value;
}

static leResult setScrollValue(leScrollBarWidget* _this,
                               uint32_t val) 
{
    LE_ASSERT_THIS();
        
    if(_this->value == val)
        return LE_SUCCESS;
        
    if(val > _this->max)
        val = _this->max;
    
    _invalidateHandleRect(_this);
   
    _this->value = val;
    
    if(_this->valueChangedEvent != NULL)
    {
        _this->valueChangedEvent(_this);
    }
    
    _invalidateHandleRect(_this);

#if LE_DEBUG == 1
    _leDebugNotify_WidgetPropertyChanged((leWidget*)_this);
#endif

    return LE_SUCCESS;
}

static uint32_t getStepSize(const leScrollBarWidget* _this)
{
    LE_ASSERT_THIS();
    
    return _this->step;
}

static leResult setStepSize(leScrollBarWidget* _this,
                            uint32_t val)
{
    LE_ASSERT_THIS();
        
    _this->step = val;

#if LE_DEBUG == 1
    _leDebugNotify_WidgetPropertyChanged((leWidget*)_this);
#endif

    return LE_SUCCESS;
}

static leResult stepBackward(leScrollBarWidget* _this)
{
    LE_ASSERT_THIS();
    
    if(_this->value == DEFAULT_MIN)
        return LE_FAILURE;
        
    _invalidateHandleRect(_this);
        
    if(((int32_t)_this->value - (int32_t)_this->step) < DEFAULT_MIN)
    {
        _this->value = DEFAULT_MIN;
    }
    else
    {
        _this->value -= _this->step;
    }
        
    if(_this->valueChangedEvent != NULL)
    {
        _this->valueChangedEvent(_this);
    }
        
    _invalidateHandleRect(_this);
        
    return LE_SUCCESS;
}

static leResult stepForward(leScrollBarWidget* _this)
{
    LE_ASSERT_THIS();
    
    if(_this->value == _this->max)
        return LE_FAILURE;
        
    _invalidateHandleRect(_this);

    if(((int32_t)_this->value + (int32_t)_this->step) > (int32_t)_this->max)
    {
        _this->value = _this->max;
    }
    else
        _this->value += _this->step;
        
    if(_this->valueChangedEvent != NULL)
    {
        _this->valueChangedEvent(_this);
    }
        
    _invalidateHandleRect(_this);
        
    return LE_SUCCESS;
}

static leScrollBarWidget_ValueChangedEvent getValueChangedEventCallback(const leScrollBarWidget* _this)
{
    LE_ASSERT_THIS();
    
    return _this->valueChangedEvent;
}

static leResult setValueChangedEventCallback(leScrollBarWidget* _this,
                                             leScrollBarWidget_ValueChangedEvent cb)
{
    LE_ASSERT_THIS();
    
    _this->valueChangedEvent = cb;
    
    return LE_SUCCESS;
}

static void handleTouchDownEvent(leScrollBarWidget* _this,
                                 leWidgetEvent_TouchDown* evt)
{
    leRect rect;
    lePoint pnt;
    
    LE_ASSERT_THIS();

    pnt.x = evt->x;
    pnt.y = evt->y;
    
    // already guaranteed to be inside widget rectangle, accept event
    leWidgetEvent_Accept((leWidgetEvent*)evt, (leWidget*)_this);

    // was the up/left button pressed
    _leScrollBar_GetUpLeftButtonRect(_this, &rect);
    
    if(leRectContainsPoint(&rect, &pnt) == LE_TRUE)
    {
        _this->state = LE_SCROLLBAR_STATE_TOP_INSIDE;
        
        _invalidateUpLeftButton(_this);
        
        return;
    }
    
    // was the up/left button pressed
    _leScrollBar_GetDownRightButtonRect(_this, &rect);
    
    if(leRectContainsPoint(&rect, &pnt) == LE_TRUE)
    {
        _this->state = LE_SCROLLBAR_STATE_BOTTOM_INSIDE;
        
        _invalidateDownRightButton(_this);
        
        return;
    }
    
    // was the handle touched
    _leScrollBar_GetHandleRect(_this, &rect);
    
    if(leRectContainsPoint(&rect, &pnt) == LE_TRUE)
    {
        _this->state = LE_SCROLLBAR_STATE_HANDLE_DOWN;
        
        _this->handleDownOffset.x = evt->x - rect.x;
        _this->handleDownOffset.y = evt->y - rect.y;
        
        return;
    }
}

static void handleTouchUpEvent(leScrollBarWidget* _this,
                               leWidgetEvent_TouchUp* evt)
{
    leRect rect = _this->widget.rect;
    lePoint pnt;
    
    LE_ASSERT_THIS();

    pnt.x = evt->x;
    pnt.y = evt->y;

    leWidgetEvent_Accept((leWidgetEvent*)evt, (leWidget*)_this);
    
    if(_this->state == LE_SCROLLBAR_STATE_TOP_INSIDE)
    {
        _leScrollBar_GetUpLeftButtonRect(_this, &rect);
    
        if(leRectContainsPoint(&rect, &pnt) == LE_TRUE)
        {
            _this->fn->stepBackward(_this);
            
            _invalidateUpLeftButton(_this);
        }
    }
    else if(_this->state == LE_SCROLLBAR_STATE_BOTTOM_INSIDE)
    {
        _leScrollBar_GetDownRightButtonRect(_this, &rect);
    
        if(leRectContainsPoint(&rect, &pnt) == LE_TRUE)
        {
            _this->fn->stepForward(_this);
            
            _invalidateDownRightButton(_this);
        }
    }
    
    _this->state = LE_SCROLLBAR_STATE_NONE;
    
    _invalidateScrollArea(_this);
}

static void handleTouchMovedEvent(leScrollBarWidget* _this,
                                  leWidgetEvent_TouchMove* evt)
{
    leRect rect = _this->widget.rect;
    lePoint pnt;
    uint32_t percent;
    uint32_t val;
    
    LE_ASSERT_THIS();

    pnt.x = evt->x;
    pnt.y = evt->y;

    leWidgetEvent_Accept((leWidgetEvent*)evt, (leWidget*)_this);

    if(_this->state == LE_SCROLLBAR_STATE_TOP_PRESSED ||
       _this->state == LE_SCROLLBAR_STATE_TOP_INSIDE)
    {
        _leScrollBar_GetUpLeftButtonRect(_this, &rect);
    
        if(_this->state == LE_SCROLLBAR_STATE_TOP_INSIDE &&
           leRectContainsPoint(&rect, &pnt) == LE_FALSE)
        {
            _this->state = LE_SCROLLBAR_STATE_TOP_PRESSED;

            _invalidateUpLeftButton(_this);
        }
        else if(_this->state == LE_SCROLLBAR_STATE_TOP_PRESSED &&
           leRectContainsPoint(&rect, &pnt) == LE_TRUE)
        {
            _this->state = LE_SCROLLBAR_STATE_TOP_INSIDE;

            _invalidateDownRightButton(_this);
        }
    }
    else if(_this->state == LE_SCROLLBAR_STATE_BOTTOM_PRESSED ||
            _this->state == LE_SCROLLBAR_STATE_BOTTOM_INSIDE)
    {
        _leScrollBar_GetDownRightButtonRect(_this, &rect);
    
        if(_this->state == LE_SCROLLBAR_STATE_BOTTOM_INSIDE &&
           leRectContainsPoint(&rect, &pnt) == LE_FALSE)
        {
            _this->state = LE_SCROLLBAR_STATE_BOTTOM_PRESSED;

            _this->fn->invalidate(_this);
        }
        else if(_this->state == LE_SCROLLBAR_STATE_BOTTOM_PRESSED &&
                leRectContainsPoint(&rect, &pnt) == LE_TRUE)
        {
            _this->state = LE_SCROLLBAR_STATE_BOTTOM_INSIDE;

            _invalidateScrollArea(_this);
        }
    }
    else if(_this->state == LE_SCROLLBAR_STATE_HANDLE_DOWN)
    {
        pnt.x = evt->x - _this->handleDownOffset.x;
        pnt.y = evt->y - _this->handleDownOffset.y;
        
        percent = _getPercentFromPoint(_this, &pnt);
        
        val = _getValueFromPercent(_this, percent);
        
        if(_this->value != val)
        {
            _this->fn->setScrollValue(_this, val);
        }
    }
}

void _leScrollBarWidget_Paint(leScrollBarWidget* _this);

#if LE_DYNAMIC_VTABLES == 1
void _leWidget_FillVTable(leWidgetVTable* tbl);

void _leScrollBarWidget_GenerateVTable(void)
{
    _leWidget_FillVTable((void*)&scrollBarWidgetVTable);

    /* overrides from base class */
    scrollBarWidgetVTable._destructor = destructor;
    scrollBarWidgetVTable._paint = _leScrollBarWidget_Paint;
    scrollBarWidgetVTable._invalidateBorderAreas = _leScrollBarWidget_InvalidateBorderAreas;
    scrollBarWidgetVTable.touchDownEvent = handleTouchDownEvent;
    scrollBarWidgetVTable.touchUpEvent = handleTouchUpEvent;
    scrollBarWidgetVTable.touchMoveEvent = handleTouchMovedEvent;
    
    /* member functions */
    scrollBarWidgetVTable.getOrientation = getOrientation;
    scrollBarWidgetVTable.setOrientation = setOrientation;
    scrollBarWidgetVTable.getMaximumValue = getMaximumValue;
    scrollBarWidgetVTable.setMaximumValue = setMaximumValue;
    scrollBarWidgetVTable.getExtentValue = getExtentValue;
    scrollBarWidgetVTable.setExtentValue = setExtentValue;
    scrollBarWidgetVTable.getScrollPercentage = getScrollPercentage;
    scrollBarWidgetVTable.setScrollPercentage = setScrollPercentage;
    scrollBarWidgetVTable.getScrollValue = getScrollValue;
    scrollBarWidgetVTable.setScrollValue = setScrollValue;
    scrollBarWidgetVTable.getStepSize = getStepSize;
    scrollBarWidgetVTable.setStepSize = setStepSize;
    scrollBarWidgetVTable.stepBackward = stepBackward;
    scrollBarWidgetVTable.stepForward = stepForward;
    scrollBarWidgetVTable.getValueChangedEventCallback = getValueChangedEventCallback;
    scrollBarWidgetVTable.setValueChangedEventCallback = setValueChangedEventCallback;
}

void _leScrollBarWidget_FillVTable(leScrollBarWidgetVTable* tbl)
{
    *tbl = scrollBarWidgetVTable;
}
#else
static const leScrollBarWidgetVTable scrollBarWidgetVTable =
{
    // base class
    .getType = (void*)_leWidget_GetType,
    .getX = (void*)_leWidget_GetX,
    .setX = (void*)_leWidget_SetX,
    .getY = (void*)_leWidget_GetY,
    .setY = (void*)_leWidget_SetY,
    .setPosition = (void*)_leWidget_SetPosition,
    .translate = (void*)_leWidget_Translate,
    .getWidth = (void*)_leWidget_GetWidth,
    .setWidth = (void*)_leWidget_SetWidth,
    .getHeight = (void*)_leWidget_GetHeight,
    .setHeight = (void*)_leWidget_SetHeight,
    .setSize = (void*)_leWidget_SetSize,
    .resize = (void*)_leWidget_Resize,
    .getAlphaEnabled = (void*)_leWidget_GetAlphaEnabled,
    .getCumulativeAlphaEnabled = (void*)_leWidget_GetCumulativeAlphaEnabled,
    .setAlphaEnabled = (void*)_leWidget_SetAlphaEnabled,
    .getAlphaAmount = (void*)_leWidget_GetAlphaAmount,
    .getCumulativeAlphaAmount = (void*)_leWidget_GetCumulativeAlphaAmount,
    .setAlphaAmount = (void*)_leWidget_SetAlphaAmount,
    .isOpaque = (void*)_leWidget_IsOpaque,
    .getEnabled = (void*)_leWidget_GetEnabled,
    .setEnabled = (void*)_leWidget_SetEnabled,
    .getVisible = (void*)_leWidget_GetVisible,
    .setVisible = (void*)_leWidget_SetVisible,
    .localRect = (void*)_leWidget_LocalRect,
    .rectToParent = (void*)_leWidget_RectToParentSpace,
    .rectToScreen = (void*)_leWidget_RectToScreenSpace,
    .addChild = (void*)_leWidget_AddChild,
    .removeChild = (void*)_leWidget_RemoveChild,
    .removeAllChildren = (void*)_leWidget_RemoveAllChildren,
    .getRootWidget = (void*)_leWidget_GetRootWidget,
    .setParent = (void*)_leWidget_SetParent,
    .getChildCount = (void*)_leWidget_GetChildCount,
    .getChildAtIndex = (void*)_leWidget_GetChildAtIndex,
    .getIndexOfChild = (void*)_leWidget_GetIndexOfChild,
    .containsDescendant = (void*)_leWidget_ContainsDescendant,
    .getScheme = (void*)_leWidget_GetScheme,
    .setScheme = (void*)_leWidget_SetScheme,
    .getBorderType = (void*)_leWidget_GetBorderType,
    .setBorderType = (void*)_leWidget_SetBorderType,
    .getBackgroundType = (void*)_leWidget_GetBackgroundType,
    .setBackgroundType = (void*)_leWidget_SetBackgroundType,
    .getHAlignment = (void*)_leWidget_GetHAlignment,
    .setHAlignment = (void*)_leWidget_SetHAlignment,
    .getVAlignment = (void*)_leWidget_GetVAlignment,
    .setVAlignment = (void*)_leWidget_SetVAlignment,
    .getMargins = (void*)_leWidget_GetMargins,
    .setMargins = (void*)_leWidget_SetMargins,
    .getCornerRadius = (void*)_leWidget_GetCornerRadius,
    .setCornerRadius = (void*)_leWidget_SetCornerRadius,
    .hasFocus = (void*)_leWidget_HasFocus,
    .setFocus = (void*)_leWidget_SetFocus,
    .invalidate = (void*)_leWidget_Invalidate,
    .invalidateContents = (void*)_leWidget_InvalidateContents,
    .installEventFilter = (void*)_leWidget_InstallEventFilter,
    .removeEventFilter = (void*)_leWidget_RemoveEventFilter,

    .update = (void*)_leWidget_Update,

    .moveEvent = (void*)_leWidget_MoveEvent,
    .resizeEvent = (void*)_leWidget_ResizeEvent,
    .focusLostEvent = (void*)_leWidget_FocusLostEvent,
    .focusGainedEvent = (void*)_leWidget_FocusGainedEvent,
    .languageChangeEvent = (void*)_leWidget_LanguageChangeEvent,

    ._handleEvent = (void*)_leWidget_HandleEvent,
    ._validateChildren = (void*)_leWidget_ValidateChildren,
    ._increaseDirtyState = (void*)_leWidget_IncreaseDirtyState,
    ._setDirtyState = (void*)_leWidget_SetDirtyState,
    ._clearDirtyState = (void*)_leWidget_ClearDirtyState,
    ._damageArea = (void*)_leWidget_DamageArea,

    /* overrides from base class */
    ._destructor = (void*)destructor,
    ._paint = (void*)_leScrollBarWidget_Paint,
    ._invalidateBorderAreas = (void*)_leScrollBarWidget_InvalidateBorderAreas,
    .touchDownEvent = (void*)handleTouchDownEvent,
    .touchUpEvent = (void*)handleTouchUpEvent,
    .touchMoveEvent = (void*)handleTouchMovedEvent,

    /* member functions */
    .getOrientation = getOrientation,
    .setOrientation = setOrientation,
    .getMaximumValue = getMaximumValue,
    .setMaximumValue = setMaximumValue,
    .getExtentValue = getExtentValue,
    .setExtentValue = setExtentValue,
    .getScrollPercentage = getScrollPercentage,
    .setScrollPercentage = setScrollPercentage,
    .getScrollValue = getScrollValue,
    .setScrollValue = setScrollValue,
    .getStepSize = getStepSize,
    .setStepSize = setStepSize,
    .stepBackward = stepBackward,
    .stepForward = stepForward,
    .getValueChangedEventCallback = getValueChangedEventCallback,
    .setValueChangedEventCallback = setValueChangedEventCallback,
};
#endif

#endif // LE_SCROLLBAR_WIDGET_ENABLED