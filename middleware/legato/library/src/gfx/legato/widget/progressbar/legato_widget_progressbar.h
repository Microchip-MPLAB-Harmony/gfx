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
    legato_widget_progressbar.h

  Summary:


  Description:
    This module implements progress bar widget functions.
*******************************************************************************/
/** \file legato_widget_progressbar.h
 * @brief Progressbar menu functions and definitions.
 *
 * @details This module implements progressbar widget functions.
 */

#ifndef LEGATO_PROGRESSBAR_H
#define LEGATO_PROGRESSBAR_H

#include "gfx/legato/common/legato_common.h"

#if LE_PROGRESSBAR_WIDGET_ENABLED == 1

#ifdef __cplusplus
extern "C" {
#endif

#include "gfx/legato/widget/legato_widget.h"

// *****************************************************************************
// *****************************************************************************
// Section: Data Types and Constants
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/* Enumeration:
    leProgressBarDirection

  Summary:
    Defines the valid values for the progress bar widget fill directions.

  Description:


  Remarks:
    None.
*/
/**
 * @brief This enum represents progress bar direction.
 * @details Used to list the possible progress bar direction modes.
 * @details .
 */
typedef enum leProgressBarDirection
{
    LE_PROGRESSBAR_DIRECTION_RIGHT,
    LE_PROGRESSBAR_DIRECTION_UP,
    LE_PROGRESSBAR_DIRECTION_LEFT,
    LE_PROGRESSBAR_DIRECTION_DOWN
} leProgressBarDirection;

/* internal use only */
/**
  * @cond INTERNAL
  *
  */typedef struct leProgressBarWidget leProgressBar;
// DOM-IGNORE-END

// *****************************************************************************
/* Function Pointer:
    leProgressBar_ValueChangedEventCallback

  Summary:
    Value changed event function callback type
*/
/**
 * @brief Used to define a value changed event callback function.
 * @details .
 */
/**
 * @brief This function represents a value change event callback.
 * @details The callback is used indicate a value has changed.
 * @details .
 */
typedef void (* leProgressBar_ValueChangedEventCallback)(leProgressBar*, uint32_t);

/* internal use only */
/**
  * @cond INTERNAL
  *
  */typedef struct leProgressBarWidget leProgressBarWidget;

#define LE_PROGRESSBARWIDGET_VTABLE(THIS_TYPE) \
    LE_WIDGET_VTABLE(THIS_TYPE) \
    \
    leProgressBarDirection (*getDirection)(const THIS_TYPE* _this); \
    leResult               (*setDirection)(THIS_TYPE* _this, leProgressBarDirection dir); \
    uint32_t               (*getValue)(const THIS_TYPE* _this); \
    leResult               (*setValue)(THIS_TYPE* _this, uint32_t value); \
    leProgressBar_ValueChangedEventCallback (*getValueChangedEventCallback)(const THIS_TYPE* _this); \
    leResult               (*setValueChangedCallback)(THIS_TYPE* _this, leProgressBar_ValueChangedEventCallback cb); \

typedef struct leProgressBarWidgetVTable
{
    LE_PROGRESSBARWIDGET_VTABLE(leProgressBarWidget)
} leProgressBarWidgetVTable;

/**
  * @endcond
  *
  */

// *****************************************************************************
/**
 * @brief This struct represents a progressbar widget struct
 * @details A progress bar widget is a widget that can fill itself with a color
 * based on a given percentage from 0-100.  This is often used to visually
 * illustrate the progress of some other activity over time.
 */
typedef struct leProgressBarWidget
{
    leWidget widget; // base widget class

    const leProgressBarWidgetVTable* fn;

    leProgressBarDirection direction; // the fill direction of the bar

    uint32_t value; // fill percentage

    leProgressBar_ValueChangedEventCallback cb; // value changed callback
} leProgressBarWidget;


// *****************************************************************************
// *****************************************************************************
// Section: Routines
// *****************************************************************************
// *****************************************************************************

/**
 * @brief Create widget.
 * @details Creates a new leProgressBarWidget and allocates memory for the widget through the
 * current active context.  Application is responsible for managing the widget
 * pointer until the widget is added to a widget tree.
 * @remark use leWidget_Delete() to free this pointer.
 * @code
 * leProgressBarWidget* wgt = leProgressBarWidget_New();
 * @endcode
 * @return a widget object pointer.
 */
leProgressBarWidget* leProgressBarWidget_New(void);

/**
 * @brief Initialize widget.
 * @details Initializes the leProgressBarWidget <span class="param">wgt</span>.
 * @code
 * leProgressBarWidget* wgt;
 * leProgressBarWidget_Constructor(wgt);
 * @endcode
 * @param wgt is the widget to initialize
 * @return void.
 */
void leProgressBarWidget_Constructor(leProgressBarWidget* wgt);

#ifdef _DOXYGEN_
#define THIS_TYPE struct leWidget

// *****************************************************************************
/**
 * @brief Get current direction.
 * @details Gets current direction using <span class="param">_this</span>.
 * @code
 * const leProgressBarWidget* _this;
 * leProgressBarDirection dir = _this->fn->getDirection(_this);
 * @endcode
 * @param _this is the widget pointer to query.
 * @returns current direction.
 */
virtual leProgressBarDirection getDirection(const leProgressBarWidget* _this);


// *****************************************************************************
/**
 * @brief Set progress bar fill direction.
 * @details Sets the progress bar fill directionto
 * <span class="param">stk</span> using <span class="param">_this</span>.
 * @code
 * const leProgressBarWidget* _this;
 * leProgressBarDirection dir;
 * leResult tch = _this->fn->setDirection(_this, dir);
 * @endcode
 * @param _this is the widget pointer to query.
 * @param dir is the setting.
 * @returns LE_SUCCESS if dir, otherwise LE_FAILURE.
 */
virtual leResult setDirection(leProgressBarWidget* _this,
                              leProgressBarDirection dir);




// *****************************************************************************
/**
 * @brief Get progress bar value.
 * @details Gets the progress bar value using <span class="param">_this</span>.
 * @code
 * const leProgressBarWidget* _this;
 * int32_t value = _this->fn->getValue(_this);
 * @endcode
 * @param _this is the widget pointer to query.
 * @returns the gauge value.
 */
virtual int32_t getValue(const leProgressBarWidget* _this);


// *****************************************************************************
/**
 * @brief Set progress bar value.
 * @details Sets the progress bar value to
 * <span class="param">value</span> using <span class="param">_this</span>.
 * @code
 * const leProgressBarWidget* _this;
 * int32_t value;
 * leResult res = _this->fn->setValue(_this, value);
 * @endcode
 * @param _this is the widget pointer to query.
 * @param value is the gauge value.
 * @returns LE_SUCCESS if set, otherwise LE_FAILURE.
 */
virtual leResult setValue(leProgressBarWidget* _this,
                          uint32_t value);


/**
 * @brief Get value changed event callback pointer.
 * @details Gets the value changed event callback pointer using <span class="param">_this</span>.
 * @code
 * leProgressBarWidget* _this;
 * leProgressBar_ValueChangedEventCallback cb = _this->fn->getValueChangedEventCallback(_this);
 * @endcode
 * @param _this is the widget to modify
 * @returns LE_SUCCESS if set, otherwise LE_FAILURE.
 */
virtual leProgressBar_ValueChangedEventCallback getValueChangedEventCallback
                                        (const leProgressBarWidget* _this);

/**
 * @brief Set value changed event callback pointer.
 * @details Sets the value changed event callback pointer to <span class="param">cb</span>
 * using <span class="param">_this</span>.
 * @code
 * leProgressBarWidget* _this;
 * leProgressBar_ValueChangedEventCallback cb;
 * leResult res = _this->fn->setValueChangedCallback(_this, cb);
 * @endcode
 * @param _this is the widget to modify
 * @param cb is the callback func
 * @returns LE_SUCCESS if set, otherwise LE_FAILURE.
 */
virtual leResult setValueChangedCallback(leProgressBarWidget* _this,
                                         leProgressBar_ValueChangedEventCallback cb);

#undef THIS_TYPE
#endif

#ifdef __cplusplus
}
#endif

#endif // LE_PROGRESSBAR_WIDGET_ENABLED
#endif /* LEGATO_PROGRESSBAR_H */
