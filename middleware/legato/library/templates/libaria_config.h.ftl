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
  MPLAB Harmony Graphics Composer Generated Definitions Header

  File Name:
    libaria_config.h

  Summary:
    Build-time generated definitions header based on output by the MPLAB Harmony
    Graphics Composer.

  Description:
    Build-time generated definitions header based on output by the MPLAB Harmony
    Graphics Composer.

    Created with MPLAB Harmony Version 3.0
*******************************************************************************/


#ifndef LIBARIA_CONFIG_H
#define LIBARIA_CONFIG_H

// DOM-IGNORE-BEGIN
#ifdef __cplusplus  // Provide C++ Compatibility
extern "C" {
#endif
// DOM-IGNORE-END 

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

#define LA_ASSERT_ENABLE                   ${assertsEnabled?string("1", "0")}

#define LA_PREEMPTION_LEVEL                ${preemptionLevel}
#define LA_EXTERNAL_STREAMING_ENABLED      ${externalStreamingEnabled?string("1", "0")}

// default Font Support
#define LA_INCLUDE_DEFAULT_1BPP_FONT       ${include1BPPFont?string("1", "0")}
#define LA_INCLUDE_DEFAULT_8BPP_FONT       ${include8BPPFont?string("1", "0")}


// OSAL support

// widget inclusion
#define LA_ARC_WIDGET_ENABLED              ${enableArcWidget?string("1", "0")}
#define LA_BAR_GRAPH_WIDGET_ENABLED        ${enableBarGraphWidget?string("1", "0")}
#define LA_BUTTON_WIDGET_ENABLED           ${enableButtonWidget?string("1", "0")}
#define LA_CHECKBOX_WIDGET_ENABLED         ${enableCheckBoxWidget?string("1", "0")}
#define LA_CIRCLE_WIDGET_ENABLED           ${enableCircleWidget?string("1", "0")}
#define LA_CIRCULAR_GAUGE_WIDGET_ENABLED   ${enableCircularGaugeWidget?string("1", "0")}
#define LA_CIRCULAR_SLIDER_WIDGET_ENABLED  ${enableCircularSliderWidget?string("1", "0")}
#define LA_DRAWSURFACE_WIDGET_ENABLED      ${enableDrawSurfaceWidget?string("1", "0")}
#define LA_GRADIENT_WIDGET_ENABLED         ${enableGradientWidget?string("1", "0")}
#define LA_GROUPBOX_WIDGET_ENABLED         ${enableGroupBoxWidget?string("1", "0")}
#define LA_IMAGE_WIDGET_ENABLED            ${enableImageWidget?string("1", "0")}
#define LA_IMAGEPLUS_WIDGET_ENABLED        ${enableImagePlusWidget?string("1", "0")}
#define LA_IMAGESEQUENCE_WIDGET_ENABLED    ${enableImageSequenceWidget?string("1", "0")}
#define LA_KEYPAD_WIDGET_ENABLED           ${enableKeyPadWidget?string("1", "0")}
#define LA_LABEL_WIDGET_ENABLED            ${enableLabelWidget?string("1", "0")}
#define LA_LINE_WIDGET_ENABLED             ${enableLineWidget?string("1", "0")}
#define LA_LINE_GRAPH_WIDGET_ENABLED       ${enableLineGraphWidget?string("1", "0")}
#define LA_LIST_WIDGET_ENABLED             ${enableListWidget?string("1", "0")}
#define LA_LISTWHEEL_WIDGET_ENABLED        ${enableListWheelWidget?string("1", "0")}
#define LA_PIE_CHART_WIDGET_ENABLED        ${enablePieChartWidget?string("1", "0")}
#define LA_PROGRESSBAR_WIDGET_ENABLED      ${enableProgressBarWidget?string("1", "0")}
#define LA_RADIAL_MENU_WIDGET_ENABLED      ${enableRadialMenuWidget?string("1", "0")}
#define LA_RADIOBUTTON_WIDGET_ENABLED      ${enableRadioButtonWidget?string("1", "0")}
#define LA_RECTANGLE_WIDGET_ENABLED        ${enableRectangleWidget?string("1", "0")}
#define LA_SCROLLBAR_WIDGET_ENABLED        ${enableScrollBarWidget?string("1", "0")}
#define LA_SLIDER_WIDGET_ENABLED           ${enableSliderWidget?string("1", "0")}
#define LA_TEXTFIELD_WIDGET_ENABLED        ${enableTextFieldWidget?string("1", "0")}
#define LA_TOUCHTEST_WIDGET_ENABLED        ${enableTouchTestWidget?string("1", "0")}
#define LA_WINDOW_WIDGET_ENABLED           ${enableWindowWidget?string("1", "0")}

// memory configuration
#define LA_USE_DEBUG_ALLOCATOR             ${useDebugAllocator?string("1", "0")}
#define LA_MALLOC_ZEROIZE                  ${zeroizeMemory?string("1", "0")}

#if LA_USE_DEBUG_ALLOCATOR == 1
#define LA_USE_ALLOCATION_TRACKING         ${useAllocationTracking?string("1", "0")}
#endif

// variable heap configuration
#define LA_VARIABLEHEAP_DEBUGLEVEL         ${variableHeapDebugLevel}
#define LA_VARIABLEHEAP_SIZE               ${variableHeapSize}
#define LA_VARIABLEHEAP_MINALLOCSIZE       ${variableHeapMinAllocSize}
#define LA_VARIABLEHEAP_ALIGNMENT          4
<#if variableHeapUseBestFit == "Best Fit">
#define LA_VARIABLEHEAP_BESTFIT            1
<#else>
#define LA_VARIABLEHEAP_BESTFIT            1
</#if>

// fixed heap configuration
#define LA_FIXEDHEAP_ENABLE                ${fixedHeapEnable?string("1", "0")}
#define LA_FIXEDHEAP_DEBUG                 ${fixedHeapDebugEnable?string("1", "0")}
#define LA_FIXEDHEAP_SIZE_16               ${fixedHeapSize16}
#define LA_FIXEDHEAP_SIZE_32               ${fixedHeapSize32}
#define LA_FIXEDHEAP_SIZE_64               ${fixedHeapSize64}
#define LA_FIXEDHEAP_SIZE_128              ${fixedHeapSize128}
#define LA_FIXEDHEAP_SIZE_196              ${fixedHeapSize196}
#define LA_FIXEDHEAP_SIZE_256              ${fixedHeapSize256}

// renderer configuration
#define LA_LAYER_COUNT                     ${layerCount}

#if LA_LAYER_COUNT <= 0
#error LA_LAYER_COUNT must be greater than or equal to 1
#endif

#define LA_WIDGET_MAX_EVENT_FILTERS        2
#define LA_IMAGE_WIDGET_DEBUG              0

// renderer config
#define LA_ALPHA_BLENDING_ENABLED          ${useAlphaBlending?string("1", "0")}
#define LA_SCRATCH_BUFFER_SIZE_KB          ${scratchBufferSize}
#define LA_USE_ARC_SCAN_FILL               1
#define LA_ARC_SMOOTH_EDGE                 LA_FALSE

//DOM-IGNORE-BEGIN
#ifdef __cplusplus
}
#endif
//DOM-IGNORE-END

#endif // LIBARIA_CONFIG_H
