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
    legato_widget_radial_menu.h

  Summary:
    

  Description:
    This module implements radial menu widget functions.
*******************************************************************************/

/** \file legato_widget_radial_menu.h
 * @brief Radial menu functions and definitions.
 *
 * @details This module implements radial menu widget functions.
 */

#ifndef LEGATO_RADIALMENU_H
#define LEGATO_RADIALMENU_H

#include "gfx/legato/common/legato_common.h"

#if LE_RADIALMENU_WIDGET_ENABLED == 1

#ifdef __cplusplus
extern "C" {
#endif

#include "gfx/legato/widget/legato_widget.h"
#include "gfx/legato/widget/image/legato_widget_image.h"
#include "gfx/legato/datastructure/legato_list.h"
#include "gfx/legato/string/legato_string.h"

typedef struct leRadialMenuWidget leRadialMenuWidget;

typedef void (* leRadialMenuWidget_ItemProminenceChangedEvent)(leRadialMenuWidget*, leWidget*, int32_t);

// *****************************************************************************
// *****************************************************************************
// Section: Data Types and Constants
// *****************************************************************************
// *****************************************************************************
/**
 * @brief This struct represents a redial menu widget state
 * @details Used to define the possible states of the radial menu widget.
 */
typedef enum leRadialMenuWidgetState
{
    LE_RADIAL_MENU_INIT,
    LE_RADIAL_MENU_INPUT_READY,
    LE_RADIAL_MENU_HANDLE_USER_MOVE_REQUEST,
    LE_RADIAL_MENU_RESET_TO_INPUT_POS
} leRadialMenuWidgetState;

/**
 * @brief This struct represents a redial menu interpolation state.
 * @details Used to define the possible interpolation modes for radial menu widget.
 */
typedef enum leRadialMenuWidgetInterpolationMode
{
    LE_RADIAL_MENU_INTERPOLATE_OFF,
    LE_RADIAL_MENU_INTERPOLATE_GRADUAL,
    LE_RADIAL_MENU_INTERPOLATE_PROMINENT
} leRadialMenuWidgetInterpolationMode;

/* internal use only */
/**
  * @cond INTERNAL
  *
  */typedef struct leRadialMenuWidget leRadialMenuWidget;

#define LE_RADIALMENUWIDGET_VTABLE(THIS_TYPE) \
    LE_WIDGET_VTABLE(THIS_TYPE) \
    \
    leBool    (*isProminent)(const THIS_TYPE* _this, const leWidget* widget); \
    leWidget* (*getProminent)(const THIS_TYPE* _this); \
    leResult  (*setProminent)(THIS_TYPE* _this, const leWidget* widget); \
    int32_t   (*getProminentIndex)(const THIS_TYPE* _this); \
    leResult  (*setProminentIndex)(THIS_TYPE* _this, int32_t index); \
    leResult  (*setNumberOfItemsShown)(THIS_TYPE* _this, uint32_t cnt); \
    leResult  (*setHighlightProminent)(THIS_TYPE* _this, leBool hl); \
    int32_t   (*getMajorAxis)(const THIS_TYPE* _this); \
    leResult  (*setMajorAxis)(THIS_TYPE* _this, int32_t a); \
    int32_t   (*getMinorAxis)(const THIS_TYPE* _this); \
    leResult  (*setMinorAxis)(THIS_TYPE* _this, int32_t b); \
    int32_t   (*getTheta)(const THIS_TYPE* _this); \
    leResult  (*setTheta)(THIS_TYPE* _this, int32_t tht); \
    leResult  (*setScaleMode)(THIS_TYPE* _this, leRadialMenuWidgetInterpolationMode mode); \
    leResult  (*setScaleRange)(THIS_TYPE* _this, int32_t min, int32_t max); \
    leResult  (*setBlendMode)(THIS_TYPE* _this, leRadialMenuWidgetInterpolationMode mode); \
    leResult  (*setBlendRange)(THIS_TYPE* _this, int32_t min, int32_t max); \
    leResult  (*setTouchArea)(THIS_TYPE* _this, int32_t x, int32_t y, int32_t width, int32_t height); \
    leResult  (*setDrawEllipse)(THIS_TYPE* _this, leBool b); \
    leRadialMenuWidget_ItemProminenceChangedEvent (*getItemProminenceChangedEventCallback)(const THIS_TYPE* _this); \
    leResult  (*setItemProminenceChangedEventCallback)(THIS_TYPE* _this, leRadialMenuWidget_ItemProminenceChangedEvent cb);

typedef struct leRadialMenuWidgetVTable
{
    LE_RADIALMENUWIDGET_VTABLE(leRadialMenuWidget)
} leRadialMenuWidgetVTable;

/**
  * @endcond
  *
  */

enum leRadialMenuItemState
{
    LE_RMI_NOT_VISIBLE,
    LE_RMI_WAITING_FOR_SORT,
    LE_RMI_VISIBLE,
    LE_RMI_PROMINENT
};

/**
 * @brief This struct represents a redial menu item node.
 * @details Used to define the attributes of an radial menu item.
 */
typedef struct leRadialMenuItemNode
{
    leWidget* widget; // point to the widget of the item
    lePoint point;
    int32_t angle;
    int32_t absAngle;
    int32_t adjustedAngle;
    int32_t paintAngle;
    uint32_t percent;

    leRect origSize;  //the original size of the widget, it is a reference point for scaling
    uint32_t origAlphaAmount; //the original alpha value of the widget, it is a reference point for scaling

    enum leRadialMenuItemState state;

} leRadialMenuItemNode;

// *****************************************************************************
/**
 * @brief This struct represents a radial menu widget.
 * @details A radial menu is a master widget that manages the movement, in an
 * elliptical track, of a list of widgets.  It also manages the draw order and
 * scaling of each widget item.
 * It is essentially a group of widgets which provides a mutually exclusive
 * selection capability so that only one item may be selected at any one time.
 */
typedef struct leRadialMenuWidget
{
    leWidget widget; // widget base class

    const leRadialMenuWidgetVTable* fn;

    leRadialMenuWidgetState state;

    int32_t prominentIndex;

    leBool drawEllipse; // indicates if the radial menu is selected
    leBool highlightProminent; // highlight the prominent widget

    struct
    {
        leBool invalid;
        int32_t a;  // the half-length of the 0-180 axis of the ellipse
        int32_t b;  // the half-length of the 90-270 axis of the ellipse
        int32_t theta; // the angle of rotation of the entire ellipse
    } ellipse;

    struct
    {
        uint32_t maxWidth;
        uint32_t maxHeight;
        uint32_t alpha;
        int32_t firstVisibleItem;
        uint32_t lastZ;
    } paintState;

    leBool touchPressed; // keep track of users touch input
    int32_t rotationDegrees;
    int32_t angleSlice;

    leRadialMenuWidgetInterpolationMode scaleMode;  // the enable item size scaling within the widget

    int32_t maxSizePercent;  // the maximum size scale between 1 - 200%
    int32_t minSizePercent;  // the minimum size scale between 1 - 200%

    uint32_t touchX; // the x offset of the touch area as a percentage
    uint32_t touchY; // the y offset of the touch area as a percentage
    uint32_t touchWidth; // the width of the touch area as a percentage
    uint32_t touchHeight; // the height of the touch area as a percentage

    leRect touchArea; // the area specified within the widget that touch events are detected in pixels

    int32_t minFlickDelta; // amount of distance that must be dragged in a single
    // frame to trigger momentum mode

    int32_t momentum; // current momentum value
    int32_t maxMomentum; // maximum momentum value
    int32_t momentumFalloff; // momentum falloff rate
    int32_t rotation; // determines actual rotation of the wheel
    int32_t rotationCounter; // time-based limiter for rotation calculations
    int32_t rotationTick; // rotation time accumulator

    int32_t snapState;
    int32_t snapCounter;
    int32_t snapMagnitude;

    leRadialMenuWidgetInterpolationMode blendMode; // the enable item alpha scaling within the widget

    int32_t maxAlphaAmount;  // the maximum alpha between 0 - 255
    int32_t minAlphaAmount;  // the minimum alpha between 0 - 255

    uint32_t itemsShown; // keeps count of how many items to visible, this number should be less than or equal to total number of widget items
    leList widgetList; // this is the list of widgets
    leBool positionsInvalid;

    leRadialMenuWidget_ItemProminenceChangedEvent itemProminenceChangedEvent; // whenever a new item is in prominence event callback
} leRadialMenuWidget;


// *****************************************************************************
// *****************************************************************************
// Section: Routines
// *****************************************************************************
// *****************************************************************************

/**
 * @brief Create widget.
 * @details Creates a new leRadialMenuWidget and allocates memory for the widget through the
 * current active context.  Application is responsible for managing the widget
 * pointer until the widget is added to a widget tree.
 * @remark use leWidget_Delete() to free this pointer.
 * @code
 * leRadialMenuWidget* wgt = leRadialMenuWidget_New();
 * @endcode
 * @return a widget object pointer.
 */
leRadialMenuWidget* leRadialMenuWidget_New(void);

/**
 * @brief Initialize widget.
 * @details Initializes the leRadialMenuWidget <span class="param">wgt</span>.
 * @code
 * leRadialMenuWidget* wgt;
 * leRadialMenuWidget_Constructor(wgt);
 * @endcode
 * @param wgt is the widget to initialize
 * @return void.
 */
void leRadialMenuWidget_Constructor(leRadialMenuWidget* wgt);

#ifdef _DOXYGEN_
#define THIS_TYPE struct leWidget

// *****************************************************************************
/* Virtual Member Function:
    leBool isProminent(const leRadialMenuWidget* _this,
                       const leWidget* widget)

  Summary:
     Evaluates a widget to see if it is the prominent widget

  Description:
     Evaluates a widget to see if it is the prominent widget

  Parameters:
    const leRadialMenuWidget* _this - The radial menu bar widget to operate on
    const leWidget* widget -

  Remarks:
    Usage - _this->fn->isProminent(_this, widget);

  Returns:
    leBool - the prominent indicator
*/

// *****************************************************************************
/* Virtual Member Function:
    leWidget* getProminent(const leRadialMenuWidget* _this)

  Summary:
     Gets the prominent child widget

  Description:
     Gets the prominent child widget

  Parameters:
    const leRadialMenuWidget* _this - The radial menu widget to operate on

  Remarks:
    Usage - _this->fn->getProminent(_this);

  Returns:
    leWidget* - the prominent child widget
*/

// *****************************************************************************
/* Virtual Member Function:
    leResult setProminent(leRadialMenuWidget* _this,
                          const leWidget* widget)

  Summary:
     Sets a widget as prominent

  Description:
     Sets a widget as prominent

  Parameters:
    leRadialMenuWidget* _this - The radial menu bar widget to operate on
    const leWidget* widget - the widget to test

  Remarks:
    Usage - _this->fn->setProminent(_this, widget);

  Returns:
    leResult - the result of the operation
*/
/**
 * @brief Set widget as prominent.
 * @details Sets a widget <span class="param">widget</span> as
 * prominent using <span class="param">_this</span>.
 * @code
 * const leRadialMenuWidget* _this;
 * leWidget* widget;
 * leResult res = _this->fn->setProminent(_this, widget);
 * @endcode
 * @param _this is the widget pointer to query.
 * @returns LE_TRUE if set, otherwise LE_FALSE;
 */
virtual leResult setProminent(leRadialMenuWidget* _this,
                              const leWidget* widget);


// *****************************************************************************
/* Virtual Member Function:
    int32_t getProminentIndex(const leRadialMenuWidget* _this)

  Summary:
     Gets the prominent item index

  Description:
     Gets the prominent item index

  Parameters:
    const leRadialMenuWidget* _this - The radial menu bar widget to operate on

  Remarks:
    Usage - _this->fn->getProminentIndex(_this);

  Returns:
    int32_t - the prominent index
*/
/**
 * @brief Get prominent item index.
 * @details Gets the prominent item index using <span class="param">_this</span>.
 * @code
 * const leRadialMenuWidget* _this;
 * leBool prom = _this->fn->getProminentIndex(_this);
 * @endcode
 * @param _this is the widget pointer to query.
 * @returns the prominent index;
 */
virtual int32_t getProminentIndex(const leRadialMenuWidget* _this);

// *****************************************************************************
/* Virtual Member Function:
    leResult setProminentIndex(leRadialMenuWidget* _this,
                               int32_t index)

  Summary:
     Sets the prominent item index

  Description:
     Sets the prominent item index

  Parameters:
    leRadialMenuWidget* _this - The radial menu bar widget to operate on
    int32_t index - the desired prominent index

  Remarks:
    Usage - _this->fn->setProminentIndex(_this, index);

  Returns:
    leResult - the result of the operation
*/
/**
 * @brief Set prominent item index.
 * @details Sets the prominent item at index <span class="param">index</span> as
 * prominent using <span class="param">_this</span>.
 * @code
 * const leRadialMenuWidget* _this;
 * int32_t index;
 * leResult res = _this->fn->setProminentIndex(_this, index);
 * @endcode
 * @param _this is the widget pointer to query.
 * @returns LE_TRUE if set, otherwise LE_FALSE;
 */
virtual leResult setProminentIndex(leRadialMenuWidget* _this,
                                   int32_t index);

// *****************************************************************************
/* Virtual Member Function:
    leResult setNumberOfItemsShown(leRadialMenuWidget* _this,
                                   uint32_t cnt)

  Summary:
     Sets the number of items shown

  Description:
     Sets the number of items shown

  Parameters:
    leRadialMenuWidget* _this - The radial menu bar widget to operate on
    uint32_t cnt - the item count

  Remarks:
    Usage - _this->fn->setNumberOfItemsShown(_this, cnt);

  Returns:
    leResult - the result of the operation
*/

// *****************************************************************************
/* Virtual Member Function:
    leResult setHighlightProminent(leRadialMenuWidget* _this,
                                   leBool hl)

  Summary:
     Sets the highlight prominent setting value

  Description:
     Sets the highlight prominent setting value

  Parameters:
    leRadialMenuWidget* _this - The radial menu bar widget to operate on
    leBool hl - the setting value

  Remarks:
    Usage - _this->fn->setHighlightProminent(_this, hl);

  Returns:
    leResult - the result of the operation
*/
/**
 * @brief Set highlight prominent setting value.
 * @details Sets the highlight prominent setting value to
 *  <span class="param">hl</span> using <span class="param">_this</span>.
 * @code
 * const leRadialMenuWidget* _this;
 * leBool hl;
 * leResult res = _this->fn->setHighlightProminent(_this, hl);
 * @endcode
 * @param _this is the widget pointer to query.
 * @returns LE_TRUE if set, otherwise LE_FALSE;
 */
virtual leResult setHighlightProminent(leRadialMenuWidget* _this,
                                       leBool hl);

// *****************************************************************************
/* Virtual Member Function:
    int32_t getTheta(const leRadialMenuWidget* _this)

  Summary:
     Gets the menu ellipse rotation coefficient

  Description:
     Gets the menu ellipse rotation coefficient

  Parameters:
    const leRadialMenuWidget* _this - The radial menu bar widget to operate on

  Remarks:
    Usage - _this->fn->getTheta(_this);

  Returns:
    int32_t - the theta value
*/
/**
 * @brief Get menu ellipse rotation coefficient.
 * @details Gets the menu ellipse rotation coefficient using <span class="param">_this</span>.
 * @code
 * const leRadialMenuWidget* _this;
 * int32_t theta = _this->fn->getTheta(_this);
 * @endcode
 * @param _this is the widget pointer to query.
 * @returns the theta value;
 */
virtual int32_t getTheta(const leRadialMenuWidget* _this);

// *****************************************************************************
/* Virtual Member Function:
    leResult setTheta(leRadialMenuWidget* _this,
                      int32_t tht)

  Summary:
     Sets the menu ellipse rotation coefficient

  Description:
     Sets the menu ellipse rotation coefficient

  Parameters:
    leRadialMenuWidget* _this - The radial menu bar widget to operate on
    int32_t tht - the theta value

  Remarks:
    Usage - _this->fn->setTheta(_this, tht);

  Returns:
    leResult - the result of the operation
*/
/**
 * @brief Set menu ellipse rotation coefficient.
 * @details Sets the menu ellipse rotation coefficient to
 *  <span class="param">tht</span> using <span class="param">_this</span>.
 * @code
 * const leRadialMenuWidget* _this;
 * int32_t tht;
 * leResult res = _this->fn->setTheta(_this, tht);
 * @endcode
 * @param _this is the widget pointer to query.
 * @returns LE_TRUE if set, otherwise LE_FALSE;
 */
virtual leResult setTheta(leRadialMenuWidget* _this,
                          int32_t tht);

// *****************************************************************************
/* Virtual Member Function:
    leResult setEllipseType(leRadialMenuWidget* _this,
                            leRadialMenuEllipseType type)

  Summary:
     Sets the menu ellipse type

  Description:
     Sets the menu ellipse type

  Parameters:
    leRadialMenuWidget* _this - The radial menu bar widget to operate on
    leRadialMenuEllipseType type - the type

  Remarks:
    Usage - _this->fn->setEllipseType(_this, type);

  Returns:
    leResult - the result of the operation
*/
/**
 * @brief Set menu ellipse type.
 * @details Sets the menu ellipse type to
 *  <span class="param">type</span> using <span class="param">_this</span>.
 * @code
 * const leRadialMenuWidget* _this;
 * leRadialMenuEllipseType type;
 * leResult res = _this->fn->setEllipseType(_this, type);
 * @endcode
 * @param _this is the widget pointer to query.
 * @returns LE_TRUE if set, otherwise LE_FALSE;
 */
virtual leResult setEllipseType(leRadialMenuWidget* _this,
                                leRadialMenuEllipseType type);

// *****************************************************************************
/* Virtual Member Function:
    leResult addWidget(leRadialMenuWidget* _this,
                       leWidget* wgt)

  Summary:
     Adds a widget to the ellipse

  Description:
     Adds a widget to the ellipse

  Parameters:
    leRadialMenuWidget* _this - The radial menu bar widget to operate on
    leWidget* wgt - the widget to add

  Remarks:
    Usage - _this->fn->addWidget(_this, wgt);

  Returns:
    leResult - the result of the operation
*/
/**
 * @brief Add widget to the ellipse.
 * @details Adds a widget <span class="param">wgt</span> to the ellipse
 *  using <span class="param">_this</span>.
 * @code
 * const leRadialMenuWidget* _this;
 * leWidget* wgt;
 * leResult res = _this->fn->addWidget(_this, wgt);
 * @endcode
 * @param _this is the widget pointer to query.
 * @returns LE_TRUE if set, otherwise LE_FALSE;
 */
virtual leResult addWidget(leRadialMenuWidget* _this,
                           leWidget* wgt);

// *****************************************************************************
/* Virtual Member Function:
    leResult removeWidget(leRadialMenuWidget* _this,
                          leWidget* wgt)

  Summary:
     Removes a widget from the ellipse

  Description:
     Removes a widget from the ellipse

  Parameters:
    leRadialMenuWidget* _this - The radial menu bar widget to operate on
    leWidget* wgt - the widget to remove

  Remarks:
    Usage - _this->fn->removeWidget(_this, wgt);

  Returns:
    leResult - the result of the operation
*/
/**
 * @brief Remove widget to the ellipse.
 * @details Removes a widget <span class="param">wgt</span> to the ellipse
 *  using <span class="param">_this</span>.
 * @code
 * const leRadialMenuWidget* _this;
 * leWidget* wgt;
 * leResult res = _this->fn->addWidget(_this, wgt);
 * @endcode
 * @param _this is the widget pointer to query.
 * @returns LE_TRUE if set, otherwise LE_FALSE;
 */
virtual leResult removeWidget(leRadialMenuWidget* _this,
                              leWidget* wgt);

// *****************************************************************************
/* Virtual Member Function:
    leWidget* getWidgetAtIndex(const leRadialMenuWidget* _this,
                               int32_t idx)

  Summary:
     Gets a widget at a given index

  Description:
     Gets a widget at a given index

  Parameters:
    const leRadialMenuWidget* _this - The radial menu bar widget to operate on
    int32_t idx - the index

  Remarks:
    Usage - _this->fn->getWidgetAtIndex(_this, idx);

  Returns:
    leWidget* - the widget
*/
/**
 * @brief Get widget at a given index.
 * @details Gets the widget at <span class="param">idx</span> using <span class="param">_this</span>.
 * @code
 * const leRadialMenuWidget* _this;
 * int32_t idx;
 * int32_t theta = _this->fn->getWidgetAtIndex(_this, idx);
 * @endcode
 * @param _this is the widget pointer to query.
 * @returns the widget;
 */
virtual leWidget* getWidgetAtIndex(const leRadialMenuWidget* _this,
                                   int32_t idx);

// *****************************************************************************
/* Virtual Member Function:
    leResult setWidgetAtIndex(leRadialMenuWidget* _this,
                              int32_t idx,
                              leWidget* wgt)

  Summary:
     Sets a widget at a given index

  Description:
     Sets a widget at a given index

  Parameters:
    leRadialMenuWidget* _this - The radial menu bar widget to operate on
    int32_t idx - the index
    leWidget* wgt - the widget to set

  Remarks:
    Usage - _this->fn->setWidgetAtIndex(_this, idx, wgt);

  Returns:
    leResult - the result of the operation
*/
/**
 * @brief Set widget at a given index.
 * @details Sets a widget <span class="param">wgt</span> at a given
 * index <span class="param">idx</span>
 *  using <span class="param">_this</span>.
 * @code
 * const leRadialMenuWidget* _this;
 * leWidget* wgt;
 * leResult res = _this->fn->setWidgetAtIndex(_this, idx, wgt);
 * @endcode
 * @param _this is the widget pointer to query.
 * @returns LE_TRUE if set, otherwise LE_FALSE;
 */
virtual leResult setWidgetAtIndex(leRadialMenuWidget* _this,
                                  int32_t idx,
                                  leWidget* wgt);

// *****************************************************************************
/* Virtual Member Function:
    leResult removeAllWidgets(leRadialMenuWidget* _this)

  Summary:
     Removes all widgets from the menu

  Description:
     Removes all widgets from the menu

  Parameters:
    leRadialMenuWidget* _this - The radial menu bar widget to operate on

  Remarks:
    Usage - _this->fn->removeAllWidgets(_this);

  Returns:
    leResult - the result of the operation
*/
/**
 * @brief Removes all widgets from the menu.
 * @details Remove all widgets from the menu using <span class="param">_this</span>.
 * @code
 * const leRadialMenuWidget* _this;
 * leWidget* wgt;
 * leResult res = _this->fn->removeAllWidgets(_this);
 * @endcode
 * @param _this is the widget pointer to query.
 * @returns LE_TRUE if set, otherwise LE_FALSE;
 */
virtual leResult removeAllWidgets(leRadialMenuWidget* _this);

// *****************************************************************************
/* Virtual Member Function:
    leResult setScaleMode(leRadialMenuWidget* _this,
                          leRadialMenuWidgetInterpolationMode mode)

  Summary:
     Sets the menu item scaling mode

  Description:
     Sets the menu item scaling mode

  Parameters:
    leRadialMenuWidget* _this - The radial menu bar widget to operate on
    leRadialMenuWidgetInterpolationMode mode -

  Remarks:
    Usage - _this->fn->setScaleMode(_this, mode);

  Returns:
    leResult - the result of the operation
*/
/**
 * @brief Sets menu item scaling mode.
 * @details Set all menu item to scaling mode <span class="param">mode</span>
 * using <span class="param">_this</span>.
 * @code
 * const leRadialMenuWidget* _this;
 * leRadialMenuWidgetInterpolationMode mode;
 * leResult res = _this->fn->setScaleMode(_this, mode);
 * @endcode
 * @param _this is the widget pointer to query.
 * @returns LE_TRUE if set, otherwise LE_FALSE;
 */
virtual leResult setScaleMode(leRadialMenuWidget* _this,
                              leRadialMenuWidgetInterpolationMode mode);

// *****************************************************************************
/* Virtual Member Function:
    leResult setScaleRange(leRadialMenuWidget* _this,
                           int32_t min,
                           int32_t max)

  Summary:
     Sets the scale range for menu items

  Description:
     Sets the scale range for menu items

  Parameters:
    leRadialMenuWidget* _this - The radial menu bar widget to operate on
    int32_t min - the minimum value
    int32_t max - the maximum value

  Remarks:
    Usage - _this->fn->setScaleRange(_this, min, max);

  Returns:
    leResult - the result of the operation
*/
/**
 * @brief Sets scale range for menu items.
 * @details Sets the scale range for menu items from <span class="param">min</span>
 * to <span class="param">max</span> using <span class="param">_this</span>.
 * @code
 * const leRadialMenuWidget* _this;
 * int32_t min;
 * int32_t max;
 * leResult res = _this->fn->setScaleRange(_this, min, max);
 * @endcode
 * @param _this is the widget pointer to query.
 * @returns LE_TRUE if set, otherwise LE_FALSE;
 */
virtual leResult setScaleRange(leRadialMenuWidget* _this,
                               int32_t min,
                               int32_t max);

// *****************************************************************************
/* Virtual Member Function:
    leResult setBlendMode(leRadialMenuWidget* _this,
                          leRadialMenuWidgetInterpolationMode mode)

  Summary:
     Sets the menu item blending mode

  Description:
     Sets the menu item blending mode

  Parameters:
    leRadialMenuWidget* _this - The radial menu bar widget to operate on
    leRadialMenuWidgetInterpolationMode mode -

  Remarks:
    Usage - _this->fn->setBlendMode(_this, mode);

  Returns:
    leResult - the result of the operation
*/
/**
 * @brief Sets menu item blending mode.
 * @details Sets the menu item blending mode to <span class="param">mode</span>
 * using <span class="param">_this</span>.
 * @code
 * const leRadialMenuWidget* _this;
 * leRadialMenuWidgetInterpolationMode mode;
 * int32_t max;
 * leResult res = _this->fn->setBlendMode(_this, mode);
 * @endcode
 * @param _this is the widget pointer to query.
 * @returns LE_TRUE if set, otherwise LE_FALSE;
 */
virtual leResult setBlendMode(leRadialMenuWidget* _this,
                              leRadialMenuWidgetInterpolationMode mode);

// *****************************************************************************
/* Virtual Member Function:
    leResult setBlendRange(leRadialMenuWidget* _this,
                           int32_t min,
                           int32_t max)

  Summary:
     Sets the blending range

  Description:
     Sets the blending range

  Parameters:
    leRadialMenuWidget* _this - The radial menu bar widget to operate on
    int32_t min - the minimum value
    int32_t max - the maximum value

  Remarks:
    Usage - _this->fn->setBlendRange(_this, min, max);

  Returns:
    leResult - the result of the operation
*/
/**
 * @brief Sets blending range.
 * @details Sets the blending range from <span class="param">min</span>
 * to <span class="param">max</span> using <span class="param">_this</span>.
 * @code
 * const leRadialMenuWidget* _this;
 * leRadialMenuWidgetInterpolationMode mode;
 * int32_t min;
 * int32_t max;
 * leResult res = _this->fn->setBlendRange(_this, min, max);
 * @endcode
 * @param _this is the widget pointer to query.
 * @returns LE_TRUE if set, otherwise LE_FALSE;
 */
virtual leResult setBlendRange(leRadialMenuWidget* _this,
                               int32_t min,
                               int32_t max);

// *****************************************************************************
/* Virtual Member Function:
    leResult setTouchArea(leRadialMenuWidget* _this,
                          int32_t x,
                          int32_t y,
                          int32_t width,
                          int32_t height)

  Summary:
     Sets the menu touch area

  Description:
     Sets the menu touch area

  Parameters:
    leRadialMenuWidget* _this - The radial menu bar widget to operate on
    int32_t x - the X value
    int32_t y - the Y value
    int32_t width - the width value
    int32_t height - the height value

  Remarks:
    Usage - _this->fn->setTouchArea(_this, x, y, width, height);

  Returns:
    leResult - the result of the operation
*/
/**
 * @brief Sets menu touch area.
 * @details Sets the menu touch area bounded by <span class="param">x</span>
 * and <span class="param">y</span> to <span class="param">width</span> and
 * <span class="param">height</span> using <span class="param">_this</span>.
 * @code
 * const leRadialMenuWidget* _this;
 * leRadialMenuWidgetInterpolationMode mode;
 * int32_t x, y, width, height;
 * leResult res = _this->fn->setTouchArea(_this, x, y, width, height);
 * @endcode
 * @param _this is the widget pointer to query.
 * @returns LE_TRUE if set, otherwise LE_FALSE;
 */
virtual leResult setTouchArea(leRadialMenuWidget* _this,
                              int32_t x,
                              int32_t y,
                              int32_t width,
                              int32_t height);

// *****************************************************************************
/* Virtual Member Function:
    leResult setDrawEllipse(leRadialMenuWidget* _this,
                            leBool b)

  Summary:
     Sets the draw ellipse setting value

  Description:
     Sets the draw ellipse setting value

  Parameters:
    leRadialMenuWidget* _this - The radial menu bar widget to operate on
    leBool b - the setting value

  Remarks:
    Usage - _this->fn->setDrawEllipse(_this, b);

  Returns:
    leResult - the result of the operation
*/
/**
 * @brief Sets draw ellipse setting value.
 * @details Sets the draw ellipse setting value to <span class="param">b</span>
 * using <span class="param">_this</span>.
 * @code
 * const leRadialMenuWidget* _this;
 * leBool b;
 * leResult res = _this->fn->setDrawEllipse(_this, b);
 * @endcode
 * @param _this is the widget pointer to query.
 * @returns LE_TRUE if set, otherwise LE_FALSE;
 */
virtual leResult setDrawEllipse(leRadialMenuWidget* _this,
                                leBool b);

/**
 * @brief Get item prominence changed event callback pointer.
 * @details Gets the item prominence changed event callback pointer using <span class="param">_this</span>.
 * @code
 * leRadialMenuWidget* _this;
 * leRadialMenuWidget_ItemSelectedEvent cb = _this->fn->getItemSelectedEventCallback(_this);
 * @endcode
 * @param _this is the widget to modify
 * @returns LE_SUCCESS if set, otherwise LE_FAILURE.
 */
virtual leRadialMenuWidget_ItemSelectedEvent getItemSelectedEventCallback(const leRadialMenuWidget* _this)

/**
 * @brief Set item prominence changed event callback pointer.
 * @details Sets the item prominence changed event callback pointer to <span class="param">cb</span>
 * using <span class="param">_this</span>.
 * @code
 * leRadialMenuWidget* _this;
 * leRadialMenuWidget_ItemSelectedEvent cb;
 * leResult res = _this->fn->setItemSelectedEventCallback(_this, cb);
 * @endcode
 * @param _this is the widget to modify
 * @param cb is the callback func
 * @returns LE_SUCCESS if set, otherwise LE_FAILURE.
 */
virtual leResult setItemSelectedEventCallback(leRadialMenuWidget* _this,
                                              leRadialMenuWidget_ItemSelectedEvent cb);

/**
 * @brief Get item prominence changed event callback pointer.
 * @details Gets the item prominence changed event callback pointer using <span class="param">_this</span>.
 * @code
 * leRadialMenuWidget* _this;
 * leRadialMenuWidget_ItemProminenceChangedEvent cb = _this->fn->getItemProminenceChangedEventCallback(_this);
 * @endcode
 * @param _this is the widget to modify
 * @returns LE_SUCCESS if set, otherwise LE_FAILURE.
 */
virtual leRadialMenuWidget_ItemProminenceChangedEvent getItemProminenceChangedEventCallback
                                                    (const leRadialMenuWidget* _this)

/**
 * @brief Set item prominence changed event callback pointer.
 * @details Sets the item prominence changed event callback pointer to <span class="param">cb</span>
 * using <span class="param">_this</span>.
 * @code
 * leRadialMenuWidget* _this;
 * leRadialMenuWidget_ItemProminenceChangedEvent cb;
 * leResult res = _this->fn->setItemProminenceChangedEventCallback(_this, cb);
 * @endcode
 * @param _this is the widget to modify
 * @param cb is the callback func
 * @returns LE_SUCCESS if set, otherwise LE_FAILURE.
 */
virtual leResult setItemProminenceChangedEventCallback(leRadialMenuWidget* _this,
                                                       leRadialMenuWidget_ItemProminenceChangedEvent cb)

#undef THIS_TYPE
#endif

#ifdef __cplusplus
}
#endif

#endif // LE_RADIALMENU_WIDGET_ENABLED
#endif /* LEGATO_RADIALMENU_H */
