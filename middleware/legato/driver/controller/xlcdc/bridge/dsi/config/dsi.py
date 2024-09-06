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

# Data Tables
# Table of D-PHY PLL Control Fields
table_pll_control = [
    {"min": 80, "max": 110, "vcorange": "0b000", "icpctrl": "0b0100", "lpfctrl": "0b010000"},
    {"min": 110, "max": 150, "vcorange": "0b000", "icpctrl": "0b1000", "lpfctrl": "0b001000"},
    {"min": 150, "max": 200, "vcorange": "0b000", "icpctrl": "0b1100", "lpfctrl": "0b001000"},
    {"min": 200, "max": 250, "vcorange": "0b001", "icpctrl": "0b1100", "lpfctrl": "0b001000"},
    {"min": 250, "max": 300, "vcorange": "0b001", "icpctrl": "0b0010", "lpfctrl": "0b001000"},
    {"min": 300, "max": 400, "vcorange": "0b010", "icpctrl": "0b0101", "lpfctrl": "0b001000"},
    {"min": 400, "max": 500, "vcorange": "0b010", "icpctrl": "0b0101", "lpfctrl": "0b000001"},
    {"min": 500, "max": 600, "vcorange": "0b011", "icpctrl": "0b0110", "lpfctrl": "0b010000"},
    {"min": 600, "max": 700, "vcorange": "0b011", "icpctrl": "0b0110", "lpfctrl": "0b000100"},
    {"min": 700, "max": 900, "vcorange": "0b100", "icpctrl": "0b0110", "lpfctrl": "0b000100"},
    {"min": 900, "max": 1000, "vcorange": "0b101", "icpctrl": "0b0111", "lpfctrl": "0b010000"},
]

# Table of D-PHY High-Speed Bit Rate Codes and Transition Times
table_hsbrc = [
    {"min": 80, "max": 89, "hsbrc": "0b0000000", "clk": {"lp_hs": 32, "hs_lp": 20}, "dat": {"lp_hs": 26, "hs_lp": 13}},
    {"min": 90, "max": 99, "hsbrc": "0b0100000", "clk": {"lp_hs": 35, "hs_lp": 23}, "dat": {"lp_hs": 28, "hs_lp": 14}},
    {"min": 100, "max": 109, "hsbrc": "0b1000000", "clk": {"lp_hs": 32, "hs_lp": 22}, "dat": {"lp_hs": 26, "hs_lp": 13}},
    {"min": 110, "max": 129, "hsbrc": "0b0000010", "clk": {"lp_hs": 31, "hs_lp": 20}, "dat": {"lp_hs": 27, "hs_lp": 13}},
    {"min": 130, "max": 139, "hsbrc": "0b0100010", "clk": {"lp_hs": 33, "hs_lp": 22}, "dat": {"lp_hs": 26, "hs_lp": 14}},
    {"min": 140, "max": 149, "hsbrc": "0b1000010", "clk": {"lp_hs": 33, "hs_lp": 21}, "dat": {"lp_hs": 26, "hs_lp": 14}},
    {"min": 150, "max": 169, "hsbrc": "0b0000100", "clk": {"lp_hs": 32, "hs_lp": 20}, "dat": {"lp_hs": 27, "hs_lp": 13}},
    {"min": 170, "max": 179, "hsbrc": "0b0100100", "clk": {"lp_hs": 36, "hs_lp": 23}, "dat": {"lp_hs": 30, "hs_lp": 15}},
    {"min": 180, "max": 199, "hsbrc": "0b1000100", "clk": {"lp_hs": 40, "hs_lp": 22}, "dat": {"lp_hs": 33, "hs_lp": 15}},
    {"min": 200, "max": 219, "hsbrc": "0b0000110", "clk": {"lp_hs": 40, "hs_lp": 22}, "dat": {"lp_hs": 33, "hs_lp": 15}},
    {"min": 220, "max": 239, "hsbrc": "0b0100110", "clk": {"lp_hs": 44, "hs_lp": 24}, "dat": {"lp_hs": 36, "hs_lp": 16}},
    {"min": 240, "max": 249, "hsbrc": "0b1000110", "clk": {"lp_hs": 48, "hs_lp": 24}, "dat": {"lp_hs": 38, "hs_lp": 17}},
    {"min": 250, "max": 269, "hsbrc": "0b0001000", "clk": {"lp_hs": 48, "hs_lp": 24}, "dat": {"lp_hs": 38, "hs_lp": 17}},
    {"min": 270, "max": 299, "hsbrc": "0b0101000", "clk": {"lp_hs": 50, "hs_lp": 27}, "dat": {"lp_hs": 41, "hs_lp": 18}},
    {"min": 300, "max": 329, "hsbrc": "0b0001010", "clk": {"lp_hs": 56, "hs_lp": 28}, "dat": {"lp_hs": 45, "hs_lp": 18}},
    {"min": 330, "max": 359, "hsbrc": "0b0101010", "clk": {"lp_hs": 59, "hs_lp": 28}, "dat": {"lp_hs": 48, "hs_lp": 19}},
    {"min": 360, "max": 399, "hsbrc": "0b1001010", "clk": {"lp_hs": 61, "hs_lp": 30}, "dat": {"lp_hs": 50, "hs_lp": 20}},
    {"min": 400, "max": 449, "hsbrc": "0b0001100", "clk": {"lp_hs": 67, "hs_lp": 31}, "dat": {"lp_hs": 55, "hs_lp": 21}},
    {"min": 450, "max": 499, "hsbrc": "0b0101100", "clk": {"lp_hs": 73, "hs_lp": 31}, "dat": {"lp_hs": 59, "hs_lp": 22}},
    {"min": 500, "max": 549, "hsbrc": "0b0001110", "clk": {"lp_hs": 79, "hs_lp": 36}, "dat": {"lp_hs": 63, "hs_lp": 24}},
    {"min": 550, "max": 599, "hsbrc": "0b0101110", "clk": {"lp_hs": 83, "hs_lp": 37}, "dat": {"lp_hs": 68, "hs_lp": 25}},
    {"min": 600, "max": 649, "hsbrc": "0b0010000", "clk": {"lp_hs": 90, "hs_lp": 38}, "dat": {"lp_hs": 73, "hs_lp": 27}},
    {"min": 650, "max": 699, "hsbrc": "0b0110000", "clk": {"lp_hs": 95, "hs_lp": 40}, "dat": {"lp_hs": 77, "hs_lp": 28}},
    {"min": 700, "max": 749, "hsbrc": "0b0010010", "clk": {"lp_hs": 102, "hs_lp": 40}, "dat": {"lp_hs": 84, "hs_lp": 28}},
    {"min": 750, "max": 799, "hsbrc": "0b0110010", "clk": {"lp_hs": 106, "hs_lp": 42}, "dat": {"lp_hs": 87, "hs_lp": 30}},
    {"min": 800, "max": 849, "hsbrc": "0b1010010", "clk": {"lp_hs": 113, "hs_lp": 44}, "dat": {"lp_hs": 93, "hs_lp": 31}},
    {"min": 850, "max": 899, "hsbrc": "0b1110010", "clk": {"lp_hs": 118, "hs_lp": 47}, "dat": {"lp_hs": 98, "hs_lp": 32}},
    {"min": 900, "max": 949, "hsbrc": "0b0010100", "clk": {"lp_hs": 124, "hs_lp": 47}, "dat": {"lp_hs": 102, "hs_lp": 34}},
    {"min": 950, "max": 1000, "hsbrc": "0b0110100", "clk": {"lp_hs": 130, "hs_lp": 49}, "dat": {"lp_hs": 107, "hs_lp": 35}},
]

if any(device in Variables.get("__PROCESSOR") for device in ["SAM9X75"]):
    symbol_clk_en = "DSI_CLOCK_ENABLE"
    symbol_gclk_en = "CLK_MIPIPHY_GCLKEN"
    symbol_gck_freq = "MIPIPHY_GCLK_FREQUENCY"
    symbol_gck_src_0_freq = "MCK_FREQUENCY"
    symbol_gck_src_1_freq = "PLLADIV2CLK_FREQUENCY"
    gck_src_0 = "MCK"
    gck_src_1 = "PLLADIV2CLK"
    gck_src_0_label = "MCK"
    gck_src_1_label = "PLLA/2"
    gck_src_0_reg = "PMC_PCR_GCLKCSS_MCK_Val"
    gck_src_1_reg = "PMC_PCR_GCLKCSS_PLLADIV2CLK_Val"
    gck_max_freq = 27000000
    mipi_csi_support = True

else:
    symbol_clk_en = "DSI_CLOCK_ENABLE"
    symbol_gclk_en = "CLK_DSI_GCLKEN"
    symbol_gck_freq = "DSI_GCLK_FREQUENCY"
    symbol_gck_src_0_freq = "MAINCK_FREQUENCY"
    symbol_gck_src_1_freq = "SYSPLL_FREQUENCY"
    gck_src_0 = "MAINCK"
    gck_src_1 = "SYSPLL"
    gck_src_0_label = gck_src_0
    gck_src_1_label = gck_src_1
    gck_src_0_reg = "PMC_PCR_GCLKCSS_MAINCK_Val"
    gck_src_1_reg = "PMC_PCR_GCLKCSS_SYSPLL_Val"
    gck_max_freq = 27000000
    mipi_csi_support = False


# Run on Module Load
def instantiateComponent(component):
    project_path = "config/" + Variables.get("__CONFIGURATION_NAME") + "/gfx/driver/xlcdc/bridge"

    # Display ID String Symbol, used by XLCDC to fetch connected display parameters
    display_id = component.createStringSymbol("DisplayIDString", None)
    display_id.setVisible(False)
    display_id.setReadOnly(True)
    display_id.setDefaultValue("None")

    # Show clock helper if possible
    show_clock_menu = clk_help_test_sym()

    # Hide Automatic Symbols
    debug = False

    # Utility Symbols (hidden)
    # Initialize DSI Clock and MIPIPHY Generic Clock inside DSI PLIB
    plib_clk_en = component.createBooleanSymbol("DSIClocksEn", None)
    plib_clk_en.setLabel("DSI Clocks Enable")
    plib_clk_en.setDefaultValue(show_clock_menu)
    plib_clk_en.setVisible(False)
    plib_clk_en.setDependencies(clk_help_cb, ["core." + symbol_clk_en, "core." + symbol_gclk_en])
    # Dependency check for selecting timing data source in FTL
    global xlcdc_connected
    xlcdc_connected = component.createBooleanSymbol("XLCDCConnected", None)
    xlcdc_connected.setLabel("XLCDC Connected")
    xlcdc_connected.setDefaultValue(False)
    xlcdc_connected.setVisible(False)
    # IP supports both CSI and DSI
    apply_workarounds = component.createBooleanSymbol("SupportCSI", None)
    apply_workarounds.setVisible(False)
    apply_workarounds.setReadOnly(True)
    apply_workarounds.setDefaultValue(mipi_csi_support)

    # Clock Config Menu
    # Only availabe if clock values were never configured by the user since symbols cannot have more than one source
    dsi_clock_menu = component.createMenuSymbol("DSIClockMenu", None)
    dsi_clock_menu.setLabel("Clock Control")
    dsi_clock_menu.setDescription("Clock Settings.")

    # Clock Config Hints
    clock_hint = component.createCommentSymbol("DSIClockHint", dsi_clock_menu)
    clock_hint.setLabel("DSI, MIPIPHY is managed by Clock Manager")
    clock_hint.setVisible(not show_clock_menu)
    clock_hint.setDependencies(clk_show_sym_inv, ["DSIClocksEn"])

    # MIPI GCLK Menu
    mipiphy_gclk_menu = component.createMenuSymbol("MIPIPhyGCLKMenu", dsi_clock_menu)
    mipiphy_gclk_menu.setLabel("MIPIPHY Generic Clock")
    mipiphy_gclk_menu.setDescription("<html>MIPIPHY Generic Clock Control Settings.<br>Use either this or clock manager.<br>Not both!</html>")
    mipiphy_gclk_menu.setVisible(show_clock_menu)
    mipiphy_gclk_menu.setDependencies(clk_show_sym, ["DSIClocksEn"])

    # MIPI GCLK Source Selection
    mipi_gclk_source = component.createKeyValueSetSymbol("MIPIPhyGCLKSource", mipiphy_gclk_menu)
    mipi_gclk_source.setLabel("Source")
    mipi_gclk_source.setDescription("Generic Clock Source.")
    mipi_gclk_source.addKey(gck_src_0, gck_src_0_reg, gck_src_0_label)
    mipi_gclk_source.addKey(gck_src_1, gck_src_1_reg, gck_src_1_label)
    mipi_gclk_source.setOutputMode("Value")
    mipi_gclk_source.setDisplayMode("Description")
    mipi_gclk_source.setDefaultValue(1)

    # MIPI GCLK Source Value Hint
    mipi_gclk_src_hint = component.createIntegerSymbol("MIPIPhyGCLKSrcHint", mipiphy_gclk_menu)
    mipi_gclk_src_hint.setLabel("Input Clock (Hz)")
    mipi_gclk_src_hint.setDescription("GCLK Input Clock.")
    mipi_gclk_src_hint.setDefaultValue(Database.getSymbolValue("core", symbol_gck_src_1_freq))
    mipi_gclk_src_hint.setReadOnly(True)
    mipi_gclk_src_hint.setDependencies(update_mipi_gclk_clock, ["MIPIPhyGCLKSource", "core." + symbol_gck_src_0_freq, "core." + symbol_gck_src_1_freq])

    # MIPI GCLK Divider
    mipi_gclk_div = component.createIntegerSymbol("MIPIPhyGCLKDiv", mipiphy_gclk_menu)
    mipi_gclk_div.setLabel("Divider")
    mipi_gclk_div.setDescription("Generic Clock Divider.")
    mipi_gclk_div.setMin(1)
    mipi_gclk_div.setMax(256)
    mipi_gclk_div.setDefaultValue(20)

    # MIPI GCLK Output Value Hint
    if not show_clock_menu:
        mipi_gclk_out_hint_hz = Database.getSymbolValue("core", symbol_gck_freq)
    else:
        mipi_gclk_out_hint_hz = mipi_gclk_src_hint.getValue() / mipi_gclk_div.getValue()
    mipi_gclk_out_hint = component.createIntegerSymbol("MIPIPhyGCLKOutHint", mipiphy_gclk_menu)
    mipi_gclk_out_hint.setLabel("Output Clock (Hz)")
    mipi_gclk_out_hint.setDescription("GCLK Output Clock, i.e. Input Clock to MIPIPHY.")
    mipi_gclk_out_hint.setDefaultValue(mipi_gclk_out_hint_hz)
    mipi_gclk_out_hint.setReadOnly(True)
    mipi_gclk_out_hint.setDependencies(update_mipi_gclk_clock, ["MIPIPhyGCLKSrcHint", "MIPIPhyGCLKDiv", "core." + symbol_clk_en, "core." + symbol_gck_freq])

    # MIPI GCLK Clock Validity Hint
    mipi_gclk_valid_hint = component.createCommentSymbol("MIPIPhyGCLKValidHint", mipiphy_gclk_menu)
    mipi_gclk_valid_hint.setLabel("[Warning! Must not exceed 27 MHz]")
    mipi_gclk_valid_hint.setVisible(False)
    mipi_gclk_valid_hint.setDependencies(show_mipi_gclk_valid_hint, ["MIPIPhyGCLKOutHint"])

    # MIPI D-PHY PLL Menu
    dphy_pll_menu = component.createMenuSymbol("DPhyPllMenu", dsi_clock_menu)
    dphy_pll_menu.setLabel("D-PHY PLL Control")
    dphy_pll_menu.setDescription("<html>D-PHY High-Speed Lane Bitrate (per lane)<br>is equal to the D-PHY PLL Clock</html>")

    # D-PHY N Ratio
    dphy_pll_nratio = component.createIntegerSymbol("DPhyPllNRatio", dphy_pll_menu)
    dphy_pll_nratio.setLabel("N Ratio")
    dphy_pll_nratio.setDescription("Divides input GCLK by N")
    dphy_pll_nratio.setDefaultValue(4)
    dphy_pll_nratio.setMin(1)
    dphy_pll_nratio.setMax(256)

    # N Ratio Validity Hint
    nratio_valid_hint = component.createCommentSymbol("DPhyPllNRatioValid", dphy_pll_menu)
    nratio_valid_hint.setLabel("[Ok!]")
    nratio_valid_hint.setVisible(False)
    nratio_valid_hint.setDependencies(update_nratio_valid_hint, ["MIPIPhyGCLKOutHint", "DPhyPllNRatio"])
    # Update start-up state by manually triggering the dependency callback
    update_nratio_valid_hint(nratio_valid_hint, {"source": nratio_valid_hint.getComponent()})

    # D-PHY M Ratio
    dphy_pll_mratio = component.createIntegerSymbol("DPhyPllMRatio", dphy_pll_menu)
    dphy_pll_mratio.setLabel("M Ratio")
    dphy_pll_mratio.setDescription("Multiplies (GCLK / N) by M")
    dphy_pll_mratio.setDefaultValue(132)
    dphy_pll_mratio.setMin(1)
    dphy_pll_mratio.setMax(256)

    # D-PHY PLL Output Clock
    dphy_pll_clk_hz = (mipi_gclk_out_hint.getValue() / dphy_pll_nratio.getValue()) * dphy_pll_mratio.getValue()
    dphy_pll_clk = component.createIntegerSymbol("DPhyPllOutClkKiloHz", dphy_pll_menu)
    dphy_pll_clk.setLabel("PLL Output (kHz)")
    dphy_pll_clk.setDescription("<html>The D-PHY High-Speed Lane Bitrate (per lane)<br>is equal to the acheivable D-PHY PLL Output.</html>")
    dphy_pll_clk.setDefaultValue(dphy_pll_clk_hz / 1000)
    dphy_pll_clk.setReadOnly(True)
    dphy_pll_clk.setDependencies(update_dphy_pll_clock, ["MIPIPhyGCLKOutHint", "DPhyPllNRatio", "DPhyPllMRatio"])

    # D-PHY PLL Output Clock Validity Hint
    dphy_pll_clk_valid_hint = component.createCommentSymbol("DPhyPllOutClkValid", dphy_pll_menu)
    dphy_pll_clk_valid_hint.setLabel("[Ok!]")
    dphy_pll_clk_valid_hint.setVisible(False)
    dphy_pll_clk_valid_hint.setDependencies(update_dphypll_clk_valid_hint, ["DPhyPllOutClkKiloHz"])
    # Update start-up state by manually triggering the dependency callback
    update_dphypll_clk_valid_hint(dphy_pll_clk_valid_hint, {"value": dphy_pll_clk.getValue()})

    # D-PHY PLL VCO Range
    dphy_pll_vcorange = component.createKeyValueSetSymbol("DPhyPllVCORANGE", dphy_pll_menu)
    dphy_pll_vcorange.setLabel("VCORANGE")
    dphy_pll_vcorange.setDescription("<html>Set the Voltage Controlled Oscillator range<br>based on the D-PHY PLL Output Frequency.</html>")
    for n in table_pll_control:
        n_min = str(n["min"])
        n_max = str(n["max"])
        n_vcorange = n["vcorange"]
        dphy_pll_vcorange.addKey(n_min + "-" + n_max, n_vcorange, n_min + " to " + n_max + " MHz")
    dphy_pll_vcorange.setOutputMode("Value")
    dphy_pll_vcorange.setDisplayMode("Value")
    dphy_pll_vcorange.setDefaultValue(8)
    dphy_pll_vcorange.setVisible(debug)
    dphy_pll_vcorange.setReadOnly(True)

    # D-PHY PLL ICPCTRL
    dphy_pll_icpctrl = component.createKeyValueSetSymbol("DPhyPllICPCTRL", dphy_pll_menu)
    dphy_pll_icpctrl.setLabel("ICPCTRL")
    dphy_pll_icpctrl.setDescription("<html>Set Charge Pump Current based on<br>the D-PHY PLL Output Frequency.</html>")
    for n in table_pll_control:
        n_min = str(n["min"])
        n_max = str(n["max"])
        n_icpctrl = n["icpctrl"]
        dphy_pll_icpctrl.addKey(n_min + "-" + n_max, n_icpctrl, n_min + " to " + n_max + " MHz")
    dphy_pll_icpctrl.setOutputMode("Value")
    dphy_pll_icpctrl.setDisplayMode("Value")
    dphy_pll_icpctrl.setDefaultValue(8)
    dphy_pll_icpctrl.setVisible(debug)
    dphy_pll_icpctrl.setReadOnly(True)

    # D-PHY PLL LPFCTRL
    dphy_pll_lpfctrl = component.createKeyValueSetSymbol("DPhyPllLPFCTRL", dphy_pll_menu)
    dphy_pll_lpfctrl.setLabel("LPFCTRL")
    dphy_pll_lpfctrl.setDescription("<html>Set Loop Low Pass Filter based on<br>the D-PHY PLL Output Frequency.</html>")
    for n in table_pll_control:
        n_min = str(n["min"])
        n_max = str(n["max"])
        n_lpfctrl = n["lpfctrl"]
        dphy_pll_lpfctrl.addKey(n_min + "-" + n_max, n_lpfctrl, n_min + " to " + n_max + " MHz")
    dphy_pll_lpfctrl.setOutputMode("Value")
    dphy_pll_lpfctrl.setDisplayMode("Value")
    dphy_pll_lpfctrl.setDefaultValue(8)
    dphy_pll_lpfctrl.setVisible(debug)
    dphy_pll_lpfctrl.setReadOnly(True)

    # D-PHY High-Speed Bit Rate Codes
    dphy_pll_hsbrc = component.createKeyValueSetSymbol("DPhyPllHSBRC", dphy_pll_menu)
    dphy_pll_hsbrc.setLabel("HSBRC")
    dphy_pll_hsbrc.setDescription("<html>Set High-Speed Bit Rate Code based<br>on the D-PHY PLL Output Frequency.</html>")
    for n in table_hsbrc:
        n_min = str(n["min"])
        n_max = str(n["max"])
        n_hsbrc = n["hsbrc"]
        dphy_pll_hsbrc.addKey(n_min + "-" + n_max, n_hsbrc, n_min + " to " + n_max + " Mbps")
    dphy_pll_hsbrc.setOutputMode("Value")
    dphy_pll_hsbrc.setDisplayMode("Value")
    dphy_pll_hsbrc.setDefaultValue(22)
    dphy_pll_hsbrc.setVisible(debug)
    dphy_pll_hsbrc.setReadOnly(True)

    # MIPI D-PHY PHY Clock Lane Transition Time Menu
    dphy_phy_menu = component.createMenuSymbol("DPhyTmrMenu", dsi_clock_menu)
    dphy_phy_menu.setLabel("D-PHY PHY Control")
    dphy_phy_menu.setVisible(debug)
    dphy_phy_menu.setDescription("<html>High-Speed Transition Times<br>expressed in lane byte clock periods</html>")

    # D-PHY PHY Clock Lane Transition Time (LP to HS)
    dphy_tmr_clk_lp_hs = component.createKeyValueSetSymbol("DPhyTmrClkLpToHs", dphy_phy_menu)
    dphy_tmr_clk_lp_hs.setLabel("Clock Lane Transition Time (LP to HS)")
    dphy_tmr_clk_lp_hs.setDescription("<html>Set Clock Lane Transition Time based<br>on the D-PHY PLL Output Frequency.</html>")
    for n in table_hsbrc:
        n_min = str(n["min"])
        n_max = str(n["max"])
        n_clk_lp_hs = str(n["clk"]["lp_hs"])
        dphy_tmr_clk_lp_hs.addKey(n_min + "-" + n_max, n_clk_lp_hs, n_min + " to " + n_max + " MHz")
    dphy_tmr_clk_lp_hs.setOutputMode("Value")
    dphy_tmr_clk_lp_hs.setDisplayMode("Value")
    dphy_tmr_clk_lp_hs.setDefaultValue(22)
    dphy_tmr_clk_lp_hs.setReadOnly(True)

    # D-PHY PHY Clock Lane Transition Time (HS to LP)
    dphy_tmr_clk_hs_lp = component.createKeyValueSetSymbol("DPhyTmrClkHsToLp", dphy_phy_menu)
    dphy_tmr_clk_hs_lp.setLabel("Clock Lane Transition Time (HS to LP)")
    dphy_tmr_clk_hs_lp.setDescription("<html>Set Clock Lane Transition Time based<br>on the D-PHY PLL Output Frequency.</html>")
    for n in table_hsbrc:
        n_min = str(n["min"])
        n_max = str(n["max"])
        n_clk_hs_lp = str(n["clk"]["hs_lp"])
        dphy_tmr_clk_hs_lp.addKey(n_min + "-" + n_max, n_clk_hs_lp, n_min + " to " + n_max + " MHz")
    dphy_tmr_clk_hs_lp.setOutputMode("Value")
    dphy_tmr_clk_hs_lp.setDisplayMode("Value")
    dphy_tmr_clk_hs_lp.setDefaultValue(22)
    dphy_tmr_clk_hs_lp.setReadOnly(True)

    # D-PHY PHY Data Lane Transition Time (LP to HS)
    dphy_tmr_dat_lp_hs = component.createKeyValueSetSymbol("DPhyTmrDatLpToHs", dphy_phy_menu)
    dphy_tmr_dat_lp_hs.setLabel("Data Lane Transition Time (LP to HS)")
    dphy_tmr_dat_lp_hs.setDescription("<html>Set Data Lane Transition Time based<br>on the D-PHY PLL Output Frequency.</html>")
    for n in table_hsbrc:
        n_min = str(n["min"])
        n_max = str(n["max"])
        n_dat_lp_hs = str(n["dat"]["lp_hs"])
        dphy_tmr_dat_lp_hs.addKey(n_min + "-" + n_max, n_dat_lp_hs, n_min + " to " + n_max + " MHz")
    dphy_tmr_dat_lp_hs.setOutputMode("Value")
    dphy_tmr_dat_lp_hs.setDisplayMode("Value")
    dphy_tmr_dat_lp_hs.setDefaultValue(22)
    dphy_tmr_dat_lp_hs.setReadOnly(True)

    # D-PHY PHY Data Lane Transition Time (HS to LP)
    dphy_tmr_dat_hs_lp = component.createKeyValueSetSymbol("DPhyTmrDatHsToLp", dphy_phy_menu)
    dphy_tmr_dat_hs_lp.setLabel("Data Lane Transition Time (HS to LP)")
    dphy_tmr_dat_hs_lp.setDescription("<html>Set Data Lane Transition Time based<br>on the D-PHY PLL Output Frequency.</html>")
    for n in table_hsbrc:
        n_min = str(n["min"])
        n_max = str(n["max"])
        n_dat_hs_lp = str(n["dat"]["hs_lp"])
        dphy_tmr_dat_hs_lp.addKey(n_min + "-" + n_max, n_dat_hs_lp, n_min + " to " + n_max + " MHz")
    dphy_tmr_dat_hs_lp.setOutputMode("Value")
    dphy_tmr_dat_hs_lp.setDisplayMode("Value")
    dphy_tmr_dat_hs_lp.setDefaultValue(22)
    dphy_tmr_dat_hs_lp.setReadOnly(True)

    # Update start-up state by manually triggering the dependency callback
    # Done here since dependent symbols should all be created by now
    update_dphy_pll_clock(dphy_pll_clk, {"source": dphy_pll_clk.getComponent()})

    # Interface Config Menu
    dsi_ifconf_menu = component.createMenuSymbol("DSIIfConfMenu", None)
    dsi_ifconf_menu.setLabel("Interface Configuration")
    dsi_ifconf_menu.setDescription("Interface Configuration Settings.")

    # Input DPI Color Coding Mode
    dsi_ifconf_dpi = component.createKeyValueSetSymbol("DSIIfDPIMode", dsi_ifconf_menu)
    dsi_ifconf_dpi.setLabel("Input DPI Mode")
    dsi_ifconf_dpi.setDescription("<html>Should match the selected<br>XLCDC DPI Output Mode.</hrml>")
    dsi_ifconf_dpi.addKey("16BIT_CFG1", "0", "16BPP CFG1")
    dsi_ifconf_dpi.addKey("16BIT_CFG2", "1", "16BPP CFG2")
    dsi_ifconf_dpi.addKey("16BIT_CFG3", "2", "16BPP CFG3")
    dsi_ifconf_dpi.addKey("18BIT_CFG1", "3", "18BPP CFG1")
    dsi_ifconf_dpi.addKey("18BIT_CFG2", "4", "18BPP CFG2")
    dsi_ifconf_dpi.addKey("24BIT", "5", "24BPP")
    dsi_ifconf_dpi.setOutputMode("Value")
    dsi_ifconf_dpi.setDisplayMode("Description")
    dsi_ifconf_dpi.setDefaultValue(5)

    # Number of Active DSI Data Lanes
    dsi_ifconf_lanes = component.createIntegerSymbol("DSIIfActiveLanes", dsi_ifconf_menu)
    dsi_ifconf_lanes.setLabel("Active Data Lanes")
    dsi_ifconf_lanes.setDescription("<html>Sets the number of<br>active DSI data lanes.</html>")
    dsi_ifconf_lanes.setDefaultValue(1)
    dsi_ifconf_lanes.setMin(1)
    dsi_ifconf_lanes.setMax(4)

    # Video Mode Config Menu
    dsi_vid_menu = component.createMenuSymbol("DSIVidModeMenu", None)
    dsi_vid_menu.setLabel("Video Mode")
    dsi_vid_menu.setDescription("DSI Video Mode Settings.")
    dsi_vid_menu.setVisible(True)

    # Video Mode Transmission Type
    dsi_vm_tx_type = component.createKeyValueSetSymbol("VidModeTxType", dsi_vid_menu)
    dsi_vm_tx_type.setLabel("Transmission Type")
    dsi_vm_tx_type.setDescription("<html>Indicates the video mode transmission type.</hrml>")
    dsi_vm_tx_type.addKey("VID_MODE_TYPE_0", "0", "Non-burst with sync pulses")
    dsi_vm_tx_type.addKey("VID_MODE_TYPE_1", "1", "Non-burst with sync events")
    dsi_vm_tx_type.addKey("VID_MODE_TYPE_2", "2", "Burst mode")
    dsi_vm_tx_type.setOutputMode("Value")
    dsi_vm_tx_type.setDisplayMode("Description")
    dsi_vm_tx_type.setDefaultValue(0)

    # Timing Config Hint
    global dsi_vm_tim_hint
    dsi_vm_tim_hint = component.createCommentSymbol("TimingMenuHint", dsi_vid_menu)
    dsi_vm_tim_hint.setLabel("Timing Configuration is managed by XLCDC")
    dsi_vm_tim_hint.setVisible(False)

    # Timing Config Menu
    global dsi_vm_tim
    dsi_vm_tim = component.createMenuSymbol("TimingMenu", dsi_vid_menu)
    dsi_vm_tim.setLabel("Timing Configuration")
    dsi_vm_tim.setDescription("DSI Video Mode Timing Configuration.")
    dsi_vm_tim.setVisible(True)

    # Resolution
    dsi_vm_tim_res = component.createMenuSymbol("DVTResMenu", dsi_vm_tim)
    dsi_vm_tim_res.setLabel("Display Resolution")
    dsi_vm_tim_res.setDescription("Contains the display's resolution.")

    dsi_vm_tim_res_x = component.createIntegerSymbol("DVTResPPL", dsi_vm_tim_res)
    dsi_vm_tim_res_x.setLabel("Width")
    dsi_vm_tim_res_x.setDescription("The horizontal width in pixels.")
    dsi_vm_tim_res_x.setDefaultValue(1)
    dsi_vm_tim_res_x.setMin(1)
    dsi_vm_tim_res_x.setMax(2**11)

    dsi_vm_tim_res_y = component.createIntegerSymbol("DVTResRPF", dsi_vm_tim_res)
    dsi_vm_tim_res_y.setLabel("Height")
    dsi_vm_tim_res_y.setDescription("The vertical height in pixels")
    dsi_vm_tim_res_y.setDefaultValue(1)
    dsi_vm_tim_res_y.setMin(1)
    dsi_vm_tim_res_y.setMax(2**11)

    # Horizontal Timings
    dsi_vm_tim_hor = component.createMenuSymbol("DVTHorMenu", dsi_vm_tim)
    dsi_vm_tim_hor.setLabel("Horizontal Timings")
    dsi_vm_tim_hor.setDescription("Contains the display's horizontal timing parameters.")

    dsi_vm_tim_hor_pw = component.createIntegerSymbol("DVTHorPW", dsi_vm_tim_hor)
    dsi_vm_tim_hor_pw.setLabel("Horizontal Pulse Width")
    dsi_vm_tim_hor_pw.setDescription("The horizontal pulse width cycles.")
    dsi_vm_tim_hor_pw.setDefaultValue(1)
    dsi_vm_tim_hor_pw.setMin(1)
    dsi_vm_tim_hor_pw.setMax(2**10)

    dsi_vm_tim_hor_bp = component.createIntegerSymbol("DVTHorBP", dsi_vm_tim_hor)
    dsi_vm_tim_hor_bp.setLabel("Horizontal Back Porch")
    dsi_vm_tim_hor_bp.setDescription("The horizontal back porch size in pixels.")
    dsi_vm_tim_hor_bp.setDefaultValue(1)
    dsi_vm_tim_hor_bp.setMin(1)
    dsi_vm_tim_hor_bp.setMax(2**10)

    dsi_vm_tim_hor_fp = component.createIntegerSymbol("DVTHorFP", dsi_vm_tim_hor)
    dsi_vm_tim_hor_fp.setLabel("Horizontal Front Porch")
    dsi_vm_tim_hor_fp.setDescription("The horizontal front porch size in pixels.")
    dsi_vm_tim_hor_fp.setDefaultValue(1)
    dsi_vm_tim_hor_fp.setMin(1)
    dsi_vm_tim_hor_fp.setMax(2**10)

    # Vertical Timings
    dsi_vm_tim_ver = component.createMenuSymbol("DVTVerMenu", dsi_vm_tim)
    dsi_vm_tim_ver.setLabel("Vertical Timings")
    dsi_vm_tim_ver.setDescription("Contains the display's verticsl timing parameters.")

    dsi_vm_tim_ver_pw = component.createIntegerSymbol("DVTVerPW", dsi_vm_tim_ver)
    dsi_vm_tim_ver_pw.setLabel("Vertical Pulse Width")
    dsi_vm_tim_ver_pw.setDescription("The vertical pulse width.")
    dsi_vm_tim_ver_pw.setDefaultValue(1)
    dsi_vm_tim_ver_pw.setMin(1)
    dsi_vm_tim_ver_pw.setMax(2**10)

    dsi_vm_tim_ver_bp = component.createIntegerSymbol("DVTVerBP", dsi_vm_tim_ver)
    dsi_vm_tim_ver_bp.setLabel("Vertical Back Porch")
    dsi_vm_tim_ver_bp.setDescription("The vertical back porch size in pixels.")
    dsi_vm_tim_ver_bp.setDefaultValue(1)
    dsi_vm_tim_ver_bp.setMin(1)
    dsi_vm_tim_ver_bp.setMax(2**10)

    dsi_vm_tim_ver_fp = component.createIntegerSymbol("DVTVerFP", dsi_vm_tim_ver)
    dsi_vm_tim_ver_fp.setLabel("Vertical Front Porch")
    dsi_vm_tim_ver_fp.setDescription("The vertical front porch size in pixels.")
    dsi_vm_tim_ver_fp.setDefaultValue(1)
    dsi_vm_tim_ver_fp.setMin(1)
    dsi_vm_tim_ver_fp.setMax(2**10)

    # LCD Misc. Settings
    dsi_vm_tim_lcd = component.createMenuSymbol("DVTLCDMenu", dsi_vm_tim)
    dsi_vm_tim_lcd.setLabel("LCD Settings")
    dsi_vm_tim_lcd.setDescription("Contains the LCD display's parameters.")

    dsi_vm_tim_lcd_clk = component.createIntegerSymbol("DVTDotClkKiloHz", dsi_vm_tim_lcd)
    dsi_vm_tim_lcd_clk.setLabel("LCD Clock (kHz)")
    dsi_vm_tim_lcd_clk.setDescription("The dot/pixel clock of the LCD panel.")
    dsi_vm_tim_lcd_clk.setMin(1)
    dsi_vm_tim_lcd_clk.setDefaultValue(1)

    # Video Mode Pattern Generator
    dsi_vm_vpg = component.createMenuSymbol("DVPatGenMenu", dsi_vid_menu)
    dsi_vm_vpg.setLabel("Pattern Generator")
    dsi_vm_vpg.setDescription("DSI Video Mode Pattern Generator Settings.")
    dsi_vm_vpg.setVisible(True)

    dsi_vm_vpg_en = component.createBooleanSymbol("DVPGEn", dsi_vm_vpg)
    dsi_vm_vpg_en.setLabel("Enable")
    dsi_vm_vpg_en.setDescription("Enables the Video Mode Pattern Generator.")
    dsi_vm_vpg_en.setDefaultValue(False)

    dsi_vm_vpg_ber = component.createBooleanSymbol("DVPGBerEn", dsi_vm_vpg)
    dsi_vm_vpg_ber.setLabel("BER Mode")
    dsi_vm_vpg_ber.setDescription("Displays BER test pattern instead of colored bars.")
    dsi_vm_vpg_ber.setDefaultValue(False)

    # Generate Files
    sys_definitions_h = component.createFileSymbol("SYS_DEFINITIONS_H", None)
    sys_definitions_h.setType("STRING")
    sys_definitions_h.setOutputName("core.LIST_SYSTEM_DEFINITIONS_H_INCLUDES")
    sys_definitions_h.setSourcePath("templates/definitions.h.ftl")
    sys_definitions_h.setMarkup(True)

    sys_init_c = component.createFileSymbol("SYS_INIT_C", None)
    sys_init_c.setType("STRING")
    sys_init_c.setOutputName("core.LIST_SYSTEM_INIT_C_SYS_INITIALIZE_PERIPHERALS")
    sys_init_c.setSourcePath("templates/init.c.ftl")
    sys_init_c.setMarkup(True)

    plib_dsi_h = component.createFileSymbol("PLIB_DSI_H", None)
    plib_dsi_h.setDestPath("gfx/driver/controller/xlcdc/bridge/dsi/")
    plib_dsi_h.setSourcePath("templates/plib_dsi.h.ftl")
    plib_dsi_h.setOutputName("plib_dsi.h")
    plib_dsi_h.setProjectPath(project_path)
    plib_dsi_h.setType("HEADER")
    plib_dsi_h.setMarkup(True)

    plib_dsi_c = component.createFileSymbol("PLIB_DSI_C", None)
    plib_dsi_c.setDestPath("gfx/driver/controller/xlcdc/bridge/dsi/")
    plib_dsi_c.setSourcePath("templates/plib_dsi.c.ftl")
    plib_dsi_c.setOutputName("plib_dsi.c")
    plib_dsi_c.setProjectPath(project_path)
    plib_dsi_c.setType("SOURCE")
    plib_dsi_c.setMarkup(True)


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

    if local_id == "gfx_dsi_display_dep":
        # Update Display ID String Symbol
        print(local_name + " Display connected " + remote_comp.getID())
        local_comp.setSymbolValue("DisplayIDString", remote_comp.getID())
        # Update DSI Video Mode Timing Configuration
        update_display_config(local_comp, remote_comp.getID())

    if remote_name == "LE XLCDC Driver":
        # Update Timing Config Visibility
        dsi_vm_tim_hint.setVisible(True)
        dsi_vm_tim.setVisible(False)
        # Set FTL Timing Data Source to XLCDC
        xlcdc_connected.setValue(True)


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
    if local_id == "gfx_dsi_display_dep":
        print(local_name + " Display disconnected " + remote_comp.getID())
        local_comp.setSymbolValue("DisplayIDString", "None")

    if remote_name == "LE XLCDC Driver":
        # Update Timing Config Visibility
        dsi_vm_tim_hint.setVisible(False)
        dsi_vm_tim.setVisible(True)
        xlcdc_connected.setValue(False)


# Update DSI Video Mode Timings
def update_display_config(component, display_id):
    # Populate Display parameters
    disp_component = Database.getComponentByID(display_id)
    display_width = disp_component.getSymbolValue("DisplayWidth")
    display_height = disp_component.getSymbolValue("DisplayHeight")
    display_hpw = disp_component.getSymbolValue("HorzPulseWidth")
    display_hbp = disp_component.getSymbolValue("HorzBackPorch")
    display_hfp = disp_component.getSymbolValue("HorzFrontPorch")
    display_vpw = disp_component.getSymbolValue("VertPulseWidth")
    display_vbp = disp_component.getSymbolValue("VertBackPorch")
    display_vfp = disp_component.getSymbolValue("VertFrontPorch")
    display_lanes = disp_component.getSymbolValue("DSILanes")
    display_dotclk = disp_component.getSymbolValue("DSIDotClk")
    # Update Timings
    component.setSymbolValue("DVTResPPL", display_width)
    component.setSymbolValue("DVTResRPF", display_height)
    component.setSymbolValue("DVTHorPW", display_hpw)
    component.setSymbolValue("DVTHorBP", display_hbp)
    component.setSymbolValue("DVTHorFP", display_hfp)
    component.setSymbolValue("DVTVerPW", display_vpw)
    component.setSymbolValue("DVTVerBP", display_vbp)
    component.setSymbolValue("DVTVerFP", display_vfp)
    component.setSymbolValue("DSIIfActiveLanes", display_lanes)
    component.setSymbolValue("DVTDotClkKiloHz", display_dotclk)


# Update Clocks
# GCLK
def update_mipi_gclk_clock(symbol, event):
    clkmgr_mipiphy_gclk_hz = 0
    gclk_src_sel = event["source"].getSymbolValue("MIPIPhyGCLKSource")
    # GCLK Source 0
    if event["id"] == symbol_gck_src_0_freq:
        if gclk_src_sel == 0:
            symbol.setValue(event["value"])
    # GCLK Source 1
    elif event["id"] == symbol_gck_src_1_freq:
        if gclk_src_sel == 1:
            symbol.setValue(event["value"])
    # Peripheral Clock Enable
    elif event["id"] == symbol_clk_en:
        clkmgr_mipiphy_gclk_hz = Database.getSymbolValue("core", symbol_gck_freq)
    # GCLK Frequency
    elif event["id"] == symbol_gck_freq:
        clkmgr_mipiphy_gclk_hz = event["value"]
    # MIPIPhyGCLKSource
    elif event["id"] == "MIPIPhyGCLKSource":
        gclk_src_sel = event["value"]
        if gclk_src_sel == 0:
            symbol.setValue(Database.getSymbolValue("core", symbol_gck_src_0_freq))
        elif gclk_src_sel == 1:
            symbol.setValue(Database.getSymbolValue("core", symbol_gck_src_1_freq))
    # MIPIPhyGCLKOutHint
    out_clk = event["source"].getSymbolValue("MIPIPhyGCLKSrcHint") / event["source"].getSymbolValue("MIPIPhyGCLKDiv")
    if clk_help_test_sym():
        event["source"].setSymbolValue("MIPIPhyGCLKOutHint", out_clk)
    else:
        event["source"].setSymbolValue("MIPIPhyGCLKOutHint", clkmgr_mipiphy_gclk_hz)


# D-PHY PLL
def update_dphy_pll_clock(symbol, event):
    # Calculate PLL Output Clock
    src = event["source"].getSymbolValue("MIPIPhyGCLKOutHint")
    n_ratio = event["source"].getSymbolValue("DPhyPllNRatio")
    m_ratio = event["source"].getSymbolValue("DPhyPllMRatio")
    pll_clk_hz = (src / n_ratio) * m_ratio
    pll_clk_khz = pll_clk_hz / 1000
    pll_clk_mhz = pll_clk_khz / 1000

    # Update DPhyPllOutClkKiloHz
    symbol.setValue(pll_clk_khz)

    # Update D-PHY PLL Control Fields
    for i, item in enumerate(table_pll_control):
        if pll_clk_mhz >= item["min"] and pll_clk_mhz <= item["max"]:
            event["source"].setSymbolValue("DPhyPllVCORANGE", i)
            event["source"].setSymbolValue("DPhyPllICPCTRL", i)
            event["source"].setSymbolValue("DPhyPllLPFCTRL", i)
            break

    # Update D-PHY High-Speed Bit Rate Codes and Transition Times
    for i, item in enumerate(table_hsbrc):
        if pll_clk_mhz >= item["min"] and pll_clk_mhz <= item["max"]:
            event["source"].setSymbolValue("DPhyPllHSBRC", i)
            event["source"].setSymbolValue("DPhyTmrClkLpToHs", i)
            event["source"].setSymbolValue("DPhyTmrClkHsToLp", i)
            event["source"].setSymbolValue("DPhyTmrDatLpToHs", i)
            event["source"].setSymbolValue("DPhyTmrDatHsToLp", i)


# Clock Validity
# GCLK
def show_mipi_gclk_valid_hint(symbol, event):
    if event["value"] > gck_max_freq:
        symbol.setLabel("[Warning! Must not exceed " + str(int(gck_max_freq / 1000000)) + " MHz]")
        symbol.setVisible(True)
    elif event["value"] <= 0:
        symbol.setLabel("[Warning! 0Hz?]")
        symbol.setVisible(True)
    else:
        symbol.setLabel("[Ok!]")
        symbol.setVisible(False)


# N RATIO
def update_nratio_valid_hint(symbol, event):
    src = event["source"].getSymbolValue("MIPIPhyGCLKOutHint")
    n_ratio = event["source"].getSymbolValue("DPhyPllNRatio")
    nr_clk = src / n_ratio
    if src > 27000000:
        if not event["source"].getSymbolValue("DSIClocksEn"):
            symbol.setLabel("[Warning! Source exceeds 27 MHz]")
            symbol.setVisible(True)
        else:
            symbol.setLabel("[Warning! Fix GCLK!]")
            symbol.setVisible(True)
    elif nr_clk < 5000000:
        symbol.setLabel("[Warning! (GCLK / N) < 5 MHz]")
        symbol.setVisible(True)
    else:
        symbol.setLabel("[Ok!]")
        symbol.setVisible(False)


# D-PHY PLL Clock Out (M RATIO)
def update_dphypll_clk_valid_hint(symbol, event):
    if event["value"] > 1000000:
        symbol.setLabel("[Warning! Must not exceed 1 GHz]")
        symbol.setVisible(True)
    elif event["value"] < 80000:
        symbol.setLabel("[Warning! Must be atleast 80 MHz]")
        symbol.setVisible(True)
    else:
        symbol.setLabel("[Ok!]")
        symbol.setVisible(False)


# Clock Helper Visibility
def clk_help_cb(symbol, event):
    symbol.setValue(clk_help_test_sym())


def clk_show_sym(symbol, event):
    symbol.setVisible(event["value"])


def clk_show_sym_inv(symbol, event):
    symbol.setVisible(not event["value"])


def clk_help_test_sym():
    is_set = False
    test_symbols = [
        ["core", symbol_clk_en],
        ["core", symbol_gclk_en],
    ]

    for item in test_symbols:
        is_set = is_set or Database.getSymbolValue(item[0], item[1])

    # Return true if none of the test symbols are set
    return not is_set
