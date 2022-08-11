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
 * @brief Image widget menu functions and definitions.
 *
 * @details This module implements image widget functions.
 */

#ifndef LEGATO_IMAGE_H
#define LEGATO_IMAGE_H

#include "gfx/legato/common/legato_common.h"

#if LE_IMAGE_WIDGET_ENABLED == 1

#ifdef __cplusplus
extern "C" {
#endif

#include "gfx/legato/widget/legato_widget.h"

typedef struct leImageWidget leImageWidget;
typedef struct leImage leImage;

// *****************************************************************************
// *****************************************************************************
// Section: Data Types and Constants
// *****************************************************************************
// *****************************************************************************

/* internal use only */
/**
  * @cond INTERNAL
  *
  */
#define LE_IMAGEWIDGET_VTABLE(THIS_TYPE) \
    LE_WIDGET_VTABLE(THIS_TYPE) \
    \
    leImage*  (*getImage)(const THIS_TYPE* _this); \
    leResult  (*setImage)(THIS_TYPE* _this, leImage* img); \

typedef struct leImageWidgetVTable
{
    LE_IMAGEWIDGET_VTABLE(leImageWidget)
} leImageWidgetVTable;

/**
  * @endcond
  *
  */

// *****************************************************************************
/* Enumeration:
    leImageWidget

  Summary:
    Image widget struct definition

  Description:
    An image widget displays an image asset.

  Remarks:
    None.
*/
/**
  * @brief This struct represents a image widget.
  * @details An image widget displays an image asset.
  */
typedef struct leImageWidget
{
    leWidget widget; // widget base class

    const leImageWidgetVTable* fn;

    const leImage* image; // pointer to image asset
} leImageWidget;

// *****************************************************************************
// *****************************************************************************
// Section: Routines
// *****************************************************************************
// *****************************************************************************

/**
 * @brief Create widget.
 * @details Creates a new leImageWidget and allocates memory for the widget through the
 * current active context.  Application is responsible for managing the widget
 * pointer until the widget is added to a widget tree.
 * @remark use leWidget_Delete() to free this pointer.
 * @code
 * leImageWidget* wgt = leImageWidget_New();
 * @endcode
 * @return a widget object pointer.
 */
leImageWidget* leImageWidget_New(void);

/**
 * @brief Initialize widget.
 * @details Initializes the leImageWidget <span class="param">wgt</span>.
 * @code
 * leImageWidget* wgt;
 * leImageWidget_Constructor(wgt);
 * @endcode
 * @param wgt is the widget to initialize
 * @return void.
 */
void leImageWidget_Constructor(leImageWidget* img);

#ifdef _DOXYGEN_
#define THIS_TYPE struct leWidget

/**
 * @brief Get image pointer.
 * @details Gets the image pointer for <span class="param">_this</span>.
 * @code
 * const leImageWidget* _this;
 * leImage* img = _this->fn->getString(_this);
 * @endcode
 * @param _this is the widget pointer to query.
 * @returns the string pointer.
 */
virtual leImage* getImage(const leImageWidget* _this);


/**
 * @brief Set image pointer.
 * @details Sets the image pointer to
 * <span class="param">img</span> for <span class="param">_this</span>.
 * @code
 * leImageWidget* _this;
 * leImage* img;
 * leResult res = _this->fn->setString(_this, img);
 * @endcode
 * @param _this is the widget pointer to modify.
 * @param img is the string pointer.
 * @returns LE_SUCCESS if set, otherwise LE_FAILURE.
 */
virtual leResult setImage(leImageWidget* _this,
                          leImage* img);

/**
 * @brief Set debug draw start callback.
 * @details Sets the debug draw start callback pointer to <span class="param">cb</span>
 * using <span class="param">_this</span>.
 * @code
 * leImageWidget* _this;
 * leImageWidget_DrawEventCallback cb;
 * leResult res = _this->fn->setDebugDrawStartCallback(_this, cb);
 * @endcode
 * @param _this is the widget to modify
 * @param cb is the callback func
 * @returns LE_SUCCESS if set, otherwise LE_FAILURE.
 */
virtual leResult setDebugDrawStartCallback(leImageWidget* _this,
                                           leImageWidget_DrawEventCallback cb);

/**
 * @brief Set debug draw end callback.
 * @details Sets the debug draw end callback pointer to <span class="param">cb</span>
 * using <span class="param">_this</span>.
 * @code
 * leImageWidget* _this;
 * leImageWidget_DrawEventCallback cb;
 * leResult res = _this->fn->setDebugDrawEndCallback(_this, cb);
 * @endcode
 * @param _this is the widget to modify
 * @param cb is the callback func
 * @returns LE_SUCCESS if set, otherwise LE_FAILURE.
 */
virtual void setDebugDrawEndCallback(leImageWidget* _this,
                                     leImageWidget_DrawEventCallback cb);

#undef THIS_TYPE
#endif

#ifdef __cplusplus
}
#endif

#endif // LE_IMAGE_WIDGET_ENABLED
#endif /* LEGATO_IMAGE_H */
