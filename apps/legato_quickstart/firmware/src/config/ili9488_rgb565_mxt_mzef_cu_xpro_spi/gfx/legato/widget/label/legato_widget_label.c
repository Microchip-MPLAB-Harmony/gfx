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


#include "gfx/legato/widget/label/legato_widget_label.h"

#if LE_LABEL_WIDGET_ENABLED

#include "gfx/legato/common/legato_error.h"
#include "gfx/legato/memory/legato_memory.h"
#include "gfx/legato/renderer/legato_renderer.h"
#include "gfx/legato/string/legato_string.h"
#include "gfx/legato/common/legato_utils.h"
#include "gfx/legato/widget/legato_widget.h"

#define DEFAULT_WIDTH           100
#define DEFAULT_HEIGHT          25

static
#if LE_DYNAMIC_VTABLES == 0
const
#endif
leLabelWidgetVTable labelWidgetVTable;

void _leLabelWidget_GetTextRect(const leLabelWidget* lbl,
                                leRect* textRect,
								leRect* drawRect);

static void invalidateContents(const leLabelWidget* lbl)
{
    leRect textRect, drawRect;
    
    _leLabelWidget_GetTextRect(lbl, &textRect, &drawRect);
    
    lbl->fn->_damageArea(lbl, &drawRect);
}

static void stringPreinvalidate(const leString* str,
                                leLabelWidget* lbl)
{
    (void)str; // unused

    invalidateContents(lbl);
}

static void stringInvalidate(const leString* str,
                             leLabelWidget* lbl)
{
    (void)str; // unused

    invalidateContents(lbl);
}

static void handleLanguageChangeEvent(leLabelWidget* _this)
{
    if(_this->string != NULL)
    {
        _this->fn->invalidate(_this);
    }
}

void leLabelWidget_Constructor(leLabelWidget* _this)
{
    LE_ASSERT_THIS();
        
    leWidget_Constructor((leWidget*)_this);

    _this->widget.fn = (void*)&labelWidgetVTable;
    _this->fn = &labelWidgetVTable;
    
    _this->widget.type = LE_WIDGET_LABEL;

    _this->widget.rect.width = DEFAULT_WIDTH;
    _this->widget.rect.height = DEFAULT_HEIGHT;

    _this->widget.style.borderType = LE_WIDGET_BORDER_NONE;
    _this->widget.style.halign = LE_HALIGN_LEFT;
     
    _this->string = NULL;
}

void _leWidget_Destructor(leWidget* _this);

static void _leLabelWidget_Destructor(leLabelWidget* _this)
{
    LE_ASSERT_THIS();
    
    _this->string = NULL;
    
    _leWidget_Destructor((leWidget*)_this);
}

/*void _leLabelWidget_Update(leLabelWidget* lbl)
{

}*/

leLabelWidget* leLabelWidget_New()
{
    leLabelWidget* lbl = NULL;

    lbl = LE_MALLOC(sizeof(leLabelWidget));

    leLabelWidget_Constructor(lbl);

    return lbl;
}

static leString* getString(const leLabelWidget* _this)
{
    LE_ASSERT_THIS();
    
    return (leString*)_this->string;
}

static leResult setString(leLabelWidget* _this,
                          leString* str)
{
    LE_ASSERT_THIS();

    if(_this->string == str)
        return LE_SUCCESS;

    if(_this->string != NULL)
    {
        invalidateContents(_this);

        _this->string->fn->setPreInvalidateCallback((leString*)_this->string,
                                                    NULL,
                                                    NULL);

        _this->string->fn->setInvalidateCallback((leString*)_this->string,
                                                 NULL,
                                                 NULL);
    }

    _this->string = str;

    _this->string->fn->setPreInvalidateCallback((leString*)_this->string,
                                                (leString_InvalidateCallback)stringPreinvalidate,
                                                _this);

    _this->string->fn->setInvalidateCallback((leString*)_this->string,
                                             (leString_InvalidateCallback)stringInvalidate,
                                             _this);

    invalidateContents(_this);
        
    return LE_SUCCESS;
}

void _leLabelWidget_Paint(leLabelWidget* _this);

#if LE_DYNAMIC_VTABLES == 1
void _leWidget_FillVTable(leWidgetVTable* tbl);

void _leLabelWidget_GenerateVTable()
{
    _leWidget_FillVTable((void*)&labelWidgetVTable);
    
    /* overrides from base class */
    labelWidgetVTable._destructor = _leLabelWidget_Destructor;
    labelWidgetVTable._paint = _leLabelWidget_Paint;
    labelWidgetVTable.languageChangeEvent = handleLanguageChangeEvent;
    
    /* member functions */
    labelWidgetVTable.getString = getString;
    labelWidgetVTable.setString = setString;
    labelWidgetVTable.invalidateContents = invalidateContents;
}

void _leLabelWidget_FillVTable(leLabelWidgetVTable* tbl)
{
    *tbl = labelWidgetVTable;
}
#else
static const leLabelWidgetVTable labelWidgetVTable =
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

    ._handleEvent = (void*)_leWidget_HandleEvent,
    ._validateChildren = (void*)_leWidget_ValidateChildren,
    ._increaseDirtyState = (void*)_leWidget_IncreaseDirtyState,
    ._setDirtyState = (void*)_leWidget_SetDirtyState,
    ._clearDirtyState = (void*)_leWidget_ClearDirtyState,
    ._invalidateBorderAreas = (void*)_leWidget_InvalidateBorderAreas,
    ._damageArea = (void*)_leWidget_DamageArea,

    /* overrides from base class */
    ._destructor = _leLabelWidget_Destructor,
    ._paint = _leLabelWidget_Paint,
    .languageChangeEvent = handleLanguageChangeEvent,
    .invalidateContents = invalidateContents,

    /* member functions */
    .getString = getString,
    .setString = setString,
};
#endif

#endif // LE_LABEL_WIDGET_ENABLED
