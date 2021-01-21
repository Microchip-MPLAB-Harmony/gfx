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


#include "gfx/libaria/inc/libaria_widget_label.h"

#if LA_LABEL_WIDGET_ENABLED

#include "gfx/libaria/inc/libaria_context.h"
#include "gfx/libaria/inc/libaria_draw.h"
#include "gfx/libaria/inc/libaria_layer.h"
#include "gfx/libaria/inc/libaria_string.h"
#include "gfx/libaria/inc/libaria_utils.h"
#include "gfx/libaria/inc/libaria_widget.h"

#include "gfx/libaria/inc/libaria_widget_skin_classic_common.h"

#define DEFAULT_NUM_LINES 5

enum
{
    NOT_STARTED = LA_WIDGET_DRAW_STATE_READY,
    DONE = LA_WIDGET_DRAW_STATE_DONE,
    DRAW_BACKGROUND,
    DRAW_STRING,
    WAIT_STRING,
    DRAW_BORDER,
};

void _laLabelWidget_GetTextRect(laLabelWidget* lbl,
                                GFX_Rect* textRect,
								GFX_Rect* drawRect)
{
    GFX_Rect bounds;
    
    bounds = laUtils_WidgetLocalRect((laWidget*)lbl);
    
    laString_GetMultiLineRect(&lbl->text, textRect, lbl->textLineSpace);
    
    // arrange relative to image rect
    laUtils_ArrangeRectangleRelative(textRect,
                                     GFX_Rect_Zero,
                                     bounds,
                                     lbl->halign,
                                     lbl->valign,
                                     0,
                                     lbl->widget.margin.left,
                                     lbl->widget.margin.top,
                                     lbl->widget.margin.right,
                                     lbl->widget.margin.bottom,
                                     0);

    GFX_RectClip(textRect, &bounds, drawRect);

	// move the rects to layer space
	laUtils_RectToLayerSpace((laWidget*)lbl, textRect);
    laUtils_RectToLayerSpace((laWidget*)lbl, drawRect);
}

static void drawBackground(laLabelWidget* lbl);
static void drawMultiLineString(laLabelWidget* lbl);
static void waitString(laLabelWidget* lbl);
static void drawBorder(laLabelWidget* lbl);



static void nextState(laLabelWidget* lbl)
{
    switch(lbl->widget.drawState)
    {
        case NOT_STARTED:
        {
            //printf("layer painting\n");
            
            if(lbl->widget.backgroundType != LA_WIDGET_BACKGROUND_NONE) 
            {
                lbl->widget.drawState = DRAW_BACKGROUND;
                lbl->widget.drawFunc = (laWidget_DrawFunction_FnPtr)&drawBackground;

                return;
            }
        }
        case DRAW_BACKGROUND:
        {
            if(laString_IsEmpty(&lbl->text) == LA_FALSE)
            {
                lbl->widget.drawState = DRAW_STRING;
                lbl->widget.drawFunc = (laWidget_DrawFunction_FnPtr)&drawMultiLineString;

                return;
            }
        }
        case DRAW_STRING:
        {
            if(lbl->widget.borderType != LA_WIDGET_BORDER_NONE)
            {
                lbl->widget.drawFunc = (laWidget_DrawFunction_FnPtr)&drawBorder;
                lbl->widget.drawState = DRAW_BORDER;
                
                return;
            }
        }
        case DRAW_BORDER:
        {
            lbl->widget.drawState = DONE;
            lbl->widget.drawFunc = NULL;
        }
    }
}

static void drawBackground(laLabelWidget* lbl)
{
    laWidget_SkinClassic_DrawStandardBackground((laWidget*)lbl);
    
    nextState(lbl);
}

static void drawMultiLineString(laLabelWidget* lbl)
{
    GFX_Rect textRect = {0}, clipRect = {0}, drawRect = {0};
    laLayer* layer = laUtils_GetLayer((laWidget*)lbl);
	uint32_t newoffset = 0;
    uint32_t end = 0;
	GFX_Rect bounds;
	uint32_t numlines = 0;
    uint32_t line = 0;
    uint32_t lineY = 0;
    uint32_t maxLines = DEFAULT_NUM_LINES;
    
    GFX_Rect * lineRect = laContext_GetActive()->memIntf.heap.calloc(maxLines * sizeof(GFX_Rect), 1);
    uint32_t * offset = laContext_GetActive()->memIntf.heap.calloc(maxLines * sizeof(uint32_t), 1);
    
	bounds = laUtils_WidgetLocalRect((laWidget*)lbl);

	while (lineRect != NULL && offset != NULL)
	{
		offset[numlines] = newoffset;

		laString_GetLineRect(&lbl->text, offset[numlines], &lineRect[numlines], &newoffset);

        if (offset[numlines] == newoffset)
        {
            if (lbl->textLineSpace >= 0)
                textRect.height += lbl->textLineSpace;
            else
                textRect.height += laString_GetHeight(&lbl->text) - 
                               laString_GetAscent(&lbl->text);            
            
            break;
        }

        if (lbl->textLineSpace >= 0)
            textRect.height += lbl->textLineSpace;
        else
            textRect.height += laString_GetAscent(&lbl->text);        
        

		if (lineRect[numlines].width > textRect.width)
		{
			textRect.width = lineRect[numlines].width;
		}

		numlines++;
        
        if (numlines >= maxLines)
        {
            maxLines += DEFAULT_NUM_LINES;
            
            lineRect = laContext_GetActive()->memIntf.heap.realloc(lineRect, maxLines * sizeof(GFX_Rect));
            offset = laContext_GetActive()->memIntf.heap.realloc(offset, maxLines * sizeof(uint32_t));
            
            if (lineRect == NULL || offset == NULL)
            {
                //Unable to allocate for all lines
                numlines = 0;
                break;
            }
        }
	}

	// arrange relative to image rect
    laUtils_ArrangeRectangleRelative(&textRect,
                                     GFX_Rect_Zero,
                                     bounds,
                                     lbl->halign,
                                     lbl->valign,
                                     0,
                                     lbl->widget.margin.left,
                                     lbl->widget.margin.top,
                                     lbl->widget.margin.right,
                                     lbl->widget.margin.bottom,
                                     0);

    //Clip text to widget rectangle size
    GFX_RectClip(&textRect, &bounds, &drawRect);

	// move the rects to layer space
	laUtils_RectToLayerSpace((laWidget*)lbl, &textRect);
    laUtils_RectToLayerSpace((laWidget*)lbl, &drawRect);

    for (line = 0; line < numlines; line++)
    {
        //Horizonally align the line rectangle relative to textRect 
        switch(lbl->halign)
        {
            case LA_HALIGN_LEFT:
                //No break. Fall through
            default:
                lineRect[line].x = textRect.x;
                break;
            case LA_HALIGN_CENTER:
                lineRect[line].x = textRect.x + textRect.width/2 - lineRect[line].width/2;
                break;
            case LA_HALIGN_RIGHT:
                lineRect[line].x = textRect.x + textRect.width - lineRect[line].width;
                break;
        }

        lineRect[line].y = textRect.y + lineY;

        //Clip to the line rectangle 
        GFX_RectClip(&textRect, &lineRect[line], &drawRect);

        if(GFX_RectIntersects(&layer->clippedDrawingRect, &drawRect) == GFX_TRUE)
        {
            GFX_Set(GFXF_DRAW_MASK_ENABLE, GFX_FALSE);
            GFX_Set(GFXF_DRAW_COLOR, lbl->widget.scheme->text);

            GFX_RectClip(&drawRect, &layer->clippedDrawingRect, &clipRect);

            if (line + 1 < numlines)
            {
                end = offset[line + 1];
            }
            else
            {
                end = newoffset;
            }

            laString_DrawSubStringClipped(&lbl->text,
                                         offset[line],
                                         end,
                                         clipRect.x,
                                         clipRect.y,
                                         clipRect.width,
                                         clipRect.height,
                                         lineRect[line].x,
                                         lineRect[line].y,
                                         &lbl->reader);
            if(lbl->reader != NULL)
            {
                lbl->widget.drawFunc = (laWidget_DrawFunction_FnPtr)&waitString;
                lbl->widget.drawState = WAIT_STRING;
                
                laContext_GetActive()->memIntf.heap.free(lineRect);
                laContext_GetActive()->memIntf.heap.free(offset);
                
                return;
            }
        }

        if (lbl->textLineSpace >= 0)
            lineY += lbl->textLineSpace;
        else
            lineY += laString_GetAscent(&lbl->text);            
            
    }

    nextState(lbl);
    
    laContext_GetActive()->memIntf.heap.free(lineRect);
    laContext_GetActive()->memIntf.heap.free(offset);
}

static void waitString(laLabelWidget* lbl)
{
    if(lbl->reader->status != GFXU_READER_STATUS_FINISHED)
    {
        lbl->reader->run(lbl->reader);
        
        return;
    }
    
    // free the reader
    lbl->reader->memIntf->heap.free(lbl->reader);
    lbl->reader = NULL;
    
    lbl->widget.drawState = DRAW_STRING;
    
    nextState(lbl);
}

static void drawBorder(laLabelWidget* lbl)
{
    if(lbl->widget.borderType == LA_WIDGET_BORDER_LINE)
        laWidget_SkinClassic_DrawStandardLineBorder((laWidget*)lbl);
    else if(lbl->widget.borderType == LA_WIDGET_BORDER_BEVEL)
        laWidget_SkinClassic_DrawStandardRaisedBorder((laWidget*)lbl);
    
    nextState(lbl);
}

void _laLabelWidget_Paint(laLabelWidget* lbl)
{
    laContext* context = laContext_GetActive();
    
    if(lbl->widget.scheme == NULL)
    {
        lbl->widget.drawState = DONE;
        
        return;
    }

    if(lbl->widget.drawState == NOT_STARTED)
        nextState(lbl);
    
    while(lbl->widget.drawState != DONE)
    {
        lbl->widget.drawFunc((laWidget*)lbl);
        
        if(context->preemptLevel == LA_PREEMPTION_LEVEL_2 ||
           lbl->widget.drawState == WAIT_STRING)
            break;
    }
}

#endif // LA_IMAGE_WIDGET_ENABLED
