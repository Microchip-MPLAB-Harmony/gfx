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


#include "gfx/legato/widget/legato_editwidget.h"

#include "gfx/legato/core/legato_state.h"
#include "gfx/legato/string/legato_string.h"

//static leEditWidget* edit; // the widget that is currently receiving edit events

static
#if LE_DYNAMIC_VTABLES == 0
const
#endif
leEditWidgetVTable editWidgetVTable;

/* base class constructor and destructor */
void _leWidget_Destructor(leWidget* _this);

void leEditWidget_Constructor(leEditWidget* _this)
{
    /* call base class constructor */
    leWidget_Constructor((leWidget*)_this);
    
    _this->widget.fn = (void*)&editWidgetVTable;
    _this->fn = &editWidgetVTable;
}

void _leEditWidget_Destructor(leEditWidget* _this)
{
    /* call base class destructor */
    _leWidget_Destructor((leWidget*)_this);
}

leResult _leEditWidget_EditStart(leEditWidget* _this)
{
    (void)_this; // unused

    return LE_FAILURE;
}

void _leEditWidget_EditEnd(leEditWidget* _this)
{
    (void)_this; // unused
}

void _leEditWidget_EditClear(leEditWidget* _this)
{
    (void)_this; // unused
}

void _leEditWidget_EditAccept(leEditWidget* _this)
{
    (void)_this; // unused
}

void _leEditWidget_EditSet(leEditWidget* _this,
                           const leString* str)
{
    (void)_this; // unused
    (void)str; // unused
}

void _leEditWidget_EditAppend(leEditWidget* _this,
                              const leString* str)
{
    (void)_this; // unused
    (void)str; // unused
}

void _leEditWidget_EditBackspace(leEditWidget* _this)
{
    (void)_this; // unused
}

void _leEditWidget_Clear()
{ 
    leEditWidget* edit = leGetEditWidget();
    
    if(edit == NULL)
        return;
        
    edit->fn->editClear(edit);
}

void _leEditWidget_Accept()
{
    leEditWidget* edit = leGetEditWidget();
    
    if(edit == NULL)
        return;
        
    edit->fn->editAccept(edit);
}

void _leEditWidget_Set(const leString* str)
{
    leEditWidget* edit = leGetEditWidget();
    
    if(edit == NULL)
        return;
        
    edit->fn->editSet(edit, str);
}

void _leEditWidget_Append(const leString* str)
{
    leEditWidget* edit = leGetEditWidget();
    
    if(edit == NULL)
        return;
        
    edit->fn->editAppend(edit, str);
}

void _leEditWidget_Backspace()
{
    leEditWidget* edit = leGetEditWidget();
    
    if(edit == NULL)
        return;
        
    edit->fn->editBackspace(edit);
}

#if LE_DYNAMIC_VTABLES == 1
void _leWidget_FillVTable(leWidgetVTable* tbl);

void _leEditWidget_GenerateVTable(void)
{
    _leWidget_FillVTable((void*)&editWidgetVTable);

    editWidgetVTable._destructor = _leEditWidget_Destructor;
    editWidgetVTable.editStart = _leEditWidget_EditStart;
    editWidgetVTable.editEnd = _leEditWidget_EditEnd;
    editWidgetVTable.editClear = _leEditWidget_EditClear;
    editWidgetVTable.editAccept = _leEditWidget_EditAccept;
    editWidgetVTable.editSet = _leEditWidget_EditSet;
    editWidgetVTable.editAppend = _leEditWidget_EditAppend;
    editWidgetVTable.editBackspace = _leEditWidget_EditBackspace;
}

void _leEditWidget_FillVTable(leEditWidgetVTable* tbl)
{
    *tbl = editWidgetVTable;
}
#else
static const leEditWidgetVTable editWidgetVTable =
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

    // edit widget members
    ._destructor = _leEditWidget_Destructor,
    .editStart = _leEditWidget_EditStart,
    .editEnd = _leEditWidget_EditEnd,
    .editClear = _leEditWidget_EditClear,
    .editAccept = _leEditWidget_EditAccept,
    .editSet = _leEditWidget_EditSet,
    .editAppend = _leEditWidget_EditAppend,
    .editBackspace = _leEditWidget_EditBackspace,
};
#endif