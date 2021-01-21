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
  GFX GFX2D Driver Interface Declarations for Static Single Instance Driver

  Company:
    Microchip Technology Inc.

  File Name:
    drv_gfx2d.h

  Summary:
    GFX GFX2D driver interface declarations for the static single instance driver.

  Description:
    The GFX2D device driver provides a simple interface to manage the GFX2D
    module on Microchip microcontrollers. This file defines the interface
    Declarations for the GFX2D driver.

  Remarks:
    Static interfaces incorporate the driver instance number within the names
    of the routines, eliminating the need for an object ID or object handle.

    Static single-open interfaces also eliminate the need for the open handle.
*******************************************************************************/


#ifndef _DRV_GFX_GFX2D_H
#define _DRV_GFX_GFX2D_H


// *****************************************************************************
// *****************************************************************************
// Section: File includes
// *****************************************************************************
// *****************************************************************************
/* Note:  A file that maps the interface definitions above to appropriate static
          implementations (depending on build mode) is included at the bottom of
          this file.
*/

#include "gfx/hal/inc/gfx_common.h"
#include "gfx/hal/inc/gfx_driver_interface.h"
#include "gfx/hal/inc/gfx_default_impl.h"
#include "peripheral/gfx2d/plib_gfx2d.h"

#ifdef __cplusplus
    extern "C" {
#endif

// *****************************************************************************
/* Function:
    void DRV_GFX2D_Initialize ()

  Summary:
    Initializes the GFX2D driver.

  Description:
    This routine initializes the GFX2D driver.

  Precondition:
    None.

  Parameters:
    None.

  Returns:

  Remarks:
    This routine must be called before any other GFX2D routine is called.
    This routine should only be called once during system initialization.
*/
void DRV_GFX2D_Initialize();


// *****************************************************************************
/* Function:
    void  DRV_GFX2D_Fill()

   Summary:
    Fill a (partial) buffer with a specified color

   Description:
    Draws and fills a rectangle with a specific color onto destination buffer.

    An optional blend mode can be specified that defines the blending of the
    color onto the destination.

   Precondition:

   Parameters:
    dest             - Pointer to a n2d_buffer_t structure that describes the buffer to be filled
    dest_rectangle   - Pointer to a rectangle that specifies the area to be filled. If rectangle is NULL,
                    the entire buffer will be filled with the specified color
    color         - The color value to use for filling the buffer
    blend         - The blending mode to be applied to each pixel. If no blending is required, set this
                    value to N2D_BLEND_NONE (0)

  Returns:
    Returns the status as defined by n2d_error_t

  Remarks:
    This function will wait until the hardware is complete, i.e. it is synchronous
*/
void  DRV_GFX2D_Fill(
    GFX2D_BUFFER *dest,
    GFX2D_RECTANGLE *dest_rectangle,
    gpu_color_t color);

// *****************************************************************************
/* Function:
    void DRV_GFX2D_Copy()

   Summary:
    Copy a source buffer to the the destination buffer

   Description:
    The specified region of the source buffer is copied to the specified region
    of the destination buffer. If the regions are different in size, simple low-quality
    scaling will automatically be performed.

    An optional blend mode can be specified that defines the blending of the
    source onto the destination.

   Precondition:

   Parameters:
    dest      - Pointer to a n2d_buffer_t structure that describes the destination of the
                            blit
    dest_rect - Optional pointer to the rectangle that defines the region inside the
                            destination buffer. If this rectangle is not specified, the entire destination
                            buffer is used as the destination region
    src        - Pointer to a n2d_buffer_t structure that describes the source of the blit
    dest_rect  - Optional pointer to the rectangle that defines the region inside the source buffer.
                            If this rectangle is not specified, the entire source buffer is used as the source
                            region

  Returns:
    Returns the status as defined by GFX_STATUS

  Remarks:
    This function will wait until the hardware is complete, i.e. it is synchronous.
  */
void DRV_GFX2D_Copy(
    GFX2D_BUFFER *dest,
    GFX2D_RECTANGLE *dest_rect,
    GFX2D_BUFFER *src,
    GFX2D_RECTANGLE *src_rect);

// *****************************************************************************
/* Function:
    void DRV_GFX2D_Blend()

   Summary:
    Blend two source buffers to a destination buffer

   Description:
    The specified region of two source buffers is composited together defined by 
    blend. The result is copied to the specified region of the destination buffer.

   Precondition:

   Parameters:
    dest      - Pointer to a n2d_buffer_t structure that describes the destination of the
                   blit
    dest_rect - Optional pointer to the rectangle that defines the region inside the
                   destination buffer. If this rectangle is not specified, the entire destination
                   buffer is used as the destination region
    src1        - Pointer to a n2d_buffer_t structure that describes the source of the blit
    src1_rect  - Optional pointer to the rectangle that defines the region inside the source buffer.
                 If this rectangle is not specified, the entire source buffer is used as the source
                 region
    src2        - Pointer to a n2d_buffer_t structure that describes the source of the blit
    src2_rect  - Optional pointer to the rectangle that defines the region inside the source buffer.
                 If this rectangle is not specified, the entire source buffer is used as the source
                 region

  Returns:
    Returns the status as defined by GFX_STATUS

  Remarks:
    This function will wait until the hardware is complete, i.e. it is synchronous.
  */
void  DRV_GFX2D_Blend(
    GFX2D_BUFFER *dest,
    GFX2D_RECTANGLE *dest_rect,
    GFX2D_BUFFER *src1,
    GFX2D_RECTANGLE *src1_rect,
    GFX2D_BUFFER *src2,
    GFX2D_RECTANGLE *src2_rect,
    GFX2D_BLEND blend);

// *****************************************************************************
/* Function:
    void DRV_GFX2D_Rop()

   Summary:
    Copy and filter pixels of a source buffer with a destination buffer.

   Description:
    The specified region of the source buffer is filtered using mask. The result 
    copied to the specified regionof the destination buffer. 

   Precondition:

   Parameters:
    dest      - Pointer to a GFX2D_BUFFER structure that describes the destination of the
                   blit
    dest_rect - Optional pointer to the rectangle that defines the region inside the
                   destination buffer. If this rectangle is not specified, the entire destination
                   buffer is used as the destination region
    src1       - Pointer to a GFX2D_BUFFER structure that describes the source of the blit
    src1_rect - Optional pointer to the rectangle that defines the region inside the source buffer.
                 If this rectangle is not specified, the entire source buffer is used as the source
                 region
    src2       - Pointer to a GFX2D_BUFFER structure that describes the source of the blit
    src2_rect - Optional pointer to the rectangle that defines the region inside the source buffer.
                 If this rectangle is not specified, the entire source buffer is used as the source
                 region
    pmask       - Pointer to a masking buffer

  Returns:
    Returns the status as defined by GFX_STATUS

  Remarks:
    This function will wait until the hardware is complete, i.e. it is synchronous.
  */
void  DRV_GFX2D_Rop(
   GFX2D_BUFFER *dest, 
   GFX2D_RECTANGLE *dest_rect, 
   GFX2D_BUFFER *src1,      
   GFX2D_RECTANGLE *src1_rect, 
   GFX2D_BUFFER *src2, 
   GFX2D_RECTANGLE *src2_rect,
   GFX2D_BUFFER *pmask, 
   GFX2D_ROP rop);

// *****************************************************************************
// *****************************************************************************
// Section: Functions
// *****************************************************************************
// *****************************************************************************

GFX_Result driverGfx2DInfoGet(GFX_DriverInfo* info);
GFX_Result driverGfx2DContextInitialize(GFX_Context* context);

#ifdef __cplusplus
    }
#endif
    
#endif // #ifndef _DRV_GFX_GFX2D_H
/*******************************************************************************
 End of File
*/
