def instantiateComponent(comp):
	execfile(Module.getPath() + "/config/ssd1963_config.py")
	execfile(Module.getPath() + "/config/ssd1963_files.py")

def onHALConnected(halConnected, event):
	halConnected.getComponent().getSymbolByID("HALComment").setVisible(event["value"] == True)
	halConnected.getComponent().getSymbolByID("DisplaySettingsMenu").setVisible(event["value"] == False)
	halConnected.getComponent().getSymbolByID("PaletteMode").setVisible(event["value"] == False)

def onAttachmentConnected(source, target):
	if source["id"] == "Display Interface":
		print(source["component"].getID() + ": Using " + target["component"].getID() + " interface ")
		source["component"].getSymbolByID("ParallelInterfaceWidth").setVisible(False)
		InterfaceType = str(target["component"].getSymbolByID("InterfaceType").getValue())
		source["component"].getSymbolByID("DisplayInterface").setValue(target["id"], 1)
		source["component"].getSymbolByID("DisplayInterfaceType").setValue(InterfaceType, 1)
		if "Parallel" in InterfaceType:
			source["component"].getSymbolByID("ParallelInterfaceWidth").setVisible(True)
			if InterfaceType == "Parallel 8-bit":
				source["component"].getSymbolByID("ParallelInterfaceWidth").setValue("8-bit", True)
				source["component"].getSymbolByID("ParallelInterfaceWidth").setReadOnly(True)
			else:
				source["component"].getSymbolByID("ParallelInterfaceWidth").setReadOnly(False)
		else:
			print("Interface does not contain 'InterfaceType' capability")

def onPixelClockSet(pixelClockSet, event):
	MasterClock = pixelClockSet.getComponent().getSymbolValue("MasterClock")
	prescalerValue = float(MasterClock/float(event["value"]))
	strValue = str(float("{0:.4f}".format(prescalerValue)))
	pixelClockSet.getComponent().getSymbolByID("PixelClockPreScaler").setValue(strValue, 1)
	if (pixelClockSet.getComponent().getSymbolValue("HALConnected") == True):
		Database.setSymbolValue("gfx_hal", "PixelClockHint", event["value"], 1)
		
def onBacklightPWMFrequencySet(pixelClockSet, event):
	MasterClock = pixelClockSet.getComponent().getSymbolValue("MasterClock")
	prescalerValue = float(MasterClock/float(event["value"]))
	strValue = str(float("{0:.4f}".format(prescalerValue)))
	pixelClockSet.getComponent().getSymbolByID("PixelClockPreScaler").setValue(strValue, 1)
