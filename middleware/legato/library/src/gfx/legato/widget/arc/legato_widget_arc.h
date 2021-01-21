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
    legato_widget_arc.h

  Summary:


  Description:
    This module implements arc drawing widget functions.
*******************************************************************************/
/** \file legato_widget_arc.h
 * @brief Arc widget definitions and functions.
 *
 * @details This module implements arc drawing widget functions.
 */

#ifndef LEGATO_WIDGET_ARC_H
#define LEGATO_WIDGET_ARC_H

#include "gfx/legato/common/legato_common.h"

#if LE_ARC_WIDGET_ENABLED == 1

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
  */
typedef struct leArcWidget leArcWidget;

#define LE_ARCWIDGET_VTABLE(THIS_TYPE) \
    LE_WIDGET_VTABLE(THIS_TYPE) \
    \
    uint32_t    (*getRadius)(const THIS_TYPE* _this); \
    leResult    (*setRadius)(THIS_TYPE* _this, uint32_t rad); \
    int32_t     (*getStartAngle)(const THIS_TYPE* _this); \
    leResult    (*setStartAngle)(THIS_TYPE* _this, int32_t ang); \
    uint32_t    (*getThickness)(const THIS_TYPE* _this); \
    leResult    (*setThickness)(THIS_TYPE* _this, uint32_t val); \
    int32_t     (*getCenterAngle)(const THIS_TYPE* _this); \
    leResult    (*setCenterAngle)(THIS_TYPE* _this, int32_t ang); \
    leBool      (*getRoundEdge)(const THIS_TYPE* _this); \
    leResult    (*setRoundEdge)(THIS_TYPE* _this, leBool rnd); \
    
typedef struct leArcWidgetVTable
{
	LE_ARCWIDGET_VTABLE(leArcWidget)
} leArcWidgetVTable; 
/**
  * @endcond
  *
  */

/**
 * @brief This struct represents a arc widget.
 *
 * @details A arc widget draws a arc of the specified origin and radius inside
 * the widget bounds.  All coordinates are expressed in local widget space. The
 * color of the arc is determined by the widget scheme's 'foreground' color.
 */
typedef struct leArcWidget
{
    leWidget widget;            /**< base widget header#widget.*/
    const leArcWidgetVTable* fn; /**< virtual function table#fn.*/
    uint32_t radius;            /**< the radius of the arc#radius.*/
    int32_t startAngle;         /**< the start angle of the arc#startAngle. */
    int32_t centerAngle;        /**< the center angle of the arc#centerAngle. */
    uint32_t endAngle;          /**< the end angle of the arc#endAngle. */
    uint32_t thickness;         /**< the thickness of the arc#thickness. */
    leBool roundEdge;           /**< draws round edge if true#roundEdge.*/
} leArcWidget;

// *****************************************************************************
// *****************************************************************************
// Section: Routines
// *****************************************************************************
// *****************************************************************************

/**
 * @brief Create widget.
 * @details Creates a new leArcWidget and allocates memory for the widget through the
 * current active context.  Application is responsible for managing the widget
 * pointer until the widget is added to a widget tree.
 * @remark use leWidget_Delete() to free this pointer.
 * @code
 * leArcWidget* wgt = leArcWidget_New();
 * @endcode
 * @return a widget object pointer.
 */
leArcWidget* leArcWidget_New(void);

/**
 * @brief Initialize widget.
 * @details Initializes the leArcWidget <span class="param">wgt</span>.
 * @code
 * leArcWidget* wgt;
 * leArcWidget_Constructor(wgt);
 * @endcode
 * @param wgt is the widget to initialize
 * @return void.
 */
void leArcWidget_Constructor(leArcWidget* wgt);

#ifdef _DOXYGEN_
#define THIS_TYPE struct leWidget

/**
 * @brief Get radius of an arc
 * @details Gets the radius of <span class="param">_this</span>.
 * @code
 * leArcWidget* _this;
 * uint32_t arc = _this->fn->getRadius(_this);
 * @endcode
 * @param _this is the widget to operate on.
 * @returns the arc radius
 */
virtual uint32_t getRadius(const leArcWidget* _this);

/**
 * @brief Set radius of an arc.
 * @details Sets the arc radius for <span class="param">_this</span> to
 * <span class="param">rad</span>.
 * @code
 * leArcWidget* _this;
 * leResult res = _this->fn->setRadius(_this, rad);
 * @endcode
 * @param _this is the widget pointer to modify.
 * @param rad the arc radius
 * @returns LE_SUCCESS if set, otherwise LE_FAILURE.
 */
virtual leResult setRadius(leArcWidget* _this,
                          uint32_t rad);

/**
 * @brief Get start angle of the arc.
 * @details Gets the start angle of <span class="param">_this</span>.
 * @code
 * leArcWidget* _this;
 * uint32_t startAngle = _this->fn->getStartAngle(_this);
 * @endcode
 * @param _this is the widget pointer to query.
 * @returns the start angle value
 */
virtual int32_t getStartAngle(const leArcWidget* _this);

/**
 * @brief Set start angle of an arc.
 * @details Sets the start angle for <span class="param">_this</span> to
 * <span class="param">angle</span>.
 * @code
 * leArcWidget* _this;
 * int32_t angle;
 * leResult res = _this->fn->setStartAngle(_this, angle);
 * @endcode
 * @param _this is the widget pointer to modify.
 * @param angle the starting angle value
 * @returns LE_SUCCESS if set, otherwise LE_FAILURE.
 */
virtual leResult setStartAngle(leArcWidget* _this,
                              int32_t angle);

/**
 * @brief Get thickness of an arc.
 * @details Gets the thickness of <span class="param">_this</span>.
 * @code
 * leArcWidget* _this;
 * uint32_t thickness = _this->fn->getThickness(_this);
 * @endcode
 * @param _this is the widget pointer to query.
 * @returns thickness of arc.
 */
virtual uint32_t getThickness(const leArcWidget* _this);

/**
 * @brief Set thickness of an arc.
 * @details Sets the arc thickness for <span class="param">_this</span> to
 * <span class="param">thickness</span>.
 * @code
 * leArcWidget* _this;
 * leResult res = _this->fn->setThickness(_this, thickness);
 * @endcode
 * @param _this is the widget pointer to modify.
 * @param thickness the arc thickness
 * @returns LE_SUCCESS if set, otherwise LE_FAILURE.
 */
virtual leResult setThickness(leArcWidget* _this,
                             uint32_t thickness);

/**
 * @brief Get thickness of an arc.
 * @details Gets the arc thickness of <span class="param">_this</span>.
 * @code
 * leArcWidget* _this;
 * uint32_t thickness = _this->fn->getThickness(_this);
 * @endcode
 * @param _this is the widget pointer to query.
 * @returns thickness of arc.
 */
virtual int32_t getCenterAngle(const leArcWidget* _this);

/**
 * @brief Set center angle of an arc.
 * @details Sets the arc center angle for <span class="param">_this</span> to
 * <span class="param">ang</span>.
 * @code
 * leArcWidget* _this;
 * leResult res = _this->fn->setCenterAngle(_this, ang);
 * @endcode
 * @param _this is the widget pointer to modify.
 * @param angle the center angle value
 * @returns LE_SUCCESS if set, otherwise LE_FAILURE.
 */
virtual leResult setCenterAngle(THIS_TYPE* _this,
                                int32_t angle);

/**
 * @brief Get rounded edge setting.
 * @details Gets the arc rounded edge setting of <span class="param">_this</span>.
 * @code
 * leArcWidget* _this;
 * leBool round = _this->fn->getRoundEdge(_this);
 * @endcode
 * @param _this is the widget pointer to query.
 * @returns LE_TRUE if round edge is set, otherwise LE_FALSE.
 */
virtual leBool getRoundEdge(const THIS_TYPE* _this);

/**
 * @brief Set draw rounded edges
 * @details Sets arc draw rounded edges flag for <span class="param">_this</span> to
 * <span class="param">round</span>.
 * @code
 * leArcWidget* _this;
 * leResult res = _this->fn->setRoundEdge(_this, round);
 * @endcode
 * @param _this is the widget pointer to modify.
 * @param round the rounded edge value
 * @returns LE_SUCCESS if set, otherwise LE_FAILURE.
 */
virtual leResult setRoundEdge(THIS_TYPE* _this,
                             leBool round);


#undef THIS_TYPE
#endif

#endif // LE_ARC_WIDGET_ENABLED
#endif /* LEGATO_WIDGET_ARC_H */
