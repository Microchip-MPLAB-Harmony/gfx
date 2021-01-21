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

utilsPath = "config/" + Variables.get("__CONFIGURATION_NAME") + "/gfx/utils"
utilsIncPath = "config/" + Variables.get("__CONFIGURATION_NAME") + "/gfx/utils/inc"
utilsSrcPath = "config/" + Variables.get("__CONFIGURATION_NAME") + "/gfx/utils/src"

decoderMenu = component.createMenuSymbol("decoderMenu", None)
decoderMenu.setLabel("Image Decoder Configuration")
decoderMenu.setDescription("Configure image decoder systems.")

JPEGEnable = component.createBooleanSymbol("JPEGEnable", decoderMenu)
JPEGEnable.setLabel("JPEG")
JPEGEnable.setDescription("Enables software JPEG image decoder.")
JPEGEnable.setDefaultValue(True)
JPEGEnable.setDependencies(onJPEGEnableChanged, ["JPEGEnable"])

PNGEnable = component.createBooleanSymbol("PNGEnable", decoderMenu)
PNGEnable.setLabel("PNG")
PNGEnable.setDescription("Enables software PNG image decoder.")
PNGEnable.setDefaultValue(True)
PNGEnable.setDependencies(onPNGEnableChanged, ["PNGEnable"])

GFX_UTILS_H = component.createFileSymbol("GFX_UTILS_H", None)
GFX_UTILS_H.setSourcePath("utils/gfx_utils.h")
GFX_UTILS_H.setDestPath("gfx/utils/")
GFX_UTILS_H.setProjectPath(utilsPath)
GFX_UTILS_H.setType("HEADER")

GFXU_CONFIG_H = component.createFileSymbol("GFXU_CONFIG_H", None)
GFXU_CONFIG_H.setSourcePath("utils/templates/gfxu_config.h.ftl")
GFXU_CONFIG_H.setDestPath("gfx/utils/")
GFXU_CONFIG_H.setOutputName("gfxu_config.h")
GFXU_CONFIG_H.setProjectPath(utilsPath)
GFXU_CONFIG_H.setType("HEADER")
GFXU_CONFIG_H.setMarkup(True)

GFXU_BINARY_H = component.createFileSymbol("GFXU_BINARY_H", None)
GFXU_BINARY_H.setSourcePath("utils/inc/gfxu_binary.h")
GFXU_BINARY_H.setDestPath("gfx/utils/inc/")
GFXU_BINARY_H.setProjectPath(utilsIncPath)
GFXU_BINARY_H.setType("HEADER")

GFXU_FONT_H = component.createFileSymbol("GFXU_FONT_H", None)
GFXU_FONT_H.setSourcePath("utils/inc/gfxu_font.h")
GFXU_FONT_H.setDestPath("gfx/utils/inc/")
GFXU_FONT_H.setProjectPath(utilsIncPath)
GFXU_FONT_H.setType("HEADER")

GFXU_GLOBAL_H = component.createFileSymbol("GFXU_GLOBAL_H", None)
GFXU_GLOBAL_H.setSourcePath("utils/inc/gfxu_global.h")
GFXU_GLOBAL_H.setDestPath("gfx/utils/inc/")
GFXU_GLOBAL_H.setProjectPath(utilsIncPath)
GFXU_GLOBAL_H.setType("HEADER")

GFXU_IMAGE_H = component.createFileSymbol("GFXU_IMAGE_H", None)
GFXU_IMAGE_H.setSourcePath("utils/inc/gfxu_image.h")
GFXU_IMAGE_H.setDestPath("gfx/utils/inc/")
GFXU_IMAGE_H.setProjectPath(utilsIncPath)
GFXU_IMAGE_H.setType("HEADER")

GFXU_IMAGE_UTILS_H = component.createFileSymbol("GFXU_IMAGE_UTILS_H", None)
GFXU_IMAGE_UTILS_H.setSourcePath("utils/inc/gfxu_image_utils.h")
GFXU_IMAGE_UTILS_H.setDestPath("gfx/utils/inc/")
GFXU_IMAGE_UTILS_H.setProjectPath(utilsIncPath)
GFXU_IMAGE_UTILS_H.setType("HEADER")

GFXU_PALETTE_H = component.createFileSymbol("GFXU_PALETTE_H", None)
GFXU_PALETTE_H.setSourcePath("utils/inc/gfxu_palette.h")
GFXU_PALETTE_H.setDestPath("gfx/utils/inc/")
GFXU_PALETTE_H.setProjectPath(utilsIncPath)
GFXU_PALETTE_H.setType("HEADER")

GFXU_STRING_H = component.createFileSymbol("GFXU_STRING_H", None)
GFXU_STRING_H.setSourcePath("utils/inc/gfxu_string.h")
GFXU_STRING_H.setDestPath("gfx/utils/inc/")
GFXU_STRING_H.setProjectPath(utilsIncPath)
GFXU_STRING_H.setType("HEADER")

GFXU_STRING_UTILS_H = component.createFileSymbol("GFXU_STRING_UTILS_H", None)
GFXU_STRING_UTILS_H.setSourcePath("utils/inc/gfxu_string_utils.h")
GFXU_STRING_UTILS_H.setDestPath("gfx/utils/inc/")
GFXU_STRING_UTILS_H.setProjectPath(utilsIncPath)
GFXU_STRING_UTILS_H.setType("HEADER")

GFXU_IMAGE_JPG_COMMON_H = component.createFileSymbol("GFXU_IMAGE_JPG_COMMON_H", None)
GFXU_IMAGE_JPG_COMMON_H.setSourcePath("utils/inc/gfxu_image_jpg_common.h")
GFXU_IMAGE_JPG_COMMON_H.setDestPath("gfx/utils/inc/")
GFXU_IMAGE_JPG_COMMON_H.setProjectPath(utilsIncPath)
GFXU_IMAGE_JPG_COMMON_H.setType("HEADER")

GFXU_IMAGE_JPG_COMMON_C = component.createFileSymbol("GFXU_IMAGE_JPG_COMMON_C", None)
GFXU_IMAGE_JPG_COMMON_C.setSourcePath("utils/src/gfxu_image_jpg_common.c")
GFXU_IMAGE_JPG_COMMON_C.setDestPath("gfx/utils/src/")
GFXU_IMAGE_JPG_COMMON_C.setProjectPath(utilsSrcPath)
GFXU_IMAGE_JPG_COMMON_C.setType("SOURCE")

GFXU_IMAGE_JPG_INTERNAL_C = component.createFileSymbol("GFXU_IMAGE_JPG_INTERNAL_C", None)
GFXU_IMAGE_JPG_INTERNAL_C.setSourcePath("utils/src/gfxu_image_jpg_internal.c")
GFXU_IMAGE_JPG_INTERNAL_C.setDestPath("gfx/utils/src/")
GFXU_IMAGE_JPG_INTERNAL_C.setProjectPath(utilsSrcPath)
GFXU_IMAGE_JPG_INTERNAL_C.setType("SOURCE")

GFXU_IMAGE_JPG_EXTERNAL_C = component.createFileSymbol("GFXU_IMAGE_JPG_EXTERNAL_C", None)
GFXU_IMAGE_JPG_EXTERNAL_C.setSourcePath("utils/src/gfxu_image_jpg_external.c")
GFXU_IMAGE_JPG_EXTERNAL_C.setDestPath("gfx/utils/src/")
GFXU_IMAGE_JPG_EXTERNAL_C.setProjectPath(utilsSrcPath)
GFXU_IMAGE_JPG_EXTERNAL_C.setType("SOURCE")

GFXU_IMAGE_PNG_EXTERNAL_C = component.createFileSymbol("GFXU_IMAGE_PNG_EXTERNAL_C", None)
GFXU_IMAGE_PNG_EXTERNAL_C.setSourcePath("utils/src/gfxu_image_png_external.c")
GFXU_IMAGE_PNG_EXTERNAL_C.setDestPath("gfx/utils/src/")
GFXU_IMAGE_PNG_EXTERNAL_C.setProjectPath(utilsSrcPath)
GFXU_IMAGE_PNG_EXTERNAL_C.setType("SOURCE")

GFXU_IMAGE_PNG_INTERNAL_C = component.createFileSymbol("GFXU_IMAGE_PNG_INTERNAL_C", None)
GFXU_IMAGE_PNG_INTERNAL_C.setSourcePath("utils/src/gfxu_image_png_internal.c")
GFXU_IMAGE_PNG_INTERNAL_C.setDestPath("gfx/utils/src/")
GFXU_IMAGE_PNG_INTERNAL_C.setProjectPath(utilsSrcPath)
GFXU_IMAGE_PNG_INTERNAL_C.setType("SOURCE")

GFXU_IMAGE_C = component.createFileSymbol("GFXU_IMAGE_C", None)
GFXU_IMAGE_C.setSourcePath("utils/src/gfxu_image.c")
GFXU_IMAGE_C.setDestPath("gfx/utils/src/")
GFXU_IMAGE_C.setProjectPath(utilsPath)
GFXU_IMAGE_C.setType("SOURCE")

GFXU_IMAGE_RAW_EXTERNAL_C = component.createFileSymbol("GFXU_IMAGE_RAW_EXTERNAL_C", None)
GFXU_IMAGE_RAW_EXTERNAL_C.setSourcePath("utils/src/gfxu_image_raw_external.c")
GFXU_IMAGE_RAW_EXTERNAL_C.setDestPath("gfx/utils/src/")
GFXU_IMAGE_RAW_EXTERNAL_C.setProjectPath(utilsPath)
GFXU_IMAGE_RAW_EXTERNAL_C.setType("SOURCE")

GFXU_IMAGE_RAW_INTERNAL_C = component.createFileSymbol("GFXU_IMAGE_RAW_INTERNAL_C", None)
GFXU_IMAGE_RAW_INTERNAL_C.setSourcePath("utils/src/gfxu_image_raw_internal.c")
GFXU_IMAGE_RAW_INTERNAL_C.setDestPath("gfx/utils/src/")
GFXU_IMAGE_RAW_INTERNAL_C.setProjectPath(utilsPath)
GFXU_IMAGE_RAW_INTERNAL_C.setType("SOURCE")

GFXU_IMAGE_RLE_EXTERNAL_C = component.createFileSymbol("GFXU_IMAGE_RLE_EXTERNAL_C", None)
GFXU_IMAGE_RLE_EXTERNAL_C.setSourcePath("utils/src/gfxu_image_rle_external.c")
GFXU_IMAGE_RLE_EXTERNAL_C.setDestPath("gfx/utils/src/")
GFXU_IMAGE_RLE_EXTERNAL_C.setProjectPath(utilsPath)
GFXU_IMAGE_RLE_EXTERNAL_C.setType("SOURCE")

GFXU_IMAGE_RLE_INTERNAL_C = component.createFileSymbol("GFXU_IMAGE_RLE_INTERNAL_C", None)
GFXU_IMAGE_RLE_INTERNAL_C.setSourcePath("utils/src/gfxu_image_rle_internal.c")
GFXU_IMAGE_RLE_INTERNAL_C.setDestPath("gfx/utils/src/")
GFXU_IMAGE_RLE_INTERNAL_C.setProjectPath(utilsPath)
GFXU_IMAGE_RLE_INTERNAL_C.setType("SOURCE")

GFXU_IMAGE_UTILS_C = component.createFileSymbol("GFXU_IMAGE_UTILS_C", None)
GFXU_IMAGE_UTILS_C.setSourcePath("utils/src/gfxu_image_utils.c")
GFXU_IMAGE_UTILS_C.setDestPath("gfx/utils/src/")
GFXU_IMAGE_UTILS_C.setProjectPath(utilsPath)
GFXU_IMAGE_UTILS_C.setType("SOURCE")

GFXU_PALETTE_C = component.createFileSymbol("GFXU_PALETTE_C", None)
GFXU_PALETTE_C.setSourcePath("utils/src/gfxu_palette.c")
GFXU_PALETTE_C.setDestPath("gfx/utils/src/")
GFXU_PALETTE_C.setProjectPath(utilsPath)
GFXU_PALETTE_C.setType("SOURCE")

GFXU_STRING_C = component.createFileSymbol("GFXU_STRING_C", None)
GFXU_STRING_C.setSourcePath("utils/src/gfxu_string.c")
GFXU_STRING_C.setDestPath("gfx/utils/src/")
GFXU_STRING_C.setProjectPath(utilsPath)
GFXU_STRING_C.setType("SOURCE")

GFXU_STRING_EXTERNAL_C = component.createFileSymbol("GFXU_STRING_EXTERNAL_C", None)
GFXU_STRING_EXTERNAL_C.setSourcePath("utils/src/gfxu_string_external.c")
GFXU_STRING_EXTERNAL_C.setDestPath("gfx/utils/src/")
GFXU_STRING_EXTERNAL_C.setProjectPath(utilsPath)
GFXU_STRING_EXTERNAL_C.setType("SOURCE")

GFXU_STRINGBUFFER_EXTERNAL_C = component.createFileSymbol("GFXU_STRINGBUFFER_EXTERNAL_C", None)
GFXU_STRINGBUFFER_EXTERNAL_C.setSourcePath("utils/src/gfxu_stringbuffer_external.c")
GFXU_STRINGBUFFER_EXTERNAL_C.setDestPath("gfx/utils/src/")
GFXU_STRINGBUFFER_EXTERNAL_C.setProjectPath(utilsPath)
GFXU_STRINGBUFFER_EXTERNAL_C.setType("SOURCE")

GFXU_STRING_INTERNAL_C = component.createFileSymbol("GFXU_STRING_INTERNAL_C", None)
GFXU_STRING_INTERNAL_C.setSourcePath("utils/src/gfxu_string_internal.c")
GFXU_STRING_INTERNAL_C.setDestPath("gfx/utils/src/")
GFXU_STRING_INTERNAL_C.setProjectPath(utilsPath)
GFXU_STRING_INTERNAL_C.setType("SOURCE")

GFXU_STRING_QUERY_C = component.createFileSymbol("GFXU_STRING_QUERY_C", None)
GFXU_STRING_QUERY_C.setSourcePath("utils/src/gfxu_string_query.c")
GFXU_STRING_QUERY_C.setDestPath("gfx/utils/src/")
GFXU_STRING_QUERY_C.setProjectPath(utilsPath)
GFXU_STRING_QUERY_C.setType("SOURCE")

GFXU_STRING_UTILS_C = component.createFileSymbol("GFXU_STRING_UTILS_C", None)
GFXU_STRING_UTILS_C.setSourcePath("utils/src/gfxu_string_utils.c")
GFXU_STRING_UTILS_C.setDestPath("gfx/utils/src/")
GFXU_STRING_UTILS_C.setProjectPath(utilsPath)
GFXU_STRING_UTILS_C.setType("SOURCE")

