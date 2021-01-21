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


#include "gfx/legato/widget/line/legato_widget_line.h"

#if LE_LINE_WIDGET_ENABLED

#include "gfx/legato/renderer/legato_renderer.h"
#include "gfx/legato/string/legato_string.h"
#include "gfx/legato/common/legato_utils.h"
#include "gfx/legato/widget/legato_widget.h"

#include "gfx/legato/widget/legato_widget_skin_classic_common.h"

enum
{
    NOT_STARTED = LE_WIDGET_DRAW_STATE_READY,
    DONE = LE_WIDGET_DRAW_STATE_DONE,
    DRAW_BACKGROUND,
    DRAW_LINE,
    DRAW_BORDER,
};

static struct
{
    uint32_t alpha;
} paintState;

static void drawBackground(leLineWidget* line);
static void drawLine(leLineWidget* line);
static void drawBorder(leLineWidget* line);

static void nextState(leLineWidget* line)
{
    switch(line->widget.status.drawState)
    {
        case NOT_STARTED:
        {
            paintState.alpha = 255;

#if LE_ALPHA_BLENDING_ENABLED == 1
            if(line->fn->getCumulativeAlphaEnabled(line) == LE_TRUE)
            {
                paintState.alpha = line->fn->getCumulativeAlphaAmount(line);
            }
#endif

            if(line->widget.style.backgroundType != LE_WIDGET_BACKGROUND_NONE)
            {
                line->widget.status.drawState = DRAW_BACKGROUND;
                line->widget.drawFunc = (leWidget_DrawFunction_FnPtr)&drawBackground;

                return;
            }
        }
        // fall through
        case DRAW_BACKGROUND:
        {
            line->widget.status.drawState = DRAW_LINE;
            line->widget.drawFunc = (leWidget_DrawFunction_FnPtr)&drawLine;

            return;
        }
        case DRAW_LINE:
        {
            if(line->widget.style.borderType != LE_WIDGET_BORDER_NONE)
            {
                line->widget.drawFunc = (leWidget_DrawFunction_FnPtr)&drawBorder;
                line->widget.status.drawState = DRAW_BORDER;

                return;
            }
        }
        // fall through
        case DRAW_BORDER:
        {
            line->widget.status.drawState = DONE;
            line->widget.drawFunc = NULL;
        }
    }
}

static void drawBackground(leLineWidget* line)
{
    leWidget_SkinClassic_DrawStandardBackground((leWidget*)line,
                                                paintState.alpha);

    nextState(line);
}

static void drawLine(leLineWidget* line)
{
    lePoint p1, p2;

    p1.x = line->x1;
    p1.y = line->y1;
    p2.x = line->x2;
    p2.y = line->y2;

    leUtils_PointToScreenSpace((leWidget*)line, &p1);
    leUtils_PointToScreenSpace((leWidget*)line, &p2);

    leRenderer_DrawLine(p1.x,
                        p1.y,
                        p2.x,
                        p2.y,
                        leScheme_GetRenderColor(line->widget.scheme, LE_SCHM_FOREGROUND),
                        paintState.alpha);

    nextState(line);
}

static void drawBorder(leLineWidget* line)
{
    if(line->widget.style.borderType == LE_WIDGET_BORDER_LINE)
    {
        leWidget_SkinClassic_DrawStandardLineBorder((leWidget*)line,
                                                    paintState.alpha);
    }
    else if(line->widget.style.borderType == LE_WIDGET_BORDER_BEVEL)
    {
        leWidget_SkinClassic_DrawStandardRaisedBorder((leWidget*)line,
                                                      paintState.alpha);
    }

    nextState(line);
}

void _leLineWidget_Paint(leLineWidget* line)
{
    if(line->widget.status.drawState == NOT_STARTED)
        nextState(line);

    while(line->widget.status.drawState != DONE)
    {
        line->widget.drawFunc((leWidget*)line);

#if LE_PREEMPTION_LEVEL == 2
        break;
#endif
    }
}

#endif // LE_LINE_WIDGET_ENABLED