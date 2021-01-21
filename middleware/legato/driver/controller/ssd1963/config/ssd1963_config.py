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
DriverInitName.setVisible(False)
DriverInitName.setReadOnly(True)
DriverInitName.setDefaultValue("ssd1963DisplayDriver")


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

ParallelInterfaceWidth = comp.createComboSymbol("ParallelInterfaceWidth", DisplayInterfaceType, ["16-bit", "8-bit"])
ParallelInterfaceWidth.setLabel("Data Width")
ParallelInterfaceWidth.setDescription("Data Width")
ParallelInterfaceWidth.setDefaultValue("16-bit")

#General Comment
DriverComment = comp.createCommentSymbol("DriverComment", None)
DriverComment.setLabel("The settings for this driver are limited to displays that support 16-bit RGB 565 format. For other display types, the driver may need to be modified manually.")

# configuration options
HALComment = comp.createCommentSymbol("HALComment", None)
HALComment.setLabel("Some settings are being managed by the GFX Core LE and have been hidden.")
HALComment.setVisible(False)

## Display Settings Menu
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

DisplayHorzPulseWidth = comp.createIntegerSymbol("DisplayHorzPulseWidth", DisplaySettingsMenu)
DisplayHorzPulseWidth.setLabel("Horizontal Pulse Width")
DisplayHorzPulseWidth.setMin(0)
DisplayHorzPulseWidth.setDefaultValue(44)

DisplayHorzBackPorch = comp.createIntegerSymbol("DisplayHorzBackPorch", DisplaySettingsMenu)
DisplayHorzBackPorch.setLabel("Horizontal Back Porch")
DisplayHorzBackPorch.setMin(0)
DisplayHorzBackPorch.setDefaultValue(2)

DisplayHorzFrontPorch = comp.createIntegerSymbol("DisplayHorzFrontPorch", DisplaySettingsMenu)
DisplayHorzFrontPorch.setLabel("Horizontal Front Porch")
DisplayHorzFrontPorch.setMin(0)
DisplayHorzFrontPorch.setDefaultValue(2)

DisplayVertPulseWidth = comp.createIntegerSymbol("DisplayVertPulseWidth", DisplaySettingsMenu)
DisplayVertPulseWidth.setLabel("Vertical Pulse Width")
DisplayVertPulseWidth.setMin(0)
DisplayVertPulseWidth.setDefaultValue(22)

DisplayVertBackPorch = comp.createIntegerSymbol("DisplayVertBackPorch", DisplaySettingsMenu)
DisplayVertBackPorch.setLabel("Vertical Back Porch")
DisplayVertBackPorch.setMin(0)
DisplayVertBackPorch.setDefaultValue(2)

DisplayVertFrontPorch = comp.createIntegerSymbol("DisplayVertFrontPorch", DisplaySettingsMenu)
DisplayVertFrontPorch.setLabel("Vertical Front Porch")
DisplayVertFrontPorch.setMin(0)
DisplayVertFrontPorch.setDefaultValue(2)

## Clock Settings Menu
MCLK_DefaultValue_Hz = 100000000 #100MHz
PCLK_DefaultValue_Hz = 15000000 #15MHz
PCLK_PRESCLR_DefaultValue = float( float(MCLK_DefaultValue_Hz)/float(PCLK_DefaultValue_Hz))

ClockSettingsMenu = comp.createMenuSymbol("ClockSettingsMenu", None)
ClockSettingsMenu.setLabel("Clock Settings")

MasterClock = comp.createIntegerSymbol("MasterClock", ClockSettingsMenu)
MasterClock.setLabel("Master Clock (Hz)")
MasterClock.setDescription("PLL Frequency")
MasterClock.setDefaultValue(MCLK_DefaultValue_Hz)
MasterClock.setReadOnly(True)

PixelClock = comp.createIntegerSymbol("PixelClock", ClockSettingsMenu)
PixelClock.setLabel("Pixel Clock (Hz)")
PixelClock.setDescription("Output Pixel Clock")
PixelClock.setDefaultValue(PCLK_DefaultValue_Hz)
PixelClock.setDependencies(onPixelClockSet, ["PixelClock"])

PixelClockPreScaler = comp.createStringSymbol("PixelClockPreScaler", ClockSettingsMenu)
PixelClockPreScaler.setLabel("Pixel Clock PreScaler")
PixelClockPreScaler.setDescription("Pixel Clock PreScaler")
PixelClockPreScaler.setDefaultValue(str(float("{0:.4f}".format(PCLK_PRESCLR_DefaultValue))))
PixelClockPreScaler.setReadOnly(True)

DisplayTimingOptionsEnabled = comp.createBooleanSymbol("DisplayTimingOptionsEnabled", None)
DisplayTimingOptionsEnabled.setLabel("Display Timing Options Enabled")
DisplayTimingOptionsEnabled.setDescription("Hints to the HAL if display timing is configurable for this display.")
DisplayTimingOptionsEnabled.setDefaultValue(True)
DisplayTimingOptionsEnabled.setVisible(False)

### Unsupported options
PaletteMode = comp.createBooleanSymbol("PaletteMode", None)
PaletteMode.setLabel("Palette Mode")
PaletteMode.setDescription("Enables Palette Mode.")
PaletteMode.setDefaultValue(False)
PaletteMode.setVisible(False)
###

### Start of Backlight config options
BacklightSettings = comp.createMenuSymbol("BacklightSettings", None)
BacklightSettings.setLabel("Backlight Settings")

BacklightPWMFrequency = comp.createIntegerSymbol("BacklightPWMFrequency", BacklightSettings)
BacklightPWMFrequency.setLabel("Backlight PWM Frequency (Hz)")
BacklightPWMFrequency.setDescription("The Backlight PWM Frequency")
BacklightPWMFrequency.setMin(0)
BacklightPWMFrequency.setMax(1000)
BacklightPWMFrequency.setDefaultValue(200)
BacklightPWMFrequency.setDependencies(onBacklightPWMFrequencySet, ["BacklightPWMFrequency"])

DefaultBrightness = comp.createIntegerSymbol("DefaultBrightness", BacklightSettings)
DefaultBrightness.setLabel("Default Brightness (%)")
DefaultBrightness.setDescription("The default brightness setting at driver startup")
DefaultBrightness.setMin(0)
DefaultBrightness.setMax(100)
DefaultBrightness.setDefaultValue(100)




