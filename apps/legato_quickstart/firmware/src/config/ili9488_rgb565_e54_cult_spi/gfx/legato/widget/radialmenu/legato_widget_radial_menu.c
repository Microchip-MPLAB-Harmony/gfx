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


#define DEFAULT_WIDTH           200
#define DEFAULT_HEIGHT          150
#define DEFAULT_A               100
#define DEFAULT_B               80
#define DEFAULT_THETA           0

#define DEFAULT_HIGHLIGHT_MARGIN 2

#define MAX_SIZE_PERCENT         200
#define MIN_SIZE_PERCENT         1
#define DEFAULT_MAX_SIZE_PERCENT 100
#define DEFAULT_MIN_SIZE_PERCENT 30

#define MAX_ALPHA_PERCENT           255
#define MIN_ALPHA_PERCENT           0
#define DEFAULT_MAX_ALPHA_SCALE     255
#define DEFAULT_MIN_ALPHA_SCALE     128

#define PROMINENT_ANGLE             90
#define BACK_ANGLE                  270
#define MIN_THETA                   0
#define MAX_THETA                   90

static
#if LE_DYNAMIC_VTABLES == 0
const
#endif
leRadialMenuWidgetVTable radialMenuWidgetVTable;

static leBool filterWidgetEvent(leWidget* wgt, leWidgetEvent* evt, void* data)
{
    if(evt->id == LE_EVENT_TOUCH_DOWN)
    {
        leWidgetEvent_TouchDown* tchEvt = (leWidgetEvent_TouchDown*)evt;

        //leWidgetEvent_Accept((leWidgetEvent*)evt, NULL);

        return LE_TRUE;
    }
    else if(evt->id == LE_EVENT_TOUCH_UP)
    {
        leWidgetEvent_TouchUp* tchEvt = (leWidgetEvent_TouchUp*)evt;

        //leWidgetEvent_Accept((leWidgetEvent*)evt, NULL);

        return LE_TRUE;
    }
    else if(evt->id == LE_EVENT_TOUCH_MOVE)
    {
        leWidgetEvent_TouchMove* tchEvt = (leWidgetEvent_TouchMove*)evt;

        //leWidgetEvent_Accept((leWidgetEvent*)evt, NULL);

        return LE_TRUE;
    }

    return LE_FALSE;
}

static void invalidateContents(leRadialMenuWidget* _this)
{
    _this->fn->_damageArea(_this, &_this->widget.rect);
}

static void handleLanguageChangeEvent(leRadialMenuWidget* _this)
{
    invalidateContents(_this);
}

//Weigh the angle t between 90 (prominent) and 270 (sunken) and returns a percent, t has to be between 0 to 360
uint32_t _weighAngle(int32_t t)
{
    int32_t angle;

    t = leNormalizeAngle(t);

    if (t >= 90 && t <= 270)
    {
        angle = t - 90;
    }
    else if (t < 90)
    {
        angle = 90 - t;
    }
    else
    {
        angle = 450 - t;
    }

    //Calculate the percent scale relative to prominent (90) and minimum (270)
    return lePercentWholeRounded(angle, 180);
}

static leResult _insertionSort(leList* list, leRadialMenuItemNode* item)
{
    uint32_t i, percentTest, percentNew;
    leRadialMenuItemNode* testItem;

    if (list->size == 0)
    {
        leList_PushFront(list, item);
        return LE_SUCCESS;
    }

    percentNew = _weighAngle(item->t);

    for(i = 0; i < list->size; ++i)
    {
        testItem = leList_Get(list,i);

        percentTest = _weighAngle(testItem->t);

        if (percentNew > percentTest)
        {
            leList_InsertAt(list, item, i);
            return LE_SUCCESS;
        }
    }

    //if we got here without an insertion, item goes to the back
    leList_PushBack(list, item);

    return LE_SUCCESS;
}

static void arrangeItems(leRadialMenuWidget* mn)
{
    lePoint itemPos;
//    leRect mnScreenRect;
    leRadialMenuItemNode* item;
    uint32_t i,percent;
    leList drawOrderList;

    leList_Create(&drawOrderList);

//    mnScreenRect = leWidget_RectToScreenSpace((leWidget*)mn);
    for (i = 0; i < mn->hiddenList.size; ++i)
    {
        item = leList_Get(&mn->hiddenList, i);

        item->widget->fn->setVisible(item->widget, LE_FALSE);
    }

    for (i = 0; i < mn->shownList.size; ++i)
    {
        item = leList_Get(&mn->shownList, i);

        percent = 100 - _weighAngle(item->t);

#if LE_ALPHA_BLENDING_ENABLED == 1
        switch(mn->blendMode)
        {
            case LE_RADIAL_MENU_INTERPOLATE_GRADUAL:
            {
                item->widget->alphaAmount = leLerp(mn->minAlphaAmount, mn->maxAlphaAmount, percent);

                break;
            }
            case LE_RADIAL_MENU_INTERPOLATE_OFF:
            {
                item->widget->alphaAmount = item->origAlphaAmount;

                break;
            }
            case LE_RADIAL_MENU_INTERPOLATE_PROMINENT:
            {
                if (mn->fn->isProminent(mn, item->widget))
                {
                    item->widget->alphaAmount = mn->maxAlphaAmount;
                }
                else
                {
                    item->widget->alphaAmount = mn->minAlphaAmount;
                }

                break;
            }
        }
#endif

        switch(mn->scaleMode)
        {
            case LE_RADIAL_MENU_INTERPOLATE_GRADUAL:
            {
                item->widget->fn->setSize(item->widget,
                                          item->origSize.width * leLerp(mn->minSizePercent, mn->maxSizePercent, percent) / 100,
                                          item->origSize.height * leLerp(mn->minSizePercent, mn->maxSizePercent, percent) / 100);

                break;
            }
            case LE_RADIAL_MENU_INTERPOLATE_OFF:
            {
                item->widget->fn->setSize(item->widget,
                                          item->origSize.width,
                                          item->origSize.height);

                break;
            }
            case LE_RADIAL_MENU_INTERPOLATE_PROMINENT:
            {
                if(mn->fn->isProminent(mn, item->widget))
                {
                    item->widget->fn->setSize(item->widget,
                                              item->origSize.width * (int32_t)mn->maxSizePercent / 100,
                                              item->origSize.height * (int32_t)mn->maxSizePercent / 100);
                }
                else
                {
                    item->widget->fn->setSize(item->widget,
                                              item->origSize.width * (int32_t)mn->minSizePercent / 100,
                                              item->origSize.height * (int32_t)mn->minSizePercent / 100);
                }

                break;
            }
        }

        //Establish widget positions
        leEllipsePoint(item->t, mn->a, mn->b, mn->theta, &itemPos);

        itemPos.x = itemPos.x + mn->widget.rect.width / 2 - item->widget->rect.width / 2;
        itemPos.y = itemPos.y + mn->widget.rect.height / 2 - item->widget->rect.height / 2;

        item->widget->fn->setPosition(item->widget, itemPos.x, itemPos.y);

        //This step is to setup arranging the draw order of each item
        leArray_Remove(&mn->widget.children, item->widget);

        item->widget->fn->setVisible(item->widget, LE_TRUE);

        //Sort by inserting into empty list
        _insertionSort(&drawOrderList, item);
    }

    //Re-add each widget item back as child from back to front
    for (i = 0; i < drawOrderList.size; ++i)
    {
        item = leList_Get(&drawOrderList, i);

        leArray_PushBack(&mn->widget.children, item->widget);
    }

    leList_Clear(&drawOrderList);
}

static leResult _rotateMenuItems(leRadialMenuWidget* _this, int32_t angle)
{
    leRadialMenuItemNode* item = NULL;
    leRadialMenuItemNode* itemToHide = NULL;
    leList itemsToHide;
    uint32_t itemsShown = _this->itemsShown;
    uint32_t i; 
    
    if(_this->widgetList.size == 0)
        return LE_FAILURE;
    
    if(angle == 0)
        return LE_SUCCESS;

    if(_this->itemsShown > _this->widgetList.size)
    {
        itemsShown = _this->widgetList.size;
    }

    leList_Create(&itemsToHide);
    
    for(i = 0; i < _this->shownList.size; ++i)
    {
        item = leList_Get(&_this->shownList, i);
        
        if (itemsShown < _this->widgetList.size)
        {                
            if(leAbsoluteValue(item->t - BACK_ANGLE) < leAbsoluteValue(angle) &&
              (item->t - BACK_ANGLE) * angle >= 0)
            {
                if (angle > 0)
                {
                    leList_PushBack(&_this->hiddenList, item);
                }
                else
                {
                    leList_PushFront(&_this->hiddenList, item);                            
                }
                
                leList_PushBack(&itemsToHide, item);
            }            
        }

        item->t -= angle;                
        item->t = leNormalizeAngle(item->t);
    }

    for(i = 0; i < itemsToHide.size; ++i)
    {
        itemToHide = leList_Get(&itemsToHide, i);
        
        if (angle > 0)
        {
            item = leList_Get(&_this->hiddenList, 0);
        }
        else
        {
            item = leList_Get(&_this->hiddenList, _this->hiddenList.size - 1);
        }                    

        if (item != NULL)
        {
            leList_Remove(&_this->hiddenList, item);                            
            leList_PushFront(&_this->shownList, item);
            item->t = itemToHide->t;
            itemToHide->t = BACK_ANGLE;
        };
        
        leList_Remove(&_this->shownList, itemToHide);
    }
    
    leList_Clear(&itemsToHide);

    return LE_SUCCESS;
}

void leRadialMenuWidget_Constructor(leRadialMenuWidget* _this)
{
    leWidget_Constructor((leWidget*)_this);
    
    _this->widget.fn = (void*)&radialMenuWidgetVTable;
    _this->fn = &radialMenuWidgetVTable;

    _this->widget.type = LE_WIDGET_RADIAL_MENU;
    
    _this->widget.rect.width = DEFAULT_WIDTH;
    _this->widget.rect.height = DEFAULT_HEIGHT;

    _this->drawEllipse = LE_TRUE;

    _this->widget.borderType = LE_WIDGET_BORDER_NONE;
    _this->widget.backgroundType = LE_WIDGET_BACKGROUND_NONE;

    _this->widget.halign = LE_HALIGN_CENTER;
    _this->widget.valign = LE_VALIGN_MIDDLE;
    
    _this->highlighter = NULL;

    _this->a = DEFAULT_A;
    _this->b = DEFAULT_B;
    _this->theta = DEFAULT_THETA;
    _this->ellipseChanged = LE_TRUE;
    _this->itemsShown = 5;

    _this->scaleMode = LE_RADIAL_MENU_INTERPOLATE_GRADUAL;
    _this->blendMode = LE_RADIAL_MENU_INTERPOLATE_GRADUAL;
    
    _this->maxSizePercent = DEFAULT_MAX_SIZE_PERCENT;
    _this->minSizePercent = DEFAULT_MIN_SIZE_PERCENT;
    
    _this->maxAlphaAmount = DEFAULT_MAX_ALPHA_SCALE;
    _this->minAlphaAmount = DEFAULT_MIN_ALPHA_SCALE;
    
    leList_Create(&_this->widgetList);     
    leList_Create(&_this->shownList);     
    leList_Create(&_this->hiddenList);     
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
    uint32_t i;

    for(i = 0; i < _this->widgetList.size; i++)
    {
        item = leList_Get(&_this->widgetList, i);

        // free all widgets
        if(item->widget != NULL)
        {
            leWidget_Delete(item->widget);
        }
    }

    // free the widget item list
    leList_Destroy(&_this->widgetList);

    _this->highlighter = NULL;

    _leWidget_Destructor((leWidget*)_this);
}

static leBool _updateWidestTallestItem(leRadialMenuWidget* _this)
{
    uint32_t i;
    leRadialMenuItemNode* item = NULL;
    
    if(_this == NULL || _this->widgetList.size == 0)
        return LE_FALSE;
    
    for(i = 0; i < _this->widgetList.size; ++i)
    {
        item = leList_Get(&_this->widgetList, i);
        
        if (item != NULL)
        {
            if (_this->widestWidgetItem == NULL)
            {
                _this->widestWidgetItem = item;
            }
            else
            {
                if (item->widget->rect.width > _this->widestWidgetItem->widget->rect.width)
                {
                    _this->widestWidgetItem = item;
                }
            }

            if (_this->tallestWidgetItem == NULL)
            {
                _this->tallestWidgetItem = item;
            }
            else
            {
                if (item->widget->rect.height > _this->widestWidgetItem->widget->rect.height)
                {
                    _this->tallestWidgetItem = item;
                }
            }        
        }
    }
    
    return LE_FAILURE;    
}

static leBool _deriveWidestTallestItem(leRadialMenuWidget* _this,
                                       leRadialMenuItemNode* item,
                                       leWidget* widget)
{
    leRadialMenuItemNode* testItem = NULL;
    uint32_t i = 0;
    
    if (_this == NULL)
        return LE_FAILURE;

    //This is a widget addition
    if (item != NULL && widget != NULL)
    {
        if (_this->widestWidgetItem == NULL)
        {
            _this->widestWidgetItem = item;
        }
        else
        {
            if (widget->rect.width > _this->widestWidgetItem->widget->rect.width)
            {
                _this->widestWidgetItem = item;
            }
        }

        if (_this->tallestWidgetItem == NULL)
        {
            _this->tallestWidgetItem = item;
        }
        else
        {
            if (widget->rect.height > _this->widestWidgetItem->widget->rect.height)
            {
                _this->tallestWidgetItem = item;
            }
        }        

        return LE_SUCCESS;    
    }
    else if (widget == NULL) //this is a widget removal, if it happens to be the tallest or the widest widget, 
                             //a new one will have to be assigned
    {
        if (_this->widestWidgetItem == item && _this->widgetList.size > 1)
        {
            _this->widestWidgetItem = leList_Get(&_this->widgetList, 0);
            
            for(i = 1; i < _this->widgetList.size; ++i)
            {
                testItem = leList_Get(&_this->widgetList, i);
                
                if (testItem != NULL && _this->widestWidgetItem != NULL 
                        && testItem->widget->rect.width > _this->widestWidgetItem->widget->rect.width)
                {
                    _this->widestWidgetItem = testItem;
                }
            }
        }

        if (_this->tallestWidgetItem == item && _this->widgetList.size > 1)
        {
            _this->tallestWidgetItem = leList_Get(&_this->widgetList, 0);
            
            for(i = 1; i < _this->widgetList.size; ++i)
            {
                testItem = leList_Get(&_this->widgetList, i);
                
                if (testItem != NULL && _this->tallestWidgetItem != NULL 
                        && testItem->widget->rect.height > _this->widestWidgetItem->widget->rect.height)
                {
                    _this->tallestWidgetItem = testItem;
                }
            }            
        }
        
        return LE_SUCCESS;        
    }
    
    return LE_FAILURE;    
}

static int32_t _getSliceAngle(const leRadialMenuWidget* _this)
{
    if(_this == NULL || _this->itemsShown == 0)
        return 0;

    if(_this->itemsShown > _this->widgetList.size)
    {
        return (360 / _this->widgetList.size);
    }
    else
    {
        return (360 / _this->itemsShown);
    }
}

static void handleResizedEvent(leRadialMenuWidget* _this,
                               leWidget_ResizeEvent* evt)
{
    LE_ASSERT_THIS();
    
    _this->ellipseChanged = LE_TRUE;
}

static int32_t _normalizeIndex(const leRadialMenuWidget* _this,
                               uint32_t index)
{
    int32_t newIndex = 0;
    
    if (index >= 0 && index < _this->widgetList.size)
        return index;
    
    if (index < 0)
    {
        newIndex = _this->widgetList.size + index; 
        
        return newIndex;
    }
    
    //if we get here, the index is larger than the total amount of items
    newIndex = index - _this->widgetList.size;    
    
    return newIndex;
}

static leBool _updateEllipse(leRadialMenuWidget* _this)
{
    int16_t sine, cosine = 0;
    int32_t widest, tallest, scale = 0;
    
    LE_ASSERT_THIS();
    
    if(_this->widestWidgetItem == NULL ||
       _this->tallestWidgetItem == NULL ||
       _this->ellipseChanged == LE_FALSE)
    {
        return LE_FAILURE;
    }
    
    _updateWidestTallestItem(_this);
    
    sine = leSineCosineGet(_this->theta, LE_TRIG_SINE_TYPE);
    cosine = leSineCosineGet(_this->theta, LE_TRIG_COSINE_TYPE);

    scale = _this->scaleMode != LE_RADIAL_MENU_INTERPOLATE_OFF ? _this->maxSizePercent : 100;
    widest = _this->widestWidgetItem->widget->rect.width;
    tallest = _this->tallestWidgetItem->widget->rect.height * scale / 100;
    
    if (_this->highlightProminent == LE_TRUE)
    {
        widest += DEFAULT_HIGHLIGHT_MARGIN * 2;
        tallest += DEFAULT_HIGHLIGHT_MARGIN * 2;
    }
    
    switch(_this->ellipseType)
    {
        case LE_RADIAL_MENU_ELLIPSE_TYPE_DEFAULT:
        {
            _this->b = _this->widget.rect.height / 2 - tallest / 2;
            _this->a = _this->widget.rect.width / 2 - widest / 2;

            _this->a = leClampi(0, _this->widget.rect.width / 2,  _this->a);
            _this->b = leClampi(0, _this->widget.rect.height / 2, _this->b);    
        
            break;
        }
        case LE_RADIAL_MENU_ELLIPSE_TYPE_ORBITAL:
        {
            _this->b = leAbsoluteValue((_this->widget.rect.height / 2 - tallest / 2) * sine / 256 - (widest / 2) * sine / 256);
            _this->a = leAbsoluteValue((_this->widget.rect.width / 2 - widest / 2) * cosine / 256 - (tallest / 2) * sine / 256);        

            _this->a = leClampi(0, _this->widget.rect.width / 2 * cosine / 256,  _this->a);
            _this->b = leClampi(0, _this->widget.rect.height / 2 * sine / 256, _this->b);
        
            break;
        }
        case LE_RADIAL_MENU_ELLIPSE_TYPE_ROLLODEX:
        {
            _this->b = 0;
            _this->a = _this->widget.rect.height / 2 - tallest / 2;
            _this->theta = 90;
        
            break;
        }
        default:
        {
            break;
        }
    }

    arrangeItems(_this);

    //Shave some calculations
    _this->ellipseChanged = LE_FALSE;
    
    return LE_SUCCESS;
}

static void update(leRadialMenuWidget* _this, uint32_t dt)
{
    leRadialMenuItemNode* prominentItem = NULL;
    int32_t diffAngle = 0;

    if (_this != NULL)
    {
        switch(_this->state)
        {
            case LE_RADIAL_MENU_INIT:
            {
                invalidateContents(_this);
                _this->state = LE_RADIAL_MENU_RESET_TO_INPUT_POS;
                break;
            }
            case LE_RADIAL_MENU_INPUT_READY:
            {
                break;
            }
            case LE_RADIAL_MENU_HANDLE_USER_MOVE_REQUEST:
            {
                //Rotate the menu to target angle
                if (_this->userRequestedAngleDiff != 0)
                {
                    //_this->userRequestedAngleDiff = _this->userRequestedAngleDiff * 2 / 3;

                    _rotateMenuItems(_this, _this->userRequestedAngleDiff);
                    arrangeItems(_this);

                    invalidateContents(_this);

                    _this->userRequestedAngleDiff = 0;
                    _this->state = LE_RADIAL_MENU_INPUT_READY;
                }
                else
                {
                    // If the user still has his finger pressed in the widget touch area,
                    // do not interfere
                    if (_this->touchPressed == LE_FALSE)
                    {
                        _this->prominentIndex = _this->fn->getProminentIndex(_this);
                        
                        prominentItem = leList_Get(&_this->widgetList, _this->prominentIndex);
                        
                        diffAngle = (PROMINENT_ANGLE - prominentItem->t) * -1;

                        //if the item is past the prominent, roll to the next one
                        if (leAbsoluteValue(diffAngle) >= 5
                                && diffAngle * _this->userRequestedDirection < 0 
                                && _this->shownList.size > 3
                                && _this->ellipseType != LE_RADIAL_MENU_ELLIPSE_TYPE_ROLLODEX)
                        {
                            _this->prominentIndex = _normalizeIndex(_this, _this->prominentIndex + _this->userRequestedDirection);
                        }
                        
                        _this->state = LE_RADIAL_MENU_RESET_TO_INPUT_POS;                        
                    }
                    else
                    {
                        _this->touchPressed = LE_TRUE;
                    }
                }
                break;
            }
            case LE_RADIAL_MENU_RESET_TO_INPUT_POS:
            {
                prominentItem = leList_Get(&_this->widgetList, _this->prominentIndex);

                if (prominentItem != NULL)
                {
                    _this->targetAngleDiff = (PROMINENT_ANGLE - prominentItem->t) * -1;
                }

                if (leAbsoluteValue(_this->targetAngleDiff) >= 2)
                {
                    _this->targetAngleDiff /= 2;     
                                   
                    _rotateMenuItems(_this, _this->targetAngleDiff);
                    arrangeItems(_this);
                }
                else //1 or -1
                {
                    _rotateMenuItems(_this, _this->targetAngleDiff);
                    arrangeItems(_this);
                    
                    _this->targetAngleDiff = 0;
                }

                invalidateContents(_this);
                
                if (_this->targetAngleDiff == 0)
                {
                    if (_this->prominentIndex != _this->lastProminentIndex)
                    {
                        if (_this->itemProminenceChangedEvent != NULL)
                        {
                            _this->itemProminenceChangedEvent(_this, prominentItem->widget, _this->prominentIndex);
                        }

                        _this->lastProminentIndex = _this->prominentIndex;
                    }
                    
                    if(_this != NULL && _this->highlightProminent && _this->highlighter != NULL)
                    {
                        _this->highlighter->fn->setPosition(_this->highlighter, 
                                prominentItem->widget->rect.x - DEFAULT_HIGHLIGHT_MARGIN, 
                                prominentItem->widget->rect.y - DEFAULT_HIGHLIGHT_MARGIN);
                        
                        _this->highlighter->fn->setSize(_this->highlighter, 
                                prominentItem->widget->rect.width + DEFAULT_HIGHLIGHT_MARGIN * 2, 
                                prominentItem->widget->rect.height + DEFAULT_HIGHLIGHT_MARGIN * 2);
                        
                        _this->highlighter->fn->setVisible(_this->highlighter, LE_TRUE);        
                    }
 
                    _this->userRequestedDirection = 0;
                    _this->state = LE_RADIAL_MENU_INPUT_READY;
                }
                break;
            }   
        }
        
        _updateEllipse(_this);
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
            {
                return item;
            }
        }
    }
    
    return NULL;
}

static leResult setHighlightProminent(leRadialMenuWidget* _this,
                                      leBool enable)
{
    LE_ASSERT_THIS();
    
    _this->highlightProminent = enable;

    invalidateContents(_this);
    
    return LE_SUCCESS;
}

static leBool isProminent(const leRadialMenuWidget* _this,
                          const leWidget* widget)
{
    leRadialMenuItemNode* item = NULL;
    uint32_t angle = 0;
    
    LE_ASSERT_THIS();

    item = _getWidgetItem(_this, widget);
    
    if(item == NULL)
        return LE_FALSE;
    
    angle = (uint32_t)_getSliceAngle(_this) / 2;

    //Check if the widget's current t is close to the prominent angle
    if (leAbsoluteValue(item->t - PROMINENT_ANGLE) <= angle)
        return LE_TRUE;
    
    return LE_FALSE;
}

static leResult setProminent(leRadialMenuWidget* _this,
                             const leWidget* widget)
{
    leRadialMenuItemNode* item = NULL;
    
    LE_ASSERT_THIS();

    item = _getWidgetItem(_this, widget);
    
    if(item == NULL)
        return LE_FALSE;
    
    _this->targetAngleDiff = PROMINENT_ANGLE - item->t;
    _this->state =  LE_RADIAL_MENU_HANDLE_USER_MOVE_REQUEST;
    
    invalidateContents(_this);
    
    return LE_SUCCESS;
}

static int32_t getProminentIndex(const leRadialMenuWidget* _this)
{
    uint32_t i;
    leRadialMenuItemNode* item = NULL;
    
    LE_ASSERT_THIS();
    
    if(_this->widgetList.size == 0)
        return -1;
    
    for(i = 0; i < _this->widgetList.size; ++i)
    {
        item = leList_Get(&_this->widgetList, i);
        
        if (item != NULL)
        {
            if(_this->fn->isProminent(_this, item->widget) == LE_TRUE)
            {
                return (int32_t)i;
            }
        }
    }
    
    return -1;
}

static leResult setProminentIndex(leRadialMenuWidget* _this,
                                  int32_t index)
{
    leRadialMenuItemNode* item = NULL;

    LE_ASSERT_THIS();

    if(_this->widgetList.size == 0)
        return LE_FAILURE;

    item = leList_Get(&_this->widgetList, index);
    
    if(item == NULL)
        return LE_FALSE;
    
    _this->targetAngleDiff = PROMINENT_ANGLE - item->t;
    _this->state =  LE_RADIAL_MENU_HANDLE_USER_MOVE_REQUEST;
    
    invalidateContents(_this);
    
    return LE_SUCCESS;
}

static leResult _respaceItems(leRadialMenuWidget* _this)
{
    uint32_t i;
    leRadialMenuItemNode* item = NULL;
    int32_t angle = 0;
    uint32_t frontLastIndex = 0;
    uint32_t backFirstIndex = 0;
    int32_t sliceCount = 0;
    uint32_t itemsShown;
    
    LE_ASSERT_THIS();
    
    if(_this->widgetList.size == 0 || _this->itemsShown == 0)
        return LE_FAILURE;

    if(_this->itemsShown > _this->widgetList.size)
    {
        itemsShown = _this->widgetList.size;
    }
    else
    {
        itemsShown = _this->itemsShown;
    }

    angle = _getSliceAngle(_this);

    frontLastIndex = itemsShown % 2 == 0 ? itemsShown / 2 : itemsShown / 2 + 1;
    backFirstIndex = (_this->widgetList.size - 1) - itemsShown / 2;
    
    leList_Clear(&_this->shownList);
    leList_Clear(&_this->hiddenList);        
    
    for(i = 0; i < _this->widgetList.size; ++i)
    {
        item = leList_Get(&_this->widgetList, i);
        
        if (item != NULL)
        {
            if (i < frontLastIndex ||
                i > backFirstIndex ||
                itemsShown == _this->widgetList.size)
            {
                item->t = sliceCount++ * angle;
                
                leList_PushFront(&_this->shownList, item);
            }
            else
            {
                //Keep the hidden items at 270
                item->t = BACK_ANGLE;
                
                leList_PushFront(&_this->hiddenList, item);
            }

            item->t += PROMINENT_ANGLE;
        }
    }

    _this->state = LE_RADIAL_MENU_RESET_TO_INPUT_POS;
    
    return LE_SUCCESS;
}

static leResult setNumberOfItemsShown(leRadialMenuWidget* _this,
                                      uint32_t num)
{
    LE_ASSERT_THIS();

    if (_this->itemsShown == num)
        return LE_SUCCESS;
    
    _this->itemsShown = leClampi(0, _this->widgetList.size, num);
    
    _respaceItems(_this);
    
    invalidateContents(_this);
    
    return LE_SUCCESS;
}

static leResult addWidget(leRadialMenuWidget* _this,
                          leWidget* widget)
{
    leRadialMenuItemNode* item = NULL;
    leWidgetEventFilter filter;
    
    LE_ASSERT_THIS();
    
    if(widget->parent == (leWidget*)_this)
        return LE_FAILURE;
    
    item = LE_MALLOC(sizeof(leRadialMenuItemNode));
    
    if(item == NULL)
        return LE_FAILURE;
    
    item->widget = widget;

    filter.filterEvent = filterWidgetEvent;
    filter.data = _this;

    widget->fn->installEventFilter(widget, filter);

    item->origAlphaAmount = widget->alphaAmount;
    item->origSize = widget->rect;
    
    leList_PushBack(&_this->widgetList, item);
    
    widget->fn->setParent(widget, (leWidget*)_this);
    
    _deriveWidestTallestItem(_this, item, widget);
    
    _respaceItems(_this);
    arrangeItems(_this);
    
    invalidateContents(_this);

    return LE_SUCCESS;
}

static leResult removeWidget(leRadialMenuWidget* _this,
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
    
    _deriveWidestTallestItem(_this, item, widget);
    
    LE_FREE(&item);
    item = NULL;

    _respaceItems(_this);
    arrangeItems(_this);
    
    invalidateContents(_this);
    
    return LE_SUCCESS;    
}

static leWidget* getWidgetAtIndex(const leRadialMenuWidget* _this,
                                  int32_t idx)
{
    leRadialMenuItemNode* item = NULL;
    
    LE_ASSERT_THIS();

    if(_this->widgetList.size == 0 || (int32_t)_this->widgetList.size < idx - 1)
        return NULL;
    
    item = leList_Get(&_this->widgetList, idx);
    
    return item->widget;
}

static leResult setWidgetAtIndex(leRadialMenuWidget* _this,
                                 int32_t idx,
                                 leWidget* widget)
{
    leRadialMenuItemNode* item = NULL;

    LE_ASSERT_THIS();

    if(widget->parent == (leWidget*)_this)
        return LE_FAILURE;
    
    if(idx < (int32_t)_this->widgetList.size)
    {
        item = leList_Get(&_this->widgetList, idx);
    }
    else
    {
        item = LE_MALLOC(sizeof(leRadialMenuItemNode));
        
        leList_InsertAt(&_this->widgetList, item, idx);
    }    
    
    if(item == NULL)
        return LE_FAILURE;
    
    item->widget = widget;

    //item->origTouchDown = widget->touchDown;
    //item->origTouchUp   = widget->touchUp;
    //item->origTouchMoved = widget->touchMoved;
    
    item->origAlphaAmount = widget->alphaAmount;
    item->origSize = widget->rect;
    
    //leWidget_OverrideTouchDownEvent(widget, &_leRadialMenuWidget_TouchDownEvent);            
    //leWidget_OverrideTouchMovedEvent(widget, &_leRadialMenuWidget_TouchMovedEvent);                
    //leWidget_OverrideTouchUpEvent(widget, &_leRadialMenuWidget_TouchUpEvent);
        
    widget->fn->setParent(widget, (leWidget*)_this);
    
    _deriveWidestTallestItem(_this, item, widget);

    _respaceItems(_this);
    arrangeItems(_this);

    invalidateContents(_this);

    return LE_SUCCESS;
}

static leResult removeAllWidgets(leRadialMenuWidget* _this)
{
    uint32_t i = 0;
    leRadialMenuItemNode* item = NULL;
    
    LE_ASSERT_THIS();
    
    _this->tallestWidgetItem = NULL;
    _this->widestWidgetItem = NULL;
    
    for(i = 0; i < _this->widgetList.size; ++i)
    {
        item = leList_Get(&_this->widgetList, i);
        
        if (item != NULL)
        {
            //leWidget_OverrideTouchDownEvent(item->widget, item->origTouchDown);            
            //leWidget_OverrideTouchMovedEvent(item->widget, item->origTouchMoved);                
            //leWidget_OverrideTouchUpEvent(item->widget, item->origTouchUp);                    
 
            LE_FREE(&item);
            
            item = NULL;
        }
    }
    
    leList_Clear(&_this->shownList);
    leList_Clear(&_this->hiddenList);
    leList_Clear(&_this->widgetList);
    
    invalidateContents(_this);
    
    return LE_SUCCESS;    
}

static leResult setScaleMode(leRadialMenuWidget* _this,
                             leRadialMenuWidgetInterpolationMode mode)
{
    LE_ASSERT_THIS();

    _this->ellipseChanged = LE_TRUE;
    _this->scaleMode = mode;

    arrangeItems(_this);

    invalidateContents(_this);

    _this->state = LE_RADIAL_MENU_RESET_TO_INPUT_POS;
    
    return LE_SUCCESS;    
}

static leResult setScaleRange(leRadialMenuWidget* _this,
                              int32_t min,
                              int32_t max)
{
    LE_ASSERT_THIS();
    
    _this->maxSizePercent = leClampi(MIN_SIZE_PERCENT, MAX_SIZE_PERCENT, max);
    _this->minSizePercent = leClampi(MIN_SIZE_PERCENT, MAX_SIZE_PERCENT, min);
    
    _this->ellipseChanged = LE_TRUE;

    arrangeItems(_this);

    invalidateContents(_this);

    return LE_SUCCESS;    
}

static leResult setBlendMode(leRadialMenuWidget* _this,
                             leRadialMenuWidgetInterpolationMode mode)
{
    uint32_t i = 0;
    leRadialMenuItemNode* item = NULL;

    LE_ASSERT_THIS();

    _this->blendMode = mode;
    
    for(i = 0; i < _this->widgetList.size; ++i)
    {
        item = leList_Get(&_this->widgetList, i);

        if (item != NULL)
        {
            item->widget->fn->setAlphaEnabled(item->widget, mode != LE_RADIAL_MENU_INTERPOLATE_OFF);
        }
    }
    
    invalidateContents(_this);

    _this->state = LE_RADIAL_MENU_RESET_TO_INPUT_POS;
    
    return LE_SUCCESS;    
}

static leResult setBlendRange(leRadialMenuWidget* _this,
                              int32_t min,
                              int32_t max)
{
    LE_ASSERT_THIS();
    
    _this->maxAlphaAmount = leClampi(MIN_ALPHA_PERCENT, MAX_ALPHA_PERCENT, max);
    _this->minAlphaAmount = leClampi(MIN_ALPHA_PERCENT, MAX_ALPHA_PERCENT, min);
    
    invalidateContents(_this);

    return LE_SUCCESS;    
}

static leResult setDrawEllipse(leRadialMenuWidget* _this,
                               leBool enable)
{
    LE_ASSERT_THIS();
    
    _this->drawEllipse = enable;
    
    invalidateContents(_this);
    
    return LE_SUCCESS;
}

static leResult setTouchArea(leRadialMenuWidget* _this, 
                             int32_t x,
                             int32_t y,
                             int32_t width,
                             int32_t height)
{
    leRect rect;
    
    LE_ASSERT_THIS();

    rect.x = x;
    rect.y = y;
    rect.width = width;
    rect.height = height;
    
    _this->touchArea = rect;
    
    return LE_SUCCESS;    
}

static leResult setEllipseType(leRadialMenuWidget* _this,
                               leRadialMenuEllipseType type)
{
    LE_ASSERT_THIS();

    _this->ellipseType = type;

    invalidateContents(_this);
    
    return LE_SUCCESS;
}

static int32_t getTheta(const leRadialMenuWidget* _this)
{
    LE_ASSERT_THIS();
    
    return _this->theta;
}

static leResult setTheta(leRadialMenuWidget* _this,
                         int32_t theta)
{
    LE_ASSERT_THIS();
    
    _this->theta = leClampi(MIN_THETA, MAX_THETA, theta);
    _this->ellipseChanged = LE_TRUE;
    
    invalidateContents(_this);

    return LE_SUCCESS;
}

static leRadialMenuWidget_ItemSelectedEvent getItemSelectedEventCallback(const leRadialMenuWidget* _this)
{
    LE_ASSERT_THIS();

    return _this->itemSelectedEvent;
}

static leResult setItemSelectedEventCallback(leRadialMenuWidget* _this,
                                             leRadialMenuWidget_ItemSelectedEvent cb)
{
    LE_ASSERT_THIS();
    
    if(_this->itemSelectedEvent == cb)
        return LE_FAILURE;

    _this->itemSelectedEvent = cb;
    
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
    leRadialMenuItemNode* widgetItem;
    leRect clipRect;
    lePoint touchPoint;
    
    if (_this->state != LE_RADIAL_MENU_INPUT_READY)
        return;

    /*if(_this->fn->getType(_this) == LE_WIDGET_RADIAL_MENU)
    {
        _this = (leRadialMenuWidget*)widget;


    }*/
#if 0
    else if(widget->parent->type == LE_WIDGET_RADIAL_MENU)
    {
        _this = (leRadialMenuWidget*)widget->parent;
        
        if (_this->state != LE_RADIAL_MENU_INPUT_READY)
            return;
    
        clipRect = _this->touchArea;
        
        leUtils_RectToScreenSpace((leWidget*)_this, &clipRect);
        
        touchPoint.x = evt->x;
        touchPoint.y = evt->y;
        
        //Only response if the widget is in the touch allowed area and is 
        //the prominent widget
        if (leRectContainsPoint(&clipRect, &touchPoint) == LE_TRUE &&
            _this->fn->isProminent(_this, widget) == LE_TRUE)
        {
            widgetItem = _getWidgetItem( _this, widget );    

            if (widgetItem != NULL)
            {
                //widgetItem->origTouchDown(widget, evt);
            }            
        }        
    }
#endif
        
    _this->touchPressed = LE_TRUE;
    
    if (_this != NULL && _this->highlighter != NULL)
    {
        _this->highlighter->fn->setVisible(_this->highlighter, LE_FALSE);        
    }

    leWidgetEvent_Accept((leWidgetEvent*)evt, (leWidget*)_this);
}

static void handleTouchUpEvent(leRadialMenuWidget* _this,
                               leWidgetEvent_TouchUp* evt)
{
    leRadialMenuItemNode* widgetItem;
    leRect clipRect;
    lePoint touchPoint;
    
#if 0
    if(widget->parent == NULL)
        return;

    if(widget->type == LE_WIDGET_RADIAL_MENU)
    {
        _this = (leRadialMenuWidget*)widget;
    }
    else if(widget->parent->type == LE_WIDGET_RADIAL_MENU)
    {
        _this = (leRadialMenuWidget*)widget->parent;
        
        clipRect = _this->touchArea;
        
        leUtils_RectToScreenSpace((leWidget*)_this, &clipRect);
        
        touchPoint.x = evt->x;
        touchPoint.y = evt->y;
        
        //Only response if the widget is in the touch allowed area
        if (leRectContainsPoint(&clipRect, &touchPoint) == LE_TRUE  &&
            _this->fn->isProminent(_this, widget) == LE_TRUE &&
            _this->state == LE_RADIAL_MENU_INPUT_READY)
        {
            widgetItem = _getWidgetItem( _this, widget );    

            if (widgetItem != NULL)
            {
                //widgetItem->origTouchUp(widget, evt);
            }

            if (_this->itemSelectedEvent != NULL)
            {
                _this->itemSelectedEvent(_this, widget, _this->prominentIndex);
            }
        }        
    }
#endif

    _this->touchPressed = LE_FALSE;
    
    if (_this != NULL && _this->highlighter != NULL)
    {
        _this->highlighter->fn->setVisible(_this->highlighter, LE_FALSE);        
    }

    leWidgetEvent_Accept((leWidgetEvent*)evt, (leWidget*)_this);
}

static int touch = 0;

//#include <stdio.h>

static void handleTouchMovedEvent(leRadialMenuWidget* _this,
                                  leWidgetEvent_TouchMove* evt)
{
    leRect clipRect;
    lePoint touchPoint;
	int32_t dx, dy;
	
    clipRect = _this->touchArea;

    leUtils_RectToScreenSpace((leWidget*)_this, &clipRect);

    touchPoint.x = evt->x;
    touchPoint.y = evt->y;

    //Only response if the widget is in the touch allowed area
    if (leRectContainsPoint(&clipRect, &touchPoint) == LE_TRUE)
    {
        dy = evt->y - evt->prevY;    
        dx = evt->x - evt->prevX;    
        
        _this->touchPressed = LE_TRUE;

        _this->userRequestedAngleDiff = dx;

        //if (_this->theta <= 45)
        //{
            //_this->userRequestedAngleDiff += dx * leSineCosineGet(_this->theta, LE_TRIG_COSINE_TYPE) / 256 / 4;
        //}
        /*else
        {
            _this->userRequestedAngleDiff += dy * leSineCosineGet(_this->theta, LE_TRIG_SINE_TYPE) / 256 / 2;
        }*/
        
        if (_this->userRequestedAngleDiff != 0)
        {
            _this->state = LE_RADIAL_MENU_HANDLE_USER_MOVE_REQUEST;
            
            if (_this->userRequestedAngleDiff > 0)
            {
                _this->userRequestedDirection = 1;
            }
            else if (_this->userRequestedAngleDiff < 0)
            {
                _this->userRequestedDirection = -1;
            }
        }

        //printf("%d: %d, %d\n", touch, _this->userRequestedAngleDiff, _this->userRequestedDirection);

        //touch++;
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
    radialMenuWidgetVTable.languageChangeEvent = handleLanguageChangeEvent;
    radialMenuWidgetVTable.touchDownEvent = handleTouchDownEvent;
    radialMenuWidgetVTable.touchUpEvent = handleTouchUpEvent;
    radialMenuWidgetVTable.touchMoveEvent = handleTouchMovedEvent;
    radialMenuWidgetVTable.resizeEvent = handleResizedEvent;
    
    /* member functions */
    radialMenuWidgetVTable.isProminent = isProminent;
    radialMenuWidgetVTable.setProminent = setProminent;
    radialMenuWidgetVTable.getProminentIndex = getProminentIndex;
    radialMenuWidgetVTable.setProminentIndex = setProminentIndex;
    radialMenuWidgetVTable.setNumberOfItemsShown = setNumberOfItemsShown;
    radialMenuWidgetVTable.setHighlightProminent = setHighlightProminent;
    radialMenuWidgetVTable.getTheta = getTheta;
    radialMenuWidgetVTable.setTheta = setTheta;
    radialMenuWidgetVTable.setEllipseType = setEllipseType;
    radialMenuWidgetVTable.addWidget = addWidget;
    radialMenuWidgetVTable.removeWidget = removeWidget;
    radialMenuWidgetVTable.getWidgetAtIndex = getWidgetAtIndex;
    radialMenuWidgetVTable.setWidgetAtIndex = setWidgetAtIndex;
    radialMenuWidgetVTable.removeAllWidgets = removeAllWidgets;
    radialMenuWidgetVTable.setScaleMode = setScaleMode;
    radialMenuWidgetVTable.setScaleRange = setScaleRange;
    radialMenuWidgetVTable.setBlendMode = setBlendMode;
    radialMenuWidgetVTable.setBlendRange = setBlendRange;
    radialMenuWidgetVTable.setTouchArea = setTouchArea;
    radialMenuWidgetVTable.setDrawEllipse = setDrawEllipse;
    radialMenuWidgetVTable.getItemSelectedEventCallback = getItemSelectedEventCallback;
    radialMenuWidgetVTable.setItemSelectedEventCallback = setItemSelectedEventCallback;
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
    .addChild = (void*)_leWidget_AddChild,
    .removeChild = (void*)_leWidget_RemoveChild,
    .removeAllChildren = (void*)_leWidget_RemoveAllChildren,
    .getRootWidget = (void*)_leWidget_GetRootWidget,
    .setParent = (void*)_leWidget_SetParent,
    .getChildCount = (void*)_leWidget_GetChildCount,
    .getChildAtIndex = (void*)_leWidget_GetChildAtIndex,
    .getIndexOfChild = (void*)_leWidget_GetIndexOfChild,
    .containsDescendent = (void*)_leWidget_ContainsDescendent,
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
    ._paint = _leRadialMenuWidget_Paint,
    .update = update,
    .languageChangeEvent = handleLanguageChangeEvent,
    .touchDownEvent = handleTouchDownEvent,
    .touchUpEvent = handleTouchUpEvent,
    .touchMoveEvent = handleTouchMovedEvent,
    .resizeEvent = handleResizedEvent,

    /* member functions */
    .isProminent = isProminent,
    .setProminent = setProminent,
    .getProminentIndex = getProminentIndex,
    .setProminentIndex = setProminentIndex,
    .setNumberOfItemsShown = setNumberOfItemsShown,
    .setHighlightProminent = setHighlightProminent,
    .getTheta = getTheta,
    .setTheta = setTheta,
    .setEllipseType = setEllipseType,
    .addWidget = addWidget,
    .removeWidget = removeWidget,
    .getWidgetAtIndex = getWidgetAtIndex,
    .setWidgetAtIndex = setWidgetAtIndex,
    .removeAllWidgets = removeAllWidgets,
    .setScaleMode = setScaleMode,
    .setScaleRange = setScaleRange,
    .setBlendMode = setBlendMode,
    .setBlendRange = setBlendRange,
    .setTouchArea = setTouchArea,
    .setDrawEllipse = setDrawEllipse,
    .getItemSelectedEventCallback = getItemSelectedEventCallback,
    .setItemSelectedEventCallback = setItemSelectedEventCallback,
    .getItemProminenceChangedEventCallback = getItemProminenceChangedEventCallback,
    .setItemProminenceChangedEventCallback = setItemProminenceChangedEventCallback,
};
#endif

#endif // LE_RADIALMENU_WIDGET_ENABLED