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

	execfile(Module.getPath() + "/config/files.py")
	execfile(Module.getPath() + "/config/config.py")

	comp.setDependencyEnabled("Parallel Display Interface", True);
	comp.setDependencyEnabled("SPI Display Interface", False);

def onAttachmentConnected(source, target):
	if source["id"] == "Display Interface":
		InterfaceType = str(target["component"].getSymbolByID("InterfaceType").getValue())
		if InterfaceType == "SPI 4-line":
			source["component"].getSymbolByID("DisplayInterfaceType").setValue("SPI 4-line", 1)
		elif "Parallel" in InterfaceType:
			source["component"].getSymbolByID("DisplayInterfaceType").setValue("Parallel 8080", 1)
	elif (source["id"] == "Graphics Display"):
		source["component"].setSymbolValue("DisplayWidth", target["component"].getSymbolByID("DisplayWidth").getValue())
		source["component"].setSymbolValue("DisplayHeight", target["component"].getSymbolByID("DisplayHeight").getValue())

def onAttachmentDisconnected(source, target):
	if source["id"] == "Display Interface":
		source["component"].clearSymbolValue("DisplayInterfaceType")
	elif (source["id"] == "Graphics Display"):
		source["component"].clearSymbolValue("DisplayWidth")
		source["component"].clearSymbolValue("DisplayHeight")

def showRTOSMenu(source, event):
	source.setVisible(event["value"] != "BareMetal")

def onInitCommandsCountChanged(source, event):
	global numCommands
	print("Init Commands Countchanged : " + str(event["value"]))
	for x in range(numCommands - 1):
		if (x < int(event["value"])):
			source.getComponent().getSymbolByID("Command" + str(x)).setVisible(True)
		else:
			source.getComponent().getSymbolByID("Command" + str(x)).setVisible(False)

def onInitCommandParmsCountChanged(source, event):
	global numParms
	print("Init Commands Countchanged : " + str(event["id"]))
	sub = re.search('Command(.*)ParmsCount', str(event["id"]))
	if (sub and sub.group(1)):
		print("sub is : " + str((sub.group(1))))
		for x in range(numParms - 1):
			if (x < int(event["value"])):
				source.getComponent().getSymbolByID("Command" + str(sub.group(1)) + "Parm" + str(x)).setVisible(True)
			else:
				source.getComponent().getSymbolByID("Command" + str(sub.group(1)) + "Parm" + str(x)).setVisible(False)

def onDataBusConfigChanged(source, event):
	DataWriteSize = source.getComponent().getSymbolByID("DataTxSize").getValue()
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
	source.getComponent().getSymbolByID("PixelDataSettingsMenu").setVisible(event["value"] == "Full")
	source.getComponent().getSymbolByID("FrameAddressSize").setVisible(event["value"] == "Full")
	source.getComponent().getSymbolByID("SetXAddressCommand").setVisible(event["value"] == "Full")
	source.getComponent().getSymbolByID("SetXAddressCommandParm0").setVisible(event["value"] == "Full")
	source.getComponent().getSymbolByID("SetXAddressCommandParm1").setVisible(event["value"] == "Full")
	source.getComponent().getSymbolByID("SetXAddressCommandParm2").setVisible(event["value"] == "Full")
	source.getComponent().getSymbolByID("SetXAddressCommandParm3").setVisible(event["value"] == "Full")
	source.getComponent().getSymbolByID("SetYAddressCommand").setVisible(event["value"] == "Full")
	source.getComponent().getSymbolByID("SetYAddressCommandParm0").setVisible(event["value"] == "Full")
	source.getComponent().getSymbolByID("SetYAddressCommandParm1").setVisible(event["value"] == "Full")
	source.getComponent().getSymbolByID("SetYAddressCommandParm2").setVisible(event["value"] == "Full")
	source.getComponent().getSymbolByID("SetYAddressCommandParm3").setVisible(event["value"] == "Full")
	source.getComponent().getSymbolByID("MemoryWriteCommand").setVisible(event["value"] == "Full")
	source.getComponent().getSymbolByID("MemoryReadCommand").setVisible(event["value"] == "Full")
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
	source.getComponent().setDependencyEnabled("Display Interface", event["value"] == False)

def onHALConnected(halConnected, event):
	print("halconnected")
	halConnected.getComponent().getSymbolByID("DisplaySettingsMenu").setVisible(event["value"] == False)

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
	comp.getSymbolByID("MemoryReadCommand").setValue(0x2e)
	
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
	comp.getSymbolByID("MemoryReadCommand").setValue(0x2e)

