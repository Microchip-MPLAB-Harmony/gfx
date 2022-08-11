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
    legato_widget_scrollbar.h

  Summary:


  Description:
    This module implements scroll bar widget functions.
*******************************************************************************/

/** \file legato_widget_scrollbar.h
 * @brief Scrollbar functions and definitions.
 *
 * @details This module implements scroll bar widget functions.
 */

#ifndef LEGATO_SCROLLBAR_H
#define LEGATO_SCROLLBAR_H

#include "gfx/legato/common/legato_common.h"

#if LE_SCROLLBAR_WIDGET_ENABLED == 1

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

// *****************************************************************************
/* Enumeration:
    leScrollBarState

  Summary:
    Defines the various scroll bar state values

  Description:


  Remarks:
    None.
*/
/**
 * @brief Defines the various scroll bar state values.
 * @details .
 */
typedef enum leScrollBarState
{
    LE_SCROLLBAR_STATE_NONE,
    LE_SCROLLBAR_STATE_TOP_PRESSED,
    LE_SCROLLBAR_STATE_TOP_INSIDE,
    LE_SCROLLBAR_STATE_BOTTOM_PRESSED,
    LE_SCROLLBAR_STATE_BOTTOM_INSIDE,
    LE_SCROLLBAR_STATE_HANDLE_DOWN
} leScrollBarState;

typedef struct leScrollBarWidget leScrollBarWidget;

// *****************************************************************************
/* Function Pointer:
    leScrollBarWidget_ValueChangedEvent

  Summary:
    Value changed event function callback type
*/
typedef void (* leScrollBarWidget_ValueChangedEvent)(leScrollBarWidget*);

/* internal use only */
/**
  * @cond INTERNAL
  *
  */typedef struct leScrollBarWidget leScrollBarWidget;

#define LE_SCROLLBARWIDGET_VTABLE(THIS_TYPE) \
    LE_WIDGET_VTABLE(THIS_TYPE) \
    \
    leOrientation (*getOrientation)(const THIS_TYPE* _this); \
    leResult      (*setOrientation)(THIS_TYPE* _this, leOrientation align, leBool swapDimensions); \
    uint32_t      (*getMaximumValue)(const THIS_TYPE* _this); \
    leResult      (*setMaximumValue)(THIS_TYPE* _this, uint32_t val); \
    uint32_t      (*getExtentValue)(const THIS_TYPE* _this); \
    leResult      (*setExtentValue)(THIS_TYPE* _this, uint32_t val); \
    uint32_t      (*getScrollPercentage)(const THIS_TYPE* _this); \
    leResult      (*setScrollPercentage)(THIS_TYPE* _this, uint32_t val); \
    uint32_t      (*getScrollValue)(const THIS_TYPE* _this); \
    leResult      (*setScrollValue)(THIS_TYPE* _this, uint32_t val); \
    uint32_t      (*getStepSize)(const THIS_TYPE* _this); \
    leResult      (*setStepSize)(THIS_TYPE* _this, uint32_t val); \
    leResult      (*stepBackward)(THIS_TYPE* _this); \
    leResult      (*stepForward)(THIS_TYPE* _this); \
    leScrollBarWidget_ValueChangedEvent (*getValueChangedEventCallback)(const THIS_TYPE* _this); \
    leResult      (*setValueChangedEventCallback)(THIS_TYPE* _this, leScrollBarWidget_ValueChangedEvent cb); \

typedef struct leScrollBarWidgetVTable
{
    LE_SCROLLBARWIDGET_VTABLE(leScrollBarWidget)
} leScrollBarWidgetVTable;

/**
  * @endcond
  *
  */

// *****************************************************************************
/**
 * @brief This struct represents a scroll bar widget.
 * @details A scroll bar is a widget that is capable of displaying a range
 * and a scroll handle.  The handle can grow and shrink in size depending
 * on the scroll range and visible scroll space and can be interacted with
 * to scroll through the available space.
 */
typedef struct leScrollBarWidget
{
    leWidget widget; // widget base class

    const leScrollBarWidgetVTable* fn;

    leScrollBarState state; // scrollbar input state
    leOrientation alignment; // scroll bar direction

    uint32_t max; // maximum scroll value
    uint32_t extent; // visible space/handle size
    uint32_t value; // current scroll value
    uint32_t step; // discreet scroll stepping value

    leScrollBarWidget_ValueChangedEvent valueChangedEvent; // value changed callback

    lePoint handleDownOffset;
} leScrollBarWidget;

// *****************************************************************************
// *****************************************************************************
// Section: Routines
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/* Function:
    leScrollBarWidget* leScrollBarWidget_New()

  Summary:
    Allocates memory for a new widget of this type.  The application is
    responsible for the managment of this memory until the widget is added to
    a widget tree.

  Description:


  Parameters:

  Returns:
    leScrollBarWidget*

  Remarks:
    Use leWidget_Delete() to free this pointer.
*/
/**
 * @brief Create widget.
 * @details Creates a new leScrollBarWidget and allocates memory for the widget through the
 * current active context.  Application is responsible for managing the widget
 * pointer until the widget is added to a widget tree.
 * @remark use leWidget_Delete() to free this pointer.
 * @code
 * leScrollBarWidget* wgt = leScrollBarWidget_New();
 * @endcode
 * @return a widget object pointer.
 */
leScrollBarWidget* leScrollBarWidget_New(void);

/**
 * @brief Initialize widget.
 * @details Initializes the leScrollBarWidget <span class="param">wgt</span>.
 * @code
 * leScrollBarWidget* wgt;
 * leScrollBarWidget_Constructor(wgt);
 * @endcode
 * @param wgt is the widget to initialize
 * @return void.
 */
void leScrollBarWidget_Constructor(leScrollBarWidget* wgt);

#ifdef _DOXYGEN_
#define THIS_TYPE struct leWidget

// *****************************************************************************
/* Virtual Member Function:
    leOrientation getOrientation(const leScrollBarWidget* _this)

  Summary:
     Gets the scroll bar orientation

  Description:
     Gets the scroll bar orientation

  Parameters:
    const leScrollBarWidget* _this - The scroll bar widget to operate on

  Remarks:
    Usage - _this->fn->getOrientation(_this);

  Returns:
    leOrientation - the orientation value
*/
/**
 * @brief Get scroll bar orientation.
 * @details Gets the scroll bar orientation using <span class="param">_this</span>.
 * @code
 * leScrollBarWidget* _this;
 * leString* str;
 * leOrientation orient = _this->fn->getOrientation(_this);
 * @endcode
 * @param _this is the widget to modify
 * @returns the orientation value.
 */
leOrientation getOrientation(const leScrollBarWidget* _this);

// *****************************************************************************
/* Virtual Member Function:
    leResult setOrientation(leScrollBarWidget* _this,
                            leOrientation align,
                            leBool swapDimensions)

  Summary:
     Sets the scroll bar orientation

  Description:
     Sets the scroll bar orientation

  Parameters:
    leScrollBarWidget* _this - The scroll bar widget to operate on
    leOrientation align - the orientation value
    leBool swapDimensions - swap the width and height values when changing this

  Remarks:
    Usage - _this->fn->setOrientation(_this, align, swapDimensions);

  Returns:
    leResult - the result of the operation
*/
/**
 * @brief Set scroll bar orientation.
 * @details Sets the scroll bar orientation to <span class="param">align</span>
 * with <span class="param">swapDimensions</span>
 * using <span class="param">_this</span>.
 * @code
 * leScrollBarWidget* _this;
 * leOrientation align;
 * leBool swapDimensions;
 * leResult res = _this->fn->setOrientation(_this, align, swapDimensions);
 * @endcode
 * @param _this is the widget to modify
 * @param align the orientation value.
 * @param swapDimensions swap the width and height values when changing this
 * @returns LE_SUCCESS if set, otherwise LE_FAILURE.
 */
virtual leResult setOrientation(leScrollBarWidget* _this,
                                              leOrientation align,
                                              leBool swapDimensions);

// *****************************************************************************
/* Virtual Member Function:
    uint32_t getMaximumValue(const leScrollBarWidget* _this)

  Summary:
     Gets the maximum scroll value

  Description:
     Gets the maximum scroll value

  Parameters:
    const leScrollBarWidget* _this - The scroll bar widget to operate on

  Remarks:
    Usage - _this->fn->getMaximumValue(_this);

  Returns:
    uint32_t - the value
*/
/**
 * @brief Get maximum scroll value.
 * @details Gets the maximum scroll value using <span class="param">_this</span>.
 * @code
 * leScrollBarWidget* _this;
 * uint32_t val = _this->fn->getMaximumValue(_this);
 * @endcode
 * @param _this is the widget to modify
 * @returns the maximum value.
 */
virtual uint32_t getMaximumValue(const leScrollBarWidget* _this);

// *****************************************************************************
/* Virtual Member Function:
    leResult setMaximumValue(leScrollBarWidget* _this,
                             uint32_t val)

  Summary:
     Sets the maximum scroll value

  Description:
     Sets the maximum scroll value

  Parameters:
    leScrollBarWidget* _this - The scroll bar widget to operate on
    uint32_t val - the value

  Remarks:
    Usage - _this->fn->setMaximumValue(_this, val);

  Returns:
    leResult - the result of the operation
*/
/**
 * @brief Set extent value.
 * @details Sets the extent value to <span class="param">val</span>
 * using <span class="param">_this</span>.
 * @code
 * leScrollBarWidget* _this;
 * uint32_t val;
 * leResult res = _this->fn->setMaximumValue(_this, val);
 * @endcode
 * @param _this is the widget to modify
 * @param val the thickness.
 * @returns LE_SUCCESS if set, otherwise LE_FAILURE.
 */
virtual leResult setMaximumValue(leScrollBarWidget* _this,
                                 uint32_t val);

// *****************************************************************************
/* Virtual Member Function:
    uint32_t getExtentValue(const leScrollBarWidget* _this)

  Summary:
     Gets the extent value

  Description:
     Gets the extent value

  Parameters:
    const leScrollBarWidget* _this - The scroll bar widget to operate on

  Remarks:
    Usage - _this->fn->getExtentValue(_this);

  Returns:
    uint32_t - the value
*/
/**
 * @brief Get extent value.
 * @details Gets the extent value using <span class="param">_this</span>.
 * @code
 * leScrollBarWidget* _this;
 * uint32_t val = _this->fn->getExtentValue(_this);
 * @endcode
 * @param _this is the widget to modify
 * @returns the value.
 */
virtual uint32_t getExtentValue(const leScrollBarWidget* _this);

// *****************************************************************************
/* Virtual Member Function:
    leResult setExtentValue(leScrollBarWidget* _this,
                            uint32_t val)

  Summary:
     Sets the extent value

  Description:
     Sets the extent value

  Parameters:
    leScrollBarWidget* _this - The scroll bar widget to operate on
    uint32_t val - the value

  Remarks:
    Usage - _this->fn->setExtentValue(_this, val);

  Returns:
    leResult - the result of the operation
*/
/**
 * @brief Set extent value.
 * @details Sets the extent value to <span class="param">val</span>
 * using <span class="param">_this</span>.
 * @code
 * leScrollBarWidget* _this;
 * uint32_t val;
 * leResult res = _this->fn->setExtentValue(_this, val);
 * @endcode
 * @param _this is the widget to modify
 * @param val the thickness.
 * @returns LE_SUCCESS if set, otherwise LE_FAILURE.
 */
virtual leResult setExtentValue(leScrollBarWidget* _this,
                                uint32_t val);


// *****************************************************************************
/* Virtual Member Function:
    uint32_t getScrollPercentage(const leScrollBarWidget* _this)

  Summary:
     Gets the scroll value as a percentage

  Description:
     Gets the scroll value as a percentage

  Parameters:
    const leScrollBarWidget* _this - The scroll bar widget to operate on

  Remarks:
    Usage - _this->fn->getScrollPercentage(_this);

  Returns:
    uint32_t - the scroll value percentage
*/
/**
 * @brief Get scroll value as a percentage.
 * @details Gets the scroll value as a percentage using <span class="param">_this</span>.
 * @code
 * leScrollBarWidget* _this;
 * uint32_t val = _this->fn->getScrollPercentage(_this);
 * @endcode
 * @param _this is the widget to modify
 * @returns the scroll value percentage.
 */
virtual uint32_t getScrollPercentage(const leScrollBarWidget* _this);


// *****************************************************************************
/* Virtual Member Function:
    leResult setScrollPercentage(leScrollBarWidget* _this,
                                 uint32_t val)

  Summary:
     Sets the scroll value using a percentage

  Description:
     Sets the scroll value using a percentage

  Parameters:
    leScrollBarWidget* _this - The scroll bar widget to operate on
    uint32_t val - the percentage value

  Remarks:
    Usage - _this->fn->setScrollPercentage(_this, val);

  Returns:
    leResult - the result of the operation
*/
/**
 * @brief Set scroll value using a percentage.
 * @details Sets the scroll value using a percentage to <span class="param">val</span>
 * using <span class="param">_this</span>.
 * @code
 * leScrollBarWidget* _this;
 * uint32_t val;
 * leResult res = _this->fn->setScrollPercentage(_this, val);
 * @endcode
 * @param _this is the widget to modify
 * @param val the thickness.
 * @returns LE_SUCCESS if set, otherwise LE_FAILURE.
 */
virtual leResult setScrollPercentage(leScrollBarWidget* _this,
                                uint32_t val);

// *****************************************************************************
/* Virtual Member Function:
    uint32_t getScrollValue(const leScrollBarWidget* _this)

  Summary:
     Gets the scroll value

  Description:
     Gets the scroll value

  Parameters:
    const leScrollBarWidget* _this - The scroll bar widget to operate on

  Remarks:
    Usage - _this->fn->getScrollValue(_this);

  Returns:
    uint32_t - the value
*/
/**
 * @brief Get scroll value.
 * @details Gets the scroll value using <span class="param">_this</span>.
 * @code
 * leScrollBarWidget* _this;
 * uint32_t val = _this->fn->getScrollValue(_this);
 * @endcode
 * @param _this is the widget to modify
 * @returns the scroll value.
 */
virtual uint32_t getScrollValue(const leScrollBarWidget* _this);

// *****************************************************************************
/* Virtual Member Function:
    leResult setScrollValue(leScrollBarWidget* _this,
                            uint32_t val)

  Summary:
     Sets the scroll value

  Description:
     Sets the scroll value

  Parameters:
    leScrollBarWidget* _this - The scroll bar widget to operate on
    uint32_t val - the value

  Remarks:
    Usage - _this->fn->setScrollValue(_this, val);

  Returns:
    leResult - the result of the operation
*/
/**
 * @brief Set scroll value.
 * @details Sets the scroll value to <span class="param">val</span>
 * using <span class="param">_this</span>.
 * @code
 * leScrollBarWidget* _this;
 * uint32_t val;
 * leResult res = _this->fn->setScrollValue(_this, val);
 * @endcode
 * @param _this is the widget to modify
 * @param val the thickness.
 * @returns LE_SUCCESS if set, otherwise LE_FAILURE.
 */
virtual leResult setScrollValue(leScrollBarWidget* _this,
                                uint32_t val);

// *****************************************************************************
/* Virtual Member Function:
    uint32_t getStepSize(const leScrollBarWidget* _this)

  Summary:
     Gets the step size

  Description:
     Gets the step size

  Parameters:
    const leScrollBarWidget* _this - The scroll bar widget to operate on

  Remarks:
    Usage - _this->fn->getStepSize(_this);

  Returns:
    uint32_t - the step size
*/
/**
 * @brief Get step size.
 * @details Gets the step size using <span class="param">_this</span>.
 * @code
 * leScrollBarWidget* _this;
 * uint32_t sz = _this->fn->getStepSize(_this);
 * @endcode
 * @param _this is the widget to modify
 * @returns the step size.
 */
virtual uint32_t getStepSize(const leScrollBarWidget* _this);


// *****************************************************************************
/* Virtual Member Function:
    leResult setStepSize(leScrollBarWidget* _this,
                         uint32_t val)

  Summary:
     Sets the step size

  Description:
     Sets the step size

  Parameters:
    leScrollBarWidget* _this - The scroll bar widget to operate on
    uint32_t val - the step size

  Remarks:
    Usage - _this->fn->setStepSize(_this, val);

  Returns:
    leResult - the result of the operation
*/
/**
 * @brief Set step size.
 * @details Sets the step size to <span class="param">val</span>
 * using <span class="param">_this</span>.
 * @code
 * leScrollBarWidget* _this;
 * uint32_t val;
 * leResult res = _this->fn->setStepSize(_this, val);
 * @endcode
 * @param _this is the widget to modify
 * @param val the thickness.
 * @returns LE_SUCCESS if set, otherwise LE_FAILURE.
 */
virtual leResult setStepSize(leScrollBarWidget* _this,
                                uint32_t val);

// *****************************************************************************
/* Virtual Member Function:
    leResult stepBackward(leScrollBarWidget* _this)

  Summary:
     Steps the value backwards

  Description:
     Steps the value backwards

  Parameters:
    leScrollBarWidget* _this - The scroll bar widget to operate on

  Remarks:
    Usage - _this->fn->stepBackward(_this);

  Returns:
    leResult - the result of the operation
*/
/**
 * @brief Step value backwards.
 * @details Steps the value backwards using <span class="param">_this</span>.
 * @code
 * leScrollBarWidget* _this;
 * leResult res = _this->fn->stepBackward(_this);
 * @endcode
 * @param _this is the widget to modify
 * @returns LE_SUCCESS if set, otherwise LE_FAILURE.
 */
virtual leResult stepBackward(leScrollBarWidget* _this);

// *****************************************************************************
/* Virtual Member Function:
    leResult stepForward(leScrollBarWidget* _this)

  Summary:
     Steps the value forwards

  Description:
     Steps the value forwards

  Parameters:
    leScrollBarWidget* _this - The scroll bar widget to operate on

  Remarks:
    Usage - _this->fn->stepForward(_this);

  Returns:
    leResult - the result of the operation
*/
/**
 * @brief Step value forwards.
 * @details Steps the value forwards using <span class="param">_this</span>.
 * @code
 * leScrollBarWidget* _this;
 * leResult res = _this->fn->stepForward(_this);
 * @endcode
 * @param _this is the widget to modify
 * @returns LE_SUCCESS if set, otherwise LE_FAILURE.
 */
virtual leResult stepForward(leScrollBarWidget* _this);

/**
 * @brief Get value changed event callback pointer.
 * @details Gets the value changed event callback pointer using <span class="param">_this</span>.
 * @code
 * leScrollBarWidget* _this;
 * leScrollBarWidget_ValueChangedEvent cb = _this->fn->getValueChangedEventCallback(_this);
 * @endcode
 * @param _this is the widget to modify
 * @returns LE_SUCCESS if set, otherwise LE_FAILURE.
 */
virtual leScrollBarWidget_ValueChangedEvent getValueChangedEventCallback
                                          (const leScrollBarWidget* _this);

/**
 * @brief Set value changed event callback pointer.
 * @details Sets the value changed event callback pointer to <span class="param">cb</span>
 * using <span class="param">_this</span>.
 * @code
 * leScrollBarWidget* _this;
 * leScrollBarWidget_ValueChangedEvent cb;
 * leResult res = _this->fn->setValueChangedEventCallback(_this, cb);
 * @endcode
 * @param _this is the widget to modify
 * @param cb is the callback func
 * @returns LE_SUCCESS if set, otherwise LE_FAILURE.
 */
virtual leResult setValueChangedEventCallback(leScrollBarWidget* _this,
                                              leScrollBarWidget_ValueChangedEvent cb);

#undef THIS_TYPE
#endif

#ifdef __cplusplus
}
#endif

#endif // LE_SCROLLBAR_WIDGET_ENABLED
#endif /* LEGATO_SCROLLBAR_H */
