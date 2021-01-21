# generate code files
projectPath = "config/" + Variables.get("__CONFIGURATION_NAME") + "/driver/input"

DRV_TOUCH_CONTROLLER_C = comp.createFileSymbol("DRV_TOUCH_CONTROLLER_C", None)
DRV_TOUCH_CONTROLLER_C.setDestPath("driver/input/")
DRV_TOUCH_CONTROLLER_C.setSourcePath("templates/drv_touch_controller.c.ftl")
DRV_TOUCH_CONTROLLER_C.setOutputName("drv_touch_controller.c")
DRV_TOUCH_CONTROLLER_C.setProjectPath(projectPath)
DRV_TOUCH_CONTROLLER_C.setType("SOURCE")
DRV_TOUCH_CONTROLLER_C.setMarkup(True)

DRV_TOUCH_CONTROLLER_H = comp.createFileSymbol("DRV_TOUCH_CONTROLLER_H", None)
DRV_TOUCH_CONTROLLER_H.setDestPath("driver/input/")
DRV_TOUCH_CONTROLLER_H.setSourcePath("templates/drv_touch_controller.h.ftl")
DRV_TOUCH_CONTROLLER_H.setOutputName("drv_touch_controller.h")
DRV_TOUCH_CONTROLLER_H.setProjectPath(projectPath)
DRV_TOUCH_CONTROLLER_H.setType("HEADER")
DRV_TOUCH_CONTROLLER_H.setMarkup(True)

DRV_TOUCH_CONTROLLER_DEFINITIONS_H = comp.createFileSymbol("DRV_TOUCH_CONTROLLER_DEFINITIONS_H", None)
DRV_TOUCH_CONTROLLER_DEFINITIONS_H.setType("STRING")
DRV_TOUCH_CONTROLLER_DEFINITIONS_H.setOutputName("core.LIST_SYSTEM_DEFINITIONS_H_INCLUDES")
DRV_TOUCH_CONTROLLER_DEFINITIONS_H.setSourcePath("templates/drv_touch_controller_definitions.h.ftl")
DRV_TOUCH_CONTROLLER_DEFINITIONS_H.setMarkup(True)

DRV_TOUCH_CONTROLLER_TASK_C = comp.createFileSymbol("DRV_TOUCH_CONTROLLER_TASK_C", None)
DRV_TOUCH_CONTROLLER_TASK_C.setType("STRING")
DRV_TOUCH_CONTROLLER_TASK_C.setOutputName("core.LIST_SYSTEM_TASKS_C_CALL_DRIVER_TASKS")
DRV_TOUCH_CONTROLLER_TASK_C.setSourcePath("templates/drv_touch_controller_tasks.c.ftl")
DRV_TOUCH_CONTROLLER_TASK_C.setMarkup(True)

DRV_TOUCH_CONTROLLER_RTOS_TASK_C = comp.createFileSymbol("DRV_TOUCH_CONTROLLER_RTOS_TASK_C", None)
DRV_TOUCH_CONTROLLER_RTOS_TASK_C.setType("STRING")
DRV_TOUCH_CONTROLLER_RTOS_TASK_C.setOutputName("core.LIST_SYSTEM_RTOS_TASKS_C_DEFINITIONS")
DRV_TOUCH_CONTROLLER_RTOS_TASK_C.setSourcePath("templates/drv_touch_controller_rtos_tasks.c.ftl")
DRV_TOUCH_CONTROLLER_RTOS_TASK_C.setMarkup(True)
DRV_TOUCH_CONTROLLER_RTOS_TASK_C.setEnabled((Database.getSymbolValue("HarmonyCore", "SELECT_RTOS") != "BareMetal"))
DRV_TOUCH_CONTROLLER_RTOS_TASK_C.setDependencies(enableTouchRTOSymbol, ["HarmonyCore.SELECT_RTOS"])