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
	projectPath = "config/" + Variables.get("__CONFIGURATION_NAME") + "/gfx/driver/glcd"
	
	GLCDInterruptVector = "GLCD_INTERRUPT_ENABLE"
	GLCDInterruptHandler = "GLCD_INTERRUPT_HANDLER"
	GLCDInterruptHandlerLock = "GLCD_INTERRUPT_HANDLER_LOCK"
	GLCDRefClkSource = "CONFIG_SYS_CLK_REFCLK5"
	
	Database.setSymbolValue("core", GLCDInterruptVector, True, 1)
	Database.setSymbolValue("core", GLCDInterruptHandlerLock, True, 1)
	Database.setSymbolValue("core", GLCDInterruptHandler, "GLCD_Interrupt_Handler", 1)
	Database.setSymbolValue("core", GLCDRefClkSource + "_ENABLE", True, 1)
	
	HALConnected = comp.createBooleanSymbol("HALConnected", None)
	HALConnected.setVisible(False)
	HALConnected.setDependencies(onHALConnected, ["HALConnected"])
	
	# these two symbols are read by the HAL for initialization purposes
	# they must match the function names in the actual driver code
	DriverInfoFunction = comp.createStringSymbol("DriverInfoFunction", None)
	DriverInfoFunction.setLabel("Driver Info Function Name")
	DriverInfoFunction.setReadOnly(True)
	DriverInfoFunction.setDefaultValue("driverGLCDInfoGet")
	DriverInfoFunction.setVisible(False)
	
	DriverInitFunction = comp.createStringSymbol("DriverInitFunction", None)
	DriverInitFunction.setLabel("Driver Init Function Name")
	DriverInitFunction.setReadOnly(True)
	DriverInitFunction.setDefaultValue("driverGLCDContextInitialize")
	DriverInitFunction.setVisible(False)
	
	# configuration options
	HALComment = comp.createCommentSymbol("HALComment", None)
	HALComment.setLabel("Some settings are being managed by the GFX Core and have been hidden.")
	HALComment.setVisible(False)
	
	DisplayWidth = comp.createIntegerSymbol("DisplayWidth", None)
	DisplayWidth.setLabel("Width")
	DisplayWidth.setDescription("The width of the frame buffer in pixels.")
	DisplayWidth.setDefaultValue(480)
	DisplayWidth.setMin(1)

	DisplayHeight = comp.createIntegerSymbol("DisplayHeight", None)
	DisplayHeight.setLabel("Height")
	DisplayHeight.setDescription("The height of the frame buffer in pixels.")
	DisplayHeight.setDefaultValue(272)
	DisplayHeight.setMin(1)

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
	
	MasterClockSourceValue = comp.createIntegerSymbol("MasterClockSourceValue", ClockSettingsMenu)
	MasterClockSourceValue.setLabel("Master Clock (Hz)")
	MasterClockSourceValue.setReadOnly(True)
	MasterClockSourceValue.setDescription("The source master clock.")
	MasterClockSourceValue.setDependencies(getGlcdClk,["core.CONFIG_SYS_CLK_REFCLK5_FREQ"]);
	MasterClockSourceValue.setUseSingleDynamicValue(True)
	MasterClockSourceValue.setMin(1)

	PixelClockDivider = comp.createIntegerSymbol("PixelClockDivider", ClockSettingsMenu)
	PixelClockDivider.setLabel("Pixel Clock Divider (1-32)")
	PixelClockDivider.setDescription("Master clock divider for GLCD.")
	PixelClockDivider.setDefaultValue(6)
	PixelClockDivider.setMax(32)
	PixelClockDivider.setMin(1)
	PixelClockDivider.setDependencies(onPixelDividerSet, ["PixelClockDivider"])

	defaultClockValue = MasterClockSourceValue.getValue() / PixelClockDivider.getValue()

	PixelClock = comp.createIntegerSymbol("PixelClock", ClockSettingsMenu)
	PixelClock.setLabel("Pixel Clock (Hz)")
	PixelClock.setDescription("The approximate pixel clock frequency generated by the GLCD.")
	PixelClock.setReadOnly(True)
	PixelClock.setDefaultValue(defaultClockValue)
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
	FrameBufferColorMode.addKey("LUT8", "GFX_COLOR_MODE_GS_8", "LUT Palette 8bpp")
	FrameBufferColorMode.addKey("RGB_565", "GFX_COLOR_MODE_RGB_565", "RGB565 16bpp")
	FrameBufferColorMode.addKey("RGBA_8888", "GFX_COLOR_MODE_RGBA_8888", "RGBA8888 32bpp")
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
	GFX_GLCD_H.setSourcePath("templates/drv_gfx_glcd.h")
	GFX_GLCD_H.setDestPath("gfx/driver/controller/glcd/")
	GFX_GLCD_H.setOutputName("drv_gfx_glcd.h")
	GFX_GLCD_H.setProjectPath(projectPath)
	GFX_GLCD_H.setType("HEADER")
	
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

def onHALConnected(halConnected, event):
	halConnected.getComponent().getSymbolByID("HALComment").setVisible(event["value"] == True)
	halConnected.getComponent().getSymbolByID("DisplayWidth").setVisible(event["value"] == False)
	halConnected.getComponent().getSymbolByID("DisplayHeight").setVisible(event["value"] == False)
	halConnected.getComponent().getSymbolByID("DoubleBuffer").setVisible(event["value"] == False)
	halConnected.getComponent().getSymbolByID("PaletteMode").setVisible(event["value"] == False)
	halConnected.getComponent().getSymbolByID("DisplaySettingsMenu").setVisible(event["value"] == False)
	halConnected.getComponent().getSymbolByID("FrameBufferColorMode").setVisible(event["value"] == False)

	### Update the layer count hint in GFX HAL
	numLayers = halConnected.getComponent().getSymbolValue("TotalNumLayers")
	Database.setSymbolValue("gfx_hal", "HardwareLayerCountHint", numLayers, 1)
	
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
	if (layerEnabled.getComponent().getSymbolValue("HALConnected") == True):
		Database.setSymbolValue("gfx_hal", "HardwareLayerCountHint", numLayers, 1)

def getGlcdClk(symbol, event):
	sourceClk = Database.getSymbolValue("core", "CONFIG_SYS_CLK_REFCLK5_FREQ")
	symbol.setValue(int(sourceClk), 1);
	clockValue = int(sourceClk) / int(symbol.getComponent().getSymbolValue("PixelClockDivider"))
	symbol.getComponent().setSymbolValue("PixelClock", clockValue, 1)

def onPixelClockSet(pixelClockSet, event):
	if (pixelClockSet.getComponent().getSymbolValue("HALConnected") == True):
		Database.setSymbolValue("gfx_hal", "PixelClockHint", event["value"], 1)

def onPixelDividerSet(pixelDividerSet, event):
	sourceClk = Database.getSymbolValue("core", "CONFIG_SYS_CLK_REFCLK5_FREQ")
	clockValue = int(sourceClk) / int(pixelDividerSet.getComponent().getSymbolValue("PixelClockDivider"))
	pixelDividerSet.getComponent().setSymbolValue("PixelClock", clockValue, 1)
	
