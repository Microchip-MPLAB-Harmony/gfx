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


#include "gfx/legato/widget/button/legato_widget_button.h"

#if LE_BUTTON_WIDGET_ENABLED == 1

#include <string.h>
#include <gfx/legato/legato.h>

#include "gfx/legato/common/legato_error.h"
#include "gfx/legato/common/legato_utils.h"
#include "gfx/legato/memory/legato_memory.h"
#include "gfx/legato/renderer/legato_renderer.h"
#include "gfx/legato/string/legato_string.h"
#include "gfx/legato/widget/legato_widget.h"

#define DEFAULT_WIDTH           100
#define DEFAULT_HEIGHT          25

#define DEFAULT_IMAGE_MARGIN    10

#define DEFAULT_PRESSED_OFFSET  1

static
#if LE_DYNAMIC_VTABLES == 0
const
#endif
leButtonWidgetVTable buttonWidgetVTable;

void _leButtonWidget_GetImageRect(const leButtonWidget* _this,
                                  leRect* imgRect,
                                  leRect* imgSrcRect);

void _leButtonWidget_GetTextRects(const leButtonWidget* btn,
                                  leRect* boundingRect,
                                  leRect* kerningRect);

void _leButtonWidget_InvalidateBorderAreas(const leButtonWidget* _this);

static void invalidateImageRect(const leButtonWidget* _this)
{
    leRect imgRect, imgSrcRect;
    
    _leButtonWidget_GetImageRect(_this, &imgRect, &imgSrcRect);
    
    _this->fn->_damageArea(_this, &imgRect);
}

static void invalidateTextRect(const leButtonWidget* _this)
{
    leRect boundingRect, kerningRect;
    
    _leButtonWidget_GetTextRects(_this, &boundingRect, &kerningRect);
    
    _this->fn->_damageArea(_this, &boundingRect);
}

static void stringPreinvalidate(const leString* str,
                                leButtonWidget* btn)
{
    (void)str; // unused

    invalidateTextRect(btn);
}

static void stringInvalidate(const leString* str,
                             leButtonWidget* btn)
{
    (void)str; // unused

    invalidateTextRect(btn);
}

void leButtonWidget_Constructor(leButtonWidget* _this)
{
    /* call base class constructor */
    leWidget_Constructor((leWidget*)_this);
    
    _this->widget.fn = (void*)&buttonWidgetVTable;
    _this->fn = &buttonWidgetVTable;

    _this->widget.type = LE_WIDGET_BUTTON;

    _this->widget.rect.width = DEFAULT_WIDTH;
    _this->widget.rect.height = DEFAULT_HEIGHT;

    _this->state = LE_BUTTON_STATE_UP;
    _this->toggleable = LE_FALSE;

    _this->widget.style.borderType = LE_WIDGET_BORDER_BEVEL;

    _this->string = NULL;

    _this->imagePosition = LE_RELATIVE_POSITION_LEFTOF;
    _this->imageMargin = DEFAULT_IMAGE_MARGIN;
    _this->pressedOffset = DEFAULT_PRESSED_OFFSET;
    
    _this->pressedImage = NULL;
    _this->releasedImage = NULL;

    _this->pressedEvent = NULL;
    _this->releasedEvent = NULL;
}

leButtonWidget* leButtonWidget_New()
{
    leButtonWidget* wgt = NULL;

    wgt = LE_MALLOC(sizeof(leButtonWidget));
    
    if(wgt == NULL)
        return NULL;
    
    leButtonWidget_Constructor(wgt);

    return wgt;
}

void _leWidget_Destructor(leWidget* wgt);

void _leButtonWidget_Destructor(leButtonWidget* _this)
{
    LE_ASSERT_THIS();
    
    _this->string = NULL;
    _this->pressedImage = NULL;
    _this->releasedImage = NULL;

    /* call base class destructor */
    _leWidget_Destructor((leWidget*)_this);
}

static leBool getToggleable(const leButtonWidget* _this)
{
    LE_ASSERT_THIS();
    
    return _this->toggleable;
}

static leResult setToggleable(leButtonWidget* _this,
                              leBool toggleable)
{
    LE_ASSERT_THIS();
    
    _this->toggleable = toggleable == 0 ? LE_FALSE : LE_TRUE;
    
    return LE_SUCCESS;
}

static leBool getPressed(const leButtonWidget* _this)
{
    LE_ASSERT_THIS();
    
    return _this->state == LE_BUTTON_STATE_DOWN ||
           _this->state == LE_BUTTON_STATE_TOGGLED;
}

static leResult setPressed(leButtonWidget* _this,
                           leBool pressed)
{
    leBool dirty = LE_FALSE;

    LE_ASSERT_THIS();
    
    if(pressed == LE_TRUE && _this->state == LE_BUTTON_STATE_TOGGLED)
        return LE_FAILURE;
        
    if(pressed == LE_TRUE)
    {
        if(_this->pressedEvent != NULL)
        {
            _this->pressedEvent(_this);
        }
            
        // move to toggled state
        if(_this->toggleable == LE_TRUE)
        {
            _this->state = LE_BUTTON_STATE_TOGGLED;
        }
        // throw released event immediately
        else
        {
            if(_this->releasedEvent != NULL)
            {
                _this->releasedEvent(_this);
            }
        }
    }
    else
    {
        _this->state = LE_BUTTON_STATE_UP;
        
        if(_this->releasedEvent != NULL)
        {
            _this->releasedEvent(_this);
        }
    }
    
    // try to find a reason to redraw
    if(_this->pressedImage != NULL && 
       _this->pressedImage != _this->releasedImage)
    {
        dirty = LE_TRUE;
    }
       
    if(_this->widget.style.borderType == LE_WIDGET_BORDER_BEVEL)
    {
        dirty = LE_TRUE;
    }
        
    if(_this->pressedOffset != 0 &&
       (_this->pressedImage != NULL ||
       (_this->string != NULL && _this->string->fn->isEmpty(_this->string) == LE_FALSE)))
    {
        dirty = LE_TRUE;
    }
        
    if(dirty == LE_TRUE)
    {
        _this->fn->invalidate(_this);
    }
        
    return LE_SUCCESS;
}

static leString* getString(const leButtonWidget* _this)
{
    LE_ASSERT_THIS();
    
    return (leString*)_this->string;
}

static leResult setString(leButtonWidget* _this,
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

        invalidateTextRect(_this);
    }
    
    return LE_SUCCESS;
}

static leImage* getPressedImage(const leButtonWidget* _this)
{
    LE_ASSERT_THIS();
    
    return (leImage*)_this->pressedImage;
}

static leResult setPressedImage(leButtonWidget* _this,
                                leImage* img)
{
    LE_ASSERT_THIS();
    
    _this->pressedImage = img;

    _this->fn->invalidateContents(_this);
    
    return LE_SUCCESS;
}

static leImage* getReleasedImage(const leButtonWidget* _this)
{
    LE_ASSERT_THIS();
    
    return (leImage*)_this->releasedImage;
}

static leResult setReleasedImage(leButtonWidget* _this,
                                 leImage* img)
{
    LE_ASSERT_THIS();
    
    _this->releasedImage = img;

    _this->fn->invalidateContents(_this);
    
    return LE_SUCCESS;
}

static leRelativePosition getImagePosition(const leButtonWidget* _this)
{
    LE_ASSERT_THIS();
    
    return _this->imagePosition;
}

static leResult setImagePosition(leButtonWidget* _this,
                                 leRelativePosition pos)
{
    LE_ASSERT_THIS();
    
    if(_this->imagePosition == pos)
        return LE_SUCCESS;
    
    _this->fn->invalidateContents(_this);
    
    _this->imagePosition = pos;
    
    _this->fn->invalidateContents(_this);
    
    return LE_SUCCESS;
}

static uint32_t getImageMargin(const leButtonWidget* _this)
{
    LE_ASSERT_THIS();
    
    return _this->imageMargin;
}

static leResult setImageMargin(leButtonWidget* _this,
                               uint32_t mg)
{
    LE_ASSERT_THIS();
    
    if(_this->imageMargin == mg)
        return LE_SUCCESS;
        
    _this->fn->invalidateContents(_this);
    
    _this->imageMargin = mg;
    
    _this->fn->invalidateContents(_this);
    
    return LE_SUCCESS;
}

static int32_t getPressedOffset(const leButtonWidget* _this)
{
    LE_ASSERT_THIS();
    
    return _this->pressedOffset;
}

static leResult setPressedOffset(leButtonWidget* _this,
                                 int32_t offs)
{
    LE_ASSERT_THIS();
    
    if(_this->pressedOffset == offs)
        return LE_SUCCESS;
        
    if(_this->state != LE_BUTTON_STATE_UP)
    {
        _this->fn->invalidateContents(_this);
    }
       
    _this->pressedOffset = offs;
    
    if(_this->state != LE_BUTTON_STATE_UP)
    {
        _this->fn->invalidateContents(_this);
    }
        
    return LE_SUCCESS;
}

static leButtonWidget_PressedEvent getPressedEventCallback(const leButtonWidget* _this)
{
    LE_ASSERT_THIS();
    
    return _this->pressedEvent;
}

static leResult setPressedEventCallback(leButtonWidget* _this,
                                        leButtonWidget_PressedEvent cb)
{   
    LE_ASSERT_THIS();
    
    _this->pressedEvent = cb;
    
    return LE_SUCCESS;
}

static leButtonWidget_ReleasedEvent getReleasedEventCallback(const leButtonWidget* _this)
{
    LE_ASSERT_THIS();
    
    return _this->releasedEvent;
}

static leResult setReleasedEventCallback(leButtonWidget* _this,
                                         leButtonWidget_ReleasedEvent cb)
{
    LE_ASSERT_THIS();
    
    _this->releasedEvent = cb;
    
    return LE_SUCCESS;
}

static void invalidateContents(const leButtonWidget* _this)
{
    LE_ASSERT_THIS();
    
    if(_this->string != NULL && _this->string->fn->isEmpty(_this->string) == LE_FALSE)
    {
        invalidateTextRect(_this);
    }
        
    if((_this->state != LE_BUTTON_STATE_UP && _this->pressedImage != NULL) ||
       (_this->state == LE_BUTTON_STATE_UP && _this->releasedImage != NULL))
    {
        invalidateImageRect(_this);
    }
}

static void languageChanging(leButtonWidget* _this)
{
    LE_ASSERT_THIS();
    
    if(_this->string != NULL)
    {
        _this->fn->invalidate(_this);
    }
}

static void touchDown(leButtonWidget* _this,
                      leWidgetEvent_TouchDown* evt)
{
    LE_ASSERT_THIS();

    leWidgetEvent_Accept((leWidgetEvent*)evt, (leWidget*)_this);

    if(_this->toggleable == LE_TRUE)
    {
        if(_this->state != LE_BUTTON_STATE_TOGGLED)
        {
            _this->state = LE_BUTTON_STATE_DOWN;

            if(_this->pressedEvent != NULL)
            {
                _this->pressedEvent(_this);
            }
        }
    }
    else
    {
        _this->state = LE_BUTTON_STATE_DOWN;

        if(_this->pressedEvent != NULL)
        {
            _this->pressedEvent(_this);
        }
    }

    if (_this->pressedOffset != 0 || 
        (_this->pressedImage != _this->releasedImage) ||
        (_this->widget.style.backgroundType != LE_WIDGET_BACKGROUND_NONE) || 
        (_this->widget.style.borderType != LE_WIDGET_BORDER_NONE))
    {
        _this->fn->invalidate(_this);
    }

    //printf("button touch down\n");
}

static void touchUp(leButtonWidget* _this,
                    leWidgetEvent_TouchUp* evt)
{
    leRect rect;
    lePoint pnt;
    
    LE_ASSERT_THIS();
    
    _this->fn->localRect(_this, &rect);

    pnt.x = evt->x;
    pnt.y = evt->y;

    leWidgetEvent_Accept((leWidgetEvent*)evt, (leWidget*)_this);

    leUtils_ClipRectToParent(_this->widget.parent, &rect);
    leUtils_RectToScreenSpace((leWidget*)_this, &rect);
    
    //Toggle buttons will not respond to touch outside the button area
    if ((leRectContainsPoint(&rect, &pnt) == LE_FALSE) && 
        (_this->toggleable == LE_TRUE))
    {
        return;
    }

    if(leRectContainsPoint(&rect, &pnt) == LE_TRUE)
    {
        if(_this->toggleable == LE_TRUE)
        {
            if(_this->state == LE_BUTTON_STATE_TOGGLED)
            {
                _this->state = LE_BUTTON_STATE_UP;

                if(_this->releasedEvent != NULL)
                {
                    _this->releasedEvent(_this);
                }
                
                //printf("_this released\n");
            }
            else
            {
                _this->state = LE_BUTTON_STATE_TOGGLED;
            }
        }
        else
        {
            _this->state = LE_BUTTON_STATE_UP;

            if(_this->releasedEvent != NULL)
            {
                _this->releasedEvent(_this);
            }
            
            //printf("_this released\n");
        }
    }
    else
    {
        if(_this->toggleable == LE_TRUE)
        {
            if(_this->state != LE_BUTTON_STATE_TOGGLED)
            {
                _this->state = LE_BUTTON_STATE_UP;
            }
        }
        else
        {
            _this->state = LE_BUTTON_STATE_UP;
        }
    }
    
    if (_this->pressedOffset != 0 || 
        (_this->pressedImage != _this->releasedImage) ||
        (_this->widget.style.backgroundType != LE_WIDGET_BACKGROUND_NONE) || 
        (_this->widget.style.borderType != LE_WIDGET_BORDER_NONE))
    {
        _this->fn->invalidate(_this);
    }

    //printf("button touch up\n");
}

static void touchMoved(leButtonWidget* _this,
                       leWidgetEvent_TouchMove* evt)
{
    leRect rect;
    lePoint pnt;
    
    LE_ASSERT_THIS();
    
    _this->fn->localRect(_this, &rect);
    
    pnt.x = evt->x;
    pnt.y = evt->y;

    leWidgetEvent_Accept((leWidgetEvent*)evt, (leWidget*)_this);
    
    //Toggle buttons will not respond to touch outside the button area
    if ((leRectContainsPoint(&rect, &pnt) == LE_FALSE) && 
        (_this->toggleable == LE_TRUE))
    {
        return;
    }

    if(_this->state == LE_BUTTON_STATE_DOWN)
    {
        leUtils_ClipRectToParent(_this->widget.parent, &rect);
        leUtils_RectToScreenSpace((leWidget*)_this, &rect);

        if(leRectContainsPoint(&rect, &pnt) == LE_FALSE)
        {
            _this->state = LE_BUTTON_STATE_UP;
            
            if (_this->pressedOffset != 0 || 
                (_this->pressedImage != _this->releasedImage) ||
                (_this->widget.style.backgroundType != LE_WIDGET_BACKGROUND_NONE) || 
                (_this->widget.style.borderType != LE_WIDGET_BORDER_NONE))
            {
                _this->fn->invalidate(_this);
            }
        }
    }
    else if(_this->state == LE_BUTTON_STATE_UP)
    {
        leUtils_ClipRectToParent(_this->widget.parent, &rect);
        leUtils_RectToScreenSpace((leWidget*)_this, &rect);

        if(leRectContainsPoint(&rect, &pnt) == LE_TRUE)
        {
            _this->state = LE_BUTTON_STATE_DOWN;

            if (_this->pressedOffset != 0 || 
                (_this->pressedImage != _this->releasedImage) ||
                (_this->widget.style.backgroundType != LE_WIDGET_BACKGROUND_NONE) || 
                (_this->widget.style.borderType != LE_WIDGET_BORDER_NONE))
            {
                _this->fn->invalidate(_this);
            }
        }
    }
}

void _leButtonWidget_Paint(leButtonWidget* _this);

#if LE_DYNAMIC_VTABLES == 1
void _leWidget_FillVTable(leWidgetVTable* tbl);

void _leButtonWidget_GenerateVTable(void)
{
    _leWidget_FillVTable((void*)&buttonWidgetVTable);
    
    /* overrides from base class */
    buttonWidgetVTable._destructor = _leButtonWidget_Destructor;
    buttonWidgetVTable.languageChangeEvent = languageChanging;
    buttonWidgetVTable._paint = _leButtonWidget_Paint;
    buttonWidgetVTable.invalidateContents = invalidateContents;
    buttonWidgetVTable._invalidateBorderAreas = _leButtonWidget_InvalidateBorderAreas;
    buttonWidgetVTable.touchDownEvent = touchDown;
    buttonWidgetVTable.touchUpEvent = touchUp;
    buttonWidgetVTable.touchMoveEvent = touchMoved;
    
    /* member functions */
    buttonWidgetVTable.getToggleable = getToggleable;
    buttonWidgetVTable.setToggleable = setToggleable;
    buttonWidgetVTable.getPressed = getPressed;
    buttonWidgetVTable.setPressed = setPressed;
    buttonWidgetVTable.getString = getString;
    buttonWidgetVTable.setString = setString;
    buttonWidgetVTable.getPressedImage = getPressedImage;
    buttonWidgetVTable.setPressedImage = setPressedImage;
    buttonWidgetVTable.getReleasedImage = getReleasedImage;
    buttonWidgetVTable.setReleasedImage = setReleasedImage;
    buttonWidgetVTable.getImagePosition = getImagePosition;
    buttonWidgetVTable.setImagePosition = setImagePosition;
    buttonWidgetVTable.getImageMargin = getImageMargin;
    buttonWidgetVTable.setImageMargin = setImageMargin;
    buttonWidgetVTable.getPressedOffset = getPressedOffset;
    buttonWidgetVTable.setPressedOffset = setPressedOffset;
    buttonWidgetVTable.getPressedEventCallback = getPressedEventCallback;
    buttonWidgetVTable.setPressedEventCallback = setPressedEventCallback;
    buttonWidgetVTable.getReleasedEventCallback = getReleasedEventCallback;
    buttonWidgetVTable.setReleasedEventCallback = setReleasedEventCallback;
}

void _leButtonWidget_FillVTable(leButtonWidgetVTable* tbl)
{
    *tbl = buttonWidgetVTable;
}
#else
static const leButtonWidgetVTable buttonWidgetVTable =
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
    ._destructor = _leButtonWidget_Destructor,
    .languageChangeEvent = languageChanging,
    ._paint = _leButtonWidget_Paint,
    .invalidateContents = invalidateContents,
    ._invalidateBorderAreas = _leButtonWidget_InvalidateBorderAreas,
    .touchDownEvent = touchDown,
    .touchUpEvent = touchUp,
    .touchMoveEvent = touchMoved,

    /* member functions */
    .getToggleable = getToggleable,
    .setToggleable = setToggleable,
    .getPressed = getPressed,
    .setPressed = setPressed,
    .getString = getString,
    .setString = setString,
    .getPressedImage = getPressedImage,
    .setPressedImage = setPressedImage,
    .getReleasedImage = getReleasedImage,
    .setReleasedImage = setReleasedImage,
    .getImagePosition = getImagePosition,
    .setImagePosition = setImagePosition,
    .getImageMargin = getImageMargin,
    .setImageMargin = setImageMargin,
    .getPressedOffset = getPressedOffset,
    .setPressedOffset = setPressedOffset,
    .getPressedEventCallback = getPressedEventCallback,
    .setPressedEventCallback = setPressedEventCallback,
    .getReleasedEventCallback = getReleasedEventCallback,
    .setReleasedEventCallback = setReleasedEventCallback,
};
#endif

#endif // LE_BUTTON_WIDGET_ENABLED
