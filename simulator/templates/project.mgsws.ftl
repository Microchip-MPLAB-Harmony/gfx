# Generated File - Do Not Modify!
[properties]
sdk_min_ver = [1, 0, 0]
config_name = "${__CONFIGURATION_NAME}"
x_resolution = ${valDispHorRes}
y_resolution = ${valDispVerRes}
color_mode = "${CtrlColorMode?replace("_", "")}"
layer_count = ${TotalNumLayers}
canvas = ${(CtrlCanvasMode == "ENABLED")?then("true", "false")}
sys_time = ${(TmrTickMode != "TMR DISABLED")?then("true", "false")}
bezel = ${(isBezelEnabled == true)?then("true", "false")}
circular = ${(isDispCircular == true)?then("true", "false")}
responsive = ${(isRespEnabled == true)?then("true", "false")}
rtos_enabled = ${(HarmonyCore.SELECT_RTOS == "BareMetal")?then("false", "true")}
c_flags = "-${optimizationLevel}"
link_flags = "-sINITIAL_MEMORY=${initialMemory}MB -sSTACK_SIZE=${stackSize}KB -sALLOW_MEMORY_GROWTH"
applications = [<#t>
<#list 0..(HarmonyCore.GEN_APP_TASK_COUNT - 1) as i>
<#if ("HarmonyCore.GEN_APP_TASK_ENABLE_" + i)?eval == true>
"${("HarmonyCore.GEN_APP_TASK_NAME_" + i)?eval}",<#t>
</#if>
</#list>
]
[properties.console]
enable = ${(isConsoleEnabled == true)?then("true", "false")}
timestamp = ${(consoleTSEnabled == true)?then("true", "false")}
closed = ${(consoleStartClosed == true)?then("true", "false")}
