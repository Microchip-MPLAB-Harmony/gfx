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


#ifndef GUIDRV_LIN_H
#define GUIDRV_LIN_H

#if defined(__cplusplus)
extern "C" {     /* Make sure we have C-declarations in C++ programs */
#endif

/*********************************************************************
*
*       Display drivers
*/
//
// Addresses
//
extern const GUI_DEVICE_API GUIDRV_Win_API;

extern const GUI_DEVICE_API GUIDRV_Lin_1_API;
extern const GUI_DEVICE_API GUIDRV_Lin_OY_1_API;
extern const GUI_DEVICE_API GUIDRV_Lin_OX_1_API;
extern const GUI_DEVICE_API GUIDRV_Lin_OXY_1_API;
extern const GUI_DEVICE_API GUIDRV_Lin_OS_1_API;
extern const GUI_DEVICE_API GUIDRV_Lin_OSY_1_API;
extern const GUI_DEVICE_API GUIDRV_Lin_OSX_1_API;
extern const GUI_DEVICE_API GUIDRV_Lin_OSXY_1_API;

extern const GUI_DEVICE_API GUIDRV_Lin_2_API;
extern const GUI_DEVICE_API GUIDRV_Lin_OY_2_API;
extern const GUI_DEVICE_API GUIDRV_Lin_OX_2_API;
extern const GUI_DEVICE_API GUIDRV_Lin_OXY_2_API;
extern const GUI_DEVICE_API GUIDRV_Lin_OS_2_API;
extern const GUI_DEVICE_API GUIDRV_Lin_OSY_2_API;
extern const GUI_DEVICE_API GUIDRV_Lin_OSX_2_API;
extern const GUI_DEVICE_API GUIDRV_Lin_OSXY_2_API;

extern const GUI_DEVICE_API GUIDRV_Lin_4_API;
extern const GUI_DEVICE_API GUIDRV_Lin_OY_4_API;
extern const GUI_DEVICE_API GUIDRV_Lin_OX_4_API;
extern const GUI_DEVICE_API GUIDRV_Lin_OXY_4_API;
extern const GUI_DEVICE_API GUIDRV_Lin_OS_4_API;
extern const GUI_DEVICE_API GUIDRV_Lin_OSY_4_API;
extern const GUI_DEVICE_API GUIDRV_Lin_OSX_4_API;
extern const GUI_DEVICE_API GUIDRV_Lin_OSXY_4_API;

extern const GUI_DEVICE_API GUIDRV_Lin_8_API;
extern const GUI_DEVICE_API GUIDRV_Lin_OY_8_API;
extern const GUI_DEVICE_API GUIDRV_Lin_OX_8_API;
extern const GUI_DEVICE_API GUIDRV_Lin_OXY_8_API;
extern const GUI_DEVICE_API GUIDRV_Lin_OS_8_API;
extern const GUI_DEVICE_API GUIDRV_Lin_OSY_8_API;
extern const GUI_DEVICE_API GUIDRV_Lin_OSX_8_API;
extern const GUI_DEVICE_API GUIDRV_Lin_OSXY_8_API;

extern const GUI_DEVICE_API GUIDRV_Lin_16_API;
extern const GUI_DEVICE_API GUIDRV_Lin_OY_16_API;
extern const GUI_DEVICE_API GUIDRV_Lin_OX_16_API;
extern const GUI_DEVICE_API GUIDRV_Lin_OXY_16_API;
extern const GUI_DEVICE_API GUIDRV_Lin_OS_16_API;
extern const GUI_DEVICE_API GUIDRV_Lin_OSY_16_API;
extern const GUI_DEVICE_API GUIDRV_Lin_OSX_16_API;
extern const GUI_DEVICE_API GUIDRV_Lin_OSXY_16_API;

extern const GUI_DEVICE_API GUIDRV_Lin_24_API;
extern const GUI_DEVICE_API GUIDRV_Lin_OY_24_API;
extern const GUI_DEVICE_API GUIDRV_Lin_OX_24_API;
extern const GUI_DEVICE_API GUIDRV_Lin_OXY_24_API;
extern const GUI_DEVICE_API GUIDRV_Lin_OS_24_API;
extern const GUI_DEVICE_API GUIDRV_Lin_OSY_24_API;
extern const GUI_DEVICE_API GUIDRV_Lin_OSX_24_API;
extern const GUI_DEVICE_API GUIDRV_Lin_OSXY_24_API;

extern const GUI_DEVICE_API GUIDRV_Lin_32_API;
extern const GUI_DEVICE_API GUIDRV_Lin_OY_32_API;
extern const GUI_DEVICE_API GUIDRV_Lin_OX_32_API;
extern const GUI_DEVICE_API GUIDRV_Lin_OXY_32_API;
extern const GUI_DEVICE_API GUIDRV_Lin_OS_32_API;
extern const GUI_DEVICE_API GUIDRV_Lin_OSY_32_API;
extern const GUI_DEVICE_API GUIDRV_Lin_OSX_32_API;
extern const GUI_DEVICE_API GUIDRV_Lin_OSXY_32_API;

//
// Macros to be used in configuration files
//
#if defined(WIN32) && !defined(LCD_SIMCONTROLLER)

  #define GUIDRV_LIN_1       &GUIDRV_Win_API
  #define GUIDRV_LIN_OY_1    &GUIDRV_Win_API
  #define GUIDRV_LIN_OX_1    &GUIDRV_Win_API
  #define GUIDRV_LIN_OXY_1   &GUIDRV_Win_API
  #define GUIDRV_LIN_OS_1    &GUIDRV_Win_API
  #define GUIDRV_LIN_OSY_1   &GUIDRV_Win_API
  #define GUIDRV_LIN_OSX_1   &GUIDRV_Win_API
  #define GUIDRV_LIN_OSXY_1  &GUIDRV_Win_API

  #define GUIDRV_LIN_2       &GUIDRV_Win_API
  #define GUIDRV_LIN_OY_2    &GUIDRV_Win_API
  #define GUIDRV_LIN_OX_2    &GUIDRV_Win_API
  #define GUIDRV_LIN_OXY_2   &GUIDRV_Win_API
  #define GUIDRV_LIN_OS_2    &GUIDRV_Win_API
  #define GUIDRV_LIN_OSY_2   &GUIDRV_Win_API
  #define GUIDRV_LIN_OSX_2   &GUIDRV_Win_API
  #define GUIDRV_LIN_OSXY_2  &GUIDRV_Win_API

  #define GUIDRV_LIN_4       &GUIDRV_Win_API
  #define GUIDRV_LIN_OY_4    &GUIDRV_Win_API
  #define GUIDRV_LIN_OX_4    &GUIDRV_Win_API
  #define GUIDRV_LIN_OXY_4   &GUIDRV_Win_API
  #define GUIDRV_LIN_OS_4    &GUIDRV_Win_API
  #define GUIDRV_LIN_OSY_4   &GUIDRV_Win_API
  #define GUIDRV_LIN_OSX_4   &GUIDRV_Win_API
  #define GUIDRV_LIN_OSXY_4  &GUIDRV_Win_API

  #define GUIDRV_LIN_8       &GUIDRV_Win_API
  #define GUIDRV_LIN_OY_8    &GUIDRV_Win_API
  #define GUIDRV_LIN_OX_8    &GUIDRV_Win_API
  #define GUIDRV_LIN_OXY_8   &GUIDRV_Win_API
  #define GUIDRV_LIN_OS_8    &GUIDRV_Win_API
  #define GUIDRV_LIN_OSY_8   &GUIDRV_Win_API
  #define GUIDRV_LIN_OSX_8   &GUIDRV_Win_API
  #define GUIDRV_LIN_OSXY_8  &GUIDRV_Win_API

  #define GUIDRV_LIN_16      &GUIDRV_Win_API
  #define GUIDRV_LIN_OY_16   &GUIDRV_Win_API
  #define GUIDRV_LIN_OX_16   &GUIDRV_Win_API
  #define GUIDRV_LIN_OXY_16  &GUIDRV_Win_API
  #define GUIDRV_LIN_OS_16   &GUIDRV_Win_API
  #define GUIDRV_LIN_OSY_16  &GUIDRV_Win_API
  #define GUIDRV_LIN_OSX_16  &GUIDRV_Win_API
  #define GUIDRV_LIN_OSXY_16 &GUIDRV_Win_API

  #define GUIDRV_LIN_24      &GUIDRV_Win_API
  #define GUIDRV_LIN_OY_24   &GUIDRV_Win_API
  #define GUIDRV_LIN_OX_24   &GUIDRV_Win_API
  #define GUIDRV_LIN_OXY_24  &GUIDRV_Win_API
  #define GUIDRV_LIN_OS_24   &GUIDRV_Win_API
  #define GUIDRV_LIN_OSY_24  &GUIDRV_Win_API
  #define GUIDRV_LIN_OSX_24  &GUIDRV_Win_API
  #define GUIDRV_LIN_OSXY_24 &GUIDRV_Win_API

  #define GUIDRV_LIN_32      &GUIDRV_Win_API
  #define GUIDRV_LIN_OY_32   &GUIDRV_Win_API
  #define GUIDRV_LIN_OX_32   &GUIDRV_Win_API
  #define GUIDRV_LIN_OXY_32  &GUIDRV_Win_API
  #define GUIDRV_LIN_OS_32   &GUIDRV_Win_API
  #define GUIDRV_LIN_OSY_32  &GUIDRV_Win_API
  #define GUIDRV_LIN_OSX_32  &GUIDRV_Win_API
  #define GUIDRV_LIN_OSXY_32 &GUIDRV_Win_API

#else

  #define GUIDRV_LIN_1       &GUIDRV_Lin_1_API
  #define GUIDRV_LIN_OY_1    &GUIDRV_Lin_OY_1_API
  #define GUIDRV_LIN_OX_1    &GUIDRV_Lin_OX_1_API
  #define GUIDRV_LIN_OXY_1   &GUIDRV_Lin_OXY_1_API
  #define GUIDRV_LIN_OS_1    &GUIDRV_Lin_OS_1_API
  #define GUIDRV_LIN_OSY_1   &GUIDRV_Lin_OSY_1_API
  #define GUIDRV_LIN_OSX_1   &GUIDRV_Lin_OSX_1_API
  #define GUIDRV_LIN_OSXY_1  &GUIDRV_Lin_OSXY_1_API

  #define GUIDRV_LIN_2       &GUIDRV_Lin_2_API
  #define GUIDRV_LIN_OY_2    &GUIDRV_Lin_OY_2_API
  #define GUIDRV_LIN_OX_2    &GUIDRV_Lin_OX_2_API
  #define GUIDRV_LIN_OXY_2   &GUIDRV_Lin_OXY_2_API
  #define GUIDRV_LIN_OS_2    &GUIDRV_Lin_OS_2_API
  #define GUIDRV_LIN_OSY_2   &GUIDRV_Lin_OSY_2_API
  #define GUIDRV_LIN_OSX_2   &GUIDRV_Lin_OSX_2_API
  #define GUIDRV_LIN_OSXY_2  &GUIDRV_Lin_OSXY_2_API

  #define GUIDRV_LIN_4       &GUIDRV_Lin_4_API
  #define GUIDRV_LIN_OY_4    &GUIDRV_Lin_OY_4_API
  #define GUIDRV_LIN_OX_4    &GUIDRV_Lin_OX_4_API
  #define GUIDRV_LIN_OXY_4   &GUIDRV_Lin_OXY_4_API
  #define GUIDRV_LIN_OS_4    &GUIDRV_Lin_OS_4_API
  #define GUIDRV_LIN_OSY_4   &GUIDRV_Lin_OSY_4_API
  #define GUIDRV_LIN_OSX_4   &GUIDRV_Lin_OSX_4_API
  #define GUIDRV_LIN_OSXY_4  &GUIDRV_Lin_OSXY_4_API

  #define GUIDRV_LIN_8       &GUIDRV_Lin_8_API
  #define GUIDRV_LIN_OY_8    &GUIDRV_Lin_OY_8_API
  #define GUIDRV_LIN_OX_8    &GUIDRV_Lin_OX_8_API
  #define GUIDRV_LIN_OXY_8   &GUIDRV_Lin_OXY_8_API
  #define GUIDRV_LIN_OS_8    &GUIDRV_Lin_OS_8_API
  #define GUIDRV_LIN_OSY_8   &GUIDRV_Lin_OSY_8_API
  #define GUIDRV_LIN_OSX_8   &GUIDRV_Lin_OSX_8_API
  #define GUIDRV_LIN_OSXY_8  &GUIDRV_Lin_OSXY_8_API

  #define GUIDRV_LIN_16      &GUIDRV_Lin_16_API
  #define GUIDRV_LIN_OY_16   &GUIDRV_Lin_OY_16_API
  #define GUIDRV_LIN_OX_16   &GUIDRV_Lin_OX_16_API
  #define GUIDRV_LIN_OXY_16  &GUIDRV_Lin_OXY_16_API
  #define GUIDRV_LIN_OS_16   &GUIDRV_Lin_OS_16_API
  #define GUIDRV_LIN_OSY_16  &GUIDRV_Lin_OSY_16_API
  #define GUIDRV_LIN_OSX_16  &GUIDRV_Lin_OSX_16_API
  #define GUIDRV_LIN_OSXY_16 &GUIDRV_Lin_OSXY_16_API

  #define GUIDRV_LIN_24      &GUIDRV_Lin_24_API
  #define GUIDRV_LIN_OY_24   &GUIDRV_Lin_OY_24_API
  #define GUIDRV_LIN_OX_24   &GUIDRV_Lin_OX_24_API
  #define GUIDRV_LIN_OXY_24  &GUIDRV_Lin_OXY_24_API
  #define GUIDRV_LIN_OS_24   &GUIDRV_Lin_OS_24_API
  #define GUIDRV_LIN_OSY_24  &GUIDRV_Lin_OSY_24_API
  #define GUIDRV_LIN_OSX_24  &GUIDRV_Lin_OSX_24_API
  #define GUIDRV_LIN_OSXY_24 &GUIDRV_Lin_OSXY_24_API

  #define GUIDRV_LIN_32      &GUIDRV_Lin_32_API
  #define GUIDRV_LIN_OY_32   &GUIDRV_Lin_OY_32_API
  #define GUIDRV_LIN_OX_32   &GUIDRV_Lin_OX_32_API
  #define GUIDRV_LIN_OXY_32  &GUIDRV_Lin_OXY_32_API
  #define GUIDRV_LIN_OS_32   &GUIDRV_Lin_OS_32_API
  #define GUIDRV_LIN_OSY_32  &GUIDRV_Lin_OSY_32_API
  #define GUIDRV_LIN_OSX_32  &GUIDRV_Lin_OSX_32_API
  #define GUIDRV_LIN_OSXY_32 &GUIDRV_Lin_OSXY_32_API

#endif

#if defined(__cplusplus)
}
#endif

#endif

/*************************** End of file ****************************/
