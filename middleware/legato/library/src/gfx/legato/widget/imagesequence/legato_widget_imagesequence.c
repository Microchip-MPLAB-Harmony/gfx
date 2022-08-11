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


#include "gfx/legato/widget/imagesequence/legato_widget_imagesequence.h"

#if LE_IMAGESEQUENCE_WIDGET_ENABLED

#include "gfx/legato/common/legato_error.h"
#include "gfx/legato/common/legato_utils.h"
#include "gfx/legato/memory/legato_memory.h"
#include "gfx/legato/widget/legato_widget.h"

#if LE_DEBUG == 1
#include "gfx/legato/core/legato_debug.h"
#endif

#define DEFAULT_WIDTH           100
#define DEFAULT_HEIGHT          100

#define DEFAULT_DELAY           1000

void _leImageSequenceWidget_GetImageRect(leImageSequenceWidget* img,
                                         leRect* imgRect,
                                         leRect* imgSrcRect);

static
#if LE_DYNAMIC_VTABLES == 0
const
#endif
leImageSequenceWidgetVTable imageSequenceWidgetVTable;

void leImageSequenceWidget_Constructor(leImageSequenceWidget* _this)
{
    leWidget_Constructor((leWidget*)_this);
    
    _this->widget.fn = (void*)&imageSequenceWidgetVTable;
    _this->fn = &imageSequenceWidgetVTable;
    
    _this->widget.type = LE_WIDGET_IMAGESEQUENCE;

    _this->widget.rect.width = DEFAULT_WIDTH;
    _this->widget.rect.height = DEFAULT_HEIGHT;

    _this->widget.style.borderType = LE_WIDGET_BORDER_NONE;
    _this->widget.style.backgroundType = LE_WIDGET_BACKGROUND_FILL;

    _this->count = 0;
    _this->images = NULL;
    _this->activeIdx = 0;
    
    _this->playing = LE_FALSE;
    _this->time = 0;
    _this->repeat = LE_FALSE;
    
    _this->cb = NULL;
}

void _leWidget_Destructor(leWidget* _this);

static void destructor(leImageSequenceWidget* _this)
{
    if(_this->images != NULL)
    {
        LE_FREE(_this->images);
        _this->images = NULL;
    }
    
    _leWidget_Destructor((leWidget*)_this);
}

static void update(leImageSequenceWidget* _this,
                   uint32_t dt)
{
    // no need to update
    if(_this->playing == LE_FALSE || _this->count <= 1)
        return;
        
    // initial position
    if(_this->activeIdx == LE_IMAGESEQ_RESTART)
    {
        _this->activeIdx = 0;
        _this->time = 0;
        
        _this->fn->invalidate(_this);
        
        //printf("invalidating\n");
    }
    // normal update
    else
    {
        _this->time += dt;
        
        // advance frame?
        if(_this->time >= _this->images[_this->activeIdx].delay)
        {
            _this->activeIdx++;
            _this->time = 0;
            
            // end of sequence?
            if(_this->activeIdx >= (int32_t)_this->count)
            {
                // repeat?
                if(_this->repeat == LE_TRUE)
                {
                    _this->activeIdx = 0;
                    
                    if(_this->cb != NULL)
                        _this->cb(_this);
                    
                    _this->fn->invalidate(_this);
                    
                    //printf("invalidating\n");
                }
                else
                {
                    // just stay on last image, no redraw necessary
                    _this->playing = LE_FALSE;
                }
            }
            else
            {
                if(_this->cb != NULL)
                    _this->cb(_this);
        
                _this->fn->invalidate(_this);
                
                //printf("invalidating\n");
            }
        }
    }
}

leImageSequenceWidget* leImageSequenceWidget_New(void)
{
    leImageSequenceWidget* img = LE_MALLOC(sizeof(leImageSequenceWidget));
    
    if(img == NULL)
        return NULL;

    leImageSequenceWidget_Constructor(img);

    return img;
}

static uint32_t getImageCount(const leImageSequenceWidget* _this)
{
    LE_ASSERT_THIS();
        
    return _this->count;
}

static leResult setImageCount(leImageSequenceWidget* _this,
                              uint32_t count)
{
    uint32_t i;
    
    LE_ASSERT_THIS();
        
    if(_this->count == count)
        return LE_SUCCESS;
        
    _this->images = LE_REALLOC(_this->images, sizeof(leImageSequenceEntry) * count);
                      
    if(count > _this->count)
    {
        for(i = _this->count; i < count; i++)
        {
            _this->images[i].image = NULL;
            _this->images[i].halign = LE_HALIGN_CENTER;
            _this->images[i].valign = LE_VALIGN_MIDDLE;
            _this->images[i].delay = DEFAULT_DELAY;
        }
    }
                                                
    _this->count = count;
    _this->activeIdx = 0; // just restart

#if LE_DEBUG == 1
    _leDebugNotify_WidgetPropertyChanged((leWidget*)_this);
#endif

    return LE_SUCCESS;
}

static leImage* getImage(const leImageSequenceWidget* _this,
                         uint32_t idx)
{
    LE_ASSERT_THIS();
    
    if(idx >= _this->count)
        return NULL;
        
    return (leImage*)_this->images[idx].image;
}

static leResult setImage(leImageSequenceWidget* _this,
                         uint32_t idx,
                         const leImage* imgAst)
{
    LE_ASSERT_THIS();
    
    if(idx >= _this->count)
        return LE_FAILURE;
        
    if(_this->images[idx].image == imgAst)
        return LE_SUCCESS;
        
    _this->images[idx].image = imgAst;
        
    if(_this->activeIdx == (int32_t)idx)
    {
        _this->fn->invalidate(_this);
    }

#if LE_DEBUG == 1
    _leDebugNotify_WidgetPropertyChanged((leWidget*)_this);
#endif

    return LE_SUCCESS;
}
                                              
static uint32_t getImageDelay(const leImageSequenceWidget* _this,
                              uint32_t idx)
{
    LE_ASSERT_THIS();
    
    if(idx >= _this->count)
        return 0;
        
    return _this->images[idx].delay;
}

static leResult setImageDelay(leImageSequenceWidget* _this,
                              uint32_t idx,
                              uint32_t delay)
{
    LE_ASSERT_THIS();
    
    if(idx >= _this->count)
        return LE_FAILURE;
        
    _this->images[idx].delay = delay;

#if LE_DEBUG == 1
    _leDebugNotify_WidgetPropertyChanged((leWidget*)_this);
#endif

    return LE_SUCCESS;
}

static leHAlignment getImageHAlignment(const leImageSequenceWidget* _this,
                                       uint32_t idx)
{
    LE_ASSERT_THIS();
  
    if(idx >= _this->count)
        return 0;
        
    return _this->images[idx].halign;
}

static leResult setImageHAlignment(leImageSequenceWidget* _this,
                                   uint32_t idx,
                                   leHAlignment align)
{
    LE_ASSERT_THIS();
    
    if(idx >= _this->count)
        return LE_FAILURE;
        
    if(_this->images[idx].halign == align)
        return LE_SUCCESS;
        
    _this->images[idx].halign = align;
        
    if(_this->activeIdx == (int32_t)idx)
    {
        _this->fn->invalidate(_this);
    }

#if LE_DEBUG == 1
    _leDebugNotify_WidgetPropertyChanged((leWidget*)_this);
#endif

    return LE_SUCCESS;
}

static leVAlignment getImageVAlignment(const leImageSequenceWidget* _this,
                                       uint32_t idx)
{
    LE_ASSERT_THIS();
 
    if(idx >= _this->count)
        return 0;
        
    return _this->images[idx].halign;
}

static leResult setImageVAlignment(leImageSequenceWidget* _this,
                                   uint32_t idx,
                                   leVAlignment align)
{
    LE_ASSERT_THIS();
    
    if(idx >= _this->count)
        return LE_FAILURE;
        
    if(_this->images[idx].valign == align)
        return LE_SUCCESS;
        
    _this->images[idx].valign = align;
        
    if(_this->activeIdx == (int32_t)idx)
    {
        _this->fn->invalidate(_this);
    }

#if LE_DEBUG == 1
    _leDebugNotify_WidgetPropertyChanged((leWidget*)_this);
#endif

    return LE_SUCCESS;
}

static leResult stop(leImageSequenceWidget* _this)
{
    LE_ASSERT_THIS();
        
    _this->playing = LE_FALSE;
    
    return LE_SUCCESS;
}

static leResult play(leImageSequenceWidget* _this)
{
    LE_ASSERT_THIS();
        
    _this->playing = LE_TRUE;
    
    return LE_SUCCESS;
}

static leResult rewind(leImageSequenceWidget* _this)
{
    LE_ASSERT_THIS();
        
    _this->activeIdx = LE_IMAGESEQ_RESTART;
    _this->time = 0;
    
    return LE_SUCCESS;
}

static leBool isPlaying(const leImageSequenceWidget* _this)
{
    LE_ASSERT_THIS();
        
    return _this->playing;
}

static leBool getRepeat(const leImageSequenceWidget* _this)
{
    LE_ASSERT_THIS();
        
    return _this->repeat;
}

static leResult setRepeat(leImageSequenceWidget* _this,
                          leBool repeat)
{
    LE_ASSERT_THIS();
        
    _this->repeat = repeat;

#if LE_DEBUG == 1
    _leDebugNotify_WidgetPropertyChanged((leWidget*)_this);
#endif

    return LE_SUCCESS;
}

static leResult showImage(leImageSequenceWidget* _this,
                          uint32_t idx)
{
    LE_ASSERT_THIS();
    
    if(idx >= _this->count)
        return LE_FAILURE;
        
    _this->activeIdx = idx;
    _this->time = 0;
    
    if(_this->cb != NULL)
    {
        _this->cb(_this);
    }
    
    _this->fn->invalidate(_this);

#if LE_DEBUG == 1
    _leDebugNotify_WidgetPropertyChanged((leWidget*)_this);
#endif

    return LE_SUCCESS;
}

static leResult showNextImage(leImageSequenceWidget* _this)
{
    LE_ASSERT_THIS();
        
    if(_this->activeIdx == (int32_t)_this->count - 1)
    {
        if(_this->repeat == LE_TRUE)
        {
            _this->fn->showImage(_this, 0);
        }
    }
    else
    {
        _this->fn->showImage(_this, _this->activeIdx + 1);
    }

#if LE_DEBUG == 1
    _leDebugNotify_WidgetPropertyChanged((leWidget*)_this);
#endif

    return LE_SUCCESS;
}

static leResult showPreviousImage(leImageSequenceWidget* _this)
{
    LE_ASSERT_THIS();
        
    if(_this->activeIdx == 0)
    {
        if(_this->repeat == LE_TRUE)
        {
            _this->fn->showImage(_this, _this->count - 1);
        }
    }
    else
    {
        _this->fn->showImage(_this, _this->activeIdx - 1);
    }

#if LE_DEBUG == 1
    _leDebugNotify_WidgetPropertyChanged((leWidget*)_this);
#endif

    return LE_SUCCESS;
}

static leImageSequenceImageChangedEvent_FnPtr getImageChangedEventCallback(const leImageSequenceWidget* _this)
{
    LE_ASSERT_THIS();
        
    return _this->cb;
}

static leResult setImageChangedEventCallback(leImageSequenceWidget* _this,
                                             leImageSequenceImageChangedEvent_FnPtr cb)
{
    LE_ASSERT_THIS();
        
    _this->cb = cb;
    
    return LE_SUCCESS;
}

void _leImageSequenceWidget_Paint(leImageSequenceWidget* _this);

#if LE_DYNAMIC_VTABLES == 1
void _leWidget_FillVTable(leWidgetVTable* tbl);

void _leImageSequenceWidget_GenerateVTable()
{
    _leWidget_FillVTable((void*)&imageSequenceWidgetVTable);
    
    /* overrides from base class */
    imageSequenceWidgetVTable._destructor = destructor;
    imageSequenceWidgetVTable.update = update;
    imageSequenceWidgetVTable._paint = _leImageSequenceWidget_Paint;
    
    /* member functions */
    imageSequenceWidgetVTable.getImageCount = getImageCount;
    imageSequenceWidgetVTable.setImageCount = setImageCount;
    imageSequenceWidgetVTable.getImage = getImage;
    imageSequenceWidgetVTable.setImage = setImage;
    imageSequenceWidgetVTable.getImageDelay = getImageDelay;
    imageSequenceWidgetVTable.setImageDelay = setImageDelay;
    imageSequenceWidgetVTable.getImageHAlignment = getImageHAlignment;
    imageSequenceWidgetVTable.setImageHAlignment = setImageHAlignment;
    imageSequenceWidgetVTable.getImageVAlignment = getImageVAlignment;
    imageSequenceWidgetVTable.setImageVAlignment = setImageVAlignment;
    imageSequenceWidgetVTable.stop = stop;
    imageSequenceWidgetVTable.play = play;
    imageSequenceWidgetVTable.rewind = rewind;
    imageSequenceWidgetVTable.isPlaying = isPlaying;
    imageSequenceWidgetVTable.getRepeat = getRepeat;
    imageSequenceWidgetVTable.setRepeat = setRepeat;
    imageSequenceWidgetVTable.showImage = showImage;
    imageSequenceWidgetVTable.showNextImage = showNextImage;
    imageSequenceWidgetVTable.showPreviousImage = showPreviousImage;
    imageSequenceWidgetVTable.getImageChangedEventCallback = getImageChangedEventCallback;
    imageSequenceWidgetVTable.setImageChangedEventCallback = setImageChangedEventCallback;
}

void _leImageSequenceWidget_FillVTable(leImageSequenceWidgetVTable* tbl)
{
    *tbl = imageSequenceWidgetVTable;
}
#else
static const leImageSequenceWidgetVTable imageSequenceWidgetVTable =
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
    ._destructor = destructor,
    .update = update,
    ._paint = _leImageSequenceWidget_Paint,

    /* member functions */
    .getImageCount = getImageCount,
    .setImageCount = setImageCount,
    .getImage = getImage,
    .setImage = setImage,
    .getImageDelay = getImageDelay,
    .setImageDelay = setImageDelay,
    .getImageHAlignment = getImageHAlignment,
    .setImageHAlignment = setImageHAlignment,
    .getImageVAlignment = getImageVAlignment,
    .setImageVAlignment = setImageVAlignment,
    .stop = stop,
    .play = play,
    .rewind = rewind,
    .isPlaying = isPlaying,
    .getRepeat = getRepeat,
    .setRepeat = setRepeat,
    .showImage = showImage,
    .showNextImage = showNextImage,
    .showPreviousImage = showPreviousImage,
    .getImageChangedEventCallback = getImageChangedEventCallback,
    .setImageChangedEventCallback = setImageChangedEventCallback,
};
#endif

#endif // LE_IMAGESEQUENCE_WIDGET_ENABLED
