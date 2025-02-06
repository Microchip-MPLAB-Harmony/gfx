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

if any(device in Variables.get("__PROCESSOR") for device in ["SAM9X72", "SAM9X75"]):
    resgister_base_address = "XLCDC"
    symbol_clk_en = "XLCDC_CLOCK_ENABLE"
    symbol_gclk_en = "CLK_XLCDC_GCLKEN"
    symbol_lvdsc_en = "LVDSC_CLOCK_ENABLE"
    symbol_lvdspll_en = "CLK_LVDSPLL_EN"
    symbol_mck_freq = "MCK_FREQUENCY"
    symbol_gck_freq = "XLCDC_GCLK_FREQUENCY"
    symbol_gck_src_0_freq = "MCK_FREQUENCY"
    symbol_gck_src_1_freq = "PLLADIV2CLK_FREQUENCY"
    gck_src_0 = "MCK"
    gck_src_1 = "PLLADIV2CLK"
    gck_src_0_label = "MCK"
    gck_src_1_label = "PLLA/2"
    gck_src_0_reg = "PMC_PCR_GCLKCSS_MCK_Val"
    gck_src_1_reg = "PMC_PCR_GCLKCSS_PLLADIV2CLK_Val"
    gck_max_freq = 75000000
    ovr2_available = True
    lvdspll_updt_id = 3
    lvdspll_isr0_mask = "PMC_PLL_ISR0_LVDSLOCK_Msk"
    out_max_freq = 550000000
    arm_neon = False
else:
    resgister_base_address = "LCDC"
    symbol_clk_en = "LCDC_CLOCK_ENABLE"
    symbol_gclk_en = "CLK_LCDC_GCLKEN"
    symbol_lvdsc_en = "LVDSC_CLOCK_ENABLE"
    symbol_lvdspll_en = "CLK_LVDSPLL_EN"
    symbol_mck_freq = "MCK3_FREQUENCY"
    symbol_gck_freq = "LCDC_GCLK_FREQUENCY"
    symbol_gck_src_0_freq = "MCK1_FREQUENCY"
    symbol_gck_src_1_freq = "MAINCK_FREQUENCY"
    gck_src_0 = "MCK1"
    gck_src_1 = "MAINCK"
    gck_src_0_label = gck_src_0
    gck_src_1_label = gck_src_1
    gck_src_0_reg = "PMC_PCR_GCLKCSS_MCK1_Val"
    gck_src_1_reg = "PMC_PCR_GCLKCSS_MAINCK_Val"
    gck_max_freq = 90000000
    ovr2_available = False
    lvdspll_updt_id = 7
    lvdspll_isr0_mask = "PMC_PLL_ISR0_LOCK7_Msk"
    out_max_freq = 630000000
    arm_neon = True


def instantiateComponent(component):
    project_path = "config/" + Variables.get("__CONFIGURATION_NAME") + "/gfx/driver/xlcdc"

    # Activate dependency components
    Database.activateComponents(["HarmonyCore"])

    # Enable "Generate Harmony Driver Common Files" option in Core
    Database.sendMessage("HarmonyCore", "ENABLE_DRV_COMMON", {"isEnabled": True})

    # Show clock helper if possible
    show_clk_help = clk_help_test_sym()

    # Utility Symbols (hidden)
    # Driver IP name
    driver_ip_name = component.createStringSymbol("IP", None)
    driver_ip_name.setVisible(False)
    driver_ip_name.setReadOnly(True)
    driver_ip_name.setDefaultValue(resgister_base_address)
    # Typically name of gfxDisplayDriver structure used by Canvas/Legato
    driver_init_name = component.createStringSymbol("DriverInitName", None)
    driver_init_name.setVisible(False)
    driver_init_name.setReadOnly(True)
    driver_init_name.setDefaultValue("xlcdcDisplayDriver")
    # Layer Counter
    total_num_layers = component.createIntegerSymbol("TotalNumLayers", None)
    total_num_layers.setLabel("Total Layers")
    total_num_layers.setDescription("Number of hardware layers enabled")
    total_num_layers.setDefaultValue(4 if ovr2_available else 3)
    total_num_layers.setVisible(False)
    # Initialize clocks inside XLCDC PLIB
    global plib_clk_en
    plib_clk_en = component.createBooleanSymbol("XLCDCClockEnable", None)
    plib_clk_en.setLabel("Clock Enable")
    plib_clk_en.setDescription("Enable Clocks in PLIB")
    plib_clk_en.setDefaultValue(show_clk_help)
    plib_clk_en.setVisible(False)
    plib_clk_en.setDependencies(clk_help_cb, ["core." + symbol_clk_en, "core." + symbol_gclk_en, "core." + symbol_lvdsc_en, "core." + symbol_lvdspll_en])
    # Overlay 2 Availability
    ovr2_support = component.createBooleanSymbol("SupportOVR2", None)
    ovr2_support.setVisible(False)
    ovr2_support.setReadOnly(True)
    ovr2_support.setDefaultValue(ovr2_available)
    # ARM NEON Availability
    neon_support = component.createBooleanSymbol("SupportNEON", None)
    neon_support.setVisible(False)
    neon_support.setReadOnly(True)
    neon_support.setDefaultValue(arm_neon)
    # LVDSPLL Index value for PLL control
    lvdspll_index = component.createIntegerSymbol("LVDSPLLIndex", None)
    lvdspll_index.setDefaultValue(lvdspll_updt_id)
    lvdspll_index.setVisible(False)
    # LVDSPLL Lock Mask
    lvdspll_mask = component.createStringSymbol("LVDSPLLISRMask", None)
    lvdspll_mask.setDefaultValue(lvdspll_isr0_mask)
    lvdspll_mask.setVisible(False)

    # RTOS Menu
    rtos_menu = component.createMenuSymbol("RTOSMenu", None)
    rtos_menu.setLabel("RTOS Settings")
    rtos_menu.setDescription("RTOS Settings")
    rtos_menu.setVisible(Database.getSymbolValue("HarmonyCore", "SELECT_RTOS") != "BareMetal")
    rtos_menu.setDependencies(rtos_dep_changed, ["HarmonyCore.SELECT_RTOS"])

    rtos_task_size = component.createIntegerSymbol("RTOSTaskSize", rtos_menu)
    rtos_task_size.setLabel("Stack Size")
    rtos_task_size.setDefaultValue(1024)

    rtos_task_priority = component.createIntegerSymbol("RTOSTaskPriority", rtos_menu)
    rtos_task_priority.setLabel("Task Priority")
    rtos_task_priority.setDefaultValue(1)

    rtos_enable_task_delay = component.createBooleanSymbol("RTOSEnableTaskDelay", rtos_menu)
    rtos_enable_task_delay.setLabel("Use Task Delay?")
    rtos_enable_task_delay.setDefaultValue(True)

    rtos_task_delay = component.createIntegerSymbol("RTOSTaskDelay", rtos_menu)
    rtos_task_delay.setLabel("Task Delay")
    rtos_task_delay.setDefaultValue(10)

    # Configuration Menus
    # Clock Config Menu
    clock_menu = component.createMenuSymbol("XLCDCClockMenu", None)
    clock_menu.setLabel("Clock Control")
    clock_menu.setDescription("XLCDC Clock Control Settings.")
    clock_menu.setVisible(True)

    # Clock Config Helper Availability Hint
    global clock_gclk_hint
    clock_gclk_hint = component.createCommentSymbol("XClockGCLKHint", clock_menu)
    clock_gclk_hint.setLabel("GCLK is managed by Clock Manager")
    clock_gclk_hint.setVisible(not show_clk_help)
    clock_gclk_hint.setDependencies(clk_show_sym_inv, ["XLCDCClockEnable"])

    # XLCDC Pixel Clock Source Selection
    global clock_src_sel
    clock_src_sel = component.createComboSymbol("XClkGenSel", clock_menu, ["GCLK", "LVDSPLL"])
    clock_src_sel.setLabel("Clock Generator")
    clock_src_sel.setDefaultValue("GCLK")
    clock_src_sel.setDependencies(clk_show_sym, ["XLCDCClockEnable"])

    # GCLK Menu
    clock_gclk_menu = component.createMenuSymbol("XClockGCLKMenu", clock_menu)
    clock_gclk_menu.setLabel("GCLK Clock")
    clock_gclk_menu.setDescription("XLCDC GCLK Clock Control Settings.")
    clock_gclk_menu.setVisible(show_clk_help)
    clock_gclk_menu.setDependencies(clk_show_sym, ["XLCDCClockEnable", "XClkGenSel"])

    # GCLK Source Selection
    gclk_source = component.createKeyValueSetSymbol("XClockGCLKSource", clock_gclk_menu)
    gclk_source.setLabel("Source")
    gclk_source.setDescription("Generic Clock Source.")
    gclk_source.addKey(gck_src_0, gck_src_0_reg, gck_src_0_label)
    gclk_source.addKey(gck_src_1, gck_src_1_reg, gck_src_1_label)
    gclk_source.setOutputMode("Value")
    gclk_source.setDisplayMode("Description")
    gclk_source.setDefaultValue(0)

    # GCLK Source Value Hint
    gclk_src_hint = component.createIntegerSymbol("XClockGCLKSrcHint", clock_gclk_menu)
    gclk_src_hint.setLabel("Input Clock (Hz)")
    gclk_src_hint.setDescription("GCLK Input Clock.")
    gclk_src_hint.setDefaultValue(Database.getSymbolValue("core", symbol_gck_src_0_freq))
    gclk_src_hint.setReadOnly(True)
    gclk_src_hint.setDependencies(update_gclk_clock, ["XClockGCLKSource", "core." + symbol_gck_src_0_freq, "core." + symbol_gck_src_1_freq])

    # GCLK Divider
    gclk_div = component.createIntegerSymbol("XClockGCLKDiv", clock_gclk_menu)
    gclk_div.setLabel("Divider")
    gclk_div.setDescription("Generic Clock Divider.")
    gclk_div.setMin(1)
    gclk_div.setMax(256)
    gclk_div.setDefaultValue(8)

    # GCLK Output Value Hint
    if not show_clk_help:
        gclk_out_hint_hz = Database.getSymbolValue("core", symbol_gck_freq)
    else:
        gclk_out_hint_hz = gclk_src_hint.getValue() / gclk_div.getValue()
    gclk_out_hint = component.createIntegerSymbol("XClockGCLKOutHint", clock_gclk_menu)
    gclk_out_hint.setLabel("Pixel Clock (Hz)")
    gclk_out_hint.setDescription("GCLK Output Clock, i.e. Pixel Clock to XLCDC.")
    gclk_out_hint.setDefaultValue(gclk_out_hint_hz)
    gclk_out_hint.setReadOnly(True)
    gclk_out_hint.setDependencies(update_gclk_clock, ["XClockGCLKSrcHint", "XClockGCLKDiv", "core." + symbol_clk_en, "core." + symbol_gck_freq])

    # GCLK Pixel Clock Validity Hint
    gclk_valid_hint = component.createCommentSymbol("XClockGCLKValidHint", clock_gclk_menu)
    gclk_valid_hint.setLabel("[Warning! Must not exceed 75 MHz]")
    gclk_valid_hint.setVisible(False)
    gclk_valid_hint.setDependencies(show_gclk_valid_hint, ["XClockGCLKOutHint"])
    # Update start-up state by manually triggering the dependency callback
    show_gclk_valid_hint(gclk_valid_hint, {"value": gclk_out_hint.getValue()})

    # LVDSPLL Clock Menu
    clock_lvdspll_menu = component.createMenuSymbol("XClockLVDSPLLMenu", clock_menu)
    clock_lvdspll_menu.setLabel("LVDSPLL Clock")
    clock_lvdspll_menu.setDescription("XLCDC LVDSPLL Clock Control Settings.")
    clock_lvdspll_menu.setVisible(False)
    clock_lvdspll_menu.setDependencies(clk_show_sym, ["XLCDCClockEnable", "XClkGenSel"])

    # Source Clock Hint
    cm_lvdspll_src = component.createIntegerSymbol("LVDSClockSrcHint", clock_lvdspll_menu)
    cm_lvdspll_src.setLabel("Source Clock (Hz)")
    cm_lvdspll_src.setDescription("LVDSPLL Input Clock (S).")
    cm_lvdspll_src.setDefaultValue(Database.getSymbolValue("core", "CLK_MOSCXT_FREQ"))
    cm_lvdspll_src.setReadOnly(True)
    cm_lvdspll_src.setDependencies(lvdspll_update_clocks, ["core.CLK_MOSCXT_FREQ"])

    # LVDSPLL Multiplier
    cm_lvdspll_mul = component.createIntegerSymbol("LVDSClockMul", clock_lvdspll_menu)
    cm_lvdspll_mul.setLabel("Muliplier")
    cm_lvdspll_mul.setDescription("LVDSPLL Clock Multiplier (M).")
    cm_lvdspll_mul.setMin(1)
    cm_lvdspll_mul.setMax(256)
    cm_lvdspll_mul.setDefaultValue(29)

    # LVDSPLL Fractional Factor
    cm_lvdspll_frac = component.createIntegerSymbol("LVDSClockFrac", clock_lvdspll_menu)
    cm_lvdspll_frac.setLabel("Fractional Factor")
    cm_lvdspll_frac.setDescription("LVDSPLL Fractional Multiplication Factor (F).")
    cm_lvdspll_frac.setMin(0)
    cm_lvdspll_frac.setMax(2**22 - 1)
    cm_lvdspll_frac.setDefaultValue(699051)

    # LVDSPLL Core Clock Validity Hint
    lvdspll_valid_hint = component.createCommentSymbol("LVDSCoreValidHint", clock_lvdspll_menu)
    lvdspll_valid_hint.setLabel("[Ok!]")
    lvdspll_valid_hint.setVisible(False)
    lvdspll_valid_hint.setDependencies(lvdspll_core_valid_hint, ["LVDSClockSrcHint", "LVDSClockMul", "LVDSClockFrac"])

    # LVDSPLL Divider
    lvdspll_divpmc = component.createIntegerSymbol("LVDSClockDivPMC", clock_lvdspll_menu)
    lvdspll_divpmc.setLabel("Divider")
    lvdspll_divpmc.setDescription("LVDSPLL Clock Divider (D).")
    lvdspll_divpmc.setMin(1)
    lvdspll_divpmc.setMax(256)
    lvdspll_divpmc.setDefaultValue(4)

    # LVDSPLL Clock Output Hint
    lvdspll_out_hint_val = int(cm_lvdspll_src.getValue() * (cm_lvdspll_mul.getValue() + (float(cm_lvdspll_frac.getValue()) / 2**22)) / lvdspll_divpmc.getValue())
    lvdspll_out_hint = component.createIntegerSymbol("LVDSClockOutHint", clock_lvdspll_menu)
    lvdspll_out_hint.setLabel("Output Clock (Hz)")
    lvdspll_out_hint.setDescription("<html>LVDSPLL Output Clock, i.e. Input Clock to LVDSC (O).<br>O = S * (M + (F / 2^22)) / D</html>")
    lvdspll_out_hint.setDefaultValue(lvdspll_out_hint_val)
    lvdspll_out_hint.setReadOnly(True)
    lvdspll_out_hint.setDependencies(lvdspll_update_clocks, ["LVDSClockSrcHint", "LVDSClockMul", "LVDSClockFrac", "LVDSClockDivPMC"])

    # LVDSPLL Clock Validity Hint
    lvdspll_valid_hint = component.createCommentSymbol("LVDSClockValidHint", clock_lvdspll_menu)
    lvdspll_valid_hint.setLabel("[Ok!]")
    lvdspll_valid_hint.setVisible(False)
    lvdspll_valid_hint.setDependencies(lvdspll_out_valid_hint, ["LVDSClockOutHint"])

    # XLCDC Pixel Clock through LVDSPLL Hint
    xlcdc_pix_hint = component.createIntegerSymbol("XLCDCPixClockHint", clock_lvdspll_menu)
    xlcdc_pix_hint.setLabel("Pixel Clock (Hz)")
    xlcdc_pix_hint.setDescription("<html>XLCDC Pixel Clock (P).<br>P = O / 7</html>")
    xlcdc_pix_hint.setDefaultValue(lvdspll_out_hint_val / 7)
    xlcdc_pix_hint.setReadOnly(True)
    xlcdc_pix_hint.setDependencies(update_xlcdc_pix_hint, ["LVDSClockOutHint"])

    # LVDSPLL Pixel Clock Validity Hint
    pixclk_valid_hint = component.createCommentSymbol("XClockLVDSPLLValidHint", clock_lvdspll_menu)
    pixclk_valid_hint.setLabel("[Warning! Must not exceed 75 MHz]")
    pixclk_valid_hint.setVisible(False)
    pixclk_valid_hint.setDependencies(show_gclk_valid_hint, ["XLCDCPixClockHint"])
    # Update start-up state by manually triggering the dependency callback
    show_gclk_valid_hint(pixclk_valid_hint, {"value": xlcdc_pix_hint.getValue()})

    # PWM Clock Menu
    clock_pwm_menu = component.createMenuSymbol("XClockPWMMenu", clock_menu)
    clock_pwm_menu.setLabel("PWM Clock")
    clock_pwm_menu.setDescription("XLCDC PWM Clock Control Settings.")
    clock_pwm_menu.setVisible(True)

    pwm_clock_src = component.createKeyValueSetSymbol("XPWMClockSrc", clock_pwm_menu)
    pwm_clock_src.setLabel("PWM Clock Source")
    pwm_clock_src.setOutputMode("Value")
    pwm_clock_src.setDisplayMode("Description")
    pwm_clock_src.setDescription("XLCDC PWM Clock Source Selection.")
    pwm_clock_src.addKey("SCK", "LCDC_LCDCFG0_CLKPWMSEL_0_Val", "Slow Clock")
    pwm_clock_src.addKey("MCK", "LCDC_LCDCFG0_CLKPWMSEL_1_Val", "MCK")
    pwm_clock_src.setDefaultValue(0)

    pwm_prescaler = component.createKeyValueSetSymbol("XPWMClockPres", clock_pwm_menu)
    pwm_prescaler.setLabel("PWM Clock Prescaler")
    pwm_prescaler.setOutputMode("Value")
    pwm_prescaler.setDisplayMode("Description")
    pwm_prescaler.setDescription("XLCDC PWM Clock Prescaler.")
    for x in range(16):
        id_desc = "DIV_" + str(2**x)
        pwm_prescaler.addKey(id_desc, str(x), id_desc)
    pwm_prescaler.setDefaultValue(0)

    # GCLK Output Value Hint
    pwm_freq_hint = component.createIntegerSymbol("XPWMClockFreqHint", clock_pwm_menu)
    pwm_freq_hint.setLabel("PWM Frequnecy (Hz)")
    pwm_freq_hint.setDescription("PWM Output Frequnecy.")
    pwm_freq_hint.setDefaultValue(32768)
    pwm_freq_hint.setReadOnly(True)
    pwm_freq_hint.setDependencies(update_pwm_clock, ["XPWMClockSrc", "XPWMClockPres", "core." + symbol_mck_freq])

    pwm_enable = component.createBooleanSymbol("XPWMEnable", clock_pwm_menu)
    pwm_enable.setLabel("Enable Backlight PWM")
    pwm_enable.setDescription("Enables the Backlight PWM Output")
    pwm_enable.setDefaultValue(False)

    pwm_brightness = component.createIntegerSymbol("XPWMBrigtness", clock_pwm_menu)
    pwm_brightness.setLabel("Startup Brightness")
    pwm_brightness.setDescription("Sets the PWM Startup Brightness")
    pwm_brightness.setDefaultValue(255)
    pwm_brightness.setMin(0)
    pwm_brightness.setMax(255)
    pwm_brightness.setVisible(pwm_enable.getValue())
    pwm_brightness.setDependencies(show_pwm_brightness, ["XPWMEnable"])

    # Timing Config Config Menu
    timing_menu = component.createMenuSymbol("XLCDCTimingMenu", None)
    timing_menu.setLabel("Timing Configuration")
    timing_menu.setDescription("XLCDC Timing Configuration.")
    timing_menu.setVisible(True)

    # Resolution
    timing_menu_res = component.createMenuSymbol("XTResMenu", timing_menu)
    timing_menu_res.setLabel("Display Resolution")
    timing_menu_res.setDescription("Contains the display's resolution.")

    timing_menu_res_x = component.createIntegerSymbol("XTResPPL", timing_menu_res)
    timing_menu_res_x.setLabel("Width")
    timing_menu_res_x.setDescription("The horizontal width in pixels.")
    timing_menu_res_x.setDefaultValue(1)
    timing_menu_res_x.setMin(1)
    timing_menu_res_x.setMax(2**11)

    global timing_menu_res_y
    timing_menu_res_y = component.createIntegerSymbol("XTResRPF", timing_menu_res)
    timing_menu_res_y.setLabel("Height")
    timing_menu_res_y.setDescription("The vertical height in pixels")
    timing_menu_res_y.setDefaultValue(1)
    timing_menu_res_y.setMin(1)
    timing_menu_res_y.setMax(2**11)

    # Horizontal Timings
    timing_menu_hor = component.createMenuSymbol("XTHorMenu", timing_menu)
    timing_menu_hor.setLabel("Horizontal Timings")
    timing_menu_hor.setDescription("Contains the display's horizontal timing parameters.")

    timing_menu_hor_pw = component.createIntegerSymbol("XTHorPW", timing_menu_hor)
    timing_menu_hor_pw.setLabel("Horizontal Pulse Width")
    timing_menu_hor_pw.setDescription("The horizontal pulse width cycles.")
    timing_menu_hor_pw.setDefaultValue(1)
    timing_menu_hor_pw.setMin(1)
    timing_menu_hor_pw.setMax(2**10)

    timing_menu_hor_bp = component.createIntegerSymbol("XTHorBP", timing_menu_hor)
    timing_menu_hor_bp.setLabel("Horizontal Back Porch")
    timing_menu_hor_bp.setDescription("The horizontal back porch size in pixels.")
    timing_menu_hor_bp.setDefaultValue(1)
    timing_menu_hor_bp.setMin(1)
    timing_menu_hor_bp.setMax(2**10)

    timing_menu_hor_fp = component.createIntegerSymbol("XTHorFP", timing_menu_hor)
    timing_menu_hor_fp.setLabel("Horizontal Front Porch")
    timing_menu_hor_fp.setDescription("The horizontal front porch size in pixels.")
    timing_menu_hor_fp.setDefaultValue(1)
    timing_menu_hor_fp.setMin(1)
    timing_menu_hor_fp.setMax(2**10)

    # Vertical Timings
    timing_menu_ver = component.createMenuSymbol("XTVerMenu", timing_menu)
    timing_menu_ver.setLabel("Vertical Timings")
    timing_menu_ver.setDescription("Contains the display's vertical timing parameters.")

    timing_menu_ver_pw = component.createIntegerSymbol("XTVerPW", timing_menu_ver)
    timing_menu_ver_pw.setLabel("Vertical Pulse Width")
    timing_menu_ver_pw.setDescription("The vertical pulse width.")
    timing_menu_ver_pw.setDefaultValue(1)
    timing_menu_ver_pw.setMin(1)
    timing_menu_ver_pw.setMax(2**10)

    timing_menu_ver_bp = component.createIntegerSymbol("XTVerBP", timing_menu_ver)
    timing_menu_ver_bp.setLabel("Vertical Back Porch")
    timing_menu_ver_bp.setDescription("The vertical back porch size in pixels.")
    timing_menu_ver_bp.setDefaultValue(1)
    timing_menu_ver_bp.setMin(1)
    timing_menu_ver_bp.setMax(2**10)

    timing_menu_ver_fp = component.createIntegerSymbol("XTVerFP", timing_menu_ver)
    timing_menu_ver_fp.setLabel("Vertical Front Porch")
    timing_menu_ver_fp.setDescription("The vertical front porch size in pixels.")
    timing_menu_ver_fp.setDefaultValue(1)
    timing_menu_ver_fp.setMin(1)
    timing_menu_ver_fp.setMax(2**10)

    # Polarity
    timing_menu_pol = component.createMenuSymbol("XTPolMenu", timing_menu)
    timing_menu_pol.setLabel("Polarity")
    timing_menu_pol.setDescription("Configure polarity signals.")

    tm_pol_de_pol = component.createBooleanSymbol("XTPolDISP", timing_menu_pol)
    tm_pol_de_pol.setLabel("DISP Active High")
    tm_pol_de_pol.setDescription("Indicates the need for positive DISP signal polarity.")
    tm_pol_de_pol.setDefaultValue(True)

    tm_pol_hsync_pol = component.createBooleanSymbol("XTPolHSyncNegative", timing_menu_pol)
    tm_pol_hsync_pol.setLabel("HSYNC Active Low")
    tm_pol_hsync_pol.setDescription("Indicates the need for negative HSYNC polarity.")
    tm_pol_hsync_pol.setDefaultValue(False)

    tm_pol_vsync_pol = component.createBooleanSymbol("XTPolVSyncNegative", timing_menu_pol)
    tm_pol_vsync_pol.setLabel("VSYNC Active Low")
    tm_pol_vsync_pol.setDescription("Indicates the need for negative VSYNC polarity.")
    tm_pol_vsync_pol.setDefaultValue(False)

    # Guard Frames
    timing_menu_guard = component.createMenuSymbol("XTGrdMenu", timing_menu)
    timing_menu_guard.setLabel("Guard Frames")
    timing_menu_guard.setDescription("Configure Guard Frames.")

    tm_guard_count = component.createIntegerSymbol("XTGrdCount", timing_menu_guard)
    tm_guard_count.setLabel("Count")
    tm_guard_count.setDescription("Number of frames inserted during start-up before asserting the DISP signal.")
    tm_guard_count.setDefaultValue(1)
    tm_guard_count.setMin(0)
    tm_guard_count.setMax(255)

    # Layer Config
    layer_menu = component.createMenuSymbol("XLCDCLayerMenu", None)
    layer_menu.setLabel("Layer Configuration")
    layer_menu.setDescription("Configures the XLCDC Layers.")
    layer_menu.setVisible(True)

    # Layer Enable Menu
    layer_menu_enable = component.createMenuSymbol("XLMEnableMenu", layer_menu)
    layer_menu_enable.setLabel("Enable Layers")
    layer_menu_enable.setDescription("Enable or Disable XLCDC Layers.")
    layer_menu_enable.setVisible(True)

    lm_enable_base = component.createBooleanSymbol("XLMEnableBase", layer_menu_enable)
    lm_enable_base.setLabel("Base Layer")
    lm_enable_base.setDescription("XLCDC Base Layer.")
    lm_enable_base.setDefaultValue(True)
    lm_enable_base.setReadOnly(True)

    lm_enable_ovr1 = component.createBooleanSymbol("XLMEnableOVR1", layer_menu_enable)
    lm_enable_ovr1.setLabel("Overlay 1")
    lm_enable_ovr1.setDescription("XLCDC Overlay 1 Layer.")
    lm_enable_ovr1.setDefaultValue(True)
    lm_enable_ovr1.setVisible(True)
    lm_enable_ovr1.setDependencies(on_layer_enable, ["XLMEnableOVR1"])

    lm_enable_heo = component.createBooleanSymbol("XLMEnableHEO", layer_menu_enable)
    lm_enable_heo.setLabel("High-End Overlay (HEO)")
    lm_enable_heo.setDescription("XLCDC High-End Overlay Layer.")
    lm_enable_heo.setDefaultValue(True)
    lm_enable_heo.setDependencies(on_layer_enable, ["XLMEnableHEO"])

    global lm_heo_vidpri
    lm_heo_vidpri = component.createBooleanSymbol("XLMHEOVIDPRI", lm_enable_heo)
    lm_heo_vidpri.setLabel("Prioritize")
    lm_heo_vidpri.setDescription("Draws HEO Layer above OVR1 Layer if enabled.")
    lm_heo_vidpri.setDefaultValue(True)

    global lm_heo_ycbcr_en
    lm_heo_ycbcr_en = component.createBooleanSymbol("XLMHEOYCbCrEN", lm_enable_heo)
    lm_heo_ycbcr_en.setLabel("YCbCr Mode")
    lm_heo_ycbcr_en.setDescription("Initialize HEO in YCbCr Mode.")
    lm_heo_ycbcr_en.setDefaultValue(False)
    lm_heo_ycbcr_en.setDependencies(on_layer_enable, ["XLMHEOYCbCrEN"])

    lm_enable_ovr2 = component.createBooleanSymbol("XLMEnableOVR2", layer_menu_enable)
    lm_enable_ovr2.setLabel("Overlay 2")
    lm_enable_ovr2.setDescription("XLCDC Overlay 2 Layer.")
    lm_enable_ovr2.setDefaultValue(ovr2_available)
    lm_enable_ovr2.setVisible(ovr2_available)
    lm_enable_ovr2.setDependencies(on_layer_enable, ["XLMEnableOVR2"])

    lm_col_mode = component.createKeyValueSetSymbol("XLMColMode", layer_menu)
    lm_col_mode.setLabel("Color Mode")
    lm_col_mode.setOutputMode("Value")
    lm_col_mode.setDisplayMode("Description")
    lm_col_mode.setDescription("XLCDC Layer Default Color Mode for All Layers.")
    lm_col_mode.addKey("0", "3", "RGB_565")
    lm_col_mode.addKey("1", "13", "RGBA_8888")
    lm_col_mode.setDefaultValue(1)

    # Driver Options
    driver_menu = component.createMenuSymbol("XLCDCDrvMenu", None)
    driver_menu.setLabel("Driver Options")
    driver_menu.setDescription("Configures the XLCDC Driver Options.")
    driver_menu.setVisible(True)

    driver_color_fb = component.createHexSymbol("XDMColorFB", driver_menu)
    driver_color_fb.setLabel("Frame Buffer Fill Color")
    driver_color_fb.setDescription("Frambefuffers are filled with specified color on initialization.")
    driver_color_fb.setMin(0x0)
    driver_color_fb.setMax(0xFFFFFFFF)
    driver_color_fb.setDefaultValue(0x0)
    driver_color_fb.setHexOutputMode(True)
    driver_color_fb.setVisible(True)

    driver_cpu_blit = component.createBooleanSymbol("XDMCPUBilt", driver_menu)
    driver_cpu_blit.setLabel("Force CPU Blit")
    driver_cpu_blit.setDescription("Use CPU to blit, even if a GPU is available.")
    driver_cpu_blit.setDefaultValue(False)
    driver_cpu_blit.setVisible(True)

    driver_cache_fb = component.createBooleanSymbol("XDMCacheFB", driver_menu)
    driver_cache_fb.setLabel("Cacheable Frame Buffers")
    driver_cache_fb.setDescription("Uses cacheable frame buffers if enabled.")
    driver_cache_fb.setDefaultValue(False)
    driver_cache_fb.setVisible(True)

    global canvas_mode
    canvas_mode = component.createBooleanSymbol("CanvasModeOnly", driver_menu)
    canvas_mode.setLabel("Canvas Mode")
    canvas_mode.setDefaultValue(False)
    canvas_mode.setVisible(True)
    canvas_mode.setDescription("Exposes hardware layer features and control.")

    # Double Buffering
    double_buffering = component.createBooleanSymbol("DoubleBuffering", driver_menu)
    double_buffering.setLabel("Double Buffering")
    double_buffering.setDefaultValue(False)
    double_buffering.setVisible(True)
    double_buffering.setDescription(
        "<html>Uses an additional buffer for off-screen drawing.<br>Eliminates screen tearing but doubles the required memory.<br>Only works when not in Canvas Mode.</html>"
    )
    double_buffering.setDependencies(db_manage_sym, ["CanvasModeOnly", "DoubleBuffering"])

    db_rect_opt = component.createComboSymbol("DBRectOpt", double_buffering, ["Basic", "Simple", "Smart"])
    db_rect_opt.setLabel("Sync Optimization")
    db_rect_opt.setDefaultValue("Basic")
    db_rect_opt.setVisible(False)
    db_rect_opt.setDependencies(show_db_opts, ["DoubleBuffering"])
    db_rect_opt.setDescription(
        "<html>Optimizes rectangles during synchronization.<br> Basic - No optimization, just tracks dirty rectangles and blits them<br> Simple - Merges dirty rectangles if close (within pixel threshold) or adjacent<br> Smart - Like Simple, but calculates thresholds dynamically</html>"
    )

    db_max_rects = component.createIntegerSymbol("DBROMaxRects", db_rect_opt)
    db_max_rects.setLabel("Sync Buffer Size")
    db_max_rects.setDescription("Specifies the maximum number of dirty rectangles buffered; beyond it, the entire screen syncs.")
    db_max_rects.setDefaultValue(32)
    db_max_rects.setMin(32)
    db_max_rects.setMax(256)
    db_max_rects.setVisible(True)

    global db_merge_threshold
    db_merge_threshold = component.createIntegerSymbol("DBROMergeThrBase", db_rect_opt)
    db_merge_threshold.setLabel("Merge Threshold (px)")
    db_merge_threshold.setDescription("Merges rectangles if they within the pixel threshold.")
    db_merge_threshold.setDefaultValue(10)
    db_merge_threshold.setMin(1)
    db_merge_threshold.setMax(50)
    db_merge_threshold.setVisible(False)
    db_merge_threshold.setDependencies(show_db_opts, ["DBRectOpt"])

    global db_merge_thr_min
    db_merge_thr_min = component.createIntegerSymbol("DBROMergeThrMin", db_rect_opt)
    db_merge_thr_min.setLabel("Minimum Threshold (px)")
    db_merge_thr_min.setDescription("Minimum Threshold for Smart Sync Optimization.")
    db_merge_thr_min.setDefaultValue(1)
    db_merge_thr_min.setMin(1)
    db_merge_thr_min.setMax(50)
    db_merge_thr_min.setVisible(False)
    db_merge_thr_min.setDependencies(show_db_opts, ["DBRectOpt"])

    global db_merge_thr_max
    db_merge_thr_max = component.createIntegerSymbol("DBROMergeThrMax", db_rect_opt)
    db_merge_thr_max.setLabel("Maximum Threshold (px)")
    db_merge_thr_max.setDescription("Maximum Threshold for Smart Sync Optimization.")
    db_merge_thr_max.setDefaultValue(10)
    db_merge_thr_max.setMin(1)
    db_merge_thr_max.setMax(50)
    db_merge_thr_max.setVisible(False)
    db_merge_thr_max.setDependencies(show_db_opts, ["DBRectOpt"])

    global db_merge_ohr
    db_merge_ohr = component.createIntegerSymbol("DBROMergeOHR", db_rect_opt)
    db_merge_ohr.setLabel("Overhead Ratio (%)")
    db_merge_ohr.setDescription("Acceptable extra pixels in percentage, beyond this merge will abort.")
    db_merge_ohr.setDefaultValue(30)
    db_merge_ohr.setMin(1)
    db_merge_ohr.setMax(50)
    db_merge_ohr.setVisible(False)
    db_merge_ohr.setDependencies(show_db_opts, ["DBRectOpt"])

    # Interrupt Configuration
    global interrupt_enable
    interrupt_enable = component.createBooleanSymbol("InterruptEnable", driver_menu)
    interrupt_enable.setLabel("Enable Interrupts")
    interrupt_enable.setDefaultValue(False)
    interrupt_enable.setVisible(True)
    interrupt_enable.setDescription("Enables LCDC Interrupts.")

    global int_en_sof
    int_en_sof = component.createBooleanSymbol("IntEnSof", interrupt_enable)
    int_en_sof.setLabel("Start of Frame")
    int_en_sof.setDefaultValue(False)
    int_en_sof.setVisible(False)
    int_en_sof.setDescription("Enables LCDC Start of Frame Interrupt.")
    int_en_sof.setDependencies(int_show_sym, ["InterruptEnable"])

    int_en_row = component.createBooleanSymbol("IntEnRow", interrupt_enable)
    int_en_row.setLabel("Row Interrupt")
    int_en_row.setDefaultValue(False)
    int_en_row.setVisible(False)
    int_en_row.setDescription("Enables Row Interrupt.")
    int_en_row.setDependencies(int_show_sym, ["InterruptEnable"])

    global int_row_num
    int_row_num = component.createIntegerSymbol("IntRowNum", int_en_row)
    int_row_num.setLabel("Row No.")
    int_row_num.setDescription(
        "<html>Triggers Row Interrupt on Specified Line Number<br>Setting it to 0 triggers an interrupt when the first line is displayed<br>Setting it to (Height - 1) triggers an interrupt when the last line is displayed</html>"
    )
    int_row_num.setDefaultValue(0)
    int_row_num.setMin(0)
    int_row_num.setMax(2**11 - 1)
    int_row_num.setVisible(False)
    int_row_num.setDependencies(int_show_sym, ["IntEnRow"])

    int_en_fifo = component.createBooleanSymbol("IntEnFifoError", interrupt_enable)
    int_en_fifo.setLabel("Output FIFO Error")
    int_en_fifo.setDefaultValue(False)
    int_en_fifo.setVisible(False)
    int_en_fifo.setDescription("Enables Output FIFO Error Interrupt.")
    int_en_fifo.setDependencies(int_show_sym, ["InterruptEnable"])

    int_en_base = component.createBooleanSymbol("IntEnBase", interrupt_enable)
    int_en_base.setLabel("BASE Layer Interrupts")
    int_en_base.setDefaultValue(False)
    int_en_base.setVisible(False)
    int_en_base.setDescription("Enables Layer Interrupts.")
    int_en_base.setDependencies(int_show_sym, ["InterruptEnable"])

    int_en_base_end = component.createBooleanSymbol("IntEnBaseEnd", int_en_base)
    int_en_base_end.setLabel("EOF DMA Transfer")
    int_en_base_end.setDefaultValue(False)
    int_en_base_end.setVisible(False)
    int_en_base_end.setDescription("Enables End of Frame DMA Transfer Interrupt.")
    int_en_base_end.setDependencies(int_show_sym, ["IntEnBase"])

    int_en_base_error = component.createBooleanSymbol("IntEnBaseError", int_en_base)
    int_en_base_error.setLabel("Transfer Error")
    int_en_base_error.setDefaultValue(False)
    int_en_base_error.setVisible(False)
    int_en_base_error.setDescription("Enables Transfer Error Interrupt.")
    int_en_base_error.setDependencies(int_show_sym, ["IntEnBase"])

    int_en_base_ovf = component.createBooleanSymbol("IntEnBaseOvf", int_en_base)
    int_en_base_ovf.setLabel("Overflow")
    int_en_base_ovf.setDefaultValue(False)
    int_en_base_ovf.setVisible(False)
    int_en_base_ovf.setDescription("Enables Overflow Interrupt.")
    int_en_base_ovf.setDependencies(int_show_sym, ["IntEnBase"])

    int_en_ovr1 = component.createBooleanSymbol("IntEnOvr1", interrupt_enable)
    int_en_ovr1.setLabel("OVR1 Layer Interrupts")
    int_en_ovr1.setDefaultValue(False)
    int_en_ovr1.setVisible(False)
    int_en_ovr1.setDescription("Enables Layer Interrupts.")
    int_en_ovr1.setDependencies(int_show_sym, ["InterruptEnable"])

    int_en_ovr1_end = component.createBooleanSymbol("IntEnOvr1End", int_en_ovr1)
    int_en_ovr1_end.setLabel("EOF DMA Transfer")
    int_en_ovr1_end.setDefaultValue(False)
    int_en_ovr1_end.setVisible(False)
    int_en_ovr1_end.setDescription("Enables End of Frame DMA Transfer Interrupt.")
    int_en_ovr1_end.setDependencies(int_show_sym, ["IntEnOvr1"])

    int_en_ovr1_error = component.createBooleanSymbol("IntEnOvr1Error", int_en_ovr1)
    int_en_ovr1_error.setLabel("Transfer Error")
    int_en_ovr1_error.setDefaultValue(False)
    int_en_ovr1_error.setVisible(False)
    int_en_ovr1_error.setDescription("Enables Transfer Error Interrupt.")
    int_en_ovr1_error.setDependencies(int_show_sym, ["IntEnOvr1"])

    int_en_ovr1_ovf = component.createBooleanSymbol("IntEnOvr1Ovf", int_en_ovr1)
    int_en_ovr1_ovf.setLabel("Overflow")
    int_en_ovr1_ovf.setDefaultValue(False)
    int_en_ovr1_ovf.setVisible(False)
    int_en_ovr1_ovf.setDescription("Enables Overflow Interrupt.")
    int_en_ovr1_ovf.setDependencies(int_show_sym, ["IntEnOvr1"])

    int_en_heo = component.createBooleanSymbol("IntEnHeo", interrupt_enable)
    int_en_heo.setLabel("HEO Layer Interrupts")
    int_en_heo.setDefaultValue(False)
    int_en_heo.setVisible(False)
    int_en_heo.setDescription("Enables Layer Interrupts.")
    int_en_heo.setDependencies(int_show_sym, ["InterruptEnable"])

    int_en_heo_end = component.createBooleanSymbol("IntEnHeoEnd", int_en_heo)
    int_en_heo_end.setLabel("EOF DMA Transfer")
    int_en_heo_end.setDefaultValue(False)
    int_en_heo_end.setVisible(False)
    int_en_heo_end.setDescription("Enables End of Frame DMA Transfer Interrupt.")
    int_en_heo_end.setDependencies(int_show_sym, ["IntEnHeo"])

    int_en_heo_error = component.createBooleanSymbol("IntEnHeoError", int_en_heo)
    int_en_heo_error.setLabel("Transfer Error")
    int_en_heo_error.setDefaultValue(False)
    int_en_heo_error.setVisible(False)
    int_en_heo_error.setDescription("Enables Transfer Error Interrupt.")
    int_en_heo_error.setDependencies(int_show_sym, ["IntEnHeo"])

    int_en_heo_ovf = component.createBooleanSymbol("IntEnHeoOvf", int_en_heo)
    int_en_heo_ovf.setLabel("Overflow")
    int_en_heo_ovf.setDefaultValue(False)
    int_en_heo_ovf.setVisible(False)
    int_en_heo_ovf.setDescription("Enables Overflow Interrupt.")
    int_en_heo_ovf.setDependencies(int_show_sym, ["IntEnHeo"])

    if ovr2_available:
        int_en_ovr2 = component.createBooleanSymbol("IntEnOvr2", interrupt_enable)
        int_en_ovr2.setLabel("OVR2 Layer Interrupts")
        int_en_ovr2.setDefaultValue(False)
        int_en_ovr2.setVisible(False)
        int_en_ovr2.setDescription("Enables Layer Interrupts.")
        int_en_ovr2.setDependencies(int_show_sym, ["InterruptEnable"])

        int_en_ovr2_end = component.createBooleanSymbol("IntEnOvr2End", int_en_ovr2)
        int_en_ovr2_end.setLabel("EOF DMA Transfer")
        int_en_ovr2_end.setDefaultValue(False)
        int_en_ovr2_end.setVisible(False)
        int_en_ovr2_end.setDescription("Enables End of Frame DMA Transfer Interrupt.")
        int_en_ovr2_end.setDependencies(int_show_sym, ["IntEnOvr2"])

        int_en_ovr2_error = component.createBooleanSymbol("IntEnOvr2Error", int_en_ovr2)
        int_en_ovr2_error.setLabel("Transfer Error")
        int_en_ovr2_error.setDefaultValue(False)
        int_en_ovr2_error.setVisible(False)
        int_en_ovr2_error.setDescription("Enables Transfer Error Interrupt.")
        int_en_ovr2_error.setDependencies(int_show_sym, ["IntEnOvr2"])

        int_en_ovr2_ovf = component.createBooleanSymbol("IntEnOvr2Ovf", int_en_ovr2)
        int_en_ovr2_ovf.setLabel("Overflow")
        int_en_ovr2_ovf.setDefaultValue(False)
        int_en_ovr2_ovf.setVisible(False)
        int_en_ovr2_ovf.setDescription("Enables Overflow Interrupt.")
        int_en_ovr2_ovf.setDependencies(int_show_sym, ["IntEnOvr2"])

    # Output Configuration
    output_menu = component.createMenuSymbol("XLCDCOutMenu", None)
    output_menu.setLabel("Output Configuration")
    output_menu.setDescription("Configures the XLCDC Output Interface.")
    output_menu.setVisible(True)

    # Output Interface
    output_iface = component.createKeyValueSetSymbol("XOutInterface", output_menu)
    output_iface.setLabel("Interface")
    output_iface.setDescription("XLCDC Output Interface Selection.")
    output_iface.addKey("DPI_ENABLE", "1", "DPI")
    output_iface.addKey("DPI_DISABLE", "0", "Legacy")
    output_iface.setOutputMode("Value")
    output_iface.setDisplayMode("Description")
    output_iface.setDefaultValue(0)
    output_iface.setVisible(True)

    # Output Mode DPI
    output_mode_dpi = component.createKeyValueSetSymbol("XOutDPI", output_menu)
    output_mode_dpi.setLabel("DPI Mode")
    output_mode_dpi.setDescription("XLCDC DPI Output Modes.")
    output_mode_dpi.addKey("OUTPUT_DPI_16BPPCFG1", "0", "16BPP CFG1")
    output_mode_dpi.addKey("OUTPUT_DPI_16BPPCFG2", "1", "16BPP CFG2")
    output_mode_dpi.addKey("OUTPUT_DPI_16BPPCFG3", "2", "16BPP CFG3")
    output_mode_dpi.addKey("OUTPUT_DPI_18BPPCFG1", "3", "18BPP CFG1")
    output_mode_dpi.addKey("OUTPUT_DPI_18BPPCFG2", "4", "18BPP CFG2")
    output_mode_dpi.addKey("OUTPUT_DPI_24BPP", "5", "24BPP")
    output_mode_dpi.setOutputMode("Value")
    output_mode_dpi.setDisplayMode("Description")
    output_mode_dpi.setDefaultValue(5)
    output_mode_dpi.setVisible(True)
    output_mode_dpi.setDependencies(refresh_output_mode, ["XOutInterface"])

    # Output Mode Legacy
    output_mode_legacy = component.createKeyValueSetSymbol("XOutLegacy", output_menu)
    output_mode_legacy.setLabel("Legacy Mode")
    output_mode_legacy.setDescription("Generic Clock Source.")
    output_mode_legacy.addKey("OUTPUT_12BPP", "0", "12BPP RGB")
    output_mode_legacy.addKey("OUTPUT_16BPP", "1", "16BPP RGB")
    output_mode_legacy.addKey("OUTPUT_18BPP", "2", "18BPP RGB")
    output_mode_legacy.addKey("OUTPUT_24BPP", "3", "24BPP RGB")
    output_mode_legacy.setOutputMode("Value")
    output_mode_legacy.setDisplayMode("Description")
    output_mode_legacy.setDefaultValue(3)
    output_mode_legacy.setVisible(False)
    output_mode_legacy.setDependencies(refresh_output_mode, ["XOutInterface"])

    # Generate Files
    sys_definitions_h = component.createFileSymbol("SYS_DEFINITIONS_H", None)
    sys_definitions_h.setType("STRING")
    sys_definitions_h.setOutputName("core.LIST_SYSTEM_DEFINITIONS_H_INCLUDES")
    sys_definitions_h.setSourcePath("templates/definitions.h.ftl")
    sys_definitions_h.setMarkup(True)

    sys_init_c = component.createFileSymbol("SYS_INIT_C", None)
    sys_init_c.setType("STRING")
    sys_init_c.setOutputName("core.LIST_SYSTEM_INIT_C_SYS_INITIALIZE_DRIVERS")
    sys_init_c.setSourcePath("templates/init.c.ftl")
    sys_init_c.setMarkup(True)

    sys_init_plib_c = component.createFileSymbol("SYS_INIT_PLIB_C", None)
    sys_init_plib_c.setType("STRING")
    # Add to peripheral list to init before XLCDC driver
    sys_init_plib_c.setOutputName("core.LIST_SYSTEM_INIT_C_SYS_INITIALIZE_PERIPHERALS")
    sys_init_plib_c.setSourcePath("templates/plib/init.c.ftl")
    sys_init_plib_c.setMarkup(True)

    sys_task_c = component.createFileSymbol("SYS_TASK_C", None)
    sys_task_c.setType("STRING")
    sys_task_c.setOutputName("core.LIST_SYSTEM_TASKS_C_CALL_DRIVER_TASKS")
    sys_task_c.setSourcePath("templates/tasks.c.ftl")
    sys_task_c.setMarkup(True)

    sys_rtos_task_c = component.createFileSymbol("SYS_RTOS_TASK_C", None)
    sys_rtos_task_c.setType("STRING")
    sys_rtos_task_c.setOutputName("core.LIST_SYSTEM_RTOS_TASKS_C_DEFINITIONS")
    sys_rtos_task_c.setSourcePath("templates/rtos_tasks.c.ftl")
    sys_rtos_task_c.setMarkup(True)

    gfx_driver_h = component.createFileSymbol("GFX_DRIVER_H", None)
    gfx_driver_h.setDestPath("gfx/driver/")
    gfx_driver_h.setSourcePath("../../templates/gfx_driver.h.ftl")
    gfx_driver_h.setOutputName("gfx_driver.h")
    gfx_driver_h.setProjectPath(project_path)
    gfx_driver_h.setType("HEADER")
    gfx_driver_h.setMarkup(True)

    gfx_driver_c = component.createFileSymbol("GFX_DRIVER_C", None)
    gfx_driver_c.setDestPath("gfx/driver/")
    gfx_driver_c.setSourcePath("../../templates/gfx_driver.c.ftl")
    gfx_driver_c.setOutputName("gfx_driver.c")
    gfx_driver_c.setProjectPath(project_path)
    gfx_driver_c.setType("SOURCE")
    gfx_driver_c.setMarkup(True)

    plib_xlcdc_h = component.createFileSymbol("PLIB_XLCDC_H", None)
    plib_xlcdc_h.setDestPath("gfx/driver/controller/xlcdc/plib")
    plib_xlcdc_h.setSourcePath("templates/plib/plib_xlcdc.h.ftl")
    plib_xlcdc_h.setOutputName("plib_xlcdc.h")
    plib_xlcdc_h.setProjectPath(project_path)
    plib_xlcdc_h.setType("HEADER")
    plib_xlcdc_h.setMarkup(True)

    plib_xldcd_c = component.createFileSymbol("PLIB_XLCDC_C", None)
    plib_xldcd_c.setDestPath("gfx/driver/controller/xlcdc/plib")
    plib_xldcd_c.setSourcePath("templates/plib/plib_xlcdc.c.ftl")
    plib_xldcd_c.setOutputName("plib_xlcdc.c")
    plib_xldcd_c.setProjectPath(project_path)
    plib_xldcd_c.setType("SOURCE")
    plib_xldcd_c.setMarkup(True)

    drv_xlcdc_h = component.createFileSymbol("DRV_XLCDC_H", None)
    drv_xlcdc_h.setDestPath("gfx/driver/controller/xlcdc/")
    drv_xlcdc_h.setSourcePath("templates/drv_gfx_xlcdc.h.ftl")
    drv_xlcdc_h.setOutputName("drv_gfx_xlcdc.h")
    drv_xlcdc_h.setProjectPath(project_path)
    drv_xlcdc_h.setType("HEADER")
    drv_xlcdc_h.setMarkup(True)

    drv_xldcd_c = component.createFileSymbol("DRV_XLCDC_C", None)
    drv_xldcd_c.setDestPath("gfx/driver/controller/xlcdc/")
    drv_xldcd_c.setSourcePath("templates/drv_gfx_xlcdc.c.ftl")
    drv_xldcd_c.setOutputName("drv_gfx_xlcdc.c")
    drv_xldcd_c.setProjectPath(project_path)
    drv_xldcd_c.setType("SOURCE")
    drv_xldcd_c.setMarkup(True)

    global drv_xldcd_int
    drv_xldcd_int = component.createFileSymbol("INTERRUPTS_C", None)
    drv_xldcd_int.setType("STRING")
    drv_xldcd_int.setSourcePath("templates/interrupt.c.ftl")
    drv_xldcd_int.setOutputName("core.LIST_SYSTEM_INTERRUPT_SHARED_HANDLERS")
    drv_xldcd_int.setMarkup(True)
    drv_xldcd_int.setEnabled(False)


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

    # Handle Dependency Visibility
    if local_id == "gfx_dpi_bridge_dep":
        local_comp.setDependencyEnabled("Graphics Display", False)
    elif local_id == "Graphics Display":
        local_comp.setDependencyEnabled("gfx_dpi_bridge_dep", False)

    # Auto Canvas Mode Toggle
    if remote_comp.getID() == "gfx_canvas":
        if remote_id == "gfx_display_driver":
            print(local_name + " Canvas Mode Enabled")
            canvas_mode.setValue(True)
            canvas_mode.setReadOnly(True)

    # Try to fetch display connected to DPI bridge
    # Define and update DisplayIDString string symbol in DPI bridge component
    if local_id == "gfx_dpi_bridge_dep":
        try:
            disp_id = remote_comp.getSymbolValue("DisplayIDString")
        except:
            disp_id = "None"
            print(local_name + "Error! DisplayIDString not defined")
        print(local_name + " Display ID is " + disp_id)
        if disp_id != "None":
            update_display_config(local_comp, disp_id)

    # Fetch display id for Graphics Display
    if local_id == "Graphics Display":
        disp_id = remote_comp.getID()
        print(local_name + " Display ID is " + disp_id)
        update_display_config(local_comp, disp_id)

    # LVDSC is connected
    if remote_id == "gfx_dpi_bridge_cap":
        if remote_name == "LVDSC":
            clock_src_sel.setValue("LVDSPLL")
            clock_src_sel.setReadOnly(True)


# Callback on component detach
def onAttachmentDisconnected(source, target):
    local_id = source["id"]
    local_comp = source["component"]
    local_name = local_comp.getDisplayName()
    remote_id = target["id"]
    remote_comp = target["component"]
    remote_name = remote_comp.getDisplayName()

    print(local_id + " of " + local_name + " disconnected from " + remote_id + " of " + remote_name)

    # Restore Dependency Visibility
    if local_id == "gfx_dpi_bridge_dep":
        local_comp.setDependencyEnabled("Graphics Display", True)
    elif local_id == "Graphics Display":
        local_comp.setDependencyEnabled("gfx_dpi_bridge_dep", True)

    # Auto Canvas Mode Toggle
    if remote_comp.getID() == "gfx_canvas":
        if remote_id == "gfx_display_driver":
            print(local_name + " Canvas Mode Disabled")
            canvas_mode.setValue(False)
            canvas_mode.setReadOnly(False)

    # LVDSC is disconnected
    if remote_id == "gfx_dpi_bridge_cap":
        if remote_name == "LVDSC":
            clock_src_sel.setReadOnly(False)


# Variable Callbacks
# Callback on RTOS enable
def rtos_dep_changed(symbol, event):
    # Set RTOS menu visibility
    symbol.setVisible(event["value"] != "BareMetal")


# Update Clocks
def update_gclk_clock(symbol, event):
    clkmgr_xlcdc_gclk_hz = 0
    gclk_src_sel = event["source"].getSymbolValue("XClockGCLKSource")
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
        clkmgr_xlcdc_gclk_hz = Database.getSymbolValue("core", symbol_gck_freq)
    # GCLK Frequency
    elif event["id"] == symbol_gck_freq:
        clkmgr_xlcdc_gclk_hz = event["value"]
    # XClockGCLKSource
    elif event["id"] == "XClockGCLKSource":
        gclk_src_sel = event["value"]
        if gclk_src_sel == 0:
            symbol.setValue(Database.getSymbolValue("core", symbol_gck_src_0_freq))
        elif gclk_src_sel == 1:
            symbol.setValue(Database.getSymbolValue("core", symbol_gck_src_1_freq))
    # XClockGCLKOutHint
    out_clk = event["source"].getSymbolValue("XClockGCLKSrcHint") / event["source"].getSymbolValue("XClockGCLKDiv")
    if clk_help_test_sym():
        event["source"].setSymbolValue("XClockGCLKOutHint", out_clk)
    else:
        event["source"].setSymbolValue("XClockGCLKOutHint", clkmgr_xlcdc_gclk_hz)


def update_pwm_clock(symbol, event):
    pwm_src = event["source"].getSymbolValue("XPWMClockSrc")
    pwm_prec = event["source"].getSymbolValue("XPWMClockPres")
    clk = 0
    if pwm_src == 0:
        clk = 32768
    elif pwm_src == 1:
        clk = Database.getSymbolValue("core", symbol_mck_freq)
    symbol.setValue(clk / 2**pwm_prec)


# Clock Validity
def show_gclk_valid_hint(symbol, event):
    if event["value"] > gck_max_freq:
        symbol.setLabel("[Warning! Must not exceed " + str(int(gck_max_freq / 1000000)) + " MHz]")
        symbol.setVisible(True)
    elif event["value"] <= 0:
        symbol.setLabel("[Warning! 0Hz?]")
        symbol.setVisible(True)
    else:
        symbol.setLabel("[Ok!]")
        symbol.setVisible(False)


# Show PWM Brightness
def show_pwm_brightness(symbol, event):
    symbol.setVisible(event["value"])


# Manage Layers
def on_layer_enable(symbol, event):
    layer_count = 1
    if event["source"].getSymbolValue("XLMEnableOVR1") == True:
        layer_count += 1
    if event["source"].getSymbolValue("XLMEnableHEO") == True:
        if event["source"].getSymbolValue("XLMHEOYCbCrEN") == False:
            layer_count += 1
        lm_heo_vidpri.setVisible(True)
        lm_heo_ycbcr_en.setVisible(True)
    else:
        lm_heo_vidpri.setVisible(False)
        lm_heo_ycbcr_en.setVisible(False)
    if event["source"].getSymbolValue("XLMEnableOVR2") == True:
        layer_count += 1
    event["source"].setSymbolValue("TotalNumLayers", layer_count)


# Refresh Output Modes
def refresh_output_mode(symbol, event):
    if event["value"] == 0:
        # DPI Mode
        if symbol.getID() == "XOutLegacy":
            symbol.setVisible(False)
        elif symbol.getID() == "XOutDPI":
            symbol.setVisible(True)
    elif event["value"] == 1:
        # Legacy Mode
        if symbol.getID() == "XOutLegacy":
            symbol.setVisible(True)
        elif symbol.getID() == "XOutDPI":
            symbol.setVisible(False)


# Utility Functions
# Update Driver Config
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
    display_de_pol = disp_component.getSymbolValue("DataEnablePolarity")
    display_hsync_pol = disp_component.getSymbolValue("HSYNCNegative")
    display_vsync_pol = disp_component.getSymbolValue("VSYNCNegative")
    # Update Driver Config
    component.setSymbolValue("XTResPPL", display_width)
    component.setSymbolValue("XTResRPF", display_height)
    component.setSymbolValue("XTHorPW", display_hpw)
    component.setSymbolValue("XTHorBP", display_hbp)
    component.setSymbolValue("XTHorFP", display_hfp)
    component.setSymbolValue("XTVerPW", display_vpw)
    component.setSymbolValue("XTVerBP", display_vbp)
    component.setSymbolValue("XTVerFP", display_vfp)
    component.setSymbolValue("XTPolDISP", display_de_pol)
    component.setSymbolValue("XTPolHSyncNegative", display_hsync_pol)
    component.setSymbolValue("XTPolVSyncNegative", display_vsync_pol)


# Clock Helper Visibility
def clk_help_cb(symbol, event):
    symbol.setValue(clk_help_test_sym())


def clk_show_sym(symbol, event):
    if event["id"] == "XClkGenSel":
        select = event["source"].getSymbolValue("XClkGenSel")
        sym = symbol.getID()
        symbol.setVisible((select == "GCLK" and sym == "XClockGCLKMenu") or (select == "LVDSPLL" and sym == "XClockLVDSPLLMenu"))
    else:
        symbol.setVisible(event["value"])


def clk_show_sym_inv(symbol, event):
    symbol.setVisible(not event["value"])


def clk_help_test_sym():
    is_set = False
    test_symbols = [
        ["core", symbol_clk_en],
        ["core", symbol_gclk_en],
        ["core", symbol_lvdsc_en],
        ["core", symbol_lvdspll_en],
    ]

    for item in test_symbols:
        is_set = is_set or Database.getSymbolValue(item[0], item[1])

    # Return true if none of the test symbols are set
    return not is_set


# Update LVDSPLL Clocks
def lvdspll_update_clocks(symbol, event):
    # CLK_MOSCXT_FREQ
    if event["id"] == "CLK_MOSCXT_FREQ":
        symbol.setValue(event["value"])

    # Set LVDSClockOutHint
    src = event["source"].getSymbolValue("LVDSClockSrcHint")
    mul = event["source"].getSymbolValue("LVDSClockMul")
    frac = event["source"].getSymbolValue("LVDSClockFrac")
    divpmc = event["source"].getSymbolValue("LVDSClockDivPMC")
    out_clk = int(src * (mul + (float(frac) / 2**22)) / divpmc)
    event["source"].setSymbolValue("LVDSClockOutHint", out_clk)


# Update XLCDC Pixel Clock through LVDSPLL Hint
def update_xlcdc_pix_hint(symbol, event):
    symbol.setValue(event["value"] / 7)


# LVDSPLL Core Validity
def lvdspll_core_valid_hint(symbol, event):
    # Calculate PLL Core Clock Output
    src = event["source"].getSymbolValue("LVDSClockSrcHint")
    mul = event["source"].getSymbolValue("LVDSClockMul")
    frac = event["source"].getSymbolValue("LVDSClockFrac")
    out_clk = int(src * (mul + (float(frac) / 2**22)))
    mul_tot = float(mul + (float(frac) / 2**22))

    if out_clk > 1200000000:
        symbol.setLabel("[Invalid! S * %.9f > 1200 MHz]" % mul_tot)
        symbol.setVisible(True)
    elif out_clk < 600000000:
        symbol.setLabel("[Invalid! S * %.9f < 600 MHz]" % mul_tot)
        symbol.setVisible(True)
    else:
        symbol.setLabel("[Ok! S * %.9f = %d]" % (mul_tot, out_clk))
        symbol.setVisible(True)


# LVDSPLL Clock Validity
def lvdspll_out_valid_hint(symbol, event):
    if event["value"] > out_max_freq:
        symbol.setLabel("[Invalid! O > " + str(int(out_max_freq / 1000000)) + " MHz]")
        symbol.setVisible(True)
    else:
        symbol.setLabel("[Ok!]")
        symbol.setVisible(False)


# Interrupt Helper
def int_show_sym(symbol, event):
    if event["id"] == "InterruptEnable":
        drv_xldcd_int.setEnabled(event["value"])
        Database.setSymbolValue("core", resgister_base_address + "_INTERRUPT_ENABLE", event["value"])
        Database.setSymbolValue("core", resgister_base_address + "_INTERRUPT_HANDLER", resgister_base_address + "_InterruptHandler")
    if event["id"] == "IntEnRow":
        y_res = timing_menu_res_y.getValue()
        int_row_num.setMax(y_res - 1)
    symbol.setVisible(event["value"])


# Double Buffering
def db_manage_sym(symbol, event):
    if event["id"] == "CanvasModeOnly":
        symbol.setReadOnly(event["value"])
    if event["id"] == "DoubleBuffering":
        interrupt_enable.setValue(event["value"])
        int_en_sof.setValue(event["value"])
        interrupt_enable.setReadOnly(event["value"])
        int_en_sof.setReadOnly(event["value"])


def show_db_opts(symbol, event):
    if event["id"] == "DoubleBuffering":
        symbol.setVisible(event["value"])
    if event["id"] == "DBRectOpt":
        if event["value"] == "Basic":
            db_merge_threshold.setVisible(False)
            db_merge_thr_min.setVisible(False)
            db_merge_thr_max.setVisible(False)
            db_merge_ohr.setVisible(False)
        if event["value"] == "Simple":
            db_merge_threshold.setVisible(True)
            db_merge_thr_min.setVisible(False)
            db_merge_thr_max.setVisible(False)
            db_merge_ohr.setVisible(False)
        if event["value"] == "Smart":
            db_merge_threshold.setVisible(True)
            db_merge_thr_min.setVisible(True)
            db_merge_thr_max.setVisible(True)
            db_merge_ohr.setVisible(True)
