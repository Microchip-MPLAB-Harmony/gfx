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

#include "gfx/legato/string/legato_string_renderer.h"

#include "gfx/legato/core/legato_state.h"
#include "gfx/legato/string/legato_stringtable.h"
#include "gfx/legato/string/legato_stringutils.h"

#if LE_STREAMING_ENABLED == 1

#define LE_STRING_SPACE     0x20 // ' '
#define LE_STRING_LINEBREAK 0xA // '\n'

typedef enum leStringStreamState
{
    SS_NONE,
    SS_INIT,
    SS_DRAW,
    SS_NEWLINE,
    SS_WAITING,
    SS_CLEANUP,
    SS_DONE
} leStringStreamState;

static struct
{
    leStreamManager manager;
    leFontStream* stream;

    leStringStreamState state;

    const leString* string;

    uint32_t charItr;
    uint32_t len;
    uint32_t lines;
    uint32_t lineItr;
    leFontGlyph glyphInfo;
    int32_t stringY;
    int32_t lineX;
    leRect stringRect;
    leRect lineRect;
    uint32_t startIdx;
    uint32_t endIdx;
    const leRasterFont* font;

    int32_t x;
    int32_t y;
    int32_t clipX;
    int32_t clipY;
    int32_t clipWidth;
    int32_t clipHeight;

    leHAlignment align;
    leColor color;
    uint32_t alpha;

    uint8_t stringData[LE_STRING_MAX_CHAR_WIDTH];

    uint32_t glyphOffset;
    uint32_t glyphRow;
    uint32_t glyphWidth;
    uint8_t* glyphDataAddress;
    uint8_t* glyphData;
    uint32_t glyphDataWidth;
    uint32_t glyphDataSize;
    uint32_t glyphDataCapacity;
} renderState;

static void drawDone(uint32_t codepoint)
{
    if(renderState.state != SS_WAITING)
        return;

    renderState.charItr += 1;

    renderState.lineX += renderState.glyphInfo.advance;

    if(renderState.charItr >= renderState.endIdx)
    {
        renderState.state = SS_NEWLINE;
    }
    else
    {
        codepoint = renderState.string->fn->charAt(renderState.string, renderState.charItr);

        leFont_GetGlyphInfo((leFont *) renderState.font, codepoint, &renderState.glyphInfo);

        renderState.state = SS_DRAW;
    }
}

static void drawDone_blocking(uint32_t codepoint)
{
    renderState.charItr += 1;

    renderState.lineX += renderState.glyphInfo.advance;

    if(renderState.charItr >= renderState.endIdx)
    {
        renderState.state = SS_NEWLINE;
    }
    else
    {
        codepoint = renderState.string->fn->charAt(renderState.string, renderState.charItr);

        leFont_GetGlyphInfo((leFont *) renderState.font, codepoint, &renderState.glyphInfo);

        renderState.state = SS_DRAW;
    }
}

static leResult draw(void)
{
    renderState.state = SS_WAITING;

    if(renderState.glyphInfo.codePoint == LE_STRING_SPACE ||
       renderState.glyphInfo.codePoint == LE_STRING_LINEBREAK)
    {
        drawDone(renderState.glyphInfo.codePoint);

        return LE_SUCCESS;
    }

    if(renderState.stream->drawGlyph(&renderState.glyphInfo,
                                     renderState.x + renderState.lineX + renderState.glyphInfo.bearingX,
                                     renderState.stringY + (renderState.font->baseline - renderState.glyphInfo.bearingY),
                                     renderState.color,
                                     renderState.alpha,
                                     drawDone) == LE_FAILURE)
    {
        drawDone(renderState.glyphInfo.codePoint);
    }

    return LE_SUCCESS;
}

static leResult draw_blocking(void)
{
    if(renderState.glyphInfo.codePoint == LE_STRING_SPACE ||
       renderState.glyphInfo.codePoint == LE_STRING_LINEBREAK)
    {
        drawDone_blocking(renderState.glyphInfo.codePoint);

        return LE_SUCCESS;
    }

    renderState.stream->drawGlyph(&renderState.glyphInfo,
                                  renderState.x + renderState.lineX + renderState.glyphInfo.bearingX,
                                  renderState.stringY + (renderState.font->baseline - renderState.glyphInfo.bearingY),
                                  renderState.color,
                                  renderState.alpha,
                                  NULL);

    drawDone_blocking(renderState.glyphInfo.codePoint);

    return LE_SUCCESS;
}

static void getLineMetrics(void)
{
    uint32_t codepoint;

    renderState.string->fn->getLineIndices(renderState.string,
                                           renderState.lineItr,
                                           &renderState.startIdx,
                                           &renderState.endIdx);

    renderState.string->fn->getLineRect(renderState.string,
                                        renderState.lineItr,
                                        &renderState.lineRect);

    if(renderState.align == LE_HALIGN_CENTER)
    {
        renderState.lineX = renderState.stringRect.x + (renderState.stringRect.width / 2) - (renderState.lineRect.width / 2);
    }
    else if(renderState.align == LE_HALIGN_RIGHT)
    {
        renderState.lineX = renderState.stringRect.x + renderState.stringRect.width - renderState.lineRect.width;
    }
    else
    {
        renderState.lineX = renderState.stringRect.x;
    }

    renderState.charItr = renderState.startIdx;

    codepoint = renderState.string->fn->charAt(renderState.string, renderState.charItr);

    leFont_GetGlyphInfo((leFont *) renderState.font, codepoint, &renderState.glyphInfo);
}

static leResult newline(void)
{
    renderState.lineItr += 1;

    if(renderState.lineItr >= renderState.lines)
    {
        renderState.state = SS_CLEANUP;

        return LE_SUCCESS;
    }

    getLineMetrics();

    renderState.stringY += renderState.font->height;

    renderState.state = SS_DRAW;

    return LE_SUCCESS;
}

static void cleanup(void)
{
    leGetState()->activeStream = NULL;

    renderState.stream->close();

    renderState.state = SS_DONE;

    if(renderState.manager.onDone != NULL)
    {
        renderState.manager.onDone(&renderState.manager);
    }
}

static leResult exec(leStreamManager* man)
{
    (void)man; // unused

    while(renderState.state != SS_DONE)
    {
        switch(renderState.state)
        {
            case SS_DRAW:
            {
                draw();

                if(renderState.state == SS_WAITING)
                    return LE_SUCCESS;

                break;
            }
            case SS_NEWLINE:
            {
                newline();

                break;
            }
            case SS_CLEANUP:
            {
                cleanup();

                break;
            }
            default:
            {
                return LE_SUCCESS;
            }
        }
    }

    return LE_SUCCESS;
}

static leResult exec_blocking(leStreamManager* man)
{
    (void)man; // unused

    while(renderState.state != SS_DONE)
    {
        switch(renderState.state)
        {
            case SS_DRAW:
            {
                draw_blocking();

                break;
            }
            case SS_NEWLINE:
            {
                newline();

                break;
            }
            case SS_CLEANUP:
            {
                cleanup();

                break;
            }
            default:
            {
                return LE_SUCCESS;
            }
        }
    }

    return LE_SUCCESS;
}

static leBool isDone(leStreamManager* man)
{
    (void)man; // unused

    return renderState.state == SS_DONE;
}

static void abortDraw(leStreamManager* man)
{
    (void)man; // unused

    renderState.state = SS_CLEANUP;
}

leResult _leStringStreamRenderer_Draw(leStringRenderRequest* req)
{
    uint32_t codepoint;

    if(leGetActiveStream() != NULL || req == NULL || req->str == NULL)
        return LE_FAILURE;

    memset(&renderState, 0, sizeof(renderState));

    // store local values
    renderState.state = SS_NONE;

    renderState.string = req->str;
    renderState.x = req->x;
    renderState.y = req->y;
    renderState.align = req->align;
    renderState.color = req->color;
    renderState.alpha = req->alpha;

    renderState.len = req->str->fn->length(req->str);
    renderState.font = (leRasterFont*)req->str->fn->getFont(req->str);

    if(renderState.len == 0 ||
       renderState.font == NULL ||
       renderState.font->glyphTable == NULL)
    {
        return LE_FAILURE;
    }

    renderState.stringY = req->y;

    renderState.lines = req->str->fn->getLineCount(req->str);

    req->str->fn->getRect(req->str, &renderState.stringRect);

    leStringUtils_KerningRect(renderState.font,
                              &renderState.stringRect);

    getLineMetrics();

    renderState.stream = leFont_GetStream((leFont*)renderState.font);

    if(renderState.stream == NULL ||
       renderState.stream->open() == LE_FAILURE)
    {
        return LE_FAILURE;
    }

    // set up first draw
    codepoint = renderState.string->fn->charAt(renderState.string, renderState.charItr);

    leFont_GetGlyphInfo((leFont*)renderState.font, codepoint, &renderState.glyphInfo);

    renderState.manager.isDone = isDone;
    renderState.manager.abort = abortDraw;

    renderState.state = SS_DRAW;

    // execute draw
    leGetState()->activeStream = (leStreamManager*)&renderState;

    if(leStream_IsBlocking((leStream*)renderState.stream) == LE_TRUE)
    {
        renderState.manager.exec = exec_blocking;
    }
    else
    {
        renderState.manager.exec = exec;
    }

    renderState.manager.exec((leStreamManager*) &renderState);

    return LE_SUCCESS;
}

#endif