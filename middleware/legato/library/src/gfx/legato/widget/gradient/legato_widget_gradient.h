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
    legato_widget_gradient.h

  Summary:


  Description:
    This module implements gradient drawing widget functions.
*******************************************************************************/
/** \file legato_widget_gradient.h
 * @brief Gradien drawing widget functions and definitions.
 *
 * @details This module implements gradient drawing widget functions.
 */

#ifndef LEGATO_GRADIENT_H
#define LEGATO_GRADIENT_H

#include "gfx/legato/common/legato_common.h"

#if LE_GRADIENT_WIDGET_ENABLED

#include "gfx/legato/widget/legato_widget.h"

// *****************************************************************************
// *****************************************************************************
// Section: Data Types and Constants
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************

/* internal use only */
/**
  * @cond INTERNAL
  *
  */
typedef struct leGradientWidget leGradientWidget;

#define LE_GRADIENTWIDGET_VTABLE(THIS_TYPE) \
    LE_WIDGET_VTABLE(THIS_TYPE) \
    \
    leDirection (*getDirection)(const THIS_TYPE* _this); \
    leResult                  (*setDirection)(THIS_TYPE* _this, leDirection dir); \

typedef struct leGradientWidgetVTable
{
	LE_GRADIENTWIDGET_VTABLE(leGradientWidget)
} leGradientWidgetVTable; 

/**
  * @endcond
  *
  */

/**
 * @brief This struct represents a gradient widget.
 * @details It is used to define gradientwidget
 */
typedef struct leGradientWidget
{
    leWidget widget; // widget base class

    const leGradientWidgetVTable* fn;

    leDirection dir; // gradient direction
} leGradientWidget;

// *****************************************************************************
// *****************************************************************************
// Section: Routines
// *****************************************************************************
// *****************************************************************************

/**
 * @brief Create widget.
 * @details Creates a new leGradientWidget and allocates memory for the widget through the
 * current active context.  Application is responsible for managing the widget
 * pointer until the widget is added to a widget tree.
 * @remark use leWidget_Delete() to free this pointer.
 * @code
 * leGradientWidget* wgt = leGradientWidget_New();
 * @endcode
 * @return a widget object pointer.
 */
leGradientWidget* leGradientWidget_New(void);

/**
 * @brief Initialize widget.
 * @details Initializes the leGradientWidget <span class="param">wgt</span>.
 * @code
 * leGradientWidget* wgt;
 * leGradientWidget_Constructor(wgt);
 * @endcode
 * @param wgt is the widget to initialize
 * @return void.
 */
void leGradientWidget_Constructor(leGradientWidget* grad);


#ifdef _DOXYGEN_
#define THIS_TYPE struct leWidget


// *****************************************************************************/
/**
 * @brief Get gradient direction.
 * @details Gets the gradient direction to
 * <span class="param">dir</span> using <span class="param">_this</span>.
 * @code
 * leGradientWidget* _this;
 * leDirection dir = _this->fn->getDirection(_this);
 * @endcode
 * @param _this is the widget pointer to modify.
 * @returns the direction.
 */
virtual leDirection getDirection(leGradientWidget* _this);


// *****************************************************************************
/**
 * @brief Set gradient direction.
 * @details Sets the gradient direction for <span class="param">_this</span> to
 * <span class="param">dir</span>.
 * @code
 * leGradientWidget* _this;
 * leDirection dir;
 * leResult res = _this->fn->setDirection(_this, dir);
 * @endcode
 * @param _this is the widget pointer to modify.
 * @param dir the direction.
 * @returns LE_SUCCESS if set, otherwise LE_FAILURE.
 */
virtual leResult setDirection(leGradientWidget* _this,
                              leDirection dir);

#undef THIS_TYPE
#endif


#endif // LE_GRADIENT_WIDGET_ENABLED
#endif /* LEGATO_RECTANGLE_H */
