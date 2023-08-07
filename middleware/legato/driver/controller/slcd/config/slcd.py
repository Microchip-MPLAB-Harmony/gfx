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
    projectPath = "config/" + Variables.get("__CONFIGURATION_NAME") + "/gfx/driver/slcd"

    # Activate dependency components
    Database.activateComponents(["HarmonyCore"])

    # Enable "Generate Harmony Driver Common Files" option in Core
    Database.sendMessage("HarmonyCore", "ENABLE_DRV_COMMON", {"isEnabled": True})

    # these two symbols are read by the HAL for initialization purposes
    # they must match the function names in the actual driver code
    DriverInitName = comp.createStringSymbol("DriverInitName", None)
    DriverInitName.setVisible(False)
    DriverInitName.setReadOnly(True)
    DriverInitName.setDefaultValue("slcdDisplayDriver")

    RunInStandby = comp.createBooleanSymbol("RunInStandby", None)
    RunInStandby.setLabel("Run In Standby")
    RunInStandby.setDescription("The standby run mode.")
    RunInStandby.setDefaultValue(False)
    RunInStandby.setVisible(False)

    WaveFormMode = comp.createComboSymbol("WaveFormMode", None, ["LOW_POWER", "STANDARD"])
    WaveFormMode.setLabel("WaveForm")
    WaveFormMode.setDefaultValue("SLCD_LOW_POWER_WAVEFORM_MODE")
    WaveFormMode.setVisible(True)

    # configuration options
    HALComment = comp.createCommentSymbol("HALComment", None)
    HALComment.setLabel("Some settings are being managed by the GFX Core LE and have been hidden.")
    HALComment.setVisible(True)

    EnableLowResistance = comp.createBooleanSymbol("EnableLowResistance", None)
    EnableLowResistance.setLabel("Enable Low Resistance")
    EnableLowResistance.setDescription("Enable Low Resistance.")
    EnableLowResistance.setDefaultValue(False)

    LowResistanceDuration = comp.createIntegerSymbol("LowResistanceDuration", None)
    LowResistanceDuration.setLabel("Low Resistance Duration")
    LowResistanceDuration.setDescription("Low Resistance Duration.")
    LowResistanceDuration.setDefaultValue(0)
    LowResistanceDuration.setMin(0)

    EnableBiasBuffer = comp.createBooleanSymbol("EnableBiasBuffer", None)
    EnableBiasBuffer.setLabel("Enable Bias Buffer")
    EnableBiasBuffer.setDescription("Enable Bias Buffer.")
    EnableBiasBuffer.setDefaultValue(False)

    BiasBufferDuration = comp.createIntegerSymbol("BiasBufferDuration", None)
    BiasBufferDuration.setLabel("Bias Buffer Duration")
    BiasBufferDuration.setDescription("Bias Buffer Duration")
    BiasBufferDuration.setDefaultValue(0)
    BiasBufferDuration.setMin(0)

    SLCD_ClockSource = comp.createComboSymbol("SLCD_ClockSource", None, ["OSCULP32K", "XOSC32K"])
    SLCD_ClockSource.setLabel("Clock Source")
    SLCD_ClockSource.setDefaultValue("OSCULP32K")
    SLCD_ClockSource.setDescription("Select SLCD clock. Use 32.768KHz OSCULP32K or XOSC32K for SLCD clock")
    SLCD_ClockSource.setVisible(True)

    SLCD_Duty = comp.createComboSymbol("SLCD_Duty", None, ["Static", "1/2", "1/3", "1/4", "1/6", "1/8"])
    SLCD_Duty.setLabel("SLCD Duty")
    SLCD_Duty.setDefaultValue("1/8")
    SLCD_Duty.setDescription("SLCD Duty")
    SLCD_Duty.setVisible(True)

    SLCD_Bias = comp.createComboSymbol("SLCD_Bias", None, ["Static", "1/2", "1/3", "1/4"])
    SLCD_Bias.setLabel("SLCD Bias")
    SLCD_Bias.setDefaultValue("1/4")
    SLCD_Bias.setDescription("SLCD Bias")
    SLCD_Bias.setVisible(True)

    ### SLCD Frame Frequency
    FrameFrequencyMenu = comp.createMenuSymbol("FrameFrequencyMenu", None)
    FrameFrequencyMenu.setLabel("Frame Frequency")

    FramePrescaler = comp.createComboSymbol("FramePrescaler", FrameFrequencyMenu, ["16", "32", "64", "128"])
    FramePrescaler.setLabel("PVAL")
    FramePrescaler.setDefaultValue("16")
    FramePrescaler.setDescription("SLCD Prescaler Value (PVAL)")
    FramePrescaler.setVisible(True)

    FrameDivider = comp.createIntegerSymbol("FrameDivider", FrameFrequencyMenu)
    FrameDivider.setLabel("CKDIV")
    FrameDivider.setDescription("SLCD Clock divider (CKDIV)")
    FrameDivider.setDefaultValue(2)
    FrameDivider.setMin(0)
    FrameDivider.setMax(7)

    VLCDSelection = comp.createComboSymbol("VLCDSelection", None, ["Internal", "External"])
    VLCDSelection.setLabel("VLCD Generation")
    VLCDSelection.setDefaultValue("Internal")
    VLCDSelection.setDescription("Internal/External VLCD selection")
    VLCDSelection.setVisible(True)

    BiasFreq = comp.createComboSymbol("BiasFreq", None, ["2KHz", "1KHz", "500Hz", "250Hz", "125Hz", "62.5Hz"])
    BiasFreq.setLabel("Bias Freq")
    BiasFreq.setDefaultValue("2KHz")
    BiasFreq.setDescription("Bias Reference refresh frequency")
    BiasFreq.setVisible(True)

    ChargeFreq = comp.createComboSymbol("ChargeFreq", None, ["2KHz", "1KHz", "500Hz", "250Hz"])
    ChargeFreq.setLabel("Charge Freq")
    ChargeFreq.setDefaultValue("2KHz")
    ChargeFreq.setDescription("Charge pump refresh frequency")
    ChargeFreq.setVisible(True)

    PowerMode = comp.createComboSymbol("PowerMode", None, ["Auto", "StepUp", "StepDown"])
    PowerMode.setLabel("Power Mode")
    PowerMode.setDefaultValue("Auto")
    PowerMode.setDescription("LCD Working Power Mode.")
    PowerMode.setVisible(True)

    PinMaskLow = comp.createStringSymbol("PinMaskLow", None)
    PinMaskLow.setLabel("Pin Mask Low")
    PinMaskLow.setDefaultValue("0xCFFFFFC0")
    PinMaskLow.setDescription("COM/SEG PIN Selection Low pin [31,0]")

    PinMaskHigh = comp.createStringSymbol("PinMaskHigh", None)
    PinMaskHigh.setLabel("Pin Mask High")
    PinMaskHigh.setDefaultValue("0x00000FF0")
    PinMaskHigh.setDescription("COM/SEG PIN Selection High [51,32]")

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

    execfile(Module.getPath() + "/config/slcd_rtos.py")

    # common gfx driver header
    GFX_DRIVER_H = comp.createFileSymbol("GFX_DRIVER_H", None)
    GFX_DRIVER_H.setSourcePath("../../templates/gfx_driver.h.ftl")
    GFX_DRIVER_H.setDestPath("gfx/driver/")
    GFX_DRIVER_H.setOutputName("gfx_driver.h")
    GFX_DRIVER_H.setProjectPath(projectPath)
    GFX_DRIVER_H.setType("HEADER")
    GFX_DRIVER_H.setMarkup(True)

    GFX_DRIVER_C = comp.createFileSymbol("GFX_DRIVER_C", None)
    GFX_DRIVER_C.setSourcePath("../../templates/gfx_driver.c.ftl")
    GFX_DRIVER_C.setDestPath("gfx/driver/")
    GFX_DRIVER_C.setOutputName("gfx_driver.c")
    GFX_DRIVER_C.setProjectPath(projectPath)
    GFX_DRIVER_C.setType("SOURCE")
    GFX_DRIVER_C.setMarkup(True)

    # generated code files
    GFX_LCDC_C = comp.createFileSymbol("GFX_SLCD_C", None)
    GFX_LCDC_C.setDestPath("gfx/driver/controller/slcd/")
    GFX_LCDC_C.setSourcePath("templates/drv_gfx_slcd.c.ftl")
    GFX_LCDC_C.setOutputName("drv_gfx_slcd.c")
    GFX_LCDC_C.setProjectPath(projectPath)
    GFX_LCDC_C.setType("SOURCE")
    GFX_LCDC_C.setMarkup(True)

    GFX_LCDC_H = comp.createFileSymbol("GFX_SLCD_H", None)
    GFX_LCDC_H.setSourcePath("templates/drv_gfx_slcd.h.ftl")
    GFX_LCDC_H.setDestPath("gfx/driver/controller/slcd/")
    GFX_LCDC_H.setOutputName("drv_gfx_slcd.h")
    GFX_LCDC_H.setProjectPath(projectPath)
    GFX_LCDC_H.setType("HEADER")
    GFX_LCDC_H.setMarkup(True)


def onAttachmentConnected(source, target):
    print("dependency Connected = " + target["component"].getDisplayName())


def showRTOSMenu(symbol, event):
    symbol.setVisible(event["value"] != "BareMetal")
