# coding: utf-8
##############################################################################
# Copyright (C) 2020 Microchip Technology Inc. and its subsidiaries.
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

def instantiateComponent(comp):
	projectPath = "config/" + Variables.get("__CONFIGURATION_NAME") + "/gfx/driver/slcc"
	
	# these two symbols are read by the HAL for initialization purposes
	# they must match the function names in the actual driver code
	DriverInitName = comp.createStringSymbol("DriverInitName", None)
	DriverInitName.setVisible(False)
	DriverInitName.setReadOnly(True)
	DriverInitName.setDefaultValue("slcdDisplayDriver")
    
	TimerMenu = comp.createMenuSymbol("TIMER_MENU", None)
	TimerMenu.setLabel("Timer Settings")
	TimerMenu.setDescription("Timer Settings")
    
	TimerName = comp.createStringSymbol("TIMER_PLIB", TimerMenu)
	TimerName.setLabel("Timer PLIB")
	TimerName.setDescription("Timer PLIB Name")
	TimerName.setReadOnly(True)
	
	IOMenu = comp.createMenuSymbol("IO_MENU", None)
	IOMenu.setLabel("Pin Settings")
	IOMenu.setDescription("Pin Settings")
    
	IOComment = comp.createCommentSymbol("IO_COMMENT", IOMenu)
	IOComment.setLabel("COM and SEG pins must be defined and named in the Pin Manager.")
	IOComment.setVisible(True)
	
	ComPins = comp.createIntegerSymbol("COM_PINS", IOMenu)
	ComPins.setLabel("COM Pins")
	ComPins.setDefaultValue(4)
	ComPins.setDescription("Number of COM Pins (Back Planes). COM Pins must be named COMx, where x is the index.")
	ComPins.setMax(32)
	ComPins.setMin(1)
    
	COMComment = comp.createCommentSymbol("COM_COMMENT", ComPins)
	COMComment.setLabel("COM Pins must be named COMx, where x is the index.")
	COMComment.setVisible(True)	
	
	SegPins = comp.createIntegerSymbol("SEG_PINS", IOMenu)
	SegPins.setLabel("SEG Pins")
	SegPins.setDefaultValue(24)
	SegPins.setDescription("Number of SEG Pins (Front Planes). SEG Pins must be named SEGx, where x is the index.")
	SegPins.setMax(32)   
	SegPins.setMin(1)

	SEGComment = comp.createCommentSymbol("SEG_COMMENT", SegPins)
	SEGComment.setLabel("SEG Pins must be named SEGx, where x is the index.")
	SEGComment.setVisible(True)

	DataMenu = comp.createMenuSymbol("DATA_MENU", None)
	DataMenu.setLabel("Data Settings")
	DataMenu.setDescription("Data Settings")

	SegOffValue = comp.createHexSymbol("SEGOFF_VAL", DataMenu)
	SegOffValue.setLabel("Seg off Value")
	SegOffValue.setDefaultValue(0xaa)
	SegOffValue.setDescription("Value to turn OFF all segments")
	SegOffValue.setMax(0xff)
	SegOffValue.setMin(0)

	SegOnValue = comp.createHexSymbol("SEGON_VAL", DataMenu)
	SegOnValue.setLabel("Seg On Value")
	SegOnValue.setDefaultValue(0x55)
	SegOnValue.setDescription("Value to turn ON all segments")
	SegOnValue.setMax(0xff)
	SegOnValue.setMin(0)    

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

	execfile(Module.getPath() + "/config/slcc_rtos.py")

	# generated code files
	GFX_SLCC_C = comp.createFileSymbol("GFX_SLCC_C", None)
	GFX_SLCC_C.setDestPath("gfx/driver/controller/slcc/")
	GFX_SLCC_C.setSourcePath("templates/drv_gfx_slcc.c.ftl")
	GFX_SLCC_C.setOutputName("drv_gfx_slcc.c")
	GFX_SLCC_C.setProjectPath(projectPath)
	GFX_SLCC_C.setType("SOURCE")
	GFX_SLCC_C.setMarkup(True)
	
	GFX_SLCC_H = comp.createFileSymbol("GFX_SLCD_H", None)
	GFX_SLCC_H.setSourcePath("templates/drv_gfx_slcc.h.ftl")
	GFX_SLCC_H.setDestPath("gfx/driver/controller/slcc/")
	GFX_SLCC_H.setOutputName("drv_gfx_slcc.h")
	GFX_SLCC_H.setProjectPath(projectPath)
	GFX_SLCC_H.setType("HEADER")
	GFX_SLCC_H.setMarkup(True)
	

def onAttachmentConnected(source, target):
	print("dependency Connected = " + target["component"].getDisplayName())

def showRTOSMenu(symbol, event):
	symbol.setVisible(event["value"] != "BareMetal")
	
def onAttachmentConnected(source, target):
	localComponent = source["component"]
	remoteComponent = target["component"]
	remoteID = remoteComponent.getID()
	connectID = source["id"]
	
	print("dependency Connected = " + target["component"].getDisplayName())
	print("dependency ID = " + connectID)

	if (connectID == "TMR"):
		plibUsed = localComponent.getSymbolByID("TIMER_PLIB")
		plibUsed.clearValue()
		plibUsed.setValue(remoteID.upper())
		#Request PLIB to publish it's capabilities

def onAttachmentDisconnected(source, target):

	localComponent = source["component"]
	remoteComponent = target["component"]
	remoteID = remoteComponent.getID()
	connectID = source["id"]
	targetID = target["id"]

	if (connectID == "TMR"):
		plibUsed = localComponent.getSymbolByID("TIMER_PLIB")
		plibUsed.clearValue()
		plibUsed.setValue("")

