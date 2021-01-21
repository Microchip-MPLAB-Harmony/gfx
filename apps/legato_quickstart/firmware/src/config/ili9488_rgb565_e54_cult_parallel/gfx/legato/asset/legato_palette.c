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


#include "gfx/legato/asset/legato_palette.h"
#include "gfx/legato/common/legato_error.h"

leColor lePalette_Get8bppColor(lePalette* pal,
                               uint32_t idx)
{
    uint8_t* ptr;
    leColor clr = 0;
    
    LE_ASSERT(pal != NULL);
    LE_ASSERT(idx < pal->colorCount);
    
    ptr = ((uint8_t*)pal->header.dataAddress) + idx;
    
    clr = ptr[0];
    
    return clr;
}

leColor lePalette_Get16bppColor(lePalette* pal,
                                uint32_t idx)
{
    uint8_t* ptr;
    leColor clr = 0;
    
    LE_ASSERT(pal != NULL);
    LE_ASSERT(idx < pal->colorCount);
    
    ptr = ((uint8_t*)pal->header.dataAddress) + (idx * 2);
    
    clr = ((uint16_t*)ptr)[0];
    
    return clr;
}

leColor lePalette_Get24bppColor(lePalette* pal,
                                uint32_t idx)
{
    uint8_t* ptr;
    leColor clr = 0;
    
    LE_ASSERT(pal != NULL);
    LE_ASSERT(idx < pal->colorCount);
    
    ptr = ((uint8_t*)pal->header.dataAddress) + (idx * 3);
            
    clr = ((uint8_t*)ptr)[0];
    clr |= ((uint8_t*)ptr)[1] << 8;
    clr |= ((uint8_t*)ptr)[2] << 16;
    
    return clr;
}

leColor lePalette_Get32bppColor(lePalette* pal,
                                uint32_t idx)
{
    uint8_t* ptr;
    leColor clr = 0;
    
    LE_ASSERT(pal != NULL);
    LE_ASSERT(idx < pal->colorCount);
    
    ptr = ((uint8_t*)pal->header.dataAddress) + (idx * 4);
    
    clr = ((uint32_t*)ptr)[0];
    
    return clr;
}

leColor lePaletteGetColor(lePalette* pal, uint32_t idx)
{
    uint8_t* ptr;
    leColor clr = 0;
    
    LE_ASSERT(pal != NULL);
    LE_ASSERT(idx < pal->colorCount);
    
    switch(pal->colorMode)
    {
        case LE_COLOR_MODE_GS_8:
        case LE_COLOR_MODE_RGB_332:
        {
            ptr = ((uint8_t*)pal->header.dataAddress) + idx;
            clr = ptr[0];
                
            break;
        }
        case LE_COLOR_MODE_RGB_565:
        case LE_COLOR_MODE_RGBA_5551:
        {
            ptr = ((uint8_t*)pal->header.dataAddress) + (idx * 2);
            clr = ((uint16_t*)ptr)[0];
            
            break;
        }
        case LE_COLOR_MODE_RGB_888:
        {
            ptr = ((uint8_t*)pal->header.dataAddress) + (idx * 3);
            
            clr = ((uint8_t*)ptr)[0];
            clr |= ((uint8_t*)ptr)[1] << 8;
            clr |= ((uint8_t*)ptr)[2] << 16;
                
            break;
        }
        case LE_COLOR_MODE_RGBA_8888:
        case LE_COLOR_MODE_ARGB_8888:
        {
            ptr = ((uint8_t*)pal->header.dataAddress) + (idx * 4);
            
            clr = ((uint32_t*)ptr)[0];
                
            break;
        }
        default:
        {
            break;
        }
    }
        
    return clr;
}