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
#include "gfx/legato/widget/piechart/legato_widget_pie_chart.h"

#if LE_PIECHART_WIDGET_ENABLED == 1

#include "gfx/legato/common/legato_error.h"
#include "gfx/legato/common/legato_math.h"
#include "gfx/legato/common/legato_utils.h"
#include "gfx/legato/memory/legato_memory.h"
#include "gfx/legato/string/legato_fixedstring.h"
#include "gfx/legato/widget/legato_widget.h"


#define DEFAULT_WIDTH           101
#define DEFAULT_HEIGHT          101

#define DEFAULT_RADIUS          50
#define DEFAULT_LABEL_OFFSET    40
#define DEFAULT_START_ANGLE     0
#define DEFAULT_CENTER_ANGLE    180
#define DEFAULT_VALUE           10

static
#if LE_DYNAMIC_VTABLES == 0
const
#endif
lePieChartWidgetVTable pieChartWidgetVTable;

void lePieChartWidget_Constructor(lePieChartWidget* _this)
{
    leWidget_Constructor((leWidget*)_this);
    
    _this->widget.fn = (void*)&pieChartWidgetVTable;
    _this->fn = &pieChartWidgetVTable;

    _this->widget.type = LE_WIDGET_ARC;
    
    _this->widget.rect.width = DEFAULT_WIDTH;
    _this->widget.rect.height = DEFAULT_HEIGHT;

    _this->widget.borderType = LE_WIDGET_BORDER_NONE;
    _this->widget.backgroundType = LE_WIDGET_BACKGROUND_NONE;

    _this->startAngle = DEFAULT_START_ANGLE;
    _this->centerAngle = DEFAULT_CENTER_ANGLE;
    
    _this->labelsVisible = LE_TRUE;
    _this->labelsOffset = DEFAULT_LABEL_OFFSET;
    
    leArray_Create(&_this->pieArray);

    _this->labelFont = NULL;

    _this->pressedCallback = NULL;
}

void _leWidget_Destructor(leWidget* _this);

static void destructor(lePieChartWidget* _this)
{
    _this->fn->clear(_this);
    
    _leWidget_Destructor((leWidget*)_this);
}

lePieChartWidget* lePieChartWidget_New()
{
    lePieChartWidget* chart = NULL;

    chart = LE_MALLOC(sizeof(lePieChartWidget));

    if(chart == NULL)
        return NULL;
    
    lePieChartWidget_Constructor(chart);

    return chart;
}

static int32_t getStartAngle(const lePieChartWidget* _this)
{
    LE_ASSERT_THIS();
        
    return _this->startAngle;
}

static leResult setStartAngle(lePieChartWidget* _this,
                              int32_t angle)
{
    LE_ASSERT_THIS();
        
    if(_this->startAngle == angle)
        return LE_SUCCESS;
        
    _this->startAngle = angle;
    
    _this->fn->invalidate(_this);
        
    return LE_SUCCESS;
}

static int32_t getCenterAngle(const lePieChartWidget* _this)
{
    LE_ASSERT_THIS();
        
    return _this->centerAngle;
}

static leResult setCenterAngle(lePieChartWidget* _this,
                               int32_t angle)
{
    LE_ASSERT_THIS();

    if(_this->centerAngle == angle)
        return LE_SUCCESS;
        
    _this->centerAngle = angle;
    
    _this->fn->invalidate(_this);
        
    return LE_SUCCESS;
}

static int32_t addEntry(lePieChartWidget* _this)
{
    lePieChartPie* pie;
    
    LE_ASSERT_THIS();
    
    pie = LE_MALLOC(sizeof(lePieChartPie));
    
    if(pie == NULL)
        return LE_FAILURE;
    
    pie->offset = 0;
    pie->radius = DEFAULT_RADIUS;
    pie->scheme = _this->fn->getScheme(_this);
    pie->value = DEFAULT_VALUE;
    
    if(leArray_PushBack(&_this->pieArray, pie) == LE_FAILURE)
    {
        LE_FREE(pie);
        
        return -1;
    }
    
    _this->fn->invalidate(_this);
    
    return _this->pieArray.size - 1;
}

static leResult clear(lePieChartWidget* _this)
{
    uint32_t i;
    
    for(i = 0; i < _this->pieArray.size; i++)
    {
        LE_FREE(_this->pieArray.values[i]);
    }
    
    leArray_Clear(&_this->pieArray);

    _this->fn->invalidate(_this);
        
    return LE_SUCCESS;
}

static uint32_t getEntryValue(const lePieChartWidget* _this,
                              int32_t index)
{
    lePieChartPie* pie;
    
    LE_ASSERT_THIS();
    
    pie = (lePieChartPie*)leArray_Get(&_this->pieArray, index);
    
    if(pie == NULL)
        return 0;
    
    return pie->value;
}

static leResult setEntryValue(lePieChartWidget* _this,
                              int32_t index,
                              uint32_t value)
{
    lePieChartPie* pie;
    
    LE_ASSERT_THIS();
        
    pie = (lePieChartPie*)leArray_Get(&_this->pieArray, index);
    
    if(pie == NULL)
        return LE_FAILURE;
    
    if(pie->value == value)
        return LE_SUCCESS;
    
    pie->value = value;
    
    _this->fn->invalidate(_this);
        
    return LE_SUCCESS;
}

static uint32_t getEntryRadius(const lePieChartWidget* _this,
                               int32_t index)
{
    lePieChartPie* pie;
    
    LE_ASSERT_THIS();
    
    pie = (lePieChartPie*)leArray_Get(&_this->pieArray, index);
    
    if(pie == NULL)
        return 0;
    
    return pie->radius;
}

static leResult setEntryRadius(lePieChartWidget* _this,
                               int32_t index,
                               uint32_t rad)
{
    lePieChartPie* pie;
    
    LE_ASSERT_THIS();
        
    pie = (lePieChartPie*)leArray_Get(&_this->pieArray, index);
    
    if(pie == NULL)
        return LE_FAILURE;
    
    if(pie->radius == rad)
        return LE_SUCCESS;
    
    pie->radius = rad;
    
    _this->fn->invalidate(_this);
        
    return LE_SUCCESS;
}

static uint32_t getEntryOffset(const lePieChartWidget* _this,
                               int32_t index)
{
    lePieChartPie* pie;
    
    LE_ASSERT_THIS();
    
    pie = (lePieChartPie*)leArray_Get(&_this->pieArray, index);
    
    if(pie == NULL)
        return 0;
    
    return pie->offset;
}

static leResult setEntryOffset(lePieChartWidget* _this,
                               int32_t index,
                               uint32_t offset)
{
    lePieChartPie* pie;
    
    LE_ASSERT_THIS();
        
    pie = (lePieChartPie*)leArray_Get(&_this->pieArray, index);
    
    if(pie == NULL)
        return LE_FAILURE;
    
    if(pie->offset == offset)
        return LE_SUCCESS;
    
    pie->offset = offset;
    
    _this->fn->invalidate(_this);
        
    return LE_SUCCESS;
}

static leScheme* getEntryScheme(const lePieChartWidget* _this,
                                int32_t index)
{
    lePieChartPie* pie;
    
    LE_ASSERT_THIS();
    
    pie = (lePieChartPie*)leArray_Get(&_this->pieArray, index);
    
    if(pie == NULL)
        return 0;
    
    return (leScheme*)pie->scheme;
}

static leResult setEntryScheme(lePieChartWidget* _this, 
                               int32_t index,
                               const leScheme* scheme)
{
    lePieChartPie* pie;
    
    LE_ASSERT_THIS();
        
    pie = (lePieChartPie*)leArray_Get(&_this->pieArray, index);
    
    if(pie == NULL)
        return LE_FAILURE;
    
    if(pie->scheme == scheme)
        return LE_SUCCESS;
    
    pie->scheme = scheme;
    
    _this->fn->invalidate(_this);
        
    return LE_SUCCESS;
}

static lePieChartWidget_PressedEvent getPressedEventCallback(const lePieChartWidget* _this)
{
    LE_ASSERT_THIS();

    return _this->pressedCallback;
}

static leResult setPressedEventCallback(lePieChartWidget* _this,
                                        lePieChartWidget_PressedEvent cb)
{
    LE_ASSERT_THIS();

    _this->pressedCallback = cb;
        
    return LE_SUCCESS;
}

static leFont* getLabelFont(const lePieChartWidget* _this)
{
    LE_ASSERT_THIS();
    
    return (leFont*)_this->labelFont;
}

static leResult setLabelFont(lePieChartWidget* _this,
                             const leFont* fnt)
{
    LE_ASSERT_THIS();
        
    _this->labelFont = fnt;
    
    _this->fn->invalidate(_this);
        
    return LE_SUCCESS;
}

static leBool getLabelsVisible(const lePieChartWidget* _this)
{
    LE_ASSERT_THIS();

    return _this->labelsVisible;
}

static leResult setLabelsVisible(lePieChartWidget* _this, 
                                 leBool visible)
{
    LE_ASSERT_THIS();

    if(_this->labelsVisible == visible)
        return LE_SUCCESS;

    _this->labelsVisible = visible;

    _this->fn->invalidate(_this);

    return LE_SUCCESS;
}

static uint32_t getLabelsOffset(const lePieChartWidget* _this)
{
    LE_ASSERT_THIS();

    return _this->labelsOffset;
}

static leResult setLabelsOffset(lePieChartWidget* _this, 
                                uint32_t offset)
{
    LE_ASSERT_THIS();

    if(_this->labelsOffset == offset)
        return LE_SUCCESS;

    _this->labelsOffset = offset;

    _this->fn->invalidate(_this);

    return LE_SUCCESS;
}

static int32_t _getPieIndexAtPoint(lePieChartWidget* _this,
                                   lePoint pnt)
{
    int32_t tempAngle, centerAngle;
    uint32_t pointAngle;
    uint32_t startAngle, endAngle;
    uint32_t i;
    uint32_t totalValue = 0;
    lePieChartPie* pie;

    tempAngle = _this->startAngle;
    
    while (tempAngle < 0)
    {
        tempAngle += 360;
    }

    startAngle = tempAngle;
    
    if(startAngle > 360)
    {
        startAngle %= 360;
    }

    if(pnt.x > 0 && pnt.y > 0)
    {
        //Q1
        pointAngle = (int32_t) ((double) leAtan((double) pnt.y / pnt.x) * (double)(180 / 3.1416));
    }
    else if(pnt.x < 0 && pnt.y > 0)
    {
        //Q2
        pointAngle = (int32_t) ((double) leAtan((double) pnt.y / pnt.x) * (double)(180 / 3.1416));
        pointAngle = 180 + pointAngle;
    }
    else if(pnt.x > 0 && pnt.y < 0)
    {
        //Q4
        pointAngle = (int32_t) ((double) leAtan((double) pnt.y / pnt.x) * (double)(180 / 3.1416));
        pointAngle = 360 + pointAngle;
    }
    else if(pnt.x < 0 && pnt.y < 0)
    {
        //Q3
        pointAngle = (int32_t) ((double) leAtan((double) pnt.y / pnt.x) * (double)(180 / 3.1416));
        pointAngle = 180 + pointAngle;
    }
    else if(pnt.x == 0 && pnt.y >= 0)
    {
        // +y
        pointAngle = 90;
    }
    else if(pnt.x == 0 && pnt.y < 0)
    {
        // -y
        pointAngle = 270;
    }
    else if(pnt.y == 0 && pnt.x > 0)
    {
        // +x
        pointAngle = 0;
    }
    else
    {
        // -x
        pointAngle = 180;
    }

    //Get the total
    for (i = 0; i < _this->pieArray.size; i++) 
    {
        pie = leArray_Get(&_this->pieArray, i);
        
        totalValue += pie->value;
    }

    //Test the angle if it's in a pie
    for (i = 0; i < _this->pieArray.size; i++) 
    {
        pie = leArray_Get(&_this->pieArray, i);

        if(_this->centerAngle < 0)
        {
            centerAngle = - ((int32_t) (((float) (-_this->centerAngle) * (float) pie->value)/(float) totalValue + 0.5));
            tempAngle = startAngle + centerAngle;

            while (tempAngle < 0)
            {
                tempAngle += 360;
            }

            endAngle = tempAngle;

            if(endAngle > 360)
            {
                endAngle %= 360;
            }
            
            if(startAngle >= endAngle)
            {
                if(pointAngle <= startAngle && pointAngle >= endAngle)
                {
                    if(((uint32_t) ((pnt.y * pnt.y) + (pnt.x * pnt.x)) <= 
                        (pie->radius + pie->offset) * (pie->radius + pie->offset)))
                    {
                        return i;
                    }
                }
            }
            //Pie overlaps the 0-deg axis
            else
            {
                if(pointAngle <= startAngle || pointAngle >= endAngle)
                {
                    if(((uint32_t) ((pnt.y * pnt.y) + (pnt.x * pnt.x)) <= 
                        (pie->radius + pie->offset) * (pie->radius + pie->offset)))
                    {
                        return i;
                    }
                }
            }
        }
        else
        {
            centerAngle = (int32_t) (((float) (_this->centerAngle) * (float) pie->value)/(float) totalValue + 0.5);
            endAngle = startAngle + centerAngle;

            if(endAngle > 360)
                endAngle %= 360;

            if(startAngle <= endAngle)
            {
                if(pointAngle <= endAngle && 
                    pointAngle >= startAngle)
                {
                    if(((uint32_t) ((pnt.y * pnt.y) + (pnt.x * pnt.x)) <= 
                        (pie->radius + pie->offset) * (pie->radius + pie->offset)))
                    {
                        return i;
                    }
                }
            }
            //Pie overlaps the 0-deg axis
            else
            {
                if(pointAngle <= endAngle || pointAngle >= startAngle)
                {
                    if(((uint32_t) ((pnt.y * pnt.y) + (pnt.x * pnt.x)) <= 
                        (pie->radius + pie->offset) * (pie->radius + pie->offset)))
                    {
                        return i;
                    }
                }
            }
        }

        startAngle = endAngle;
    } 

    return -1;
    
}

static void handleTouchDownEvent(lePieChartWidget* _this,
                                 leWidgetEvent_TouchDown* evt)
{
    lePoint pnt;
    int32_t index;

    pnt.x = evt->x;
    pnt.y = evt->y;

    //Adjust point relative to widget local
    leUtils_PointScreenToLocalSpace((leWidget*)_this, &pnt);

    //Adjust point relative widget center/origin
    pnt.x -= _this->widget.rect.width/2 ; 
    pnt.y = _this->widget.rect.height/2 - pnt.y;

    index = _getPieIndexAtPoint(_this, pnt);
    
    if(index != -1)
    {
        leWidgetEvent_Accept((leWidgetEvent*)evt, (leWidget*)_this);
        
        if(_this->pressedCallback != NULL)  
        {
            _this->pressedCallback(_this, index);
        }    
    }
}

void _lePieChartWidget_Paint(lePieChartWidget* _this);

#if LE_DYNAMIC_VTABLES == 1
void _leWidget_FillVTable(leWidgetVTable* tbl);

void _lePieChartWidget_GenerateVTable()
{
    _leWidget_FillVTable((void*)&pieChartWidgetVTable);
    
    /* overrides from base class */
    pieChartWidgetVTable._destructor = destructor;
    pieChartWidgetVTable._paint = _lePieChartWidget_Paint;
    pieChartWidgetVTable.touchDownEvent = handleTouchDownEvent;
    
    /* member functions */
    pieChartWidgetVTable.getStartAngle = getStartAngle;
    pieChartWidgetVTable.setStartAngle = setStartAngle;
    pieChartWidgetVTable.getCenterAngle = getCenterAngle;
    pieChartWidgetVTable.setCenterAngle = setCenterAngle;
    pieChartWidgetVTable.addEntry = addEntry;
    pieChartWidgetVTable.clear = clear;
    pieChartWidgetVTable.getEntryValue = getEntryValue;
    pieChartWidgetVTable.setEntryValue = setEntryValue;
    pieChartWidgetVTable.getEntryRadius = getEntryRadius;
    pieChartWidgetVTable.setEntryRadius = setEntryRadius;
    pieChartWidgetVTable.getEntryOffset = getEntryOffset;
    pieChartWidgetVTable.setEntryOffset = setEntryOffset;
    pieChartWidgetVTable.getEntryScheme = getEntryScheme;
    pieChartWidgetVTable.setEntryScheme = setEntryScheme;
    pieChartWidgetVTable.getPressedEventCallback = getPressedEventCallback;
    pieChartWidgetVTable.setPressedEventCallback = setPressedEventCallback;
    pieChartWidgetVTable.getLabelFont = getLabelFont;
    pieChartWidgetVTable.setLabelFont = setLabelFont;
    pieChartWidgetVTable.getLabelsVisible = getLabelsVisible;
    pieChartWidgetVTable.setLabelsVisible = setLabelsVisible;
    pieChartWidgetVTable.getLabelsOffset = getLabelsOffset;
    pieChartWidgetVTable.setLabelsOffset = setLabelsOffset;
}

void _lePieChartWidget_FillVTable(lePieChartWidgetVTable* tbl)
{
    *tbl = pieChartWidgetVTable;
}
#else
static const lePieChartWidgetVTable pieChartWidgetVTable =
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

    .update = (void*)_leWidget_Update,

    .touchUpEvent = (void*)_leWidget_TouchUpEvent,
    .touchMoveEvent = (void*)_leWidget_TouchMoveEvent,
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
    ._invalidateBorderAreas = (void*)_leWidget_InvalidateBorderAreas,
    ._damageArea = (void*)_leWidget_DamageArea,

    /* overrides from base class */
    ._destructor = destructor,
    ._paint = _lePieChartWidget_Paint,
    .touchDownEvent = handleTouchDownEvent,

    /* member functions */
    .getStartAngle = getStartAngle,
    .setStartAngle = setStartAngle,
    .getCenterAngle = getCenterAngle,
    .setCenterAngle = setCenterAngle,
    .addEntry = addEntry,
    .clear = clear,
    .getEntryValue = getEntryValue,
    .setEntryValue = setEntryValue,
    .getEntryRadius = getEntryRadius,
    .setEntryRadius = setEntryRadius,
    .getEntryOffset = getEntryOffset,
    .setEntryOffset = setEntryOffset,
    .getEntryScheme = getEntryScheme,
    .setEntryScheme = setEntryScheme,
    .getPressedEventCallback = getPressedEventCallback,
    .setPressedEventCallback = setPressedEventCallback,
    .getLabelFont = getLabelFont,
    .setLabelFont = setLabelFont,
    .getLabelsVisible = getLabelsVisible,
    .setLabelsVisible = setLabelsVisible,
    .getLabelsOffset = getLabelsOffset,
    .setLabelsOffset = setLabelsOffset,
};
#endif

#endif // LE_PIECHART_WIDGET_ENABLED