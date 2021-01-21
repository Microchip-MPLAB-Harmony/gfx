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


#include "gfx/legato/common/legato_rect.h"

#include "gfx/legato/common/legato_math.h"

leBool leRectContainsPoint(const leRect* rect, const lePoint* point)
{
    if(point->x < rect->x || point->x >= rect->x + rect->width ||
       point->y < rect->y || point->y >= rect->y + rect->height)
        return LE_FALSE;
        
    return LE_TRUE;
}

leBool leRectContainsRect(const leRect* l_rect, const leRect* r_rect)
{
    if (l_rect->x <= r_rect->x && 
        r_rect->x  + r_rect->width - 1 <= l_rect->x + l_rect->width - 1 &&
        l_rect->y <= r_rect->y && 
        r_rect->y  + r_rect->height - 1 <= l_rect->y + l_rect->height - 1)
        return LE_TRUE;
    
    return LE_FALSE;
}

leBool leRectIntersects(const leRect* l_rect, const leRect* r_rect)
{
    if (l_rect->x <= r_rect->x + r_rect->width - 1 &&
        r_rect->x <= l_rect->x + l_rect->width - 1 &&
        l_rect->y <= r_rect->y + r_rect->height - 1)
        return r_rect->y <= l_rect->y + l_rect->height - 1;
    
    return LE_FALSE;
}

void leRectCombine(const leRect* l_rect,
                   const leRect* r_rect,
                   leRect* res)
{
    leRect result;

    result.x = leMini(l_rect->x, r_rect->x);
    result.y = leMini(l_rect->y, r_rect->y);

    result.width = leMaxi(l_rect->x + l_rect->width, r_rect->x + r_rect->width) - result.x;
    result.height = leMaxi(l_rect->y + l_rect->height, r_rect->y + r_rect->height) - result.y;

    *res = result;
}

void leRectClipAdj(const leRect* l_rect,
                   const leRect* r_rect,
                   leRect* adj,
                   leRect* res)
{
    *res = *l_rect;

    if(res->x < r_rect->x)
    {
        adj->x += r_rect->x - res->x;
        adj->width -= r_rect->x - res->x;
        
        res->x = r_rect->x;
        res->width -= res->x - l_rect->x;
    }
    else
    {
        res->x = l_rect->x;
    }
    
    if(res->x + res->width > r_rect->x + r_rect->width)
    {
        adj->width -= res->width - (r_rect->x + r_rect->width - res->x);
        
        res->width = r_rect->x + r_rect->width - res->x;
    }
       
    if(res->y < r_rect->y)
    {
        adj->y += r_rect->y - res->y;
        adj->height -= r_rect->y - res->y;
        
        res->y = r_rect->y;
        res->height -= res->y - l_rect->y;
    }
    else
    {
        res->y = l_rect->y;
    }
    
    if(res->y + res->height > r_rect->y + r_rect->height)
    {
        adj->height -= res->height - (r_rect->y + r_rect->height - res->y);
        
        res->height = r_rect->y + r_rect->height - res->y;
    }
}

void leRectClip(const leRect* l_rect,
                const leRect* r_rect,
                leRect* result)
{
    leRect res;

    res = *l_rect;

    if(res.x < r_rect->x)
    {
        res.x = r_rect->x;
        res.width -= res.x - l_rect->x;
    }
    
    if(res.x + res.width - 1 > r_rect->x + r_rect->width - 1)
        res.width = r_rect->x + r_rect->width - res.x;
        
    if(res.y < r_rect->y)
    {
        res.y = r_rect->y;
        res.height -= res.y - l_rect->y;
    }
    
    if(res.y + res.height > r_rect->y + r_rect->height)
        res.height = r_rect->y + r_rect->height - res.y;
        
    *result = res;
}

void leRectFromPoints(const lePoint* p1,
                      const lePoint* p2,
                      leRect* res)
{
    int32_t minx, miny, maxx, maxy;
    
    if(p1->x < p2->x)
    {
        minx = p1->x;
        maxx = p2->x;
    }
    else
    {
        minx = p2->x;
        maxx = p1->x;
    }
    
    if(p1->y < p2->y)
    {
        miny = p1->y;
        maxy = p2->y;
    }
    else
    {
        miny = p2->y;
        maxy = p1->y;
    }
    
    res->x = minx;
    res->y = miny;
    
    res->width = maxx - minx + 1;
    res->height = maxy - miny + 1;
}
                                       
void leRectToPoints(const leRect* rect,
                    lePoint* p1,
                    lePoint* p2)
{
    p1->x = rect->x;
    p1->y = rect->y;
    p2->x = rect->x + rect->width - 1;
    p2->y = rect->y + rect->height - 1;
}

uint32_t leRectSplit(const leRect* sub,
                       const leRect* obj,
                       leRect res[4])
{
    leRect rem = *sub;
    uint32_t count = 0;
    
    // rectangles actually have to intersect
    if(leRectIntersects(sub, obj) == LE_FALSE)
        return 0;
    
    // left subrect
    if(rem.x < obj->x)
    {
        res[count].x = rem.x;
        res[count].y = rem.y;
        res[count].width = obj->x - rem.x;
        res[count].height = rem.height;
        
        rem.x += res[count].width;
        rem.width -= res[count].width;
        
        count++;
    }
    
    // right subrect
    if(rem.x + rem.width > obj->x + obj->width)
    {
        res[count].x = (rem.x + rem.width) - ((rem.x + rem.width) - (obj->x + obj->width));
        res[count].y = rem.y;
        res[count].width = rem.x + rem.width - res[count].x;
        res[count].height = rem.height;
        
        rem.width -= res[count].width;
        
        count++;
    }
    
    // top subrect
    if(rem.y < obj->y)
    {
        res[count].x = rem.x;
        res[count].y = rem.y;
        res[count].width = rem.width;
        res[count].height = obj->y - rem.y;
        
        rem.y += res[count].height;
        rem.height -= res[count].height;
        
        count++;
    }
    
    // bottom subrect
    if(rem.y + rem.height > obj->y + obj->height)
    {
        res[count].x = rem.x;
        res[count].y = (rem.y + rem.height) - ((rem.y + rem.height) - (obj->y + obj->height));
        res[count].width = rem.width;
        res[count].height = rem.y + rem.height - res[count].y;
        
        rem.height -= res[count].height;
        
        count++;
    }
    
    return count;
} 

int32_t leRectCompare(const leRect* l, const leRect* r)
{
    return l->x == r->x && l->y == r->y && 
           l->width == r->width && l->height == r->height;
}

leBool leRectsAreSimilar(const leRect* l,
                             const leRect* r)
{
    //leRect combinedRect;
    //uint32_t lArea, rArea, cArea;
    
    // check for horizontal adjacency
    if(l->y == r->y &&
       l->height == r->height &&
       (l->x + l->width + 1 == r->x ||
        r->x + r->width + 1 == l->x))
        return LE_TRUE;
        
    // check for vertical adjacency
    if(l->x == r->x &&
       l->width == r->width &&
       (l->y + l->height + 1 == r->y ||
        r->y + r->height + 1 == l->y))
        return LE_TRUE;
        
    return LE_FALSE;
        
    // check rectangle areas
    /*lArea = l->width * l->height;
    rArea = r->width * r->height;
    
    combinedRect = leRectCombine(l, r);
    
    cArea = combinedRect.width * combinedRect.height;
    
    // if combined area is less than the cumulative area
    // of the two rectangles then these rectangles are
    // similar
    if(cArea >= lArea + rArea)
        return LE_FALSE;
        
    return LE_TRUE;*/
}
