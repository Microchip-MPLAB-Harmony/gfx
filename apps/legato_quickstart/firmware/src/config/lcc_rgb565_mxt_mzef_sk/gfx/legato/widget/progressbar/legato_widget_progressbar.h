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
    legato_widget_progressbar.h

  Summary:


  Description:
    This module implements progress bar widget functions.
*******************************************************************************/

// DOM-IGNORE-BEGIN

#ifndef LEGATO_PROGRESSBAR_H
#define LEGATO_PROGRESSBAR_H
//DOM-IGNORE-END

#include "gfx/legato/common/legato_common.h"

#if LE_PROGRESSBAR_WIDGET_ENABLED == 1

#include "gfx/legato/widget/legato_widget.h"

// *****************************************************************************
// *****************************************************************************
// Section: Data Types and Constants
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/* Enumeration:
    leProgressBarDirection

  Summary:
    Defines the valid values for the progress bar widget fill directions.

  Description:


  Remarks:
    None.
*/
typedef enum leProgressBarDirection
{
    LE_PROGRESSBAR_DIRECTION_RIGHT,
    LE_PROGRESSBAR_DIRECTION_UP,
    LE_PROGRESSBAR_DIRECTION_LEFT,
    LE_PROGRESSBAR_DIRECTION_DOWN
} leProgressBarDirection;

// DOM-IGNORE-BEGIN
typedef struct leProgressBarWidget leProgressBar;
// DOM-IGNORE-END

// *****************************************************************************
/* Function Pointer:
    leProgressBar_ValueChangedEventCallback

  Summary:
    Value changed event function callback type
*/
typedef void (*leProgressBar_ValueChangedEventCallback)(leProgressBar*, uint32_t);

// DOM-IGNORE-BEGIN
typedef struct leProgressBarWidget leProgressBarWidget;

#define LE_PROGRESSBARWIDGET_VTABLE(THIS_TYPE) \
    LE_WIDGET_VTABLE(THIS_TYPE) \
    \
    leProgressBarDirection (*getDirection)(const THIS_TYPE* _this); \
    leResult               (*setDirection)(THIS_TYPE* _this, leProgressBarDirection dir); \
    uint32_t               (*getValue)(const THIS_TYPE* _this); \
    leResult               (*setValue)(THIS_TYPE* _this, uint32_t value); \
    leProgressBar_ValueChangedEventCallback (*getValueChangedEventCallback)(const THIS_TYPE* _this); \
    leResult               (*setValueChangedCallback)(THIS_TYPE* _this, leProgressBar_ValueChangedEventCallback cb); \
    
typedef struct leProgressBarWidgetVTable
{
	LE_PROGRESSBARWIDGET_VTABLE(leProgressBarWidget)
} leProgressBarWidgetVTable; 

// DOM-IGNORE-END

// *****************************************************************************
/* Structure:
    leProgressBarDirection

  Summary:
    Implementation of a progressbar widget struct

  Description:
    A progress bar widget is a widget that can fill itself with a color based
    on a given percentage from 0-100.  This is often used to visually illustrate
    the progress of some other activity over time.

  Remarks:
    None.
*/
typedef struct leProgressBarWidget
{
    leWidget widget; // base widget class

    const leProgressBarWidgetVTable* fn;

    leProgressBarDirection direction; // the fill direction of the bar

    uint32_t value; // fill percentage

    leProgressBar_ValueChangedEventCallback cb; // value changed callback
} leProgressBarWidget;


// *****************************************************************************
// *****************************************************************************
// Section: Routines
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/* Function:
    leProgressBarWidget* leProgressBarWidget_New()

  Summary:
    Allocates memory for a new widget of this type.  The application is
    responsible for the managment of this memory until the widget is added to
    a widget tree.

  Description:


  Parameters:

  Returns:
    leProgressBarWidget*

  Remarks:
    Use leWidget_Delete() to free this pointer.
*/
LIB_EXPORT leProgressBarWidget* leProgressBarWidget_New();

/* Function:
    void leProgressBarWidget_Constructor(leProgressBarWidget* wgt)

  Summary:
    Initializes an leProgressBarWidget widget pointer.

  Description:
    Initializes an leProgressBarWidget widget pointer.

  Parameters:
    leProgressBarWidget* wgt - the pointer to initialize

  Returns:
    void

  Remarks:

*/
LIB_EXPORT void leProgressBarWidget_Constructor(leProgressBarWidget* bar);

// *****************************************************************************
/* Virtual Member Function:
    leProgressBarDirection getDirection(const leProgressBarWidget* _this)

  Summary:
     Gets the progress bar fill direction

  Description:
     Gets the progress bar fill direction

  Parameters:
    const leProgressBarWidget* _this - The progress bar widget to operate on

  Remarks:
    Usage - _this->fn->getDirection(_this);

  Returns:
    leProgressBarDirection - the direction
*/

// *****************************************************************************
/* Virtual Member Function:
    leResult setDirection(leProgressBarWidget* _this,
                          leProgressBarDirection dir)

  Summary:
     Sets the progress bar fill direction

  Description:
     Sets the progress bar fill direction

  Parameters:
    leProgressBarWidget* _this - The progress bar widget to operate on
    leProgressBarDirection dir - the direction

  Remarks:
    Usage - _this->fn->setDirection(_this, dir);

  Returns:
    leResult - the result of the operation
*/

// *****************************************************************************
/* Virtual Member Function:
    uint32_t getValue(const leProgressBarWidget* _this)

  Summary:
     Gets the progress bar value

  Description:
     Gets the progress bar value

  Parameters:
    const leProgressBarWidget* _this - The progress bar widget to operate on

  Remarks:
    Usage - _this->fn->getValue(_this);

  Returns:
    uint32_t - the value
*/

// *****************************************************************************
/* Virtual Member Function:
    leResult setValue(leProgressBarWidget* _this,
                      uint32_t value)

  Summary:
     Sets the progress bar value

  Description:
     Sets the progress bar value

  Parameters:
    leProgressBarWidget* _this - The progress bar widget to operate on
    uint32_t value - the value

  Remarks:
    Usage - _this->fn->setValue(_this, value);

  Returns:
    leResult - the result of the operation
*/

// *****************************************************************************
/* Virtual Member Function:
    leProgressBar_ValueChangedEventCallback getValueChangedEventCallback(const leProgressBarWidget* _this)

  Summary:
     Gets the progress bar value changed event callback pointer

  Description:
     Gets the progress bar value changed event callback pointer

  Parameters:
    const leProgressBarWidget* _this - The progress bar widget to operate on

  Remarks:
    Usage - _this->fn->getValueChangedEventCallback(_this);

  Returns:
    leProgressBar_ValueChangedEventCallback - the value
*/

// *****************************************************************************
/* Virtual Member Function:
    leResult setValueChangedCallback(leProgressBarWidget* _this,
                                     leProgressBar_ValueChangedEventCallback cb)

  Summary:
     Sets the progress bar value changed event callback pointer

  Description:
     Sets the progress bar value changed event callback pointer

  Parameters:
    leProgressBarWidget* _this - The progress bar widget to operate on
    leProgressBar_ValueChangedEventCallback cb - the callback pointer

  Remarks:
    Usage - _this->fn->setValueChangedCallback(_this, cb);

  Returns:
    leResult - the result of the operation
*/



#endif // LE_PROGRESSBAR_WIDGET_ENABLED
#endif /* LEGATO_PROGRESSBAR_H */
