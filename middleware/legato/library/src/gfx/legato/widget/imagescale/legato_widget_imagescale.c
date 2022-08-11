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


#include "gfx/legato/widget/imagescale/legato_widget_imagescale.h"

#if LE_IMAGESCALE_WIDGET_ENABLED

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
leImageScaleWidgetVTable imageScaleWidgetVTable;

void _leImageScaleWidget_GetImageRect(const leImageScaleWidget* img,
                                      leRect* imgRect,
                                      leRect* imgSrcRect);

static void invalidateImageRect(const leImageScaleWidget* _this)
{
    leRect imgRect, imgSrcRect;
    
    _leImageScaleWidget_GetImageRect(_this, &imgRect, &imgSrcRect);
    
    _this->fn->_damageArea(_this, &imgRect);
}

void leImageScaleWidget_Constructor(leImageScaleWidget* _this)
{
    leWidget_Constructor((leWidget*)_this);
    
    _this->widget.fn = (void*)&imageScaleWidgetVTable;
    _this->fn = &imageScaleWidgetVTable;
    
    _this->widget.type = LE_WIDGET_IMAGESCALE;

    _this->widget.rect.width = DEFAULT_WIDTH;
    _this->widget.rect.height = DEFAULT_HEIGHT;

    _this->widget.style.borderType = LE_WIDGET_BORDER_NONE;
    _this->widget.style.backgroundType = LE_WIDGET_BACKGROUND_FILL;
    
    _this->transformX = 0;
    _this->transformY = 0;
    _this->transformWidth = 00;
    _this->transformHeight = 0;
    
    _this->resizeToFit = LE_FALSE;
    _this->preserveAspect = LE_FALSE;
    _this->filter = LE_IMAGEFILTER_NEAREST_NEIGHBOR;
    _this->inputEnabled = LE_FALSE;
    
    _this->image = NULL;
}

void _leWidget_Destructor(leWidget* _this);

void _leImageScaleWidget_Destructor(leImageScaleWidget* _this)
{
    _leWidget_Destructor((leWidget*)_this);
}

leImageScaleWidget* leImageScaleWidget_New()
{
    leImageScaleWidget* img = NULL;

    img = LE_MALLOC(sizeof(leImageScaleWidget));

    if(img == NULL)
        return NULL;
    
    leImageScaleWidget_Constructor(img);

    return img;
}

static leImage* getImage(const leImageScaleWidget* _this)
{
    LE_ASSERT_THIS();

    return (leImage*)_this->image;
}

static leResult setImage(leImageScaleWidget* _this,
                         const leImage* imgAst)
{
    LE_ASSERT_THIS();

    _this->image = imgAst;

    _this->fn->invalidate(_this);

#if LE_DEBUG == 1
    _leDebugNotify_WidgetPropertyChanged((leWidget*)_this);
#endif

    return LE_SUCCESS;
}

static int32_t getTransformX(const leImageScaleWidget* _this)
{
    LE_ASSERT_THIS();

    return _this->transformX;
}

static leResult setTransformX(leImageScaleWidget* _this,
                              int32_t x)
{
    LE_ASSERT_THIS();
        
    if(_this->transformX == x)
        return LE_SUCCESS;

    if(_this->image != NULL)
    {
        invalidateImageRect(_this);
    }
        
    _this->transformX = x;

    if(_this->image != NULL)
    {
        invalidateImageRect(_this);
    }

#if LE_DEBUG == 1
    _leDebugNotify_WidgetPropertyChanged((leWidget*)_this);
#endif

    return LE_SUCCESS;
}

static int32_t getTransformY(const leImageScaleWidget* _this)
{
    LE_ASSERT_THIS();

    return _this->transformY;
}

static leResult setTransformY(leImageScaleWidget* _this,
                              int32_t y)
{
    LE_ASSERT_THIS();
        
    if(_this->transformY == y)
        return LE_SUCCESS;

    if(_this->image != NULL)
    {
        invalidateImageRect(_this);
    }
        
    _this->transformY = y;
    
    if(_this->image != NULL)
    {
        invalidateImageRect(_this);
    }

#if LE_DEBUG == 1
    _leDebugNotify_WidgetPropertyChanged((leWidget*)_this);
#endif

    return LE_SUCCESS;
}

static int32_t getTransformWidth(const leImageScaleWidget* _this)
{
    LE_ASSERT_THIS();

    return _this->transformWidth;
}

static leResult setTransformWidth(leImageScaleWidget* _this,
                                  int32_t width)
{
    LE_ASSERT_THIS();
        
    if(_this->transformWidth == width)
        return LE_SUCCESS;

    if(_this->image != NULL)
    {
        invalidateImageRect(_this);
    }
        
    _this->transformWidth = width;
    
    if(_this->image != NULL)
    {
        invalidateImageRect(_this);
    }

#if LE_DEBUG == 1
    _leDebugNotify_WidgetPropertyChanged((leWidget*)_this);
#endif

    return LE_SUCCESS;
}

static int32_t getTransformHeight(const leImageScaleWidget* _this)
{
    LE_ASSERT_THIS();

    return _this->transformHeight;
}

static leResult setTransformHeight(leImageScaleWidget* _this,
                                   int32_t height)
{
    LE_ASSERT_THIS();
        
    if(_this->transformHeight == height)
        return LE_SUCCESS;

    if(_this->image != NULL)
    {
        invalidateImageRect(_this);
    }
        
    _this->transformHeight = height;
    
    if(_this->image != NULL)
    {
        invalidateImageRect(_this);
    }

#if LE_DEBUG == 1
    _leDebugNotify_WidgetPropertyChanged((leWidget*)_this);
#endif

    return LE_SUCCESS;
}

static leResult resetTransform(leImageScaleWidget* _this)
{
    LE_ASSERT_THIS();
        
    if(_this->image != NULL)
    {
        invalidateImageRect(_this);
    }
       
    _this->transformX = 0;
    _this->transformY = 0;
    _this->transformWidth = 0;
    _this->transformHeight = 0;
    
    if(_this->image != NULL)
    {
        invalidateImageRect(_this);
    }

#if LE_DEBUG == 1
    _leDebugNotify_WidgetPropertyChanged((leWidget*)_this);
#endif

    return LE_SUCCESS;
}

static leBool getStretchEnabled(const leImageScaleWidget* _this)
{
    LE_ASSERT_THIS();
        
    return _this->resizeToFit;
}

static leResult setStretchEnabled(leImageScaleWidget* _this,
                                  leBool stretch)
{
    LE_ASSERT_THIS();
        
    if(_this->resizeToFit == stretch)
        return LE_SUCCESS;
        
    if(_this->image != NULL)
    {
        invalidateImageRect(_this);
    }
        
    _this->resizeToFit = stretch;
    
    if(_this->image != NULL)
    {
        invalidateImageRect(_this);
    }

#if LE_DEBUG == 1
    _leDebugNotify_WidgetPropertyChanged((leWidget*)_this);
#endif

    return LE_SUCCESS;
}

static leBool getPreserveAspectEnabled(const leImageScaleWidget* _this)
{
    LE_ASSERT_THIS();
        
    return _this->preserveAspect;
}

static leResult setPreserveAspectEnabled(leImageScaleWidget* _this,
                                         leBool preserve)
{
    LE_ASSERT_THIS();
        
    if(_this->preserveAspect == preserve)
        return LE_SUCCESS;
        
    if(_this->image != NULL)
    {
        invalidateImageRect(_this);
    }
        
    _this->preserveAspect = preserve;
    
    if(_this->image != NULL)
    {
        invalidateImageRect(_this);
    }

#if LE_DEBUG == 1
    _leDebugNotify_WidgetPropertyChanged((leWidget*)_this);
#endif

    return LE_SUCCESS;
}

static leImageFilterMode getFilter(const leImageScaleWidget* _this)
{
    LE_ASSERT_THIS();
        
    return _this->filter;
}

static leResult setFilter(leImageScaleWidget* _this,
                          leImageFilterMode filter)
{
    LE_ASSERT_THIS();
        
    if(_this->filter == filter)
        return LE_SUCCESS;
        
    if(_this->image != NULL)
    {
        invalidateImageRect(_this);
    }
        
    _this->filter = filter;
    
    if(_this->image != NULL)
    {
        invalidateImageRect(_this);
    }

#if LE_DEBUG == 1
    _leDebugNotify_WidgetPropertyChanged((leWidget*)_this);
#endif

    return LE_SUCCESS;
}

#if LE_DYNAMIC_VTABLES == 1
void _leWidget_FillVTable(leWidgetVTable* tbl);

void _leImageScaleWidget_GenerateVTable()
{
    _leWidget_FillVTable((void*)&imageScaleWidgetVTable);
    
    /* overrides from base class */
    imageScaleWidgetVTable._destructor = _leImageScaleWidget_Destructor;
    imageScaleWidgetVTable._paint = _leImageScaleWidget_Paint;
    imageScaleWidgetVTable.invalidateContents = invalidateImageRect;

    /* member functions */
    imageScaleWidgetVTable.getImage = getImage;
    imageScaleWidgetVTable.setImage = setImage;
    imageScaleWidgetVTable.getTransformX = getTransformX;
    imageScaleWidgetVTable.setTransformX = setTransformX;
    imageScaleWidgetVTable.getTransformY = getTransformY;
    imageScaleWidgetVTable.setTransformY = setTransformY;
    imageScaleWidgetVTable.getTransformWidth = getTransformWidth;
    imageScaleWidgetVTable.setTransformWidth = setTransformWidth;
    imageScaleWidgetVTable.getTransformHeight = getTransformHeight;
    imageScaleWidgetVTable.setTransformHeight = setTransformHeight;
    imageScaleWidgetVTable.resetTransform = resetTransform;
    imageScaleWidgetVTable.getStretchEnabled = getStretchEnabled;
    imageScaleWidgetVTable.setStretchEnabled = setStretchEnabled;
    imageScaleWidgetVTable.getPreserveAspectEnabled = getPreserveAspectEnabled;
    imageScaleWidgetVTable.setPreserveAspectEnabled = setPreserveAspectEnabled;
    imageScaleWidgetVTable.getFilter = getFilter;
    imageScaleWidgetVTable.setFilter = setFilter;
}

void _leImageScaleWidget_FillVTable(leImageScaleWidgetVTable* tbl)
{
    *tbl = imageScaleWidgetVTable;
}
#else
static const leImageScaleWidgetVTable imageScaleWidgetVTable =
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
    .languageChangeEvent = (void*)_leWidget_LanguageChangeEvent,

    ._handleEvent = (void*)_leWidget_HandleEvent,
    ._validateChildren = (void*)_leWidget_ValidateChildren,
    ._increaseDirtyState = (void*)_leWidget_IncreaseDirtyState,
    ._setDirtyState = (void*)_leWidget_SetDirtyState,
    ._clearDirtyState = (void*)_leWidget_ClearDirtyState,
    ._invalidateBorderAreas = (void*)_leWidget_InvalidateBorderAreas,
    ._damageArea = (void*)_leWidget_DamageArea,

    /* overrides from base class */
    ._destructor = _leImageScaleWidget_Destructor,
    ._paint = _leImageScaleWidget_Paint,
    .invalidateContents = invalidateImageRect,

    /* member functions */
    .getImage = getImage,
    .setImage = setImage,
    .getTransformX = getTransformX,
    .setTransformX = setTransformX,
    .getTransformY = getTransformY,
    .setTransformY = setTransformY,
    .getTransformWidth = getTransformWidth,
    .setTransformWidth = setTransformWidth,
    .getTransformHeight = getTransformHeight,
    .setTransformHeight = setTransformHeight,
    .resetTransform = resetTransform,
    .getStretchEnabled = getStretchEnabled,
    .setStretchEnabled = setStretchEnabled,
    .getPreserveAspectEnabled = getPreserveAspectEnabled,
    .setPreserveAspectEnabled = setPreserveAspectEnabled,
    .getFilter = getFilter,
    .setFilter = setFilter,
};
#endif

#endif // LE_IMAGESCALE_WIDGET_ENABLED