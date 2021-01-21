# coding: utf-8
##############################################################################
# Copyright (C) 2019 Microchip Technology Inc. and its subsidiaries.
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

def setCommonMode(symbol, event):
	rtos_mode = event["value"]


def instantiateComponent(comp):
	print("Instantiated 2DGPU nano2d driver component")
	projectPath = "config/" + Variables.get("__CONFIGURATION_NAME") + "/gfx/driver/2dgpu"

	nano2d_default_mode = "Synchronous"

	DriverInitName = comp.createStringSymbol("DriverInitName", None)
	DriverInitName.setLabel("Driver Name")
	DriverInitName.setDefaultValue("_2dgpuGraphicsProcessor")
	DriverInitName.setReadOnly(True)

	Nano2DSymLIB = comp.createStringSymbol("DRV_NANO2D_LIB", None)
	Nano2DSymLIB.setLabel("LIB Used")
	Nano2DSymLIB.setDefaultValue("libnano2d.a")
	Nano2DSymLIB.setReadOnly(True)

	Nano2DMode = comp.createComboSymbol("DRV_NANO2D_MODE", None, ["Asynchronous", "Synchronous"])
	Nano2DMode.setLabel("Driver Mode")
	Nano2DMode.setDefaultValue(nano2d_default_mode)
	Nano2DMode.setDependencies(setCommonMode, ["HarmonyCore.SELECT_RTOS"])
	#Nano2DMode.setReadOnly(True)

	Nano2DSymQueueSize = comp.createIntegerSymbol("DRV_NANO2D_QUEUE_SIZE", None)
	Nano2DSymQueueSize.setLabel("Transfer Queue Size")
	Nano2DSymQueueSize.setMax(64)
	Nano2DSymQueueSize.setVisible((Database.getSymbolValue("drv_2dgpu", "DRV_NANO2D_MODE") == "Asynchronous"))
	Nano2DSymQueueSize.setDefaultValue(2)
	Nano2DSymQueueSize.setDependencies(asyncModeOptions, ["drv_2dgpu.DRV_NANO2D_MODE"])
	Nano2DSymQueueSize.setReadOnly(True)

	#
	#GPU_DRIVER_H = comp.createFileSymbol("GPU_DRIVER_H", None)
	#GPU_DRIVER_H.setSourcePath("../gpu_driver.h")
	#GPU_DRIVER_H.setDestPath("gfx/driver/processor/")
	#GPU_DRIVER_H.setProjectPath(projectPath)
	#GPU_DRIVER_H.setOutputName("gpu_driver.h")
	#GPU_DRIVER_H.setType("HEADER")

		# common display controller header

	# Add Library
	libnano2d_a = comp.createLibrarySymbol("DRV_LIB_NANO2D", None)
	libnano2d_a.setDestPath("gfx/driver/processor/2dgpu/lib")
	libnano2d_a.setOutputName("libnano2d.a")
	libnano2d_a.setSourcePath("lib/libnano2d.a")

	# Add Interface
	Nano2DHeaderFile = comp.createFileSymbol("NANO2D_FILE_MAIN_HEADER", None)
	Nano2DHeaderFile.setSourcePath("templates/libnano2d.h.ftl")
	Nano2DHeaderFile.setOutputName("libnano2d.h")
	Nano2DHeaderFile.setDestPath("gfx/driver/processor/2dgpu")
	Nano2DHeaderFile.setProjectPath(projectPath)
	Nano2DHeaderFile.setType("HEADER")
	Nano2DHeaderFile.setMarkup(True)
	Nano2DHeaderFile.setOverwrite(True)

	# Add Adapter
	Nano2DFile = comp.createFileSymbol("NANO2D_FILE", None)
	Nano2DFile.setSourcePath("templates/libnano2d.c.ftl")
	Nano2DFile.setOutputName("libnano2d.c")
	Nano2DFile.setDestPath("gfx/driver/processor/2dgpu")
	Nano2DFile.setProjectPath(projectPath)
	Nano2DFile.setType("SOURCE")
	Nano2DFile.setMarkup(True)
	Nano2DFile.setOverwrite(True)

	# these two symbols are read by the HAL for initialization purposes
	# they must match the function names in the actual driver code
	ProcInfoFunction = comp.createStringSymbol("ProcInfoFunction", None)
	ProcInfoFunction.setLabel("Processor Info Function Name")
	ProcInfoFunction.setDefaultValue("procNANO2DInfoGet")
	ProcInfoFunction.setReadOnly(True)
	ProcInfoFunction.setVisible(False)

	# System level 
	ProcSysDefinitions = comp.createFileSymbol("SYS_DEFINITIONS_H", None)
	ProcSysDefinitions.setType("STRING")
	ProcSysDefinitions.setOutputName("core.LIST_SYSTEM_DEFINITIONS_H_INCLUDES")
	ProcSysDefinitions.setSourcePath("templates/definitions.h.ftl")
	ProcSysDefinitions.setMarkup(True)

	ProcSysInit = comp.createFileSymbol("SYS_INIT_C", None)
	ProcSysInit.setType("STRING")
	ProcSysInit.setOutputName("core.LIST_SYSTEM_INIT_C_SYS_INITIALIZE_DRIVERS")
	ProcSysInit.setSourcePath("templates/init.c.ftl")
	ProcSysInit.setMarkup(True)

def asyncModeOptions(symbol, event):
	if (event["value"] == "Asynchronous"):
		symbol.setVisible(True)
	else:
		symbol.setVisible(False)

def configure2DGPUComponent(gpuComponent, comp):
	print("2DGPU Nano2D Driver: Connecting Nano2D") 
	
def reset2DGPUComponent(gpuComponent, comp):
	print("2DGPU Nano2D Driver: Disconnecting Nano2D")

def onAttachmentConnected(source, target):
	print("dependency Connected = " + str(target['id']))
	#### test for 2DGPU dependency

def onAttachmentDisconnected(source, target):
	print("dependency Disconnected = " + str(target['id']))

