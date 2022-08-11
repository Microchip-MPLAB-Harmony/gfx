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

#include "gfx/legato/renderer/legato_renderer.h"

#include "gfx/legato/common/legato_math.h"
#include "gfx/legato/datastructure/legato_rectarray.h"

leResult leRenderer_RectLine(const leRect* rect,
                             leColor clr,
                             uint32_t a)
{
    leRenderer_HorzLine(rect->x,
                        rect->y,
                        rect->width,
                        clr,
                        a);
                        
    leRenderer_HorzLine(rect->x,
                        rect->y + rect->height - 1,
                        rect->width,
                        clr,
                        a);
                        
    leRenderer_VertLine(rect->x,
                        rect->y,
                        rect->height,
                        clr,
                        a);
                        
    leRenderer_VertLine(rect->x + rect->width - 1,
                        rect->y,
                        rect->height,
                        clr,
                        a);
    
    return LE_SUCCESS;
}

leResult leRenderer_RectFill(const leRect* rect,
                             leColor clr,
                             uint32_t a)
{
    leRect clipRect;

    if(leRenderer_CullDrawRect(rect) == LE_TRUE)
        return LE_FAILURE;
        
    leRenderer_ClipDrawRect(rect, &clipRect);
    
    a = leClampi(0, 255, a);

    leRenderer_FillArea(clipRect.x,
                        clipRect.y,
                        clipRect.width,
                        clipRect.height,
                        clr,
                        a);

    return LE_SUCCESS;
}

leResult leRenderer_HorzGradientRect(const leRect* rect,
                                     leColor clr1,
                                     leColor clr2,
                                     uint32_t a)
{
    lePoint drawPoint;
    leColor clr;
    int32_t val;
    uint32_t percent;
    
    // calculate minimums
    drawPoint.y = rect->y;
    
    for(drawPoint.x = rect->x; drawPoint.x < rect->x + rect->width; drawPoint.x++)
    {
        val = drawPoint.x - rect->x;
        
        percent = lePercentWholeRounded(val, rect->width);
        
        clr = leColorLerp(clr1, clr2, percent, leRenderer_CurrentColorMode());
                                 
        leRenderer_VertLine(drawPoint.x,
                            drawPoint.y,
                            rect->height,
                            clr,
                            a);
    }
    
    return LE_SUCCESS;
}
                                          
leResult leRenderer_VertGradientRect(const leRect* rect,
                                     leColor clr1,
                                     leColor clr2,
                                     uint32_t a)
{
    lePoint drawPoint;
    leColor clr;
    int32_t val;
    uint32_t percent;
    
    // calculate minimums
    drawPoint.x = rect->x; 
    
    for(drawPoint.y = rect->y; drawPoint.y < rect->y + rect->height; drawPoint.y++)
    {
        val = drawPoint.y - rect->y;
        
        percent = lePercentWholeRounded(val, rect->height);
        
        clr = leColorLerp(clr1, clr2, percent, leRenderer_CurrentColorMode());
                                 
        leRenderer_HorzLine(drawPoint.x,
                            drawPoint.y,
                            rect->width,
                            clr,
                            a);
    }
    
    return LE_SUCCESS;
}