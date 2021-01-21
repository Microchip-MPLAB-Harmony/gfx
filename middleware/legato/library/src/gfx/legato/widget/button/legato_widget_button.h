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
    Defines a button widget
*******************************************************************************/

/** \file legato_widget_button.h
 * @brief Button widget functions and definitions.
 *
 * @details This module implements button drawing widget functions.
 */

#ifndef LEGATO_BUTTON_H
#define LEGATO_BUTTON_H

#include "gfx/legato/common/legato_common.h"

#if LE_BUTTON_WIDGET_ENABLED == 1

#include "gfx/legato/widget/legato_widget.h"

#include "gfx/legato/image/legato_image.h"
#include "gfx/legato/string/legato_string.h"

// *****************************************************************************
// *****************************************************************************
// Section: Data Types and Constants
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/* Enumeration:
    leButtonState

  Summary:
    Controls the button pressed state
*/
/**
  * @brief This enum represents button state.
  * @details A button state is used to define possible button positions.
  */
typedef enum leButtonState
{
    LE_BUTTON_STATE_UP,
    LE_BUTTON_STATE_DOWN,
    LE_BUTTON_STATE_TOGGLED
} leButtonState;

/**
  * @brief This struct represents button widget type.
  * @details A button widget type is used .
  */
typedef struct leButtonWidget leButtonWidget;

// *****************************************************************************
/* Function Pointer:
    leButtonWidget_PressedEvent

  Summary:
    Button pressed event function callback type
*/
/**
 * @brief This function represents a pressed event callback.
 * @details The callback is used indicate a button press has occured.
 */
typedef void (*leButtonWidget_PressedEvent)(leButtonWidget*);

// *****************************************************************************
/* Function Pointer:
    leButtonWidget_ReleasedEvent

  Summary:
    Button released event function callback type
*/
/**
 * @brief This function represents a released event callback.
 * @details The callback is used indicate a radio button has released.
 */
typedef void (*leButtonWidget_ReleasedEvent)(leButtonWidget*);


/* internal use only */
/**
  * @cond INTERNAL
  *
  */

typedef struct leButtonWidget leButtonWidget;

#define LE_BUTTONWIDGET_VTABLE(THIS_TYPE) \
    LE_WIDGET_VTABLE(THIS_TYPE) \
    \
    leBool                       (*getToggleable)(const THIS_TYPE* _this); \
    leResult                     (*setToggleable)(THIS_TYPE* _this, leBool toggleable); \
    leBool                       (*getPressed)(const THIS_TYPE* _this); \
    leResult                     (*setPressed)(THIS_TYPE* _this, leBool pressed); \
    leString*                    (*getString)(const THIS_TYPE* _this); \
    leResult                     (*setString)(THIS_TYPE* _this, const leString* str); \
    leImage*                     (*getPressedImage)(const THIS_TYPE* _this); \
    leResult                     (*setPressedImage)(THIS_TYPE* _this, leImage* img); \
    leImage*                     (*getReleasedImage)(const THIS_TYPE* _this); \
    leResult                     (*setReleasedImage)(THIS_TYPE* _this, leImage* img); \
    leRelativePosition           (*getImagePosition)(const THIS_TYPE* _this); \
    leResult                     (*setImagePosition)(THIS_TYPE* _this, leRelativePosition pos); \
    uint32_t                     (*getImageMargin)(const THIS_TYPE* _this); \
    leResult                     (*setImageMargin)(THIS_TYPE* _this, uint32_t mg); \
    int32_t                      (*getPressedOffset)(const THIS_TYPE* _this); \
    leResult                     (*setPressedOffset)(THIS_TYPE* _this, int32_t offs); \
    leButtonWidget_PressedEvent  (*getPressedEventCallback)(const THIS_TYPE* _this); \
    leResult                     (*setPressedEventCallback)(THIS_TYPE* _this, leButtonWidget_PressedEvent cb); \
    leButtonWidget_ReleasedEvent (*getReleasedEventCallback)(const THIS_TYPE* _this); \
    leResult                     (*setReleasedEventCallback)(THIS_TYPE* _this, leButtonWidget_ReleasedEvent cb); \
    
    
typedef struct leButtonWidgetVTable
{
	LE_BUTTONWIDGET_VTABLE(leButtonWidget)
} leButtonWidgetVTable; 

/**
  * @endcond
  *
  */


// *****************************************************************************
/* Structure:
    leButtonWidget

  Summary:
    Implementation of a button widget.  A button is an interactive element
    that simulates a typical button with a pressed an released state.

  Description:


  Remarks:
    None.
*/
/**
 * @brief This struct represents a button widget.
 * @details Implementation of a button widget.  A button is an interactive
 * element that simulates a typical button with a pressed an released state.
 */
typedef struct leButtonWidget
{
    leWidget widget;  // base widget header

    const leButtonWidgetVTable* fn;

    leButtonState state; // button state
    uint8_t toggleable; // indicates if the button is toggleable

    const leString* string; // the string that holds the button text

    const leImage* pressedImage; // button pressed icon image
    const leImage* releasedImage; // button released icon image

    leRelativePosition imagePosition; // icon position in relation to text
    uint32_t imageMargin; // distance between text and icon
    int32_t pressedOffset; // pressed text offset distance

    leButtonWidget_PressedEvent pressedEvent; // pressed event callback
    leButtonWidget_ReleasedEvent releasedEvent; // released event callback
} leButtonWidget;

// *****************************************************************************
// *****************************************************************************
// Section: Routines
// *****************************************************************************
// *****************************************************************************

/**
 * @brief Create widget.
 * @details Creates a new leButtonWidget and allocates memory for the widget through the
 * current active context.  Application is responsible for managing the widget
 * pointer until the widget is added to a widget tree.
 * @remark use leWidget_Delete() to free this pointer.
 * @code
 * leButtonWidget* wgt = leButtonWidget_New();
 * @endcode
 * @return a widget object pointer.
 */
leButtonWidget* leButtonWidget_New(void);

/**
 * @brief Initialize widget.
 * @details Initializes the leButtonWidget <span class="param">wgt</span>.
 * @code
 * leButtonWidget* wgt;
 * leButtonWidget_Constructor(wgt);
 * @endcode
 * @param wgt is the widget to initialize
 * @return void.
 */
void leButtonWidget_Constructor(leButtonWidget* wgt);

#ifdef _DOXYGEN_
#define THIS_TYPE struct leWidget

/**
 * @brief Get toggle state.
 * @details Gets the toggle state using <span class="param">_this</span>.
 * @code
 * const leButtonWidget* _this;
 * leBool toggled = _this->fn->getToggleable(_this);
 * @endcode
 * @param _this is the widget pointer to query.
 * @returns LE_TRUE if set, otherwise LE_FALSE;
 */
virtual leBool getToggleable(const leButtonWidget* _this);

/**
 * @brief Set togglable setting.
 * @details Sets the togglable setting to
 * <span class="param">toggleable</span> using <span class="param">_this</span>.
 * @code
 * leButtonWidget* _this;
 * leBool toggleable;
 * leResult res = _this->fn->setToggleable(_this, toggleable);
 * @endcode
 * @param _this is the widget pointer to modify.
 * @param toggleable is the toggleable state.
 * @returns LE_SUCCESS if set, otherwise LE_FAILURE.
 */
virtual leResult setToggleable(leButtonWidget* _this,
                               leBool toggleable);

/**
 * @brief Get pressed state.
 * @details Gets the pressed state using <span class="param">_this</span>.
 * @code
 * const leButtonWidget* _this;
 * leBool pressed = _this->fn->getPressed(_this);
 * @endcode
 * @param _this is the widget pointer to query.
 * @returns LE_TRUE if set, otherwise LE_FALSE;
 */
virtual leBool getPressed(const leButtonWidget* _this);

/**
 * @brief Set pressed state.
 * @details Sets the pressed state to
 * <span class="param">pressed</span> using <span class="param">_this</span>.
 * @code
 * leButtonWidget* _this;
 * leBool pressed;
 * leResult res = _this->fn->setPressed(_this, pressed);
 * @endcode
 * @param _this is the widget pointer to modify.
 * @param pressed is the press state.
 * @returns LE_SUCCESS if set, otherwise LE_FAILURE.
 */
virtual leResult setPressed(leButtonWidget* _this,
                            leBool pressed);


/**
 * @brief Get string assigned.
 * @details Gets the string assigned using <span class="param">_this</span>.
 * @code
 * const leButtonWidget* _this;
 * leString* str = _this->fn->getString(_this);
 * @endcode
 * @param _this is the widget pointer to query.
 * @returns the string pointer.
 */
virtual leString* getString(const leButtonWidget* _this);


/**
 * @brief Set string.
 * @details Sets the string to
 * <span class="param">str</span> using <span class="param">_this</span>.
 * @code
 * leButtonWidget* _this;
 * const leString* str;
 * leResult res = _this->fn->setString(_this, str);
 * @endcode
 * @param _this is the widget pointer to modify.
 * @param str is the string pointer.
 * @returns LE_SUCCESS if set, otherwise LE_FAILURE.
 */
virtual leResult setString(leButtonWidget* _this,
                           const leString* str);

/**
 * @brief Get pressed image pointer.
 * @details Gets the pressed image pointer using <span class="param">_this</span>.
 * @code
 * const leButtonWidget* _this;
 * leImage* img = _this->fn->getReleasedImage(_this);
 * @endcode
 * @param _this is the widget pointer to query.
 * @returns the image pointer.
 */
virtual leImage* getPressedImage(const leButtonWidget* _this);


/**
 * @brief Set pressed image pointer.
 * @details Sets the pressed image pointer to
 * <span class="param">img</span> using <span class="param">_this</span>.
 * @code
 * leButtonWidget* _this;
 * leImage* img;
 * leResult res = _this->fn->setReleasedImage(_this, img);
 * @endcode
 * @param _this is the widget pointer to modify.
 * @param img is the image pointer.
 * @returns LE_SUCCESS if set, otherwise LE_FAILURE.
 */
virtual leResult setPressedImage(leButtonWidget* _this,
                                 leImage* img);


/**
 * @brief Get released image pointer.
 * @details Gets the released image pointer using <span class="param">_this</span>.
 * @code
 * const leButtonWidget* _this;
 * leImage* pos = _this->fn->getReleasedImage(_this);
 * @endcode
 * @param _this is the widget pointer to query.
 * @returns the image pointer.
 */
virtual leImage* getReleasedImage(const leButtonWidget* _this);

/**
 * @brief Set released image pointer.
 * @details Sets the released image pointer to
 * <span class="param">img</span> using <span class="param">_this</span>.
 * @code
 * leButtonWidget* _this;
 * leImage* img;
 * leResult res = _this->fn->setReleasedImage(_this, img);
 * @endcode
 * @param _this is the widget pointer to modify.
 * @param img is the image pointer.
 * @returns LE_SUCCESS if set, otherwise LE_FAILURE.
 */
virtual leResult setReleasedImage(leButtonWidget* _this,
                                  leImage* img);


/**
 * @brief Get relative position of the image to the text.
 * @details Gets the relative position of the image to the text
 * using <span class="param">_this</span>.
 * @code
 * const leButtonWidget* _this;
 * leRelativePosition pos = _this->fn->getImagePosition(_this);
 * @endcode
 * @param _this is the widget pointer to query.
 * @returns the relative position.
 */
virtual leRelativePosition getImagePosition(const leButtonWidget* _this);

/**
 * @brief Set relative position of the image to the text.
 * @details Sets the relative position of the image to the text using <span class="param">_this</span> to
 * <span class="param">pos</span>.
 * @code
 * leButtonWidget* _this;
 * leRelativePosition pos;
 * leResult res = _this->fn->setImagePosition(_this, pos);
 * @endcode
 * @param _this is the widget pointer to modify.
 * @param pos the relative position.
 * @returns LE_SUCCESS if set, otherwise LE_FAILURE.
 */
virtual leResult setImagePosition(leButtonWidget* _this,
                                  leRelativePosition pos);

/**
 * @brief Get image margin.
 * @details Gets the image margin using <span class="param">_this</span>.
 * @code
 * const leButtonWidget* _this;
 * uint32_t margin = _this->fn->getImageMargin(_this);
 * @endcode
 * @param _this is the widget pointer to modify.
 * @returns the margin value.
 */
virtual uint32_t getImageMargin(const leButtonWidget* _this);

/**
 * @brief Set image margin.
 * @details Sets the image margin to
 * <span class="param">mg</span> using <span class="param">_this</span>.
 * @code
 * leButtonWidget* _this;
 * uint32_t mg;
 * leResult res = _this->fn->setImageMargin(_this, mg);
 * @endcode
 * @param _this is the widget pointer to modify.
 * @param mg the margin value.
 * @returns LE_SUCCESS if set, otherwise LE_FAILURE.
 */
virtual leResult setImageMargin(leButtonWidget* _this,
                                  uint32_t mg);


/**
 * @brief Get pressed offset.
 * @details Gets the pressed offset to
 * <span class="param">offs</span> using <span class="param">_this</span>.
 * @code
 * leButtonWidget* _this;
 * int32_t offset = _this->fn->getPressedOffset(_this);
 * @endcode
 * @param _this is the widget pointer to modify.
 * @returns pressed offset.
 */
virtual int32_t getPressedOffset(const leButtonWidget* _this);


/**
 * @brief Set pressed offset value.
 * @details Sets the pressed offset value to
 * <span class="param">offs</span> using <span class="param">_this</span>.
 * @code
 * leButtonWidget* _this;
 * int32_t offs;
 * leResult res = _this->fn->setPressedOffset(_this, offs);
 * @endcode
 * @param _this is the widget pointer to modify.
 * @param offs the offset value.
 * @returns LE_SUCCESS if set, otherwise LE_FAILURE.
 */
virtual leResult setPressedOffset(leButtonWidget* _this,
                                  int32_t offs);

/**
 * @brief Get pressed event callback pointer.
 * @details Gets the pressed event callback pointer pointer using <span class="param">_this</span>.
 * @code
 * leButtonWidget* _this;
 * leButtonWidget_PressedEvent cb = _this->fn->getPressedEventCallback(_this);
 * @endcode
 * @param _this is the widget to modify
 * @returns the callback pointer.
 */
virtual leButtonWidget_PressedEvent getPressedEventCallback(const leButtonWidget* _this);

/**
 * @brief Set pressed event callback pointer.
 * @details Sets the pressed event callback pointer to <span class="param">cb</span>
 * using <span class="param">_this</span>.
 * @code
 * leButtonWidget* _this;
 * leButtonWidget_PressedEvent cb;
 * leResult res = _this->fn->setReleasedEventCallback(_this, cb);
 * @endcode
 * @param _this is the widget to modify
 * @param cb is the callback func
 * @returns LE_SUCCESS if set, otherwise LE_FAILURE.
 */
virtual leResult setPressedEventCallback(leButtonWidget* _this,
                                         leButtonWidget_PressedEvent cb);

/**
 * @brief Get released event callback pointer.
 * @details Gets the released event callback pointer pointer using <span class="param">_this</span>.
 * @code
 * leButtonWidget* _this;
 * leButtonWidget_ReleasedEvent cb = _this->fn->getReleasedEventCallback(_this);
 * @endcode
 * @param _this is the widget to modify
 * @returns the callback pointer.
 */
virtual leButtonWidget_ReleasedEvent getReleasedEventCallback(const leButtonWidget* _this);


/**
 * @brief Set released event callback pointer.
 * @details Sets the released event callback pointer to <span class="param">cb</span>
 * using <span class="param">_this</span>.
 * @code
 * leButtonWidget* _this;
 * leButtonWidget_ReleasedEvent cb;
 * leResult res = _this->fn->setReleasedEventCallback(_this, cb);
 * @endcode
 * @param _this is the widget to modify
 * @param cb is the callback func
 * @returns LE_SUCCESS if set, otherwise LE_FAILURE.
 */
virtual leResult setReleasedEventCallback(leButtonWidget* _this,
                                          leButtonWidget_ReleasedEvent cb);

#undef THIS_TYPE
#endif


#endif /* LE_BUTTON_WIDGET_ENABLED */
#endif /* LEGATO_BUTTON_H */

