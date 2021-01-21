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
 Module for Microchip Graphics Library

  Company:
    Microchip Technology Inc.

  File Name:
    libaria_rtos.h

  Summary:
    This header file contains prototypes of the RTOS extensions of the aria 
    library top level APIs.

  Description:
    This header file contains prototypes of the RTOS extensions of the aria 
    library top level APIs.
*******************************************************************************/


// *****************************************************************************
/* Type:
    void laUpdate_RTOS()

  Summary:
    RTOS version of the Legato library update (tasks) function.

  Description:
    This function updates the active Aria library context state.  It performs
    event processing as well as servicing of the widget paint loop. 
 
    This function may block to wait for events to process. Setting fullBlock = LA_TRUE
    will fully block without timeout, otherwise it won't block.

  Parameters:
    None 

*/

/** \file legato_rtos.h
* @brief RTOS version of the Legato library update (tasks) function.
*
* @details This header file contains prototypes of the RTOS extensions of the aria library top level APIs.
*
*/

void laUpdate_RTOS(laBool fullBlock, uint32_t dt);
    