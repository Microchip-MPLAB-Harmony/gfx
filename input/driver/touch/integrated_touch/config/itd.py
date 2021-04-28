def instantiateComponent(comp):
	execfile(Module.getPath() + "/config/files.py")
	execfile(Module.getPath() + "/config/config.py")

def onAttachmentConnected(source, target):
	if source["id"] == "touch_panel":
		source["component"].setSymbolValue("TouchWidth", target["component"].getSymbolValue("TouchWidth"), 1)
		source["component"].setSymbolValue("TouchHeight", target["component"].getSymbolValue("TouchHeight"), 1)

def onAttachmentDisconnected(source, target):
	if source["id"] == "touch_panel":
		source["component"].clearSymbolValue("TouchWidth")
		source["component"].clearSymbolValue("TouchHeight")

def showRTOSMenu(source, event):
	source.setVisible(event["value"] != "BareMetal")

def enableTouchRTOSymbol(source, event):
	source.getComponent().getSymbolByID("RTOSEnabled").setVisible(event['value'] != "BareMetal")

def showTouchRTOSMenu(source, event):
	source.getComponent().getSymbolByID("RTOSMenu").setVisible(event['value'] != "BareMetal")

