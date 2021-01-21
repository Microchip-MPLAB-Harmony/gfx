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

HALConnected = comp.createBooleanSymbol("HALConnected", None)
HALConnected.setVisible(False)
HALConnected.setDependencies(onHALConnected, ["HALConnected"])

# these two symbols are read by the HAL for initialization purposes
# they must match the function names in the actual driver code
DriverInfoFunction = comp.createStringSymbol("DriverInfoFunction", None)
DriverInfoFunction.setLabel("Driver Info Function Name")
DriverInfoFunction.setReadOnly(True)
DriverInfoFunction.setDefaultValue("driverSSD1309InfoGet")
DriverInfoFunction.setVisible(False)

DriverInitFunction = comp.createStringSymbol("DriverInitFunction", None)
DriverInitFunction.setLabel("Driver Init Function Name")
DriverInitFunction.setReadOnly(True)
DriverInitFunction.setDefaultValue("driverSSD1309ContextInitialize")
DriverInitFunction.setVisible(False)

# configuration options
Interface = comp.createComboSymbol("Interface", None, ["SPI 4-Line"])
Interface.setLabel("Interface Mode")
Interface.setDescription("Interface to SSD1309.")
Interface.setDefaultValue("SPI 4-Line")
Interface.setVisible(False)

## Display Settings Menu
DisplaySettingsMenu = comp.createMenuSymbol("DisplaySettingsMenu", None)
DisplaySettingsMenu.setLabel("Display Settings")

HALComment = comp.createCommentSymbol("HALComment", DisplaySettingsMenu)
HALComment.setLabel("Display settings are being managed by the GFX HAL and have been hidden.")
HALComment.setVisible(False)

DisplayWidth = comp.createIntegerSymbol("DisplayWidth", DisplaySettingsMenu)
DisplayWidth.setLabel("Width")
DisplayWidth.setDescription("The width of the frame buffer in pixels.")
DisplayWidth.setDefaultValue(128)

DisplayHeight = comp.createIntegerSymbol("DisplayHeight", DisplaySettingsMenu)
DisplayHeight.setLabel("Height")
DisplayHeight.setDescription("The height of the frame buffer in pixels.")
DisplayHeight.setDefaultValue(64)

## Driver Settings Menu
DriverSettingsMenu = comp.createMenuSymbol("DRIVER_SETTINGS_MENU", None)
DriverSettingsMenu.setLabel("Driver Settings")

DriverComment = comp.createCommentSymbol("DriverComment", DriverSettingsMenu)
DriverComment.setLabel("Settings are limited to basic controller setup. For advanced setup, modify the generated driver source.")

MultiplexRatio = comp.createIntegerSymbol("MultiplexRatio", DriverSettingsMenu)
MultiplexRatio.setLabel("Multiplex Ratio")
MultiplexRatio.setDescription("Sets default multiplex ratio")
MultiplexRatio.setDefaultValue(31)
MultiplexRatio.setMin(16)
MultiplexRatio.setMax(63)

DisplayOffset = comp.createIntegerSymbol("DisplayOffset", DriverSettingsMenu)
DisplayOffset.setLabel("Display Offset")
DisplayOffset.setDescription("Sets the display offset")
DisplayOffset.setDefaultValue(0)
DisplayOffset.setMin(0)
DisplayOffset.setMax(63)

DisplayStartLine = comp.createIntegerSymbol("DisplayStartLine", DriverSettingsMenu)
DisplayStartLine.setLabel("Display Start line")
DisplayStartLine.setDescription("Sets the display start line")
DisplayStartLine.setDefaultValue(0)
DisplayStartLine.setMin(0)
DisplayStartLine.setMax(63)

OutputScanDirection = comp.createComboSymbol("OutputScanDirection", DriverSettingsMenu, ["Up", "Down"])
OutputScanDirection.setLabel("Output Scan Direction")
OutputScanDirection.setDescription("The output scan direction")
OutputScanDirection.setDefaultValue("Down")

SegmentRemap = comp.createComboSymbol("SegmentRemap", DriverSettingsMenu, ["Col0", "Col127"])
SegmentRemap.setLabel("Segment Remap")
SegmentRemap.setDescription("Sets the Segment Remap")
SegmentRemap.setDefaultValue("Col127")

ComPinConfiguration = comp.createHexSymbol("ComPinConfiguration", DriverSettingsMenu)
ComPinConfiguration.setLabel("COM Pin Configuration")
ComPinConfiguration.setDescription("Sets the COM Pin Configuration value")
ComPinConfiguration.setDefaultValue(0x12)
ComPinConfiguration.setMin(0)
ComPinConfiguration.setMax(0xff)

ContrastControlBank0 = comp.createHexSymbol("ContrastControlBank0", DriverSettingsMenu)
ContrastControlBank0.setLabel("Contrast Control for Bank0")
ContrastControlBank0.setDescription("Sets the contrast control for Bank 0")
ContrastControlBank0.setDefaultValue(0xef)
ContrastControlBank0.setMin(0)
ContrastControlBank0.setMax(0xff)

DisplayMode = comp.createComboSymbol("DisplayMode", DriverSettingsMenu, ["Normal", "Inverse"])
DisplayMode.setLabel("Display Mode")
DisplayMode.setDescription("The display mode")
DisplayMode.setDefaultValue("Normal")

OscillatorFreq = comp.createHexSymbol("OscillatorFreq", DriverSettingsMenu)
OscillatorFreq.setLabel("Oscillator Frequency ")
OscillatorFreq.setDescription("Sets the parameter for the set oscillator freuency command (D5h)")
OscillatorFreq.setDefaultValue(0xA1)
OscillatorFreq.setMin(0)
OscillatorFreq.setMax(0xff)

VcomDeselectLevel = comp.createHexSymbol("VcomDeselectLevel", DriverSettingsMenu)
VcomDeselectLevel.setLabel("VCOM Deselect Level")
VcomDeselectLevel.setDescription("Sets the VCOM Deselect Level")
VcomDeselectLevel.setDefaultValue(0x08)
VcomDeselectLevel.setMin(0)
VcomDeselectLevel.setMax(0xff)

PreChargePeriod = comp.createHexSymbol("PreChargePeriod", DriverSettingsMenu)
PreChargePeriod.setLabel("Pre-charge period")
PreChargePeriod.setDescription("Sets the pre-charge period")
PreChargePeriod.setDefaultValue(0x15)
PreChargePeriod.setMin(0)
PreChargePeriod.setMax(0xff)


#### Unused but needed by HAL
PaletteMode = comp.createBooleanSymbol("PaletteMode", DriverSettingsMenu)
PaletteMode.setLabel("Palette Mode")
PaletteMode.setDescription("Enables Palette Mode.")
PaletteMode.setDefaultValue(False)
PaletteMode.setVisible(False)