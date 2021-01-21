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

/********************************************************************************
  GFX GFX2D Driver Functions

  Company:
    Microchip Technology Inc.

  File Name:
    drv_glcd.c

  Summary:
    Source code for the GFX GFX2D driver static implementation.

  Description:
    This file contains the source code for the static implementation of the
    GFX GFX2D driver.
*******************************************************************************/


// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include "gfx/driver/processor/gfx2d/drv_gfx2d.h"
#include "definitions.h"
#include "system/time/sys_time.h"

// *****************************************************************************
// *****************************************************************************
// Section: Global Data
// *****************************************************************************
// *****************************************************************************

#define DISPLAY_WIDTH  480
#define DISPLAY_HEIGHT 272
uint32_t  __attribute__ ((aligned (64))) blitbuffer[DISPLAY_WIDTH * DISPLAY_HEIGHT] ;
uint32_t  __attribute__ ((aligned (64))) maskbuffer[DISPLAY_WIDTH * DISPLAY_HEIGHT] ;


/* Indicate end of execute instruction */
volatile uint8_t gpu_end = 0;

void _IntHandler(uintptr_t context)
{
    gpu_end = 1;
}

/**** End Hardware Abstraction Interfaces ****/


void DRV_GFX2D_Initialize()
{
    // call the plib initialization routines
    PLIB_GFX2D_Initialize();
    PLIB_GFX2D_Enable();
    PLIB_GFX2D_IRQ_CallbackRegister(_IntHandler, 0);
}

void  DRV_GFX2D_Fill(
    GFX2D_BUFFER *dest,
    GFX2D_RECTANGLE *dest_rect,
    gpu_color_t color)
{
    PLIB_GFX2D_Fill(dest, dest_rect, color);

    /* Wait for instruction to complete */
    while ( PLIB_GFX2D_GetGlobalStatusBusy() == true ) ;
    //while (gpu_end == 0) {
    //};
}

void  DRV_GFX2D_Copy(
    GFX2D_BUFFER *dest,
    GFX2D_RECTANGLE *dest_rect,
    GFX2D_BUFFER *src,
    GFX2D_RECTANGLE *src_rect)
{
    PLIB_GFX2D_Copy(dest, dest_rect, src, src_rect);

    /* Wait for instruction to complete */
    while ( PLIB_GFX2D_GetGlobalStatusBusy() == true ) ;
    //while (gpu_end == 0) {
    //};
}

void  DRV_GFX2D_Blend(
    GFX2D_BUFFER *dest,
    GFX2D_RECTANGLE *dest_rect,
    GFX2D_BUFFER *src1,
    GFX2D_RECTANGLE *src1_rect,
    GFX2D_BUFFER *src2,
    GFX2D_RECTANGLE *src2_rect,
    GFX2D_BLEND blend)
{
    PLIB_GFX2D_Blend(dest, dest_rect, src1, src1_rect, src2, src2_rect, blend);

    /* Wait for instruction to complete */
    while ( PLIB_GFX2D_GetGlobalStatusBusy() == true ) ;
    //while (gpu_end == 0) {
    //};
}

void  DRV_GFX2D_Rop(
   GFX2D_BUFFER *dest, 
   GFX2D_RECTANGLE *dest_rect, 
   GFX2D_BUFFER *src1,      
   GFX2D_RECTANGLE *src1_rect, 
   GFX2D_BUFFER *src2, 
   GFX2D_RECTANGLE *src2_rect,
   GFX2D_BUFFER *pmask, 
   GFX2D_ROP rop)
{
    PLIB_GFX2D_Rop(dest, dest_rect, src1, src1_rect, src2, src2_rect, pmask, rop);

    /* Wait for instruction to complete */
    while ( PLIB_GFX2D_GetGlobalStatusBusy() == true ) ;
    //while (gpu_end == 0) {
    //};
}
/*******************************************************************************
 End of File
*/
