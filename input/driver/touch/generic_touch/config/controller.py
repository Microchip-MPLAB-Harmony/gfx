numCommands = 32
defaultNumCommands = 3
numParms = 8
defaultNumParms = 0

import re

def instantiateComponent(comp):
	global numCommands
	global defaultNumCommands
	global numArgs
	global defaultNumArgs

	execfile(Module.getPath() + "/config/files.py")
	execfile(Module.getPath() + "/config/config.py")

def onAttachmentConnected(source, target):
	if source["id"] == "touch_panel":
		source["component"].setSymbolValue("TouchWidth", target["component"].getSymbolValue("TouchWidth"), 1)
		source["component"].setSymbolValue("TouchHeight", target["component"].getSymbolValue("TouchHeight"), 1)
		
	if source["id"] == "i2c":
		I2CIndex = source["component"].getSymbolByID("I2CIndex")
		I2CIndex.setValue(int(target["component"].getID()[-1]), 1)
		I2CIndex.setReadOnly(True)
	
def onAttachmentDisconnected(source, target):
	if source["id"] == "touch_panel":
		source["component"].clearSymbolValue("TouchWidth")
		source["component"].clearSymbolValue("TouchHeight")
	
	if source["id"] == "i2c":
		I2CIndex = source["component"].getSymbolByID("I2CIndex")
		I2CIndex.clearValue()
		I2CIndex.setReadOnly(False)


def showRTOSMenu(source, event):
	source.setVisible(event["value"] != "BareMetal")

def onInitCommandsCountChanged(source, event):
	global numCommands
	print("Init Commands Countchanged : " + str(event["value"]))
	for x in range(numCommands - 1):
		if (x < int(event["value"])):
			source.getComponent().getSymbolByID("Command" + str(x)).setVisible(True)
		else:
			source.getComponent().getSymbolByID("Command" + str(x)).setVisible(False)

def onInitCommandParmsCountChanged(source, event):
	global numParms
	print("Init Commands Countchanged : " + str(event["id"]))
	sub = re.search('Command(.*)ParmsCount', str(event["id"]))
	if (sub and sub.group(1)):
		print("sub is : " + str((sub.group(1))))
		for x in range(numParms - 1):
			if (x < int(event["value"])):
				source.getComponent().getSymbolByID("Command" + str(sub.group(1)) + "Parm" + str(x)).setVisible(True)
			else:
				source.getComponent().getSymbolByID("Command" + str(sub.group(1)) + "Parm" + str(x)).setVisible(False)

def onTouchPointPosSizeChanged(source, event):
	source.getComponent().getSymbolByID("TouchPointPosXHIndex").setVisible(event['value'] == "2")
	source.getComponent().getSymbolByID("TouchPointPosXHMask").setVisible(event['value'] == "2")
	source.getComponent().getSymbolByID("TouchPointPosXHShift").setVisible(event['value'] == "2")
	source.getComponent().getSymbolByID("TouchPointPosYHIndex").setVisible(event['value'] == "2")
	source.getComponent().getSymbolByID("TouchPointPosYHMask").setVisible(event['value'] == "2")
	source.getComponent().getSymbolByID("TouchPointPosYHShift").setVisible(event['value'] == "2")
	
def onTouchDataProcessFunctionGenerateModeChanged(source, event):
	source.getComponent().getSymbolByID("TouchDataMenu").setVisible(event['value'] == "Generate")
	
def enableTouchRTOSymbol(source, event):
	source.getComponent().getSymbolByID("RTOSEnabled").setVisible(event['value'] != "BareMetal")

def showTouchRTOSMenu(source, event):
	source.getComponent().getSymbolByID("RTOSMenu").setVisible(event['value'] != "BareMetal")

