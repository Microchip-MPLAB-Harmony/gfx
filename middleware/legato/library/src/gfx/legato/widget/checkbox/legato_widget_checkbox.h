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
    legato_widget_button.h

  Summary:


  Description:
    This module implements button widget functions.
*******************************************************************************/

/** \file legato_widget_checkbox.h
 * @brief Checkbox widget functions and definitions.
 *
 * @details This module implements checkbox widget functions.
 */

#ifndef LEGATO_CHECKBOX_H
#define LEGATO_CHECKBOX_H

#include "gfx/legato/common/legato_common.h"

#if LE_CHECKBOX_WIDGET_ENABLED == 1

#ifdef __cplusplus
extern "C" {
#endif

#include "gfx/legato/image/legato_image.h"
#include "gfx/legato/widget/legato_widget.h"
#include "gfx/legato/string/legato_string.h"

typedef struct leCheckBoxWidget leCheckBoxWidget;

// *****************************************************************************
/* Function Pointer:
    leCheckBoxWidget_CheckedEvent

  Summary:
    Checkbox checked event function callback type
*/
/**
 * @brief This function represents a checked event callback.
 * @details Checked event callback is used indicate a check box checked.
 * @details .
 */
typedef void (*leCheckBoxWidget_CheckedEvent)(leCheckBoxWidget*);

// *****************************************************************************
/* Function Pointer:
    leCheckBoxWidget_UncheckedEvent

  Summary:
    Checkbox unchecked event function callback type
*/
/**
 * @brief This function represents a unchecked event callback.
 * @details Checked event callback is used indicate a check box unchecked.
 * @details .
 */
typedef void (*leCheckBoxWidget_UncheckedEvent)(leCheckBoxWidget*);

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
typedef struct leCheckBoxWidget leCheckBoxWidget;

#define LE_CHECKBOXWIDGET_VTABLE(THIS_TYPE) \
    LE_WIDGET_VTABLE(THIS_TYPE) \
    \
    leBool           (*getChecked)(const THIS_TYPE* _this); \
    leResult         (*setChecked)(THIS_TYPE* _this, leBool toggleable); \
    leImage*         (*getCheckedImage)(const THIS_TYPE* _this); \
    leResult         (*setCheckedImage)(THIS_TYPE* _this, leImage* img); \
    leImage*         (*getUncheckedImage)(const THIS_TYPE* _this); \
    leResult         (*setUncheckedImage)(THIS_TYPE* _this, leImage* img); \
    leRelativePosition  (*getImagePosition)(const THIS_TYPE* _this); \
    leResult         (*setImagePosition)(THIS_TYPE* _this, leRelativePosition pos); \
    uint32_t         (*getImageMargin)(const THIS_TYPE* _this); \
    leResult         (*setImageMargin)(THIS_TYPE* _this, uint32_t mg); \
    leString*        (*getString)(const THIS_TYPE* _this); \
    leResult         (*setString)(THIS_TYPE* _this, const leString* str); \
    leCheckBoxWidget_CheckedEvent   (*getCheckedEventCallback)(const THIS_TYPE* _this); \
    leResult         (*setCheckedEventCallback)(THIS_TYPE* _this, leCheckBoxWidget_CheckedEvent cb); \
    leCheckBoxWidget_UncheckedEvent (*getUncheckedEventCallback)(const THIS_TYPE* _this); \
    leResult         (*setUncheckedEventCallback)(THIS_TYPE* _this, leCheckBoxWidget_UncheckedEvent cb); \
    
    
typedef struct leCheckBoxWidgetVTable
{
	LE_CHECKBOXWIDGET_VTABLE(leCheckBoxWidget)
} leCheckBoxWidgetVTable; 

/**
  * @endcond
  *
  */


// *****************************************************************************
/* Structure:
    leCheckBoxWidget

  Summary:
    Implementation of a checkbox widget.

  Description:
    A check box widget contains an interactive two-state box indicating on or
    off.  The check box may also contain descriptive text.  Custom images for
    the check box may be used in place of the default box graphic.

  Remarks:
    None.
*/
typedef struct leCheckBoxWidget
{
    leWidget widget; // base class properties

    const leCheckBoxWidgetVTable* fn;

    leBool checked; // the state of the box

    const leString* string; // the text of the box

    const leImage* checkedImage; // pointer to a custom image to use for the
                                 // checked image
    const leImage* uncheckedImage; // pointer to a custom image to use for the
                                   // unchecked image

    leRelativePosition imagePosition; // postition of the image relative to the text
                                      // of the box
    uint32_t imageMargin; // the distance between the image and the text

    leCheckBoxWidget_CheckedEvent checkedEvent; // callback for checked events
    leCheckBoxWidget_CheckedEvent uncheckedEvent; // callback for unchecked events
} leCheckBoxWidget;

// *****************************************************************************
// *****************************************************************************
// Section: Routines
// *****************************************************************************
// *****************************************************************************

/**
 * @brief Create widget.
 * @details Creates a new leCheckBoxWidget and allocates memory for the widget through the
 * current active context.  Application is responsible for managing the widget
 * pointer until the widget is added to a widget tree.
 * @remark use leWidget_Delete() to free this pointer.
 * @code
 * leCheckBoxWidget* _this = leCheckBoxWidget_New();
 * @endcode
 * @return a widget object pointer.
 */
leCheckBoxWidget* leCheckBoxWidget_New(void);

/**
 * @brief Initialize widget.
 * @details Initializes the leCheckBoxWidget <span class="param">_this</span>.
 * @code
 * leCheckBoxWidget* _this;
 * leCheckBoxWidget_Constructor(_this);
 * @endcode
 * @param _this is the widget to initialize
 * @return void.
 */
void leCheckBoxWidget_Constructor(leCheckBoxWidget* _this);

#ifdef _DOXYGEN_
#define THIS_TYPE struct leWidget

// *****************************************************************************
/**
 * @brief Get checked state.
 * @details Gets the checked state using <span class="param">_this</span>.
 * @code
 * const leCheckBoxWidget* _this;
 * leBool checked = _this->fn->getChecked(_this);
 * @endcode
 * @param _this is the widget pointer to query.
 * @returns LE_TRUE if set, otherwise LE_FALSE;
 */
virtual leBool getChecked(const leCheckBoxWidget* _this);


// *****************************************************************************
/**
 * @brief Set checked state.
 * @details Sets the checked state to <span class="param">toggleable</span>
 * using <span class="param">_this</span>.
 * @code
 * leCheckBoxWidget* _this;
 * leBool toggleable;
 * leResult res = _this->fn->setToggleable(_this, toggleable);
 * @endcode
 * @param _this is the widget pointer to modify.
 * @param toggleable is the toggleable state.
 * @returns LE_SUCCESS if set, otherwise LE_FAILURE.
 */
virtual leResult setChecked(leCheckBoxWidget* _this,
                            leBool toggleable);


// *****************************************************************************
/**
 * @brief Get checked image pointer.
 * @details Gets the checked image pointer using <span class="param">_this</span>.
 * @code
 * const leCheckBoxWidget* _this;
 * leImage* img = _this->fn->getCheckedImage(_this);
 * @endcode
 * @param _this is the widget pointer to query.
 * @returns the checked image pointer.
 */
virtual leImage* getCheckedImage(const leCheckBoxWidget* _this);

// *****************************************************************************
/**
 * @brief Set checked image pointer.
 * @details Sets the checked image pointer to
 * <span class="param">img</span> using <span class="param">_this</span>.
 * @code
 * leCheckBoxWidget* _this;
 * leImage* img;
 * leResult res = _this->fn->setCheckedImage(_this, img);
 * @endcode
 * @param _this is the widget pointer to modify.
 * @param img is the image pointer.
 * @returns LE_SUCCESS if set, otherwise LE_FAILURE.
 */
virtual leResult setCheckedImage(leCheckBoxWidget* _this,
                                 leImage* img);


// *****************************************************************************
/**
 * @brief Get unchecked image pointer.
 * @details Gets the unchecked image pointer using <span class="param">_this</span>.
 * @code
 * const leCheckBoxWidget* _this;
 * leImage* img = _this->fn->getUncheckedImage(_this);
 * @endcode
 * @param _this is the widget pointer to query.
 * @returns the unchecked image pointer.
 */
virtual leImage* getUncheckedImage(const leCheckBoxWidget* _this);


// *****************************************************************************
/**
 * @brief Set unchecked image pointer.
 * @details Sets the unchecked image pointer to
 * <span class="param">img</span> using <span class="param">_this</span>.
 * @code
 * leCheckBoxWidget* _this;
 * leImage* img;
 * leResult res = _this->fn->setUncheckedImage(_this, img);
 * @endcode
 * @param _this is the widget pointer to modify.
 * @param img is the image pointer.
 * @returns LE_SUCCESS if set, otherwise LE_FAILURE.
 */
virtual leResult setUncheckedImage(leCheckBoxWidget* _this,
                                 leImage* img);


// *****************************************************************************
/**
 * @brief Get relative position of the image to the text.
 * @details Gets the relative position of the image to the text using
 *  <span class="param">_this</span>.
 * @code
 * const leCheckBoxWidget* _this;
 * leRelativePosition pos = _this->fn->getImagePosition(_this);
 * @endcode
 * @param _this is the widget pointer to query.
 * @returns the relative position.
 */
virtual leRelativePosition getImagePosition(const leCheckBoxWidget* _this);


// *****************************************************************************
/**
 * @brief Set relative position of the image to the text.
 * @details Sets the relative position of the image to the text using
 *  <span class="param">_this</span> to
 * <span class="param">pos</span>.
 * @code
 * leCheckBoxWidget* _this;
 * leRelativePosition pos;
 * leResult res = _this->fn->setImagePosition(_this, pos);
 * @endcode
 * @param _this is the widget pointer to modify.
 * @param pos the relative position.
 * @returns LE_SUCCESS if set, otherwise LE_FAILURE.
 */
virtual setImagePosition(leCheckBoxWidget* _this,
                         leRelativePosition pos);


// *****************************************************************************
/**
 * @brief Get image margin.
 * @details Gets the image margin using <span class="param">_this</span>.
 * @code
 * const leCheckBoxWidget* _this;
 * uint32_t margin = _this->fn->getString(_this);
 * @endcode
 * @param _this is the widget pointer to query.
 * @returns the image margin.
 */
virtual uint32_t getImageMargin(const leCheckBoxWidget* _this);


// *****************************************************************************
/**
 * @brief Set image margin.
 * @details Sets the image margin using <span class="param">_this</span> to
 * <span class="param">mg</span>.
 * @code
 * leCheckBoxWidget* _this;
 * uint32_t mg;
 * leResult res = _this->fn->setImageMargin(_this, mg);
 * @endcode
 * @param _this is the widget pointer to modify.
 * @param mg the offset value.
 * @returns LE_SUCCESS if set, otherwise LE_FAILURE.
 */
virtual leResult setImageMargin(leCheckBoxWidget* _this,
                                uint32_t mg));


/**
 * @brief Get current string pointer.
 * @details Gets the current string pointer using <span class="param">_this</span>.
 * @code
 * const leCheckBoxWidget* _this;
 * leString* str = _this->fn->getString(_this);
 * @endcode
 * @param _this is the widget pointer to query.
 * @returns the current string pointer.
 */
virtual leString* getString(const leCheckBoxWidget* _this);


// *****************************************************************************
/**
 * @brief Set string.
 * @details Sets the string  to <span class="param">str</span> using
 *  <span class="param">_this</span>.
 * @code
 * leCheckBoxWidget* _this;
 * const leString* str;
 * leResult res = _this->fn->setString(_this, str);
 * @endcode
 * @param _this is the widget pointer to modify.
 * @param str is the string pointer.
 * @returns LE_SUCCESS if set, otherwise LE_FAILURE.
 */
virtual setString(leCheckBoxWidget* _this,
                  const leString* str);


/**
 * @brief Get checked event callback.
 * @details Gets the checked event callback pointer pointer
 * using <span class="param">_this</span>.
 * @code
 * leCheckBoxWidget* _this;
 * leCheckBoxWidget_CheckedEvent cb = _this->fn->getCheckedEventCallback(_this);
 * @endcode
 * @param _this is the widget to modify
 * @returns the event callback.
 */
virtual leCheckBoxWidget_CheckedEvent getCheckedEventCallback(const leCheckBoxWidget* _this);


/**
 * @brief Set checked event callback.
 * @details Sets the checked event callback pointer to
 * <span class="param">cb</span> using <span class="param">_this</span>.
 * @code
 * leCheckBoxWidget* _this;
 * leCheckBoxWidget_CheckedEvent cb;
 * leResult res = _this->fn->setCheckedEventCallback(_this, cb);
 * @endcode
 * @param _this is the widget to modify
 * @param cb is the callback func
 * @returns LE_SUCCESS if set, otherwise LE_FAILURE.
 */
virtual leResult setCheckedEventCallback(leCheckBoxWidget* _this,
                                         leCheckBoxWidget_CheckedEvent cb);


/**
 * @brief Get unchecked event callback pointer.
 * @details Gets the unchecked event callback pointer pointer using
 * <span class="param">_this</span>.
 * @code
 * leCheckBoxWidget* _this;
 * leCheckBoxWidget_UncheckedEvent cb = _this->fn->getUncheckedEventCallback(_this);
 * @endcode
 * @param _this is the widget to modify
 * @returns LE_SUCCESS if set, otherwise LE_FAILURE.
 */
virtual leCheckBoxWidget_UncheckedEvent getUncheckedEventCallback
                                                (const leCheckBoxWidget* _this);

/**
 * @brief Set unchecked event callback pointer.
 * @details Sets the unchecked event callback pointer to
 * <span class="param">cb</span>  using <span class="param">_this</span>.
 * @code
 * leCheckBoxWidget* _this;
 * leCheckBoxWidget_UncheckedEvent cb;
 * leResult res = _this->fn->setUncheckedEventCallback(_this, cb);
 * @endcode
 * @param _this is the widget to modify
 * @param cb is the callback func
 * @returns LE_SUCCESS if set, otherwise LE_FAILURE.
 */
virtual leResult setUncheckedEventCallback(leCheckBoxWidget* _this,
                                           leCheckBoxWidget_UncheckedEvent cb);

#undef THIS_TYPE
#endif

#ifdef __cplusplus
}
#endif

#endif // LE_CHECKBOX_WIDGET_ENABLED
#endif /* LEGATO_BUTTON_H */
