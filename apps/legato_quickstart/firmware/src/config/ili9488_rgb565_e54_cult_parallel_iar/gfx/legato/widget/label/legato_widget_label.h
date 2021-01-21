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
    legato_widget_label.h

  Summary:


  Description:
    This module implements label (text) widget functions.
*******************************************************************************/

// DOM-IGNORE-BEGIN
#ifndef LEGATO_LABEL_H
#define LEGATO_LABEL_H
//DOM-IGNORE-END

#include "gfx/legato/common/legato_common.h"

#if LE_LABEL_WIDGET_ENABLED

#include "gfx/legato/widget/legato_widget.h"
#include "gfx/legato/string/legato_string.h"

// *****************************************************************************
// *****************************************************************************
// Section: Data Types and Constants
// *****************************************************************************
// *****************************************************************************

// DOM-IGNORE-BEGIN
typedef struct leLabelWidget leLabelWidget;

#define LE_LABELWIDGET_VTABLE(THIS_TYPE) \
    LE_WIDGET_VTABLE(THIS_TYPE) \
    \
    leString*  (*getString)(const THIS_TYPE* _this); \
    leResult   (*setString)(THIS_TYPE* _this, leString* str); \
    
typedef struct leLabelWidgetVTable
{
	LE_LABELWIDGET_VTABLE(leLabelWidget)
} leLabelWidgetVTable; 

// DOM-IGNORE-END

// *****************************************************************************
/* Structure:
    leLabelWidget

  Summary:
    Implementation of a label widget struct

  Description:
    A label widget is a simple widget that draws a string of text.

  Remarks:
    None.
*/
typedef struct leLabelWidget
{
    leWidget widget; // widget base class

    const leLabelWidgetVTable* fn;

    const leString* string; // string to draw
} leLabelWidget;


// *****************************************************************************
// *****************************************************************************
// Section: Routines
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/* Function:
    leLabelWidget* leLabelWidget_New()

  Summary:
    Allocates memory for a new widget of this type.  The application is
    responsible for the managment of this memory until the widget is added to
    a widget tree.

  Description:


  Parameters:

  Returns:
    leLabelWidget*

  Remarks:
    Use leWidget_Delete() to free this pointer.
*/
LIB_EXPORT leLabelWidget* leLabelWidget_New();

/* Function:
    void leLabelWidget_Constructor(leLabelWidget* wgt)

  Summary:
    Initializes an leLabelWidget widget pointer.

  Description:
    Initializes an leLabelWidget widget pointer.

  Parameters:
    leLabelWidget* wgt - the pointer to initialize

  Returns:
    void

  Remarks:

*/
LIB_EXPORT void leLabelWidget_Constructor(leLabelWidget* lbl);

// *****************************************************************************
/* Virtual Member Function:
    leString* getString(const leLabelWidget* _this)

  Summary:
     Gets the label string

  Description:
     Gets the label string

  Parameters:
    const leLabelWidget* _this - The label widget to operate on

  Remarks:
    Usage - _this->fn->getString(_this);

  Returns:
    leString* - the string pointer
*/

// *****************************************************************************
/* Virtual Member Function:
    leResult setString(leLabelWidget* _this,
                       leString* str)

  Summary:
     Sets the label string

  Description:
     Sets the label string

  Parameters:
    leLabelWidget* _this - The label widget to operate on
    leString* str - the string pointer

  Remarks:
    Usage - _this->fn->setString(_this, str);

  Returns:
    leResult - the result of the operation
*/



#endif // LE_LABEL_WIDGET_ENABLED
#endif /* LEGATO_LABEL_H */
