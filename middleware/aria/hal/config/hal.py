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

def instantiateComponent(halComponent):
	projectPath = "config/" + Variables.get("__CONFIGURATION_NAME") + "/gfx/hal"
	
	halComponent.setHelpFile("../docs/help_harmony_gfx_html_alias.h")
	#halComponent.setHelp("IDH_HTML_GFX_CMP__4__GFX_Core_Component")
	
	execfile(Module.getPath() + "/config/hal_pipeline.py")
	execfile(Module.getPath() + "/config/hal_display.py")
	execfile(Module.getPath() + "/config/hal_driver.py")
	execfile(Module.getPath() + "/config/hal_gpu.py")
	execfile(Module.getPath() + "/config/hal_hints.py")
	execfile(Module.getPath() + "/config/hal_files.py")
	
	GFXRTOSMenu = halComponent.createMenuSymbol("GFXRTOSMenu", None)
	GFXRTOSMenu.setLabel("RTOS settings")
	GFXRTOSMenu.setDescription("RTOS settings")
	GFXRTOSMenu.setVisible((Database.getSymbolValue("HarmonyCore", "SELECT_RTOS") != "BareMetal"))
	GFXRTOSMenu.setDependencies(showGFXRTOSMenu, ["HarmonyCore.SELECT_RTOS"])
	
	GFXRTOSTask = halComponent.createComboSymbol("GFXRTOSTask", GFXRTOSMenu, ["Standalone"])
	GFXRTOSTask.setLabel("Run Library Tasks As")
	GFXRTOSTask.setDefaultValue("Standalone")
	GFXRTOSTask.setVisible(False)
	
	GFXRTOSStackSize = halComponent.createIntegerSymbol("GFXRTOSStackSize", GFXRTOSMenu)
	GFXRTOSStackSize.setLabel("Stack Size")
	GFXRTOSStackSize.setDefaultValue(1024)

	GFXRTOSTaskPriority = halComponent.createIntegerSymbol("GFXRTOSTaskPriority", GFXRTOSMenu)
	GFXRTOSTaskPriority.setLabel("Task Priority")
	GFXRTOSTaskPriority.setDefaultValue(1)

	GFXRTOSTaskUseDelay = halComponent.createBooleanSymbol("GFXRTOSTaskUseDelay", GFXRTOSMenu)
	GFXRTOSTaskUseDelay.setLabel("Use Task Delay?")
	GFXRTOSTaskUseDelay.setDefaultValue(True)

	GFXRTOSTaskDelayVal = halComponent.createIntegerSymbol("GFXRTOSTaskDelayVal", GFXRTOSMenu)
	GFXRTOSTaskDelayVal.setLabel("Task Delay")
	GFXRTOSTaskDelayVal.setDefaultValue(10)
	GFXRTOSTaskDelayVal.setDependencies(showGFXRTOSTaskDel, ["GFXRTOSTaskDelay"])
	
	GFX_SYS_DEFINITIONS_H = halComponent.createFileSymbol("GFX_SYS_DEFINITIONS_H", None)
	GFX_SYS_DEFINITIONS_H.setType("STRING")
	GFX_SYS_DEFINITIONS_H.setOutputName("core.LIST_SYSTEM_DEFINITIONS_H_INCLUDES")
	GFX_SYS_DEFINITIONS_H.setSourcePath("templates/system/gfx_definitions.h.ftl")
	GFX_SYS_DEFINITIONS_H.setMarkup(True)

	GFX_SYS_INIT_C = halComponent.createFileSymbol("GFX_SYS_INIT_C", None)
	GFX_SYS_INIT_C.setType("STRING")
	GFX_SYS_INIT_C.setOutputName("core.LIST_SYSTEM_INIT_C_SYS_INITIALIZE_DRIVERS")
	GFX_SYS_INIT_C.setSourcePath("templates/system/gfx_initialize.c.ftl")
	GFX_SYS_INIT_C.setMarkup(True)

	GFX_DRV_TASK_C = halComponent.createFileSymbol("GFX_DRV_TASK_C", None)
	GFX_DRV_TASK_C.setType("STRING")
	GFX_DRV_TASK_C.setOutputName("core.LIST_SYSTEM_TASKS_C_CALL_DRIVER_TASKS")
	GFX_DRV_TASK_C.setSourcePath("templates/system/gfx_tasks.c.ftl")
	GFX_DRV_TASK_C.setMarkup(True)

	GFX_SYS_RTOS_TASK_C = halComponent.createFileSymbol("GFX_SYS_RTOS_TASK_C", None)
	GFX_SYS_RTOS_TASK_C.setType("STRING")
	GFX_SYS_RTOS_TASK_C.setOutputName("core.LIST_SYSTEM_RTOS_TASKS_C_DEFINITIONS")
	GFX_SYS_RTOS_TASK_C.setSourcePath("templates/system/gfx_rtos_tasks.c.ftl")
	GFX_SYS_RTOS_TASK_C.setMarkup(True)
	GFX_SYS_RTOS_TASK_C.setEnabled((Database.getSymbolValue("HarmonyCore", "SELECT_RTOS") != "BareMetal"))
	GFX_SYS_RTOS_TASK_C.setDependencies(getGFXRTOSTask, ["HarmonyCore.SELECT_RTOS"])
	
def showGFXRTOSMenu(symbol, event):
	if (event["value"] != "BareMetal"):
		symbol.setVisible(True)
	else:
		symbol.setVisible(False)

def showGFXRTOSTaskDel(symbol, enable):
	symbol.setVisible(enable["value"])
	
def onAttachmentConnected(source, target):
	if source["id"] == "gfx_display_driver":
		source["component"].setSymbolValue("DriverInfoFunction", target["component"].getSymbolValue("DriverInfoFunction"), 1)
		source["component"].setSymbolValue("DriverInitFunction", target["component"].getSymbolValue("DriverInitFunction"), 1)

def getGFXRTOSTask(symbol, event):
	if (event["value"] != "BareMetal"):
		symbol.setEnabled(True)
	else:
		symbol.setEnabled(False)

def onAttachmentConnected(source, target):
	if source["id"] == "gfx_display_driver":
		source["component"].setSymbolValue("DriverInfoFunction", target["component"].getSymbolValue("DriverInfoFunction"), 1)
		source["component"].setSymbolValue("DriverInitFunction", target["component"].getSymbolValue("DriverInitFunction"), 1)
		
		target["component"].setSymbolValue("HALConnected", True, 1)
		
		try:
			if (target["component"].getSymbolValue("DisplayTimingOptionsEnabled") == True):
				showDisplayTimingSettings(source["component"], target["component"])
			else:
				hideDisplayTimingSettings(source["component"], target["component"])
		except:
			pass

	if source["id"] == "gfx_graphics_processor":
		source["component"].setSymbolValue("ProcInfoFunction", target["component"].getSymbolValue("ProcInfoFunction"), 1)
		source["component"].setSymbolValue("ProcInitFunction", target["component"].getSymbolValue("ProcInitFunction"), 1)
		

	if source["id"] == "gfx_display":
		updateDisplayValues(source["component"], target["component"])
	
def onAttachmentDisconnected(source, target):
	if source["id"] == "gfx_display_driver":
		source["component"].clearSymbolValue("DriverInfoFunction")
		source["component"].clearSymbolValue("DriverInitFunction")
	
		target["component"].clearSymbolValue("HALConnected")
		
		try:
			if (target["component"].getSymbolValue("DisplayTimingOptionsEnabled") == True):
				hideDisplayTimingSettings(source["component"], target["component"])
		except:
			pass

	if source["id"] == "gfx_graphics_processor":
		source["component"].clearSymbolValue("ProcInfoFunction")
		source["component"].clearSymbolValue("ProcInitFunction")
	
	if source["id"] == "gfx_display":
		clearDisplayValues(source["component"])

def onDrawPipelineEnableChanged(menu, event):
	menu.setVisible(event["value"])

def updateDisplayValues(halComponent, displayComponent):
	halComponent.setSymbolValue("DisplayName", displayComponent.getSymbolValue("Name"), 1)
	halComponent.setSymbolValue("DisplayWidth", displayComponent.getSymbolValue("DisplayWidth"), 1)
	halComponent.setSymbolValue("DisplayHeight", displayComponent.getSymbolValue("DisplayHeight"), 1)
	halComponent.setSymbolValue("DisplayDataWidth", displayComponent.getSymbolValue("DataWidth"), 1)
	halComponent.setSymbolValue("DisplayHorzPulseWidth", displayComponent.getSymbolValue("HorzPulseWidth"), 1)
	halComponent.setSymbolValue("DisplayHorzBackPorch", displayComponent.getSymbolValue("HorzBackPorch"), 1)
	halComponent.setSymbolValue("DisplayHorzFrontPorch", displayComponent.getSymbolValue("HorzFrontPorch"), 1)
	halComponent.setSymbolValue("DisplayVertPulseWidth", displayComponent.getSymbolValue("VertPulseWidth"), 1)
	halComponent.setSymbolValue("DisplayVertBackPorch", displayComponent.getSymbolValue("VertBackPorch"), 1)
	halComponent.setSymbolValue("DisplayVertFrontPorch", displayComponent.getSymbolValue("VertFrontPorch"), 1)
	halComponent.setSymbolValue("DisplayInvLeftShift", displayComponent.getSymbolValue("InvLeftShift"), 1)
	halComponent.setSymbolValue("DisplayBacklightDisable", displayComponent.getSymbolValue("BacklightDisable"), 1)
	halComponent.setSymbolValue("DisplayBacklightEnable", displayComponent.getSymbolValue("BacklightEnable"), 1)
	halComponent.setSymbolValue("DisplayVSYNCNegative", displayComponent.getSymbolValue("VSYNCNegative"), 1)
	halComponent.setSymbolValue("DisplayHSYNCNegative", displayComponent.getSymbolValue("HSYNCNegative"), 1)
	halComponent.setSymbolValue("DisplayDataEnable", displayComponent.getSymbolValue("DataEnable"), 1)
	halComponent.setSymbolValue("DisplayDataEnablePolarity", displayComponent.getSymbolValue("DataEnablePolarity"), 1)
	halComponent.setSymbolValue("DisplayUseReset", displayComponent.getSymbolValue("UseReset"), 1)
	halComponent.setSymbolValue("DisplayResetPolarity", displayComponent.getSymbolValue("ResetPolarity"), 1)
	halComponent.setSymbolValue("DisplayUseChipSelect", displayComponent.getSymbolValue("UseChipSelect"), 1)
	halComponent.setSymbolValue("DisplayChipSelectPolarity", displayComponent.getSymbolValue("ChipSelectPolarity"), 1)

def clearDisplayValues(halComponent):
	halComponent.clearSymbolValue("DisplayName")
	halComponent.clearSymbolValue("DisplayWidth")
	halComponent.clearSymbolValue("DisplayHeight")
	halComponent.clearSymbolValue("DisplayDataWidth")
	halComponent.clearSymbolValue("DisplayHorzPulseWidth")
	halComponent.clearSymbolValue("DisplayHorzBackPorch")
	halComponent.clearSymbolValue("DisplayHorzFrontPorch")
	halComponent.clearSymbolValue("DisplayVertPulseWidth")
	halComponent.clearSymbolValue("DisplayVertBackPorch")
	halComponent.clearSymbolValue("DisplayVertFrontPorch")
	halComponent.clearSymbolValue("DisplayInvLeftShift")
	halComponent.clearSymbolValue("DisplayBacklightDisable")
	halComponent.clearSymbolValue("DisplayBacklightEnable")
	halComponent.clearSymbolValue("DisplayVSYNCNegative")
	halComponent.clearSymbolValue("DisplayHSYNCNegative")
	halComponent.clearSymbolValue("DisplayDataEnable")
	halComponent.clearSymbolValue("DisplayDataEnablePolarity")
	halComponent.clearSymbolValue("DisplayUseReset")
	halComponent.clearSymbolValue("DisplayResetPolarity")
	halComponent.clearSymbolValue("DisplayUseChipSelect")
	halComponent.clearSymbolValue("DisplayChipSelectPolarity")

def updateRefreshRate(halComponent):
	totalHorzTiming = int(halComponent.getSymbolValue("DisplayWidth")) + int(halComponent.getSymbolValue("DisplayHorzPulseWidth")) \
					+ int(halComponent.getSymbolValue("DisplayHorzBackPorch")) + int(halComponent.getSymbolValue("DisplayHorzFrontPorch"))
	totalVertTiming = int(halComponent.getSymbolValue("DisplayHeight")) + int(halComponent.getSymbolValue("DisplayVertPulseWidth")) \
					+ int(halComponent.getSymbolValue("DisplayVertBackPorch")) + int(halComponent.getSymbolValue("DisplayVertFrontPorch"))
	refreshRate = halComponent.getSymbolValue("PixelClock") / (totalHorzTiming * totalVertTiming)
	halComponent.setSymbolValue("RefreshRate", refreshRate, 1)

def showDisplayTimingSettings(halComponent, displayDriverComponent):
	halComponent.getSymbolByID("PixelClock").setVisible(True)
	halComponent.setSymbolValue("PixelClock", displayDriverComponent.getSymbolValue("PixelClock"), 1)
	halComponent.getSymbolByID("RefreshRate").setVisible(True)
	updateRefreshRate(halComponent)
	halComponent.getSymbolByID("DisplayHorzMenu").setVisible(True)
	halComponent.getSymbolByID("DisplayVertMenu").setVisible(True)

def hideDisplayTimingSettings(halComponent, displayDriverComponent):
	halComponent.getSymbolByID("PixelClock").setVisible(False)
	halComponent.getSymbolByID("RefreshRate").setVisible(False)
	halComponent.getSymbolByID("DisplayHorzMenu").setVisible(False)
	halComponent.getSymbolByID("DisplayVertMenu").setVisible(False)

def onUpdateDisplayTiming(symbol, event):
	halComponent = event["source"]
	updateRefreshRate(halComponent)

def onPixelClockSet(symbol, event):
	halComponent = event["source"]
	halComponent.setSymbolValue("PixelClock", event["value"], 1)
	updateRefreshRate(halComponent)

def onDisableHint(symbol, event):
	halComponent = event["source"]
	
	ColorModeHint = halComponent.getSymbolByID("ColorModeHint")
	ColorModeHint.setReadOnly(halComponent.getSymbolValue("DisableColorModeHint"))
	
	GlobalPaletteModeHint = halComponent.getSymbolByID("GlobalPaletteModeHint")
	GlobalPaletteModeHint.setReadOnly(halComponent.getSymbolValue("DisableGlobalPaletteModeHint"))
	
	DoubleBufferHint = halComponent.getSymbolByID("DoubleBufferHint")
	DoubleBufferHint.setReadOnly(halComponent.getSymbolValue("DisableDoubleBufferHint"))
	
	LCCRefreshHint = halComponent.getSymbolByID("LCCRefreshHint")
	LCCRefreshHint.setReadOnly(halComponent.getSymbolValue("DisableLCCRefreshHint"))
	
	Orientation = halComponent.getSymbolByID("DisplayOrientation")
	Orientation.setReadOnly(halComponent.getSymbolValue("DisableDisplayOrientation"))
