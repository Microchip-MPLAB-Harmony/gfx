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
    legato_editwidget.h

  Summary:
    

  Description:
    This module implements the routines to enable edit of library widgets.
*******************************************************************************/

/** \file legato_editwidget.h
 * @brief Edit widget functions and definitions.
 *
 * @details This module implements the routines to enable edit of library widgets.
 */

#ifndef LEGATO_EDITWIDGET_H
#define LEGATO_EDITWIDGET_H

#include "gfx/legato/common/legato_common.h"
#include "gfx/legato/widget/legato_widget.h"
#include "gfx/legato/string/legato_string.h"

// DOM-IGNORE-BEGIN
typedef struct leEditWidget leEditWidget;

#define LE_EDITWIDGET_VTABLE(THIS_TYPE) \
    LE_WIDGET_VTABLE(THIS_TYPE) \
    \
    leResult (*editStart)(THIS_TYPE* _this); \
    void     (*editEnd)(THIS_TYPE* _this); \
    void     (*editClear)(THIS_TYPE* _this); \
    void     (*editAccept)(THIS_TYPE* _this); \
    void     (*editSet)(THIS_TYPE* _this, const leString* str); \
    void     (*editAppend)(THIS_TYPE* _this, const leString* str); \
    void     (*editBackspace)(THIS_TYPE* _this); \
    
typedef struct leEditWidgetVTable
{
	LE_EDITWIDGET_VTABLE(leEditWidget)
} leEditWidgetVTable; 

// DOM-IGNORE-END

/* Structure:
        leEditWidget

    Summary:
        Specifies the edit widget structure to manage all properties and events
        associated with edit widgets

    Description:
        Edit widgets are a subset of normal widgets that are capable of receiving
        edit events from the UI kernel.  Specialized widgets are capable of
        broadcasting edit events and the active edit event will react to them.

    Remarks:
        None.
        
*/
typedef struct leEditWidget
{
    leWidget widget;
    
    const leEditWidgetVTable* fn;
    
} leEditWidget;

// *****************************************************************************
/* Virtual Member Function:
    leResult editStart(leEditWidget* _this)

  Summary:
     Start editing command

  Description:
     Start editing command

  Parameters:
    leEditWidget* _this - The edit widget to operate on

  Remarks:
    Usage - _this->fn->editStart(_this);

  Returns:
    leResult - the result of the operation
*/
leResult _leEditWidget_EditStart(leEditWidget* _this);

// *****************************************************************************
/* Virtual Member Function:
    void editEnd(leEditWidget* _this)

  Summary:
     End editing command

  Description:
     End editing command

  Parameters:
    leEditWidget* _this - The edit widget to operate on

  Remarks:
    Usage - _this->fn->editEnd(_this);

  Returns:
    void
*/
void _leEditWidget_EditEnd(leEditWidget* _this);

// *****************************************************************************
/* Virtual Member Function:
    void editClear(leEditWidget* _this)

  Summary:
     Clear command

  Description:
     Clear command

  Parameters:
    leEditWidget* _this - The edit widget to operate on

  Remarks:
    Usage - _this->fn->editClear(_this);

  Returns:
    void
*/
void _leEditWidget_EditClear(leEditWidget* _this);

// *****************************************************************************
/* Virtual Member Function:
    void editAccept(leEditWidget* _this)

  Summary:
     Accept command

  Description:
     Accept command

  Parameters:
    leEditWidget* _this - The edit widget to operate on

  Remarks:
    Usage - _this->fn->editAccept(_this);

  Returns:
    void
*/
void _leEditWidget_EditAccept(leEditWidget* _this);

// *****************************************************************************
/* Virtual Member Function:
    void editSet(leEditWidget* _this,
                 const leString* str)

  Summary:
     Set command

  Description:
     Set command

  Parameters:
    leEditWidget* _this - The edit widget to operate on
    const leString* str - the string pointer

  Remarks:
    Usage - _this->fn->editSet(_this, str);

  Returns:
    void
*/
void _leEditWidget_EditSet(leEditWidget* _this,
                           const leString* str);

// *****************************************************************************
/* Virtual Member Function:
    void editAppend(leEditWidget* _this,
                    const leString* str)

  Summary:
     Append command

  Description:
     Append command

  Parameters:
    leEditWidget* _this - The edit widget to operate on
    const leString* str - the string pointer

  Remarks:
    Usage - _this->fn->editAppend(_this, str);

  Returns:
    void
*/
void _leEditWidget_EditAppend(leEditWidget* _this,
                              const leString* str);

// *****************************************************************************
/* Virtual Member Function:
    void editBackspace(leEditWidget* _this)

  Summary:
     Backspace command

  Description:
     Backspace command

  Parameters:
    leEditWidget* _this - The edit widget to operate on

  Remarks:
    Usage - _this->fn->editBackspace(_this);

  Returns:
    void
*/
void _leEditWidget_EditBackspace(leEditWidget* _this);


// DOM-IGNORE-BEGIN
leResult _leEditWidget_StartEdit(void);
void _leEditWidget_EndEdit(void);

void _leEditWidget_Clear(void);
void _leEditWidget_Accept(void);
void _leEditWidget_Set(const leString* str);
void _leEditWidget_Append(const leString* str);
void _leEditWidget_Backspace(void);
// DOM-IGNORE-END

#endif // LEGATO_EDITWIDGET_H
