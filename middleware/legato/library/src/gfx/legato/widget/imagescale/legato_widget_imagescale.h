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

/** \file legato_widget_image.h
 * @brief Image widget functions and definitions.
 *
 * @details This module implements image widget functions.
 */


#ifndef LEGATO_IMAGESCALE_H
#define LEGATO_IMAGESCALE_H

#include "gfx/legato/common/legato_common.h"

#if LE_IMAGESCALE_WIDGET_ENABLED == 1

#ifdef __cplusplus
extern "C" {
#endif

#include "gfx/legato/image/legato_image.h"
#include "gfx/legato/widget/legato_widget.h"

typedef struct leImageScaleWidget leImageScaleWidget;

// *****************************************************************************
// *****************************************************************************
// Section: Data Types and Constants
// *****************************************************************************
// *****************************************************************************

/* internal use only */
/**
  * @cond INTERNAL
  *
  */typedef struct leImageScaleWidget leImageScaleWidget;

#define LE_IMAGEPLUSWIDGET_VTABLE(THIS_TYPE) \
    LE_WIDGET_VTABLE(THIS_TYPE) \
    \
    leImage*          (*getImage)(const THIS_TYPE* _this); \
    leResult          (*setImage)(THIS_TYPE* _this, const leImage* img); \
    int32_t           (*getTransformX)(const THIS_TYPE* _this); \
    leResult          (*setTransformX)(THIS_TYPE* _this, int32_t x); \
    int32_t           (*getTransformY)(const THIS_TYPE* _this); \
    leResult          (*setTransformY)(THIS_TYPE* _this, int32_t y); \
    int32_t           (*getTransformWidth)(const THIS_TYPE* _this); \
    leResult          (*setTransformWidth)(THIS_TYPE* _this, int32_t w); \
    int32_t           (*getTransformHeight)(const THIS_TYPE* _this); \
    leResult          (*setTransformHeight)(THIS_TYPE* _this, int32_t h); \
    leResult          (*resetTransform)(THIS_TYPE* _this); \
    leBool            (*getStretchEnabled)(const THIS_TYPE* _this); \
    leResult          (*setStretchEnabled)(THIS_TYPE* _this, leBool b); \
    leBool            (*getPreserveAspectEnabled)(const THIS_TYPE* _this); \
    leResult          (*setPreserveAspectEnabled)(THIS_TYPE* _this, leBool b); \
    leImageFilterMode (*getFilter)(const THIS_TYPE* _this); \
    leResult          (*setFilter)(THIS_TYPE* _this, leImageFilterMode filter); \

typedef struct leImageScaleWidgetVTable
{
    LE_IMAGEPLUSWIDGET_VTABLE(leImageScaleWidget)
} leImageScaleWidgetVTable;

/**
  * @endcond
  *
  */
// *****************************************************************************
/**
 * @brief This struct represents a Image plus widget
 * @details An image plus widget displays an image asset and can translate
 * and resize that image.
 */
typedef struct leImageScaleWidget
{
    leWidget widget; // widget base class

    const leImageScaleWidgetVTable* fn;

    const leImage* image; // pointer to image asset

    int32_t transformX;
    int32_t transformY;
    int32_t transformWidth;
    int32_t transformHeight;

    leBool resizeToFit;
    leBool preserveAspect;

    leImageFilterMode filter;

    leBool inputEnabled;
} leImageScaleWidget;

/* internal use only */
/**
  * @cond INTERNAL
  *
  */

void _leImageScaleWidget_Destructor(leImageScaleWidget* img);

void _leImageScaleWidget_Paint(leImageScaleWidget* img);
// DOM-IGNORE-END

// *****************************************************************************
// *****************************************************************************
// Section: Routines
// *****************************************************************************
// *****************************************************************************

/**
 * @brief Create widget.
 * @details Creates a new leImageScaleWidget and allocates memory for the widget through the
 * current active context.  Application is responsible for managing the widget
 * pointer until the widget is added to a widget tree.
 * @remark use leWidget_Delete() to free this pointer.
 * @code
 * leImageScaleWidget* wgt = leImageScaleWidget_New();
 * @endcode
 * @return a widget object pointer.
 */
leImageScaleWidget* leImageScaleWidget_New(void);

/**
 * @brief Initialize widget.
 * @details Initializes the leImageScaleWidget <span class="param">wgt</span>.
 * @code
 * leImageScaleWidget* wgt;
 * leImageScaleWidget_Constructor(wgt);
 * @endcode
 * @param wgt is the widget to initialize
 * @return void.
 */
void leImageScaleWidget_Constructor(leImageScaleWidget* wgt);

#ifdef _DOXYGEN_
#define THIS_TYPE struct leWidget

// *****************************************************************************
/* Virtual Member Function:
    leImage* getImage(const leImageScaleWidget* _this)

  Summary:
     Gets the image pointer

  Description:
     Gets the image pointer

  Parameters:
    const leImageScaleWidget* _this - The image plus widget to operate on

  Remarks:
    Usage - _this->fn->getImage(_this);

  Returns:
    leImage* - the image pointer
*/
/**
 * @brief Get image pointer.
 * @details Gets the image pointer for <span class="param">_this</span>.
 * @code
 * const leImageScaleWidget* _this;
 * leImage* img = _this->fn->getString(_this);
 * @endcode
 * @param _this is the widget pointer to query.
 * @returns the string pointer.
 */
virtual leImage* getImage(const leImageScaleWidget* _this);

// *****************************************************************************
/* Virtual Member Function:
    leResult setImage(leImageScaleWidget* _this,
                      const leImage* img)

  Summary:
     Sets the image pointer

  Description:
     Sets the image pointer

  Parameters:
    leImageScaleWidget* _this - The image plus widget to operate on
    const leImage* img - the image pointer

  Remarks:
    Usage - _this->fn->setImage(_this, img);

  Returns:
    leResult - the result of the operation
*/
/**
 * @brief Set image pointer.
 * @details Sets the image pointer to
 * <span class="param">img</span> using <span class="param">_this</span>.
 * @code
 * leImageScaleWidget* _this;
 * leImage* img;
 * leResult res = _this->fn->setString(_this, img);
 * @endcode
 * @param _this is the widget pointer to modify.
 * @param img is the string pointer.
 * @returns LE_SUCCESS if set, otherwise LE_FAILURE.
 */
virtual leResult setImage(leImageScaleWidget* _this,
                          leImage* img);

// *****************************************************************************
/* Virtual Member Function:
    int32_t getTransformX(const leImageScaleWidget* _this)

  Summary:
     Gets the X transform

  Description:
     Gets the X transform

  Parameters:
    const leImageScaleWidget* _this - The image plus widget to operate on

  Remarks:
    Usage - _this->fn->getTransformX(_this);

  Returns:
    int32_t - the x value
*/
/**
 * @brief Get X transform.
 * @details Gets the X transform for <span class="param">_this</span>.
 * @code
 * const leImageScaleWidget* _this;
 * int32_t x = _this->fn->getTransformX(_this);
 * @endcode
 * @param _this is the widget pointer to query.
 * @returns the x value.
 */
virtual int32_t getTransformX(const leImageScaleWidget* _this);


// *****************************************************************************
/* Virtual Member Function:
    leResult setTransformX(leImageScaleWidget* _this,
                           int32_t x)

  Summary:
     Sets the X transform

  Description:
     Sets the X transform

  Parameters:
    leImageScaleWidget* _this - The image plus widget to operate on
    int32_t x - the X value

  Remarks:
    Usage - _this->fn->setTransformX(_this, x);

  Returns:
    leResult - the result of the operation
*/
/**
 * @brief Set X transform.
 * @details Sets the X transform to
 * <span class="param">x</span> using <span class="param">_this</span>.
 * @code
 * leImageScaleWidget* _this;
 * leImage* x;
 * leResult res = _this->fn->setTransformX(_this, x);
 * @endcode
 * @param _this is the widget pointer to modify.
 * @param x is the X value.
 * @returns LE_SUCCESS if set, otherwise LE_FAILURE.
 */
virtual leResult setTransformX(leImageScaleWidget* _this,
                               int32_t x);


// *****************************************************************************
/* Virtual Member Function:
    int32_t getTransformY(const leImageScaleWidget* _this)

  Summary:
     Gets the Y transform

  Description:
     Gets the Y transform

  Parameters:
    const leImageScaleWidget* _this - The image plus widget to operate on

  Remarks:
    Usage - _this->fn->getTransformY(_this);

  Returns:
    int32_t - the y value
*/
/**
 * @brief Get Y transform.
 * @details Gets the Y transform for <span class="param">_this</span>.
 * @code
 * const leImageScaleWidget* _this;
 * int32_t y = _this->fn->getTransformX(_this);
 * @endcode
 * @param _this is the widget pointer to query.
 * @returns the y value.
 */
virtual int32_t getTransformY(const leImageScaleWidget* _this);


// *****************************************************************************
/* Virtual Member Function:
    leResult setTransformY(leImageScaleWidget* _this,
                           int32_t y)

  Summary:
     Sets the Y transform

  Description:
     Sets the Y transform

  Parameters:
    leImageScaleWidget* _this - The image plus widget to operate on
    int32_t y - the Y value

  Remarks:
    Usage - _this->fn->setTransformY(_this, y);

  Returns:
    leResult - the result of the operation
*/
/**
 * @brief Set Y transform.
 * @details Sets the Y transform to
 * <span class="param">y</span> using <span class="param">_this</span>.
 * @code
 * leImageScaleWidget* _this;
 * leImage* y;
 * leResult res = _this->fn->setTransformX(_this, y);
 * @endcode
 * @param _this is the widget pointer to modify.
 * @param y is the Y value.
 * @returns LE_SUCCESS if set, otherwise LE_FAILURE.
 */
virtual leResult setTransformY(leImageScaleWidget* _this,
                               int32_t y);

// *****************************************************************************
/* Virtual Member Function:
    int32_t getTransformWidth(const leImageScaleWidget* _this)

  Summary:
     Gets the transform width

  Description:
     Gets the transform width

  Parameters:
    const leImageScaleWidget* _this - The image plus widget to operate on

  Remarks:
    Usage - _this->fn->getTransformWidth(_this);

  Returns:
    int32_t - the width value
*/
/**
 * @brief Get transform width.
 * @details Gets the transform width for <span class="param">_this</span>.
 * @code
 * const leImageScaleWidget* _this;
 * int32_t width = _this->fn->getTransformWidth(_this);
 * @endcode
 * @param _this is the widget pointer to query.
 * @returns the width value.
 */
virtual int32_t getTransformWidth(const leImageScaleWidget* _this);

// *****************************************************************************
/* Virtual Member Function:
    leResult setTransformWidth(leImageScaleWidget* _this,
                               int32_t w)

  Summary:
     Sets the transform width

  Description:
     Sets the transform width

  Parameters:
    leImageScaleWidget* _this - The image plus widget to operate on
    int32_t w - the width value

  Remarks:
    Usage - _this->fn->setTransformWidth(_this, w);

  Returns:
    leResult - the result of the operation
*/
/**
 * @brief Set transform width.
 * @details Sets the transform width to
 * <span class="param">w</span> using <span class="param">_this</span>.
 * @code
 * leImageScaleWidget* _this;
 * int32_t w;
 * leResult res = _this->fn->setTransformWidth(_this, w);
 * @endcode
 * @param _this is the widget pointer to modify.
 * @param w is the width value.
 * @returns LE_SUCCESS if set, otherwise LE_FAILURE.
 */
virtual leResult setTransformWidth(leImageScaleWidget* _this,
                                   int32_t w);


// *****************************************************************************
/* Virtual Member Function:
    int32_t getTransformHeight(const leImageScaleWidget* _this)

  Summary:
     Gets the transform height

  Description:
     Gets the transform height

  Parameters:
    const leImageScaleWidget* _this - The image plus widget to operate on

  Remarks:
    Usage - _this->fn->getTransformHeight(_this);

  Returns:
    int32_t - the height value
*/
/**
 * @brief Get transform height.
 * @details Gets the transform height for <span class="param">_this</span>.
 * @code
 * const leImageScaleWidget* _this;
 * int32_t width = _this->fn->getTransformHeight(_this);
 * @endcode
 * @param _this is the widget pointer to query.
 * @returns the height value.
 */
virtual int32_t getTransformHeight(const leImageScaleWidget* _this);


// *****************************************************************************
/* Virtual Member Function:
    leResult setTransformHeight(leImageScaleWidget* _this,
                                int32_t h)

  Summary:
     Sets the transform height

  Description:
     Sets the transform height

  Parameters:
    leImageScaleWidget* _this - The image plus widget to operate on
    int32_t h - the height value

  Remarks:
    Usage - _this->fn->setTransformHeight(_this, h);

  Returns:
    leResult - the result of the operation
*/
/**
 * @brief Set transform height.
 * @details Sets the transform height to
 * <span class="param">h</span> using <span class="param">_this</span>.
 * @code
 * leImageScaleWidget* _this;
 * int32_t h;
 * leResult res = _this->fn->setTransformHeight(_this, h);
 * @endcode
 * @param _this is the widget pointer to modify.
 * @param h is the height value.
 * @returns LE_SUCCESS if set, otherwise LE_FAILURE.
 */
virtual leResult setTransformHeight(leImageScaleWidget* _this,
                                    int32_t h);

// *****************************************************************************
/* Virtual Member Function:
    leResult resetTransform(leImageScaleWidget* _this)

  Summary:
     Resets the current transform state

  Description:
     Resets the current transform state

  Parameters:
    leImageScaleWidget* _this - The image plus widget to operate on

  Remarks:
    Usage - _this->fn->resetTransform(_this);

  Returns:
    leResult - the result of the operation
*/
/**
 * @brief Reset current transform state.
 * @details Resets the current transform state for <span class="param">_this</span>.
 * @code
 * const leImageScaleWidget* _this;
 * leResult res = _this->fn->resetTransform(_this);
 * @endcode
 * @param _this is the widget pointer to query.
 * @returns LE_SUCCESS if set, otherwise LE_FAILURE.
 */
virtual leResult resetTransform(leImageScaleWidget* _this);

// *****************************************************************************
/* Virtual Member Function:
    leBool getStretchEnabled(const leImageScaleWidget* _this)

  Summary:
     Gets the stretch mode value

  Description:
     Gets the stretch mode value

  Parameters:
    const leImageScaleWidget* _this - The image plus widget to operate on

  Remarks:
    Usage - _this->fn->getStretchEnabled(_this);

  Returns:
    leBool - the flag value
*/
/**
 * @brief Get stretch mode value.
 * @details Gets the stretch mode value for <span class="param">_this</span>.
 * @code
 * const leImageScaleWidget* _this;
 * leBool enabled = _this->fn->getStretchEnabled(_this);
 * @endcode
 * @param _this is the widget pointer to query.
 * @returns the flag value.
 */
virtual leBool getStretchEnabled(const leImageScaleWidget* _this);

// *****************************************************************************
/* Virtual Member Function:
    leResult setStretchEnabled(leImageScaleWidget* _this,
                               leBool b)

  Summary:
     Sets the stretch mode value

  Description:
     Sets the stretch mode value

  Parameters:
    leImageScaleWidget* _this - The image plus widget to operate on
    leBool b - the flag value

  Remarks:
    Usage - _this->fn->setStretchEnabled(_this, b);

  Returns:
    leResult - the result of the operation
*/
/**
 * @brief Set stretch mode value.
 * @details Sets the stretch mode value to
 * <span class="param">h</span> using <span class="param">_this</span>.
 * @code
 * leImageScaleWidget* _this;
 * leBool b;
 * leResult res = _this->fn->setStretchEnabled(_this, b);
 * @endcode
 * @param _this is the widget pointer to modify.
 * @param b is the flag value.
 * @returns LE_SUCCESS if set, otherwise LE_FAILURE.
 */
virtual leResult setStretchEnabled(leImageScaleWidget* _this,
                                   leBool b);

// *****************************************************************************
/* Virtual Member Function:
    leBool getPreserveAspectEnabled(const leImageScaleWidget* _this)

  Summary:
     Gets the aspect preservation mode value

  Description:
     Gets the aspect preservation mode value

  Parameters:
    const leImageScaleWidget* _this - The image plus widget to operate on

  Remarks:
    Usage - _this->fn->getPreserveAspectEnabled(_this);

  Returns:
    leBool - the flag value
*/
/**
 * @brief Get aspect preservation mode value.
 * @details Gets the aspect preservation mode value for <span class="param">_this</span>.
 * @code
 * const leImageScaleWidget* _this;
 * leBool enabled = _this->fn->getPreserveAspectEnabled(_this);
 * @endcode
 * @param _this is the widget pointer to query.
 * @returns LE_TRUE if enabled, otherwise LE_FALSE.
 */
virtual leBool getPreserveAspectEnabled(const leImageScaleWidget* _this);

// *****************************************************************************
/* Virtual Member Function:
    leResult setPreserveAspectEnabled(leImageScaleWidget* _this,
                                      leBool b)

  Summary:
     Sets the aspect preseration mode value

  Description:
     Sets the aspect preseration mode value

  Parameters:
    leImageScaleWidget* _this - The image plus widget to operate on
    leBool b - the flag value

  Remarks:
    Usage - _this->fn->setPreserveAspectEnabled(_this, b);

  Returns:
    leResult - the result of the operation
*/
/**
 * @brief Set aspect preseration mode value.
 * @details Sets the aspect preseration mode value to
 * <span class="param">b</span> using <span class="param">_this</span>.
 * @code
 * leImageScaleWidget* _this;
 * leBool b;
 * leResult res = _this->fn->setPreserveAspectEnabled(_this, b);
 * @endcode
 * @param _this is the widget pointer to modify.
 * @param b is the flag value.
 * @returns LE_SUCCESS if set, otherwise LE_FAILURE.
 */
virtual leResult setPreserveAspectEnabled(leImageScaleWidget* _this,
                                          leBool b);

// *****************************************************************************
/* Virtual Member Function:
    leImageScaleWidget_ResizeFilter getResizeFilter(const leImageScaleWidget* _this)

  Summary:
     Gets the current resize filter

  Description:
     Gets the current resize filter

  Parameters:
    const leImageScaleWidget* _this - The image plus widget to operate on

  Remarks:
    Usage - _this->fn->getResizeFilter(_this);

  Returns:
    leImageScaleWidget_ResizeFilter - the filter
*/
/**
 * @brief Get current resize filter.
 * @details Gets the current resize filter value for <span class="param">_this</span>.
 * @code
 * const leImageScaleWidget* _this;
 * leImageScaleWidget_ResizeFilter filter = _this->fn->getPreserveAspectEnabled(_this);
 * @endcode
 * @param _this is the widget pointer to query.
 * @returns the filter.
 */
virtual leImageScaleWidget_ResizeFilter getResizeFilter(const leImageScaleWidget* _this);

// *****************************************************************************
/* Virtual Member Function:
    leResult setResizeFilter(leImageScaleWidget* _this,
                             leImageScaleWidget_ResizeFilter filter)

  Summary:
     Sets the current resize filter

  Description:
     Sets the current resize filter

  Parameters:
    leImageScaleWidget* _this - The image plus widget to operate on
    leImageScaleWidget_ResizeFilter filter - the filter

  Remarks:
    Usage - _this->fn->setResizeFilter(_this, filter);

  Returns:
    leResult - the result of the operation
*/
/**
 * @brief Set current resize filter.
 * @details Sets the current resize filter to
 * <span class="param">filter</span> using <span class="param">_this</span>.
 * @code
 * leImageScaleWidget* _this;
 * leImageScaleWidget_ResizeFilter filter;
 * leResult res = _this->fn->setResizeFilter(_this, filter);
 * @endcode
 * @param _this is the widget pointer to modify.
 * @param filter is the filter.
 * @returns LE_SUCCESS if set, otherwise LE_FAILURE.
 */
virtual leResult setResizeFilter(leImageScaleWidget* _this,
                                 leImageScaleWidget_ResizeFilter filter);

#undef THIS_TYPE
#endif

#ifdef __cplusplus
}
#endif

#endif // LE_IMAGESCALE_WIDGET_ENABLED
#endif /* LEGATO_IMAGE_H */
