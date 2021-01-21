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

    if rtos_mode != None:
        if rtos_mode == "BareMetal":
            symbol.setValue("Asynchronous", 1)
        else:
            symbol.setValue("Synchronous", 1)

def instantiateComponent(comp):
    print("Instantiated GFX2D driver component")
    projectPath = "config/" + Variables.get("__CONFIGURATION_NAME") + "/gfx/driver/gfx2d"

    gfx2d_default_mode = "Synchronous"

    DriverInitName = comp.createStringSymbol("DriverInitName", None)
    DriverInitName.setLabel("Driver Name")
    DriverInitName.setDefaultValue("_gfx2dGraphicsProcessor")
    DriverInitName.setReadOnly(True)

    gfx2dSymPLIB = comp.createStringSymbol("DRV_GFX2D_PLIB", None)
    gfx2dSymPLIB.setLabel("PLIB Used")
    gfx2dSymPLIB.setReadOnly(True)
    gfx2dSymPLIB.setDefaultValue("")

    gfx2dMode = comp.createComboSymbol("DRV_GFX2D_MODE", None, ["Asynchronous", "Synchronous"])
    gfx2dMode.setLabel("Driver Mode")
    gfx2dMode.setDefaultValue(gfx2d_default_mode)
    gfx2dMode.setDependencies(setCommonMode, ["HarmonyCore.SELECT_RTOS"])

    gfx2dSymQueueSize = comp.createIntegerSymbol("DRV_GFX2D_QUEUE_SIZE", None)
    gfx2dSymQueueSize.setLabel("Transfer Queue Size")
    gfx2dSymQueueSize.setMax(64)
    gfx2dSymQueueSize.setVisible((Database.getSymbolValue("drv_gfx2d", "DRV_GFX2D_MODE") == "Asynchronous"))
    gfx2dSymQueueSize.setDefaultValue(2)
    gfx2dSymQueueSize.setDependencies(asyncModeOptions, ["drv_gfx2d.DRV_GFX2D_MODE"])

    gfx2dScratchBufferSize = comp.createIntegerSymbol("SCRATCHBUFFER_SIZE", None)
    gfx2dScratchBufferSize.setLabel("Scratch Buffer Size")
    gfx2dScratchBufferSize.setMin(1)
    gfx2dScratchBufferSize.setDefaultValue(4096)

    # Add Interface
    GFX_GFX2D_H = comp.createFileSymbol("GFX_GFX2D_H", None)
    GFX_GFX2D_H.setSourcePath("templates/drv_gfx2d.h.ftl")
    GFX_GFX2D_H.setOutputName("drv_gfx2d.h")
    GFX_GFX2D_H.setDestPath("gfx/driver/processor/gfx2d/")
    GFX_GFX2D_H.setOutputName("drv_gfx2d.h")
    GFX_GFX2D_H.setProjectPath(projectPath)
    GFX_GFX2D_H.setType("HEADER")
    GFX_GFX2D_H.setMarkup(True)
    GFX_GFX2D_H.setOverwrite(True)

    # Add source file
    GFX_GFX2D_C = comp.createFileSymbol("GFX_GFX2D_C", None)
    GFX_GFX2D_C.setDestPath("gfx/driver/processor/gfx2d/")
    GFX_GFX2D_C.setOutputName("drv_gfx2d.c")
    GFX_GFX2D_C.setProjectPath(projectPath)
    GFX_GFX2D_C.setType("SOURCE")
    GFX_GFX2D_C.setMarkup(True)
    GFX_GFX2D_C.setSourcePath("templates/drv_gfx2d.c.ftl")

    # these two symbols are read by the HAL for initialization purposes
    # they must match the function names in the actual driver code
    ProcInfoFunction = comp.createStringSymbol("ProcInfoFunction", None)
    ProcInfoFunction.setLabel("Processor Info Function Name")
    ProcInfoFunction.setDefaultValue("procGFX2DInfoGet")
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

def configureGFX2DComponent(gpuComponent, comp):
	print("GFX2D Driver: Connecting GFX2D")
	
def resetGFX2DComponent(gpuComponent, comp):
	print("GFX2D Driver: Disconnecting GFX2D")

def onAttachmentConnected(source, target):
	print("plib dependency Connected = " + str(target['id']))
	#### test for GFX2D dependency

def onAttachmentDisconnected(source, target):
	print("plib dependency Disconnected = " + str(target['id']))

