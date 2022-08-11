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


#include "gfx/legato/widget/image/legato_widget_image.h"

#if LE_DEBUG == 1
#include "gfx/legato/core/legato_debug.h"
#endif

#if LE_IMAGE_WIDGET_ENABLED

#include <string.h>

#include "gfx/legato/image/legato_image.h"
#include "gfx/legato/string/legato_string.h"
#include "gfx/legato/common/legato_utils.h"
#include "gfx/legato/memory/legato_memory.h"
#include "gfx/legato/renderer/legato_renderer.h"
#include "gfx/legato/widget/legato_widget.h"

static
#if LE_DYNAMIC_VTABLES == 0
const
#endif
leImageWidgetVTable imageWidgetVTable;

#define DEFAULT_WIDTH           100
#define DEFAULT_HEIGHT          100

void _leImageWidget_GetImageRect(const leImageWidget* img,
                                 leRect* imgRect,
                                 leRect* imgSrcRect);

static void invalidateImageRect(const leImageWidget* _this)
{
    leRect rect, imgRect, clipRect;
    
    _leImageWidget_GetImageRect(_this, &imgRect, &clipRect);
    
    _this->fn->localRect(_this, &rect);
    
    leRectClip(&rect, &imgRect, &clipRect);
    
    leUtils_RectToScreenSpace((leWidget*)_this, &clipRect);
    
    _this->fn->_damageArea(_this, &clipRect);
}

/* base class constructor and destructor */
void _leWidget_Destructor(leWidget* _this);

void _leImageWidget_Constructor(leImageWidget* _this)
{
    leWidget_Constructor((leWidget*)_this);
    
    _this->widget.fn = (void*)&imageWidgetVTable;
    _this->fn = &imageWidgetVTable;
    
    _this->widget.type = LE_WIDGET_IMAGE;

    _this->widget.rect.width = DEFAULT_WIDTH;
    _this->widget.rect.height = DEFAULT_HEIGHT;

    _this->widget.style.borderType = LE_WIDGET_BORDER_NONE;
    _this->widget.style.backgroundType = LE_WIDGET_BACKGROUND_FILL;

    _this->image = NULL;
}

void _leImageWidget_Destructor(leImageWidget* _this)
{    
    _leWidget_Destructor((leWidget*)_this);
}

leImageWidget* leImageWidget_New()
{
    leImageWidget* img = NULL;

    img = LE_MALLOC(sizeof(leImageWidget));

    if(img == NULL)
        return NULL;
    
    _leImageWidget_Constructor(img);

    return img;
}

/*leResult leImageWidget_PNew(leImageWidget* img)
{
    if(img == NULL)
        return LE_FAILURE;
        
    memset(img, 0, sizeof(leImageWidget));
    
    _leImageWidget_Constructor(img);

    return LE_SUCCESS;
}*/

static void invalidateContents(const leImageWidget* _this)
{
    if(_this->image != NULL)
    {
        invalidateImageRect(_this);
    }
}

leImage* _leImageWidget_GetImage(const leImageWidget* _this)
{
    return (leImage*)_this->image;
}

leResult _leImageWidget_SetImage(leImageWidget* _this,
                                 leImage* imgAst)
{
    if(_this->image == imgAst)
        return LE_SUCCESS;
        
    if(_this->image != NULL)
        invalidateImageRect(_this);

    _this->image = imgAst;

    if(_this->image != NULL)
        invalidateImageRect(_this);

#if LE_DEBUG == 1
    _leDebugNotify_WidgetPropertyChanged((leWidget*)_this);
#endif

    return LE_SUCCESS;
}

void _leImageWidget_Paint(leImageWidget* _this);

#if LE_DYNAMIC_VTABLES == 1
void _leWidget_FillVTable(leWidgetVTable* tbl);

void _leImageWidget_GenerateVTable()
{
    _leWidget_FillVTable((void*)&imageWidgetVTable);
    
    /* overrides from base class */
    imageWidgetVTable._destructor = _leImageWidget_Destructor;
    imageWidgetVTable._paint = _leImageWidget_Paint;
    imageWidgetVTable.invalidateContents = invalidateContents;
    
    /* member functions */
    imageWidgetVTable.getImage = _leImageWidget_GetImage;
    imageWidgetVTable.setImage = _leImageWidget_SetImage;
}

void _leImageWidget_FillVTable(leImageWidgetVTable* tbl)
{
    *tbl = imageWidgetVTable;
}
#else
static const leImageWidgetVTable imageWidgetVTable =
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
    ._destructor = _leImageWidget_Destructor,
    ._paint = _leImageWidget_Paint,
    .invalidateContents = invalidateContents,

    /* member functions */
    .getImage = _leImageWidget_GetImage,
    .setImage = _leImageWidget_SetImage,
};
#endif

#endif // LE_IMAGE_WIDGET_ENABLED