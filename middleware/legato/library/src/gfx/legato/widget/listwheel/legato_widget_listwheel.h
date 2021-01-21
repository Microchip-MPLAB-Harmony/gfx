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
    legato_widget_listwheel.h

  Summary:


  Description:
    This module implements list wheel (drawing-style list box) widget functions.
*******************************************************************************/

/** \file legato_widget_listwheel.h
 * @brief List wheel functions and definitions.
 *
 * @details This module implements list wheel (drawing-style list box) widget
 * functions.
 */
#ifndef LEGATO_LISTWHEELWIDGET_H
#define LEGATO_LISTWHEELWIDGET_H

#include "gfx/legato/common/legato_common.h"

#if LE_LISTWHEEL_WIDGET_ENABLED == 1

//#include "gfx/legato/datastructure/legato_list.h"
#include "gfx/legato/image/legato_image.h"
#include "gfx/legato/widget/legato_widget.h"
#include "gfx/legato/string/legato_string.h"

typedef struct leListWheelWidget leListWheelWidget;

// *****************************************************************************
/* Function Pointer:
    leListWheelWidget_SelectedItemChangedEvent

  Summary:
    Selected item changed event function callback type
*/
/**
 * @brief Used to define a item changed event callback function.
 * @details .
 */
typedef void (*leListWheelWidget_SelectedItemChangedEvent)(leListWheelWidget*,
                                                           int32_t idx);

// *****************************************************************************
// *****************************************************************************
// Section: Data Types and Constants
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/* Enumeration:
    leListWheelIndicatorFill

  Summary:
    Indicates the fill type for the listwheel indicator area.
*/
/**
 * @brief Defines the fill type for the listwheel indicator area.
 * @details .
 */
typedef enum leListWheelIndicatorFill
{
    LE_LISTWHEEL_INDICATOR_FILL_NONE,
    LE_LISTWHEEL_INDICATOR_FILL_SOLID,
    LE_LISTWHEEL_INDICATOR_FILL_GRADIENT,
} leListWheelIndicatorFill;

// *****************************************************************************
/* Enumeration:
    leListWheelEffects

  Summary:
    Indicates the zoom effects for the list wheel items.
*/
/**
 * @brief Defines the zoom effects for the list wheel items.
 * @details .
 */
typedef enum leListWheelZoomEffects
{
    LE_LISTWHEEL_ZOOM_EFFECT_NONE,
    LE_LISTWHEEL_ZOOM_EFFECT_VSCALE,
    LE_LISTWHEEL_ZOOM_EFFECT_FULL_SCALE,
    LE_LISTWHEEL_ZOOM_EFFECT_FIXED_SCALE,
} leListWheelZoomEffects;

// *****************************************************************************
/* Structure:
    leListWheelItem

  Summary:
    Implementation of a list wheel widget item struct

  Description:
    A list wheel item contains either a text string, an icon, or both

  Remarks:
    None.
*/
/**
 * @brief Defines list wheel item possibilities.
 * @details .
 */
typedef struct leListWheelItem
{
    const leString* string;
    const leImage* icon;
} leListWheelItem;

/* internal use only */
/**
  * @cond INTERNAL
  *
  */
typedef struct leListWheelWidget leListWheelWidget;

#define LE_LISTWHEELWIDGET_VTABLE(THIS_TYPE) \
    LE_WIDGET_VTABLE(THIS_TYPE) \
    \
    uint32_t            (*getVisibleItemCount)(const THIS_TYPE* _this); \
    leResult            (*setVisibleItemCount)(THIS_TYPE* _this, uint32_t cnt); \
    leRelativePosition  (*getIconPosition)(const THIS_TYPE* _this); \
    leResult            (*setIconPosition)(THIS_TYPE* _this, leRelativePosition pos); \
    uint32_t            (*getIconMargin)(const THIS_TYPE* _this); \
    leResult            (*setIconMargin)(THIS_TYPE* _this, uint32_t mg); \
    leBool              (*getShowIndicators)(const THIS_TYPE* _this); \
    leResult            (*setShowIndicators)(THIS_TYPE* _this, leBool b); \
    uint32_t            (*getIndicatorArea)(const THIS_TYPE* _this); \
    leResult            (*setIndicatorArea)(THIS_TYPE* _this, uint32_t area); \
    leBool              (*getShaded)(const THIS_TYPE* _this); \
    leResult            (*setShaded)(THIS_TYPE* _this, leBool b); \
    uint32_t            (*getFlickInitSpeed)(const THIS_TYPE* _this); \
    leResult            (*setFlickInitSpeed)(THIS_TYPE* _this, uint32_t sp); \
    uint32_t            (*getMaxMomentum)(const THIS_TYPE* _this); \
    leResult            (*setMaxMomentum)(THIS_TYPE* _this, uint32_t max); \
    uint32_t            (*getMomentumFalloffRate)(const THIS_TYPE* _this); \
    leResult            (*setMomentumFalloffRate)(THIS_TYPE* _this, uint32_t percent); \
    uint32_t            (*getRotationUpdateRate)(const THIS_TYPE* _this); \
    leResult            (*setRotationUpdateRate)(THIS_TYPE* _this, uint32_t ms); \
    uint32_t            (*getItemCount)(const THIS_TYPE* _this); \
    int32_t             (*appendItem)(THIS_TYPE* _this); \
    int32_t             (*insertItem)(THIS_TYPE* _this, int32_t idx); \
    leResult            (*removeItem)(THIS_TYPE* _this, int32_t idx); \
    leResult            (*removeAllItems)(THIS_TYPE* _this); \
    int32_t             (*getSelectedItem)(const THIS_TYPE* _this); \
    leResult            (*setSelectedItem)(THIS_TYPE* _this, int32_t idx); \
    leResult            (*selectPreviousItem)(THIS_TYPE* _this); \
    leResult            (*selectNextItem)(THIS_TYPE* _this); \
    leString*           (*getItemString)(const THIS_TYPE* _this, int32_t idx); \
    leResult            (*setItemString)(THIS_TYPE* _this, int32_t idx, const leString* str); \
    leImage*            (*getItemIcon)(const THIS_TYPE* _this, int32_t idx); \
    leResult            (*setItemIcon)(THIS_TYPE* _this, int32_t idx, const leImage* img); \
    leListWheelWidget_SelectedItemChangedEvent (*getSelectedItemChangedEventCallback)(const THIS_TYPE* _this); \
    leResult            (*setSelectedItemChangedEventCallback)(THIS_TYPE* _this, leListWheelWidget_SelectedItemChangedEvent cb); \
    leListWheelIndicatorFill (*getIndicatorFill)(const THIS_TYPE* _this); \
    leResult            (*setIndicatorFill)(THIS_TYPE* _this, leListWheelIndicatorFill fill); \
    leListWheelZoomEffects (*getZoomEffects)(const THIS_TYPE* _this); \
    leResult            (*setZoomEffects)(THIS_TYPE* _this, leListWheelZoomEffects zfx); \
    leBool              (*getAutoHideWheel)(const THIS_TYPE* _this); \
    leResult            (*setAutoHideWheel)(THIS_TYPE* _this, leBool b); \
    
typedef struct leListWheelWidgetVTable
{
	LE_LISTWHEELWIDGET_VTABLE(leListWheelWidget)
} leListWheelWidgetVTable; 

    /**
      * @endcond
      *
      */

// *****************************************************************************
/**
 * @brief This struct represents a listwheel widget
 * @details A list wheel widget is a widget that is similar to a normal list
 * widget but can be dragged up or down to cycle through a single active
 * value. This widget is also capable of momentum and motion over time.
 */
typedef struct leListWheelWidget
{
    leWidget widget; // widget base class

    const leListWheelWidgetVTable* fn;

    leArray items; // list of items for the wheel
    int32_t selectedItem; // currently selected item
    int32_t visibleItems; // number of visible items in the wheel
                          // must be odd and >= 3
    int32_t topItem; // the current top item

    leRelativePosition iconPos; // the icon position of the items
    uint32_t iconMargin; // the icon margin of the items

    leBool showIndicators; // controls the visibility of the horizontal
                           // indicator bars in the center of the widget
    uint32_t indicatorArea; // controls the distance between the indicator bars

    uint32_t shaded; // determins if the background of the widget uses gradient
                     // shading to show depth

    int32_t cycleDistance; // determins the amount of drag distance needed
                           // to cycle between items
    int32_t cycleDelta; // tracks the current amount of drag distance

    // these track drag movement over time
    int32_t firstTouchY;
    int32_t touchY;
    int32_t lastTouchY;
    leBool  stillTouching;

    int32_t minFlickDelta; // amount of distance that must be dragged in a single
                           // frame to trigger momentum mode

    int32_t momentum; // current momentum value
    int32_t maxMomentum; // maximum momentum value
    int32_t momentumFalloff; // momentum falloff rate
    int32_t rotation; // determines actual rotation of the wheel
    int32_t rotationCounter; // time-based limiter for rotation calculations
    int32_t rotationTick; // rotation time accumulator

    leBool snapPending;

    leListWheelIndicatorFill indicatorFill; // the indicator's fill type
    leListWheelZoomEffects zoomEffects; // zoomEffects
    leBool autoHideWheel;   //auto hides the wheel
    leBool hideWheel;       //flag to hide/show the wheel

    struct
    {
        int32_t y;
        int32_t per;

        uint32_t nextItem;
    } paintState;

    leListWheelWidget_SelectedItemChangedEvent cb; // item changed callback

    leBorderType borderTypeCache; // Copy of border type, used to restore borders on auto-hide
    leBackgroundType backgroundTypeCache; // Copy of background type, used to restore borders on auto-hide
} leListWheelWidget;

// *****************************************************************************
// *****************************************************************************
// Section: Routines
// *****************************************************************************
// *****************************************************************************

/**
 * @brief Create widget.
 * @details Creates a new leListWheelWidget and allocates memory for the widget through the
 * current active context.  Application is responsible for managing the widget
 * pointer until the widget is added to a widget tree.
 * @remark use leWidget_Delete() to free this pointer.
 * @code
 * leListWheelWidget* wgt = leListWheelWidget_New();
 * @endcode
 * @return a widget object pointer.
 */
leListWheelWidget* leListWheelWidget_New(void);

/**
 * @brief Initialize widget.
 * @details Initializes the leListWidget <span class="param">wgt</span>.
 * @code
 * leListWheelWidget* wgt;
 * leListWheelWidget_Constructor(wgt);
 * @endcode
 * @param wgt is the widget to initialize
 * @return void.
 */
void leListWheelWidget_Constructor(leListWheelWidget* wgt);


#ifdef _DOXYGEN_
#define THIS_TYPE struct leWidget

// *****************************************************************************
/* Virtual Member Function:
    uint32_t getVisibleItemCount(const leListWheelWidget* _this)

  Summary:
     Gets the visible item count

  Description:
     Gets the visible item count

  Parameters:
    const leListWheelWidget* _this - The list wheel widget to operate on

  Remarks:
    Usage - _this->fn->getVisibleItemCount(_this);

  Returns:
    uint32_t - the visibility setting
*/

// *****************************************************************************
/* Virtual Member Function:
    leResult setVisibleItemCount(leListWheelWidget* _this,
                                 uint32_t cnt)

  Summary:
     Sets the visible item count

  Description:
     Sets the visible item count

  Parameters:
    leListWheelWidget* _this - The list wheel widget to operate on
    uint32_t cnt - the item count

  Remarks:
    Usage - _this->fn->setVisibleItemCount(_this, cnt);

  Returns:
    leResult - the result of the operation
*/
/**
 * @brief Set visible item count.
 * @details Sets the visible item count to <span class="param">cnt</span>
 * using <span class="param">_this</span>.
 * @code
 * const leListWheelWidget* _this;
 * leResult res = _this->fn->setVisibleItemCount(_this, cnt);
 * @endcode
 * @param _this is the widget pointer to query.
 * @param cnt is the item count.
 * @returns LE_SUCCESS if set, otherwise LE_FAILURE.
 */
virtual leResult setVisibleItemCount(leListWheelWidget* _this,
                                     uint32_t cnt);


// *****************************************************************************
/* Virtual Member Function:
    leRelativePosition getIconPosition(const leListWheelWidget* _this)

  Summary:
     Gets the image icon position

  Description:
     Gets the image icon position

  Parameters:
    const leListWheelWidget* _this - The list wheel widget to operate on

  Remarks:
    Usage - _this->fn->getIconPosition(_this);

  Returns:
    leRelativePosition - the position
*/

// *****************************************************************************
/* Virtual Member Function:
    leResult setIconPosition(leListWheelWidget* _this,
                             leRelativePosition pos)

  Summary:
     Sets the image icon position

  Description:
     Sets the image icon position

  Parameters:
    leListWheelWidget* _this - The list wheel widget to operate on
    leRelativePosition pos - the position value

  Remarks:
    Usage - _this->fn->setIconPosition(_this, pos);

  Returns:
    leResult - the result of the operation
*/
/**
 * @brief Set image icon position.
 * @details Sets the image icon position to <span class="param">pos</span>
 * using <span class="param">_this</span>.
 * @code
 * const leListWheelWidget* _this;
 * leRelativePosition pos;
 * leResult res = _this->fn->setIconPosition(_this, pos);
 * @endcode
 * @param _this is the widget pointer to query.
 * @param pos is the position value.
 * @returns LE_SUCCESS if set, otherwise LE_FAILURE.
 */
virtual leResult setIconPosition(leListWheelWidget* _this,
                                 leRelativePosition pos);

// *****************************************************************************
/* Virtual Member Function:
    uint32_t getIconMargin(const leListWheelWidget* _this)

  Summary:
     Gets the icon margin

  Description:
     Gets the icon margin

  Parameters:
    const leListWheelWidget* _this - The list wheel widget to operate on

  Remarks:
    Usage - _this->fn->getIconMargin(_this);

  Returns:
    uint32_t - the icon margin value
*/

// *****************************************************************************
/* Virtual Member Function:
    leResult setIconMargin(leListWheelWidget* _this,
                           uint32_t mg)

  Summary:
     Sets the icon margin

  Description:
     Sets the icon margin

  Parameters:
    leListWheelWidget* _this - The list wheel widget to operate on
    uint32_t mg - the margin value

  Remarks:
    Usage - _this->fn->setIconMargin(_this, mg);

  Returns:
    leResult - the result of the operation
*/
/**
 * @brief Set icon margin.
 * @details Sets the icon margin to <span class="param">mg</span>
 * using <span class="param">_this</span>.
 * @code
 * const leListWheelWidget* _this;
 * uint32_t mg;
 * leResult res = _this->fn->setIconMargin(_this, mg);
 * @endcode
 * @param _this is the widget pointer to query.
 * @param mg is the margin value.
 * @returns LE_SUCCESS if set, otherwise LE_FAILURE.
 */
virtual leResult setIconMargin(leListWheelWidget* _this,
                               uint32_t mg);

// *****************************************************************************
/* Virtual Member Function:
    leBool getShowIndicators(const leListWheelWidget* _this)

  Summary:
     Indicates if the selection indicators should be drawn

  Description:
     Indicates if the selection indicators should be drawn

  Parameters:
    const leListWheelWidget* _this - The list wheel widget to operate on

  Remarks:
    Usage - _this->fn->getShowIndicators(_this);

  Returns:
    leBool - the setting value
*/
/**
 * @brief Get show selection indicators setting.
 * @details Gets the show selection indicators setting
 * for <span class="param">_this</span>.
 * @code
 * const leListWheelWidget* _this;
 * leBool show = _this->fn->getShowIndicators(_this);
 * @endcode
 * @param _this is the widget pointer to query.
 * @returns LE_TRUE if set, otherwise LE_FALSE;
 */
virtual leBool getShowIndicators(const leListWheelWidget* _this);

// *****************************************************************************
/* Virtual Member Function:
    leResult setShowIndicators(leListWheelWidget* _this,
                               leBool b)

  Summary:
     Set the draw indicator setting value

  Description:
     Set the draw indicator setting value

  Parameters:
    leListWheelWidget* _this - The list wheel widget to operate on
    leBool b - the setting value

  Remarks:
    Usage - _this->fn->setShowIndicators(_this, b);

  Returns:
    leResult - the result of the operation
*/
/**
 * @brief Set draw indicator setting value.
 * @details Sets the draw indicator setting value to <span class="param">b</span>
 * using <span class="param">_this</span>.
 * @code
 * const leListWheelWidget* _this;
 * leBool b;
 * leResult res = _this->fn->setShowIndicators(_this, b);
 * @endcode
 * @param _this is the widget pointer to query.
 * @param b is the setting value.
 * @returns LE_SUCCESS if set, otherwise LE_FAILURE.
 */
virtual leResult setShowIndicators(leListWheelWidget* _this,
                                   leBool b);

// *****************************************************************************
/* Virtual Member Function:
    uint32_t getIndicatorArea(const leListWheelWidget* _this)

  Summary:
     Gets the area of the selection indicators

  Description:
     Gets the area of the selection indicators

  Parameters:
    const leListWheelWidget* _this - The list wheel widget to operate on

  Remarks:
    Usage - _this->fn->getIndicatorArea(_this);

  Returns:
    uint32_t - the area value
*/

// *****************************************************************************
/* Virtual Member Function:
    leResult setIndicatorArea(leListWheelWidget* _this,
                              uint32_t area)

  Summary:
     Sets the area of hte selection indicators

  Description:
     Sets the area of hte selection indicators

  Parameters:
    leListWheelWidget* _this - The list wheel widget to operate on
    uint32_t area - the area value

  Remarks:
    Usage - _this->fn->setIndicatorArea(_this, area);

  Returns:
    leResult - the result of the operation
*/
/**
 * @brief Set draw indicator setting value.
 * @details Sets the draw indicator setting value to <span class="param">area</span>
 * using <span class="param">_this</span>.
 * @code
 * const leListWheelWidget* _this;
 * uint32_t area;
 * leResult res = _this->fn->setIndicatorArea(_this, area);
 * @endcode
 * @param _this is the widget pointer to query.
 * @param area is the area value.
 * @returns LE_SUCCESS if set, otherwise LE_FAILURE.
 */
virtual leResult setIndicatorArea(leListWheelWidget* _this,
                                  uint32_t area);

// *****************************************************************************
/* Virtual Member Function:
    leBool getShaded(const leListWheelWidget* _this)

  Summary:
     Gets the shaded setting value

  Description:
     Gets the shaded setting value

  Parameters:
    const leListWheelWidget* _this - The list wheel widget to operate on

  Remarks:
    Usage - _this->fn->getShaded(_this);

  Returns:
    leBool - the setting value
*/
/**
 * @brief Get shaded setting value setting.
 * @details Gets the shaded setting value setting
 * for <span class="param">_this</span>.
 * @code
 * const leListWheelWidget* _this;
 * leBool show = _this->fn->getShaded(_this);
 * @endcode
 * @param _this is the widget pointer to query.
 * @returns LE_TRUE if set, otherwise LE_FALSE;
 */
virtual leBool getShaded(const leListWheelWidget* _this);

// *****************************************************************************
/* Virtual Member Function:
    leResult setShaded(leListWheelWidget* _this,
                       leBool b)

  Summary:
     Sets the shaded setting value

  Description:
     Sets the shaded setting value

  Parameters:
    leListWheelWidget* _this - The list wheel widget to operate on
    leBool b - the setting value

  Remarks:
    Usage - _this->fn->setShaded(_this, b);

  Returns:
    leResult - the result of the operation
*/
/**
 * @brief Set shaded setting value.
 * @details Sets the shaded setting value to <span class="param">sp</span>
 * using <span class="param">_this</span>.
 * @code
 * const leListWheelWidget* _this;
 * leBool sp;
 * leResult res = _this->fn->setShaded(_this, b);
 * @endcode
 * @param _this is the widget pointer to query.
 * @param b is the setting value.
 * @returns LE_SUCCESS if set, otherwise LE_FAILURE.
 */
virtual leResult setShaded(leListWheelWidget* _this,
                           leBool sp);


// *****************************************************************************
/* Virtual Member Function:
    uint32_t getFlickInitSpeed(const leListWheelWidget* _this)

  Summary:
     Gets the flick init speed

  Description:
     Gets the flick init speed

  Parameters:
    const leListWheelWidget* _this - The list wheel widget to operate on

  Remarks:
    Usage - _this->fn->getFlickInitSpeed(_this);

  Returns:
    uint32_t - the speed value
*/
/**
 * @brief Get flick init speed.
 * @details Gets the flick init speed for <span class="param">_this</span>.
 * @code
 * const leListWheelWidget* _this;
 * uint32_t speed = _this->fn->getFlickInitSpeed(_this);
 * @endcode
 * @param _this is the widget pointer to query.
 * @returns the speed value.
 */
virtual uint32_t getFlickInitSpeed(const leListWheelWidget* _this);

// *****************************************************************************
/* Virtual Member Function:
    leResult setFlickInitSpeed(leListWheelWidget* _this,
                               uint32_t sp)

  Summary:
     Sets the flick init speed

  Description:
     Sets the flick init speed

  Parameters:
    leListWheelWidget* _this - The list wheel widget to operate on
    uint32_t sp - the speed value

  Remarks:
    Usage - _this->fn->setFlickInitSpeed(_this, sp);

  Returns:
    leResult - the result of the operation
*/
/**
 * @brief Set flick init speed.
 * @details Sets the flick init speed to <span class="param">sp</span>
 * using <span class="param">_this</span>.
 * @code
 * const leListWheelWidget* _this;
 * uint32_t sp;
 * leResult res = _this->fn->setFlickInitSpeed(_this, sp);
 * @endcode
 * @param _this is the widget pointer to query.
 * @param sp is the speed value.
 * @returns LE_SUCCESS if set, otherwise LE_FAILURE.
 */
virtual leResult setFlickInitSpeed(leListWheelWidget* _this,
                                   uint32_t sp);

// *****************************************************************************
/* Virtual Member Function:
    uint32_t getMaxMomentum(const leListWheelWidget* _this)

  Summary:
     Get the max momentum value

  Description:
     Get the max momentum value

  Parameters:
    const leListWheelWidget* _this - The list wheel widget to operate on

  Remarks:
    Usage - _this->fn->getMaxMomentum(_this);

  Returns:
    uint32_t - the value
*/
/**
 * @brief Get max momentum value.
 * @details Gets the max momentum value for <span class="param">_this</span>.
 * @code
 * const leListWheelWidget* _this;
 * uint32_t max = _this->fn->getMaxMomentum(_this);
 * @endcode
 * @param _this is the widget pointer to query.
 * @returns the value.
 */
virtual uint32_t getMaxMomentum(const leListWheelWidget* _this);

// *****************************************************************************
/* Virtual Member Function:
    leResult setMaxMomentum(leListWheelWidget* _this,
                            uint32_t max)

  Summary:
     Sets the max momentum value

  Description:
     Sets the max momentum value

  Parameters:
    leListWheelWidget* _this - The list wheel widget to operate on
    uint32_t max - the maximum value

  Remarks:
    Usage - _this->fn->setMaxMomentum(_this, max);

  Returns:
    leResult - the result of the operation
*/
/**
 * @brief Set max momentum value.
 * @details Sets the max momentum value to <span class="param">max</span>
 * using <span class="param">_this</span>.
 * @code
 * const leListWheelWidget* _this;
 * uint32_t max;
 * leResult res = _this->fn->setMaxMomentum(_this, max);
 * @endcode
 * @param _this is the widget pointer to query.
 * @param max is the maximum value.
 * @returns LE_SUCCESS if set, otherwise LE_FAILURE.
 */
virtual leResult setMaxMomentum(leListWheelWidget* _this,
                                uint32_t max);

// *****************************************************************************
/* Virtual Member Function:
    uint32_t getMomentumFalloffRate(const leListWheelWidget* _this)

  Summary:
     Gets the momentum falloff rate

  Description:
     Gets the momentum falloff rate

  Parameters:
    const leListWheelWidget* _this - The list wheel widget to operate on

  Remarks:
    Usage - _this->fn->getMomentumFalloffRate(_this);

  Returns:
    uint32_t - the value
*/
/**
 * @brief Get max momentum falloff rate.
 * @details Gets the momentum falloff rate for <span class="param">_this</span>.
 * @code
 * const leListWheelWidget* _this;
 * uint32_t max = _this->fn->getMomentumFalloffRate(_this);
 * @endcode
 * @param _this is the widget pointer to query.
 * @returns the value.
 */
virtual uint32_t getMomentumFalloffRate(const leListWheelWidget* _this);

// *****************************************************************************
/* Virtual Member Function:
    leResult setMomentumFalloffRate(leListWheelWidget* _this,
                                    uint32_t percent)

  Summary:
     Sets the momentum falloff rate

  Description:
     Sets the momentum falloff rate

  Parameters:
    leListWheelWidget* _this - The list wheel widget to operate on
    uint32_t percent - the value

  Remarks:
    Usage - _this->fn->setMomentumFalloffRate(_this, percent);

  Returns:
    leResult - the result of the operation
*/
/**
 * @brief Set momentum falloff rate.
 * @details Sets the momentum falloff rate to <span class="param">percent</span>
 * using <span class="param">_this</span>.
 * @code
 * const leListWheelWidget* _this;
 * uint32_t percent;
 * leResult res = _this->fn->setMomentumFalloffRate(_this, percent);
 * @endcode
 * @param _this is the widget pointer to query.
 * @param percent is the value.
 * @returns LE_SUCCESS if set, otherwise LE_FAILURE.
 */
virtual leResult setMomentumFalloffRate(leListWheelWidget* _this,
                                        uint32_t percent);

// *****************************************************************************
/* Virtual Member Function:
    uint32_t getRotationUpdateRate(const leListWheelWidget* _this)

  Summary:
     Gets the rotation update rate

  Description:
     Gets the rotation update rate

  Parameters:
    const leListWheelWidget* _this - The list wheel widget to operate on

  Remarks:
    Usage - _this->fn->getRotationUpdateRate(_this);

  Returns:
    uint32_t - the value
*/
/**
 * @brief Get max rotation update rate.
 * @details Gets the rotation update rate for <span class="param">_this</span>.
 * @code
 * const leListWheelWidget* _this;
 * uint32_t rate = _this->fn->getRotationUpdateRate(_this);
 * @endcode
 * @param _this is the widget pointer to query.
 * @returns the value.
 */
virtual uint32_t getRotationUpdateRate(const leListWheelWidget* _this);

// *****************************************************************************
/* Virtual Member Function:
    leResult setRotationUpdateRate(leListWheelWidget* _this,
                                   uint32_t ms)

  Summary:
     Sets the rotation update rate

  Description:
     Sets the rotation update rate

  Parameters:
    leListWheelWidget* _this - The list wheel widget to operate on
    uint32_t ms - the value

  Remarks:
    Usage - _this->fn->setRotationUpdateRate(_this, ms);

  Returns:
    leResult - the result of the operation
*/
/**
 * @brief Set rotation update rate.
 * @details Sets the rotation update rate to <span class="param">ms</span>
 * using <span class="param">_this</span>.
 * @code
 * const leListWheelWidget* _this;
 * uint32_t ms;
 * leResult res = _this->fn->setRotationUpdateRate(_this, ms);
 * @endcode
 * @param _this is the widget pointer to query.
 * @param ms is the value.
 * @returns LE_SUCCESS if set, otherwise LE_FAILURE.
 */
virtual leResult setRotationUpdateRate(leListWheelWidget* _this,
                                       uint32_t ms);

// *****************************************************************************
/* Virtual Member Function:
    uint32_t getItemCount(const leListWheelWidget* _this)

  Summary:
     Gets the total item count

  Description:
     Gets the total item count

  Parameters:
    const leListWheelWidget* _this - The list wheel widget to operate on

  Remarks:
    Usage - _this->fn->getItemCount(_this);

  Returns:
    uint32_t - the item count value
*/
/**
 * @brief Get total item count.
 * @details Gets the total item count for <span class="param">_this</span>.
 * @code
 * const leListWheelWidget* _this;
 * uint32_t cnt = _this->fn->getItemCount(_this);
 * @endcode
 * @param _this is the widget pointer to query.
 * @returns the item count value.
 */
virtual uint32_t getItemCount(const leListWheelWidget* _this);

// *****************************************************************************
/* Virtual Member Function:
    int32_t appendItem(leListWheelWidget* _this)

  Summary:
     Appends an item to the list

  Description:
     Appends an item to the list

  Parameters:
    leListWheelWidget* _this - The list wheel widget to operate on

  Remarks:
    Usage - _this->fn->appendItem(_this);

  Returns:
    int32_t - the index of the appended item
*/
/**
 * @brief Append item to the list.
 * @details Appends an item to the list using <span class="param">_this</span>.
 * @code
 * const leListWheelWidget* _this;
 * uint32_t cnt = _this->fn->appendItem(_this);
 * @endcode
 * @param _this is the widget pointer to query.
 * @returns the index of the appended item.
 */
virtual int32_t appendItem(const leListWheelWidget* _this);

// *****************************************************************************
/* Virtual Member Function:
    int32_t insertItem(leListWheelWidget* _this,
                       int32_t idx)

  Summary:
     Inserts an item into the list

  Description:
     Inserts an item into the list

  Parameters:
    leListWheelWidget* _this - The list wheel widget to operate on
    int32_t idx - the index

  Remarks:
    Usage - _this->fn->insertItem(_this, idx);

  Returns:
    int32_t - the index of the appended item
*/

// *****************************************************************************
/* Virtual Member Function:
    leResult removeItem(leListWheelWidget* _this,
                        int32_t idx)

  Summary:
     Removes an item from the list

  Description:
     Removes an item from the list

  Parameters:
    leListWheelWidget* _this - The list wheel widget to operate on
    int32_t idx - the index

  Remarks:
    Usage - _this->fn->removeItem(_this, idx);

  Returns:
    leResult - the result of the operation
*/
/**
 * @brief Remove item from the list.
 * @details Removes an item from the list at <span class="param">idx</span>
 * using <span class="param">_this</span>.
 * @code
 * const leListWheelWidget* _this;
 * int32_t idx;
 * leResult res = _this->fn->removeItem(_this, idx);
 * @endcode
 * @param _this is the widget pointer to query.
 * @param idx is the index.
 * @returns LE_SUCCESS if set, otherwise LE_FAILURE.
 */
virtual leResult removeItem(leListWheelWidget* _this,
                            int32_t idx);

// *****************************************************************************
/* Virtual Member Function:
    leResult removeAllItems(leListWheelWidget* _this)

  Summary:
     Removes all items from the list

  Description:
     Removes all items from the list

  Parameters:
    leListWheelWidget* _this - The list wheel widget to operate on

  Remarks:
    Usage - _this->fn->removeAllItems(_this);

  Returns:
    leResult - the result of the operation
*/
/**
 * @brief Remove all items from the list.
 * @details Removes all items from the list
 * using <span class="param">_this</span>.
 * @code
 * const leListWheelWidget* _this;
 * leResult res = _this->fn->removeAllItems(_this);
 * @endcode
 * @param _this is the widget pointer to query.
 * @returns LE_SUCCESS if set, otherwise LE_FAILURE.
 */
virtual leResult removeAllItems(leListWheelWidget* _this);

// *****************************************************************************
/* Virtual Member Function:
    int32_t getSelectedItem(const leListWheelWidget* _this)

  Summary:
     Gets the selected item

  Description:
     Gets the selected item

  Parameters:
    const leListWheelWidget* _this - The list wheel widget to operate on

  Remarks:
    Usage - _this->fn->getSelectedItem(_this);

  Returns:
    int32_t - the index of the selected item
*/
/**
 * @brief Get selected item index.
 * @details Gets the selected item index using <span class="param">_this</span>.
 * @code
 * const leListWheelWidget* _this;
 * uint32_t idx = _this->fn->getSelectedItem(_this);
 * @endcode
 * @param _this is the widget pointer to query.
 * @returns the index of the selected item.
 */
virtual int32_t getSelectedItem(const leListWheelWidget* _this);

// *****************************************************************************
/* Virtual Member Function:
    leResult setSelectedItem(leListWheelWidget* _this,
                             int32_t idx)

  Summary:
     Sets the selected item

  Description:
     Sets the selected item

  Parameters:
    leListWheelWidget* _this - The list wheel widget to operate on
    int32_t idx - the index

  Remarks:
    Usage - _this->fn->setSelectedItem(_this, idx);

  Returns:
    leResult - the result of the operation
*/
/**
 * @brief Set rotation update rate.
 * @details Sets the rotation update rate to <span class="param">idx</span>
 * using <span class="param">_this</span>.
 * @code
 * const leListWheelWidget* _this;
 * int32_t idx;
 * leResult res = _this->fn->setSelectedItem(_this, idx);
 * @endcode
 * @param _this is the widget pointer to query.
 * @param idx is the value.
 * @returns LE_SUCCESS if set, otherwise LE_FAILURE.
 */
virtual leResult setSelectedItem(leListWheelWidget* _this,
                                 int32_t idx);


// *****************************************************************************
/* Virtual Member Function:
    leResult selectPreviousItem(leListWheelWidget* _this)

  Summary:
     Selects the previous item

  Description:
     Selects the previous item

  Parameters:
    leListWheelWidget* _this - The list wheel widget to operate on

  Remarks:
    Usage - _this->fn->selectPreviousItem(_this);

  Returns:
    leResult - the result of the operation
*/
/**
 * @brief Select previous item.
 * @details Selects the previous item using <span class="param">_this</span>.
 * @code
 * const leListWheelWidget* _this;
 * leResult res = _this->fn->selectPreviousItem(_this);
 * @endcode
 * @param _this is the widget pointer to query.
 * @returns LE_SUCCESS if set, otherwise LE_FAILURE.
 */
virtual leResult selectPreviousItem(leListWheelWidget* _this);

// *****************************************************************************
/* Virtual Member Function:
    leResult selectNextItem(leListWheelWidget* _this)

  Summary:
     Selects the next item

  Description:
     Selects the next item

  Parameters:
    leListWheelWidget* _this - The list wheel widget to operate on

  Remarks:
    Usage - _this->fn->selectNextItem(_this);

  Returns:
    leResult - the result of the operation
*/
/**
 * @brief Select next item.
 * @details Selects the next item using <span class="param">_this</span>.
 * @code
 * const leListWheelWidget* _this;
 * leResult res = _this->fn->selectNextItem(_this);
 * @endcode
 * @param _this is the widget pointer to query.
 * @returns LE_SUCCESS if set, otherwise LE_FAILURE.
 */
virtual leResult selectNextItem(leListWheelWidget* _this);

// *****************************************************************************
/* Virtual Member Function:
    leString* getItemString(const leListWheelWidget* _this,
                            int32_t idx)

  Summary:
     Gets the string pointer for an item

  Description:
     Gets the string pointer for an item

  Parameters:
    const leListWheelWidget* _this - The list wheel widget to operate on
    int32_t idx - the index

  Remarks:
    Usage - _this->fn->getItemString(_this, idx);

  Returns:
    leString* - the string pointer
*/
/**
 * @brief Get string pointer for an item.
 * @details Gets the string pointer for an item using <span class="param">_this</span>.
 * @code
 * const leListWheelWidget* _this;
 * int32_t idx;
 * leString* str = _this->fn->getItemString(_this, idx);
 * @endcode
 * @param _this is the widget pointer to query.
 * @param idx the index.
 * @returns the string pointer.
 */
virtual leString* getItemString(const leListWheelWidget* _this,
                              int32_t idx);

// *****************************************************************************
/* Virtual Member Function:
    leResult setItemString(leListWheelWidget* _this,
                           int32_t idx,
                           const leString* str)

  Summary:
     Sets the string pointer for an item

  Description:
     Sets the string pointer for an item

  Parameters:
    leListWheelWidget* _this - The list wheel widget to operate on
    int32_t idx - the index
    const leString* str - the string pointer

  Remarks:
    Usage - _this->fn->setItemString(_this, idx, str);

  Returns:
    leResult - the result of the operation
*/
/**
 * @brief Set string pointer for an item.
 * @details Sets the string pointer to <span class="param">str</span> for an
 * item at <span class="param">idx</span> using <span class="param">_this</span>.
 * @code
 * const leListWheelWidget* _this;
 * int32_t idx;
 * leResult res = _this->fn->setItemString(_this, idx, str);
 * @endcode
 * @param _this is the widget pointer to query.
 * @param idx is the value.
 * @param str is the string pointer
 * @returns LE_SUCCESS if set, otherwise LE_FAILURE.
 */
virtual leResult setItemString(leListWheelWidget* _this,
                               int32_t idx,
                               const leString* str);

// *****************************************************************************
/* Virtual Member Function:
    leImage* getItemIcon(const leListWheelWidget* _this,
                         int32_t idx)

  Summary:
     Gets the image pointer for an item

  Description:
     Gets the image pointer for an item

  Parameters:
    const leListWheelWidget* _this - The list wheel widget to operate on
    int32_t idx - the index

  Remarks:
    Usage - _this->fn->getItemIcon(_this, idx);

  Returns:
    leImage* - the image pointer
*/
/**
 * @brief Get image pointer for an item.
 * @details Gets the image pointer for an item at <span class="param">_this</span>
 * using <span class="param">_this</span>.
 * @code
 * const leListWheelWidget* _this;
 * int32_t idx;
 * leImage* img = _this->fn->getItemIcon(_this, idx);
 * @endcode
 * @param _this is the widget pointer to query.
 * @returns the string pointer.
 */
virtual leImage* getItemIcon(const leListWheelWidget* _this,
                             int32_t idx);

// *****************************************************************************
/* Virtual Member Function:
    leResult setItemIcon(leListWheelWidget* _this,
                         int32_t idx,
                         const leImage* img)

  Summary:
     Sets the image pointer for an item

  Description:
     Sets the image pointer for an item

  Parameters:
    leListWheelWidget* _this - The list wheel widget to operate on
    int32_t idx - the index
    const leImage* img - the image pointer

  Remarks:
    Usage - _this->fn->setItemIcon(_this, idx, img);

  Returns:
    leResult - the result of the operation
*/
/**
 * @brief Set image pointer for an item.
 * @details Sets the image pointer for an item to <span class="param">img</span> for an
 * item at <span class="param">idx</span> using <span class="param">_this</span>.
 * @code
 * const leListWheelWidget* _this;
 * int32_t idx;
 * leImage* img;
 * leResult res = _this->fn->setItemString(_this, idx, img);
 * @endcode
 * @param _this is the widget pointer to query.
 * @param idx is the value.
 * @param img is the image pointer
 * @returns LE_SUCCESS if set, otherwise LE_FAILURE.
 */
virtual leResult setItemIcon(leListWheelWidget* _this,
                             int32_t idx,
                             const leImage* img);


/**
 * @brief Get selected item changed event callback pointer.
 * @details Gets the selected item changed event callback pointer using <span class="param">_this</span>.
 * @code
 * leListWheelWidget* _this;
 * leListWheelWidget_SelectedItemChangedEvent cb = _this->fn->getSelectedItemChangedEventCallback(_this);
 * @endcode
 * @param _this is the widget to modify
 * @returns the callback pointer.
 */
virtual leListWheelWidget_SelectedItemChangedEvent getSelectedItemChangedEventCallback
                                                    (const leListWheelWidget* _this);

/**
 * @brief Set selected item changed event callback pointer.
 * @details Sets the selected item changed event callback pointer to <span class="param">cb</span>
 * using <span class="param">_this</span>.
 * @code
 * leListWheelWidget* _this;
 * leListWheelWidget_SelectedItemChangedEvent cb;
 * leResult res = _this->fn->setSelectedItemChangedEventCallback(_this, cb);
 * @endcode
 * @param _this is the widget to modify
 * @param cb is the callback func
 * @returns LE_SUCCESS if set, otherwise LE_FAILURE.
 */
virtual leResult setSelectedItemChangedEventCallback(leListWheelWidget* _this,
                                                     leListWheelWidget_SelectedItemChangedEvent cb);



// *****************************************************************************
/* Virtual Member Function:
    leListWheelIndicatorFill getIndicatorFill(const leListWheelWidget* _this)

  Summary:
     Gets the indicator fill setting value

  Description:
     Gets the indicator fill setting value

  Parameters:
    const leListWheelWidget* _this - The list wheel widget to operate on

  Remarks:
    Usage - _this->fn->getIndicatorFill(_this);

  Returns:
    leListWheelIndicatorFill - the setting value
*/
/**
 * @brief Get indicator fill setting value.
 * @details Gets indicator fill setting value
 * using <span class="param">_this</span>.
 * @code
 * const leListWheelWidget* _this;
 * leListWheelIndicatorFill setting = _this->fn->getIndicatorFill(_this);
 * @endcode
 * @param _this is the widget pointer to query.
 * @returns the setting value.
 */
virtual leListWheelIndicatorFill getIndicatorFill(const leListWheelWidget* _this);

// *****************************************************************************
/* Virtual Member Function:
    leResult setIndicatorFill(leListWheelWidget* _this,
                              leListWheelIndicatorFill fill)

  Summary:
     Sets the indicator fill setting value

  Description:
     Sets the indicator fill setting value

  Parameters:
    leListWheelWidget* _this - The list wheel widget to operate on
    leListWheelIndicatorFill fill - the setting value

  Remarks:
    Usage - _this->fn->setIndicatorFill(_this, fill);

  Returns:
    leResult - the result of the operation
*/
/**
 * @brief Set indicator fill setting valuer.
 * @details Sets the indicator fill setting value to <span class="param">fill</span>
 * using <span class="param">_this</span>.
 * @code
 * leListWheelWidget* _this;
 * leListWheelIndicatorFill fill;
 * leResult res = _this->fn->setSelectedItemChangedEventCallback(_this, fill);
 * @endcode
 * @param _this is the widget to modify
 * @param fill is the fill setting
 * @returns LE_SUCCESS if set, otherwise LE_FAILURE.
 */
virtual leResult setIndicatorFill(leListWheelWidget* _this,
                                  leListWheelIndicatorFill fill);


// *****************************************************************************
/* Virtual Member Function:
    leListWheelZoomEffects getZoomEffects(const leListWheelWidget* _this)

  Summary:
     Gets the current zoom effects

  Description:
     Gets the current zoom effects

  Parameters:
    const leListWheelWidget* _this - The list wheel widget to operate on

  Remarks:
    Usage - _this->fn->getZoomEffects(_this);

  Returns:
    leListWheelZoomEffects - the setting value
*/
/**
 * @brief Get current zoom effects.
 * @details Gets the current zoom effects
 * using <span class="param">_this</span>.
 * @code
 * const leListWheelWidget* _this;
 * leListWheelZoomEffects effects = _this->fn->getZoomEffects(_this);
 * @endcode
 * @param _this is the widget pointer to query.
 * @returns the setting value.
 */
virtual leListWheelZoomEffects getZoomEffects(const leListWheelWidget* _this);

// *****************************************************************************
/* Virtual Member Function:
    leResult setZoomEffects(leListWheelWidget* _this,
                            leListWheelZoomEffects zfx)

  Summary:
     Sets the zoom effects value

  Description:
     Sets the zoom effects value

  Parameters:
    leListWheelWidget* _this - The list wheel widget to operate on
    leListWheelZoomEffects zfx - the setting value

  Remarks:
    Usage - _this->fn->setZoomEffects(_this, zfx);

  Returns:
    leResult - the result of the operation
*/
/**
 * @brief Set zoom effects value.
 * @details Sets the zoom effects value to <span class="param">fill</span>
 * using <span class="param">_this</span>.
 * @code
 * leListWheelWidget* _this;
 * leListWheelZoomEffects zfx;
 * leResult res = _this->fn->setZoomEffects(_this, zfx);
 * @endcode
 * @param _this is the widget to modify
 * @param zfx is the setting value
 * @returns LE_SUCCESS if set, otherwise LE_FAILURE.
 */
virtual leResult setZoomEffects(leListWheelWidget* _this,
                                leListWheelZoomEffects zfx);

// *****************************************************************************
/* Virtual Member Function:
    leBool getAutoHideWheel(const leListWheelWidget* _this)

  Summary:
     Gets the wheel auto-hide setting

  Description:
     Gets the wheel auto-hide setting

  Parameters:
    const leListWheelWidget* _this - The list wheel widget to operate on

  Remarks:
    Usage - _this->fn->getAutoHideWheel(_this);

  Returns:
    leBool - the setting value
*/
/**
 * @brief Get wheel auto-hide setting.
 * @details Gets the wheel auto-hide setting for <span class="param">_this</span>.
 * @code
 * const leListWheelWidget* _this;
 * leBool hide = _this->fn->getAutoHideWheel(_this);
 * @endcode
 * @param _this is the widget pointer to query.
 * @returns LE_TRUE if set, otherwise LE_FALSE;
 */
virtual leBool getAutoHideWheel(const leListWheelWidget* _this);

// *****************************************************************************
/* Virtual Member Function:
    leResult setAutoHideWheel(leListWheelWidget* _this,
                              leBool b)

  Summary:
     Sets the wheel auto-hide setting

  Description:
     Sets the wheel auto-hide setting

  Parameters:
    leListWheelWidget* _this - The list wheel widget to operate on
    leBool b - the setting value

  Remarks:
    Usage - _this->fn->setAutoHideWheel(_this, b);

  Returns:
    leResult - the result of the operation
*/
/**
 * @brief Set wheel auto-hide setting.
 * @details Sets the wheel auto-hide setting to <span class="param">fill</span>
 * using <span class="param">_this</span>.
 * @code
 * leListWheelWidget* _this;
 * leBool b;
 * leResult res = _this->fn->setAutoHideWheel(_this, b);
 * @endcode
 * @param _this is the widget to modify
 * @param b is the setting value
 * @returns LE_SUCCESS if set, otherwise LE_FAILURE.
 */
virtual leResult setAutoHideWheel(leListWheelWidget* _this,
                                  leBool b);

#undef THIS_TYPE
#endif


#endif // LE_LISTWHEEL_WIDGET_ENABLED
#endif /* LEGATO_LISTWHEELWIDGET_H */
