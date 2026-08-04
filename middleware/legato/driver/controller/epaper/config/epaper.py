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

	execfile(Module.getPath() + "/config/config.py")
	execfile(Module.getPath() + "/config/files.py")
	execfile(Module.getPath() + "/config/rtos.py")

def showRTOSMenu(source, event):
	source.setVisible(event["value"] != "BareMetal")

def updateDisplayResolution(symbol, vendor, display):
	width = 0
	height = 0
	if (vendor == "AMP"):
		if (display == "E0352A01-AF"):
			width = 240
			height = 416
		elif (display == "AXE37A3AFS"):
			width = 240
			height = 360
	elif (vendor == "MikroE"):
		if (display == "E-paper Bundle 2"):
			width = 120
			height = 250

	symbol.getComponent().getSymbolByID("EPDWidth").setValue(width)
	symbol.getComponent().getSymbolByID("EPDHeight").setValue(height)

def updateDisplayFiles(symbol, vendor, display):
	component = symbol.getComponent()

	# UltraChip UC8253 file symbols
	uc8253Files = [
		"GFX_EPD_UC8253_C",
		"GFX_EPD_UC8253_H",
		"GFX_EPD_UC8253_LOCAL_H",
		"GFX_EPD_UC8253_LUT_C",
		"GFX_EPD_UC8253_LUT_H"
	]

	# Waveshare MIKROE 2.13" file symbols
	mikroe2n13Files = [
		"GFX_EPD_MIKROE_2N13_C",
		"GFX_EPD_MIKROE_2N13_H",
		"GFX_EPD_MIKROE_2N13_LOCAL_H",
		"GFX_EPD_MIKROE_2N13_LUT_C",
		"GFX_EPD_MIKROE_2N13_LUT_H",
		"GFX_EPD_MIKROE_2N13_IMAGE_H"
	]

	# Disable all vendor-specific files first
	for fileId in uc8253Files:
		component.getSymbolByID(fileId).setEnabled(False)
	for fileId in mikroe2n13Files:
		component.getSymbolByID(fileId).setEnabled(False)

	# Enable files based on vendor and display selection
	if (vendor == "AMP"):
		if (display == "E0352A01-AF") or (display == "AXE37A3AFS"):
			for fileId in uc8253Files:
				component.getSymbolByID(fileId).setEnabled(True)
	elif (vendor == "MikroE"):
		if (display == "E-paper Bundle 2"):
			for fileId in mikroe2n13Files:
				component.getSymbolByID(fileId).setEnabled(True)

def onDisplayVendor(symbol, event):
	if (event["value"] == "AMP"):
		symbol.getComponent().getSymbolByID("MikroEDisplay").setVisible(False)
		symbol.getComponent().getSymbolByID("AMPDisplay").setVisible(True)
		displayModule = symbol.getComponent().getSymbolByID("AMPDisplay").getValue()
	elif (event["value"] == "MikroE"):
		symbol.getComponent().getSymbolByID("AMPDisplay").setVisible(False)
		symbol.getComponent().getSymbolByID("MikroEDisplay").setVisible(True)
		displayModule = symbol.getComponent().getSymbolByID("MikroEDisplay").getValue()
	
	updateDisplayResolution(symbol, event["value"], displayModule)
	updateDisplayFiles(symbol, event["value"], displayModule)

def onAmpDisplay(symbol, event):
	updateDisplayResolution(symbol, "AMP", event["value"])
	updateDisplayFiles(symbol, "AMP", event["value"])

def onMikroEDisplay(symbol, event):
	updateDisplayResolution(symbol, "MikroE", event["value"])
	updateDisplayFiles(symbol, "MikroE", event["value"])	
	