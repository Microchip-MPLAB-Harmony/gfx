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
    legato_widget_radiobutton.h

  Summary:
    

  Description:
    This module implements radio button widget functions.
*******************************************************************************/

/** \file legato_widget_radiobutton.h
 * @brief Radiobutton functions and definitions.
 *
 * @details This module implements scroll bar widget functions.
 */

#ifndef LEGATO_RADIOBUTTON_H
#define LEGATO_RADIOBUTTON_H

#include "gfx/legato/common/legato_common.h"

#if LE_RADIOBUTTON_WIDGET_ENABLED == 1

#include "gfx/legato/image/legato_image.h"
#include "gfx/legato/widget/legato_widget.h"
#include "gfx/legato/widget/radiobutton/legato_radiobutton_group.h"
#include "gfx/legato/string/legato_string.h"

typedef struct leRadioButtonWidget leRadioButtonWidget;
typedef struct leRadioButtonGroup leRadioButtonGroup;

// *****************************************************************************
/* Function Pointer:
    leRadioButtonWidget_SelectedEvent

  Summary:
    Radio button selected function callback type
*/
/**
 * @brief This function represents a select event callback.
 * @details Select event callback is used indicate a radio button
 * selection.
 * @details .
 */
typedef void (*leRadioButtonWidget_SelectedEvent)(leRadioButtonWidget*);

// *****************************************************************************
/* Function Pointer:
    leRadioButtonWidget_DeselectedEvent

  Summary:
    Radio button deselected function callback type
*/
/**
 * @brief This function represents a deselect event callback.
 * @details Deselect event callback is used indicate a radio button
 * deselection.
 * @details .
 */
typedef void (*leRadioButtonWidget_DeselectedEvent)(leRadioButtonWidget*);

// *****************************************************************************
// *****************************************************************************
// Section: Data Types and Constants
// *****************************************************************************
// *****************************************************************************

/* internal use only */
/**
  * @cond INTERNAL
  *
  */typedef struct leRadioButtonWidget leRadioButtonWidget;

#define LE_RADIOBUTTONWIDGET_VTABLE(THIS_TYPE) \
    LE_WIDGET_VTABLE(THIS_TYPE) \
    \
    leRadioButtonGroup* (*getGroup)(const THIS_TYPE* _this); \
    leBool              (*getSelected)(const THIS_TYPE* _this); \
    leResult            (*setSelected)(THIS_TYPE* _this); \
    void                (*select)(THIS_TYPE* _this); \
    void                (*deselect)(THIS_TYPE* _this); \
    leString*           (*getString)(const THIS_TYPE* _this); \
    leResult            (*setString)(THIS_TYPE* _this, const leString* str); \
    leImage*            (*getSelectedImage)(const THIS_TYPE* _this); \
    leResult            (*setSelectedImage)(THIS_TYPE* _this, const leImage* img); \
    leImage*            (*getUnselectedImage)(const THIS_TYPE* _this); \
    leResult            (*setUnselectedImage)(THIS_TYPE* _this, const leImage* img); \
    leRelativePosition  (*getImagePosition)(const THIS_TYPE* _this); \
    leResult            (*setImagePosition)(THIS_TYPE* _this, leRelativePosition pos); \
    uint32_t            (*getImageMargin)(const THIS_TYPE* _this); \
    leResult            (*setImageMargin)(THIS_TYPE* _this, uint32_t mg); \
    leRadioButtonWidget_SelectedEvent (*getSelectedEventCallback)(const THIS_TYPE* _this); \
    leResult            (*setSelectedEventCallback)(THIS_TYPE* _this, leRadioButtonWidget_SelectedEvent cb); \
    leRadioButtonWidget_DeselectedEvent (*getDeselectedEventCallback)(const THIS_TYPE* _this); \
    leResult            (*setDeselectedEventCallback)(THIS_TYPE* _this, leRadioButtonWidget_DeselectedEvent cb); \
    uint32_t            (*getCircleButtonSize)(const THIS_TYPE* _this); \
    leResult            (*setCircleButtonSize)(THIS_TYPE* _this, uint32_t sz); \
    
typedef struct leRadioButtonWidgetVTable
{
	LE_RADIOBUTTONWIDGET_VTABLE(leRadioButtonWidget)
} leRadioButtonWidgetVTable; 

    /**
      * @endcond
      *
      */

// *****************************************************************************
/* Enumeration:
    leRadioButtonWidget

  Summary:
    Implementation of a radio button widget struct

  Description:
    A radio button is similar to a checkbox widget in that it has an on and
    off state.  It is further capable of being added to a radio button group.
    This group provides a mutually exclusive selection capability so that
    only one radio button may be selected at any one time.

  Remarks:
    None.
*/
/**
 * @brief This struct represents a radio button widget.
 * @details A radio button is similar to a checkbox widget in that it has an on
 * and off state.  It is further capable of being added to a radio button group. This
 * group provides a mutually exclusive selection capability so that only one
 * radio button may be selected at any one time.
 */
typedef struct leRadioButtonWidget
{
    leWidget widget; // widget base class

    const leRadioButtonWidgetVTable* fn;

    leBool selected; // indicates if the radio button is selected

    const leString* string; // radio button text

    const leImage* selectedImage; // button custom selected image
    const leImage* unselectedImage; // buton custom unselected image
    
    leRelativePosition imagePosition; // image icon relative position
    
    uint32_t imageMargin; // image margin
    uint32_t circleButtonSize; // size of radio circle button in pixels

    leRadioButtonWidget_SelectedEvent selectedEvent; // button selected event callback
    leRadioButtonWidget_DeselectedEvent deselectedEvent; // button deselected event callback

    leRadioButtonGroup* group; // radio button group
} leRadioButtonWidget;


// *****************************************************************************
// *****************************************************************************
// Section: Routines
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/* Function:
    leRadioButtonWidget* leRadioButtonWidget_New()

  Summary:
    Allocates memory for a new widget of this type.  The application is
    responsible for the managment of this memory until the widget is added to
    a widget tree.

  Description:
    

  Parameters:
    
  Returns:
    leRadioButtonWidget*
    
  Remarks:
    Use leWidget_Delete() to free this pointer.
*/
/**
 * @brief Create widget.
 * @details Creates a new leRadioButtonWidget and allocates memory for the widget through the
 * current active context.  Application is responsible for managing the widget
 * pointer until the widget is added to a widget tree.
 * @remark use leWidget_Delete() to free this pointer.
 * @code
 * leRadioButtonWidget* wgt = leRadioButtonWidget_New();
 * @endcode
 * @return a widget object pointer.
 */
leRadioButtonWidget* leRadioButtonWidget_New(void);

/**
 * @brief Initialize widget.
 * @details Initializes the leRadioButtonWidget <span class="param">wgt</span>.
 * @code
 * leRadioButtonWidget* wgt;
 * leRadioButtonWidget_Constructor(wgt);
 * @endcode
 * @param wgt is the widget to initialize
 * @return void.
 */
void leRadioButtonWidget_Constructor(leRadioButtonWidget* wgt);


#ifdef _DOXYGEN_
#define THIS_TYPE struct leWidget


// *****************************************************************************
/* Virtual Member Function:
    leRadioButtonGroup* getGroup(const leRadioButtonWidget* _this)

  Summary:
     Gets the button's group pointer

  Description:
     Gets the button's group pointer

  Parameters:
    const leRadioButtonWidget* _this - The radio button widget to operate on

  Remarks:
    Usage - _this->fn->getGroup(_this);

  Returns:
    leRadioButtonGroup* - the radio button group
*/
/**
 * @brief Get button's group pointe.
 * @details Gets the button's group pointer using <span class="param">_this</span>.
 * @code
 * leRadioButtonWidget* _this;
 * leRadioButtonGroup* ptr = wgt->fn->getGroup(_this);
 * @endcode
 * @param _this is the widget to modify
 * @returns the radio button group;
 */
virtual leBool getSelected(const leRadioButtonWidget* _this);

// *****************************************************************************
/* Virtual Member Function:
    leBool getSelected(const leRadioButtonWidget* _this)

  Summary:
     Gets the button's selection state

  Description:
     Gets the button's selection state

  Parameters:
    const leRadioButtonWidget* _this - The radio button widget to operate on

  Remarks:
    Usage - _this->fn->getSelected(_this);

  Returns:
    leBool - the selected state
*/
/**
 * @brief Set selected.
 * @details Set selected using <span class="param">_this</span>.
 * @code
 * leRadioButtonWidget* _this;
 * leBool selected = wgt->fn->getSelected(_this);
 * @endcode
 * @param _this is the widget to modify
 * @returns LE_TRUE if set, otherwise LE_FALSE;
 */
virtual leBool getSelected(const leRadioButtonWidget* _this);


// *****************************************************************************
/* Virtual Member Function:
    leResult setSelected(leRadioButtonWidget* _this)

  Summary:
     Selects the button

  Description:
     Selects the button

  Parameters:
    leRadioButtonWidget* _this - The radio button widget to operate on

  Remarks:
    Usage - _this->fn->setSelected(_this);

  Returns:
    leResult - the result of the operation
*/
/**
 * @brief Set selected.
 * @details Set selected using <span class="param">_this</span>.
 * @code
 * leRadioButtonWidget* _this;
 * leResult res = wgt->fn->setSelected(_this);
 * @endcode
 * @param _this is the widget to modify
 * @returns LE_SUCCESS if set, otherwise LE_FAILURE.
 */
virtual leResult setSelected(leRadioButtonWidget* _this);


// *****************************************************************************
/* Virtual Member Function:
    void selected(leRadioButtonWidget* _this)

  Summary:
     internal use only
*/

// *****************************************************************************
/* Virtual Member Function:
    void deselected(leRadioButtonWidget* _this)

  Summary:
     internal use only
*/

// *****************************************************************************
/* Virtual Member Function:
    leString* getString(const leRadioButtonWidget* _this)

  Summary:
     Gets the text string pointer

  Description:
     Gets the text string pointer

  Parameters:
    const leRadioButtonWidget* _this - The radio button widget to operate on

  Remarks:
    Usage - _this->fn->getString(_this);

  Returns:
    leString* - the string pointer
*/
/**
 * @brief Get text string pointer.
 * @details Gets the text string pointer using <span class="param">_this</span>.
 * @code
 * leRadioButtonWidget* _this;
 * leString* str = wgt->fn->getString(_this);
 * @endcode
 * @param _this is the widget to modify
 * @returns the string pointer;
 */
virtual leString* getString(const leRadioButtonWidget* _this);

// *****************************************************************************
/* Virtual Member Function:
    leResult setString(leRadioButtonWidget* _this,
                       const leString* str)

  Summary:
     Sets the text string pointer

  Description:
     Sets the text string pointer

  Parameters:
    leRadioButtonWidget* _this - The radio button widget to operate on
    const leString* str - the string pointer

  Remarks:
    Usage - _this->fn->setString(_this, str);

  Returns:
    leResult - the result of the operation
*/
/**
 * @brief Set text string pointer.
 * @details Sets the text string pointer to <span class="param">str</span>
 * using <span class="param">_this</span>.
 * @code
 * leRadioButtonWidget* _this;
 * leString* str;
 * leResult res = wgt->fn->setString(_this, str);
 * @endcode
 * @param _this is the widget to modify
 * @param str the string pointer.
 * @returns LE_SUCCESS if set, otherwise LE_FAILURE.
 */
virtual leResult setString(leRadioButtonWidget* _this,
                           const leString* str);


// *****************************************************************************
/* Virtual Member Function:
    leImage* getSelectedImage(const leRadioButtonWidget* _this)

  Summary:
     Gets the selected image pointer

  Description:
     Gets the selected image pointer

  Parameters:
    const leRadioButtonWidget* _this - The radio button widget to operate on

  Remarks:
    Usage - _this->fn->getSelectedImage(_this);

  Returns:
    leImage* - the image pointer
*/
/**
 * @brief Get text string pointer.
 * @details Gets the text string pointer using <span class="param">_this</span>.
 * @code
 * leRadioButtonWidget* _this;
 * leString* str = wgt->fn->getString(_this);
 * @endcode
 * @param _this is the widget to modify
 * @returns the string pointer;
 */
virtual leString* getString(const leRadioButtonWidget* _this);


// *****************************************************************************
/* Virtual Member Function:
    leResult setSelectedImage(leRadioButtonWidget* _this,
                              const leImage* img)

  Summary:
     Sets the selected image pointer

  Description:
     Sets the selected image pointer

  Parameters:
    leRadioButtonWidget* _this - The radio button widget to operate on
    const leImage* img - the image pointer

  Remarks:
    Usage - _this->fn->setSelectedImage(_this, img);

  Returns:
    leResult - the result of the operation
*/
/**
 * @brief Set selected image pointer.
 * @details Sets the selected image pointer to <span class="param">img</span>
 * using <span class="param">_this</span>.
 * @code
 * leRadioButtonWidget* _this;
 * leImage* img;
 * leResult res = wgt->fn->setSelectedImage(_this, img);
 * @endcode
 * @param _this is the widget to modify
 * @param img the image pointer.
 * @returns LE_SUCCESS if set, otherwise LE_FAILURE.
 */
virtual leResult setSelectedImage(leRadioButtonWidget* _this,
                                  const leImage* img);


// *****************************************************************************
/* Virtual Member Function:
    leImage* getUnselectedImage(const leRadioButtonWidget* _this)

  Summary:
     Gets the unselected image pointer

  Description:
     Gets the unselected image pointer

  Parameters:
    const leRadioButtonWidget* _this - The radio button widget to operate on

  Remarks:
    Usage - _this->fn->getUnselectedImage(_this);

  Returns:
    leImage* - the image pointer
*/
/**
 * @brief Get unselected image pointer.
 * @details Gets the unselected image pointer using <span class="param">_this</span>.
 * @code
 * leRadioButtonWidget* _this;
 * leImage* img = wgt->fn->getUnselectedImage(_this);
 * @endcode
 * @param _this is the widget to modify
 * @returns the image pointer;
 */
virtual leImage* getUnselectedImage(const leRadioButtonWidget* _this);

// *****************************************************************************
/* Virtual Member Function:
    leResult setUnselectedImage(leRadioButtonWidget* _this,
                                const leImage* img)

  Summary:
     Sets the unselected image pointer

  Description:
     Sets the unselected image pointer

  Parameters:
    leRadioButtonWidget* _this - The radio button widget to operate on
    const leImage* img - the image pointer

  Remarks:
    Usage - _this->fn->setUnselectedImage(_this, img);

  Returns:
    leResult - the result of the operation
*/
/**
 * @brief Set image position.
 * @details Sets the image position to <span class="param">pos</span>
 * using <span class="param">_this</span>.
 * @code
 * leRadioButtonWidget* _this;
 * leImage* img;
 * leResult res = wgt->fn->setUnselectedImage(_this, img);
 * @endcode
 * @param _this is the widget to modify
 * @param img the image pointer.
 * @returns LE_SUCCESS if set, otherwise LE_FAILURE.
 */
virtual leResult setUnselectedImage(leRadioButtonWidget* _this,
                                    const leImage* img);

// *****************************************************************************
/* Virtual Member Function:
    leRelativePosition getImagePosition(const leRadioButtonWidget* _this)

  Summary:
     Gets the image position

  Description:
     Gets the image position

  Parameters:
    const leRadioButtonWidget* _this - The radio button widget to operate on

  Remarks:
    Usage - _this->fn->getImagePosition(_this);

  Returns:
    leRelativePosition - the position
*/
/**
 * @brief Get unselected image pointer.
 * @details Gets the unselected image pointer using <span class="param">_this</span>.
 * @code
 * leRadioButtonWidget* _this;
 * leRelativePosition pos = wgt->fn->getImagePosition(_this);
 * @endcode
 * @param _this is the widget to modify
 * @returns the position;
 */
virtual leRelativePosition getImagePosition(const leRadioButtonWidget* _this);

// *****************************************************************************
/* Virtual Member Function:
    leResult setImagePosition(leRadioButtonWidget* _this,
                              leRelativePosition pos)

  Summary:
     Sets the image position

  Description:
     Sets the image position

  Parameters:
    leRadioButtonWidget* _this - The radio button widget to operate on
    leRelativePosition pos - the position value

  Remarks:
    Usage - _this->fn->setImagePosition(_this, pos);

  Returns:
    leResult - the result of the operation
*/
/**
 * @brief Set image position.
 * @details Sets the image position to <span class="param">pos</span>
 * using <span class="param">_this</span>.
 * @code
 * leRadioButtonWidget* _this;
 * leRelativePosition pos;
 * leResult res = wgt->fn->setImagePosition(_this, pos);
 * @endcode
 * @param _this is the widget to modify
 * @param pos the position value.
 * @returns LE_SUCCESS if set, otherwise LE_FAILURE.
 */
virtual leResult setImagePosition(leRadioButtonWidget* _this,
                                  leRelativePosition pos);


// *****************************************************************************
/* Virtual Member Function:
    uint32_t getImageMargin(const leRadioButtonWidget* _this)

  Summary:
     Gets the image margin

  Description:
     Gets the image margin

  Parameters:
    const leRadioButtonWidget* _this - The radio button widget to operate on

  Remarks:
    Usage - _this->fn->getImageMargin(_this);

  Returns:
    uint32_t - the margin value
*/
/**
 * @brief Get image margin.
 * @details Gets the image margin using <span class="param">_this</span>.
 * @code
 * leWindowWidget* _this;
 * uint32_t margin = wgt->fn->getCircleButtonSize(_this);
 * @endcode
 * @param _this is the widget to query
 * @return the margin value.
 */
virtual uint32_t getImageMargin(const leRadioButtonWidget* _this);

// *****************************************************************************
/* Virtual Member Function:
    leResult setImageMargin(leRadioButtonWidget* _this,
                            uint32_t mg)

  Summary:
     Sets the image margin

  Description:
     Sets the image margin

  Parameters:
    leRadioButtonWidget* _this - The radio button widget to operate on
    uint32_t mg - the margin value

  Remarks:
    Usage - _this->fn->setImageMargin(_this, mg);

  Returns:
    leResult - the result of the operation
*/
/**
 * @brief Set image margin.
 * @details Sets the image margin to <span class="param">mg</span>
 * using <span class="param">_this</span>.
 * @code
 * leRadioButtonWidget* _this;
 * uint32_t sz;
 * leResult res = wgt->fn->setCircleButtonSize(_this, mg);
 * @endcode
 * @param _this is the widget to modify
 * @param mg the margin value.
 * @returns LE_SUCCESS if set, otherwise LE_FAILURE.
 */
virtual leResult setImageMargin(leRadioButtonWidget* _this,
                        uint32_t mg);

// *****************************************************************************
/**
 * @brief Get circle button size.
 * @details Gets the circle button size using <span class="param">_this</span>.
 * @code
 * leWindowWidget* _this;
 * uint32_t sz = wgt->fn->getCircleButtonSize(_this);
 * @endcode
 * @param _this is the widget to query
 * @return the circle button size.
 */
uint32_t getCircleButtonSize(const leRadioButtonWidget* _this);

// *****************************************************************************
/**
 * @brief Set circle button size.
 * @details Sets circle button size to <span class="param">sz</span>
 * using <span class="param">wgt</span>.
 * @code
 * leRadioButtonWidget* wgt;
 * uint32_t sz;
 * leResult res = wgt->fn->setCircleButtonSize(_this, sz);
 * @endcode
 * @param _this is the widget to modify
 * @param sz the size value.
 * @returns LE_SUCCESS if set, otherwise LE_FAILURE.
 */
virtual leResult setCircleButtonSize(leRadioButtonWidget* _this,
                                     uint32_t sz);

/**
 * @brief Get deselected event callback pointer.
 * @details Gets the deselected event callback pointer using <span class="param">_this</span>.
 * @code
 * leRadioButtonWidget* _this;
 * leRadioButtonWidget_SelectedEvent cb = _this->fn->getDeselectedEventCallback(_this);
 * @endcode
 * @param _this is the widget to modify
 * @returns LE_SUCCESS if set, otherwise LE_FAILURE.
 */
virtual leRadioButtonWidget_SelectedEvent getSelectedEventCallback
                                        (const leRadioButtonWidget* _this);

/**
 * @brief Set deselected event callback pointer.
 * @details Sets the deselected event callback pointer to <span class="param">cb</span>
 * using <span class="param">_this</span>.
 * @code
 * leRadioButtonWidget* _this;
 * leRadioButtonWidget_SelectedEvent cb;
 * leResult res = _this->fn->setSelectedEventCallback(_this, cb);
 * @endcode
 * @param _this is the widget to modify
 * @param cb is the callback func
 * @returns LE_SUCCESS if set, otherwise LE_FAILURE.
 */
virtual leResult setSelectedEventCallback(leRadioButtonWidget* _this,
                                            leRadioButtonWidget_SelectedEvent cb);

/**
 * @brief Get deselected event callback pointer.
 * @details Gets the deselected event callback pointer using <span class="param">_this</span>.
 * @code
 * leRadioButtonWidget* _this;
 * leRadioButtonWidget_DeselectedEvent cb = _this->fn->getDeselectedEventCallback(_this);
 * @endcode
 * @param _this is the widget to modify
 * @returns LE_SUCCESS if set, otherwise LE_FAILURE.
 */
virtual leRadioButtonWidget_DeselectedEvent getDeselectedEventCallback
                                        (const leRadioButtonWidget* _this);

/**
 * @brief Set deselected event callback pointer.
 * @details Sets the deselected event callback pointer to <span class="param">cb</span>
 * using <span class="param">_this</span>.
 * @code
 * leRadioButtonWidget* _this;
 * leRadioButtonWidget_DeselectedEvent cb;
 * leResult res = _this->fn->setDeselectedEventCallback(_this, cb);
 * @endcode
 * @param _this is the widget to modify
 * @param cb is the callback func
 * @returns LE_SUCCESS if set, otherwise LE_FAILURE.
 */
virtual leResult setDeselectedEventCallback(leRadioButtonWidget* _this,
                                            leRadioButtonWidget_DeselectedEvent cb);

#undef THIS_TYPE
#endif

#endif // LE_RADIOBUTTON_WIDGET_ENABLED
#endif /* LEGATO_RADIOBUTTON_H */
