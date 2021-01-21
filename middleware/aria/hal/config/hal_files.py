# coding: utf-8
##############################################################################
# Copyright (C) 2018 Microchip Technology Inc. and its subsidiaries.
#
# Subject to your compliance with these terms, you may use Microchip software
# and any derivatives exclusively with Microchip products. It is your
# responsibility to comply with third party license terms applicable to your
# use of third party software (including open source software) that may
# accompany Microchip software.
#
# THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
# EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
# WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
# PARTICULAR PURPOSE.
#
# IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
# INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
# WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
# BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
# FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
# ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
# THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
##############################################################################

GFX_H = halComponent.createFileSymbol("GFX_H", None)
GFX_H.setSourcePath("gfx.h")
GFX_H.setDestPath("gfx/hal/")
GFX_H.setProjectPath(projectPath)
GFX_H.setType("HEADER")

GFX_COLOR_H = halComponent.createFileSymbol("GFX_COLOR_H", None)
GFX_COLOR_H.setSourcePath("inc/gfx_color.h")
GFX_COLOR_H.setDestPath("gfx/hal/inc/")
GFX_COLOR_H.setProjectPath(projectPath)
GFX_COLOR_H.setType("HEADER")

GFX_COMMON_H = halComponent.createFileSymbol("GFX_COMMON_H", None)
GFX_COMMON_H.setSourcePath("inc/gfx_common.h")
GFX_COMMON_H.setDestPath("gfx/hal/inc/")
GFX_COMMON_H.setProjectPath(projectPath)
GFX_COMMON_H.setType("HEADER")

GFX_CONTEXT_H = halComponent.createFileSymbol("GFX_CONTEXT_H", None)
GFX_CONTEXT_H.setSourcePath("inc/gfx_context.h")
GFX_CONTEXT_H.setDestPath("gfx/hal/inc/")
GFX_CONTEXT_H.setProjectPath(projectPath)
GFX_CONTEXT_H.setType("HEADER")

GFX_DEFAULT_IMPL_H = halComponent.createFileSymbol("GFX_DEFAULT_IMPL_H", None)
GFX_DEFAULT_IMPL_H.setSourcePath("inc/gfx_default_impl.h")
GFX_DEFAULT_IMPL_H.setDestPath("gfx/hal/inc/")
GFX_DEFAULT_IMPL_H.setProjectPath(projectPath)
GFX_DEFAULT_IMPL_H.setType("HEADER")

GFX_DISPLAY_H = halComponent.createFileSymbol("GFX_DISPLAY_H", None)
GFX_DISPLAY_H.setSourcePath("inc/gfx_display.h")
GFX_DISPLAY_H.setDestPath("gfx/hal/inc/")
GFX_DISPLAY_H.setProjectPath(projectPath)
GFX_DISPLAY_H.setType("HEADER")

GFX_DRIVER_INTERFACE_H = halComponent.createFileSymbol("GFX_DRIVER_INTERFACE_H", None)
GFX_DRIVER_INTERFACE_H.setSourcePath("inc/gfx_driver_interface.h")
GFX_DRIVER_INTERFACE_H.setDestPath("gfx/hal/inc/")
GFX_DRIVER_INTERFACE_H.setProjectPath(projectPath)
GFX_DRIVER_INTERFACE_H.setType("HEADER")

GFX_HAL_H = halComponent.createFileSymbol("GFX_HAL_H", None)
GFX_HAL_H.setSourcePath("inc/gfx_hal.h")
GFX_HAL_H.setDestPath("gfx/hal/inc/")
GFX_HAL_H.setProjectPath(projectPath)
GFX_HAL_H.setType("HEADER")

GFX_INTERFACE_H = halComponent.createFileSymbol("GFX_INTERFACE_H", None)
GFX_INTERFACE_H.setSourcePath("inc/gfx_interface.h")
GFX_INTERFACE_H.setDestPath("gfx/hal/inc/")
GFX_INTERFACE_H.setProjectPath(projectPath)
GFX_INTERFACE_H.setType("HEADER")

GFX_LAYER_H = halComponent.createFileSymbol("GFX_LAYER_H", None)
GFX_LAYER_H.setSourcePath("inc/gfx_layer.h")
GFX_LAYER_H.setDestPath("gfx/hal/inc/")
GFX_LAYER_H.setProjectPath(projectPath)
GFX_LAYER_H.setType("HEADER")

GFX_PIXEL_BUFFER_H = halComponent.createFileSymbol("GFX_PIXEL_BUFFER_H", None)
GFX_PIXEL_BUFFER_H.setSourcePath("inc/gfx_pixel_buffer.h")
GFX_PIXEL_BUFFER_H.setDestPath("gfx/hal/inc/")
GFX_PIXEL_BUFFER_H.setProjectPath(projectPath)
GFX_PIXEL_BUFFER_H.setType("HEADER")

GFX_PROCESSOR_INTERFACE_H = halComponent.createFileSymbol("GFX_PROCESSOR_INTERFACE_H", None)
GFX_PROCESSOR_INTERFACE_H.setSourcePath("inc/gfx_processor_interface.h")
GFX_PROCESSOR_INTERFACE_H.setDestPath("gfx/hal/inc/")
GFX_PROCESSOR_INTERFACE_H.setProjectPath(projectPath)
GFX_PROCESSOR_INTERFACE_H.setType("HEADER")

GFX_RECT_H = halComponent.createFileSymbol("GFX_RECT_H", None)
GFX_RECT_H.setSourcePath("inc/gfx_rect.h")
GFX_RECT_H.setDestPath("gfx/hal/inc/")
GFX_RECT_H.setProjectPath(projectPath)
GFX_RECT_H.setType("HEADER")

GFX_UTIL_H = halComponent.createFileSymbol("GFX_UTIL_H", None)
GFX_UTIL_H.setSourcePath("inc/gfx_util.h")
GFX_UTIL_H.setDestPath("gfx/hal/inc/")
GFX_UTIL_H.setProjectPath(projectPath)
GFX_UTIL_H.setType("HEADER")

GFX_C = halComponent.createFileSymbol("GFX_C", None)
GFX_C.setSourcePath("src/gfx.c")
GFX_C.setDestPath("gfx/hal/src/")
GFX_C.setProjectPath(projectPath)
GFX_C.setType("SOURCE")

GFX_COLOR_C = halComponent.createFileSymbol("GFX_COLOR_C", None)
GFX_COLOR_C.setSourcePath("src/gfx_color.c")
GFX_COLOR_C.setDestPath("gfx/hal/src/")
GFX_COLOR_C.setProjectPath(projectPath)
GFX_COLOR_C.setType("SOURCE")

GFX_CONTEXT_C = halComponent.createFileSymbol("GFX_CONTEXT_C", None)
GFX_CONTEXT_C.setSourcePath("src/gfx_context.c")
GFX_CONTEXT_C.setDestPath("gfx/hal/src/")
GFX_CONTEXT_C.setProjectPath(projectPath)
GFX_CONTEXT_C.setType("SOURCE")

GFX_DEFAULT_IMPL_C = halComponent.createFileSymbol("GFX_DEFAULT_IMPL_C", None)
GFX_DEFAULT_IMPL_C.setSourcePath("src/gfx_default_impl.c")
GFX_DEFAULT_IMPL_C.setDestPath("gfx/hal/src/")
GFX_DEFAULT_IMPL_C.setProjectPath(projectPath)
GFX_DEFAULT_IMPL_C.setType("SOURCE")

GFX_DISPLAY_C = halComponent.createFileSymbol("GFX_DISPLAY_C", None)
GFX_DISPLAY_C.setSourcePath("src/gfx_display.c")
GFX_DISPLAY_C.setDestPath("gfx/hal/src/")
GFX_DISPLAY_C.setProjectPath(projectPath)
GFX_DISPLAY_C.setType("SOURCE")

GFX_DISPLAY_DEF_C = halComponent.createFileSymbol("GFX_DISPLAY_DEF_C", None)
GFX_DISPLAY_DEF_C.setSourcePath("templates/gfx_display_def.c.ftl")
GFX_DISPLAY_DEF_C.setDestPath("gfx/hal/src/")
GFX_DISPLAY_DEF_C.setOutputName("gfx_display_def.c")
GFX_DISPLAY_DEF_C.setProjectPath(projectPath)
GFX_DISPLAY_DEF_C.setType("SOURCE")
GFX_DISPLAY_DEF_C.setMarkup(True)

GFX_DRIVER_INTERFACE_C = halComponent.createFileSymbol("GFX_DRIVER_INTERFACE_C", None)
GFX_DRIVER_INTERFACE_C.setSourcePath("src/gfx_driver_interface.c")
GFX_DRIVER_INTERFACE_C.setDestPath("gfx/hal/src/")
GFX_DRIVER_INTERFACE_C.setProjectPath(projectPath)
GFX_DRIVER_INTERFACE_C.setType("SOURCE")

GFX_DRIVER_DEF_C = halComponent.createFileSymbol("GFX_DRIVER_DEF_C", None)
GFX_DRIVER_DEF_C.setSourcePath("templates/gfx_driver_def.c.ftl")
GFX_DRIVER_DEF_C.setDestPath("gfx/hal/src")
GFX_DRIVER_DEF_C.setOutputName("gfx_driver_def.c")
GFX_DRIVER_DEF_C.setProjectPath(projectPath)
GFX_DRIVER_DEF_C.setType("SOURCE")
GFX_DRIVER_DEF_C.setMarkup(True)

GFX_PROCESSOR_DEF_C = halComponent.createFileSymbol("GFX_PROCESSOR_DEF_C", None)
GFX_PROCESSOR_DEF_C.setSourcePath("templates/gfx_processor_def.c.ftl")
GFX_PROCESSOR_DEF_C.setDestPath("gfx/hal/src")
GFX_PROCESSOR_DEF_C.setOutputName("gfx_processor_def.c")
GFX_PROCESSOR_DEF_C.setProjectPath(projectPath)
GFX_PROCESSOR_DEF_C.setType("SOURCE")
GFX_PROCESSOR_DEF_C.setMarkup(True)

GFX_GET_C = halComponent.createFileSymbol("GFX_GET_C", None)
GFX_GET_C.setSourcePath("src/gfx_get.c")
GFX_GET_C.setDestPath("gfx/hal/src/")
GFX_GET_C.setProjectPath(projectPath)
GFX_GET_C.setType("SOURCE")

GFX_INTERFACE_C = halComponent.createFileSymbol("GFX_INTERFACE_C", None)
GFX_INTERFACE_C.setSourcePath("src/gfx_interface.c")
GFX_INTERFACE_C.setDestPath("gfx/hal/src/")
GFX_INTERFACE_C.setProjectPath(projectPath)
GFX_INTERFACE_C.setType("SOURCE")

GFX_LAYER_C = halComponent.createFileSymbol("GFX_LAYER_C", None)
GFX_LAYER_C.setSourcePath("src/gfx_layer.c")
GFX_LAYER_C.setDestPath("gfx/hal/src/")
GFX_LAYER_C.setProjectPath(projectPath)
GFX_LAYER_C.setType("SOURCE")

GFX_PIXEL_BUFFER_C = halComponent.createFileSymbol("GFX_PIXEL_BUFFER_C", None)
GFX_PIXEL_BUFFER_C.setSourcePath("src/gfx_pixel_buffer.c")
GFX_PIXEL_BUFFER_C.setDestPath("gfx/hal/src/")
GFX_PIXEL_BUFFER_C.setProjectPath(projectPath)
GFX_PIXEL_BUFFER_C.setType("SOURCE")

GFX_PROCESSOR_INTERFACE_C = halComponent.createFileSymbol("GFX_PROCESSOR_INTERFACE_C", None)
GFX_PROCESSOR_INTERFACE_C.setSourcePath("src/gfx_processor_interface.c")
GFX_PROCESSOR_INTERFACE_C.setDestPath("gfx/hal/src/")
GFX_PROCESSOR_INTERFACE_C.setProjectPath(projectPath)
GFX_PROCESSOR_INTERFACE_C.setType("SOURCE")

GFX_RECT_C = halComponent.createFileSymbol("GFX_RECT_C", None)
GFX_RECT_C.setSourcePath("src/gfx_rect.c")
GFX_RECT_C.setDestPath("gfx/hal/src/")
GFX_RECT_C.setProjectPath(projectPath)
GFX_RECT_C.setType("SOURCE")

GFX_UTIL_C = halComponent.createFileSymbol("GFX_UTIL_C", None)
GFX_UTIL_C.setSourcePath("src/gfx_util.c")
GFX_UTIL_C.setDestPath("gfx/hal/src/")
GFX_UTIL_C.setProjectPath(projectPath)
GFX_UTIL_C.setType("SOURCE")

GFX_SET_C = halComponent.createFileSymbol("GFX_SET_C", None)
GFX_SET_C.setSourcePath("src/gfx_set.c")
GFX_SET_C.setDestPath("gfx/hal/src/")
GFX_SET_C.setProjectPath(projectPath)
GFX_SET_C.setType("SOURCE")

GFX_DRAW_H = halComponent.createFileSymbol("GFX_DRAW_H", None)
GFX_DRAW_H.setSourcePath("inc/gfx_draw.h")
GFX_DRAW_H.setDestPath("gfx/hal/inc/")
GFX_DRAW_H.setProjectPath(projectPath)
GFX_DRAW_H.setType("HEADER")

GFX_DRAW_ARC_H = halComponent.createFileSymbol("GFX_DRAW_ARC_H", None)
GFX_DRAW_ARC_H.setSourcePath("inc/gfx_draw_arc.h")
GFX_DRAW_ARC_H.setDestPath("gfx/hal/inc/")
GFX_DRAW_ARC_H.setProjectPath(projectPath)
GFX_DRAW_ARC_H.setType("HEADER")

GFX_DRAW_BLIT_H = halComponent.createFileSymbol("GFX_DRAW_BLIT_H", None)
GFX_DRAW_BLIT_H.setSourcePath("inc/gfx_draw_blit.h")
GFX_DRAW_BLIT_H.setDestPath("gfx/hal/inc/")
GFX_DRAW_BLIT_H.setProjectPath(projectPath)
GFX_DRAW_BLIT_H.setType("HEADER")

GFX_DRAW_CIRCLE_H = halComponent.createFileSymbol("GFX_DRAW_CIRCLE_H", None)
GFX_DRAW_CIRCLE_H.setSourcePath("inc/gfx_draw_circle.h")
GFX_DRAW_CIRCLE_H.setDestPath("gfx/hal/inc/")
GFX_DRAW_CIRCLE_H.setProjectPath(projectPath)
GFX_DRAW_CIRCLE_H.setType("HEADER")

GFX_DRAW_ELLIPSE_H = halComponent.createFileSymbol("GFX_DRAW_ELLIPSE_H", None)
GFX_DRAW_ELLIPSE_H.setSourcePath("inc/gfx_draw_ellipse.h")
GFX_DRAW_ELLIPSE_H.setDestPath("gfx/hal/inc/")
GFX_DRAW_ELLIPSE_H.setProjectPath(projectPath)
GFX_DRAW_ELLIPSE_H.setType("HEADER")

GFX_DRAW_LINE_H = halComponent.createFileSymbol("GFX_DRAW_LINE_H", None)
GFX_DRAW_LINE_H.setSourcePath("inc/gfx_draw_line.h")
GFX_DRAW_LINE_H.setDestPath("gfx/hal/inc/")
GFX_DRAW_LINE_H.setProjectPath(projectPath)
GFX_DRAW_LINE_H.setType("HEADER")

GFX_DRAW_PIXEL_H = halComponent.createFileSymbol("GFX_DRAW_PIXEL_H", None)
GFX_DRAW_PIXEL_H.setSourcePath("inc/gfx_draw_pixel.h")
GFX_DRAW_PIXEL_H.setDestPath("gfx/hal/inc/")
GFX_DRAW_PIXEL_H.setProjectPath(projectPath)
GFX_DRAW_PIXEL_H.setType("HEADER")

GFX_DRAW_RECT_H = halComponent.createFileSymbol("GFX_DRAW_RECT_H", None)
GFX_DRAW_RECT_H.setSourcePath("inc/gfx_draw_rect.h")
GFX_DRAW_RECT_H.setDestPath("gfx/hal/inc/")
GFX_DRAW_RECT_H.setProjectPath(projectPath)
GFX_DRAW_RECT_H.setType("HEADER")

GFX_DRAW_STRETCHBLIT_H = halComponent.createFileSymbol("GFX_DRAW_STRETCHBLIT_H", None)
GFX_DRAW_STRETCHBLIT_H.setSourcePath("inc/gfx_draw_stretchblit.h")
GFX_DRAW_STRETCHBLIT_H.setDestPath("gfx/hal/inc/")
GFX_DRAW_STRETCHBLIT_H.setProjectPath(projectPath)
GFX_DRAW_STRETCHBLIT_H.setType("HEADER")

GFX_MATH_H = halComponent.createFileSymbol("GFX_MATH_H", None)
GFX_MATH_H.setSourcePath("inc/gfx_math.h")
GFX_MATH_H.setDestPath("gfx/hal/inc/")
GFX_MATH_H.setProjectPath(projectPath)
GFX_MATH_H.setType("HEADER")

GFX_COLOR_BLEND_C = halComponent.createFileSymbol("GFX_COLOR_BLEND_C", None)
GFX_COLOR_BLEND_C.setSourcePath("src/gfx_color_blend.c")
GFX_COLOR_BLEND_C.setDestPath("gfx/hal/src/")
GFX_COLOR_BLEND_C.setProjectPath(projectPath)
GFX_COLOR_BLEND_C.setType("SOURCE")

GFX_COLOR_CONVERT_C = halComponent.createFileSymbol("GFX_COLOR_CONVERT_C", None)
GFX_COLOR_CONVERT_C.setSourcePath("src/gfx_color_convert.c")
GFX_COLOR_CONVERT_C.setDestPath("gfx/hal/src/")
GFX_COLOR_CONVERT_C.setProjectPath(projectPath)
GFX_COLOR_CONVERT_C.setType("SOURCE")

GFX_COLOR_LERP_C = halComponent.createFileSymbol("GFX_COLOR_LERP_C", None)
GFX_COLOR_LERP_C.setSourcePath("src/gfx_color_lerp.c")
GFX_COLOR_LERP_C.setDestPath("gfx/hal/src/")
GFX_COLOR_LERP_C.setProjectPath(projectPath)
GFX_COLOR_LERP_C.setType("SOURCE")

GFX_COLOR_VALUE_C = halComponent.createFileSymbol("GFX_COLOR_VALUE_C", None)
GFX_COLOR_VALUE_C.setSourcePath("src/gfx_color_value.c")
GFX_COLOR_VALUE_C.setDestPath("gfx/hal/src/")
GFX_COLOR_VALUE_C.setProjectPath(projectPath)
GFX_COLOR_VALUE_C.setType("SOURCE")

GFX_DRAW_ARC_C = halComponent.createFileSymbol("GFX_DRAW_ARC_C", None)
GFX_DRAW_ARC_C.setSourcePath("src/gfx_draw_arc.c")
GFX_DRAW_ARC_C.setDestPath("gfx/hal/src/")
GFX_DRAW_ARC_C.setProjectPath(projectPath)
GFX_DRAW_ARC_C.setType("SOURCE")

GFX_DRAW_BLIT_C = halComponent.createFileSymbol("GFX_DRAW_BLIT_C", None)
GFX_DRAW_BLIT_C.setSourcePath("src/gfx_draw_blit.c")
GFX_DRAW_BLIT_C.setDestPath("gfx/hal/src/")
GFX_DRAW_BLIT_C.setProjectPath(projectPath)
GFX_DRAW_BLIT_C.setType("SOURCE")

GFX_DRAW_CIRCLE_C = halComponent.createFileSymbol("GFX_DRAW_CIRCLE_C", None)
GFX_DRAW_CIRCLE_C.setSourcePath("src/gfx_draw_circle.c")
GFX_DRAW_CIRCLE_C.setDestPath("gfx/hal/src/")
GFX_DRAW_CIRCLE_C.setProjectPath(projectPath)
GFX_DRAW_CIRCLE_C.setType("SOURCE")

GFX_DRAW_ELLIPSE_C = halComponent.createFileSymbol("GFX_DRAW_ELLIPSE_C", None)
GFX_DRAW_ELLIPSE_C.setSourcePath("src/gfx_draw_ellipse.c")
GFX_DRAW_ELLIPSE_C.setDestPath("gfx/hal/src/")
GFX_DRAW_ELLIPSE_C.setProjectPath(projectPath)
GFX_DRAW_ELLIPSE_C.setType("SOURCE")

GFX_DRAW_LINE_C = halComponent.createFileSymbol("GFX_DRAW_LINE_C", None)
GFX_DRAW_LINE_C.setSourcePath("src/gfx_draw_line.c")
GFX_DRAW_LINE_C.setDestPath("gfx/hal/src/")
GFX_DRAW_LINE_C.setProjectPath(projectPath)
GFX_DRAW_LINE_C.setType("SOURCE")

GFX_DRAW_PIXEL_C = halComponent.createFileSymbol("GFX_DRAW_PIXEL_C", None)
GFX_DRAW_PIXEL_C.setSourcePath("src/gfx_draw_pixel.c")
GFX_DRAW_PIXEL_C.setDestPath("gfx/hal/src/")
GFX_DRAW_PIXEL_C.setProjectPath(projectPath)
GFX_DRAW_PIXEL_C.setType("SOURCE")

GFX_DRAW_RECT_C = halComponent.createFileSymbol("GFX_DRAW_RECT_C", None)
GFX_DRAW_RECT_C.setSourcePath("src/gfx_draw_rect.c")
GFX_DRAW_RECT_C.setDestPath("gfx/hal/src/")
GFX_DRAW_RECT_C.setProjectPath(projectPath)
GFX_DRAW_RECT_C.setType("SOURCE")

GFX_DRAW_STRETCHBLIT_C = halComponent.createFileSymbol("GFX_DRAW_STRETCHBLIT_C", None)
GFX_DRAW_STRETCHBLIT_C.setSourcePath("src/gfx_draw_stretchblit.c")
GFX_DRAW_STRETCHBLIT_C.setDestPath("gfx/hal/src/")
GFX_DRAW_STRETCHBLIT_C.setProjectPath(projectPath)
GFX_DRAW_STRETCHBLIT_C.setType("SOURCE")

GFX_MATH_C = halComponent.createFileSymbol("GFX_MATH_C", None)
GFX_MATH_C.setSourcePath("src/gfx_math.c")
GFX_MATH_C.setDestPath("gfx/hal/src/")
GFX_MATH_C.setProjectPath(projectPath)
GFX_MATH_C.setType("SOURCE")

GFX_CONFIG_H = halComponent.createFileSymbol("GFX_CONFIG_H", None)
GFX_CONFIG_H.setSourcePath("templates/gfx_config.h.ftl")
GFX_CONFIG_H.setDestPath("gfx/hal/")
GFX_CONFIG_H.setOutputName("gfx_config.h")
GFX_CONFIG_H.setProjectPath(projectPath)
GFX_CONFIG_H.setType("HEADER")
GFX_CONFIG_H.setMarkup(True)