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


#include "gfx/legato/common/legato_utils.h"

#include "gfx/legato/datastructure/legato_list.h"
#include "gfx/legato/renderer/legato_renderer.h"
#include "gfx/legato/widget/legato_widget.h"

#include "gfx/legato/common/legato_rect.h"

leWidget* leUtils_PickFromWidget(const leWidget* parent,
                                 int32_t x,
                                 int32_t y)
{
    const leWidget* child;
    const leWidget* result;
    const leWidget* childResult;
    lePoint pnt;
    uint32_t i;

    pnt.x = x;
    pnt.y = y;

    result = parent;

    for(i = 0; i < parent->children.size; i++)
    {
        child = leArray_Get(&parent->children, i);

        // widget must be enabled and visible
        if(LE_TEST_FLAG(child->flags, LE_WIDGET_ENABLED) == LE_TRUE &&
           LE_TEST_FLAG(child->flags, LE_WIDGET_VISIBLE) == LE_TRUE &&
           leRectContainsPoint(&child->rect, &pnt) == LE_TRUE)
        {
            childResult = leUtils_PickFromWidget(child, x - child->rect.x, y - child->rect.y);

            if(childResult != NULL && LE_TEST_FLAG(childResult->flags, LE_WIDGET_IGNOREPICK) == LE_FALSE)
            {
                result = childResult;
            }
        }
    }

    if(result != NULL && LE_TEST_FLAG(result->flags, LE_WIDGET_IGNOREPICK) == LE_TRUE)
    {
        result = NULL;
    }

    return (leWidget*)result;
}

void leUtils_PointToScreenSpace(const leWidget* widget,
                                lePoint* pnt)
{
    if(widget == NULL || pnt == NULL)
        return;
        
    while(widget != NULL)
    {
        pnt->x += widget->rect.x;
        pnt->y += widget->rect.y;

        widget = widget->parent;
    }
}

void leUtils_PointScreenToLocalSpace(const leWidget* widget,
                                     lePoint* pnt)
{
    leWidget* parent;

    if(widget == NULL || pnt == NULL)
        return;
        
    pnt->x -= widget->rect.x;
    pnt->y -= widget->rect.y;

    parent = widget->parent;

    while(parent != NULL)
    {
        pnt->x -= parent->rect.x;
        pnt->y -= parent->rect.y;

        parent = parent->parent;
    }
}

void leUtils_ClipRectToParent(const leWidget* widget,
                              leRect* rect)
{
    leRect result;

    if(widget == NULL || rect == NULL)
        return;

    rect->x += widget->rect.x;
    rect->y += widget->rect.y;

    leRectClip(rect, &widget->rect, &result);

    result.x -= widget->rect.x;
    result.y -= widget->rect.y;

    *rect = result;
}

void leUtils_RectToParentSpace(const leWidget* widget,
                               leRect* rect)
{
    if(widget == NULL || rect == NULL || widget->parent == NULL)
        return;

    rect->x += widget->parent->rect.x;
    rect->y += widget->parent->rect.y;
}

void leUtils_RectFromParentSpace(const leWidget* widget,
                                 leRect* rect)
{
    if(widget == NULL || rect == NULL || widget->parent == NULL)
        return;

    rect->x -= widget->parent->rect.x;
    rect->y -= widget->parent->rect.y;
}

void leUtils_RectToScreenSpace(const leWidget* widget,
                               leRect* rect)
{
    if(widget == NULL || rect == NULL)
        return;
        
    while(widget != NULL)
    {
        rect->x += widget->rect.x;
        rect->y += widget->rect.y;

        widget = widget->parent;
    }
}

leBool leUtils_ChildIntersectsParent(const leWidget* parent,
                                     const leWidget* child)
{
    leRect childRect;
    
    if(parent == NULL || child == NULL)
        return LE_FALSE;
        
    childRect = child->rect;
    
    childRect.x += parent->rect.x;
    childRect.y += parent->rect.y;

    return leRectIntersects(&childRect, &parent->rect);
}

void leUtils_ClipRectToAncestors(const leWidget* wgt,
                                 struct leRect* res)
{
    leRect rct, prct;
    leWidget* parent;

    LE_PCALL(wgt, rectToParent, &rct);

    parent = wgt->parent;

    while(parent != NULL)
    {
        LE_PCALL(parent, localRect, &prct);

        leRectClip(&rct, &prct, &rct);

        rct.x += parent->rect.x;
        rct.y += parent->rect.y;

        parent = parent->parent;
    }

    *res = rct;
}

leWidget* leUtils_GetNextHighestWidget(const leWidget* wgt)
{
    int32_t idx;
    leWidget *target, *res;
    
    // return first child
    if(wgt->children.size != 0)
        return wgt->children.values[0];
    
    if(wgt->parent == NULL)
        return NULL;
    
    target = wgt->parent;
    
    // return next viable sibling
    while(target != NULL)
    {
        idx = wgt->fn->getIndexOfChild(wgt, target);
    
        res = target->fn->getChildAtIndex(target, ++idx);
        
        if(res != NULL)
            return res;
            
        wgt = target;
        target = wgt->parent;
    }
    
    return NULL;
}

void leUtils_ArrangeRectangle(leRect* sub,
                              leRect obj,
                              leRect bounds,
                              leHAlignment hAlignment,
                              leVAlignment vAlignment,
                              leRelativePosition position,
                              uint8_t leftMargin,
                              uint8_t topMargin,
                              uint8_t rightMargin,
                              uint8_t bottomMargin,
                              uint16_t rectMargin)
{
    sub->x = 0;
    sub->y = 0;
    
    if(hAlignment == LE_HALIGN_LEFT)
    {
        sub->x = bounds.x + leftMargin;
        
        if(obj.width > 0 && position == LE_RELATIVE_POSITION_RIGHTOF)
            sub->x += obj.width + rectMargin;
    }
    else if(hAlignment == LE_HALIGN_CENTER)
    {
        sub->x = bounds.x + (bounds.width / 2) - (sub->width / 2);
        
        if(obj.width > 0)
        {
            if(position == LE_RELATIVE_POSITION_LEFTOF)
                sub->x -= (obj.width / 2) + (rectMargin / 2);
            else if(position == LE_RELATIVE_POSITION_RIGHTOF)
                sub->x += (obj.width / 2) + (rectMargin / 2);
        }
    }
    else
    {
        sub->x = bounds.x + bounds.width - sub->width - rightMargin;
        
        if(obj.width > 0 && position == LE_RELATIVE_POSITION_LEFTOF)
            sub->x -= obj.width + rectMargin;
    }
    
    if(vAlignment == LE_VALIGN_TOP)
    {
        sub->y = bounds.y + topMargin + 1;
        
        if(obj.height > 0)
        {
            if(position == LE_RELATIVE_POSITION_BELOW)
                sub->y += obj.height + rectMargin;
        }
    }
    else if(vAlignment == LE_VALIGN_MIDDLE)
    {
        sub->y = bounds.y + (bounds.height / 2) - (sub->height / 2);
        
        if(obj.height > 0)
        {
            if(position == LE_RELATIVE_POSITION_ABOVE)
                sub->y -= (obj.height / 2) + (rectMargin / 2);
            else if(position == LE_RELATIVE_POSITION_BELOW)
                sub->y += (obj.height / 2) + (rectMargin / 2);
        }
    }
    else
    {
        sub->y = bounds.y + bounds.height - sub->height - bottomMargin ;
        
        if(obj.height > 0 && position == LE_RELATIVE_POSITION_ABOVE)
            sub->y -= obj.height + rectMargin;
    }
}

void leUtils_ArrangeRectangleRelative(leRect* sub,
                                      leRect obj,
                                      leRect bounds,
                                      leHAlignment hAlignment,
                                      leVAlignment vAlignment,
                                      leRelativePosition position,
                                      uint8_t leftMargin,
                                      uint8_t topMargin,
                                      uint8_t rightMargin,
                                      uint8_t bottomMargin,
                                      uint16_t rectMargin)
{
    sub->x = 0;
    sub->y = 0;
    
    if(hAlignment == LE_HALIGN_LEFT)
    {
        sub->x = bounds.x + leftMargin;
        
        if(obj.width > 0)
        {
            if(position == LE_RELATIVE_POSITION_LEFTOF)
                sub->x += obj.width + rectMargin;
            //else
            //    sub->x += (obj.width / 2) - (sub->width / 2);
        }
    }
    else if(hAlignment == LE_HALIGN_CENTER)
    {
        sub->x = bounds.x + (bounds.width / 2) - (sub->width / 2);
        
        if(obj.width > 0)
        {
            if(position == LE_RELATIVE_POSITION_LEFTOF)
                sub->x += (obj.width / 2) + (rectMargin / 2);
            else if(position == LE_RELATIVE_POSITION_RIGHTOF)
                sub->x -= (obj.width / 2) + (rectMargin / 2);
        }
    }
    else
    {
        sub->x = bounds.x + bounds.width - sub->width - rightMargin;
        
        if(obj.width > 0)
        {   
            if(position == LE_RELATIVE_POSITION_RIGHTOF)
                sub->x -= obj.width + rectMargin;
            //else
            //    sub->x -= (obj.width / 2) - (sub->width / 2);
        }
    }
    
    if(vAlignment == LE_VALIGN_TOP)
    {
        sub->y = bounds.y + topMargin + 1;
        
        if(obj.height > 0 && position != LE_RELATIVE_POSITION_BELOW)
        {
            if(position == LE_RELATIVE_POSITION_ABOVE)
                sub->y += obj.height + rectMargin;
            else
                sub->y += (obj.height / 2) - (sub->height / 2);
            
            if (sub->y < (bounds.y + topMargin + 1))
                sub->y = bounds.y + topMargin + 1;
        }
    }
    else if(vAlignment == LE_VALIGN_MIDDLE)
    {
        sub->y = bounds.y + (bounds.height / 2) - (sub->height / 2);
        
        if(obj.height > 0)
        {
            if(position == LE_RELATIVE_POSITION_ABOVE)
                sub->y += (obj.height / 2) + (rectMargin / 2);
            else if(position == LE_RELATIVE_POSITION_BELOW)
                sub->y -= (obj.height / 2) + (rectMargin / 2);
        }
    }
    else
    {
        sub->y = bounds.y + bounds.height - sub->height - bottomMargin;
        
        if(obj.height > 0 && position != LE_RELATIVE_POSITION_ABOVE)
        {
            if(position == LE_RELATIVE_POSITION_BELOW)
                sub->y -= obj.height + rectMargin;
            else
                sub->y -= (obj.height / 2) - (sub->height / 2);
            
            if ((sub->y + sub->height) > (bounds.y + bounds.height - bottomMargin))
                sub->y = bounds.y + bounds.height - sub->height - bottomMargin;
        }
    }
}

#if 0
leBool _occlusionCullTestAscending(const leWidget* widget,
                                   leRect* bounds)
{
    uint32_t i, idx;
    leWidget *parent, *child;
    leRect childRect;
    
    // root node of tree, can't possibly occlude
    if(widget->parent == NULL)
        return LE_FALSE;
        
    parent = widget->parent;
        
    bounds->x += parent->rect.x;
    bounds->y += parent->rect.y;
    
    idx = leArray_Find(&parent->children, widget);
    
    if(idx < parent->children.size - 1)
    {    
        idx += 1;
        
        // test all rightside siblings, leftside can't occlude
        for(i = idx; i < parent->children.size; i++)
        {
            child = parent->children.values[i];
            
            childRect = child->rect;
            
            // move child rect into parent space
            //childRect.x += widget->rect.x;
            //childRect.y += widget->rect.y;
            
            if(leRectContainsRect(&childRect, bounds) == LE_TRUE)
                return LE_TRUE;
        }
    }
    
    return _occlusionCullTestAscending(parent, bounds);
}

leBool leUtils_OcclusionCullTest(const leWidget* widget)
{
    uint32_t i;
    leWidget* child;
    leRect childRect;
    
    if(widget == NULL)
        return LE_FALSE;
    
    // test children first
    for(i = 0; i < widget->children.size; i++)
    {
        child = widget->children.values[i];
        
        childRect = child->rect;
        
        // move child rect into parent space
        //childRect.x += widget->rect.x;
        //childRect.y += widget->rect.y;
        
        if(leRectContainsRect(&childRect, &widget->rect) == LE_TRUE)
            return LE_TRUE;
    }
    
    // test rightmost ancestors
    childRect = widget->rect;
    
    return _occlusionCullTestAscending(widget, &childRect);
}

void leUtils_ListOcclusionCullTest(const leList* list, leRect rect)
{
    uint32_t i, j;
    leWidget *widget, *higherWidget;
    leRect widgetRect;//, lclippedRect, rclippedRect;
    leList cullList;
    leBool cull;
    
    leList_Create(&cullList);
    leList_Copy(&cullList, list);
    leList_Clear(list);
    
    if(cullList.size == 0)
        return;

    for(i = 0; i <= cullList.size - 1; i++)
    {
        cull = LE_FALSE;
        
        widget = (leWidget*)leList_Get(&cullList, i);
        
        // topmost widget is always a pass
        if(i == cullList.size - 1)
        {
            leList_PushBack(list, widget);
        
            continue;
        }
        
        widgetRect = widget->fn->rectToScreen(widget);
        
        //leRectClip(&widgetRect, &rect, &lclippedRect);
        
        for(j = i + 1; j < cullList.size; j++)
        {
            higherWidget = (leWidget*)leList_Get(&cullList, j);
            
            //leRectClip(&higherWidget->rect, &widgetRect, &rclippedRect);
            
            if(leRectContainsRect(&higherWidget->rect, &widgetRect) == LE_TRUE &&
               higherWidget->fn->isOpaque(higherWidget) == LE_TRUE)
            {
                cull = LE_TRUE;
                
                break;
            }
        }
        
        if(cull == LE_FALSE)
            leList_PushBack(list, widget);
    }
    
    leList_Clear(&cullList);
}

leBool leUtils_WidgetIsOccluded(const leWidget* wgt, const leRect* rect)
{
    leWidget* target;
    leRect targetRect;
    
    if(wgt->fn->getCumulativeAlphaEnabled(wgt) == LE_TRUE)
        return LE_FALSE;
    
    target = leUtils_GetNextHighestWidget(wgt);
    
    while(target != NULL)
    {
        targetRect = target->fn->rectToScreen(target);
        
        if(target->visible == LE_TRUE &&
           target->alphaEnabled == LE_FALSE &&
           target->backgroundType != LE_WIDGET_BACKGROUND_NONE &&
           leRectContainsRect(&targetRect, rect) == LE_TRUE)
            return LE_TRUE;
            
        target = leUtils_GetNextHighestWidget(target);
    }
    
    return LE_FALSE;
}
#endif

void leUtils_PointLogicalToScratch(int16_t* x,
                                   int16_t* y)
{
    int32_t rotX, rotY;

#if LE_RENDER_ORIENTATION == 0
    rotX = *x;
    rotY = *y;
#elif LE_RENDER_ORIENTATION == 90
    lePixelBuffer* buf = leGetRenderBuffer();

    rotY = *x;
    rotX = buf->size.width - 1 - *y;
#elif LE_RENDER_ORIENTATION == 180
    lePixelBuffer* buf = leGetRenderBuffer();

    rotX = buf->size.width - 1 - *x;
    rotY = buf->size.height - 1 - *y;
#else
    lePixelBuffer* buf = leGetRenderBuffer();

    rotY = buf->size.height - 1 - *x;
    rotX = *y;
#endif

    *x = rotX;
    *y = rotY;
}

void leUtils_RectLogicalToScratch(leRect* rect)
{
    int32_t rotX, rotY;


#if LE_RENDER_ORIENTATION != 0

#endif

#if LE_RENDER_ORIENTATION == 0
    rotX = rect->x;
    rotY = rect->y;
#elif LE_RENDER_ORIENTATION == 90
    lePixelBuffer* buf = leGetRenderBuffer();
    int16_t t;

    rotY = rect->x;
    rotX = buf->size.width - (rect->y + rect->height);

    t = rect->width;
    rect->width = rect->height;
    rect->height = t;
#elif LE_RENDER_ORIENTATION == 180
    lePixelBuffer* buf = leGetRenderBuffer();

    rotX = buf->size.width - rect->width - rect->x;
    rotY = buf->size.height - rect->height - rect->y;
#else
    lePixelBuffer* buf = leGetRenderBuffer();
    int16_t t;

    rotY = buf->size.height - rect->width - rect->x;
    rotX = rect->y;

    t = rect->width;
    rect->width = rect->height;
    rect->height = t;
#endif

    rect->x = rotX;
    rect->y = rotY;
}

const leBlendLookupTable* leUtils_GetSchemeLookupTable(const leScheme* schm,
                                                       leColor foreground,
                                                       leColor background)
{
    uint32_t itr;

    if(schm == NULL)
        return NULL;

    for(itr = 0; itr < schm->blendTableCount; ++itr)
    {
        if(schm->blendTables[itr]->foreground == foreground &&
           schm->blendTables[itr]->background == background)
        {
            return schm->blendTables[itr];
        }
    }

    return NULL;
}