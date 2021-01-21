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

/** \file legato_state.h
 * @brief .
 *
 * @details .
 */

#ifndef LEGATO_STATE_H
#define LEGATO_STATE_H


#include "gfx/legato/common/legato_common.h"

#include "gfx/driver/gfx_driver.h"
#include "gfx/legato/core/legato_input.h"
#include "gfx/legato/core/legato_scheme.h"
#include "gfx/legato/string/legato_stringtable.h"
#include "gfx/legato/widget/legato_editwidget.h" 

// *****************************************************************************
/**
 * @brief This function pointer represents a language change callback.
 * @details A language change callback is called when
 * when the language change event occurs.
 */
typedef void (*leLanguageChangedCallback_FnPtr)(uint32_t);

// *****************************************************************************
/**
 * @brief This struct describes the layer state for a layer.
 * @details This struct contains useful information about a layer such as color mode,
 * rendering options, etc.
 */
typedef struct
{
    leColorMode colorMode;
    leBool renderHorizontal;
    lePoint driverPosition;
} leLayerState;

// *****************************************************************************
/**
 * @brief This struct represents the state of the library.
 * @details The state of the libary is a global structure.
 */
typedef struct leState
{
    const leStringTable* stringTable; /**< the string table for the instance */
    uint32_t languageID; /**< the currently active language */
    
    uint32_t widgetIDs; /**< the next unique widget ID */
    
    leScheme defaultScheme; /**< an internal default scheme that widgets use by */
                            /**< default if the user doesn't set one */
                            
    leWidget* focus; /**< the widget that currently has focus */
    leEditWidget* edit; /**< the widget that is currently receiving edit events */
    
    leLanguageChangedCallback_FnPtr languageChangedCB; /**< language changed callback */

    leWidget rootWidget[LE_LAYER_COUNT]; /**< root widgets of the scene */
    leLayerState layerStates[LE_LAYER_COUNT]; /**< layer states */

#if LE_STREAMING_ENABLED == 1
    leStreamManager* activeStream; /**< active stream state */
#endif
} leState;

/* internal use only */
/**
  * @cond INTERNAL
  */
leState* leGetState(void);
/**
  * @endcond
  *
  */

// *****************************************************************************
/**
 * @brief Initialize Legato library.
 * @details This function performs basic initialization of the Legato
 * library state.
 * It accepts and intializes a display controller driver specified
 * by <span class="param">dispDriver</span>
 * @pre Must be called before any other library operations are performed.
 * @code
 * leResult res = leInitialize(dispDriver);
 * @endcode
 * @param dispDriver is the display controller driver.
 * @returns LE_SUCCESS if set, otherwise LE_FAILURE.
 */
leResult leInitialize(const gfxDisplayDriver* dispDriver,
                      const gfxGraphicsProcessor* gpuDriver);

// *****************************************************************************
/**
 * @brief Shutdown Legato library.
 * @details Shutdowns the active Legato library state.
 * @code
 * leShutdown();
 * @endcode
 * @return void.
 */
void leShutdown(void);

// *****************************************************************************
/**
 * @brief Shutdown Legato library.
 * @details Shutdown the Legato library in <span class="param">dt</span>
 * milliseconds after the last leUpdate() was called. If the value
 * of dt is zero then time-dependent features will not advance.
 * the active Legato library state.
 * This function updates the active Legato library context state.
 * It performs event processing as well as servicing of the widget paint loop.
 * It should be called often.
 * @code
 * uint32_t dt=2000;
 * leResult res = leUpdateleShutdown(dt);
 * @endcode
 * @param dt is a delta time.
 * @returns LE_SUCCESS if set, otherwise LE_FAILURE.
 */
leResult leUpdate(uint32_t dt);

// *****************************************************************************
/**
 * @brief Get color mode.
 * @details Gets the the color mode of the current context.
 * @code
 * leColorMode mode = leGetColorMode();
 * @endcode
 * @param void.
 * @return the color mode of the current context.
 */
leColorMode leGetColorMode(void);
    
/**
 * @brief Get layer color mode.
 * @details Gets the the layer color mode
 * @code
 * leColorMode mode = leGetLayerColorMode(0);
 * @endcode
 * @param lyrIdx is the index of the layer to query
 * @return the color mode of layer.
 */
leColorMode leGetLayerColorMode(uint32_t lyrIdx);

/**
 * @brief Set layer color mode.
 * @details Sets the the layer color mode at index
 * @code
 * leResult res = leGetLayerColorMode(0, LE_RGB_565);
 * @endcode
 * @param lyrIdx is the index of the layer to query
 * @param mode is the color mode to set
 * @return result.
 */
leResult leSetLayerColorMode(uint32_t lyrIdx, leColorMode mode);

/**
 * @brief Get layer render direction.
 * @details Gets the the layer render direction
 * @code
 * bool horz = leGetLayerRenderHorizontal(0);
 * @endcode
 * @param lyrIdx is the index of the layer to query
 * @return true if the layer is set to render horizontally.
 */
leBool leGetLayerRenderHorizontal(uint32_t lyrIdx);

/**
 * @brief Set layer render direction.
 * @details Sets the the layer render direction at index.  By default a layer
 * is rendered from top-down with dirty rectangles sliced favoring height.  In
 * horizontal mode layers are rendered from left to right with rectangles
 * sliced favoring width.  This can help reduce tearing for scenes that have
 * horizontal movement.
 * @code
 * leResult res = leSetLayerRenderHorizontal(0, true);
 * @endcode
 * @param lyrIdx is the index of the layer to modify
 * @param true if the layer should use horizontal mode
 * @return result.
 */
leResult leSetLayerRenderHorizontal(uint32_t lyrIdx, leBool horz);

#if 0
// *****************************************************************************
/**
 * @brief Get display rectangle.
 * @details Gets the display rectangles width height and upper left
 * corner coordinates of the physical display.
 * @code
 * leRect rect = leGetDisplayRect();
 * @endcode
 * @return the display rectangle.
 */
leRect leGetDisplayRect();
#endif

// *****************************************************************************
/**
 * @brief Get string table.
 * @details Get a pointer to the leStringTable structure that
 * maintains the strings, associated fonts, etc.
 * @code
 * leStringTable* tbl = leGetStringTable();
 * @endcode
 * @param void.
 * @return leStringTable pointer.
 */
leStringTable* leGetStringTable(void);

// *****************************************************************************
/* Function:
    void leSetStringTable(leStringTable* table)

   Summary:
    Set the StringTable pointer to the specified new StringTableAsset structure  

   Description:
    Set the StringTable pointer to the specified new StringTableAsset structure 

   Precondition:

   Parameters:
    leStringTable*

  Returns:
    void

  Remarks:    
    
*/
/**
 * @brief Set string table.
 * @details Set string table to <span class="param">table</span>.
 * @code
 * leStringTable* table
 * leSetStringTable(table);
 * @endcode
 * @param table is pointer to a string table.
 * @return void.
 */
void leSetStringTable(const leStringTable* table);

// *****************************************************************************
/* Function:
    uint32_t leGetStringLanguage()

   Summary:
    Returns the language index of the current context  

   Description:
    Returns the language index of the current context

   Precondition:

   Parameters:
    
  Returns:
    uint32_t

  Remarks:    
    
*/
/**
 * @brief Get language index.
 * @details Returns the language index of the current context.
 * @code
 * uint32_t idx = leGetStringLanguage();
 * @endcode
 * @param void.
 * @return void.
 */
uint32_t leGetStringLanguage(void);

// *****************************************************************************
/* Function:
    void leSetStringLanguage(uint32_t id)

   Summary:
    Sets the current language index 

   Description:
    Sets the current language index

   Precondition:

   Parameters:
    uint32_t

  Returns:
    void

  Remarks:    
    
*/
/**
 * @brief Get language by index.
 * @details Returns the language at the location specified by <span class="param">idx</span>.
 * @code
 * uint32_t id;
 * leSetStringLanguage(id);
 * @endcode
 * @param void.
 * @return void.
 */
void leSetStringLanguage(uint32_t id);

// *****************************************************************************
/* Function:
    leScheme* leGetDefaultScheme()

   Summary:
    Returns the pointer to the current default scheme  

   Description:
    Returns the pointer to the current default scheme

   Precondition:

   Parameters:
    
  Returns:
    leScheme*

  Remarks:    
    
*/
/**
 * @brief Get default scheme.
 * @details Returns the pointer to the current default scheme.
 * @code
 * leScheme* scheme = leGetDefaultScheme();
 * @endcode
 * @param void.
 * @return void.
 */
leScheme* leGetDefaultScheme(void);

// *****************************************************************************
/* Function:
    leWidget* leGetFocusWidget()

   Summary:
    Return a pointer to the widget in focus 

   Description:
    The focus widget is the widget that is currently receiving all input events.
    This can happen when the user initiates a touch down event on the widget
    and is currently dragging their finger on the display.  The widget will
    receive all touch moved events until a touch up event is received.

   Precondition:

   Parameters:
    
  Returns:
    leWidget*

  Remarks:    
    
*/
/**
 * @brief Get focus widget.
 * @details The focus widget is the widget that is currently receiving all input
 * events. This can happen when the user initiates a touch down event on the widget
 * and is currently dragging their finger on the display.  The widget will
 * receive all touch moved events until a touch up event is received.
 * @code
 * leWidget* widget = leGetFocusWidget();
 * @endcode
 * @param void.
 * @return void.
 */
leWidget* leGetFocusWidget(void);

// *****************************************************************************
/* Function:
    leResult leSetFocusWidget(leWidget* widget)

   Summary:
    Set into focus the widget specified as the argument 

   Description:
    Set into focus the widget specified as the argument

   Precondition:

   Parameters:
    leWidget*

  Returns:
    leResult

  Remarks:    
    
*/
/**
 * @brief Get language by index.
 * @details Sets the focus widget to <span class="param">widget</span>.
 * @code
 * uint32_t id;
 * leResult res = leSetFocusWidget(id);
 * @endcode
 * @param void.
 * @return void.
 */
leResult leSetFocusWidget(leWidget* widget);

// *****************************************************************************
/* Function:
    leEditWidget* leGetEditWidget()

   Summary:
    Gets the widget that is currently recieving all widget edit events.

   Description:
    Edit widgets are widgets that inherit the 'edit widget' API function list.
    These widgets are capable of receiving edit events from other widgets
    that are edit event broadcasters.  A broadcaster could be a 'key pad' and
    a receiver could be a 'text edit' box.

   Precondition:

   Parameters:
    void

  Returns:
    leEditWidget*

  Remarks:    
    
*/
/**
 * @brief Get edit widget.
 * @details Edit widgets are widgets that inherit the 'edit widget' API function
 * list. These widgets are capable of receiving edit events from other widgets
 * that are edit event broadcasters.  A broadcaster could be a 'key pad' and a
 * receiver could be a 'text edit' box.
 * @code
 * leEditWidget* widget = leGetEditWidget();
 * @endcode
 * @param void.
 * @return leEditWidget pointer.
 */
leEditWidget* leGetEditWidget(void);

// *****************************************************************************
/* Function:
    leResult leSetEditWidget(leEditWidget* widget)

   Summary:
    Sets the currently active edit widget.  

   Precondition:

   Parameters:
    leEditWidget* - a widget that inherits the edit widget API

  Returns:
    leResult

  Remarks:    
    
*/
/**
 * @brief Set edit widget.
 * @details Sets the active edit widget to <span class="param">widget</span>. Edit widgets are widgets that inherit the 'edit widget' API function
 * list. These widgets are capable of receiving edit events from other widgets
 * that are edit event broadcasters.  A broadcaster could be a 'key pad' and a
 * receiver could be a 'text edit' box.
 * @code
 * leEditWidget* widget = leSetEditWidget(widget);
 * @endcode
 * @param void.
 * @return leEditWidget pointer.
 */
leResult leSetEditWidget(leEditWidget* widget);
    
// *****************************************************************************
/* Function:
    leResult leSetLanguageChangedCallback(leLanguageChangedCallback_FnPtr cb)

   Summary:
    Set the callback function pointer when the language change event occurs  

   Description:
    Set the callback function pointer when the language change event occurs

   Precondition:

   Parameters:
    leLanguageChangedCallback_FnPtr

  Returns:
    leResult

  Remarks:    
    
*/
/**
 * @brief Set language change callback.
 * @details Sets the callback function to <span class="param">cb</span>
 * to handle the language change event.
 * @code
 * leLanguageChangedCallback_FnPtr cb;
 * leResult res = leSetLanguageChangedCallback(cb);
 * @endcode
 * @param cb is the new callback function.
 * @return leResult.
 */
leResult leSetLanguageChangedCallback(leLanguageChangedCallback_FnPtr cb);
 
// *****************************************************************************
/* Function:
    void leRedrawAll()

   Summary:
    Forces the library to redraw the currently active scene in its entirety.  

   Precondition:

   Parameters:
    
  Returns:
    void

  Remarks:    
    
*/
/**
 * @brief Redraw All.
 * @details Forces the library to redraw the currently active scene in its entirety.
 * @code
 * leRedrawAll();
 * @endcode
 * @param void.
 * @return void.
 */
void leRedrawAll(void);

/*  Function:
        leBool leIsDrawing()
 
    Summary:
        Indicates if any layers of the active screen are currently drawing a
        frame.
        
    Description:
        Indicates if the library currently drawing a frame.  Because frame updates
        can asynchronous to making changes to the UI state it is best to only make 
        updates to the state of a widget only when the layer is not drawing.
        
    Returns:
        leResult    

*/
/**
 * @brief Check active screen drawing state.
 * @details Indicates if the library currently drawing a frame.  Because frame
 * updates can asynchronous to making changes to the UI state it is best to only
 * make updates to the state of a widget only when the layer is not drawing.
 * @code
 * leBool flag = leIsDrawing();
 * @endcode
 * @param void.
 * @return true.
 */
leBool leIsDrawing(void);

/*  Function:
        leResult leAddRootWidget(leWidget* wgt, uint32_t layer)

    Summary:
        Adds a custom widget to a static scene root widget.

    Description:
        The library maintains a static list of widgets that are considered
        to be scene roots.  The library treats these roots as layers and
        the display driver may configure itself to render these roots
        to different hardware layers.

        This API adds a child widget to one of the static roots.

    Returns:
        leResult

*/
/**
 * @brief Add root widget to layer.
 * @details Adds the root widget <span class="param">wgt</span>
 * to the layer <span class="param">layer</span>.
 * The library maintains a static list of widgets that are considered
 * to be scene roots.  The library treats these roots as layers and
 * the display driver may configure itself to render these roots
 * to different hardware layers.
 * This API adds a child widget to one of the static roots.
 * @code
 * leResult res = leAddRootWidget(wgt, layer);
 * @endcode
 * @param void.
 * @return leResult.
 */
leResult leAddRootWidget(leWidget* wgt,
                         uint32_t layer);

/*  Function:
        leResult leRemoveRootWidget(leWidget* wgt, uint32_t layer)

    Summary:
        Removes a custom widget from a static scene root widget.

    Description:
        The library maintains a static list of widgets that are considered
        to be scene roots.  This API removes a child widget from one of the
        static roots.

    Returns:
        leResult

*/
/**
 * @brief Remove root widget to layer.
 * @details Remove the root widget <span class="param">wgt</span>
 * from the layer <span class="param">layer</span>.
 * The library maintains a static list of widgets that are considered
 * to be scene roots.  The library treats these roots as layers and
 * the display driver may configure itself to render these roots
 * to different hardware layers.
 * This API adds a child widget to one of the static roots.
 * @code
 * leResult res = leRemoveRootWidget(wgt, layer);
 * @endcode
 * @param void.
 * @return leResult.
 */
leResult leRemoveRootWidget(leWidget* wgt,
                            uint32_t layer);

/*  Function:
        leBool leWidgetIsInScene(const leWidget* wgt)

    Summary:
        Searches the active scene for a given widget.

    Description:
        Searches the active scene for a given widget.

    Returns:
        leResult - LE_TRUE if the widget is in the current scene

*/
/**
 * @brief Determines whether the widget is in the scene.
 * @details Determines if <span class="param">wgt</span>
 * is in he current scheme.
 * Searches the active scene for a given widget.
 * @code
 * leBool flag = leWidgetIsInScene(wgt);
 * @endcode
 * @param void.
 * @returns LE_SUCCESS if set, otherwise LE_FAILURE.
 */
leBool leWidgetIsInScene(const leWidget* wgt);

int32_t leGetWidgetLayer(const leWidget* wgt);


/*  Function:
        leResult leEdit_StartEdit(const leWidget* wgt)

    Summary:
        Passes a 'start edit' command to the current edit widget,
        if one exists

    Description:
        The edit widget is the widget in the scene that is currently receiving
        edit events.

    Returns:
        leResult - LE_TRUE the command was processed successfully

*/
/**
 * @brief Send start edit command.
 * @details Passes a 'start edit' command to the current edit widget, if one exists.
 * @code
 * leResult res = leEdit_StartEdit();
 * @endcode
 * @param void.
 * @returns LE_SUCCESS if set, otherwise LE_FAILURE.
 */
leResult leEdit_StartEdit(void);

/**
 * @brief Send end edit command.
 * @details Sends an 'end edit' command to the current edit widget, if one exists.
 * @code
 * leEdit_EndEdit();
 * @endcode
 * @return void.
 */
void leEdit_EndEdit(void);

/**
 * @brief Send clear command.
 * @details Sends a 'clear' command to the current edit widget, if one exists.
 * @code
 * leEdit_Clear();
 * @endcode
 * @return void.
 */
void leEdit_Clear(void);

/**
 * @brief Send accept command.
 * @details Passes a 'accept' command to the current edit widget, if one exists.
 * @code
 * leEdit_Accept();
 * @endcode
 * @return void.
 */
void leEdit_Accept(void);

/*  Function:
        leResult leEdit_Accept(const leWidget* wgt)

    Summary:
        Passes a 'set' command and a string to the current edit widget,
        if one exists

    Description:
        Passes a 'set' command and a string to the current edit widget,
        if one exists

    Returns:

*/
/**
 * @brief Send set command.
 * @details Passes a 'set' command and string <span class="param">str</span> to the current edit widget, if one exists.
 * @code
 * leEdit_Set(str);
 * @endcode
 * @param void.
 * @return leResult.
 */
void leEdit_Set(leString* str);

/*  Function:
        leResult leEdit_Append(const leWidget* wgt)

    Summary:
        Passes an 'append' command and a string to the current edit widget,
        if one exists

    Description:
        Passes an 'append' command and a string to the current edit widget,
        if one exists

    Returns:

*/
/**
 * @brief Send start edit command.
 * @details Passes a 'append' command and string <span class="param">str</span> to the current edit widget, if one exists.
 * @code
 * leString* str;
 * leEdit_Append(str);
 * @endcode
 * @param void.
 * @return void.
 */
void leEdit_Append(leString* str);

/*  Function:
        leResult leEdit_Append(const leWidget* wgt)

    Summary:
        Passes a 'backspace' command to the current edit widget,
        if one exists

    Description:
        Passes a 'backspace' command to the current edit widget,
        if one exists

    Returns:

*/
/**
 * @brief Send backspace command.
 * @details Passes a 'backspace' command to the current edit widget, if one exists.
 * @code
 * leEdit_Backspace();
 * @endcode
 * @param void.
 * @return void.
 */
void leEdit_Backspace(void);

#if LE_STREAMING_ENABLED == 1
/*  Function:
        leStreamManager* leGetActiveStream()

    Summary:
        Gets the global active data stream manager for the library.

    Description:
        A data stream manager is a state machine that is currently
        attempting to stream data from a source that is external
        to the library and possibly local memory.  This stream must
        be handled to completion before any further rendering can take place.

    Returns:

*/
leStreamManager* leGetActiveStream(void);

/*  Function:
        leResult leRunActiveStream()

    Summary:
        Allows the current active data stream to run until it preempts or
        completes.

    Description:
        Allows the current active data stream to run until it preempts or
        completes.

    Returns:

*/
leResult leRunActiveStream(void);

/*  Function:
        void leAbortActiveStream

    Summary:
        Terminates the current active data stream regardless of whether
        it still has data to receive.

    Description:
        Terminates the current active data stream regardless of whether
        it still has data to receive.  It will likely attempt to clean itself up
        as best it can.  Care should be taken in the application to not send data
        to the stream after it has been aborted.

    Returns:

*/
void leAbortActiveStream(void);
#endif

#endif /* LEGATO_STATE_H */
