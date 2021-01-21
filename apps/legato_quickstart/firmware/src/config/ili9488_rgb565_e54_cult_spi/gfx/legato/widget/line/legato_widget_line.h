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
    legato_widget_line.h

  Summary:


  Description:
    This module implements line draw widget functions.
*******************************************************************************/

// DOM-IGNORE-BEGIN
#ifndef LEGATO_LINE_H
#define LEGATO_LINE_H
//DOM-IGNORE-END

#include "gfx/legato/common/legato_common.h"

#if LE_LINE_WIDGET_ENABLED

#include "gfx/legato/widget/legato_widget.h"

typedef struct leLineWidget leLineWidget;

// *****************************************************************************
// *****************************************************************************
// Section: Data Types and Constants
// *****************************************************************************
// *****************************************************************************

// DOM-IGNORE-BEGIN
typedef struct leLineWidget leLineWidget;

#define LE_LINEWIDGET_VTABLE(THIS_TYPE) \
    LE_WIDGET_VTABLE(THIS_TYPE) \
    \
    lePoint   (*getStartPoint)(const THIS_TYPE* _this); \
    leResult  (*setStartPoint)(THIS_TYPE* _this, int32_t x, int32_t y); \
    lePoint   (*getEndPoint)(const THIS_TYPE* _this); \
    leResult  (*setEndPoint)(THIS_TYPE* _this, int32_t x, int32_t y); \
    
typedef struct leLineWidgetVTable
{
	LE_LINEWIDGET_VTABLE(leLineWidget)
} leLineWidgetVTable; 

// DOM-IGNORE-END

// *****************************************************************************
/* Structure:
    leLineWidget

  Summary:
    Defines the implementation of a line widget struct

  Description:
    A line widget draws a simple line shape within the confines of its
    bounding rectangle.  All coordinates are expressed in local widget space.

    The color of the line is determined by the widget scheme's 'foreground'
    color.

  Remarks:
    None.
*/
typedef struct leLineWidget
{
    leWidget widget; // widget base class

    const leLineWidgetVTable* fn;

    int32_t x1; // point 1 x
    int32_t y1; // point 1 y
    int32_t x2; // point 2 x
    int32_t y2; // point 2 y
} leLineWidget;

// *****************************************************************************
// *****************************************************************************
// Section: Routines
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/* Function:
    leLineWidget* leLineWidget_New()

  Summary:
    Allocates memory for and initializes a new widget of this type.  The
    application is responsible for the managment of this memory until the
    widget is added to a widget tree.

  Description:


  Parameters:

  Returns:
    leLineWidget*

  Remarks:
    Use leWidget_Delete() to free this pointer.
*/
LIB_EXPORT leLineWidget* leLineWidget_New();

/* Function:
    void leLineWidget_Constructor(leLineWidget* wgt)

  Summary:
    Initializes an leLineWidget widget pointer.

  Description:
    Initializes an leLineWidget widget pointer.

  Parameters:
    leLineWidget* wgt - the pointer to initialize

  Returns:
    void

  Remarks:

*/
LIB_EXPORT void leLineWidget_Constructor(leLineWidget* line);

// *****************************************************************************
/* Virtual Member Function:
    lePoint getStartPoint(const leLineWidget* _this)

  Summary:
     Gets the line start point

  Description:
     Gets the line start point

  Parameters:
    const leLineWidget* _this - The line widget to operate on

  Remarks:
    Usage - _this->fn->getStartPoint(_this);

  Returns:
    lePoint - the point
*/

// *****************************************************************************
/* Virtual Member Function:
    leResult setStartPoint(leLineWidget* _this,
                           int32_t x,
                           int32_t y)

  Summary:
     Sets the line start point

  Description:
     Sets the line start point

  Parameters:
    leLineWidget* _this - The line widget to operate on
    int32_t x - the X value
    int32_t y - the Y value

  Remarks:
    Usage - _this->fn->setStartPoint(_this, x, y);

  Returns:
    leResult - the result of the operation
*/

// *****************************************************************************
/* Virtual Member Function:
    lePoint getEndPoint(const leLineWidget* _this)

  Summary:
     Gets the line end point

  Description:
     Gets the line end point

  Parameters:
    const leLineWidget* _this - The line widget to operate on

  Remarks:
    Usage - _this->fn->getEndPoint(_this);

  Returns:
    lePoint - the point
*/

// *****************************************************************************
/* Virtual Member Function:
    leResult setEndPoint(leLineWidget* _this,
                         int32_t x,
                         int32_t y)

  Summary:
     Sets the line end point

  Description:
     Sets the line end point

  Parameters:
    leLineWidget* _this - The line widget to operate on
    int32_t x - the X value
    int32_t y - the Y value

  Remarks:
    Usage - _this->fn->setEndPoint(_this, x, y);

  Returns:
    leResult - the result of the operation
*/



#endif // LE_LINE_WIDGET_ENABLED
#endif /* LEGATO_LINE_H */