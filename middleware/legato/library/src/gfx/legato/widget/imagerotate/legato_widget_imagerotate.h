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
    legato_widget_imagerotate.h

  Summary:


  Description:
    This module implements a rotatable image widget.
*******************************************************************************/

/** \file legato_widget_imagerotate.h
 * @brief Image rotate functions and definitions.
 *
 * @details This module implements a rotatable image widget.
 */

#ifndef LEGATO_IMAGEROTATE_H
#define LEGATO_IMAGEROTATE_H

#include "gfx/legato/common/legato_common.h"

#if LE_IMAGEROTATE_WIDGET_ENABLED == 1

#ifdef __cplusplus
extern "C" {
#endif

#include "gfx/legato/image/legato_image.h"
#include "gfx/legato/widget/legato_widget.h"

typedef struct leImageRotateWidget leImageRotateWidget;

// *****************************************************************************
// *****************************************************************************
// Section: Data Types and Constants
// *****************************************************************************
// *****************************************************************************

/* internal use only */
/**
  * @cond INTERNAL
  *
  */typedef struct leImageRotateWidget leImageRotateWidget;

#define LE_IMAGEROTATEWIDGET_VTABLE(THIS_TYPE) \
    LE_WIDGET_VTABLE(THIS_TYPE) \
    \
    leImage*          (*getImage)(const THIS_TYPE* _this); \
    leResult          (*setImage)(THIS_TYPE* _this, const leImage* img); \
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

/**
 * @brief This struct represents a image rotate widget.
 * @details An image rotate widget displays an image asset and can translate
 * and resize that image.
 */
typedef struct leImageRotateWidget
{
    leWidget widget; // widget base class

    const leImageRotateWidgetVTable* fn;

    const leImage* image; // pointer to image asset

    int32_t angle;

    leImageFilterMode filter;
} leImageRotateWidget;

/* internal use only */
/**
  * @cond INTERNAL
  *
  */

void _leImageRotateWidget_Destructor(leImageRotateWidget* img);

void _leImageRotateWidget_Paint(leImageRotateWidget* img);
/**
  * @endcond
  *
  */

// *****************************************************************************
// *****************************************************************************
// Section: Routines
// *****************************************************************************
// *****************************************************************************

/**
 * @brief Create widget.
 * @details Creates a new leImageRotateWidget and allocates memory for the widget through the
 * current active context.  Application is responsible for managing the widget
 * pointer until the widget is added to a widget tree.
 * @remark use leWidget_Delete() to free this pointer.
 * @code
 * leImageRotateWidget* wgt = leImageRotateWidget_New();
 * @endcode
 * @return a widget object pointer.
 */
leImageRotateWidget* leImageRotateWidget_New(void);

/**
 * @brief Initialize widget.
 * @details Initializes the leImageRotateWidget <span class="param">wgt</span>.
 * @code
 * leImageRotateWidget* wgt;
 * leImageRotateWidget_Constructor(wgt);
 * @endcode
 * @param wgt is the widget to initialize
 * @return void.
 */
void leImageRotateWidget_Constructor(leImageRotateWidget* wgt);

#ifdef _DOXYGEN_
#define THIS_TYPE struct leWidget

/**
 * @brief Get image pointer.
 * @details Gets the image pointer for <span class="param">_this</span>.
 * @code
 * const leImageRotateWidget* _this;
 * leImage* img = _this->fn->getString(_this);
 * @endcode
 * @param _this is the widget pointer to query.
 * @returns the string pointer.
 */
virtual leImage* getImage(const leImageRotateWidget* _this);


/**
 * @brief Set image pointer.
 * @details Sets the image pointer to
 * <span class="param">str</span> for <span class="param">_this</span>.
 * @code
 * leImageRotateWidget* _this;
 * leImage* img;
 * leResult res = _this->fn->setString(_this, img);
 * @endcode
 * @param _this is the widget pointer to modify.
 * @param img is the string pointer.
 * @returns LE_SUCCESS if set, otherwise LE_FAILURE.
 */
virtual leResult setImage(leImageRotateWidget* _this,
                          leImage* img);


/**
 * @brief Get rotation origin component.
 * @details Gets the rotation origin component for <span class="param">_this</span>.
 * @code
 * const leImageRotateWidget* _this;
 * lePoint pnt = _this->fn->getOrigin(_this);
 * @endcode
 * @param _this is the widget pointer to query.
 * @returns the string pointer.
 */
virtual lePoint getOrigin(const leImageRotateWidget* _this);


/**
 * @brief Set image pointer.
 * @details Sets the image pointer to
 * <span class="param">str</span> for <span class="param">_this</span>.
 * @code
 * leImageRotateWidget* _this;
 * lePoint origin;
 * leResult res = _this->fn->setOrigin(_this, origin);
 * @endcode
 * @param _this is the widget pointer to modify.
 * @param origin is the origin point.
 * @returns LE_SUCCESS if set, otherwise LE_FAILURE.
 */
virtual leResult setOrigin(leImageRotateWidget* _this,
                           lePoint origin);

// *****************************************************************************
/**
 * @brief Get rotation angle.
 * @details Gets the rotation angle using <span class="param">_this</span>.
 * @code
 * const leImageRotateWidget* _this;
 * int32_t angle = _this->fn->getStartAngle(_this);
 * @endcode
 * @param _this is the widget pointer to query.
 * @returns the rotation angle.
 */
virtual int32_t getAngle(const leImageRotateWidget* _this);


// *****************************************************************************
/**
 * @brief Set rotation angle.
 * @details Sets the rotation angle using <span class="param">_this</span> to
 * <span class="param">ang</span>.
 * @code
 * const leImageRotateWidget* _this;
 * int32_t angle;
 * leResult res = _this->fn->setStartAngle(_this, angle);
 * @endcode
 * @param _this is the widget pointer to query.
 * @param angle is the rotation angle.
 * @returns LE_SUCCESS if set, otherwise LE_FAILURE.
 */
virtual leResult setAngle(leImageRotateWidget* _this,
                          int32_t angle);


// *****************************************************************************
/**
 * @brief Get current transform filter.
 * @details Gets the current transform filter using <span class="param">_this</span>.
 * @code
 * const leImageRotateWidget* _this;
 * leImageFilter filter = _this->fn->getFilter(_this);
 * @endcode
 * @param _this is the widget pointer to query.
 * @returns the current filter.
 */
virtual leImageFilter getFilter(const leImageRotateWidget* _this);


// *****************************************************************************
/**
 * @brief Set current transform filter.
 * @details Sets the current transform filter using <span class="param">_this</span> to
 * <span class="param">ang</span>.
 * @code
 * const leImageRotateWidget* _this;
 * leImageFilter filter;
 * leResult res = _this->fn->setFilter(_this, filter);
 * @endcode
 * @param _this is the widget pointer to query.
 * @param filter is the transform filter.
 * @returns LE_SUCCESS if set, otherwise LE_FAILURE.
 */
virtual leResult setFilter(leImageRotateWidget* _this,
                           leImageFilter filter);

#undef THIS_TYPE
#endif

#ifdef __cplusplus
}
#endif

#endif // LE_IMAGEROTATE_WIDGET_ENABLED
#endif /* LEGATO_IMAGEROTATE_H */
