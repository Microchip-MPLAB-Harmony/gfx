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
    legato_widget_drawsurface.h

  Summary:


  Description:
    This module implements surface container drawing functions.
*******************************************************************************/

/** \file legato_widget_drawsurface.h
 * @brief Surface widget functions and definitions.
 *
 * @details This module implements surface container drawing functions.
 */

#ifndef LEGATO_DRAWSURFACE_H
#define LEGATO_DRAWSURFACE_H

#include "gfx/legato/common/legato_common.h"

#if LE_DRAWSURFACE_WIDGET_ENABLED

#include "gfx/legato/widget/legato_widget.h"

typedef struct leDrawSurfaceWidget leDrawSurfaceWidget;

// *****************************************************************************
/* Function Pointer:
    leDrawSurfaceWidget_DrawCallback

  Summary:
    Draw surface draw event function callback type
*/
/**
 * @brief This function represents a draw callback.
 * @details Draw callback is used indicate a draw changed event.
 * @details .
 */
typedef leBool (*leDrawSurfaceWidget_DrawCallback)(leDrawSurfaceWidget* sfc,
                                                   leRect* bounds);

// *****************************************************************************
// *****************************************************************************
// Section: Data Types and Constants
// *****************************************************************************
// *****************************************************************************

/* internal use only */
/**
  * @cond INTERNAL
  *
  */
typedef struct leDrawSurfaceWidget leDrawSurfaceWidget;

#define LE_DRAWSURFACEWIDGET_VTABLE(THIS_TYPE) \
    LE_WIDGET_VTABLE(THIS_TYPE) \
    \
    leDrawSurfaceWidget_DrawCallback (*getDrawCallback)(const THIS_TYPE* _this); \
    leResult  (*setDrawCallback)(THIS_TYPE* _this, leDrawSurfaceWidget_DrawCallback cb); \
    
typedef struct leDrawSurfaceWidgetVTable
{
	LE_DRAWSURFACEWIDGET_VTABLE(leDrawSurfaceWidget)
} leDrawSurfaceWidgetVTable; 

/**
  * @endcond
  *
  */

// *****************************************************************************
/* Structure:
    leDrawSurfaceWidget

  Summary:
    Implementation of a Drawsurface widget.

  Description:
    A draw surface widget is a special widget that allows an application to
    perform custom HAL draw calls during Legato's paint loop.  To use, create
    and add a draw surface widget to the desired place in the widget tree.
    Then register for the callback through the API
    'leDrawSurfaceWidget_SetDrawCallback'.  This callback occurs during the
    paint loop.  The application should then be free to adjust the HAL
    draw state and issue draw calls as desired.  The HAL layer, buffer, or
    context state must not be adjusted in any way.

    It is also important to not stall for too long during the draw callback.

  Remarks:
    None.
*/

/**
  * @brief This struct represents a draw surface widget.
  * @details A draw surface widget is a special widget that allows an
  * application to perform custom HAL draw calls during Legato's paint loop.
  * To use, create and add a draw surface widget to the desired place in the
  * widget tree. Then register for the callback through the API
  * 'leDrawSurfaceWidget_SetDrawCallback'.  This callback occurs during the
  * paint loop.  The application should then be free to adjust the HAL  draw
  * state and issue draw calls as desired.  The HAL layer, buffer, or
  *  context state must not be adjusted in any way.
  */
typedef struct leDrawSurfaceWidget
{
    leWidget widget; // the widget base class

    const leDrawSurfaceWidgetVTable* fn;

    leDrawSurfaceWidget_DrawCallback cb; // the draw callback
} leDrawSurfaceWidget;

// *****************************************************************************
// *****************************************************************************
// Section: Routines
// *****************************************************************************
// *****************************************************************************

/**
 * @brief Create widget.
 * @details Creates a new leDrawSurfaceWidget and allocates memory for the widget through the
 * current active context.  Application is responsible for managing the widget
 * pointer until the widget is added to a widget tree.
 * @remark use leWidget_Delete() to free this pointer.
 * @code
 * leDrawSurfaceWidget* wgt = leDrawSurfaceWidget_New();
 * @endcode
 * @return a widget object pointer.
 */
leDrawSurfaceWidget* leDrawSurfaceWidget_New(void);

/**
 * @brief Initialize widget.
 * @details Initializes the leDrawSurfaceWidget <span class="param">wgt</span>.
 * @code
 * leDrawSurfaceWidget* wgt;
 * leDrawSurfaceWidget_Constructor(wgt);
 * @endcode
 * @param wgt is the widget to initialize
 * @return void.
 */
void leDrawSurfaceWidget_Constructor(leDrawSurfaceWidget* sfc);
// *****************************************************************************

#ifdef _DOXYGEN_
#define THIS_TYPE struct leWidget

// *****************************************************************************
/**
 * @brief Get draw callback pointer.
 * @details Gets the draw callback pointer using <span class="param">_this</span>.
 * @code
 * leDrawSurfaceWidget* _this;
 * leDrawSurfaceWidget_DrawCallback cb = _this->fn->getDrawCallback(_this);
 * @endcode
 * @param _this is the widget to modify
 * @returns the callback pointer.
 */
virtual leDrawSurfaceWidget_DrawCallback getDrawCallback(const leDrawSurfaceWidget* _this);

// *****************************************************************************
/**
 * @brief Set draw callback pointer.
 * @details Sets the draw callback pointer to <span class="param">cb</span>
 * using <span class="param">_this</span>.
 * @code
 * leDrawSurfaceWidget* _this;
 * leDrawSurfaceWidget_DrawCallback cb;
 * leResult res = _this->fn->setDrawCallback(_this, cb);
 * @endcode
 * @param _this is the widget to modify
 * @param cb is the callback func
 * @returns LE_SUCCESS if set, otherwise LE_FAILURE.
 */
virtual leResult setDrawCallback(leDrawSurfaceWidget* _this,
                                 leDrawSurfaceWidget_DrawCallback cb);

#undef THIS_TYPE
#endif

#endif // LE_DRAWSURFACE_WIDGET_ENABLED
#endif /* LEGATO_DRAWSURFACE_H */
