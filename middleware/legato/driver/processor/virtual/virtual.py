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

def instantiateComponent(comp):
	projectPath = "config/" + Variables.get("__CONFIGURATION_NAME") + "/gfx/driver/vgpu"

	SYS_DEFINITIONS_H = comp.createFileSymbol("SYS_DEFINITIONS_H", None)
	SYS_DEFINITIONS_H.setType("STRING")
	SYS_DEFINITIONS_H.setOutputName("core.LIST_SYSTEM_DEFINITIONS_H_INCLUDES")
	SYS_DEFINITIONS_H.setSourcePath("templates/definitions.h.ftl")
	SYS_DEFINITIONS_H.setMarkup(True)

	SYS_INIT_C = comp.createFileSymbol("SYS_INIT_C", None)
	SYS_INIT_C.setType("STRING")
	SYS_INIT_C.setOutputName("core.LIST_SYSTEM_INIT_C_SYS_INITIALIZE_DRIVERS")
	SYS_INIT_C.setSourcePath("templates/init.c.ftl")
	SYS_INIT_C.setMarkup(True)

	GFX_VGPU_H = comp.createFileSymbol("GFX_VGPU_H", None)
	GFX_VGPU_H.setSourcePath("templates/drv_gfx_vgpu.h.ftl")
	GFX_VGPU_H.setDestPath("gfx/driver/processor/vgpu")
	GFX_VGPU_H.setOutputName("drv_gfx_vgpu.h")
	GFX_VGPU_H.setProjectPath(projectPath)
	GFX_VGPU_H.setType("HEADER")
	GFX_VGPU_H.setMarkup(True)

	vgpuIntf = comp.createComboSymbol("vgpuIntf", None, ["Custom"])
	vgpuIntf.setLabel("GPU Interface")
	vgpuIntf.setDependencies(vgpuIntfSet, ["vgpuIntf"])
    
	vgpuIntfCustomMenu = comp.createMenuSymbol("vgpuIntfCustomMenu", None)
	vgpuIntfCustomMenu.setLabel("Interface Functions")
    
	vgpuIntfCustomComment = comp.createCommentSymbol("vgpuIntfCustomComment", vgpuIntfCustomMenu)
	vgpuIntfCustomComment.setLabel('Note: Please define all user-specified interface functions in application code.')

	
	intfInit = comp.createStringSymbol("intfInit", vgpuIntfCustomMenu)
	intfInit.setLabel("Initialization")
	intfInit.setDefaultValue("NULL")
	
	drawLineFunc = comp.createStringSymbol("drawLineFunc", vgpuIntfCustomMenu)
	drawLineFunc.setLabel("Line Draw")
	drawLineFunc.setDefaultValue("NULL")
	
	fillRectFunc = comp.createStringSymbol("fillRectFunc", vgpuIntfCustomMenu)
	fillRectFunc.setLabel("Rect Fill")
	fillRectFunc.setDefaultValue("NULL")
	
	blitBuffFunc = comp.createStringSymbol("blitBuffFunc", vgpuIntfCustomMenu)
	blitBuffFunc.setLabel("Blit Buffer")
	blitBuffFunc.setDefaultValue("NULL")
	
	blendFunc = comp.createStringSymbol("blendFunc", vgpuIntfCustomMenu)
	blendFunc.setLabel("Blend")
	blendFunc.setDefaultValue("NULL")
	
	setGlobalAlphaFunc = comp.createStringSymbol("setGlobalAlphaFunc", vgpuIntfCustomMenu)
	setGlobalAlphaFunc.setLabel("Set Global Alpha")
	setGlobalAlphaFunc.setDefaultValue("NULL")
	
	setPaletteFunc = comp.createStringSymbol("setPaletteFunc", vgpuIntfCustomMenu)
	setPaletteFunc.setLabel("Set Palette")
	setPaletteFunc.setDefaultValue("NULL")
	
	setTransFunc = comp.createStringSymbol("setTransFunc", vgpuIntfCustomMenu)
	setTransFunc.setLabel("Set Transparency")
	setTransFunc.setDefaultValue("NULL")   
	
def vgpuIntfSet(source, event):
	source.getComponent().getSymbolByID("vgpuIntfCustomMenu").setVisible(event["value"] == "Custom")
	source.getComponent().getSymbolByID("vgpuIntfCustomComment").setVisible(event["value"] == "Custom")

