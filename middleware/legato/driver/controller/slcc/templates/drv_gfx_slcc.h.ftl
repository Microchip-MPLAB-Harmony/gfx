/*******************************************************************************
  Low Cost Controller-less Segment (SLCC) Display Driver

  Company:
    Microchip Technology Inc.

  File Name:
    drv_gfx_slcc.h

  Summary:
    SLCC Driver Header File

  Description:
    This file provides an interface to control and interact with the SLCC
    Display Driver.

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

#ifndef DRV_GFX_SLCC_H
#define	DRV_GFX_SLCC_H

#ifdef	__cplusplus
extern "C" {
#endif

#define LCD_FP_CNT		${SEG_PINS} /* number of frontplane electrodes */
#define LCD_BP_CNT 		${COM_PINS} /* number of backplane electrodes */    
#define SLCC_ON  		true
#define SLCC_OFF 		false

/* Segtable value to turn off/on all segments */
#define SLCC_SEG_OFF_VALUE 	0x${SEGOFF_VAL}
#define SLCC_SEG_ON_VALUE 	0x${SEGON_VAL}

typedef enum
{
<#list 0.. (COM_PINS - 1) as i>
    COM${i}_SET,
    COM${i}_CLEAR,  
</#list>
} COM_STATE;

typedef enum
{
<#list 0.. (COM_PINS - 1) as i>
    COM_PIN${i},  
</#list>
    COM_PIN_MAX = LCD_BP_CNT,
} COM_PIN;

typedef enum
{
<#list 0.. (SEG_PINS - 1) as i>
    SEG_PIN${i},  
</#list>
    SEG_PIN_MAX = LCD_FP_CNT,
} SEG_PIN;
    
// *****************************************************************************
/* Function:
    void SLCC_SegmentClear(SEG_PIN segnumber)

  Summary:
    Set the segment GPIO pin to LOW.

  Description:
    This function will set the specified SEG_PIN low. The corresponding pin should be
	user configured in using the MHC Pin manager and named SEGn where n is the segment
	number <segnumber>.

  Precondition:
    The segment pin must have been configured in MHC as GPIO output, and labeled 'SEGn'
	where n is the segment number.

  Parameters:
    segnumber - the segmend pin index

  Returns:
    None.

  Example:
    <code>

    SLCC_SegmentClear(SEG_PIN0);

    </code>

  Remarks:
    None.
*/
void SLCC_SegmentClear(SEG_PIN segnumber);

// *****************************************************************************
/* Function:
    void SLCC_SegmentClear(SEG_PIN segnumber)

  Summary:
    Set the segment GPIO pin to HIGH.

  Description:
    This function will set the specified SEG_PIN high. The corresponding pin should be
	user configured in using the MHC Pin manager and named SEGn where n is the segment
	number <segnumber>.

  Precondition:
    The segment pin must have been configured in MHC as GPIO output, and labeled 'SEGn'
	where n is the segment number.

  Parameters:
    segnumber - the segmend pin index

  Returns:
    None.

  Example:
    <code>

    SLCC_SegmentSet(SEG_PIN0);

    </code>

  Remarks:
    None.
*/
void SLCC_SegmentSet(SEG_PIN segnumber);

// *****************************************************************************
/* Function:
    void SLCC_SegmentOnOff(SEG_PIN seg, COM_PIN com, bool on, const unsigned char * com_table)

  Summary:
    Turn on/off a specific segment/display element on the LCD

  Description:
    This function will turn on or off a display segment/unit on the LCD, based on the specified
	COM and SEG pin. 

  Precondition:
    The segment and COM pins must have been configured in MHC as GPIO output, and labeled 'SEGn'
	and 'COMn' where n is the segment or com number.

  Parameters:
    seg - the segmend pin index
	com - the com pin index
	on - turns on the segment if true, off if false
	com_table - the application defined com table

  Returns:
    None.

  Example:
    <code>

    SLCC_SegmentOnOff(SEG17,COM0,ON, com_table);

    </code>

  Remarks:
    None.
*/
void SLCC_SegmentOnOff(SEG_PIN seg, COM_PIN com, bool on, const unsigned char * com_table);

// *****************************************************************************
/* Function:
    void SLCC_AllSegmentsOnOff(bool on)

  Summary:
    Turns on/off all the display segments on the LCD

  Description:
    This function will turn on or off all the display segments on the LCD. 

  Precondition:
    The segment and COM pins must have been configured in MHC as GPIO output, and labeled 'SEGn'
	and 'COMn' where n is the segment or com number.

  Parameters:
	on - turns on the segments if true, off if false

  Returns:
    None.

  Example:
    <code>

    SLCC_AllSegmentsOnOff(true); //Turn on all segments

    </code>

  Remarks:
    None.
*/
void SLCC_AllSegmentsOnOff(bool on);

// *****************************************************************************
/* Function:
    void SLCC_Update_SegTable(SEG_PIN seg, unsigned char c)

  Summary:
    Updates the segtable value for the specified segment pin

  Description:
    This will set the segment pin value in the seg_table

  Precondition:
    The segment pin must have been configured in MHC as GPIO output, and labeled 'SEGn'
	where n is the segment number.

  Parameters:
	seg - the segment pin index
	c - the seg table value for the segment pin

  Returns:
    None.

  Example:
    <code>

    SLCC_Update_SegTable(SEG0, 0xAA);

    </code>

  Remarks:
    None.
*/
void SLCC_Update_SegTable(SEG_PIN seg, unsigned char c);

// *****************************************************************************
/* Function:
    void SLCC_Update_SegCode(SEG_PIN seg, unsigned char code)

  Summary:
    Updates the segcode value for the specified segment pin

  Description:
    This will set the segment pin value in the seg_code

  Precondition:
    The segment pin must have been configured in MHC as GPIO output, and labeled 'SEGn'
	where n is the segment number.

  Parameters:
	seg - the segment pin index
	code - the seg code value for the segment pin

  Returns:
    None.

  Example:
    <code>

    SLCC_Update_SegCode(SEG0, 0xA9);

    </code>

  Remarks:
    None.
*/
void SLCC_Update_SegCode(SEG_PIN seg, unsigned char code);

// *****************************************************************************
/* Function:
    void SLCC_Update_SegCodeTable(SEG_PIN seg, unsigned char c, const unsigned char * com_table)

  Summary:
    Updates the seg table and code value for the specified segment pin, based on the com_table

  Description:
    This will set the segment pin value in the seg_code and seg_table

  Precondition:
    The segment pin must have been configured in MHC as GPIO output, and labeled 'SEGn'
	where n is the segment number.

  Parameters:
	seg - the segment pin index
	c - the seg table value for the segment pin
    com_table - the application specific com_table

  Returns:
    None.

  Example:
    <code>

    SLCC_Update_SegCodeTable(SEG0, 0xAA, com_table);

    </code>

  Remarks:
    None.
*/
void SLCC_Update_SegCodeTable(SEG_PIN seg, unsigned char c, const unsigned char * com_table);

// *****************************************************************************
/* Function:
    void SLCC_Initialize(void)

  Summary:
    Updates the segcode value for the specified segment pin

  Description:
    This will set the segment pin value in the seg_code

  Precondition:
    The segment pin must have been configured in MHC as GPIO output, and labeled 'SEGn'
	where n is the segment number.

  Parameters:
	seg - the segment pin index
	code - the seg code value for the segment pin

  Returns:
    None.

  Example:
    <code>

    SLCC_Initialize();

    </code>

  Remarks:
    None.
*/
void SLCC_Initialize(void);
    
#ifdef	__cplusplus
}
#endif

#endif	/* DRV_GFX_SLCC_H */

