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
            symbol.setValue("Synchronous", 1)
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
    DriverInitName.setVisible(False)

    gfx2dSymPLIB = comp.createStringSymbol("DRV_GFX2D_PLIB", None)
    gfx2dSymPLIB.setLabel("PLIB Used")
    gfx2dSymPLIB.setReadOnly(True)
    gfx2dSymPLIB.setDefaultValue("")
    gfx2dSymPLIB.setVisible(False)

    gfx2dMode = comp.createComboSymbol("DRV_GFX2D_MODE", None, ["Asynchronous", "Synchronous"])
    gfx2dMode.setLabel("Driver Mode")
    gfx2dMode.setDefaultValue(gfx2d_default_mode)
    gfx2dMode.setDependencies(setCommonMode, ["HarmonyCore.SELECT_RTOS"])
    gfx2dMode.setVisible(False)

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
    gfx2dScratchBufferSize.setVisible(False)

    gfx2d_regen_default = True

    gfx2dOutstandingRegulation = comp.createBooleanSymbol("DRV_GFX2D_REG_EN", None)
    gfx2dOutstandingRegulation.setLabel("Enable Outstanding Regulation")
    gfx2dOutstandingRegulation.setDescription("GFX2D delays transtactions if LCDC FIFO health is suboptimal.")
    gfx2dOutstandingRegulation.setDefaultValue(gfx2d_regen_default)
    gfx2dOutstandingRegulation.setVisible(True)

    gfx2dREGQOS1 = comp.createKeyValueSetSymbol("DRV_GFX2D_REG_QOS_1", None)
    gfx2dREGQOS1.setLabel("QOS1 Cycles")
    gfx2dREGQOS1.setOutputMode("Value")
    gfx2dREGQOS1.setDisplayMode("Description")
    gfx2dREGQOS1.setDescription("Cycles inserted to mitigate moderate LCDC FIFO health.")
    for x in range(11):
        id_val = "GFX2D_QOS_" + str(2**x - 1) + "_CYCLES"
        id_desc = str(2**x - 1)
        gfx2dREGQOS1.addKey(str(x), id_val, id_desc)
    gfx2dREGQOS1.setDefaultValue(4)
    gfx2dREGQOS1.setVisible(gfx2d_regen_default)
    gfx2dREGQOS1.setDependencies(show_qos_settings, ["DRV_GFX2D_REG_EN"])

    gfx2dREGQOS2 = comp.createKeyValueSetSymbol("DRV_GFX2D_REG_QOS_2", None)
    gfx2dREGQOS2.setLabel("QOS2 Cycles")
    gfx2dREGQOS2.setOutputMode("Value")
    gfx2dREGQOS2.setDisplayMode("Description")
    gfx2dREGQOS2.setDescription("Cycles inserted to mitigate low LCDC FIFO health.")
    for x in range(11):
        id_val = "GFX2D_QOS_" + str(2**x - 1) + "_CYCLES"
        id_desc = str(2**x - 1)
        gfx2dREGQOS2.addKey(str(x), id_val, id_desc)
    gfx2dREGQOS2.setDefaultValue(5)
    gfx2dREGQOS2.setVisible(gfx2d_regen_default)
    gfx2dREGQOS2.setDependencies(show_qos_settings, ["DRV_GFX2D_REG_EN"])

    gfx2dREGQOS3 = comp.createKeyValueSetSymbol("DRV_GFX2D_REG_QOS_3", None)
    gfx2dREGQOS3.setLabel("QOS3 Cycles")
    gfx2dREGQOS3.setOutputMode("Value")
    gfx2dREGQOS3.setDisplayMode("Description")
    gfx2dREGQOS3.setDescription("Cycles inserted to mitigate very low LCDC FIFO health.")
    for x in range(11):
        id_val = "GFX2D_QOS_" + str(2**x - 1) + "_CYCLES"
        id_desc = str(2**x - 1)
        gfx2dREGQOS3.addKey(str(x), id_val, id_desc)
    gfx2dREGQOS3.setDefaultValue(6)
    gfx2dREGQOS3.setVisible(gfx2d_regen_default)
    gfx2dREGQOS3.setDependencies(show_qos_settings, ["DRV_GFX2D_REG_EN"])

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
    if event["value"] == "Asynchronous":
        symbol.setVisible(True)
    else:
        symbol.setVisible(False)


def configureGFX2DComponent(gpuComponent, comp):
    print("GFX2D Driver: Connecting GFX2D")


def resetGFX2DComponent(gpuComponent, comp):
    print("GFX2D Driver: Disconnecting GFX2D")


def onAttachmentConnected(source, target):
    print("plib dependency Connected = " + str(target["id"]))
    #### test for GFX2D dependency


def onAttachmentDisconnected(source, target):
    print("plib dependency Disconnected = " + str(target["id"]))


def show_qos_settings(symbol, event):
    symbol.setVisible(event["value"])
