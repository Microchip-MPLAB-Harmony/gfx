// DOM-IGNORE-BEGIN
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
// DOM-IGNORE-END

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

// DOM-IGNORE-BEGIN
#ifndef LEGATO_LISTWHEELWIDGET_H
#define LEGATO_LISTWHEELWIDGET_H
//DOM-IGNORE-END

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
typedef struct leListWheelItem
{
    const leString* string;
    const leImage* icon;
} leListWheelItem;

// DOM-IGNORE-BEGIN
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

// DOM-IGNORE-END

// *****************************************************************************
/* Structure:
    leListWheelWidget

  Summary:
    Implementation of a list wheel widget struct

  Description:
    A list wheel widget is a widget that is similar to a normal list widget
    but can be dragged up or down to cycle through a single active value.  This
    widget is also capable of momentum and motion over time.

  Remarks:
    None.
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

// *****************************************************************************
/* Function:
    leListWheelWidget* leListWheelWidget_New()

  Summary:
    Allocates memory for a new widget of this type.  The application is
    responsible for the managment of this memory until the widget is added to
    a widget tree.

  Description:


  Parameters:

  Returns:
    leListWheelWidget*

  Remarks:
    Use leWidget_Delete() to free this pointer.
*/
LIB_EXPORT leListWheelWidget* leListWheelWidget_New();

/* Function:
    void leListWheelWidget_Constructor(leListWheelWidget* wgt)

  Summary:
    Initializes an leListWheelWidget widget pointer.

  Description:
    Initializes an leListWheelWidget widget pointer.

  Parameters:
    leListWheelWidget* wgt - the pointer to initialize

  Returns:
    void

  Remarks:

*/
LIB_EXPORT void leListWheelWidget_Constructor(leListWheelWidget* whl);

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

// *****************************************************************************
/* Virtual Member Function:
    leListWheelWidget_SelectedItemChangedEvent getSelectedItemChangedEventCallback(const leListWheelWidget* _this)

  Summary:
     Gets the selected item changed event callback pointer.

  Description:
     Gets the selected item changed event callback pointer.

  Parameters:
    const leListWheelWidget* _this - The list wheel widget to operate on

  Remarks:
    Usage - _this->fn->getSelectedItemChangedEventCallback(_this);

  Returns:
    leListWheelWidget_SelectedItemChangedEvent - the callback pointer
*/

// *****************************************************************************
/* Virtual Member Function:
    leResult setSelectedItemChangedEventCallback(leListWheelWidget* _this,
                                                 leListWheelWidget_SelectedItemChangedEvent cb)

  Summary:
     Sets the selected item changed event callback pointer.

  Description:
     Sets the selected item changed event callback pointer.

  Parameters:
    leListWheelWidget* _this - The list wheel widget to operate on
    leListWheelWidget_SelectedItemChangedEvent cb - the callback pointer

  Remarks:
    Usage - _this->fn->setSelectedItemChangedEventCallback(_this, cb);

  Returns:
    leResult - the result of the operation
*/

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



#endif // LE_LISTWHEEL_WIDGET_ENABLED
#endif /* LEGATO_LISTWHEELWIDGET_H */
