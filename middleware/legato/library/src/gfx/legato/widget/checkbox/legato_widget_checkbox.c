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
#include "gfx/legato/widget/checkbox/legato_widget_checkbox.h"

#if LE_CHECKBOX_WIDGET_ENABLED == 1

#include "gfx/legato/common/legato_error.h"
#include "gfx/legato/common/legato_utils.h"
#include "gfx/legato/memory/legato_memory.h"
#include "gfx/legato/renderer/legato_renderer.h"
#include "gfx/legato/string/legato_string.h"
#include "gfx/legato/widget/legato_widget.h"

#if LE_DEBUG == 1
#include "gfx/legato/core/legato_debug.h"
#endif

#define DEFAULT_WIDTH    100
#define DEFAULT_HEIGHT   23

#define DEFAULT_IMAGE_MARGIN    10

static
#if LE_DYNAMIC_VTABLES == 0
const
#endif
leCheckBoxWidgetVTable checkBoxWidgetVTable;

void _leCheckBoxWidget_GetImageRect(const leCheckBoxWidget* cbox,
									leRect* imgRect,
									leRect* imgSrcRect);
									
void _leCheckBoxWidget_GetTextRects(const leCheckBoxWidget* cbox,
								    leRect* boundingRect,
								    leRect* kerningRect);

static void invalidateImageRect(const leCheckBoxWidget* _this)
{
    leRect imgRect, imgSrcRect;
    
    _leCheckBoxWidget_GetImageRect(_this, &imgRect, &imgSrcRect);
    
    _this->fn->_damageArea(_this, &imgRect);
}

static void invalidateTextRect(const leCheckBoxWidget* _this)
{
    leRect boundingRect, kerningRect;
    
    _leCheckBoxWidget_GetTextRects(_this, &boundingRect, &kerningRect);
    
    _this->fn->_damageArea(_this, &boundingRect);
}

static void invalidateContents(const leCheckBoxWidget* _this)
{
    LE_ASSERT_THIS();

    invalidateImageRect(_this);
    invalidateTextRect(_this);
}

static void stringPreinvalidate(const leString* str,
                                leCheckBoxWidget* cbox)
{
    (void)str; // unused

    invalidateContents(cbox);
}

static void stringInvalidate(const leString* str,
                             leCheckBoxWidget* cbox)
{
    (void)str; // unused

    invalidateContents(cbox);
}

static void handleLanguageChangeEvent(leCheckBoxWidget* _this)
{   
    LE_ASSERT_THIS();
    
    if(_this->string != NULL)
    {
        _this->fn->invalidate(_this);
    }
}

void _leCheckBoxWidget_Constructor(leCheckBoxWidget* _this)
{
    LE_ASSERT_THIS();
    
    /* call base class constructor */
    leWidget_Constructor((leWidget*)_this);

    _this->widget.fn = (void*)&checkBoxWidgetVTable;
    _this->fn = &checkBoxWidgetVTable;

    _this->widget.type = LE_WIDGET_CHECKBOX;

    _this->widget.rect.width = DEFAULT_WIDTH;
    _this->widget.rect.height = DEFAULT_HEIGHT;

    _this->checked = LE_FALSE;

    _this->widget.style.borderType = LE_WIDGET_BORDER_NONE;

    _this->string = NULL;

    _this->imagePosition = LE_RELATIVE_POSITION_LEFTOF;
    _this->imageMargin = DEFAULT_IMAGE_MARGIN;

    _this->widget.style.halign = LE_HALIGN_LEFT;
    _this->widget.style.valign = LE_VALIGN_MIDDLE;
    
    _this->checkedImage = NULL;
    _this->uncheckedImage = NULL;

    _this->checkedEvent = NULL;
    _this->uncheckedEvent = NULL;
}

void _leWidget_Destructor(leWidget* _this);

void _leCheckBoxWidget_Destructor(leCheckBoxWidget* _this)
{
    LE_ASSERT_THIS();
    
    _this->string = NULL;
    
    /* call base class destructor */
    _leWidget_Destructor((leWidget*)_this);
}

leCheckBoxWidget* leCheckBoxWidget_New()
{
    leCheckBoxWidget* wgt = NULL;

    wgt = LE_MALLOC(sizeof(leCheckBoxWidget));

    if(wgt == NULL)
        return NULL;
    
    _leCheckBoxWidget_Constructor(wgt);

    return wgt;
}

static leBool getChecked(const leCheckBoxWidget* _this)
{
    LE_ASSERT_THIS();
    
    return _this->checked;
}

static leResult setChecked(leCheckBoxWidget* _this,
                           leBool checked)
{
    LE_ASSERT_THIS();
        
    if(_this->checked == checked)
        return LE_SUCCESS;
        
    _this->checked = checked;
    
    if (_this->checked == LE_TRUE && _this->checkedEvent != NULL)
    {
        _this->checkedEvent(_this);
    }
    else if (_this->checked == LE_FALSE && _this->uncheckedEvent != NULL)
    {
        _this->uncheckedEvent(_this);
    }

    _this->fn->invalidate(_this);

#if LE_DEBUG == 1
    _leDebugNotify_WidgetPropertyChanged((leWidget*)_this);
#endif

    return LE_SUCCESS;
}

static leString* getString(const leCheckBoxWidget* _this)
{
    LE_ASSERT_THIS();
    
    return (leString*)_this->string;
}

static leResult setString(leCheckBoxWidget* _this,
                          const leString* str)
{
    LE_ASSERT_THIS();

    if(_this->string != NULL)
    {
        invalidateTextRect(_this);

        _this->string->fn->setPreInvalidateCallback((leString*)_this->string,
                                                    NULL,
                                                    NULL);

        _this->string->fn->setInvalidateCallback((leString*)_this->string,
                                                 NULL,
                                                 NULL);
    }

    _this->string = str;

    if(_this->string != NULL)
    {
        _this->string->fn->setPreInvalidateCallback((leString*) _this->string,
                                                    (leString_InvalidateCallback) stringPreinvalidate,
                                                    _this);

        _this->string->fn->setInvalidateCallback((leString*) _this->string,
                                                 (leString_InvalidateCallback) stringInvalidate,
                                                 _this);
    }

    invalidateTextRect(_this);

#if LE_DEBUG == 1
    _leDebugNotify_WidgetPropertyChanged((leWidget*)_this);
#endif

    return LE_SUCCESS;
}

static leImage* getCheckedImage(const leCheckBoxWidget* _this)
{
    LE_ASSERT_THIS();

    return (leImage*)_this->checkedImage;
}

static leResult setCheckedImage(leCheckBoxWidget* _this,
                                leImage* img)
{
    LE_ASSERT_THIS();

    _this->checkedImage = img;

    _this->fn->invalidate(_this);

#if LE_DEBUG == 1
    _leDebugNotify_WidgetPropertyChanged((leWidget*)_this);
#endif

    return LE_SUCCESS;
}

static leImage* getUncheckedImage(const leCheckBoxWidget* _this)
{
    LE_ASSERT_THIS();

    return (leImage*)_this->uncheckedImage;
}

static leResult setUncheckedImage(leCheckBoxWidget* _this,
                                  leImage* img)
{
    LE_ASSERT_THIS();

    _this->uncheckedImage = img;

    _this->fn->invalidate(_this);

#if LE_DEBUG == 1
    _leDebugNotify_WidgetPropertyChanged((leWidget*)_this);
#endif

    return LE_SUCCESS;
}

static leRelativePosition getImagePosition(const leCheckBoxWidget* _this)
{
    LE_ASSERT_THIS();
        
    return _this->imagePosition;
}

static leResult setImagePosition(leCheckBoxWidget* _this,
                                 leRelativePosition pos)
{
    LE_ASSERT_THIS();
    
    _this->imagePosition = pos;

    _this->fn->invalidate(_this);

#if LE_DEBUG == 1
    _leDebugNotify_WidgetPropertyChanged((leWidget*)_this);
#endif

    return LE_SUCCESS;
}

static uint32_t getImageMargin(const leCheckBoxWidget* _this)
{
    LE_ASSERT_THIS();
        
    return _this->imageMargin;
}

static leResult setImageMargin(leCheckBoxWidget* _this,
                               uint32_t mg)
{
    LE_ASSERT_THIS();
    
    _this->imageMargin = mg;

    _this->fn->invalidate(_this);

#if LE_DEBUG == 1
    _leDebugNotify_WidgetPropertyChanged((leWidget*)_this);
#endif

    return LE_SUCCESS;
}

static leCheckBoxWidget_CheckedEvent getCheckedEventCallback(const leCheckBoxWidget* _this)
{
    LE_ASSERT_THIS();

    return _this->checkedEvent;
}

static leResult setCheckedEventCallback(leCheckBoxWidget* _this,
                                        leCheckBoxWidget_CheckedEvent cb)
{
    LE_ASSERT_THIS();

    _this->checkedEvent = cb;
    
    return LE_SUCCESS;
}

static leCheckBoxWidget_UncheckedEvent getUncheckedEventCallback(const leCheckBoxWidget* _this)
{
    LE_ASSERT_THIS();

    return _this->uncheckedEvent;
}

static leResult setUncheckedEventCallback(leCheckBoxWidget* _this,
                                          leCheckBoxWidget_UncheckedEvent cb)
{
    LE_ASSERT_THIS();

    _this->uncheckedEvent = cb;
    
    return LE_SUCCESS;
}

static void handleTouchDownEvent(leCheckBoxWidget* _this,
                                 leWidgetEvent_TouchDown* evt)
{
    leRect imgRect, imgSrcRect;
    lePoint pt;
    
    LE_ASSERT_THIS();
    
    _leCheckBoxWidget_GetImageRect(_this, &imgRect, &imgSrcRect);
    
    pt.x = evt->x;
    pt.y = evt->y;
    
    if(leRectContainsPoint(&imgRect, &pt) == LE_TRUE)
    {
        leWidgetEvent_Accept((leWidgetEvent*)evt, (leWidget*)_this);

       // printf("cbox touch down\n");
    }
}

static void handleTouchUpEvent(leCheckBoxWidget* _this,
                               leWidgetEvent_TouchUp* evt)
{
    leRect imgRect, imgSrcRect;
    lePoint pnt;
    
    LE_ASSERT_THIS();

    pnt.x = evt->x;
    pnt.y = evt->y;

    leWidgetEvent_Accept((leWidgetEvent*)evt, (leWidget*)_this);

    _leCheckBoxWidget_GetImageRect(_this, &imgRect, &imgSrcRect);
    
    if(leRectContainsPoint(&imgRect, &pnt) == LE_TRUE)
    {
        _this->fn->setChecked(_this, !_this->checked);
    }
}

static void handleTouchMovedEvent(leCheckBoxWidget* _this,
                                  leWidgetEvent_TouchMove* evt)
{
    LE_ASSERT_THIS();

    leWidgetEvent_Accept((leWidgetEvent*)evt, (leWidget*)_this);
}

void _leCheckBoxWidget_Paint(leCheckBoxWidget* _this);

#if LE_DYNAMIC_VTABLES == 1
void _leWidget_FillVTable(leWidgetVTable* tbl);

void _leCheckBoxWidget_GenerateVTable()
{
    _leWidget_FillVTable((void*)&checkBoxWidgetVTable);
    
    /* overrides from base class */
    checkBoxWidgetVTable._destructor = _leCheckBoxWidget_Destructor;
    checkBoxWidgetVTable.languageChangeEvent = handleLanguageChangeEvent;
    checkBoxWidgetVTable._paint = _leCheckBoxWidget_Paint;
    checkBoxWidgetVTable.invalidateContents = invalidateContents;
    checkBoxWidgetVTable._invalidateBorderAreas = invalidateContents;
    checkBoxWidgetVTable.touchDownEvent = handleTouchDownEvent;
    checkBoxWidgetVTable.touchUpEvent = handleTouchUpEvent;
    checkBoxWidgetVTable.touchMoveEvent = handleTouchMovedEvent;
    
    /* member functions */
    checkBoxWidgetVTable.getChecked = getChecked;
    checkBoxWidgetVTable.setChecked = setChecked;
    checkBoxWidgetVTable.getCheckedImage = getCheckedImage;
    checkBoxWidgetVTable.setCheckedImage = setCheckedImage;
    checkBoxWidgetVTable.getUncheckedImage = getUncheckedImage;
    checkBoxWidgetVTable.setUncheckedImage = setUncheckedImage;
    checkBoxWidgetVTable.getImagePosition = getImagePosition;
    checkBoxWidgetVTable.setImagePosition = setImagePosition;
    checkBoxWidgetVTable.getImageMargin = getImageMargin;
    checkBoxWidgetVTable.setImageMargin = setImageMargin;
    checkBoxWidgetVTable.getString = getString;
    checkBoxWidgetVTable.setString = setString;
    checkBoxWidgetVTable.getCheckedEventCallback = getCheckedEventCallback;
    checkBoxWidgetVTable.setCheckedEventCallback = setCheckedEventCallback;
    checkBoxWidgetVTable.getUncheckedEventCallback = getUncheckedEventCallback;
    checkBoxWidgetVTable.setUncheckedEventCallback = setUncheckedEventCallback;
}

void _leCheckBoxWidget_FillVTable(leCheckBoxWidgetVTable* tbl)
{
    *tbl = checkBoxWidgetVTable;
}
#else
static const leCheckBoxWidgetVTable checkBoxWidgetVTable =
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

    .moveEvent = (void*)_leWidget_MoveEvent,
    .resizeEvent = (void*)_leWidget_ResizeEvent,
    .focusLostEvent = (void*)_leWidget_FocusLostEvent,
    .focusGainedEvent = (void*)_leWidget_FocusGainedEvent,

    ._handleEvent = (void*)_leWidget_HandleEvent,
    ._validateChildren = (void*)_leWidget_ValidateChildren,
    ._increaseDirtyState = (void*)_leWidget_IncreaseDirtyState,
    ._setDirtyState = (void*)_leWidget_SetDirtyState,
    ._clearDirtyState = (void*)_leWidget_ClearDirtyState,
    ._damageArea = (void*)_leWidget_DamageArea,

    /* overrides from base class */
    ._destructor = _leCheckBoxWidget_Destructor,
    .languageChangeEvent = handleLanguageChangeEvent,
    ._paint = _leCheckBoxWidget_Paint,
    .invalidateContents = invalidateContents,
    ._invalidateBorderAreas = invalidateContents,
    .touchDownEvent = handleTouchDownEvent,
    .touchUpEvent = handleTouchUpEvent,
    .touchMoveEvent = handleTouchMovedEvent,

    /* member functions */
    .getChecked = getChecked,
    .setChecked = setChecked,
    .getCheckedImage = getCheckedImage,
    .setCheckedImage = setCheckedImage,
    .getUncheckedImage = getUncheckedImage,
    .setUncheckedImage = setUncheckedImage,
    .getImagePosition = getImagePosition,
    .setImagePosition = setImagePosition,
    .getImageMargin = getImageMargin,
    .setImageMargin = setImageMargin,
    .getString = getString,
    .setString = setString,
    .getCheckedEventCallback = getCheckedEventCallback,
    .setCheckedEventCallback = setCheckedEventCallback,
    .getUncheckedEventCallback = getUncheckedEventCallback,
    .setUncheckedEventCallback = setUncheckedEventCallback,
};
#endif

#endif // LE_CHECKBOX_WIDGET_ENABLED
