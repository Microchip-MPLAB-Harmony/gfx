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

DisplayInterface = comp.createStringSymbol("DisplayInterface", None)
DisplayInterface.setLabel("Display Interface")
DisplayInterface.setVisible(True)
DisplayInterface.setReadOnly(True)
DisplayInterface.setUseSingleDynamicValue(True)

DisplayInterfaceType = comp.createStringSymbol("DisplayInterfaceType", None)
DisplayInterfaceType.setLabel("Display Interface Type")
DisplayInterfaceType.setVisible(True)
DisplayInterfaceType.setReadOnly(True)
DisplayInterfaceType.setUseSingleDynamicValue(True)

### Parallel mode specific options
ParallelInterfaceWidth = comp.createComboSymbol("ParallelInterfaceWidth", DisplayInterfaceType, ["16-bit", "8-bit"])
ParallelInterfaceWidth.setLabel("Data Width")
ParallelInterfaceWidth.setDescription("Data Width")
ParallelInterfaceWidth.setDefaultValue("16-bit")
ParallelInterfaceWidth.setVisible(False)

# these two symbols are read by the HAL for initialization purposes
# they must match the function names in the actual driver code
DriverInfoFunction = comp.createStringSymbol("DriverInfoFunction", None)
DriverInfoFunction.setLabel("Driver Info Function Name")
DriverInfoFunction.setReadOnly(True)
DriverInfoFunction.setDefaultValue("driverILI9488InfoGet")
DriverInfoFunction.setVisible(False)

DriverInitFunction = comp.createStringSymbol("DriverInitFunction", None)
DriverInitFunction.setLabel("Driver Init Function Name")
DriverInitFunction.setReadOnly(True)
DriverInitFunction.setDefaultValue("driverILI9488ContextInitialize")
DriverInitFunction.setVisible(False)

## Display Settings Menu
DisplaySettingsMenu = comp.createMenuSymbol("DisplaySettingsMenu", None)
DisplaySettingsMenu.setLabel("Display Settings")

HALComment = comp.createCommentSymbol("HALComment", DisplaySettingsMenu)
HALComment.setLabel("Display settings are being managed by the GFX HAL and have been hidden.")
HALComment.setVisible(False)

DisplayWidth = comp.createIntegerSymbol("DisplayWidth", DisplaySettingsMenu)
DisplayWidth.setLabel("Width")
DisplayWidth.setDescription("The width of the frame buffer in pixels.")
DisplayWidth.setDefaultValue(320)

DisplayHeight = comp.createIntegerSymbol("DisplayHeight", DisplaySettingsMenu)
DisplayHeight.setLabel("Height")
DisplayHeight.setDescription("The height of the frame buffer in pixels.")
DisplayHeight.setDefaultValue(480)

## Driver Settings Menu
DriverSettingsMenu = comp.createMenuSymbol("DRIVER_SETTINGS_MENU", None)
DriverSettingsMenu.setLabel("Driver Settings")

UseSyncBarriers = comp.createBooleanSymbol("UseSyncBarriers", DriverSettingsMenu)
UseSyncBarriers.setLabel("Use Synchronization Barriers")
UseSyncBarriers.setDescription("Use Synchronization Barriers.")
UseSyncBarriers.setDefaultValue(True)
UseSyncBarriers.setVisible(False)

PaletteMode = comp.createBooleanSymbol("PaletteMode", DriverSettingsMenu)
PaletteMode.setLabel("Palette Mode")
PaletteMode.setDescription("Enables Palette Mode.")
PaletteMode.setDefaultValue(False)
PaletteMode.setVisible(False)

DrawBufferSize = comp.createComboSymbol("DrawBufferSize", DriverSettingsMenu, ["Pixel", "Line", "Frame"])
DrawBufferSize.setLabel("Draw Buffer Size")
DrawBufferSize.setDescription("Size of the buffer that is written to the controller.")
DrawBufferSize.setDefaultValue("Line")
DrawBufferSize.setDependencies(onDrawBufferSizeSet, ["DrawBufferSize"])

DrawBufferPreRead = comp.createBooleanSymbol("DrawBufferPreRead", DrawBufferSize)
DrawBufferPreRead.setLabel("Pre-read draw buffer")
DrawBufferPreRead.setDescription("Driver will pre-fill buffer with data from GRAM")
DrawBufferPreRead.setDefaultValue(True)
DrawBufferPreRead.setDependencies(onDrawBufferPreReadSet, ["DrawBufferPreRead"])

DrawBufferPreFillValue = comp.createHexSymbol("DrawBufferPreFillValue", DrawBufferSize)
DrawBufferPreFillValue.setLabel("Draw Buffer Fill Color")
DrawBufferPreFillValue.setDescription("Fill color for draw buffer. This applies only if pre-read is disabled.")
DrawBufferPreFillValue.setDefaultValue(0xffff)
DrawBufferPreFillValue.setVisible(False)
DrawBufferPreFillValue.setMax(0xfffff)

PixelOrder = comp.createComboSymbol("PixelOrder", DriverSettingsMenu, ["RGB", "BGR"])
PixelOrder.setLabel("PixelOrder")
PixelOrder.setDescription("Configures Pixel Order.")
PixelOrder.setDefaultValue("BGR")

RowColumnExchange = comp.createComboSymbol("RowColumnExchange", DriverSettingsMenu, ["Normal", "Reverse"])
RowColumnExchange.setLabel("Row Column Exchange")
RowColumnExchange.setDescription("Row Column Exchange")
RowColumnExchange.setDefaultValue("Normal")

ColumnAddressOrder = comp.createComboSymbol("ColumnAddressOrder", DriverSettingsMenu, ["Left-To-Right", "Right-To-Left"])
ColumnAddressOrder.setLabel("Column Address Order")
ColumnAddressOrder.setDescription("Column Address Order")
ColumnAddressOrder.setDefaultValue("Right-To-Left")

RowAddressOrder = comp.createComboSymbol("RowAddressOrder", DriverSettingsMenu, ["Top-To-Bottom", "Bottom-To-Top"])
RowAddressOrder.setLabel("Row Address Order")
RowAddressOrder.setDescription("Row Address Order")
RowAddressOrder.setDefaultValue("Top-To-Bottom")
