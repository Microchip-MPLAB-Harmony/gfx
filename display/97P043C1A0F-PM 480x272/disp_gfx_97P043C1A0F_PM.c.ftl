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
/********************************************************************************
  97P043C1A0F-PM Display Support Source File

  Company:
    Microchip Technology Inc.

  File Name:
    disp_gfx_97P043C1A0F-PM.c.ftl

  Summary:
    Source code to support 97P043C1A0F-PM display initialization

  Description:
    This file contains the source code for supporting the 97P043C1A0F-PM Display.
	
*******************************************************************************/

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include "definitions.h"

// *****************************************************************************
// *****************************************************************************
// Section: Local Static Functions
// *****************************************************************************
// *****************************************************************************

static void DelayX1ms(unsigned int i)
{
    unsigned int loop1, loop2, loop3;
    for(loop1=0;loop1<i;loop1++)
    {
        for(loop2=0;loop2<2000;loop2++) { }
            for(loop3=0;loop3<20000;loop3++) { _nop(); }
    }     
    
}

void DISP_97P043C1A0F_PM_Initialize(void)
{
    BACKLIGHT_TYPE_2_Clear();
    CSX_Set();
    RESX_Clear();
    DelayX1ms(100);

    RESX_Set();
    DelayX1ms(10);
    DISPLAY_ENABLE_Set();
    DelayX1ms(200);
 
    BACKLIGHT_TYPE_2_Set();    
}


void DISP_97P043C1A0F_PM_Reset(void)
{
    unsigned int i;

    RESX_Set();
    for(i=0;i<50000;i++) { _nop(); }
    RESX_Clear();
    for(i=0;i<100000;i++) { _nop(); }
    for(i=0;i<100000;i++) { _nop(); }
    RESX_Set();
    for(i=0;i<50000;i++) { _nop(); }

}
    
/*******************************************************************************
 End of File
*/
