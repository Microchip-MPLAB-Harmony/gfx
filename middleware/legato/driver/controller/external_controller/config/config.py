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

DriverInitName = comp.createStringSymbol("DriverInitName", None)
DriverInitName.setLabel("Driver Name")
DriverInitName.setDefaultValue("ExternalController")
DriverInitName.setVisible(False)

BaseDriverType = comp.createComboSymbol("BaseDriverType", None, ["Custom", "ILI9488", "SSD1963"])
BaseDriverType.setLabel("Base Driver")
BaseDriverType.setDefaultValue("Custom")
BaseDriverType.setDependencies(onBaseDriverTypeChanged, ["BaseDriverType"])
BaseDriverType.setDescription("<html>Set to customize a driver or use an ILI9488 or SSD1963 driver as base.<br>"
							"The ILI9488 base driver targets the ILI9488 controller on the 320x480 maXTouch Curiosity Pro display.<br>"
							"The SSD1963 base driver targets an SSD1963 driving a PDA 4.3in TM4301b WQVGA </html>")

ControllerName = comp.createStringSymbol("ControllerName", BaseDriverType)
ControllerName.setLabel("Controller")
ControllerName.setDefaultValue("ExternalController")
ControllerName.setDescription("<html>Set to target controller name. This will be used as part of the function names in the generated code.</html>")

DisplayInterfaceType = comp.createComboSymbol("DisplayInterfaceType", None, ["Parallel 8080", "SPI 4-line"])
DisplayInterfaceType.setLabel("Display Interface Type")
DisplayInterfaceType.setDefaultValue("Parallel 8080")
DisplayInterfaceType.setDependencies(onDisplayInterfaceTypeChanged, ["DisplayInterfaceType"])

PassiveDriver = comp.createBooleanSymbol("PassiveDriver", None)
PassiveDriver.setLabel("Passive Driver?")
PassiveDriver.setDefaultValue(False)
PassiveDriver.setDescription("<html> Set to generate a passive driver. A passive driver will only send the initialization commands.<br>"
							"It will not attach to the graphics library and will not write pixel data to the display controller. </html>")
PassiveDriver.setDependencies(onPassiveDriverChanged, ["PassiveDriver"])

DisplaySettingsMenu = comp.createMenuSymbol("DisplaySettingsMenu", None)
DisplaySettingsMenu.setLabel("Display Settings")

DisplayWidth = comp.createIntegerSymbol("DisplayWidth", DisplaySettingsMenu)
DisplayWidth.setLabel("Width")
DisplayWidth.setDescription("The width of the frame buffer in pixels.")
DisplayWidth.setMin(1)
DisplayWidth.setDefaultValue(480)

DisplayHeight = comp.createIntegerSymbol("DisplayHeight", DisplaySettingsMenu)
DisplayHeight.setLabel("Height")
DisplayHeight.setDescription("The height of the frame buffer in pixels.")
DisplayWidth.setMin(1)
DisplayHeight.setDefaultValue(272)

ResetMenu = comp.createMenuSymbol("ResetMenu", None)
ResetMenu.setLabel("Reset Settings")

ResetEnable = comp.createBooleanSymbol("ResetEnable", ResetMenu)
ResetEnable.setLabel("Enable Reset?")
ResetEnable.setDefaultValue(True)
ResetEnable.setDescription("<html> Enabling Reset will generate code that will toggle (active low) the reset pin to the device.<br>"
						"The reset pin must be defined in the Pin Settings as GFX_DISP_INTF_PIN_RESET</html>")

ResetAssertDuration = comp.createIntegerSymbol("ResetAssertDuration", ResetMenu)
ResetAssertDuration.setLabel("Reset Duration (ms)")
ResetAssertDuration.setDefaultValue(10)
ResetAssertDuration.setDescription("<html> The duration of the active low reset signal.</html>")

PostResetDelay = comp.createIntegerSymbol("PostResetDelay", ResetMenu)
PostResetDelay.setLabel("Post Reset Delay(ms)")
PostResetDelay.setDefaultValue(10)
PostResetDelay.setDescription("<html> The delay added after the reset signal is pulled high (deasserted). <br>"
							"Set this to create a delay between controller reset and initialization. </html>")

IntializationMenu = comp.createMenuSymbol("IntializationMenu", None)
IntializationMenu.setLabel("Initialization Settings")

MaXNumCommands = comp.createIntegerSymbol("MaXNumCommands", IntializationMenu)
MaXNumCommands.setLabel("Max Number of Commands")
MaXNumCommands.setDefaultValue(numCommands)
MaXNumCommands.setReadOnly(True)
MaXNumCommands.setDescription("<html> Maximum number (" + str(numCommands) + ") of initialization commands. <br>"
							"If more commands are needed, set 'numCommands' in gfx\middleware\legato\driver\controller\external_controller\config\controller.py. <br>"
							"More commands will cause component to load longer in MHC. </html>")

MaXNumParms = comp.createIntegerSymbol("MaXNumParms", IntializationMenu)
MaXNumParms.setLabel("Max Number of Parameters")
MaXNumParms.setDefaultValue(numParms)
MaXNumParms.setReadOnly(True)
MaXNumParms.setDescription("<html> Maximum number (" + str(numParms) + ") of parameters per command. <br>"
							"If more parameters are needed, set 'numParms' in gfx\middleware\legato\driver\controller\external_controller\config\controller.py. <br>"
							"More parameters will cause component to load longer in MHC. </html>")

InitCommandsCount = comp.createIntegerSymbol("InitCommandsCount", IntializationMenu)
InitCommandsCount.setLabel("Number of Init Commands")
InitCommandsCount.setDefaultValue(defaultNumCommands)
InitCommandsCount.setMax(numCommands)
InitCommandsCount.setMin(0)
InitCommandsCount.setDependencies(onInitCommandsCountChanged, ["InitCommandsCount"])
InitCommandsCount.setDescription("<html> The number of initialization commands to generate. </html>")

CommandList = comp.createListSymbol("CommandList", None)

for x in range(numCommands):
	Command = comp.createHexSymbol("Command" + str(x), InitCommandsCount)
	Command.setLabel("CMD[" + str(x) + "]")
	Command.setMax(0xff)
	Command.setMin(0x0)
	Command.setDescription("<html> Value of command in hex. </html>")
	
	CommandComment = comp.createStringSymbol("Command" + str(x) + "Comment", Command)
	CommandComment.setLabel("Comment")
	CommandComment.setDescription("<html> Description of command. Will be added as a code comment above the command. </html>")
	
	CommandDelay = comp.createIntegerSymbol("Command" + str(x) + "Delay", Command)
	CommandDelay.setLabel("Delay (ms)")
	CommandDelay.setMin(0)
	CommandDelay.setDefaultValue(0)
	CommandDelay.setDescription("<html> Delay before next command. </html>")
	
	CommandParmsCount = comp.createIntegerSymbol("Command" + str(x) + "ParmsCount", Command)
	CommandParmsCount.setLabel("Number of Parameters")
	CommandParmsCount.setDefaultValue(defaultNumParms)
	CommandParmsCount.setMax(numParms)
	CommandParmsCount.setMin(0)
	CommandParmsCount.setDependencies(onInitCommandParmsCountChanged, ["Command" + str(x) + "ParmsCount"])
	CommandParmsCount.setDescription("<html> Number of parameters for command. </html>")

	for y in range(numParms):
		Parm = comp.createHexSymbol("Command" + str(x) + "Parm" + str(y), CommandParmsCount)
		Parm.setLabel("PARM[" + str(y) + "]")
		Parm.setMax(0xff)
		Parm.setMin(0)
		Parm.setDescription("<html> Value of parameter in hex. </html>")
		if (y > defaultNumParms - 1):
			Parm.setVisible(False)
	if (x > defaultNumCommands - 1):
		Command.setVisible(False)

BlitBufferFunctionSettings = comp.createMenuSymbol("BlitBufferFunctionSettings", None)
BlitBufferFunctionSettings.setLabel("Blit Buffer Function Settings")

BlitBufferFunctionGenerateMode = comp.createComboSymbol("BlitBufferFunctionGenerateMode", BlitBufferFunctionSettings, ["Use Bulk Write", "Stub"])
BlitBufferFunctionGenerateMode.setLabel("Blit Buffer Function Generation Mode")
BlitBufferFunctionGenerateMode.setDependencies(onBlitBufferFunctionGenerateModeChanged, ["BlitBufferFunctionGenerateMode"])
BlitBufferFunctionGenerateMode.setDescription("<html> Set to generate a blit buffer function. This function is called by the library to write<br>"
								"a subsection of the frame to the display controller.<br>"
								"'Use Bulk Write' will generate a function that sets the UL and LR points of the subframe, then write the pixel data in burst.<br>"
								"'Stub' will generate an empty blit function that will need to be defined based on the controller's memory write operation. </html>")
								
BlitType = comp.createComboSymbol("BlitType", BlitBufferFunctionSettings, ["Synchronous", "Driver Asynchronous", "Interface Asynchronous"])
BlitType.setLabel("Blit Transfer Type")
BlitType.setDefaultValue("Synchronous")
BlitType.setDependencies(onBlitTypeChanged, ["BlitType"])
BlitType.setDescription("<html> Set the blit transfer type<br>"
								"Synchronous - blocking transfer. The blit function will block until the buffer is completely written to the display.<br>"
								"Driver Asynchronous - non-blocking transfer. The blit function will not block, and will return immediately.<br>"
								"                    Buffer blits will be managed thru the display driver task. <br>"
								"                    Interface driver must be non-blocking.<br>"
								"Interface Asynchronous - non-blocking transfer. the blit function will not block, and will return immediately.<br>"
								"                    Buffer blits will be managed by the display interface, thru the interface callback.<br>"
								"                    Make sure that the interface driver is non-blocking.</html>")

PixelDataSettingsMenu = comp.createMenuSymbol("PixelDataSettingsMenu", BlitBufferFunctionSettings)
PixelDataSettingsMenu.setLabel("Pixel Data Settings")
PixelDataSettingsMenu.setVisible(True)

PixelDataFormat = comp.createComboSymbol("PixelDataFormat", PixelDataSettingsMenu, ["RGB565"])
PixelDataFormat.setLabel("Pixel Format")
PixelDataFormat.setDefaultValue("RGB565")

DataWriteSize = comp.createComboSymbol("DataWriteSize", PixelDataSettingsMenu, ["16", "8"])
DataWriteSize.setLabel("Pixel Data Write Size (bits)")
DataWriteSize.setDefaultValue("16")
DataWriteSize.setDependencies(onDataBusConfigChanged, ["DataWriteSize"])
DataWriteSize.setDescription("<html> Size of data bus in bits. </html>")

PixelDataTxSize8Bit = comp.createComboSymbol("PixelDataTxSize8Bit", DataWriteSize, ["2 (Normal)", "2 (Little-Endian)", "3"])
PixelDataTxSize8Bit.setLabel("Pixel Data Transfer Size (Bytes)")
PixelDataTxSize8Bit.setDefaultValue("2 (Normal)")
PixelDataTxSize8Bit.setDependencies(onDataBusConfigChanged, ["PixelDataTxSize8Bit"])
PixelDataTxSize8Bit.setVisible(False)
PixelDataTxSize8Bit.setDescription("<html> Number of bytes to transfer pixel data thru 8-bit data bus.<br>"
								"'2 (Normal)' will transfer upper byte first <br>."
								"'2 (Little-Endian)' will transfer lower byte first.<br>"
								"'3' will transfer each color in separate bytes (3).</html>")

PixelDataInfoString16bit = comp.createCommentSymbol("PixelDataInfoString16bit", DataWriteSize)
PixelDataInfoString16bit.setLabel("Data0[15:0] = [r4|r3|r2|r1|r0|g5|g4|g3|g2|g1|g0|b4|b3|b2|b1|b0]")

PixelDataInfoString8bit2byteNorm = comp.createCommentSymbol("PixelDataInfoString8bit2byteNorm", DataWriteSize)
PixelDataInfoString8bit2byteNorm.setLabel("Data0[7:0] = [r4|r3|r2|r1|r0|g5|g4|g3], Data1[7:0] = [g2|g1|g0|b4|b3|b2|b1|b0]")
PixelDataInfoString8bit2byteNorm.setVisible(False)

PixelDataInfoString8bit2byteLE = comp.createCommentSymbol("PixelDataInfoString8bit2byteLE", DataWriteSize)
PixelDataInfoString8bit2byteLE.setLabel("Data0[7:0] = [g2|g1|g0|b4|b3|b2|b1|b0], Data1[7:0] = [r4|r3|r2|r1|r0|g5|g4|g3]")
PixelDataInfoString8bit2byteLE.setVisible(False)

PixelDataInfoString8bit3byte = comp.createCommentSymbol("PixelDataInfoString8bit3byte", DataWriteSize)
PixelDataInfoString8bit3byte.setLabel("Data0[7:0] = [r4|r3|r2|r1|r0|x|x|x], Data1[7:0] = [g5|g4|g3|g2|g1|g0|x|x], Data2[7:0] = [b4|b3|b2|b1|b0|x|x|x]")
PixelDataInfoString8bit3byte.setVisible(False)

FrameAddressSize = comp.createComboSymbol("FrameAddressSize", BlitBufferFunctionSettings, ["2-bytes", "1-byte"])
FrameAddressSize.setLabel("X/Y Address Size")
FrameAddressSize.setDependencies(onFrameAddressSizeChanged, ["FrameAddressSize"])
FrameAddressSize.setDefaultValue("2-bytes")
FrameAddressSize.setDescription("<html> The size of the X and Y coordinates of the address in the frame where the buffer will be written to. </html>")

SetXAddressCommand = comp.createHexSymbol("SetXAddressCommand", BlitBufferFunctionSettings)
SetXAddressCommand.setLabel("Set Column/X Command")
SetXAddressCommand.setMax(0xff)
SetXAddressCommand.setMin(0x0)
SetXAddressCommand.setDescription("<html> The command sent to the controller to write the column or X address. </html>")
XAddressOffset = comp.createIntegerSymbol("XAddressOffset", SetXAddressCommand)
XAddressOffset.setLabel("Offset")
XAddressOffset.setMin(0)
XAddressOffset.setDefaultValue(0)
XAddressOffset.setDescription("<html> X offset. </html>")
SetXAddressParmMenuWord = comp.createMenuSymbol("SetXAddressParmMenuWord", SetXAddressCommand)
SetXAddressParmMenuWord.setLabel("Parameters")
SetXAddressCommandParm0 = comp.createCommentSymbol("SetXAddressCommandParm0", SetXAddressParmMenuWord)
SetXAddressCommandParm0.setLabel("Parm0[7:0] = StartX[15:8]")
SetXAddressCommandParm1 = comp.createCommentSymbol("SetXAddressCommandParm1", SetXAddressParmMenuWord)
SetXAddressCommandParm1.setLabel("Parm1[7:0] = StartX[7:0]")
SetXAddressCommandParm2 = comp.createCommentSymbol("SetXAddressCommandParm2", SetXAddressParmMenuWord)
SetXAddressCommandParm2.setLabel("Parm2[7:0] = EndX[15:8]")
SetXAddressCommandParm3 = comp.createCommentSymbol("SetXAddressCommandParm3", SetXAddressParmMenuWord)
SetXAddressCommandParm3.setLabel("Parm3[7:0] = EndX[7:0]")
SetXAddressParmMenuByte = comp.createMenuSymbol("SetXAddressParmMenuByte", SetXAddressCommand)
SetXAddressParmMenuByte.setLabel("Parameters")
SetXAddressParmMenuByte.setVisible(False)
SetXAddressCommandParm0Byte = comp.createCommentSymbol("SetXAddressCommandParm0Byte", SetXAddressParmMenuByte)
SetXAddressCommandParm0Byte.setLabel("Parm0[7:0] = StartX[7:0]")
SetXAddressCommandParm1Byte = comp.createCommentSymbol("SetXAddressCommandParm1Byte", SetXAddressParmMenuByte)
SetXAddressCommandParm1Byte.setLabel("Parm1[7:0] = EndX[7:0]")

SetYAddressCommand = comp.createHexSymbol("SetYAddressCommand", BlitBufferFunctionSettings)
SetYAddressCommand.setLabel("Set Page/Y Command")
SetYAddressCommand.setMax(0xff)
SetYAddressCommand.setMin(0x0)
SetYAddressCommand.setDescription("<html> The command sent to the controller to write the page or Y address. </html>")
YAddressOffset = comp.createIntegerSymbol("YAddressOffset", SetYAddressCommand)
YAddressOffset.setLabel("Offset")
YAddressOffset.setMin(0)
YAddressOffset.setDefaultValue(0)
YAddressOffset.setDescription("<html> Y offset. </html>")
SetYAddressParmMenuWord = comp.createMenuSymbol("SetYAddressParmMenuWord", SetYAddressCommand)
SetYAddressParmMenuWord.setLabel("Parameter Format")
SetYAddressCommandParm0 = comp.createCommentSymbol("SetYAddressCommandParm0", SetYAddressParmMenuWord)
SetYAddressCommandParm0.setLabel("Parm0[7:0] = StartX[15:8]")
SetYAddressCommandParm1 = comp.createCommentSymbol("SetYAddressCommandParm1", SetYAddressParmMenuWord)
SetYAddressCommandParm1.setLabel("Parm1[7:0] = StartX[7:0]")
SetYAddressCommandParm2 = comp.createCommentSymbol("SetYAddressCommandParm2", SetYAddressParmMenuWord)
SetYAddressCommandParm2.setLabel("Parm2[7:0] = StartX[15:8]")
SetYAddressCommandParm3 = comp.createCommentSymbol("SetYAddressCommandParm3", SetYAddressParmMenuWord)
SetYAddressCommandParm3.setLabel("Parm3[7:0] = StartX[7:0]")
SetYAddressParmMenuByte = comp.createMenuSymbol("SetYAddressParmMenuByte", SetYAddressCommand)
SetYAddressParmMenuByte.setLabel("Parameter Format")
SetYAddressParmMenuByte.setVisible(False)
SetYAddressCommandParm0Byte = comp.createCommentSymbol("SetYAddressCommandParm0Byte", SetYAddressParmMenuByte)
SetYAddressCommandParm0Byte.setLabel("Parm0[7:0] = StartX[7:0]")
SetYAddressCommandParm1Byte = comp.createCommentSymbol("SetYAddressCommandParm1Byte", SetYAddressParmMenuByte)
SetYAddressCommandParm1Byte.setLabel("Parm1[7:0] = EndX[7:0]")

MemoryWriteCommand = comp.createHexSymbol("MemoryWriteCommand", BlitBufferFunctionSettings)
MemoryWriteCommand.setLabel("Memory Write Command")
MemoryWriteCommand.setMax(0xff)
MemoryWriteCommand.setMin(0x0)
MemoryWriteCommand.setDescription("<html> The command sent to the controller to start the memory write operation. </html>")

StubGenerateModeComment = comp.createCommentSymbol("StubGenerateModeComment", BlitBufferFunctionGenerateMode)
StubGenerateModeComment.setLabel("Only stub function will be generated. Please call appropriate routines in the BlitBuffer() function.")
StubGenerateModeComment.setVisible(False)

StubGenerateBuildErrorDisable = comp.createBooleanSymbol("StubGenerateBuildErrorDisable", StubGenerateModeComment)
StubGenerateBuildErrorDisable.setLabel("Disable #error")
StubGenerateBuildErrorDisable.setDescription("<html> Set to disable the #error that tells the user to complete the blit stub function definition. </html>")