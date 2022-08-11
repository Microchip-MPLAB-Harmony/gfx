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
#include "gfx/legato/widget/radialmenu/legato_widget_radial_menu.h"

#if LE_RADIALMENU_WIDGET_ENABLED

#include "gfx/legato/common/legato_error.h"
#include "gfx/legato/common/legato_math.h"
#include "gfx/legato/common/legato_utils.h"
#include "gfx/legato/memory/legato_memory.h"
#include "gfx/legato/string/legato_string.h"
#include "gfx/legato/widget/legato_widget.h"

#if LE_DEBUG == 1
#include "gfx/legato/core/legato_debug.h"
#endif

#define DEFAULT_WIDTH           200
#define DEFAULT_HEIGHT          150
#define DEFAULT_A               100
#define DEFAULT_B               30
#define DEFAULT_THETA           0

#define MAX_SIZE_PERCENT         200
#define MIN_SIZE_PERCENT         1
#define DEFAULT_MAX_SIZE_PERCENT 100
#define DEFAULT_MIN_SIZE_PERCENT 30

#define MAX_ALPHA_PERCENT           255
#define MIN_ALPHA_PERCENT           0
#define DEFAULT_MAX_ALPHA_SCALE     255
#define DEFAULT_MIN_ALPHA_SCALE     128

#define MIN_THETA                   -360
#define MAX_THETA                   360

#define DEFAULT_DRAG_DELTA        10
#define MAX_MOMENTUM              30
#define DEFAULT_MOMENTUM_FALLOFF  1
#define DEFAULT_ROTATION_TICK     60

#define SNAP_DELAY                750
#define SNAP_TICK                 10
#define SNAP_MAXSPEED             10

enum SnapState
{
    SNAP_NONE,
    SNAP_PENDING,
    SNAP_INPROCESS
};

static
#if LE_DYNAMIC_VTABLES == 0
const
#endif
leRadialMenuWidgetVTable radialMenuWidgetVTable;

void _leRadialMenu_RebuildEllipse(leRadialMenuWidget* wgt);
void _leRadialMenu_RecalculateArrangement(leRadialMenuWidget* wgt);

static void _recalculateTouchRect(leRadialMenuWidget* wgt)
{
    wgt->touchArea.x = lePercentOf(wgt->fn->getWidth(wgt), wgt->touchX);
    wgt->touchArea.y = lePercentOf(wgt->fn->getHeight(wgt), wgt->touchY);
    wgt->touchArea.width = lePercentOf(wgt->fn->getWidth(wgt), wgt->touchWidth);
    wgt->touchArea.height = lePercentOf(wgt->fn->getHeight(wgt), wgt->touchHeight);
}

static leBool filterWidgetEvent(leWidget* wgt, leWidgetEvent* evt, void* data)
{
    leRadialMenuWidget* _this = (leRadialMenuWidget*)data;
    leWidget* prominent = _this->fn->getProminent(_this);
    leBool stationary = _this->rotationDegrees == 0;

    if(evt->id == LE_EVENT_TOUCH_DOWN ||
       evt->id == LE_EVENT_TOUCH_UP ||
       evt->id == LE_EVENT_TOUCH_MOVE)
    {
        if(stationary == LE_FALSE || wgt != prominent)
            return LE_TRUE;

        return LE_FALSE;
    }

    return LE_FALSE;
}

void leRadialMenuWidget_Constructor(leRadialMenuWidget* _this)
{
    leWidget_Constructor((leWidget*)_this);
    
    _this->widget.fn = (void*)&radialMenuWidgetVTable;
    _this->fn = &radialMenuWidgetVTable;

    _this->widget.type = LE_WIDGET_RADIAL_MENU;
    
    _this->widget.rect.width = DEFAULT_WIDTH;
    _this->widget.rect.height = DEFAULT_HEIGHT;

    _this->prominentIndex = 0;
    _this->highlightProminent = LE_FALSE;

    _this->drawEllipse = LE_TRUE;

    _this->widget.style.borderType = LE_WIDGET_BORDER_NONE;
    _this->widget.style.backgroundType = LE_WIDGET_BACKGROUND_NONE;

    _this->widget.style.halign = LE_HALIGN_CENTER;
    _this->widget.style.valign = LE_VALIGN_MIDDLE;

    _this->ellipse.a = DEFAULT_A;
    _this->ellipse.b = DEFAULT_B;
    _this->ellipse.theta = DEFAULT_THETA;
    _this->ellipse.invalid = LE_TRUE;
    _this->positionsInvalid = LE_TRUE;
    _this->itemsShown = 5;

    _this->scaleMode = LE_RADIAL_MENU_INTERPOLATE_GRADUAL;
    _this->blendMode = LE_RADIAL_MENU_INTERPOLATE_GRADUAL;
    
    _this->maxSizePercent = DEFAULT_MAX_SIZE_PERCENT;
    _this->minSizePercent = DEFAULT_MIN_SIZE_PERCENT;
    
    _this->maxAlphaAmount = DEFAULT_MAX_ALPHA_SCALE;
    _this->minAlphaAmount = DEFAULT_MIN_ALPHA_SCALE;

    _this->touchX = 0;
    _this->touchY = 0;
    _this->touchWidth = 100;
    _this->touchHeight = 100;

    _this->momentum = 0;
    _this->maxMomentum = 0;
    _this->momentumFalloff = DEFAULT_MOMENTUM_FALLOFF;
    _this->snapState = SNAP_NONE;
    _this->rotation = 0;
    _this->rotationCounter = 0;
    _this->rotationTick = 0;

    _this->touchPressed = LE_FALSE;
    _this->rotationDegrees = 0;
    _this->angleSlice = 0;

    leList_Create(&_this->widgetList);

    _recalculateTouchRect(_this);
}

leRadialMenuWidget* leRadialMenuWidget_New()
{
    leRadialMenuWidget* mn = NULL;

    mn = LE_MALLOC(sizeof(leRadialMenuWidget));

    if(mn == NULL)
        return mn;
        
    leRadialMenuWidget_Constructor(mn);

    return mn;
}

void _leWidget_Destructor(leWidget* wgt);

static void destructor(leRadialMenuWidget* _this)
{
    leRadialMenuItemNode* item;

    while(_this->widgetList.size > 0)
    {
        item = leList_Get(&_this->widgetList, 0);

        // free all widgets
        if(item->widget != NULL)
        {
            leWidget_Delete(item->widget);
        }
    }

    // free the widget item list
    leList_Destroy(&_this->widgetList);

    _leWidget_Destructor((leWidget*)_this);
}

static void handleResizedEvent(leRadialMenuWidget* _this,
                               leWidget_ResizeEvent* evt)
{
    LE_ASSERT_THIS();

    (void)evt; // unused

    _recalculateTouchRect(_this);

    _this->ellipse.invalid = LE_TRUE;
}

static void rotateMenu(leRadialMenuWidget* _this, int32_t mag)
{
    _this->rotationDegrees += mag;
    int32_t chunk = _this->angleSlice >> 1;

    //printf("%i:%i:%i\n", -chunk, _this->rotationDegrees, chunk);

    while(_this->rotationDegrees > chunk)
    {
        _this->prominentIndex -= 1;
        _this->rotationDegrees -= _this->angleSlice;

        if(_this->prominentIndex < 0)
        {
            _this->prominentIndex = _this->widgetList.size - 1;
        }
    }

    while(_this->rotationDegrees <- chunk)
    {
        _this->prominentIndex += 1;
        _this->rotationDegrees += _this->angleSlice;

        if(_this->prominentIndex >= (int32_t)_this->widgetList.size)
        {
            _this->prominentIndex = 0;
        }
    }

    if(_this->rotationDegrees == 0 && _this->itemProminenceChangedEvent != NULL)
    {
        _this->itemProminenceChangedEvent(_this, _this->fn->getProminent(_this), _this->prominentIndex);
    }

    /*if(_this->rotationDegrees > chunk)
    {
        _this->prominentIndex -= 1;
        _this->rotationDegrees -= _this->angleSlice;

        if(_this->prominentIndex < 0)
        {
            _this->prominentIndex = _this->widgetList.size - 1;
        }
    }
    else if(_this->rotationDegrees < -chunk)
    {
        _this->prominentIndex += 1;
        _this->rotationDegrees += _this->angleSlice;

        if(_this->prominentIndex >= _this->widgetList.size)
        {
            _this->prominentIndex = 0;
        }
    }*/

    _this->positionsInvalid = LE_TRUE;
}

static void snapRotation(leRadialMenuWidget* _this,
                         uint32_t ms)
{
    if(_this->snapCounter < SNAP_TICK)
    {
        _this->snapCounter += ms;

        return;
    }

    _this->snapCounter = 0;

    if(_this->rotationDegrees <= 1 && _this->rotationDegrees >= -1)
    {
        _this->snapState = SNAP_NONE;

        rotateMenu(_this, -_this->rotationDegrees);
    }
    else
    {
        rotateMenu(_this, -(_this->rotationDegrees / 2));
    }

    _this->positionsInvalid = LE_TRUE;

    _this->fn->invalidate(_this);
}

static void updateRotation(leRadialMenuWidget* _this,
                           uint32_t ms)
{
    uint32_t absMomentum = leAbsoluteValue(_this->momentum);
    uint32_t sig, dec;
    uint32_t fallOff;

    _this->rotationCounter += ms;

    _this->rotationCounter = 0;

    // if the user has a finger on the wheel, we should increase the friction
    if (_this->touchPressed == LE_TRUE)
    {
        fallOff = _this->momentumFalloff * 4;
    }
    else
    {
        fallOff = _this->momentumFalloff;
    }

    if (fallOff > 100)
    {
        fallOff = 100;
    }

    lePercentOfDec(absMomentum,
                   100 - fallOff,
                   &sig,
                   &dec);

    if(_this->momentum < 0)
    {
        _this->momentum = -(int32_t)sig;

        rotateMenu(_this, -_this->momentum);
    }
    else
    {
        _this->momentum = sig;

        rotateMenu(_this, -_this->momentum);
    }

    // below reasonable threshold, stop rotation
    if(_this->momentum == 0 && _this->touchPressed == LE_FALSE && _this->rotationDegrees != 0)
    {
        _this->snapState = SNAP_PENDING;
        _this->snapCounter = SNAP_DELAY;

        return;
    }
}

static void update(leRadialMenuWidget* _this, uint32_t ms)
{
    if(_this->ellipse.invalid == LE_TRUE)
    {
        _leRadialMenu_RebuildEllipse(_this);
    }
    else if(_this->positionsInvalid == LE_TRUE)
    {
        _leRadialMenu_RecalculateArrangement(_this);
    }

    if(_this->momentum != 0)
    {
        updateRotation(_this, ms);
    }

    if(_this->snapState == SNAP_PENDING)
    {
        _this->snapCounter -= ms;

        if(_this->snapCounter < 0)
        {
            _this->snapState = SNAP_INPROCESS;
            _this->snapCounter = 0;
        }
    }
    else if(_this->snapState == SNAP_INPROCESS)
    {
        snapRotation(_this, ms);
    }
}

static leRadialMenuItemNode* _getWidgetItem(const leRadialMenuWidget* _this,
                                            const leWidget* widget)
{
    uint32_t i;
    leRadialMenuItemNode* item = NULL;
    
    LE_ASSERT_THIS();
    
    if(_this->widgetList.size == 0)
        return NULL;
    
    for(i = 0; i < _this->widgetList.size; ++i)
    {
        item = leList_Get(&_this->widgetList, i);
        
        if (item != NULL)
        {
            if(item->widget == widget)
                return item;
        }
    }
    
    return NULL;
}

static leResult setHighlightProminent(leRadialMenuWidget* _this,
                                      leBool enable)
{
    LE_ASSERT_THIS();
    
    _this->highlightProminent = enable;

    _this->fn->invalidate(_this);

#if LE_DEBUG == 1
    _leDebugNotify_WidgetPropertyChanged((leWidget*)_this);
#endif

    return LE_SUCCESS;
}

static leBool isProminent(const leRadialMenuWidget* _this,
                          const leWidget* widget)
{
    leRadialMenuItemNode* item = NULL;

    LE_ASSERT_THIS();

    item = _getWidgetItem(_this, widget);
    
    if(item == NULL)
        return LE_FALSE;

    return item->state == LE_RMI_PROMINENT;
}

static leWidget* getProminent(const leRadialMenuWidget* _this)
{
    leRadialMenuItemNode* item = NULL;

    LE_ASSERT_THIS();

    item = leList_Get(&_this->widgetList, _this->prominentIndex);

    if(item == NULL)
        return LE_FALSE;

    return item->widget;
}

static leResult setProminent(leRadialMenuWidget* _this,
                             const leWidget* widget)
{
    leRadialMenuItemNode* item = NULL;
    
    LE_ASSERT_THIS();

    item = _getWidgetItem(_this, widget);
    
    if(item == NULL)
        return LE_FALSE;

#if LE_DEBUG == 1
    _leDebugNotify_WidgetPropertyChanged((leWidget*)_this);
#endif

    return _this->fn->setProminentIndex(_this, leList_Find(&_this->widgetList, item));
}

static int32_t getProminentIndex(const leRadialMenuWidget* _this)
{
    LE_ASSERT_THIS();
    
    if(_this->widgetList.size == 0)
        return -1;
    
    return _this->prominentIndex;
}

static leResult setProminentIndex(leRadialMenuWidget* _this,
                                  int32_t index)
{
    LE_ASSERT_THIS();

    if(_this->widgetList.size == 0)
        return LE_FAILURE;

    _this->prominentIndex = index;

    _this->positionsInvalid = LE_TRUE;
    
    _this->fn->invalidate(_this);

#if LE_DEBUG == 1
    _leDebugNotify_WidgetPropertyChanged((leWidget*)_this);
#endif

    return LE_SUCCESS;
}

static leResult setNumberOfItemsShown(leRadialMenuWidget* _this,
                                      uint32_t num)
{
    LE_ASSERT_THIS();

    if (_this->itemsShown == num || num < 3 || num > _this->widgetList.size || num % 2 != 1)
        return LE_FAILURE;
    
    _this->itemsShown = num;
    
    _this->positionsInvalid = LE_TRUE;
    
    _this->fn->invalidate(_this);

#if LE_DEBUG == 1
    _leDebugNotify_WidgetPropertyChanged((leWidget*)_this);
#endif

    return LE_SUCCESS;
}

static leResult addChild(leRadialMenuWidget* _this,
                          leWidget* widget)
{
    leRadialMenuItemNode* item = NULL;
    leWidgetEventFilter filter;
    
    LE_ASSERT_THIS();
    
    if(widget->parent == (leWidget*)_this)
        return LE_FAILURE;
    
    item = LE_MALLOC(sizeof(leRadialMenuItemNode));

    LE_ASSERT(item != NULL);

    if(item == NULL)
        return LE_FAILURE;

    item->widget = widget;

    filter.filterEvent = filterWidgetEvent;
    filter.data = _this;

    widget->fn->installEventFilter(widget, filter);

    item->origAlphaAmount = widget->style.alphaAmount;
    item->origSize = widget->rect;
    
    leList_PushBack(&_this->widgetList, item);
    
    _leWidget_AddChild((leWidget*)_this, widget);

    _this->positionsInvalid = LE_TRUE;
    _this->ellipse.invalid = LE_TRUE;

    _this->fn->invalidate(_this);

#if LE_DEBUG == 1
    _leDebugNotify_WidgetPropertyChanged((leWidget*)_this);
#endif

    return LE_SUCCESS;
}

static leResult insertChild(leRadialMenuWidget* _this,
                            leWidget* widget,
                            uint32_t idx)
{
    leRadialMenuItemNode* item = NULL;
    leWidgetEventFilter filter;

    LE_ASSERT_THIS();

    if(widget->parent == (leWidget*)_this)
        return LE_FAILURE;

    item = LE_MALLOC(sizeof(leRadialMenuItemNode));

    LE_ASSERT(item != NULL);

    if(item == NULL)
        return LE_FAILURE;

    item->widget = widget;

    filter.filterEvent = filterWidgetEvent;
    filter.data = _this;

    widget->fn->installEventFilter(widget, filter);

    item->origAlphaAmount = widget->style.alphaAmount;
    item->origSize = widget->rect;

    leList_InsertAt(&_this->widgetList, item, idx);

    _leWidget_InsertChild((leWidget*)_this, widget, idx);

    _this->positionsInvalid = LE_TRUE;
    _this->ellipse.invalid = LE_TRUE;

    _this->fn->invalidate(_this);

#if LE_DEBUG == 1
    _leDebugNotify_WidgetPropertyChanged((leWidget*)_this);
#endif

    return LE_SUCCESS;
}

static leResult removeChild(leRadialMenuWidget* _this,
                            leWidget* widget)
{
    leRadialMenuItemNode* item = NULL;
    leWidgetEventFilter filter;

    LE_ASSERT_THIS();
    
    if(_this->widgetList.size == 0 || widget == NULL)
        return LE_FAILURE;
    
    item = _getWidgetItem(_this, widget);
    
    if(item == NULL)
        return LE_FAILURE;

    filter.filterEvent = filterWidgetEvent;
    filter.data = _this;

    widget->fn->removeEventFilter(widget, filter);
    
    leList_Remove(&_this->widgetList, item);
    
    LE_FREE(item);
    item = NULL;

    _leWidget_RemoveChild((leWidget*)_this, widget);

    if(_this->itemsShown > _this->widgetList.size)
    {
        _this->itemsShown = _this->widgetList.size;
    }

    _this->positionsInvalid = LE_TRUE;
    _this->ellipse.invalid = LE_TRUE;

    _this->fn->invalidate(_this);

#if LE_DEBUG == 1
    _leDebugNotify_WidgetPropertyChanged((leWidget*)_this);
#endif

    return LE_SUCCESS;    
}

static leResult removeChildAt(leRadialMenuWidget* _this,
                              uint32_t idx)
{
    leRadialMenuItemNode* item = NULL;
    leWidgetEventFilter filter;

    LE_ASSERT_THIS();

    if(idx >= _this->widgetList.size)
        return LE_FAILURE;

    item = leList_Get(&_this->widgetList, idx);

    if(item == NULL)
        return LE_FAILURE;

    filter.filterEvent = filterWidgetEvent;
    filter.data = _this;

    item->widget->fn->removeEventFilter(item->widget, filter);

    leList_RemoveAt(&_this->widgetList, idx);

    _leWidget_RemoveChild((leWidget*)_this, item->widget);

    LE_FREE(&item);
    item = NULL;

    if(_this->itemsShown > _this->widgetList.size)
    {
        _this->itemsShown = _this->widgetList.size;
    }

    _this->positionsInvalid = LE_TRUE;
    _this->ellipse.invalid = LE_TRUE;

    _this->fn->invalidate(_this);

#if LE_DEBUG == 1
    _leDebugNotify_WidgetPropertyChanged((leWidget*)_this);
#endif

    return LE_SUCCESS;
}

static leResult removeAllChildren(leRadialMenuWidget* _this)
{
    uint32_t i = 0;
    leRadialMenuItemNode* item = NULL;
    leWidgetEventFilter filter;
    leWidget* wgt;
    
    LE_ASSERT_THIS();

    filter.filterEvent = filterWidgetEvent;
    filter.data = _this;
    
    for(i = 0; i < _this->widgetList.size; ++i)
    {
        item = leList_Get(&_this->widgetList, i);
        
        if (item != NULL)
        {
            LE_FREE(item);
            
            item = NULL;
        }
    }
    
    leList_Clear(&_this->widgetList);

    for(i = 0; i < _this->widget.children.size; ++i)
    {
        wgt = leArray_Get(&_this->widget.children, i);

        wgt->fn->removeEventFilter(wgt, filter);
    }

    _leWidget_RemoveAllChildren((leWidget*)_this);

    _this->itemsShown = 3;

    _this->positionsInvalid = LE_TRUE;
    _this->ellipse.invalid = LE_TRUE;

    _this->fn->invalidate(_this);

#if LE_DEBUG == 1
    _leDebugNotify_WidgetPropertyChanged((leWidget*)_this);
#endif

    return LE_SUCCESS;    
}

static leResult setScaleMode(leRadialMenuWidget* _this,
                             leRadialMenuWidgetInterpolationMode mode)
{
    LE_ASSERT_THIS();

    if(_this->scaleMode == mode)
        return LE_FAILURE;

    _this->scaleMode = mode;

    _this->fn->invalidate(_this);

    _this->positionsInvalid = LE_TRUE;

    _this->state = LE_RADIAL_MENU_RESET_TO_INPUT_POS;

#if LE_DEBUG == 1
    _leDebugNotify_WidgetPropertyChanged((leWidget*)_this);
#endif

    return LE_SUCCESS;    
}

static leResult setScaleRange(leRadialMenuWidget* _this,
                              int32_t min,
                              int32_t max)
{
    LE_ASSERT_THIS();

    if(_this->maxSizePercent == max && _this->minSizePercent == min)
        return LE_FAILURE;
    
    _this->maxSizePercent = leClampi(MIN_SIZE_PERCENT, MAX_SIZE_PERCENT, max);
    _this->minSizePercent = leClampi(MIN_SIZE_PERCENT, MAX_SIZE_PERCENT, min);

    _this->positionsInvalid = LE_TRUE;

    _this->fn->invalidate(_this);

#if LE_DEBUG == 1
    _leDebugNotify_WidgetPropertyChanged((leWidget*)_this);
#endif

    return LE_SUCCESS;    
}

static leResult setBlendMode(leRadialMenuWidget* _this,
                             leRadialMenuWidgetInterpolationMode mode)
{
    uint32_t i = 0;
    leRadialMenuItemNode* item = NULL;

    LE_ASSERT_THIS();

    if(_this->blendMode == mode)
        return LE_FAILURE;

    _this->blendMode = mode;
    
    for(i = 0; i < _this->widgetList.size; ++i)
    {
        item = leList_Get(&_this->widgetList, i);

        if (item != NULL)
        {
            item->widget->fn->setAlphaEnabled(item->widget, mode != LE_RADIAL_MENU_INTERPOLATE_OFF);
        }
    }
    
    _this->fn->invalidate(_this);

    //_this->positionsInvalid = LE_TRUE;

    _this->state = LE_RADIAL_MENU_RESET_TO_INPUT_POS;

#if LE_DEBUG == 1
    _leDebugNotify_WidgetPropertyChanged((leWidget*)_this);
#endif

    return LE_SUCCESS;    
}

static leResult setBlendRange(leRadialMenuWidget* _this,
                              int32_t min,
                              int32_t max)
{
    LE_ASSERT_THIS();

    if(_this->maxAlphaAmount == max && _this->minAlphaAmount == min)
        return LE_FAILURE;
    
    _this->maxAlphaAmount = leClampi(MIN_ALPHA_PERCENT, MAX_ALPHA_PERCENT, max);
    _this->minAlphaAmount = leClampi(MIN_ALPHA_PERCENT, MAX_ALPHA_PERCENT, min);

    _this->positionsInvalid = LE_TRUE;

    _this->fn->invalidate(_this);

#if LE_DEBUG == 1
    _leDebugNotify_WidgetPropertyChanged((leWidget*)_this);
#endif

    return LE_SUCCESS;    
}

static leResult setDrawEllipse(leRadialMenuWidget* _this,
                               leBool enable)
{
    LE_ASSERT_THIS();

    if(_this->drawEllipse == enable)
        return LE_FAILURE;
    
    _this->drawEllipse = enable;
    
    _this->fn->invalidate(_this);

#if LE_DEBUG == 1
    _leDebugNotify_WidgetPropertyChanged((leWidget*)_this);
#endif

    return LE_SUCCESS;
}

static leResult setTouchArea(leRadialMenuWidget* _this, 
                             int32_t x,
                             int32_t y,
                             int32_t width,
                             int32_t height)
{
    LE_ASSERT_THIS();

    _this->touchX = x;
    _this->touchY = y;
    _this->touchWidth = width;
    _this->touchHeight = height;

    _recalculateTouchRect(_this);

#if LE_DEBUG == 1
    _leDebugNotify_WidgetPropertyChanged((leWidget*)_this);
#endif

    return LE_SUCCESS;    
}

static int32_t getMajorAxis(const leRadialMenuWidget* _this)
{
    LE_ASSERT_THIS();

    return _this->ellipse.a;
}

static leResult setMajorAxis(leRadialMenuWidget* _this,
                             int32_t a)
{
    LE_ASSERT_THIS();

    if(_this->ellipse.a == a)
        return LE_FAILURE;

    _this->ellipse.a = a;

    _this->positionsInvalid = LE_TRUE;
    _this->ellipse.invalid = LE_TRUE;

    _this->fn->invalidate(_this);

#if LE_DEBUG == 1
    _leDebugNotify_WidgetPropertyChanged((leWidget*)_this);
#endif

    return LE_SUCCESS;
}

static int32_t getMinorAxis(const leRadialMenuWidget* _this)
{
    LE_ASSERT_THIS();

    return _this->ellipse.b;
}

static leResult setMinorAxis(leRadialMenuWidget* _this,
                             int32_t b)
{
    LE_ASSERT_THIS();

    if(_this->ellipse.b == b)
        return LE_FAILURE;

    _this->ellipse.b = b;

    _this->positionsInvalid = LE_TRUE;
    _this->ellipse.invalid = LE_TRUE;

    _this->fn->invalidate(_this);

#if LE_DEBUG == 1
    _leDebugNotify_WidgetPropertyChanged((leWidget*)_this);
#endif

    return LE_SUCCESS;
}

static int32_t getTheta(const leRadialMenuWidget* _this)
{
    LE_ASSERT_THIS();
    
    return _this->ellipse.theta;
}

static leResult setTheta(leRadialMenuWidget* _this,
                         int32_t theta)
{
    LE_ASSERT_THIS();

    if(_this->ellipse.theta == theta)
        return LE_FAILURE;

    _this->ellipse.theta = leClampi(MIN_THETA, MAX_THETA, theta);

    _this->positionsInvalid = LE_TRUE;
    _this->ellipse.invalid = LE_TRUE;

    _this->fn->invalidate(_this);

#if LE_DEBUG == 1
    _leDebugNotify_WidgetPropertyChanged((leWidget*)_this);
#endif

    return LE_SUCCESS;
}

static leRadialMenuWidget_ItemProminenceChangedEvent getItemProminenceChangedEventCallback(const leRadialMenuWidget* _this)
{
    LE_ASSERT_THIS();

    return _this->itemProminenceChangedEvent;
}

static leResult setItemProminenceChangedEventCallback(leRadialMenuWidget* _this,
                                                      leRadialMenuWidget_ItemProminenceChangedEvent cb)
{
    LE_ASSERT_THIS();
    
    _this->itemProminenceChangedEvent = cb;
    
    return LE_SUCCESS;
}

static void handleTouchDownEvent(leRadialMenuWidget* _this,
                                 leWidgetEvent_TouchDown* evt)
{
    //leRadialMenuItemNode* widgetItem;
    leRect clipRect;
    lePoint touchPoint;
    
    //if (_this->state != LE_RADIAL_MENU_INPUT_READY)
    //    return;

    /*if(_this->fn->getType(_this) == LE_WIDGET_RADIAL_MENU)
    {
        _this = (leRadialMenuWidget*)widget;


    }*/

    clipRect = _this->touchArea;

    _this->snapState = SNAP_NONE;

    leUtils_RectToScreenSpace((leWidget*)_this, &clipRect);

    touchPoint.x = evt->x;
    touchPoint.y = evt->y;

    // only respond if the event is in the touch allowed area
    if (leRectContainsPoint(&clipRect, &touchPoint) == LE_FALSE)
        return;

    _this->touchPressed = LE_TRUE;
    
    leWidgetEvent_Accept((leWidgetEvent*)evt, (leWidget*)_this);
}

static void handleTouchUpEvent(leRadialMenuWidget* _this,
                               leWidgetEvent_TouchUp* evt)
{
    if(_this->momentum == 0 && _this->rotationDegrees != 0)
    {
        _this->snapState = SNAP_PENDING;
        _this->snapCounter = SNAP_DELAY;
    }

    _this->touchPressed = LE_FALSE;
    
    leWidgetEvent_Accept((leWidgetEvent*)evt, (leWidget*)_this);
}

struct Vector2
{
    float x;
    float y;
};

#include <math.h>

static float vectorLength(const struct Vector2* vec)
{
    return leSqrt((vec->x * vec->x) + (vec->y * vec->y));
}

static void projectVector(struct Vector2* vec, const struct Vector2* ref)
{
    float f;
    float dot;
    float length;

    dot = (vec->x * ref->x) + (vec->y * ref->y);

    length = (ref->x * ref->x) + (ref->y * ref->y);
    f = dot / length;

    vec->x = ref->x * f;
    vec->y = ref->y * f;
}

static void rotateVector(struct Vector2* vec, float ang)
{
    float cs, sn, x, y;
    float rad = ang * (3.14159f / 180.0f);


    cs = cosf(rad);
    sn = sinf(rad);

    x = vec->x * cs - vec->y * sn;
    y = vec->x * sn + vec->y * cs;

    vec->x = x;
    vec->y = y;
}

static void handleTouchMovedEvent(leRadialMenuWidget* _this,
                                  leWidgetEvent_TouchMove* evt)
{
    leRect clipRect;
	struct Vector2 touchVector, rotationVector;
    float length, dot;
    int32_t rotationMagnitude;

    clipRect = _this->touchArea;

    leUtils_RectToScreenSpace((leWidget*)_this, &clipRect);

    rotationVector.x = 1.0f;
    rotationVector.y = 0.0f;

    rotateVector(&rotationVector, _this->ellipse.theta);

    touchVector.x = evt->x - evt->prevX;
    touchVector.y = evt->y - evt->prevY;

    dot = (touchVector.x * rotationVector.x) + (touchVector.y * rotationVector.y);

    projectVector(&touchVector, &rotationVector);

    length = vectorLength(&touchVector);

    if(length >= 0.75f && length < 1.0f)
    {
        length = 1.0f;
    }

    rotationMagnitude = (int32_t)length;

    if(dot < 0.0f)
    {
        rotationMagnitude *= -1;
    }

    _this->snapState = SNAP_NONE;

    if(rotationMagnitude > DEFAULT_DRAG_DELTA)
    {
        _this->momentum += rotationMagnitude;

        if(_this->momentum > MAX_MOMENTUM)
        {
            _this->momentum = MAX_MOMENTUM;
        }
    }
    else if(rotationMagnitude < -DEFAULT_DRAG_DELTA)
    {
        _this->momentum += rotationMagnitude;

        if(_this->momentum < -MAX_MOMENTUM)
        {
            _this->momentum = -MAX_MOMENTUM;
        }
    }
    else
    {
        rotateMenu(_this, -rotationMagnitude);
    }

    leWidgetEvent_Accept((leWidgetEvent*)evt, (leWidget*)_this);
}

void _leRadialMenuWidget_Paint(leRadialMenuWidget* _this);

#if LE_DYNAMIC_VTABLES == 1
void _leWidget_FillVTable(leWidgetVTable* tbl);

void _leRadialMenuWidget_GenerateVTable()
{
    _leWidget_FillVTable((void*)&radialMenuWidgetVTable);
    
    /* overrides from base class */
    radialMenuWidgetVTable._destructor = destructor;
    radialMenuWidgetVTable._paint = _leRadialMenuWidget_Paint;
    radialMenuWidgetVTable.update = update;
    radialMenuWidgetVTable.touchDownEvent = handleTouchDownEvent;
    radialMenuWidgetVTable.touchUpEvent = handleTouchUpEvent;
    radialMenuWidgetVTable.touchMoveEvent = handleTouchMovedEvent;
    radialMenuWidgetVTable.resizeEvent = handleResizedEvent;
    radialMenuWidgetVTable.addChild = addChild,
    radialMenuWidgetVTable.insertChild = insertChild,
    radialMenuWidgetVTable.removeChild = removeChild,
    radialMenuWidgetVTable.removeChildAt = removeChildAt,
    radialMenuWidgetVTable.removeAllChildren = (void*)removeAllChildren,
    
    /* member functions */
    radialMenuWidgetVTable.isProminent = isProminent;
    radialMenuWidgetVTable.getProminent = getProminent;
    radialMenuWidgetVTable.setProminent = setProminent;
    radialMenuWidgetVTable.getProminentIndex = getProminentIndex;
    radialMenuWidgetVTable.setProminentIndex = setProminentIndex;
    radialMenuWidgetVTable.setNumberOfItemsShown = setNumberOfItemsShown;
    radialMenuWidgetVTable.setHighlightProminent = setHighlightProminent;
    radialMenuWidgetVTable.getMajorAxis = getMajorAxis,
    radialMenuWidgetVTable.setMajorAxis = setMajorAxis,
    radialMenuWidgetVTable.getMinorAxis = getMinorAxis,
    radialMenuWidgetVTable.setMinorAxis = setMinorAxis,
    radialMenuWidgetVTable.getTheta = getTheta;
    radialMenuWidgetVTable.setTheta = setTheta;
    radialMenuWidgetVTable.setScaleMode = setScaleMode;
    radialMenuWidgetVTable.setScaleRange = setScaleRange;
    radialMenuWidgetVTable.setBlendMode = setBlendMode;
    radialMenuWidgetVTable.setBlendRange = setBlendRange;
    radialMenuWidgetVTable.setTouchArea = setTouchArea;
    radialMenuWidgetVTable.setDrawEllipse = setDrawEllipse;
    radialMenuWidgetVTable.getItemProminenceChangedEventCallback = getItemProminenceChangedEventCallback;
    radialMenuWidgetVTable.setItemProminenceChangedEventCallback = setItemProminenceChangedEventCallback;
}

void _leRadialMenuWidget_FillVTable(leRadialMenuWidgetVTable* tbl)
{
    *tbl = radialMenuWidgetVTable;
}
#else
static const leRadialMenuWidgetVTable radialMenuWidgetVTable =
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
    .addChild = (void*)addChild,
    .insertChild = (void*)insertChild,
    .removeChild = (void*)removeChild,
    .removeChildAt = (void*)removeChildAt,
    .removeAllChildren = (void*)removeAllChildren,
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
    .installEventFilter = (void*)_leWidget_InstallEventFilter,
    .removeEventFilter = (void*)_leWidget_RemoveEventFilter,

    .moveEvent = (void*)_leWidget_MoveEvent,
    .focusLostEvent = (void*)_leWidget_FocusLostEvent,
    .focusGainedEvent = (void*)_leWidget_FocusGainedEvent,

    ._handleEvent = (void*)_leWidget_HandleEvent,
    ._validateChildren = (void*)_leWidget_ValidateChildren,
    ._increaseDirtyState = (void*)_leWidget_IncreaseDirtyState,
    ._setDirtyState = (void*)_leWidget_SetDirtyState,
    ._clearDirtyState = (void*)_leWidget_ClearDirtyState,
    ._invalidateBorderAreas = (void*)_leWidget_InvalidateBorderAreas,
    ._damageArea = (void*)_leWidget_DamageArea,

    /* overrides from base class */
    ._destructor = destructor,
    ._paint = _leRadialMenuWidget_Paint,
    .update = update,
    .touchDownEvent = handleTouchDownEvent,
    .touchUpEvent = handleTouchUpEvent,
    .touchMoveEvent = handleTouchMovedEvent,
    .resizeEvent = handleResizedEvent,

    /* member functions */
    .isProminent = isProminent,
    .getProminent = getProminent,
    .setProminent = setProminent,
    .getProminentIndex = getProminentIndex,
    .setProminentIndex = setProminentIndex,
    .setNumberOfItemsShown = setNumberOfItemsShown,
    .setHighlightProminent = setHighlightProminent,
    .getMajorAxis = getMajorAxis,
    .setMajorAxis = setMajorAxis,
    .getMinorAxis = getMinorAxis,
    .setMinorAxis = setMinorAxis,
    .getTheta = getTheta,
    .setTheta = setTheta,
    .setScaleMode = setScaleMode,
    .setScaleRange = setScaleRange,
    .setBlendMode = setBlendMode,
    .setBlendRange = setBlendRange,
    .setTouchArea = setTouchArea,
    .setDrawEllipse = setDrawEllipse,
    .getItemProminenceChangedEventCallback = getItemProminenceChangedEventCallback,
    .setItemProminenceChangedEventCallback = setItemProminenceChangedEventCallback,
};
#endif

#endif // LE_RADIALMENU_WIDGET_ENABLED
