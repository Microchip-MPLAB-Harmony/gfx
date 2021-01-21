def instantiateComponent(comp):

	SYS_DEFINITIONS_H = comp.createFileSymbol("SYS_DEFINITIONS_H", None)
	SYS_DEFINITIONS_H.setType("STRING")
	SYS_DEFINITIONS_H.setOutputName("core.LIST_SYSTEM_DEFINITIONS_H_INCLUDES")
	SYS_DEFINITIONS_H.setSourcePath("templates/definitions.h.ftl")
	SYS_DEFINITIONS_H.setMarkup(True)

	SYS_INIT_C = comp.createFileSymbol("SYS_INIT_C", None)
	SYS_INIT_C.setType("STRING")
	SYS_INIT_C.setOutputName("core.LIST_SYSTEM_INIT_C_SYS_INITIALIZE_DRIVERS")
	SYS_INIT_C.setSourcePath("templates/init.c.ftl")
	SYS_INIT_C.setMarkup(True)

	SYS_TASK_C = comp.createFileSymbol("SYS_TASK_C", None)
	SYS_TASK_C.setType("STRING")
	SYS_TASK_C.setOutputName("core.LIST_SYSTEM_TASKS_C_CALL_DRIVER_TASKS")
	SYS_TASK_C.setSourcePath("templates/tasks.c.ftl")
	SYS_TASK_C.setMarkup(True)
	
	SYS_RTOS_TASK_C = comp.createFileSymbol("SYS_RTOS_TASK_C", None)
	SYS_RTOS_TASK_C.setType("STRING")
	SYS_RTOS_TASK_C.setOutputName("core.LIST_SYSTEM_RTOS_TASKS_C_DEFINITIONS")
	SYS_RTOS_TASK_C.setSourcePath("templates/rtos_tasks.c.ftl")
	SYS_RTOS_TASK_C.setMarkup(True)

	execfile(Module.getPath() + "/config/ssd1963_config.py")
	execfile(Module.getPath() + "/config/ssd1963_files.py")
	execfile(Module.getPath() + "/config/ssd1963_rtos.py")

def configureDisplaySettings(lccComponent, displayComponent):
	lccComponent.setSymbolValue("DisplayHorzPulseWidth", displayComponent.getSymbolByID("HorzPulseWidth").getValue())
	lccComponent.setSymbolValue("DisplayHorzBackPorch", displayComponent.getSymbolByID("HorzBackPorch").getValue())
	lccComponent.setSymbolValue("DisplayHorzFrontPorch", displayComponent.getSymbolByID("HorzFrontPorch").getValue())
	lccComponent.setSymbolValue("DisplayVertPulseWidth", displayComponent.getSymbolByID("VertPulseWidth").getValue())
	lccComponent.setSymbolValue("DisplayVertBackPorch", displayComponent.getSymbolByID("VertBackPorch").getValue())
	lccComponent.setSymbolValue("DisplayVertFrontPorch", displayComponent.getSymbolByID("VertFrontPorch").getValue())
	lccComponent.setSymbolValue("DisplayWidth", displayComponent.getSymbolByID("DisplayWidth").getValue())
	lccComponent.setSymbolValue("DisplayHeight", displayComponent.getSymbolByID("DisplayHeight").getValue())
	if (Database.getComponentByID("gfx_hal_le") is not None):
		Database.setSymbolValue("gfx_hal_le", "gfx_display", displayComponent.getDisplayName(), 1)
	
def resetDisplaySettings(lccComponent, displayComponent):
	lccComponent.clearSymbolValue("DisplayHorzPulseWidth")
	lccComponent.clearSymbolValue("DisplayHorzBackPorch")
	lccComponent.clearSymbolValue("DisplayHorzFrontPorch")
	lccComponent.clearSymbolValue("DisplayVertPulseWidth")
	lccComponent.clearSymbolValue("DisplayVertBackPorch")
	lccComponent.clearSymbolValue("DisplayVertFrontPorch")
	lccComponent.clearSymbolValue("DisplayWidth")
	lccComponent.clearSymbolValue("DisplayHeight")

def onAttachmentConnected(source, target):
	print("dependency Connected = " + target["component"].getDisplayName())
	gfxCoreComponentTable = ["gfx_hal_le"]
	if (Database.getComponentByID("gfx_hal_le") is None):
		Database.activateComponents(gfxCoreComponentTable)
	updateDisplayManager(source["component"], target)
	if source["id"] == "Display Interface":
		print(source["component"].getID() + ": Using " + target["component"].getID() + " interface ")
		InterfaceType = str(target["component"].getSymbolByID("InterfaceType").getValue())
		source["component"].getSymbolByID("DisplayInterface").setValue(target["id"], 1)
		source["component"].getSymbolByID("DisplayInterfaceType").setValue(InterfaceType, 1)
		if "Parallel" in InterfaceType:
			if InterfaceType == "Parallel 8-bit":
				source["component"].getSymbolByID("ParallelInterfaceWidth").setValue("8-bit", True)
		else:
			print("Interface does not contain 'InterfaceType' capability")
	elif (source["id"] == "Graphics Display"):
		configureDisplaySettings(source["component"], target["component"])

def onAttachmentDisconnected(source, target):
	if source["id"] == "Display Interface":
		source["component"].getSymbolByID("ParallelInterfaceWidth").setReadOnly(False)
	elif (source["id"] == "Graphics Display"):
		resetDisplaySettings(source["component"], target["component"])

def onPixelClockSet(pixelClockSet, event):
	MasterClock = pixelClockSet.getComponent().getSymbolValue("MasterClock")
	prescalerValue = float(MasterClock/float(event["value"]))
	strValue = str(float("{0:.4f}".format(prescalerValue)))
	pixelClockSet.getComponent().getSymbolByID("PixelClockPreScaler").setValue(strValue, 1)
	if (Database.getComponentByID("gfx_hal_le") is not None):
		Database.setSymbolValue("gfx_hal_le", "PixelClock", event["value"], 1)
		
def onBacklightPWMFrequencySet(pixelClockSet, event):
	MasterClock = pixelClockSet.getComponent().getSymbolValue("MasterClock")
	prescalerValue = float(MasterClock/float(event["value"]))
	strValue = str(float("{0:.4f}".format(prescalerValue)))
	pixelClockSet.getComponent().getSymbolByID("PixelClockPreScaler").setValue(strValue, 1)

def showRTOSMenu(symbol, event):
	symbol.setVisible(event["value"] != "BareMetal")

def updateDisplayManager(component, target):
	if (Database.getComponentByID("gfx_hal_le") is not None):
		if target["id"] == "gfx_display":
			Database.setSymbolValue("gfx_hal_le", "gfx_display", component.getDependencyComponent("Graphics Display").getID(), 1)
			Database.setSymbolValue("gfx_hal_le", "DisplayName", component.getDependencyComponent("Graphics Display").getDisplayName(), 1)
			Database.setSymbolValue("gfx_hal_le", "DisplayWidth", target["component"].getSymbolValue("DisplayWidth"), 1)    
			Database.setSymbolValue("gfx_hal_le", "DisplayHeight", target["component"].getSymbolValue("DisplayHeight"), 1)
			Database.setSymbolValue("gfx_hal_le", "DisplayHorzPulseWidth", target["component"].getSymbolValue("HorzPulseWidth"), 1)    
			Database.setSymbolValue("gfx_hal_le", "DisplayHorzBackPorch", target["component"].getSymbolValue("HorzBackPorch"), 1)
			Database.setSymbolValue("gfx_hal_le", "DisplayHorzFrontPorch", target["component"].getSymbolValue("HorzFrontPorch"), 1)    
			Database.setSymbolValue("gfx_hal_le", "DisplayVertPulseWidth", target["component"].getSymbolValue("VertPulseWidth"), 1)
			Database.setSymbolValue("gfx_hal_le", "DisplayVertBackPorch", target["component"].getSymbolValue("VertBackPorch"), 1)    
			Database.setSymbolValue("gfx_hal_le", "DisplayVertFrontPorch", target["component"].getSymbolValue("VertFrontPorch"), 1)
			Database.setSymbolValue("gfx_hal_le", "DisplayDataEnablePolarity", target["component"].getSymbolValue("DataEnablePolarity"), 1)
			Database.setSymbolValue("gfx_hal_le", "DisplayVSYNCNegative", target["component"].getSymbolValue("VSYNCNegative"), 1)
			Database.setSymbolValue("gfx_hal_le", "DisplayHSYNCNegative", target["component"].getSymbolValue("HSYNCNegative"), 1)
			component.setSymbolValue("DisplayWidth", target["component"].getSymbolValue("DisplayWidth"), 1)
			component.setSymbolValue("DisplayHeight", target["component"].getSymbolValue("DisplayHeight"), 1)
		Database.setSymbolValue("gfx_hal_le", "gfx_driver", component.getID(), 1)
		Database.setSymbolValue("gfx_hal_le", "DriverName", component.getDisplayName(), 1)
		Database.setSymbolValue("gfx_hal_le", "PixelClock", component.getSymbolValue("PixelClock"), 1)
		component.getSymbolByID("DisplaySettingsMenu").setVisible(False)
		component.getSymbolByID("HALComment").setVisible(True)
    