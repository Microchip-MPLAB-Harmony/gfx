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
#include "gfx/legato/widget/listwheel/legato_widget_listwheel.h"

#if LE_LISTWHEEL_WIDGET_ENABLED

#include "gfx/legato/common/legato_error.h"
#include "gfx/legato/common/legato_math.h"
#include "gfx/legato/common/legato_utils.h"
#include "gfx/legato/memory/legato_memory.h"
#include "gfx/legato/string/legato_string.h"
#include "gfx/legato/widget/legato_widget.h"

#if LE_DEBUG == 1
#include "gfx/legato/core/legato_debug.h"
#endif

#define DEFAULT_WIDTH             100
#define DEFAULT_HEIGHT            100

#define DEFAULT_MARGIN            10
#define DEFAULT_VISIBLE_COUNT     5

#define DEFAULT_INDICATOR_AREA    20

#define DEFAULT_DRAG_DELTA        20
#define MAX_MOMENTUM              100
#define DEFAULT_MOMENTUM_FALLOFF  2
#define DEFAULT_ROTATION_TICK     60

static
#if LE_DYNAMIC_VTABLES == 0
const
#endif
leListWheelWidgetVTable listWheelWidgetVTable;

static int32_t previousItem(int32_t size, int32_t idx)
{
    int32_t prevIdx = idx;
    
    if(prevIdx == 0)
    {
        prevIdx = size - 1;
    }
    else
    {
        prevIdx--;
    }
    
    return prevIdx;
}

static int32_t nextItem(int32_t size, int32_t idx)
{
    int32_t nextIdx = idx;
    
    if(nextIdx + 1 == size)
    {
        nextIdx = 0;
    }
    else
    {
        nextIdx++;
    }
    
    return nextIdx;
}

static void calculateTopItem(leListWheelWidget* _this)
{
    int32_t idx;
    int32_t item = _this->selectedItem;
    int32_t count = (_this->visibleItems / 2) + 1;
    
    _this->topItem = 0;
    
    if(_this->items.size == 0)
        return;
    
    for(idx = 0; idx < count; idx++)
    {
        item = previousItem(_this->items.size, item);
    }
        
    _this->topItem = item;
}

static int32_t calculateSelectedItem(leListWheelWidget* _this)
{
    int32_t idx;
    int32_t item = _this->topItem;
    int32_t count = (_this->visibleItems / 2) + 1;
    
    if(_this->items.size == 0)
        return item;
    
    for(idx = 0; idx < count; idx++)
    {
        item = nextItem(_this->items.size, item);
    }
        
    // account for spinning close to another value but not over it
    if(_this->rotation > 0 && _this->rotation > _this->cycleDistance / 2)
    {
        item = previousItem(_this->items.size, item);
    }
    else if(_this->rotation < 0 && _this->rotation < -_this->cycleDistance / 2)
    {
        item = nextItem(_this->items.size, item);
    }
        
    return item;
}

static void stringPreinvalidate(const leString* str,
                                leListWheelWidget* lst)
{
    (void)str; // unused

    lst->fn->invalidate(lst);
}

static void stringInvalidate(const leString* str,
                             leListWheelWidget* lst)
{
    (void)str; // unused

    lst->fn->invalidate(lst);
}

static void handleLanguageChangeEvent(leListWheelWidget* _this)
{
    LE_ASSERT_THIS();

    _this->fn->invalidate(_this);
}

void leListWheelWidget_Constructor(leListWheelWidget* _this)
{
    leWidget_Constructor((leWidget*)_this);
    
    _this->widget.fn = (void*)&listWheelWidgetVTable;
    _this->fn = &listWheelWidgetVTable;
    
    _this->widget.type = LE_WIDGET_LISTWHEEL;

    _this->widget.rect.width = DEFAULT_WIDTH;
    _this->widget.rect.height = DEFAULT_HEIGHT;

    _this->widget.style.borderType = LE_WIDGET_BORDER_BEVEL;
    _this->widget.style.backgroundType = LE_WIDGET_BACKGROUND_FILL;
    
    _this->borderTypeCache = _this->widget.style.borderType;
    _this->backgroundTypeCache = _this->widget.style.backgroundType;
    //_this->allowEmpty = LE_TRUE;
    

    //leString_Initialize(&_this->text);
    
    leArray_Create(&_this->items);
    
    _this->widget.style.halign = LE_HALIGN_CENTER;
    _this->iconPos = LE_RELATIVE_POSITION_LEFTOF;
    _this->iconMargin = DEFAULT_MARGIN;
    _this->visibleItems = DEFAULT_VISIBLE_COUNT;
    _this->selectedItem = -1;
    _this->topItem = -1;
    
    calculateTopItem(_this);
    //_this->scrollBarWidth = DEFAULT_SCROLL_WIDTH;

    _this->showIndicators = LE_TRUE;
    _this->indicatorArea = DEFAULT_INDICATOR_AREA;
    _this->shaded = LE_TRUE;
    
    _this->cycleDistance = leDivideRounding(_this->widget.rect.height,
                                            _this->visibleItems - 1);
    _this->cycleDelta = 0;
    _this->firstTouchY = 0;
    _this->touchY = 0;
    _this->lastTouchY = 0;

    //_this->cycleSteps = DEFAULT_CYCLE_STEPS;
    
    _this->minFlickDelta = DEFAULT_DRAG_DELTA;
    _this->rotationTick = DEFAULT_ROTATION_TICK;

    _this->stillTouching = LE_FALSE;
    _this->autoHideWheel = LE_FALSE;
    _this->hideWheel = LE_FALSE;
    _this->indicatorFill = LE_LISTWHEEL_INDICATOR_FILL_SOLID;
    _this->zoomEffects = LE_LISTWHEEL_ZOOM_EFFECT_NONE;

    _this->momentum = 0;
    _this->maxMomentum = 0;
    _this->momentumFalloff = DEFAULT_MOMENTUM_FALLOFF;
    _this->snapPending = LE_FALSE;
    _this->rotation = 0;
    _this->rotationCounter = 0;
    _this->rotationTick = 0;

    _this->cb = NULL;
}

void _leWidget_Destructor(leWidget* _this);

static void destructor(leListWheelWidget* _this)
{
    LE_ASSERT_THIS();
    
    _this->fn->removeAllItems(_this);
    
    _leWidget_Destructor((leWidget*)_this);
}

leListWheelWidget* leListWheelWidget_New()
{
    leListWheelWidget* whl = NULL;

    whl = LE_MALLOC(sizeof(leListWheelWidget));

    leListWheelWidget_Constructor(whl);

    return whl;
}

static void adjustRotation(leListWheelWidget* _this,
                           int32_t rot)
{
    uint32_t rem;
    uint32_t i;
    
    //printf("rot: %i\n", rot);
    
    _this->rotation += rot;
    
    rem = leAbsoluteValue(_this->rotation) / _this->cycleDistance;
    
    if(rem == 0 || rot == 0)
        return;
    
    // adjust top item as necessary
    if(rot < 0)
    {
        for(i = 0; i < rem; i++)
        {
            //printf("before: %i, %i", _this->topItem, _this->rotation);
            
            _this->topItem = nextItem(_this->items.size, _this->topItem);
            //_this->touchY -= (_this->cycleDistance * 2);
            
            _this->rotation += (_this->cycleDistance);
            
            //printf(" after: %i, %i\n", _this->topItem, _this->rotation);
            
            //if(_this->rotation > 1000)
            //    _this->rotation = _this->rotation;
        }
    }
    else
    {
        for(i = 0; i < rem; i++)
        {
            _this->topItem = previousItem(_this->items.size, _this->topItem);
            //_this->touchY += rem * (_this->cycleDistance * 2);
            
            _this->rotation -= (_this->cycleDistance);
        }
    } 
}

static void snapRotation(leListWheelWidget* _this)
{
    int32_t selectedItem;
    
    selectedItem = calculateSelectedItem(_this);
        
    _this->rotation = 0;
    _this->momentum = 0;
           
    // set selected item and issue callback
    if(selectedItem != _this->selectedItem)
    {
        _this->selectedItem = selectedItem;
        
        calculateTopItem(_this);
        
        if(_this->cb != NULL)
        {
            _this->cb(_this, _this->selectedItem);
        }
    }
    
    // reset draw state on invalidate in case the wheel is partially redrawn
    //_this->widget.status.drawState = LE_WIDGET_DRAW_STATE_READY;
        
    //printf("snap, %i, %i, %i\n", _this->rotation, _this->momentum, _this->paintState.offs);
}

static void updateRotation(leListWheelWidget* _this,
                           uint32_t ms)
{
    uint32_t absMomentum = leAbsoluteValue(_this->momentum);
    uint32_t sig, dec;
    uint32_t fallOff;
    
    _this->rotationCounter += ms;
    
    //if(_this->rotationCounter >= _this->rotationTick)
    //{
        //invalidateContents(_this);
        
        _this->rotationCounter = 0;
        
		// if the user has a finger on the wheel, we should increase the friction
        if (_this->stillTouching == LE_TRUE)
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
                         
        //printf("mom: %i\n", _this->momentum);
                         
        if(_this->momentum < 0)
        {
            _this->momentum = -(int32_t)sig;
            
            adjustRotation(_this, _this->momentum);
        }
        else
        {
            _this->momentum = sig;
            
            adjustRotation(_this, _this->momentum);
        }
        
        // below reasonable threshold, stop rotation
        if(_this->momentum == 0 && _this->stillTouching == LE_FALSE)
        {
            _this->snapPending = LE_TRUE;
            
            return;
        }
            
        //printf("mom: %i.%i\n", _this->momentum, _this->momentumDec);
        
        //invalidateContents(_this);
        
        _this->fn->invalidate(_this);
    //}
    
    
    
    //tickPer = tickPer;
}

static void update(leListWheelWidget* _this,
                   uint32_t ms)
{
    if(_this->momentum != 0)
    {
        updateRotation(_this, ms);
        
        if (_this->autoHideWheel == LE_TRUE &&
            _this->hideWheel == LE_TRUE)
        {
            //show wheel
            _this->hideWheel = LE_FALSE;
        
            _this->fn->setBorderType(_this, _this->borderTypeCache);
            _this->fn->setBackgroundType(_this, _this->backgroundTypeCache);                
        }
    }
        
    if(_this->snapPending == LE_TRUE)
    {
        //invalidateContents(_this);
        
        snapRotation(_this);
        
        //invalidateContents(_this);
        
        _this->fn->invalidate(_this);
        
        _this->snapPending = LE_FALSE;
        
        if (_this->autoHideWheel == LE_TRUE &&
            _this->hideWheel == LE_FALSE &&
            _this->stillTouching == LE_FALSE)
        {
            //Hide wheel
            _this->hideWheel = LE_TRUE;

            _this->borderTypeCache = _this->fn->getBorderType(_this);
            _this->backgroundTypeCache = _this->fn->getBackgroundType(_this);

            _this->fn->setBorderType(_this, LE_WIDGET_BORDER_NONE);
            _this->fn->setBackgroundType(_this, LE_WIDGET_BACKGROUND_NONE);           
        }
    }
}

static void handleResizedEvent(leListWheelWidget* _this,
                               leWidget_ResizeEvent* evt)
{
    (void)evt; // unused

    _this->cycleDistance = leDivideRounding(_this->widget.rect.height, _this->visibleItems - 1);
}

static void handleTouchDownEvent(leListWheelWidget* _this,
                                 leWidgetEvent_TouchDown* evt)
{
    _this->touchY = evt->y;
    _this->lastTouchY = evt->y;
    _this->firstTouchY = evt->y;
    
    //printf("down: %i\n", _this->touchY);
    
    _this->stillTouching = LE_TRUE;
    leWidgetEvent_Accept((leWidgetEvent*)evt, (leWidget*)_this);
    
    if (_this->autoHideWheel == LE_TRUE && _this->hideWheel == LE_TRUE)    
    {
        //show wheel
        _this->hideWheel = LE_FALSE;
        
        _this->fn->setBorderType(_this, _this->borderTypeCache);
        _this->fn->setBackgroundType(_this, _this->backgroundTypeCache);
    }
    
    _this->fn->invalidate(_this);
}

static void handleTouchUpEvent(leListWheelWidget* _this,
                               leWidgetEvent_TouchUp* evt)
{
    if(_this->momentum == 0 || leAbsoluteValue(_this->firstTouchY - _this->lastTouchY) < 10)
    {
        _this->snapPending = LE_TRUE;
    }
    
    _this->stillTouching = LE_FALSE;
    leWidgetEvent_Accept((leWidgetEvent*)evt, (leWidget*)_this);
    
    if (_this->autoHideWheel == LE_TRUE &&
        _this->hideWheel == LE_FALSE &&
        _this->snapPending == LE_FALSE)
    {
        //Hide wheel
        _this->hideWheel = LE_TRUE;
        
        _this->borderTypeCache = _this->fn->getBorderType(_this);
        _this->backgroundTypeCache = _this->fn->getBackgroundType(_this);
        
        _this->fn->setBorderType(_this, LE_WIDGET_BORDER_NONE);
        _this->fn->setBackgroundType(_this, LE_WIDGET_BACKGROUND_NONE);
    }
    
    _this->fn->invalidate(_this);
}

static void handleTouchMovedEvent(leListWheelWidget* _this,
                                  leWidgetEvent_TouchMove* evt)
{
    //leRect rect = _this->fn->rectToScreen(_this);
    
    //printf("%i, %i, %i\n", evt->y, _this->touchY, evt->y - _this->touchY);
    //printf("%i\n", leAbsoluteValue(evt->y - _this->lastTouchY));
    
    //invalidateContents(_this);
    
    if (evt->y < _this->lastTouchY)
    {
        if(_this->lastTouchY - evt->y > _this->minFlickDelta)
        {
            _this->momentum -= (_this->lastTouchY - evt->y) / 2;

            //printf("drag: %i\n", _this->lastTouchY - evt->y);

            if(_this->momentum < -MAX_MOMENTUM)
            {
                _this->momentum = -MAX_MOMENTUM;
            }
            
            //printf("plus %i\n", _this->momentum);
        }
        else
            _this->momentum = -(_this->lastTouchY - evt->y);
    }
    else if (evt->y > _this->lastTouchY)
    {
        if(evt->y - _this->lastTouchY > _this->minFlickDelta)
        {
            _this->momentum += (evt->y - _this->lastTouchY) / 2;

            //printf("drag: %i\n", evt->y - _this->lastTouchY);

            if(_this->momentum > MAX_MOMENTUM)
            {
                _this->momentum = MAX_MOMENTUM;
            }
            
            //printf("minus %i\n", _this->momentum);
        }
        else
            _this->momentum = evt->y - _this->lastTouchY;
    }    
    
    if (_this->autoHideWheel == LE_TRUE && _this->hideWheel == LE_TRUE)
    {
        _this->hideWheel = LE_FALSE;
        
        _this->fn->setBorderType(_this, _this->borderTypeCache);
        _this->fn->setBackgroundType(_this, _this->backgroundTypeCache);
    }
    
    _this->fn->invalidate(_this);
    
    _this->lastTouchY = evt->y;

    leWidgetEvent_Accept((leWidgetEvent*)evt, (leWidget*)_this);
}

static uint32_t getVisibleItemCount(const leListWheelWidget* _this)
{
    LE_ASSERT_THIS();

    return _this->visibleItems;
}

static leResult setVisibleItemCount(leListWheelWidget* _this,
                                    uint32_t cnt)
{
    LE_ASSERT_THIS();
        
    if(_this->visibleItems == (int32_t)cnt)
        return LE_SUCCESS;
    
    //invalidateContents(_this);

    _this->visibleItems = cnt;
    
    _this->cycleDistance = leDivideRounding(_this->widget.rect.height,
                                            _this->visibleItems - 1);

    calculateTopItem(_this);

    //invalidateContents(_this);
    
    _this->fn->invalidate(_this);

#if LE_DEBUG == 1
    _leDebugNotify_WidgetPropertyChanged((leWidget*)_this);
#endif

    return LE_SUCCESS;
}

static leRelativePosition getIconPosition(const leListWheelWidget* _this)
{
    LE_ASSERT_THIS();

    return _this->iconPos;
}
                                               
static leResult setIconPosition(leListWheelWidget* _this,
                                leRelativePosition pos)
{
    LE_ASSERT_THIS();
    
    if(pos != LE_RELATIVE_POSITION_LEFTOF &&
       pos != LE_RELATIVE_POSITION_RIGHTOF)
        return LE_FAILURE;
        
    if(_this->iconPos == pos)
        return LE_SUCCESS;

    //invalidateContents(_this);

    _this->iconPos = pos;

    //invalidateContents(_this);
    
    _this->fn->invalidate(_this);

#if LE_DEBUG == 1
    _leDebugNotify_WidgetPropertyChanged((leWidget*)_this);
#endif

    return LE_SUCCESS;
}

static uint32_t getIconMargin(const leListWheelWidget* _this)
{
    LE_ASSERT_THIS();

    return _this->iconMargin;
}
                                               
static leResult setIconMargin(leListWheelWidget* _this,
                              uint32_t mg)
{
    LE_ASSERT_THIS();
        
    if(_this->iconMargin == mg)
        return LE_SUCCESS;

    //invalidateContents(_this);

    _this->iconMargin = mg;

   // invalidateContents(_this);
   
   _this->fn->invalidate(_this);

#if LE_DEBUG == 1
    _leDebugNotify_WidgetPropertyChanged((leWidget*)_this);
#endif

    return LE_SUCCESS;
}

static leBool getShowIndicators(const leListWheelWidget* _this)
{
    LE_ASSERT_THIS();
        
    return _this->showIndicators;
}

static leResult setShowIndicators(leListWheelWidget* _this,
                                  leBool b)
{
    LE_ASSERT_THIS();
        
    if(_this->showIndicators == b)
        return LE_SUCCESS;

    //invalidateContents(_this);

    _this->showIndicators = b;

    //invalidateContents(_this);
    
    _this->fn->invalidate(_this);

#if LE_DEBUG == 1
    _leDebugNotify_WidgetPropertyChanged((leWidget*)_this);
#endif

    return LE_SUCCESS;
}

static uint32_t getIndicatorArea(const leListWheelWidget* _this)
{
    LE_ASSERT_THIS();
        
    return _this->indicatorArea;
}

static leResult setIndicatorArea(leListWheelWidget* _this,
                                 uint32_t area)
{
    LE_ASSERT_THIS();
        
    if(_this->indicatorArea == area)
        return LE_SUCCESS;

// todo
    //invalidateContents(_this);

    _this->indicatorArea = area;

    //invalidateContents(_this);
    
    _this->fn->invalidate(_this);

#if LE_DEBUG == 1
    _leDebugNotify_WidgetPropertyChanged((leWidget*)_this);
#endif

    return LE_SUCCESS;
}

static leBool getShaded(const leListWheelWidget* _this)
{
    LE_ASSERT_THIS();
        
    return _this->shaded;
}

static leResult setShaded(leListWheelWidget* _this,
                          leBool b)
{
    LE_ASSERT_THIS();
        
    if(_this->shaded == b)
        return LE_SUCCESS;

    _this->shaded = b;

    _this->fn->invalidate(_this);

#if LE_DEBUG == 1
    _leDebugNotify_WidgetPropertyChanged((leWidget*)_this);
#endif

    return LE_SUCCESS;
}

static uint32_t getFlickInitSpeed(const leListWheelWidget* _this)
{
    LE_ASSERT_THIS();
        
    return _this->minFlickDelta;
}

static leResult setFlickInitSpeed(leListWheelWidget* _this,
                                  uint32_t speed)
{
    LE_ASSERT_THIS();
        
    _this->minFlickDelta = speed;

#if LE_DEBUG == 1
    _leDebugNotify_WidgetPropertyChanged((leWidget*)_this);
#endif

    return LE_SUCCESS;
}

static uint32_t getMaxMomentum(const leListWheelWidget* _this)
{
    LE_ASSERT_THIS();
        
    return _this->maxMomentum;
}

static leResult setMaxMomentum(leListWheelWidget* _this,
                               uint32_t max)
{
    LE_ASSERT_THIS();
        
    _this->maxMomentum = max;

#if LE_DEBUG == 1
    _leDebugNotify_WidgetPropertyChanged((leWidget*)_this);
#endif

    return LE_SUCCESS;
}

static uint32_t getMomentumFalloffRate(const leListWheelWidget* _this)
{
    LE_ASSERT_THIS();
        
    return _this->momentumFalloff;
}

static leResult setMomentumFalloffRate(leListWheelWidget* _this,
                                       uint32_t percent)
{
    LE_ASSERT_THIS();
        
    _this->momentumFalloff = percent;

#if LE_DEBUG == 1
    _leDebugNotify_WidgetPropertyChanged((leWidget*)_this);
#endif

    return LE_SUCCESS;
}

static uint32_t getRotationUpdateRate(const leListWheelWidget* _this)
{
    LE_ASSERT_THIS();
        
    return _this->rotationTick;
}

static leResult setRotationUpdateRate(leListWheelWidget* _this,
                                      uint32_t ms)
{
    LE_ASSERT_THIS();
        
    _this->rotationTick = ms;

#if LE_DEBUG == 1
    _leDebugNotify_WidgetPropertyChanged((leWidget*)_this);
#endif

    return LE_SUCCESS;
}
                                                        
static uint32_t getItemCount(const leListWheelWidget* _this)
{
    LE_ASSERT_THIS();

    return _this->items.size;
}

static int32_t appendItem(leListWheelWidget* _this)
{
    leListWheelItem* item;
    
    LE_ASSERT_THIS();
        
    item = LE_MALLOC(sizeof(leListWheelItem));
    
    if(item == NULL)
        return -1;
        
    //invalidateContents(_this);
    
    item->icon = NULL;
    item->string = NULL;
    
    leArray_PushBack(&_this->items, item);
    
    if(_this->items.size == 1)
    {
        _this->selectedItem = 0;
    }
        
    calculateTopItem(_this);
    
    //invalidateContents(_this);
    
    _this->fn->invalidate(_this);

#if LE_DEBUG == 1
    _leDebugNotify_WidgetPropertyChanged((leWidget*)_this);
#endif

    return (int32_t)_this->items.size - 1;
}

static int32_t insertItem(leListWheelWidget* _this,
                          int32_t idx)
{
    leListWheelItem* item;
    
    LE_ASSERT_THIS();
    
    if(idx < 0)
        return -1;
    
    item = LE_MALLOC(sizeof(leListWheelItem));
    
    if(item == NULL)
        return -1;
        
    item->icon = NULL;
    item->string = NULL;
    
    //invalidateContents(_this);
    
    leArray_InsertAt(&_this->items, (uint32_t)idx, item);
    
    if(_this->items.size == 1)
    {
        _this->selectedItem = 0;
    }
        
    calculateTopItem(_this);
    
    //invalidateContents(_this);
    
    _this->fn->invalidate(_this);

#if LE_DEBUG == 1
    _leDebugNotify_WidgetPropertyChanged((leWidget*)_this);
#endif

    return idx;
}

static leResult removeItem(leListWheelWidget* _this,
                           int32_t idx)
{
    leResult res;
    leListWheelItem* item;
    
    LE_ASSERT_THIS();
    
    if(idx < 0 || idx >= (int32_t)_this->items.size)
        return LE_FAILURE;
        
    item = _this->items.values[idx];
    
    if(item == NULL)
        return LE_FAILURE;
        
    //if(idx < _this->items.size)        
    //    invalidateContents(_this);
        
    res = leArray_RemoveAt(&_this->items, (uint32_t)idx);
    
    if(_this->items.size == 0)
    {
        _this->selectedItem = -1;
    }
    
    LE_FREE(item);
    
    calculateTopItem(_this);

    //if(res == LE_SUCCESS)
    //    invalidateContents(_this);
    
    _this->fn->invalidate(_this);

#if LE_DEBUG == 1
    _leDebugNotify_WidgetPropertyChanged((leWidget*)_this);
#endif

    return res;
}

static leResult removeAllItems(leListWheelWidget* _this)
{
    uint32_t i;

    leListWheelItem* item;
    
    LE_ASSERT_THIS();

    for(i = 0; i < _this->items.size; i++)
    {
        item = _this->items.values[i];

        if(item != NULL)
        {
            LE_FREE(item);
        }
    }

    leArray_Clear(&_this->items);
    
    _this->selectedItem = -1;
    
    calculateTopItem(_this);
        
    _this->fn->invalidate(_this);

#if LE_DEBUG == 1
    _leDebugNotify_WidgetPropertyChanged((leWidget*)_this);
#endif

    return LE_SUCCESS;
}

static int32_t getSelectedItem(const leListWheelWidget* _this)
{
    LE_ASSERT_THIS();
        
    return _this->selectedItem;
}

static leResult setSelectedItem(leListWheelWidget* _this,
                                int32_t idx)
{
    LE_ASSERT_THIS();
    
    if(idx < 0 || idx >= (int32_t)_this->items.size)
        return LE_FAILURE;
        
    if(_this->selectedItem == idx)
        return LE_SUCCESS;
        
    //invalidateContents(_this);
        
    _this->selectedItem = idx;
    
    if(_this->cb != NULL)
    {
        _this->cb(_this, idx);
    }
    
    calculateTopItem(_this);
    
    //invalidateContents(_this);
    
    _this->fn->invalidate(_this);

#if LE_DEBUG == 1
    _leDebugNotify_WidgetPropertyChanged((leWidget*)_this);
#endif

    return LE_SUCCESS;
}

static leResult selectPreviousItem(leListWheelWidget* _this)
{
    int32_t idx;
    
    LE_ASSERT_THIS();
    
    if(_this->items.size == 0)
        return LE_FAILURE;
        
    //invalidateContents(_this);
        
    idx = _this->selectedItem;
    
    if(idx == 0)
    {
        idx = _this->items.size - 1;
    }
    else
    {
        idx -= 1; 
    }
        
    if(_this->selectedItem == idx)
        return LE_SUCCESS;
        
    _this->selectedItem = idx;
    
    if(_this->cb != NULL)
    {
        _this->cb(_this, idx);
    }
    
    calculateTopItem(_this);
    
    //invalidateContents(_this);
    
    _this->fn->invalidate(_this);

#if LE_DEBUG == 1
    _leDebugNotify_WidgetPropertyChanged((leWidget*)_this);
#endif

    return LE_SUCCESS;
}

static leResult selectNextItem(leListWheelWidget* _this)
{
    int32_t idx;
    
    LE_ASSERT_THIS();
    
    if(_this->items.size == 0)
        return LE_FAILURE;
        
    //invalidateContents(_this);
        
    idx = _this->selectedItem;
    
    if(idx == (int32_t)_this->items.size - 1)
    {
        idx = 0;
    }
    else
    {
        idx += 1;  
    }
        
    if(_this->selectedItem == idx)
        return LE_SUCCESS;
        
    _this->selectedItem = idx;
    
    if(_this->cb != NULL)
    {
        _this->cb(_this, idx);
    }
    
    calculateTopItem(_this);
    
    //invalidateContents(_this);
    
    _this->fn->invalidate(_this);

#if LE_DEBUG == 1
    _leDebugNotify_WidgetPropertyChanged((leWidget*)_this);
#endif

    return LE_SUCCESS;
}
                                                 
static leString* getItemString(const leListWheelWidget* _this,
                             int32_t idx)
{
    leListWheelItem* item;
    
    LE_ASSERT_THIS();
    
    if(idx < 0 || idx >= (int32_t)_this->items.size)
        return NULL;
        
    item = _this->items.values[idx];
    
    return (leString*)item->string;
}
                                               
static leResult setItemString(leListWheelWidget* _this,
                            int32_t idx,
                            const leString* str)
{
    leListWheelItem* item;
    
    LE_ASSERT_THIS();
    
    if(idx < 0 || idx >= (int32_t)_this->items.size)
        return LE_FAILURE;
        
    //invalidateContents(_this);
       
    item = _this->items.values[idx];

    if(item->string != NULL)
    {
        item->string->fn->setPreInvalidateCallback((leString*)item->string,
                                                   NULL,
                                                   NULL);

        item->string->fn->setInvalidateCallback((leString*)item->string,
                                                NULL,
                                                NULL);
    }
    
    item->string = str;

    item->string->fn->setPreInvalidateCallback((leString*)item->string,
                                               (leString_InvalidateCallback)stringPreinvalidate,
                                               _this);

    item->string->fn->setInvalidateCallback((leString*)item->string,
                                            (leString_InvalidateCallback)stringInvalidate,
                                            _this);
    
    _this->fn->invalidate(_this);

#if LE_DEBUG == 1
    _leDebugNotify_WidgetPropertyChanged((leWidget*)_this);
#endif

    return LE_SUCCESS;
}
                                               
static leImage* getItemIcon(const leListWheelWidget* _this,
                            int32_t idx)
{
    leListWheelItem* item;
    
    LE_ASSERT_THIS();
    
    if(idx < 0 || idx >= (int32_t)_this->items.size)
        return NULL;
        
    item = _this->items.values[idx];
        
    return (leImage*)item->icon;
}
                                                              
static leResult setItemIcon(leListWheelWidget* _this,
                            int32_t idx,
                            const leImage* img)
{
    leListWheelItem* item;
    
    LE_ASSERT_THIS();
    
    if(idx < 0 || idx >= (int32_t)_this->items.size)
        return LE_FAILURE;
        
    //invalidateContents(_this);
        
    item = _this->items.values[idx];
        
    item->icon = img;
    
    //invalidateContents(_this);
    
    _this->fn->invalidate(_this);

#if LE_DEBUG == 1
    _leDebugNotify_WidgetPropertyChanged((leWidget*)_this);
#endif

    return LE_SUCCESS;
}

static leListWheelWidget_SelectedItemChangedEvent getSelectedItemChangedEventCallback(const leListWheelWidget* _this)
{
    LE_ASSERT_THIS();

    return _this->cb;
}

static leResult setSelectedItemChangedEventCallback(leListWheelWidget* _this,
                                                    leListWheelWidget_SelectedItemChangedEvent cb)
{
    LE_ASSERT_THIS();
    
    if(_this->cb == cb)
        return LE_FAILURE;

    _this->cb = cb;
    
    return LE_SUCCESS;
}

static leListWheelIndicatorFill getIndicatorFill(const leListWheelWidget* _this)
{
    LE_ASSERT_THIS();
    
    return _this->indicatorFill;
}

static leResult setIndicatorFill(leListWheelWidget* _this,
                                 leListWheelIndicatorFill fill)
{
    LE_ASSERT_THIS();
    
    if(fill > LE_LISTWHEEL_INDICATOR_FILL_GRADIENT)
        return LE_FAILURE;
    
    if(_this->indicatorFill == fill)
        return LE_SUCCESS;
    
    _this->indicatorFill = fill;
    
    _this->fn->invalidate(_this);

#if LE_DEBUG == 1
    _leDebugNotify_WidgetPropertyChanged((leWidget*)_this);
#endif

    return LE_SUCCESS;
}

static leListWheelZoomEffects getZoomEffects(const leListWheelWidget* _this)
{
    LE_ASSERT_THIS();
    
    return _this->zoomEffects;
}

static leResult setZoomEffects(leListWheelWidget* _this,
                               leListWheelZoomEffects zoomEffects)
{
    LE_ASSERT_THIS();
    
    if(zoomEffects > LE_LISTWHEEL_ZOOM_EFFECT_FIXED_SCALE)
        return LE_FAILURE;
    
    if(_this->zoomEffects == zoomEffects)
        return LE_SUCCESS;
    
    _this->zoomEffects = zoomEffects;
    
    _this->fn->invalidate(_this);

#if LE_DEBUG == 1
    _leDebugNotify_WidgetPropertyChanged((leWidget*)_this);
#endif

    return LE_SUCCESS;
}

static leBool getAutoHideWheel(const leListWheelWidget* _this)
{
    LE_ASSERT_THIS();
    
    return _this->autoHideWheel;
}

static leResult setAutoHideWheel(leListWheelWidget* _this,
                                 leBool autoHide)
{
    LE_ASSERT_THIS();
    
    if (_this->autoHideWheel == autoHide)
        return LE_SUCCESS;
    
    _this->autoHideWheel = autoHide;
    
    _this->hideWheel = autoHide;
    
    if (_this->autoHideWheel == LE_TRUE)
    {
        _this->borderTypeCache = _this->fn->getBorderType(_this);
        _this->backgroundTypeCache = _this->fn->getBackgroundType(_this);

        _this->fn->setBorderType(_this, LE_WIDGET_BORDER_NONE);
        _this->fn->setBackgroundType(_this, LE_WIDGET_BACKGROUND_NONE);   
    }
    else
    {
        _this->fn->setBorderType(_this, _this->borderTypeCache);
        _this->fn->setBackgroundType(_this, _this->backgroundTypeCache);
    }
    
    _this->fn->invalidate(_this);

#if LE_DEBUG == 1
    _leDebugNotify_WidgetPropertyChanged((leWidget*)_this);
#endif

    return LE_SUCCESS;
}

void _leListWheelWidget_Paint(leListWheelWidget* _this);

#if LE_DYNAMIC_VTABLES == 1
void _leWidget_FillVTable(leWidgetVTable* tbl);

void _leListWheelWidget_GenerateVTable(void)
{
    _leWidget_FillVTable((void*)&listWheelWidgetVTable);
    
    /* overrides from base class */
    listWheelWidgetVTable._destructor = destructor;
    listWheelWidgetVTable.update = update;
    listWheelWidgetVTable.touchDownEvent = handleTouchDownEvent;
    listWheelWidgetVTable.touchMoveEvent = handleTouchMovedEvent;
    listWheelWidgetVTable.touchUpEvent = handleTouchUpEvent;
    listWheelWidgetVTable.resizeEvent = handleResizedEvent;
    listWheelWidgetVTable._paint = _leListWheelWidget_Paint;
    listWheelWidgetVTable.languageChangeEvent = handleLanguageChangeEvent;
    
    /* member functions */
    listWheelWidgetVTable.getVisibleItemCount = getVisibleItemCount;
    listWheelWidgetVTable.setVisibleItemCount = setVisibleItemCount;
    listWheelWidgetVTable.getIconPosition = getIconPosition;
    listWheelWidgetVTable.setIconPosition = setIconPosition;
    listWheelWidgetVTable.getIconMargin = getIconMargin;
    listWheelWidgetVTable.setIconMargin = setIconMargin;
    listWheelWidgetVTable.getShowIndicators = getShowIndicators;
    listWheelWidgetVTable.setShowIndicators = setShowIndicators;
    listWheelWidgetVTable.getIndicatorArea = getIndicatorArea;
    listWheelWidgetVTable.setIndicatorArea = setIndicatorArea;
    listWheelWidgetVTable.getShaded = getShaded;
    listWheelWidgetVTable.setShaded = setShaded;
    listWheelWidgetVTable.getFlickInitSpeed = getFlickInitSpeed;
    listWheelWidgetVTable.setFlickInitSpeed = setFlickInitSpeed;
    listWheelWidgetVTable.getMaxMomentum = getMaxMomentum;
    listWheelWidgetVTable.setMaxMomentum = setMaxMomentum;
    listWheelWidgetVTable.getMomentumFalloffRate = getMomentumFalloffRate;
    listWheelWidgetVTable.setMomentumFalloffRate = setMomentumFalloffRate;
    listWheelWidgetVTable.getRotationUpdateRate = getRotationUpdateRate;
    listWheelWidgetVTable.setRotationUpdateRate = setRotationUpdateRate;
    listWheelWidgetVTable.getItemCount = getItemCount;
    listWheelWidgetVTable.appendItem = appendItem;
    listWheelWidgetVTable.insertItem = insertItem;
    listWheelWidgetVTable.removeItem = removeItem;
    listWheelWidgetVTable.removeAllItems = removeAllItems;
    listWheelWidgetVTable.getSelectedItem = getSelectedItem;
    listWheelWidgetVTable.setSelectedItem = setSelectedItem;
    listWheelWidgetVTable.selectPreviousItem = selectPreviousItem;
    listWheelWidgetVTable.selectNextItem = selectNextItem;
    listWheelWidgetVTable.getItemString = getItemString;
    listWheelWidgetVTable.setItemString = setItemString;
    listWheelWidgetVTable.getItemIcon = getItemIcon;
    listWheelWidgetVTable.setItemIcon = setItemIcon;
    listWheelWidgetVTable.getSelectedItemChangedEventCallback = getSelectedItemChangedEventCallback;
    listWheelWidgetVTable.setSelectedItemChangedEventCallback = setSelectedItemChangedEventCallback;
    listWheelWidgetVTable.getIndicatorFill = getIndicatorFill;
    listWheelWidgetVTable.setIndicatorFill = setIndicatorFill;
    listWheelWidgetVTable.getZoomEffects = getZoomEffects;
    listWheelWidgetVTable.setZoomEffects = setZoomEffects;
    listWheelWidgetVTable.getAutoHideWheel = getAutoHideWheel;
    listWheelWidgetVTable.setAutoHideWheel = setAutoHideWheel;
}

void _leListWheelWidget_FillVTable(leListWheelWidgetVTable* tbl)
{
    *tbl = listWheelWidgetVTable;
}
#else
static const leListWheelWidgetVTable listWheelWidgetVTable =
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
    .update = update,
    .touchDownEvent = handleTouchDownEvent,
    .touchMoveEvent = handleTouchMovedEvent,
    .touchUpEvent = handleTouchUpEvent,
    .resizeEvent = handleResizedEvent,
    ._paint = _leListWheelWidget_Paint,
    .languageChangeEvent = handleLanguageChangeEvent,

    /* member functions */
    .getVisibleItemCount = getVisibleItemCount,
    .setVisibleItemCount = setVisibleItemCount,
    .getIconPosition = getIconPosition,
    .setIconPosition = setIconPosition,
    .getIconMargin = getIconMargin,
    .setIconMargin = setIconMargin,
    .getShowIndicators = getShowIndicators,
    .setShowIndicators = setShowIndicators,
    .getIndicatorArea = getIndicatorArea,
    .setIndicatorArea = setIndicatorArea,
    .getShaded = getShaded,
    .setShaded = setShaded,
    .getFlickInitSpeed = getFlickInitSpeed,
    .setFlickInitSpeed = setFlickInitSpeed,
    .getMaxMomentum = getMaxMomentum,
    .setMaxMomentum = setMaxMomentum,
    .getMomentumFalloffRate = getMomentumFalloffRate,
    .setMomentumFalloffRate = setMomentumFalloffRate,
    .getRotationUpdateRate = getRotationUpdateRate,
    .setRotationUpdateRate = setRotationUpdateRate,
    .getItemCount = getItemCount,
    .appendItem = appendItem,
    .insertItem = insertItem,
    .removeItem = removeItem,
    .removeAllItems = removeAllItems,
    .getSelectedItem = getSelectedItem,
    .setSelectedItem = setSelectedItem,
    .selectPreviousItem = selectPreviousItem,
    .selectNextItem = selectNextItem,
    .getItemString = getItemString,
    .setItemString = setItemString,
    .getItemIcon = getItemIcon,
    .setItemIcon = setItemIcon,
    .getSelectedItemChangedEventCallback = getSelectedItemChangedEventCallback,
    .setSelectedItemChangedEventCallback = setSelectedItemChangedEventCallback,
    .getIndicatorFill = getIndicatorFill,
    .setIndicatorFill = setIndicatorFill,
    .getZoomEffects = getZoomEffects,
    .setZoomEffects = setZoomEffects,
    .getAutoHideWheel = getAutoHideWheel,
    .setAutoHideWheel = setAutoHideWheel,
};
#endif

#endif // LE_LISTWHEEL_WIDGET_ENABLED
