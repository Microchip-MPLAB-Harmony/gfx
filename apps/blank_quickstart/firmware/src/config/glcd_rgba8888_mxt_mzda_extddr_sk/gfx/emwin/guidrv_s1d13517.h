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


#ifndef GUIDRV_S1D13517_H
#define GUIDRV_S1D13517_H

#if defined(__cplusplus)
extern "C" {     /* Make sure we have C-declarations in C++ programs */
#endif

/*********************************************************************
*
*       Configuration structure
*/
typedef struct {
  //
  // Driver specific configuration items
  //
  U32 TransColorIndex;
} CONFIG_S1D13517;

/*********************************************************************
*
*       Display drivers
*/
//
// Addresses
//
extern const GUI_DEVICE_API GUIDRV_S1D13517_16C0_API;
extern const GUI_DEVICE_API GUIDRV_S1D13517_OY_16C0_API;
extern const GUI_DEVICE_API GUIDRV_S1D13517_OX_16C0_API;
extern const GUI_DEVICE_API GUIDRV_S1D13517_OXY_16C0_API;
extern const GUI_DEVICE_API GUIDRV_S1D13517_OS_16C0_API;
extern const GUI_DEVICE_API GUIDRV_S1D13517_OSY_16C0_API;
extern const GUI_DEVICE_API GUIDRV_S1D13517_OSX_16C0_API;
extern const GUI_DEVICE_API GUIDRV_S1D13517_OSXY_16C0_API;

//
// Macros to be used in configuration files
//
#if defined(WIN32) && !defined(LCD_SIMCONTROLLER)

  #define GUIDRV_S1D13517_16C0       &GUIDRV_Win_API
  #define GUIDRV_S1D13517_OY_16C0    &GUIDRV_Win_API
  #define GUIDRV_S1D13517_OX_16C0    &GUIDRV_Win_API
  #define GUIDRV_S1D13517_OXY_16C0   &GUIDRV_Win_API
  #define GUIDRV_S1D13517_OS_16C0    &GUIDRV_Win_API
  #define GUIDRV_S1D13517_OSY_16C0   &GUIDRV_Win_API
  #define GUIDRV_S1D13517_OSX_16C0   &GUIDRV_Win_API
  #define GUIDRV_S1D13517_OSXY_16C0  &GUIDRV_Win_API

#else

  #define GUIDRV_S1D13517_16C0       &GUIDRV_S1D13517_16C0_API
  #define GUIDRV_S1D13517_OY_16C0    &GUIDRV_S1D13517_OY_16C0_API
  #define GUIDRV_S1D13517_OX_16C0    &GUIDRV_S1D13517_OX_16C0_API
  #define GUIDRV_S1D13517_OXY_16C0   &GUIDRV_S1D13517_OXY_16C0_API
  #define GUIDRV_S1D13517_OS_16C0    &GUIDRV_S1D13517_OS_16C0_API
  #define GUIDRV_S1D13517_OSY_16C0   &GUIDRV_S1D13517_OSY_16C0_API
  #define GUIDRV_S1D13517_OSX_16C0   &GUIDRV_S1D13517_OSX_16C0_API
  #define GUIDRV_S1D13517_OSXY_16C0  &GUIDRV_S1D13517_OSXY_16C0_API

#endif

/*********************************************************************
*
*       Public routines
*/
#if defined(WIN32) && !defined(LCD_SIMCONTROLLER)
  #define GUIDRV_S1D13517_SetBus16(pDevice, pHW_API)
  #define GUIDRV_S1D13517_Config(pDevice, pConfig)
#else
  void GUIDRV_S1D13517_SetBus16(GUI_DEVICE * pDevice, GUI_PORT_API * pHW_API);
  void GUIDRV_S1D13517_Config  (GUI_DEVICE * pDevice, CONFIG_S1D13517 * pConfig);
#endif

#if defined(__cplusplus)
}
#endif

#endif

/*************************** End of file ****************************/
