# coding: utf-8
##############################################################################
# Copyright (C) 2018-2020 Microchip Technology Inc. and its subsidiaries.
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

maxNumCanvas = 20
defaultNumCanvas = 3

import re

def instantiateComponent(comp):
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

	SYS_TASK_C = comp.createFileSymbol("SYS_TASK_C", None)
	SYS_TASK_C.setType("STRING")
	SYS_TASK_C.setOutputName("core.LIST_SYSTEM_TASKS_C_CALL_DRIVER_TASKS")
	SYS_TASK_C.setSourcePath("templates/tasks.c.ftl")
	SYS_TASK_C.setMarkup(True)
	
	SYS_RTOS_TASK_C = comp.createFileSymbol("SYS_RTOS_TASK_C", None)
	SYS_RTOS_TASK_C.setType("STRING")
	SYS_RTOS_TASK_C.setOutputName("core.LIST_SYSTEM_RTOS_TASKS_C_DEFINITIONS")
	SYS_RTOS_TASK_C.setSourcePath("templates/rtos_tasks.c.ftl")
	SYS_RTOS_TASK_C.setMarkup(True)
	
	DriverInitName = comp.createStringSymbol("DriverInitName", None)
	DriverInitName.setVisible(False)
	DriverInitName.setReadOnly(True)
	DriverInitName.setDefaultValue("gfxcVirtualDisplayDriver")

	CanvasSettings = comp.createMenuSymbol("CanvasSettings", None)
	CanvasSettings.setLabel("Canvas Settings")
	
	WindowClippingEnabled = comp.createBooleanSymbol("WindowClippingEnabled", CanvasSettings)
	WindowClippingEnabled.setLabel("Enable Window Clipping")
	WindowClippingEnabled.setDefaultValue(True)
	WindowClippingEnabled.setDescription("<html> Clips the canvas window if the canvas goes out of bounds. <br>"
			"Enable for display controllers that don't allow out of bounds frames.</html>")

	ColorMode = comp.createComboSymbol("ColorMode" , CanvasSettings, ["GS_8", "RGB_565", "RGBA_8888"])
	ColorMode.setLabel("Default Color Mode")
	ColorMode.setDescription("<html> The default color mode of all the canvas objects</html>")
	ColorMode.setDefaultValue("RGBA_8888")

	DefaultNumLayers = comp.createIntegerSymbol("DefaultNumLayers", CanvasSettings)
	DefaultNumLayers.setLabel("Default Number of Layers")
	DefaultNumLayers.setDescription("<html>The default number of layeres. <br>"
		"This value will be overridden by the actual layers in the display controller, if used.</html>")
	DefaultNumLayers.setDefaultValue(3)

	MaXCanvasObjects = comp.createIntegerSymbol("MaXCanvasObjects", CanvasSettings)
	MaXCanvasObjects.setLabel("Max Number of Parameters")
	MaXCanvasObjects.setDefaultValue(maxNumCanvas)
	MaXCanvasObjects.setReadOnly(True)
	MaXCanvasObjects.setDescription("<html> Maximum number (" + str(maxNumCanvas) + ") of canvas objects. <br>"
			"To modify, set 'maxNumCanvas' in gfx\middleware\legato\driver\virtual\canvas\config\canvas.py.</html>")

	NumCanvasObjects = comp.createIntegerSymbol("NumCanvasObjects", CanvasSettings)
	NumCanvasObjects.setLabel("Canvas Objects")
	NumCanvasObjects.setMax(maxNumCanvas)
	NumCanvasObjects.setDefaultValue(defaultNumCanvas)
	NumCanvasObjects.setDescription("<html> The number of canvas that will be available. </html>")
	NumCanvasObjects.setDependencies(onNumCanvasObjectsChanged, ["NumCanvasObjects"])

	for x in range(maxNumCanvas):
		Canvas = comp.createMenuSymbol("Canvas" + str(x), NumCanvasObjects)
		Canvas.setLabel("Canvas[" + str(x) + "]")
		
		Width = comp.createIntegerSymbol("Canvas" + str(x) + "Width" , Canvas)
		Width.setLabel("Width")
		Width.setDefaultValue(480)
		Width.setDescription("<html> The horizontal resolution (width) of the canvas in pixels. </html>")
		
		Height = comp.createIntegerSymbol("Canvas" + str(x) + "Height" , Canvas)
		Height.setLabel("Height")
		Height.setDefaultValue(272)
		Height.setDescription("<html> The vertical resolution (height) of the canvas in pixels. </html>")
		
		Mode = comp.createComboSymbol("Canvas" + str(x) + "Mode" , Canvas, ["GS_8", "RGB_565", "RGBA_8888"])
		Mode.setLabel("Color Mode")
		Mode.setDescription("<html> The color mode of the canvas. </html>")
		Mode.setDefaultValue("RGBA_8888")
		
		FrameBufferAlloc = comp.createComboSymbol("Canvas" + str(x) + "FrameBufferAlloc", Canvas, ["None", "Manual", "Auto"])
		FrameBufferAlloc.setLabel("Frame Buffer Allocation")
		FrameBufferAlloc.setDescription("<html> The allocation type for the canvas frame/pixel buffer."
			"<br> 'None' will assign a NULL value. Application code will need to call gfxcSetPixelBuffer() to set the pixel buffer properties."
			"<br> 'Manual' will let you specify the address. Use this if using memory that is not managed by the linker."
			"<br> 'Auto' will statically declare the buffer array and will be allocated in memory that is managed by the linker (e.g., internal SRAM).</html>")
		FrameBufferAlloc.setDependencies(onFrameBufferAllocChanged, ["Canvas" + str(x) + "FrameBufferAlloc"])
		
		Address = comp.createHexSymbol("Canvas" + str(x) + "Address", FrameBufferAlloc)
		Address.setLabel("Address")
		Address.setDescription("<html> Frame buffer address in memory. </html>")
		Address.setVisible(False)

		if (x > defaultNumCanvas - 1):
			Canvas.setVisible(False)

	EffectsEnabled = comp.createBooleanSymbol("EffectsEnabled", None)
	EffectsEnabled.setLabel("Effects Settings")
	EffectsEnabled.setDefaultValue(True)
	EffectsEnabled.setDescription("<html> Enable effects support.</html>")
	EffectsEnabled.setDependencies(onEffectsEnabled, ["EffectsEnabled"])

	EffectsInterval = comp.createIntegerSymbol("EffectsInterval", EffectsEnabled)
	EffectsInterval.setLabel("Effects Interval (ms)")
	EffectsInterval.setDescription("<html>Update interval for canvas effects. <br>"
		"A small value will process effects at faster rate.</html>")
	EffectsInterval.setDefaultValue(40)
	EffectsInterval.setVisible(True)

	FadeEffectsEnabled = comp.createBooleanSymbol("FadeEffectsEnabled", EffectsEnabled)
	FadeEffectsEnabled.setLabel("Enable Fade Effects")
	FadeEffectsEnabled.setDefaultValue(True)
	FadeEffectsEnabled.setDescription("<html> Enable fade effects. Only supported in display controllers that allow per layer alpha.</html>")
	FadeEffectsEnabled.setVisible(True)

	MoveEffectsEnabled = comp.createBooleanSymbol("MoveEffectsEnabled", EffectsEnabled)
	MoveEffectsEnabled.setLabel("Enable Move Effects")
	MoveEffectsEnabled.setDefaultValue(True)
	MoveEffectsEnabled.setDescription("<html> Enable move effects. Only supported in display controllers that allow per layer positioning and sizing.</html>")
	MoveEffectsEnabled.setVisible(True)

	DisplayDriverSettings = comp.createMenuSymbol("DisplayDriverSettings", None)
	DisplayDriverSettings.setLabel("Display Driver Settings")

	DisplayDriverName = comp.createStringSymbol("DisplayDriverName", DisplayDriverSettings)
	DisplayDriverName.setLabel("Display Driver")
	DisplayDriverName.setReadOnly(True)
	DisplayDriverName.setDefaultValue("NULL")
	DisplayDriverName.setDescription("<html> The display controller driver that is used to display the canvas objects on the screen. </html>")
	
	GraphicsProcessorSettings = comp.createMenuSymbol("GraphicsProcessorSettings", None)
	GraphicsProcessorSettings.setLabel("Graphics Processor Driver Settings")

	GPUBlitEnabled = comp.createBooleanSymbol("GPUBlitEnabled", GraphicsProcessorSettings)
	GPUBlitEnabled.setLabel("Use GPU to blit scratch buffer")
	GPUBlitEnabled.setDefaultValue(False)
	GPUBlitEnabled.setDescription("<html> Enable to use the GPU for blit'ng the scratch buffer to the frame.</html>")
	GPUBlitEnabled.setDependencies(onGPUBlitEnabled, ["GPUBlitEnabled"])

	GraphicsProcessorDriverName = comp.createStringSymbol("GraphicsProcessorDriverName", GraphicsProcessorSettings)
	GraphicsProcessorDriverName.setLabel("Graphics Processor Driver")
	GraphicsProcessorDriverName.setReadOnly(True)
	GraphicsProcessorDriverName.setDefaultValue("_2dgpuGraphicsProcessor")
	GraphicsProcessorDriverName.setDescription("<html> The graphics processor  driver that is used to blit scratch buffers. </html>")
	GraphicsProcessorDriverName.setVisible(False)

	execfile(Module.getPath() + "/config/files.py")
	execfile(Module.getPath() + "/config/rtos.py")


def onAttachmentConnected(source, target):
	print("dependency Connected = " + str(target['id']))
	if source["id"] == "gfx_display_driver":
		driverName = target["component"].getSymbolValue("DriverInitName")
		source["component"].setSymbolValue("DisplayDriverName", driverName)

def showRTOSMenu(symbol, event):
	symbol.setVisible(event["value"] != "BareMetal")

def onGPUBlitEnabled(symbol, event):
	symbol.getComponent().getSymbolByID("GraphicsProcessorDriverName").setVisible((event["value"]) == True)

def onNumCanvasObjectsChanged(source, event):
	global maxNumCanvas
	print("Canvas count : " + str(event["value"]))
	for x in range(maxNumCanvas - 1):
		if (x < int(event["value"])):
			source.getComponent().getSymbolByID("Canvas" + str(x)).setVisible(x < int(event["value"]))

def onFrameBufferAllocChanged(source, event):
	sub = re.search('Canvas(.*)FrameBufferAlloc', str(event["id"]))
	if (sub and sub.group(1)):
		source.getComponent().getSymbolByID("Canvas" + str(sub.group(1)) + "Address").setVisible(event["value"] == "Manual")
		
def onEffectsEnabled(source, event):
	source.getComponent().getSymbolByID("EffectsInterval").setVisible(event["value"])
	source.getComponent().getSymbolByID("FadeEffectsEnabled").setVisible(event["value"])
	source.getComponent().getSymbolByID("MoveEffectsEnabled").setVisible(event["value"])

