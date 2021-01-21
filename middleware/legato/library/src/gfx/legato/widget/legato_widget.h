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
    legato_widget.h

  Summary:


  Description:
    This module implements top level widget control functions.
*******************************************************************************/

/** \file legato_widget.h
 * @brief Legato widget definitions.
 *
 * @details Defines the widget that are used in the UI library.  Widgets
 * are created and stored for later processing during a library context's update loop.
 */

#ifndef LEGATO_WIDGET_H
#define LEGATO_WIDGET_H


#include "gfx/legato/common/legato_common.h"
#include "gfx/legato/common/legato_rect.h"
#include "gfx/legato/core/legato_event.h"
#include "gfx/legato/core/legato_scheme.h"
#include "gfx/legato/datastructure/legato_array.h"

/* internal use only */
/**
  * @cond INTERNAL
  *
  */#define DEFAULT_BORDER_MARGIN   4
/**
  * @endcond
  *
  */
// *****************************************************************************
// *****************************************************************************
// Section: Data Types and Constants
// *****************************************************************************
// *****************************************************************************

/*
Enumeration:
        leWidgetType

    Summary:
        Specifies the different widget types used in the library

    Description:
        This enumeration specifies the different widget types
        used in the library.

    Remarks:
        None.
 */
/**
 * @brief Used to define widget types.
 * @details Used to define widget types.
 */
typedef enum leWidgetType
{
    LE_WIDGET_WIDGET,
#if LE_ARC_WIDGET_ENABLED == 1
    LE_WIDGET_ARC,
#endif
#if LE_BARGRAPH_WIDGET_ENABLED == 1
    LE_WIDGET_BAR_GRAPH,
#endif
#if LE_BUTTON_WIDGET_ENABLED == 1
    LE_WIDGET_BUTTON,
#endif
#if LE_CHECKBOX_WIDGET_ENABLED == 1
    LE_WIDGET_CHECKBOX,
#endif
#if LE_CIRCLE_WIDGET_ENABLED == 1
    LE_WIDGET_CIRCLE,
#endif
#if LE_CIRCULARGAUGE_WIDGET_ENABLED == 1
    LE_WIDGET_CIRCULAR_GAUGE,
#endif
#if LE_CIRCULARSLIDER_WIDGET_ENABLED == 1
    LE_WIDGET_CIRCULAR_SLIDER,
#endif
#if LE_DRAWSURFACE_WIDGET_ENABLED == 1
    LE_WIDGET_DRAWSURFACE,
#endif
#if LE_IMAGE_WIDGET_ENABLED == 1
    LE_WIDGET_IMAGE,
#endif
#if LE_IMAGEROTATE_WIDGET_ENABLED == 1
    LE_WIDGET_IMAGEROTATE,
#endif
#if LE_IMAGESCALE_WIDGET_ENABLED == 1
    LE_WIDGET_IMAGESCALE,
#endif
#if LE_IMAGESEQUENCE_WIDGET_ENABLED == 1
    LE_WIDGET_IMAGESEQUENCE,
#endif
#if LE_GRADIENT_WIDGET_ENABLED == 1
    LE_WIDGET_GRADIENT,
#endif
#if LE_GROUPBOX_WIDGET_ENABLED == 1
    LE_WIDGET_GROUPBOX,
#endif
#if LE_KEYPAD_WIDGET_ENABLED  == 1 && LE_BUTTON_WIDGET_ENABLED == 1
    LE_WIDGET_KEYPAD,
#endif
#if LE_LABEL_WIDGET_ENABLED == 1
    LE_WIDGET_LABEL,
#endif
#if LE_LINE_WIDGET_ENABLED == 1
    LE_WIDGET_LINE,
#endif
#if LE_LINEGRAPH_WIDGET_ENABLED == 1
    LE_WIDGET_LINE_GRAPH,
#endif
#if LE_LIST_WIDGET_ENABLED == 1 && LE_SCROLLBAR_WIDGET_ENABLED == 1
    LE_WIDGET_LIST,
#endif
#if LE_LISTWHEEL_WIDGET_ENABLED == 1
    LE_WIDGET_LISTWHEEL,
#endif
#if LE_PIECHART_WIDGET_ENABLED == 1
    LE_WIDGET_PIE_CHART,
#endif
#if LE_PROGRESSBAR_WIDGET_ENABLED == 1
    LE_WIDGET_PROGRESSBAR,
#endif
#if LE_RADIALMENU_WIDGET_ENABLED == 1
    LE_WIDGET_RADIAL_MENU,
#endif
#if LE_RADIOBUTTON_WIDGET_ENABLED == 1
    LE_WIDGET_RADIOBUTTON,
#endif
#if LE_RECTANGLE_WIDGET_ENABLED == 1
    LE_WIDGET_RECTANGLE,
#endif
#if LE_SCROLLBAR_WIDGET_ENABLED == 1
    LE_WIDGET_SCROLLBAR,
#endif
#if LE_SLIDER_WIDGET_ENABLED == 1
    LE_WIDGET_SLIDER,
#endif
#if LE_TEXTFIELD_WIDGET_ENABLED == 1
    LE_WIDGET_TEXTFIELD,
#endif
#if LE_TOUCHTEST_WIDGET_ENABLED == 1
    LE_WIDGET_TOUCHTEST,
#endif
#if LE_WINDOW_WIDGET_ENABLED == 1
    LE_WIDGET_WINDOW,
#endif
    // begin custom types, not essential but good for identifying base pointers

    // end custom types
    //LE_WIDGET_LAST = LE_WIDGET_WINDOW, // change this if adding custom
    //LE_WIDGET_TYPE_COUNT = LE_WIDGET_LAST + 1
} leWidgetType;

/*
Enumeration:
        leBackgroundType

    Summary:
        Specifies the different background types used for the widgets in the library

    Description:
        Specifies the different background types used for the widgets in the library

        None - No background fill.  Widget must defer to its parent to erase dirty
               pixels.  This may cause additional overhead as clean pixels may be
               repainted as well.

        Fill - a scheme color is used to fill the widget rectangle.

        Cache - a local framebuffer cache is maintained by the widget and used to
                clean up dirty pixels.  Will not cause a parent repaint event but
                will use additional memory to contain the cache.

    Remarks:
        None.
 */
/**
 * @brief Used to define widget background types.
 * @details Specifies the different background types used for the widgets in the library.
 * None - No background fill.  Widget must defer to its parent to erase dirty
 * pixels.  This may cause additional overhead as clean pixels may be repainted as well.
 * Fill - a scheme color is used to fill the widget rectangle.
 * Cache - a local framebuffer cache is maintained by the widget and used to
 * clean up dirty pixels.  Will not cause a parent repaint event but will use additional
 *  memory to contain the cache.
 */
typedef enum leBackgroundType
{
    LE_WIDGET_BACKGROUND_NONE,      /**< No background fill. */
    LE_WIDGET_BACKGROUND_FILL,      /**< Uses scheme color. */
    LE_WIDGET_BACKGROUND_LAST = LE_WIDGET_BACKGROUND_FILL /**< Uses Cache. */
} leBackgroundType;

/*
Enumeration:
        leBorderType

    Summary:
        Specifies the different border types used for the widgets in the library

    Description:
        Specifies the different border types used for the widgets in the library

    Remarks:
        None.
 */
/**
 * @brief Used to define widget border types.
 * @details Specifies the different background types used for the widgets in the library.
 */
typedef enum leBorderType
{
    LE_WIDGET_BORDER_NONE,
    LE_WIDGET_BORDER_LINE,
    LE_WIDGET_BORDER_BEVEL,
    LE_WIDGET_BORDER_LAST = LE_WIDGET_BORDER_BEVEL
} leBorderType;

/*
Enumeration:
        leWidgetDirtyState

    Summary:
        Specifies the different dirty states the widget can be assigned

    Description:
        Specifies the different dirty states the widget can be assigned
        This decides whether the particular widget would be re-drawn or not.
        Dirty widget are re-drawn and clean are not painted over.

    Remarks:
        None.
 */
/**
 * @brief Used to define widget dirty state.
 * @details Specifies the different dirty states the widget can be assigned.
 * This decides whether the particular widget would be re-drawn or not. Dirty widget
 * are re-drawn and clean are not painted over.
 */
typedef enum leWidgetDirtyState
{
    LE_WIDGET_DIRTY_STATE_CLEAN,
    LE_WIDGET_DIRTY_STATE_CHILD,
    LE_WIDGET_DIRTY_STATE_DIRTY,
} leWidgetDirtyState;

/*
Enumeration:
        leWidgetDrawState

    Summary:
        Specifies the different draw states the widget can be assigned

    Description:
        Specifies the different draw states the widget can be assigned

    Remarks:
        None.
 */
/**
 * @brief Used to define widget dirty state.
 * @details Specifies the different dirty states the widget can be assigned.
 * This decides whether the particular widget would be re-drawn or not. Dirty widget
 * are re-drawn and clean are not painted over.
 */
typedef enum leWidgetDrawState
{
    LE_WIDGET_DRAW_STATE_READY,
    LE_WIDGET_DRAW_STATE_DONE,

    // widget-specific draw states start here
} leWidgetDrawState;

/**
 * @brief Used to define widget move event.
 * @details Specifies the old a new position of a widget..
 */
typedef struct leWidget_MoveEvent
{
    leEvent evt;
    
    int32_t oldX;
    int32_t oldY;
    int32_t newX;
    int32_t newY;
} leWidget_MoveEvent;

/**
 * @brief Used to define widget resize event.
 * @details Specifies the old a new size of a widget.
 */
typedef struct leWidget_ResizeEvent
{
    leEvent evt;
    
    uint32_t oldWidth;
    uint32_t oldHeight;
    uint32_t newWidth;
    uint32_t newHeight;
} leWidget_ResizeEvent;

typedef struct leWidget leWidget;

// *****************************************************************************
/* Structure:
    leWidgetEvent

  Summary:
    Basic widget event definition
*/
/**
 * @brief Used to define widget event.
 * @details
 */
typedef struct leWidgetEvent
{
    leEventID id;
    leBool accepted;
    leWidget* owner;
} leWidgetEvent;


// *****************************************************************************
/* Structure:
    leInput_TouchDownEvent_t

  Summary:
    Register and handle the touch press detect event

  Description:
    Register and handle the touch press detect event

  Remarks:
    None.
*/
/**
 * @brief Used to define widget touch down event.
 * @details Specifies event, id and location.
 */
typedef struct leWidgetEvent_TouchDown
{
    leWidgetEvent event;

    int32_t touchID;
    int32_t x;
    int32_t y;
} leWidgetEvent_TouchDown;

// *****************************************************************************
/* Structure:
    leInput_TouchUpEvent

  Summary:
    Register and handle the touch release detect event

  Description:
    Register and handle the touch release detect event

  Remarks:
    None.
*/
/**
 * @brief Used to define widget touch up event.
 * @details Specifies event, id and location.
 */
typedef struct leWidgetEvent_TouchUp
{
    leWidgetEvent event;

    int32_t touchID;
    int32_t x;
    int32_t y;
} leWidgetEvent_TouchUp;

// *****************************************************************************
/* Structure:
    leInput_TouchMoveEvent

  Summary:
    Register and handle the touch coordinates changed event

  Description:
    Register and handle the touch coordinates changed event

  Remarks:
    None.
*/
/**
 * @brief Used to define widget touch move event.
 * @details Specifies event, id and location.
 */
typedef struct leWidgetEvent_TouchMove
{
    leWidgetEvent event;

    uint32_t touchID;
    int32_t prevX;
    int32_t prevY;
    int32_t x;
    int32_t y;
} leWidgetEvent_TouchMove;

void leWidgetEvent_Accept(leWidgetEvent* evt, leWidget* owner);

// *****************************************************************************
/* Struct Definition:
    leWidgetEventFilter

  Summary:
    Struct that defines an event filter.  Event filters allow a
    receiver to discard undesirable events
*/
/**
 * @brief Used to define widget touch event filter.
 * @details Event filters allow a receiver to discard undesirable events.
 */
typedef struct
{
    /**
     * @brief Virtual function - Filter Event.
     * @details Sets an event filter with the value <span style="color: #820a32"><em>evt</em></span> and
     * data arguments <span style="color: #820a32"><em>data</em></span> for the specified widget
     * <span style="color: #820a32"><em>target</em></span>.
     * @code
     * leResult res = leImage_Create();
     * @endcode
     * @param param1 void.
     * @return number of events.
     */
    leBool (*filterEvent)(leWidget* target, leWidgetEvent* evt, void* data);
    void* data;
} leWidgetEventFilter;

/* internal use only */
/**
  * @cond INTERNAL
  *
  */
typedef struct leWidget leWidget;

#define LE_WIDGET_VTABLE(THIS_TYPE) \
    leWidgetType       (*getType)(const THIS_TYPE* _this); \
    int32_t            (*getX)(const THIS_TYPE* _this); \
    leResult           (*setX)(THIS_TYPE* _this, int32_t x); \
    int32_t            (*getY)(const THIS_TYPE* _this); \
    leResult           (*setY)(THIS_TYPE* _this, int32_t y); \
    leResult           (*setPosition)(THIS_TYPE* _this, int32_t x, int32_t y); \
    leResult           (*translate)(THIS_TYPE* _this, int32_t x, int32_t y); \
    uint32_t           (*getWidth)(const THIS_TYPE* _this); \
    leResult           (*setWidth)(THIS_TYPE* _this, uint32_t w); \
    uint32_t           (*getHeight)(const THIS_TYPE* _this); \
    leResult           (*setHeight)(THIS_TYPE* _this, uint32_t h); \
    leResult           (*setSize)(THIS_TYPE* _this, uint32_t w, uint32_t h); \
    leResult           (*resize)(THIS_TYPE* _this, int32_t w, int32_t h); \
    leBool             (*getAlphaEnabled)(const THIS_TYPE* _this); \
    leBool             (*getCumulativeAlphaEnabled)(const THIS_TYPE* _this); \
    leResult           (*setAlphaEnabled)(THIS_TYPE* _this, leBool enbl); \
    uint32_t           (*getAlphaAmount)(const THIS_TYPE* _this); \
    uint32_t           (*getCumulativeAlphaAmount)(const THIS_TYPE* _this); \
    leResult           (*setAlphaAmount)(THIS_TYPE* _this, uint32_t a); \
    leBool             (*isOpaque)(const THIS_TYPE* _this); \
    leBool             (*getEnabled)(const THIS_TYPE* _this); \
    leResult           (*setEnabled)(THIS_TYPE* _this, leBool enbl); \
    leBool             (*getVisible)(const THIS_TYPE* _this); \
    leResult           (*setVisible)(THIS_TYPE* _this, leBool vis); \
    void               (*localRect)(const THIS_TYPE* _this, leRect* res); \
    void               (*rectToParent)(const THIS_TYPE* _this, leRect* res); \
    void               (*rectToScreen)(const THIS_TYPE* _this, leRect* res); \
    leResult           (*addChild)(THIS_TYPE* _this, leWidget* chld); \
    leResult           (*insertChild)(THIS_TYPE* _this, leWidget* chld, uint32_t idx); \
    leResult           (*removeChild)(THIS_TYPE* _this, leWidget* chld); \
    leResult           (*removeChildAt)(THIS_TYPE* _this, uint32_t idx); \
    void               (*removeAllChildren)(THIS_TYPE* _this); \
    leWidget*          (*getRootWidget)(const THIS_TYPE* _this); \
    leResult           (*setParent)(THIS_TYPE* _this, leWidget* pnt); \
    uint32_t           (*getChildCount)(const THIS_TYPE* _this); \
    leWidget*          (*getChildAtIndex)(const THIS_TYPE* _this, uint32_t idx); \
    uint32_t           (*getIndexOfChild)(const THIS_TYPE* _this, const leWidget* chld); \
    leBool             (*containsDescendant)(const THIS_TYPE* _this, const leWidget* wgt); \
    leScheme*          (*getScheme)(const THIS_TYPE* _this); \
    leResult           (*setScheme)(THIS_TYPE* _this, const leScheme* schm); \
    leBorderType       (*getBorderType)(const THIS_TYPE* _this); \
    leResult           (*setBorderType)(THIS_TYPE* _this, leBorderType type); \
    leBackgroundType   (*getBackgroundType)(const THIS_TYPE* _this); \
    leResult           (*setBackgroundType)(THIS_TYPE* _this, leBackgroundType type); \
    leHAlignment       (*getHAlignment)(const THIS_TYPE* _this); \
    leResult           (*setHAlignment)(THIS_TYPE* _this, leHAlignment halgn); \
    leVAlignment       (*getVAlignment)(const THIS_TYPE* _this); \
    leResult           (*setVAlignment)(THIS_TYPE* _this, leVAlignment valgn); \
    leResult           (*getMargins)(const THIS_TYPE* _this, leMargin* mg); \
    leResult           (*setMargins)(THIS_TYPE* _this, uint32_t l, uint32_t t, uint32_t r, uint32_t b); \
    uint32_t           (*getCornerRadius)(const THIS_TYPE* _this); \
    leResult           (*setCornerRadius)(THIS_TYPE* _this, uint32_t rad); \
    leBool             (*hasFocus)(const THIS_TYPE* _this); \
    leResult           (*setFocus)(THIS_TYPE* _this); \
    /* internal functions follow */ \
    void               (*invalidate)(const THIS_TYPE* _this); \
    void               (*invalidateContents)(const THIS_TYPE* _this); \
    leResult           (*installEventFilter)(THIS_TYPE* _this, leWidgetEventFilter fltr); \
    leResult           (*removeEventFilter)(THIS_TYPE* _this, leWidgetEventFilter fltr); \
    void               (*update)(THIS_TYPE* _this, uint32_t dt); \
    \
    void               (*moveEvent)(THIS_TYPE* _this, leWidget_MoveEvent* evt); \
    void               (*resizeEvent)(THIS_TYPE* _this, leWidget_ResizeEvent* evt); \
    void               (*focusGainedEvent)(THIS_TYPE* _this); \
    void               (*focusLostEvent)(THIS_TYPE* _this); \
    void               (*languageChangeEvent)(THIS_TYPE* _this); \
    void               (*touchDownEvent)(THIS_TYPE* _this, leWidgetEvent_TouchDown* evt); \
    void               (*touchUpEvent)(THIS_TYPE* _this, leWidgetEvent_TouchUp* evt); \
    void               (*touchMoveEvent)(THIS_TYPE* _this, leWidgetEvent_TouchMove* evt); \
    \
    void               (*_destructor)(THIS_TYPE* _this); \
    \
    void               (*_handleEvent)(THIS_TYPE* _this, leEvent* evt); \
    void               (*_validateChildren)(THIS_TYPE* _this); \
    void               (*_increaseDirtyState)(THIS_TYPE* _this, uint32_t state); \
    void               (*_setDirtyState)(THIS_TYPE* _this, uint32_t state); \
    void               (*_clearDirtyState)(THIS_TYPE* _this); \
    void               (*_invalidateBorderAreas)(const THIS_TYPE* _this); \
    void               (*_damageArea)(const THIS_TYPE* _this, leRect* rect); \
    void               (*_paint)(THIS_TYPE* _this); \
    
typedef struct leWidgetVTable
{
	LE_WIDGET_VTABLE(leWidget)
} leWidgetVTable;


typedef struct leWidget leWidget;
typedef struct leRectArray leRectArray;

typedef void (*leWidget_DrawFunction_FnPtr)(void*);

enum leWidgetFlags
{
    LE_WIDGET_ENABLED      = 0x1,  // indicates that the widget is enabled
    LE_WIDGET_VISIBLE      = 0x2,  // indicates that the widget is visible
    LE_WIDGET_ALPHAENABLED = 0x4,  // indicates that the widget is using alpha blending
    LE_WIDGET_ISROOT       = 0x8,  // indicates that this widget is a root widget
    LE_WIDGET_IGNOREEVENTS = 0x10, // indicates that the widget should ignore input/focus events
    LE_WIDGET_IGNOREPICK   = 0x20  // indicates that the widget should be ignored for pick tests
};

typedef struct leWidgetStyle
{
    uint8_t backgroundType;  // the widget background type
    uint8_t borderType;      // the widget border type
    uint8_t halign;          // horizontal alignment of the widget
    uint8_t valign;          // vertical alignment of the widget
    uint8_t alphaAmount;     // the global alpha amount to apply to this widget (cumulative with parent widgets)
    uint8_t cornerRadius;    //corner radius, draws round corners if > 0
} leWidgetStyle;

typedef struct leWidgetStatus
{
    uint8_t dirtyState;
    uint8_t drawState;

} leWidgetStatus;

/**
  * @endcond
  *
  */


/* Structure:
        leWidget

    Summary:
        Specifies Graphics widget structure to manage all properties and events associated with the widget

    Description:
        Specifies Graphics widget structure to manage all properties and events associated with the widget.
        It also contains information about the parent and children for the widget to manage the tree structure that the library supports.


    Remarks:
        None.

*/
/**
 * @brief Used to define a widget.
 * @details .
 */
typedef struct leWidget
{
    const leWidgetVTable* fn;
    
    uint32_t id;  // the id of the widget
    leWidgetType type; // the type of the widget

    leRect rect; // the bounding rectangle of the widget

    uint32_t flags;        // widget state flags
    leWidgetStyle style;   // widget style values
    leWidgetStatus status; // widget status values

    leMargin margin; // the margin settings for the widget

    uint32_t drawCount; // number of times this widget has been drawn
                        // for the active screen

    leWidget_DrawFunction_FnPtr drawFunc; // the next draw function to call

    const leScheme* scheme; // the widget's color scheme

    leWidgetEventFilter eventFilters[LE_WIDGET_MAX_EVENT_FILTERS];

    leWidget* parent; // pointer to the widget's parent
    leArray children;  // pointers for the widget's children
} leWidget;

// *****************************************************************************
// *****************************************************************************
// Section: Routines
// *****************************************************************************
// *****************************************************************************

/*    Function:
        leWidget* leWidget_New()

    Summary:
        Create a new widget.


    Description:
        Create a new widget, allocate memory for the widget through the current
        active context.  Returns a widget object pointer.  Application is
        responsible for managing the widget pointer until the widget is added
        to a widget tree.

    Parameters:


    Returns:
        lawidget*

*/
/**
 * @brief Create widget.
 * @details Creates a new widget and allocates memory for the widget through the
 * current active context.  Application is responsible for managing the widget
 * pointer until the widget is added to a widget tree.
 * @code
 * leWidget* wgt = leWidget_New();
 * @endcode
 * @return a widget object pointer.
 */
leWidget* leWidget_New(void);

/**
 * @brief Initialize widget.
 * @details Initializes <span class="param">wgt</span>.
 * @code
 * leWidget* wgt;
 * leWidget_Constructor(wgt);
 * @endcode
 * @param wgt is the widget to initialize
 * @return void.
 */
void leWidget_Constructor(leWidget* wgt);

/*    Function:
        void leWidget_Delete(leWidget* wgt)

    Summary:
        Delete the widget object specified

    Description:
        Delete a widget object specified, de-allocate memory for the widget
        through the current active context.  All child widgets are also
        destructed and freed.

    Parameters:
        leWidget* wgt - the widget to be freed

    Returns:
        void

    Remarks:
        All widgets that are dynamically allocated using should be freed using
        this function regardless of widget type.  This ensures tha all widget
        destructors are properly called.

*/
/**
 * @brief Delete widget.
 * @details Deletes <span style="color: #820a32"><em>wgt</em></span>.
 * @code
 * leWidget* wgt;
 * leWidget_Delete(wgt);
 * @endcode
 * @param param1 wgt is the widget to delete
 * @return void.
 */
void leWidget_Delete(leWidget* wgt);

// *****************************************************************************
/* Virtual Member Function:
    leWidgetType getType(const leWidgetWidget* _this)

  Summary:
     Gets the widget type

  Description:
     Gets the widget type

  Parameters:
    const leWidgetWidget* _this - The widget to operate on

  Remarks:
    Usage - _this->fn->getType(_this);

  Returns:
    leWidgetType - the type
*/
/**
 * @brief Get widget type.
 * @details Returns the widget type from  <span style="color: #820a32"><em>wgt</em></span>.
 * @remark This is a Virtual Member Function
 * @code
 * leWidget* wgt;
 * leWidgetType type = wgt->fn->getType(wgt);
 * @endcode
 * @param param1 wgt is the widget to query
 * @return return leWidgetType.
 */
leWidgetType _leWidget_GetType(const leWidget* _this);

// *****************************************************************************
/* Virtual Member Function:
    int32_t getX(const leWidgetWidget* _this)

  Summary:
     Gets the widget X position

  Description:
     Gets the widget X position

  Parameters:
    const leWidgetWidget* _this - The widget to operate on

  Remarks:
    Usage - _this->fn->getX(_this);

  Returns:
    int32_t - the x value
*/
/**
 * @brief Get widget x position.
 * @details Returns the widget x position from  <span style="color: #820a32"><em>wgt</em></span>.
 * @remark This is a Virtual Member Function
 * @code
 * leWidget* wgt;
 * leWidgetType type = wgt->fn->getX(wgt);
 * @endcode
 * @param param1 wgt is the widget to query
 * @return returns leWidgetType.
 */
int32_t _leWidget_GetX(const leWidget* _this);

// *****************************************************************************
/* Virtual Member Function:
    leResult setX(leWidgetWidget* _this,
                  int32_t x)

  Summary:
     Sets the widget X position

  Description:
     Sets the widget X position

  Parameters:
    leWidgetWidget* _this - The widget to operate on
    int32_t x - the X value

  Remarks:
    Usage - _this->fn->setX(_this, x);

  Returns:
    leResult - the result of the operation
*/
/**
 * @brief Set widget x position.
 * @details Sets the widget x position of <span style="color: #820a32"><em>wgt</em></span> to
 * <span style="color: #820a32"><em>x</em></span>.
 * @remark This is a Virtual Member Function
 * @code
 * leWidget* wgt;
 * leResult res = wgt->fn->setX(wgt);
 * @endcode
 * @param param1 wgt is the widget to modify
 * @param param1 x is the new x position
 * @return returns leResult.
 */
leResult _leWidget_SetX(leWidget* _this,
                        int32_t x);

// *****************************************************************************
/* Virtual Member Function:
    int32_t getY(const leWidgetWidget* _this)

  Summary:
     Gets the widget Y position

  Description:
     Gets the widget Y position

  Parameters:
    const leWidgetWidget* _this - The widget to operate on

  Remarks:
    Usage - _this->fn->getY(_this);

  Returns:
    int32_t - the y value
*/
/**
 * @brief Get widget y position
 * @details Returns the y position from  <span style="color: #820a32"><em>wgt</em></span>.
 * @remark This is a Virtual Member Function
 * @code
 * leWidget* wgt;
 * leWidgetType type = wgt->fn->getY(wgt);
 * @endcode
 * @param param1 wgt is the widget to query
 * @return returns leWidgetType.
 */
int32_t _leWidget_GetY(const leWidget* _this);

// *****************************************************************************
/* Virtual Member Function:
    leResult setY(leWidgetWidget* _this,
                  int32_t y)

  Summary:
     Sets the widget Y position

  Description:
     Sets the widget Y position

  Parameters:
    leWidgetWidget* _this - The widget to operate on
    int32_t y - the Y value

  Remarks:
    Usage - _this->fn->setY(_this, y);

  Returns:
    leResult - the result of the operation
*/
/**
 * @brief Set widget y position.
 * @details Sets the y position of <span style="color: #820a32"><em>wgt</em></span> to
 * <span style="color: #820a32"><em>y</em></span>.
 * @remark This is a Virtual Member Function
 * @code
 * leWidget* wgt;
 * leResult res = wgt->fn->setY(wgt);
 * @endcode
 * @param param1 wgt is the widget to modify
 * @param param1 y is the new y position
 * @return returns leResult.
 */
leResult _leWidget_SetY(leWidget* _this,
                        int32_t y);

// *****************************************************************************
/* Virtual Member Function:
    leResult setPosition(leWidgetWidget* _this,
                         int32_t x,
                         int32_t y)

  Summary:
     Sets the widget position

  Description:
     Sets the widget position

  Parameters:
    leWidgetWidget* _this - The widget to operate on
    int32_t x - the X value
    int32_t y - the Y value

  Remarks:
    Usage - _this->fn->setPosition(_this, x, y);

  Returns:
    leResult - the result of the operation
*/
/**
 * @brief Set widget x and y position.
 * @details Sets the x and y position of <span style="color: #820a32"><em>wgt</em></span> to
 * <span style="color: #820a32"><em>x</em></span> and <span style="color: #820a32"><em>y</em></span>.
 * @remark This is a Virtual Member Function
 * @code
 * leWidget* wgt;
 * leResult res = wgt->fn->setPosition(wgt, x, y);
 * @endcode
 * @param param1 wgt is the widget to modify
 * @param param1 x is the new x position
 * @param param1 y is the new y position
 * @return returns leResult.
 */
leResult _leWidget_SetPosition(leWidget* _this,
                               int32_t x,
                               int32_t y);

// *****************************************************************************
/* Virtual Member Function:
    leResult translate(leWidgetWidget* _this,
                       int32_t x,
                       int32_t y)

  Summary:
     Translate the widget position

  Description:
     Translate the widget position

  Parameters:
    leWidgetWidget* _this - The widget to operate on
    int32_t x - the X value
    int32_t y - the Y value

  Remarks:
    Usage - _this->fn->translate(_this, x, y);

  Returns:
    leResult - the result of the operation
*/
/**
 * @brief Translate widget x and y position.
 * @details Translates the x and y position of <span style="color: #820a32"><em>wgt</em></span> to
 * <span style="color: #820a32"><em>x</em></span> and <span style="color: #820a32"><em>y</em></span>.
 * @remark This is a Virtual Member Function
 * @code
 * leWidget* wgt;
 * leResult res = wgt->fn->translate(wgt, x, y);
 * @endcode
 * @param param1 wgt is the widget to modify
 * @param param1 x is the new x position
 * @param param1 y is the new y position
 * @return returns leResult.
 */
leResult _leWidget_Translate(leWidget* _this,
                             int32_t x,
                             int32_t y);

// *****************************************************************************
/* Virtual Member Function:
    uint32_t getWidth(const leWidgetWidget* _this)

  Summary:
     Gets the widget width

  Description:
     Gets the widget width

  Parameters:
    const leWidgetWidget* _this - The widget to operate on

  Remarks:
    Usage - _this->fn->getWidth(_this);

  Returns:
    uint32_t - the width value
*/
/**
 * @brief Get widget width.
 * @details Gets the width of <span style="color: #820a32"><em>wgt</em></span>.
 * @remark This is a Virtual Member Function
 * @code
 * leWidget* wgt;
 * uint32_t width = wgt->fn->getWidth(wgt);
 * @endcode
 * @param param1 wgt is the widget to modify
 * @return returns leResult.
 */
uint32_t _leWidget_GetWidth(const leWidget* _this);

// *****************************************************************************
/* Virtual Member Function:
    leResult setWidth(leWidgetWidget* _this,
                      uint32_t w)

  Summary:
     Sets the widget width

  Description:
     Sets the widget width

  Parameters:
    leWidgetWidget* _this - The widget to operate on
    uint32_t w - the width value

  Remarks:
    Usage - _this->fn->setWidth(_this, w);

  Returns:
    leResult - the result of the operation
*/
/**
 * @brief Set the widget width.
 * @details Sets the width of <span style="color: #820a32"><em>wgt</em></span> to
 * <span style="color: #820a32"><em>width</em></span>.
 * @remark This is a Virtual Member Function
 * @code
 * leWidget* wgt;
 * uint32_t width;
 * leResult res = wgt->fn->setWidth(wgt, width);
 * @endcode
 * @param param1 wgt is the widget to modify
 * @param param2 width is the new width of widget
 * @return returns leResult.
 */
leResult _leWidget_SetWidth(leWidget* _this,
                            uint32_t width);

// *****************************************************************************
/* Virtual Member Function:
    uint32_t getHeight(const leWidgetWidget* _this)

  Summary:
     Gets the widget height

  Description:
     Gets the widget height

  Parameters:
    const leWidgetWidget* _this - The widget to operate on

  Remarks:
    Usage - _this->fn->getHeight(_this);

  Returns:
    uint32_t - the height value
*/
/**
 * @brief Get widget height.
 * @details Gets the height of <span style="color: #820a32"><em>wgt</em></span>.
 * @remark This is a Virtual Member Function
 * @code
 * leWidget* wgt;
 * uint32_t height = wgt->fn->getHeight(wgt);
 * @endcode
 * @param param1 wgt is the widget to modify
 * @return returns leResult.
 */
uint32_t _leWidget_GetHeight(const leWidget* _this);

// *****************************************************************************
/* Virtual Member Function:
    leResult setHeight(leWidgetWidget* _this,
                       uint32_t h)

  Summary:
     Sets the widget height

  Description:
     Sets the widget height

  Parameters:
    leWidgetWidget* _this - The widget to operate on
    uint32_t h - the height value

  Remarks:
    Usage - _this->fn->setHeight(_this, h);

  Returns:
    leResult - the result of the operation
*/
/**
 * @brief Set widget height.
 * @details Sets the widget height of <span style="color: #820a32"><em>wgt</em></span> to
 * <span style="color: #820a32"><em>width</em></span>.
 * @remark This is a Virtual Member Function
 * @code
 * leWidget* wgt;
 * uint32_t height;
 * leResult res = wgt->fn->getHeight(wgt, height);
 * @endcode
 * @param param1 wgt is the widget to modify
 * @param param2 height is the new height of widget
 * @returns LE_SUCCESS if set, otherwise LE_FAILURE.
 */
leResult _leWidget_SetHeight(leWidget* _this,
                             uint32_t height);

// *****************************************************************************
/* Virtual Member Function:
    leResult setSize(leWidgetWidget* _this,
                     uint32_t w,
                     uint32_t h)

  Summary:
     Sets the widget size

  Description:
     Sets the widget size

  Parameters:
    leWidgetWidget* _this - The widget to operate on
    uint32_t w - the width value
    uint32_t h - the height value

  Remarks:
    Usage - _this->fn->setSize(_this, w, h);

  Returns:
    leResult - the result of the operation
*/
/**
 * @brief Set widget height.
 * @details Sets the widget height of <span style="color: #820a32"><em>wgt</em></span> to
 * <span style="color: #820a32"><em>width</em></span>.
 * @remark This is a Virtual Member Function
 * @code
 * leWidget* wgt;
 * uint32_t height;
 * leResult res = wgt->fn->getHeight(wgt, height);
 * @endcode
 * @param param1 wgt is the widget to modify
 * @param param2 height is the new height of widget
 * @returns LE_SUCCESS if set, otherwise LE_FAILURE.
 */
leResult _leWidget_SetSize(leWidget* _this,
                           uint32_t width,
                           uint32_t height);

// *****************************************************************************
/* Virtual Member Function:
    leResult resize(leWidgetWidget* _this,
                    int32_t w,
                    int32_t h)

  Summary:
     Resizes the widget

  Description:
     Resizes the widget

  Parameters:
    leWidgetWidget* _this - The widget to operate on
    int32_t w - the width value
    int32_t h - the height value

  Remarks:
    Usage - _this->fn->resize(_this, w, h);

  Returns:
    leResult - the result of the operation
*/
/**
 * @brief Resize widget.
 * @details Resizes the size of <span style="color: #820a32"><em>wgt</em></span> to
 * <span style="color: #820a32"><em>width</em></span> and <span style="color: #820a32"><em>height</em></span>.
 * @remark This is a Virtual Member Function
 * @code
 * leWidget* wgt;
 * int32_t width;
 * uint32_t height;
 * leResult res = wgt->fn->resize(wgt, width, height);
 * @endcode
 * @param param1 wgt is the widget to modify
 * @param param2 width is the new width of widget
 * @param param3 height is the new height of widget
 * @returns LE_SUCCESS if set, otherwise LE_FAILURE.
 */
leResult _leWidget_Resize(leWidget* _this,
                          int32_t width,
                          int32_t height);

// *****************************************************************************
/* Virtual Member Function:
    leBool getAlphaEnabled(const leWidgetWidget* _this)

  Summary:
     Gets the widget alpha enabled flag

  Description:
     Gets the widget alpha enabled flag

  Parameters:
    const leWidgetWidget* _this - The widget to operate on

  Remarks:
    Usage - _this->fn->getAlphaEnabled(_this);

  Returns:
    leBool - true if alpha blending is enabled
*/
/**
 * @brief Get alpha enable status.
 * @details Gets the alpha enable status of <span style="color: #820a32"><em>wgt</em></span>.
 * @remark This is a Virtual Member Function
 * @code
 * leWidget* wgt;
 * leBool enabled = wgt->fn->getAlphaEnabled(wgt);
 * @endcode
 * @param param1 wgt is the widget to modify
 * @return returns leBool.
 */
leBool _leWidget_GetAlphaEnabled(const leWidget* _this);

// *****************************************************************************
/* Virtual Member Function:
    leBool getCumulativeAlphaEnabled(const leWidgetWidget* _this)

  Summary:
     Determines if any widget in this widget's hierarchy has its alpha flag
     enabled

  Description:
     Determines if any widget in this widget's hierarchy has its alpha flag
     enabled

  Parameters:
    const leWidgetWidget* _this - The widget to operate on

  Remarks:
    Usage - _this->fn->getCumulativeAlphaEnabled(_this);

  Returns:
    leBool - true if alpha blending is enabled
*/
/**
 * @brief Get cumulative alpha enable status.
 * @details Gets the cumulative alpha enable status of <span style="color: #820a32"><em>wgt</em></span>.
 * @remark This is a Virtual Member Function
 * @code
 * leWidget* wgt;
 * leBool enabled = wgt->fn->getCumulativeAlphaEnabled(wgt);
 * @endcode
 * @param param1 wgt is the widget to modify
 * @return returns leBool.
 */
leBool _leWidget_GetCumulativeAlphaEnabled(const leWidget* _this);

// *****************************************************************************
/* Virtual Member Function:
    leResult setAlphaEnabled(leWidgetWidget* _this,
                             leBool enbl)

  Summary:
     Sets the widget's alpha flag enable state

  Description:
     Sets the widget's alpha flag enable state

  Parameters:
    leWidgetWidget* _this - The widget to operate on
    leBool enbl -

  Remarks:
    Usage - _this->fn->setAlphaEnabled(_this, enbl);

  Returns:
    leResult - the result of the operation
*/
/**
 * @brief Set cumulative alpha enable status.
 * @details Sets the cumulative alpha enable status of <span style="color: #820a32"><em>wgt</em></span>.
 * @remark This is a Virtual Member Function
 * @code
 * leWidget* wgt;
 * leBool enable;
 * leResult res = wgt->fn->setAlphaEnabled(wgt, enable);
 * @endcode
 * @param param1 wgt is the widget to modify
 * @return returns leBool.
 */
leResult _leWidget_SetAlphaEnabled(leWidget* _this,
                                   leBool enable);

// *****************************************************************************
/* Virtual Member Function:
    uint32_t getAlphaAmount(const leWidgetWidget* _this)

  Summary:
     Gets the widget alpha blending value

  Description:
     Gets the widget alpha blending value

  Parameters:
    const leWidgetWidget* _this - The widget to operate on

  Remarks:
    Usage - _this->fn->getAlphaAmount(_this);

  Returns:
    uint32_t - the alpha amount
*/
/**
 * @brief Get alpha amount.
 * @details Gets the alpha amount of <span style="color: #820a32"><em>wgt</em></span>.
 * @remark This is a Virtual Member Function
 * @code
 * leWidget* wgt;
 * uint32_t amount = wgt->fn->getAlphaAmount(wgt);
 * @endcode
 * @param param1 wgt is the widget to query
 * @return returns the alpha amount.
 */
uint32_t _leWidget_GetAlphaAmount(const leWidget* _this);

// *****************************************************************************
/* Virtual Member Function:
    uint32_t getCumulativeAlphaAmount(const leWidgetWidget* _this)

  Summary:
     Gets the cumulative amount of alpha blending applied to this widget

  Description:
     Gets the cumulative amount of alpha blending applied to this widget

  Parameters:
    const leWidgetWidget* _this - The widget to operate on

  Remarks:
    Usage - _this->fn->getCumulativeAlphaAmount(_this);

  Returns:
    uint32_t - the alpha amount
*/
/**
 * @brief Get cumulative alpha amount.
 * @details Gets the cumulative alpha amount of <span style="color: #820a32"><em>wgt</em></span>.
 * @remark This is a Virtual Member Function
 * @code
 * leWidget* wgt;
 * uint32_t amount = wgt->fn->getCumulativeAlphaAmount(wgt);
 * @endcode
 * @param param1 wgt is the widget to query
 * @return returns the alpha amount.
 */
uint32_t _leWidget_GetCumulativeAlphaAmount(const leWidget* _this);

// *****************************************************************************
/* Virtual Member Function:
    leResult setAlphaAmount(leWidgetWidget* _this,
                            uint32_t a)

  Summary:
     Sets this widget's alpha amount

  Description:
     Sets this widget's alpha amount

  Parameters:
    leWidgetWidget* _this - The widget to operate on
    uint32_t a - the alpha amount

  Remarks:
    Usage - _this->fn->setAlphaAmount(_this, a);

  Returns:
    leResult - the result of the operation
*/
/**
 * @brief Set alpha amount.
 * @details Sets the alpha amount of <span style="color: #820a32"><em>wgt</em></span>.
 * @remark This is a Virtual Member Function
 * @code
 * leWidget* wgt;
 * uint32_t alpha;
 * leResult res = wgt->fn->setAlphaAmount(wgt, alpha);
 * @endcode
 * @param param1 wgt is the widget to query
 * @returns LE_SUCCESS if set, otherwise LE_FAILURE.
 */
leResult _leWidget_SetAlphaAmount(leWidget* _this,
                                  uint32_t alpha);

// *****************************************************************************
/* Virtual Member Function:
    leBool isOpaque(const leWidgetWidget* _this)

  Summary:
     Determines if this widget is opaque

  Description:
     Determines if this widget is opaque

  Parameters:
    const leWidgetWidget* _this - The widget to operate on

  Remarks:
    Usage - _this->fn->isOpaque(_this);

  Returns:
    leBool - true if opaque
*/
/**
 * @brief Determine is widget is opaque.
 * @details Returns the opaque status of <span style="color: #820a32"><em>wgt</em></span>.
 * @remark This is a Virtual Member Function
 * @code
 * leWidget* wgt;
 * leBool opaque = wgt->fn->isOpaque(wgt);
 * @endcode
 * @param param1 wgt is the widget to query
 * @return returns true is widget is opaque, otherwise false.
 */
leBool _leWidget_IsOpaque(const leWidget* _this);

// *****************************************************************************
/* Virtual Member Function:
    leBool getEnabled(const leWidgetWidget* _this)

  Summary:
     Gets this widget's enabled flag

  Description:
     Gets this widget's enabled flag

  Parameters:
    const leWidgetWidget* _this - The widget to operate on

  Remarks:
    Usage - _this->fn->getEnabled(_this);

  Returns:
    leBool - the setting value
*/
/**
 * @brief Get widget enabled flag.
 * @details Gets the enabled status of <span style="color: #820a32"><em>wgt</em></span>.
 * @remark This is a Virtual Member Function
 * @code
 * leWidget* wgt;
 * leBool enabled = wgt->fn->getEnabled(wgt);
 * @endcode
 * @param param1 wgt is the widget to query
 * @return returns true if widget is enabled, otherwise false.
 */
leBool _leWidget_GetEnabled(const leWidget* _this);

// *****************************************************************************
/* Virtual Member Function:
    leResult setEnabled(leWidgetWidget* _this,
                        leBool enbl)

  Summary:
     Sets this widget's enabled flag

  Description:
     Sets this widget's enabled flag

  Parameters:
    leWidgetWidget* _this - The widget to operate on
    leBool enbl - the setting value

  Remarks:
    Usage - _this->fn->setEnabled(_this, enbl);

  Returns:
    leResult - the result of the operation
*/
/**
 * @brief Set widget enabled flag.
 * @details Sets the enabled status of <span style="color: #820a32"><em>wgt</em></span> to
 * the value of <span style="color: #820a32"><em>enable</em></span>.
 * @remark This is a Virtual Member Function
 * @code
 * leWidget* wgt;
 * leBool enable;
 * leResult res = wgt->fn->setEnabled(wgt, enable);
 * @endcode
 * @param param1 wgt is the widget to query
 * @return returns true if widget is enabled, otherwise false.
 */
leResult _leWidget_SetEnabled(leWidget* _this, leBool enable);

// *****************************************************************************
/* Virtual Member Function:
    leBool getVisible(const leWidgetWidget* _this)

  Summary:
     Gets this widget's visible flag

  Description:
     Gets this widget's visible flag

  Parameters:
    const leWidgetWidget* _this - The widget to operate on

  Remarks:
    Usage - _this->fn->getVisible(_this);

  Returns:
    leBool - the visibility setting
*/
/**
 * @brief Get widget visible status.
 * @details Gets the visible status of <span style="color: #820a32"><em>wgt</em></span>.
 * @remark This is a Virtual Member Function
 * @code
 * leWidget* wgt;
 * leBool visible = wgt->fn->getVisible(wgt);
 * @endcode
 * @param param1 wgt is the widget to query
 * @return returns true if widget is enabled, otherwise false.
 */
leBool _leWidget_GetVisible(const leWidget* _this);

// *****************************************************************************
/* Virtual Member Function:
    leResult setVisible(leWidgetWidget* _this,
                        leBool vis)

  Summary:
     Sets this widget's visible flag

  Description:
     Sets this widget's visible flag

  Parameters:
    leWidgetWidget* _this - The widget to operate on
    leBool vis - the visibility setting

  Remarks:
    Usage - _this->fn->setVisible(_this, vis);

  Returns:
    leResult - the result of the operation
*/
/**
 * @brief Set widget visible status.
 * @details Sets the visible status of <span style="color: #820a32"><em>wgt</em></span> to
 * the value of <span style="color: #820a32"><em>visible</em></span>.
 * @remark This is a Virtual Member Function
 * @code
 * leWidget* wgt;
 * leBool enable;
 * leResult res = wgt->fn->setVisible(wgt, visible);
 * @endcode
 * @param param1 wgt is the widget to query
 * @param param1 visible is the new flag status
 * @returns LE_SUCCESS if set, otherwise LE_FAILURE.
 */
leResult _leWidget_SetVisible(leWidget* _this, leBool visible);

// *****************************************************************************
/* Virtual Member Function:
    leRect localRect(const leWidgetWidget* _this)

  Summary:
     Gets the widget's bounding rectangle in local space

  Description:
     Gets the widget's bounding rectangle in local space

  Parameters:
    const leWidgetWidget* _this - The widget to operate on

  Remarks:
    Usage - _this->fn->localRect(_this);

  Returns:
    leRect - the bounding rectangle
*/
/**
 * @brief Get widget rectangle.
 * @details Gets the bounding rectangle of <span style="color: #820a32"><em>wgt</em></span>.
 * @remark This is a Virtual Member Function
 * @code
 * leWidget* wgt;
 * leRect rec = wgt->fn->localRect(wgt);
 * @endcode
 * @param wgt the widget to query
 * @param res the resultant rectangle
 * @return returns void.
 */
void _leWidget_LocalRect(const leWidget* _this,
                         leRect* res);

// *****************************************************************************
/* Virtual Member Function:
    leRect rectToParent(const leWidgetWidget* _this)

  Summary:
     Gets the widget's bounding rectangle in parent space

  Description:
     Gets the widget's bounding rectangle in parent space

  Parameters:
    const leWidgetWidget* _this - The widget to operate on

  Remarks:
    Usage - _this->fn->rectToParent(_this);

  Returns:
    leRect - the bounding rectangle
*/
/**
 * @brief Get widget rectangle.
 * @details Gets the bounding rectangle of <span style="color: #820a32"><em>wgt</em></span>
 * relative to parent.
 * @remark This is a Virtual Member Function
 * @code
 * leWidget* wgt;
 * leRect rec = wgt->fn->rectToParent(wgt);
 * @endcode
 * @param wgt the widget to query
 * @param res the resultant rectangle
 * @return returns void.
 */
void _leWidget_RectToParentSpace(const leWidget* _this,
                                 leRect* res);

// *****************************************************************************
/* Virtual Member Function:
    void rectToScreen(const leWidgetWidget* _this, leRect* res)

  Summary:
     Gets the widget's bounding rectangle in screen space

  Description:
     Gets the widget's bounding rectangle in screen space

  Parameters:
    const leWidgetWidget* _this - The widget to operate on
    leRect* res - the resultant rectangle

  Remarks:
    Usage - _this->fn->rectToScreen(_this);

  Returns:
    void
*/
/**
 * @brief Get widget rectangle.
 * @details Gets the bounding rectangle of <span style="color: #820a32"><em>wgt</em></span>
 * relative to screen.
 * @remark This is a Virtual Member Function
 * @code
 * leWidget* wgt;
 * leRect rec = wgt->fn->rectToScreen(wgt);
 * @endcode
 * @param wgt the widget to query
 * @param res the resultant rectangle
 * @return returns void.
 */
void _leWidget_RectToScreenSpace(const leWidget* _this,
                                 leRect* res);

// *****************************************************************************
/* Virtual Member Function:
    leResult addChild(leWidgetWidget* _this,
                      leWidget* chld)

  Summary:
     Adds a child widget to this widget

  Description:
     Adds a child widget to this widget

  Parameters:
    leWidgetWidget* _this - The widget to operate on
    leWidget* chld - the child widget

  Remarks:
    Usage - _this->fn->addChild(_this, chld);

  Returns:
    leResult - the result of the operation
*/
/**
 * @brief Add child to widget.
 * @details Adds <span style="color: #820a32"><em>child</em></span> widget
 * to <span style="color: #820a32"><em>wgt</em></span>.
 * @remark This is a Virtual Member Function
 * @code
 * leWidget* wgt;
 * leResult res = wgt->fn->addChild(wgt, child);
 * @endcode
 * @param param1 wgt is the widget to query
 * @param param2 child is the widget to add
 * @returns LE_SUCCESS if set, otherwise LE_FAILURE.
 */
leResult _leWidget_AddChild(leWidget* _this,
                            leWidget* child);

/**
 * @brief Adds child to widget at index.
 * @details Adds <span style="color: #820a32"><em>child</em></span> widget
 * to <span style="color: #820a32"><em>wgt</em></span> at the given index.
 * @remark This is a Virtual Member Function
 * @code
 * leWidget* wgt;
 * leResult res = wgt->fn->addChild(wgt, child, 0);
 * @endcode
 * @param param1 wgt is the widget to modify
 * @param param2 child is the widget to add
 * @param param3 idx is the index to insert at
 * @returns LE_SUCCESS if set, otherwise LE_FAILURE.
 */
leResult _leWidget_InsertChild(leWidget* _this,
                               leWidget* child,
                               uint32_t idx);

// *****************************************************************************
/* Virtual Member Function:
    leResult removeChild(leWidgetWidget* _this,
                         leWidget* chld)

  Summary:
     Removes a child widget from this widget

  Description:
     Removes a child widget from this widget

  Parameters:
    leWidgetWidget* _this - The widget to operate on
    leWidget* chld - the child widget

  Remarks:
    Usage - _this->fn->removeChild(_this, chld);

  Returns:
    leResult - the result of the operation
*/
/**
 * @brief Remove child from widget.
 * @details Remove <span style="color: #820a32"><em>child</em></span> widget
 * from <span style="color: #820a32"><em>wgt</em></span>.
 * @remark This is a Virtual Member Function
 * @code
 * leWidget* wgt;
 * leResult res = wgt->fn->removeChild(wgt, child);
 * @endcode
 * @param param1 wgt is the widget to query
 * @param param2 child is the widget to remove
 * @returns LE_SUCCESS if set, otherwise LE_FAILURE.
 */
leResult _leWidget_RemoveChild(leWidget* _this,
                               leWidget* child);

/**
 * @brief Remove child from widget at index.
 * @details Remove <span style="color: #820a32"><em>child</em></span> widget
 * from <span style="color: #820a32"><em>wgt</em></span>.
 * @remark This is a Virtual Member Function
 * @code
 * leWidget* wgt;
 * leResult res = wgt->fn->removeChild(wgt, idx);
 * @endcode
 * @param param1 wgt is the widget to query
 * @param param2 idx is the index of the child to remove
 * @returns LE_SUCCESS if set, otherwise LE_FAILURE.
 */
leResult _leWidget_RemoveChildAt(leWidget* _this,
                                 uint32_t idx);

// *****************************************************************************
/* Virtual Member Function:
    void removeAllChildren(leWidgetWidget* _this)

  Summary:
     Removes all children from this widget

  Description:
     Removes all children from this widget

  Parameters:
    leWidgetWidget* _this - The widget to operate on

  Remarks:
    Usage - _this->fn->removeAllChildren(_this);

  Returns:
    void
*/
/**
 * @brief Remove all children from widget.
 * @details Removes all children from <span style="color: #820a32"><em>wgt</em></span>.
 * @remark This is a Virtual Member Function
 * @code
 * leWidget* wgt;
 * wgt->fn->removeAllChildren(wgt, child);
 * @endcode
 * @param param1 wgt is the widget to query
 * @return returns void.
 */
void _leWidget_RemoveAllChildren(leWidget* _this);

// *****************************************************************************
/* Virtual Member Function:
    leWidget* getRootWidget(const leWidgetWidget* _this)

  Summary:
     Gets the topmost ancestor for this widget

  Description:
     Gets the topmost ancestor for this widget

  Parameters:
    const leWidgetWidget* _this - The widget to operate on

  Remarks:
    Usage - _this->fn->getRootWidget(_this);

  Returns:
    leWidget* - the root widget
*/
/**
 * @brief Get root widget.
 * @details Gets root widget of <span style="color: #820a32"><em>wgt</em></span>.
 * @remark This is a Virtual Member Function
 * @code
 * leWidget* wgt;
 * leWidget* root = wgt->fn->getRootWidget(wgt);
 * @endcode
 * @param param1 wgt is the widget to query
 * @return returns leWidget.
 */
leWidget* _leWidget_GetRootWidget(const leWidget* _this);

// *****************************************************************************
/* Virtual Member Function:
    leResult setParent(leWidgetWidget* _this,
                       leWidget* pnt)

  Summary:
     Sets this widget's parent

  Description:
     Sets this widget's parent

  Parameters:
    leWidgetWidget* _this - The widget to operate on
    leWidget* pnt - the parent widget

  Remarks:
    Usage - _this->fn->setParent(_this, pnt);

  Returns:
    leResult - the result of the operation
*/
/**
 * @brief Set parent widget.
 * @details Sets the parent widget of <span style="color: #820a32"><em>wgt</em></span>
 * to <span style="color: #820a32"><em>parent</em></span>.
 * @remark This is a Virtual Member Function
 * @code
 * leWidget* wgt;
 * leResult res = wgt->fn->setParent(wgt);
 * @endcode
 * @param param1 wgt is the widget to modify
 * @param param2 parent is the new parent widget
 * @return returns leWidget.
 */
leResult _leWidget_SetParent(leWidget* _this,
                             leWidget* parent);

// *****************************************************************************
/* Virtual Member Function:
    uint32_t getChildCount(const leWidgetWidget* _this)

  Summary:
     Gets the number of children this widget owns

  Description:
     Gets the number of children this widget owns

  Parameters:
    const leWidgetWidget* _this - The widget to operate on

  Remarks:
    Usage - _this->fn->getChildCount(_this);

  Returns:
    uint32_t - the child count
*/
/**
 * @brief Get child count.
 * @details Gets the number of children in <span style="color: #820a32"><em>wgt</em></span>.
 * @remark This is a Virtual Member Function
 * @code
 * leWidget* wgt;
 * uint32_t cnt = wgt->fn->getChildCount(wgt);
 * @endcode
 * @param param1 wgt is the widget to query
 * @return returns leWidget.
 */
uint32_t _leWidget_GetChildCount(const leWidget* _this);

// *****************************************************************************
/* Virtual Member Function:
    leWidget* getChildAtIndex(const leWidgetWidget* _this,
                              uint32_t idx)

  Summary:
     Gets a widget child at a given index

  Description:
     Gets a widget child at a given index

  Parameters:
    const leWidgetWidget* _this - The widget to operate on
    uint32_t idx - the index

  Remarks:
    Usage - _this->fn->getChildAtIndex(_this, idx);

  Returns:
    leWidget* - the child widget
*/
/**
 * @brief Get child at index.
 * @details Returns the child widget at <span style="color: #820a32"><em>idx</em></span>
 * in <span style="color: #820a32"><em>wgt</em></span>.
 * @remark This is a Virtual Member Function
 * @code
 * leWidget* wgt;
 * leWidget * child = wgt->fn->getChildAtIndex(wgt);
 * @endcode
 * @param param1 wgt is the widget to query
 * @return returns leWidget.
 */
leWidget* _leWidget_GetChildAtIndex(const leWidget* _this,
                                    uint32_t idx);

// *****************************************************************************
/* Virtual Member Function:
    uint32_t getIndexOfChild(const leWidgetWidget* _this,
                             const leWidget* chld)

  Summary:
     Get the index of a given child widget

  Description:
     Get the index of a given child widget

  Parameters:
    const leWidgetWidget* _this - The widget to operate on
    const leWidget* chld - the child widget

  Remarks:
    Usage - _this->fn->getIndexOfChild(_this, chld);

  Returns:
    uint32_t -
*/
/**
 * @brief Get index of child.
 * @details Returns the index of <span style="color: #820a32"><em>child</em></span>
 * in <span style="color: #820a32"><em>wgt</em></span>.
 * @remark This is a Virtual Member Function
 * @code
 * leWidget* wgt;
 * leWidget * child = wgt->fn->getIndexOfChild(wgt);
 * @endcode
 * @param param1 wgt is the widget to query
 * @param param2 child is the child to get index
 * @return returns leWidget.
 */
uint32_t _leWidget_GetIndexOfChild(const leWidget* _this,
                                   const leWidget* child);

// *****************************************************************************
/* Virtual Member Function:
    leBool containsDescendant(const leWidgetWidget* _this,
                              const leWidget* wgt)

  Summary:
     Determines of a widget's descendant tree contains a given widget

  Description:
     Determines of a widget's descendant tree contains a given widget

  Parameters:
    const leWidgetWidget* _this - The widget to operate on
    const leWidget* wgt - the widget

  Remarks:
    Usage - _this->fn->containsDescendant(_this, wgt);

  Returns:
    leBool - LE_TRUE if the widget is a descentdent of this
*/
/**
 * @brief Determine widget exists.
 * @details Returns the existence status of <span style="color: #820a32"><em>wgt</em></span>
 * in <span style="color: #820a32"><em>_this</em></span>.
 * @remark This is a Virtual Member Function
 * @code
 * leWidget* wgt;
 * leWidget* descendent;
 * leBool exists = wgt->fn->getIndexOfChild(wgt, descendent);
 * @endcode
 * @param param1 wgt is the widget to query
 * @param param2 child is the child to get index
 * @return returns true if widget is a decendent, otherwise false.
 */
leBool _leWidget_ContainsDescendant(const leWidget* _this,
                                    const leWidget* wgt);

// *****************************************************************************
/* Virtual Member Function:
    leScheme* getScheme(const leWidgetWidget* _this)

  Summary:
     Gets this widget's scheme

  Description:
     Gets this widget's scheme

  Parameters:
    const leWidgetWidget* _this - The widget to operate on

  Remarks:
    Usage - _this->fn->getScheme(_this);

  Returns:
    leScheme* - the scheme pointer
*/
/**
 * @brief Get widget scheme.
 * @details Returns the scheme of <span style="color: #820a32"><em>wgt</em></span>.
 * @remark This is a Virtual Member Function
 * @code
 * leWidget* wgt;
 * leScheme * sch = wgt->fn->getScheme(wgt);
 * @endcode
 * @param param1 wgt is the widget to query
 * @return returns leScheme.
 */
leScheme* _leWidget_GetScheme(const leWidget* _this);

// *****************************************************************************
/* Virtual Member Function:
    leResult setScheme(const leWidgetWidget* _this,
                       leScheme* schm)

  Summary:
     Sets this widget's scheme

  Description:
     Sets this widget's scheme

  Parameters:
    leWidgetWidget* _this - The widget to operate on
    const leScheme* schm - the scheme pointer

  Remarks:
    Usage - _this->fn->setScheme(_this, schm);

  Returns:
    leResult - the result of the operation
*/
/**
 * @brief Set widget scheme.
 * @details Sets the scheme of <span style="color: #820a32"><em>wgt</em></span> to
 * <span style="color: #820a32"><em>scheme</em></span>.
 * @remark This is a Virtual Member Function
 * @code
 * leWidget* wgt;
 * leScheme* scheme
 * leResult res = wgt->fn->setScheme(wgt, scheme);
 * @endcode
 * @param param1 wgt is the widget to modify
 * @param param1 scheme is the scheme to set
 * @returns LE_SUCCESS if set, otherwise LE_FAILURE.
 */
leResult _leWidget_SetScheme(leWidget* _this,
                             const leScheme* scheme);

// *****************************************************************************
/* Virtual Member Function:
    leBorderType getBorderType(const leWidgetWidget* _this)

  Summary:
     Gets this widget's border type

  Description:
     Gets this widget's border type

  Parameters:
    const leWidgetWidget* _this - The widget to operate on

  Remarks:
    Usage - _this->fn->getBorderType(_this);

  Returns:
    leBorderType - the type
*/
/**
 * @brief Get widget bordertype.
 * @details Returns the border type of <span style="color: #820a32"><em>wgt</em></span>.
 * @remark This is a Virtual Member Function
 * @code
 * leWidget* wgt;
 * leBorderType type = wgt->fn->getBorderType(wgt);
 * @endcode
 * @param param1 wgt is the widget to query
 * @return returns leBorderType.
 */
leBorderType _leWidget_GetBorderType(const leWidget* _this);

// *****************************************************************************
/* Virtual Member Function:
    leResult setBorderType(leWidgetWidget* _this,
                           leBorderType type)

  Summary:
     Sets this widget's border type

  Description:
     Sets this widget's border type

  Parameters:
    leWidgetWidget* _this - The widget to operate on
    leBorderType type - the type

  Remarks:
    Usage - _this->fn->setBorderType(_this, type);

  Returns:
    leResult - the result of the operation
*/
/**
 * @brief Set widget scheme.
 * @details Sets the scheme of <span style="color: #820a32"><em>wgt</em></span> to
 * <span style="color: #820a32"><em>scheme</em></span>.
 * @remark This is a Virtual Member Function
 * @code
 * leWidget* wgt;
 * leScheme* scheme
 * leResult res = wgt->fn->setScheme(wgt, scheme);
 * @endcode
 * @param param1 wgt is the widget to modify
 * @param param1 scheme is the scheme to set
 * @returns LE_SUCCESS if set, otherwise LE_FAILURE.
 */
leResult _leWidget_SetBorderType(leWidget* _this,
                                 leBorderType type);

// *****************************************************************************
/* Virtual Member Function:
    leBackgroundType getBackgroundType(const leWidgetWidget* _this)

  Summary:
     Gets this widget's background type

  Description:
     Gets this widget's background type

  Parameters:
    const leWidgetWidget* _this - The widget to operate on

  Remarks:
    Usage - _this->fn->getBackgroundType(_this);

  Returns:
    leBackgroundType - the type
*/
/**
 * @brief Get widget background type.
 * @details Returns the background type of <span style="color: #820a32"><em>wgt</em></span>.
 * @remark This is a Virtual Member Function
 * @code
 * leWidget* wgt;
 * leBackgroundType type = wgt->fn->getBorderType(wgt);
 * @endcode
 * @param param1 wgt is the widget to query
 * @return returns leBackgroundType.
 */
leBackgroundType _leWidget_GetBackgroundType(const leWidget* _this);

// *****************************************************************************
/* Virtual Member Function:
    leResult setBackgroundType(leWidgetWidget* _this,
                               leBackgroundType type)

  Summary:
     Sets this widget's background type

  Description:
     Sets this widget's background type

  Parameters:
    leWidgetWidget* _this - The widget to operate on
    leBackgroundType type - the type

  Remarks:
    Usage - _this->fn->setBackgroundType(_this, type);

  Returns:
    leResult - the result of the operation
*/
/**
 * @brief Set widget scheme.
 * @details Sets the scheme of <span style="color: #820a32"><em>wgt</em></span> to
 * <span style="color: #820a32"><em>scheme</em></span>.
 * @remark This is a Virtual Member Function
 * @code
 * leWidget* wgt;
 * leScheme* scheme
 * leResult res = wgt->fn->setScheme(wgt, scheme);
 * @endcode
 * @param param1 wgt is the widget to modify
 * @param param1 scheme is the scheme to set
 * @returns LE_SUCCESS if set, otherwise LE_FAILURE.
 */
leResult _leWidget_SetBackgroundType(leWidget* _this,
                                     leBackgroundType type);

// *****************************************************************************
/* Virtual Member Function:
    leHAlignment getHAlignment(const leWidgetWidget* _this)

  Summary:
     Gets this widget's horizontal alignment setting

  Description:
     Gets this widget's horizontal alignment setting

  Parameters:
    const leWidgetWidget* _this - The widget to operate on

  Remarks:
    Usage - _this->fn->getHAlignment(_this);

  Returns:
    leHAlignment - the horizontal alignment
*/
/**
 * @brief Get widget horizontal alignment.
 * @details Returns the horizontal alignment of <span style="color: #820a32"><em>wgt</em></span>.
 * @remark This is a Virtual Member Function
 * @code
 * leWidget* wgt;
 * leHAlignment ha = wgt->fn->getHAlignment(wgt);
 * @endcode
 * @param param1 wgt is the widget to query
 * @return returns leHAlignment.
 */
leHAlignment _leWidget_GetHAlignment(const leWidget* _this);

// *****************************************************************************
/* Virtual Member Function:
    leResult setHAlignment(leWidgetWidget* _this,
                           leHAlignment halgn)

  Summary:
     Sets this widget's horizontal alignment setting

  Description:
     Sets this widget's horizontal alignment setting

  Parameters:
    leWidgetWidget* _this - The widget to operate on
    leHAlignment halgn - the horizontal alignment

  Remarks:
    Usage - _this->fn->setHAlignment(_this, halgn);

  Returns:
    leResult - the result of the operation
*/
/**
 * @brief Set widget scheme.
 * @details Sets the scheme of <span style="color: #820a32"><em>wgt</em></span> to
 * <span style="color: #820a32"><em>scheme</em></span>.
 * @remark This is a Virtual Member Function
 * @code
 * leWidget* wgt;
 * leScheme* scheme
 * leResult res = wgt->fn->setScheme(wgt, scheme);
 * @endcode
 * @param param1 wgt is the widget to modify
 * @param param1 scheme is the scheme to set
 * @returns LE_SUCCESS if set, otherwise LE_FAILURE.
 */
leResult _leWidget_SetHAlignment(leWidget* _this,
                                 leHAlignment align);

// *****************************************************************************
/* Virtual Member Function:
    leVAlignment getVAlignment(const leWidgetWidget* _this)

  Summary:
     Gets this widget's vertical alignment setting

  Description:
     Gets this widget's vertical alignment setting

  Parameters:
    const leWidgetWidget* _this - The widget to operate on

  Remarks:
    Usage - _this->fn->getVAlignment(_this);

  Returns:
    leVAlignment - the vertical alignment
*/
/**
 * @brief Get widget vertical alignment.
 * @details Returns the horizontal alignment of <span style="color: #820a32"><em>wgt</em></span>.
 * @remark This is a Virtual Member Function
 * @code
 * leWidget* wgt;
 * leHAlignment ha = wgt->fn->getVAlignment(wgt);
 * @endcode
 * @param param1 wgt is the widget to query
 * @return returns leVAlignment.
 */
leVAlignment _leWidget_GetVAlignment(const leWidget* _this);

// *****************************************************************************
/* Virtual Member Function:
    leResult setVAlignment(leWidgetWidget* _this,
                           leVAlignment valgn)

  Summary:
     Sets this widget's vertical alignment setting

  Description:
     Sets this widget's vertical alignment setting

  Parameters:
    leWidgetWidget* _this - The widget to operate on
    leVAlignment valgn - the vertical alignment

  Remarks:
    Usage - _this->fn->setVAlignment(_this, valgn);

  Returns:
    leResult - the result of the operation
*/
/**
 * @brief Set widget vertical alignment.
 * @details Sets the vertical alignment of <span style="color: #820a32"><em>wgt</em></span> to
 * <span style="color: #820a32"><em>scheme</em></span>.
 * @remark This is a Virtual Member Function
 * @code
 * leWidget* wgt;
 * leVAlignment align;
 * leResult res = wgt->fn->setVAlignment(wgt, align);
 * @endcode
 * @param param1 wgt is the widget to modify
 * @param param1 scheme is the alignment to set
 * @returns LE_SUCCESS if set, otherwise LE_FAILURE.
 */
leResult _leWidget_SetVAlignment(leWidget* _this,
                                 leVAlignment align);

// *****************************************************************************
/* Virtual Member Function:
    leMargin getMargins(const leWidgetWidget* _this)

  Summary:
     Gets this widget's margins

  Description:
     Gets this widget's margins

  Parameters:
    const leWidgetWidget* _this - The widget to operate on

  Remarks:
    Usage - _this->fn->getMargins(_this);

  Returns:
    leMargin - the margin value
*/
/**
 * @brief Get widget margins.
 * @details Returns the margins of <span style="color: #820a32"><em>wgt</em></span>.
 * @remark This is a Virtual Member Function
 * @code
 * leWidget* wgt;
 * leMargin margin = wgt->fn->getMargins(wgt);
 * @endcode
 * @param wgt the widget to query
 * @param mg the margins
 * @return returns leResult.
 */
leResult _leWidget_GetMargins(const leWidget* _this, leMargin* mg);

// *****************************************************************************
/* Virtual Member Function:
    leResult setMargins(leWidgetWidget* _this,
                        uint32_t l,
                        uint32_t t,
                        uint32_t r,
                        uint32_t b)

  Summary:
     Sets this widget's margins

  Description:
     Sets this widget's margins

  Parameters:
    leWidgetWidget* _this - The widget to operate on
    uint32_t l - the left margin value
    uint32_t t - the top margin value
    uint32_t r - the right margin value
    uint32_t b - the bottom margin value

  Remarks:
    Usage - _this->fn->setMargins(_this, l, t, r, b);

  Returns:
    leResult - the result of the operation
*/
/**
 * @brief Set widget margins.
 * @details Sets the margins for <span style="color: #820a32"><em>wgt</em></span> to
 * the specified values <span style="color: #820a32"><em>left</em></span>
 * <span style="color: #820a32"><em>top</em></span>
 * <span style="color: #820a32"><em>right</em></span>
 * <span style="color: #820a32"><em>bottom</em></span>.
 * @remark This is a Virtual Member Function
 * @code
 * leWidget* wgt;
 * leScheme* scheme
 * leResult res = wgt->fn->setScheme(wgt, l, t, r,b );
 * @endcode
 * @param param1 wgt is the widget to modify
 * @returns LE_SUCCESS if set, otherwise LE_FAILURE.
 */
leResult _leWidget_SetMargins(leWidget* _this,
                              uint32_t l,
                              uint32_t t,
                              uint32_t r,
                              uint32_t b);

// *****************************************************************************
/* Virtual Member Function:
    uint32_t getCornerRadius(const leWidgetWidget* _this)

  Summary:
     Gets this widget's corner radius value

  Description:
     Gets this widget's corner radius value

  Parameters:
    const leWidgetWidget* _this - The widget to operate on

  Remarks:
    Usage - _this->fn->getCornerRadius(_this);

  Returns:
    uint32_t - the radius value
*/
/**
 * @brief Get widget corner radius.
 * @details Returns the corner radius of <span style="color: #820a32"><em>wgt</em></span>.
 * @remark This is a Virtual Member Function
 * @code
 * leWidget* wgt;
 * uint32_t radius = wgt->fn->getMargins(wgt);
 * @endcode
 * @param param1 wgt is the widget to query
 * @return returns uint32_t.
 */
uint32_t _leWidget_GetCornerRadius(const leWidget* _this);

// *****************************************************************************
/* Virtual Member Function:
    leResult setCornerRadius(leWidgetWidget* _this,
                             uint32_t rad)

  Summary:
     Sets this widget's corner radius value

  Description:
     Sets this widget's corner radius value

  Parameters:
    leWidgetWidget* _this - The widget to operate on
    uint32_t rad - the radius value

  Remarks:
    Usage - _this->fn->setCornerRadius(_this, rad);

  Returns:
    leResult - the result of the operation
*/
/**
 * @brief Set widget scheme.
 * @details Sets the scheme of <span style="color: #820a32"><em>wgt</em></span> to
 * <span style="color: #820a32"><em>scheme</em></span>.
 * @remark This is a Virtual Member Function
 * @code
 * leWidget* wgt;
 * leScheme* scheme
 * leResult res = wgt->fn->setScheme(wgt, scheme);
 * @endcode
 * @param param1 wgt is the widget to modify
 * @param param1 scheme is the scheme to set
 * @returns LE_SUCCESS if set, otherwise LE_FAILURE.
 */
leResult _leWidget_SetCornerRadius(leWidget* _this,
                                   uint32_t radius);

// *****************************************************************************
/* Virtual Member Function:
    leBool hasFocus(const leWidgetWidget* _this)

  Summary:
     Indicates if this widget currently has input focus

  Description:
     Indicates if this widget currently has input focus

  Parameters:
    const leWidgetWidget* _this - The widget to operate on

  Remarks:
    Usage - _this->fn->hasFocus(_this);

  Returns:
    leBool - true if focused
*/
/**
 * @brief Determines the focus status.
 * @details Returns the focus status of <span style="color: #820a32"><em>wgt</em></span>.
 * @remark This is a Virtual Member Function
 * @code
 * leWidget* wgt;
 * leResult res = wgt->fn->setScheme(wgt);
 * @endcode
 * @param param1 wgt is the widget to modify
 * @return returns true if widget has focus, otherwise false.
 */
leBool _leWidget_HasFocus(const leWidget* _this);

// *****************************************************************************
/* Virtual Member Function:
    leResult setFocus(leWidgetWidget* _this)

  Summary:
     Attempts to focus this widget

  Description:
     Attempts to focus this widget

  Parameters:
    leWidgetWidget* _this - The widget to operate on

  Remarks:
    Usage - _this->fn->setFocus(_this);

  Returns:
    leResult - the result of the operation
*/
/**
 * @brief Set widget scheme.
 * @details Sets the scheme of <span style="color: #820a32"><em>wgt</em></span> to
 * <span style="color: #820a32"><em>scheme</em></span>.
 * @remark This is a Virtual Member Function
 * @code
 * leWidget* wgt;
 * leScheme* scheme
 * leResult res = wgt->fn->setScheme(wgt, scheme);
 * @endcode
 * @param param1 wgt is the widget to modify
 * @param param1 scheme is the scheme to set
 * @returns LE_SUCCESS if set, otherwise LE_FAILURE.
 */
leResult _leWidget_SetFocus(leWidget* _this);

// *****************************************************************************
/* Virtual Member Function:
    void invalidate(const leWidgetWidget* _this)

  Summary:
     Invalidates this widget so it will redraw itself

  Description:
     Invalidates this widget so it will redraw itself

  Parameters:
    const leWidgetWidget* _this - The widget to operate on

  Remarks:
    Usage - _this->fn->invalidate(_this);

  Returns:
    void
*/
/**
 * @brief Invalidate widget.
 * @details Invalidate <span style="color: #820a32"><em>wgt</em></span>. The invalidation
 * will cause the widget to be redrawn.
 * @remark This is a Virtual Member Function
 * @code
 * leWidget* wgt;
 * wgt->fn->invalidate(wgt);
 * @endcode
 * @param param1 wgt is the widget to invalidate
 * @return returns void.
 */
void _leWidget_Invalidate(const leWidget* _this);

// *****************************************************************************
/* Virtual Member Function:
    void invalidateContents(const leWidgetWidget* _this)

  Summary:
     Invalidates the contents of this widget

  Description:
     Invalidates the contents of this widget

  Parameters:
    const leWidgetWidget* _this - The widget to operate on

  Remarks:
    Usage - _this->fn->invalidateContents(_this);

  Returns:
    void
*/
/**
 * @brief Invalidate widget contents.
 * @details Invalidate contents of <span style="color: #820a32"><em>wgt</em></span> contents.
 * @remark This is a Virtual Member Function
 * @code
 * leWidget* wgt;
 * wgt->fn->invalidateContents(wgt);
 * @endcode
 * @param param1 wgt is the widget to invalidate
 * @return returns void.
 */
void _leWidget_InvalidateContents(const leWidget* _this);

// *****************************************************************************
/* Virtual Member Function:
    leResult installEventFilter(leWidgetWidget* _this,
                                leWidgetEventFilter fltr)

  Summary:
     Installs a widget event filter

  Description:
     Installs a widget event filter

  Parameters:
    leWidgetWidget* _this - The widget to operate on
    leWidgetEventFilter fltr - the callback pointer

  Remarks:
    Usage - _this->fn->installEventFilter(_this, fltr);

  Returns:
    leResult - the result of the operation
*/
/**
 * @brief Install event filter.
 * @details Sets the event filter of <span style="color: #820a32"><em>wgt</em></span> to
 * <span style="color: #820a32"><em>fltr</em></span>.
 * @remark This is a Virtual Member Function
 * @code
 * leWidget* wgt;
 * leWidgetEventFilter fltr;
 * leResult res = wgt->fn->invalidateContents(wgt, fltr);
 * @endcode
 * @param param1 wgt is the widget to modify
 * @param param2 fltr is the filter to install
 * @returns LE_SUCCESS if set, otherwise LE_FAILURE.
 */
leResult _leWidget_InstallEventFilter(leWidget* _this,
                                      leWidgetEventFilter fltr);

// *****************************************************************************
/* Virtual Member Function:
    leResult removeEventFilter(leWidgetWidget* _this,
                               leWidgetEventFilter fltr)

  Summary:
     Removes a widget event filter

  Description:
     Removes a widget event filter

  Parameters:
    leWidgetWidget* _this - The widget to operate on
    leWidgetEventFilter fltr - the callback pointer

  Remarks:
    Usage - _this->fn->removeEventFilter(_this, fltr);

  Returns:
    leResult - the result of the operation
*/
/**
 * @brief Remove event filter.
 * @details Removes the event filter <span style="color: #820a32"><em>fltr</em></span> from
 * <span style="color: #820a32"><em>wgt</em></span>.
 * @remark This is a Virtual Member Function
 * @code
 * leWidget* wgt;
 * leWidgetEventFilter fltr;
 * leResult res = wgt->fn->removeEventFilter(wgt, fltr);
 * @endcode
 * @param param1 wgt is the widget to modify
 * @param param2 fltr is the filter to install
 * @returns LE_SUCCESS if set, otherwise LE_FAILURE.
 */
leResult _leWidget_RemoveEventFilter(leWidget* _this,
                                     leWidgetEventFilter fltr);

// *****************************************************************************
/* Virtual Member Function:
    void update(leWidgetWidget* _this,
                uint32_t dt)

  Summary:
     The widget update/tasks function

  Description:
     The widget update/tasks function

  Parameters:
    leWidgetWidget* _this - The widget to operate on
    uint32_t dt -

  Remarks:
    Usage - _this->fn->update(_this, dt);

  Returns:
    void
*/
/**
 * @brief Update widget.
 * @details Updates <span style="color: #820a32"><em>wgt</em></span>. This is automatically
 * called from the task machine.
 * @remark This is a Virtual Member Function
 * @code
 * leWidget* wgt;
 * uint32_t dt;
 * wgt->fn->update(wgt);
 * @endcode
 * @param param1 wgt is the widget to modify
 * @param param2 dt is
 * @return returns void.
 */
void _leWidget_Update(leWidget* _this, uint32_t dt);

void _leWidget_HandleEvent(leWidget*, leEvent*);
void _leWidget_ValidateChildren(leWidget*);
void _leWidget_IncreaseDirtyState(leWidget*, uint32_t);
void _leWidget_SetDirtyState(leWidget*, uint32_t);
void _leWidget_ClearDirtyState(leWidget*);
void _leWidget_InvalidateBorderAreas(const leWidget*);
void _leWidget_DamageArea(const leWidget*, leRect*);
void _leWidget_TouchDownEvent(leWidget*, leWidgetEvent_TouchDown*);
void _leWidget_TouchUpEvent(leWidget*, leWidgetEvent_TouchUp*);
void _leWidget_TouchMoveEvent(leWidget*, leWidgetEvent_TouchMove*);
void _leWidget_MoveEvent(leWidget*, leWidget_MoveEvent*);
void _leWidget_ResizeEvent(leWidget*, leWidget_ResizeEvent*);
void _leWidget_FocusLostEvent(leWidget*);
void _leWidget_FocusGainedEvent(leWidget*);
void _leWidget_LanguageChangeEvent(leWidget*);

#endif /* LEGATO_WIDGET_H */
