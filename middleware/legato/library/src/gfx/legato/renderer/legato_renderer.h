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
/** \file legato_renderer.h
  * @brief .
  *
  * @details .
  */

#ifndef LEGATO_RENDERER_H
#define LEGATO_RENDERER_H

#include "gfx/legato/common/legato_common.h"
#include "gfx/legato/common/legato_pixelbuffer.h"
#include "gfx/legato/common/legato_rect.h"
#include "gfx/legato/datastructure/legato_rectarray.h"
#include "gfx/legato/image/legato_palette.h"
#include "gfx/driver/gfx_driver.h"
#include "gfx/legato/widget/legato_widget.h"


// *****************************************************************************
/**
 * @brief This enum represents state of the renderer.
 * @details This enum type describes the state at which a
 * widget is being drawn.
 */
typedef enum leFrameState
{
    LE_FRAME_READY = 0,
    LE_FRAME_PREFRAME,
    LE_FRAME_PRELAYER,
    LE_FRAME_PRERECT,
    LE_FRAME_PREWIDGET,
    LE_FRAME_DRAWING,
    LE_FRAME_POSTWIDGET,
    LE_FRAME_POSTRECT,
    LE_FRAME_WAITFORBUFFER,
    LE_FRAME_POSTLAYER,
    LE_FRAME_POSTFRAME,
} leFrameState;

typedef struct leRenderLayerState
{
    leRectArray prevDamageRects;    // previous damaged rectangle list
    leRectArray currentDamageRects; // queued damaged rectangle list
    leRectArray pendingDamageRects; // pending damaged rectangle list
                                    // these are rectangles added during
                                    // a frame in progress

    leRectArray scratchRectList; // used for rectangle culling phase
    leRectArray frameRectList;   // rects to draw for a frame

    leBool drawingPrev;           // indicates if the layer is currently
                                  // drawing from its previous rectangle
                                  // array
} leRenderLayerState;

/**
 * @brief This structs represents global state of the renderer.
 * @details This struct type describes the state at which a
 * widget is being drawn.
 */
typedef struct leRenderState
{
    const gfxDisplayDriver* dispDriver;  // the display driver pointer
    const gfxGraphicsProcessor* gpuDriver; // the gpu driver pointer
    
    uint32_t layerIdx;           // the current layer index

    //leRect displayRect;          // the driver physical display rectangle
    uint32_t bufferCount;        // the number of scratch buffers the library supports

    uint32_t frameRectIdx;       // the current frame draw rectangle index
    leWidget* currentWidget;     // the widget that is currently drawing
    
    leRect drawRect;              // the current damage rectangle clipped
                                  // to the currently rendering widget

    leRenderLayerState layerStates[LE_LAYER_COUNT];

    leFrameState frameState;      // the current frame render state

    uint32_t drawCount;           // the number of times the screen has drawn

    uint32_t frameDrawCount;      // the number of widgets that have rendered
                                  // on the screen

    uint32_t deltaTime;           // stores delta time for updates that happen
                                  // during rendering

#if LE_ALPHA_BLENDING_ENABLED == 1
    leBool alphaEnable;           // the global alpha enabled flag
    uint8_t alpha;                // the current global alpha value
#endif
                                  
    lePalette* globalPalette;     // the pointer to the global palette

    int32_t currentScratchBuffer; // the index of the current scratch buffer

    gfxIOCTLArg_Value val;
} leRenderState;

typedef struct leGradient
{
    leColor c0;
    leColor c1;
    leColor c2;
    leColor c3;
} leGradient;

/* internal use only */
/**
  * @cond INTERNAL
  *
  */
leResult leRenderer_Initialize(const gfxDisplayDriver* dispDriver,
                               const gfxGraphicsProcessor* gpuDriver);

// internal use only
void leRenderer_Shutdown(void);
/**
  * @endcond
  *
  */


// *****************************************************************************
/* Function:
    leResult leRenderer_DamageArea(const leRect* rect)

  Summary:
    Damages a section of a display layer.  The renderer will redraw it as part
    of the next draw cycle.

  Description:
    Damages a section of a display layer.  The renderer will redraw it as part
    of the next draw cycle.

  Parameters:
    const leRect* rect - the rectangle area to damage
    uint32_t layerIdx - the layer to damage

  Returns:
    leResult
*/
leResult leRenderer_DamageArea(const leRect* rect, uint32_t layerIdx);

// internal use only
void leRenderer_Paint(void);

// *****************************************************************************
/* Function:
    leRenderState* leGetRenderState();

  Summary:
    Gets a pointer to the current render state.  Use with caution.

  Description:
    Gets a pointer to the current render state.  Use with caution.

  Parameters:

  Returns:
    leRenderState* - the current render state
*/
leRenderState* leGetRenderState(void);

// *****************************************************************************
/* Function:
    lePixelBuffer* leGetRenderBuffer();

  Summary:
    Gets a pointer to the current render buffer.  Use with caution.

  Description:
    Gets a pointer to the current render buffer.  Use with caution.

  Parameters:

  Returns:
    lePixelBuffer* - the current render buffer
*/
lePixelBuffer* leGetRenderBuffer(void);

// *****************************************************************************
/* Function:
    leColorMode leRenderer_CurrentColorMode();

  Summary:
    Gets the color mode of the current rendering layer.

  Description:
    Gets the color mode of the current rendering layer.

  Parameters:

  Returns:
    leColorMode - the current render color mode
*/
leColorMode leRenderer_CurrentColorMode(void);

// *****************************************************************************
/* Function:
    lePalette* leRenderer_GetGlobalPalette()

  Summary:
    Gets a pointer to the active global palette.

  Description:
    Gets a pointer to the active global palette.

  Parameters:

  Returns:
    lePalette* - the current global palette
*/
lePalette* leRenderer_GetGlobalPalette(void);

// *****************************************************************************
/* Function:
    leResult leRenderer_SetGlobalPalette(lePalette* pal)

  Summary:
    Sets the global palette to the given pointer.

  Description:
    Sets the global palette to the given pointer.

  Parameters:
    lePalette* pal - the palette to set

  Returns:
    leResult
*/
leResult leRenderer_SetGlobalPalette(lePalette* pal);

// *****************************************************************************
/* Function:
    leColor leRenderer_GlobalPaletteLookup(uint32_t idx)

  Summary:
    Queries the global palette for a lookup value.

  Description:
    Queries the global palette for a lookup value.

  Parameters:
    uint32_t idx - index to query

  Returns:
    leColor - the result color
*/
leColor leRenderer_GlobalPaletteLookup(uint32_t idx);

// *****************************************************************************
/* Function:
    leColor leRenderer_ConvertColor(leColor inColor, leColorMode inMode)

  Summary:
    Converts a color from the input mode to the current layer render color.

  Description:
    Converts a color from the input mode to the current layer render color.

  Parameters:
    leColor inColor - the input color
    leColorMode inMode - the color mode of the input

  Returns:
    leColor - the result color
*/
leColor leRenderer_ConvertColor(leColor inColor, leColorMode inMode);

// *****************************************************************************
/* Function:
    void leRenderer_GetDrawRect(leRect* rect)

  Summary:
    Gets the current draw rectangle.

  Description:
    Gets the current draw rectangle.

  Parameters:

  Returns:
    void
*/
void leRenderer_GetDrawRect(leRect* rect);

// *****************************************************************************
/* Function:
    leBool leRenderer_CullDrawRect(const leRect* rect)

  Summary:
    Attempts to cull a rectangle against the current draw rectangle.

  Description:
    Attempts to cull a rectangle against the current draw rectangle.

  Parameters:
    const leRect* rect - rectangle to cull (screen space)

  Returns:
    leBool - LE_TRUE if the rectangle is outside of the draw rectangle
*/
leBool leRenderer_CullDrawRect(const leRect* rect);

// *****************************************************************************
/* Function:
    leRenderer_CullDrawXY(int32_t x, int32_t y)

  Summary:
    Attempts to cull a point against the current draw rectangle.

  Description:
    Attempts to cull a point against the current draw rectangle.

  Parameters:
    int32_t x - the x component of the point to cull (screen space)
    int32_t y - the y component of the point to cull (screen space)

  Returns:
    leBool - LE_TRUE if the point is outside of the draw rectangle
*/
leBool leRenderer_CullDrawXY(int32_t x, int32_t y);

// *****************************************************************************
/* Function:
    leBool leRenderer_CullDrawPoint(const lePoint* pt)

  Summary:
    Attempts to cull a point against the current draw rectangle.

  Description:
    Attempts to cull a point against the current draw rectangle.

  Parameters:
    const lePoint* pt - the point to cull (screen space)

  Returns:
    leBool - LE_TRUE if the point is outside of the draw rectangle
*/
leBool leRenderer_CullDrawPoint(const lePoint* pt);

// *****************************************************************************
/* Function:
    void leRenderer_ClipDrawRect(const leRect* rect, leRect* res)

  Summary:
    Clips a rectangle against the current draw rectangle.

  Description:
    Clips a rectangle against the current draw rectangle.

  Parameters:
    const leRect* rect - rectangle to clip (screen space)
    leRect* res - the clip result (screen space)

  Returns:
    void
*/
void leRenderer_ClipDrawRect(const leRect* rect, leRect* res);

// *****************************************************************************
/* Function:
    leColor leRenderer_GetPixel(int32_t x, int32_t y)

  Summary:
    Reads a pixel from the current render buffer.  Does not do bounds checking.

  Description:
    Reads a pixel from the current render buffer.  Does not do bounds checking.

  Parameters:
    int32_t x - the x component of the point to cull (screen space)
    int32_t y - the y component of the point to cull (screen space)

  Returns:
    leColor - the color that was read
*/
leColor leRenderer_GetPixel(int32_t x,
                            int32_t y);

// *****************************************************************************
/* Function:
    leResult leRenderer_GetPixel_Safe(int32_t x,
                                      int32_t y,
                                      leColor* clr)

  Summary:
    Safely reads a pixel from the current render buffer.

  Description:
    Safely reads a pixel from the current render buffer.

  Parameters:
    int32_t x - the x component of the point to read (screen space)
    int32_t y - the y component of the point to read (screen space)
    leColor* clr - the color that was read

  Returns:
    leResult - LE_TRUE if the point could be read
*/
leResult leRenderer_GetPixel_Safe(int32_t x,
                                  int32_t y,
                                  leColor* clr);

// *****************************************************************************
/* Function:
    leResult leRenderer_PutPixel(int32_t x,
                                 int32_t y,
                                 leColor clr)

  Summary:
    Writes a pixel to the current render buffer.  Does not do bounds checking.

  Description:
    Writes a pixel to the current render buffer.  Does not do bounds checking.

  Parameters:
    int32_t x - the x component of the point to write (screen space)
    int32_t y - the y component of the point to write (screen space)
    leColor clr - the color to write

  Returns:
    leResult
*/
leResult leRenderer_PutPixel(int32_t x,
                             int32_t y,
                             leColor clr);

// *****************************************************************************
/* Function:
    leResult leRenderer_BlendPixel(int32_t x,
                                   int32_t y,
                                   leColor clr,
                                   uint32_t a)

  Summary:
    Writes a pixel to the current render buffer.  Does not do bounds checking.

  Description:
    Writes a pixel to the current render buffer.  Does not do bounds checking.

  Parameters:
    int32_t x - the x component of the point to write (screen space)
    int32_t y - the y component of the point to write (screen space)
    leColor clr - the color to write
    uint32_t a - the global alpha blending value to use

  Returns:
    leResult
*/
leResult leRenderer_BlendPixel(int32_t x,
                               int32_t y,
                               leColor clr,
                               uint32_t a);

// *****************************************************************************
/* Function:
    leResult leRenderer_PutPixel_Safe(int32_t x,
                                      int32_t y,
                                      leColor clr)

  Summary:
    Safely writes a pixel to the current render buffer.

  Description:
    Safely writes a pixel to the current render buffer.

  Parameters:
    int32_t x - the x component of the point to write (screen space)
    int32_t y - the y component of the point to write (screen space)
    leColor clr - the color to write

  Returns:
    leResult
*/
leResult leRenderer_PutPixel_Safe(int32_t x,
                                  int32_t y,
                                  leColor clr);

// *****************************************************************************
/* Function:
    leResult leRenderer_BlendPixel_Safe(int32_t x,
                                        int32_t y,
                                        leColor clr,
                                        uint32_t a)

  Summary:
    Safely writes a pixel to the current render buffer.

  Description:
    Safely writes a pixel to the current render buffer.

  Parameters:
    int32_t x - the x component of the point to write (screen space)
    int32_t y - the y component of the point to write (screen space)
    leColor clr - the color to write
    uint32_t a - the global alpha blending value to use

  Returns:
    leResult
*/
leResult leRenderer_BlendPixel_Safe(int32_t x,
                                    int32_t y,
                                    leColor clr,
                                    uint32_t a);

// *****************************************************************************
/* Function:
    leResult leRenderer_FillArea(int32_t x,
                                 int32_t y,
                                 uint32_t width,
                                 uint32_t height,
                                 leColor clr,
                                 uint32_t a)

  Summary:
    Fills an area of the buffer with a single color.  Does not do bounds
    checking.

  Description:
    Fills an area of the buffer with a single color.  Does not do bounds
    checking.

  Parameters:
    int32_t x - the x component of the area to fill (screen space)
    int32_t y - the y component of the area to fill (screen space)
    uint32_t width - the width of the area
    uint32_t height - the height of the area
    leColor clr - the color to write
    uint32_t a - the global alpha blending value to use

  Returns:
    leResult
*/
leResult leRenderer_FillArea(int32_t x,
                             int32_t y,
                             uint32_t width,
                             uint32_t height,
                             leColor clr,
                             uint32_t a);

// *****************************************************************************
/* Function:
    leResult leRenderer_FillArea_Safe(int32_t x,
                                      int32_t y,
                                      uint32_t width,
                                      uint32_t height,
                                      leColor clr,
                                      uint32_t a)

  Summary:
    Fills an area of the buffer with a single color.

  Description:
    Fills an area of the buffer with a single color.

  Parameters:
    int32_t x - the x component of the area to fill (screen space)
    int32_t y - the y component of the area to fill (screen space)
    uint32_t width - the width of the area
    uint32_t height - the height of the area
    leColor clr - the color to write
    uint32_t a - the global alpha blending value to use

  Returns:
    leResult
*/
leResult leRenderer_FillArea_Safe(int32_t x,
                                  int32_t y,
                                  uint32_t width,
                                  uint32_t height,
                                  leColor clr,
                                  uint32_t a);

// *****************************************************************************
/* Function:
    leResult leRenderer_HorzLine(int32_t x,
                                 int32_t y,
                                 int32_t w,
                                 leColor clr,
                                 uint32_t a)

  Summary:
    Draws a horizontal line.

  Description:
    Draws a horizontal line.

  Parameters:
    int32_t x - the x component of the line (screen space)
    int32_t y - the y component of the line (screen space)
    uint32_t w - the width of the line
    leColor clr - the color to write
    uint32_t a - the global alpha blending value to use

  Returns:
    leResult
*/
leResult leRenderer_HorzLine(int32_t x,
                             int32_t y,
                             int32_t w,
                             leColor clr,
                             uint32_t a);

// *****************************************************************************
/* Function:
    leResult leRenderer_VertLine(int32_t x,
                                 int32_t y,
                                 int32_t h,
                                 leColor clr,
                                 uint32_t a)

  Summary:
    Draws a vertical line.

  Description:
    Draws a vertical line.

  Parameters:
    int32_t x - the x component of the line (screen space)
    int32_t y - the y component of the line (screen space)
    uint32_t h - the height of the line
    leColor clr - the color to write
    uint32_t a - the global alpha blending value to use

  Returns:
    leResult
*/
leResult leRenderer_VertLine(int32_t x,
                             int32_t y,
                             int32_t h,
                             leColor clr,
                             uint32_t a);

// *****************************************************************************
/* Function:
    leResult leRenderer_DrawLine(int32_t x0,
                                 int32_t y0,
                                 int32_t x1,
                                 int32_t y1,
                                 leColor clr,
                                 uint32_t a)

  Summary:
    Draws a line.

  Description:
    Draws a line.

  Parameters:
    int32_t x0 - the start x component of the line (screen space)
    int32_t y0 - the start y component of the line (screen space)
    int32_t x1 - the end x component of the line (screen space)
    int32_t y1 - the end y component of the line (screen space)
    leColor clr - the color to write
    uint32_t a - the global alpha blending value to use

  Returns:
    leResult
*/
leResult leRenderer_DrawLine(int32_t x0,
                             int32_t y0,
                             int32_t x1,
                             int32_t y1,
                             leColor clr,
                             uint32_t a);

// *****************************************************************************
/* Function:
    leResult leRenderer_RectLine(const leRect* rect,
                                 leColor clr,
                                 uint32_t a)

  Summary:
    Draws an outline rectangle.

  Description:
    Draws an outline rectangle.

  Parameters:
    const leRect* rect - the rectangle to draw (screen space)
    leColor clr - the color to write
    uint32_t a - the global alpha blending value to use

  Returns:
    leResult
*/
leResult leRenderer_RectLine(const leRect* rect,
                             leColor clr,
                             uint32_t a);

// *****************************************************************************
/* Function:
    leResult leRenderer_RectFill(const leRect* rect,
                                 leColor clr,
                                 uint32_t a)

  Summary:
    Draws a filled rectangle.

  Description:
    Draws a filled rectangle.

  Parameters:
    const leRect* rect - the rectangle to draw (screen space)
    leColor clr - the color to write
    uint32_t a - the global alpha blending value to use

  Returns:
    leResult
*/
leResult leRenderer_RectFill(const leRect* rect,
                             leColor clr,
                             uint32_t a);

// *****************************************************************************
/* Function:
    leResult leRenderer_HorzGradientRect(const leRect* rect,
                                     leColor clr1,
                                     leColor clr2,
                                     uint32_t a)

  Summary:
    Draws a horizontal gradient filled rectangle.

  Description:
    Draws a horizontal gradient filled rectangle.

  Parameters:
    const leRect* rect - the rectangle to draw (screen space)
    leColor clr1 - the start color
    leColor clr2 - the end color
    uint32_t a - the global alpha blending value to use

  Returns:
    leResult
*/
leResult leRenderer_HorzGradientRect(const leRect* rect,
                                     leColor clr1,
                                     leColor clr2,
                                     uint32_t a);

// *****************************************************************************
/* Function:
    leResult leRenderer_VertGradientRect(const leRect* rect,
                                         leColor clr1,
                                         leColor clr2,
                                         uint32_t a)

  Summary:
    Draws a vertical gradient filled rectangle.

  Description:
    Draws a vertical gradient filled rectangle.

  Parameters:
    const leRect* rect - the rectangle to draw (screen space)
    leColor clr1 - the start color
    leColor clr2 - the end color
    uint32_t a - the global alpha blending value to use

  Returns:
    leResult
*/
leResult leRenderer_VertGradientRect(const leRect* rect,
                                     leColor clr1,
                                     leColor clr2,
                                     uint32_t a);

// *****************************************************************************
/* Function:
    leResult leRenderer_CircleDraw(const leRect* rect,
                                   int32_t x,
                                   int32_t y,
                                   uint32_t radius,
                                   uint32_t thickness,
                                   leColor clr,
                                   uint32_t alpha)

  Summary:
    Draws a circle outline.

  Description:
    Draws a circle outline.

  Parameters:
    const leRect* rect - the bounds of the circle area (screen space)
    int32_t x - the x component of the origin (bound space)
    int32_t y - the y component of the origin (bound space)
    uint32_t radius - the radius of the circle
    uint32_t thickness - the thickness of the circle
    leColor clr - the color to write
    uint32_t a - the global alpha blending value to use

  Returns:
    leResult
*/
leResult leRenderer_CircleDraw(const leRect* rect,
                               uint32_t thickness,
                               leColor clr,
                               uint32_t alpha);

// *****************************************************************************
/* Function:
    leResult leRenderer_CircleDraw(const leRect* rect,
                                   int32_t x,
                                   int32_t y,
                                   uint32_t radius,
                                   uint32_t thickness,
                                   leColor clr,
                                   uint32_t alpha)

  Summary:
    Draws a filled circle.

  Description:
    Draws a filled circle.

  Parameters:
    const leRect* rect - the bounds of the circle area (screen space)
    int32_t x - the x component of the origin (bound space)
    int32_t y - the y component of the origin (bound space)
    uint32_t radius - the radius of the circle
    uint32_t thickness - the thickness of the circle
    leColor borderClr - the outline color to write
    leColor fillClr - the fill color to write
    uint32_t a - the global alpha blending value to use

  Returns:
    leResult
*/
leResult leRenderer_CircleFill(const leRect* rect,
                               uint32_t thickness,
                               leColor borderClr,
                               leColor fillClr,
                               uint32_t alpha);

// *****************************************************************************
/* Function:
    leResult leRenderer_ArcLine(int32_t x,
                                int32_t y,
                                int32_t r,
                                int32_t startAngle,
                                int32_t centerAngle,
                                leColor clr,
                                uint32_t a)

  Summary:
    Draws an arc line.

  Description:
    Draws an arc line.

  Parameters:
    int32_t x - the x component of the origin (screen space)
    int32_t y - the y component of the origin (screen space)
    uint32_t radius - the radius of the arc
    uint32_t startAngle - the arc starting angle
    uint32_t centerAngle - the arc center angle
    leColor clr - the color to write
    uint32_t a - the global alpha blending value to use

  Returns:
    leResult
*/
leResult leRenderer_ArcLine(int32_t x,
                            int32_t y,
                            int32_t radius,
                            int32_t startAngle,
                            int32_t centerAngle,
                            leColor clr,
                            uint32_t a);

// *****************************************************************************
/* Function:
    leResult leRenderer_ArcFill(const leRect* drawRect,
                                int32_t startAngle,
                                int32_t centerAngle,
                                uint32_t thickness,
                                leBool rounded,
                                leColor clr,
                                leBool antialias,
                                uint32_t a)

  Summary:
    Draws a filled arc.

  Description:
    Draws a filled arc.

  Parameters:
    const leRect* drawRect - the bounds of the arc (screen space)
    int32_t x - the x component of the origin (bound space)
    int32_t y - the y component of the origin (bound space)
    uint32_t radius - the radius of the arc
    uint32_t startAngle - the arc starting angle
    uint32_t spanAngle - the arc span/center angle
    leBool rounded - indicates that the arc should draw rounded ends
    leColor clr - the color to write
    leBool antialias - indicates that an antialiased arc should be drawn (not supported yet)
    uint32_t a - the global alpha blending value to use

  Returns:
    leResult
*/
leResult leRenderer_ArcFill(const leRect* drawRect,
                            int32_t startAngle,
                            int32_t spanAngle,
                            uint32_t thickness,
                            leBool rounded,
                            leColor clr,
                            leBool antialias,
                            uint32_t a);

// *****************************************************************************
/* Function:
    leResult leRenderer_EllipseLine(int32_t x,
                                    int32_t y,
                                    int32_t a,
                                    int32_t b,
                                    int32_t theta,
                                    int32_t startAngle,
                                    int32_t endAngle,
                                    leColor clr,
                                    uint32_t alpha)

  Summary:
    Draws an ellipse.

  Description:
    Draws an ellipse.

  Parameters:
    int32_t x - the x component of the ellipse origin (screen space)
    int32_t y - the y component of the ellipse origin (screen space)
    int32_t a - the major axis component of the ellipse
    int32_t b - the minor axis component of the ellipse
    int32_t theta - the rotation component of the ellipse
    int32_t startAngle - the starting angle
    int32_t centerAngle - the center angle
    leColor clr - the color to write
    uint32_t alpha - the global alpha blending value to use

  Returns:
    leResult
*/
leResult leRenderer_EllipseLine(int32_t x,
                                int32_t y,
                                int32_t a,
                                int32_t b,
                                int32_t theta,
                                int32_t startAngle,
                                int32_t endAngle,
                                leColor clr,
                                uint32_t alpha);                                                                         
                             
/**
  * @endcond
  *
  */

#endif // LEGATO_RENDERER_H
