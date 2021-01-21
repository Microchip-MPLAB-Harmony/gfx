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

/** \file legato_string_renderer.h
 * @brief String Renderer.
 *
 * @details String Renderer functions and definitions
 *
 */

#ifndef LE_STRING_RENDERER_H
#define LE_STRING_RENDERER_H

#include "gfx/legato/string/legato_string.h"

// *****************************************************************************
/* Structure:
    struct leStringRenderRequest

  Summary:
    Defines the arguments necessary to perform an leString draw operation

   Description:
    const leString* str - the string to draw
    int32_t x - the x position in screen space
    int32_t y - the y position in screen space
    leHAlignment align - the alignment of the string
    leColor color - the color to write
    uint32_t alpha - global alpha value to apply

  Remarks:
    None.
*/
/**
 * @brief This struct represents a string render request.
 * @details Defines the arguments necessary to perform an leString draw operation
 */
typedef struct leStringRenderRequest
{
    const leString* str;
    int32_t x;
    int32_t y;
    leHAlignment align;
    leColor color;
    uint32_t alpha;
} leStringRenderRequest;

// *****************************************************************************
/* Structure:
    struct leUStringRenderRequest

  Summary:
    Defines the arguments necessary to perform an leChar string draw operation

   Description:
    const leChar* str - the string buffer to draw
    uint32_t length - the length of the string in leChar
    const leFont* font - the font to reference for rendering
    int32_t x - the x position in screen space
    int32_t y - the y position in screen space
    leHAlignment align - the alignment of the string
    leColor color - the color to write
    uint32_t alpha - global alpha value to apply

  Remarks:
    None.
*/
/**
 * @brief This struct represents a lechar string render request.
 * @details Defines the arguments necessary to perform an leChar string draw operation
 */
typedef struct leUStringRenderRequest
{
    const leChar* str;
    uint32_t length;
    const leFont* font;
    int32_t x;
    int32_t y;
    leHAlignment align;
    leColor color;
    uint32_t alpha;
} leUStringRenderRequest;

// *****************************************************************************
/* Structure:
    struct leCStringRenderRequest

  Summary:
    Defines the arguments necessary to perform a C-style string draw operation

   Description:
    const char* str - the c string buffer to draw
    const leFont* font - the font to reference for rendering
    int32_t x - the x position in screen space
    int32_t y - the y position in screen space
    leHAlignment align - the alignment of the string
    leColor color - the color to write
    uint32_t alpha - global alpha value to apply

  Remarks:
    None.
*/
/**
 * @brief This struct represents a c-style string render request.
 * @details Defines the arguments necessary to perform a C-style string draw operation.
 */
typedef struct leCStringRenderRequest
{
    const char* str;
    const leFont* font;
    int32_t x;
    int32_t y;
    leHAlignment align;
    leColor color;
    uint32_t alpha;
} leCStringRenderRequest;

// *****************************************************************************
/**
 * @brief Draw an leString.
 * @details Draws an leString using <span class="param">req</span>.
 * @code
 * leStringRenderRequest* req;
 * leResult res = leStringRenderer_DrawString(req);
 * @endcode
 * @param req the render request details.
 * @return LE_SUCCESS if set, otherwise LE_FAILURE.
 */
leResult leStringRenderer_DrawString(leStringRenderRequest* req);

// *****************************************************************************
/**
 * @brief Draw leChar string.
 * @details Draws a leChar string using <span class="param">req</span>.
 * @code
 * leStringRenderRequest* req;
 * leResult res = leStringRenderer_DrawCString(req);
 * @endcode
 * @param req the result of the operation.
 * @return LE_SUCCESS if set, otherwise LE_FAILURE.
 */
leResult leStringRenderer_DrawCString(leCStringRenderRequest* req);

// *****************************************************************************
/**
 * @brief Draw C-Style string.
 * @details Draws a C-Style string using <span class="param">req</span>.
 * @code
 * leStringRenderRequest* req;
 * leResult res = leStringRenderer_DrawCString(req);
 * @endcode
 * @param req is an integer.
 * @return the result of the operation.
 */
leResult leStringRenderer_DrawUString(leUStringRenderRequest* req);

#endif /* LE_STRING_RENDERER_H */
