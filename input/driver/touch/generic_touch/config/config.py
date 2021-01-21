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
DriverInitName.setDefaultValue("GenericTouchController")
DriverInitName.setVisible(False)

ControllerName = comp.createStringSymbol("ControllerName", None)
ControllerName.setLabel("Controller")
ControllerName.setDefaultValue("GenericTouchController")
ControllerName.setDescription("<html>Set to target controller name. Use C-compatible label (e.g., no spaces, special characters). <br> This will be used as part of the function names in the generated code.</html>")

TouchPanelSettingsMenu = comp.createMenuSymbol("TouchPanelSettingsMenu", None)
TouchPanelSettingsMenu.setLabel("Touch Panel Settings")

TouchWidth = comp.createIntegerSymbol("TouchWidth", TouchPanelSettingsMenu)
TouchWidth.setLabel("Width")
TouchWidth.setDescription("The width of the touch area in pixels.")
TouchWidth.setMin(1)
TouchWidth.setDefaultValue(480)

TouchHeight = comp.createIntegerSymbol("TouchHeight", TouchPanelSettingsMenu)
TouchHeight.setLabel("Height")
TouchHeight.setDescription("The height of the touch area in pixels.")
TouchHeight.setMin(1)
TouchHeight.setDefaultValue(272)

InterfaceSettingsMenu = comp.createMenuSymbol("InterfaceSettingsMenu", None)
InterfaceSettingsMenu.setLabel("Interface Settings")

I2CIndex = comp.createIntegerSymbol("I2CIndex", InterfaceSettingsMenu)
I2CIndex.setLabel("I2C Index")
I2CIndex.setMax(32)
I2CIndex.setMin(0)
I2CIndex.setDescription("<html> The I2C driver index. </html>")

DeviceAddress = comp.createHexSymbol("DeviceAddress", InterfaceSettingsMenu)
DeviceAddress.setLabel("Device Address")
DeviceAddress.setMax(0xff)
DeviceAddress.setMin(0x0)
DeviceAddress.setDescription("<html> Slave Device Address. </html>")

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
"If more commands are needed, set 'numCommands' in H3\gfx\input\driver\touch\generic_touch\config\controller.py. <br>"
"More commands will cause component to load longer in MHC. </html>")

MaXNumParms = comp.createIntegerSymbol("MaXNumParms", IntializationMenu)
MaXNumParms.setLabel("Max Number of Parameters")
MaXNumParms.setDefaultValue(numParms)
MaXNumParms.setReadOnly(True)
MaXNumParms.setDescription("<html> Maximum number (" + str(numParms) + ") of parameters per command. <br>"
"If more parameters are needed, set 'numParms' in in H3\gfx\input\driver\touch\generic_touch\config\controller.py. <br>"
"More parameters will cause component to load longer in MHC. </html>")

InitCommandsCount = comp.createIntegerSymbol("InitCommandsCount", IntializationMenu)
InitCommandsCount.setLabel("Number of Init Commands")
InitCommandsCount.setDefaultValue(defaultNumCommands)
InitCommandsCount.setMax(numCommands)
InitCommandsCount.setMin(0)
InitCommandsCount.setDescription("<html> The number of initialization commands to generate. </html>")
InitCommandsCount.setDependencies(onInitCommandsCountChanged, ["InitCommandsCount"])

CommandList = comp.createListSymbol("CommandList", None)

for x in range(numCommands - 1):
	Command = comp.createHexSymbol("Command" + str(x), InitCommandsCount)
	Command.setLabel("CMD[" + str(x) + "]")
	Command.setMax(0xff)
	Command.setMin(0x0)
	Command.setDescription("<html> Value of command in hex. </html>")

	CommandComment = comp.createStringSymbol("Command" + str(x) + "Comment", Command)
	CommandComment.setLabel("Comment")
	CommandComment.setDescription("<html> Description of command. Will be added as a code comment. </html>")

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
	CommandParmsCount.setDescription("<html> Number of parameters for command. </html>")
	CommandParmsCount.setDependencies(onInitCommandParmsCountChanged, ["Command" + str(x) + "ParmsCount"])

	for y in range(numParms - 1):
		Parm = comp.createHexSymbol("Command" + str(x) + "Parm" + str(y), CommandParmsCount)
		Parm.setLabel("PARM[" + str(y) + "]")
		Parm.setMax(0xff)
		Parm.setMin(0)
		Parm.setDescription("<html> Value of parameter in hex. </html>")
		if (y > defaultNumParms - 1):
			Parm.setVisible(False)
		if (x > defaultNumCommands - 1):
			Command.setVisible(False)

TouchEventSettings = comp.createMenuSymbol("TouchEventSettings", None)
TouchEventSettings.setLabel("Touch Event Settings")

TouchInterruptValue = comp.createComboSymbol("TouchInterruptValue", TouchEventSettings, ["0", "1"])
TouchInterruptValue.setLabel("Valid Interrupt Value")
TouchInterruptValue.setDescription("The interrupt signal state for valid touch data")

TouchDataBufferSize = comp.createIntegerSymbol("TouchDataBufferSize", TouchEventSettings)
TouchDataBufferSize.setLabel("Size of touch data to read (bytes)")
TouchDataBufferSize.setDefaultValue(6)
TouchDataBufferSize.setMax(1024)
TouchDataBufferSize.setMin(0)
TouchDataBufferSize.setDescription("<html> The size of the touch data to read from the touch controller.<br>This will be read every time the touch interrupt pin is asserted. </html>")

TouchDataReadAddress = comp.createHexSymbol("TouchDataReadAddress", TouchEventSettings)
TouchDataReadAddress.setLabel("Read Register Address")
TouchDataReadAddress.setMax(0xff)
TouchDataReadAddress.setMin(0x0)
TouchDataReadAddress.setDescription("<html> The register/start address on the device where touch data will be read from. </html>")

TouchDataProcessFunctionGenerateMode = comp.createComboSymbol("TouchDataProcessFunctionGenerateMode", TouchEventSettings, ["Generate", "Stub"])
TouchDataProcessFunctionGenerateMode.setLabel("Generate Touch Data Function?")
TouchDataProcessFunctionGenerateMode.setDescription("<html>Select 'Generate' if touch data will be processed using the touch data settings.<br>"
"Select 'Stub' if an empty function will be generated, and user will provide custom function definition</html>")
TouchDataProcessFunctionGenerateMode.setDependencies(onTouchDataProcessFunctionGenerateModeChanged, ["TouchDataProcessFunctionGenerateMode"])

TouchDataMenu = comp.createMenuSymbol("TouchDataMenu", TouchEventSettings)
TouchDataMenu.setLabel("Touch Data Settings")

TouchPointEventMenu = comp.createMenuSymbol("TouchPointEventMenu", TouchDataMenu)
TouchPointEventMenu.setLabel("Touch Event")

TouchPointEventIndex = comp.createIntegerSymbol("TouchPointEventIndex", TouchPointEventMenu)
TouchPointEventIndex.setLabel("Event Byte Index")
TouchPointEventIndex.setDescription("The offset/index of the event byte in the touch data. The event byte indicates the type of event")
TouchPointEventIndex.setMin(0)

TouchPointEventMask = comp.createHexSymbol("TouchPointEventMask", TouchPointEventIndex)
TouchPointEventMask.setLabel("Event Mask")
TouchPointEventMask.setDescription("The bit mask/valid bits for the event value in the event byte")
TouchPointEventMask.setMax(0xff)
TouchPointEventMask.setMin(0x1)

TouchPointEventShift = comp.createIntegerSymbol("TouchPointEventShift", TouchPointEventIndex)
TouchPointEventShift.setLabel("Event Shift")
TouchPointEventShift.setDescription("The offset of the event value in the event byte")
TouchPointEventShift.setMin(0)

TouchPointEventDownValue = comp.createHexSymbol("TouchPointEventDownValue", TouchPointEventMenu)
TouchPointEventDownValue.setLabel("Down Event Value")
TouchPointEventDownValue.setDescription("The value of a touch down event.")
TouchPointEventDownValue.setMin(0x0)

TouchPointEventUpValue = comp.createHexSymbol("TouchPointEventUpValue", TouchPointEventMenu)
TouchPointEventUpValue.setLabel("Up Event Value")
TouchPointEventUpValue.setDescription("The value of a touch up event.")
TouchPointEventUpValue.setMin(0x1)

TouchPointEventHoldValue = comp.createHexSymbol("TouchPointEventHoldValue", TouchPointEventMenu)
TouchPointEventHoldValue.setLabel("Hold Event Value")
TouchPointEventHoldValue.setDescription("<html>The value of a touch hold event. <br>On some controllers, this can be the contact or move event.</html>")
TouchPointEventHoldValue.setMin(0x2)

TouchPointPositionMenu = comp.createMenuSymbol("TouchPointPositionMenu", TouchDataMenu)
TouchPointPositionMenu.setLabel("Touch Position")

TouchPointPosSize = comp.createComboSymbol("TouchPointPosSize", TouchPointPositionMenu, ["2", "1"])
TouchPointPosSize.setLabel("Width of position data (bytes)")
TouchPointPosSize.setDescription("The width of the position data, per axis.")
TouchPointPosSize.setDependencies(onTouchPointPosSizeChanged, ["TouchPointPosSize"])

TouchPointPosXMenu = comp.createMenuSymbol("TouchPointPosXMenu", TouchPointPositionMenu)
TouchPointPosXMenu.setLabel("X Position")

TouchPointPosXHIndex = comp.createIntegerSymbol("TouchPointPosXHIndex", TouchPointPosXMenu)
TouchPointPosXHIndex.setLabel("XH Byte Index")
TouchPointPosXHIndex.setDescription("The offset/index of the upper byte of the X position in the touch data")
TouchPointPosXHIndex.setMin(0)

TouchPointPosXHMask = comp.createHexSymbol("TouchPointPosXHMask", TouchPointPosXHIndex)
TouchPointPosXHMask.setLabel("XH Mask")
TouchPointPosXHMask.setDescription("The bit mask/valid bits for the X position in the XH byte")
TouchPointPosXHMask.setMax(0xff)
TouchPointPosXHMask.setDefaultValue(0xff)

TouchPointPosXHShift = comp.createIntegerSymbol("TouchPointPosXHShift", TouchPointPosXHIndex)
TouchPointPosXHShift.setLabel("XH Shift")
TouchPointPosXHShift.setDescription("The offset of the X position in the in the XH byte")
TouchPointPosXHShift.setMax(7)
TouchPointPosXHShift.setMin(0)

TouchPointPosXLIndex = comp.createIntegerSymbol("TouchPointPosXLIndex", TouchPointPosXMenu)
TouchPointPosXLIndex.setLabel("XL Byte Index")
TouchPointPosXLIndex.setDescription("The offset/index of the lower byte of the X position in the touch data")
TouchPointPosXLIndex.setMin(0)

TouchPointPosXLMask = comp.createHexSymbol("TouchPointPosXLMask", TouchPointPosXLIndex)
TouchPointPosXLMask.setLabel("XL Mask")
TouchPointPosXLMask.setDescription("The bit mask/valid bits for the X position in the XL byte")
TouchPointPosXLMask.setMax(0xff)
TouchPointPosXLMask.setDefaultValue(0xff)

TouchPointPosXLShift = comp.createIntegerSymbol("TouchPointPosXLShift", TouchPointPosXLIndex)
TouchPointPosXLShift.setLabel("XL Shift")
TouchPointPosXLShift.setDescription("The offset of the X position in the in the XL byte")
TouchPointPosXLShift.setMax(7)
TouchPointPosXLShift.setMin(0)

TouchPointPosYMenu = comp.createMenuSymbol("TouchPointPosYMenu", TouchPointPositionMenu)
TouchPointPosYMenu.setLabel("Y Position")

TouchPointPosYHIndex = comp.createIntegerSymbol("TouchPointPosYHIndex", TouchPointPosYMenu)
TouchPointPosYHIndex.setLabel("YH Byte Index")
TouchPointPosYHIndex.setDescription("The offset/index of the upper byte of the Y position in the touch data")
TouchPointPosYHIndex.setMin(0)

TouchPointPosYHMask = comp.createHexSymbol("TouchPointPosYHMask", TouchPointPosYHIndex)
TouchPointPosYHMask.setLabel("YH Mask")
TouchPointPosYHMask.setDescription("The bit mask/valid bits for the Y position in the YH byte")
TouchPointPosYHMask.setMax(0xff)
TouchPointPosYHMask.setDefaultValue(0xff)

TouchPointPosYHShift = comp.createIntegerSymbol("TouchPointPosYHShift", TouchPointPosYHIndex)
TouchPointPosYHShift.setLabel("YH Shift")
TouchPointPosYHShift.setDescription("The offset of the Y position in the in the YH byte")
TouchPointPosYHShift.setMax(7)
TouchPointPosYHShift.setMin(0)

TouchPointPosYLIndex = comp.createIntegerSymbol("TouchPointPosYLIndex", TouchPointPosYMenu)
TouchPointPosYLIndex.setLabel("YL Byte Index")
TouchPointPosYLIndex.setDescription("The offset/index of the lower byte of the Y position in the touch data")
TouchPointPosYLIndex.setMin(0)

TouchPointPosYLMask = comp.createHexSymbol("TouchPointPosYLMask", TouchPointPosYLIndex)
TouchPointPosYLMask.setLabel("YL Mask")
TouchPointPosYLMask.setDescription("The bit mask/valid bits for the Y position in the YL byte")
TouchPointPosYLMask.setMin(0xff)
TouchPointPosYLMask.setDefaultValue(0xff)

TouchPointPosYLShift = comp.createIntegerSymbol("TouchPointPosYLShift", TouchPointPosYLIndex)
TouchPointPosYLShift.setLabel("YL Shift")
TouchPointPosYLShift.setDescription("The offset of the Y position in the in the YL byte")
TouchPointPosYLShift.setMax(7)
TouchPointPosYLShift.setMin(0)

TouchGestureSettings = comp.createBooleanSymbol("TouchGestureSettings", TouchDataMenu)
TouchGestureSettings.setLabel("Touch Gestures")
TouchGestureSettings.setDescription("Check if touch gestures will be parsed from the touch data")
TouchGestureSettings.setVisible(False)
TouchGestureSettings.setDefaultValue(False)

RTOSMenu = comp.createMenuSymbol("RTOSMenu", None)
RTOSMenu.setLabel("RTOS Settings")
RTOSMenu.setDescription("RTOS Settings")
RTOSMenu.setVisible(Database.getSymbolValue("HarmonyCore", "SELECT_RTOS") != "BareMetal")
RTOSMenu.setDependencies(showTouchRTOSMenu, ["HarmonyCore.SELECT_RTOS"])

RTOSEnabled = comp.createBooleanSymbol("RTOSEnabled", RTOSMenu)
RTOSEnabled.setLabel("RTOS Support Enabled")
RTOSEnabled.setDefaultValue(Database.getSymbolValue("HarmonyCore", "SELECT_RTOS") != "BareMetal")
RTOSEnabled.setDependencies(enableTouchRTOSymbol, ["HarmonyCore.SELECT_RTOS"])
RTOSEnabled.setVisible(False)

RTOSStackSize = comp.createIntegerSymbol("RTOSStackSize", RTOSMenu)
RTOSStackSize.setLabel("Stack Size")
RTOSStackSize.setMin(0)
RTOSStackSize.setDefaultValue(1024)

RTOSTaskPriority = comp.createIntegerSymbol("RTOSTaskPriority", RTOSMenu)
RTOSTaskPriority.setLabel("Task Priority")
RTOSTaskPriority.setDefaultValue(1)

RTOSUseDelay = comp.createBooleanSymbol("RTOSUseDelay", RTOSMenu)
RTOSUseDelay.setLabel("Use Task Delay?")
RTOSUseDelay.setDefaultValue(True)

RTOSTaskDelay = comp.createIntegerSymbol("RTOSTaskDelay", RTOSMenu)
RTOSTaskDelay.setLabel("Task Delay")
RTOSTaskDelay.setDefaultValue(10)
RTOSTaskDelay.setMin(0)






