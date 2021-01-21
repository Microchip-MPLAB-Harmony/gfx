# coding: utf-8
##############################################################################
# Copyright (C) 2018 Microchip Technology Inc. and its subsidiaries.
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

enableDemoMode = component.createBooleanSymbol("enableDemoMode", None)
enableDemoMode.setLabel("Enable Demo mode?")
enableDemoMode.setDescription("Indicates that Aria should generate the code needed to run in 'demo mode'.")
enableDemoMode.setDependencies(onDemoModeEnable, ["enableDemoMode"])
enableDemoMode.setHelp("IDH_HTML_QSG__Enabling_Demo_Mode_in_a_MPLAB_Harmony_Graphics_Application")

#demoModeMenu = component.createMenuSymbol("demoModeMenu", None)
#demoModeMenu.setLabel("Demo Mode Configuration")
#demoModeMenu.setVisible(False)
#demoModeMenu.setDependencies(onDemoModeEnable, ["enableDemoMode"])

demoModeRecordInput = component.createBooleanSymbol("demoModeRecordInput", enableDemoMode)
demoModeRecordInput.setLabel("Record Input Events?")
demoModeRecordInput.setVisible(False)

demoModeRecordTickPeriod = component.createIntegerSymbol("demoModeRecordTickPeriod", enableDemoMode)
demoModeRecordTickPeriod.setLabel("Record Tick Period (ms)")
demoModeRecordTickPeriod.setDefaultValue(10)
demoModeRecordTickPeriod.setVisible(False)

demoModeMaxEvents = component.createIntegerSymbol("demoModeMaxEvents", enableDemoMode)
demoModeMaxEvents.setLabel("Maximum Number of Events")
demoModeMaxEvents.setDefaultValue(500)
demoModeMaxEvents.setVisible(False)

demoModeIdleTimeout = component.createIntegerSymbol("demoModeIdleTimeout", enableDemoMode)
demoModeIdleTimeout.setLabel("Idle Timeout (seconds)")
demoModeIdleTimeout.setDefaultValue(20)
demoModeIdleTimeout.setVisible(False)

demoModeReplayDelay = component.createIntegerSymbol("demoModeReplayDelay", enableDemoMode)
demoModeReplayDelay.setLabel("Replay Delay (seconds)")
demoModeReplayDelay.setDefaultValue(5)
demoModeReplayDelay.setVisible(False)

LIBARIA_DEMO_MODE_H = component.createFileSymbol("LIBARIA_DEMO_MODE_H", None)
LIBARIA_DEMO_MODE_H.setSourcePath("templates/libaria_demo_mode.h.ftl")
LIBARIA_DEMO_MODE_H.setDestPath("gfx/libaria/")
LIBARIA_DEMO_MODE_H.setOutputName("libaria_demo_mode.h")
LIBARIA_DEMO_MODE_H.setProjectPath(projectPath)
LIBARIA_DEMO_MODE_H.setType("HEADER")
LIBARIA_DEMO_MODE_H.setMarkup(True)
LIBARIA_DEMO_MODE_H.setEnabled(False)

LIBARIA_DEMO_MODE_C = component.createFileSymbol("LIBARIA_DEMO_MODE_C", None)
LIBARIA_DEMO_MODE_C.setSourcePath("templates/libaria_demo_mode.c.ftl")
LIBARIA_DEMO_MODE_C.setDestPath("gfx/libaria/")
LIBARIA_DEMO_MODE_C.setOutputName("libaria_demo_mode.c")
LIBARIA_DEMO_MODE_C.setProjectPath(projectPath)
LIBARIA_DEMO_MODE_C.setType("SOURCE")
LIBARIA_DEMO_MODE_C.setMarkup(True)
LIBARIA_DEMO_MODE_C.setEnabled(False)