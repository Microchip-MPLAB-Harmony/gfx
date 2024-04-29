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
	
	try:
		MasterClockValue = Database.getSymbolValue("core", "CONFIG_SYS_CLK_REFCLK5_FREQ")
	except:
		print(MasterClockSource.getValue() + " symbol not found")

	#### print("MasterClockValue = " + str(MasterClockValue))
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
	PixelClockDivider.setDependencies(onPixelDividerSet, ["core:CONFIG_SYS_CLK_REFCLK5_FREQ"])

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
	
	TotalNumLayers = comp.createIntegerSymbol("TotalNumLayers", LayerConfigurationMenu)
	TotalNumLayers.setLabel("Number of Layers Enabled")
	TotalNumLayers.setDescription("Number of layers enabled")
	TotalNumLayers.setMin(0)
	TotalNumLayers.setReadOnly(True)
	TotalNumLayers.setVisible(True)
	
	AdvancedLayerSettings = comp.createBooleanSymbol("AdvancedLayerSettings", LayerConfigurationMenu)
	AdvancedLayerSettings.setLabel("Enable Advanced Layer Configuration")
	AdvancedLayerSettings.setDescription("Enable Advanced Layer Configuration")
	AdvancedLayerSettings.setDefaultValue(False)
	AdvancedLayerSettings.setVisible(True)
	AdvancedLayerSettings.setDependencies(OnAdvancedLayerEnabled, ["AdvancedLayerSettings"])
	
	EnableLayersMenu = comp.createMenuSymbol("EnableLayersMenu", AdvancedLayerSettings)
	EnableLayersMenu.setLabel("Enable Layers")
	
	Layer0Enable = comp.createBooleanSymbol("Layer0Enable", EnableLayersMenu)
	Layer0Enable.setLabel("Layer 0")
	Layer0Enable.setDescription("Enables Layer 0")
	Layer0Enable.setDefaultValue(True)
	Layer0Enable.setDependencies(OnLayersEnabled, ["Layer0Enable"])
	
	Layer0Width = comp.createIntegerSymbol("Layer0Width", Layer0Enable)
	Layer0Width.setLabel("Layer 0 Width")
	Layer0Width.setDescription("Width of Layer 0 in pixels")
	Layer0Width.setDefaultValue(480)
	Layer0Width.setMin(0)
	Layer0Width.setVisible(False)

	Layer0Height = comp.createIntegerSymbol("Layer0Height", Layer0Enable)
	Layer0Height.setLabel("Layer 0 Height")
	Layer0Height.setDescription("Height of Layer 0 in pixels")
	Layer0Height.setDefaultValue(272)
	Layer0Height.setMin(0)
	Layer0Height.setVisible(False)

	Layer0StartX = comp.createIntegerSymbol("Layer0StartX", Layer0Enable)
	Layer0StartX.setLabel("Layer 0 Start X")
	Layer0StartX.setDescription("Upper Left X Coordinate of Layer 0 (Relative to full display resolution)")
	Layer0StartX.setDefaultValue(480)
	Layer0StartX.setMin(0)
	Layer0StartX.setVisible(False)

	Layer0StartY = comp.createIntegerSymbol("Layer0StartY", Layer0Enable)
	Layer0StartY.setLabel("Layer 0 Start Y")
	Layer0StartY.setDescription("Upper Left Y Coordinate of Layer 0 (Relative to full display resolution)")
	Layer0StartY.setDefaultValue(272)
	Layer0StartY.setMin(0)
	Layer0StartY.setVisible(False)

	Layer0ColorMode = comp.createKeyValueSetSymbol("Layer0ColorMode", Layer0Enable)
	Layer0ColorMode.setLabel("Layer 0 Frame Buffer Color Mode")
	Layer0ColorMode.setOutputMode("Value")
	Layer0ColorMode.setDescription("Layer 0 Frame Buffer Color Mode")
	Layer0ColorMode.addKey("LUT_8", "GLCD_LAYER_COLOR_MODE_LUT8", "LUT Palette 8bpp")
	Layer0ColorMode.addKey("RGB_565", "GLCD_LAYER_COLOR_MODE_RGB565", "RGB565 16bpp")
	Layer0ColorMode.addKey("RGBA_8888", "GLCD_LAYER_COLOR_MODE_RGBA8888", "RGBA8888 32bpp")
	Layer0ColorMode.addKey("YUYV", "GLCD_LAYER_COLOR_MODE_YUYV", "YCbCr")
	Layer0ColorMode.setDefaultValue(2)
	Layer0ColorMode.setVisible(False)

	Layer0Buffer0 = comp.createStringSymbol("Layer0Buffer0", Layer0Enable)
	Layer0Buffer0.setLabel("Layer 0 Frame Buffer 0 Address")
	Layer0Buffer0.setDefaultValue("0xA8000000")
	Layer0Buffer0.setVisible(False)

	Layer0Buffer1 = comp.createStringSymbol("Layer0Buffer1", Layer0Enable)
	Layer0Buffer1.setLabel("Layer 0 Frame Buffer 1 Address")
	Layer0Buffer1.setDefaultValue("0xA8465000")
	Layer0Buffer1.setVisible(False)

	Layer1Enable = comp.createBooleanSymbol("Layer1Enable", EnableLayersMenu)
	Layer1Enable.setLabel("Layer 1")
	Layer1Enable.setDescription("Enables Layer 1")
	Layer1Enable.setDefaultValue(True)
	Layer1Enable.setDependencies(OnLayersEnabled, ["Layer1Enable"])

	Layer1Width = comp.createIntegerSymbol("Layer1Width", Layer1Enable)
	Layer1Width.setLabel("Layer 1 Width")
	Layer1Width.setDescription("Width of Layer 1 in pixels")
	Layer1Width.setDefaultValue(480)
	Layer1Width.setMin(0)
	Layer1Width.setVisible(False)

	Layer1Height = comp.createIntegerSymbol("Layer1Height", Layer1Enable)
	Layer1Height.setLabel("Layer 1 Height")
	Layer1Height.setDescription("Height of Layer 1 in pixels")
	Layer1Height.setDefaultValue(272)
	Layer1Height.setMin(0)
	Layer1Height.setVisible(False)

	Layer1StartX = comp.createIntegerSymbol("Layer1StartX", Layer1Enable)
	Layer1StartX.setLabel("Layer 1 Start X")
	Layer1StartX.setDescription("Upper Left X Coordinate of Layer 1 (Relative to full display resolution)")
	Layer1StartX.setDefaultValue(480)
	Layer1StartX.setMin(0)
	Layer1StartX.setVisible(False)

	Layer1StartY = comp.createIntegerSymbol("Layer1StartY", Layer1Enable)
	Layer1StartY.setLabel("Layer 1 Start Y")
	Layer1StartY.setDescription("Upper Left Y Coordinate of Layer 1 (Relative to full display resolution)")
	Layer1StartY.setDefaultValue(272)
	Layer1StartY.setMin(0)
	Layer1StartY.setVisible(False)

	Layer1ColorMode = comp.createKeyValueSetSymbol("Layer1ColorMode", Layer1Enable)
	Layer1ColorMode.setLabel("Layer 1 Frame Buffer Color Mode")
	Layer1ColorMode.setOutputMode("Value")
	Layer1ColorMode.setDescription("Layer 1 Frame Buffer Color Mode")
	Layer1ColorMode.addKey("LUT_8", "GLCD_LAYER_COLOR_MODE_LUT8", "LUT Palette 8bpp")
	Layer1ColorMode.addKey("RGB_565", "GLCD_LAYER_COLOR_MODE_RGB565", "RGB565 16bpp")
	Layer1ColorMode.addKey("RGBA_8888", "GLCD_LAYER_COLOR_MODE_RGBA8888", "RGBA8888 32bpp")
	Layer1ColorMode.addKey("YUYV", "GLCD_LAYER_COLOR_MODE_YUYV", "YCbCr")
	Layer1ColorMode.setDefaultValue(2)
	Layer1ColorMode.setVisible(False)

	Layer1Buffer0 = comp.createStringSymbol("Layer1Buffer0", Layer1Enable)
	Layer1Buffer0.setLabel("Layer 1 Frame Buffer 0 Address")
	Layer1Buffer0.setDefaultValue("0xA8177000")
	Layer1Buffer0.setVisible(False)

	Layer1Buffer1 = comp.createStringSymbol("Layer1Buffer1", Layer1Enable)
	Layer1Buffer1.setLabel("Layer 1 Frame Buffer 1 Address")
	Layer1Buffer1.setDefaultValue("0xA85DC000")
	Layer1Buffer1.setVisible(False)

	Layer2Enable = comp.createBooleanSymbol("Layer2Enable", EnableLayersMenu)
	Layer2Enable.setLabel("Layer 2")
	Layer2Enable.setDescription("Enables Layer 2")
	Layer2Enable.setDefaultValue(True)
	Layer2Enable.setDependencies(OnLayersEnabled, ["Layer2Enable"])

	Layer2Width = comp.createIntegerSymbol("Layer2Width", Layer2Enable)
	Layer2Width.setLabel("Layer 2 Width")
	Layer2Width.setDescription("Width of Layer 2 in pixels")
	Layer2Width.setDefaultValue(480)
	Layer2Width.setMin(0)
	Layer2Width.setVisible(False)

	Layer2Height = comp.createIntegerSymbol("Layer2Height", Layer2Enable)
	Layer2Height.setLabel("Layer 2 Height")
	Layer2Height.setDescription("Height of Layer 2 in pixels")
	Layer2Height.setDefaultValue(272)
	Layer2Height.setMin(0)
	Layer2Height.setVisible(False)

	Layer2StartX = comp.createIntegerSymbol("Layer2StartX", Layer2Enable)
	Layer2StartX.setLabel("Layer 2 Start X")
	Layer2StartX.setDescription("Upper Left X Coordinate of Layer 2 (Relative to full display resolution)")
	Layer2StartX.setDefaultValue(480)
	Layer2StartX.setMin(0)
	Layer2StartX.setVisible(False)

	Layer2StartY = comp.createIntegerSymbol("Layer2StartY", Layer2Enable)
	Layer2StartY.setLabel("Layer 2 Start Y")
	Layer2StartY.setDescription("Upper Left Y Coordinate of Layer 2 (Relative to full display resolution)")
	Layer2StartY.setDefaultValue(272)
	Layer2StartY.setMin(0)
	Layer2StartY.setVisible(False)

	Layer2ColorMode = comp.createKeyValueSetSymbol("Layer2ColorMode", Layer2Enable)
	Layer2ColorMode.setLabel("Layer 2 Frame Buffer Color Mode")
	Layer2ColorMode.setOutputMode("Value")
	Layer2ColorMode.setDescription("Layer 2 Frame Buffer Color Mode")
	Layer2ColorMode.addKey("LUT_8", "GLCD_LAYER_COLOR_MODE_LUT8", "LUT Palette 8bpp")
	Layer2ColorMode.addKey("RGB_565", "GLCD_LAYER_COLOR_MODE_RGB565", "RGB565 16bpp")
	Layer2ColorMode.addKey("RGBA_8888", "GLCD_LAYER_COLOR_MODE_RGBA8888", "RGBA8888 32bpp")
	Layer2ColorMode.addKey("YUYV", "GLCD_LAYER_COLOR_MODE_YUYV", "YCbCr")
	Layer2ColorMode.setDefaultValue(2)
	Layer2ColorMode.setVisible(False)

	Layer2Buffer0 = comp.createStringSymbol("Layer2Buffer0", Layer2Enable)
	Layer2Buffer0.setLabel("Layer 2 Frame Buffer 0 Address")
	Layer2Buffer0.setDefaultValue("0xA82EE000")
	Layer2Buffer0.setVisible(False)

	Layer2Buffer1 = comp.createStringSymbol("Layer2Buffer1", Layer2Enable)
	Layer2Buffer1.setLabel("Layer 2 Frame Buffer 1 Address")
	Layer2Buffer1.setDefaultValue("0xA8753000")
	Layer2Buffer1.setVisible(False)

	### Display Timing Settings
	DisplaySettingsMenu = comp.createMenuSymbol("DisplaySettingsMenu", None)
	DisplaySettingsMenu.setLabel("Display Settings")

	DisplayWidth = comp.createIntegerSymbol("DisplayWidth", DisplaySettingsMenu)
	DisplayWidth.setLabel("Display Width")
	DisplayWidth.setDescription("The width of the frame buffer in pixels.")
	DisplayWidth.setDefaultValue(480)
	DisplayWidth.setMin(1)

	DisplayHeight = comp.createIntegerSymbol("DisplayHeight", DisplaySettingsMenu)
	DisplayHeight.setLabel("Display Height")
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
	FrameBufferColorMode.addKey("LUT_8", "GS_8", "LUT Palette 8bpp")
	FrameBufferColorMode.addKey("RGB_565", "RGB_565", "RGB565 16bpp")
	FrameBufferColorMode.addKey("RGBA_8888", "RGBA_8888", "RGBA8888 32bpp")
	FrameBufferColorMode.addKey("YUYV", "YUYV", "YCbCr")
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
	
	VblankUpdate = comp.createBooleanSymbol("VblankUpdate", OtherSettingsMenu)
	VblankUpdate.setLabel("Update on Vblank")
	VblankUpdate.setDefaultValue(True)
	VblankUpdate.setVisible(True)
	VblankUpdate.setDescription("<html>Schedule register updates in vblank.</html>")
    
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
		Layer0Width = DisplayWidth.getValue();
		Layer0Height = DisplayHeight.getValue();
	if (Layer1Enable.getValue() == True):
		numLayers += 1
		Layer1Width = DisplayWidth.getValue();
		Layer1Height = DisplayHeight.getValue();
	if (Layer2Enable.getValue() == True):
		numLayers += 1
		Layer1Width = DisplayWidth.getValue();
		Layer1Height = DisplayHeight.getValue();
	TotalNumLayers.setValue(numLayers, 1)
	
	if (Database.getComponentByID("gfx_hal_le") is not None):
		DisplayWidth.setValue(Database.getSymbolValue("gfx_hal_le", "DisplayWidth"))
		DisplayHeight.setValue(Database.getSymbolValue("gfx_hal_le", "DisplayHeight"))
	
def onAttachmentConnected(source, target):
	print("dependency Connected = " + target["component"].getDisplayName())
	gfxCoreComponentTable = ["gfx_hal_le"]
	if (Database.getComponentByID("gfx_hal_le") is None):
		Database.activateComponents(gfxCoreComponentTable)
	updateDisplayManager(source["component"], target)

def OnCacheEnabled(cacheEnabled, event):
	cacheEnabled.getComponent().setSymbolValue("UseCachedFrameBuffer", event["value"] == True, 1)
	
def OnAdvancedLayerEnabled(layerEnabled, event):
	layerEnabled.getComponent().getSymbolByID("FrameBufferColorMode").setVisible(event["value"] == False)
	layerEnabled.getComponent().getSymbolByID("Layer0Width").setVisible(event["value"] == True)
	layerEnabled.getComponent().getSymbolByID("Layer0Height").setVisible(event["value"] == True)
	layerEnabled.getComponent().getSymbolByID("Layer0Buffer0").setVisible(event["value"] == True)
	layerEnabled.getComponent().getSymbolByID("Layer0Buffer1").setVisible(event["value"] == True)
	layerEnabled.getComponent().getSymbolByID("Layer0StartX").setVisible(event["value"] == True)
	layerEnabled.getComponent().getSymbolByID("Layer0StartY").setVisible(event["value"] == True)
	layerEnabled.getComponent().getSymbolByID("Layer0ColorMode").setVisible(event["value"] == True)
	layerEnabled.getComponent().getSymbolByID("Layer1Width").setVisible(event["value"] == True)
	layerEnabled.getComponent().getSymbolByID("Layer1Height").setVisible(event["value"] == True)
	layerEnabled.getComponent().getSymbolByID("Layer1Buffer0").setVisible(event["value"] == True)
	layerEnabled.getComponent().getSymbolByID("Layer1Buffer1").setVisible(event["value"] == True)
	layerEnabled.getComponent().getSymbolByID("Layer1StartX").setVisible(event["value"] == True)
	layerEnabled.getComponent().getSymbolByID("Layer1StartY").setVisible(event["value"] == True)
	layerEnabled.getComponent().getSymbolByID("Layer1ColorMode").setVisible(event["value"] == True)
	layerEnabled.getComponent().getSymbolByID("Layer2Width").setVisible(event["value"] == True)
	layerEnabled.getComponent().getSymbolByID("Layer2Height").setVisible(event["value"] == True)
	layerEnabled.getComponent().getSymbolByID("Layer2Buffer0").setVisible(event["value"] == True)
	layerEnabled.getComponent().getSymbolByID("Layer2Buffer1").setVisible(event["value"] == True)
	layerEnabled.getComponent().getSymbolByID("Layer2StartX").setVisible(event["value"] == True)
	layerEnabled.getComponent().getSymbolByID("Layer2StartY").setVisible(event["value"] == True)
	layerEnabled.getComponent().getSymbolByID("Layer2ColorMode").setVisible(event["value"] == True)
	
def OnLayersEnabled(layerEnabled, event):
	if event["id"] == "Layer0Enable":
		if event["value"] == True and Database.getComponentByID("gfx_hal_le") is not None:
			Database.setSymbolValue("le_gfx_driver_glcd", "Layer0Width", Database.getSymbolValue("gfx_hal_le", "DisplayWidth"))
			Database.setSymbolValue("le_gfx_driver_glcd", "Layer0Height", Database.getSymbolValue("gfx_hal_le", "DisplayHeight"))
			Database.setSymbolValue("le_gfx_driver_glcd", "Layer0StartX", 0)
			Database.setSymbolValue("le_gfx_driver_glcd", "Layer0StartY", 0)
			Database.setSymbolValue("le_gfx_driver_glcd", "Layer0ColorMode", Database.getSymbolValue("le_gfx_driver_glcd", "FrameBufferColorMode"))
	if event["id"] == "Layer1Enable":
		if event["value"] == True and Database.getComponentByID("gfx_hal_le") is not None:
			Database.setSymbolValue("le_gfx_driver_glcd", "Layer1Width", Database.getSymbolValue("gfx_hal_le", "DisplayWidth"))
			Database.setSymbolValue("le_gfx_driver_glcd", "Layer1Height", Database.getSymbolValue("gfx_hal_le", "DisplayHeight"))
			Database.setSymbolValue("le_gfx_driver_glcd", "Layer1StartX", 0)
			Database.setSymbolValue("le_gfx_driver_glcd", "Layer1StartY", 0)
			Database.setSymbolValue("le_gfx_driver_glcd", "Layer1ColorMode", Database.getSymbolValue("le_gfx_driver_glcd", "FrameBufferColorMode"))
	if event["id"] == "Layer2Enable":
		if event["value"] == True and Database.getComponentByID("gfx_hal_le") is not None:
			Database.setSymbolValue("le_gfx_driver_glcd", "Layer2Width", Database.getSymbolValue("gfx_hal_le", "DisplayWidth"))
			Database.setSymbolValue("le_gfx_driver_glcd", "Layer2Height", Database.getSymbolValue("gfx_hal_le", "DisplayHeight"))
			Database.setSymbolValue("le_gfx_driver_glcd", "Layer2StartX", 0)
			Database.setSymbolValue("le_gfx_driver_glcd", "Layer2StartY", 0)
			Database.setSymbolValue("le_gfx_driver_glcd", "Layer2ColorMode", Database.getSymbolValue("le_gfx_driver_glcd", "FrameBufferColorMode"))
	numLayers = 0;
	if layerEnabled.getComponent().getSymbolValue("Layer0Enable") == True:
		numLayers += 1;
	if layerEnabled.getComponent().getSymbolValue("Layer1Enable") == True:
		numLayers += 1;
	if layerEnabled.getComponent().getSymbolValue("Layer2Enable") == True:
		numLayers += 1;
	Database.setSymbolValue("le_gfx_driver_glcd", "TotalNumLayers", numLayers, 1)
	if (Database.getComponentByID("gfx_hal_le") is not None):
		Database.setSymbolValue("gfx_hal_le", "HardwareLayerCountHint", numLayers, 1)

def onPixelClockSet(pixelClockSet, event):
	if (Database.getComponentByID("gfx_hal_le") is not None):
		Database.setSymbolValue("gfx_hal_le", "PixelClock", event["value"], 1)

def onPixelDividerSet(pixelDividerSet, event):
	try:
		MasterClockValue = Database.getSymbolValue("core", "CONFIG_SYS_CLK_REFCLK5_FREQ")
	except:
		print(MasterClockSource.getValue() + " symbol not found")
	Database.setSymbolValue("le_gfx_driver_glcd", "MasterClockSourceValue", int(MasterClockValue))
	#### print("MasterClockValue = " + str(MasterClockValue))
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
