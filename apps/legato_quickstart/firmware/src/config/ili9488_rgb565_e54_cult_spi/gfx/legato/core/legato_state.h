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

#ifndef LEGATO_STATE_H
#define LEGATO_STATE_H

// DOM-IGNORE-BEGIN

#include "gfx/legato/common/legato_common.h"

#include "gfx/driver/gfx_driver.h"
#include "gfx/legato/core/legato_input.h"
#include "gfx/legato/core/legato_scheme.h"
#include "gfx/legato/string/legato_stringtable.h"
#include "gfx/legato/widget/legato_editwidget.h" 

// *****************************************************************************
/* Type:
    leLanguageChangedCallback_FnPtr

  Summary:
    Callback pointer for when the language change event occurs.

  Description:
    Callback pointer for when the language change event occurs.

  Remarks:

*/
typedef void (*leLanguageChangedCallback_FnPtr)(uint32_t);


// *****************************************************************************
/* Structure:
    struct leState

  Summary:
    The global structure that holds the state of the library

  Description:
    The global structure that holds the state of the library

  Remarks:

*/
typedef struct leState
{
    const leStringTable* stringTable; // the string table for the instance
    uint32_t languageID; // the currently active language
    
    uint32_t widgetIDs; // the next unique widget ID
    
    leScheme defaultScheme; // an internal default scheme that widgets use by
                            // default if the user doesn't set one
                            
    leWidget* focus; // the widget that currently has focus
    leEditWidget* edit; // the widget that is currently receiving edit events
    
    leLanguageChangedCallback_FnPtr languageChangedCB; // language changed callback

    leWidget rootWidget[LE_LAYER_COUNT]; // root widgets of the scene

#if LE_STREAMING_ENABLED == 1
    leStreamManager* activeStream; // active stream state
#endif
} leState;

/* internal use only */
LIB_EXPORT leState* leGetState();
// DOM-IGNORE-END

// *****************************************************************************
/* Type:
    leResult leInitialize()

  Summary:
    Function to perform basic initialization of the Legato library state.

  Description:
    Function to perform basic initialization of the Legato library state.  Must
    be called before any other library operations are performed.

  Remarks:
    
  Returns;
    leResult - the result of the initialization operation
*/
LIB_EXPORT leResult leInitialize(const gfxDisplayDriver* dispDriver);

// *****************************************************************************
/* Type:
    void leShutdown()

  Summary:
    Function to shutdown the active Legato library state.

  Description:
    Function to shutdown the active Legato library state.

  Remarks:
*/
LIB_EXPORT void leShutdown();

// *****************************************************************************
/* Type:
    void leUpdate(uint32_t dt)

  Summary:
    Legato library update (tasks) function.

  Description:
    This function updates the active Legato library context state.  It performs
    event processing as well as servicing of the widget paint loop.  It should
    be called often.

  Parameters:
    uint32_t dt - a delta time, typically expressed in milliseconds, since the
                  last time leUpdate was called.  If this value is zero then
                  time-dependent features will not advance.
*/
LIB_EXPORT leResult leUpdate(uint32_t dt);

// *****************************************************************************
/* Function:
    leColorMode leGetColorMode()
    
   Summary:
    Returns the color mode of the current context

   Returns:
    leColorMode
*/
LIB_EXPORT leColorMode leGetColorMode();
    
// *****************************************************************************
/* Function:
    LIB_EXPORT leRect leGetDisplayRect()

   Summary:
    Returns the display rectangle structure of the physical display

   Description:
    Returns the display rectangle - width height and upper left corner coordinates
    of the physical display

   Precondition:

   Parameters:
    
  Returns:
    leRect

  Remarks:    
    
*/
LIB_EXPORT leRect leGetDisplayRect();
    
// *****************************************************************************
/* Function:
    leStringTable* leGetStringTable()

   Summary:
    Get a pointer to the leStringTable structure that maintains the strings,
    associated fonts, etc

   Description:
    Get a pointer to the leStringTable structure that maintains the strings,
    associated fonts, etc

   Precondition:

   Parameters:
    
  Returns:
    leStringTable*

  Remarks:    
    
*/
LIB_EXPORT leStringTable* leGetStringTable();

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
LIB_EXPORT void leSetStringTable(const leStringTable* table);

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
LIB_EXPORT uint32_t leGetStringLanguage();

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
LIB_EXPORT void leSetStringLanguage(uint32_t id);

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
LIB_EXPORT leScheme* leGetDefaultScheme();

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
LIB_EXPORT leWidget* leGetFocusWidget();

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
LIB_EXPORT leResult leSetFocusWidget(leWidget* widget);

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
LIB_EXPORT leEditWidget* leGetEditWidget();

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
LIB_EXPORT leResult leSetEditWidget(leEditWidget* widget);
    
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
LIB_EXPORT leResult leSetLanguageChangedCallback(leLanguageChangedCallback_FnPtr cb);
 
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
LIB_EXPORT void leRedrawAll();

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
LIB_EXPORT leBool leIsDrawing();

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
LIB_EXPORT leResult leAddRootWidget(leWidget* wgt,
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
LIB_EXPORT leResult leRemoveRootWidget(leWidget* wgt,
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
LIB_EXPORT leBool leWidgetIsInScene(const leWidget* wgt);


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
leResult leEdit_StartEdit();

/*  Function:
        leResult leEdit_EndEdit(const leWidget* wgt)

    Summary:
        Passes an 'end edit' command to the current edit widget,
        if one exists

    Description:
        Passes an 'end edit' command to the current edit widget,
        if one exists

    Returns:
        leResult - LE_TRUE the command was processed successfully

*/
void leEdit_EndEdit();

/*  Function:
        leResult leEdit_Clear(const leWidget* wgt)

    Summary:
        Passes a 'clear' command to the current edit widget,
        if one exists

    Description:
        Passes a 'clear' command to the current edit widget,
        if one exists

    Returns:


*/
void leEdit_Clear();

/*  Function:
        leResult leEdit_Accept(const leWidget* wgt)

    Summary:
        Passes an 'accept' command to the current edit widget,
        if one exists

    Description:
        Passes an 'accept' command to the current edit widget,
        if one exists

    Returns:


*/
void leEdit_Accept();

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
void leEdit_Backspace();

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
leStreamManager* leGetActiveStream();

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
leResult leRunActiveStream();

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
void leAbortActiveStream();
#endif

#endif /* LEGATO_STATE_H */
