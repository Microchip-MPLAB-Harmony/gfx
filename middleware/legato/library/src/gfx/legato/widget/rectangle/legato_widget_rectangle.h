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
    legato_widget_rectangle.h

  Summary:
    

  Description:
    This module implements rectangle drawing widget functions.
*******************************************************************************/

// DOM-IGNORE-BEGIN

#ifndef LEGATO_RECTANGLE_H
#define LEGATO_RECTANGLE_H
//DOM-IGNORE-END

#include "gfx/legato/common/legato_common.h"

#if LE_RECTANGLE_WIDGET_ENABLED == 1

#include "gfx/legato/widget/legato_widget.h"

// *****************************************************************************
// *****************************************************************************
// Section: Data Types and Constants
// *****************************************************************************
// *****************************************************************************

typedef struct leRectangleWidget leRectangleWidget;

#define LE_RECTANGLEWIDGET_VTABLE(THIS_TYPE) \
    LE_WIDGET_VTABLE(THIS_TYPE) \
    \
    int32_t   (*getThickness)(const THIS_TYPE* _this); \
    leResult  (*setThickness)(THIS_TYPE* _this, int32_t thk); \
    
typedef struct leRectangleWidgetVTable
{
	LE_RECTANGLEWIDGET_VTABLE(leRectangleWidget)
} leRectangleWidgetVTable; 

// *****************************************************************************
/* Enumeration:
    leRectangleWidget

  Summary:
    Implementation of a rectangle widget struct

  Description:
    A rectangle widget draws a basic rectangle of a given thickness using the
    widget's bounding box as the dimensions.

  Remarks:
    None.
*/
typedef struct leRectangleWidget
{
    leWidget widget; // widget base class

    const leRectangleWidgetVTable* fn;

    int32_t thickness; // rectangle border thickness
} leRectangleWidget;

// *****************************************************************************
// *****************************************************************************
// Section: Routines
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/* Function:
    leRectangleWidget* leRectangleWidget_New()

  Summary:
    Allocates memory for a new widget of this type.  The application is
    responsible for the managment of this memory until the widget is added to
    a widget tree.

  Description:
    

  Parameters:
    
  Returns:
    leRectangleWidget*
    
  Remarks:
    Use leWidget_Delete() to free this pointer.
*/
LIB_EXPORT leRectangleWidget* leRectangleWidget_New();

/* Function:
    void leRectangleWidget_Constructor(leRectangleWidget* wgt)

  Summary:
    Initializes an leRectangleWidget widget pointer.

  Description:
    Initializes an leRectangleWidget widget pointer.

  Parameters:
    leRectangleWidget* wgt - the pointer to initialize

  Returns:
    void

  Remarks:

*/
void leRectangleWidget_Constructor(leRectangleWidget* _this);

// *****************************************************************************
/* Virtual Member Function:
    int32_t getThickness(const leRectangleWidget* _this)

  Summary:
     Gets the rectangle border thickness

  Description:
     Gets the rectangle border thickness

  Parameters:
    const leRectangleWidget* _this - The rectangle widget to operate on

  Remarks:
    Usage - _this->fn->getThickness(_this);

  Returns:
    int32_t - the thickness value
*/

// *****************************************************************************
/* Virtual Member Function:
    leResult setThickness(leRectangleWidget* _this,
                          int32_t thk)

  Summary:
     Sets the rectangle border thickness

  Description:
     Sets the rectangle border thickness

  Parameters:
    leRectangleWidget* _this - The rectangle widget to operate on
    int32_t thk -

  Remarks:
    Usage - _this->fn->setThickness(_this, thk);

  Returns:
    leResult - the result of the operation
*/



#endif // LE_RECTANGLE_WIDGET_ENABLED
#endif /* LEGATO_RECTANGLE_H */