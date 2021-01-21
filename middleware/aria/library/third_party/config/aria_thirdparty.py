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

utilsPath = "config/" + Variables.get("__CONFIGURATION_NAME") + "/gfx/third_party/"

JIDCTINT_C = component.createFileSymbol("JIDCTINT_C", None)
JIDCTINT_C.setSourcePath("third_party/decoder/jidctint/src/jidctint.c")
JIDCTINT_C.setDestPath("gfx/third_party/decoder/src/")
JIDCTINT_C.setProjectPath("config/" + Variables.get("__CONFIGURATION_NAME") + "/gfx/third_party/")
JIDCTINT_C.setType("SOURCE")

LODE_PNG_DECODER_H = component.createFileSymbol("LODE_PNG_DECODER_H", None)
LODE_PNG_DECODER_H.setSourcePath("third_party/decoder/lodepng/lodepng.h")
LODE_PNG_DECODER_H.setDestPath("gfx/third_party/lodepng/")
LODE_PNG_DECODER_H.setProjectPath("config/" + Variables.get("__CONFIGURATION_NAME") + "/gfx/third_party/lodepng/")
LODE_PNG_DECODER_H.setType("HEADER")

LODE_PNG_DECODER_C = component.createFileSymbol("LODE_PNG_DECODER_C", None)
LODE_PNG_DECODER_C.setSourcePath("third_party/decoder/lodepng/lodepng.c")
LODE_PNG_DECODER_C.setDestPath("gfx/third_party/src/lodepng/")
LODE_PNG_DECODER_C.setProjectPath("config/" + Variables.get("__CONFIGURATION_NAME") + "/gfx/third_party/lodepng/")
LODE_PNG_DECODER_C.setType("SOURCE")