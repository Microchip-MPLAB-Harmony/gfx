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
  MPLAB Harmony Graphics Composer Generated Definitions Header

  File Name:
    legato_config.h

  Summary:
    Build-time generated definitions header based on output by the MPLAB Harmony
    Graphics Composer.

  Description:
    Build-time generated definitions header based on output by the MPLAB Harmony
    Graphics Composer.

    Created with MPLAB Harmony Version 3.0
*******************************************************************************/


#ifndef LEGATO_CONFIG_H
#define LEGATO_CONFIG_H

#ifdef __cplusplus  // Provide C++ Compatibility

extern "C" {

/* export macros for shared libraries */
#ifdef _WIN32
#ifdef LIBRARY
#define LIB_EXPORT __declspec( dllexport )
#else
#define LIB_EXPORT __declspec( dllimport )
#endif
#else
#define LIB_EXPORT 
#endif

#define LE_ASSERT_ENABLE                   1

#define LE_PREEMPTION_LEVEL                0
#define LE_STREAMING_ENABLED               0

// default Font Support
#define LE_INCLUDE_DEFAULT_1BPP_FONT       1
#define LE_INCLUDE_DEFAULT_8BPP_FONT       1


// OSAL support

// widget inclusion
#define LE_ARC_WIDGET_ENABLED              1
#define LE_BARGRAPH_WIDGET_ENABLED         1
#define LE_BUTTON_WIDGET_ENABLED           1
#define LE_CHECKBOX_WIDGET_ENABLED         1
#define LE_CIRCLE_WIDGET_ENABLED           1
#define LE_CIRCULARGAUGE_WIDGET_ENABLED    1
#define LE_CIRCULARSLIDER_WIDGET_ENABLED   1
#define LE_DRAWSURFACE_WIDGET_ENABLED      1
#define LE_GRADIENT_WIDGET_ENABLED         1
#define LE_GROUPBOX_WIDGET_ENABLED         1
#define LE_IMAGE_WIDGET_ENABLED            1
#define LE_IMAGEPLUS_WIDGET_ENABLED        1
#define LE_IMAGESEQUENCE_WIDGET_ENABLED    1
#define LE_KEYPAD_WIDGET_ENABLED           1
#define LE_LABEL_WIDGET_ENABLED            1
#define LE_LINE_WIDGET_ENABLED             1
#define LE_LINEGRAPH_WIDGET_ENABLED        1
#define LE_LIST_WIDGET_ENABLED             1
#define LE_LISTWHEEL_WIDGET_ENABLED        1
#define LE_PIECHART_WIDGET_ENABLED         1
#define LE_PROGRESSBAR_WIDGET_ENABLED      1
#define LE_RADIALMENU_WIDGET_ENABLED       1
#define LE_RADIOBUTTON_WIDGET_ENABLED      1
#define LE_RECTANGLE_WIDGET_ENABLED        1
#define LE_SCROLLBAR_WIDGET_ENABLED        1
#define LE_SLIDER_WIDGET_ENABLED           1
#define LE_TEXTFIELD_WIDGET_ENABLED        1
#define LE_TOUCHTEST_WIDGET_ENABLED        1
#define LE_WINDOW_WIDGET_ENABLED           1

// memory configuration
#define LE_USE_DEBUG_ALLOCATOR             1
#define LE_MALLOC_ZEROIZE                  1

#if LE_USE_DEBUG_ALLOCATOR == 1
#define LE_USE_ALLOCATION_TRACKING         1
#endif

// variable heap configuration
#define LE_VARIABLEHEAP_DEBUGLEVEL         1
#define LE_VARIABLEHEAP_SIZE               1024*2
#define LE_VARIABLEHEAP_MINALLOCSIZE       32
#define LE_VARIABLEHEAP_ALIGNMENT          4
#define LE_VARIABLEHEAP_BESTFIT            1  

// fixed heap configuration
#define LE_FIXEDHEAP_ENABLE                1
#define LE_FIXEDHEAP_DEBUG                 1
#define LE_FIXEDHEAP_SIZE_16               300
#define LE_FIXEDHEAP_SIZE_32               96
#define LE_FIXEDHEAP_SIZE_64               96
#define LE_FIXEDHEAP_SIZE_128              32
#define LE_FIXEDHEAP_SIZE_196              48
#define LE_FIXEDHEAP_SIZE_256              24

// renderer configuration
//#define LE_VSYNC_STALL

#define LE_LAYER_COUNT                     1

#if LE_LAYER_COUNT <= 0
#error LE_LAYER_COUNT must be greater than or equal to 1
#endif

#define LE_WIDGET_MAX_EVENT_FILTERS        2
#define LE_IMAGE_WIDGET_DEBUG              0

// renderer config
#define LE_ALPHA_BLENDING_ENABLED          1
#define LE_SCRATCH_BUFFER_SIZE_KB          256
#define LE_USE_ARC_SCAN_FILL               1
#define LE_ARC_SMOOTH_EDGE                 LE_FALSE

//DOM-IGNORE-BEGIN
#ifdef __cplusplus
}
#endif
//DOM-IGNORE-END

#endif // LEGATO_EVENTS_H
/*******************************************************************************
 End of File
*/
