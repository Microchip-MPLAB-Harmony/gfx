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
    legato_widget_imagerotate.h

  Summary:


  Description:
    This module implements a rotatable image widget.
*******************************************************************************/

// DOM-IGNORE-BEGIN

#ifndef LEGATO_IMAGEROTATE_H
#define LEGATO_IMAGEROTATE_H
//DOM-IGNORE-END

#include "gfx/legato/common/legato_common.h"

#if LE_IMAGEROTATE_WIDGET_ENABLED

#include "gfx/legato/image/legato_image.h"
#include "gfx/legato/widget/legato_widget.h"

typedef struct leImageRotateWidget leImageRotateWidget;

// *****************************************************************************
// *****************************************************************************
// Section: Data Types and Constants
// *****************************************************************************
// *****************************************************************************

// DOM-IGNORE-BEGIN
typedef struct leImageRotateWidget leImageRotateWidget;

#define LE_IMAGEROTATEWIDGET_VTABLE(THIS_TYPE) \
    LE_WIDGET_VTABLE(THIS_TYPE) \
    \
    leImage*          (*getImage)(const THIS_TYPE* _this); \
    leResult          (*setImage)(THIS_TYPE* _this, const leImage* img); \
    lePoint           (*getOrigin)(const THIS_TYPE* _this); \
    leResult          (*setOriginX)(THIS_TYPE* _this, int32_t x); \
    leResult          (*setOriginY)(THIS_TYPE* _this, int32_t y); \
    int32_t           (*getAngle)(const THIS_TYPE* _this); \
    leResult          (*setAngle)(THIS_TYPE* _this, int32_t a); \
    leImageFilterMode (*getFilter)(const THIS_TYPE* _this); \
    leResult          (*setFilter)(THIS_TYPE* _this, leImageFilterMode filter); \

typedef struct leImageRotateWidgetVTable
{
	LE_IMAGEROTATEWIDGET_VTABLE(leImageRotateWidget)
} leImageRotateWidgetVTable;

// DOM-IGNORE-END

// *****************************************************************************
/* Enumeration:
    leImageRotateWidget

  Summary:
    Image rotate widget struct definition

  Description:
    An image rotate widget displays an image asset and can translate and resize that image.

  Remarks:
    None.
*/
typedef struct leImageRotateWidget
{
    leWidget widget; // widget base class
    
    const leImageRotateWidgetVTable* fn;

    const leImage* image; // pointer to image asset

    leRect imageRect;

    lePoint origin;
    int32_t angle;

    leImageFilterMode filter;
} leImageRotateWidget;

// DOM-IGNORE-BEGIN
// internal use only

void _leImageRotateWidget_Destructor(leImageRotateWidget* img);

void _leImageRotateWidget_Paint(leImageRotateWidget* img);
// DOM-IGNORE-END

// *****************************************************************************
// *****************************************************************************
// Section: Routines
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/* Function:
    leImageRotateWidget* leImageRotateWidget_New()

  Summary:
    Allocates memory for and initializes a new widget of this type.  The
    application is responsible for the managment of this memory until the
    widget is added to a widget tree.

  Description:


  Parameters:
    void

  Returns:
    leImageRotateWidget* - the widget

  Remarks:
    Use leWidget_Delete() to free this pointer.
*/
LIB_EXPORT leImageRotateWidget* leImageRotateWidget_New();

/* Function:
    void leImageRotateWidget_Constructor(leImageRotateWidget* wgt)

  Summary:
    Initializes an leImageRotateWidget widget pointer.

  Description:
    Initializes an leImageRotateWidget widget pointer.

  Parameters:
    leImageRotateWidget* wgt - the pointer to initialize

  Returns:
    void

  Remarks:

*/
LIB_EXPORT void leImageRotateWidget_Constructor(leImageRotateWidget* img);

// *****************************************************************************
/* Virtual Member Function:
    leImage* getImage(const leImageRotateWidget* _this)

  Summary:
     Gets the image pointer

  Description:
     Gets the image pointer

  Parameters:
    const leImageRotateWidget* _this - The image rotate widget to operate on

  Remarks:
    Usage - _this->fn->getImage(_this);

  Returns:
    leImage* - the image pointer
*/

// *****************************************************************************
/* Virtual Member Function:
    leResult setImage(leImageRotateWidget* _this,
                      const leImage* img)

  Summary:
     Sets the image pointer

  Description:
     Sets the image pointer

  Parameters:
    leImageRotateWidget* _this - The image rotate widget to operate on
    const leImage* img - the image pointer

  Remarks:
    Usage - _this->fn->setImage(_this, img);

  Returns:
    leResult - the result of the operation
*/

// *****************************************************************************
/* Virtual Member Function:
    lePoint getOrigin(const leImageRotateWidget* _this)

  Summary:
     Gets the rotation origin component

  Description:
     Gets the rotation origin component

  Parameters:
    const leImageRotateWidget* _this - The image rotate widget to operate on

  Remarks:
    Usage - _this->fn->getOrigin(_this);

  Returns:
    lePoint - the origin point
*/

// *****************************************************************************
/* Virtual Member Function:
    leResult setOrigin(leImageRotateWidget* _this,
                       lePoint origin)

  Summary:
     Sets the rotation origin

  Description:
     Sets the rotation origin

  Parameters:
    leImageRotateWidget* _this - The image rotate widget to operate on
    lePoint origin - the origin point

  Remarks:
    Usage - _this->fn->setOrigin(_this, org);

  Returns:
    leResult - the result of the operation
*/

// *****************************************************************************
/* Virtual Member Function:
    int32_t getAngle(const leImageRotateWidget* _this)

  Summary:
     Gets the rotation angle

  Description:
     Gets the rotation angle

  Parameters:
    const leImageRotateWidget* _this - The image rotate widget to operate on

  Remarks:
    Usage - _this->fn->getAngle(_this);

  Returns:
    int32_t - the angle value in degrees
*/

// *****************************************************************************
/* Virtual Member Function:
    leResult setAngle(leImageRotateWidget* _this,
                      int32_t angle)

  Summary:
     Sets the rotation angle

  Description:
     Sets the rotation angle

  Parameters:
    leImageRotateWidget* _this - The image rotate widget to operate on
    int32_t angle - the rotation angle value in degrees

  Remarks:
    Usage - _this->fn->setAngle(_this, ang);

  Returns:
    leResult - the result of the operation
*/

// *****************************************************************************
/* Virtual Member Function:
    leImageFilter getFilter(const leImageRotateWidget* _this)

  Summary:
     Gets the current transform filter

  Description:
     Gets the current transform filter

  Parameters:
    const leImageRotateWidget* _this - The image rotate widget to operate on

  Remarks:
    Usage - _this->fn->getFilter(_this);

  Returns:
    leImageFilter - the current filter
*/

// *****************************************************************************
/* Virtual Member Function:
    leResult setFilter(leImageRotateWidget* _this,
                       leImageFilter filter)

  Summary:
     Sets the current transform filter

  Description:
     Sets the current transform filter

  Parameters:
    leImageRotateWidget* _this - The image rotate widget to operate on
    leImageFilter filter - the transform filter to use

  Remarks:
    Usage - _this->fn->setFilter(_this, filter);

  Returns:
    leResult - the result of the operation
*/

#endif // LE_IMAGEROTATE_WIDGET_ENABLED
#endif /* LEGATO_IMAGEROTATE_H */