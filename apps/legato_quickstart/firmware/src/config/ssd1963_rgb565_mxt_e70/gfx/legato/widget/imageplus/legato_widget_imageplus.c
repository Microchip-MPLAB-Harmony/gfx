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


#include "gfx/legato/widget/imageplus/legato_widget_imageplus.h"

#if LE_IMAGEPLUS_WIDGET_ENABLED

#include "gfx/legato/common/legato_error.h"
#include "gfx/legato/common/legato_utils.h"
#include "gfx/legato/memory/legato_memory.h"
#include "gfx/legato/string/legato_string.h"
#include "gfx/legato/widget/legato_widget.h"

#define DEFAULT_WIDTH           100
#define DEFAULT_HEIGHT          100

static leImagePlusWidgetVTable imagePlusWidgetVTable;

void _leImagePlusWidget_GetImageRect(const leImagePlusWidget* img,
                                     leRect* imgRect,
                                     leRect* imgSrcRect);

static void invalidateImageRect(const leImagePlusWidget* _this)
{
    leRect imgRect, imgSrcRect;
    
    _leImagePlusWidget_GetImageRect(_this, &imgRect, &imgSrcRect);
    
    _this->fn->_damageArea(_this, &imgRect);
}

void leImagePlusWidget_Constructor(leImagePlusWidget* _this)
{
    leWidget_Constructor((leWidget*)_this);
    
    _this->widget.fn = (void*)&imagePlusWidgetVTable;
    _this->fn = &imagePlusWidgetVTable;
    
    _this->widget.type = LE_WIDGET_IMAGEPLUS;

    _this->widget.rect.width = DEFAULT_WIDTH;
    _this->widget.rect.height = DEFAULT_HEIGHT;

    _this->widget.borderType = LE_WIDGET_BORDER_NONE;
    _this->widget.backgroundType = LE_WIDGET_BACKGROUND_FILL;
    
    _this->transformX = 0;
    _this->transformY = 0;
    _this->transformWidth = 00;
    _this->transformHeight = 0;
    
    _this->resizeToFit = LE_FALSE;
    _this->preserveAspect = LE_FALSE;
    _this->resizeFilter = LE_IMAGEFILTER_NEARESTNEIGHBOR;
    _this->inputEnabled = LE_FALSE;
    
    _this->image = NULL;
    
    _this->touch0.x = 0;
    _this->touch0.y = 0;
    _this->touch0_down = LE_FALSE;
    
    _this->touch1.x = 0;
    _this->touch1.y = 0;
    _this->touch1_down = LE_FALSE;
}

void _leWidget_Destructor(leWidget* _this);

void _leImagePlusWidget_Destructor(leImagePlusWidget* _this)
{
    _leWidget_Destructor((leWidget*)_this);
}

leImagePlusWidget* leImagePlusWidget_New()
{
    leImagePlusWidget* img = NULL;

    img = LE_MALLOC(sizeof(leImagePlusWidget));

    if(img == NULL)
        return NULL;
    
    leImagePlusWidget_Constructor(img);

    return img;
}

static leImage* getImage(const leImagePlusWidget* _this)
{
    LE_ASSERT_THIS();

    return (leImage*)_this->image;
}

static leResult setImage(leImagePlusWidget* _this,
                         const leImage* imgAst)
{
    LE_ASSERT_THIS();

    _this->image = imgAst;

    _this->fn->invalidate(_this);
    
    return LE_SUCCESS;
}

static int32_t getTransformX(const leImagePlusWidget* _this)
{
    LE_ASSERT_THIS();

    return _this->transformX;
}

static leResult setTransformX(leImagePlusWidget* _this,
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
        
    return LE_SUCCESS;
}

static int32_t getTransformY(const leImagePlusWidget* _this)
{
    LE_ASSERT_THIS();

    return _this->transformY;
}

static leResult setTransformY(leImagePlusWidget* _this,
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
        
    return LE_SUCCESS;
}

static int32_t getTransformWidth(const leImagePlusWidget* _this)
{
    LE_ASSERT_THIS();

    return _this->transformWidth;
}

static leResult setTransformWidth(leImagePlusWidget* _this,
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
        
    return LE_SUCCESS;
}

static int32_t getTransformHeight(const leImagePlusWidget* _this)
{
    LE_ASSERT_THIS();

    return _this->transformHeight;
}

static leResult setTransformHeight(leImagePlusWidget* _this,
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
        
    return LE_SUCCESS;
}

static leResult resetTransform(leImagePlusWidget* _this)
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
        
    return LE_SUCCESS;
}

static leBool getStretchEnabled(const leImagePlusWidget* _this)
{
    LE_ASSERT_THIS();
        
    return _this->resizeToFit;
}

static leResult setStretchEnabled(leImagePlusWidget* _this,
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
        
    return LE_SUCCESS;
}

static leBool getPreserveAspectEnabled(const leImagePlusWidget* _this)
{
    LE_ASSERT_THIS();
        
    return _this->preserveAspect;
}

static leResult setPreserveAspectEnabled(leImagePlusWidget* _this,
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
        
    return LE_SUCCESS;
}

static leImagePlusWidget_ResizeFilter getResizeFilter(const leImagePlusWidget* _this)
{
    LE_ASSERT_THIS();
        
    return _this->resizeFilter;
}

static leResult setResizeFilter(leImagePlusWidget* _this,
                                leImagePlusWidget_ResizeFilter filter)
{
    LE_ASSERT_THIS();
        
    if(_this->resizeFilter == filter)
        return LE_SUCCESS;
        
    if(_this->image != NULL)
    {
        invalidateImageRect(_this);
    }
        
    _this->resizeFilter = filter;
    
    if(_this->image != NULL)
    {
        invalidateImageRect(_this);
    }
        
    return LE_SUCCESS;
}

static leBool getInteractive(const leImagePlusWidget* _this)
{
    LE_ASSERT_THIS();
        
    return _this->inputEnabled;
}

static leResult setInteractive(leImagePlusWidget* _this,
                               leBool interactive)
{
    LE_ASSERT_THIS();
        
    if(_this->inputEnabled == interactive)
        return LE_SUCCESS;
        
    _this->inputEnabled = interactive;
        
    return LE_SUCCESS;
}

static void handleTouchDownEvent(leImagePlusWidget* _this,
                                 leWidgetEvent_TouchDown* evt)
{    
    LE_ASSERT_THIS();
    
    if(_this->inputEnabled == LE_FALSE)
        return;

    leWidgetEvent_Accept((leWidgetEvent*)evt, (leWidget*)_this);
    
    if(evt->touchID == 0)
    {
        _this->touch0.x = evt->x;
        _this->touch0.y = evt->y;
        _this->touch0_down = LE_TRUE;
    }
    else if(evt->touchID == 1)
    {
        _this->touch1.x = evt->x;
        _this->touch1.y = evt->y;
        _this->touch1_down = LE_TRUE;
    }

    //printf("_this touch down, evt->touchID\n");
}

static void handleTouchUpEvent(leImagePlusWidget* _this,
                               leWidgetEvent_TouchUp* evt)
{
    LE_ASSERT_THIS();
    
    if(_this->inputEnabled == LE_FALSE)
        return;

    leWidgetEvent_Accept((leWidgetEvent*)evt, (leWidget*)_this);
    
    if(evt->touchID == 0)
    {
        _this->touch0_down = LE_FALSE;
        _this->touch1_down = LE_FALSE;
    }
    else if(evt->touchID == 1)
    {
        _this->touch1_down = LE_FALSE;
    }

    //printf("btn touch up %i\n", evt->touchID);
}

static void handleTouchMovedEvent(leImagePlusWidget* _this,
                                  leWidgetEvent_TouchMove* evt)
{    
    LE_ASSERT_THIS();
    
    if(_this->inputEnabled == LE_FALSE)
        return;

    leWidgetEvent_Accept((leWidgetEvent*)evt, (leWidget*)_this);
    
    if(_this->image != NULL)
    {
        invalidateImageRect(_this);
    }
    
    switch(evt->touchID)
    {
        case 0:
        {
            // image translate
            if(_this->touch1_down == LE_FALSE)
            {
                _this->transformX += evt->x - _this->touch0.x;
                _this->transformY += evt->y - _this->touch0.y;
            }
            else
            {
                _this->transformWidth -= evt->x - _this->touch0.x;
                _this->transformHeight -= evt->y - _this->touch0.y;
            }
            
            _this->touch0.x = evt->x;
            _this->touch0.y = evt->y;
            
            break;
        }
        case 1:
        {
            _this->transformWidth += evt->x - _this->touch1.x;
            _this->transformHeight += evt->y - _this->touch1.y;
                
            _this->touch1.x = evt->x;
            _this->touch1.y = evt->y;
            
            break;
        }
    }
    
    if(_this->image != NULL)
    {
        invalidateImageRect(_this);
    }
}

void _leWidget_FillVTable(leWidgetVTable* tbl);
void _leImagePlusWidget_Paint(leImagePlusWidget* _this);

void _leImagePlusWidget_GenerateVTable()
{
    _leWidget_FillVTable((void*)&imagePlusWidgetVTable);
    
    /* overrides from base class */
    imagePlusWidgetVTable._destructor = _leImagePlusWidget_Destructor;
    imagePlusWidgetVTable._paint = _leImagePlusWidget_Paint;
    imagePlusWidgetVTable.invalidateContents = invalidateImageRect;
    imagePlusWidgetVTable.touchDownEvent = handleTouchDownEvent;
    imagePlusWidgetVTable.touchUpEvent = handleTouchUpEvent;
    imagePlusWidgetVTable.touchMoveEvent = handleTouchMovedEvent;
    
    /* member functions */
    imagePlusWidgetVTable.getImage = getImage;
    imagePlusWidgetVTable.setImage = setImage;
    imagePlusWidgetVTable.getTransformX = getTransformX;
    imagePlusWidgetVTable.setTransformX = setTransformX;
    imagePlusWidgetVTable.getTransformY = getTransformY;
    imagePlusWidgetVTable.setTransformY = setTransformY;
    imagePlusWidgetVTable.getTransformWidth = getTransformWidth;
    imagePlusWidgetVTable.setTransformWidth = setTransformWidth;
    imagePlusWidgetVTable.getTransformHeight = getTransformHeight;
    imagePlusWidgetVTable.setTransformHeight = setTransformHeight;
    imagePlusWidgetVTable.resetTransform = resetTransform;
    imagePlusWidgetVTable.getStretchEnabled = getStretchEnabled;
    imagePlusWidgetVTable.setStretchEnabled = setStretchEnabled;
    imagePlusWidgetVTable.getPreserveAspectEnabled = getPreserveAspectEnabled;
    imagePlusWidgetVTable.setPreserveAspectEnabled = setPreserveAspectEnabled;
    imagePlusWidgetVTable.getResizeFilter = getResizeFilter;
    imagePlusWidgetVTable.setResizeFilter = setResizeFilter;
    imagePlusWidgetVTable.getInteractive = getInteractive;
    imagePlusWidgetVTable.setInteractive = setInteractive;
}

void _leImagePlusWidget_FillVTable(leImagePlusWidgetVTable* tbl)
{
    *tbl = imagePlusWidgetVTable;
}


#endif // LE_IMAGEPLUS_WIDGET_ENABLED