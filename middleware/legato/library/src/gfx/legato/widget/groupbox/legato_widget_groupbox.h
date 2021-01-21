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
    legato_widget_groupbox.h

  Summary:


  Description:
    This module implements group box widget functions.
*******************************************************************************/

/** \file legato_widget_groupbox.h
 * @brief Group box widget menu functions and definitions.
 *
 * @details This module implements group box widget functions.
 */

#ifndef LEGATO_GROUPBOX_H
#define LEGATO_GROUPBOX_H

#include "gfx/legato/common/legato_common.h"

#if LE_GROUPBOX_WIDGET_ENABLED

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
  */typedef struct leGroupBoxWidget leGroupBoxWidget;

#define LE_GROUPBOXWIDGET_VTABLE(THIS_TYPE) \
    LE_WIDGET_VTABLE(THIS_TYPE) \
    \
    leString*        (*getString)(const THIS_TYPE* _this); \
    leResult         (*setString)(THIS_TYPE* _this, const leString* str); \
    
typedef struct leGroupBoxWidgetVTable
{
	LE_GROUPBOXWIDGET_VTABLE(leGroupBoxWidget)
} leGroupBoxWidgetVTable; 

/**
  * @endcond
  *
  */

// *****************************************************************************
/* Enumeration:
    leGroupBoxWidget

  Summary:
    Group box struct definition.

  Description:
    A group box is a widget that is similar to a basic panel but provides
    a line border and title text.  Used for grouping and describing widgets
    of similar function.

  Remarks:
    None.
*/

/**
  * @brief This struct represents a group box widget.
  * @details A group box is a widget that is similar to a basic panel
  * but provides a line border and title text.  Used for grouping and
  * describing widgets of similar function.
  */
typedef struct leGroupBoxWidget
{
    leWidget widget; // widget base class

    const leGroupBoxWidgetVTable* fn;

    const leString* string; // group box title text
} leGroupBoxWidget;

// *****************************************************************************
// *****************************************************************************
// Section: Routines
// *****************************************************************************
// *****************************************************************************

/**
 * @brief Create widget.
 * @details Creates a new leGroupBoxWidget and allocates memory for the widget through the
 * current active context.  Application is responsible for managing the widget
 * pointer until the widget is added to a widget tree.
 * @remark use leWidget_Delete() to free this pointer.
 * @code
 * leGroupBoxWidget* wgt = leGroupBoxWidget_New();
 * @endcode
 * @return a widget object pointer.
 */
leGroupBoxWidget* leGroupBoxWidget_New(void);

/**
 * @brief Initialize widget.
 * @details Initializes the leGroupBoxWidget <span class="param">wgt</span>.
 * @code
 * leGroupBoxWidget* wgt;
 * leGroupBoxWidget_Constructor(wgt);
 * @endcode
 * @param wgt is the widget to initialize
 * @return void.
 */
void leGroupBoxWidget_Constructor(leGroupBoxWidget* box);

#ifdef _DOXYGEN_
#define THIS_TYPE struct leWidget

/**
 * @brief Get string assigned.
 * @details Gets the string assigned for <span class="param">_this</span>.
 * @code
 * const leGroupBoxWidget* _this;
 * leString* str = _this->fn->getString(_this);
 * @endcode
 * @param _this is the widget pointer to query.
 * @returns the string pointer.
 */
virtual leString* getString(const leGroupBoxWidget* _this);


/**
 * @brief Set string.
 * @details Sets the string for <span class="param">_this</span> to
 * <span class="param">str</span>.
 * @code
 * leGroupBoxWidget* _this;
 * const leString* str;
 * leResult res = _this->fn->setString(_this, str);
 * @endcode
 * @param _this is the widget pointer to modify.
 * @param str is the string pointer.
 * @returns LE_SUCCESS if set, otherwise LE_FAILURE.
 */
virtual leResult setString(leGroupBoxWidget* _this,
                           const leString* str);

#undef THIS_TYPE
#endif



#endif // LE_GROUPBOX_WIDGET_ENABLED
#endif /* LEGATO_GROUPBOX_H */
