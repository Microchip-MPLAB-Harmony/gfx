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
    legato_widget_radial_menu.h

  Summary:
    

  Description:
    This module implements radial menu widget functions.
*******************************************************************************/

// DOM-IGNORE-BEGIN
#ifndef LEGATO_RADIALMENU_H
#define LEGATO_RADIALMENU_H
//DOM-IGNORE-END

#include "gfx/legato/common/legato_common.h"

#if LE_RADIALMENU_WIDGET_ENABLED == 1

#include "gfx/legato/widget/legato_widget.h"
#include "gfx/legato/widget/image/legato_widget_image.h"
#include "gfx/legato/datastructure/legato_list.h"
#include "gfx/legato/string/legato_string.h"

typedef struct leRadialMenuWidget leRadialMenuWidget;

typedef void (*leRadialMenuWidget_ItemSelectedEvent)(leRadialMenuWidget*, leWidget*, int32_t);
typedef void (*leRadialMenuWidget_ItemProminenceChangedEvent)(leRadialMenuWidget*, leWidget*, int32_t);

// *****************************************************************************
// *****************************************************************************
// Section: Data Types and Constants
// *****************************************************************************
// *****************************************************************************

typedef enum leRadialMenuWidgetState
{
    LE_RADIAL_MENU_INIT,
	LE_RADIAL_MENU_INPUT_READY,
	LE_RADIAL_MENU_HANDLE_USER_MOVE_REQUEST,
    LE_RADIAL_MENU_RESET_TO_INPUT_POS            
} leRadialMenuWidgetState;

typedef enum leRadialMenuWidgetInterpolationMode
{
	LE_RADIAL_MENU_INTERPOLATE_OFF,
	LE_RADIAL_MENU_INTERPOLATE_GRADUAL,
	LE_RADIAL_MENU_INTERPOLATE_PROMINENT
} leRadialMenuWidgetInterpolationMode;

typedef enum leRadialMenuEllipseType
{
	LE_RADIAL_MENU_ELLIPSE_TYPE_DEFAULT,
	LE_RADIAL_MENU_ELLIPSE_TYPE_ORBITAL,
	LE_RADIAL_MENU_ELLIPSE_TYPE_ROLLODEX
} leRadialMenuEllipseType;

// DOM-IGNORE-BEGIN
typedef struct leRadialMenuWidget leRadialMenuWidget;

#define LE_RADIALMENUWIDGET_VTABLE(THIS_TYPE) \
    LE_WIDGET_VTABLE(THIS_TYPE) \
    \
    leBool    (*isProminent)(const THIS_TYPE* _this, const leWidget* widget); \
    leResult  (*setProminent)(THIS_TYPE* _this, const leWidget* widget); \
    int32_t   (*getProminentIndex)(const THIS_TYPE* _this); \
    leResult  (*setProminentIndex)(THIS_TYPE* _this, int32_t index); \
    leResult  (*setNumberOfItemsShown)(THIS_TYPE* _this, uint32_t cnt); \
    leResult  (*setHighlightProminent)(THIS_TYPE* _this, leBool hl); \
    int32_t   (*getTheta)(const THIS_TYPE* _this); \
    leResult  (*setTheta)(THIS_TYPE* _this, int32_t tht); \
    leResult  (*setEllipseType)(THIS_TYPE* _this, leRadialMenuEllipseType type); \
    leResult  (*addWidget)(THIS_TYPE* _this, leWidget* wgt); \
    leResult  (*removeWidget)(THIS_TYPE* _this, leWidget* wgt); \
    leWidget* (*getWidgetAtIndex)(const THIS_TYPE* _this, int32_t idx); \
    leResult  (*setWidgetAtIndex)(THIS_TYPE* _this, int32_t idx, leWidget* wgt); \
    leResult  (*removeAllWidgets)(THIS_TYPE* _this); \
    leResult  (*setScaleMode)(THIS_TYPE* _this, leRadialMenuWidgetInterpolationMode mode); \
    leResult  (*setScaleRange)(THIS_TYPE* _this, int32_t min, int32_t max); \
    leResult  (*setBlendMode)(THIS_TYPE* _this, leRadialMenuWidgetInterpolationMode mode); \
    leResult  (*setBlendRange)(THIS_TYPE* _this, int32_t min, int32_t max); \
    leResult  (*setTouchArea)(THIS_TYPE* _this, int32_t x, int32_t y, int32_t width, int32_t height); \
    leResult  (*setDrawEllipse)(THIS_TYPE* _this, leBool b); \
    leRadialMenuWidget_ItemSelectedEvent (*getItemSelectedEventCallback)(const THIS_TYPE* _this); \
    leResult  (*setItemSelectedEventCallback)(THIS_TYPE* _this, leRadialMenuWidget_ItemSelectedEvent cb); \
    leRadialMenuWidget_ItemProminenceChangedEvent (*getItemProminenceChangedEventCallback)(const THIS_TYPE* _this); \
    leResult  (*setItemProminenceChangedEventCallback)(THIS_TYPE* _this, leRadialMenuWidget_ItemProminenceChangedEvent cb); \
    
typedef struct leRadialMenuWidgetVTable
{
	LE_RADIALMENUWIDGET_VTABLE(leRadialMenuWidget)
} leRadialMenuWidgetVTable; 

// DOM-IGNORE-END

typedef struct leRadialMenuItemNode
{
	leWidget* widget; // point to the widget of the item
	int32_t t; // the angle in degress between 0 - 360, representing the relative position of the item on the track
	
	//leWidgetInputHandler inputHandler; 
    
	//leWidget_TouchDownEvent_FnPtr origTouchDown; //the widget item's original touch down event, 
                                                 //allows the radial menu to work as a hub to route to the appropriate widget
	//leWidget_TouchUpEvent_FnPtr origTouchUp; //the widget item's original touch up event
                                                 //allows the radial menu to work as a hub to route to the appropriate widget
	//leWidget_TouchMovedEvent_FnPtr origTouchMoved; //the widget item's original touch move event

    leRect origSize;  //the original size of the widget, it is a reference point for scaling
    uint32_t origAlphaAmount; //the original alpha value of the widget, it is a reference point for scaling
    
} leRadialMenuItemNode;

// *****************************************************************************
/* Enumeration:
    leRadialMenuWidget

  Summary:
    Implementation of a radial menu widget struct

  Description:
    A radial menu is a master widget that manages the movement, in an elliptical 
	track, of a list of widgets.  It also manages the draw order and scaling of 
	each widget item.

	It is essentially a group of widgets which provides a mutually exclusive 
	selection capability so that only one item may be selected at any one time.

  Remarks:
    None.
*/
typedef struct leRadialMenuWidget
{
    leWidget widget; // widget base class
    
    const leRadialMenuWidgetVTable* fn;

	leRadialMenuWidgetState state;

    int32_t prominentIndex;
    int32_t lastProminentIndex;
	int32_t userRequestedAngleDiff; // the angle for the radial menu to rotate as requested by user
	int32_t targetAngleDiff; // the angle for the radial menu to rotate for prominent item to be in front
    int32_t userRequestedDirection; // tracks the direction that the user requested for rotation
    
    leBool drawEllipse; // indicates if the radial menu is selected
    leBool highlightProminent; // highlight the prominent widget

	leImageWidget* highlighter; // this widget manages the selector art asset
    
	int32_t a;  // the half-length of the 0-180 axis of the ellipse
	int32_t b;  // the half-length of the 90-270 axis of the ellipse
	int32_t theta; // the angle of rotation of the entire ellipse
	
    leBool touchPressed; // keep track of users touch input
    
    leBool ellipseChanged; // keeps track if the elliptical track has changed
    
	leRadialMenuWidgetInterpolationMode scaleMode;  // the enable item size scaling within the widget

	int32_t maxSizePercent;  // the maximum size scale between 1 - 200%
	int32_t minSizePercent;  // the minimum size scale between 1 - 200%

	leRect touchArea; // the area specified within the widget that touch events are detected

	leRadialMenuWidgetInterpolationMode blendMode; // the enable item alpha scaling within the widget

	int32_t maxAlphaAmount;  // the maximum alpha between 0 - 255
	int32_t minAlphaAmount;  // the minimum alpha between 0 - 255

    uint32_t itemsShown; // keeps count of how many items to visible, this number should be less than or equal to total number of widget items
    leList widgetList; // this is the list of widgets
    leList shownList; // this is the partial list of widgets shown
    leList hiddenList; // this is the partial list of widgets hidden

    leRadialMenuItemNode* widestWidgetItem; // keeps track of which widget is the widgets for major axis calculation
    leRadialMenuItemNode* tallestWidgetItem; // keeps track of which widget is the widgets for minor axis calculation
    
    leRadialMenuWidget_ItemSelectedEvent itemSelectedEvent; // an item is selected event callback
	leRadialMenuWidget_ItemProminenceChangedEvent itemProminenceChangedEvent; // whenever a new item is in prominence event callback

    leRadialMenuEllipseType ellipseType;
} leRadialMenuWidget;


// *****************************************************************************
// *****************************************************************************
// Section: Routines
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/* Function:
    leRadialMenuWidget* leRadialMenuWidget_New()

  Summary:
    Allocates memory for a new widget of this type.  The application is
    responsible for the management of this memory until the widget is added to
    a widget tree.

  Description:
    

  Parameters:
    
  Returns:
    leRadialMenuWidget*
    
  Remarks:
    Use leWidget_Delete() to free this pointer.
*/
LIB_EXPORT leRadialMenuWidget* leRadialMenuWidget_New();

/* Function:
    void leRadialMenuWidget_Constructor(leRadialMenuWidget* wgt)

  Summary:
    Initializes an leRadialMenuWidget widget pointer.

  Description:
    Initializes an leRadialMenuWidget widget pointer.

  Parameters:
    leRadialMenuWidget* wgt - the pointer to initialize

  Returns:
    void

  Remarks:

*/
LIB_EXPORT void leRadialMenuWidget_Constructor(leRadialMenuWidget* rad);

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

// *****************************************************************************
/* Virtual Member Function:
    leRadialMenuWidget_ItemSelectedEvent getItemSelectedEventCallback(const leRadialMenuWidget* _this)

  Summary:
     Gets the item selected event callback pointer

  Description:
     Gets the item selected event callback pointer

  Parameters:
    const leRadialMenuWidget* _this - The radial menu bar widget to operate on

  Remarks:
    Usage - _this->fn->getItemSelectedEventCallback(_this);

  Returns:
    leRadialMenuWidget_ItemSelectedEvent - the callback pointer
*/

// *****************************************************************************
/* Virtual Member Function:
    leResult setItemSelectedEventCallback(leRadialMenuWidget* _this,
                                          leRadialMenuWidget_ItemSelectedEvent cb)

  Summary:
     Sets the item selected event callback pointer

  Description:
     Sets the item selected event callback pointer

  Parameters:
    leRadialMenuWidget* _this - The radial menu bar widget to operate on
    leRadialMenuWidget_ItemSelectedEvent cb - the callback pointer

  Remarks:
    Usage - _this->fn->setItemSelectedEventCallback(_this, cb);

  Returns:
    leResult - the result of the operation
*/

// *****************************************************************************
/* Virtual Member Function:
    leRadialMenuWidget_ItemProminenceChangedEvent getItemProminenceChangedEventCallback(const leRadialMenuWidget* _this)

  Summary:
     Gets the item prominence changed event callback pointer

  Description:
     Gets the item prominence changed event callback pointer

  Parameters:
    const leRadialMenuWidget* _this - The radial menu bar widget to operate on

  Remarks:
    Usage - _this->fn->getItemProminenceChangedEventCallback(_this);

  Returns:
    leRadialMenuWidget_ItemProminenceChangedEvent - the callback pointer
*/

// *****************************************************************************
/* Virtual Member Function:
    leResult setItemProminenceChangedEventCallback(leRadialMenuWidget* _this,
                                                   leRadialMenuWidget_ItemProminenceChangedEvent cb)

  Summary:
     Sets the item prominence changed event callback pointer

  Description:
     Sets the item prominence changed event callback pointer

  Parameters:
    leRadialMenuWidget* _this - The radial menu bar widget to operate on
    leRadialMenuWidget_ItemProminenceChangedEvent cb - the callback pointer

  Remarks:
    Usage - _this->fn->setItemProminenceChangedEventCallback(_this, cb);

  Returns:
    leResult - the result of the operation
*/



// DOM-IGNORE-BEGIN
// internal use only
leWidgetUpdateState _leRadialMenuWidget_Update(leRadialMenuWidget* mn);
// DOM-IGNORE-END

#endif // LE_RADIALMENU_WIDGET_ENABLED
#endif /* LEGATO_RADIALMENU_H */