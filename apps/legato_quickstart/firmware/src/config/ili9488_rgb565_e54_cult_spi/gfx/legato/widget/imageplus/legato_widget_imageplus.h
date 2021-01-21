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

/*******************************************************************************
 Module for Microchip Graphics Library - Legato User Interface Library

  Company:
    Microchip Technology Inc.

  File Name:
    legato_widget_image.h

  Summary:


  Description:
    This module implements image widget functions.
*******************************************************************************/

// DOM-IGNORE-BEGIN

#ifndef LEGATO_IMAGEPLUS_H
#define LEGATO_IMAGEPLUS_H
//DOM-IGNORE-END

#include "gfx/legato/common/legato_common.h"

#if LE_IMAGEPLUS_WIDGET_ENABLED

#include "gfx/legato/image/legato_image.h"
#include "gfx/legato/widget/legato_widget.h"

typedef struct leImagePlusWidget leImagePlusWidget;

// *****************************************************************************
// *****************************************************************************
// Section: Data Types and Constants
// *****************************************************************************
// *****************************************************************************

typedef enum leImagePlusWidget_ResizeFilter
{
    LE_IMAGEFILTER_NEARESTNEIGHBOR = 0x0,
    LE_IMAGEFILTER_BILINEAR
} leImagePlusWidget_ResizeFilter;

// DOM-IGNORE-BEGIN
typedef struct leImagePlusWidget leImagePlusWidget;

#define LE_IMAGEPLUSWIDGET_VTABLE(THIS_TYPE) \
    LE_WIDGET_VTABLE(THIS_TYPE) \
    \
    leImage*         (*getImage)(const THIS_TYPE* _this); \
    leResult         (*setImage)(THIS_TYPE* _this, const leImage* img); \
    int32_t          (*getTransformX)(const THIS_TYPE* _this); \
    leResult         (*setTransformX)(THIS_TYPE* _this, int32_t x); \
    int32_t          (*getTransformY)(const THIS_TYPE* _this); \
    leResult         (*setTransformY)(THIS_TYPE* _this, int32_t y); \
    int32_t          (*getTransformWidth)(const THIS_TYPE* _this); \
    leResult         (*setTransformWidth)(THIS_TYPE* _this, int32_t w); \
    int32_t          (*getTransformHeight)(const THIS_TYPE* _this); \
    leResult         (*setTransformHeight)(THIS_TYPE* _this, int32_t h); \
    leResult         (*resetTransform)(THIS_TYPE* _this); \
    leBool           (*getStretchEnabled)(const THIS_TYPE* _this); \
    leResult         (*setStretchEnabled)(THIS_TYPE* _this, leBool b); \
    leBool           (*getPreserveAspectEnabled)(const THIS_TYPE* _this); \
    leResult         (*setPreserveAspectEnabled)(THIS_TYPE* _this, leBool b); \
    leImagePlusWidget_ResizeFilter (*getResizeFilter)(const THIS_TYPE* _this); \
    leResult         (*setResizeFilter)(THIS_TYPE* _this, leImagePlusWidget_ResizeFilter filter); \
    leBool           (*getInteractive)(const THIS_TYPE* _this); \
    leResult         (*setInteractive)(THIS_TYPE* _this, leBool b); \
    
typedef struct leImagePlusWidgetVTable
{
	LE_IMAGEPLUSWIDGET_VTABLE(leImagePlusWidget)
} leImagePlusWidgetVTable; 

// DOM-IGNORE-END

// *****************************************************************************
/* Enumeration:
    leImagePlusWidget

  Summary:
    Image plus widget struct definition

  Description:
    An image plus widget displays an image asset and can translate and resize that image.

  Remarks:
    None.
*/
typedef struct leImagePlusWidget
{
    leWidget widget; // widget base class
    
    leImagePlusWidgetVTable* fn;

    const leImage* image; // pointer to image asset

    int32_t transformX;
    int32_t transformY;
    int32_t transformWidth;
    int32_t transformHeight;

    leBool resizeToFit;
    leBool preserveAspect;

    leImagePlusWidget_ResizeFilter resizeFilter;

    leBool inputEnabled;

    lePoint touch0;
    leBool touch0_down;

    lePoint touch1;
    leBool touch1_down;
} leImagePlusWidget;

// DOM-IGNORE-BEGIN
// internal use only

void _leImagePlusWidget_Destructor(leImagePlusWidget* img);

void _leImagePlusWidget_Paint(leImagePlusWidget* img);
// DOM-IGNORE-END

// *****************************************************************************
// *****************************************************************************
// Section: Routines
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/* Function:
    leImagePlusWidget* leImagePlusWidget_New()

  Summary:
    Allocates memory for and initializes a new widget of this type.  The
    application is responsible for the managment of this memory until the
    widget is added to a widget tree.

  Description:


  Parameters:
    void

  Returns:
    leImagePlusWidget* - the widget

  Remarks:
    Use leWidget_Delete() to free this pointer.
*/
LIB_EXPORT leImagePlusWidget* leImagePlusWidget_New();

/* Function:
    void leImagePlusWidget_Constructor(leImagePlusWidget* wgt)

  Summary:
    Initializes an leImagePlusWidget widget pointer.

  Description:
    Initializes an leImagePlusWidget widget pointer.

  Parameters:
    leImagePlusWidget* wgt - the pointer to initialize

  Returns:
    void

  Remarks:

*/
LIB_EXPORT void leImagePlusWidget_Constructor(leImagePlusWidget* img);

// *****************************************************************************
/* Virtual Member Function:
    leImage* getImage(const leImagePlusWidget* _this)

  Summary:
     Gets the image pointer

  Description:
     Gets the image pointer

  Parameters:
    const leImagePlusWidget* _this - The image plus widget to operate on

  Remarks:
    Usage - _this->fn->getImage(_this);

  Returns:
    leImage* - the image pointer
*/

// *****************************************************************************
/* Virtual Member Function:
    leResult setImage(leImagePlusWidget* _this,
                      const leImage* img)

  Summary:
     Sets the image pointer

  Description:
     Sets the image pointer

  Parameters:
    leImagePlusWidget* _this - The image plus widget to operate on
    const leImage* img - the image pointer

  Remarks:
    Usage - _this->fn->setImage(_this, img);

  Returns:
    leResult - the result of the operation
*/

// *****************************************************************************
/* Virtual Member Function:
    int32_t getTransformX(const leImagePlusWidget* _this)

  Summary:
     Gets the X transform

  Description:
     Gets the X transform

  Parameters:
    const leImagePlusWidget* _this - The image plus widget to operate on

  Remarks:
    Usage - _this->fn->getTransformX(_this);

  Returns:
    int32_t - the x value
*/

// *****************************************************************************
/* Virtual Member Function:
    leResult setTransformX(leImagePlusWidget* _this,
                           int32_t x)

  Summary:
     Sets the X transform

  Description:
     Sets the X transform

  Parameters:
    leImagePlusWidget* _this - The image plus widget to operate on
    int32_t x - the X value

  Remarks:
    Usage - _this->fn->setTransformX(_this, x);

  Returns:
    leResult - the result of the operation
*/

// *****************************************************************************
/* Virtual Member Function:
    int32_t getTransformY(const leImagePlusWidget* _this)

  Summary:
     Gets the Y transform

  Description:
     Gets the Y transform

  Parameters:
    const leImagePlusWidget* _this - The image plus widget to operate on

  Remarks:
    Usage - _this->fn->getTransformY(_this);

  Returns:
    int32_t - the y value
*/

// *****************************************************************************
/* Virtual Member Function:
    leResult setTransformY(leImagePlusWidget* _this,
                           int32_t y)

  Summary:
     Sets the Y transform

  Description:
     Sets the Y transform

  Parameters:
    leImagePlusWidget* _this - The image plus widget to operate on
    int32_t y - the Y value

  Remarks:
    Usage - _this->fn->setTransformY(_this, y);

  Returns:
    leResult - the result of the operation
*/

// *****************************************************************************
/* Virtual Member Function:
    int32_t getTransformWidth(const leImagePlusWidget* _this)

  Summary:
     Gets the transform width

  Description:
     Gets the transform width

  Parameters:
    const leImagePlusWidget* _this - The image plus widget to operate on

  Remarks:
    Usage - _this->fn->getTransformWidth(_this);

  Returns:
    int32_t - the width value
*/

// *****************************************************************************
/* Virtual Member Function:
    leResult setTransformWidth(leImagePlusWidget* _this,
                               int32_t w)

  Summary:
     Sets the transform width

  Description:
     Sets the transform width

  Parameters:
    leImagePlusWidget* _this - The image plus widget to operate on
    int32_t w - the width value

  Remarks:
    Usage - _this->fn->setTransformWidth(_this, w);

  Returns:
    leResult - the result of the operation
*/

// *****************************************************************************
/* Virtual Member Function:
    int32_t getTransformHeight(const leImagePlusWidget* _this)

  Summary:
     Gets the transform height

  Description:
     Gets the transform height

  Parameters:
    const leImagePlusWidget* _this - The image plus widget to operate on

  Remarks:
    Usage - _this->fn->getTransformHeight(_this);

  Returns:
    int32_t - the height value
*/

// *****************************************************************************
/* Virtual Member Function:
    leResult setTransformHeight(leImagePlusWidget* _this,
                                int32_t h)

  Summary:
     Sets the transform height

  Description:
     Sets the transform height

  Parameters:
    leImagePlusWidget* _this - The image plus widget to operate on
    int32_t h - the height value

  Remarks:
    Usage - _this->fn->setTransformHeight(_this, h);

  Returns:
    leResult - the result of the operation
*/

// *****************************************************************************
/* Virtual Member Function:
    leResult resetTransform(leImagePlusWidget* _this)

  Summary:
     Resets the current transform state

  Description:
     Resets the current transform state

  Parameters:
    leImagePlusWidget* _this - The image plus widget to operate on

  Remarks:
    Usage - _this->fn->resetTransform(_this);

  Returns:
    leResult - the result of the operation
*/

// *****************************************************************************
/* Virtual Member Function:
    leBool getStretchEnabled(const leImagePlusWidget* _this)

  Summary:
     Gets the stretch mode value

  Description:
     Gets the stretch mode value

  Parameters:
    const leImagePlusWidget* _this - The image plus widget to operate on

  Remarks:
    Usage - _this->fn->getStretchEnabled(_this);

  Returns:
    leBool - the flag value
*/

// *****************************************************************************
/* Virtual Member Function:
    leResult setStretchEnabled(leImagePlusWidget* _this,
                               leBool b)

  Summary:
     Sets the stretch mode value

  Description:
     Sets the stretch mode value

  Parameters:
    leImagePlusWidget* _this - The image plus widget to operate on
    leBool b - the flag value

  Remarks:
    Usage - _this->fn->setStretchEnabled(_this, b);

  Returns:
    leResult - the result of the operation
*/

// *****************************************************************************
/* Virtual Member Function:
    leBool getPreserveAspectEnabled(const leImagePlusWidget* _this)

  Summary:
     Gets the aspect preservation mode value

  Description:
     Gets the aspect preservation mode value

  Parameters:
    const leImagePlusWidget* _this - The image plus widget to operate on

  Remarks:
    Usage - _this->fn->getPreserveAspectEnabled(_this);

  Returns:
    leBool - the flag value
*/

// *****************************************************************************
/* Virtual Member Function:
    leResult setPreserveAspectEnabled(leImagePlusWidget* _this,
                                      leBool b)

  Summary:
     Sets the aspect preseration mode value

  Description:
     Sets the aspect preseration mode value

  Parameters:
    leImagePlusWidget* _this - The image plus widget to operate on
    leBool b - the flag value

  Remarks:
    Usage - _this->fn->setPreserveAspectEnabled(_this, b);

  Returns:
    leResult - the result of the operation
*/

// *****************************************************************************
/* Virtual Member Function:
    leImagePlusWidget_ResizeFilter getResizeFilter(const leImagePlusWidget* _this)

  Summary:
     Gets the current resize filter

  Description:
     Gets the current resize filter

  Parameters:
    const leImagePlusWidget* _this - The image plus widget to operate on

  Remarks:
    Usage - _this->fn->getResizeFilter(_this);

  Returns:
    leImagePlusWidget_ResizeFilter - the filter
*/

// *****************************************************************************
/* Virtual Member Function:
    leResult setResizeFilter(leImagePlusWidget* _this,
                             leImagePlusWidget_ResizeFilter filter)

  Summary:
     Sets the current resize filter

  Description:
     Sets the current resize filter

  Parameters:
    leImagePlusWidget* _this - The image plus widget to operate on
    leImagePlusWidget_ResizeFilter filter - the filter

  Remarks:
    Usage - _this->fn->setResizeFilter(_this, filter);

  Returns:
    leResult - the result of the operation
*/

// *****************************************************************************
/* Virtual Member Function:
    leBool getInteractive(const leImagePlusWidget* _this)

  Summary:
     Gets the interactive mode value

  Description:
     Gets the interactive mode value

  Parameters:
    const leImagePlusWidget* _this - The image plus widget to operate on

  Remarks:
    Usage - _this->fn->getInteractive(_this);

  Returns:
    leBool - the flag value
*/

// *****************************************************************************
/* Virtual Member Function:
    leResult setInteractive(leImagePlusWidget* _this,
                            leBool b)

  Summary:
     Sets the interactive mode value

  Description:
     Sets the interactive mode value

  Parameters:
    leImagePlusWidget* _this - The image plus widget to operate on
    leBool b - the flag value

  Remarks:
    Usage - _this->fn->setInteractive(_this, b);

  Returns:
    leResult - the result of the operation
*/

// DOM-IGNORE-BEGIN
// internal use only
void _leImagePlusWidget_GetImageRect(leImagePlusWidget* img,
                                     leRect* imgRect,
                                     leRect* imgSrcRect);
// DOM-IGNORE-END

#endif // LE_IMAGE_WIDGET_ENABLED
#endif /* LEGATO_IMAGE_H */