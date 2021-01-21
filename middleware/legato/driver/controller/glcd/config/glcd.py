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

def instantiateComponent(comp):
	projectPath = "config/" + Variables.get("__CONFIGURATION_NAME") + "/gfx/driver/glcd"
	
	GLCDInterruptVector = "GLCD_INTERRUPT_ENABLE"
	GLCDInterruptHandler = "GLCD_INTERRUPT_HANDLER"
	GLCDInterruptHandlerLock = "GLCD_INTERRUPT_HANDLER_LOCK"
	GLCDRefClkSource = "CONFIG_SYS_CLK_REFCLK5"
	
	Database.setSymbolValue("core", GLCDInterruptVector, True, 1)
	Database.setSymbolValue("core", GLCDInterruptHandlerLock, True, 1)
	Database.setSymbolValue("core", GLCDInterruptHandler, "GLCD_Interrupt_Handler", 1)
	Database.setSymbolValue("core", GLCDRefClkSource + "_ENABLE", True, 1)

	DriverInitName = comp.createStringSymbol("DriverInitName", None)
	DriverInitName.setVisible(False)
	DriverInitName.setReadOnly(True)
	DriverInitName.setDefaultValue("glcdDisplayDriver")

	# System level 
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

	execfile(Module.getPath() + "/config/glcd_rtos.py")

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

        # common display controller header
	#DISPLAY_CONTROLLER_H = comp.createFileSymbol("DISPLAY_CONTROLLER_H", None)
	#DISPLAY_CONTROLLER_H.setSourcePath("../templates/display_controller.h.ftl")
	#DISPLAY_CONTROLLER_H.setDestPath("gfx/driver/controller/")
	#DISPLAY_CONTROLLER_H.setOutputName("display_controller.h")
	#DISPLAY_CONTROLLER_H.setProjectPath(projectPath)
	#DISPLAY_CONTROLLER_H.setType("HEADER")
        #DISPLAY_CONTROLLER_H.setMarkup(True)

	#DISPLAY_CONTROLLER_C = comp.createFileSymbol("DISPLAY_CONTROLLER_C", None)
	#DISPLAY_CONTROLLER_C.setSourcePath("../templates/display_controller.c.ftl")
	#DISPLAY_CONTROLLER_C.setDestPath("gfx/driver/controller/")
	#DISPLAY_CONTROLLER_C.setOutputName("display_controller.c")
	#DISPLAY_CONTROLLER_C.setProjectPath(projectPath)
	#DISPLAY_CONTROLLER_C.setType("SOURCE")
        #DISPLAY_CONTROLLER_C.setMarkup(True)
	
	# configuration options
	HALComment = comp.createCommentSymbol("HALComment", None)
	HALComment.setLabel("Some settings are being managed by the GFX Core LE and have been hidden.")
	HALComment.setVisible(False)

	DisplayTimingOptionsEnabled = comp.createBooleanSymbol("DisplayTimingOptionsEnabled", None)
	DisplayTimingOptionsEnabled.setLabel("Display Timing Options Enabled")
	DisplayTimingOptionsEnabled.setDescription("Hints to the HAL if display timing is configurable for this display.")
	DisplayTimingOptionsEnabled.setDefaultValue(True)
	DisplayTimingOptionsEnabled.setVisible(False)

	### Clock Settings Menu
	ClockSettingsMenu = comp.createMenuSymbol("ClockSettingsMenu", None)
	ClockSettingsMenu.setLabel("Clock Settings")

	#### Get the master clock source value
	MasterClockSource = comp.createComboSymbol("MasterClockSource", ClockSettingsMenu, ["REFCLK5"])
	MasterClockSource.setLabel("Master Clock Source")
	MasterClockSource.setDescription("The source master clock")
	MasterClockSource.setDefaultValue("REFCLK5")
	
	MasterClockValue = 200000000
	try:
		MasterClockValue = Database.getSymbolValue("core", GLCDRefClkSource + "_FREQ")
	except:
		print(MasterClockSource.getValue() + " symbol not found")

	print("MasterClockValue = " + str(MasterClockValue))
	MasterClockSourceValue = comp.createIntegerSymbol("MasterClockSourceValue", ClockSettingsMenu)
	MasterClockSourceValue.setLabel("Master Clock (Hz)")
	MasterClockSourceValue.setReadOnly(True)
	MasterClockSourceValue.setDescription("The source master clock.")
	MasterClockSourceValue.setDefaultValue(int(MasterClockValue))
	MasterClockSourceValue.setUseSingleDynamicValue(True)
	MasterClockSourceValue.setMin(1)

	PixelClockDivider = comp.createIntegerSymbol("PixelClockDivider", ClockSettingsMenu)
	PixelClockDivider.setLabel("Pixel Clock Divider (1-32)")
	PixelClockDivider.setDescription("Master clock divider for GLCD.")
	PixelClockDivider.setDefaultValue(6)
	PixelClockDivider.setMax(32)
	PixelClockDivider.setMin(1)
	PixelClockDivider.setDependencies(onPixelDividerSet, ["PixelClockDivider"])

	clockValue = MasterClockSourceValue.getValue() / PixelClockDivider.getValue()

	PixelClock = comp.createIntegerSymbol("PixelClock", ClockSettingsMenu)
	PixelClock.setLabel("Pixel Clock (Hz)")
	PixelClock.setDescription("The approximate pixel clock frequency generated by the GLCD.")
	PixelClock.setReadOnly(True)
	PixelClock.setDefaultValue(clockValue)
	PixelClock.setMin(1)
	PixelClock.setDependencies(onPixelClockSet, ["PixelClock"])

	### Layer Configuration Menu
	LayerConfigurationMenu = comp.createMenuSymbol("LayerConfigurationMenu", None)
	LayerConfigurationMenu.setLabel("Layer Configuration")
	
	EnableLayersMenu = comp.createMenuSymbol("EnableLayersMenu", LayerConfigurationMenu)
	EnableLayersMenu.setLabel("Enable Layers")
	
	Layer0Enable = comp.createBooleanSymbol("Layer0Enable", EnableLayersMenu)
	Layer0Enable.setLabel("Layer 0")
	Layer0Enable.setDescription("Enables Layer 0")
	Layer0Enable.setDefaultValue(True)
	Layer0Enable.setDependencies(OnLayersEnabled, ["Layer0Enable"])
	
	Layer1Enable = comp.createBooleanSymbol("Layer1Enable", EnableLayersMenu)
	Layer1Enable.setLabel("Layer 1")
	Layer1Enable.setDescription("Enables Layer 1")
	Layer1Enable.setDefaultValue(True)
	Layer1Enable.setDependencies(OnLayersEnabled, ["Layer1Enable"])
	
	Layer2Enable = comp.createBooleanSymbol("Layer2Enable", EnableLayersMenu)
	Layer2Enable.setLabel("Layer 2")
	Layer2Enable.setDescription("Enables Layer 2")
	Layer2Enable.setDefaultValue(True)
	Layer2Enable.setDependencies(OnLayersEnabled, ["Layer2Enable"])

	Layer0Buffer0 = comp.createStringSymbol("Layer0Buffer0", None)
	Layer0Buffer0.setDefaultValue("0xA8000000")
	Layer0Buffer0.setVisible(False)
	Layer0Buffer1 = comp.createStringSymbol("Layer0Buffer1", None)
	Layer0Buffer1.setDefaultValue("0xA8465000")
	Layer0Buffer1.setVisible(False)

	Layer1Buffer0 = comp.createStringSymbol("Layer1Buffer0", None)
	Layer1Buffer0.setDefaultValue("0xA8177000")
	Layer1Buffer0.setVisible(False)
	Layer1Buffer1 = comp.createStringSymbol("Layer1Buffer1", None)
	Layer1Buffer1.setDefaultValue("0xA85DC000")
	Layer1Buffer1.setVisible(False)

	Layer2Buffer0 = comp.createStringSymbol("Layer2Buffer0", None)
	Layer2Buffer0.setDefaultValue("0xA82EE000")
	Layer2Buffer0.setVisible(False)
	Layer2Buffer1 = comp.createStringSymbol("Layer2Buffer1", None)
	Layer2Buffer1.setDefaultValue("0xA8753000")
	Layer2Buffer1.setVisible(False)

	#Shadow symbol counter for number of layers used - not user modifiable (hidden)
	TotalNumLayers = comp.createIntegerSymbol("TotalNumLayers", LayerConfigurationMenu)
	TotalNumLayers.setLabel("Number of Layers")
	TotalNumLayers.setDescription("Number of layers enabled")
	TotalNumLayers.setVisible(False)
	
	### Display Timing Settings
	DisplaySettingsMenu = comp.createMenuSymbol("DisplaySettingsMenu", None)
	DisplaySettingsMenu.setLabel("Display Settings")

	DisplayWidth = comp.createIntegerSymbol("DisplayWidth", DisplaySettingsMenu)
	DisplayWidth.setLabel("Width")
	DisplayWidth.setDescription("The width of the frame buffer in pixels.")
	DisplayWidth.setDefaultValue(480)
	DisplayWidth.setMin(1)

	DisplayHeight = comp.createIntegerSymbol("DisplayHeight", DisplaySettingsMenu)
	DisplayHeight.setLabel("Height")
	DisplayHeight.setDescription("The height of the frame buffer in pixels.")
	DisplayHeight.setDefaultValue(272)
	DisplayHeight.setMin(1)

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
	DisplayVSYNCNegative.setLabel("VSYNC Polarity Positive?")
	DisplayVSYNCNegative.setDescription("Indicates if this display requries positive VSYNC polarity.")
	DisplayVSYNCNegative.setDefaultValue(False)

	DisplayHSYNCNegative = comp.createBooleanSymbol("DisplayHSYNCNegative", DisplaySettingsMenu)
	DisplayHSYNCNegative.setLabel("HSYNC Polarity Positive?")
	DisplayHSYNCNegative.setDescription("Indicates if this display requries positive HSYNC polarity.")
	DisplayHSYNCNegative.setDefaultValue(False)

	DisplayDataEnable = comp.createBooleanSymbol("DisplayDataEnable", DisplaySettingsMenu)
	DisplayDataEnable.setLabel("Use Data Enable?")
	DisplayDataEnable.setDescription("Indicates if this display requries the use of the Data Enable line.")
	DisplayDataEnable.setDefaultValue(True)

	DisplayDataEnablePolarity = comp.createBooleanSymbol("DisplayDataEnablePolarity", DisplaySettingsMenu)
	DisplayDataEnablePolarity.setLabel("Data Enable Polarity Positive?")
	DisplayDataEnablePolarity.setDescription("Indicates if this display Data Enable polarity is positive.")
	DisplayDataEnablePolarity.setDefaultValue(True)

	##### GLCD Does not control the Chip Select I/O, disable and hide
	DisplayUseChipSelect = comp.createBooleanSymbol("DisplayUseChipSelect", DisplaySettingsMenu)
	DisplayUseChipSelect.setLabel("Use Chip Select?")
	DisplayUseChipSelect.setDescription("Indicates if this display uses the chip select line.")
	DisplayUseChipSelect.setDefaultValue(False)
	DisplayUseChipSelect.setVisible(False)

	DisplayChipSelectPolarity = comp.createBooleanSymbol("DisplayChipSelectPolarity", DisplaySettingsMenu)
	DisplayChipSelectPolarity.setLabel("Chip Select Polarity Positive?")
	DisplayChipSelectPolarity.setDescription("Indicates if this display chip select line should be positive.")
	DisplayChipSelectPolarity.setDefaultValue(True)
	DisplayChipSelectPolarity.setVisible(False)
	
	DisplayUseReset = comp.createBooleanSymbol("DisplayUseReset", DisplaySettingsMenu)
	DisplayUseReset.setLabel("Use Reset?")
	DisplayUseReset.setDescription("Indicates if this display reset line should be used.")
	DisplayUseReset.setDefaultValue(True)
	DisplayUseReset.setVisible(False)

	DisplayResetPolarity = comp.createBooleanSymbol("DisplayResetPolarity", DisplaySettingsMenu)
	DisplayResetPolarity.setLabel("Reset Polarity Positive?")
	DisplayResetPolarity.setDescription("Indicates if this display reset line should be reset positive.")
	DisplayResetPolarity.setDefaultValue(True)
	DisplayResetPolarity.setVisible(False)
	### End of unused settings

	### Frame buffer settings
	FrameBufferSettingsMenu = comp.createMenuSymbol("FrameBufferSettingsMenu", None)
	FrameBufferSettingsMenu.setLabel("Frame Buffer Settings")
	
	FrameBufferColorMode = comp.createKeyValueSetSymbol("FrameBufferColorMode", FrameBufferSettingsMenu)
	FrameBufferColorMode.setLabel("Frame Buffer Color Mode")
	FrameBufferColorMode.setOutputMode("Value")
	FrameBufferColorMode.setDescription("FrameBuffer Color Mode")
	FrameBufferColorMode.addKey("GS_8", "GS_8", "LUT Palette 8bpp")
	FrameBufferColorMode.addKey("RGB_565", "RGB_565", "RGB565 16bpp")
	FrameBufferColorMode.addKey("RGBA_8888", "RGBA_8888", "RGBA8888 32bpp")
	FrameBufferColorMode.setDefaultValue(2)

	DoubleBuffer = comp.createBooleanSymbol("DoubleBuffer", FrameBufferSettingsMenu)
	DoubleBuffer.setLabel("Use Double Buffering?")
	DoubleBuffer.setDescription("<html>Uses an additional buffer for off-screen drawing.<br>Eliminates screen tearing but doubles the required memory.</html>")

	FrameBufferMemoryMode = comp.createKeyValueSetSymbol("FrameBufferMemoryMode", FrameBufferSettingsMenu)
	FrameBufferMemoryMode.setLabel("Frame Buffer Memory Mode")
	FrameBufferMemoryMode.setOutputMode("Value")
	FrameBufferMemoryMode.setDescription("FrameBuffer Memory Mode")
	FrameBufferMemoryMode.addKey("INT_SRAM", "INT_SRAM", "Internal SRAM")
	FrameBufferMemoryMode.addKey("DDR", "DDR", "DDR (INT/EXT)")
	FrameBufferMemoryMode.setDefaultValue(1)

	PaletteMode = comp.createBooleanSymbol("PaletteMode", FrameBufferSettingsMenu)
	PaletteMode.setLabel("Use 8-bit Palette?")
	PaletteMode.setDescription("<html>Enables frame buffer compression.<br>Uses an 8-bit color lookup table to reduce the required<br>frame buffer memory size.  This also reduces the<br>maximum avilable color count to 256 and significantly<br>slows down display refresh speed.</html>")
	PaletteMode.setDefaultValue(False)
	PaletteMode.setVisible(True)
	### End of frame buffer settings

	### Other Timing Settings
	OtherSettingsMenu = comp.createMenuSymbol("OtherMenu", None)
	OtherSettingsMenu.setLabel("Other Settings")
	
	CanvasModeOnly = comp.createBooleanSymbol("CanvasModeOnly", OtherSettingsMenu)
	CanvasModeOnly.setLabel("Canvas Mode")
	CanvasModeOnly.setDefaultValue(False)
	CanvasModeOnly.setDescription("<html>Enables Canvas Mode in the driver. Only the GFX Canvas interface will be enabled. </html>")

	UseGPU = comp.createBooleanSymbol("UseGPU", OtherSettingsMenu)
	UseGPU.setLabel("Use GPU for Blits?")
	UseGPU.setDefaultValue(True)
	UseGPU.setDescription("<html>Uses the Nano2D GPU for buffer blits.</html>")
	UseGPU.setDependencies(useGPUChanged, ["UseGPU"])

	VblankBlit = comp.createBooleanSymbol("VblankBlit", UseGPU)
	VblankBlit.setLabel("Blit on Vblank")
	VblankBlit.setDefaultValue(False)
	VblankBlit.setVisible(True)
	VblankBlit.setDescription("<html>Schedule blits during vblank.</html>")
	
	### Unsupported symbols, but may be queried by GFX HAL
	UnsupportedOptionsMenu = comp.createMenuSymbol("UnsupportedOptionsMenu", None)
	UnsupportedOptionsMenu.setLabel("Unsupported options group")
	UnsupportedOptionsMenu.setVisible(False)
	
	LCCRefresh = comp.createBooleanSymbol("LCCRefresh", UnsupportedOptionsMenu)
	LCCRefresh.setLabel("Use Aggressive Refresh Strategy?")
	### End of unsupported options
	
	# generated code files
	GFX_GLCD_C = comp.createFileSymbol("GFX_GLCD_C", None)
	GFX_GLCD_C.setDestPath("gfx/driver/controller/glcd/")
	GFX_GLCD_C.setSourcePath("templates/drv_gfx_glcd.c.ftl")
	GFX_GLCD_C.setOutputName("drv_gfx_glcd.c")
	GFX_GLCD_C.setProjectPath(projectPath)
	GFX_GLCD_C.setType("SOURCE")
	GFX_GLCD_C.setMarkup(True)
	
	GFX_GLCD_H = comp.createFileSymbol("GFX_GLCD_H", None)
	GFX_GLCD_H.setSourcePath("templates/drv_gfx_glcd.h.ftl")
	GFX_GLCD_H.setDestPath("gfx/driver/controller/glcd/")
	GFX_GLCD_H.setOutputName("drv_gfx_glcd.h")
	GFX_GLCD_H.setProjectPath(projectPath)
	GFX_GLCD_H.setType("HEADER")
	GFX_GLCD_H.setMarkup(True)
	
	GFX_GLCD_PLIB_C = comp.createFileSymbol("GFX_GLCD_PLIB_C", None)
	GFX_GLCD_PLIB_C.setDestPath("gfx/driver/controller/glcd/")
	GFX_GLCD_PLIB_C.setSourcePath("templates/plib_glcd.c")
	GFX_GLCD_PLIB_C.setOutputName("plib_glcd.c")
	GFX_GLCD_PLIB_C.setProjectPath(projectPath)
	GFX_GLCD_PLIB_C.setType("SOURCE")
	
	GFX_GLCD_PLIB_H = comp.createFileSymbol("GFX_GLCD_PLIB_H", None)
	GFX_GLCD_PLIB_H.setSourcePath("templates/plib_glcd.h")
	GFX_GLCD_PLIB_H.setDestPath("gfx/driver/controller/glcd/")
	GFX_GLCD_PLIB_H.setOutputName("plib_glcd.h")
	GFX_GLCD_PLIB_H.setProjectPath(projectPath)
	GFX_GLCD_PLIB_H.setType("HEADER")
	
	### Update the layers count - do this last
	numLayers = 0
	if (Layer0Enable.getValue() == True):
		numLayers += 1
	if (Layer1Enable.getValue() == True):
		numLayers += 1
	if (Layer2Enable.getValue() == True):
		numLayers += 1
	TotalNumLayers.setValue(numLayers, 1)

###def onHALConnected(halConnected, event):
###	halConnected.getComponent().getSymbolByID("HALComment").setVisible(event["value"] == True)
###	halConnected.getComponent().getSymbolByID("DisplayWidth").setVisible(event["value"] == False)
###	halConnected.getComponent().getSymbolByID("DisplayHeight").setVisible(event["value"] == False)
###	halConnected.getComponent().getSymbolByID("DoubleBuffer").setVisible(event["value"] == False)
###	halConnected.getComponent().getSymbolByID("PaletteMode").setVisible(event["value"] == False)
###	halConnected.getComponent().getSymbolByID("DisplaySettingsMenu").setVisible(event["value"] == False)
###	halConnected.getComponent().getSymbolByID("FrameBufferColorMode").setVisible(event["value"] == False)

	### Update the layer count hint in GFX HAL
###	numLayers = halConnected.getComponent().getSymbolValue("TotalNumLayers")
###	Database.setSymbolValue("gfx_hal", "HardwareLayerCountHint", numLayers, 1)
	
def onAttachmentConnected(source, target):
	print("dependency Connected = " + target["component"].getDisplayName())
	gfxCoreComponentTable = ["gfx_hal_le"]
	if (Database.getComponentByID("gfx_hal_le") is None):
		Database.activateComponents(gfxCoreComponentTable)
	updateDisplayManager(source["component"], target)

def OnCacheEnabled(cacheEnabled, event):
	cacheEnabled.getComponent().setSymbolValue("UseCachedFrameBuffer", event["value"] == True, 1)
	
def OnLayersEnabled(layerEnabled, event):
	numLayers = 0
	if (layerEnabled.getComponent().getSymbolValue("Layer0Enable") == True):
		numLayers += 1
	if (layerEnabled.getComponent().getSymbolValue("Layer1Enable") == True):
		numLayers += 1
	if (layerEnabled.getComponent().getSymbolValue("Layer2Enable") == True):
		numLayers += 1
	layerEnabled.getComponent().setSymbolValue("TotalNumLayers", numLayers, 1)
	if (Database.getComponentByID("gfx_hal_le") is not None):
		Database.setSymbolValue("gfx_hal_le", "HardwareLayerCountHint", numLayers, 1)

def onPixelClockSet(pixelClockSet, event):
	if (Database.getComponentByID("gfx_hal_le") is not None):
		Database.setSymbolValue("gfx_hal_le", "PixelClock", event["value"], 1)

def onPixelDividerSet(pixelDividerSet, event):
	clockValue = int(pixelDividerSet.getComponent().getSymbolValue("MasterClockSourceValue")) / int(pixelDividerSet.getComponent().getSymbolValue("PixelClockDivider"))
	pixelDividerSet.getComponent().setSymbolValue("PixelClock", clockValue, 1)
	
def showRTOSMenu(symbol, event):
	symbol.setVisible(event["value"] != "BareMetal")

def useGPUChanged(symbol, event):
	symbol.getComponent().getSymbolByID("VblankBlit").setVisible(event["value"] == True)

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
		Database.setSymbolValue("gfx_hal_le", "PixelClock", component.getSymbolValue("PixelClock"), 1)
		component.getSymbolByID("DisplaySettingsMenu").setVisible(False)
		component.getSymbolByID("HALComment").setVisible(True)
