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
#include "gfx/legato/widget/slider/legato_widget_slider.h"

#if LE_SLIDER_WIDGET_ENABLED

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
#define DEFAULT_VALUE           0

#define DEFAULT_STEP            1
#define DEFAULT_GRIP            10

static
#if LE_DYNAMIC_VTABLES == 0
const
#endif
leSliderWidgetVTable sliderWidgetVTable;

void _leSliderWidget_GetSlideAreaRect(const leSliderWidget* sld, leRect* rect);
void _leSliderWidget_GetHandleRect(const leSliderWidget* sld, leRect* rect);
void _leSliderWidget_InvalidateBorderAreas(const leSliderWidget* sld);

/*
static void invalidateSlideArea(leSliderWidget* _this)
{
    leRect rect;

    _leSliderWidget_GetSlideAreaRect(_this, &rect);

    leRenderer_DamageArea(leUtils_GetLayer((leWidget*)_this),
                          &rect,
                          LE_FALSE);
}*/

static void invalidateHandle(const leSliderWidget* _this)
{
    leRect rect;

    _leSliderWidget_GetHandleRect(_this, &rect);
    leUtils_RectToScreenSpace((leWidget*)_this, &rect);

    _this->fn->_damageArea(_this, &rect);
}

uint32_t _getPercentFromPoint(const leSliderWidget* _this,
                                    const lePoint* pnt)
{
    leRect scrollRect;
    int32_t val;

    _leSliderWidget_GetSlideAreaRect(_this, &scrollRect);
    leUtils_RectToScreenSpace((leWidget*)_this, &scrollRect);

    if(_this->alignment == LE_ORIENTATION_VERTICAL)
    {
        // translate rect and point in to rect space
        val = scrollRect.height - (pnt->y - scrollRect.y);
        scrollRect.y = 0;

        if(val <= 0)
        {
            return 0;
        }
        else if(val >= scrollRect.height)
        {
            return 100;
        }

        return lePercentWholeRounded(val, scrollRect.height);
    }
    else
    {
        // translate rect and point in to rect space
        val = pnt->x - scrollRect.x;
        scrollRect.x = 0;

        if(val <= 0)
        {
            return 0;
        }
        else if(val >= scrollRect.width)
        {
            return 100;
        }

        return lePercentWholeRounded(val, scrollRect.width);
    }
}

static uint32_t _getValueFromPercent(const leSliderWidget* _this,
                                     uint32_t per)
{
    int32_t max, val;

    if(per == 0)
    {
        return _this->min;
    }
    else if(per == 100)
    {
        return _this->max;
    }

    // zero based range
    max = _this->max - _this->min;

    val = lePercentOf(max, per);

    return val + _this->min;
}

void leSliderWidget_Constructor(leSliderWidget* _this)
{
    leWidget_Constructor((leWidget*)_this);

    _this->widget.fn = (void*)&sliderWidgetVTable;
    _this->fn = &sliderWidgetVTable;

    _this->widget.type = LE_WIDGET_SLIDER;

    _this->widget.rect.width = DEFAULT_WIDTH;
    _this->widget.rect.height = DEFAULT_HEIGHT;

    _this->state = LE_SLIDER_STATE_NONE;

    _this->widget.style.borderType = LE_WIDGET_BORDER_BEVEL;
    _this->widget.style.backgroundType = LE_WIDGET_BACKGROUND_FILL;

    _this->alignment = LE_ORIENTATION_VERTICAL;

    _this->min = DEFAULT_MIN;
    _this->max = DEFAULT_MAX;
    _this->value = DEFAULT_VALUE;
    _this->grip = DEFAULT_GRIP;

    _this->valueChangedEvent = NULL;
    _this->handleDownOffset = lePoint_Zero;
}

void _leWidget_Destructor(leWidget* wgt);

static void destructor(leSliderWidget* _this)
{
    _leWidget_Destructor((leWidget*)_this);
}

leSliderWidget* leSliderWidget_New()
{
    leSliderWidget* sld = NULL;

    sld = LE_MALLOC(sizeof(leSliderWidget));

    leSliderWidget_Constructor(sld);

    return sld;
}

static leOrientation getOrientation(const leSliderWidget* _this)
{
    LE_ASSERT_THIS();

    return _this->alignment;
}

static leResult setOrientation(leSliderWidget* _this,
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

static uint32_t getGripSize(const leSliderWidget* _this)
{
    LE_ASSERT_THIS();

    return _this->grip;
}

static leResult setGripSize(leSliderWidget* _this,
                            uint32_t size)
{
    LE_ASSERT_THIS();

    if(_this->grip == size)
        return LE_SUCCESS;

    invalidateHandle(_this);

    _this->grip = size;

    invalidateHandle(_this);

#if LE_DEBUG == 1
    _leDebugNotify_WidgetPropertyChanged((leWidget*)_this);
#endif

    return LE_SUCCESS;
}

static uint32_t getMininumValue(const leSliderWidget* _this)
{
    LE_ASSERT_THIS();

    return _this->min;
}

static leResult setMinimumValue(leSliderWidget* _this,
                                uint32_t val)
{
    LE_ASSERT_THIS();

    if(_this->min == (int32_t)val)
        return LE_SUCCESS;

    invalidateHandle(_this);

    _this->min = val;

    invalidateHandle(_this);

    if(_this->valueChangedEvent != NULL)
    {
        _this->valueChangedEvent(_this);
    }

#if LE_DEBUG == 1
    _leDebugNotify_WidgetPropertyChanged((leWidget*)_this);
#endif

    return LE_SUCCESS;
}

static uint32_t getMaximumValue(const leSliderWidget* _this)
{
    LE_ASSERT_THIS();

    return _this->max;
}

static leResult setMaximumValue(leSliderWidget* _this,
                                uint32_t val)
{
    LE_ASSERT_THIS();

    if(_this->max == (int32_t)val)
        return LE_SUCCESS;

    invalidateHandle(_this);

    _this->max = val;

    invalidateHandle(_this);

    if(_this->valueChangedEvent != NULL)
    {
        _this->valueChangedEvent(_this);
    }

#if LE_DEBUG == 1
    _leDebugNotify_WidgetPropertyChanged((leWidget*)_this);
#endif

    return LE_SUCCESS;
}

static uint32_t getPercentage(const leSliderWidget* _this)
{
    LE_ASSERT_THIS();

    return lePercentWholeRounded(_this->value - _this->min, _this->max - _this->min);
}

static leResult setPercentage(leSliderWidget* _this,
                              uint32_t val)
{
    uint32_t value;

    LE_ASSERT_THIS();

    value = lePercentOf(_this->max - _this->min, val) + _this->min;

    if(_this->value == (int32_t)value)
        return LE_SUCCESS;

    invalidateHandle(_this);

    _this->value = value;

    invalidateHandle(_this);

    if(_this->valueChangedEvent != NULL)
    {
        _this->valueChangedEvent(_this);
    }

#if LE_DEBUG == 1
    _leDebugNotify_WidgetPropertyChanged((leWidget*)_this);
#endif

    return LE_SUCCESS;
}

static int32_t getValue(const leSliderWidget* _this)
{
    LE_ASSERT_THIS();

    return _this->value;
}

static leResult setValue(leSliderWidget* _this,
                         int32_t val)
{
    LE_ASSERT_THIS();

    if(_this->value == val)
        return LE_SUCCESS;

    if(val > _this->max)
    {
        val = _this->max;
    }

    if(val < _this->min)
    {
        val = _this->min;
    }

    invalidateHandle(_this);

    _this->value = val;

    invalidateHandle(_this);

    if(_this->valueChangedEvent != NULL)
    {
        _this->valueChangedEvent(_this);
    }

#if LE_DEBUG == 1
    _leDebugNotify_WidgetPropertyChanged((leWidget*)_this);
#endif

    return LE_SUCCESS;
}

static leResult step(leSliderWidget* _this,
                     int32_t amount)
{
    LE_ASSERT_THIS();

    if(amount == 0)
        return LE_FAILURE;

    invalidateHandle(_this);

    if(amount < 0)
    {
        _this->value -= amount;

        if(_this->value < _this->min)
        {
            _this->value = _this->min;
        }
    }
    else
    {
        _this->value += amount;

        if(_this->value > _this->max)
        {
            _this->value = _this->max;
        }
    }

    if(_this->valueChangedEvent != NULL)
    {
        _this->valueChangedEvent(_this);
    }

    //printf("%u\n", _this->value);

    invalidateHandle(_this);

    return LE_SUCCESS;
}

static leSliderWidget_ValueChangedEvent getValueChangedEventCallback(const leSliderWidget* _this)
{
    LE_ASSERT_THIS();

    return _this->valueChangedEvent;
}

static leResult setValueChangedEventCallback(leSliderWidget* _this,
                                             leSliderWidget_ValueChangedEvent cb)
{
    LE_ASSERT_THIS();

    if(_this->valueChangedEvent == cb)
        return LE_FAILURE;

    _this->valueChangedEvent = cb;

#if LE_DEBUG == 1
    _leDebugNotify_WidgetPropertyChanged((leWidget*)_this);
#endif

    return LE_SUCCESS;
}

static void handleTouchDownEvent(leSliderWidget* _this,
                                 leWidgetEvent_TouchDown* evt)
{
    leRect rect;
    lePoint pnt;
    uint32_t percent;
    uint32_t value;

    LE_ASSERT_THIS();

    pnt.x = evt->x;
    pnt.y = evt->y;

    // already guaranteed to be inside widget rectangle, accept event
    leWidgetEvent_Accept((leWidgetEvent*)evt, (leWidget*)_this);

    // was the handle touched
    _leSliderWidget_GetHandleRect(_this, &rect);
    leUtils_RectToScreenSpace((leWidget*)_this, &rect);

    if(leRectContainsPoint(&rect, &pnt) == LE_TRUE)
    {
        _this->handleDownOffset.x = evt->x - rect.x - (rect.width / 2);
        _this->handleDownOffset.y = evt->y - rect.y - (rect.height / 2);
    }
    else
    {
        _this->handleDownOffset.x = 0;
        _this->handleDownOffset.y = 0;

        percent = _getPercentFromPoint(_this, &pnt);

        value = _getValueFromPercent(_this, percent);

        if(_this->value != (int32_t)value)
        {
            invalidateHandle(_this);

            _this->value = value;

            invalidateHandle(_this);

            if(_this->valueChangedEvent != NULL)
            {
                _this->valueChangedEvent(_this);
            }
        }
    }

    _this->state = LE_SLIDER_STATE_HANDLE_DOWN;
}

static void handleTouchUpEvent(leSliderWidget* _this,
                               leWidgetEvent_TouchUp* evt)
{
    LE_ASSERT_THIS();

    leWidgetEvent_Accept((leWidgetEvent*)evt, (leWidget*)_this);

    _this->state = LE_SLIDER_STATE_NONE;
}

static void handleTouchMovedEvent(leSliderWidget* _this,
                                  leWidgetEvent_TouchMove* evt)
{
    lePoint pnt;
    uint32_t percent;

    LE_ASSERT_THIS();

    pnt.x = evt->x;
    pnt.y = evt->y;

    leWidgetEvent_Accept((leWidgetEvent*)evt, (leWidget*)_this);

    if(_this->state == LE_SLIDER_STATE_HANDLE_DOWN)
    {
        pnt.x = evt->x - _this->handleDownOffset.x;
        pnt.y = evt->y - _this->handleDownOffset.y;

        //printf("%i, %i\n", pnt.x, pnt.y);

        percent = _getPercentFromPoint(_this, &pnt);

        //printf("%u\n", percent);

        percent = _getValueFromPercent(_this, percent);

        //printf("    %u, %u\n", percent, i);

        if((int32_t)percent != _this->value)
        {
            invalidateHandle(_this);

            _this->value = percent;

            invalidateHandle(_this);

            if(_this->valueChangedEvent != NULL)
            {
                _this->valueChangedEvent(_this);
            }
        }
    }
}

void _leSliderWidget_Paint(leSliderWidget* _this);

#if LE_DYNAMIC_VTABLES == 1
void _leWidget_FillVTable(leWidgetVTable* tbl);

void _leSliderWidget_GenerateVTable()
{
    _leWidget_FillVTable((void*)&sliderWidgetVTable);

    /* overrides from base class */
    sliderWidgetVTable._destructor = destructor;
    sliderWidgetVTable._paint = _leSliderWidget_Paint;
    sliderWidgetVTable._invalidateBorderAreas = _leSliderWidget_InvalidateBorderAreas;
    sliderWidgetVTable.touchDownEvent = handleTouchDownEvent;
    sliderWidgetVTable.touchUpEvent = handleTouchUpEvent;
    sliderWidgetVTable.touchMoveEvent = handleTouchMovedEvent;

    /* member functions */
    sliderWidgetVTable.getOrientation = getOrientation;
    sliderWidgetVTable.setOrientation = setOrientation;
    sliderWidgetVTable.getGripSize = getGripSize;
    sliderWidgetVTable.setGripSize = setGripSize;
    sliderWidgetVTable.getMininumValue = getMininumValue;
    sliderWidgetVTable.setMinimumValue = setMinimumValue;
    sliderWidgetVTable.getMaximumValue = getMaximumValue;
    sliderWidgetVTable.setMaximumValue = setMaximumValue;
    sliderWidgetVTable.getPercentage = getPercentage;
    sliderWidgetVTable.setPercentage = setPercentage;
    sliderWidgetVTable.getValue = getValue;
    sliderWidgetVTable.setValue = setValue;
    sliderWidgetVTable.step = step;
    sliderWidgetVTable.getValueChangedEventCallback = getValueChangedEventCallback;
    sliderWidgetVTable.setValueChangedEventCallback = setValueChangedEventCallback;
}

void _leSliderWidget_FillVTable(leSliderWidgetVTable* tbl)
{
    *tbl = sliderWidgetVTable;
}
#else
static const leSliderWidgetVTable sliderWidgetVTable =
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
    ._destructor = destructor,
    ._paint = _leSliderWidget_Paint,
    ._invalidateBorderAreas = _leSliderWidget_InvalidateBorderAreas,
    .touchDownEvent = handleTouchDownEvent,
    .touchUpEvent = handleTouchUpEvent,
    .touchMoveEvent = handleTouchMovedEvent,

    /* member functions */
    .getOrientation = getOrientation,
    .setOrientation = setOrientation,
    .getGripSize = getGripSize,
    .setGripSize = setGripSize,
    .getMininumValue = getMininumValue,
    .setMinimumValue = setMinimumValue,
    .getMaximumValue = getMaximumValue,
    .setMaximumValue = setMaximumValue,
    .getPercentage = getPercentage,
    .setPercentage = setPercentage,
    .getValue = getValue,
    .setValue = setValue,
    .step = step,
    .getValueChangedEventCallback = getValueChangedEventCallback,
    .setValueChangedEventCallback = setValueChangedEventCallback,
};
#endif

#endif // LE_SLIDER_WIDGET_ENABLED