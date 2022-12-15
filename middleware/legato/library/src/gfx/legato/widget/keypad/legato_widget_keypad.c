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
#include "gfx/legato/widget/keypad/legato_widget_keypad.h"

#if LE_KEYPAD_WIDGET_ENABLED && LE_BUTTON_WIDGET_ENABLED

#include "gfx/legato/common/legato_error.h"
#include "gfx/legato/common/legato_utils.h"
#include "gfx/legato/memory/legato_memory.h"
#include "gfx/legato/string/legato_string.h"
#include "gfx/legato/widget/button/legato_widget_button.h"

#if LE_DEBUG == 1
#include "gfx/legato/core/legato_debug.h"
#endif

#define DEFAULT_WIDTH           200
#define DEFAULT_HEIGHT          200

static
#if LE_DYNAMIC_VTABLES == 0
const
#endif

leKeyPadWidgetVTable keyPadWidgetVTable;

static void buttonClicked(leButtonWidget* btn);

static leResult _findButtonCell(leKeyPadWidget* _this,
                                leButtonWidget* btn,
                                uint32_t* row,
                                uint32_t* col)
{
    uint32_t r, c;

    for(r = 0; r < _this->rows; ++r)
    {
        for(c = 0; c < _this->cols; ++c)
        {
            if(_this->cells[r][c].button == btn)
            {
                *row = r;
                *col = c;

                return LE_SUCCESS;
            }
        }
    }

    return LE_FAILURE;
}

static void _reallocateCells(leKeyPadWidget* _this,
                             uint32_t rows,
                             uint32_t cols)
{
    leKeyPadCell** newCells;
    uint32_t r, c;

    // create new array
    newCells = LE_MALLOC(rows * sizeof(leKeyPadCell**));

    memset(newCells, 0, rows * sizeof(leKeyPadCell**));

    for(r = 0; r < rows; ++r)
    {
        newCells[r] = LE_MALLOC(cols * sizeof(leKeyPadCell));

        memset(newCells[r], 0, cols * sizeof(leKeyPadCell));
    }

    // copy old values or initialize new cells
    for(r = 0; r < rows; ++r)
    {
        for(c = 0; c < cols; ++c)
        {
            if(r >= _this->rows || c >= _this->cols)
            {
                newCells[r][c].button = leButtonWidget_New();

                LE_ASSERT(newCells[r][c].button != NULL);

                LE_PCALL(newCells[r][c].button, setPressedEventCallback, (leButtonWidget_PressedEvent)buttonClicked);
                LE_PCALL(newCells[r][c].button, setReleasedEventCallback, (leButtonWidget_PressedEvent)buttonClicked);

                _this->fn->addChild(_this, (leWidget*)newCells[r][c].button);
            }
            else
            {
                newCells[r][c] = _this->cells[r][c];
            }
        }
    }

    // delete old out-of-bound values
    for(r = 0; r < _this->rows; ++r)
    {
        for(c = 0; c < _this->cols; ++c)
        {
            if(r >= rows || c >= cols)
            {
                LE_PCALL(_this, removeChild, (leWidget*)_this->cells[r][c].button);

                leWidget_Delete((leWidget*)_this->cells[r][c].button);
            }
        }
    }

    // free old array
    if(_this->cells != NULL)
    {
        for(r = 0; r < _this->rows; ++r)
        {
            LE_FREE(_this->cells[r]);
        }

        LE_FREE(_this->cells);
    }

    // set new array
    _this->rows = rows;
    _this->cols = cols;
    _this->cells = newCells;
}

static void _resizeCells(leKeyPadWidget* _this)
{
    uint32_t width, height, wrem, hrem, row, col, wmod, hmod;
    int32_t x, y;
    leButtonWidget* btn;

    if(_this->widget.children.size == 0)
        return;
    
    x = 0;
    y = 0;
    width = _this->widget.rect.width / _this->cols;
    height = _this->widget.rect.height / _this->rows;
    
    wrem = _this->widget.rect.width - (width * _this->cols);
    hrem = _this->widget.rect.height - (height * _this->rows);
    
    hmod = 0;
    wmod = 0;
    
    // resize and position all button cells
    for(row = 0; row < _this->rows; row++)
    {
        hmod = row < hrem ? 1 : 0;
           
        for(col = 0; col < _this->cols; col++)
        {
            btn = (leButtonWidget*)_this->fn->getChildAtIndex(_this, (row * _this->cols) + col);
                          
            if(btn == NULL)
                return;
                                                            
            btn->fn->setPosition(btn, x, y);
            
            wmod = col < wrem ? 1 : 0;
            
            btn->fn->setSize(btn, width + wmod, height + hmod);
            
            x += width + wmod;
        }
        
        x = 0;
        
        y += height + hmod;
    }
}

static void _resizeCellEvent(leKeyPadWidget* _this,
                             leWidget_ResizeEvent* evt)
{
    (void)evt; // unused

    _resizeCells(_this);
}

static void buttonClicked(leButtonWidget* btn)
{
    uint32_t row = 0;
    uint32_t col = 0;
    leKeyPadWidget* _this = (leKeyPadWidget*)btn->widget.parent;
    
    LE_ASSERT_THIS();
    
    if ((btn->fn->getPressed(btn) == LE_TRUE && _this->trigger == LE_KEYPAD_TRIGGER_KEYRELEASED) ||
        (btn->fn->getPressed(btn) == LE_FALSE && _this->trigger == LE_KEYPAD_TRIGGER_KEYPRESSED))
        return;

    _findButtonCell(_this, btn, &row, &col);

    if(_this->clickEvt != NULL)
    {
        _this->clickEvt(_this, btn, row, col);
    }
    
    //printf("%i, %i\n", row, col);
    
    if(_this->cells[row][col].action == LE_KEYPAD_CELL_ACTION_APPEND)
    {
        _leEditWidget_Append(_this->cells[row][col].value);
    }
    else if(_this->cells[row][col].action == LE_KEYPAD_CELL_ACTION_SET)
    {
        _leEditWidget_Set(_this->cells[row][col].value);
    }
    else if(_this->cells[row][col].action == LE_KEYPAD_CELL_ACTION_BACKSPACE)
    {
        _leEditWidget_Backspace();
    }
    else if(_this->cells[row][col].action == LE_KEYPAD_CELL_ACTION_CLEAR)
    {
        _leEditWidget_Clear();
    }
    else if(_this->cells[row][col].action == LE_KEYPAD_CELL_ACTION_ACCEPT)
    {
        _leEditWidget_Accept();
    }
}

/*static void languageChanging(leKeyPadWidget* _this)
{
    uint32_t i;
    
    for(i = 0; i < _this->rows * _this->cols; i++)
    {
        if(_this->cells[i].button->text != NULL)
        {
            leWidget_Invalidate((leWidget*)_this);
            
            return;
        }
    }
}*/

void leKeyPadWidget_Constructor(leKeyPadWidget* _this,
                                uint32_t rows,
                                uint32_t cols)
{
    //leButtonWidget* button;
    //uint32_t i;
    
    leWidget_Constructor((leWidget*)_this);
    
    _this->widget.fn = (void*)&keyPadWidgetVTable;
    _this->fn = &keyPadWidgetVTable;
    
    _this->widget.type = LE_WIDGET_KEYPAD;

    _this->widget.rect.width = DEFAULT_WIDTH;
    _this->widget.rect.height = DEFAULT_HEIGHT;

    _this->widget.style.borderType = LE_WIDGET_BORDER_BEVEL;
    _this->widget.style.backgroundType = LE_WIDGET_BACKGROUND_NONE;
    
    _this->rows = 0;
    _this->cols = 0;
    _this->cells = 0;
    
    _this->trigger = LE_KEYPAD_TRIGGER_KEYRELEASED;

    _reallocateCells(_this, rows, cols);

#if 0
    _this->cells = LE_MALLOC(rows * cols * sizeof(leKeyPadCell));
    
    for(i = 0; i < rows * cols; i++)
    {
        // create button
        button = leButtonWidget_New();
        
        LE_ASSERT_VAR(button != NULL, "%s: Failed to allocate memory for button cell!\n", __FUNCTION__); 
        
        if(button == NULL)
            return;
            
        button->fn->setPressedEventCallback(button, (leButtonWidget_PressedEvent)buttonClicked);
        button->fn->setReleasedEventCallback(button, (leButtonWidget_ReleasedEvent)buttonClicked);        
        
        _this->fn->addChild(_this, (leWidget*)button);
    }
#endif

    _this->clickEvt = NULL;

    _resizeCells(_this);
}

void _leWidget_Destructor(leWidget* _this);

static void destructor(leKeyPadWidget* _this)
{
    LE_FREE(_this->cells);
    
    _leWidget_Destructor((leWidget*)_this);
}

leKeyPadWidget* leKeyPadWidget_New(uint32_t rows, uint32_t cols)
{
    leKeyPadWidget* pad = NULL;

    pad = LE_MALLOC(sizeof(leKeyPadWidget));
    
    leKeyPadWidget_Constructor(pad, rows, cols);

    return pad;
}

static leResult setCellArraySize(leKeyPadWidget* _this,
                                 uint32_t rows,
                                 uint32_t cols)
{
    LE_ASSERT_THIS();

    if(_this->rows == rows && _this->cols == cols)
        return LE_FAILURE;

    _reallocateCells(_this, rows, cols);

    _resizeCells(_this);

#if LE_DEBUG == 1
    _leDebugNotify_WidgetPropertyChanged((leWidget*)_this);
#endif

    return LE_SUCCESS;
}

static leKeyPadActionTrigger getKeyPadActionTrigger(const leKeyPadWidget* _this)
{
    LE_ASSERT_THIS();

    return _this->trigger;
}

static leResult setKeyPadActionTrigger(leKeyPadWidget* _this,
                                       leKeyPadActionTrigger trigger)
{
    LE_ASSERT_THIS();

    _this->trigger = trigger;

#if LE_DEBUG == 1
    _leDebugNotify_WidgetPropertyChanged((leWidget*)_this);
#endif

    return LE_SUCCESS;
}

static leKeyPadWidget_KeyClickEvent getKeyClickEventCallback(const leKeyPadWidget* _this)
{
    LE_ASSERT_THIS();

    return _this->clickEvt;
}

static leResult setKeyClickEventCallback(leKeyPadWidget* _this,
                                         leKeyPadWidget_KeyClickEvent cb)
{
    LE_ASSERT_THIS();
    
    _this->clickEvt = cb;
    
    return LE_SUCCESS;
}

static leResult setKeyVisible(leKeyPadWidget* _this,
                              uint32_t row,
                              uint32_t col,
                              leBool enabled)
{
    leButtonWidget* btn;
    
    LE_ASSERT_THIS();

    if(row >= _this->rows || col >= _this->cols)
        return LE_FAILURE;
        
    btn = _this->fn->getCellButton(_this, row, col);

#if LE_DEBUG == 1
    _leDebugNotify_WidgetPropertyChanged((leWidget*)_this);
#endif

    return btn->fn->setVisible(btn, enabled);
}

static leKeyPadCellAction getKeyAction(const leKeyPadWidget* _this,
                                       uint32_t row,
                                       uint32_t col)
{
    LE_ASSERT_THIS();
    
    if(row >= _this->rows || col >= _this->cols)
        return 0;
    
    return _this->cells[row][col].action;
}
                                               
static leResult setKeyAction(leKeyPadWidget* _this,
                             uint32_t row,
                             uint32_t col,
                             leKeyPadCellAction action)
{
    LE_ASSERT_THIS();
    
    if(row >= _this->rows || col >= _this->cols)
        return LE_FAILURE;
        
    _this->cells[row][col].action = action;

#if LE_DEBUG == 1
    _leDebugNotify_WidgetPropertyChanged((leWidget*)_this);
#endif

    return LE_SUCCESS;
}

static leString* getKeyValue(const leKeyPadWidget* _this,
                             uint32_t row,
                             uint32_t col)
{
    if(row >= _this->rows || col >= _this->cols)
        return NULL;
    
    return (leString*)_this->cells[row][col].value;
}

static leResult setKeyValue(leKeyPadWidget* _this,
                            uint32_t row,
                            uint32_t col,
                            const leString* val)
{
    if(row >= _this->rows || col >= _this->cols)
        return LE_FAILURE;

    _this->cells[row][col].value = val;

    _this->fn->invalidate(_this);

    return LE_SUCCESS;
}
                                               
static leButtonWidget* getCellButton(const leKeyPadWidget* _this,
                                     uint32_t row,
                                     uint32_t col)
{
    leButtonWidget* btn;
    
    LE_ASSERT_THIS();
    
    if(row >= _this->rows || col >= _this->cols)
        return NULL;
       
    btn = (leButtonWidget*)_this->fn->getChildAtIndex(_this, (row * _this->cols) + col);

    return btn;
}

static void handleLanguageChangeEvent(leKeyPadWidget* _this)
{
    LE_ASSERT_THIS();

    _this->fn->invalidate(_this);
}

void _leKeyPadWidget_Paint(leKeyPadWidget* _this);

#if LE_DYNAMIC_VTABLES == 1
void _leWidget_FillVTable(leWidgetVTable* tbl);

void _leKeyPadWidget_GenerateVTable()
{
    _leWidget_FillVTable((void*)&keyPadWidgetVTable);
    
    /* overrides from base class */
    keyPadWidgetVTable._destructor = destructor;
    keyPadWidgetVTable.resizeEvent = _resizeCellEvent;
    keyPadWidgetVTable._paint = _leKeyPadWidget_Paint;
    keyPadWidgetVTable.languageChangeEvent = handleLanguageChangeEvent;
    
    /* member functions */
    keyPadWidgetVTable.setCellArraySize = setCellArraySize;
    keyPadWidgetVTable.getKeyPadActionTrigger = getKeyPadActionTrigger;
    keyPadWidgetVTable.setKeyPadActionTrigger = setKeyPadActionTrigger;
    keyPadWidgetVTable.getKeyClickEventCallback = getKeyClickEventCallback;
    keyPadWidgetVTable.setKeyClickEventCallback = setKeyClickEventCallback;
    keyPadWidgetVTable.setKeyVisible = setKeyVisible;
    keyPadWidgetVTable.getKeyAction = getKeyAction;
    keyPadWidgetVTable.setKeyAction = setKeyAction;
    keyPadWidgetVTable.getKeyValue = getKeyValue;
    keyPadWidgetVTable.setKeyValue = setKeyValue;
    keyPadWidgetVTable.getCellButton = getCellButton;
}

void _leKeyPadWidget_FillVTable(leKeyPadWidgetVTable* tbl)
{
    *tbl = keyPadWidgetVTable;
}
#else
static const leKeyPadWidgetVTable keyPadWidgetVTable =
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
    .resizeEvent = _resizeCellEvent,
    ._paint = _leKeyPadWidget_Paint,
    .languageChangeEvent = handleLanguageChangeEvent,

    /* member functions */
    .setCellArraySize = setCellArraySize,
    .getKeyPadActionTrigger = getKeyPadActionTrigger,
    .setKeyPadActionTrigger = setKeyPadActionTrigger,
    .getKeyClickEventCallback = getKeyClickEventCallback,
    .setKeyClickEventCallback = setKeyClickEventCallback,
    .setKeyVisible = setKeyVisible,
    .getKeyAction = getKeyAction,
    .setKeyAction = setKeyAction,
    .getKeyValue = getKeyValue,
    .setKeyValue = setKeyValue,
    .getCellButton = getCellButton,
};
#endif
                                                    
#endif // LE_WIDGET_KEYPAD_ENABLED && LE_WIDGET_BUTTON_ENABLED