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
    project_path = "config/" + Variables.get("__CONFIGURATION_NAME") + "/gfx/driver/xlcdc/bridge"

    # Display ID String Symbol, used by XLCDC to fetch connected display parameters
    display_id = component.createStringSymbol("DisplayIDString", None)
    display_id.setVisible(False)
    display_id.setReadOnly(True)
    display_id.setDefaultValue("None")

    # Configuration Menu
    config_menu = component.createMenuSymbol("LVDSCConfig", None)
    config_menu.setLabel("Configuration")
    config_menu.setDescription("LVDSC Settings.")
    config_menu.setVisible(True)

    # LVDSC DPI Input Selection
    dpi_input_type = component.createKeyValueSetSymbol("LVDSCDPIInput", config_menu)
    dpi_input_type.setLabel("DPI Input Type")
    dpi_input_type.setDescription("Must match the DPI Output Type selected in XLCDC.")
    dpi_input_type.addKey("24BITS", "LVDSC_CFGR_LCDC_PIXSIZE_24BITS_Val", "24-bit")
    dpi_input_type.addKey("18BITS", "LVDSC_CFGR_LCDC_PIXSIZE_18BITS_Val", "18-bit")
    dpi_input_type.setOutputMode("Value")
    dpi_input_type.setDisplayMode("Description")
    dpi_input_type.setDefaultValue(0)
    dpi_input_type.setVisible(True)

    # Data Enable Polarity Selection
    den_polarity = component.createKeyValueSetSymbol("LVDSCDENPol", config_menu)
    den_polarity.setDescription("Sets the Data Enable Polarity.")
    den_polarity.setLabel("Data Enable Polarity")
    den_polarity.addKey("LOW", "LVDSC_CFGR_LCDC_DEN_POL_LOW_Val", "Active Low")
    den_polarity.addKey("HIGH", "LVDSC_CFGR_LCDC_DEN_POL_HIGH_Val", "Active High")
    den_polarity.setOutputMode("Value")
    den_polarity.setDisplayMode("Description")
    den_polarity.setDefaultValue(1)
    den_polarity.setVisible(True)

    # DC Output Mode
    dc_mode = component.createKeyValueSetSymbol("LVDSCDCMode", config_menu)
    dc_mode.setDescription("Sets the DC Mode")
    dc_mode.setLabel("DC Mode")
    dc_mode.addKey("UNBALANCED", "LVDSC_CFGR_DC_BAL_UNBALANCED_Val", "DC Unbalanced")
    dc_mode.addKey("BALANCED", "LVDSC_CFGR_DC_BAL_BALANCED_Val", "DC Balanced")
    dc_mode.setOutputMode("Value")
    dc_mode.setDisplayMode("Description")
    dc_mode.setDefaultValue(0)
    dc_mode.setVisible(True)

    # DC Unbalanced Mode Format
    mapping_format = component.createKeyValueSetSymbol("LVDSCDCMapping", config_menu)
    mapping_format.setDescription("Sets the Unbalanced DC Mode Mapping Format")
    mapping_format.setLabel("Mapping Format")
    mapping_format.addKey("JEIDA", "LVDSC_CFGR_MAPPING_JEIDA_Val", "JEIDA format")
    mapping_format.addKey("VESA", "LVDSC_CFGR_MAPPING_VESA_Val", "VESA format")
    mapping_format.setOutputMode("Value")
    mapping_format.setDisplayMode("Description")
    mapping_format.setDefaultValue(0)
    mapping_format.setVisible(True)
    mapping_format.setDependencies(show_symbol_mapping, ["LVDSCDCMode"])

    # Data Lane A3 Reserved Bit Value
    lane_a3_bit = component.createKeyValueSetSymbol("LVDSCA3Bit", config_menu)
    lane_a3_bit.setDescription("Sets the lane A3 reserved bit value")
    lane_a3_bit.setLabel("Lane A3 Bit")
    lane_a3_bit.addKey("LOW", "LVDSC_UCBR_RESA3_0_Val", "Assert 0")
    lane_a3_bit.addKey("HIGH", "LVDSC_UCBR_RESA3_1_Val", "Assert 1")
    lane_a3_bit.setOutputMode("Value")
    lane_a3_bit.setDisplayMode("Description")
    lane_a3_bit.setDefaultValue(0)
    lane_a3_bit.setVisible(True)

    # Analog Control Menu
    analog_menu = component.createMenuSymbol("LVDSCAnalogControl", None)
    analog_menu.setLabel("Analog Control")
    analog_menu.setDescription("LVDSC Analog Control Settings.")
    analog_menu.setVisible(True)

    # DC Bias
    dc_bias = component.createIntegerSymbol("LVDSCDCBias", analog_menu)
    dc_bias.setLabel("DC Bias")
    dc_bias.setDescription("Common DC Bias Control for LVDS Lanes.")
    dc_bias.setMin(0)
    dc_bias.setMax(31)
    dc_bias.setDefaultValue(9)

    # Clock Lane Pre-Emphasis
    clk_preemph = component.createIntegerSymbol("LVDSClkPremph", analog_menu)
    clk_preemph.setLabel("Pre-Emphasis CLK")
    clk_preemph.setDescription("Pre-Emphasis Control for CLK LVDS Lane")
    clk_preemph.setMin(0)
    clk_preemph.setMax(7)
    clk_preemph.setDefaultValue(0)

    # Data Lane Pre-Emphasis
    # Lane A0
    a0_premph = component.createIntegerSymbol("LVDSCA0Premph", analog_menu)
    a0_premph.setLabel("Pre-Emphasis A0")
    a0_premph.setDescription("Pre-Emphasis Control for A0 LVDS Lane")
    a0_premph.setMin(0)
    a0_premph.setMax(7)
    a0_premph.setDefaultValue(0)
    # Lane A1
    a1_premph = component.createIntegerSymbol("LVDSCA1Premph", analog_menu)
    a1_premph.setLabel("Pre-Emphasis A1")
    a1_premph.setDescription("Pre-Emphasis Control for A1 LVDS Lane")
    a1_premph.setMin(0)
    a1_premph.setMax(7)
    a1_premph.setDefaultValue(0)
    # Lane A2
    a2_premph = component.createIntegerSymbol("LVDSCA2Premph", analog_menu)
    a2_premph.setLabel("Pre-Emphasis A2")
    a2_premph.setDescription("Pre-Emphasis Control for A2 LVDS Lane")
    a2_premph.setMin(0)
    a2_premph.setMax(7)
    a2_premph.setDefaultValue(0)
    # Lane A3
    a3_premph = component.createIntegerSymbol("LVDSCA3Premph", analog_menu)
    a3_premph.setLabel("Pre-Emphasis A3")
    a3_premph.setDescription("Pre-Emphasis Control for A3 LVDS Lane")
    a3_premph.setMin(0)
    a3_premph.setMax(7)
    a3_premph.setDefaultValue(0)

    # Generate Files
    sys_definitions_h = component.createFileSymbol("SYS_DEFINITIONS_H", None)
    sys_definitions_h.setType("STRING")
    sys_definitions_h.setOutputName("core.LIST_SYSTEM_DEFINITIONS_H_INCLUDES")
    sys_definitions_h.setSourcePath("templates/definitions.h.ftl")
    sys_definitions_h.setMarkup(True)

    # Will be initialized after XLCDC. If XLCDC is not present, fall back to normal init.
    # Check init.c.ftl for implementation
    sys_init_c = component.createFileSymbol("SYS_INIT_C", None)
    sys_init_c.setType("STRING")
    sys_init_c.setOutputName("core.LIST_SYSTEM_INIT_C_SYS_INITIALIZE_PERIPHERALS")
    sys_init_c.setSourcePath("templates/init.c.ftl")
    sys_init_c.setMarkup(True)

    plib_lvdsc_h = component.createFileSymbol("PLIB_LVDSC_H", None)
    plib_lvdsc_h.setDestPath("gfx/driver/controller/xlcdc/bridge/lvdsc/")
    plib_lvdsc_h.setSourcePath("templates/plib_lvdsc.h.ftl")
    plib_lvdsc_h.setOutputName("plib_lvdsc.h")
    plib_lvdsc_h.setProjectPath(project_path)
    plib_lvdsc_h.setType("HEADER")
    plib_lvdsc_h.setMarkup(True)

    plib_lvdsc_c = component.createFileSymbol("PLIB_LVDSC_C", None)
    plib_lvdsc_c.setDestPath("gfx/driver/controller/xlcdc/bridge/lvdsc/")
    plib_lvdsc_c.setSourcePath("templates/plib_lvdsc.c.ftl")
    plib_lvdsc_c.setOutputName("plib_lvdsc.c")
    plib_lvdsc_c.setProjectPath(project_path)
    plib_lvdsc_c.setType("SOURCE")
    plib_lvdsc_c.setMarkup(True)


# Callbacks
# Callback on component attach
def onAttachmentConnected(source, target):
    local_id = source["id"]
    local_comp = source["component"]
    local_name = local_comp.getDisplayName()
    remote_id = target["id"]
    remote_comp = target["component"]
    remote_name = remote_comp.getDisplayName()

    print(local_id + " of " + local_name + " connected to " + remote_id + " of " + remote_name)

    if local_id == "gfx_lvds_display_dep":
        # Update Display ID String Symbol
        print(local_name + " Display connected " + remote_comp.getID())
        local_comp.setSymbolValue("DisplayIDString", remote_comp.getID())

        # Update config based on display LVDSMode symbol (VESA, JEIDA or DC_BAL)
        mode = remote_comp.getSymbolValue("LVDSMode")
        if mode == "JEIDA":
            local_comp.setSymbolValue("LVDSCDCMode", 0)
            local_comp.setSymbolValue("LVDSCDCMapping", 0)
        elif mode == "VESA":
            local_comp.setSymbolValue("LVDSCDCMode", 0)
            local_comp.setSymbolValue("LVDSCDCMapping", 1)
        elif mode == "DC_BAL":
            local_comp.setSymbolValue("LVDSCDCMode", 1)
        else:
            print(local_name + " Warning! Unable to load config from " + remote_comp.getID())

        # Update config based on display DataEnablePolarity symbol
        dpol = remote_comp.getSymbolValue("DataEnablePolarity")
        if dpol == True:
            local_comp.setSymbolValue("LVDSCDENPol", 1)
        elif dpol == False:
            local_comp.setSymbolValue("LVDSCDENPol", 0)
        else:
            print(local_name + " Warning! Unable to load config from " + remote_comp.getID())


# Callback on component detach
def onAttachmentDisconnected(source, target):
    local_id = source["id"]
    local_comp = source["component"]
    local_name = local_comp.getDisplayName()
    remote_id = target["id"]
    remote_comp = target["component"]
    remote_name = remote_comp.getDisplayName()

    print(local_id + " of " + local_name + " disconnected from " + remote_id + " of " + remote_name)

    # Update display component ID symbol
    if local_id == "gfx_lvds_display_dep":
        print(local_name + " Display disconnected " + remote_comp.getID())
        local_comp.setSymbolValue("DisplayIDString", "None")


# Variable Callbacks
# Set Mapping Format Visibility
def show_symbol_mapping(symbol, event):
    selection = event["symbol"].getKey(event["value"])
    if selection == "BALANCED":
        symbol.setVisible(False)
    elif selection == "UNBALANCED":
        symbol.setVisible(True)
