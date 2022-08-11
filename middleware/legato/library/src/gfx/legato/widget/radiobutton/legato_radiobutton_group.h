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
    legato_radiobutton_group.h

  Summary:
    

  Description:
    This module implements functions to control a radio button group.
*******************************************************************************/

/** \file legato_radiobutton_group.h
 * @brief Radiobutton widget functions and definitions.
 *
 * @details This module implements functions to control a radio button group.
 */

#ifndef LEGATO_RADIOBUTTON_GROUP_H
#define LEGATO_RADIOBUTTON_GROUP_H

#include "gfx/legato/common/legato_common.h"

#if LE_RADIOBUTTON_WIDGET_ENABLED == 1

#ifdef __cplusplus
extern "C" {
#endif

#include "gfx/legato/datastructure/legato_array.h"

// *****************************************************************************
// *****************************************************************************
// Section: Data Types and Constants
// *****************************************************************************
// *****************************************************************************


typedef struct leRadioButtonGroup leRadioButtonGroup;
typedef struct leRadioButtonWidget leRadioButtonWidget;


// *****************************************************************************
/*  Structure
        leRadioButtonGroup

    Summary:
        Defines the structure used for the Radio Button group.
        
    Description:
        Defines the parameters required for a Radio Button group.
        Marks the current selected Radio button within the group
        
    Remarks:
        None.
        
*/
/**
 * @brief This struct represents radio button group.
 * @details Defines the parameters required for a Radio Button
 * group. Marks the current selected Radio button within the group.
 */
typedef struct leRadioButtonGroup
{
    leArray buttonList;

    leBool initialized;
    leRadioButtonWidget* selected;

} leRadioButtonGroup;

// *****************************************************************************
// *****************************************************************************
// Section: Routines
// *****************************************************************************
// *****************************************************************************

/*    Function:
        leResult leRadioButtonGroup_Create(leRadioButtonGroup** grp)
 
    Summary:
        This function creates a GFX_GOL_RADIOBUTTON group with the
        provided button list. 

    Description:
        This function creates a GFX_GOL_RADIOBUTTON group with the
        given pointer and the button list provided within the leRadioButtonGroup object.

    Parameters:
    leRadioButtnGroup **
    
    Returns:
        leResult    

*/
/**
 * @brief Initialize widget.
 * @details Initializes <span class="param">wgt</span>.
 * @code
 * leRadioButtonGroup** wgt;
 * leResult res = leRadioButtonGroup_Create(wgt);
 * @endcode
 * @param param1 wgt is the widget to initialize
 * @return void.
 */
leResult leRadioButtonGroup_Create(leRadioButtonGroup** grp);

/*    Function:
        void leRadioButtonGroup_Destroy(leRadioButtonGroup* grp)
 
    Summary:
        This function destroys the radio button group

    Description:
        This function destroys he radio button group with the
        given pointer. It frees the memory allocated to the button group and clears the button list.
        
    Parameters:
        leRadioButtnGroup *
    
    Returns:
        void

*/
/**
 * @brief Initialize widget.
 * @details Initializes <span class="param">wgt</span>.
 * @code
 * leRadioButtonGroup** grp;
 * leRadioButtonGroup_Destroy(grp);
 * @endcode
 * @param param1 wgt is the widget to initialize
 * @return void.
 */
void leRadioButtonGroup_Destroy(leRadioButtonGroup* grp);

/*    Function:
        leResult leRadioButtonGroup_AddButton(leRadioButtonGroup* grp,
                                                 leRadioButtonWidget* btn)
 
    Summary:
        Add a button widget to the button list of the selected Radio button group.

    Description:
        Add a button widget to the button list of the selected Radio button group.
        The function makes sure the radio button grp is valid and the button widget to be added is not already a part of the group.
        The button is then added as the last button in the group button list
        
    Parameters:
        leRadioButtnGroup *
        leRadioButtonWidget*
    
    Returns:
        leResult

*/
/**
 * @brief Initialize widget.
 * @details Initializes <span class="param">wgt</span>.
 * @code
 * leRadioButtonGroup** grp;
 * leResult res = leRadioButtonGroup_AddButton(grp);
 * @endcode
 * @param param1 wgt is the widget to initialize
 * @return void.
 */
leResult leRadioButtonGroup_AddButton(leRadioButtonGroup* grp, leRadioButtonWidget* btn);

/*    Function:
        leResult leRadioButtonGroup_RemoveButton(leRadioButtonGroup* grp,
                                                    leRadioButtonWidget* btn);
 
    Summary:
        Remove a button widget to the button list of the selected Radio button group.

    Description:
        Remove a button widget to the button list of the selected Radio button group.
        The function makes sure the radio button grp is valid and the button widget to be removed is a part of the group.
        The button is then removed properly making sure to handle the list correctly. If the list size is 0, the group is destroyed.
        
    Parameters:
        leRadioButtnGroup *
        leRadioButtonWidget*
    
    Returns:
        leResult

*/
/**
 * @brief Initialize widget.
 * @details Initializes <span class="param">wgt</span>.
 * @code
 * leRadioButtonGroup** grp;
 * leResult res = leRadioButtonGroup_RemoveButton(grp);
 * @endcode
 * @param param1 wgt is the widget to initialize
 * @return void.
 */
leResult leRadioButtonGroup_RemoveButton(leRadioButtonGroup* grp,
                                         leRadioButtonWidget* btn);

/*    Function:
        leResult leRadioButtonGroup_SelectButton(leRadioButtonGroup* grp,
                                                    leRadioButtonWidget* btn)
 
    Summary:
        Select the button widget specified from the button list for the Radio button group.

    Description:
        Select the button widget specified from the button list for the Radio button group.
        The function makes sure the specified button widget is a part of the group.
        It deselects the currently selected button widget and reassigns the focus to the button widget specified.
        
    Parameters:
        leRadioButtnGroup *
        leRadioButtonWidget*
    
    Returns:
        leResult

*/
/**
 * @brief Initialize widget.
 * @details Initializes <span class="param">wgt</span>.
 * @code
 * leRadioButtonGroup** grp;
 * leRadioButtonWidget* btn;
 * leResult res = leRadioButtonGroup_SelectButton(grp, btn);
 * @endcode
 * @param param1 grp is the button group to modify
 * @param param2 btn is the button to select
 * @return void.
 */
leResult leRadioButtonGroup_SelectButton(leRadioButtonGroup* grp,
                                         leRadioButtonWidget* btn);

#ifdef __cplusplus
}
#endif

#endif // LE_RADIOBUTTON_WIDGET_ENABLED
#endif /* LEGATO_RADIOBUTTON_GROUP_H */
