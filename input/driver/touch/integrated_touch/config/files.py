# generate code files
projectPath = "config/" + Variables.get("__CONFIGURATION_NAME") + "/driver/input"

DRV_TOUCH_ITD_C = comp.createFileSymbol("DRV_TOUCH_ITD_C", None)
DRV_TOUCH_ITD_C.setDestPath("driver/input/")
DRV_TOUCH_ITD_C.setSourcePath("templates/drv_touch_itd.c.ftl")
DRV_TOUCH_ITD_C.setOutputName("drv_touch_itd.c")
DRV_TOUCH_ITD_C.setProjectPath(projectPath)
DRV_TOUCH_ITD_C.setType("SOURCE")
DRV_TOUCH_ITD_C.setMarkup(True)

DRV_TOUCH_ITD_H = comp.createFileSymbol("DRV_TOUCH_ITD_H", None)
DRV_TOUCH_ITD_H.setDestPath("driver/input/")
DRV_TOUCH_ITD_H.setSourcePath("templates/drv_touch_itd.h.ftl")
DRV_TOUCH_ITD_H.setOutputName("drv_touch_itd.h")
DRV_TOUCH_ITD_H.setProjectPath(projectPath)
DRV_TOUCH_ITD_H.setType("HEADER")
DRV_TOUCH_ITD_H.setMarkup(True)

DRV_TOUCH_ITD_DEFINITIONS_H = comp.createFileSymbol("DRV_TOUCH_ITD_DEFINITIONS_H", None)
DRV_TOUCH_ITD_DEFINITIONS_H.setType("STRING")
DRV_TOUCH_ITD_DEFINITIONS_H.setOutputName("core.LIST_SYSTEM_DEFINITIONS_H_INCLUDES")
DRV_TOUCH_ITD_DEFINITIONS_H.setSourcePath("templates/drv_touch_itd_definitions.h.ftl")
DRV_TOUCH_ITD_DEFINITIONS_H.setMarkup(True)

DRV_TOUCH_ITD_INIT_C = comp.createFileSymbol("DRV_TOUCH_ITD_INIT_C", None)
DRV_TOUCH_ITD_INIT_C.setType("STRING")
DRV_TOUCH_ITD_INIT_C.setOutputName("core.LIST_SYSTEM_INIT_C_SYS_INITIALIZE_DRIVERS")
DRV_TOUCH_ITD_INIT_C.setSourcePath("templates/drv_touch_itd_initialize.c.ftl")
DRV_TOUCH_ITD_INIT_C.setMarkup(True)

DRV_TOUCH_ITD_TASK_C = comp.createFileSymbol("DRV_TOUCH_ITD_TASK_C", None)
DRV_TOUCH_ITD_TASK_C.setType("STRING")
DRV_TOUCH_ITD_TASK_C.setOutputName("core.LIST_SYSTEM_TASKS_C_CALL_DRIVER_TASKS")
DRV_TOUCH_ITD_TASK_C.setSourcePath("templates/drv_touch_itd_tasks.c.ftl")
DRV_TOUCH_ITD_TASK_C.setMarkup(True)

DRV_TOUCH_ITD_RTOS_TASK_C = comp.createFileSymbol("DRV_TOUCH_ITD_RTOS_TASK_C", None)
DRV_TOUCH_ITD_RTOS_TASK_C.setType("STRING")
DRV_TOUCH_ITD_RTOS_TASK_C.setOutputName("core.LIST_SYSTEM_RTOS_TASKS_C_DEFINITIONS")
DRV_TOUCH_ITD_RTOS_TASK_C.setSourcePath("templates/drv_touch_itd_rtos_tasks.c.ftl")
DRV_TOUCH_ITD_RTOS_TASK_C.setMarkup(True)
DRV_TOUCH_ITD_RTOS_TASK_C.setEnabled((Database.getSymbolValue("HarmonyCore", "SELECT_RTOS") != "BareMetal"))
DRV_TOUCH_ITD_RTOS_TASK_C.setDependencies(enableTouchRTOSymbol, ["HarmonyCore.SELECT_RTOS"])