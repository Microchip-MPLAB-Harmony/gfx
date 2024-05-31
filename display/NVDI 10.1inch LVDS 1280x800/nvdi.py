# coding: utf-8
##############################################################################
# Copyright (C) 2023 Microchip Technology Inc. and its subsidiaries.
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


def instantiateComponent(component):
    name = component.createStringSymbol("Name", None)
    name.setLabel("Model Number")
    name.setDescription("The display model number.")
    name.setDefaultValue("1M.NVDI-TM2218DA101L-01")
    name.setReadOnly(True)

    display_width = component.createIntegerSymbol("DisplayWidth", None)
    display_width.setLabel("Display Width")
    display_width.setDescription("The width of the graphics display in pixels.")
    display_width.setDefaultValue(1280)
    display_width.setReadOnly(True)

    height = component.createIntegerSymbol("DisplayHeight", None)
    height.setLabel("Display Height")
    height.setDescription("The height of the graphics display in pixels.")
    height.setDefaultValue(800)
    height.setReadOnly(True)

    touch_width = component.createIntegerSymbol("TouchWidth", None)
    touch_width.setLabel("Touch Panel Width")
    touch_width.setDescription("The width of the touch panel in pixels.")
    touch_width.setDefaultValue(1280)
    touch_width.setReadOnly(True)
    touch_width.setVisible(False)

    touch_height = component.createIntegerSymbol("TouchHeight", None)
    touch_height.setLabel("Touch Panel Height")
    touch_height.setDescription("The height of the touch panel in pixels.")
    touch_height.setDefaultValue(800)
    touch_height.setReadOnly(True)
    touch_height.setVisible(False)

    horz_menu = component.createMenuSymbol("HorzMenu", None)
    horz_menu.setLabel("Horizontal Attributes")
    horz_menu.setDescription("Contains display horizontal parameters.")

    horz_pulse_width = component.createIntegerSymbol("HorzPulseWidth", horz_menu)
    horz_pulse_width.setLabel("Horizontal Pulse Width")
    horz_pulse_width.setDescription("The horizontal pulse width.")
    horz_pulse_width.setDefaultValue(32)
    horz_pulse_width.setReadOnly(True)

    horz_back_porch = component.createIntegerSymbol("HorzBackPorch", horz_menu)
    horz_back_porch.setLabel("Horizontal Back Porch")
    horz_back_porch.setDescription("The horizontal back porch size in pixels.")
    horz_back_porch.setDefaultValue(80)
    horz_back_porch.setReadOnly(True)

    horz_front_porch = component.createIntegerSymbol("HorzFrontPorch", horz_menu)
    horz_front_porch.setLabel("Horizontal Front Porch")
    horz_front_porch.setDescription("The horizontal front porch size in pixels.")
    horz_front_porch.setDefaultValue(48)
    horz_front_porch.setReadOnly(True)

    vert_menu = component.createMenuSymbol("VertMenu", None)
    vert_menu.setLabel("Vertical Attributes")
    vert_menu.setDescription("Contains display vertical parameters.")

    vert_pulse_width = component.createIntegerSymbol("VertPulseWidth", vert_menu)
    vert_pulse_width.setLabel("Vertical Pulse Width")
    vert_pulse_width.setDescription("The vertical pulse width.")
    vert_pulse_width.setDefaultValue(6)
    vert_pulse_width.setReadOnly(True)

    vert_back_porch = component.createIntegerSymbol("VertBackPorch", vert_menu)
    vert_back_porch.setLabel("Vertical Back Porch")
    vert_back_porch.setDescription("The vertical back porch size in pixels.")
    vert_back_porch.setDefaultValue(6)
    vert_back_porch.setReadOnly(True)

    vert_front_porch = component.createIntegerSymbol("VertFrontPorch", vert_menu)
    vert_front_porch.setLabel("Vertical Front Porch")
    vert_front_porch.setDescription("The vertical front porch size in pixels.")
    vert_front_porch.setDefaultValue(3)
    vert_front_porch.setReadOnly(True)

    polarity_menu = component.createMenuSymbol("PolarityMenu", None)
    polarity_menu.setLabel("Polarity Settings")
    polarity_menu.setDescription("Contains display polarity values.")

    data_enable_polarity = component.createBooleanSymbol("DataEnablePolarity", polarity_menu)
    data_enable_polarity.setLabel("DE Active High")
    data_enable_polarity.setDescription("Indicates the need for positive DE polarity.")
    data_enable_polarity.setDefaultValue(True)
    data_enable_polarity.setReadOnly(True)

    hsync_negative = component.createBooleanSymbol("HSYNCNegative", polarity_menu)
    hsync_negative.setLabel("HSYNC Active Low")
    hsync_negative.setDescription("Indicates the need for negative HSYNC polarity.")
    hsync_negative.setDefaultValue(True)
    hsync_negative.setReadOnly(True)

    vsync_negative = component.createBooleanSymbol("VSYNCNegative", polarity_menu)
    vsync_negative.setLabel("VSYNC Active Low")
    vsync_negative.setDescription("Indicates the need for negative VSYNC polarity.")
    vsync_negative.setDefaultValue(True)
    vsync_negative.setReadOnly(True)

    lvds_menu = component.createMenuSymbol("LVDSMenu", None)
    lvds_menu.setLabel("LVDS Settings")
    lvds_menu.setDescription("Indicates supported LVDS modes and other parameters.")

    lvds_mode = component.createStringSymbol("LVDSMode", lvds_menu)
    lvds_mode.setLabel("Mode")
    lvds_mode.setDescription("The LVDS output format/mode.")
    lvds_mode.setDefaultValue("VESA")  # VESA, JEIDA or DC_BAL
    lvds_mode.setReadOnly(True)

    lvds_bpp = component.createIntegerSymbol("LVDSBitsPerPixel", lvds_menu)
    lvds_bpp.setLabel("Bits Per Pixel")
    lvds_bpp.setDescription("The supported pixel data size in bits.")
    lvds_bpp.setDefaultValue(24)  # 18 or 24
    lvds_bpp.setReadOnly(True)

    lvds_clk = component.createIntegerSymbol("LVDSDotClk", lvds_menu)
    lvds_clk.setLabel("LCD Pixel Clock (kHz)")
    lvds_clk.setDescription("The pixel clock of the LCD panel.")
    lvds_clk.setDefaultValue(35000)
    lvds_clk.setReadOnly(True)
