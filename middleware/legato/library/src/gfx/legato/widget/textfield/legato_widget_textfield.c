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
#include "gfx/legato/widget/textfield/legato_widget_textfield.h"

#if LE_TEXTFIELD_WIDGET_ENABLED == 1

#include "gfx/legato/common/legato_error.h"
#include "gfx/legato/common/legato_utils.h"
#include "gfx/legato/core/legato_state.h"
#include "gfx/legato/memory/legato_memory.h"
#include "gfx/legato/string/legato_string.h"
#include "gfx/legato/widget/legato_widget.h"

#define DEFAULT_WIDTH           100
#define DEFAULT_HEIGHT          25

#define DEFAULT_CURSOR_TIME     1000

static
#if LE_DYNAMIC_VTABLES == 0
const
#endif
leTextFieldWidgetVTable textFieldWidgetVTable;

void _leTextFieldWidget_GetTextRect(const leTextFieldWidget* _this,
                                    leRect* textRect,
                                    leRect* drawRect);
                                    
void _leTextFieldWidget_GetCursorRect(const leTextFieldWidget* _this,
                                      leRect* cursorRect);

static void _invalidateText(const leTextFieldWidget* _this)
{
    leRect textRect, drawRect;
    
    _leTextFieldWidget_GetTextRect(_this, &textRect, &drawRect);
    
    _this->fn->_damageArea(_this, &drawRect);
}

static void stringPreinvalidate(const leString* str,
                                leTextFieldWidget* txt)
{
    (void)str; // unused

    _invalidateText(txt);
}

static void stringInvalidate(const leString* str,
                             leTextFieldWidget* txt)
{
    (void)str; // unused

    _invalidateText(txt);
}

static void _invalidateCursor(const leTextFieldWidget* _this)
{
    leRect cursorRect;
    
    _leTextFieldWidget_GetCursorRect(_this, &cursorRect);
    
    _this->fn->_damageArea(_this, &cursorRect);
}

static void _setCursorFromPoint(leTextFieldWidget* _this,
                                const lePoint* pnt)
{
    leRect widgetRect;
    leRect textRect, charRect;
    uint32_t charIdx;
    lePoint lclPnt = *pnt;

    _this->text.fn->getRect(&_this->text, &textRect);

    widgetRect = _this->editWidget.widget.rect;
    widgetRect.x = 0;
    widgetRect.y = 0;

    leUtils_ArrangeRectangleRelative(&textRect,
                                     leRect_Zero,
                                     widgetRect,
                                     _this->editWidget.widget.style.halign,
                                     LE_VALIGN_MIDDLE,
                                     0,
                                     _this->editWidget.widget.margin.left,
                                     _this->editWidget.widget.margin.top,
                                     _this->editWidget.widget.margin.right,
                                     _this->editWidget.widget.margin.bottom,
                                     0);

    // move text rect into screen space
    leUtils_RectToScreenSpace((leWidget *) _this, &textRect);

    lclPnt.x = pnt->x - textRect.x;

    if(lclPnt.x >= textRect.width)
    {
        _this->fn->setCursorPosition(_this, _this->text.fn->length(&_this->text));
    }
    else if(lclPnt.x < 0)
    {
        _this->fn->setCursorPosition(_this, 0);
    }
    else
    {
        // ignore y coordinate
        lclPnt.y = 0;

        _this->text.fn->getCharIndexAtPoint(&_this->text,
                                            &lclPnt,
                                            &charIdx);

        // round the point
        _this->text.fn->getCharRect(&_this->text,
                                    charIdx,
                                    &charRect);

        lclPnt.x -= charRect.x;

        if(lclPnt.x >= charRect.width >> 1)
        {
            charIdx += 1;
        }

        _this->fn->setCursorPosition(_this, charIdx);
    }
}

void leEditWidget_Constructor(leEditWidget* editWgt);

void leTextFieldWidget_Constructor(leTextFieldWidget* _this)
{
    LE_ASSERT_THIS();
    
    leEditWidget_Constructor((leEditWidget*)_this);

    _this->editWidget.widget.fn = (void*)&textFieldWidgetVTable;
    _this->editWidget.fn = (void*)&textFieldWidgetVTable;
    _this->fn = &textFieldWidgetVTable;
    
    _this->editWidget.widget.type = LE_WIDGET_TEXTFIELD;

    _this->editWidget.widget.rect.width = DEFAULT_WIDTH;
    _this->editWidget.widget.rect.height = DEFAULT_HEIGHT;
    
    _this->editWidget.widget.style.borderType = LE_WIDGET_BORDER_BEVEL;

    leDynamicString_Constructor(&_this->text);
    _this->hintText = NULL;
    
    _this->editWidget.widget.style.halign = LE_HALIGN_LEFT;
    _this->cursorPos = 0;
    _this->cursorEnable = LE_TRUE;
    _this->cursorDelay = DEFAULT_CURSOR_TIME;
    _this->clearOnFirstEdit = LE_TRUE;

    _this->textChangedEvent = NULL;
    _this->focusChangedEvent = NULL;
}

void _leEditWidget_Destructor(leEditWidget* edit);

static void destructor(leTextFieldWidget* _this)
{
    _this->text.fn->destructor(&_this->text);
    _this->hintText = NULL;
    
    _leEditWidget_Destructor((leEditWidget*)_this);
}

leTextFieldWidget* leTextFieldWidget_New()
{
    leTextFieldWidget* txt = NULL;

    txt = LE_MALLOC(sizeof(leTextFieldWidget));

    leTextFieldWidget_Constructor(txt);

    return txt;
}

static void update(leTextFieldWidget* _this,
                   uint32_t dt)
{   
    LE_ASSERT_THIS();
    
    if(leGetEditWidget() == (leEditWidget*)_this)
    {
        _this->cursorTime += dt;
        
        if(_this->cursorTime > _this->cursorDelay)
        {
            _this->cursorVisible = !_this->cursorVisible;
            
            //printf("%u\n", _this->cursorTime);
            
            _this->cursorTime = 0;
            
            _invalidateCursor(_this);
        }
    }
}

static uint32_t getCursorDelay(const leTextFieldWidget* _this)
{
    LE_ASSERT_THIS();

    return _this->cursorDelay;
}

static leResult setCursorDelay(leTextFieldWidget* _this,
                               uint32_t dt)
{
    LE_ASSERT_THIS();
    
    if(_this->cursorDelay == dt)
        return LE_FAILURE;

    _this->cursorDelay = dt;
        
    return LE_SUCCESS;
}

static leBool getCursorEnabled(const leTextFieldWidget* _this)
{
    LE_ASSERT_THIS();

    return _this->cursorEnable;
}

static leResult setCursorEnabled(leTextFieldWidget* _this,
                                 leBool en)
{
    LE_ASSERT_THIS();
    
    if(_this->cursorEnable == en)
        return LE_FAILURE;

    _this->cursorEnable = en;

    _this->fn->invalidate(_this);
        
    return LE_SUCCESS;
}

static uint32_t getCursorPosition(const leTextFieldWidget* _this)
{
    LE_ASSERT_THIS();

    return _this->cursorPos;
}

static leResult setCursorPosition(leTextFieldWidget* _this,
                                  uint32_t pos)
{
    LE_ASSERT_THIS();
    
    if(_this->cursorPos == pos)
        return LE_FAILURE;

    _this->cursorPos = pos;

    _this->cursorVisible = LE_TRUE;

    _this->fn->invalidate(_this);
        
    return LE_SUCCESS;
}

static const leString* getString(const leTextFieldWidget* _this)
{
    LE_ASSERT_THIS();
    
    return (const leString*)&_this->text;
}

static leResult setString(leTextFieldWidget* _this,
                          const leString* str)
{
    LE_ASSERT_THIS();
    
    _this->text.fn->setFromString(&_this->text, str);
    _this->cursorPos = _this->text.length;

    _this->fn->invalidate(_this);
    
    return LE_SUCCESS;
}

static leFont* getFont(const leTextFieldWidget* _this)
{
    LE_ASSERT_THIS();

    return _this->text.fn->getFont(&_this->text);
}

static leResult setFont(leTextFieldWidget* _this,
                        const leFont* fnt)
{
    LE_ASSERT_THIS();

    _this->text.fn->setFont(&_this->text, fnt);

    _this->fn->invalidate(_this);

    return LE_SUCCESS;
}

static leString* getHintString(const leTextFieldWidget* _this)
{
    LE_ASSERT_THIS();
    
    return (leString*)_this->hintText;
}

static leResult setHintString(leTextFieldWidget* _this,
                              const leString* str)
{
    LE_ASSERT_THIS();

    if(_this->hintText != NULL)
    {
        _invalidateText(_this);

        _this->hintText->fn->setPreInvalidateCallback((leString*)_this->hintText,
                                                      NULL,
                                                      NULL);

        _this->hintText->fn->setInvalidateCallback((leString*)_this->hintText,
                                                   NULL,
                                                   NULL);
    }

    _this->hintText = str;

    _this->hintText->fn->setPreInvalidateCallback((leString*)_this->hintText,
                                                  (leString_InvalidateCallback)stringPreinvalidate,
                                                  _this);

    _this->hintText->fn->setInvalidateCallback((leString*)_this->hintText,
                                               (leString_InvalidateCallback)stringInvalidate,
                                               _this);

    _invalidateText(_this);
    
    return LE_SUCCESS;
}

static leResult setClearValueOnFirstEdit(leTextFieldWidget* _this,
                                         leBool clear)
{
    LE_ASSERT_THIS();
    
    if(_this->clearOnFirstEdit == clear)
        return LE_FAILURE;

    _this->clearOnFirstEdit = clear;

    return LE_SUCCESS;
}

static void handleFocusGainedEvent(leTextFieldWidget* _this)
{
    LE_ASSERT_THIS();
    
    leSetEditWidget((leEditWidget*)_this);
    
    if (_this->focusChangedEvent != NULL)
    {
        _this->focusChangedEvent(_this, LE_TRUE);
    }

    _this->fn->invalidate(_this);
}

static void handleFocusLostEvent(leTextFieldWidget* _this)
{
    LE_ASSERT_THIS();
    
    if (_this->focusChangedEvent != NULL)
    {
        _this->focusChangedEvent(_this, LE_FALSE);
    }

    _this->fn->invalidate(_this);
}

static leTextFieldWidget_TextChangedCallback getTextChangedEventCallback(const leTextFieldWidget* _this)
{
    LE_ASSERT_THIS();

    return _this->textChangedEvent;
}

static leResult setTextChangedEventCallback(leTextFieldWidget* _this,
                                            leTextFieldWidget_TextChangedCallback cb)
{   
    LE_ASSERT_THIS();
    
    if(_this->textChangedEvent == cb)
        return LE_FAILURE;

    _this->textChangedEvent = cb;

    return LE_SUCCESS;
}

static leTextFieldWidget_FocusChangedCallback getFocusChangedEventCallback(const leTextFieldWidget* _this)
{
    LE_ASSERT_THIS();

    return _this->focusChangedEvent;
}

static leResult setFocusChangedEventCallback(leTextFieldWidget* _this,
                                             leTextFieldWidget_FocusChangedCallback cb)
{
    LE_ASSERT_THIS();
    
    if(_this->focusChangedEvent == cb)
        return LE_FAILURE;

    _this->focusChangedEvent = cb;

    return LE_SUCCESS;
}

static leResult editStart(leTextFieldWidget* _this)
{
    LE_ASSERT_THIS();
    
    _this->cursorTime = 0;
    _this->cursorVisible = LE_TRUE;

    if (_this->clearOnFirstEdit == LE_TRUE)
    {
        _this->text.fn->clear(&_this->text);
        _this->clearOnFirstEdit  = LE_FALSE;
    }

    _this->fn->invalidate(_this);
    
    return LE_SUCCESS;
}

static void editEnd(leTextFieldWidget* _this)
{
    LE_ASSERT_THIS();
    
    _this->cursorTime = 0;
    _this->cursorVisible = LE_FALSE;

    _this->fn->invalidate(_this);
}

static void editClear(leTextFieldWidget* _this)
{
    LE_ASSERT_THIS();

    _this->fn->invalidate(_this);
    
    _this->text.fn->clear(&_this->text);

    if (_this->textChangedEvent != NULL)
    {
        _this->textChangedEvent(_this);
    }
}

static void editAccept(leTextFieldWidget* _this)
{   
    LE_ASSERT_THIS();

    (void)_this; // unused
    
    leSetEditWidget(NULL);
}

static void editAppend(leTextFieldWidget* _this,
                       const leString* str)
{
    LE_ASSERT_THIS();

    if(str == NULL)
        return;

    _this->text.fn->insert(&_this->text, str, _this->cursorPos);
    _this->cursorPos += str->fn->length(str);

    if(_this->cursorEnable == LE_TRUE)
    {
        _this->cursorTime = 0;
        _this->cursorVisible = LE_TRUE;
    }

    if (_this->textChangedEvent != NULL)
    {
        _this->textChangedEvent(_this);
    }

    _this->fn->invalidate(_this);
}

static void editSet(leTextFieldWidget* _this,
                    const leString* str)
{
    LE_ASSERT_THIS();
    
    _this->text.fn->setFromString(&_this->text, str);
    
    _this->cursorPos = _this->text.length;
    
    if(_this->cursorEnable == LE_TRUE)
    {
        _this->cursorTime = 0;
        _this->cursorVisible = LE_TRUE;
    }

    if (_this->textChangedEvent != NULL)
    {
        _this->textChangedEvent(_this);
    }

    _this->fn->invalidate(_this);
}

static void editBackspace(leTextFieldWidget* _this)
{
    LE_ASSERT_THIS();
    
    if(_this->cursorPos == 0)
        return;
        
    _this->text.fn->remove(&_this->text, _this->cursorPos - 1, 1);
        
    if(_this->cursorPos > _this->text.length)
    {
        _this->cursorPos = _this->text.length;
    }
    else
    {
        _this->cursorPos--;
    }
        
    if(_this->cursorEnable == LE_TRUE)
    {
        _this->cursorTime = 0;
        _this->cursorVisible = LE_TRUE;
    }

    if (_this->textChangedEvent != NULL)
    {
        _this->textChangedEvent(_this);
    }

    _this->fn->invalidate(_this);
}

static void handleTouchDownEvent(leTextFieldWidget* _this,
                                 leWidgetEvent_TouchDown* evt)
{
    lePoint pnt;
    
    LE_ASSERT_THIS();
    
    pnt.x = evt->x;
    pnt.y = evt->y;
    
    _setCursorFromPoint(_this, &pnt);

    leWidgetEvent_Accept((leWidgetEvent*)evt, (leWidget*)_this);
}

static void handleTouchUpEvent(leTextFieldWidget* _this,
                               leWidgetEvent_TouchUp* evt)
{
    LE_ASSERT_THIS();

    leWidgetEvent_Accept((leWidgetEvent*)evt, (leWidget*)_this);
}

static void handleTouchMovedEvent(leTextFieldWidget* _this,
                                  leWidgetEvent_TouchMove* evt)
{
    LE_ASSERT_THIS();
    
    /*lePoint pnt;
    
    pnt.x = evt->x;
    pnt.y = evt->y;
    
    _setCursorFromPoint(_this, &pnt);*/

    leWidgetEvent_Accept((leWidgetEvent*)evt, (leWidget*)_this);
}

void _leTextFieldWidget_Paint(leTextFieldWidget* _this);

static void handleLanguageChangeEvent(leTextFieldWidget* _this)
{
    LE_ASSERT_THIS();

    _this->fn->invalidate(_this);
}

#if LE_DYNAMIC_VTABLES == 1
void _leEditWidget_FillVTable(leEditWidgetVTable* tbl);

void _leTextFieldWidget_GenerateVTable()
{
    _leEditWidget_FillVTable((void*)&textFieldWidgetVTable);

    /* overrides from base class */
    textFieldWidgetVTable._destructor = destructor;
    textFieldWidgetVTable._paint = _leTextFieldWidget_Paint;
    textFieldWidgetVTable.update = update;
    textFieldWidgetVTable.touchDownEvent = handleTouchDownEvent;
    textFieldWidgetVTable.touchUpEvent = handleTouchUpEvent;
    textFieldWidgetVTable.touchMoveEvent = handleTouchMovedEvent;
    textFieldWidgetVTable.focusLostEvent = handleFocusLostEvent;
    textFieldWidgetVTable.focusGainedEvent = handleFocusGainedEvent;
    textFieldWidgetVTable.languageChangeEvent = handleLanguageChangeEvent;
    
    /* override from edit widget */
    textFieldWidgetVTable.editStart = editStart;
    textFieldWidgetVTable.editEnd = editEnd;
    textFieldWidgetVTable.editClear = editClear;
    textFieldWidgetVTable.editAccept = editAccept;
    textFieldWidgetVTable.editSet = editSet;
    textFieldWidgetVTable.editAppend = editAppend;
    textFieldWidgetVTable.editBackspace = editBackspace;
    
    /* member functions */
    textFieldWidgetVTable.getCursorDelay = getCursorDelay;
    textFieldWidgetVTable.setCursorDelay = setCursorDelay;
    textFieldWidgetVTable.getCursorEnabled = getCursorEnabled;
    textFieldWidgetVTable.setCursorEnabled = setCursorEnabled;
    textFieldWidgetVTable.getCursorPosition = getCursorPosition;
    textFieldWidgetVTable.setCursorPosition = setCursorPosition;
    textFieldWidgetVTable.getString = getString;
    textFieldWidgetVTable.setString = setString;
    textFieldWidgetVTable.getHintString = getHintString;
    textFieldWidgetVTable.setHintString = setHintString;
    textFieldWidgetVTable.getFont = getFont;
    textFieldWidgetVTable.setFont = setFont;
    textFieldWidgetVTable.setClearValueOnFirstEdit = setClearValueOnFirstEdit;
    textFieldWidgetVTable.getTextChangedEventCallback = getTextChangedEventCallback;
    textFieldWidgetVTable.setTextChangedEventCallback = setTextChangedEventCallback;
    textFieldWidgetVTable.getFocusChangedEventCallback = getFocusChangedEventCallback;
    textFieldWidgetVTable.setFocusChangedEventCallback = setFocusChangedEventCallback;
}

void _leTextFieldWidget_FillVTable(leTextFieldWidgetVTable* tbl)
{
    *tbl = textFieldWidgetVTable;
}
#else
static const leTextFieldWidgetVTable textFieldWidgetVTable =
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
    .resizeEvent = (void*)_leWidget_ResizeEvent,

    ._handleEvent = (void*)_leWidget_HandleEvent,
    ._validateChildren = (void*)_leWidget_ValidateChildren,
    ._increaseDirtyState = (void*)_leWidget_IncreaseDirtyState,
    ._setDirtyState = (void*)_leWidget_SetDirtyState,
    ._clearDirtyState = (void*)_leWidget_ClearDirtyState,
    ._invalidateBorderAreas = (void*)_leWidget_InvalidateBorderAreas,
    ._damageArea = (void*)_leWidget_DamageArea,

    /* overrides from base class */
    ._destructor = destructor,
    ._paint = _leTextFieldWidget_Paint,
    .update = update,
    .touchDownEvent = handleTouchDownEvent,
    .touchUpEvent = handleTouchUpEvent,
    .touchMoveEvent = handleTouchMovedEvent,
    .focusLostEvent = handleFocusLostEvent,
    .focusGainedEvent = handleFocusGainedEvent,
    .languageChangeEvent = handleLanguageChangeEvent,

    /* override from edit widget */
    .editStart = editStart,
    .editEnd = editEnd,
    .editClear = editClear,
    .editAccept = editAccept,
    .editSet = editSet,
    .editAppend = editAppend,
    .editBackspace = editBackspace,

    /* member functions */
    .getCursorDelay = getCursorDelay,
    .setCursorDelay = setCursorDelay,
    .getCursorEnabled = getCursorEnabled,
    .setCursorEnabled = setCursorEnabled,
    .getCursorPosition = getCursorPosition,
    .setCursorPosition = setCursorPosition,
    .getString = getString,
    .setString = setString,
    .getHintString = getHintString,
    .setHintString = setHintString,
    .getFont = getFont,
    .setFont = setFont,
    .setClearValueOnFirstEdit = setClearValueOnFirstEdit,
    .getTextChangedEventCallback = getTextChangedEventCallback,
    .setTextChangedEventCallback = setTextChangedEventCallback,
    .getFocusChangedEventCallback = getFocusChangedEventCallback,
    .setFocusChangedEventCallback = setFocusChangedEventCallback,
};
#endif

#endif // LE_TEXTFIELD_WIDGET_ENABLED
