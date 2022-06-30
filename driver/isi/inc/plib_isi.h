// DOM-IGNORE-BEGIN
/*******************************************************************************
* Copyright (C) 2021 Microchip Technology Inc. and its subsidiaries.
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
  Image Sensor Interface (ISI) Peripheral Library

  Company:
    Microchip Technology Inc.

  File Name:
    plib_isi.h

  Summary:
    The header file for the Image Sensor Interface (ISI) peripheral library

  Description:
    This header file includes all the function definitions for the 
    Image Sensor Interface (ISI) peripheral library. 
*******************************************************************************/

#ifndef PLIB_ISI_H
#define PLIB_ISI_H

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

/*  This section lists the other files that are included in this file. */
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

// DOM-IGNORE-BEGIN
#ifdef __cplusplus
extern "C" {
#endif
// DOM-IGNORE-END

// *****************************************************************************
// *****************************************************************************
// Section: Data Types
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/* enum: ISI_THMASK_Value

    Summary:
      Threshold Mask
    Description:
      Threshold Mask
    Remarks:
      None.
*/
typedef enum
{
    // Only 4 beats AHB burst allowed
    ISI_THMASK_BEATS_4 = 0,

    // Only 4 and 8 beats AHB burst allowed
    ISI_THMASK_BEATS_8 = 1,

    // 4, 8 and 16 beats AHB burst allowed
    ISI_THMASK_BEATS_16 = 2,
} ISI_THMASK_Value;

// *****************************************************************************
/* enum: ISI_YCC_SWAP_Value

    Summary:
      YCrCb Format Swap Mode
    Description:
      YCrCb Format Swap Mode
    Remarks:
      None.
*/
typedef enum
{
    // Byte 0 Cb(i) Byte 1 Y(i) Byte 2 Cr(i) Byte 3 Y(i+1)
    ISI_YCC_SWAP_DEFAULT = 0,

    // Byte 0 Cr(i) Byte 1 Y(i) Byte 2 Cb(i) Byte 3 Y(i+1)
    ISI_YCC_SWAP_MODE1 = 1,

    // Byte 0 Y(i) Byte 1 Cb(i) Byte 2 Y(i+1) Byte 3 Cr(i)
    ISI_YCC_SWAP_MODE2 = 2,

    // Byte 0 Y(i) Byte 1 Cr(i) Byte 2 Y(i+1) Byte 3 Cb(i)
    ISI_YCC_SWAP_MODE3 = 3,
} ISI_YCC_SWAP_Value;

// *****************************************************************************
/* enum: ISI_RGB_CFG_Value

    Summary:
      RGB Pixel Mapping Configuration
    Description:
      RGB Pixel Mapping Configuration
    Remarks:
      None.
*/
typedef enum
{
    // Byte 0 R/G(MSB) Byte 1 G(LSB)/B Byte 2 R/G(MSB) Byte 3 G(LSB)/B
    ISI_RGB_CFG_DEFAULT = 0,

    // Byte 0 B/G(MSB) Byte 1 G(LSB)/R Byte 2 B/G(MSB) Byte 3 G(LSB)/R
    ISI_RGB_CFG_MODE1 = 1,

    // Byte 0 G(LSB)/R Byte 1 B/G(MSB) Byte 2 G(LSB)/R Byte 3 B/G(MSB)
    ISI_RGB_CFG_MODE2 = 2,

    // Byte 0 G(LSB)/B Byte 1 R/G(MSB) Byte 2 G(LSB)/B Byte 3 R/G(MSB)
    ISI_RGB_CFG_MODE3 = 3,
} ISI_RGB_CFG_Value;

// *****************************************************************************
/* enum: ISI_WPKEY_Value

    Summary:
      Write Protection Key Password
    Description:
      Write Protection Key Password
    Remarks:
      None.
*/
typedef enum
{
    // Writing any other value in this field aborts the write operation of the WPEN bit. Always reads as 0.
    ISI_WPKEY_PASSWD = 4805449,
} ISI_WPKEY_Value;

// *****************************************************************************
// *****************************************************************************
// Section: Interface Functions
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/* function:
     PLIB_ISI_CFG1_HSYNC_POL_Get

    Summary:
      Returns the value of SFR: HSYNC_POL

    Description:
      Returns the value of SFR: HSYNC_POL

    Parameters:
      None

    Returns:
      uint32_t

    Remarks:
      None.
*/
uint32_t PLIB_ISI_CFG1_HSYNC_POL_Get(void);

// *****************************************************************************
/* function:
     PLIB_ISI_CFG1_HSYNC_POL_Set

    Summary:
      Returns the value of SFR: HSYNC_POL

    Description:
      Returns the value of SFR: HSYNC_POL

    Parameters:
      uint32_t - val

    Returns:
      void

    Remarks:
      None.
*/
void PLIB_ISI_CFG1_HSYNC_POL_Set(uint32_t val);

// *****************************************************************************
/* function:
     PLIB_ISI_CFG1_VSYNC_POL_Get

    Summary:
      Returns the value of SFR: VSYNC_POL

    Description:
      Returns the value of SFR: VSYNC_POL

    Parameters:
      None

    Returns:
      uint32_t

    Remarks:
      None.
*/
uint32_t PLIB_ISI_CFG1_VSYNC_POL_Get(void);

// *****************************************************************************
/* function:
     PLIB_ISI_CFG1_VSYNC_POL_Set

    Summary:
      Returns the value of SFR: VSYNC_POL

    Description:
      Returns the value of SFR: VSYNC_POL

    Parameters:
      uint32_t - val

    Returns:
      void

    Remarks:
      None.
*/
void PLIB_ISI_CFG1_VSYNC_POL_Set(uint32_t val);

// *****************************************************************************
/* function:
     PLIB_ISI_CFG1_PIXCLK_POL_Get

    Summary:
      Returns the value of SFR: PIXCLK_POL

    Description:
      Returns the value of SFR: PIXCLK_POL

    Parameters:
      None

    Returns:
      uint32_t

    Remarks:
      None.
*/
uint32_t PLIB_ISI_CFG1_PIXCLK_POL_Get(void);

// *****************************************************************************
/* function:
     PLIB_ISI_CFG1_PIXCLK_POL_Set

    Summary:
      Returns the value of SFR: PIXCLK_POL

    Description:
      Returns the value of SFR: PIXCLK_POL

    Parameters:
      uint32_t - val

    Returns:
      void

    Remarks:
      None.
*/
void PLIB_ISI_CFG1_PIXCLK_POL_Set(uint32_t val);

// *****************************************************************************
/* function:
     PLIB_ISI_CFG1_EMB_SYNC_Get

    Summary:
      Returns the value of SFR: EMB_SYNC

    Description:
      Returns the value of SFR: EMB_SYNC

    Parameters:
      None

    Returns:
      uint32_t

    Remarks:
      None.
*/
uint32_t PLIB_ISI_CFG1_EMB_SYNC_Get(void);

// *****************************************************************************
/* function:
     PLIB_ISI_CFG1_EMB_SYNC_Set

    Summary:
      Returns the value of SFR: EMB_SYNC

    Description:
      Returns the value of SFR: EMB_SYNC

    Parameters:
      uint32_t - val

    Returns:
      void

    Remarks:
      None.
*/
void PLIB_ISI_CFG1_EMB_SYNC_Set(uint32_t val);

// *****************************************************************************
/* function:
     PLIB_ISI_CFG1_CRC_SYNC_Get

    Summary:
      Returns the value of SFR: CRC_SYNC

    Description:
      Returns the value of SFR: CRC_SYNC

    Parameters:
      None

    Returns:
      uint32_t

    Remarks:
      None.
*/
uint32_t PLIB_ISI_CFG1_CRC_SYNC_Get(void);

// *****************************************************************************
/* function:
     PLIB_ISI_CFG1_CRC_SYNC_Set

    Summary:
      Returns the value of SFR: CRC_SYNC

    Description:
      Returns the value of SFR: CRC_SYNC

    Parameters:
      uint32_t - val

    Returns:
      void

    Remarks:
      None.
*/
void PLIB_ISI_CFG1_CRC_SYNC_Set(uint32_t val);

// *****************************************************************************
/* function:
     PLIB_ISI_CFG1_FRATE_Get

    Summary:
      Returns the value of SFR: FRATE

    Description:
      Returns the value of SFR: FRATE

    Parameters:
      None

    Returns:
      uint32_t

    Remarks:
      None.
*/
uint32_t PLIB_ISI_CFG1_FRATE_Get(void);

// *****************************************************************************
/* function:
     PLIB_ISI_CFG1_FRATE_Set

    Summary:
      Returns the value of SFR: FRATE

    Description:
      Returns the value of SFR: FRATE

    Parameters:
      uint32_t - val

    Returns:
      void

    Remarks:
      None.
*/
void PLIB_ISI_CFG1_FRATE_Set(uint32_t val);

// *****************************************************************************
/* function:
     PLIB_ISI_CFG1_DISCR_Get

    Summary:
      Returns the value of SFR: DISCR

    Description:
      Returns the value of SFR: DISCR

    Parameters:
      None

    Returns:
      uint32_t

    Remarks:
      None.
*/
uint32_t PLIB_ISI_CFG1_DISCR_Get(void);

// *****************************************************************************
/* function:
     PLIB_ISI_CFG1_DISCR_Set

    Summary:
      Returns the value of SFR: DISCR

    Description:
      Returns the value of SFR: DISCR

    Parameters:
      uint32_t - val

    Returns:
      void

    Remarks:
      None.
*/
void PLIB_ISI_CFG1_DISCR_Set(uint32_t val);

// *****************************************************************************
/* function:
     PLIB_ISI_CFG1_FULL_Get

    Summary:
      Returns the value of SFR: FULL

    Description:
      Returns the value of SFR: FULL

    Parameters:
      None

    Returns:
      uint32_t

    Remarks:
      None.
*/
uint32_t PLIB_ISI_CFG1_FULL_Get(void);

// *****************************************************************************
/* function:
     PLIB_ISI_CFG1_FULL_Set

    Summary:
      Returns the value of SFR: FULL

    Description:
      Returns the value of SFR: FULL

    Parameters:
      uint32_t - val

    Returns:
      void

    Remarks:
      None.
*/
void PLIB_ISI_CFG1_FULL_Set(uint32_t val);

// *****************************************************************************
/* function:
     PLIB_ISI_CFG1_THMASK_Get

    Summary:
      Returns the value of SFR: THMASK

    Description:
      Returns the value of SFR: THMASK

    Parameters:
      None

    Returns:
      ISI_THMASK_Value

    Remarks:
      None.
*/
ISI_THMASK_Value PLIB_ISI_CFG1_THMASK_Get(void);

// *****************************************************************************
/* function:
     PLIB_ISI_CFG1_THMASK_Set

    Summary:
      Returns the value of SFR: THMASK

    Description:
      Returns the value of SFR: THMASK

    Parameters:
      ISI_THMASK_Value - val

    Returns:
      void

    Remarks:
      None.
*/
void PLIB_ISI_CFG1_THMASK_Set(ISI_THMASK_Value val);

// *****************************************************************************
/* function:
     PLIB_ISI_CFG1_SLD_Get

    Summary:
      Returns the value of SFR: SLD

    Description:
      Returns the value of SFR: SLD

    Parameters:
      None

    Returns:
      uint32_t

    Remarks:
      None.
*/
uint32_t PLIB_ISI_CFG1_SLD_Get(void);

// *****************************************************************************
/* function:
     PLIB_ISI_CFG1_SLD_Set

    Summary:
      Returns the value of SFR: SLD

    Description:
      Returns the value of SFR: SLD

    Parameters:
      uint32_t - val

    Returns:
      void

    Remarks:
      None.
*/
void PLIB_ISI_CFG1_SLD_Set(uint32_t val);

// *****************************************************************************
/* function:
     PLIB_ISI_CFG1_SFD_Get

    Summary:
      Returns the value of SFR: SFD

    Description:
      Returns the value of SFR: SFD

    Parameters:
      None

    Returns:
      uint32_t

    Remarks:
      None.
*/
uint32_t PLIB_ISI_CFG1_SFD_Get(void);

// *****************************************************************************
/* function:
     PLIB_ISI_CFG1_SFD_Set

    Summary:
      Returns the value of SFR: SFD

    Description:
      Returns the value of SFR: SFD

    Parameters:
      uint32_t - val

    Returns:
      void

    Remarks:
      None.
*/
void PLIB_ISI_CFG1_SFD_Set(uint32_t val);

// *****************************************************************************
/* function:
     PLIB_ISI_CFG2_IM_VSIZE_Get

    Summary:
      Returns the value of SFR: IM_VSIZE

    Description:
      Returns the value of SFR: IM_VSIZE

    Parameters:
      None

    Returns:
      uint32_t

    Remarks:
      None.
*/
uint32_t PLIB_ISI_CFG2_IM_VSIZE_Get(void);

// *****************************************************************************
/* function:
     PLIB_ISI_CFG2_IM_VSIZE_Set

    Summary:
      Returns the value of SFR: IM_VSIZE

    Description:
      Returns the value of SFR: IM_VSIZE

    Parameters:
      uint32_t - val

    Returns:
      void

    Remarks:
      None.
*/
void PLIB_ISI_CFG2_IM_VSIZE_Set(uint32_t val);

// *****************************************************************************
/* function:
     PLIB_ISI_CFG2_GS_MODE_Get

    Summary:
      Returns the value of SFR: GS_MODE

    Description:
      Returns the value of SFR: GS_MODE

    Parameters:
      None

    Returns:
      uint32_t

    Remarks:
      None.
*/
uint32_t PLIB_ISI_CFG2_GS_MODE_Get(void);

// *****************************************************************************
/* function:
     PLIB_ISI_CFG2_GS_MODE_Set

    Summary:
      Returns the value of SFR: GS_MODE

    Description:
      Returns the value of SFR: GS_MODE

    Parameters:
      uint32_t - val

    Returns:
      void

    Remarks:
      None.
*/
void PLIB_ISI_CFG2_GS_MODE_Set(uint32_t val);

// *****************************************************************************
/* function:
     PLIB_ISI_CFG2_RGB_MODE_Get

    Summary:
      Returns the value of SFR: RGB_MODE

    Description:
      Returns the value of SFR: RGB_MODE

    Parameters:
      None

    Returns:
      uint32_t

    Remarks:
      None.
*/
uint32_t PLIB_ISI_CFG2_RGB_MODE_Get(void);

// *****************************************************************************
/* function:
     PLIB_ISI_CFG2_RGB_MODE_Set

    Summary:
      Returns the value of SFR: RGB_MODE

    Description:
      Returns the value of SFR: RGB_MODE

    Parameters:
      uint32_t - val

    Returns:
      void

    Remarks:
      None.
*/
void PLIB_ISI_CFG2_RGB_MODE_Set(uint32_t val);

// *****************************************************************************
/* function:
     PLIB_ISI_CFG2_GRAYSCALE_Get

    Summary:
      Returns the value of SFR: GRAYSCALE

    Description:
      Returns the value of SFR: GRAYSCALE

    Parameters:
      None

    Returns:
      uint32_t

    Remarks:
      None.
*/
uint32_t PLIB_ISI_CFG2_GRAYSCALE_Get(void);

// *****************************************************************************
/* function:
     PLIB_ISI_CFG2_GRAYSCALE_Set

    Summary:
      Returns the value of SFR: GRAYSCALE

    Description:
      Returns the value of SFR: GRAYSCALE

    Parameters:
      uint32_t - val

    Returns:
      void

    Remarks:
      None.
*/
void PLIB_ISI_CFG2_GRAYSCALE_Set(uint32_t val);

// *****************************************************************************
/* function:
     PLIB_ISI_CFG2_RGB_SWAP_Get

    Summary:
      Returns the value of SFR: RGB_SWAP

    Description:
      Returns the value of SFR: RGB_SWAP

    Parameters:
      None

    Returns:
      uint32_t

    Remarks:
      None.
*/
uint32_t PLIB_ISI_CFG2_RGB_SWAP_Get(void);

// *****************************************************************************
/* function:
     PLIB_ISI_CFG2_RGB_SWAP_Set

    Summary:
      Returns the value of SFR: RGB_SWAP

    Description:
      Returns the value of SFR: RGB_SWAP

    Parameters:
      uint32_t - val

    Returns:
      void

    Remarks:
      None.
*/
void PLIB_ISI_CFG2_RGB_SWAP_Set(uint32_t val);

// *****************************************************************************
/* function:
     PLIB_ISI_CFG2_COL_SPACE_Get

    Summary:
      Returns the value of SFR: COL_SPACE

    Description:
      Returns the value of SFR: COL_SPACE

    Parameters:
      None

    Returns:
      uint32_t

    Remarks:
      None.
*/
uint32_t PLIB_ISI_CFG2_COL_SPACE_Get(void);

// *****************************************************************************
/* function:
     PLIB_ISI_CFG2_COL_SPACE_Set

    Summary:
      Returns the value of SFR: COL_SPACE

    Description:
      Returns the value of SFR: COL_SPACE

    Parameters:
      uint32_t - val

    Returns:
      void

    Remarks:
      None.
*/
void PLIB_ISI_CFG2_COL_SPACE_Set(uint32_t val);

// *****************************************************************************
/* function:
     PLIB_ISI_CFG2_IM_HSIZE_Get

    Summary:
      Returns the value of SFR: IM_HSIZE

    Description:
      Returns the value of SFR: IM_HSIZE

    Parameters:
      None

    Returns:
      uint32_t

    Remarks:
      None.
*/
uint32_t PLIB_ISI_CFG2_IM_HSIZE_Get(void);

// *****************************************************************************
/* function:
     PLIB_ISI_CFG2_IM_HSIZE_Set

    Summary:
      Returns the value of SFR: IM_HSIZE

    Description:
      Returns the value of SFR: IM_HSIZE

    Parameters:
      uint32_t - val

    Returns:
      void

    Remarks:
      None.
*/
void PLIB_ISI_CFG2_IM_HSIZE_Set(uint32_t val);

// *****************************************************************************
/* function:
     PLIB_ISI_CFG2_YCC_SWAP_Get

    Summary:
      Returns the value of SFR: YCC_SWAP

    Description:
      Returns the value of SFR: YCC_SWAP

    Parameters:
      None

    Returns:
      ISI_YCC_SWAP_Value

    Remarks:
      None.
*/
ISI_YCC_SWAP_Value PLIB_ISI_CFG2_YCC_SWAP_Get(void);

// *****************************************************************************
/* function:
     PLIB_ISI_CFG2_YCC_SWAP_Set

    Summary:
      Returns the value of SFR: YCC_SWAP

    Description:
      Returns the value of SFR: YCC_SWAP

    Parameters:
      ISI_YCC_SWAP_Value - val

    Returns:
      void

    Remarks:
      None.
*/
void PLIB_ISI_CFG2_YCC_SWAP_Set(ISI_YCC_SWAP_Value val);

// *****************************************************************************
/* function:
     PLIB_ISI_CFG2_RGB_CFG_Get

    Summary:
      Returns the value of SFR: RGB_CFG

    Description:
      Returns the value of SFR: RGB_CFG

    Parameters:
      None

    Returns:
      ISI_RGB_CFG_Value

    Remarks:
      None.
*/
ISI_RGB_CFG_Value PLIB_ISI_CFG2_RGB_CFG_Get(void);

// *****************************************************************************
/* function:
     PLIB_ISI_CFG2_RGB_CFG_Set

    Summary:
      Returns the value of SFR: RGB_CFG

    Description:
      Returns the value of SFR: RGB_CFG

    Parameters:
      ISI_RGB_CFG_Value - val

    Returns:
      void

    Remarks:
      None.
*/
void PLIB_ISI_CFG2_RGB_CFG_Set(ISI_RGB_CFG_Value val);

// *****************************************************************************
/* function:
     PLIB_ISI_PSIZE_PREV_VSIZE_Get

    Summary:
      Returns the value of SFR: PREV_VSIZE

    Description:
      Returns the value of SFR: PREV_VSIZE

    Parameters:
      None

    Returns:
      uint32_t

    Remarks:
      None.
*/
uint32_t PLIB_ISI_PSIZE_PREV_VSIZE_Get(void);

// *****************************************************************************
/* function:
     PLIB_ISI_PSIZE_PREV_VSIZE_Set

    Summary:
      Returns the value of SFR: PREV_VSIZE

    Description:
      Returns the value of SFR: PREV_VSIZE

    Parameters:
      uint32_t - val

    Returns:
      void

    Remarks:
      None.
*/
void PLIB_ISI_PSIZE_PREV_VSIZE_Set(uint32_t val);

// *****************************************************************************
/* function:
     PLIB_ISI_PSIZE_PREV_HSIZE_Get

    Summary:
      Returns the value of SFR: PREV_HSIZE

    Description:
      Returns the value of SFR: PREV_HSIZE

    Parameters:
      None

    Returns:
      uint32_t

    Remarks:
      None.
*/
uint32_t PLIB_ISI_PSIZE_PREV_HSIZE_Get(void);

// *****************************************************************************
/* function:
     PLIB_ISI_PSIZE_PREV_HSIZE_Set

    Summary:
      Returns the value of SFR: PREV_HSIZE

    Description:
      Returns the value of SFR: PREV_HSIZE

    Parameters:
      uint32_t - val

    Returns:
      void

    Remarks:
      None.
*/
void PLIB_ISI_PSIZE_PREV_HSIZE_Set(uint32_t val);

// *****************************************************************************
/* function:
     PLIB_ISI_PDECF_DEC_FACTOR_Get

    Summary:
      Returns the value of SFR: DEC_FACTOR

    Description:
      Returns the value of SFR: DEC_FACTOR

    Parameters:
      None

    Returns:
      uint32_t

    Remarks:
      None.
*/
uint32_t PLIB_ISI_PDECF_DEC_FACTOR_Get(void);

// *****************************************************************************
/* function:
     PLIB_ISI_PDECF_DEC_FACTOR_Set

    Summary:
      Returns the value of SFR: DEC_FACTOR

    Description:
      Returns the value of SFR: DEC_FACTOR

    Parameters:
      uint32_t - val

    Returns:
      void

    Remarks:
      None.
*/
void PLIB_ISI_PDECF_DEC_FACTOR_Set(uint32_t val);

// *****************************************************************************
/* function:
     PLIB_ISI_Y2R_SET0_C0_Get

    Summary:
      Returns the value of SFR: C0

    Description:
      Returns the value of SFR: C0

    Parameters:
      None

    Returns:
      uint32_t

    Remarks:
      None.
*/
uint32_t PLIB_ISI_Y2R_SET0_C0_Get(void);

// *****************************************************************************
/* function:
     PLIB_ISI_Y2R_SET0_C0_Set

    Summary:
      Returns the value of SFR: C0

    Description:
      Returns the value of SFR: C0

    Parameters:
      uint32_t - val

    Returns:
      void

    Remarks:
      None.
*/
void PLIB_ISI_Y2R_SET0_C0_Set(uint32_t val);

// *****************************************************************************
/* function:
     PLIB_ISI_Y2R_SET0_C1_Get

    Summary:
      Returns the value of SFR: C1

    Description:
      Returns the value of SFR: C1

    Parameters:
      None

    Returns:
      uint32_t

    Remarks:
      None.
*/
uint32_t PLIB_ISI_Y2R_SET0_C1_Get(void);

// *****************************************************************************
/* function:
     PLIB_ISI_Y2R_SET0_C1_Set

    Summary:
      Returns the value of SFR: C1

    Description:
      Returns the value of SFR: C1

    Parameters:
      uint32_t - val

    Returns:
      void

    Remarks:
      None.
*/
void PLIB_ISI_Y2R_SET0_C1_Set(uint32_t val);

// *****************************************************************************
/* function:
     PLIB_ISI_Y2R_SET0_C2_Get

    Summary:
      Returns the value of SFR: C2

    Description:
      Returns the value of SFR: C2

    Parameters:
      None

    Returns:
      uint32_t

    Remarks:
      None.
*/
uint32_t PLIB_ISI_Y2R_SET0_C2_Get(void);

// *****************************************************************************
/* function:
     PLIB_ISI_Y2R_SET0_C2_Set

    Summary:
      Returns the value of SFR: C2

    Description:
      Returns the value of SFR: C2

    Parameters:
      uint32_t - val

    Returns:
      void

    Remarks:
      None.
*/
void PLIB_ISI_Y2R_SET0_C2_Set(uint32_t val);

// *****************************************************************************
/* function:
     PLIB_ISI_Y2R_SET0_C3_Get

    Summary:
      Returns the value of SFR: C3

    Description:
      Returns the value of SFR: C3

    Parameters:
      None

    Returns:
      uint32_t

    Remarks:
      None.
*/
uint32_t PLIB_ISI_Y2R_SET0_C3_Get(void);

// *****************************************************************************
/* function:
     PLIB_ISI_Y2R_SET0_C3_Set

    Summary:
      Returns the value of SFR: C3

    Description:
      Returns the value of SFR: C3

    Parameters:
      uint32_t - val

    Returns:
      void

    Remarks:
      None.
*/
void PLIB_ISI_Y2R_SET0_C3_Set(uint32_t val);

// *****************************************************************************
/* function:
     PLIB_ISI_Y2R_SET1_C4_Get

    Summary:
      Returns the value of SFR: C4

    Description:
      Returns the value of SFR: C4

    Parameters:
      None

    Returns:
      uint32_t

    Remarks:
      None.
*/
uint32_t PLIB_ISI_Y2R_SET1_C4_Get(void);

// *****************************************************************************
/* function:
     PLIB_ISI_Y2R_SET1_C4_Set

    Summary:
      Returns the value of SFR: C4

    Description:
      Returns the value of SFR: C4

    Parameters:
      uint32_t - val

    Returns:
      void

    Remarks:
      None.
*/
void PLIB_ISI_Y2R_SET1_C4_Set(uint32_t val);

// *****************************************************************************
/* function:
     PLIB_ISI_Y2R_SET1_Yoff_Get

    Summary:
      Returns the value of SFR: Yoff

    Description:
      Returns the value of SFR: Yoff

    Parameters:
      None

    Returns:
      uint32_t

    Remarks:
      None.
*/
uint32_t PLIB_ISI_Y2R_SET1_Yoff_Get(void);

// *****************************************************************************
/* function:
     PLIB_ISI_Y2R_SET1_Yoff_Set

    Summary:
      Returns the value of SFR: Yoff

    Description:
      Returns the value of SFR: Yoff

    Parameters:
      uint32_t - val

    Returns:
      void

    Remarks:
      None.
*/
void PLIB_ISI_Y2R_SET1_Yoff_Set(uint32_t val);

// *****************************************************************************
/* function:
     PLIB_ISI_Y2R_SET1_Croff_Get

    Summary:
      Returns the value of SFR: Croff

    Description:
      Returns the value of SFR: Croff

    Parameters:
      None

    Returns:
      uint32_t

    Remarks:
      None.
*/
uint32_t PLIB_ISI_Y2R_SET1_Croff_Get(void);

// *****************************************************************************
/* function:
     PLIB_ISI_Y2R_SET1_Croff_Set

    Summary:
      Returns the value of SFR: Croff

    Description:
      Returns the value of SFR: Croff

    Parameters:
      uint32_t - val

    Returns:
      void

    Remarks:
      None.
*/
void PLIB_ISI_Y2R_SET1_Croff_Set(uint32_t val);

// *****************************************************************************
/* function:
     PLIB_ISI_Y2R_SET1_Cboff_Get

    Summary:
      Returns the value of SFR: Cboff

    Description:
      Returns the value of SFR: Cboff

    Parameters:
      None

    Returns:
      uint32_t

    Remarks:
      None.
*/
uint32_t PLIB_ISI_Y2R_SET1_Cboff_Get(void);

// *****************************************************************************
/* function:
     PLIB_ISI_Y2R_SET1_Cboff_Set

    Summary:
      Returns the value of SFR: Cboff

    Description:
      Returns the value of SFR: Cboff

    Parameters:
      uint32_t - val

    Returns:
      void

    Remarks:
      None.
*/
void PLIB_ISI_Y2R_SET1_Cboff_Set(uint32_t val);

// *****************************************************************************
/* function:
     PLIB_ISI_R2Y_SET0_C0_Get

    Summary:
      Returns the value of SFR: C0

    Description:
      Returns the value of SFR: C0

    Parameters:
      None

    Returns:
      uint32_t

    Remarks:
      None.
*/
uint32_t PLIB_ISI_R2Y_SET0_C0_Get(void);

// *****************************************************************************
/* function:
     PLIB_ISI_R2Y_SET0_C0_Set

    Summary:
      Returns the value of SFR: C0

    Description:
      Returns the value of SFR: C0

    Parameters:
      uint32_t - val

    Returns:
      void

    Remarks:
      None.
*/
void PLIB_ISI_R2Y_SET0_C0_Set(uint32_t val);

// *****************************************************************************
/* function:
     PLIB_ISI_R2Y_SET0_C1_Get

    Summary:
      Returns the value of SFR: C1

    Description:
      Returns the value of SFR: C1

    Parameters:
      None

    Returns:
      uint32_t

    Remarks:
      None.
*/
uint32_t PLIB_ISI_R2Y_SET0_C1_Get(void);

// *****************************************************************************
/* function:
     PLIB_ISI_R2Y_SET0_C1_Set

    Summary:
      Returns the value of SFR: C1

    Description:
      Returns the value of SFR: C1

    Parameters:
      uint32_t - val

    Returns:
      void

    Remarks:
      None.
*/
void PLIB_ISI_R2Y_SET0_C1_Set(uint32_t val);

// *****************************************************************************
/* function:
     PLIB_ISI_R2Y_SET0_C2_Get

    Summary:
      Returns the value of SFR: C2

    Description:
      Returns the value of SFR: C2

    Parameters:
      None

    Returns:
      uint32_t

    Remarks:
      None.
*/
uint32_t PLIB_ISI_R2Y_SET0_C2_Get(void);

// *****************************************************************************
/* function:
     PLIB_ISI_R2Y_SET0_C2_Set

    Summary:
      Returns the value of SFR: C2

    Description:
      Returns the value of SFR: C2

    Parameters:
      uint32_t - val

    Returns:
      void

    Remarks:
      None.
*/
void PLIB_ISI_R2Y_SET0_C2_Set(uint32_t val);

// *****************************************************************************
/* function:
     PLIB_ISI_R2Y_SET0_Roff_Get

    Summary:
      Returns the value of SFR: Roff

    Description:
      Returns the value of SFR: Roff

    Parameters:
      None

    Returns:
      uint32_t

    Remarks:
      None.
*/
uint32_t PLIB_ISI_R2Y_SET0_Roff_Get(void);

// *****************************************************************************
/* function:
     PLIB_ISI_R2Y_SET0_Roff_Set

    Summary:
      Returns the value of SFR: Roff

    Description:
      Returns the value of SFR: Roff

    Parameters:
      uint32_t - val

    Returns:
      void

    Remarks:
      None.
*/
void PLIB_ISI_R2Y_SET0_Roff_Set(uint32_t val);

// *****************************************************************************
/* function:
     PLIB_ISI_R2Y_SET1_C3_Get

    Summary:
      Returns the value of SFR: C3

    Description:
      Returns the value of SFR: C3

    Parameters:
      None

    Returns:
      uint32_t

    Remarks:
      None.
*/
uint32_t PLIB_ISI_R2Y_SET1_C3_Get(void);

// *****************************************************************************
/* function:
     PLIB_ISI_R2Y_SET1_C3_Set

    Summary:
      Returns the value of SFR: C3

    Description:
      Returns the value of SFR: C3

    Parameters:
      uint32_t - val

    Returns:
      void

    Remarks:
      None.
*/
void PLIB_ISI_R2Y_SET1_C3_Set(uint32_t val);

// *****************************************************************************
/* function:
     PLIB_ISI_R2Y_SET1_C4_Get

    Summary:
      Returns the value of SFR: C4

    Description:
      Returns the value of SFR: C4

    Parameters:
      None

    Returns:
      uint32_t

    Remarks:
      None.
*/
uint32_t PLIB_ISI_R2Y_SET1_C4_Get(void);

// *****************************************************************************
/* function:
     PLIB_ISI_R2Y_SET1_C4_Set

    Summary:
      Returns the value of SFR: C4

    Description:
      Returns the value of SFR: C4

    Parameters:
      uint32_t - val

    Returns:
      void

    Remarks:
      None.
*/
void PLIB_ISI_R2Y_SET1_C4_Set(uint32_t val);

// *****************************************************************************
/* function:
     PLIB_ISI_R2Y_SET1_C5_Get

    Summary:
      Returns the value of SFR: C5

    Description:
      Returns the value of SFR: C5

    Parameters:
      None

    Returns:
      uint32_t

    Remarks:
      None.
*/
uint32_t PLIB_ISI_R2Y_SET1_C5_Get(void);

// *****************************************************************************
/* function:
     PLIB_ISI_R2Y_SET1_C5_Set

    Summary:
      Returns the value of SFR: C5

    Description:
      Returns the value of SFR: C5

    Parameters:
      uint32_t - val

    Returns:
      void

    Remarks:
      None.
*/
void PLIB_ISI_R2Y_SET1_C5_Set(uint32_t val);

// *****************************************************************************
/* function:
     PLIB_ISI_R2Y_SET1_Goff_Get

    Summary:
      Returns the value of SFR: Goff

    Description:
      Returns the value of SFR: Goff

    Parameters:
      None

    Returns:
      uint32_t

    Remarks:
      None.
*/
uint32_t PLIB_ISI_R2Y_SET1_Goff_Get(void);

// *****************************************************************************
/* function:
     PLIB_ISI_R2Y_SET1_Goff_Set

    Summary:
      Returns the value of SFR: Goff

    Description:
      Returns the value of SFR: Goff

    Parameters:
      uint32_t - val

    Returns:
      void

    Remarks:
      None.
*/
void PLIB_ISI_R2Y_SET1_Goff_Set(uint32_t val);

// *****************************************************************************
/* function:
     PLIB_ISI_R2Y_SET2_C6_Get

    Summary:
      Returns the value of SFR: C6

    Description:
      Returns the value of SFR: C6

    Parameters:
      None

    Returns:
      uint32_t

    Remarks:
      None.
*/
uint32_t PLIB_ISI_R2Y_SET2_C6_Get(void);

// *****************************************************************************
/* function:
     PLIB_ISI_R2Y_SET2_C6_Set

    Summary:
      Returns the value of SFR: C6

    Description:
      Returns the value of SFR: C6

    Parameters:
      uint32_t - val

    Returns:
      void

    Remarks:
      None.
*/
void PLIB_ISI_R2Y_SET2_C6_Set(uint32_t val);

// *****************************************************************************
/* function:
     PLIB_ISI_R2Y_SET2_C7_Get

    Summary:
      Returns the value of SFR: C7

    Description:
      Returns the value of SFR: C7

    Parameters:
      None

    Returns:
      uint32_t

    Remarks:
      None.
*/
uint32_t PLIB_ISI_R2Y_SET2_C7_Get(void);

// *****************************************************************************
/* function:
     PLIB_ISI_R2Y_SET2_C7_Set

    Summary:
      Returns the value of SFR: C7

    Description:
      Returns the value of SFR: C7

    Parameters:
      uint32_t - val

    Returns:
      void

    Remarks:
      None.
*/
void PLIB_ISI_R2Y_SET2_C7_Set(uint32_t val);

// *****************************************************************************
/* function:
     PLIB_ISI_R2Y_SET2_C8_Get

    Summary:
      Returns the value of SFR: C8

    Description:
      Returns the value of SFR: C8

    Parameters:
      None

    Returns:
      uint32_t

    Remarks:
      None.
*/
uint32_t PLIB_ISI_R2Y_SET2_C8_Get(void);

// *****************************************************************************
/* function:
     PLIB_ISI_R2Y_SET2_C8_Set

    Summary:
      Returns the value of SFR: C8

    Description:
      Returns the value of SFR: C8

    Parameters:
      uint32_t - val

    Returns:
      void

    Remarks:
      None.
*/
void PLIB_ISI_R2Y_SET2_C8_Set(uint32_t val);

// *****************************************************************************
/* function:
     PLIB_ISI_R2Y_SET2_Boff_Get

    Summary:
      Returns the value of SFR: Boff

    Description:
      Returns the value of SFR: Boff

    Parameters:
      None

    Returns:
      uint32_t

    Remarks:
      None.
*/
uint32_t PLIB_ISI_R2Y_SET2_Boff_Get(void);

// *****************************************************************************
/* function:
     PLIB_ISI_R2Y_SET2_Boff_Set

    Summary:
      Returns the value of SFR: Boff

    Description:
      Returns the value of SFR: Boff

    Parameters:
      uint32_t - val

    Returns:
      void

    Remarks:
      None.
*/
void PLIB_ISI_R2Y_SET2_Boff_Set(uint32_t val);

// *****************************************************************************
/* function:
     PLIB_ISI_CR_ISI_EN_Set

    Summary:
      Returns the value of SFR: ISI_EN

    Description:
      Returns the value of SFR: ISI_EN

    Parameters:
      uint32_t - val

    Returns:
      void

    Remarks:
      None.
*/
void PLIB_ISI_CR_ISI_EN_Set(uint32_t val);

// *****************************************************************************
/* function:
     PLIB_ISI_CR_ISI_DIS_Set

    Summary:
      Returns the value of SFR: ISI_DIS

    Description:
      Returns the value of SFR: ISI_DIS

    Parameters:
      uint32_t - val

    Returns:
      void

    Remarks:
      None.
*/
void PLIB_ISI_CR_ISI_DIS_Set(uint32_t val);

// *****************************************************************************
/* function:
     PLIB_ISI_CR_ISI_SRST_Set

    Summary:
      Returns the value of SFR: ISI_SRST

    Description:
      Returns the value of SFR: ISI_SRST

    Parameters:
      uint32_t - val

    Returns:
      void

    Remarks:
      None.
*/
void PLIB_ISI_CR_ISI_SRST_Set(uint32_t val);

// *****************************************************************************
/* function:
     PLIB_ISI_CR_ISI_CDC_Set

    Summary:
      Returns the value of SFR: ISI_CDC

    Description:
      Returns the value of SFR: ISI_CDC

    Parameters:
      uint32_t - val

    Returns:
      void

    Remarks:
      None.
*/
void PLIB_ISI_CR_ISI_CDC_Set(uint32_t val);

// *****************************************************************************
/* function:
     PLIB_ISI_SR_ENABLE_Get

    Summary:
      Returns the value of SFR: ENABLE

    Description:
      Returns the value of SFR: ENABLE

    Parameters:
      None

    Returns:
      uint32_t

    Remarks:
      None.
*/
uint32_t PLIB_ISI_SR_ENABLE_Get(void);

// *****************************************************************************
/* function:
     PLIB_ISI_SR_DIS_DONE_Get

    Summary:
      Returns the value of SFR: DIS_DONE

    Description:
      Returns the value of SFR: DIS_DONE

    Parameters:
      None

    Returns:
      uint32_t

    Remarks:
      None.
*/
uint32_t PLIB_ISI_SR_DIS_DONE_Get(void);

// *****************************************************************************
/* function:
     PLIB_ISI_SR_SRST_Get

    Summary:
      Returns the value of SFR: SRST

    Description:
      Returns the value of SFR: SRST

    Parameters:
      None

    Returns:
      uint32_t

    Remarks:
      None.
*/
uint32_t PLIB_ISI_SR_SRST_Get(void);

// *****************************************************************************
/* function:
     PLIB_ISI_SR_CDC_PND_Get

    Summary:
      Returns the value of SFR: CDC_PND

    Description:
      Returns the value of SFR: CDC_PND

    Parameters:
      None

    Returns:
      uint32_t

    Remarks:
      None.
*/
uint32_t PLIB_ISI_SR_CDC_PND_Get(void);

// *****************************************************************************
/* function:
     PLIB_ISI_SR_VSYNC_Get

    Summary:
      Returns the value of SFR: VSYNC

    Description:
      Returns the value of SFR: VSYNC

    Parameters:
      None

    Returns:
      uint32_t

    Remarks:
      None.
*/
uint32_t PLIB_ISI_SR_VSYNC_Get(void);

// *****************************************************************************
/* function:
     PLIB_ISI_SR_PXFR_DONE_Get

    Summary:
      Returns the value of SFR: PXFR_DONE

    Description:
      Returns the value of SFR: PXFR_DONE

    Parameters:
      None

    Returns:
      uint32_t

    Remarks:
      None.
*/
uint32_t PLIB_ISI_SR_PXFR_DONE_Get(void);

// *****************************************************************************
/* function:
     PLIB_ISI_SR_CXFR_DONE_Get

    Summary:
      Returns the value of SFR: CXFR_DONE

    Description:
      Returns the value of SFR: CXFR_DONE

    Parameters:
      None

    Returns:
      uint32_t

    Remarks:
      None.
*/
uint32_t PLIB_ISI_SR_CXFR_DONE_Get(void);

// *****************************************************************************
/* function:
     PLIB_ISI_SR_SIP_Get

    Summary:
      Returns the value of SFR: SIP

    Description:
      Returns the value of SFR: SIP

    Parameters:
      None

    Returns:
      uint32_t

    Remarks:
      None.
*/
uint32_t PLIB_ISI_SR_SIP_Get(void);

// *****************************************************************************
/* function:
     PLIB_ISI_SR_P_OVR_Get

    Summary:
      Returns the value of SFR: P_OVR

    Description:
      Returns the value of SFR: P_OVR

    Parameters:
      None

    Returns:
      uint32_t

    Remarks:
      None.
*/
uint32_t PLIB_ISI_SR_P_OVR_Get(void);

// *****************************************************************************
/* function:
     PLIB_ISI_SR_C_OVR_Get

    Summary:
      Returns the value of SFR: C_OVR

    Description:
      Returns the value of SFR: C_OVR

    Parameters:
      None

    Returns:
      uint32_t

    Remarks:
      None.
*/
uint32_t PLIB_ISI_SR_C_OVR_Get(void);

// *****************************************************************************
/* function:
     PLIB_ISI_SR_CRC_ERR_Get

    Summary:
      Returns the value of SFR: CRC_ERR

    Description:
      Returns the value of SFR: CRC_ERR

    Parameters:
      None

    Returns:
      uint32_t

    Remarks:
      None.
*/
uint32_t PLIB_ISI_SR_CRC_ERR_Get(void);

// *****************************************************************************
/* function:
     PLIB_ISI_SR_FR_OVR_Get

    Summary:
      Returns the value of SFR: FR_OVR

    Description:
      Returns the value of SFR: FR_OVR

    Parameters:
      None

    Returns:
      uint32_t

    Remarks:
      None.
*/
uint32_t PLIB_ISI_SR_FR_OVR_Get(void);

// *****************************************************************************
/* function:
     PLIB_ISI_IER_DIS_DONE_Set

    Summary:
      Returns the value of SFR: DIS_DONE

    Description:
      Returns the value of SFR: DIS_DONE

    Parameters:
      uint32_t - val

    Returns:
      void

    Remarks:
      None.
*/
void PLIB_ISI_IER_DIS_DONE_Set(uint32_t val);

// *****************************************************************************
/* function:
     PLIB_ISI_IER_SRST_Set

    Summary:
      Returns the value of SFR: SRST

    Description:
      Returns the value of SFR: SRST

    Parameters:
      uint32_t - val

    Returns:
      void

    Remarks:
      None.
*/
void PLIB_ISI_IER_SRST_Set(uint32_t val);

// *****************************************************************************
/* function:
     PLIB_ISI_IER_VSYNC_Set

    Summary:
      Returns the value of SFR: VSYNC

    Description:
      Returns the value of SFR: VSYNC

    Parameters:
      uint32_t - val

    Returns:
      void

    Remarks:
      None.
*/
void PLIB_ISI_IER_VSYNC_Set(uint32_t val);

// *****************************************************************************
/* function:
     PLIB_ISI_IER_PXFR_DONE_Set

    Summary:
      Returns the value of SFR: PXFR_DONE

    Description:
      Returns the value of SFR: PXFR_DONE

    Parameters:
      uint32_t - val

    Returns:
      void

    Remarks:
      None.
*/
void PLIB_ISI_IER_PXFR_DONE_Set(uint32_t val);

// *****************************************************************************
/* function:
     PLIB_ISI_IER_CXFR_DONE_Set

    Summary:
      Returns the value of SFR: CXFR_DONE

    Description:
      Returns the value of SFR: CXFR_DONE

    Parameters:
      uint32_t - val

    Returns:
      void

    Remarks:
      None.
*/
void PLIB_ISI_IER_CXFR_DONE_Set(uint32_t val);

// *****************************************************************************
/* function:
     PLIB_ISI_IER_P_OVR_Set

    Summary:
      Returns the value of SFR: P_OVR

    Description:
      Returns the value of SFR: P_OVR

    Parameters:
      uint32_t - val

    Returns:
      void

    Remarks:
      None.
*/
void PLIB_ISI_IER_P_OVR_Set(uint32_t val);

// *****************************************************************************
/* function:
     PLIB_ISI_IER_C_OVR_Set

    Summary:
      Returns the value of SFR: C_OVR

    Description:
      Returns the value of SFR: C_OVR

    Parameters:
      uint32_t - val

    Returns:
      void

    Remarks:
      None.
*/
void PLIB_ISI_IER_C_OVR_Set(uint32_t val);

// *****************************************************************************
/* function:
     PLIB_ISI_IER_CRC_ERR_Set

    Summary:
      Returns the value of SFR: CRC_ERR

    Description:
      Returns the value of SFR: CRC_ERR

    Parameters:
      uint32_t - val

    Returns:
      void

    Remarks:
      None.
*/
void PLIB_ISI_IER_CRC_ERR_Set(uint32_t val);

// *****************************************************************************
/* function:
     PLIB_ISI_IER_FR_OVR_Set

    Summary:
      Returns the value of SFR: FR_OVR

    Description:
      Returns the value of SFR: FR_OVR

    Parameters:
      uint32_t - val

    Returns:
      void

    Remarks:
      None.
*/
void PLIB_ISI_IER_FR_OVR_Set(uint32_t val);

// *****************************************************************************
/* function:
     PLIB_ISI_IDR_DIS_DONE_Set

    Summary:
      Returns the value of SFR: DIS_DONE

    Description:
      Returns the value of SFR: DIS_DONE

    Parameters:
      uint32_t - val

    Returns:
      void

    Remarks:
      None.
*/
void PLIB_ISI_IDR_DIS_DONE_Set(uint32_t val);

// *****************************************************************************
/* function:
     PLIB_ISI_IDR_SRST_Set

    Summary:
      Returns the value of SFR: SRST

    Description:
      Returns the value of SFR: SRST

    Parameters:
      uint32_t - val

    Returns:
      void

    Remarks:
      None.
*/
void PLIB_ISI_IDR_SRST_Set(uint32_t val);

// *****************************************************************************
/* function:
     PLIB_ISI_IDR_VSYNC_Set

    Summary:
      Returns the value of SFR: VSYNC

    Description:
      Returns the value of SFR: VSYNC

    Parameters:
      uint32_t - val

    Returns:
      void

    Remarks:
      None.
*/
void PLIB_ISI_IDR_VSYNC_Set(uint32_t val);

// *****************************************************************************
/* function:
     PLIB_ISI_IDR_PXFR_DONE_Set

    Summary:
      Returns the value of SFR: PXFR_DONE

    Description:
      Returns the value of SFR: PXFR_DONE

    Parameters:
      uint32_t - val

    Returns:
      void

    Remarks:
      None.
*/
void PLIB_ISI_IDR_PXFR_DONE_Set(uint32_t val);

// *****************************************************************************
/* function:
     PLIB_ISI_IDR_CXFR_DONE_Set

    Summary:
      Returns the value of SFR: CXFR_DONE

    Description:
      Returns the value of SFR: CXFR_DONE

    Parameters:
      uint32_t - val

    Returns:
      void

    Remarks:
      None.
*/
void PLIB_ISI_IDR_CXFR_DONE_Set(uint32_t val);

// *****************************************************************************
/* function:
     PLIB_ISI_IDR_P_OVR_Set

    Summary:
      Returns the value of SFR: P_OVR

    Description:
      Returns the value of SFR: P_OVR

    Parameters:
      uint32_t - val

    Returns:
      void

    Remarks:
      None.
*/
void PLIB_ISI_IDR_P_OVR_Set(uint32_t val);

// *****************************************************************************
/* function:
     PLIB_ISI_IDR_C_OVR_Set

    Summary:
      Returns the value of SFR: C_OVR

    Description:
      Returns the value of SFR: C_OVR

    Parameters:
      uint32_t - val

    Returns:
      void

    Remarks:
      None.
*/
void PLIB_ISI_IDR_C_OVR_Set(uint32_t val);

// *****************************************************************************
/* function:
     PLIB_ISI_IDR_CRC_ERR_Set

    Summary:
      Returns the value of SFR: CRC_ERR

    Description:
      Returns the value of SFR: CRC_ERR

    Parameters:
      uint32_t - val

    Returns:
      void

    Remarks:
      None.
*/
void PLIB_ISI_IDR_CRC_ERR_Set(uint32_t val);

// *****************************************************************************
/* function:
     PLIB_ISI_IDR_FR_OVR_Set

    Summary:
      Returns the value of SFR: FR_OVR

    Description:
      Returns the value of SFR: FR_OVR

    Parameters:
      uint32_t - val

    Returns:
      void

    Remarks:
      None.
*/
void PLIB_ISI_IDR_FR_OVR_Set(uint32_t val);

// *****************************************************************************
/* function:
     PLIB_ISI_IMR_DIS_DONE_Get

    Summary:
      Returns the value of SFR: DIS_DONE

    Description:
      Returns the value of SFR: DIS_DONE

    Parameters:
      None

    Returns:
      uint32_t

    Remarks:
      None.
*/
uint32_t PLIB_ISI_IMR_DIS_DONE_Get(void);

// *****************************************************************************
/* function:
     PLIB_ISI_IMR_SRST_Get

    Summary:
      Returns the value of SFR: SRST

    Description:
      Returns the value of SFR: SRST

    Parameters:
      None

    Returns:
      uint32_t

    Remarks:
      None.
*/
uint32_t PLIB_ISI_IMR_SRST_Get(void);

// *****************************************************************************
/* function:
     PLIB_ISI_IMR_VSYNC_Get

    Summary:
      Returns the value of SFR: VSYNC

    Description:
      Returns the value of SFR: VSYNC

    Parameters:
      None

    Returns:
      uint32_t

    Remarks:
      None.
*/
uint32_t PLIB_ISI_IMR_VSYNC_Get(void);

// *****************************************************************************
/* function:
     PLIB_ISI_IMR_PXFR_DONE_Get

    Summary:
      Returns the value of SFR: PXFR_DONE

    Description:
      Returns the value of SFR: PXFR_DONE

    Parameters:
      None

    Returns:
      uint32_t

    Remarks:
      None.
*/
uint32_t PLIB_ISI_IMR_PXFR_DONE_Get(void);

// *****************************************************************************
/* function:
     PLIB_ISI_IMR_CXFR_DONE_Get

    Summary:
      Returns the value of SFR: CXFR_DONE

    Description:
      Returns the value of SFR: CXFR_DONE

    Parameters:
      None

    Returns:
      uint32_t

    Remarks:
      None.
*/
uint32_t PLIB_ISI_IMR_CXFR_DONE_Get(void);

// *****************************************************************************
/* function:
     PLIB_ISI_IMR_P_OVR_Get

    Summary:
      Returns the value of SFR: P_OVR

    Description:
      Returns the value of SFR: P_OVR

    Parameters:
      None

    Returns:
      uint32_t

    Remarks:
      None.
*/
uint32_t PLIB_ISI_IMR_P_OVR_Get(void);

// *****************************************************************************
/* function:
     PLIB_ISI_IMR_C_OVR_Get

    Summary:
      Returns the value of SFR: C_OVR

    Description:
      Returns the value of SFR: C_OVR

    Parameters:
      None

    Returns:
      uint32_t

    Remarks:
      None.
*/
uint32_t PLIB_ISI_IMR_C_OVR_Get(void);

// *****************************************************************************
/* function:
     PLIB_ISI_IMR_CRC_ERR_Get

    Summary:
      Returns the value of SFR: CRC_ERR

    Description:
      Returns the value of SFR: CRC_ERR

    Parameters:
      None

    Returns:
      uint32_t

    Remarks:
      None.
*/
uint32_t PLIB_ISI_IMR_CRC_ERR_Get(void);

// *****************************************************************************
/* function:
     PLIB_ISI_IMR_FR_OVR_Get

    Summary:
      Returns the value of SFR: FR_OVR

    Description:
      Returns the value of SFR: FR_OVR

    Parameters:
      None

    Returns:
      uint32_t

    Remarks:
      None.
*/
uint32_t PLIB_ISI_IMR_FR_OVR_Get(void);

// *****************************************************************************
/* function:
     PLIB_ISI_DMA_CHER_P_CH_EN_Set

    Summary:
      Returns the value of SFR: P_CH_EN

    Description:
      Returns the value of SFR: P_CH_EN

    Parameters:
      uint32_t - val

    Returns:
      void

    Remarks:
      None.
*/
void PLIB_ISI_DMA_CHER_P_CH_EN_Set(uint32_t val);

// *****************************************************************************
/* function:
     PLIB_ISI_DMA_CHER_C_CH_EN_Set

    Summary:
      Returns the value of SFR: C_CH_EN

    Description:
      Returns the value of SFR: C_CH_EN

    Parameters:
      uint32_t - val

    Returns:
      void

    Remarks:
      None.
*/
void PLIB_ISI_DMA_CHER_C_CH_EN_Set(uint32_t val);

// *****************************************************************************
/* function:
     PLIB_ISI_DMA_CHDR_P_CH_DIS_Set

    Summary:
      Returns the value of SFR: P_CH_DIS

    Description:
      Returns the value of SFR: P_CH_DIS

    Parameters:
      uint32_t - val

    Returns:
      void

    Remarks:
      None.
*/
void PLIB_ISI_DMA_CHDR_P_CH_DIS_Set(uint32_t val);

// *****************************************************************************
/* function:
     PLIB_ISI_DMA_CHDR_C_CH_DIS_Set

    Summary:
      Returns the value of SFR: C_CH_DIS

    Description:
      Returns the value of SFR: C_CH_DIS

    Parameters:
      uint32_t - val

    Returns:
      void

    Remarks:
      None.
*/
void PLIB_ISI_DMA_CHDR_C_CH_DIS_Set(uint32_t val);

// *****************************************************************************
/* function:
     PLIB_ISI_DMA_CHSR_P_CH_S_Get

    Summary:
      Returns the value of SFR: P_CH_S

    Description:
      Returns the value of SFR: P_CH_S

    Parameters:
      None

    Returns:
      uint32_t

    Remarks:
      None.
*/
uint32_t PLIB_ISI_DMA_CHSR_P_CH_S_Get(void);

// *****************************************************************************
/* function:
     PLIB_ISI_DMA_CHSR_C_CH_S_Get

    Summary:
      Returns the value of SFR: C_CH_S

    Description:
      Returns the value of SFR: C_CH_S

    Parameters:
      None

    Returns:
      uint32_t

    Remarks:
      None.
*/
uint32_t PLIB_ISI_DMA_CHSR_C_CH_S_Get(void);

// *****************************************************************************
/* function:
     PLIB_ISI_DMA_P_ADDR_Get

    Summary:
      Returns the value of SFR: P_ADDR

    Description:
      Returns the value of SFR: P_ADDR

    Parameters:
      None

    Returns:
      uint32_t

    Remarks:
      None.
*/
uint32_t PLIB_ISI_DMA_P_ADDR_Get(void);

// *****************************************************************************
/* function:
     PLIB_ISI_DMA_P_ADDR_Set

    Summary:
      Returns the value of SFR: P_ADDR

    Description:
      Returns the value of SFR: P_ADDR

    Parameters:
      uint32_t - val

    Returns:
      void

    Remarks:
      None.
*/
void PLIB_ISI_DMA_P_ADDR_Set(uint32_t val);

// *****************************************************************************
/* function:
     PLIB_ISI_DMA_P_CTRL_P_FETCH_Get

    Summary:
      Returns the value of SFR: P_FETCH

    Description:
      Returns the value of SFR: P_FETCH

    Parameters:
      None

    Returns:
      uint32_t

    Remarks:
      None.
*/
uint32_t PLIB_ISI_DMA_P_CTRL_P_FETCH_Get(void);

// *****************************************************************************
/* function:
     PLIB_ISI_DMA_P_CTRL_P_FETCH_Set

    Summary:
      Returns the value of SFR: P_FETCH

    Description:
      Returns the value of SFR: P_FETCH

    Parameters:
      uint32_t - val

    Returns:
      void

    Remarks:
      None.
*/
void PLIB_ISI_DMA_P_CTRL_P_FETCH_Set(uint32_t val);

// *****************************************************************************
/* function:
     PLIB_ISI_DMA_P_CTRL_P_WB_Get

    Summary:
      Returns the value of SFR: P_WB

    Description:
      Returns the value of SFR: P_WB

    Parameters:
      None

    Returns:
      uint32_t

    Remarks:
      None.
*/
uint32_t PLIB_ISI_DMA_P_CTRL_P_WB_Get(void);

// *****************************************************************************
/* function:
     PLIB_ISI_DMA_P_CTRL_P_WB_Set

    Summary:
      Returns the value of SFR: P_WB

    Description:
      Returns the value of SFR: P_WB

    Parameters:
      uint32_t - val

    Returns:
      void

    Remarks:
      None.
*/
void PLIB_ISI_DMA_P_CTRL_P_WB_Set(uint32_t val);

// *****************************************************************************
/* function:
     PLIB_ISI_DMA_P_CTRL_P_IEN_Get

    Summary:
      Returns the value of SFR: P_IEN

    Description:
      Returns the value of SFR: P_IEN

    Parameters:
      None

    Returns:
      uint32_t

    Remarks:
      None.
*/
uint32_t PLIB_ISI_DMA_P_CTRL_P_IEN_Get(void);

// *****************************************************************************
/* function:
     PLIB_ISI_DMA_P_CTRL_P_IEN_Set

    Summary:
      Returns the value of SFR: P_IEN

    Description:
      Returns the value of SFR: P_IEN

    Parameters:
      uint32_t - val

    Returns:
      void

    Remarks:
      None.
*/
void PLIB_ISI_DMA_P_CTRL_P_IEN_Set(uint32_t val);

// *****************************************************************************
/* function:
     PLIB_ISI_DMA_P_CTRL_P_DONE_Get

    Summary:
      Returns the value of SFR: P_DONE

    Description:
      Returns the value of SFR: P_DONE

    Parameters:
      None

    Returns:
      uint32_t

    Remarks:
      None.
*/
uint32_t PLIB_ISI_DMA_P_CTRL_P_DONE_Get(void);

// *****************************************************************************
/* function:
     PLIB_ISI_DMA_P_CTRL_P_DONE_Set

    Summary:
      Returns the value of SFR: P_DONE

    Description:
      Returns the value of SFR: P_DONE

    Parameters:
      uint32_t - val

    Returns:
      void

    Remarks:
      None.
*/
void PLIB_ISI_DMA_P_CTRL_P_DONE_Set(uint32_t val);

// *****************************************************************************
/* function:
     PLIB_ISI_DMA_P_DSCR_Get

    Summary:
      Returns the value of SFR: P_DSCR

    Description:
      Returns the value of SFR: P_DSCR

    Parameters:
      None

    Returns:
      uint32_t

    Remarks:
      None.
*/
uint32_t PLIB_ISI_DMA_P_DSCR_Get(void);

// *****************************************************************************
/* function:
     PLIB_ISI_DMA_P_DSCR_Set

    Summary:
      Returns the value of SFR: P_DSCR

    Description:
      Returns the value of SFR: P_DSCR

    Parameters:
      uint32_t - val

    Returns:
      void

    Remarks:
      None.
*/
void PLIB_ISI_DMA_P_DSCR_Set(uint32_t val);

// *****************************************************************************
/* function:
     PLIB_ISI_DMA_C_ADDR_Get

    Summary:
      Returns the value of SFR: C_ADDR

    Description:
      Returns the value of SFR: C_ADDR

    Parameters:
      None

    Returns:
      uint32_t

    Remarks:
      None.
*/
uint32_t PLIB_ISI_DMA_C_ADDR_Get(void);

// *****************************************************************************
/* function:
     PLIB_ISI_DMA_C_ADDR_Set

    Summary:
      Returns the value of SFR: C_ADDR

    Description:
      Returns the value of SFR: C_ADDR

    Parameters:
      uint32_t - val

    Returns:
      void

    Remarks:
      None.
*/
void PLIB_ISI_DMA_C_ADDR_Set(uint32_t val);

// *****************************************************************************
/* function:
     PLIB_ISI_DMA_C_CTRL_C_FETCH_Get

    Summary:
      Returns the value of SFR: C_FETCH

    Description:
      Returns the value of SFR: C_FETCH

    Parameters:
      None

    Returns:
      uint32_t

    Remarks:
      None.
*/
uint32_t PLIB_ISI_DMA_C_CTRL_C_FETCH_Get(void);

// *****************************************************************************
/* function:
     PLIB_ISI_DMA_C_CTRL_C_FETCH_Set

    Summary:
      Returns the value of SFR: C_FETCH

    Description:
      Returns the value of SFR: C_FETCH

    Parameters:
      uint32_t - val

    Returns:
      void

    Remarks:
      None.
*/
void PLIB_ISI_DMA_C_CTRL_C_FETCH_Set(uint32_t val);

// *****************************************************************************
/* function:
     PLIB_ISI_DMA_C_CTRL_C_WB_Get

    Summary:
      Returns the value of SFR: C_WB

    Description:
      Returns the value of SFR: C_WB

    Parameters:
      None

    Returns:
      uint32_t

    Remarks:
      None.
*/
uint32_t PLIB_ISI_DMA_C_CTRL_C_WB_Get(void);

// *****************************************************************************
/* function:
     PLIB_ISI_DMA_C_CTRL_C_WB_Set

    Summary:
      Returns the value of SFR: C_WB

    Description:
      Returns the value of SFR: C_WB

    Parameters:
      uint32_t - val

    Returns:
      void

    Remarks:
      None.
*/
void PLIB_ISI_DMA_C_CTRL_C_WB_Set(uint32_t val);

// *****************************************************************************
/* function:
     PLIB_ISI_DMA_C_CTRL_C_IEN_Get

    Summary:
      Returns the value of SFR: C_IEN

    Description:
      Returns the value of SFR: C_IEN

    Parameters:
      None

    Returns:
      uint32_t

    Remarks:
      None.
*/
uint32_t PLIB_ISI_DMA_C_CTRL_C_IEN_Get(void);

// *****************************************************************************
/* function:
     PLIB_ISI_DMA_C_CTRL_C_IEN_Set

    Summary:
      Returns the value of SFR: C_IEN

    Description:
      Returns the value of SFR: C_IEN

    Parameters:
      uint32_t - val

    Returns:
      void

    Remarks:
      None.
*/
void PLIB_ISI_DMA_C_CTRL_C_IEN_Set(uint32_t val);

// *****************************************************************************
/* function:
     PLIB_ISI_DMA_C_CTRL_C_DONE_Get

    Summary:
      Returns the value of SFR: C_DONE

    Description:
      Returns the value of SFR: C_DONE

    Parameters:
      None

    Returns:
      uint32_t

    Remarks:
      None.
*/
uint32_t PLIB_ISI_DMA_C_CTRL_C_DONE_Get(void);

// *****************************************************************************
/* function:
     PLIB_ISI_DMA_C_CTRL_C_DONE_Set

    Summary:
      Returns the value of SFR: C_DONE

    Description:
      Returns the value of SFR: C_DONE

    Parameters:
      uint32_t - val

    Returns:
      void

    Remarks:
      None.
*/
void PLIB_ISI_DMA_C_CTRL_C_DONE_Set(uint32_t val);

// *****************************************************************************
/* function:
     PLIB_ISI_DMA_C_DSCR_C_DSCR_Get

    Summary:
      Returns the value of SFR: C_DSCR

    Description:
      Returns the value of SFR: C_DSCR

    Parameters:
      None

    Returns:
      uint32_t

    Remarks:
      None.
*/
uint32_t PLIB_ISI_DMA_C_DSCR_Get(void);

// *****************************************************************************
/* function:
     PLIB_ISI_DMA_C_DSCR_Set

    Summary:
      Returns the value of SFR: C_DSCR

    Description:
      Returns the value of SFR: C_DSCR

    Parameters:
      uint32_t - val

    Returns:
      void

    Remarks:
      None.
*/
void PLIB_ISI_DMA_C_DSCR_Set(uint32_t val);

// *****************************************************************************
/* function:
     PLIB_ISI_WPMR_WPEN_Get

    Summary:
      Returns the value of SFR: WPEN

    Description:
      Returns the value of SFR: WPEN

    Parameters:
      None

    Returns:
      uint32_t

    Remarks:
      None.
*/
uint32_t PLIB_ISI_WPMR_WPEN_Get(void);

// *****************************************************************************
/* function:
     PLIB_ISI_WPMR_WPEN_Set

    Summary:
      Returns the value of SFR: WPEN

    Description:
      Returns the value of SFR: WPEN

    Parameters:
      uint32_t - val

    Returns:
      void

    Remarks:
      None.
*/
void PLIB_ISI_WPMR_WPEN_Set(uint32_t val);

// *****************************************************************************
/* function:
     PLIB_ISI_WPMR_WPKEY_Get

    Summary:
      Returns the value of SFR: WPKEY

    Description:
      Returns the value of SFR: WPKEY

    Parameters:
      None

    Returns:
      ISI_WPKEY_Value

    Remarks:
      None.
*/
ISI_WPKEY_Value PLIB_ISI_WPMR_WPKEY_Get(void);

// *****************************************************************************
/* function:
     PLIB_ISI_WPMR_WPKEY_Set

    Summary:
      Returns the value of SFR: WPKEY

    Description:
      Returns the value of SFR: WPKEY

    Parameters:
      ISI_WPKEY_Value - val

    Returns:
      void

    Remarks:
      None.
*/
void PLIB_ISI_WPMR_WPKEY_Set(ISI_WPKEY_Value val);

// *****************************************************************************
/* function:
     PLIB_ISI_WPSR_WPVS_Get

    Summary:
      Returns the value of SFR: WPVS

    Description:
      Returns the value of SFR: WPVS

    Parameters:
      None

    Returns:
      uint32_t

    Remarks:
      None.
*/
uint32_t PLIB_ISI_WPSR_WPVS_Get(void);

// *****************************************************************************
/* function:
     PLIB_ISI_WPSR_WPVSRC_Get

    Summary:
      Returns the value of SFR: WPVSRC

    Description:
      Returns the value of SFR: WPVSRC

    Parameters:
      None

    Returns:
      uint32_t

    Remarks:
      None.
*/
uint32_t PLIB_ISI_WPSR_WPVSRC_Get(void);

// DOM-IGNORE-BEGIN
#ifdef __cplusplus
}
#endif
// DOM-IGNORE-END

#endif // PLIB_ISI_H
