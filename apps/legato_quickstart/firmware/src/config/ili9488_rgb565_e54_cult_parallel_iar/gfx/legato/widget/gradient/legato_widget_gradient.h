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
    legato_widget_gradient.h

  Summary:


  Description:
    This module implements gradient drawing widget functions.
*******************************************************************************/

// DOM-IGNORE-BEGIN

#ifndef LEGATO_GRADIENT_H
#define LEGATO_GRADIENT_H
//DOM-IGNORE-END

#include "gfx/legato/common/legato_common.h"

#if LE_GRADIENT_WIDGET_ENABLED

#include "gfx/legato/widget/legato_widget.h"

// *****************************************************************************
// *****************************************************************************
// Section: Data Types and Constants
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************

// DOM-IGNORE-BEGIN

typedef struct leGradientWidget leGradientWidget;

#define LE_GRADIENTWIDGET_VTABLE(THIS_TYPE) \
    LE_WIDGET_VTABLE(THIS_TYPE) \
    \
    leDirection (*getDirection)(const THIS_TYPE* _this); \
    leResult                  (*setDirection)(THIS_TYPE* _this, leDirection dir); \

typedef struct leGradientWidgetVTable
{
	LE_GRADIENTWIDGET_VTABLE(leGradientWidget)
} leGradientWidgetVTable; 

// DOM-IGNORE-END

// *****************************************************************************
/* Enumeration:
    leGradientWidget

  Summary:
    Gradient widget struct definition.

  Description:


  Remarks:
    None.
*/
typedef struct leGradientWidget
{
    leWidget widget; // widget base class

    const leGradientWidgetVTable* fn;

    leDirection dir; // gradient direction
} leGradientWidget;

// *****************************************************************************
// *****************************************************************************
// Section: Routines
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/* Function:
    leGradientWidget* leGradientWidget_New()

  Summary:
    Allocates memory for and initializes a new widget of this type.  The
    application is responsible for the managment of this memory until the
    widget is added to a widget tree.

  Description:


  Parameters:
    void

  Returns:
    leGradientWidget*

  Remarks:
    Use leWidget_Delete() to free this pointer.
*/
LIB_EXPORT leGradientWidget* leGradientWidget_New();

/* Function:
    void leGradientWidget_Constructor(leGradientWidget* wgt)

  Summary:
    Initializes an leGradientWidget widget pointer.

  Description:
    Initializes an leGradientWidget widget pointer.

  Parameters:
    leGradientWidget* wgt - the pointer to initialize

  Returns:
    void

  Remarks:

*/
LIB_EXPORT void leGradientWidget_Constructor(leGradientWidget* grad);

// *****************************************************************************
/* Virtual Member Function:
    leDirection getDirection(const leGradientWidget* _this)

  Summary:
     Gets the gradient direction

  Description:
     Gets the gradient direction

  Parameters:
    const leGradientWidget* _this - The gradient to operate on

  Remarks:
    Usage - _this->fn->getDirection(_this);

  Returns:
    leDirection - the direction
*/

// *****************************************************************************
/* Virtual Member Function:
    leResult setDirection(leGradientWidget* _this,
                          leDirection dir)

  Summary:
     Sets the gradient direction

  Description:
     Sets the gradient direction

  Parameters:
    leGradientWidget* _this - The gradient to operate on
    leDirection dir - the direction

  Remarks:
    Usage - _this->fn->setDirection(_this, dir);

  Returns:
    leResult - the result of the operation
*/



#endif // LE_GRADIENT_WIDGET_ENABLED
#endif /* LEGATO_RECTANGLE_H */