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
#include "gfx/legato/widget/circle/legato_widget_circle.h"

#if LE_CIRCLE_WIDGET_ENABLED == 1

#include "gfx/legato/common/legato_error.h"
#include "gfx/legato/common/legato_utils.h"
#include "gfx/legato/memory/legato_memory.h"
#include "gfx/legato/string/legato_string.h"
#include "gfx/legato/widget/legato_widget.h"

#define DEFAULT_WIDTH           100
#define DEFAULT_HEIGHT          100

#define DEFAULT_RADIUS          30
#define DEFAULT_THICKNESS       1

static
#if LE_DYNAMIC_VTABLES == 0
const
#endif
leCircleWidgetVTable circleWidgetVTable;

void _leCircleWidget_Constructor(leCircleWidget* _this)
{
    LE_ASSERT_THIS();
    
    leWidget_Constructor((leWidget*)_this);
    
    _this->widget.fn = (void*)&circleWidgetVTable;
    _this->fn = &circleWidgetVTable;

    _this->widget.type = LE_WIDGET_CIRCLE;
    
    _this->widget.rect.width = DEFAULT_WIDTH;
    _this->widget.rect.height = DEFAULT_HEIGHT;

    _this->widget.style.borderType = LE_WIDGET_BORDER_NONE;
    _this->widget.style.backgroundType = LE_WIDGET_BACKGROUND_NONE;

    _this->x = 0;
    _this->y = 0;
    _this->radius = DEFAULT_RADIUS;
    _this->thickness = DEFAULT_THICKNESS;
    _this->filled = LE_FALSE;
}

void _leWidget_Destructor(leWidget* wgt);

void _leCircleWidget_Destructor(leCircleWidget* _this)
{
    LE_ASSERT_THIS();
    
    _leWidget_Destructor((leWidget*)_this);
}

leCircleWidget* leCircleWidget_New()
{
    leCircleWidget* cir = NULL;

    cir = LE_MALLOC(sizeof(leCircleWidget));

    if(cir == NULL)
        return NULL;
    
    _leCircleWidget_Constructor(cir);

    return cir;
}

static leResult getOrigin(const leCircleWidget* _this,
                          lePoint* pnt)
{
    LE_ASSERT_THIS();

    if(pnt == NULL)
        return LE_FAILURE;
    
    pnt->x = _this->x;
    pnt->y = _this->y;

    return LE_SUCCESS;
}

static leResult setOrigin(leCircleWidget* _this,
                          lePoint org)
{
    LE_ASSERT_THIS();
        
    if(_this->x == org.x && _this->y == org.y)
        return LE_SUCCESS;
        
    _this->x = org.x;
    _this->y = org.y;
    
    _this->fn->invalidate(_this);
        
    return LE_SUCCESS;
}

static int32_t getOriginX(const leCircleWidget* _this)
{
    LE_ASSERT_THIS();
    
    return _this->x;
}

static leResult setOriginX(leCircleWidget* _this,
                           int32_t x)
{
    LE_ASSERT_THIS();
        
    if(_this->x == x)
        return LE_SUCCESS;
        
    _this->x = x;
    
    _this->fn->invalidate(_this);
        
    return LE_SUCCESS;
}

static int32_t getOriginY(const leCircleWidget* _this)
{
    LE_ASSERT_THIS();
    
    return _this->y;
}

static leResult setOriginY(leCircleWidget* _this,
                           int32_t y)
{
    LE_ASSERT_THIS();
        
    if(_this->y == y)
        return LE_SUCCESS;
        
    _this->y = y;
    
    _this->fn->invalidate(_this);
        
    return LE_SUCCESS;
}

static uint32_t getRadius(const leCircleWidget* _this)
{
    LE_ASSERT_THIS();
        
    return _this->radius;
}

static leResult setRadius(leCircleWidget* _this,
                          uint32_t rad)
{
    LE_ASSERT_THIS();
        
    if(_this->radius == rad)
        return LE_SUCCESS;
        
    _this->radius = rad;
    
    _this->fn->invalidate(_this);
        
    return LE_SUCCESS;
}

static uint32_t getThickness(const leCircleWidget* _this)
{
    LE_ASSERT_THIS();
        
    return _this->thickness;
}

static leResult setThickness(leCircleWidget* _this,
                             uint32_t thickness)
{
    LE_ASSERT_THIS();
    
    if(thickness < 1)
        return LE_FAILURE;
        
    if(_this->thickness == thickness)
        return LE_SUCCESS;
        
    _this->thickness = thickness;
    
    _this->fn->invalidate(_this);
        
    return LE_SUCCESS;
}

static leBool getFilled(const leCircleWidget* _this)
{
    LE_ASSERT_THIS();
        
    return _this->filled;
}

static leResult setFilled(leCircleWidget* _this,
                          leBool filled)
{
    LE_ASSERT_THIS();
        
    if(_this->filled == filled)
        return LE_SUCCESS;
        
    _this->filled = filled;
    
    _this->fn->invalidate(_this);
        
    return LE_SUCCESS;
}

void _leCircleWidget_Paint(leCircleWidget* _this);

#if LE_DYNAMIC_VTABLES == 1
void _leWidget_FillVTable(leWidgetVTable* tbl);

void _leCircleWidget_GenerateVTable()
{
    _leWidget_FillVTable((void*)&circleWidgetVTable);
    
    /* overrides from base class */
    circleWidgetVTable._destructor = _leCircleWidget_Destructor;
    circleWidgetVTable._paint = _leCircleWidget_Paint;
    
    /* member functions */
    circleWidgetVTable.getOrigin = getOrigin;
    circleWidgetVTable.setOrigin = setOrigin;
    circleWidgetVTable.getOriginX = getOriginX;
    circleWidgetVTable.setOriginX = setOriginX;
    circleWidgetVTable.getOriginY = getOriginY;
    circleWidgetVTable.setOriginY = setOriginY;
    circleWidgetVTable.getRadius = getRadius;
    circleWidgetVTable.setRadius = setRadius;
    circleWidgetVTable.getThickness = getThickness;
    circleWidgetVTable.setThickness = setThickness;
    circleWidgetVTable.getFilled = getFilled;
    circleWidgetVTable.setFilled = setFilled;
}

void _leCircleWidget_FillVTable(leCircleWidgetVTable* tbl)
{
    *tbl = circleWidgetVTable;
}
#else
static const leCircleWidgetVTable circleWidgetVTable =
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

    .touchDownEvent = (void*)_leWidget_TouchDownEvent,
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
    ._destructor = _leCircleWidget_Destructor,
    ._paint = _leCircleWidget_Paint,

    /* member functions */
    .getOrigin = getOrigin,
    .setOrigin = setOrigin,
    .getOriginX = getOriginX,
    .setOriginX = setOriginX,
    .getOriginY = getOriginY,
    .setOriginY = setOriginY,
    .getRadius = getRadius,
    .setRadius = setRadius,
    .getThickness = getThickness,
    .setThickness = setThickness,
    .getFilled = getFilled,
    .setFilled = setFilled,
};
#endif

#endif // LE_CIRCLE_WIDGET_ENABLED