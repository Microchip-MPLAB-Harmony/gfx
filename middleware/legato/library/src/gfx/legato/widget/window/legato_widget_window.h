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
    legato_widget_window.h

  Summary:
    Window Widget

  Description:
    This module implements window container widget functions.
*******************************************************************************/

/** \file legato_widget_window.h
 * @brief Window Widget functions and definitions.
 *
 * @details This module implements window container widget functions.
 */

#ifndef LEGATO_WINDOW_H
#define LEGATO_WINDOW_H

#include "gfx/legato/common/legato_common.h"

#if LE_WINDOW_WIDGET_ENABLED == 1

#include "gfx/legato/image/legato_image.h"
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
  */typedef struct leWindowWidget leWindowWidget;

#define LE_WINDOWWIDGET_VTABLE(THIS_TYPE) \
    LE_WIDGET_VTABLE(THIS_TYPE) \
    uint32_t  (*getTitleHeight)(const THIS_TYPE* _this); \
    leResult  (*setTitleHeight)(THIS_TYPE* this, uint32_t ht); \
    leString* (*getString)(const THIS_TYPE* _this); \
    leResult  (*setString)(THIS_TYPE* _this, const leString* str); \
    leImage*  (*getIcon)(const THIS_TYPE* _this); \
    leResult  (*setIcon)(THIS_TYPE* _this, const leImage* img); \
    uint32_t  (*getIconMargin)(const THIS_TYPE* this); \
    leResult  (*setIconMargin)(THIS_TYPE* this, uint32_t mg); \
    
typedef struct leWindowWidgetVTable
{
	LE_WINDOWWIDGET_VTABLE(leWindowWidget)
} leWindowWidgetVTable; 

/**
  * @endcond
  *
  */

// *****************************************************************************
/* Structure:
    leWindowWidget

  Summary:
    Implementation of a window widget struct

  Description:
    A window widget is an extension of a basic panel.  It adds a title bar
    with text and an icon.

  Remarks:
    None.
*/
/**
 * @brief This struct represents a window widget.
 * @details A window widget is an extension of a basic panel.  It adds a title
 * bar with text and an icon.
 */
typedef struct leWindowWidget
{
    leWidget widget; // base widget class

    const leWindowWidgetVTable* fn;

    uint32_t titleHeight; // title height
    const leString* title; // title text

    const leImage* icon; // title icon
    uint32_t iconMargin; // title icon margin
} leWindowWidget;


// *****************************************************************************
// *****************************************************************************
// Section: Routines
// *****************************************************************************
// *****************************************************************************

/**
 * @brief Create widget.
 * @details Creates a new leWindowWidget and allocates memory for the widget through the
 * current active context.  Application is responsible for managing the widget
 * pointer until the widget is added to a widget tree.
 * @remark use leWidget_Delete() to free this pointer.
 * @code
 * leWindowWidget* wgt = leWindowWidget_New();
 * @endcode
 * @return a widget object pointer.
 */
leWindowWidget* leWindowWidget_New(void);

/**
 * @brief Initialize widget.
 * @details Initializes the leWindowWidget <span class="param">wgt</span>.
 * @code
 * leWindowWidget* _this;
 * leWindowWidget_Constructor(wgt);
 * @endcode
 * @param wgt is the widget to initialize
 * @return void.
 */
void leWindowWidget_Constructor(leWindowWidget* wgt);

#ifdef _DOXYGEN_
#define THIS_TYPE struct leWidget

// *****************************************************************************
/**
 * @brief Get title bar height.
 * @details Returns the title bar height using <span class="param">_this</span>.
 * @code
 * leWindowWidget* _this;
 * leString * title = _this->fn->getTitleHeight(_this);
 * @endcode
 * @param  _this is the widget to query
 * @return the height value.
 */
virtual uint32_t getTitleHeight(const leWindowWidget* _this);

// *****************************************************************************
/**
 * @brief Set title height.
 * @details Sets the title height using <span class="param">wgt</span>.
 * @code
 * leWindowWidget* _this;
 * uint32_t ht;
 * leResult res = _this->fn->setTitleHeight(wgt, ht);
 * @endcode
 * @param  wgt is the widget to modify;
 * @param ht is the string pointer;
 * @return LE_SUCCESS if set, otherwise LE_FAILURE.
 */
virtual leResult setTitleHeight(leWindowWidget* _this,
                        uint32_t ht);

// *****************************************************************************
/**
 * @brief Get title bar string.
 * @details Gets the title bar string using <span class="param">_this</span>.
 * @code
 * leWindowWidget* _this;
 * leResult res = _this->fn->getString(_this);
 * @endcode
 * @param _this is the widget to modify
 * @return title bar string.
 */
virtual leString* getString(const leWindowWidget* _this);

// *****************************************************************************
/**
 * @brief Set title bar string.
 * @details Sets the title bar string to <span class="param">str</span>
 * using <span class="param">_this</span>.
 * @code
 * leWindowWidget* _this;
 * const leString* str;
 * leResult res = _this->fn->setString(_this, str);
 * @endcode
 * @param  _this is the widget to modify
 * @param str is the string pointer;
 * @return LE_SUCCESS if set, otherwise LE_FAILURE.
 */
virtual leResult setString(leWindowWidget* _this,
                           const leString* str);


// *****************************************************************************
/**
 * @brief Get title icon.
 * @details Gets the title icon from <span class="param">_this</span>.
 * @code
 * leWindowWidget* _this;
 * leImage * icon = _this->fn->getIcon(_this);
 * @endcode
 * @param _this is the widget to query
 * @return returns the title icon.
 */
virtual leImage* getIcon(const leWindowWidget* _this);

// *****************************************************************************
/**
 * @brief Set title bar icon.
 * @details Sets the title bar icon for <span class="param">_this</span>.
 * @code
 * leWindowWidget* _this;
 * leImage* img;
 * leResult res = _this->fn->setIcon(_this, img);
 * @endcode
 * @param  _this is the widget to modify
 * @return LE_SUCCESS if set, otherwise LE_FAILURE.
 */
virtual leResult setIcon(leWindowWidget* _this,
                         const leImage* img);

// *****************************************************************************
/**
 * @brief Get title bar icon margin.
 * @details Gets the title bar icon margin from <span class="param">_this</span>.
 * @code
 * leWindowWidget* _this;
 * uint32_t margin = _this->fn->getIconMargin(_this);
 * @endcode
 * @param  _this is the widget to query
 * @return the title bar icon margin.
 */
virtual uint32_t getIconMargin(const leWindowWidget* _this);

// *****************************************************************************
/**
 * @brief Set title icon margin.
 * @details Sets the title icon margin to <span class="param">mg</span> for
 * <span class="param">_this</span>.
 * @code
 * leWindowWidget* _this;
 * uint32_t mg;
 * leResult res = _this->fn->setIconMargin(_this, mg);
 * @endcode
 * @param _this is the widget to modify
 * @returns LE_SUCCESS if set, otherwise LE_FAILURE.
 */
virtual leResult setIconMargin(const leWindowWidget* this, uint32_t mg);


#undef THIS_TYPE
#endif

#endif // LE_WINDOW_WIDGET_ENABLED
#endif /* LEGATO_WINDOW_H */
