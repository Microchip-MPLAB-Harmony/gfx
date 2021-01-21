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
    legato_widget_line.h

  Summary:


  Description:
    This module implements line draw widget functions.
*******************************************************************************/

/** \file legato_widget_line_graph.h
 * @brief Line draw functions and definitions.
 *
 * @details This module implements line draw widget functions.
 */

#ifndef LEGATO_LINE_H
#define LEGATO_LINE_H

#include "gfx/legato/common/legato_common.h"

#if LE_LINE_WIDGET_ENABLED

#include "gfx/legato/widget/legato_widget.h"

typedef struct leLineWidget leLineWidget;

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
typedef struct leLineWidget leLineWidget;

#define LE_LINEWIDGET_VTABLE(THIS_TYPE) \
    LE_WIDGET_VTABLE(THIS_TYPE) \
    \
    leResult  (*getStartPoint)(const THIS_TYPE* _this, lePoint* pnt); \
    leResult  (*setStartPoint)(THIS_TYPE* _this, int32_t x, int32_t y); \
    leResult  (*getEndPoint)(const THIS_TYPE* _this, lePoint* pnt); \
    leResult  (*setEndPoint)(THIS_TYPE* _this, int32_t x, int32_t y); \
    
typedef struct leLineWidgetVTable
{
	LE_LINEWIDGET_VTABLE(leLineWidget)
} leLineWidgetVTable; 

/**
  * @endcond
  *
  */

// *****************************************************************************
/**
 * @brief This struct represents a line widget
 * @details A line widget draws a simple line shape within the confines of
 * its  bounding rectangle.  All coordinates are expressed in local widget
 * space. The color of the line is determined by the widget scheme's
 * 'foreground' color.
 */
typedef struct leLineWidget
{
    leWidget widget; // widget base class

    const leLineWidgetVTable* fn;

    int32_t x1; // point 1 x
    int32_t y1; // point 1 y
    int32_t x2; // point 2 x
    int32_t y2; // point 2 y
} leLineWidget;

// *****************************************************************************
// *****************************************************************************
// Section: Routines
// *****************************************************************************
// *****************************************************************************

/**
 * @brief Create widget.
 * @details Creates a new leLineWidget and allocates memory for the widget through the
 * current active context.  Application is responsible for managing the widget
 * pointer until the widget is added to a widget tree.
 * @remark use leWidget_Delete() to free this pointer.
 * @code
 * leLineWidget* wgt = leLineWidget_New();
 * @endcode
 * @return a widget object pointer.
 */
leLineWidget* leLineWidget_New(void);

/**
 * @brief Initialize widget.
 * @details Initializes the leLabelWidget <span class="param">wgt</span>.
 * @code
 * leLabelWidget* wgt;
 * leImageScaleWidget_Constructor(wgt);
 * @endcode
 * @param wgt is the widget to initialize
 * @return void.
 */
void leLineWidget_Constructor(leLineWidget* wgt);

#ifdef _DOXYGEN_
#define THIS_TYPE struct leWidget

/**
 * @brief Get line start point.
 * @details Gets the line start point for <span class="param">_this</span>.
 * @code
 * const leLineWidget* _this;
 * lePoint pnt = _this->fn->getStartPoint(_this);
 * @endcode
 * @param _this is the widget pointer to query.
 * @returns the start point.
 */
virtual lePoint getStartPoint(const leLineWidget* _this);


/**
 * @brief Set line start point.
 * @details Sets line start point toy
 * <span class="param">x</span> and <span class="param">x</span>
 * for <span class="param">_this</span>.
 * @code
 * leButtonWidget* _this;
 * int32_t x, y;
 * leResult res = _this->fn->setStartPoint(_this, x, y);
 * @endcode
 * @param _this is the widget pointer to modify.
 * @param str is the string pointer.
 * @returns LE_SUCCESS if set, otherwise LE_FAILURE.
 */
virtual leResult setStartPoint(leLineWidget* _this,
                               int32_t x,
                               int32_t y);


/**
 * @brief Get line end point.
 * @details Gets the line end point for <span class="param">_this</span>.
 * @code
 * const leLineWidget* _this;
 * lePoint pnt = _this->fn->getStartPoint(_this);
 * @endcode
 * @param _this is the widget pointer to query.
 * @returns the end point.
 */
virtual lePoint getEndPoint(const leLineWidget* _this);


/**
 * @brief Set line end point.
 * @details Sets line end point toy
 * <span class="param">x</span> and <span class="param">x</span>
 * for <span class="param">_this</span>.
 * @code
 * leLineWidget* _this;
 * int32_t x, y;
 * leResult res = _this->fn->setStartPoint(_this, x, y);
 * @endcode
 * @param _this is the widget pointer to modify.
 * @param x is the x point.
 * @param y is the y point.
 * @returns LE_SUCCESS if set, otherwise LE_FAILURE.
 */
virtual leResult setEndPoint(leLineWidget* _this,
                               int32_t x,
                               int32_t y);

#undef THIS_TYPE
#endif


#endif // LE_LINE_WIDGET_ENABLED
#endif /* LEGATO_LINE_H */
