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
    legato_widget_groupbox.h

  Summary:


  Description:
    This module implements group box widget functions.
*******************************************************************************/

// DOM-IGNORE-BEGIN

#ifndef LEGATO_GROUPBOX_H
#define LEGATO_GROUPBOX_H
//DOM-IGNORE-END

#include "gfx/legato/common/legato_common.h"

#if LE_GROUPBOX_WIDGET_ENABLED

#include "gfx/legato/widget/legato_widget.h"
#include "gfx/legato/string/legato_string.h"

// *****************************************************************************
// *****************************************************************************
// Section: Data Types and Constants
// *****************************************************************************
// *****************************************************************************

// DOM-IGNORE-BEGIN
typedef struct leGroupBoxWidget leGroupBoxWidget;

#define LE_GROUPBOXWIDGET_VTABLE(THIS_TYPE) \
    LE_WIDGET_VTABLE(THIS_TYPE) \
    \
    leString*        (*getString)(const THIS_TYPE* _this); \
    leResult         (*setString)(THIS_TYPE* _this, const leString* str); \
    
typedef struct leGroupBoxWidgetVTable
{
	LE_GROUPBOXWIDGET_VTABLE(leGroupBoxWidget)
} leGroupBoxWidgetVTable; 

// DOM-IGNORE-END

// *****************************************************************************
/* Enumeration:
    leGroupBoxWidget

  Summary:
    Group box struct definition.

  Description:
    A group box is a widget that is similar to a basic panel but provides
    a line border and title text.  Used for grouping and describing widgets
    of similar function.

  Remarks:
    None.
*/
typedef struct leGroupBoxWidget
{
    leWidget widget; // widget base class

    const leGroupBoxWidgetVTable* fn;

    const leString* string; // group box title text
} leGroupBoxWidget;

// *****************************************************************************
// *****************************************************************************
// Section: Routines
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/* Function:
    leGroupBoxWidget* leGroupBoxWidget_New()

  Summary:
    Allocates memory for and initializes a new widget of this type.  The
    application is responsible for the managment of this memory until the
    widget is added to a widget tree.

  Description:
    Allocates memory for and initializes a new widget of this type.  The
    application is responsible for the managment of this memory until the
    widget is added to a widget tree.

  Parameters:
    void

  Returns:
    leGroupBoxWidget*

  Remarks:
    Use leWidget_Delete() to free this pointer.
*/
LIB_EXPORT leGroupBoxWidget* leGroupBoxWidget_New();

/* Function:
    void leGroupBoxWidget_Constructor(leGroupBoxWidget* wgt)

  Summary:
    Initializes an leGroupBoxWidget widget pointer.

  Description:
    Initializes an leGroupBoxWidget widget pointer.

  Parameters:
    leGroupBoxWidget* wgt - the pointer to initialize

  Returns:
    void

  Remarks:

*/
void leGroupBoxWidget_Constructor(leGroupBoxWidget* box);

// *****************************************************************************
/* Virtual Member Function:
    leString* getString(const leGroupBoxWidget* _this)

  Summary:
     Gets the box title string

  Description:
     Gets the box title string

  Parameters:
    const leGroupBoxWidget* _this - The group box to operate on

  Remarks:
    Usage - _this->fn->getString(_this);

  Returns:
    leString* - the string pointer
*/

// *****************************************************************************
/* Virtual Member Function:
    leResult setString(leGroupBoxWidget* _this,
                       const leString* str)

  Summary:
     Sets the box title string

  Description:
     Sets the box title string

  Parameters:
    leGroupBoxWidget* _this - The group box to operate on
    const leString* str - the string pointer

  Remarks:
    Usage - _this->fn->setString(_this, str);

  Returns:
    leResult - the result of the operation
*/



#endif // LE_GROUPBOX_WIDGET_ENABLED
#endif /* LEGATO_GROUPBOX_H */