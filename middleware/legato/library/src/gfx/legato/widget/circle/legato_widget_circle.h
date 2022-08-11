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
    legato_widget_circle.h

  Summary:


  Description:
    This module implements circle drawing widget functions.
*******************************************************************************/

/** \file legato_widget_circle.h
 * @brief Circle widget functions and definitions.
 *
 * @details This module implements circle drawing widget functions.
 */

#ifndef LEGATO_CIRCLE_H
#define LEGATO_CIRCLE_H

#include "gfx/legato/common/legato_common.h"

#if LE_CIRCLE_WIDGET_ENABLED == 1

#ifdef __cplusplus
extern "C" {
#endif

#include "gfx/legato/widget/legato_widget.h"

// *****************************************************************************
// *****************************************************************************
// Section: Data Types and Constants
// *****************************************************************************
// *****************************************************************************

/* internal use only */
/**
  * @cond INTERNAL
  *
  */typedef struct leCircleWidget leCircleWidget;

#define LE_CIRCLEWIDGET_VTABLE(THIS_TYPE) \
    LE_WIDGET_VTABLE(THIS_TYPE) \
    \
    leResult         (*getOrigin)(const THIS_TYPE* _this, lePoint* pnt); \
    leResult         (*setOrigin)(THIS_TYPE* _this, const lePoint org); \
    int32_t          (*getOriginX)(const THIS_TYPE* _this); \
    leResult         (*setOriginX)(THIS_TYPE* _this, const int32_t x); \
    int32_t          (*getOriginY)(const THIS_TYPE* _this); \
    leResult         (*setOriginY)(THIS_TYPE* _this, const int32_t y); \
    uint32_t         (*getRadius)(const THIS_TYPE* _this); \
    leResult         (*setRadius)(THIS_TYPE* _this, uint32_t rad); \
    uint32_t         (*getThickness)(const THIS_TYPE* _this); \
    leResult         (*setThickness)(THIS_TYPE* _this, uint32_t thk); \
    leBool           (*getFilled)(const THIS_TYPE* _this); \
    leResult         (*setFilled)(THIS_TYPE* _this, leBool fill); \

typedef struct leCircleWidgetVTable
{
    LE_CIRCLEWIDGET_VTABLE(leCircleWidget)
} leCircleWidgetVTable;

/**
  * @endcond
  *
  */

// *****************************************************************************
/* Structure:
    leCircleWidget

  Summary:
    Implementation of a circle widget.

  Description:
    A circle widget draws a circle of the specified origin and radius inside
    the widget bounds.  All coordinates are expressed in local widget space.

    The color of the circle is determined by the widget scheme's 'foreground'
    color.

  Remarks:
    None.
*/
typedef struct leCircleWidget
{
    leWidget widget; // base widget header

    const leCircleWidgetVTable* fn;

    int32_t x; // the origin x cooridnate
    int32_t y; // the origin y coordinate
    uint32_t radius; // the radius of the circle
    uint32_t thickness; // the thickness of the circle outline
    leBool filled; // fills the circle area
} leCircleWidget;

// *****************************************************************************
// *****************************************************************************
// Section: Routines
// *****************************************************************************
// *****************************************************************************

/**
 * @brief Create widget.
 * @details Creates a new leCircleWidget and allocates memory for the widget through the
 * current active context.  Application is responsible for managing the widget
 * pointer until the widget is added to a widget tree.
 * @remark use leWidget_Delete() to free this pointer.
 * @code
 * leCircleWidget* wgt = leCircleWidget_New();
 * @endcode
 * @return a widget object pointer.
 */
leCircleWidget* leCircleWidget_New(void);

/**
 * @brief Initialize widget.
 * @details Initializes the leCircleWidget <span class="param">wgt</span>.
 * @code
 * leCircleWidget* wgt;
 * leCircleWidget_Constructor(wgt);
 * @endcode
 * @param wgt is the widget to initialize
 * @return void.
 */
void leCircleWidget_Constructor(leCircleWidget* cir);

#ifdef _DOXYGEN_
#define THIS_TYPE struct leWidget


// *****************************************************************************
/**
 * @brief Get circle origin.
 * @details Gets the circle origin using <span class="param">_this</span>.
 * @code
 * const leCircleWidget* _this;
 * lePoint point = _this->fn->getOrigin(_this);
 * @endcode
 * @param _this is the widget pointer to query.
 * @returns the origin.
 */
virtual lePoint getOrigin(const leCheckBoxWidget* _this);


// *****************************************************************************
/**
 * @brief Set circle origin.
 * @details Sets the circle origin to
 * <span class="param">org</span> using <span class="param">_this</span>.
 * @code
 * leCircleWidget* _this;
 * const lePoint org;
 * leResult res = _this->fn->setOrigin(_this, org);
 * @endcode
 * @param _this is the widget pointer to modify.
 * @param org the circle origin.
 * @returns LE_SUCCESS if set, otherwise LE_FAILURE.
 */
virtual leResult setOrigin(leCircleWidget* _this,
                           const lePoint org);


// *****************************************************************************
/**
 * @brief Get X origin.
 * @details Gets the X origin using <span class="param">_this</span>.
 * @code
 * const leCircleWidget* _this;
 * int32_t xOrigin = _this->fn->getOriginX(_this);
 * @endcode
 * @param _this is the widget pointer to query.
 * @returns the X origin.
 */
virtual int32_t getOriginX(const leCheckBoxWidget* _this);


// *****************************************************************************
/**
 * @brief Set X value of the circle origin.
 * @details Sets the X value of the circle origin to
 * <span class="param">x</span> using <span class="param">_this</span>.
 * @code
 * leCircleWidget* _this;
 * int32_t x;
 * leResult res = _this->fn->setRadius(_this, x);
 * @endcode
 * @param _this is the widget pointer to modify.
 * @param x the X value of the circle origin.
 * @returns LE_SUCCESS if set, otherwise LE_FAILURE.
 */
virtual leResult setOriginY(leCircleWidget* _this,
                            const int32_t x);


// *****************************************************************************
/**
 * @brief Get Y value of the circle origin.
 * @details Gets the Y value of the circle origin using <span class="param">_this</span>.
 * @code
 * const leCircleWidget* _this;
 * int32_t xOrigin = _this->fn->getOriginY(_this);
 * @endcode
 * @param _this is the widget pointer to query.
 * @returns the Y origin.
 */
virtual int32_t getOriginY(const leCircleWidget* _this);

// *****************************************************************************
/**
 * @brief Set Y value of the circle origin.
 * @details Sets the Y value of the circle origin to
 * <span class="param">y</span> using <span class="param">_this</span>.
 * @code
 * leCircleWidget* _this;
 * int32_t y;
 * leResult res = _this->fn->setRadius(_this, y);
 * @endcode
 * @param _this is the widget pointer to modify.
 * @param y the Y value of the circle origin.
 * @returns LE_SUCCESS if set, otherwise LE_FAILURE.
 */
virtual leResult setOriginY(leCircleWidget* _this,
                            const int32_t y);


// *****************************************************************************
/**
 * @brief Get radius.
 * @details Gets the radius using <span class="param">_this</span>.
 * @code
 * const leCircleWidget* _this;
 * uint32_t radius = _this->fn->getRadius(_this);
 * @endcode
 * @param _this is the widget pointer to query.
 * @returns the radius.
 */
virtual uint32_t getRadius(const leCircleWidget* _this);


// *****************************************************************************
/**
 * @brief Set circle radius.
 * @details Sets the circle radius to
 * <span class="param">rad</span> using <span class="param">_this</span>.
 * @code
 * leCircleWidget* _this;
 * uint32_t rad;
 * leResult res = _this->fn->setRadius(_this, rad);
 * @endcode
 * @param _this is the widget pointer to modify.
 * @param rad the circle radius.
 * @returns LE_SUCCESS if set, otherwise LE_FAILURE.
 */
virtual leResult setRadius(leCircleWidget* _this,
                           uint32_t rad);


// *****************************************************************************
/**
 * @brief Get thickness.
 * @details Gets the thickness using <span class="param">_this</span>.
 * @code
 * const leCircleWidget* _this;
 * uint32_t thickness = _this->fn->getThickness(_this);
 * @endcode
 * @param _this is the widget pointer to query.
 * @returns the thickness.
 */
virtual uint32_t getThickness(const leCircleWidget* _this);


// *****************************************************************************
/**
 * @brief Set circle thickness.
 * @details Sets the circle thickness to
 * <span class="param">thk</span> using <span class="param">_this</span>.
 * @code
 * leCircleWidget* _this;
 * uint32_t thk;
 * leResult res = _this->fn->setThickness(_this, thk);
 * @endcode
 * @param _this is the widget pointer to modify.
 * @param thk the thickness value.
 * @returns LE_SUCCESS if set, otherwise LE_FAILURE.
 */
virtual leResult setThickness(leCircleWidget* _this,
                              uint32_t thk);


// *****************************************************************************
/**
 * @brief Get filled setting.
 * @details Gets the filled setting using <span class="param">_this</span>.
 * @code
 * const leCircleWidget* _this;
 * leBool filled = _this->fn->getFilled(_this);
 * @endcode
 * @param _this is the widget pointer to query.
 * @returns LE_TRUE if set, otherwise LE_FALSE.
 */
virtual leBool getFilled(const leCircleWidget* _this);

// *****************************************************************************
/**
 * @brief Set filled setting.
 * @details Sets the filled setting using <span class="param">_this</span> to
 * <span class="param">fill</span>.
 * @code
 * leCircleWidget* _this;
 * leBool fill;
 * leResult res = _this->fn->setFilled(_this, fill);
 * @endcode
 * @param _this is the widget pointer to modify.
 * @param fill the filled setting.
 * @returns LE_SUCCESS if set, otherwise LE_FAILURE.
 */
virtual leResult setFilled(leCircleWidget* _this,
                           leBool fill);


#undef THIS_TYPE
#endif

#ifdef __cplusplus
}
#endif

#endif // LE_CIRCLE_WIDGET_ENABLED
#endif /* LEGATO_CIRCLE_H */
