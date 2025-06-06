// DOM-IGNORE-BEGIN
/*******************************************************************************
* Copyright (C) 2024 Microchip Technology Inc. and its subsidiaries.
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
#include "gfx/legato/widget/keyboard/legato_widget_keyboard.h"

#if LE_KEYBOARD_WIDGET_ENABLED

#include "gfx/legato/common/legato_error.h"
#include "gfx/legato/common/legato_utils.h"
#include "gfx/legato/memory/legato_memory.h"
#include "gfx/legato/string/legato_string.h"
#include "gfx/legato/widget/button/legato_widget_button.h"

#if LE_DEBUG == 1
#include "gfx/legato/core/legato_debug.h"
#endif

#define DEFAULT_WIDTH           200
#define DEFAULT_HEIGHT          100

#define DEFAULT_KEYMODE         LE_KEYBOARD_MODE_DEFAULT

static
#if LE_DYNAMIC_VTABLES == 0
const
#endif

leKeyboardWidgetVTable keyboardWidgetVTable;
leKeyboardKeyType defaultKeysRow0[LE_KEYBOARD_MODE_MAX][NUM_KEYS_ROW0] = 
{ 
    {'q', 'w', 'e', 'r', 't', 'y', 'u', 'i','o', 'p'},
    {'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I','O', 'P'},
    {'1', '2', '3', '4', '5', '6', '7', '8','9', '0'},
    {'~', '`', '|', '.', '?', '/', '\\', '[',']', '0'},
};

leKeyboardKeyType defaultKeysRow1[LE_KEYBOARD_MODE_MAX][NUM_KEYS_ROW1] = 
{ 
    {'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l'},
    {'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L'},
    {'@', '#', '$', '_', '&', '-', '+', '(', ')'},
    {0xA3 /*£*/, 0xA2 /*¢*/, '?', 0xA5 /*¥*/, '^', '|', '=', '{', '}'},
};

leKeyboardKeyType defaultKeysRow2[LE_KEYBOARD_MODE_MAX][NUM_KEYS_ROW2] = 
{ 
    {LE_KEYBOARD_KEY_SHIFT, 'z', 'x', 'c', 'v', 'b', 'n', 'm', LE_KEYBOARD_KEY_BACKSPACE},
    {LE_KEYBOARD_KEY_SHIFT, 'Z', 'X', 'C', 'V', 'B', 'N', 'M', LE_KEYBOARD_KEY_BACKSPACE},
    {LE_KEYBOARD_KEY_SYMBOL2, '*', '"', '\'', ':', ';', '!', '?', LE_KEYBOARD_KEY_BACKSPACE},
    {LE_KEYBOARD_KEY_SYMBOL1, '%', 0xA9 /*©*/, 0xAE /*®*/, 0xB0 /*�*/, 0xF7 /*÷*/, '<', '>', LE_KEYBOARD_KEY_BACKSPACE},
};

leKeyboardKeyType defaultKeysRow3[LE_KEYBOARD_MODE_MAX][NUM_KEYS_ROW3] = 
{ 
    {LE_KEYBOARD_KEY_SYMBOL1, ',', ' ', '.', LE_KEYBOARD_KEY_RETURN},
    {LE_KEYBOARD_KEY_SYMBOL1, ',', ' ', '.', LE_KEYBOARD_KEY_RETURN},
    {LE_KEYBOARD_KEY_ABC, ',', ' ', '.', LE_KEYBOARD_KEY_RETURN},
    {LE_KEYBOARD_KEY_ABC, ',', ' ', '.', LE_KEYBOARD_KEY_RETURN},
};

static leResult _initializeKeys(leKeyboardWidget* _this)
{
    uint32_t row, keys, keyIdx;
    leKeyboardKey * key;

    /* TODO: Add check for damage area */
    for (row = 0; row < NUM_KEY_ROWS; row++)
    {
        keys = _leKeyboardWidget_getKeyCount(_this, row);
        for (keyIdx = 0; keyIdx < keys; keyIdx++)
        {
            key = _leKeyboardWidget_getKey(_this, row, keyIdx);

            key->state = LE_KEYBOARD_KEY_RELEASED;
        }
    } 
    
    return LE_SUCCESS;
}

/* New */
static leResult _resizeKeys(leKeyboardWidget* _this)
{
    leRect keyAreaRect;
    leRect keyButtonRect;
    uint32_t gapX, gapY;
    uint32_t i;

    keyAreaRect.x = 0;
    keyAreaRect.y = 0;
    keyAreaRect.width = _this->widget.rect.width / NUM_KEYS_ROW0;
    keyAreaRect.height = _this->widget.rect.height / NUM_KEY_ROWS;

    keyButtonRect.width = (keyAreaRect.width * (KEY_AREA_GAP_RATIO - 1)) / KEY_AREA_GAP_RATIO;
    keyButtonRect.height = (keyAreaRect.height * (KEY_AREA_GAP_RATIO - 1)) / KEY_AREA_GAP_RATIO;

    gapX = (keyAreaRect.width - keyButtonRect.width);
    gapY = (keyAreaRect.height - keyButtonRect.height);

    keyButtonRect.x = gapX/2;
    keyButtonRect.y = gapY/2;
    
    /* Resize row 1 keys (all standard keys)*/
    for (i = 0; i < NUM_KEYS_ROW0; i++)
    {
        _this->keysRow0[i].rect.x = (i * keyAreaRect.width) + gapX/2;
        _this->keysRow0[i].rect.y = gapY/2;
        _this->keysRow0[i].rect.height = keyButtonRect.height;
        _this->keysRow0[i].rect.width = keyButtonRect.width;
    }

    /* Resize row 2 keys (all standard keys, x-offset half a key )*/
    for (i = 0; i < NUM_KEYS_ROW1; i++)
    {
        _this->keysRow1[i].rect.x = (i * keyAreaRect.width) + gapX/2 + keyAreaRect.width / 2;
        _this->keysRow1[i].rect.y = keyAreaRect.height + gapY/2;
        _this->keysRow1[i].rect.height = keyButtonRect.height;
        _this->keysRow1[i].rect.width = keyButtonRect.width;
    }

    /* Resize row 3 keys: shift + 7 std keys + bkspace*/
        /* shift key */
    _this->keysRow2[0].rect.x = gapX/2;
    _this->keysRow2[0].rect.y = keyAreaRect.height * 2 + gapY/2;
    _this->keysRow2[0].rect.height = keyButtonRect.height;
    _this->keysRow2[0].rect.width = ((keyAreaRect.width * 3) / 2) - gapX;
        /* 7 standard keys */
    for (i = 1; i < NUM_KEYS_ROW2 - 1; i++)
    {
        _this->keysRow2[i].rect.x = (i * keyAreaRect.width) + gapX/2 + keyAreaRect.width / 2;
        _this->keysRow2[i].rect.y = keyAreaRect.height * 2 + gapY/2;
        _this->keysRow2[i].rect.height = keyButtonRect.height;
        _this->keysRow2[i].rect.width = keyButtonRect.width;
    }
        /* backspace */
    _this->keysRow2[NUM_KEYS_ROW2 - 1].rect.x = (keyAreaRect.width / 2) + (keyAreaRect.width * (NUM_KEYS_ROW2 - 1)) + gapX/2;
    _this->keysRow2[NUM_KEYS_ROW2 - 1].rect.y = keyAreaRect.height * 2 + gapY/2;
    _this->keysRow2[NUM_KEYS_ROW2 - 1].rect.height = keyButtonRect.height;
    _this->keysRow2[NUM_KEYS_ROW2 - 1].rect.width = ((keyAreaRect.width * 3) / 2) - gapX;

    /* Resize row 4: 123 key + std key + space + std key + return */
        /* 123 key */
    _this->keysRow3[0].rect.x = gapX/2;
    _this->keysRow3[0].rect.y = keyAreaRect.height * 3 + gapY/2;
    _this->keysRow3[0].rect.height = keyButtonRect.height;
    _this->keysRow3[0].rect.width = ((keyAreaRect.width * 3) / 2) - gapX;
        /* 1 std key */
    _this->keysRow3[1].rect.x = (keyAreaRect.width * 3) / 2 + gapX/2;
    _this->keysRow3[1].rect.y = keyAreaRect.height * 3 + gapY/2;
    _this->keysRow3[1].rect.height = keyButtonRect.height;
    _this->keysRow3[1].rect.width = keyButtonRect.width;
        /* space bar */
    _this->keysRow3[2].rect.x = (keyAreaRect.width * 5) / 2 + gapX/2;
    _this->keysRow3[2].rect.y = keyAreaRect.height * 3 + gapY/2;
    _this->keysRow3[2].rect.height = keyButtonRect.height;
    _this->keysRow3[2].rect.width = (keyAreaRect.width * 5) - gapX;        
        /* 1 std key */
    _this->keysRow3[3].rect.x = (keyAreaRect.width * 15) / 2 + gapX/2;
    _this->keysRow3[3].rect.y = keyAreaRect.height * 3 + gapY/2;
    _this->keysRow3[3].rect.height = keyButtonRect.height;
    _this->keysRow3[3].rect.width = keyButtonRect.width;        
        /* return  */
    _this->keysRow3[4].rect.x = (keyAreaRect.width * 17) / 2 + gapX/2;
    _this->keysRow3[4].rect.y = keyAreaRect.height * 3 + gapY/2;
    _this->keysRow3[4].rect.height = keyButtonRect.height;
    _this->keysRow3[4].rect.width = ((keyAreaRect.width * 3) / 2) - gapX;
    
    return LE_SUCCESS;
}

uint32_t _leKeyboardWidget_getKeyCount(leKeyboardWidget* _this, uint32_t row)
{
    LE_ASSERT_THIS();

    switch(row)
    {
        case 0:
        {
            return NUM_KEYS_ROW0;
        }
        case 1:
        {
            return NUM_KEYS_ROW1;
        }
        case 2:
        {
            return NUM_KEYS_ROW2;
        }
        case 3:
        {
            return NUM_KEYS_ROW3;
        }
        default:
            break;                        
    }

    return 0;
}

leKeyboardKey * _leKeyboardWidget_getKey(leKeyboardWidget* _this, uint32_t row, uint32_t idx)
{
    LE_ASSERT_THIS();

    switch(row)
    {
        case 0:
        {
            if (idx < NUM_KEYS_ROW0)
            {
                return &_this->keysRow0[idx];
            }
            break;
        }
        case 1:
        {
            if (idx < NUM_KEYS_ROW1)
            {
                return &_this->keysRow1[idx];
            }
            break;
        }
        case 2:
        {
            if (idx < NUM_KEYS_ROW2)
            {
                return &_this->keysRow2[idx];
            }
            break;
        }
        case 3:
        {
            if (idx < NUM_KEYS_ROW3)
            {
                return &_this->keysRow3[idx];
            }
            break;
        }
        default:
            break;                        
    }

    return NULL;
}

const leRect * _leKeyboardWidget_getKeyRect(leKeyboardWidget* _this, uint32_t row, uint32_t idx)
{
    leKeyboardKey * key;
    leRect * keyRect = NULL;

    LE_ASSERT_THIS();
    
    key = _leKeyboardWidget_getKey(_this, row, idx);
    if (key != NULL)
    {
        keyRect = &key->rect;
    }
    
    return keyRect;
}

/* returns the key that's in focus, pnt is local widget space */
static leKeyboardKey * _leKeyboardWidget_getFocusKey(leKeyboardWidget * _this, lePoint * pnt)
{
    uint32_t row;

    LE_ASSERT_THIS();

    for (row = 0; row < NUM_KEY_ROWS; row++)
    {
        leRect rowRect = leRect_Zero;
        
        rowRect.width = _this->widget.rect.width;
        rowRect.height = _this->widget.rect.height / NUM_KEY_ROWS;
        rowRect.x = 0;
        rowRect.y = rowRect.height * row;

        if (leRectContainsPoint(&rowRect, pnt) == LE_TRUE)
        {
            uint32_t numKeys = _leKeyboardWidget_getKeyCount(_this, row);
            uint32_t keyIdx;
            leKeyboardKey * key;

            for (keyIdx = 0; keyIdx < numKeys; keyIdx++)
            {
               key = _leKeyboardWidget_getKey(_this, row, keyIdx);
               if (key != NULL && leRectContainsPoint(&key->rect, pnt) == LE_TRUE)
               {
                    return key;
               }
            }
        }
    }

    return NULL;
}

static void _resizeCellEvent(leKeyboardWidget* _this,
                             leWidget_ResizeEvent* evt)
{
    (void)evt; // unused

    _resizeKeys(_this);

    _this->fn->invalidate(_this);
}

const leString * _leKeyboardWidget_getKeyString(leKeyboardWidget* _this, leKeyboardKey * key)
{
    _this->_keyString.fn->setFont(&_this->_keyString, (leFont*) _this->keyFont); 
    
    switch(key->type)
    {
        case LE_KEYBOARD_KEY_SYMBOL1:
        {
            _this->_keyString.fn->setFromCStr(&_this->_keyString, "?123");
            break;
        }
        case LE_KEYBOARD_KEY_SYMBOL2:
        {
            _this->_keyString.fn->setFromCStr(&_this->_keyString, "=\\<");
            break;
        }
        case LE_KEYBOARD_KEY_ABC:
        {
            _this->_keyString.fn->setFromCStr(&_this->_keyString, "ABC");
            break;
        }
        default:
        {
            leChar c = (leChar) key->type;
            _this->_keyString.fn->setFromChar(&_this->_keyString, (leChar *) &c, 1);
            break;
        }
    }

    return (const leString *) &_this->_keyString;

}

static void keyClicked(leKeyboardWidget* _this, leKeyboardKey * key)
{
    const leString * _keyString;
    
    LE_ASSERT_THIS();
    
    if ((key->state == LE_KEYBOARD_KEY_RELEASED && _this->trigger == LE_KEYBOARD_TRIGGER_KEYPRESSED) ||
        (key->state == LE_KEYBOARD_KEY_PRESSED && _this->trigger == LE_KEYBOARD_TRIGGER_KEYRELEASED))
        return;

    if (key->type & LE_KEYBOARD_KEY_SPECIAL_MASK)
    {
        switch(key->type)
        {
            case LE_KEYBOARD_KEY_SHIFT:
            {
                if (_this->mode == LE_KEYBOARD_MODE_DEFAULT)
                {
                    _this->fn->setKeyMode(_this, LE_KEYBOARD_MODE_SHIFT);
                }
                else if (_this->mode == LE_KEYBOARD_MODE_SHIFT)
                {
                    _this->fn->setKeyMode(_this, LE_KEYBOARD_MODE_DEFAULT);
                }
                break;
            }
            case LE_KEYBOARD_KEY_BACKSPACE:
            {
                _leEditWidget_Backspace();
                break;
            }
            case LE_KEYBOARD_KEY_SYMBOL1:
            {
                _this->fn->setKeyMode(_this, LE_KEYBOARD_MODE_SYMBOL1);
                break;
            }
            case LE_KEYBOARD_KEY_ABC:
            {
                _this->fn->setKeyMode(_this, LE_KEYBOARD_MODE_DEFAULT);
                break;
            }
            case LE_KEYBOARD_KEY_SYMBOL2:
            {
                _this->fn->setKeyMode(_this, LE_KEYBOARD_MODE_SYMBOL2);
                break;
            }
            case LE_KEYBOARD_KEY_RETURN:
            {
                _leEditWidget_Accept();
                break;
            }
            default:
                break;
        }
    }
    //Standard character key
    else 
    {
        _keyString = _leKeyboardWidget_getKeyString(_this, key);
        _leEditWidget_Append(_keyString);
    }
}

void leKeyboardWidget_Constructor(leKeyboardWidget* _this)
{
    leWidget_Constructor((leWidget*)_this);
    
    _this->widget.fn = (void*)&keyboardWidgetVTable;
    _this->fn = &keyboardWidgetVTable;
    
    _this->widget.type = LE_WIDGET_KEYBOARD;

    _this->widget.rect.width = DEFAULT_WIDTH;
    _this->widget.rect.height = DEFAULT_HEIGHT;

    _this->widget.style.borderEnabled = LE_TRUE;
    _this->widget.style.backgroundEnabled = LE_FALSE;

    _this->keyStyle = &_this->widget.style.backgroundStyle;
    
    _this->trigger = LE_KEYBOARD_TRIGGER_KEYRELEASED;

    _this->clickEvt = NULL;

    _initializeKeys(_this);
    _resizeKeys(_this);

    leFixedString_Constructor(&_this->_keyString, _this->_keyStringChar, MAX_KEYSTR_LENGTH);

    /* Default Keymap */
    _this->mode = DEFAULT_KEYMODE;
    _this->fn->setKeyMap(_this, 0, defaultKeysRow0[_this->mode], NUM_KEYS_ROW0);
    _this->fn->setKeyMap(_this, 1, defaultKeysRow1[_this->mode], NUM_KEYS_ROW1);
    _this->fn->setKeyMap(_this, 2, defaultKeysRow2[_this->mode], NUM_KEYS_ROW2);
    _this->fn->setKeyMap(_this, 3, defaultKeysRow3[_this->mode], NUM_KEYS_ROW3);

    _this->keyFont = NULL;
    _this->currFocusKey = NULL;
}

leKeyboardWidget* leKeyboardWidget_New()
{
    leKeyboardWidget* keyboard = NULL;

    keyboard = LE_MALLOC(sizeof(leKeyboardWidget));
    
    leKeyboardWidget_Constructor(keyboard);

    return keyboard;
}

void _leWidget_Destructor(leWidget* _this);

void _leKeyboardWidget_Destructor(leKeyboardWidget* _this)
{
    LE_ASSERT_THIS();
    
    _this->clickEvt = NULL;
    _this->keyFont = NULL;
    _this->currFocusKey = NULL;

    /* call base class destructor */
    _leWidget_Destructor((leWidget*)_this);
}

static leKeyboardActionTrigger getKeyboardActionTrigger(const leKeyboardWidget* _this)
{
    LE_ASSERT_THIS();

    return _this->trigger;
}

static leResult setKeyboardActionTrigger(leKeyboardWidget* _this,
                                       leKeyboardActionTrigger trigger)
{
    LE_ASSERT_THIS();

    _this->trigger = trigger;

#if LE_DEBUG == 1
    _leDebugNotify_WidgetPropertyChanged((leWidget*)_this);
#endif

    return LE_SUCCESS;
}

static leKeyboardWidget_KeyClickEvent getKeyClickEventCallback(const leKeyboardWidget* _this)
{
    LE_ASSERT_THIS();

    return _this->clickEvt;
}

static leResult setKeyClickEventCallback(leKeyboardWidget* _this,
                                         leKeyboardWidget_KeyClickEvent cb)
{
    LE_ASSERT_THIS();
    
    _this->clickEvt = cb;
    
    return LE_SUCCESS;
}

/* New */
static leResult setKeyFillStyle(leKeyboardWidget* _this, leFillStyle* style)
{
    LE_ASSERT_THIS();

    if (style != _this->keyStyle)
    {
        _this->keyStyle = style;
        _this->fn->invalidate(_this);
    }
    
    return LE_SUCCESS;
}

static leResult setKeyFont(leKeyboardWidget* _this, const leFont* font)
{
    LE_ASSERT_THIS();

    if (font != _this->keyFont)
    {
        _this->keyFont = font;
        _this->fn->invalidate(_this);
    }
    
    return LE_SUCCESS;
}

static leResult setKeyMap(leKeyboardWidget* _this, uint32_t row, leKeyboardKeyType * type, uint32_t numkeys)
{
    leResult result = LE_FAILURE;
    leKeyboardKey * keyRow = NULL;
    uint32_t keyCount = 0;

    LE_ASSERT_THIS();

    if ((type != NULL) &&
        (numkeys > 0))
    {
        switch(row)
        {
            case 0:
            {
                keyRow = _this->keysRow0;
                keyCount = NUM_KEYS_ROW0;
                break;
            }
            case 1:
            {
                keyRow = _this->keysRow1;
                keyCount = NUM_KEYS_ROW1;
                break;
            }
            case 2:
            {
                keyRow = _this->keysRow2;
                keyCount = NUM_KEYS_ROW2;
                break;
            }
            case 3:
            {
                keyRow = _this->keysRow3;
                keyCount = NUM_KEYS_ROW3;
                break;
            }
            default:
            {
                break;
            }
        }

        if (keyRow != NULL)
        {
            uint32_t i;
            keyCount = (numkeys < keyCount) ? numkeys : keyCount;

            for (i = 0; i < keyCount; i++)
            {
                keyRow[i].type = type[i];
            }

            result = LE_SUCCESS;
        }
    }

    return result;
}

static leResult setKeyMode(leKeyboardWidget * _this, leKeyboardKeyMode mode)
{
    LE_ASSERT_THIS();

    if (mode <  LE_KEYBOARD_MODE_MAX && mode != _this->mode)
    {
        _this->mode = mode;
        _this->fn->setKeyMap(_this, 0, defaultKeysRow0[_this->mode], NUM_KEYS_ROW0);
        _this->fn->setKeyMap(_this, 1, defaultKeysRow1[_this->mode], NUM_KEYS_ROW1);
        _this->fn->setKeyMap(_this, 2, defaultKeysRow2[_this->mode], NUM_KEYS_ROW2);
        _this->fn->setKeyMap(_this, 3, defaultKeysRow3[_this->mode], NUM_KEYS_ROW3);

        _this->fn->invalidate(_this);

        return LE_SUCCESS;
    }

    return LE_FAILURE;
}

void touchDown(leKeyboardWidget* _this,
               leWidgetEvent_TouchDown* evt)
{
    lePoint pnt;
    leKeyboardKey * key;

    LE_ASSERT_THIS();

    pnt.x = evt->x;
    pnt.y = evt->y;

    leUtils_PointScreenToLocalSpace((const leWidget*) _this, &pnt);

    leWidgetEvent_Accept((leWidgetEvent*)evt, (leWidget*)_this);

    key = _leKeyboardWidget_getFocusKey(_this, &pnt);
    if (key != NULL)
    {
        leRect keyRect = key->rect;
        
        leUtils_RectToScreenSpace(&_this->widget, &keyRect);

        keyClicked(_this, key);

        key->state = LE_KEYBOARD_KEY_PRESSED;

        _this->currFocusKey = key;

        _this->fn->_damageArea(_this, &keyRect);
    }
}

void touchUp(leKeyboardWidget* _this,
             leWidgetEvent_TouchUp* evt)
{
    lePoint pnt;
    leKeyboardKey * key;

    LE_ASSERT_THIS();

    pnt.x = evt->x;
    pnt.y = evt->y;

    leUtils_PointScreenToLocalSpace((const leWidget*) _this, &pnt);

    leWidgetEvent_Accept((leWidgetEvent*)evt, (leWidget*)_this);

    key = _leKeyboardWidget_getFocusKey(_this, &pnt);
    if (key != NULL)
    {
        leRect keyRect = key->rect;

        if (_this->currFocusKey == key)
        {
            keyRect = key->rect;

            keyClicked(_this, key);

            key->state = LE_KEYBOARD_KEY_RELEASED;
        }
        /* different key from previously pressed */
        else
        {
            keyRect = _this->currFocusKey->rect;

            _this->currFocusKey->state = LE_KEYBOARD_KEY_RELEASED;
            
        }

        leUtils_RectToScreenSpace(&_this->widget, &keyRect);

        _this->fn->_damageArea(_this, &keyRect);
    }
}

void touchMoved(leKeyboardWidget* _this,
                leWidgetEvent_TouchMove* evt)
{
    lePoint pnt;
    leKeyboardKey * key;

    LE_ASSERT_THIS();

    pnt.x = evt->x;
    pnt.y = evt->y;

    leUtils_PointScreenToLocalSpace((const leWidget*) _this, &pnt);

    leWidgetEvent_Accept((leWidgetEvent*)evt, (leWidget*)_this);

    key = _leKeyboardWidget_getFocusKey(_this, &pnt);
    if (key != _this->currFocusKey && _this->currFocusKey != NULL)
    {
        leRect keyRect = _this->currFocusKey->rect;

        _this->currFocusKey->state = LE_KEYBOARD_KEY_RELEASED;

        leUtils_RectToScreenSpace(&_this->widget, &keyRect);

        _this->fn->_damageArea(_this, &keyRect);
    }
}

static void handleLanguageChangeEvent(leKeyboardWidget* _this)
{
    LE_ASSERT_THIS();

    _this->fn->invalidate(_this);
}

void _leKeyboardWidget_Paint(leKeyboardWidget* _this);

#if LE_DYNAMIC_VTABLES == 1
void _leWidget_FillVTable(leWidgetVTable* tbl);

void _leKeyboardWidget_GenerateVTable()
{
    _leWidget_FillVTable((void*)&keyboardWidgetVTable);
    
    /* overrides from base class */
    keyboardWidgetVTable._destructor = _leKeyboardWidget_Destructor;
    keyboardWidgetVTable.resizeEvent = _resizeCellEvent;
    keyboardWidgetVTable._paint = _leKeyboardWidget_Paint;
    keyboardWidgetVTable.languageChangeEvent = handleLanguageChangeEvent;
    
    /* member functions */
    keyboardWidgetVTable.getKeyboardActionTrigger = getKeyboardActionTrigger;
    keyboardWidgetVTable.setKeyboardActionTrigger = setKeyboardActionTrigger;
    keyboardWidgetVTable.getKeyClickEventCallback = getKeyClickEventCallback;
    keyboardWidgetVTable.setKeyClickEventCallback = setKeyClickEventCallback;
    keyboardWidgetVTable.setKeyFillStyle = setKeyFillStyle;
    
    keyboardWidgetVTable.getKeyboardActionTrigger = getKeyboardActionTrigger;
    keyboardWidgetVTable.setKeyboardActionTrigger = setKeyboardActionTrigger;
    keyboardWidgetVTable.getKeyClickEventCallback = getKeyClickEventCallback;
    keyboardWidgetVTable.setKeyClickEventCallback = setKeyClickEventCallback;
    keyboardWidgetVTable.setKeyFillStyle = setKeyFillStyle;
    keyboardWidgetVTable.setKeyFont = setKeyFont;
    keyboardWidgetVTable.setKeyMap = setKeyMap;
    keyboardWidgetVTable.setKeyMode = setKeyMode;
}

void _leKeyboardWidget_FillVTable(leKeyboardWidgetVTable* tbl)
{
    *tbl = keyboardWidgetVTable;
}
#else
static const leKeyboardWidgetVTable keyboardWidgetVTable =
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
    .getBorderEnabled = (void*)_leWidget_GetBorderEnabled,
    .setBorderEnabled = (void*)_leWidget_SetBorderEnabled,
    .getBorderStyle = (void*)_leWidget_GetBorderStyle,
    .setBorderStyle = (void*)_leWidget_SetBorderStyle,
    .getBackgroundEnabled = (void*)_leWidget_GetBackgroundEnabled,
    .setBackgroundEnabled = (void*)_leWidget_SetBackgroundEnabled,
    .getBackgroundStyle = (void*)_leWidget_GetBackgroundStyle,
    .setBackgroundStyle = (void*)_leWidget_SetBackgroundStyle,
    .getHAlignment = (void*)_leWidget_GetHAlignment,
    .setHAlignment = (void*)_leWidget_SetHAlignment,
    .getVAlignment = (void*)_leWidget_GetVAlignment,
    .setVAlignment = (void*)_leWidget_SetVAlignment,
    .getMargins = (void*)_leWidget_GetMargins,
    .setMargins = (void*)_leWidget_SetMargins,
    .hasFocus = (void*)_leWidget_HasFocus,
    .setFocus = (void*)_leWidget_SetFocus,
    .invalidate = (void*)_leWidget_Invalidate,
    .invalidateContents = (void*)_leWidget_InvalidateContents,
    .installEventFilter = (void*)_leWidget_InstallEventFilter,
    .removeEventFilter = (void*)_leWidget_RemoveEventFilter,

    .update = (void*)_leWidget_Update,

    .touchDownEvent = (void*)touchDown,
    .touchUpEvent = (void*)touchUp,
    .touchMoveEvent = (void*)touchMoved,
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
    ._destructor = _leKeyboardWidget_Destructor,
    .resizeEvent = _resizeCellEvent,
    ._paint = _leKeyboardWidget_Paint,
    .languageChangeEvent = handleLanguageChangeEvent,

    /* member functions */
    .getKeyboardActionTrigger = getKeyboardActionTrigger,
    .setKeyboardActionTrigger = setKeyboardActionTrigger,
    .getKeyClickEventCallback = getKeyClickEventCallback,
    .setKeyClickEventCallback = setKeyClickEventCallback,
    .setKeyFillStyle = setKeyFillStyle,
    .setKeyFont = setKeyFont,
    .setKeyMap = setKeyMap,
    .setKeyMode = setKeyMode,
};
#endif
                                                    
#endif // LE_WIDGET_KEYPAD_ENABLED && LE_WIDGET_BUTTON_ENABLED
