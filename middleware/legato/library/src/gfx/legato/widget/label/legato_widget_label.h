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
    legato_widget_label.h

  Summary:


  Description:
    This module implements label (text) widget functions.
*******************************************************************************/

/** \file legato_widget_label.h
 * @brief label widget functions and definitions.
 *
 * @details This module implements label (text) widget functions.
 */

#ifndef LEGATO_LABEL_H
#define LEGATO_LABEL_H

#include "gfx/legato/common/legato_common.h"

#if LE_LABEL_WIDGET_ENABLED == 1

#ifdef __cplusplus
extern "C" {
#endif

#include "gfx/legato/widget/legato_widget.h"
#include "gfx/legato/string/legato_string.h"

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
typedef struct leLabelWidget leLabelWidget;

#define LE_LABELWIDGET_VTABLE(THIS_TYPE) \
    LE_WIDGET_VTABLE(THIS_TYPE) \
    \
    leString*  (*getString)(const THIS_TYPE* _this); \
    leResult   (*setString)(THIS_TYPE* _this, leString* str); \

typedef struct leLabelWidgetVTable
{
    LE_LABELWIDGET_VTABLE(leLabelWidget)
} leLabelWidgetVTable;

/**
  * @endcond
  *
  */

// *****************************************************************************
/**
 * @brief This struct represents a label widget
 * @details A label widget is a simple widget that draws a string of text.
 */
typedef struct leLabelWidget
{
    leWidget widget; // widget base class

    const leLabelWidgetVTable* fn;

    const leString* string; // string to draw
} leLabelWidget;


// *****************************************************************************
// *****************************************************************************
// Section: Routines
// *****************************************************************************
// *****************************************************************************

/**
 * @brief Create widget.
 * @details Creates a new leLabelWidget and allocates memory for the widget through the
 * current active context.  Application is responsible for managing the widget
 * pointer until the widget is added to a widget tree.
 * @remark use leWidget_Delete() to free this pointer.
 * @code
 * leLabelWidget* wgt = leLabelWidget_New();
 * @endcode
 * @return a widget object pointer.
 */
leLabelWidget* leLabelWidget_New(void);

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
void leLabelWidget_Constructor(leLabelWidget* wgt);

#ifdef _DOXYGEN_
#define THIS_TYPE struct leWidget

/**
 * @brief Get string assigned.
 * @details Gets the string assigned for <span class="param">_this</span>.
 * @code
 * const leLabelWidget* _this;
 * leString* str = _this->fn->getString(_this);
 * @endcode
 * @param _this is the widget pointer to query.
 * @returns the string pointer.
 */
virtual leString* getString(const leLabelWidget* _this);


/**
 * @brief Set string.
 * @details Sets the string for <span class="param">_this</span> to
 * <span class="param">str</span>.
 * @code
 * leLabelWidget* _this;
 * const leString* str;
 * leResult res = _this->fn->setString(_this, str);
 * @endcode
 * @param _this is the widget pointer to modify.
 * @param str is the string pointer.
 * @returns LE_SUCCESS if set, otherwise LE_FAILURE.
 */
virtual leResult setString(leLabelWidget* _this,
                           const leString* str);

#undef THIS_TYPE
#endif

#ifdef __cplusplus
}
#endif

#endif // LE_LABEL_WIDGET_ENABLED
#endif /* LEGATO_LABEL_H */
