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
    legato_widget_touchtest.h

  Summary:


  Description:
    This module implements graphical touch test (box) widget functions.
*******************************************************************************/

// DOM-IGNORE-BEGIN
#ifndef LEGATO_TOUCHTEST_H
#define LEGATO_TOUCHTEST_H
//DOM-IGNORE-END

#include "gfx/legato/common/legato_common.h"

#if LE_TOUCHTEST_WIDGET_ENABLED

#include "gfx/legato/widget/legato_widget.h"

// DOM-IGNORE-BEGIN
#define LE_TOUCHTEST_MEMORY_SIZE 20
// DOM-IGNORE-END

// *****************************************************************************
// *****************************************************************************
// Section: Data Types and Constants
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/* Enumeration:
    leTouchTestState

  Summary:
    Touch test states

  Description:


  Remarks:
    None.
*/
typedef enum leTouchTestState
{
    LE_TOUCHTEST_STATE_UP,
    LE_TOUCHTEST_STATE_DOWN
} leTouchTestState;

typedef struct leTouchTestWidget leTouchTestWidget;

// *****************************************************************************
/* Function Pointer:
    leTouchTestWidget_PointAddedEventCallback

  Summary:
    Point added event function callback type
*/
typedef void (*leTouchTestWidget_PointAddedEventCallback)(leTouchTestWidget*, lePoint*);


// DOM-IGNORE-BEGIN
typedef struct leTouchTestWidget leTouchTestWidget;

#define LE_TOUCHTESTWIDGET_VTABLE(THIS_TYPE) \
    LE_WIDGET_VTABLE(THIS_TYPE) \
    \
    leResult  (*addPoint)(THIS_TYPE* _this, lePoint* pnt); \
    leResult  (*clearPoints)(THIS_TYPE* _this); \
    leTouchTestWidget_PointAddedEventCallback (*getPointAddedEventCallback)(const THIS_TYPE* _this); \
    leResult  (*setPointAddedEventCallback)(THIS_TYPE* _this, leTouchTestWidget_PointAddedEventCallback cb); \
    
typedef struct leTouchTestWidgetVTable
{
	LE_TOUCHTESTWIDGET_VTABLE(leTouchTestWidget)
} leTouchTestWidgetVTable; 

// DOM-IGNORE-END


// *****************************************************************************
/* Structure:
    leTouchTestWidget

  Summary:
    Implementation of a touch test widget struct

  Description:
    The touch test widget is a specialized widget that displays intersecting
    lines based on input events.  This can help visualize touch interaction
    and aid determining accurate input coordinates.

  Remarks:
    None.
*/
typedef struct leTouchTestWidget
{
    leWidget widget; // widget base class

    const leTouchTestWidgetVTable* fn;

    leTouchTestState state; // touch test state

    lePoint pnts[LE_TOUCHTEST_MEMORY_SIZE]; // touch point array
    uint32_t size; // current number of valid touch points

    uint32_t start; // first valid touch point
    uint32_t next; // next available touch point entry

    leTouchTestWidget_PointAddedEventCallback cb; // point added callback
} leTouchTestWidget;

// *****************************************************************************
// *****************************************************************************
// Section: Routines
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/* Function:
    leTouchTestWidget* leTouchTestWidget_New()

  Summary:
    Allocates memory for a new widget of this type.  The application is
    responsible for the managment of this memory until the widget is added to
    a widget tree.

  Description:


  Parameters:

  Returns:
    leTouchTestWidget*

  Remarks:
    Use leWidget_Delete() to free this pointer.
*/
LIB_EXPORT leTouchTestWidget* leTouchTestWidget_New();

/* Function:
    void leTouchTestWidget_Constructor(leTouchTestWidget* wgt)

  Summary:
    Initializes an leTouchTestWidget widget pointer.

  Description:
    Initializes an leTouchTestWidget widget pointer.

  Parameters:
    leTouchTestWidget* wgt - the pointer to initialize

  Returns:
    void

  Remarks:

*/
LIB_EXPORT void leTouchTestWidget_Constructor(leTouchTestWidget* tch);

// *****************************************************************************
/* Virtual Member Function:
    leResult addPoint(leTouchTestWidget* _this,
                      lePoint* pnt)

  Summary:
     Add a point to the widget

  Description:
     Add a point to the widget

  Parameters:
    leTouchTestWidget* _this - The touchtest widget to operate on
    lePoint* pnt - the point

  Remarks:
    Usage - _this->fn->addPoint(_this, pnt);

  Returns:
    leResult - the result of the operation
*/

// *****************************************************************************
/* Virtual Member Function:
    leResult clearPoints(leTouchTestWidget* _this)

  Summary:
     Clear all points

  Description:
     Clear all points

  Parameters:
    leTouchTestWidget* _this - The touchtest widget to operate on

  Remarks:
    Usage - _this->fn->clearPoints(_this);

  Returns:
    leResult - the result of the operation
*/

// *****************************************************************************
/* Virtual Member Function:
    leTouchTestWidget_PointAddedEventCallback getPointAddedEventCallback(const leTouchTestWidget* _this)

  Summary:
     Get the point added event callback pointer

  Description:
     Get the point added event callback pointer

  Parameters:
    const leTouchTestWidget* _this - The touchtest widget to operate on

  Remarks:
    Usage - _this->fn->getPointAddedEventCallback(_this);

  Returns:
    leTouchTestWidget_PointAddedEventCallback - the callback pointer
*/

// *****************************************************************************
/* Virtual Member Function:
    leResult setPointAddedEventCallback(leTouchTestWidget* _this,
                                        leTouchTestWidget_PointAddedEventCallback cb)

  Summary:
     Set the point added event callback pointer

  Description:
     Set the point added event callback pointer

  Parameters:
    leTouchTestWidget* _this - The touchtest widget to operate on
    leTouchTestWidget_PointAddedEventCallback cb - the callback pointer

  Remarks:
    Usage - _this->fn->setPointAddedEventCallback(_this, cb);

  Returns:
    leResult - the result of the operation
*/



#endif // LE_TOUCHTEST_WIDGET_ENABLED
#endif /* LEGATO_TOUCHTEST_H */
