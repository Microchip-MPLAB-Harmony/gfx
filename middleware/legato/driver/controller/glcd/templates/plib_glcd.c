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
  GLCD Peripheral Library

  Company:
    Microchip Technology Inc.

  File Name:
    plib_glcd.c

  Summary:
    GLCD function implementations for the GLCD PLIB.

  Description:
    The GLCD PLIB provides a simple interface to manage the GLCD controller.
*******************************************************************************/


#include <stdint.h>
#include <stdbool.h>
#include "gfx/driver/controller/glcd/plib_glcd.h"
#include "definitions.h"

/*
 * Address typedefs
 */
typedef unsigned long _paddr_t; /* a physical address */
typedef unsigned long _vaddr_t; /* a virtual address */

/* 
 * Translate a kernel virtual address in KSEG0 or KSEG1 to a real
 * physical address and back.
 */
#define KVA_TO_PA(v)     ((_paddr_t)(v) & 0x1fffffff)
#define PA_TO_KVA0(pa)    ((void *) ((pa) | 0x80000000))
#define PA_TO_KVA1(pa)    ((void *) ((pa) | 0xa0000000))

/* translate between KSEG0 and KSEG1 virtual addresses */
#define KVA0_TO_KVA1(v)    ((void *) ((unsigned)(v) | 0x20000000))
#define KVA1_TO_KVA0(v)    ((void *) ((unsigned)(v) & ~0x20000000))

typedef volatile uint32_t SFR_TYPE;

//******************************************************************************
/* Function :  PLIB_GLCD_LayerGlobalAlphaGet

  Summary:
    Returns the layer's global alpha 

  Description:
    
*/
uint8_t PLIB_GLCD_LayerGlobalAlphaGet(GLCD_LAYER_ID layerId)
{
    return (  ( *((SFR_TYPE *)(( (&GLCDL0MODE) + ( layerId * 8 ) )))&(_GLCDL0MODE_ALPHA_MASK) ) \
            >> (_GLCDL0MODE_ALPHA_POSITION)  );
}

//******************************************************************************
/* Function :  PLIB_GLCD_LayerEnable

  Summary:
    Enables the specified layer

  Description:
    
*/
void PLIB_GLCD_LayerEnable(GLCD_LAYER_ID layerId)
{
       *((SFR_TYPE *)(( (&GLCDL0MODE)  + ( layerId * 8 ) ))) = ( ( *((SFR_TYPE *)(( (&GLCDL0MODE)  + ( layerId * 8 ) ))) ) & \
    ~(1<<(_GLCDL0MODE_LAYEREN_POSITION)) ) | (0x1&(1))<<(_GLCDL0MODE_LAYEREN_POSITION);
}

//******************************************************************************
/* Function :  PLIB_GLCD_LayerDisable

  Summary:
    Disables the specified layer

  Description:
    
*/
void PLIB_GLCD_LayerDisable(GLCD_LAYER_ID layerId)
{
       *((SFR_TYPE *)(( (&GLCDL0MODE)  + ( layerId * 8 ) ))) = ( ( *((SFR_TYPE *)(( (&GLCDL0MODE)  + ( layerId * 8 ) ))) ) & \
    ~(1<<(_GLCDL0MODE_LAYEREN_POSITION)) ) | (0x1&(0))<<(_GLCDL0MODE_LAYEREN_POSITION);
}

//******************************************************************************
/* Function :  PLIB_GLCD_Disable

  Summary:
    Disables the GLCD peripheral

  Description:
    
*/
void PLIB_GLCD_Disable(void)
{
    GLCDMODEbits.LCDEN = 0;                
    

    CFGCON2bits.GLCDPINEN = 0;    
}

//******************************************************************************
/* Function :  PLIB_GLCD_BackgroundColorSet

  Summary:
    Sets the background layer color

  Description:
    
*/
void PLIB_GLCD_BackgroundColorSet(uint32_t bgColor)
{
    GLCDBGCOLOR = bgColor;
}

//******************************************************************************
/* Function :  PLIB_GLCD_HSyncInterruptDisable

  Summary:
    Enables the GLCD HSYNC Interrupt

  Description:
    
*/
void PLIB_GLCD_HSyncInterruptDisable(void)
{
    GLCDINTbits.HSYNCINT = 0;
}

//******************************************************************************
/* Function :  PLIB_GLCD_RGBSequentialModeSet

  Summary:
    Sets the GLCD sequential mode

  Description:
    
*/
void PLIB_GLCD_RGBSequentialModeSet(GLCD_RGB_MODE mode)
{
   switch( mode )
    {
        case GLCD_RGB_MODE_PARALLEL_RGB565:
        {
            CFGCON2bits.GLCDMODE = 1;
            GLCDMODEbits.RGBSEQ = 0;
            break;
        }
        
        case GLCD_RGB_MODE_PARALLEL_RGB888:
        {
            CFGCON2bits.GLCDMODE = 0;
            GLCDMODEbits.RGBSEQ = 0;
            break;
        }
        
        case GLCD_RGB_MODE_SERIAL_RGB_3:
        {
            GLCDMODEbits.RGBSEQ = 1;
            break;
        }
        
        case GLCD_RGB_MODE_SERIAL_RGBA_4:
        {
            GLCDMODEbits.RGBSEQ = 2;
            break;
        }
        
        case GLCD_RGB_MODE_SERIAL_12BIT:
        {
            GLCDMODEbits.RGBSEQ = 3;
            break;
        }
        
        case GLCD_RGB_MODE_YUYV_16BIT:
        {
            GLCDMODEbits.RGBSEQ = 6;
            break;
        }
        
        case GLCD_RGB_MODE_BT_656:
        {
            GLCDMODEbits.RGBSEQ = 7;
            break;
        }
    }
}

//******************************************************************************
/* Function :  PLIB_GLCD_FrontPorchXYSet

  Summary:
    Sets the V and H Front porch

  Description:
    
*/
void PLIB_GLCD_FrontPorchXYSet(uint32_t frontPorchX, uint32_t frontPorchY)
{
    GLCDFPORCHbits.FPORCHX = frontPorchX;
    GLCDFPORCHbits.FPORCHY = frontPorchY;
}

//******************************************************************************
/* Function :  PLIB_GLCD_BlankingXYSet

  Summary:
    Sets the V and H blanking period

  Description:
    
*/
void PLIB_GLCD_BlankingXYSet(uint32_t blankingX, uint32_t blankingY)
{
    GLCDBLANKINGbits.BLANKINGX = blankingX;
    GLCDBLANKINGbits.BLANKINGY = blankingY;
}

//******************************************************************************
/* Function :  PLIB_GLCD_BackPorchXYSet

  Summary:
    Sets the V and H back porch

  Description:
    
*/
void PLIB_GLCD_BackPorchXYSet(uint32_t backPorchX, uint32_t backPorchY)
{
    GLCDBPORCHbits.BPORCHX = backPorchX;
    GLCDBPORCHbits.BPORCHY = backPorchY;   
}

//******************************************************************************
/* Function :  PLIB_GLCD_ClockDividerSet

  Summary:
    Sets the GLCD clock divider for pixel clock

  Description:
    
*/
void PLIB_GLCD_ClockDividerSet(uint32_t clockDivider)
{
    *((SFR_TYPE *)(&GLCDCLKCON)) = \
        ( (*((SFR_TYPE *)(&GLCDCLKCON))) & ~(_GLCDCLKCON_CLKDIV_MASK) ) | ( (_GLCDCLKCON_CLKDIV_MASK)& \
        ((clockDivider)<<(_GLCDCLKCON_CLKDIV_POSITION)) ) ;
}

//******************************************************************************
/* Function :  PLIB_GLCD_ResolutionXYSet

  Summary:
    Sets the V and H resolution

  Description:
    
*/
void PLIB_GLCD_ResolutionXYSet(uint32_t resolutionX, uint32_t resolutionY)
{
    GLCDRESbits.RESX = resolutionX;
    GLCDRESbits.RESY = resolutionY;
}

//******************************************************************************
/* Function :  PLIB_GLCD_SignalPolaritySet

  Summary:
    Sets the polarity of the GLCD control signals

  Description:
    
*/
void PLIB_GLCD_SignalPolaritySet(GLCD_SIGNAL_POLARITY polarity)
{
    uint32_t mask = _GLCDMODE_PCLKPOL_MASK | _GLCDMODE_DEPOL_MASK |
                    _GLCDMODE_HSYNCPOL_MASK | _GLCDMODE_VSYNCPOL_MASK;
    GLCDMODE = polarity & mask;
}

//******************************************************************************
/* Function :  PLIB_GLCD_Enable

  Summary:
    Enables the GLCD peripheral

  Description:
    
*/
void PLIB_GLCD_Enable(void)
{

    GLCDMODEbits.LCDEN = 1;                
    

     CFGCON2bits.GLCDPINEN = 1;   
}

//******************************************************************************
/* Function :  PLIB_GLCD_LayerBaseAddressSet

  Summary:
    Sets the layer base address

  Description:
    
*/
void PLIB_GLCD_LayerBaseAddressSet(GLCD_LAYER_ID layerId, uint32_t baseAddress)
{
    *((SFR_TYPE *)(( (&GLCDL0BADDR) + ( layerId * 8 ) )))  =  ( KVA_TO_PA( (void *) baseAddress) );
}

//******************************************************************************
/* Function :  PLIB_GLCD_LayerStrideSet

  Summary:
    Sets the layer stride

  Description:
    
*/
void PLIB_GLCD_LayerStrideSet(GLCD_LAYER_ID layerId, uint32_t stride)
{
    *((SFR_TYPE *)(( (&GLCDL0STRIDE) + ( layerId * 8 ) ))) = \
        ( (*((SFR_TYPE *)(( (&GLCDL0STRIDE) + ( layerId * 8 ) )))) & ~(_GLCDL0STRIDE_STRIDE_MASK) ) | \
        ( (_GLCDL0STRIDE_STRIDE_MASK)&((stride)<<(_GLCDL0STRIDE_STRIDE_POSITION)) );  
}

//******************************************************************************
/* Function :  PLIB_GLCD_LayerResXYSet

  Summary:
    Sets the layer V and H resolution

  Description:
    
*/
void PLIB_GLCD_LayerResXYSet(GLCD_LAYER_ID layerId, uint32_t resolutionX, uint32_t resolutionY)
{
    *((SFR_TYPE *)(( (&GLCDL0RES) + ( layerId * 8 ) ))) = \
        ( (*((SFR_TYPE *)(( (&GLCDL0RES) + ( layerId * 8 ) )))) & ~(_GLCDL0RES_RESX_MASK) ) | \
        ( (_GLCDL0RES_RESX_MASK)&((resolutionX)<<(_GLCDL0RES_RESX_POSITION)) );  
    
    *((SFR_TYPE *)(( (&GLCDL0RES) + ( layerId * 8 ) ))) = \
        ( (*((SFR_TYPE *)(( (&GLCDL0RES) + ( layerId * 8 ) )))) & ~(_GLCDL0RES_RESY_MASK) ) | \
        ( (_GLCDL0RES_RESY_MASK)&((resolutionY)<<(_GLCDL0RES_RESY_POSITION)) ); 
}

//******************************************************************************
/* Function :  PLIB_GLCD_LayerStartXYSet

  Summary:
    Sets the layer V and H offset

  Description:
    
*/
void PLIB_GLCD_LayerStartXYSet(GLCD_LAYER_ID layerId, uint32_t startX, uint32_t startY)
{
    *((SFR_TYPE *)(( (&GLCDL0START) + ( layerId * 8 ) ))) = \
        ( (*((SFR_TYPE *)(( (&GLCDL0START) + ( layerId * 8 ) )))) & ~(_GLCDL0START_STARTX_MASK) ) | \
        ( (_GLCDL0START_STARTX_MASK)&((startX)<<(_GLCDL0START_STARTX_POSITION)) );  
    
    *((SFR_TYPE *)(( (&GLCDL0START) + ( layerId * 8 ) ))) = \
        ( (*((SFR_TYPE *)(( (&GLCDL0START) + ( layerId * 8 ) )))) & ~(_GLCDL0START_STARTY_MASK) ) | \
        ( (_GLCDL0START_STARTY_MASK)&((startY)<<(_GLCDL0START_STARTY_POSITION)) );  
}

//******************************************************************************
/* Function :  PLIB_GLCD_LayerSizeXYSet

  Summary:
    Sets the layer X, Y size

  Description:
    
*/
void PLIB_GLCD_LayerSizeXYSet(GLCD_LAYER_ID layerId, uint32_t sizeX, uint32_t sizeY)
{
    *((SFR_TYPE *)(( (&GLCDL0SIZE) + ( layerId * 8 ) ))) = \
        ( (*((SFR_TYPE *)(( (&GLCDL0SIZE) + ( layerId * 8 ) )))) & ~(_GLCDL0SIZE_SIZEX_MASK) ) | \
        ( (_GLCDL0SIZE_SIZEX_MASK)&((sizeX)<<(_GLCDL0SIZE_SIZEX_POSITION)) );  
    
    *((SFR_TYPE *)(( (&GLCDL0SIZE) + ( layerId * 8 ) ))) = \
        ( (*((SFR_TYPE *)(( (&GLCDL0SIZE) + ( layerId * 8 ) )))) & ~(_GLCDL0SIZE_SIZEY_MASK) ) | \
        ( (_GLCDL0SIZE_SIZEY_MASK)&((sizeY)<<(_GLCDL0SIZE_SIZEY_POSITION)) );  
}

//******************************************************************************
/* Function :  PLIB_GLCD_LayerGlobalAlphaSet

  Summary:
    Sets the layer alpha

  Description:
    
*/
void PLIB_GLCD_LayerGlobalAlphaSet(GLCD_LAYER_ID layerId, uint8_t value)
{
    *((SFR_TYPE *)(( (&GLCDL0MODE) + ( layerId * 8 ) ))) = \
            ( (*((SFR_TYPE *)(( (&GLCDL0MODE) + ( layerId * 8 ) )))) & ~(_GLCDL0MODE_ALPHA_MASK) ) \
            | ( (_GLCDL0MODE_ALPHA_MASK)&((value)<<(_GLCDL0MODE_ALPHA_POSITION)) );
}

//******************************************************************************
/* Function :  PLIB_GLCD_LayerDestBlendFuncSet

  Summary:
    Sets the layer destination blend function

  Description:
    
*/
void PLIB_GLCD_LayerDestBlendFuncSet(GLCD_LAYER_ID layerId, GLCD_LAYER_DEST_BLEND_FUNC blendFunc)
{
    *((SFR_TYPE *)(( (&GLCDL0MODE) + ( layerId * 8 ) ))) = \
        ( (*((SFR_TYPE *)(( (&GLCDL0MODE) + ( layerId * 8 ) )))) & ~(_GLCDL0MODE_DESTBLEND_MASK) ) | \
        ( (_GLCDL0MODE_DESTBLEND_MASK)&(blendFunc) );
}

//******************************************************************************
/* Function :  PLIB_GLCD_LayerSrcBlendFuncSet

  Summary:
    Sets the layer source blend function

  Description:
    
*/
void PLIB_GLCD_LayerSrcBlendFuncSet(GLCD_LAYER_ID layerId, GLCD_LAYER_SRC_BLEND_FUNC blendFunc)
{
    *((SFR_TYPE *)(( (&GLCDL0MODE) + ( layerId * 8 ) ))) = \
        ( (*((SFR_TYPE *)(( (&GLCDL0MODE) + ( layerId * 8 ) )))) & ~(_GLCDL0MODE_SRCBLEND_MASK) ) | \
        ( _GLCDL0MODE_SRCBLEND_MASK & blendFunc );  
}

//******************************************************************************
/* Function :  PLIB_GLCD_LayerColorModeSet

  Summary:
    Sets the layer color mode

  Description:
    
*/
void PLIB_GLCD_LayerColorModeSet(GLCD_LAYER_ID layerId, GLCD_LAYER_COLOR_MODE colorMode)
{
    *((SFR_TYPE *)(( (&GLCDL0MODE) + ( layerId * 8 ) ))) = \
        ( (*((SFR_TYPE *)(( (&GLCDL0MODE) + ( layerId * 8 ) )))) & ~(_GLCDL0MODE_COLORMODE_MASK) ) | ( (_GLCDL0MODE_COLORMODE_MASK)&(colorMode) );
}

//******************************************************************************
/* Function :  PLIB_GLCD_VSyncInterruptEnable

  Summary:
    Enables the GLCD VSYNC interrupt

  Description:
    
*/
void PLIB_GLCD_VSyncInterruptEnable(void)
{
    GLCDINTbits.VSYNCINT = 1;
}

//******************************************************************************
/* Function :  PLIB_GLCD_VSyncInterruptDisable

  Summary:
    Disables the GLCD VSYNC Interrupt

  Description:
    
*/
void PLIB_GLCD_VSyncInterruptDisable(void)
{
    GLCDINTbits.VSYNCINT = 0;
}

//******************************************************************************
/* Function :  PLIB_GLCD_IsVerticalBlankingActive

  Summary:
    Returns 1 if GLCD is in VBLANK period

  Description:
    
*/
bool PLIB_GLCD_IsVerticalBlankingActive(void)
{
     return (!GLCDSTATbits.ACTIVE);
}

//******************************************************************************
/* Function :  PLIB_GLCD_GlobalColorLUTSet

  Summary:
    Sets the global color LUT table

  Description:
    
*/
void PLIB_GLCD_GlobalColorLUTSet(uint32_t* globalLUT)
{
    uint32_t colorIndex = 0;
    
    for( colorIndex = 0; colorIndex < 256; colorIndex++ )
    {
		*((SFR_TYPE *)(( (&GLCDCLUT0) + colorIndex )))  =  (globalLUT[ colorIndex ]);
    }
}

//******************************************************************************
/* Function :  PLIB_GLCD_PaletteGammaRampEnable

  Summary:
    Enables the Palette Gamma Ramp

  Description:
    
*/
void PLIB_GLCD_PaletteGammaRampEnable(void)
{
    GLCDMODEbits.PGRAMPEN = 1;
}

//******************************************************************************
/* Function :  PLIB_GLCD_PaletteGammaRampDisable

  Summary:
    Disables the Palette Gamma Ramp

  Description:
    
*/
void PLIB_GLCD_PaletteGammaRampDisable(void)
{
    GLCDMODEbits.PGRAMPEN = 0;
}

