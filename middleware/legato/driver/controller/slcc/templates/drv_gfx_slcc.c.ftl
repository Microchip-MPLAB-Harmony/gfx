/*******************************************************************************
  Low Cost Controller-less Segment (SLCC) Display Driver

  Company:
    Microchip Technology Inc.

  File Name:
    drv_gfx_slcc.c

  Summary:
    SLCC Driver Source File

  Description:
    The SLCC driver provides a controller-less display interface to segment LCDs.
    It uses GPIOs and a timer to generate the signals that drive the display.

*******************************************************************************/

/*******************************************************************************
* Copyright (C) 2022 Microchip Technology Inc. and its subsidiaries.
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

#include "definitions.h"
#include "drv_gfx_slcc.h"

static volatile unsigned char seg_table[LCD_FP_CNT];
static volatile unsigned char seg_code[LCD_FP_CNT]={0};
static volatile unsigned char comstate = 0;
static volatile unsigned char pos = 0;
static unsigned char ComCode = 0; //TODO: Can be a local variable.

static void GenerateSegSignal(void);
static void GenerateComSignal(unsigned char comstate);
static void SLCC_Timer_IRQ(TC_TIMER_STATUS status, uintptr_t context);
static unsigned char FindSegmentCode(unsigned char seg_code, const unsigned char * com_table);

/* Set the segment pin i.e set the GPIO to HIGH (VCC) */
void SLCC_SegmentClear(SEG_PIN segnumber)
{
    switch(segnumber)
    {
<#list 0.. (SEG_PINS - 1) as i>
        case SEG_PIN${i}:
        {
            SEG${i}_Clear();
            break;
        }
</#list>	
        default:
        {
            break;
        }
    }    
}

/* Set the segment pin i.e set the GPIO to HIGH (VCC) */
void SLCC_SegmentSet(SEG_PIN segnumber)
{
    switch(segnumber)
    {
<#list 0.. (SEG_PINS - 1) as i>
        case SEG_PIN${i}:
        {
            SEG${i}_Set();
            break;
        }
</#list>		
        
        default:
        {
            break;
        }
        
    }    
}

/* Turn on/off a specific segment on the LCD */
void SLCC_SegmentOnOff(SEG_PIN seg, COM_PIN com, bool on, const unsigned char * com_table)
{
    if (com_table == NULL)
        return;
    
    if (on)
    {
        ComCode |= 1<< com;
        if(seg_code[seg]!=(seg_code[seg]|ComCode))
        {
            seg_table[seg]=com_table[ComCode+seg_code[seg]]; 
            seg_code[seg]=FindSegmentCode(seg_table[seg], com_table);
        }
    }
    else
    {
        ComCode |=~(1<< com); 
        if(seg_code[seg]!=(seg_code[seg]& ComCode))
        {
           seg_table[seg]=com_table[seg_code[seg] - ComCode]; 
           seg_code[seg]=FindSegmentCode(seg_table[seg], com_table);
        }
     }
      
    ComCode=0;
}

/* Turns on/off all the segments of the LCD */
void SLCC_AllSegmentsOnOff(bool on)
{
    unsigned char seg = 0;
    unsigned char value;
    
    value = (on == true) ? SLCC_SEG_ON_VALUE : SLCC_SEG_OFF_VALUE;
    
    for(seg = 0; seg < LCD_FP_CNT; seg++)
    {
        seg_table[seg] = value;    
    }
}

/* Update the Segment Table */
void SLCC_Update_SegTable(SEG_PIN seg, unsigned char c)
{
    seg_table[seg] = c;
}

/* Update the Segment Code */
void SLCC_Update_SegCode(SEG_PIN seg, unsigned char code)
{
    seg_code[seg] = code;
}

/* Update the Seg table and code */
void SLCC_Update_SegCodeTable(SEG_PIN seg, unsigned char c, const unsigned char * com_table)
{
    seg_table[seg] = c;
    seg_code[seg]=FindSegmentCode(seg_table[seg], com_table);
}

void SLCC_Initialize(void)
{
	${.vars["${TIMER_PLIB?lower_case}"].CALLBACK_API_NAME}(SLCC_Timer_IRQ, (uintptr_t)NULL);
	${.vars["${TIMER_PLIB?lower_case}"].TIMER_START_API_NAME}();
}

static unsigned char FindSegmentCode(unsigned char seg_code, const unsigned char * com_table)
{
    unsigned char pos = 0;
    
    for(unsigned char i = SEG_PIN0; i < SEG_PIN${SEG_PINS - 1}; i++)
    {
        if(com_table[i] == seg_code)
        {
            pos = i;
            break;
        }
    }
    
    return(pos);
}

static void _GenerateSegSignal(unsigned char code,unsigned char seg)
{
    
    if((code & ( 1 << pos))==0)
    {
        SLCC_SegmentSet(seg);
    }
    else
    {
        SLCC_SegmentClear(seg); 
    }
}

static void GenerateSegSignal(void)
{
    unsigned char seg=0;
    
    for(seg=0; seg < LCD_FP_CNT; seg++)
    {
        _GenerateSegSignal(seg_table[seg],seg);
    }
}

static void SLCC_Timer_IRQ(TC_TIMER_STATUS status, uintptr_t context)
{   
    pos+=1;
    comstate+=1;
    
    if(comstate > COM${COM_PINS - 1}_CLEAR)
        comstate = COM0_SET; 

    if(pos > COM${COM_PINS - 1}_CLEAR)
        pos = COM0_SET;
    
    GenerateComSignal(comstate); 
    GenerateSegSignal();
    
}

static void GenerateComSignal(unsigned char comstate)
{
  
    switch(comstate)
    {
<#list 0.. (COM_PINS - 1) as i>
        case COM${i}_SET:
        {
			/* Enable and Set COM${i} Pin */
			COM${i}_OutputEnable();
			COM${i}_Set();
			
			/* Disable other COM Pins */
<#list 0.. (COM_PINS - 1) as j>
<#if j != i>
			COM${j}_InputEnable();
</#if>
</#list>

            break;
        }
		
        case COM${i}_CLEAR:
        {
			/* Clear COM Pin */
            COM${i}_Clear(); 
            break;
        }		
</#list>
        default:
            break;
    }
}