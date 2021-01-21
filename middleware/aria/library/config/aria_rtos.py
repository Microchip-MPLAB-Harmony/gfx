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

AriaRTOSMenu = component.createMenuSymbol("AriaRTOSMenu", None)
AriaRTOSMenu.setLabel("RTOS Settings")
AriaRTOSMenu.setDescription("RTOS Settings")
AriaRTOSMenu.setVisible(Database.getSymbolValue("HarmonyCore", "SELECT_RTOS") != "BareMetal")
AriaRTOSMenu.setDependencies(showAriaRTOSMenu, ["HarmonyCore.SELECT_RTOS"])

useRTOSExtensions = component.createBooleanSymbol("useRTOSExtensions", AriaRTOSMenu)
useRTOSExtensions.setLabel("Use RTOS Extensions?")
useRTOSExtensions.setDefaultValue(Database.getSymbolValue("HarmonyCore", "SELECT_RTOS") != "BareMetal")
useRTOSExtensions.setDependencies(enableAriaRTOSExtensions, ["useRTOSExtensions"])
useRTOSExtensions.setHelp("IDH_HTML_MHGC_UG_Enabling_RTOS")

rtosFullBlockingMode = component.createBooleanSymbol("rtosFullBlockingMode", useRTOSExtensions)
rtosFullBlockingMode.setLabel("Full Blocking?")
rtosFullBlockingMode.setDefaultValue(True)
rtosFullBlockingMode.setVisible(True)

rtosTaskSize = component.createIntegerSymbol("rtosTaskSize", AriaRTOSMenu)
rtosTaskSize.setLabel("Stack Size")
rtosTaskSize.setDefaultValue(1024)

rtosTaskPriority = component.createIntegerSymbol("rtosTaskPriority", AriaRTOSMenu)
rtosTaskPriority.setLabel("Task Priority")
rtosTaskPriority.setDefaultValue(1)

rtosEnableTaskDelay = component.createBooleanSymbol("rtosEnableTaskDelay", AriaRTOSMenu)
rtosEnableTaskDelay.setLabel("Use Task Delay?")
rtosEnableTaskDelay.setDefaultValue(False)

rtosTaskDelay = component.createIntegerSymbol("rtosTaskDelay", AriaRTOSMenu)
rtosTaskDelay.setLabel("Task Delay")
rtosTaskDelay.setDefaultValue(10)

### Aria RTOS extension files
LIBARIA_RTOS_H = component.createFileSymbol("LIBARIA_RTOS_H", None)
LIBARIA_RTOS_H.setSourcePath("inc/libaria_rtos.h")
LIBARIA_RTOS_H.setDestPath("gfx/libaria/")
LIBARIA_RTOS_H.setOutputName("libaria_rtos.h")
LIBARIA_RTOS_H.setProjectPath(projectPath)
LIBARIA_RTOS_H.setType("HEADER")
LIBARIA_RTOS_H.setEnabled(useRTOSExtensions.getValue() == True)
#LIBARIA_RTOS_H.setDependencies(enableAriaRTOSSymbol, ["useRTOSExtensions"])

LIBARIA_CONTEXT_RTOS_H = component.createFileSymbol("LIBARIA_CONTEXT_RTOS_H", None)
LIBARIA_CONTEXT_RTOS_H.setSourcePath("inc/libaria_context_rtos.h")
LIBARIA_CONTEXT_RTOS_H.setDestPath("gfx/libaria/inc/")
LIBARIA_CONTEXT_RTOS_H.setOutputName("libaria_context_rtos.h")
LIBARIA_CONTEXT_RTOS_H.setProjectPath(projectPath)
LIBARIA_CONTEXT_RTOS_H.setType("HEADER")
LIBARIA_CONTEXT_RTOS_H.setEnabled(useRTOSExtensions.getValue() == True)
#LIBARIA_CONTEXT_RTOS_H.setDependencies(enableAriaRTOSSymbol, ["useRTOSExtensions"])

LIBARIA_EVENT_RTOS_H = component.createFileSymbol("LIBARIA_EVENT_RTOS_H", None)
LIBARIA_EVENT_RTOS_H.setSourcePath("inc/libaria_event_rtos.h")
LIBARIA_EVENT_RTOS_H.setDestPath("gfx/libaria/inc/")
LIBARIA_EVENT_RTOS_H.setOutputName("libaria_event_rtos.h")
LIBARIA_EVENT_RTOS_H.setProjectPath(projectPath)
LIBARIA_EVENT_RTOS_H.setType("HEADER")
LIBARIA_EVENT_RTOS_H.setEnabled(useRTOSExtensions.getValue() == True)
#LIBARIA_EVENT_RTOS_H.setDependencies(enableAriaRTOSSymbol, ["useRTOSExtensions"])

LIBARIA_INPUT_RTOS_H = component.createFileSymbol("LIBARIA_INPUT_RTOS_H", None)
LIBARIA_INPUT_RTOS_H.setSourcePath("inc/libaria_input_rtos.h")
LIBARIA_INPUT_RTOS_H.setDestPath("gfx/libaria/inc/")
LIBARIA_INPUT_RTOS_H.setOutputName("libaria_input_rtos.h")
LIBARIA_INPUT_RTOS_H.setProjectPath(projectPath)
LIBARIA_INPUT_RTOS_H.setType("HEADER")
LIBARIA_INPUT_RTOS_H.setEnabled(useRTOSExtensions.getValue() == True)
#LIBARIA_INPUT_RTOS_H.setDependencies(enableAriaRTOSSymbol, ["useRTOSExtensions"])

LIBARIA_RTOS_C = component.createFileSymbol("LIBARIA_RTOS_C", None)
LIBARIA_RTOS_C.setSourcePath("src/libaria_rtos.c")
LIBARIA_RTOS_C.setDestPath("gfx/libaria")
LIBARIA_RTOS_C.setOutputName("libaria_rtos.c")
LIBARIA_RTOS_C.setProjectPath(projectPath)
LIBARIA_RTOS_C.setType("SOURCE")
LIBARIA_RTOS_C.setEnabled(useRTOSExtensions.getValue() == True)
#LIBARIA_RTOS_C.setDependencies(enableAriaRTOSSymbol, ["useRTOSExtensions"])

LIBARIA_CONTEXT_RTOS_C = component.createFileSymbol("LIBARIA_CONTEXT_RTOS_C", None)
LIBARIA_CONTEXT_RTOS_C.setSourcePath("src/libaria_context_rtos.c")
LIBARIA_CONTEXT_RTOS_C.setDestPath("gfx/libaria")
LIBARIA_CONTEXT_RTOS_C.setOutputName("libaria_context_rtos.c")
LIBARIA_CONTEXT_RTOS_C.setProjectPath(projectPath)
LIBARIA_CONTEXT_RTOS_C.setType("SOURCE")
LIBARIA_CONTEXT_RTOS_C.setEnabled(useRTOSExtensions.getValue() == True)
#LIBARIA_CONTEXT_RTOS_C.setDependencies(enableAriaRTOSSymbol, ["useRTOSExtensions"])

LIBARIA_EVENT_RTOS_C = component.createFileSymbol("LIBARIA_EVENT_RTOS_C", None)
LIBARIA_EVENT_RTOS_C.setSourcePath("src/libaria_event_rtos.c")
LIBARIA_EVENT_RTOS_C.setDestPath("gfx/libaria")
LIBARIA_EVENT_RTOS_C.setOutputName("libaria_event_rtos.c")
LIBARIA_EVENT_RTOS_C.setProjectPath(projectPath)
LIBARIA_EVENT_RTOS_C.setType("SOURCE")
LIBARIA_EVENT_RTOS_C.setEnabled(useRTOSExtensions.getValue() == True)
#LIBARIA_EVENT_RTOS_C.setDependencies(enableAriaRTOSSymbol, ["useRTOSExtensions"])

LIBARIA_INPUT_RTOS_C = component.createFileSymbol("LIBARIA_INPUT_RTOS_C", None)
LIBARIA_INPUT_RTOS_C.setSourcePath("src/libaria_input_rtos.c")
LIBARIA_INPUT_RTOS_C.setDestPath("gfx/libaria")
LIBARIA_INPUT_RTOS_C.setOutputName("libaria_input_rtos.c")
LIBARIA_INPUT_RTOS_C.setProjectPath(projectPath)
LIBARIA_INPUT_RTOS_C.setType("SOURCE")
LIBARIA_INPUT_RTOS_C.setEnabled(useRTOSExtensions.getValue() == True)
#LIBARIA_INPUT_RTOS_C.setDependencies(enableAriaRTOSSymbol, ["useRTOSExtensions"])