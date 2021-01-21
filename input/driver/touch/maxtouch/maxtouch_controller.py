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

def syncFileGen(symbol, event):
    if event["value"] == "Synchronous":
       symbol.setEnabled(True)
    else:
       symbol.setEnabled(False)

def asyncFileGen(symbol, event):
    if event["value"] == "Asynchronous":
       symbol.setEnabled(True)
    else:
       symbol.setEnabled(False)

def setCommonMode(symbol, event):
    rtos_mode = event["value"]

    if rtos_mode != None:
        if rtos_mode == "BareMetal":
            symbol.setValue("Asynchronous")
        else:
            symbol.setValue("Synchronous")


def instantiateComponent(component):
	component.setHelpFile("../../../../docs/help_harmony_gfx_html_alias.h")
	#component.setHelp("IDH_HTML_CMP_GFX__8__MaxTouch_Controller_Component")

        rtos_mode = Database.getSymbolValue("HarmonyCore", "SELECT_RTOS")

        i2c_default_mode = "Asynchronous"

        if ((rtos_mode != "BareMetal") and (rtos_mode != None)):
            i2c_default_mode = "Synchronous"

        i2cMode = component.createComboSymbol("DRV_I2C_MODE", None, ["Asynchronous", "Synchronous"])
        i2cMode.setLabel("Driver Mode")
        i2cMode.setDefaultValue(i2c_default_mode)
        i2cMode.setDependencies(setCommonMode, ["HarmonyCore.SELECT_RTOS"])

        ChgMode = component.createComboSymbol("ChgMode", None, ["Polling", "Interrupt"])
        ChgMode.setLabel("CHG Mode")
        ChgMode.setDefaultValue("Polling")
	
	Width = component.createIntegerSymbol("Width", None)
	Width.setLabel("Width")
	Width.setDescription("The width of the touch panel in pixels.")
	Width.setDefaultValue(480)

	Height = component.createIntegerSymbol("Height", None)
	Height.setLabel("Height")
	Height.setDescription("The height of the touch panel in pixels.")
	Height.setDefaultValue(272)

	projectPath = "config/" + Variables.get("__CONFIGURATION_NAME") + "/driver/input"
	
	DRV_MAXTOUCH_H = component.createFileSymbol("DRV_MAXTOUCH_H", None)
	DRV_MAXTOUCH_H.setSourcePath("inc/drv_maxtouch.h")
	DRV_MAXTOUCH_H.setDestPath("driver/input/")
	DRV_MAXTOUCH_H.setProjectPath(projectPath)
	DRV_MAXTOUCH_H.setType("HEADER")
        DRV_MAXTOUCH_H.setOverwrite(True)

        # Async Source Files
        DRV_ASYNC_MAXTOUCH_C = component.createFileSymbol("DRV_MAXTOUCH_C", None)
        DRV_ASYNC_MAXTOUCH_C.setSourcePath("src/async/drv_maxtouch.c.ftl")
        DRV_ASYNC_MAXTOUCH_C.setOutputName("drv_maxtouch.c")
        DRV_ASYNC_MAXTOUCH_C.setDestPath("driver/input/")
        DRV_ASYNC_MAXTOUCH_C.setProjectPath(projectPath)
        DRV_ASYNC_MAXTOUCH_C.setType("SOURCE")
        DRV_ASYNC_MAXTOUCH_C.setOverwrite(True)
        DRV_ASYNC_MAXTOUCH_C.setEnabled((i2cMode.getValue() == "Asynchronous"))
        DRV_ASYNC_MAXTOUCH_C.setDependencies(asyncFileGen, ["DRV_I2C_MODE"])
	DRV_ASYNC_MAXTOUCH_C.setMarkup(True)

        # Sync Source Files
        DRV_SYNC_MAXTOUCH_C = component.createFileSymbol("DRV_I2C_SYNC_SRC", None)
        DRV_SYNC_MAXTOUCH_C.setSourcePath("src/sync/drv_maxtouch.c.ftl")
        DRV_SYNC_MAXTOUCH_C.setOutputName("drv_maxtouch.c")
        DRV_SYNC_MAXTOUCH_C.setDestPath("driver/input/")
        DRV_SYNC_MAXTOUCH_C.setProjectPath(projectPath)
        DRV_SYNC_MAXTOUCH_C.setType("SOURCE")
        DRV_SYNC_MAXTOUCH_C.setOverwrite(True)
        DRV_SYNC_MAXTOUCH_C.setEnabled((i2cMode.getValue() == "Synchronous"))
        DRV_SYNC_MAXTOUCH_C.setDependencies(syncFileGen, ["DRV_I2C_MODE"])
	DRV_SYNC_MAXTOUCH_C.setMarkup(True)
	
        I2CIndex = component.createIntegerSymbol("DRV_MAXTOUCH_I2C_INDEX", None)
	I2CIndex.setLabel("I2C Driver Index")
	I2CIndex.setDefaultValue(0)
	I2CIndex.setMin(0)
	
	RTOSMenu = component.createMenuSymbol("RTOSMenu", None)
	RTOSMenu.setLabel("RTOS Settings")
	RTOSMenu.setDescription("RTOS Settings")
	RTOSMenu.setVisible(Database.getSymbolValue("HarmonyCore", "SELECT_RTOS") != "BareMetal")
	RTOSMenu.setDependencies(showTouchRTOSMenu, ["HarmonyCore.SELECT_RTOS"])
	
	RTOSEnabled = component.createBooleanSymbol("RTOSEnabled", RTOSMenu)
	RTOSEnabled.setLabel("RTOS Support Enabled")
	RTOSEnabled.setDefaultValue(Database.getSymbolValue("HarmonyCore", "SELECT_RTOS") != "BareMetal")
	RTOSEnabled.setDependencies(enableTouchRTOSymbol, ["HarmonyCore.SELECT_RTOS"])
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

	TOUCH_DRV_CONFIG_H = component.createFileSymbol("TOUCH_DRV_CONFIG_H", None)
	TOUCH_DRV_CONFIG_H.setType("STRING")
	TOUCH_DRV_CONFIG_H.setOutputName("core.LIST_SYSTEM_CONFIG_H_DRIVER_CONFIGURATION")
	TOUCH_DRV_CONFIG_H.setSourcePath("templates/system/drv_maxtouch_defines.ftl")
	TOUCH_DRV_CONFIG_H.setMarkup(True)
	
	TOUCH_DRV_INIT_DATA = component.createFileSymbol("TOUCH_DRV_INIT_DATA", None)
	TOUCH_DRV_INIT_DATA.setType("STRING")
	TOUCH_DRV_INIT_DATA.setOutputName("core.LIST_SYSTEM_INIT_C_DRIVER_INITIALIZATION_DATA")
	TOUCH_DRV_INIT_DATA.setSourcePath("templates/system/drv_maxtouch_init.ftl")
	TOUCH_DRV_INIT_DATA.setMarkup(True)
	
	TOUCH_DRV_OBJ_DATA = component.createFileSymbol("TOUCH_DRV_OBJ_DATA", None)
	TOUCH_DRV_OBJ_DATA.setType("STRING")
	TOUCH_DRV_OBJ_DATA.setOutputName("core.LIST_SYSTEM_DEFINITIONS_H_OBJECTS")
	TOUCH_DRV_OBJ_DATA.setSourcePath("templates/system/drv_maxtouch_definitions_obj.h.ftl")
	TOUCH_DRV_OBJ_DATA.setMarkup(True)

	TOUCH_DRV_DEFINITIONS_H = component.createFileSymbol("TOUCH_DRV_DEFINITIONS_H", None)
	TOUCH_DRV_DEFINITIONS_H.setType("STRING")
	TOUCH_DRV_DEFINITIONS_H.setOutputName("core.LIST_SYSTEM_DEFINITIONS_H_INCLUDES")
	TOUCH_DRV_DEFINITIONS_H.setSourcePath("templates/system/drv_maxtouch_definitions.h.ftl")
	TOUCH_DRV_DEFINITIONS_H.setMarkup(True)
	
	TOUCH_DRV_INIT_C = component.createFileSymbol("TOUCH_DRV_INIT_C", None)
	TOUCH_DRV_INIT_C.setType("STRING")
	TOUCH_DRV_INIT_C.setOutputName("core.LIST_SYSTEM_INIT_C_SYS_INITIALIZE_DRIVERS")
	TOUCH_DRV_INIT_C.setSourcePath("templates/system/drv_maxtouch_initialize.c.ftl")
	TOUCH_DRV_INIT_C.setMarkup(True)

	TOUCH_DRV_TASK_C = component.createFileSymbol("TOUCH_DRV_TASK_C", None)
	TOUCH_DRV_TASK_C.setType("STRING")
	TOUCH_DRV_TASK_C.setOutputName("core.LIST_SYSTEM_TASKS_C_CALL_DRIVER_TASKS")
	TOUCH_DRV_TASK_C.setSourcePath("templates/system/drv_maxtouch_tasks.c.ftl")
	TOUCH_DRV_TASK_C.setMarkup(True)

	TOUCH_DRV_RTOS_TASK_C = component.createFileSymbol("TOUCH_DRV_RTOS_TASK_C", None)
	TOUCH_DRV_RTOS_TASK_C.setType("STRING")
	TOUCH_DRV_RTOS_TASK_C.setOutputName("core.LIST_SYSTEM_RTOS_TASKS_C_DEFINITIONS")
	TOUCH_DRV_RTOS_TASK_C.setSourcePath("templates/system/drv_maxtouch_rtos_tasks.c.ftl")
	TOUCH_DRV_RTOS_TASK_C.setMarkup(True)
	TOUCH_DRV_RTOS_TASK_C.setEnabled((Database.getSymbolValue("HarmonyCore", "SELECT_RTOS") != "BareMetal"))
	TOUCH_DRV_RTOS_TASK_C.setDependencies(enableTouchRTOSymbol, ["HarmonyCore.SELECT_RTOS"])
	
def onAttachmentConnected(source, target):
	#print(component)
	
	if source["id"] == "touch_panel":
		source["component"].setSymbolValue("Width", target["component"].getSymbolValue("TouchWidth"), 1)
		source["component"].setSymbolValue("Height", target["component"].getSymbolValue("TouchHeight"), 1)
		
	if source["id"] == "i2c":
		I2CIndex = source["component"].getSymbolByID("DRV_MAXTOUCH_I2C_INDEX")
		I2CIndex.setValue(int(target["component"].getID()[-1]), 1)
		I2CIndex.setReadOnly(True)
	
def onAttachmentDisconnected(source, target):
	if source["id"] == "touch_panel":
		source["component"].clearSymbolValue("Width")
		source["component"].clearSymbolValue("Height")
	
	if source["id"] == "i2c":
		I2CIndex = source["component"].getSymbolByID("DRV_MAXTOUCH_I2C_INDEX")
		I2CIndex.clearValue()
		I2CIndex.setReadOnly(False)
		
def enableTouchRTOSymbol(symbol, event):
    if (event["value"] != "BareMetal"):
        symbol.setEnabled(True)
    else:
        symbol.setEnabled(False)
		
def showTouchRTOSMenu(symbol, event):
	if (event["value"] != "BareMetal"):
		symbol.setVisible(True)
	else:
		symbol.setVisible(False)
