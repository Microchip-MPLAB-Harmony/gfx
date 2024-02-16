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

import re

def instantiateComponent(comp):
	projectPath = "config/" + Variables.get("__CONFIGURATION_NAME") + "/gfx/driver/lcc"
	
	comp.setHelpFile("../../../docs/help_harmony_gfx_html_alias.h")
	#comp.setHelp("IDH_HTML_GFX_CMP__3__Display_Driver_Component")
	
	# common gfx driver header
	GFX_DRIVER_H = comp.createFileSymbol("GFX_DRIVER_H", None)
	GFX_DRIVER_H.setSourcePath("../../templates/gfx_driver.h.ftl")
	GFX_DRIVER_H.setDestPath("gfx/driver/")
	GFX_DRIVER_H.setOutputName("gfx_driver.h")
	GFX_DRIVER_H.setProjectPath(projectPath)
	GFX_DRIVER_H.setType("HEADER")
	GFX_DRIVER_H.setMarkup(True)

	GFX_DRIVER_C = comp.createFileSymbol("GFX_DRIVER_C", None)
	GFX_DRIVER_C.setSourcePath("../../templates/gfx_driver.c.ftl")
	GFX_DRIVER_C.setDestPath("gfx/driver/")
	GFX_DRIVER_C.setOutputName("gfx_driver.c")
	GFX_DRIVER_C.setProjectPath(projectPath)
	GFX_DRIVER_C.setType("SOURCE")
	GFX_DRIVER_C.setMarkup(True)

	DriverInitName = comp.createStringSymbol("DriverInitName", None)
	DriverInitName.setVisible(False)
	DriverInitName.setReadOnly(True)
	DriverInitName.setDefaultValue("lccDisplayDriver")

	# these two symbols are read by the HAL for initialization purposes
	# they must match the function names in the actual driver code
	DriverInfoFunction = comp.createStringSymbol("DriverInfoFunction", None)
	DriverInfoFunction.setLabel("Driver Info Function Name")
	DriverInfoFunction.setReadOnly(True)
	DriverInfoFunction.setDefaultValue("driverLCCInfoGet")
	DriverInfoFunction.setVisible(False)
	
	DriverInitFunction = comp.createStringSymbol("DriverInitFunction", None)
	DriverInitFunction.setLabel("Driver Init Function Name")
	DriverInitFunction.setReadOnly(True)
	DriverInitFunction.setDefaultValue("driverLCCContextInitialize")
	DriverInitFunction.setVisible(False)

	SYS_DEFINITIONS_H = comp.createFileSymbol("SYS_DEFINITIONS_H", None)
	SYS_DEFINITIONS_H.setType("STRING")
	SYS_DEFINITIONS_H.setOutputName("core.LIST_SYSTEM_DEFINITIONS_H_INCLUDES")
	SYS_DEFINITIONS_H.setSourcePath("templates/definitions.h.ftl")
	SYS_DEFINITIONS_H.setMarkup(True)
	
	# configuration options
	HALComment = comp.createCommentSymbol("HALComment", None)
	HALComment.setLabel("Some settings are being managed by the GFX Core LE and have been hidden.")
	HALComment.setVisible(False)
	
	DisplayTimingOptionsEnabled = comp.createBooleanSymbol("DisplayTimingOptionsEnabled", None)
	DisplayTimingOptionsEnabled.setLabel("Display Timing Options Enabled")
	DisplayTimingOptionsEnabled.setDescription("Hints to the HAL if display timing is configurable for this display.")
	DisplayTimingOptionsEnabled.setDefaultValue(True)
	DisplayTimingOptionsEnabled.setVisible(False)
	
	DisplaySettingsMenu = comp.createMenuSymbol("DisplaySettingsMenu", None)
	DisplaySettingsMenu.setLabel("Display Settings")

	DisplayWidth = comp.createIntegerSymbol("DisplayWidth", DisplaySettingsMenu)
	DisplayWidth.setLabel("Width")
	DisplayWidth.setDescription("The width of the frame buffer in pixels.")
	DisplayWidth.setDefaultValue(480)

	DisplayHeight = comp.createIntegerSymbol("DisplayHeight", DisplaySettingsMenu)
	DisplayHeight.setLabel("Height")
	DisplayHeight.setDescription("The height of the frame buffer in pixels.")
	DisplayHeight.setDefaultValue(272)
	
	DisplayHorzMenu = comp.createMenuSymbol("DisplayHorzMenu", DisplaySettingsMenu)
	DisplayHorzMenu.setLabel("Horizontal Attributes")
	DisplayHorzMenu.setDescription("Contains the display horizontal refresh values.")

	DisplayHorzPulseWidth = comp.createIntegerSymbol("DisplayHorzPulseWidth", DisplayHorzMenu)
	DisplayHorzPulseWidth.setLabel("Horizontal Pulse Width")
	DisplayHorzPulseWidth.setDescription("The horizontal pulse width.")
	DisplayHorzPulseWidth.setDefaultValue(41)
	DisplayHorzPulseWidth.setMin(0)

	DisplayHorzBackPorch = comp.createIntegerSymbol("DisplayHorzBackPorch", DisplayHorzMenu)
	DisplayHorzBackPorch.setLabel("Horizontal Back Porch")
	DisplayHorzBackPorch.setDescription("The horizontal back porch size in pixels.")
	DisplayHorzBackPorch.setDefaultValue(2)
	DisplayHorzBackPorch.setMin(0)

	DisplayHorzFrontPorch = comp.createIntegerSymbol("DisplayHorzFrontPorch", DisplayHorzMenu)
	DisplayHorzFrontPorch.setLabel("Horizontal Front Porch")
	DisplayHorzFrontPorch.setDescription("The horizontal front porch size in pixels.")
	DisplayHorzFrontPorch.setDefaultValue(2)
	DisplayHorzFrontPorch.setMin(0)

	DisplayVertMenu = comp.createMenuSymbol("DisplayVertMenu", DisplaySettingsMenu)
	DisplayVertMenu.setLabel("Vertical Attributes")
	DisplayVertMenu.setDescription("Contains the display vertical refresh values.")

	DisplayVertPulseWidth = comp.createIntegerSymbol("DisplayVertPulseWidth", DisplayVertMenu)
	DisplayVertPulseWidth.setLabel("Vertical Pulse Width")
	DisplayVertPulseWidth.setDescription("The vertical pulse width.")
	DisplayVertPulseWidth.setDefaultValue(10)
	DisplayVertPulseWidth.setMin(0)

	DisplayVertBackPorch = comp.createIntegerSymbol("DisplayVertBackPorch", DisplayVertMenu)
	DisplayVertBackPorch.setLabel("Vertical Back Porch")
	DisplayVertBackPorch.setDescription("The vertical back porch size in pixels.")
	DisplayVertBackPorch.setDefaultValue(2)
	DisplayVertBackPorch.setMin(0)

	DisplayVertFrontPorch = comp.createIntegerSymbol("DisplayVertFrontPorch", DisplayVertMenu)
	DisplayVertFrontPorch.setLabel("Vertical Front Porch")
	DisplayVertFrontPorch.setDescription("The vertical front porch size in pixels.")
	DisplayVertFrontPorch.setDefaultValue(2)
	DisplayVertFrontPorch.setMin(0)
	
	DisplayBacklightEnable = comp.createIntegerSymbol("DisplayBacklightEnable", DisplaySettingsMenu)
	DisplayBacklightEnable.setLabel("Back Light Enable Value")
	DisplayBacklightEnable.setDescription("The value used to enable the display back light.")
	DisplayBacklightEnable.setDefaultValue(1)

	DisplayVSYNCNegative = comp.createBooleanSymbol("DisplayVSYNCNegative", DisplaySettingsMenu)
	DisplayVSYNCNegative.setLabel("VSYNC Polarity Negative?")
	DisplayVSYNCNegative.setDescription("Indicates if this display requries negative VSYNC polarity.")
	DisplayVSYNCNegative.setDefaultValue(True)

	DisplayHSYNCNegative = comp.createBooleanSymbol("DisplayHSYNCNegative", DisplaySettingsMenu)
	DisplayHSYNCNegative.setLabel("HSYNC Polarity Negative?")
	DisplayHSYNCNegative.setDescription("Indicates if this display requries negative HSYNC polarity.")
	DisplayHSYNCNegative.setDefaultValue(True)

	DisplayDataEnable = comp.createBooleanSymbol("DisplayDataEnable", DisplaySettingsMenu)
	DisplayDataEnable.setLabel("Use Data Enable?")
	DisplayDataEnable.setDescription("Indicates if this display requries the use of the Data Enable line.")
	DisplayDataEnable.setDefaultValue(True)

	DisplayDataEnablePolarity = comp.createBooleanSymbol("DisplayDataEnablePolarity", DisplaySettingsMenu)
	DisplayDataEnablePolarity.setLabel("Data Enable Polarity Positive?")
	DisplayDataEnablePolarity.setDescription("Indicates if this display Data Enable polarity is positive.")
	DisplayDataEnablePolarity.setDefaultValue(True)

	DisplayUseReset = comp.createBooleanSymbol("DisplayUseReset", DisplaySettingsMenu)
	DisplayUseReset.setLabel("Use Reset?")
	DisplayUseReset.setDescription("Indicates if this display reset line should be used.")
	DisplayUseReset.setDefaultValue(True)

	DisplayResetPolarity = comp.createBooleanSymbol("DisplayResetPolarity", DisplaySettingsMenu)
	DisplayResetPolarity.setLabel("Reset Polarity Positive?")
	DisplayResetPolarity.setDescription("Indicates if this display reset line should be reset positive.")
	DisplayResetPolarity.setDefaultValue(True)
	
	DisplayUseChipSelect = comp.createBooleanSymbol("DisplayUseChipSelect", DisplaySettingsMenu)
	DisplayUseChipSelect.setLabel("Use Chip Select?")
	DisplayUseChipSelect.setDescription("Indicates if this display uses the chip select line.")
	DisplayUseChipSelect.setDefaultValue(True)

	DisplayChipSelectPolarity = comp.createBooleanSymbol("DisplayChipSelectPolarity", DisplaySettingsMenu)
	DisplayChipSelectPolarity.setLabel("Chip Select Polarity Positive?")
	DisplayChipSelectPolarity.setDescription("Indicates if this display chip select line should be positive.")
	DisplayChipSelectPolarity.setDefaultValue(True)
	
	FrameBufferSettingsMenu = comp.createMenuSymbol("FrameBufferSettingsMenu", None)
	FrameBufferSettingsMenu.setLabel("Frame Buffer Settings")

	ColorMode = comp.createComboSymbol("ColorMode", FrameBufferSettingsMenu, ["RGB565","RGB332"])
	ColorMode.setLabel("Color Mode")
	ColorMode.setDescription("Frame Buffer and Interface Color Mode")
	ColorMode.setDefaultValue("RGB565")
	
	PaletteMode = comp.createBooleanSymbol("PaletteMode", FrameBufferSettingsMenu)
	PaletteMode.setLabel("Use 8-bit Palette?")
	PaletteMode.setDescription("<html>Enables frame buffer compression.<br>Uses an 8-bit color lookup table to reduce the required<br>frame buffer memory size.  This also reduces the<br>maximum avilable color count to 256 and significantly<br>slows down display refresh speed.</html>")
	PaletteMode.setDependencies(onPaletteModeEnabled, ["PaletteMode"])

	DoubleLineBuffer = comp.createBooleanSymbol("DoubleLineBuffer", PaletteMode)
	DoubleLineBuffer.setLabel("Use Double Line Buffering?")
	DoubleLineBuffer.setDescription("<html>Uses ping-pong line buffers to minimize refresh latency due to palette lookup. Doubles memory requirement for line buffer. </html>")
	DoubleLineBuffer.setDefaultValue(False)
	DoubleLineBuffer.setVisible(False)
	
	### DMA Menu
	DMAMenu = comp.createMenuSymbol("DMAMenu", None)
	DMAMenu.setLabel("DMA Settings")
	
	DMAController = comp.createStringSymbol("DMAController", DMAMenu)
	DMAController.setLabel("DMA Controller")
	DMAController.setDefaultValue("DMAC")
	DMAController.setReadOnly(True)
	
	DMAChannel = comp.createIntegerSymbol("DMAChannel", DMAMenu)
	DMAChannel.setLabel("DMA Channel")
	DMAChannel.setDefaultValue(0)
	DMAChannel.setMin(0)
	DMAChannel.setMax(23)
	
	DMATimerTriggerIndex = comp.createIntegerSymbol("DMATimerTriggerIndex", DMAMenu)
	DMATimerTriggerIndex.setLabel("Timer Trigger Index")
	DMATimerTriggerIndex.setDescription("The TC Timer trigger source for DMA. Configure Timer to adjust the Pixel Clock frequency.")
	DMATimerTriggerIndex.setDefaultValue(0)
	DMATimerTriggerIndex.setMin(0)
	DMATimerTriggerIndex.setMax(7)

	### Start of Backlight config options
	BacklightSettings = comp.createMenuSymbol("BacklightSettings", None)
	BacklightSettings.setLabel("Backlight Settings")
	
	DefaultBrightness = comp.createIntegerSymbol("DefaultBrightness", BacklightSettings)
	DefaultBrightness.setLabel("Default Brightness (%)")
	DefaultBrightness.setDescription("The default brightness setting at driver startup")
	DefaultBrightness.setMin(0)
	DefaultBrightness.setMax(100)
	DefaultBrightness.setDefaultValue(100)

	PeripheralControl = comp.createComboSymbol("PeripheralControl", BacklightSettings, ["GPIO"])
	PeripheralControl.setLabel("Peripheral")
	PeripheralControl.setDescription("Peripheral used to control the backlight.")
	PeripheralControl.setDefaultValue("GPIO")
	### End of Backlight config options
	
	### Start of Display Inteface Port config
	DisplayInterfaceMenu = comp.createMenuSymbol("DisplayInterfaceMenu", None)
	DisplayInterfaceMenu.setLabel("Interface Settings")

	PortGroup = comp.createIntegerSymbol("PortGroup", DisplayInterfaceMenu)
	PortGroup.setLabel("PortGroup")
	PortGroup.setDefaultValue(1)
	PortGroup.setDescription("Port Group used for 16-bit LCD Data. ")
	PortGroup.setMax(3)
    
	PinGroup = comp.createComboSymbol("PinGroup", PortGroup, ["31 - 16", "15 - 0"])
	PinGroup.setLabel("Pins")
	PinGroup.setDescription("Pins used for 16-bit LCD Data.")

	### End of Display Inteface Port config

	# generated code files
	GFX_LCC_C = comp.createFileSymbol("GFX_LCC_C", None)
	GFX_LCC_C.setSourcePath("templates/drv_gfx_lcc_e5xd5x.c.ftl")
	GFX_LCC_C.setDestPath("gfx/driver/controller/lcc/")
	GFX_LCC_C.setOutputName("drv_gfx_lcc.c")
	GFX_LCC_C.setProjectPath(projectPath)
	GFX_LCC_C.setType("SOURCE")
	GFX_LCC_C.setMarkup(True)
	
	GFX_LCC_H = comp.createFileSymbol("GFX_LCC_H", None)
	GFX_LCC_H.setSourcePath("templates/drv_gfx_lcc.h.ftl")
	GFX_LCC_H.setDestPath("gfx/driver/controller/lcc/")
	GFX_LCC_H.setOutputName("drv_gfx_lcc.h")
	GFX_LCC_H.setProjectPath(projectPath)
	GFX_LCC_H.setType("HEADER")

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

	execfile(Module.getPath() + "/config/lcc_rtos.py")
	
	if (Database.getComponentByID("ccl") is None):
		Database.activateComponents("ccl")	 

def configureDisplaySettings(lccComponent, displayComponent):
	lccComponent.setSymbolValue("DisplayHorzPulseWidth", displayComponent.getSymbolByID("HorzPulseWidth").getValue())
	lccComponent.setSymbolValue("DisplayHorzBackPorch", displayComponent.getSymbolByID("HorzBackPorch").getValue())
	lccComponent.setSymbolValue("DisplayHorzFrontPorch", displayComponent.getSymbolByID("HorzFrontPorch").getValue())
	lccComponent.setSymbolValue("DisplayVertPulseWidth", displayComponent.getSymbolByID("VertPulseWidth").getValue())
	lccComponent.setSymbolValue("DisplayVertBackPorch", displayComponent.getSymbolByID("VertBackPorch").getValue())
	lccComponent.setSymbolValue("DisplayVertFrontPorch", displayComponent.getSymbolByID("VertFrontPorch").getValue())
	lccComponent.setSymbolValue("DisplayWidth", displayComponent.getSymbolByID("DisplayWidth").getValue())
	lccComponent.setSymbolValue("DisplayHeight", displayComponent.getSymbolByID("DisplayHeight").getValue())
	if (Database.getComponentByID("gfx_hal_le") is not None):
		Database.setSymbolValue("gfx_hal_le", "gfx_display", displayComponent.getDisplayName(), 1)
	
def resetDisplaySettings(lccComponent, displayComponent):
	lccComponent.clearSymbolValue("DisplayHorzPulseWidth")
	lccComponent.clearSymbolValue("DisplayHorzBackPorch")
	lccComponent.clearSymbolValue("DisplayHorzFrontPorch")
	lccComponent.clearSymbolValue("DisplayVertPulseWidth")
	lccComponent.clearSymbolValue("DisplayVertBackPorch")
	lccComponent.clearSymbolValue("DisplayVertFrontPorch")
	lccComponent.clearSymbolValue("DisplayWidth")
	lccComponent.clearSymbolValue("DisplayHeight")

def configureTimerComponent(timerComponent):
	timerComponent.setSymbolValue("TC_CTRLA_MODE", 1, 1)
	timerComponent.setSymbolValue("TC_OPERATION_MODE", "Compare", 1)

def onAttachmentConnected(source, target):
	print("dependency Connected = " + target["component"].getDisplayName())
	if (Database.getComponentByID("gfx_hal_le") is None):
		Database.activateComponents("gfx_hal_le")
	updateDisplayManager(source["component"], target)
	if (source["id"] == "Graphics Display"):
		configureDisplaySettings(source["component"], target["component"])
	elif (source["id"] == "TMR"):
		sub = re.search('TC(.*)', target["component"].getDisplayName())
		if (sub and sub.group(1)):
			source["component"].getSymbolByID("DMATimerTriggerIndex").setValue(int(sub.group(1)), 1)
		configureTimerComponent(target["component"])

def onAttachmentDisconnected(source, target):
	if (source["id"] == "Graphics Display"):
		resetDisplaySettings(source["component"], target["component"])

def showRTOSMenu(symbol, event):
	symbol.setVisible(event["value"] != "BareMetal")
	
def onPaletteModeEnabled(symbol, event):
	symbol.getComponent().getSymbolByID("DoubleLineBuffer").setVisible(event["value"])
	
def updateDisplayManager(component, target):
	if (Database.getComponentByID("gfx_hal_le") is not None):
		if target["id"] == "gfx_display":
			Database.setSymbolValue("gfx_hal_le", "gfx_display", component.getDependencyComponent("Graphics Display").getID(), 1)
			Database.setSymbolValue("gfx_hal_le", "DisplayName", component.getDependencyComponent("Graphics Display").getDisplayName(), 1)
			Database.setSymbolValue("gfx_hal_le", "DisplayWidth", target["component"].getSymbolValue("DisplayWidth"), 1)	
			Database.setSymbolValue("gfx_hal_le", "DisplayHeight", target["component"].getSymbolValue("DisplayHeight"), 1)
			Database.setSymbolValue("gfx_hal_le", "DisplayHorzPulseWidth", target["component"].getSymbolValue("HorzPulseWidth"), 1)	
			Database.setSymbolValue("gfx_hal_le", "DisplayHorzBackPorch", target["component"].getSymbolValue("HorzBackPorch"), 1)
			Database.setSymbolValue("gfx_hal_le", "DisplayHorzFrontPorch", target["component"].getSymbolValue("HorzFrontPorch"), 1)	
			Database.setSymbolValue("gfx_hal_le", "DisplayVertPulseWidth", target["component"].getSymbolValue("VertPulseWidth"), 1)
			Database.setSymbolValue("gfx_hal_le", "DisplayVertBackPorch", target["component"].getSymbolValue("VertBackPorch"), 1)	
			Database.setSymbolValue("gfx_hal_le", "DisplayVertFrontPorch", target["component"].getSymbolValue("VertFrontPorch"), 1)
			Database.setSymbolValue("gfx_hal_le", "DisplayDataEnablePolarity", target["component"].getSymbolValue("DataEnablePolarity"), 1)
			Database.setSymbolValue("gfx_hal_le", "DisplayVSYNCNegative", target["component"].getSymbolValue("VSYNCNegative"), 1)
			Database.setSymbolValue("gfx_hal_le", "DisplayHSYNCNegative", target["component"].getSymbolValue("HSYNCNegative"), 1)
			component.setSymbolValue("DisplayWidth", target["component"].getSymbolValue("DisplayWidth"), 1)
			component.setSymbolValue("DisplayHeight", target["component"].getSymbolValue("DisplayHeight"), 1)
		Database.setSymbolValue("gfx_hal_le", "gfx_driver", component.getID(), 1)
		Database.setSymbolValue("gfx_hal_le", "DriverName", component.getDisplayName(), 1)
		component.getSymbolByID("DisplaySettingsMenu").setVisible(False)
		component.getSymbolByID("HALComment").setVisible(True)

