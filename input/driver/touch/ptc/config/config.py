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

DriverInitName = comp.createStringSymbol("DriverInitName", None)
DriverInitName.setLabel("Driver Name")
DriverInitName.setDefaultValue("PeripheralTouchController")
DriverInitName.setVisible(False)

TouchPanelSettingsMenu = comp.createMenuSymbol("TouchPanelSettingsMenu", None)
TouchPanelSettingsMenu.setLabel("Touch Panel Settings")

TouchWidth = comp.createIntegerSymbol("TouchWidth", TouchPanelSettingsMenu)
TouchWidth.setLabel("Width")
TouchWidth.setDescription("The width of the touch area in pixels.")
TouchWidth.setMin(1)
TouchWidth.setDefaultValue(480)

TouchWidthEdgeAdjust = comp.createIntegerSymbol("TouchWidthEdgeAdjust", TouchWidth)
TouchWidthEdgeAdjust.setLabel("Edge Adjustment")
TouchWidthEdgeAdjust.setDescription("Edge Adjustment.")
TouchWidthEdgeAdjust.setMin(0)
TouchWidthEdgeAdjust.setDefaultValue(0)

TouchHeight = comp.createIntegerSymbol("TouchHeight", TouchPanelSettingsMenu)
TouchHeight.setLabel("Height")
TouchHeight.setDescription("The height of the touch area in pixels.")
TouchHeight.setMin(1)
TouchHeight.setDefaultValue(320)

TouchHeightEdgeAdjust = comp.createIntegerSymbol("TouchHeightEdgeAdjust", TouchHeight)
TouchHeightEdgeAdjust.setLabel("Edge Adjustment")
TouchHeightEdgeAdjust.setDescription("Height Edge Adjustment")
TouchHeightEdgeAdjust.setMin(0)
TouchHeightEdgeAdjust.setDefaultValue(0)

TouchDataSettings = comp.createMenuSymbol("TouchDataSettings", None)
TouchDataSettings.setLabel("Touch Position Settings")
TouchDataSettings.setDescription("Touch Position Settings")

HorzTouchDataFlip = comp.createBooleanSymbol("HorzTouchDataFlip", TouchDataSettings)
HorzTouchDataFlip.setLabel("Flip Horizontal Position")
HorzTouchDataFlip.setDescription("Flip/Mirror the horizontal position data")

VertTouchDataFlip = comp.createBooleanSymbol("VertTouchDataFlip", TouchDataSettings)
VertTouchDataFlip.setLabel("Flip Vertical Position")
VertTouchDataFlip.setDescription("Flip/Mirror the vertical position data")

DataResolutionBits = comp.createComboSymbol("DataResolutionBits", TouchDataSettings, ["8", "10", "12"])
DataResolutionBits.setLabel("Touch Data Resolution (Bits)")
DataResolutionBits.setDescription("The resolution of the PTC touch data (bits). This should match the PTC library configuration.")

RTOSMenu = comp.createMenuSymbol("RTOSMenu", None)
RTOSMenu.setLabel("RTOS Settings")
RTOSMenu.setDescription("RTOS Settings")
RTOSMenu.setVisible(Database.getSymbolValue("HarmonyCore", "SELECT_RTOS") != "BareMetal")
RTOSMenu.setDependencies(showTouchRTOSMenu, ["HarmonyCore.SELECT_RTOS"])

RTOSEnabled = comp.createBooleanSymbol("RTOSEnabled", RTOSMenu)
RTOSEnabled.setLabel("RTOS Support Enabled")
RTOSEnabled.setDefaultValue(Database.getSymbolValue("HarmonyCore", "SELECT_RTOS") != "BareMetal")
RTOSEnabled.setDependencies(enableTouchRTOSymbol, ["HarmonyCore.SELECT_RTOS"])
RTOSEnabled.setVisible(False)

RTOSStackSize = comp.createIntegerSymbol("RTOSStackSize", RTOSMenu)
RTOSStackSize.setLabel("Stack Size")
RTOSStackSize.setMin(0)
RTOSStackSize.setDefaultValue(1024)

RTOSTaskPriority = comp.createIntegerSymbol("RTOSTaskPriority", RTOSMenu)
RTOSTaskPriority.setLabel("Task Priority")
RTOSTaskPriority.setDefaultValue(1)

RTOSUseDelay = comp.createBooleanSymbol("RTOSUseDelay", RTOSMenu)
RTOSUseDelay.setLabel("Use Task Delay?")
RTOSUseDelay.setDefaultValue(True)

RTOSTaskDelay = comp.createIntegerSymbol("RTOSTaskDelay", RTOSMenu)
RTOSTaskDelay.setLabel("Task Delay")
RTOSTaskDelay.setDefaultValue(10)
RTOSTaskDelay.setMin(0)






