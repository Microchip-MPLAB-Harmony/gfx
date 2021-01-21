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
    legato_widget.h

  Summary:


  Description:
    This module implements top level widget control functions.
*******************************************************************************/

// DOM-IGNORE-BEGIN

#ifndef LEGATO_WIDGET_H
#define LEGATO_WIDGET_H
//DOM-IGNORE-END


#include "gfx/legato/common/legato_common.h"
#include "gfx/legato/common/legato_rect.h"
#include "gfx/legato/core/legato_event.h"
#include "gfx/legato/core/legato_scheme.h"
#include "gfx/legato/datastructure/legato_array.h"

// DOM-IGNORE-BEGIN
#define DEFAULT_BORDER_MARGIN   4
// DOM-IGNORE-END

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
#if LE_BUTTON_WIDGET_ENABLED == 1
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
#if LE_LIST_WIDGET_ENABLED && LE_SCROLLBAR_WIDGET_ENABLED == 1
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
typedef enum leBackgroundType
{
    LE_WIDGET_BACKGROUND_NONE,
    LE_WIDGET_BACKGROUND_FILL,
    LE_WIDGET_BACKGROUND_LAST = LE_WIDGET_BACKGROUND_FILL
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
typedef enum leWidgetDrawState
{
    LE_WIDGET_DRAW_STATE_READY,
    LE_WIDGET_DRAW_STATE_DONE,

    // widget-specific draw states start here
} leWidgetDrawState;

typedef struct leWidget_MoveEvent
{
    leEvent evt;
    
    int32_t oldX;
    int32_t oldY;
    int32_t newX;
    int32_t newY;
} leWidget_MoveEvent;

typedef struct leWidget_ResizeEvent
{
    leEvent evt;
    
    uint32_t oldWidth;
    uint32_t oldHeight;
    uint32_t newWidth;
    uint32_t newHeight;
} leWidget_ResizeEvent;

// *****************************************************************************
/* Structure:
    leWidgetEvent

  Summary:
    Basic widget event definition
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
typedef struct
{
    leBool (*filterEvent)(leWidget* target, leWidgetEvent* evt, void* data);
    void* data;
} leWidgetEventFilter;

// DOM-IGNORE-BEGIN
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
    leRect             (*localRect)(const THIS_TYPE* _this); \
    leRect             (*rectToParent)(const THIS_TYPE* _this); \
    leRect             (*rectToScreen)(const THIS_TYPE* _this); \
    leResult           (*addChild)(THIS_TYPE* _this, leWidget* chld); \
    leResult           (*removeChild)(THIS_TYPE* _this, leWidget* chld); \
    void               (*removeAllChildren)(THIS_TYPE* _this); \
    leWidget*          (*getRootWidget)(const THIS_TYPE* _this); \
    leResult           (*setParent)(THIS_TYPE* _this, leWidget* pnt); \
    uint32_t           (*getChildCount)(const THIS_TYPE* _this); \
    leWidget*          (*getChildAtIndex)(const THIS_TYPE* _this, uint32_t idx); \
    uint32_t           (*getIndexOfChild)(const THIS_TYPE* _this, const leWidget* chld); \
    leBool             (*containsDescendent)(const THIS_TYPE* _this, const leWidget* wgt); \
    leScheme*          (*getScheme)(const THIS_TYPE* _this); \
    leResult           (*setScheme)(THIS_TYPE* _this, leScheme* schm); \
    leBorderType       (*getBorderType)(const THIS_TYPE* _this); \
    leResult           (*setBorderType)(THIS_TYPE* _this, leBorderType type); \
    leBackgroundType   (*getBackgroundType)(const THIS_TYPE* _this); \
    leResult           (*setBackgroundType)(THIS_TYPE* _this, leBackgroundType type); \
    leHAlignment       (*getHAlignment)(const THIS_TYPE* _this); \
    leResult           (*setHAlignment)(THIS_TYPE* _this, leHAlignment halgn); \
    leVAlignment       (*getVAlignment)(const THIS_TYPE* _this); \
    leResult           (*setVAlignment)(THIS_TYPE* _this, leVAlignment valgn); \
    leMargin           (*getMargins)(const THIS_TYPE* _this); \
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

// DOM-IGNORE-END

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
typedef struct leWidget
{
    const leWidgetVTable* fn;
    
    uint32_t id;  // the id of the widget
    leWidgetType type; // the type of the widget

    leBool visible; // the widget visible flag
    leBool enabled; // the widget enabled flag

    leRect rect; // the bounding rectangle of the widget
    uint32_t cornerRadius; //corner radius, draws round corners if > 0
    
    leHAlignment halign; // horizontal alignment of the widget
    leVAlignment valign; // vertical alignment of the widget

    leMargin margin; // the margin settings for the widget

    leBorderType borderType; // the widget border type
    leBackgroundType backgroundType; // the widget background type

    uint32_t optimizationFlags; // optimization flags

    uint32_t drawCount; // number of times this widget has been drawn
                        // for the active screen

    leBool alphaEnabled;    // indicates that the global alpha blending
                            // setting is enabled for this widget
    uint32_t alphaAmount;   // the global alpha amount to apply to this
                            // widget (cumulative with parent widgets)

    uint32_t dirtyState;    // the widget's dirty state
    uint32_t drawState;     // the widget's draw state

    leWidget_DrawFunction_FnPtr drawFunc; // the next draw function to call

    const leScheme* scheme; // the widget's color scheme

    //leWidgetInputHandler externalHandler;

    leWidgetEventFilter eventFilters[LE_WIDGET_MAX_EVENT_FILTERS];

    leBool root;      // indicates if this widget is a root widget
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
LIB_EXPORT leWidget* leWidget_New();

/* Function:
    void leWidget_Constructor(leWidget* wgt)

  Summary:
    Initializes an leWidget widget pointer.

  Description:
    Initializes an leWidget widget pointer.

  Parameters:
    leWidget* wgt - the pointer to initialize

  Returns:
    void

  Remarks:

*/
LIB_EXPORT void leWidget_Constructor(leWidget* wgt);

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
LIB_EXPORT void leWidget_Delete(leWidget* wgt);

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
leRect _leWidget_LocalRect(const leWidget* _this);

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
leRect _leWidget_RectToParentSpace(const leWidget* _this);

// *****************************************************************************
/* Virtual Member Function:
    leRect rectToScreen(const leWidgetWidget* _this)

  Summary:
     Gets the widget's bounding rectangle in screen space

  Description:
     Gets the widget's bounding rectangle in screen space

  Parameters:
    const leWidgetWidget* _this - The widget to operate on

  Remarks:
    Usage - _this->fn->rectToScreen(_this);

  Returns:
    leRect - the bounding rectangle
*/
leRect _leWidget_RectToScreenSpace(const leWidget* _this);

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
leResult _leWidget_AddChild(leWidget* _this,
                            leWidget* child);

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
leResult _leWidget_RemoveChild(leWidget* _this,
                               leWidget* child);

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
uint32_t _leWidget_GetIndexOfChild(const leWidget* _this,
                                   const leWidget* child);

// *****************************************************************************
/* Virtual Member Function:
    leBool containsDescendent(const leWidgetWidget* _this,
                              const leWidget* wgt)

  Summary:
     Determines of a widget's descendent tree contains a given widget

  Description:
     Determines of a widget's descendent tree contains a given widget

  Parameters:
    const leWidgetWidget* _this - The widget to operate on
    const leWidget* wgt - the widget

  Remarks:
    Usage - _this->fn->containsDescendent(_this, wgt);

  Returns:
    leBool - LE_TRUE if the widget is a descentdent of this
*/
leBool _leWidget_ContainsDescendent(const leWidget* _this,
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
leScheme* _leWidget_GetScheme(const leWidget* _this);

// *****************************************************************************
/* Virtual Member Function:
    leResult setScheme(leWidgetWidget* _this,
                       leScheme* schm)

  Summary:
     Sets this widget's scheme

  Description:
     Sets this widget's scheme

  Parameters:
    leWidgetWidget* _this - The widget to operate on
    leScheme* schm - the scheme pointer

  Remarks:
    Usage - _this->fn->setScheme(_this, schm);

  Returns:
    leResult - the result of the operation
*/
leResult _leWidget_SetScheme(leWidget* _this,
                             leScheme* scheme);

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
leMargin _leWidget_GetMargins(const leWidget* _this);

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
