numCommands = 32
defaultNumCommands = 3
numParms = 16
defaultNumParms = 0

import re

def instantiateComponent(comp):
	global numCommands
	global defaultNumCommands
	global numArgs
	global defaultNumArgs

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
	
	execfile(Module.getPath() + "/config/config.py")
	execfile(Module.getPath() + "/config/files.py")
	execfile(Module.getPath() + "/config/rtos.py")

	comp.setDependencyEnabled("Parallel Display Interface", True);
	comp.setDependencyEnabled("SPI Display Interface", False);

def onAttachmentConnected(source, target):
	print("dependency Connected = " + target["component"].getDisplayName())
	gfxCoreComponentTable = ["gfx_hal_le"]
	if (Database.getComponentByID("gfx_hal_le") is None):
		Database.activateComponents(gfxCoreComponentTable)
	updateDisplayManager(source["component"], target)
	if source["id"] == "Display Interface":
		print("Display Interface Connected")
	elif (source["id"] == "Graphics Display"):
		source["component"].setSymbolValue("DisplayWidth", target["component"].getSymbolByID("DisplayWidth").getValue())
		source["component"].setSymbolValue("DisplayHeight", target["component"].getSymbolByID("DisplayHeight").getValue())

def onAttachmentDisconnected(source, target):
	if source["id"] == "Display Interface":
		print("Display Interface disconnected")
	elif (source["id"] == "Graphics Display"):
		source["component"].clearSymbolValue("DisplayWidth")
		source["component"].clearSymbolValue("DisplayHeight")

def showRTOSMenu(source, event):
	source.setVisible(event["value"] != "BareMetal")
	
def onDisplayInterfaceTypeChanged(source, event):
	source.getComponent().setDependencyEnabled("Parallel Display Interface", event["value"] == "Parallel 8080")
	source.getComponent().setDependencyEnabled("SPI Display Interface", event["value"] == "SPI 4-line")

def onInitCommandsCountChanged(source, event):
	global numCommands
	print("Init Commands Countchanged : " + str(event["value"]))
	for x in range(numCommands):
		if (x < int(event["value"])):
			print("setvisible command " + str(x))
			source.getComponent().getSymbolByID("Command" + str(x)).setVisible(True)
		else:
			print("hide command " + str(x))
			source.getComponent().getSymbolByID("Command" + str(x)).setVisible(False)

def onInitCommandParmsCountChanged(source, event):
	global numParms
	print("Init Commands Countchanged : " + str(event["id"]))
	sub = re.search('Command(.*)ParmsCount', str(event["id"]))
	if (sub and sub.group(1)):
		print("sub is : " + str((sub.group(1))))
		for x in range(numParms):
			if (x < int(event["value"])):
				source.getComponent().getSymbolByID("Command" + str(sub.group(1)) + "Parm" + str(x)).setVisible(True)
			else:
				source.getComponent().getSymbolByID("Command" + str(sub.group(1)) + "Parm" + str(x)).setVisible(False)

def onDataBusConfigChanged(source, event):
	DataWriteSize = source.getComponent().getSymbolByID("DataWriteSize").getValue()
	source.getComponent().getSymbolByID("PixelDataTxSize8Bit").setVisible(False)
	source.getComponent().getSymbolByID("PixelDataInfoString16bit").setVisible(False)
	source.getComponent().getSymbolByID("PixelDataInfoString8bit2byteNorm").setVisible(False)
	source.getComponent().getSymbolByID("PixelDataInfoString8bit2byteLE").setVisible(False)
	source.getComponent().getSymbolByID("PixelDataInfoString8bit3byte").setVisible(False)
	if (DataWriteSize == "16"):
		source.getComponent().getSymbolByID("PixelDataInfoString16bit").setVisible(True)
	else:
		source.getComponent().getSymbolByID("PixelDataTxSize8Bit").setVisible(True)
		PixelDataTxSize8Bit = source.getComponent().getSymbolByID("PixelDataTxSize8Bit").getValue()
		if (PixelDataTxSize8Bit == "2 (Normal)"):
			source.getComponent().getSymbolByID("PixelDataInfoString8bit2byteNorm").setVisible(True)
		elif (PixelDataTxSize8Bit == "2 (Little-Endian)"):
			source.getComponent().getSymbolByID("PixelDataInfoString8bit2byteLE").setVisible(True)
		else:
			source.getComponent().getSymbolByID("PixelDataInfoString8bit3byte").setVisible(True)

def onBlitBufferFunctionGenerateModeChanged(source, event):
	source.getComponent().getSymbolByID("FrameAddressSize").setVisible(event["value"] == "Use Bulk Write")
	source.getComponent().getSymbolByID("SetXAddressCommand").setVisible(event["value"] == "Use Bulk Write")
	source.getComponent().getSymbolByID("SetXAddressCommandParm0").setVisible(event["value"] == "Use Bulk Write")
	source.getComponent().getSymbolByID("SetXAddressCommandParm1").setVisible(event["value"] == "Use Bulk Write")
	source.getComponent().getSymbolByID("SetXAddressCommandParm2").setVisible(event["value"] == "Use Bulk Write")
	source.getComponent().getSymbolByID("SetXAddressCommandParm3").setVisible(event["value"] == "Use Bulk Write")
	source.getComponent().getSymbolByID("SetYAddressCommand").setVisible(event["value"] == "Use Bulk Write")
	source.getComponent().getSymbolByID("SetYAddressCommandParm0").setVisible(event["value"] == "Use Bulk Write")
	source.getComponent().getSymbolByID("SetYAddressCommandParm1").setVisible(event["value"] == "Use Bulk Write")
	source.getComponent().getSymbolByID("SetYAddressCommandParm2").setVisible(event["value"] == "Use Bulk Write")
	source.getComponent().getSymbolByID("SetYAddressCommandParm3").setVisible(event["value"] == "Use Bulk Write")
	source.getComponent().getSymbolByID("MemoryWriteCommand").setVisible(event["value"] == "Use Bulk Write")
	source.getComponent().getSymbolByID("StubGenerateModeComment").setVisible(event["value"] == "Stub")


def onFrameAddressSizeChanged(source, event):
	source.getComponent().getSymbolByID("SetXAddressParmMenuWord").setVisible(event["value"] == "2-bytes")
	source.getComponent().getSymbolByID("SetYAddressParmMenuWord").setVisible(event["value"] == "2-bytes")
	source.getComponent().getSymbolByID("SetXAddressParmMenuByte").setVisible(event["value"] == "1-byte")
	source.getComponent().getSymbolByID("SetYAddressParmMenuByte").setVisible(event["value"] == "1-byte")
	
def onBaseDriverTypeChanged(source, event):
	source.getComponent().getSymbolByID("ControllerName").setValue(event["value"])
	if (event["value"] == "ILI9488"):
		configureBaseDriverILI9488(source.getComponent())
	elif (event["value"] == "SSD1963"):
		configureBaseDriverSSD1963(source.getComponent())

def onPassiveDriverChanged(source, event):
	source.getComponent().getSymbolByID("BlitBufferFunctionSettings").setVisible(event["value"] == False)
	source.getComponent().setCapabilityEnabled("gfx_driver_external", event["value"] == False)
	source.getComponent().setDependencyEnabled("legato_library", event["value"] == False)
	source.getComponent().setDependencyEnabled("Graphics Display", event["value"] == False)

def configureBaseDriverILI9488(comp):
	comp.getSymbolByID("PostResetDelay").setValue(30)
	comp.getSymbolByID("InitCommandsCount").setValue(6)
	
	comp.getSymbolByID("Command0Comment").setValue("Pixel Format Set")
	comp.getSymbolByID("Command0").setValue(0x3a)
	comp.getSymbolByID("Command0ParmsCount").setValue(1)
	comp.getSymbolByID("Command0Parm0").setValue(0x5)
	
	comp.getSymbolByID("Command1Comment").setValue("Set Image Function")
	comp.getSymbolByID("Command1").setValue(0xe9)
	comp.getSymbolByID("Command1ParmsCount").setValue(1)
	comp.getSymbolByID("Command1Parm0").setValue(0x1)
	
	comp.getSymbolByID("Command2Comment").setValue("Interface Mode Control")
	comp.getSymbolByID("Command2").setValue(0xb0)
	comp.getSymbolByID("Command2ParmsCount").setValue(1)
	comp.getSymbolByID("Command2Parm0").setValue(0)
	
	comp.getSymbolByID("Command3Comment").setValue("Memory Access Control")
	comp.getSymbolByID("Command3").setValue(0x36)
	comp.getSymbolByID("Command3ParmsCount").setValue(1)
	comp.getSymbolByID("Command3Parm0").setValue(0x48)
	
	comp.getSymbolByID("Command4Comment").setValue("Sleep Out")
	comp.getSymbolByID("Command4").setValue(0x11)
	comp.getSymbolByID("Command4ParmsCount").setValue(0)
	
	comp.getSymbolByID("Command5Comment").setValue("Display On")
	comp.getSymbolByID("Command5").setValue(0x29)
	comp.getSymbolByID("Command5ParmsCount").setValue(0)

	comp.getSymbolByID("SetXAddressCommand").setValue(0x2a)
	comp.getSymbolByID("SetYAddressCommand").setValue(0x2b)
	comp.getSymbolByID("MemoryWriteCommand").setValue(0x2c)
	
def configureBaseDriverSSD1963(comp):
	comp.getSymbolByID("PostResetDelay").setValue(30)
	comp.getSymbolByID("InitCommandsCount").setValue(12)

	comp.getSymbolByID("Command0Comment").setValue("Set PLL Config")
	comp.getSymbolByID("Command0").setValue(0xE2)
	comp.getSymbolByID("Command0Delay").setValue(10)
	comp.getSymbolByID("Command0ParmsCount").setValue(3)
	comp.getSymbolByID("Command0Parm0").setValue(0x1d)
	comp.getSymbolByID("Command0Parm1").setValue(0x2)
	comp.getSymbolByID("Command0Parm2").setValue(0x54)

	comp.getSymbolByID("Command1Comment").setValue("Enable PLL")
	comp.getSymbolByID("Command1").setValue(0xE0)
	comp.getSymbolByID("Command1Delay").setValue(10)
	comp.getSymbolByID("Command1ParmsCount").setValue(1)
	comp.getSymbolByID("Command1Parm0").setValue(0x1)

	comp.getSymbolByID("Command2Comment").setValue("Use PLL as System Clock")
	comp.getSymbolByID("Command2").setValue(0xE0)
	comp.getSymbolByID("Command2Delay").setValue(10)
	comp.getSymbolByID("Command2ParmsCount").setValue(1)
	comp.getSymbolByID("Command2Parm0").setValue(0x3)

	comp.getSymbolByID("Command3Comment").setValue("Software Reset")
	comp.getSymbolByID("Command3").setValue(0x1)
	comp.getSymbolByID("Command3Delay").setValue(10)
	comp.getSymbolByID("Command3ParmsCount").setValue(0)

	comp.getSymbolByID("Command4Comment").setValue("Set Pixel Clock to 15MHz")
	comp.getSymbolByID("Command4").setValue(0xE6)
	comp.getSymbolByID("Command4ParmsCount").setValue(3)
	comp.getSymbolByID("Command4Parm0").setValue(0x2)
	comp.getSymbolByID("Command4Parm1").setValue(0x66)
	comp.getSymbolByID("Command4Parm2").setValue(0x64)

	comp.getSymbolByID("Command4Comment").setValue("Set Pixel Clock to 15MHz")
	comp.getSymbolByID("Command4").setValue(0xE6)
	comp.getSymbolByID("Command4ParmsCount").setValue(3)
	comp.getSymbolByID("Command4Parm0").setValue(0x2)
	comp.getSymbolByID("Command4Parm1").setValue(0x66)
	comp.getSymbolByID("Command4Parm2").setValue(0x64)

	comp.getSymbolByID("Command5Comment").setValue("Set Panel Mode, 480x272, RGB")
	comp.getSymbolByID("Command5").setValue(0xB0)
	comp.getSymbolByID("Command5ParmsCount").setValue(7)
	comp.getSymbolByID("Command5Parm0").setValue(0x20)
	comp.getSymbolByID("Command5Parm1").setValue(0x0)
	comp.getSymbolByID("Command5Parm2").setValue(0x1)
	comp.getSymbolByID("Command5Parm3").setValue(0xDF)
	comp.getSymbolByID("Command5Parm4").setValue(0x1)
	comp.getSymbolByID("Command5Parm5").setValue(0xF)
	comp.getSymbolByID("Command5Parm6").setValue(0x0)

	comp.getSymbolByID("Command6Comment").setValue("Set Horizontal Period")
	comp.getSymbolByID("Command6").setValue(0xB4)
	comp.getSymbolByID("Command6ParmsCount").setValue(8)
	comp.getSymbolByID("Command6Parm0").setValue(0x2)
	comp.getSymbolByID("Command6Parm1").setValue(0xC)
	comp.getSymbolByID("Command6Parm2").setValue(0x0)
	comp.getSymbolByID("Command6Parm3").setValue(0x2A)
	comp.getSymbolByID("Command6Parm4").setValue(0x28)
	comp.getSymbolByID("Command6Parm5").setValue(0x0)
	comp.getSymbolByID("Command6Parm6").setValue(0x0)
	comp.getSymbolByID("Command6Parm7").setValue(0x0)

	comp.getSymbolByID("Command7Comment").setValue("Set Vertical Period")
	comp.getSymbolByID("Command7").setValue(0xB6)
	comp.getSymbolByID("Command7ParmsCount").setValue(8)
	comp.getSymbolByID("Command7Parm0").setValue(0x1)
	comp.getSymbolByID("Command7Parm1").setValue(0x1d)
	comp.getSymbolByID("Command7Parm2").setValue(0x0)
	comp.getSymbolByID("Command7Parm3").setValue(0xb)
	comp.getSymbolByID("Command7Parm4").setValue(0x9)
	comp.getSymbolByID("Command7Parm5").setValue(0x0)
	comp.getSymbolByID("Command7Parm6").setValue(0x0)
	comp.getSymbolByID("Command7Parm7").setValue(0x0)

	comp.getSymbolByID("Command8Comment").setValue("Set Pixel Format RGB565 (16bpp)")
	comp.getSymbolByID("Command8").setValue(0x3a)
	comp.getSymbolByID("Command8ParmsCount").setValue(1)
	comp.getSymbolByID("Command8Parm0").setValue(0x55)

	comp.getSymbolByID("Command9Comment").setValue("Set Data Interface")
	comp.getSymbolByID("Command9").setValue(0xF0)
	comp.getSymbolByID("Command9ParmsCount").setValue(1)
	comp.getSymbolByID("Command9Parm0").setValue(0x3)

	comp.getSymbolByID("Command10Comment").setValue("Turn on backlight")
	comp.getSymbolByID("Command10").setValue(0xBe)
	comp.getSymbolByID("Command10ParmsCount").setValue(5)
	comp.getSymbolByID("Command10Parm0").setValue(0x6)
	comp.getSymbolByID("Command10Parm1").setValue(0xff)
	comp.getSymbolByID("Command10Parm2").setValue(0x1)
	comp.getSymbolByID("Command10Parm3").setValue(0x0)
	comp.getSymbolByID("Command10Parm4").setValue(0x0)

	comp.getSymbolByID("Command11Comment").setValue("Turn on the display")
	comp.getSymbolByID("Command11").setValue(0x29)
	comp.getSymbolByID("Command11ParmsCount").setValue(0)
	
	comp.getSymbolByID("SetXAddressCommand").setValue(0x2a)
	comp.getSymbolByID("SetYAddressCommand").setValue(0x2b)
	comp.getSymbolByID("MemoryWriteCommand").setValue(0x2c)

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

def onBlitTypeChanged(source, event):
	source.getComponent().getSymbolByID("GFX_EXTERNAL_CONTROLLER_C").setEnabled(event['value'] == "Synchronous")
	source.getComponent().getSymbolByID("GFX_EXTERNAL_CONTROLLER_ASYNC_C").setEnabled(event['value'] != "Synchronous")


