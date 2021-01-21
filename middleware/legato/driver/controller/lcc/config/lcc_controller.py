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
	
	#comp.setHelpFile("../../../doc/html/help_harmony_gfx_html_alias.h")
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
	
	#PixelSupportLevel = comp.createStringSymbol("PixelSupportLevel")
	#PixelSupportLevel.setLabel("WQVGA or lower")
	#PixelSupportLevel.setReadOnly(True)
	#PixelSupportLevel.setDescription("The total number of pixels expected to be supportable.")
	
	#LCCRefresh = comp.createBooleanSymbol("LCCRefresh", None)
	#LCCRefresh.setLabel("Use Aggressive Refresh Strategy?")
	#LCCRefresh.setDescription("<html>Indicates that the LCC refresh loop should attempt<br>to aggresively refresh the display.  May cause<br>display artifacts but is needed for some larger displays.</html>")

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
	
	PixelClock = comp.createIntegerSymbol("PixelClock", DisplaySettingsMenu)
	PixelClock.setLabel("Pixel Clock (Hz)")
	PixelClock.setReadOnly(True)
	PixelClock.setDescription("The approximate pixel clock frequency generated by the LCC. Fixed value, no configuration options available.")
	PixelClock.setDefaultValue(6250000)
	
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

	#DoubleBuffer = comp.createBooleanSymbol("DoubleBuffer", FrameBufferSettingsMenu)
	#DoubleBuffer.setLabel("Use Double Buffering?")
	#DoubleBuffer.setDescription("<html>Uses an additional buffer for off-screen drawing.<br>Eliminates screen tearing but doubles the required memory.</html>")

	#PaletteMode = comp.createBooleanSymbol("PaletteMode", FrameBufferSettingsMenu)
	#PaletteMode.setLabel("Use 8-bit Palette?")
	#PaletteMode.setDescription("<html>Enables frame buffer compression.<br>Uses an 8-bit color lookup table to reduce the required<br>frame buffer memory size.  This also reduces the<br>maximum avilable color count to 256 and significantly<br>slows down display refresh speed.</html>")

	UseCachedFrameBuffer = comp.createBooleanSymbol("UseCachedFrameBuffer", FrameBufferSettingsMenu)
	UseCachedFrameBuffer.setLabel("Uses Cache?")
	UseCachedFrameBuffer.setDescription("Specifies if frame buffer is cached and needs to be managed by the LCC driver.")
	UseCachedFrameBuffer.setDefaultValue(True)
	UseCachedFrameBuffer.setDependencies(OnCacheEnabled, ["core.DATA_CACHE_ENABLE"])

	#FrameBufferMemory = comp.createComboSymbol("FrameBufferMemory", FrameBufferSettingsMenu, ["Internal SRAM", "External SDRAM"])
	FrameBufferMemory = comp.createComboSymbol("FrameBufferMemory", FrameBufferSettingsMenu, ["Internal SRAM"])
	FrameBufferMemory.setLabel("Memory Interface")
	FrameBufferMemory.setDescription("Memory used for Frame Buffer")
	FrameBufferMemory.setDefaultValue("Internal SRAM")
	
	DMAMenu = comp.createMenuSymbol("DMAMenu", None)
	DMAMenu.setLabel("DMA Settings")
	
	DMAChannel = comp.createIntegerSymbol("DMAChannel", DMAMenu)
	DMAChannel.setLabel("DMA Channel")
	DMAChannel.setDefaultValue(0)
	DMAChannel.setMin(0)
	DMAChannel.setMax(23)
	DMAChannel.setDependencies(onDMAChannelSet, ["DMAChannel"])

	OldDMAChannel = comp.createIntegerSymbol("OldDMAChannel", DMAMenu)
	OldDMAChannel.setLabel("Old DMA Channel")
	OldDMAChannel.setVisible(False)
	
	DMAChannelSelected = comp.createBooleanSymbol("DMAChannelSelected", DMAMenu)
	DMAChannelSelected.setDefaultValue(False)
	DMAChannelSelected.setVisible(False)
	
	# IntPriority = comp.createIntegerSymbol("IntPriority", DMAMenu)
	# IntPriority.setLabel("Interrupt Priority Level")
	# IntPriority.setMin(0)
	# IntPriority.setMax(7)

	EBIChipSelectIndex = comp.createIntegerSymbol("EBIChipSelectIndex", None)
	EBIChipSelectIndex.setLabel("EBI Chip Select Index")
	EBIChipSelectIndex.setDescription("The chip select index")
	EBIChipSelectIndex.setMin(0)
	EBIChipSelectIndex.setMax(4)
	EBIChipSelectIndex.setDefaultValue(0)
	EBIChipSelectIndex.setVisible(False)

	### Start of Backlight config options
	BacklightSettings = comp.createMenuSymbol("BacklightSettings", None)
	BacklightSettings.setLabel("Backlight Settings")

	DefaultBrightness = comp.createIntegerSymbol("DefaultBrightness", BacklightSettings)
	DefaultBrightness.setLabel("Default Brightness (%)")
	DefaultBrightness.setDescription("The default brightness setting at driver startup")
	DefaultBrightness.setMin(0)
	DefaultBrightness.setMax(100)
	DefaultBrightness.setDefaultValue(100)

	PeripheralControl = comp.createComboSymbol("PeripheralControl", BacklightSettings, ["GPIO", "TC"])
	PeripheralControl.setLabel("Peripheral")
	PeripheralControl.setDescription("Peripheral used to control the backlight PWM")
	PeripheralControl.setDefaultValue("GPIO")
	PeripheralControl.setDependencies(onBacklightPeripheralSelected, ["PeripheralControl"])
	
	TCPeripheralSettings = comp.createMenuSymbol("TCPeripheralSettings", PeripheralControl)
	TCPeripheralSettings.setLabel("Timer Counter Settings")
	TCPeripheralSettings.setDescription("Settings for using the TC peripheral library")
	TCPeripheralSettings.setVisible(False)

	TCInstance = comp.createIntegerSymbol("TCInstance", TCPeripheralSettings)
	TCInstance.setLabel("TC Instance")
	TCInstance.setDescription("The TC peripheral IDx")
	TCInstance.setMin(0)
	TCInstance.setMax(64)
	TCInstance.setDefaultValue(0)

	TCChannel = comp.createIntegerSymbol("TCChannel", TCPeripheralSettings)
	TCChannel.setLabel("TC Channel")
	TCChannel.setDescription("The TC channel that controls the PWM signal")
	TCChannel.setMin(0)
	TCChannel.setMax(64)
	TCChannel.setDefaultValue(0)

	TCChannelCompare = comp.createComboSymbol("TCChannelCompare", TCPeripheralSettings, ["A", "B"])
	TCChannelCompare.setLabel("Compare Register")
	TCChannelCompare.setDescription("Compare Register for PWM")
	TCChannelCompare.setDefaultValue("A")
	### End of Backlight config options

### Start of Blit config
	BlitSettings = comp.createMenuSymbol("BlitSettings", None)
	BlitSettings.setLabel("Buffer Blit Settings")

	BlitMode = comp.createComboSymbol("BlitMode", BlitSettings, ["CPU", "DMA"])
	BlitMode.setLabel("Blit Mode")
	BlitMode.setDescription("Peripheral used for scratch buffer blit.")
	BlitMode.setDefaultValue("CPU")
	BlitMode.setDependencies(onBlitModeSet, ["BlitMode"])

	DMABlitChannel = comp.createIntegerSymbol("DMABlitChannel", BlitMode)
	DMABlitChannel.setLabel("DMA Channel")
	DMABlitChannel.setDescription("DMA channel for DMA blit. Please enable and configure DMA separately")
	DMABlitChannel.setDefaultValue(1)
	DMABlitChannel.setMin(0)
	DMABlitChannel.setMax(15)
	DMABlitChannel.setVisible(False)
	
	BlitModeComment = comp.createCommentSymbol("BlitModeComment", BlitMode)
	BlitModeComment.setLabel('Please enable and configure selected DMAC channel')
	BlitModeComment.setVisible(False)

	# generated code files
	GFX_LCC_C = comp.createFileSymbol("GFX_LCC_C", None)
	GFX_LCC_C.setSourcePath("templates/drv_gfx_lcc.c.ftl")
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

	autoSelectDMAChannel(DMAChannelSelected, DMAChannel, OldDMAChannel)

def autoSelectDMAChannel(DMAChannelSelected, DMAChannel, OldDMAChannel):
	if DMAChannelSelected.getValue() == False:
		for channel in range(0, 23):
			enabled = Database.getComponentByID("core").getSymbolByID("XDMAC_CH" + str(channel) + "_ENABLE").getValue();
			if enabled == False:
				configureDMAChannel(channel)
				DMAChannelSelected.setValue(True, 1)
				DMAChannel.setValue(channel, 1)
				OldDMAChannel.setValue(channel, 1)
				break

def configureDMAChannel(channel):
	Database.getComponentByID("core").getSymbolByID("XDMAC_CH" + str(channel) + "_ENABLE").setValue(True, 1)
	Database.getComponentByID("core").getSymbolByID("XDMAC_CC" + str(channel) + "_DAM").setSelectedKey("FIXED_AM", 1)
	Database.getComponentByID("core").getSymbolByID("XDMAC_CC" + str(channel) + "_DWIDTH").setSelectedKey("HALFWORD", 1)
	Database.getComponentByID("core").getSymbolByID("XDMAC_CC" + str(channel) + "_SIF").setSelectedKey("AHB_IF0", 1)
	Database.getComponentByID("core").getSymbolByID("XDMAC_CC" + str(channel) + "_MBSIZE").setSelectedKey("SIXTEEN", 1)

def unconfigureDMAChannel(channel):
	Database.clearSymbolValue("core", "XDMAC_CH" + str(channel) + "_ENABLE")
	Database.clearSymbolValue("core", "XDMAC_CC" + str(channel) + "_DAM")
	Database.clearSymbolValue("core", "XDMAC_CC" + str(channel) + "_DWIDTH")
	Database.clearSymbolValue("core", "XDMAC_CC" + str(channel) + "_SIF")
	Database.clearSymbolValue("core", "XDMAC_CC" + str(channel) + "_MBSIZE")

def onDMAChannelSet(dmaChannelSet, event):
	newDMAChannel = dmaChannelSet.getComponent().getSymbolByID("DMAChannel").getValue()
	oldDMAChannel = dmaChannelSet.getComponent().getSymbolByID("OldDMAChannel").getValue()
	unconfigureDMAChannel(str(oldDMAChannel))
	configureDMAChannel(str(newDMAChannel))
	dmaChannelSet.getComponent().getSymbolByID("OldDMAChannel").setValue(newDMAChannel, 1)

def configureSMCComponent(lccComponent, smcComponent, smcChipSelNum):
	print("LCC: Connecting SMC_CS" + str(smcChipSelNum))
	smcComponent.setSymbolValue("SMC_CHIP_SELECT" + str(smcChipSelNum), True, 1)
	smcComponent.setSymbolValue("SMC_MEM_SCRAMBLING_CS" + str(smcChipSelNum), False, 1)
	smcComponent.setSymbolValue("SMC_NWE_SETUP_CS" + str(smcChipSelNum), 5, 1)
	smcComponent.setSymbolValue("SMC_NCS_WR_SETUP_CS" + str(smcChipSelNum), 0, 1)
	smcComponent.setSymbolValue("SMC_NWE_PULSE_CS" + str(smcChipSelNum), 2, 1)
	smcComponent.setSymbolValue("SMC_NWE_CYCLE_CS" + str(smcChipSelNum), 6, 1)
	smcComponent.setSymbolValue("SMC_NCS_WR_PULSE_CS" + str(smcChipSelNum), 6, 1)
	smcComponent.setSymbolValue("SMC_DATA_BUS_CS" + str(smcChipSelNum), 1, 1)
	smcComponent.setSymbolValue("SMC_WRITE_ENABLE_MODE_CS" + str(smcChipSelNum), False, 1)
	lccComponent.setSymbolValue("EBIChipSelectIndex", smcChipSelNum, 1)
	
def resetSMCComponent(lccComponent, smcComponent, smcChipSelNum):
	print("LCC: Disconnecting SMC_CS" + str(smcChipSelNum))
	smcComponent.clearSymbolValue("SMC_CHIP_SELECT" + str(smcChipSelNum))
	smcComponent.clearSymbolValue("SMC_MEM_SCRAMBLING_CS" + str(smcChipSelNum))
	smcComponent.clearSymbolValue("SMC_NWE_SETUP_CS" + str(smcChipSelNum))
	smcComponent.clearSymbolValue("SMC_NCS_WR_SETUP_CS" + str(smcChipSelNum))
	smcComponent.clearSymbolValue("SMC_NWE_PULSE_CS" + str(smcChipSelNum))
	smcComponent.clearSymbolValue("SMC_NWE_CYCLE_CS" + str(smcChipSelNum))
	smcComponent.clearSymbolValue("SMC_NCS_WR_PULSE_CS" + str(smcChipSelNum))
	smcComponent.clearSymbolValue("SMC_DATA_BUS_CS" + str(smcChipSelNum))
	smcComponent.clearSymbolValue("SMC_WRITE_ENABLE_MODE_CS" + str(smcChipSelNum))
	lccComponent.clearSymbolValue("EBIChipSelectIndex")

def configureDisplayTiming(lccComponent, displayComponent):
	lccComponent.setSymbolValue("DisplayHorzPulseWidth", displayComponent.getSymbolByID("HorzPulseWidth").getValue())
	lccComponent.setSymbolValue("DisplayHorzBackPorch", displayComponent.getSymbolByID("HorzBackPorch").getValue())
	lccComponent.setSymbolValue("DisplayHorzFrontPorch", displayComponent.getSymbolByID("HorzFrontPorch").getValue())
	lccComponent.setSymbolValue("DisplayVertPulseWidth", displayComponent.getSymbolByID("VertPulseWidth").getValue())
	lccComponent.setSymbolValue("DisplayVertBackPorch", displayComponent.getSymbolByID("VertBackPorch").getValue())
	lccComponent.setSymbolValue("DisplayVertFrontPorch", displayComponent.getSymbolByID("VertFrontPorch").getValue())
	if (Database.getComponentByID("gfx_hal_le") is not None):
		Database.setSymbolValue("gfx_hal_le", "gfx_display", displayComponent.getDisplayName(), 1)
	
def resetDisplayTiming(lccComponent, displayComponent):
	lccComponent.clearSymbolValue("DisplayHorzPulseWidth")
	lccComponent.clearSymbolValue("DisplayHorzBackPorch")
	lccComponent.clearSymbolValue("DisplayHorzFrontPorch")
	lccComponent.clearSymbolValue("DisplayVertPulseWidth")
	lccComponent.clearSymbolValue("DisplayVertBackPorch")
	lccComponent.clearSymbolValue("DisplayVertFrontPorch")

def onAttachmentConnected(source, target):
	print("dependency Connected = " + target["component"].getDisplayName())
	gfxCoreComponentTable = ["gfx_hal_le"]
	if (Database.getComponentByID("gfx_hal_le") is None):
		Database.activateComponents(gfxCoreComponentTable)
	updateDisplayManager(source["component"], target)
	#### test for SMC dependency
	if (source["id"] == "SMC_CS"):
		sub = re.search('smc_cs(.*)', str(target["id"]))
		if (sub and sub.group(1)):
			configureSMCComponent(source["component"], target["component"], int(sub.group(1)))
	#### test for TC dependency (backlight)
	elif (source["id"] == "TMR"):
		sub = re.search('TC(.*)', target["component"].getDisplayName())
		if (sub and sub.group(1)):
			source["component"].getSymbolByID("TCInstance").setValue(int(sub.group(1)), 1)
	elif (source["id"] == "Graphics Display"):
		#configure timing
		configureDisplayTiming(source["component"], target["component"])

	
def onAttachmentDisconnected(source, target):
	if (source["id"] == "SMC_CS"):
		sub = re.search('smc_cs(.*)', str(target["id"]))
		if (sub and sub.group(1)):
			resetSMCComponent(source["component"], target["component"], int(sub.group(1)))
	elif (source["id"] == "Graphics Display"):
		#reset timing
		resetDisplayTiming(source["component"], target["component"])

def OnCacheEnabled(cacheEnabled, event):
	print("LCC: cache enabled")
	cacheEnabled.getComponent().setSymbolValue("UseCachedFrameBuffer", event["value"] == True, 1)
	print("LCC: UseCachedFrameBuffer enabled")

def onBacklightPeripheralSelected(symbol, event):
	print("onBacklightPeripheralSelected = " + event["value"])
	if (event["value"] == "TC"):
		symbol.getComponent().setDependencyEnabled("TMR", True)
		symbol.getComponent().getSymbolByID("TCPeripheralSettings").setVisible(True)
	else:
		symbol.getComponent().getSymbolByID("TCPeripheralSettings").setVisible(False)
		symbol.getComponent().setDependencyEnabled("TMR", False)

def showRTOSMenu(symbol, event):
	symbol.setVisible(event["value"] != "BareMetal")
   
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

def onBlitModeSet(symbol, event):
	symbol.getComponent().getSymbolByID("DMABlitChannel").setVisible(event["value"] == "DMA")
	symbol.getComponent().getSymbolByID("BlitModeComment").setVisible(event["value"] == "DMA")
