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

def instantiateComponent(comp):
	comp.setHelpFile("../../docs/help_harmony_gfx_html_alias.h")
	#comp.setHelp("IDH_HTML_GFX_CMP__2__Display_Component")
	
	Name = comp.createStringSymbol("Name", None)
	Name.setLabel("Name")
	Name.setDescription("The display name.")
	Name.setDefaultValue("Generic Display")
	Name.setReadOnly(True)
	
	Comment = comp.createCommentSymbol("Comment", None)
	Comment.setLabel('Use GFX Core LE Component to configure display parameters')
	Comment.setVisible(True)
	
	DisplayWidth = comp.createIntegerSymbol("DisplayWidth", None)
	DisplayWidth.setLabel("Graphics Display Width")
	DisplayWidth.setDescription("The width of the graphics display in pixels.")
	DisplayWidth.setDefaultValue(480)
	DisplayWidth.setReadOnly(True)
	DisplayWidth.setDependencies(onDisplayValueChange, ["gfx_hal_le.DisplayWidth"])
	
	DisplayHeight = comp.createIntegerSymbol("DisplayHeight", None)
	DisplayHeight.setLabel("Graphics Display Height")
	DisplayHeight.setDescription("The height of the graphics display in pixels.")
	DisplayHeight.setDefaultValue(272)
	DisplayHeight.setReadOnly(True)
	DisplayHeight.setDependencies(onDisplayValueChange, ["gfx_hal_le.DisplayHeight"])
	
	Comment0 = comp.createCommentSymbol("Comment0", None)
	Comment0.setLabel('Use Touch driver component to configure touch width and height')
	Comment0.setVisible(True)
	
	TouchWidth = comp.createIntegerSymbol("TouchWidth", None)
	TouchWidth.setLabel("Touch Panel Width")
	TouchWidth.setDescription("The width of the touch panel in pixels.")
	TouchWidth.setDefaultValue(480)
	TouchWidth.setVisible(False)
	
	TouchHeight = comp.createIntegerSymbol("TouchHeight", None)
	TouchHeight.setLabel("Touch Panel Height")
	TouchHeight.setDescription("The height of the touch panel in pixels.")
	TouchHeight.setDefaultValue(272)
	TouchHeight.setVisible(False)
	
	HorzMenu = comp.createMenuSymbol("HorzMenu", None)
	HorzMenu.setLabel("Horizontal Attributes")
	HorzMenu.setDescription("Contains the display horizontal refresh values.")
	
	HorzPulseWidth = comp.createIntegerSymbol("HorzPulseWidth", HorzMenu)
	HorzPulseWidth.setLabel("Horizontal Pulse Width")
	HorzPulseWidth.setDescription("The horizontal pulse width.")
	HorzPulseWidth.setDefaultValue(41)
	HorzPulseWidth.setReadOnly(True)
	HorzPulseWidth.setDependencies(onDisplayValueChange, ["gfx_hal_le.DisplayHorzPulseWidth"])
	
	HorzBackPorch = comp.createIntegerSymbol("HorzBackPorch", HorzMenu)
	HorzBackPorch.setLabel("Horizontal Back Porch")
	HorzBackPorch.setDescription("The horizontal back porch size in pixels.")
	HorzBackPorch.setDefaultValue(2)
	HorzBackPorch.setReadOnly(True)
	HorzBackPorch.setDependencies(onDisplayValueChange, ["gfx_hal_le.DisplayHorzBackPorch"])
	
	HorzFrontPorch = comp.createIntegerSymbol("HorzFrontPorch", HorzMenu)
	HorzFrontPorch.setLabel("Horizontal Front Porch")
	HorzFrontPorch.setDescription("The horizontal front porch size in pixels.")
	HorzFrontPorch.setDefaultValue(2)
	HorzFrontPorch.setReadOnly(True)
	HorzFrontPorch.setDependencies(onDisplayValueChange, ["gfx_hal_le.DisplayHorzFrontPorch"])
	
	VertMenu = comp.createMenuSymbol("VertMenu", None)
	VertMenu.setLabel("Vertical Attributes")
	VertMenu.setDescription("Contains the display vertical refresh values.")
	
	VertPulseWidth = comp.createIntegerSymbol("VertPulseWidth", VertMenu)
	VertPulseWidth.setLabel("Vertical Pulse Width")
	VertPulseWidth.setDescription("The vertical pulse width.")
	VertPulseWidth.setDefaultValue(10)
	VertPulseWidth.setReadOnly(True)
	VertPulseWidth.setDependencies(onDisplayValueChange, ["gfx_hal_le.DisplayVertPulseWidth"])
	
	VertBackPorch = comp.createIntegerSymbol("VertBackPorch", VertMenu)
	VertBackPorch.setLabel("Vertical Back Porch")
	VertBackPorch.setDescription("The vertical back porch size in pixels.")
	VertBackPorch.setDefaultValue(2)
	VertBackPorch.setReadOnly(True)
	VertBackPorch.setDependencies(onDisplayValueChange, ["gfx_hal_le.DisplayVertBackPorch"])
	
	VertFrontPorch = comp.createIntegerSymbol("VertFrontPorch", VertMenu)
	VertFrontPorch.setLabel("Vertical Front Porch")
	VertFrontPorch.setDescription("The vertical front porch size in pixels.")
	VertFrontPorch.setDefaultValue(2)
	VertFrontPorch.setReadOnly(True)
	VertFrontPorch.setDependencies(onDisplayValueChange, ["gfx_hal_le.DisplayVertFrontPorch"])
	
	BackLightMenu = comp.createMenuSymbol("BackLightMenu", None)
	BackLightMenu.setLabel("Back Light Settings")
	BackLightMenu.setDescription("Contains values for controlling the back light.")
	
	BacklightDisable = comp.createIntegerSymbol("BacklightDisable", BackLightMenu)
	BacklightDisable.setLabel("Back Light Disable Value")
	BacklightDisable.setDescription("The value used to disable the display back light.")
	BacklightDisable.setDefaultValue(0)
	BacklightDisable.setReadOnly(True)
	BacklightDisable.setDependencies(onDisplayValueChange, ["gfx_hal_le.DisplayBacklightDisable"])
	
	BacklightEnable = comp.createIntegerSymbol("BacklightEnable", BackLightMenu)
	BacklightEnable.setLabel("Back Light Enable Value")
	BacklightEnable.setDescription("The value used to enable the display back light.")
	BacklightEnable.setDefaultValue(1)
	BacklightEnable.setReadOnly(True)
	BacklightEnable.setDependencies(onDisplayValueChange, ["gfx_hal_le.DisplayBacklightEnable"])
	
	PolarityMenu = comp.createMenuSymbol("PolarityMenu", None)
	PolarityMenu.setLabel("Polarity Settings")
	PolarityMenu.setDescription("Contains the display polarity values.")
	
	HSYNCNegative = comp.createBooleanSymbol("HSYNCNegative", PolarityMenu)
	HSYNCNegative.setLabel("HSYNC Negative?")
	HSYNCNegative.setDescription("Indicates if this display requries negative HSYNC polarity.")
	HSYNCNegative.setDefaultValue(False)
	HSYNCNegative.setReadOnly(True)
	HSYNCNegative.setDependencies(onDisplayValueChange, ["gfx_hal_le.DisplayHSYNCNegative"])
	
	VSYNCNegative = comp.createBooleanSymbol("VSYNCNegative", PolarityMenu)
	VSYNCNegative.setLabel("VSYNC Negative?")
	VSYNCNegative.setDescription("Indicates if this display requries negative VSYNC polarity.")
	VSYNCNegative.setDefaultValue(False)
	VSYNCNegative.setReadOnly(True)
	VSYNCNegative.setDependencies(onDisplayValueChange, ["gfx_hal_le.DisplayVSYNCNegative"])
	
	DataEnable = comp.createBooleanSymbol("DataEnable", None)
	DataEnable.setLabel("Use Data Enable?")
	DataEnable.setDescription("Indicates if this display requries the use of the Data Enable line.")
	DataEnable.setDefaultValue(True)
	DataEnable.setReadOnly(True)
	DataEnable.setDependencies(onDisplayValueChange, ["gfx_hal_le.DisplayDataEnable"])
	
	DataEnablePolarity = comp.createBooleanSymbol("DataEnablePolarity", None)
	DataEnablePolarity.setLabel("Data Enable Polarity Positive?")
	DataEnablePolarity.setDescription("Indicates if this display Data Enable polarity is positive.")
	DataEnablePolarity.setDefaultValue(True)
	DataEnablePolarity.setReadOnly(True)
	DataEnablePolarity.setDependencies(onDisplayValueChange, ["gfx_hal_le.DisplayDataEnablePolarity"])
	
	UseReset = comp.createBooleanSymbol("UseReset", None)
	UseReset.setLabel("Use Reset?")
	UseReset.setDescription("Indicates if this display reset line should be used.")
	UseReset.setDefaultValue(True)
	UseReset.setReadOnly(True)
	UseReset.setDependencies(onDisplayValueChange, ["gfx_hal_le.DisplayUseReset"])
	
	ResetPolarity = comp.createBooleanSymbol("ResetPolarity", None)
	ResetPolarity.setLabel("Reset Polarity Positive?")
	ResetPolarity.setDescription("Indicates if this display reset line should be reset positive.")
	ResetPolarity.setDefaultValue(True)
	ResetPolarity.setReadOnly(True)
	ResetPolarity.setDependencies(onDisplayValueChange, ["gfx_hal_le.DisplayResetPolarity"])
	
	UseChipSelect = comp.createBooleanSymbol("UseChipSelect", None)
	UseChipSelect.setLabel("Use Chip Select?")
	UseChipSelect.setDescription("Indicates if this display uses the chip select line.")
	UseChipSelect.setDefaultValue(False)
	UseChipSelect.setReadOnly(True)
	UseChipSelect.setDependencies(onDisplayValueChange, ["gfx_hal_le.DisplayUseChipSelect"])
	
	ChipSelectPolarity = comp.createBooleanSymbol("ChipSelectPolarity", None)
	ChipSelectPolarity.setLabel("Chip Select Polarity Positive?")
	ChipSelectPolarity.setDescription("Indicates if this display chip select line should be positive.")
	ChipSelectPolarity.setDefaultValue(True)
	ChipSelectPolarity.setReadOnly(True)
	ChipSelectPolarity.setDependencies(onDisplayValueChange, ["gfx_hal_le.DisplayChipSelectPolarity"])

def onDisplayValueChange(mySymbol, event):

	DisplayWidthValue = Database.getSymbolValue("gfx_hal_le", "DisplayWidth")
	mySymbol.getComponent().setSymbolValue("DisplayWidth", DisplayWidthValue, 1)

	DisplayHeightValue = Database.getSymbolValue("gfx_hal_le", "DisplayHeight")
	mySymbol.getComponent().setSymbolValue("DisplayHeight", DisplayHeightValue, 1)

	HorzPulseWidthValue = Database.getSymbolValue("gfx_hal_le", "DisplayHorzPulseWidth")
	mySymbol.getComponent().setSymbolValue("HorzPulseWidth", HorzPulseWidthValue, 1)

	HorzBackPorchValue = Database.getSymbolValue("gfx_hal_le", "DisplayHorzBackPorch")
	mySymbol.getComponent().setSymbolValue("HorzBackPorch", HorzBackPorchValue, 1)

	HorzFrontPorchValue = Database.getSymbolValue("gfx_hal_le", "DisplayHorzFrontPorch")
	mySymbol.getComponent().setSymbolValue("HorzFrontPorch", HorzFrontPorchValue, 1)

	VertPulseWidthValue = Database.getSymbolValue("gfx_hal_le", "DisplayVertPulseWidth")
	mySymbol.getComponent().setSymbolValue("VertPulseWidth", VertPulseWidthValue, 1)

	VertBackPorchValue = Database.getSymbolValue("gfx_hal_le", "DisplayVertBackPorch")
	mySymbol.getComponent().setSymbolValue("VertBackPorch", VertBackPorchValue, 1)

	VertFrontPorchValue = Database.getSymbolValue("gfx_hal_le", "DisplayVertFrontPorch")
	mySymbol.getComponent().setSymbolValue("VertFrontPorch", VertFrontPorchValue, 1)

	BacklightDisableValue = Database.getSymbolValue("gfx_hal_le", "DisplayBacklightDisable")
	mySymbol.getComponent().setSymbolValue("BacklightDisable", BacklightDisableValue, 1)

	BacklightEnableValue = Database.getSymbolValue("gfx_hal_le", "DisplayBacklightEnable")
	mySymbol.getComponent().setSymbolValue("BacklightEnable", BacklightEnableValue, 1)

	HSYNCNegativeValue = Database.getSymbolValue("gfx_hal_le", "DisplayHSYNCNegative")
	mySymbol.getComponent().setSymbolValue("HSYNCNegative", HSYNCNegativeValue, 1)

	VSYNCNegativeValue = Database.getSymbolValue("gfx_hal_le", "DisplayVSYNCNegative")
	mySymbol.getComponent().setSymbolValue("VSYNCNegative", VSYNCNegativeValue, 1)

	DataEnableValue = Database.getSymbolValue("gfx_hal_le", "DisplayDataEnable")
	mySymbol.getComponent().setSymbolValue("DataEnable", DataEnableValue, 1)

	DataEnablePolarityValue = Database.getSymbolValue("gfx_hal_le", "DisplayDataEnablePolarity")
	mySymbol.getComponent().setSymbolValue("DataEnablePolarity", DataEnablePolarityValue, 1)

	UseResetValue = Database.getSymbolValue("gfx_hal_le", "DisplayUseReset")
	mySymbol.getComponent().setSymbolValue("UseReset", UseResetValue, 1)

	ResetPolarityValue = Database.getSymbolValue("gfx_hal_le", "DisplayResetPolarity")
	mySymbol.getComponent().setSymbolValue("ResetPolarity", ResetPolarityValue, 1)

	UseChipSelectValue = Database.getSymbolValue("gfx_hal_le", "DisplayUseChipSelect")
	mySymbol.getComponent().setSymbolValue("UseChipSelect", UseChipSelectValue, 1)

	ChipSelectPolarityValue = Database.getSymbolValue("gfx_hal_le", "DisplayChipSelectPolarity")
	mySymbol.getComponent().setSymbolValue("ChipSelectPolarity", ChipSelectPolarityValue, 1)

