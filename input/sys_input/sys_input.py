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

def instantiateComponent(component):
	component.setHelpFile("../../docs/help_harmony_gfx_html_alias.h")
	#component.setHelp("IDH_HTML_GFX_CMP__7__Input_System_Service_Component")

	RTOSMenu = component.createMenuSymbol("RTOSMenu", None)
	RTOSMenu.setLabel("RTOS Settings")
	RTOSMenu.setDescription("RTOS Settings")
	RTOSMenu.setVisible(Database.getSymbolValue("HarmonyCore", "SELECT_RTOS") != "BareMetal")
	RTOSMenu.setDependencies(showInputRTOSMenu, ["HarmonyCore.SELECT_RTOS"])
	
	RTOSEnabled = component.createBooleanSymbol("RTOSEnabled", RTOSMenu)
	RTOSEnabled.setLabel("RTOS Support Enabled")
	RTOSEnabled.setDefaultValue(Database.getSymbolValue("HarmonyCore", "SELECT_RTOS") != "BareMetal")
	RTOSEnabled.setDependencies(enableInputRTOS, ["HarmonyCore.SELECT_RTOS"])
	RTOSEnabled.setVisible(False)

	RTOSStackSize = component.createIntegerSymbol("RTOSStackSize", RTOSMenu)
	RTOSStackSize.setLabel("Stack Size")
	RTOSStackSize.setMin(0)
	RTOSStackSize.setDefaultValue(1024)
	
	RTOSTaskPriority = component.createIntegerSymbol("RTOSTaskPriority", RTOSMenu)
	RTOSTaskPriority.setLabel("Task Priority")
	RTOSTaskPriority.setDefaultValue(1)
	
	RTOSUseDelay = component.createBooleanSymbol("RTOSUseDelay", RTOSMenu)
	RTOSUseDelay.setLabel("Use Task Delay?")
	RTOSUseDelay.setDefaultValue(True)
	
	RTOSTaskDelay = component.createIntegerSymbol("RTOSTaskDelay", RTOSMenu)
	RTOSTaskDelay.setLabel("Task Delay")
	RTOSTaskDelay.setDefaultValue(10)
	RTOSTaskDelay.setMin(0)

	enableDemoMode = component.createBooleanSymbol("enableDemoMode", None)
	enableDemoMode.setLabel("Enable Demo mode?")
	enableDemoMode.setDescription("Indicates that Input Service should generate the code needed to run Legato in 'demo mode'.")
	enableDemoMode.setDependencies(onDemoModeEnable, ["enableDemoMode"])

	demoModeIdleTimeout = component.createIntegerSymbol("demoModeIdleTimeout", enableDemoMode)
	demoModeIdleTimeout.setLabel("Idle Timeout (seconds)")
	demoModeIdleTimeout.setDefaultValue(20)
	demoModeIdleTimeout.setVisible(False)

	demoModeReplayDelay = component.createIntegerSymbol("demoModeReplayDelay", enableDemoMode)
	demoModeReplayDelay.setLabel("Replay Delay (seconds)")
	demoModeReplayDelay.setDefaultValue(5)
	demoModeReplayDelay.setVisible(False)

	projectPath = "config/" + Variables.get("__CONFIGURATION_NAME") + "/system/input"
	
	SYS_INPUT_H = component.createFileSymbol("SYS_INPUT_H", None)
	SYS_INPUT_H.setSourcePath("inc/sys_input.h")
	SYS_INPUT_H.setDestPath("system/input/")
	SYS_INPUT_H.setOutputName("sys_input.h")
	SYS_INPUT_H.setProjectPath(projectPath)
	SYS_INPUT_H.setType("HEADER")
	
	SYS_INPUT_GESTURE_H = component.createFileSymbol("SYS_INPUT_GESTURE_H", None)
	SYS_INPUT_GESTURE_H.setSourcePath("inc/sys_input_gesture.h")
	SYS_INPUT_GESTURE_H.setDestPath("system/input/")
	SYS_INPUT_GESTURE_H.setOutputName("sys_input_gesture.h")
	SYS_INPUT_GESTURE_H.setProjectPath(projectPath)
	SYS_INPUT_GESTURE_H.setType("HEADER")
	
	SYS_INPUT_KEYBOARD_H = component.createFileSymbol("SYS_INPUT_KEYBOARD_H", None)
	SYS_INPUT_KEYBOARD_H.setSourcePath("inc/sys_input_keyboard.h")
	SYS_INPUT_KEYBOARD_H.setDestPath("system/input/")
	SYS_INPUT_KEYBOARD_H.setOutputName("sys_input_keyboard.h")
	SYS_INPUT_KEYBOARD_H.setProjectPath(projectPath)
	SYS_INPUT_KEYBOARD_H.setType("HEADER")
	
	SYS_INPUT_LISTENER_H = component.createFileSymbol("SYS_INPUT_LISTENER_H", None)
	SYS_INPUT_LISTENER_H.setSourcePath("inc/sys_input_listener.h")
	SYS_INPUT_LISTENER_H.setDestPath("system/input/")
	SYS_INPUT_LISTENER_H.setOutputName("sys_input_listener.h")
	SYS_INPUT_LISTENER_H.setProjectPath(projectPath)
	SYS_INPUT_LISTENER_H.setType("HEADER")
	
	SYS_INPUT_MOUSE_H = component.createFileSymbol("SYS_INPUT_MOUSE_H", None)
	SYS_INPUT_MOUSE_H.setSourcePath("inc/sys_input_mouse.h")
	SYS_INPUT_MOUSE_H.setDestPath("system/input/")
	SYS_INPUT_MOUSE_H.setOutputName("sys_input_mouse.h")
	SYS_INPUT_MOUSE_H.setProjectPath(projectPath)
	SYS_INPUT_MOUSE_H.setType("HEADER")
	
	SYS_INPUT_TOUCH_H = component.createFileSymbol("SYS_INPUT_TOUCH_H", None)
	SYS_INPUT_TOUCH_H.setSourcePath("inc/sys_input_touch.h")
	SYS_INPUT_TOUCH_H.setDestPath("system/input/")
	SYS_INPUT_TOUCH_H.setOutputName("sys_input_touch.h")
	SYS_INPUT_TOUCH_H.setProjectPath(projectPath)
	SYS_INPUT_TOUCH_H.setType("HEADER")
	
	SYS_INPUT_C = component.createFileSymbol("SYS_INPUT_C", None)
	SYS_INPUT_C.setSourcePath("templates/sys_input.c.ftl")
	SYS_INPUT_C.setDestPath("system/input/")
	SYS_INPUT_C.setOutputName("sys_input.c")
	SYS_INPUT_C.setProjectPath(projectPath)
	SYS_INPUT_C.setType("SOURCE")
	SYS_INPUT_C.setMarkup(True)
	
	SYS_INPUT_LISTENER_C = component.createFileSymbol("SYS_INPUT_LISTENER_C", None)
	SYS_INPUT_LISTENER_C.setSourcePath("src/sys_input_listener.c")
	SYS_INPUT_LISTENER_C.setDestPath("system/input/")
	SYS_INPUT_LISTENER_C.setOutputName("sys_input_listener.c")
	SYS_INPUT_LISTENER_C.setProjectPath(projectPath)
	SYS_INPUT_LISTENER_C.setType("SOURCE")

	INPUT_SYS_DEMO_MODE_H = component.createFileSymbol("INPUT_SYS_DEMO_MODE_H", None)
	INPUT_SYS_DEMO_MODE_H.setSourcePath("templates/sys_input_demo_mode.h.ftl")
	INPUT_SYS_DEMO_MODE_H.setDestPath("system/input/")
	INPUT_SYS_DEMO_MODE_H.setOutputName("sys_input_demo_mode.h")
	INPUT_SYS_DEMO_MODE_H.setProjectPath(projectPath)
	INPUT_SYS_DEMO_MODE_H.setType("HEADER")
	INPUT_SYS_DEMO_MODE_H.setMarkup(True)
	INPUT_SYS_DEMO_MODE_H.setEnabled(False)

	INPUT_SYS_DEMO_MODE_C = component.createFileSymbol("INPUT_SYS_DEMO_MODE_C", None)
	INPUT_SYS_DEMO_MODE_C.setSourcePath("src/sys_input_demo_mode.c")
	INPUT_SYS_DEMO_MODE_C.setDestPath("system/input/")
	INPUT_SYS_DEMO_MODE_C.setOutputName("sys_input_demo_mode.c")
	INPUT_SYS_DEMO_MODE_C.setProjectPath(projectPath)
	INPUT_SYS_DEMO_MODE_C.setType("SOURCE")
	INPUT_SYS_DEMO_MODE_C.setEnabled(False)
	
	INPUT_SYS_DEFINITIONS_H = component.createFileSymbol("INPUT_SYS_DEFINITIONS_H", None)
	INPUT_SYS_DEFINITIONS_H.setType("STRING")
	INPUT_SYS_DEFINITIONS_H.setOutputName("core.LIST_SYSTEM_DEFINITIONS_H_INCLUDES")
	INPUT_SYS_DEFINITIONS_H.setSourcePath("templates/system/input_definitions.h.ftl")
	INPUT_SYS_DEFINITIONS_H.setMarkup(True)
	
	INPUT_SYS_INIT_C = component.createFileSymbol("INPUT_SYS_INIT_C", None)
	INPUT_SYS_INIT_C.setType("STRING")
	INPUT_SYS_INIT_C.setOutputName("core.LIST_SYSTEM_INIT_C_INITIALIZE_SYSTEM_SERVICES")
	INPUT_SYS_INIT_C.setSourcePath("templates/system/input_initialize.c.ftl")
	INPUT_SYS_INIT_C.setMarkup(True)

	INPUT_SYS_TASK_C = component.createFileSymbol("INPUT_SYS_TASK_C", None)
	INPUT_SYS_TASK_C.setType("STRING")
	INPUT_SYS_TASK_C.setOutputName("core.LIST_SYSTEM_TASKS_C_CALL_LIB_TASKS")
	INPUT_SYS_TASK_C.setSourcePath("templates/system/input_tasks.c.ftl")
	INPUT_SYS_TASK_C.setMarkup(True)

	INPUT_SYS_RTOS_TASK_C = component.createFileSymbol("INPUT_SYS_RTOS_TASK_C", None)
	INPUT_SYS_RTOS_TASK_C.setType("STRING")
	INPUT_SYS_RTOS_TASK_C.setOutputName("core.LIST_SYSTEM_RTOS_TASKS_C_DEFINITIONS")
	INPUT_SYS_RTOS_TASK_C.setSourcePath("templates/system/input_rtos_tasks.c.ftl")
	INPUT_SYS_RTOS_TASK_C.setMarkup(True)
	INPUT_SYS_RTOS_TASK_C.setEnabled((Database.getSymbolValue("HarmonyCore", "SELECT_RTOS") != "BareMetal"))
	INPUT_SYS_RTOS_TASK_C.setDependencies(enableInputRTOSTask, ["HarmonyCore.SELECT_RTOS"])

def enableInputRTOS(symbol, event):
	if (event["value"] != "BareMetal"):
		symbol.setValue(True, 1)
	else:
		symbol.setValue(False, 1)

def enableInputRTOSTask(symbol, event):
	if (event["value"] != "BareMetal"):
		symbol.setEnabled(True)
	else:
		symbol.setEnabled(False)

def showInputRTOSMenu(symbol, event):
	if (event["value"] != "BareMetal"):
		symbol.setVisible(True)
	else:
		symbol.setVisible(False)
		
def onDemoModeEnable(enableDemoMode, event):
	enableDemoMode.getComponent().getSymbolByID("demoModeIdleTimeout").setVisible(event["value"])
	enableDemoMode.getComponent().getSymbolByID("demoModeReplayDelay").setVisible(event["value"])
	event["source"].getSymbolByID("INPUT_SYS_DEMO_MODE_H").setEnabled(event["value"])
	event["source"].getSymbolByID("INPUT_SYS_DEMO_MODE_C").setEnabled(event["value"])
