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
    project_path = "config/" + Variables.get("__CONFIGURATION_NAME") + "/gfx/display"

    # Display Configuration Symbols
    name = component.createStringSymbol("Name", None)
    name.setLabel("Name")
    name.setDescription("The display name.")
    name.setDefaultValue("AC40T08A")
    name.setReadOnly(True)

    display_width = component.createIntegerSymbol("DisplayWidth", None)
    display_width.setLabel("Display Width")
    display_width.setDescription("The width of the graphics display in pixels.")
    display_width.setDefaultValue(720)
    display_width.setReadOnly(True)

    height = component.createIntegerSymbol("DisplayHeight", None)
    height.setLabel("Display Height")
    height.setDescription("The height of the graphics display in pixels.")
    height.setDefaultValue(1280)
    height.setReadOnly(True)

    touch_width = component.createIntegerSymbol("TouchWidth", None)
    touch_width.setLabel("Touch Panel Width")
    touch_width.setDescription("The width of the touch panel in pixels.")
    touch_width.setDefaultValue(720)
    touch_width.setReadOnly(True)
    touch_width.setVisible(False)

    touch_height = component.createIntegerSymbol("TouchHeight", None)
    touch_height.setLabel("Touch Panel Height")
    touch_height.setDescription("The height of the touch panel in pixels.")
    touch_height.setDefaultValue(1280)
    touch_height.setReadOnly(True)
    touch_height.setVisible(False)

    horz_menu = component.createMenuSymbol("HorzMenu", None)
    horz_menu.setLabel("Horizontal Attributes")
    horz_menu.setDescription("Contains display horizontal parameters.")

    horz_pulse_width = component.createIntegerSymbol("HorzPulseWidth", horz_menu)
    horz_pulse_width.setLabel("Horizontal Pulse Width")
    horz_pulse_width.setDescription("The horizontal pulse width.")
    horz_pulse_width.setDefaultValue(24)
    horz_pulse_width.setReadOnly(True)

    horz_back_porch = component.createIntegerSymbol("HorzBackPorch", horz_menu)
    horz_back_porch.setLabel("Horizontal Back Porch")
    horz_back_porch.setDescription("The horizontal back porch size in pixels.")
    horz_back_porch.setDefaultValue(12)
    horz_back_porch.setReadOnly(True)

    horz_front_porch = component.createIntegerSymbol("HorzFrontPorch", horz_menu)
    horz_front_porch.setLabel("Horizontal Front Porch")
    horz_front_porch.setDescription("The horizontal front porch size in pixels.")
    horz_front_porch.setDefaultValue(12)
    horz_front_porch.setReadOnly(True)

    vert_menu = component.createMenuSymbol("VertMenu", None)
    vert_menu.setLabel("Vertical Attributes")
    vert_menu.setDescription("Contains display vertical parameters.")

    vert_pulse_width = component.createIntegerSymbol("VertPulseWidth", vert_menu)
    vert_pulse_width.setLabel("Vertical Pulse Width")
    vert_pulse_width.setDescription("The vertical pulse width.")
    vert_pulse_width.setDefaultValue(1)
    vert_pulse_width.setReadOnly(True)

    vert_back_porch = component.createIntegerSymbol("VertBackPorch", vert_menu)
    vert_back_porch.setLabel("Vertical Back Porch")
    vert_back_porch.setDescription("The vertical back porch size in pixels.")
    vert_back_porch.setDefaultValue(13)
    vert_back_porch.setReadOnly(True)

    vert_front_porch = component.createIntegerSymbol("VertFrontPorch", vert_menu)
    vert_front_porch.setLabel("Vertical Front Porch")
    vert_front_porch.setDescription("The vertical front porch size in pixels.")
    vert_front_porch.setDefaultValue(13)
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

    dsi_menu = component.createMenuSymbol("DSIMenu", None)
    dsi_menu.setLabel("DSI Settings")
    dsi_menu.setDescription("Indicates supported DSI bitrate and other parameters.")

    dsi_bitrate = component.createIntegerSymbol("DSILaneBitrate", dsi_menu)
    dsi_bitrate.setLabel("Lane Bitrate (kHz)")
    dsi_bitrate.setDescription("The recommended DSI High-Speed Lane Bitrate (per lane).")
    dsi_bitrate.setDefaultValue(456000)
    dsi_bitrate.setReadOnly(True)

    dsi_nlanes = component.createIntegerSymbol("DSILanes", dsi_menu)
    dsi_nlanes.setLabel("No. of Lanes")
    dsi_nlanes.setDescription("The number of DSI data lanes supported by the display.")
    dsi_nlanes.setDefaultValue(4)
    dsi_nlanes.setReadOnly(True)

    dsi_lcd_clk = component.createIntegerSymbol("DSIDotClk", dsi_menu)
    dsi_lcd_clk.setLabel("LCD Pixel Clock (kHz)")
    dsi_lcd_clk.setDescription("The pixel clock of the LCD panel.")
    dsi_lcd_clk.setDefaultValue(60226)
    dsi_lcd_clk.setReadOnly(True)

    dsi_vm = component.createKeyValueSetSymbol("DSIVidMode", dsi_menu)
    dsi_vm.setLabel("Transmission Type")
    dsi_vm.setDescription("<html>Indicates the video mode transmission type.</hrml>")
    dsi_vm.addKey("VID_MODE_TYPE_0", "0", "Non-burst with sync pulses")
    dsi_vm.addKey("VID_MODE_TYPE_1", "1", "Non-burst with sync events")
    dsi_vm.addKey("VID_MODE_TYPE_2", "2", "Burst mode")
    dsi_vm.setOutputMode("Value")
    dsi_vm.setDisplayMode("Description")
    dsi_vm.setDefaultValue(2)
    dsi_vm.setReadOnly(True)

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

    # Generate Files
    sys_definitions_h = component.createFileSymbol("SYS_DEFINITIONS_H", None)
    sys_definitions_h.setType("STRING")
    sys_definitions_h.setOutputName("core.LIST_SYSTEM_DEFINITIONS_H_INCLUDES")
    sys_definitions_h.setSourcePath("templates/definitions.h.ftl")
    sys_definitions_h.setMarkup(True)

    sys_init_c = component.createFileSymbol("SYS_INIT_C", None)
    sys_init_c.setType("STRING")
    sys_init_c.setOutputName("core.LIST_SYSTEM_INIT_C_INITIALIZE_MIDDLEWARE")
    sys_init_c.setSourcePath("templates/init.c.ftl")
    sys_init_c.setMarkup(True)

    sys_task_c = component.createFileSymbol("SYS_TASK_C", None)
    sys_task_c.setType("STRING")
    sys_task_c.setOutputName("core.LIST_SYSTEM_TASKS_C_CALL_LIB_TASKS")
    sys_task_c.setSourcePath("templates/tasks.c.ftl")
    sys_task_c.setMarkup(True)

    sys_rtos_task_c = component.createFileSymbol("SYS_RTOS_TASK_C", None)
    sys_rtos_task_c.setType("STRING")
    sys_rtos_task_c.setOutputName("core.LIST_SYSTEM_RTOS_TASKS_C_DEFINITIONS")
    sys_rtos_task_c.setSourcePath("templates/rtos_tasks.c.ftl")
    sys_rtos_task_c.setMarkup(True)

    drv_disp_h = component.createFileSymbol("DRV_DISP_H", None)
    drv_disp_h.setDestPath("gfx/display/")
    drv_disp_h.setSourcePath("templates/touch_display.h.ftl")
    drv_disp_h.setOutputName("touch_display.h")
    drv_disp_h.setProjectPath(project_path)
    drv_disp_h.setType("HEADER")
    drv_disp_h.setMarkup(True)

    drv_disp_c = component.createFileSymbol("DRV_DISP_C", None)
    drv_disp_c.setDestPath("gfx/display/")
    drv_disp_c.setSourcePath("templates/touch_display.c.ftl")
    drv_disp_c.setOutputName("touch_display.c")
    drv_disp_c.setProjectPath(project_path)
    drv_disp_c.setType("SOURCE")
    drv_disp_c.setOverwrite(True)
    drv_disp_c.setEnabled(True)
    drv_disp_c.setMarkup(True)


# Variable Callbacks
# Callback on RTOS enable
def rtos_dep_changed(symbol, event):
    # Set RTOS menu visibility
    symbol.setVisible(event["value"] != "BareMetal")
