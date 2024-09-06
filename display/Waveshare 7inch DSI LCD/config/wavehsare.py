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

    # Activate dependency components
    Database.activateComponents(["sys_input", "sys_time"])

    # Enable "Generate Harmony Driver Common Files" option in Core
    Database.sendMessage("HarmonyCore", "ENABLE_DRV_COMMON", {"isEnabled": True})

    # Display Configuration Symbols
    name = component.createStringSymbol("Name", None)
    name.setLabel("Name")
    name.setDescription("The display name.")
    name.setDefaultValue("Waveshare 7inch DSI LCD (C)")
    name.setReadOnly(True)

    display_width = component.createIntegerSymbol("DisplayWidth", None)
    display_width.setLabel("Display Width")
    display_width.setDescription("The width of the graphics display in pixels.")
    display_width.setDefaultValue(1024)
    display_width.setReadOnly(True)

    height = component.createIntegerSymbol("DisplayHeight", None)
    height.setLabel("Display Height")
    height.setDescription("The height of the graphics display in pixels.")
    height.setDefaultValue(600)
    height.setReadOnly(True)

    disp_conf_menu = component.createMenuSymbol("DispConfMenu", None)
    disp_conf_menu.setLabel("Display Config")
    disp_conf_menu.setDescription("Contains display configuration.")

    default_bl = component.createIntegerSymbol("DefaultBL", disp_conf_menu)
    default_bl.setLabel("Default Brightness")
    default_bl.setDescription("Default display backlight brightness on startup.")
    default_bl.setDefaultValue(200)
    default_bl.setMin(0)
    default_bl.setMax(255)

    delay_bl = component.createIntegerSymbol("DelayBL", disp_conf_menu)
    delay_bl.setLabel("Backlight Delay (ms)")
    delay_bl.setDescription("Delay the display backlight to mask startup artifacts.")
    delay_bl.setDefaultValue(50)
    delay_bl.setMin(0)
    delay_bl.setMax(2000)

    touch_orient = component.createKeyValueSetSymbol("TouchOrient", disp_conf_menu)
    touch_orient.setLabel("Touch Orientation")
    touch_orient.setOutputMode("Value")
    touch_orient.setDisplayMode("Description")
    touch_orient.setDescription("Transforms coordinates in software.")
    touch_orient.addKey("0", str(0), "0 Deg")
    touch_orient.addKey("90", str(90), "90 Deg")
    touch_orient.addKey("180", str(180), "180 Deg")
    touch_orient.addKey("270", str(270), "270 Deg")
    touch_orient.setDefaultValue(0)

    horz_menu = component.createMenuSymbol("HorzMenu", None)
    horz_menu.setLabel("Horizontal Attributes")
    horz_menu.setDescription("Contains display horizontal parameters.")

    horz_pulse_width = component.createIntegerSymbol("HorzPulseWidth", horz_menu)
    horz_pulse_width.setLabel("Horizontal Pulse Width")
    horz_pulse_width.setDescription("The horizontal pulse width.")
    horz_pulse_width.setDefaultValue(100)
    horz_pulse_width.setReadOnly(True)

    horz_back_porch = component.createIntegerSymbol("HorzBackPorch", horz_menu)
    horz_back_porch.setLabel("Horizontal Back Porch")
    horz_back_porch.setDescription("The horizontal back porch size in pixels.")
    horz_back_porch.setDefaultValue(100)
    horz_back_porch.setReadOnly(True)

    horz_front_porch = component.createIntegerSymbol("HorzFrontPorch", horz_menu)
    horz_front_porch.setLabel("Horizontal Front Porch")
    horz_front_porch.setDescription("The horizontal front porch size in pixels.")
    horz_front_porch.setDefaultValue(100)
    horz_front_porch.setReadOnly(True)

    vert_menu = component.createMenuSymbol("VertMenu", None)
    vert_menu.setLabel("Vertical Attributes")
    vert_menu.setDescription("Contains display vertical parameters.")

    vert_pulse_width = component.createIntegerSymbol("VertPulseWidth", vert_menu)
    vert_pulse_width.setLabel("Vertical Pulse Width")
    vert_pulse_width.setDescription("The vertical pulse width.")
    vert_pulse_width.setDefaultValue(10)
    vert_pulse_width.setReadOnly(True)

    vert_back_porch = component.createIntegerSymbol("VertBackPorch", vert_menu)
    vert_back_porch.setLabel("Vertical Back Porch")
    vert_back_porch.setDescription("The vertical back porch size in pixels.")
    vert_back_porch.setDefaultValue(10)
    vert_back_porch.setReadOnly(True)

    vert_front_porch = component.createIntegerSymbol("VertFrontPorch", vert_menu)
    vert_front_porch.setLabel("Vertical Front Porch")
    vert_front_porch.setDescription("The vertical front porch size in pixels.")
    vert_front_porch.setDefaultValue(10)
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
    hsync_negative.setDefaultValue(False)
    hsync_negative.setReadOnly(True)

    vsync_negative = component.createBooleanSymbol("VSYNCNegative", polarity_menu)
    vsync_negative.setLabel("VSYNC Active Low")
    vsync_negative.setDescription("Indicates the need for negative VSYNC polarity.")
    vsync_negative.setDefaultValue(False)
    vsync_negative.setReadOnly(True)

    dsi_menu = component.createMenuSymbol("DSIMenu", None)
    dsi_menu.setLabel("DSI Settings")
    dsi_menu.setDescription("Indicates supported DSI bitrate and other parameters.")

    dsi_bitrate = component.createIntegerSymbol("DSILaneBitrate", dsi_menu)
    dsi_bitrate.setLabel("Lane Bitrate (kHz)")
    dsi_bitrate.setDescription("The recommended DSI High-Speed Lane Bitrate (per lane).")
    dsi_bitrate.setDefaultValue(500000)
    dsi_bitrate.setReadOnly(True)

    dsi_nlanes = component.createIntegerSymbol("DSILanes", dsi_menu)
    dsi_nlanes.setLabel("No. of Lanes")
    dsi_nlanes.setDescription("The number of DSI data lanes supported by the display.")
    dsi_nlanes.setDefaultValue(2)
    dsi_nlanes.setReadOnly(True)

    dsi_lcd_clk = component.createIntegerSymbol("DSIDotClk", dsi_menu)
    dsi_lcd_clk.setLabel("LCD Pixel Clock (kHz)")
    dsi_lcd_clk.setDescription("The pixel clock of the LCD panel.")
    dsi_lcd_clk.setDefaultValue(50000)
    dsi_lcd_clk.setReadOnly(True)

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

    # Generator Config Symbols
    global drv_i2c_idx
    drv_i2c_idx = component.createIntegerSymbol("I2CIdx", None)
    drv_i2c_idx.setLabel("I2C Instance")
    drv_i2c_idx.setDefaultValue(0)
    drv_i2c_idx.setReadOnly(True)
    drv_i2c_idx.setVisible(False)

    drv_i2c_mode_default = Database.getSymbolValue("drv_i2c", "DRV_I2C_MODE")

    if drv_i2c_mode_default == None:
        drv_i2c_mode_default = "Asynchronous"

    drv_i2c_mode = component.createComboSymbol("I2CMode", None, ["Asynchronous", "Synchronous"])
    drv_i2c_mode.setLabel("Driver Mode")
    drv_i2c_mode.setDefaultValue(drv_i2c_mode_default)
    drv_i2c_mode.setDependencies(i2c_mode_change, ["drv_i2c.DRV_I2C_MODE"])
    drv_i2c_mode.setVisible(False)

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

    disp_ws_h = component.createFileSymbol("DISP_WS_H", None)
    disp_ws_h.setDestPath("gfx/display/")
    disp_ws_h.setSourcePath("templates/ws_touch_display.h.ftl")
    disp_ws_h.setOutputName("ws_touch_display.h")
    disp_ws_h.setProjectPath(project_path)
    disp_ws_h.setType("HEADER")
    disp_ws_h.setMarkup(True)

    disp_ws_async_c = component.createFileSymbol("DISP_WS_ASYNC_C", None)
    disp_ws_async_c.setDestPath("gfx/display/")
    disp_ws_async_c.setSourcePath("templates/ws_touch_display_async.c.ftl")
    disp_ws_async_c.setOutputName("ws_touch_display.c")
    disp_ws_async_c.setProjectPath(project_path)
    disp_ws_async_c.setType("SOURCE")
    disp_ws_async_c.setOverwrite(True)
    disp_ws_async_c.setEnabled((drv_i2c_mode_default == "Asynchronous"))
    disp_ws_async_c.setDependencies(i2c_mode_change, ["I2CMode"])
    disp_ws_async_c.setMarkup(True)

    disp_ws_sync_c = component.createFileSymbol("DISP_WS_SYNC_C", None)
    disp_ws_sync_c.setDestPath("gfx/display/")
    disp_ws_sync_c.setSourcePath("templates/ws_touch_display_sync.c.ftl")
    disp_ws_sync_c.setOutputName("ws_touch_display.c")
    disp_ws_sync_c.setProjectPath(project_path)
    disp_ws_sync_c.setType("SOURCE")
    disp_ws_sync_c.setOverwrite(True)
    disp_ws_sync_c.setEnabled((drv_i2c_mode_default == "Synchronous"))
    disp_ws_sync_c.setDependencies(i2c_mode_change, ["I2CMode"])
    disp_ws_sync_c.setMarkup(True)


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

    # Handle I2C Instance
    if remote_id == "drv_i2c":
        i2c_instance = remote_comp.getInstanceNumber()
        drv_i2c_idx.setValue(i2c_instance)
        print("I2C instance " + str(drv_i2c_idx.getValue()) + " connected")
        # Set number of clients to 2
        Database.setSymbolValue(remote_comp.getID(), "DRV_I2C_NUM_CLIENTS", 2)


# Callback on component detach
def onAttachmentDisconnected(source, target):
    local_id = source["id"]
    local_comp = source["component"]
    local_name = local_comp.getDisplayName()
    remote_id = target["id"]
    remote_comp = target["component"]
    remote_name = remote_comp.getDisplayName()

    print(local_id + " of " + local_name + " disconnected from " + remote_id + " of " + remote_name)

    # Handle I2C Instance
    if remote_id == "drv_i2c":
        i2c_instance = remote_comp.getInstanceNumber()
        print("I2C instance " + str(drv_i2c_idx.getValue()) + " disconnected")
        # Reset index and clients
        drv_i2c_idx.setValue(0)
        Database.setSymbolValue(remote_comp.getID(), "DRV_I2C_NUM_CLIENTS", 1)


# Variable Callbacks
# Callback on RTOS enable
def rtos_dep_changed(symbol, event):
    # Set RTOS menu visibility
    symbol.setVisible(event["value"] != "BareMetal")


# Callback on I2C mode change
def i2c_mode_change(symbol, event):
    if symbol.getID() == "I2CMode":
        # Set driver i2c mode
        symbol.setValue(event["value"])
        print("I2CMode set to " + symbol.getValue())

    elif symbol.getID() == "DISP_WS_ASYNC_C":
        # Enable file for Asynchronous I2C
        symbol.setEnabled((event["value"] == "Asynchronous"))
        print(" DISP_WS_ASYNC_C set to " + str(symbol.getEnabled()))

    elif symbol.getID() == "DISP_WS_SYNC_C":
        # Enable file for Synchronous I2C
        symbol.setEnabled((event["value"] == "Synchronous"))
        print(" DISP_WS_SYNC_C set to " + str(symbol.getEnabled()))
