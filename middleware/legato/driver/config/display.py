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

DisplayMenu = halComponent.createMenuSymbol("DisplayMenu", None)
DisplayMenu.setLabel("Display Settings")
DisplayMenu.setDescription("Contains various settings for the graphics display.")

DisplayTypeName = halComponent.createStringSymbol("DisplayName", DisplayMenu)
DisplayTypeName.setLabel("Display")
DisplayTypeName.setDefaultValue("None")
DisplayTypeName.setReadOnly(True)

DriverTypeName = halComponent.createStringSymbol("DriverName", DisplayMenu)
DriverTypeName.setLabel("Graphics Driver")
DriverTypeName.setDefaultValue("None")
DriverTypeName.setReadOnly(True)
	
DisplayType = halComponent.createStringSymbol("gfx_display", DisplayMenu)
DisplayType.setDefaultValue("None")
DisplayType.setVisible(False)

DriverType = halComponent.createStringSymbol("gfx_driver", DisplayMenu)
DriverType.setDefaultValue("None")
DriverType.setVisible(False)
	
DisplayWidth = halComponent.createIntegerSymbol("DisplayWidth", DisplayMenu)
DisplayWidth.setLabel("Width")
DisplayWidth.setDescription("The width of the display in pixels.")
DisplayWidth.setDefaultValue(480)
DisplayWidth.setMin(0)
DisplayWidth.setDependencies(onUpdateDisplayTiming, ["DisplayWidth"])

DisplayHeight = halComponent.createIntegerSymbol("DisplayHeight", DisplayMenu)
DisplayHeight.setLabel("Height")
DisplayHeight.setDescription("The height of the display in pixels.")
DisplayHeight.setDefaultValue(272)
DisplayHeight.setMin(0)
DisplayHeight.setDependencies(onUpdateDisplayTiming, ["DisplayHeight"])

DisplayOrientation = halComponent.createComboSymbol("DisplayOrientation", DisplayMenu, ["0","90","180","270"])
DisplayOrientation.setLabel("Display Orientation")
DisplayOrientation.setDefaultValue("0")
DisplayOrientation.setDescription("Configures the display orientation at the hardware level.  If this setting is not supported then it may be possible to configure it at the middleware level.")

DisableDisplayOrientation = halComponent.createBooleanSymbol("DisableDisplayOrientation", DisplayMenu)
DisableDisplayOrientation.setDefaultValue(False)
DisableDisplayOrientation.setVisible(False)
DisableDisplayOrientation.setDependencies(onDisableHint, ["DisableDisplayOrientation"])

DisplayDataWidth = halComponent.createIntegerSymbol("DisplayDataWidth", DisplayMenu)
DisplayDataWidth.setLabel("Data Width")
DisplayDataWidth.setDescription("The width of the display data bus in bits.")
DisplayDataWidth.setMin(0)
DisplayDataWidth.setDefaultValue(16)

PixelClock = halComponent.createIntegerSymbol("PixelClock", DisplayMenu)
PixelClock.setLabel("Pixel Clock (Hz)")
PixelClock.setDescription("The pixel clock frequency (Hz). Configuration may be available in the Display Driver component.")
PixelClock.setDefaultValue(0)
PixelClock.setMin(0)
PixelClock.setReadOnly(True)
PixelClock.setVisible(True)
PixelClock.setDependencies(onPixelClockSet, ["PixelClock"])
PixelClock.setUseSingleDynamicValue(True)

RefreshRate = halComponent.createIntegerSymbol("RefreshRate", DisplayMenu)
RefreshRate.setLabel("Refresh Rate (Hz)")
RefreshRate.setDescription("The display refresh frequency (Hz).")
RefreshRate.setDefaultValue(0)
RefreshRate.setMin(0)
PixelClock.setReadOnly(True)
RefreshRate.setVisible(True)

DisplayHorzMenu = halComponent.createMenuSymbol("DisplayHorzMenu", DisplayMenu)
DisplayHorzMenu.setLabel("Horizontal Attributes")
DisplayHorzMenu.setDescription("Contains the display horizontal refresh values.")
DisplayHorzMenu.setVisible(True)

DisplayHorzPulseWidth = halComponent.createIntegerSymbol("DisplayHorzPulseWidth", DisplayHorzMenu)
DisplayHorzPulseWidth.setLabel("Horizontal Pulse Width")
DisplayHorzPulseWidth.setDescription("The horizontal pulse width.")
DisplayHorzPulseWidth.setDefaultValue(41)
DisplayHorzPulseWidth.setMin(0)
DisplayHorzPulseWidth.setDependencies(onUpdateDisplayTiming, ["DisplayHorzPulseWidth"])

DisplayHorzBackPorch = halComponent.createIntegerSymbol("DisplayHorzBackPorch", DisplayHorzMenu)
DisplayHorzBackPorch.setLabel("Horizontal Back Porch")
DisplayHorzBackPorch.setDescription("The horizontal back porch size in pixels.")
DisplayHorzBackPorch.setDefaultValue(2)
DisplayHorzBackPorch.setMin(0)
DisplayHorzBackPorch.setDependencies(onUpdateDisplayTiming, ["DisplayHorzBackPorch"])

DisplayHorzFrontPorch = halComponent.createIntegerSymbol("DisplayHorzFrontPorch", DisplayHorzMenu)
DisplayHorzFrontPorch.setLabel("Horizontal Front Porch")
DisplayHorzFrontPorch.setDescription("The horizontal front porch size in pixels.")
DisplayHorzFrontPorch.setDefaultValue(2)
DisplayHorzFrontPorch.setMin(0)
DisplayHorzFrontPorch.setDependencies(onUpdateDisplayTiming, ["DisplayHorzFrontPorch"])

DisplayVertMenu = halComponent.createMenuSymbol("DisplayVertMenu", DisplayMenu)
DisplayVertMenu.setLabel("Vertical Attributes")
DisplayVertMenu.setDescription("Contains the display vertical refresh values.")
DisplayVertMenu.setVisible(True)

DisplayVertPulseWidth = halComponent.createIntegerSymbol("DisplayVertPulseWidth", DisplayVertMenu)
DisplayVertPulseWidth.setLabel("Vertical Pulse Width")
DisplayVertPulseWidth.setDescription("The vertical pulse width.")
DisplayVertPulseWidth.setDefaultValue(10)
DisplayVertPulseWidth.setMin(0)
DisplayVertPulseWidth.setDependencies(onUpdateDisplayTiming, ["DisplayVertPulseWidth"])

DisplayVertBackPorch = halComponent.createIntegerSymbol("DisplayVertBackPorch", DisplayVertMenu)
DisplayVertBackPorch.setLabel("Vertical Back Porch")
DisplayVertBackPorch.setDescription("The vertical back porch size in pixels.")
DisplayVertBackPorch.setDefaultValue(2)
DisplayVertBackPorch.setMin(0)
DisplayVertBackPorch.setDependencies(onUpdateDisplayTiming, ["DisplayVertBackPorch"])

DisplayVertFrontPorch = halComponent.createIntegerSymbol("DisplayVertFrontPorch", DisplayVertMenu)
DisplayVertFrontPorch.setLabel("Vertical Front Porch")
DisplayVertFrontPorch.setDescription("The vertical front porch size in pixels.")
DisplayVertFrontPorch.setDefaultValue(2)
DisplayVertFrontPorch.setMin(0)
DisplayVertFrontPorch.setDependencies(onUpdateDisplayTiming, ["DisplayVertFrontPorch"])

DisplayInvLeftShift = halComponent.createBooleanSymbol("DisplayInvLeftShift", DisplayMenu)
DisplayInvLeftShift.setLabel("Inverted Left Shift")
DisplayInvLeftShift.setDescription("Indicates if this display requires inverted left shift.")
DisplayInvLeftShift.setDefaultValue(False)

DisplayBackLightMenu = halComponent.createMenuSymbol("DisplayBackLightMenu", DisplayMenu)
DisplayBackLightMenu.setLabel("Back Light Settings")
DisplayBackLightMenu.setDescription("Contains values for controlling the back light.")

DisplayBacklightDisable = halComponent.createIntegerSymbol("DisplayBacklightDisable", DisplayBackLightMenu)
DisplayBacklightDisable.setLabel("Back Light Disable Value")
DisplayBacklightDisable.setDescription("The value used to disable the display back light.")
DisplayBacklightDisable.setDefaultValue(0)

DisplayBacklightEnable = halComponent.createIntegerSymbol("DisplayBacklightEnable", DisplayBackLightMenu)
DisplayBacklightEnable.setLabel("Back Light Enable Value")
DisplayBacklightEnable.setDescription("The value used to enable the display back light.")
DisplayBacklightEnable.setDefaultValue(1)

DisplayPolarityMenu = halComponent.createMenuSymbol("DisplayPolarityMenu", DisplayMenu)
DisplayPolarityMenu.setLabel("Polarity Settings")
DisplayPolarityMenu.setDescription("Contains the display polarity values.")

DisplayVSYNCNegative = halComponent.createBooleanSymbol("DisplayVSYNCNegative", DisplayPolarityMenu)
DisplayVSYNCNegative.setLabel("VSYNC Polarity Positive?")
DisplayVSYNCNegative.setDescription("Indicates if this display requires positive VSYNC polarity.")
DisplayVSYNCNegative.setDefaultValue(True)

DisplayHSYNCNegative = halComponent.createBooleanSymbol("DisplayHSYNCNegative", DisplayPolarityMenu)
DisplayHSYNCNegative.setLabel("HSYNC Polarity Positive?")
DisplayHSYNCNegative.setDescription("Indicates if this display requires positive HSYNC polarity.")
DisplayHSYNCNegative.setDefaultValue(True)

DisplayDataEnable = halComponent.createBooleanSymbol("DisplayDataEnable", DisplayMenu)
DisplayDataEnable.setLabel("Use Data Enable?")
DisplayDataEnable.setDescription("Indicates if this display requires the use of the Data Enable line.")
DisplayDataEnable.setDefaultValue(True)

DisplayDataEnablePolarity = halComponent.createBooleanSymbol("DisplayDataEnablePolarity", DisplayMenu)
DisplayDataEnablePolarity.setLabel("Data Enable Polarity Positive?")
DisplayDataEnablePolarity.setDescription("Indicates if this display Data Enable polarity is positive.")
DisplayDataEnablePolarity.setDefaultValue(True)

DisplayUseReset = halComponent.createBooleanSymbol("DisplayUseReset", DisplayMenu)
DisplayUseReset.setLabel("Use Reset?")
DisplayUseReset.setDescription("Indicates if this display reset line should be used.")
DisplayUseReset.setDefaultValue(True)

DisplayResetPolarity = halComponent.createBooleanSymbol("DisplayResetPolarity", DisplayMenu)
DisplayResetPolarity.setLabel("Reset Polarity Positive?")
DisplayResetPolarity.setDescription("Indicates if this display reset line should be reset positive.")
DisplayResetPolarity.setDefaultValue(True)

DisplayUseChipSelect = halComponent.createBooleanSymbol("DisplayUseChipSelect", DisplayMenu)
DisplayUseChipSelect.setLabel("Use Chip Select?")
DisplayUseChipSelect.setDescription("Indicates if this display uses the chip select line.")
DisplayUseChipSelect.setDefaultValue(True)

DisplayChipSelectPolarity = halComponent.createBooleanSymbol("DisplayChipSelectPolarity", DisplayMenu)
DisplayChipSelectPolarity.setLabel("Chip Select Polarity Positive?")
DisplayChipSelectPolarity.setDescription("Indicates if this display chip select line should be positive.")
DisplayChipSelectPolarity.setDefaultValue(True)



