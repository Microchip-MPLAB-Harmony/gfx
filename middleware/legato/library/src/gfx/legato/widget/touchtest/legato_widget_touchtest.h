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
    legato_widget_touchtest.h

  Summary:


  Description:
    This module implements graphical touch test (box) widget functions.
*******************************************************************************/

/** \file legato_widget_touchtest.h
 * @brief Window Widget functions and definitions.
 *
 * @details This module implements window container widget functions.
 */

#ifndef LEGATO_TOUCHTEST_H
#define LEGATO_TOUCHTEST_H

#include "gfx/legato/common/legato_common.h"

#if LE_TOUCHTEST_WIDGET_ENABLED

#include "gfx/legato/widget/legato_widget.h"

/* internal use only */
/**
  * @cond INTERNAL
  *
  */
#define LE_TOUCHTEST_MEMORY_SIZE 20
/**
  * @endcond
  *
  */

// *****************************************************************************
// *****************************************************************************
// Section: Data Types and Constants
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/* Enumeration:
    leTouchTestState

  Summary:
    Touch test states

  Description:


  Remarks:
    None.
*/
/**
 * @brief This enum represents touch test state.
 * @details Used to define possible test states.
 * @details .
 */
typedef enum leTouchTestState
{
    LE_TOUCHTEST_STATE_UP,
    LE_TOUCHTEST_STATE_DOWN
} leTouchTestState;

typedef struct leTouchTestWidget leTouchTestWidget;

// *****************************************************************************
/* Function Pointer:
    leTouchTestWidget_PointAddedEventCallback

  Summary:
    Point added event function callback type
*/
/**
 * @brief This function represents a point added event callback.
 * @details The callback is used indicate a point added has occured.
 */
typedef void (*leTouchTestWidget_PointAddedEventCallback)(leTouchTestWidget*, lePoint*);


/* internal use only */
/**
  * @cond INTERNAL
  *
  */typedef struct leTouchTestWidget leTouchTestWidget;

#define LE_TOUCHTESTWIDGET_VTABLE(THIS_TYPE) \
    LE_WIDGET_VTABLE(THIS_TYPE) \
    \
    leResult  (*addPoint)(THIS_TYPE* _this, lePoint* pnt); \
    leResult  (*clearPoints)(THIS_TYPE* _this); \
    leTouchTestWidget_PointAddedEventCallback (*getPointAddedEventCallback)(const THIS_TYPE* _this); \
    leResult  (*setPointAddedEventCallback)(THIS_TYPE* _this, leTouchTestWidget_PointAddedEventCallback cb); \
    
typedef struct leTouchTestWidgetVTable
{
	LE_TOUCHTESTWIDGET_VTABLE(leTouchTestWidget)
} leTouchTestWidgetVTable; 

/**
  * @endcond
  *
  */



// *****************************************************************************
/* Structure:
    leTouchTestWidget

  Summary:
    Implementation of a touch test widget struct

  Description:
    The touch test widget is a specialized widget that displays intersecting
    lines based on input events.  This can help visualize touch interaction
    and aid determining accurate input coordinates.

  Remarks:
    None.
*/
/**
 * @brief This struct represents a touch test widget.
 * @details The touch test widget is a specialized widget that displays
 * intersecting lines based on input events.  This can help visualize touch
 * interaction and aid determining accurate input coordinates.
 */
typedef struct leTouchTestWidget
{
    leWidget widget; // widget base class

    const leTouchTestWidgetVTable* fn;

    leTouchTestState state; // touch test state

    lePoint pnts[LE_TOUCHTEST_MEMORY_SIZE]; // touch point array
    uint32_t size; // current number of valid touch points

    uint32_t start; // first valid touch point
    uint32_t next; // next available touch point entry

    leTouchTestWidget_PointAddedEventCallback cb; // point added callback
} leTouchTestWidget;

// *****************************************************************************
// *****************************************************************************
// Section: Routines
// *****************************************************************************
// *****************************************************************************

/**
 * @brief Create widget.
 * @details Creates a new leTouchTestWidget and allocates memory for the widget through the
 * current active context.  Application is responsible for managing the widget
 * pointer until the widget is added to a widget tree.
 * @remark use leWidget_Delete() to free this pointer.
 * @code
 * leTouchTestWidget* wgt = leTouchTestWidget_New();
 * @endcode
 * @return a widget object pointer.
 */
leTouchTestWidget* leTouchTestWidget_New(void);

/**
 * @brief Initialize widget.
 * @details Initializes the leTouchTestWidget <span class="param">wgt</span>.
 * @code
 * leTouchTestWidget* wgt;
 * leTouchTestWidget_Constructor(wgt);
 * @endcode
 * @param wgt is the widget to initialize
 * @return void.
 */
void leTouchTestWidget_Constructor(leTouchTestWidget* wgt);

#ifdef _DOXYGEN_
#define THIS_TYPE struct leWidget

// *****************************************************************************
/**
 * @brief Add point.
 * @details Adds a point for <span class="param">_this</span>.
 * @code
 * leTouchTestWidget* _this;
 * lePoint* pnt;
 * leResult res = _this->fn->addPoint(_this, pnt);
 * @endcode
 * @param  _this is the widget to query.
 * @param pnt is the point.
 * @return LE_SUCCESS if set, otherwise LE_FAILURE.
 */
virtual leResult addPoint(leTouchTestWidget* _this,
                  lePoint* pnt);

// *****************************************************************************
/**
 * @brief Clear points.
 * @details Clears points for <span class="param">_this</span>.
 * @code
 * leTouchTestWidget* _this;
 * leResult res = _this->fn->clearPoints(_this);
 * @endcode
 * @param  _this is the widget to query.
 * @return LE_SUCCESS if set, otherwise LE_FAILURE.
 */
virtual leResult clearPoints(leTouchTestWidget* _this);

// *****************************************************************************
/**
 * @brief Set point add event callback.
 * @details Sets point added event callback to <span class="param">cb</span>
 * using <span class="param">_this</span>.
 * @code
 * leTouchTestWidget* _this;
 * leTouchTestWidget_PointAddedEventCallback cb;
 * leResult res = _this->fn->setPointAddedEventCallback(_this, cb);
 * @endcode
 * @param _this is the widget to modify
 * @param cb is the callback func
 * @returns LE_SUCCESS if set, otherwise LE_FAILURE.
 */
virtual leResult setPointAddedEventCallback(leTouchTestWidget* _this,
                                    leTouchTestWidget_PointAddedEventCallback cb);

#undef THIS_TYPE
#endif

#endif // LE_TOUCHTEST_WIDGET_ENABLED
#endif /* LEGATO_TOUCHTEST_H */
