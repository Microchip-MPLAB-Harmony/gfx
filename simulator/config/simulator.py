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


# Imports
import os.path

# Component ID prefix for debug print
comp_id_str = "[mgs_web_sim] "

# Local Variables
# Display Subsystem Vars
# List of Supported Displays
# Note: display_name must be unique!
listDispSupport = [
    # ["display_name", x_res, y_res, is_portrait, is_circle]
    ["QVGA", 320, 240, False, False],
    ["WQVGA", 480, 272, False, False],
    ["HVGA", 480, 320, False, False],
    ["432C", 432, 432, False, True],
    ["VGA", 640, 480, False, False],
    ["WVGA", 800, 480, False, False],
    ["720C", 720, 720, False, True],
    ["XGA", 1024, 768, False, False],
]
listDispType = []
# Default display index of listDispSupport
idxDispDefault = 1  # WQVGA
# Default Display Attributes
strDispName = listDispSupport[idxDispDefault][0]
valDispHorRes = listDispSupport[idxDispDefault][1]
valDispVerRes = listDispSupport[idxDispDefault][2]
boolDispIsPortrait = listDispSupport[idxDispDefault][3]
boolDispIsCircular = listDispSupport[idxDispDefault][4]

# Controller Subsystem Vars
listCtrlCanvasMode = ["DISABLED", "ENABLED"]
listCtrlColorMode = ["RGB_565", "RGBA_8888"]
defaultCtrlColorMode = listCtrlColorMode[0]
valCrtlNumLayers = 1

# Timer Subsystem Vars
listTmrTickMode = ["TMR DISABLED", "TICKLESS", "TICK BASED"]
valTmrMaxVal = pow(2, int(32)) - 1

# Build Options Vars
listOptimizationLevel = ["O0", "O1", "O2", "O3", "Os", "Oz"]
defaultOptimizationLevel = listOptimizationLevel[1]

# Init Local Variables
# Set number of layers based on processor
if (
    any(device in Variables.get("__PROCESSOR") for device in ["SAM9X72", "SAM9X75"])
    or "A5D2" in str(Variables.get("__PROCESSOR"))
    or "9X60" in str(Variables.get("__PROCESSOR"))
    or ("PIC32MZ" in str(Variables.get("__PROCESSOR")) and "DA" in str(Variables.get("__PROCESSOR")))
):
    valCrtlNumLayers = 3
    defaultCtrlColorMode = listCtrlColorMode[1]


# Initialize Module GUI and Variables
def instantiateComponent(component):
    # Activate dependency components
    Database.activateComponents(["HarmonyCore", "sys_input"])

    # Enable "Generate Harmony Driver Common Files" option in harmony core
    Database.sendMessage("HarmonyCore", "ENABLE_DRV_COMMON", {"isEnabled": True})

    # Typically name of gfxDisplayDriver structure used by Canvas
    global driver_init_name
    driver_init_name = component.createStringSymbol("DriverInitName", None)
    driver_init_name.setVisible(False)
    driver_init_name.setReadOnly(True)
    driver_init_name.setDefaultValue("gfxSimulatorDriver")

    # Display Subsystem Group
    # Group Name
    disp_subsys_menu = component.createMenuSymbol("DispSubsys", None)
    disp_subsys_menu.setLabel("Graphics Display Subsystem")
    disp_subsys_menu.setDescription("Contains settings for the simulated graphics display.")

    # Display Name
    # Polulate display list
    listDispType = [disp_config[0] for disp_config in listDispSupport]
    # Add 'Custom' option to list
    listDispType.append("Custom")
    disp_subsys_display_type = component.createComboSymbol("DisplayType", disp_subsys_menu, listDispType)
    disp_subsys_display_type.setLabel("Display Type")
    disp_subsys_display_type.setDefaultValue(listDispType[listDispType.index(strDispName)])
    disp_subsys_display_type.setDependencies(onDisplayTypeChanged, ["DisplayType"])

    # Display Horizontal Resolution
    global disp_subsys_hor_res
    disp_subsys_hor_res = component.createIntegerSymbol("valDispHorRes", disp_subsys_menu)
    disp_subsys_hor_res.setLabel("Horizontal Resolution")
    disp_subsys_hor_res.setDescription("Simulated display's horizontal resolution in pixels")
    disp_subsys_hor_res.setDefaultValue(valDispHorRes)
    disp_subsys_hor_res.setMin(32)
    disp_subsys_hor_res.setMax(1280)
    disp_subsys_hor_res.setReadOnly(True)

    # Display Vertical Resolution
    global disp_subsys_ver_res
    disp_subsys_ver_res = component.createIntegerSymbol("valDispVerRes", disp_subsys_menu)
    disp_subsys_ver_res.setLabel("Vertical Resolution")
    disp_subsys_ver_res.setDescription("Simulated display's vertical resolution in pixels")
    disp_subsys_ver_res.setDefaultValue(valDispVerRes)
    disp_subsys_ver_res.setMin(32)
    disp_subsys_ver_res.setMax(1280)
    disp_subsys_ver_res.setReadOnly(True)

    # Display Type
    global disp_subsys_circ_toggle
    disp_subsys_circ_toggle = component.createBooleanSymbol("isDispCircular", disp_subsys_menu)
    disp_subsys_circ_toggle.setLabel("Circular Display")
    disp_subsys_circ_toggle.setDescription("Simulated display is circular if enabled")
    disp_subsys_circ_toggle.setDefaultValue(boolDispIsCircular)
    disp_subsys_circ_toggle.setReadOnly(True)

    # Controller Subsystem Group
    # Group Name
    ctrl_subsys_menu = component.createMenuSymbol("CtrlSubsys", None)
    ctrl_subsys_menu.setLabel("Graphics Controller Subsystem")
    ctrl_subsys_menu.setDescription("Contains settings for the simulated graphics controller.")

    # Controller Color Mode
    ctrl_susbys_color_mode = component.createComboSymbol("CtrlColorMode", ctrl_subsys_menu, listCtrlColorMode)
    ctrl_susbys_color_mode.setLabel("Color Mode")
    ctrl_susbys_color_mode.setDescription("The default color mode of all simulated layers")
    ctrl_susbys_color_mode.setDefaultValue(defaultCtrlColorMode)

    # Controller Layer Count
    ctrl_subsys_layer_count = component.createIntegerSymbol("TotalNumLayers", ctrl_subsys_menu)
    ctrl_subsys_layer_count.setLabel("Layer Count")
    ctrl_subsys_layer_count.setDescription("Simulated controller's hardware layer count.")
    ctrl_subsys_layer_count.setDefaultValue(valCrtlNumLayers)
    ctrl_subsys_layer_count.setMin(1)
    ctrl_subsys_layer_count.setMax(4)
    ctrl_subsys_layer_count.setDependencies(onCtrlNumLayersSet, ["TotalNumLayers"])

    # Controller Canvas Support Sub Group Name
    ctrl_subsys_submenu = component.createMenuSymbol("CtrlSubsysSubMenu", ctrl_subsys_menu)
    ctrl_subsys_submenu.setLabel("Canvas Support")
    ctrl_subsys_submenu.setDescription("Displays status of Canvas Mode support.")

    # Controller Canvas Mode
    global ctrl_subsys_canvas_mode
    ctrl_subsys_canvas_mode = component.createComboSymbol("CtrlCanvasMode", ctrl_subsys_submenu, listCtrlCanvasMode)
    ctrl_subsys_canvas_mode.setLabel("Canvas Mode")
    ctrl_subsys_canvas_mode.setDescription("Sets the gfxDisplayDriver name to " + driver_init_name.getValue() + " to support the Graphics Canvas Virtual Display")
    ctrl_subsys_canvas_mode.setDefaultValue(listCtrlCanvasMode[0])
    ctrl_subsys_canvas_mode.setReadOnly(True)

    # Controller Canvas Mode Help comment
    global ctrl_subsys_help_comment
    ctrl_subsys_help_comment = component.createCommentSymbol("CtrlHelp", ctrl_subsys_submenu)
    ctrl_subsys_help_comment.setLabel("Connect the Graphics Canvas Virtual Display to enable Canvas Mode.")
    ctrl_subsys_help_comment.setVisible(True)

    # Timer Subsystem Group
    # Group Name
    tmr_subsys_menu = component.createMenuSymbol("TmrSubsys", None)
    tmr_subsys_menu.setLabel("Timer Subsystem")
    tmr_subsys_menu.setDescription("Displays configuration status of the simulated timer.")

    # Timer Tick Mode
    global tmr_subsys_tick_mode
    tmr_subsys_tick_mode = component.createComboSymbol("TmrTickMode", tmr_subsys_menu, listTmrTickMode)
    tmr_subsys_tick_mode.setLabel("Tick Mode")
    tmr_subsys_tick_mode.setDescription("The simulated timer's tick mode.")
    tmr_subsys_tick_mode.setDefaultValue(listTmrTickMode[0])
    tmr_subsys_tick_mode.setReadOnly(True)

    # Timer Tick Resolution
    global tmr_subsys_tick_res
    tmr_subsys_tick_res = component.createIntegerSymbol("TmrTickRes", tmr_subsys_menu)
    tmr_subsys_tick_res.setLabel("Tick Resolution (ms)")
    tmr_subsys_tick_res.setDescription("The simulated timer's tick resolution in milliseconds.")
    tmr_subsys_tick_res.setDefaultValue(1)
    tmr_subsys_tick_res.setMin(1)
    tmr_subsys_tick_res.setMax(20)
    tmr_subsys_tick_res.setReadOnly(True)

    # Timer Help comment
    global tmr_subsys_help_comment
    tmr_subsys_help_comment = component.createCommentSymbol("TmrHelp", tmr_subsys_menu)
    tmr_subsys_help_comment.setLabel("Connect TIME System Service to enable.")
    tmr_subsys_help_comment.setVisible(True)

    # Symbols to support TIME System Service
    tmr_subsys_width = component.createIntegerSymbol("TIMER_WIDTH", tmr_subsys_menu)
    tmr_subsys_width.setVisible(False)
    tmr_subsys_width.setDefaultValue(32)

    tmr_subsys_period_max = component.createStringSymbol("TIMER_PERIOD_MAX", tmr_subsys_menu)
    tmr_subsys_period_max.setVisible(False)
    tmr_subsys_period_max.setDefaultValue(str(valTmrMaxVal))

    tmr_subsys_start_api_name = component.createStringSymbol("TIMER_START_API_NAME", tmr_subsys_menu)
    tmr_subsys_start_api_name.setVisible(False)

    tmr_subsys_stop_api_name = component.createStringSymbol("TIMER_STOP_API_NAME", tmr_subsys_menu)
    tmr_subsys_stop_api_name.setVisible(False)

    tmr_subsys_set_api_name = component.createStringSymbol("COMPARE_SET_API_NAME", tmr_subsys_menu)
    tmr_subsys_set_api_name.setVisible(False)

    tmr_subsys_get_api_name = component.createStringSymbol("COUNTER_GET_API_NAME", tmr_subsys_menu)
    tmr_subsys_get_api_name.setVisible(False)

    tmr_subsys_get_hz_api_name = component.createStringSymbol("FREQUENCY_GET_API_NAME", tmr_subsys_menu)
    tmr_subsys_get_hz_api_name.setVisible(False)

    tmr_subsys_callback_api_name = component.createStringSymbol("CALLBACK_API_NAME", tmr_subsys_menu)
    tmr_subsys_callback_api_name.setVisible(False)

    tmr_subsys_irq_enum_api_stub = component.createStringSymbol("IRQ_ENUM_NAME", tmr_subsys_menu)
    tmr_subsys_irq_enum_api_stub.setVisible(False)

    tmr_subsys_start_api_name.setDefaultValue("GFX_SIM_TimerStart")
    tmr_subsys_stop_api_name.setDefaultValue("GFX_SIM_TimerStop")
    tmr_subsys_set_api_name.setDefaultValue("GFX_SIM_TimerStubCompareSet")
    tmr_subsys_get_api_name.setDefaultValue("GFX_SIM_TimerCounterGet")
    tmr_subsys_get_hz_api_name.setDefaultValue("GFX_SIM_TimerGetFreqHz")
    tmr_subsys_callback_api_name.setDefaultValue("GFX_SIM_TimerRegisterCallback")
    tmr_subsys_irq_enum_api_stub.setDefaultValue("0")

    # Web Mode Options
    # Group Name
    web_opt_menu = component.createMenuSymbol("webOpts", None)
    web_opt_menu.setLabel("Web Mode Options")
    web_opt_menu.setDescription("Contains options for Web mode.")

    # Web page title
    default_title = Variables.get("__CONFIGURATION_NAME")
    web_opt_title = component.createStringSymbol("webPageTitle", web_opt_menu)
    web_opt_title.setLabel("Title")
    web_opt_title.setVisible(True)
    web_opt_title.setReadOnly(False)
    web_opt_title.setDefaultValue(default_title)
    web_opt_title.setDescription("Sets the title of the simulator's generated webpage.")

    # Bezel Enable
    web_opt_bezel_toggle = component.createBooleanSymbol("isBezelEnabled", web_opt_menu)
    web_opt_bezel_toggle.setLabel("Bezel")
    web_opt_bezel_toggle.setDescription("<html>Enables virtual bezel.<br />You may want to disable this for embedding<br />into an &lt;iframe&gt; tag.</html>")
    web_opt_bezel_toggle.setDefaultValue(True)
    web_opt_bezel_toggle.setReadOnly(False)

    # Responsive Enable
    web_opt_resp_toggle = component.createBooleanSymbol("isRespEnabled", web_opt_menu)
    web_opt_resp_toggle.setLabel("Responsive")
    web_opt_resp_toggle.setDescription("<html>Scale display to fit inside the browser viewport.<br />Makes the page mobile and tablet friendly.</html>")
    web_opt_resp_toggle.setDefaultValue(True)
    web_opt_resp_toggle.setReadOnly(False)

    # Console Enable
    web_opt_console_toggle = component.createBooleanSymbol("isConsoleEnabled", web_opt_menu)
    web_opt_console_toggle.setLabel("Console")
    web_opt_console_toggle.setDescription("<html>Enables the debug console overlay.<br />Note that stdio and stderr are line-buffered.<br />Don't forget to append \\n to your debug messages.</html>")
    web_opt_console_toggle.setDefaultValue(False)
    web_opt_console_toggle.setReadOnly(False)

    # Console Timestamp Enable
    web_opt_con_tstamp_toggle = component.createBooleanSymbol("consoleTSEnabled", web_opt_console_toggle)
    web_opt_con_tstamp_toggle.setLabel("Timestamp")
    web_opt_con_tstamp_toggle.setDescription("Prepends a timestamp to your debug messages.")
    web_opt_con_tstamp_toggle.setDefaultValue(True)
    web_opt_con_tstamp_toggle.setReadOnly(False)
    web_opt_con_tstamp_toggle.setVisible(False)
    web_opt_con_tstamp_toggle.setDependencies(consoleEnabled, ["isConsoleEnabled"])

    # Console Closed on start-up
    web_opt_con_srtclo_toggle = component.createBooleanSymbol("consoleStartClosed", web_opt_console_toggle)
    web_opt_con_srtclo_toggle.setLabel("Start Closed")
    web_opt_con_srtclo_toggle.setDescription("<html>Console is closed on start-up if enabled.<br />Click the console button to open/close it.<br />Un-check to open console on page load.</html>")
    web_opt_con_srtclo_toggle.setDefaultValue(True)
    web_opt_con_srtclo_toggle.setReadOnly(False)
    web_opt_con_srtclo_toggle.setVisible(False)
    web_opt_con_srtclo_toggle.setDependencies(consoleEnabled, ["isConsoleEnabled"])

    # Build Options
    # Group Name
    build_opt_menu = component.createMenuSymbol("buildOpts", web_opt_menu)
    build_opt_menu.setLabel("Build Options")
    build_opt_menu.setDescription("Customize options for the simulator build.")

    # Initial Memory
    build_opt_initial_memory = component.createIntegerSymbol("initialMemory", build_opt_menu)
    build_opt_initial_memory.setLabel("Initial Memory (MB)")
    build_opt_initial_memory.setDescription("Initial memory to allocate in Megabytes.")
    build_opt_initial_memory.setDefaultValue(32)
    build_opt_initial_memory.setMin(16)
    build_opt_initial_memory.setMax(256)

    # Stack Size
    build_opt_stack_size = component.createIntegerSymbol("stackSize", build_opt_menu)
    build_opt_stack_size.setLabel("Stack Size (KB)")
    build_opt_stack_size.setDescription("Fixed total stack size in Kilobytes.")
    build_opt_stack_size.setDefaultValue(64)
    build_opt_stack_size.setMin(64)
    build_opt_stack_size.setMax(8192)

    # Optimization
    build_opt_optimization = component.createComboSymbol("optimizationLevel", build_opt_menu, listOptimizationLevel)
    build_opt_optimization.setLabel("Optimization")
    build_opt_optimization.setDescription(
        "<html> Sets the optimization level."
        "<br> 'O0' This is the recommended setting while starting to port a project."
        "<br> 'O1' Simple optimizations (default)."
        "<br> 'O2' Like 'O1', but enables more optimizations."
        "<br> 'O3' Like 'O2', but with additional optimizations that may take longer to run."
        "<br> 'Os' Like 'O3', but focuses more on code size (and may make tradeoffs with speed)."
        "<br> 'Oz' Like 'Os', but reduces code size even further, and may take longer to run."
        "<br> </html>"
    )
    build_opt_optimization.setDefaultValue(defaultOptimizationLevel)

    # Native Mode Options
    # Group Name
    native_opt_menu = component.createMenuSymbol("nativeOpts", None)
    native_opt_menu.setLabel("Native Mode Options")
    native_opt_menu.setDescription("Contains options for Native Mode.")

    # AOT Enable
    native_opt_window_aot = component.createBooleanSymbol("alwaysOnTop", native_opt_menu)
    native_opt_window_aot.setLabel("Window Always On Top")
    native_opt_window_aot.setDescription("<html>Enables always on top window.<br />This may help keep the window in view while debugging<br />with breakpoints.</html>")
    native_opt_window_aot.setDefaultValue(True)
    native_opt_window_aot.setReadOnly(False)

    # Code Generation
    # Paths
    config_name = Variables.get("__CONFIGURATION_NAME")
    project_path = "config/" + config_name + "/gfx/simulator"
    common_driver_project_path = "config/" + config_name + "/gfx/driver"

    # Normalize absolute path based on OS
    framework_path = Variables.get("__FRAMEWORK_ROOT")
    common_driver_template_path = "/gfx/middleware/legato/driver/templates/gfx_driver"
    common_driver_template_path = framework_path + os.path.normpath(common_driver_template_path)
    print(comp_id_str + "Common driver path set to: " + common_driver_template_path)

    # File Generation Toggle
    global file_gen_toggle
    file_gen_toggle = component.createBooleanSymbol("fileGenToggle", None)
    file_gen_toggle.setLabel("Generate Source Code")
    file_gen_toggle.setDescription("Enables source code generation.")
    file_gen_toggle.setDefaultValue(False)
    file_gen_toggle.setReadOnly(True)
    file_gen_toggle.setVisible(False)

    # Core System Files
    core_sys_def_h = component.createFileSymbol("CORE_SYS_DEF_H", None)
    core_sys_def_h.setType("STRING")
    core_sys_def_h.setOutputName("core.LIST_SYSTEM_DEFINITIONS_H_INCLUDES")
    core_sys_def_h.setSourcePath("templates/definitions.h.ftl")
    core_sys_def_h.setMarkup(True)
    core_sys_def_h.setEnabled(False)
    core_sys_def_h.setDependencies(toggleFileGen, ["fileGenToggle"])

    # Common GFX Driver Files
    gfx_drv_h = component.createFileSymbol("GFX_DRIVER_H", None)
    gfx_drv_h.setRelative(False)  # Relative to module.xml if True, else absolute
    gfx_drv_h.setSourcePath(common_driver_template_path + ".h.ftl")
    gfx_drv_h.setDestPath("gfx/driver/")  # Physical destination
    gfx_drv_h.setOutputName("gfx_driver.h")
    gfx_drv_h.setProjectPath(common_driver_project_path)  # MPLAB X Logical Folder
    gfx_drv_h.setType("HEADER")
    gfx_drv_h.setMarkup(True)
    gfx_drv_h.setEnabled(False)
    gfx_drv_h.setDependencies(toggleFileGen, ["fileGenToggle"])

    gfx_drv_c = component.createFileSymbol("GFX_DRIVER_C", None)
    gfx_drv_c.setRelative(False)
    gfx_drv_c.setSourcePath(common_driver_template_path + ".c.ftl")
    gfx_drv_c.setDestPath("gfx/driver/")
    gfx_drv_c.setOutputName("gfx_driver.c")
    gfx_drv_c.setProjectPath(common_driver_project_path)
    gfx_drv_c.setType("SOURCE")
    gfx_drv_c.setMarkup(True)
    gfx_drv_c.setEnabled(False)
    gfx_drv_c.setDependencies(toggleFileGen, ["fileGenToggle"])

    # GFX Simulator Files
    gfx_sim_h = component.createFileSymbol("GFX_SIM_H", None)
    gfx_sim_h.setSourcePath("templates/gfx_simulator.h.ftl")
    gfx_sim_h.setDestPath("gfx/simulator/")
    gfx_sim_h.setOutputName("gfx_simulator.h")
    gfx_sim_h.setProjectPath(project_path)
    gfx_sim_h.setType("HEADER")
    gfx_sim_h.setMarkup(True)
    gfx_sim_h.setEnabled(False)
    gfx_sim_h.setDependencies(toggleFileGen, ["fileGenToggle"])

    gfx_sim_c = component.createFileSymbol("GFX_SIM_C", None)
    gfx_sim_c.setSourcePath("templates/gfx_simulator.c.ftl")
    gfx_sim_c.setDestPath("gfx/simulator/")
    gfx_sim_c.setOutputName("gfx_simulator.c")
    gfx_sim_c.setProjectPath(project_path)
    gfx_sim_c.setType("SOURCE")
    gfx_sim_c.setMarkup(True)
    gfx_sim_c.setEnabled(False)
    gfx_sim_c.setDependencies(toggleFileGen, ["fileGenToggle"])

    gfx_sim_proj_name = config_name + ".mgsws"
    gfx_sim_proj_path = "config/" + config_name + "/"
    gfx_sim_proj = component.createFileSymbol("GFX_SIM_PROPS", None)
    gfx_sim_proj.setSourcePath("templates/project.mgsws.ftl")
    gfx_sim_proj.setDestPath("")
    gfx_sim_proj.setOutputName(gfx_sim_proj_name)
    gfx_sim_proj.setProjectPath(gfx_sim_proj_path)
    gfx_sim_proj.setType("IMPORTANT")
    gfx_sim_proj.setMarkup(True)
    gfx_sim_proj.setEnabled(False)
    gfx_sim_proj.setDependencies(toggleFileGen, ["fileGenToggle"])


# Callbacks
# On Attachment Connected Callback
def onAttachmentConnected(source, target):
    if target["id"] == "sys_time_TMR_dependency":
        # Remove Timer Help Comment
        tmr_subsys_help_comment.setVisible(False)
        tmr_subsys_tick_res.setReadOnly(False)
    if target["id"] in ["gfx_driver", "gfx_display_driver"]:
        print(comp_id_str + "File Generation Enabled")
        file_gen_toggle.setValue(True)
        if target["component"].getID() == "gfx_canvas":
            print(comp_id_str + "Canvas Mode Enabled")
            ctrl_subsys_canvas_mode.setValue(listCtrlCanvasMode[1])
            ctrl_subsys_help_comment.setVisible(False)


# On Attachment Disconnected Callback
def onAttachmentDisconnected(source, target):
    if target["id"] == "sys_time_TMR_dependency":
        # Reset Timer Tick Mode symbol
        tmr_subsys_tick_mode.setValue(listTmrTickMode[0])
        # Restore Timer Help Comment
        tmr_subsys_help_comment.setVisible(True)
        tmr_subsys_tick_res.setReadOnly(True)
    if target["id"] in ["gfx_driver", "gfx_display_driver"]:
        print(comp_id_str + "File Generation Disabled")
        file_gen_toggle.setValue(False)
        if target["component"].getID() == "gfx_canvas":
            print(comp_id_str + "Canvas Mode Disabled")
            ctrl_subsys_canvas_mode.setValue(listCtrlCanvasMode[0])
            ctrl_subsys_help_comment.setVisible(True)


# Toggle File Generation
def toggleFileGen(symbol, event):
    if event["value"]:
        symbol.setEnabled(True)
    else:
        symbol.setEnabled(False)


# Update CtrlNumLayers Callback
def onCtrlNumLayersSet(source, event):
    valCrtlNumLayers = source.getValue()
    print(comp_id_str + "Num of controller layers updated to " + str(valCrtlNumLayers))


# On DisplayType Change
def onDisplayTypeChanged(source, event):
    disp_type_new = source.getValue()
    print(comp_id_str + "Display Type modified to " + disp_type_new)
    if disp_type_new == "Custom":
        disp_subsys_hor_res.setReadOnly(False)
        disp_subsys_ver_res.setReadOnly(False)
        disp_subsys_circ_toggle.setReadOnly(False)
    else:
        # Assumimg disp_name is unique!
        idx = index2d(listDispSupport, disp_type_new)
        disp_subsys_hor_res.setValue(listDispSupport[idx[0][0]][1])
        disp_subsys_ver_res.setValue(listDispSupport[idx[0][0]][2])
        disp_subsys_circ_toggle.setValue(listDispSupport[idx[0][0]][4])
        disp_subsys_hor_res.setReadOnly(True)
        disp_subsys_ver_res.setReadOnly(True)
        disp_subsys_circ_toggle.setReadOnly(True)


# Console options visibility
def consoleEnabled(symbol, event):
    if event["value"] == True:
        symbol.setVisible(True)
    else:
        symbol.setVisible(False)


# Handle Message Callback
def handleMessage(messageID, args):
    return_dict = {}

    # Publish capability to TIME System Service
    if messageID == "SYS_TIME_PUBLISH_CAPABILITIES":
        # Advertise Tickless Mode
        return_dict = Database.sendMessage(args["ID"], "SYS_TIME_PLIB_CAPABILITY", {"plib_mode": "COMPARE_MODE"})
        # Set Timer Tick Mode symbol to Tickless
        tmr_subsys_tick_mode.setValue(listTmrTickMode[1])

    return return_dict


# Utils
# Get indices for an item in a 2d list
# Returns a list of tuples for each matching item [(list_idx, sublist_idx), ...]
def index2d(list, item):
    return [(i, sublist.index(item)) for (i, sublist) in enumerate(list) if item in sublist]
