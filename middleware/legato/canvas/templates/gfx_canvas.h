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
  GFX GLCD Driver Interface Declarations for Static Single Instance Driver

  Company:
    Microchip Technology Inc.

  File Name:
    gfx_canvas.h

  Summary:
    GFX Canvas Virtual Display Component private header file

  Description:
    The GFX Canvas provides a virtual display driver interface. 

  Remarks:
    None
*******************************************************************************/

/** \file gfx_canvas.h
 * @brief  GFX Canvas Virtual Display definitions and functions.
 *
 * @details The GFX Canvas provides a virtual display driver interface.
 */

#ifndef _GFX_CANVAS_H    /* Guard against multiple inclusion */
#define _GFX_CANVAS_H

#include "gfx/driver/gfx_driver.h"

#ifdef __cplusplus  // Provide C++ Compatibility
extern "C" {
#endif
    
#define CANVAS_ID_INVALID (-1)
#define LAYER_ID_INVALID (-1)

#define CONFIG_BUFFER_PER_LAYER 1
#define CONFIG_NUM_CANVAS_OBJ ${NumCanvasObjects}
#define CONFIG_NUM_LAYERS ${DefaultNumLayers}
#define CONFIG_FX_INTERVAL_MS ${EffectsInterval}
#define CANVAS_DEFAULT_FORMAT GFX_COLOR_MODE_${ColorMode}
    
#define GFXC_BOOL gfxBool
    
/**
 * @brief This struct represents a gfx canvas.
 * @details .
 */
typedef struct 
{
    int id;                         // Canvas ID
    GFXC_BOOL active;               //The canvas is shown on its layer
    
    gfxPixelBuffer pixelBuffer;
    
    struct
    {
        int id;                    // The canvas layer ID
        uint8_t alpha;             //The canvas layer alpha
        
        struct                     // Canvas layer position
        {
            int xpos;
            int ypos;
        } pos;
        
        struct                     // Canvas layer size
        {
            int width;
            int height;
        } size;
        
    } layer;
    
    struct
    {
        gfxcCallback cb;
        void * parm;
        
        struct
        {
            GFXC_FX_STATUS status;
            uint8_t startAlpha;
            uint8_t endAlpha;
            uint8_t delta;
        } fade;
        
        struct
        {
            GFXC_FX_STATUS status;
            int startX;
            int startY;
            int endX;
            int endY;
            unsigned int delta;
            GFXC_FX_MOVE_TYPE type;
        } move;
    } effects;

    
} GFXC_CANVAS;

//private functions
/**
  * @cond INTERNAL
  *
  */
GFXC_RESULT _gfxcCanvasUpdate(unsigned int canvasID);
GFXC_RESULT _gfxcSetEffectsIntervalMS(unsigned int ms);
GFXC_RESULT _gfxcStartEffects(void);


//private variables 
extern GFXC_CANVAS canvas[];
extern unsigned int numLayers;
/**
  * @endcond
  *
  */

    /* Provide C++ Compatibility */
#ifdef __cplusplus
}
#endif

#endif /* _GFX_CANVAS_H */

/* *****************************************************************************
 End of File
 */
