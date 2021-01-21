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

DrawPipelineEnable = halComponent.createBooleanSymbol("DrawPipelineEnable", None)
DrawPipelineEnable.setLabel("Draw Pipeline Configuration")
DrawPipelineEnable.setDefaultValue(True)
DrawPipelineEnable.setDescription("<html>Disables the entire draw pipeline.<br>Disable this if the HAL native rasterizer is not needed.</html>")
DrawPipelineEnable.setHelp("IDH_HTML_MHGC_UG_Draw_Pipeline_Options")

DrawPipelineStagesMenu = halComponent.createMenuSymbol("DrawPipelineStages", DrawPipelineEnable)
DrawPipelineStagesMenu.setLabel("Draw Pipeline Stages")
DrawPipelineStagesMenu.setDescription("Configure individual pixel pipeline stages.")
DrawPipelineStagesMenu.setDependencies(onDrawPipelineEnableChanged, ["DrawPipelineEnable"])

AlphaBlendingEnable = halComponent.createBooleanSymbol("AlphaBlendingEnable", DrawPipelineStagesMenu)
AlphaBlendingEnable.setLabel("Alpha Blending")
AlphaBlendingEnable.setDescription("<html>Enables the alpha blending stage.<br>Any form of pixel color blending support requires this stage.</html>")
AlphaBlendingEnable.setDefaultValue(True)
AlphaBlendingEnable.setHelp("IDH_HTML_MHGC_UG_Graphics_Pipeline_Options")

BoundsClippingEnable = halComponent.createBooleanSymbol("BoundsClippingEnable", DrawPipelineStagesMenu)
BoundsClippingEnable.setLabel("Bounds Clipping")
BoundsClippingEnable.setDescription("<html>Enables discrete boundary area clipping.<br>Draw operations may render outside of boundary<br>limitations if this stage is disabled.</html>")
BoundsClippingEnable.setDefaultValue(True)
BoundsClippingEnable.setHelp("IDH_HTML_MHGC_UG_Graphics_Pipeline_Options")

LayerClippingEnable = halComponent.createBooleanSymbol("LayerClippingEnable", DrawPipelineStagesMenu)
LayerClippingEnable.setLabel("Layer Clipping")
LayerClippingEnable.setDescription("<html>Enables discrete layer bounds clipping.<br>Draw operations may render outside of the memoryarea of a<br>frame buffer and cause undefined application behavior.</html>")
LayerClippingEnable.setDefaultValue(True)
LayerClippingEnable.setHelp("IDH_HTML_MHGC_UG_Graphics_Pipeline_Options")

ColorConversionEnable = halComponent.createBooleanSymbol("ColorConversionEnable", DrawPipelineStagesMenu)
ColorConversionEnable.setLabel("Color Format Conversion")
ColorConversionEnable.setDescription("<html>Enables per-pixel color format conversion.<br>This is not needed if all draw operations use the same<br>color format as the target frame buffer.</html>")
ColorConversionEnable.setDefaultValue(True)
ColorConversionEnable.setHelp("IDH_HTML_MHGC_UG_Graphics_Pipeline_Options")

ColorMaskingEnable = halComponent.createBooleanSymbol("ColorMaskingEnable", DrawPipelineStagesMenu)
ColorMaskingEnable.setLabel("Color Masking")
ColorMaskingEnable.setDescription("<html>Enables per-pixel color discard stage.<br>Color masking involves discarding all pixels of<br>a certain color to achieve a form of transparency.</html>")
ColorMaskingEnable.setDefaultValue(True)
ColorMaskingEnable.setHelp("IDH_HTML_MHGC_UG_Graphics_Pipeline_Options")

DisplayOrienetationEnable = halComponent.createBooleanSymbol("DisplayOrientationEnable", DrawPipelineStagesMenu)
DisplayOrienetationEnable.setLabel("Display Orientation")
DisplayOrienetationEnable.setDescription("<html>Enables per-pixel orientation transformation stage.<br>This stage allows displays to be orthogonally rotated.</html>")
DisplayOrienetationEnable.setDefaultValue(True)
DisplayOrienetationEnable.setHelp("IDH_HTML_MHGC_UG_Graphics_Pipeline_Options")

DisplayMirroringEnable = halComponent.createBooleanSymbol("DisplayMirroringEnable", DrawPipelineStagesMenu)
DisplayMirroringEnable.setLabel("Display Mirroring")
DisplayMirroringEnable.setDescription("<html>Enables per-pixel mirror transformation stage.<br>This stage allows displays to be mirrored.</html>")
DisplayMirroringEnable.setDefaultValue(True)
DisplayMirroringEnable.setHelp("IDH_HTML_MHGC_UG_Graphics_Pipeline_Options")

PrimitiveInterfacesMenu = halComponent.createMenuSymbol("PrimitiveInterfacesMenu", DrawPipelineEnable)
PrimitiveInterfacesMenu.setLabel("Primitive Interface Configuration")
PrimitiveInterfacesMenu.setDescription("Configure individual primitive drawing interfaces.")
PrimitiveInterfacesMenu.setDependencies(onDrawPipelineEnableChanged, ["DrawPipelineEnable"])
PrimitiveInterfacesMenu.setHelp("IDH_HTML_MHGC_UG_Graphics_Pipeline_Options")

DrawArcEnable = halComponent.createBooleanSymbol("DrawArcEnable", PrimitiveInterfacesMenu)
DrawArcEnable.setLabel("Arc")
DrawArcEnable.setDescription("Enables Arc primitive drawing interface.")
DrawArcEnable.setDefaultValue(True)
DrawArcEnable.setHelp("IDH_HTML_MHGC_UG_Graphics_Pipeline_Options")

DrawEllipseEnable = halComponent.createBooleanSymbol("DrawEllipseEnable", PrimitiveInterfacesMenu)
DrawEllipseEnable.setLabel("Ellipse")
DrawEllipseEnable.setDescription("Enables Ellipse primitive drawing interface.")
DrawEllipseEnable.setDefaultValue(True)
DrawEllipseEnable.setHelp("IDH_HTML_MHGC_UG_Graphics_Pipeline_Options")




