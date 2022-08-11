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


#include "gfx/legato/widget/window/legato_widget_window.h"

#if LE_WINDOW_WIDGET_ENABLED

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

#define DEFAULT_TITLE_HEIGHT    20
#define DEFAULT_ICON_MARGIN     6

static
#if LE_DYNAMIC_VTABLES == 0
const
#endif
leWindowWidgetVTable windowWidgetVTable;

void _leWindowWidget_GetTitleBarRect(const leWindowWidget* win, leRect* barRect);

void _leWindowWidget_GetIconRect(const leWindowWidget* win,
                                 leRect* imgRect,
                                 leRect* imgSrcRect);

void _leWindowWidget_GetTextRect(const leWindowWidget* win,
                                 leRect* textRect,
                                 leRect* drawRect);

static void _invalidateTitleBar(const leWindowWidget* _this)
{
    leRect barRect;
    
    _leWindowWidget_GetTitleBarRect(_this, &barRect);
    
    _this->fn->_damageArea(_this, &barRect);
}

static void _invalidateTitleText(const leWindowWidget* _this)
{
    leRect textRect, drawRect;
    
    _leWindowWidget_GetTextRect(_this, &textRect, &drawRect);
                          
    _this->fn->_damageArea(_this, &drawRect);
}

static void stringPreinvalidate(const leString* str,
                                leWindowWidget* win)
{
    (void)str;
    (void)win;

    _invalidateTitleText(win);
}

static void stringInvalidate(const leString* str,
                             leWindowWidget* win)
{
    (void)str;

    _invalidateTitleText(win);
}

static void handleLanguageChangeEvent(leWindowWidget* _this)
{
    if(_this->title != NULL)
    {
        _this->fn->invalidate(_this);
    }
}

void leWindowWidget_Constructor(leWindowWidget* _this)
{
    leWidget_Constructor((leWidget*)_this);

    _this->widget.fn = (void*)&windowWidgetVTable;
    _this->fn = &windowWidgetVTable;
    
    _this->widget.type = LE_WIDGET_WINDOW;

    _this->widget.rect.width = DEFAULT_WIDTH;
    _this->widget.rect.height = DEFAULT_HEIGHT;

    _this->widget.style.borderType = LE_WIDGET_BORDER_BEVEL;

    _this->titleHeight = DEFAULT_TITLE_HEIGHT;
    _this->iconMargin = DEFAULT_ICON_MARGIN;
    _this->title = NULL;
    _this->icon = NULL;
}

void _leWidget_Destructor(leWidget* wgt);

static void destructor(leWindowWidget* _this)
{
    _this->title = NULL;

    _leWidget_Destructor((leWidget*)_this);
}

leWindowWidget* leWindowWidget_New()
{
    leWindowWidget* win = NULL;

    win = LE_MALLOC(sizeof(leWindowWidget));

    leWindowWidget_Constructor(win);

    return win;
}

static uint32_t getTitleHeight(const leWindowWidget* _this)
{
    LE_ASSERT_THIS();

    return _this->titleHeight;
}

static leResult setTitleHeight(leWindowWidget* _this,
                               uint32_t ht)
{
    LE_ASSERT_THIS();

    if(_this->titleHeight == ht)
        return LE_SUCCESS;

    _this->titleHeight = ht;

    _this->fn->invalidate(_this);

#if LE_DEBUG == 1
    _leDebugNotify_WidgetPropertyChanged((leWidget*)_this);
#endif

    return LE_SUCCESS;
}

static leString* getString(const leWindowWidget* _this)
{
    LE_ASSERT_THIS();
    
    return (leString*)_this->title;
}

static leResult setString(leWindowWidget* _this,
                         const leString* str)
{
    LE_ASSERT_THIS();

    if(_this->title != NULL)
    {
        _invalidateTitleBar(_this);

        _this->title->fn->setPreInvalidateCallback((leString*)_this->title,
                                                    NULL,
                                                    NULL);

        _this->title->fn->setInvalidateCallback((leString*)_this->title,
                                                 NULL,
                                                 NULL);
    }

    _this->title = str;

    if(_this->title != NULL)
    {
        _this->title->fn->setPreInvalidateCallback((leString*) _this->title,
                                                   (leString_InvalidateCallback) stringPreinvalidate,
                                                   _this);

        _this->title->fn->setInvalidateCallback((leString*) _this->title,
                                                (leString_InvalidateCallback) stringInvalidate,
                                                _this);

        _invalidateTitleBar(_this);
    }

#if LE_DEBUG == 1
    _leDebugNotify_WidgetPropertyChanged((leWidget*)_this);
#endif

    return LE_SUCCESS;
}

static leImage* getIcon(const leWindowWidget* _this)
{
    LE_ASSERT_THIS();
        
    return (leImage*)_this->icon;
}
                                                              
static leResult setIcon(leWindowWidget* _this,
                        const leImage* img)
{
    LE_ASSERT_THIS();
        
    if(_this->icon == img)
        return LE_SUCCESS;
        
    _this->icon = img;
    
    _invalidateTitleBar(_this);

#if LE_DEBUG == 1
    _leDebugNotify_WidgetPropertyChanged((leWidget*)_this);
#endif

    return LE_SUCCESS;
}

static uint32_t getIconMargin(const leWindowWidget* _this)
{
    LE_ASSERT_THIS();

    return _this->iconMargin;
}
                                               
static leResult setIconMargin(leWindowWidget* _this,
                              uint32_t mg)
{
    LE_ASSERT_THIS();
        
    if(_this->iconMargin == mg)
        return LE_SUCCESS;

    _this->iconMargin = mg;

    _invalidateTitleBar(_this);

#if LE_DEBUG == 1
    _leDebugNotify_WidgetPropertyChanged((leWidget*)_this);
#endif

    return LE_SUCCESS;
}

void _leWindowWidget_Paint(leWindowWidget* _this);

#if LE_DYNAMIC_VTABLES == 1
void _leWidget_FillVTable(leWidgetVTable* tbl);

void _leWindowWidget_GenerateVTable()
{
    _leWidget_FillVTable((void*)&windowWidgetVTable);
    
    /* overrides from base class */
    windowWidgetVTable._destructor = destructor;
    windowWidgetVTable._paint = _leWindowWidget_Paint;
    windowWidgetVTable.languageChangeEvent = handleLanguageChangeEvent;
    
    /* member functions */
    windowWidgetVTable.getTitleHeight = getTitleHeight;
    windowWidgetVTable.setTitleHeight = setTitleHeight;
    windowWidgetVTable.getString = getString;
    windowWidgetVTable.setString = setString;
    windowWidgetVTable.getIcon = getIcon;
    windowWidgetVTable.setIcon = setIcon;
    windowWidgetVTable.getIconMargin = getIconMargin;
    windowWidgetVTable.setIconMargin = setIconMargin;
}

void _leWindowWidget_FillVTable(leWindowWidgetVTable* tbl)
{
    *tbl = windowWidgetVTable;
}
#else
static const leWindowWidgetVTable windowWidgetVTable =
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

    ._handleEvent = (void*)_leWidget_HandleEvent,
    ._validateChildren = (void*)_leWidget_ValidateChildren,
    ._increaseDirtyState = (void*)_leWidget_IncreaseDirtyState,
    ._setDirtyState = (void*)_leWidget_SetDirtyState,
    ._clearDirtyState = (void*)_leWidget_ClearDirtyState,
    ._invalidateBorderAreas = (void*)_leWidget_InvalidateBorderAreas,
    ._damageArea = (void*)_leWidget_DamageArea,

    /* overrides from base class */
    ._destructor = destructor,
    ._paint = _leWindowWidget_Paint,
    .languageChangeEvent = handleLanguageChangeEvent,

    /* member functions */
    .getTitleHeight = getTitleHeight,
    .setTitleHeight = setTitleHeight,
    .getString = getString,
    .setString = setString,
    .getIcon = getIcon,
    .setIcon = setIcon,
    .getIconMargin = getIconMargin,
    .setIconMargin = setIconMargin,
};
#endif

#endif // LE_WINDOW_WIDGET_ENABLED