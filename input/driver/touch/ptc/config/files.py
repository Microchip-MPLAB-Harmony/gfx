# generate code files
projectPath = "config/" + Variables.get("__CONFIGURATION_NAME") + "/driver/input"

DRV_TOUCH_PTC_C = comp.createFileSymbol("DRV_TOUCH_PTC_C", None)
DRV_TOUCH_PTC_C.setDestPath("driver/input/")
DRV_TOUCH_PTC_C.setSourcePath("templates/drv_touch_ptc.c")
DRV_TOUCH_PTC_C.setOutputName("drv_touch_ptc.c")
DRV_TOUCH_PTC_C.setProjectPath(projectPath)
DRV_TOUCH_PTC_C.setType("SOURCE")
#DRV_TOUCH_PTC_C.setMarkup(True)

DRV_TOUCH_PTC_H = comp.createFileSymbol("DRV_TOUCH_PTC_H", None)
DRV_TOUCH_PTC_H.setDestPath("driver/input/")
DRV_TOUCH_PTC_H.setSourcePath("templates/drv_touch_ptc.h.ftl")
DRV_TOUCH_PTC_H.setOutputName("drv_touch_ptc.h")
DRV_TOUCH_PTC_H.setProjectPath(projectPath)
DRV_TOUCH_PTC_H.setType("HEADER")
DRV_TOUCH_PTC_H.setMarkup(True)

DRV_TOUCH_PTC_DEFINITIONS_H = comp.createFileSymbol("DRV_TOUCH_PTC_DEFINITIONS_H", None)
DRV_TOUCH_PTC_DEFINITIONS_H.setType("STRING")
DRV_TOUCH_PTC_DEFINITIONS_H.setOutputName("core.LIST_SYSTEM_DEFINITIONS_H_INCLUDES")
DRV_TOUCH_PTC_DEFINITIONS_H.setSourcePath("templates/drv_touch_ptc_definitions.h.ftl")
DRV_TOUCH_PTC_DEFINITIONS_H.setMarkup(True)

DRV_TOUCH_PTC_INIT_C = comp.createFileSymbol("DRV_TOUCH_PTC_INIT_C", None)
DRV_TOUCH_PTC_INIT_C.setType("STRING")
DRV_TOUCH_PTC_INIT_C.setOutputName("core.LIST_SYSTEM_INIT_C_SYS_INITIALIZE_DRIVERS")
DRV_TOUCH_PTC_INIT_C.setSourcePath("templates/drv_touch_ptc_initialize.c.ftl")
DRV_TOUCH_PTC_INIT_C.setMarkup(True)

DRV_TOUCH_PTC_TASK_C = comp.createFileSymbol("DRV_TOUCH_PTC_TASK_C", None)
DRV_TOUCH_PTC_TASK_C.setType("STRING")
DRV_TOUCH_PTC_TASK_C.setOutputName("core.LIST_SYSTEM_TASKS_C_CALL_DRIVER_TASKS")
DRV_TOUCH_PTC_TASK_C.setSourcePath("templates/drv_touch_ptc_tasks.c.ftl")
DRV_TOUCH_PTC_TASK_C.setMarkup(True)

DRV_TOUCH_PTC_RTOS_TASK_C = comp.createFileSymbol("DRV_TOUCH_PTC_RTOS_TASK_C", None)
DRV_TOUCH_PTC_RTOS_TASK_C.setType("STRING")
DRV_TOUCH_PTC_RTOS_TASK_C.setOutputName("core.LIST_SYSTEM_RTOS_TASKS_C_DEFINITIONS")
DRV_TOUCH_PTC_RTOS_TASK_C.setSourcePath("templates/drv_touch_ptc_rtos_tasks.c.ftl")
DRV_TOUCH_PTC_RTOS_TASK_C.setMarkup(True)
DRV_TOUCH_PTC_RTOS_TASK_C.setEnabled((Database.getSymbolValue("HarmonyCore", "SELECT_RTOS") != "BareMetal"))
DRV_TOUCH_PTC_RTOS_TASK_C.setDependencies(enableTouchRTOSymbol, ["HarmonyCore.SELECT_RTOS"])