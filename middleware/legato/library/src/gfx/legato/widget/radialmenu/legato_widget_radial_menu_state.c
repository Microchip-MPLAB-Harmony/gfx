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
#define DEFAULT_A               70
#define DEFAULT_B               30
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

static int _prevItem(uint32_t itemCount,
                     int32_t curr)
{
    int prevIdx = curr;

    if(prevIdx == 0)
    {
        prevIdx = itemCount - 1;
    }
    else
    {
        prevIdx--;
    }

    return prevIdx;
}

static int _firstItem(uint32_t currentItem,
                      uint32_t itemCount,
                      uint32_t visibleItemCount)
{
    int item = currentItem;

    if(itemCount == 0)
        return -1;

    for(uint32_t i = 0; i < visibleItemCount / 2; i++)
    {
        item = _prevItem(itemCount, item);
    }

    return item;
}

static int _nextItem(uint32_t itemCount,
                     int32_t curr)
{
    uint32_t item = curr + 1;

    if(item >= itemCount)
        return 0;

    return item;
}

static void _calculateEllipseScale(leRadialMenuWidget* _this)
{
    uint32_t i;
    leRadialMenuItemNode* item = NULL;

    _this->paintState.maxWidth = 0;
    _this->paintState.maxHeight = 0;

    if(_this == NULL || _this->widgetList.size == 0)
        return;

    for(i = 0; i < _this->widgetList.size; ++i)
    {
        item = leList_Get(&_this->widgetList, i);

        if (item != NULL)
        {
            if(item->widget->fn->getWidth(item->widget) > _this->paintState.maxWidth)
            {
                _this->paintState.maxWidth = item->widget->fn->getWidth(item->widget);
            }

            if(item->widget->fn->getHeight(item->widget) > _this->paintState.maxHeight)
            {
                _this->paintState.maxHeight = item->widget->fn->getHeight(item->widget);
            }
        }
    }
}

static leResult _calculateEllipse(leRadialMenuWidget* _this)
{
    //int16_t sine, cosine = 0;
    int32_t widest, tallest, scale = 0;
    

    LE_ASSERT_THIS();

    _calculateEllipseScale(_this);

    //sine = leSin(_this->ellipse.theta);
    //cosine = leCos(_this->ellipse.theta);

    scale = _this->scaleMode != LE_RADIAL_MENU_INTERPOLATE_OFF ? _this->maxSizePercent : 100;
    widest = _this->paintState.maxWidth;
    tallest = (_this->paintState.maxHeight * scale) / 100;

    if (_this->highlightProminent == LE_TRUE)
    {
        widest += DEFAULT_HIGHLIGHT_MARGIN * 2;
        tallest += DEFAULT_HIGHLIGHT_MARGIN * 2;
    }

    /*switch(_this->ellipse.type)
    {
        case LE_RADIAL_MENU_ELLIPSE_TYPE_DEFAULT:
        {
            _this->ellipse.b = _this->widget.rect.height / 2 - tallest / 2;
            _this->ellipse.a = _this->widget.rect.width / 2 - widest / 2;

            _this->ellipse.a = leClampi(0, _this->widget.rect.width / 2,  _this->ellipse.a);
            _this->ellipse.b = leClampi(0, _this->widget.rect.height / 2, _this->ellipse.b);

            break;
        }
        case LE_RADIAL_MENU_ELLIPSE_TYPE_ORBITAL:
        {
            _this->ellipse.b = leAbsoluteValue((_this->widget.rect.height / 2 - tallest / 2) * sine / 256 - (widest / 2) * sine / 256);
            _this->ellipse.a = leAbsoluteValue((_this->widget.rect.width / 2 - widest / 2) * cosine / 256 - (tallest / 2) * sine / 256);

            _this->ellipse.a = leClampi(0, _this->widget.rect.width / 2 * cosine / 256,  _this->ellipse.a);
            _this->ellipse.b = leClampi(0, _this->widget.rect.height / 2 * sine / 256, _this->ellipse.b);

            break;
        }
        case LE_RADIAL_MENU_ELLIPSE_TYPE_ROLLODEX:
        {
            _this->ellipse.b = 0;
            _this->ellipse.a = _this->widget.rect.height / 2 - tallest / 2;
            _this->ellipse.theta = 90;

            break;
        }
        default:
        {
            break;
        }
    }*/

    _this->ellipse.invalid = LE_FALSE;

    return LE_SUCCESS;
}

static leRadialMenuItemNode* _nextZItem(leRadialMenuWidget* mn)
{
    leRadialMenuItemNode* item = NULL;
    leRadialMenuItemNode* highestZ = NULL;
    int32_t highestAngle = -360;
    //int32_t highestIndex = -1;
    int32_t i;

    for(i = 0; i < (int32_t)mn->widgetList.size; ++i)
    {
        item = leList_Get(&mn->widgetList, i);

        if(item->state != LE_RMI_WAITING_FOR_SORT)
            continue;

        /*if(highestZ == NULL)
        {
            highestZ = item;
            highestIndex = i;

            continue;
        }*/

        item->absAngle = item->adjustedAngle;

        if(item->absAngle < 0)
        {
            item->absAngle *= -1;
        }

        if(item->absAngle > highestAngle)
        {
            highestAngle = item->absAngle;
            highestZ = item;
            //highestIndex = i;
        }
    }

    return highestZ;
}

static void calculateScaling(leRadialMenuItemNode* item,
                             enum leRadialMenuWidgetInterpolationMode mode,
                             uint32_t min,
                             uint32_t max,
                             leBool isProminent)
{
    switch(mode)
    {
        case LE_RADIAL_MENU_INTERPOLATE_GRADUAL:
        {
            item->widget->fn->setSize(item->widget,
                                      item->origSize.width * leLerp(min, max, item->percent) / 100,
                                      item->origSize.height * leLerp(min, max, item->percent) / 100);

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
            if(isProminent)
            {
                item->widget->fn->setSize(item->widget,
                                          item->origSize.width * (int32_t)max / 100,
                                          item->origSize.height * (int32_t)max / 100);
            }
            else
            {
                item->widget->fn->setSize(item->widget,
                                          item->origSize.width * (int32_t)min / 100,
                                          item->origSize.height * (int32_t)min / 100);
            }

            break;
        }
    }
}

#if LE_ALPHA_BLENDING_ENABLED == 1
static void calculateBlending(leRadialMenuItemNode* item,
                              enum leRadialMenuWidgetInterpolationMode mode,
                              uint32_t min,
                              uint32_t max,
                              leBool isProminent)
{
    uint32_t val;

    switch(mode)
    {
        case LE_RADIAL_MENU_INTERPOLATE_GRADUAL:
        {
            item->widget->fn->setAlphaEnabled(item->widget, LE_TRUE);

            val = leLerp(min, max, item->percent);

            item->widget->fn->setAlphaAmount(item->widget, val);

            break;
        }
        case LE_RADIAL_MENU_INTERPOLATE_OFF:
        {
            item->widget->fn->setAlphaEnabled(item->widget, LE_FALSE);
            item->widget->style.alphaAmount = item->origAlphaAmount;

            break;
        }
        case LE_RADIAL_MENU_INTERPOLATE_PROMINENT:
        {
            if (isProminent)
            {
                val = lePercentOf(255, max);

                item->widget->fn->setAlphaEnabled(item->widget, LE_TRUE);
                item->widget->fn->setAlphaAmount(item->widget, val);
            }
            else
            {
                val = lePercentOf(255, min);

                item->widget->fn->setAlphaEnabled(item->widget, LE_TRUE);
                item->widget->fn->setAlphaAmount(item->widget, val);
            }

            break;
        }
    }
}
#endif

static void calculatePosition(leRadialMenuItemNode* item,
                              int32_t a,
                              int32_t b,
                              int32_t theta,
                              uint32_t maxWidth,
                              uint32_t maxHeight)
{
    lePoint itemPos;

    // calculate ellipse point
    leEllipsePoint(item->paintAngle, a, b, theta, &itemPos);

    // translate item pos based on parent and child widget dimensions
    itemPos.x = itemPos.x + maxWidth / 2 - item->widget->rect.width / 2;
    itemPos.y = itemPos.y + maxHeight / 2 - item->widget->rect.height / 2;

    // set child item position
    item->widget->fn->setPosition(item->widget, itemPos.x, itemPos.y);
}

#include <stdio.h>

void _leRadialMenu_RecalculateArrangement(leRadialMenuWidget* mn)
{
    uint32_t currItem;
    uint32_t leftItems;
    int32_t drawPoint;

    leRadialMenuItemNode* item;
    uint32_t i;

    if(mn->positionsInvalid == LE_FALSE)
        return;

    for(i = 0; i < mn->widgetList.size; ++i)
    {
        item = leList_Get(&mn->widgetList, i);

        item->widget->fn->setVisible(item->widget, LE_FALSE);

        item->state = LE_RMI_NOT_VISIBLE;
    }

    if(mn->widgetList.size < 3)
    {
        mn->positionsInvalid = LE_FALSE;

        return;
    }

    leftItems = mn->itemsShown / 2;
    mn->angleSlice = 360 / mn->itemsShown;
    drawPoint = 0 - (mn->angleSlice * leftItems);

    mn->paintState.firstVisibleItem = _firstItem(mn->prominentIndex,
                                                 mn->widgetList.size,
                                                 mn->itemsShown);

    //printf("current: %i\n", mn->paintState.currentItem);
    //printf("first visible: %i\n", mn->paintState.firstVisibleItem);

    mn->paintState.lastZ = 360;

    // clear existing children array
    // children are still stored as radial menu items
    leArray_Clear(&mn->widget.children);

    for(i = 0; i < mn->widgetList.size; ++i)
    {
        item = leList_Get(&mn->widgetList, i);

        item->widget->fn->setVisible(item->widget, LE_FALSE);

        item->state = LE_RMI_NOT_VISIBLE;
    }

    // for each visible item, set it to be sorted by Z order
    currItem = mn->paintState.firstVisibleItem;

    for(i = 0; i < mn->itemsShown; ++i)
    {
        item = leList_Get(&mn->widgetList, currItem);

        if(leArray_Find(&mn->widget.children, item->widget) == -1)
        {
            item->state = LE_RMI_WAITING_FOR_SORT;
            item->angle = drawPoint + (mn->angleSlice * i);
            item->adjustedAngle = item->angle + mn->rotationDegrees;
            item->paintAngle = item->adjustedAngle + 90;
        }

        currItem = _nextItem(mn->widgetList.size, currItem);
    }

    // sort the visible items by Z and add back to child list
    for(i = 0; i < mn->itemsShown; ++i)
    {
        if(i >= mn->widgetList.size)
            continue;

        item = _nextZItem(mn);

        leArray_PushBack(&mn->widget.children, item->widget);
        item->widget->fn->setVisible(item->widget, LE_TRUE);

        if(i < mn->itemsShown - 1)
        {
            item->state = LE_RMI_VISIBLE;
        }
        // set the last item (closest to angle 0) to prominent
        else
        {
            item->state = LE_RMI_PROMINENT;
        }

        // calculate the item distance from 180 (back of the ellipse) and invert
        item->percent = lePercentWholeRounded(item->absAngle, 180);
        item->percent = 100 - item->percent;

        calculateScaling(item,
                         mn->scaleMode,
                         mn->minSizePercent,
                         mn->maxSizePercent,
                         mn->fn->isProminent(mn, item->widget));

#if LE_ALPHA_BLENDING_ENABLED == 1
        calculateBlending(item,
                          mn->blendMode,
                          mn->minAlphaAmount,
                          mn->maxAlphaAmount,
                          mn->fn->isProminent(mn, item->widget));
#endif

        calculatePosition(item,
                          mn->ellipse.a,
                          mn->ellipse.b,
                          mn->ellipse.theta,
                          mn->fn->getWidth(mn),
                          mn->fn->getHeight(mn));
    }

    // add all invisible items back into child list
    for(i = 0; i < mn->widgetList.size; ++i)
    {
        item = leList_Get(&mn->widgetList, i);

        if(item->state == LE_RMI_NOT_VISIBLE)
        {
            leArray_PushBack(&mn->widget.children, item->widget);
        }
    }

    mn->fn->invalidate(mn);

    mn->positionsInvalid = LE_FALSE;
}

void _leRadialMenu_RebuildEllipse(leRadialMenuWidget* wgt)
{
    if(wgt->widgetList.size == 0)
        return;

    _calculateEllipse(wgt);
    _leRadialMenu_RecalculateArrangement(wgt);
}

#endif // LE_RADIALMENU_WIDGET_ENABLED
