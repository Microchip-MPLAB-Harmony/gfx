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


#include "gfx/legato/widget/touchtest/legato_widget_touchtest.h"

#if LE_TOUCHTEST_WIDGET_ENABLED

#include "gfx/legato/common/legato_error.h"
#include "gfx/legato/common/legato_utils.h"
#include "gfx/legato/memory/legato_memory.h"
#include "gfx/legato/string/legato_string.h"
#include "gfx/legato/widget/legato_widget.h"

#if LE_DEBUG == 1
#include "gfx/legato/core/legato_debug.h"
#endif

#define DEFAULT_WIDTH           100
#define DEFAULT_HEIGHT          100

static
#if LE_DYNAMIC_VTABLES == 0
const
#endif
leTouchTestWidgetVTable touchTestWidgetVTable;

void _leTouchTestWidget_GetLineRects(const leTouchTestWidget* _this,
                                     uint32_t idx,
                                     leRect* horzRect,
                                     leRect* vertRect);

void leTouchTestWidget_Constructor(leTouchTestWidget* _this)
{
    leWidget_Constructor((leWidget*)_this);
    
    _this->widget.fn = (void*)&touchTestWidgetVTable;
    _this->fn = &touchTestWidgetVTable;
    
    _this->widget.type = LE_WIDGET_TOUCHTEST;
    _this->state = LE_TOUCHTEST_STATE_UP;

    // override base class methods
    _this->widget.rect.width = DEFAULT_WIDTH;
    _this->widget.rect.height = DEFAULT_HEIGHT;

    _this->widget.style.borderType = LE_WIDGET_BORDER_NONE;
    _this->widget.style.backgroundType = LE_WIDGET_BACKGROUND_FILL;
    
    _this->size = 0;
    _this->start = 0;
    _this->next = 0;

    _this->cb = NULL;
}

void _leWidget_Destructor(leWidget* wgt);

static void destructor(leTouchTestWidget* _this)
{
    _leWidget_Destructor((leWidget*)_this);
}

/*void _leTouchTestWidget_Update(leTouchTestWidget* _this)
{

}*/

leTouchTestWidget* leTouchTestWidget_New()
{
    leTouchTestWidget* tch = NULL;

    tch = LE_MALLOC(sizeof(leTouchTestWidget));

    if(tch == NULL)
        return NULL;
    
    leTouchTestWidget_Constructor(tch);

    return tch;
}

static leResult addPoint(leTouchTestWidget* _this,
                         lePoint* pnt)
{
    LE_ASSERT_THIS();
        
    if(_this->size > 0 && _this->next == _this->start)
    {
        _this->start++;
    }
       
    if(_this->size < LE_TOUCHTEST_MEMORY_SIZE)
    {
        _this->size++;
    }
        
    _this->pnts[_this->next] = *pnt;
    
    //printf("adding point %i: %i, %i\n", _this->next, pnt->x, pnt->y);
        
    _this->next += 1;
    
    if(_this->next >= LE_TOUCHTEST_MEMORY_SIZE)
    {
        _this->next = 0;
    }
        
    if(_this->cb != NULL)
    {
        _this->cb(_this, pnt);
    }

    _this->fn->invalidate(_this);

#if LE_DEBUG == 1
    _leDebugNotify_WidgetPropertyChanged((leWidget*)_this);
#endif

    return LE_SUCCESS;
}

static leResult clearPoints(leTouchTestWidget* _this)
{
    LE_ASSERT_THIS();

    _this->start = 0;
    _this->size = 0;
    _this->next = 0;

    _this->fn->invalidate(_this);

#if LE_DEBUG == 1
    _leDebugNotify_WidgetPropertyChanged((leWidget*)_this);
#endif

    return LE_SUCCESS;
}

static leTouchTestWidget_PointAddedEventCallback getPointAddedEventCallback(const leTouchTestWidget* _this)
{
    LE_ASSERT_THIS();

    return _this->cb;
}

static leResult setPointAddedEventCallback(leTouchTestWidget* _this,
                                           leTouchTestWidget_PointAddedEventCallback cb)
{
    LE_ASSERT_THIS();

    _this->cb = cb;
    
    return LE_SUCCESS;
}

static void handleTouchDownEvent(leTouchTestWidget* _this,
                                 leWidgetEvent_TouchDown* evt)
{
    lePoint pnt;
    
    LE_ASSERT_THIS();
    
    pnt.x = evt->x;
    pnt.y = evt->y;

    leWidgetEvent_Accept((leWidgetEvent*)evt, (leWidget*)_this);

    _this->state = LE_TOUCHTEST_STATE_DOWN;
    
    leUtils_PointScreenToLocalSpace((leWidget*)_this, &pnt);
    
    addPoint(_this, &pnt);

    //printf("_this touch down\n");
}

static void handleTouchUpEvent(leTouchTestWidget* _this,
                               leWidgetEvent_TouchUp* evt)
{
    LE_ASSERT_THIS();

    leWidgetEvent_Accept((leWidgetEvent*)evt, (leWidget*)_this);
    _this->state = LE_TOUCHTEST_STATE_UP;

    //printf("_this touch up\n");
    
    _this->fn->clearPoints(_this);
}

static void handleTouchMovedEvent(leTouchTestWidget* _this,
                                  leWidgetEvent_TouchMove* evt)
{
    leRect rect;
    lePoint pnt;
    
    LE_ASSERT_THIS();
    
    _this->fn->rectToScreen(_this, &rect);

    pnt.x = evt->x;
    pnt.y = evt->y;

    leWidgetEvent_Accept((leWidgetEvent*)evt, (leWidget*)_this);

    if(leRectContainsPoint(&rect, &pnt) == LE_TRUE)
    {
        leUtils_PointScreenToLocalSpace((leWidget*)_this, &pnt);
        
        _this->fn->addPoint(_this, &pnt);
    }
}

void _leTouchTestWidget_Paint(leTouchTestWidget* _this);

#if LE_DYNAMIC_VTABLES == 1
void _leWidget_FillVTable(leWidgetVTable* tbl);

void _leTouchTestWidget_GenerateVTable()
{
    _leWidget_FillVTable((void*)&touchTestWidgetVTable);
    
    /* overrides from base class */
    touchTestWidgetVTable._destructor = destructor;
    touchTestWidgetVTable._paint = _leTouchTestWidget_Paint;
    touchTestWidgetVTable.touchDownEvent = handleTouchDownEvent;
    touchTestWidgetVTable.touchUpEvent = handleTouchUpEvent;
    touchTestWidgetVTable.touchMoveEvent = handleTouchMovedEvent;
    
    /* member functions */
    touchTestWidgetVTable.addPoint = addPoint;
    touchTestWidgetVTable.clearPoints = clearPoints;
    touchTestWidgetVTable.getPointAddedEventCallback = getPointAddedEventCallback;
    touchTestWidgetVTable.setPointAddedEventCallback = setPointAddedEventCallback;
}

void _leTouchTestWidget_FillVTable(leTouchTestWidgetVTable* tbl)
{
    *tbl = touchTestWidgetVTable;
}
#else
static const leTouchTestWidgetVTable touchTestWidgetVTable =
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
    ._invalidateBorderAreas = (void*)_leWidget_InvalidateBorderAreas,
    ._damageArea = (void*)_leWidget_DamageArea,

    /* overrides from base class */
    ._destructor = destructor,
    ._paint = _leTouchTestWidget_Paint,
    .touchDownEvent = handleTouchDownEvent,
    .touchUpEvent = handleTouchUpEvent,
    .touchMoveEvent = handleTouchMovedEvent,

    /* member functions */
    .addPoint = addPoint,
    .clearPoints = clearPoints,
    .getPointAddedEventCallback = getPointAddedEventCallback,
    .setPointAddedEventCallback = setPointAddedEventCallback,
};
#endif

#endif // LE_TOUCHTEST_WIDGET_ENABLED