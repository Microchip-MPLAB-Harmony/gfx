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
#include "gfx/legato/widget/radiobutton/legato_widget_radiobutton.h"

#if LE_DEBUG == 1
#include "gfx/legato/core/legato_debug.h"
#endif

#if LE_RADIOBUTTON_WIDGET_ENABLED == 1

#include "gfx/legato/common/legato_error.h"
#include "gfx/legato/common/legato_utils.h"
#include "gfx/legato/memory/legato_memory.h"
#include "gfx/legato/string/legato_string.h"
#include "gfx/legato/widget/legato_widget.h"
#include "gfx/legato/widget/radiobutton/legato_radiobutton_group.h"

#define DEFAULT_WIDTH           100
#define DEFAULT_HEIGHT          25

#define DEFAULT_IMAGE_MARGIN    10

#define DEFAULT_CIRCLE_SIZE     12

static
#if LE_DYNAMIC_VTABLES == 0
const
#endif
leRadioButtonWidgetVTable radioButtonWidgetVTable;

void _leRadioButtonWidget_Paint(leRadioButtonWidget* _this);

void _leRadioButtonWidget_GetImageRect(const leRadioButtonWidget* btn,
									   leRect* imgRect,
									   leRect* imgSrcRect);

void _leRadioButtonWidget_GetTextRects(const leRadioButtonWidget* btn,
								       leRect* boundingRect,
								       leRect* kerningRect);

static void _invalidateImageRect(const leRadioButtonWidget* _this)
{
    leRect imgRect, imgSrcRect;
    
    _leRadioButtonWidget_GetImageRect(_this, &imgRect, &imgSrcRect);
    
    _this->fn->_damageArea(_this, &imgRect);
}

static void _invalidateTextRect(const leRadioButtonWidget* _this)
{
    leRect boundingRect, kerningRect;
    
    _leRadioButtonWidget_GetTextRects(_this, &boundingRect, &kerningRect);
    
    _this->fn->_damageArea(_this, &boundingRect);
}

static void _invalidateContents(const leRadioButtonWidget* _this)
{
    if(_this->string != NULL)
    {
        _invalidateTextRect(_this);
    }
        
    _invalidateImageRect(_this);
}

static void stringPreinvalidate(const leString* str,
                                leRadioButtonWidget* btn)
{
    (void)str; // unused

    _invalidateTextRect(btn);
}

static void stringInvalidate(const leString* str,
                             leRadioButtonWidget* btn)
{
    (void)str; // unused

    _invalidateTextRect(btn);
}

static void handleLanguageChangeEvent(leRadioButtonWidget* _this)
{
    if(_this->string != NULL)
    {
        _invalidateContents(_this);
    }
}

void leRadioButtonWidget_Constructor(leRadioButtonWidget* _this)
{
    leWidget_Constructor((leWidget*)_this);
    
    _this->widget.fn = (void*)&radioButtonWidgetVTable;
    _this->fn = &radioButtonWidgetVTable;
    
    _this->widget.type = LE_WIDGET_RADIOBUTTON;

    _this->widget.rect.width = DEFAULT_WIDTH;
    _this->widget.rect.height = DEFAULT_HEIGHT;

    _this->selected = LE_TRUE;

    _this->widget.style.borderType = LE_WIDGET_BORDER_NONE;

    _this->string = NULL;

    _this->imagePosition = LE_RELATIVE_POSITION_LEFTOF;
    _this->imageMargin = DEFAULT_IMAGE_MARGIN;

    _this->widget.style.halign = LE_HALIGN_LEFT;
    _this->widget.style.valign = LE_VALIGN_MIDDLE;
    
    _this->selectedImage = NULL;
    _this->unselectedImage = NULL;
    _this->circleButtonSize = DEFAULT_CIRCLE_SIZE;

    _this->selectedEvent = NULL;
    _this->deselectedEvent = NULL;

    _this->group = NULL;
}

void _leWidget_Destructor(leWidget* wgt);

static void destructor(leRadioButtonWidget* _this)
{
    if(_this->group != NULL)
    {
        leRadioButtonGroup_RemoveButton(_this->group, _this);   
    }
    
    _this->string = NULL;
    
    _leWidget_Destructor((leWidget*)_this);
}

/*leWidgetUpdateState _leRadioButtonWidget_Update(leRadioButtonWidget* _this)
{

}*/

leRadioButtonWidget* leRadioButtonWidget_New()
{
    leRadioButtonWidget* btn = NULL;

    btn = LE_MALLOC(sizeof(leRadioButtonWidget));

    if(btn == NULL)
        return btn;
        
    leRadioButtonWidget_Constructor(btn);

    return btn;
}

static leRadioButtonGroup* getGroup(const leRadioButtonWidget* _this)
{
    LE_ASSERT_THIS();
        
    return _this->group;
}

static leBool getSelected(const leRadioButtonWidget* _this)
{
    LE_ASSERT_THIS();

    if(_this->group == NULL)
        return _this->selected;

    return _this->group->selected == _this;
}

static leResult setSelected(leRadioButtonWidget* _this)
{
    leResult res;
    
    LE_ASSERT_THIS();
    
    if(_this->selected == LE_TRUE)
        return LE_FAILURE;

    if(_this->group == NULL)
        return LE_SUCCESS;
   
    _invalidateContents(_this);
    
    res = leRadioButtonGroup_SelectButton(_this->group, _this);
    
    if(res == LE_SUCCESS)
    {
        _invalidateContents(_this);
    }

#if LE_DEBUG == 1
    _leDebugNotify_WidgetPropertyChanged((leWidget*)_this);
#endif

    return res;
}

static void selected(leRadioButtonWidget* _this)
{
    LE_ASSERT_THIS();
    
    if(_this->selected == LE_TRUE)
        return;
    
    _invalidateContents(_this);

    _this->selected = LE_TRUE;

    if(_this->selectedEvent != NULL)
    {
        _this->selectedEvent(_this);
    }
        
    _invalidateContents(_this);
}

static void deselected(leRadioButtonWidget* _this)
{
    LE_ASSERT_THIS();
    
    if(_this->selected == LE_FALSE)
        return;

    _invalidateContents(_this);
    
    _this->selected = LE_FALSE;

    if(_this->deselectedEvent != NULL)
    {
        _this->deselectedEvent(_this);
    }
        
    _invalidateContents(_this);
}

static leString* getString(const leRadioButtonWidget* _this)
{
    LE_ASSERT_THIS();
    
    return (leString*)_this->string;
}

static leResult setString(leRadioButtonWidget* _this,
                          const leString* str)
{
    LE_ASSERT_THIS();

    if(_this->string != NULL)
    {
        _invalidateContents(_this);

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

    _invalidateContents(_this);

#if LE_DEBUG == 1
    _leDebugNotify_WidgetPropertyChanged((leWidget*)_this);
#endif

    return LE_SUCCESS;
}

static leImage* getSelectedImage(const leRadioButtonWidget* _this)
{
    LE_ASSERT_THIS();

    return (leImage*)_this->selectedImage;
}

static leResult setSelectedImage(leRadioButtonWidget* _this,
                                 const leImage* img)
{
    LE_ASSERT_THIS();

    if(_this->selectedImage == img)
        return LE_SUCCESS;

    _invalidateContents(_this);

    _this->selectedImage = img;

    _invalidateContents(_this);

#if LE_DEBUG == 1
    _leDebugNotify_WidgetPropertyChanged((leWidget*)_this);
#endif

    return LE_SUCCESS;
}

static leImage* getUnselectedImage(const leRadioButtonWidget* _this)
{
    LE_ASSERT_THIS();

    return (leImage*)_this->unselectedImage;
}

static leResult setUnselectedImage(leRadioButtonWidget* _this,
                                   const leImage* img)
{
    LE_ASSERT_THIS();
        
    if(_this->unselectedImage == img)
        return LE_SUCCESS;
        
    _invalidateContents(_this);

    _this->unselectedImage = img;

    _invalidateContents(_this);

#if LE_DEBUG == 1
    _leDebugNotify_WidgetPropertyChanged((leWidget*)_this);
#endif

    return LE_SUCCESS;
}

static leRelativePosition getImagePosition(const leRadioButtonWidget* _this)
{
    LE_ASSERT_THIS();
        
    return _this->imagePosition;
}

static leResult setImagePosition(leRadioButtonWidget* _this,
                                 leRelativePosition pos)
{
    LE_ASSERT_THIS();
    
    if(_this->imagePosition == pos)
        return LE_SUCCESS;
    
    _invalidateContents(_this);
    
    _this->imagePosition = pos;
    
    _invalidateContents(_this);

#if LE_DEBUG == 1
    _leDebugNotify_WidgetPropertyChanged((leWidget*)_this);
#endif

    return LE_SUCCESS;
}

static uint32_t getImageMargin(const leRadioButtonWidget* _this)
{
    LE_ASSERT_THIS();
        
    return _this->imageMargin;
}

static leResult setImageMargin(leRadioButtonWidget* _this,
                               uint32_t mg)
{
    LE_ASSERT_THIS();
    
    if(_this->imageMargin == mg)
        return LE_SUCCESS;
        
    _invalidateContents(_this);
    
    _this->imageMargin = mg;
    
    _invalidateContents(_this);

#if LE_DEBUG == 1
    _leDebugNotify_WidgetPropertyChanged((leWidget*)_this);
#endif

    return LE_SUCCESS;
}

static leRadioButtonWidget_SelectedEvent getSelectedEventCallback(const leRadioButtonWidget* _this)
{
    LE_ASSERT_THIS();

    return _this->selectedEvent;
}

static leResult setSelectedEventCallback(leRadioButtonWidget* _this,
                                         leRadioButtonWidget_SelectedEvent cb)
{
    LE_ASSERT_THIS();

    if(_this->selectedEvent == cb)
        return LE_FAILURE;

    _this->selectedEvent = cb;
    
    return LE_SUCCESS;
}

static leRadioButtonWidget_DeselectedEvent getDeselectedEventCallback(const leRadioButtonWidget* _this)
{
    LE_ASSERT_THIS();

    return _this->deselectedEvent;
}

static leResult setDeselectedEventCallback(leRadioButtonWidget* _this,
                                           leRadioButtonWidget_DeselectedEvent cb)
{
    LE_ASSERT_THIS();
    
    if(_this->deselectedEvent == cb)
        return LE_FAILURE;

    _this->deselectedEvent = cb;
    
    return LE_SUCCESS;
}

static uint32_t getCircleButtonSize(const leRadioButtonWidget* _this)
{
    LE_ASSERT_THIS();
    
    return _this->circleButtonSize;
}

static leResult setCircleButtonSize(leRadioButtonWidget* _this,
                                    uint32_t size)
{
    LE_ASSERT_THIS();

    if(size < DEFAULT_CIRCLE_SIZE)
        return LE_FAILURE;
    
    if((int32_t)size > (int32_t)_this->widget.rect.height)
        return LE_FAILURE;
    
    if((int32_t)size > (int32_t)_this->widget.rect.width)
        return LE_FAILURE;

    if((int32_t)size == (int32_t)_this->circleButtonSize)
        return LE_SUCCESS;
    
    _this->circleButtonSize = size;
    
    _this->fn->invalidate(_this);

#if LE_DEBUG == 1
    _leDebugNotify_WidgetPropertyChanged((leWidget*)_this);
#endif

    return LE_SUCCESS;
}

static void handleTouchDownEvent(leRadioButtonWidget* _this,
                                 leWidgetEvent_TouchDown* evt)
{
    leRect imgRect, imgSrcRect, widgetRect, clipRect;
    lePoint pt;
        
    LE_ASSERT_THIS();
    
    _leRadioButtonWidget_GetImageRect(_this, &imgRect, &imgSrcRect);
    
    _this->fn->rectToScreen(_this, &widgetRect);
    
    leRectClip(&imgRect, &widgetRect, &clipRect);
    
    pt.x = evt->x;
    pt.y = evt->y;
    
    if(leRectContainsPoint(&clipRect, &pt) == LE_TRUE &&
       _this->selected == LE_FALSE)
    {
        leWidgetEvent_Accept((leWidgetEvent*)evt, (leWidget*)_this);
    }
}

static void handleTouchUpEvent(leRadioButtonWidget* _this,
                               leWidgetEvent_TouchUp* evt)
{
    leRect imgRect, imgSrcRect, widgetRect, clipRect;
    lePoint pt;
    
    LE_ASSERT_THIS();
    
    _leRadioButtonWidget_GetImageRect(_this, &imgRect, &imgSrcRect);
    
    _this->fn->rectToScreen(_this, &widgetRect);
    
    leRectClip(&imgRect, &widgetRect, &clipRect);

    pt.x = evt->x;
    pt.y = evt->y;

    leWidgetEvent_Accept((leWidgetEvent*)evt, (leWidget*)_this);

    if(leRectContainsPoint(&clipRect, &pt) == LE_TRUE)
    {
        leRadioButtonGroup_SelectButton(_this->group, _this);
    }
    
    //printf("cbox touch up\n");
}

static void handleTouchMovedEvent(leRadioButtonWidget* _this,
                                  leWidgetEvent_TouchMove* evt)
{
    LE_ASSERT_THIS();

    leWidgetEvent_Accept((leWidgetEvent*)evt, (leWidget*)_this);
}

#if LE_DYNAMIC_VTABLES == 1
void _leWidget_FillVTable(leWidgetVTable* tbl);

void _leRadioButtonWidget_GenerateVTable()
{
    _leWidget_FillVTable((void*)&radioButtonWidgetVTable);
    
    /* overrides from base class */
    radioButtonWidgetVTable._destructor = destructor;
    radioButtonWidgetVTable._paint = _leRadioButtonWidget_Paint;
    radioButtonWidgetVTable.invalidateContents = _invalidateContents;
    radioButtonWidgetVTable.languageChangeEvent = handleLanguageChangeEvent;
    radioButtonWidgetVTable.touchDownEvent = handleTouchDownEvent;
    radioButtonWidgetVTable.touchUpEvent = handleTouchUpEvent;
    radioButtonWidgetVTable.touchMoveEvent = handleTouchMovedEvent;
    
    /* member functions */
    radioButtonWidgetVTable.getGroup = getGroup;
    radioButtonWidgetVTable.getSelected = getSelected;
    radioButtonWidgetVTable.setSelected = setSelected;
    radioButtonWidgetVTable.select = selected;
    radioButtonWidgetVTable.deselect = deselected;
    radioButtonWidgetVTable.getString = getString;
    radioButtonWidgetVTable.setString = setString;
    radioButtonWidgetVTable.getSelectedImage = getSelectedImage;
    radioButtonWidgetVTable.setSelectedImage = setSelectedImage;
    radioButtonWidgetVTable.getUnselectedImage = getUnselectedImage;
    radioButtonWidgetVTable.setUnselectedImage = setUnselectedImage;
    radioButtonWidgetVTable.getImagePosition = getImagePosition;
    radioButtonWidgetVTable.setImagePosition = setImagePosition;
    radioButtonWidgetVTable.getImageMargin = getImageMargin;
    radioButtonWidgetVTable.setImageMargin = setImageMargin;
    radioButtonWidgetVTable.getSelectedEventCallback = getSelectedEventCallback;
    radioButtonWidgetVTable.setSelectedEventCallback = setSelectedEventCallback;
    radioButtonWidgetVTable.getDeselectedEventCallback = getDeselectedEventCallback;
    radioButtonWidgetVTable.setDeselectedEventCallback = setDeselectedEventCallback;
    radioButtonWidgetVTable.getCircleButtonSize = getCircleButtonSize;
    radioButtonWidgetVTable.setCircleButtonSize = setCircleButtonSize;
}

void _leRadioButtonWidget_FillVTable(leRadioButtonWidgetVTable* tbl)
{
    *tbl = radioButtonWidgetVTable;
}
#else
static const leRadioButtonWidgetVTable radioButtonWidgetVTable =
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
    ._invalidateBorderAreas = (void*)_leWidget_InvalidateBorderAreas,
    ._damageArea = (void*)_leWidget_DamageArea,

    /* overrides from base class */
    ._destructor = destructor,
    ._paint = _leRadioButtonWidget_Paint,
    .invalidateContents = _invalidateContents,
    .languageChangeEvent = handleLanguageChangeEvent,
    .touchDownEvent = handleTouchDownEvent,
    .touchUpEvent = handleTouchUpEvent,
    .touchMoveEvent = handleTouchMovedEvent,

    /* member functions */
    .getGroup = getGroup,
    .getSelected = getSelected,
    .setSelected = setSelected,
    .select = selected,
    .deselect = deselected,
    .getString = getString,
    .setString = setString,
    .getSelectedImage = getSelectedImage,
    .setSelectedImage = setSelectedImage,
    .getUnselectedImage = getUnselectedImage,
    .setUnselectedImage = setUnselectedImage,
    .getImagePosition = getImagePosition,
    .setImagePosition = setImagePosition,
    .getImageMargin = getImageMargin,
    .setImageMargin = setImageMargin,
    .getSelectedEventCallback = getSelectedEventCallback,
    .setSelectedEventCallback = setSelectedEventCallback,
    .getDeselectedEventCallback = getDeselectedEventCallback,
    .setDeselectedEventCallback = setDeselectedEventCallback,
    .getCircleButtonSize = getCircleButtonSize,
    .setCircleButtonSize = setCircleButtonSize,
};
#endif

#endif // LE_RADIOBUTTON_WIDGET_ENABLED