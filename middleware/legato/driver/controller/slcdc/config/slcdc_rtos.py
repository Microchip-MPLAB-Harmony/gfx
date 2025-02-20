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

rtosMenu = comp.createMenuSymbol("rtosMenu", None)
rtosMenu.setLabel("RTOS Settings")
rtosMenu.setDescription("RTOS Settings")
rtosMenu.setVisible(Database.getSymbolValue("HarmonyCore", "SELECT_RTOS") != "BareMetal")
rtosMenu.setDependencies(showRTOSMenu, ["HarmonyCore.SELECT_RTOS"])

rtosTaskSize = comp.createIntegerSymbol("rtosTaskSize", rtosMenu)
rtosTaskSize.setLabel("Stack Size")
rtosTaskSize.setDefaultValue(2048)

rtosTaskPriority = comp.createIntegerSymbol("rtosTaskPriority", rtosMenu)
rtosTaskPriority.setLabel("Task Priority")
rtosTaskPriority.setDefaultValue(1)

rtosEnableTaskDelay = comp.createBooleanSymbol("rtosEnableTaskDelay", rtosMenu)
rtosEnableTaskDelay.setLabel("Use Task Delay?")
rtosEnableTaskDelay.setDefaultValue(True)

rtosTaskDelay = comp.createIntegerSymbol("rtosTaskDelay", rtosMenu)
rtosTaskDelay.setLabel("Task Delay")
rtosTaskDelay.setDefaultValue(10)