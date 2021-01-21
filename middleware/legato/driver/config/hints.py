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

DriverHintMenu = halComponent.createMenuSymbol("DriverHintMenu", None)
DriverHintMenu.setLabel("Driver Configuration Hints")
DriverHintMenu.setDescription("Contains compile-time configuration hints for various graphics subsystems.")

DisableLCCRefreshHint = halComponent.createBooleanSymbol("DisableLCCRefreshHint", DriverHintMenu)
DisableLCCRefreshHint.setDefaultValue(False)
DisableLCCRefreshHint.setVisible(False)
DisableLCCRefreshHint.setDependencies(onDisableHint, ["DisableLCCRefreshHint"])

HardwareLayerCountHint = halComponent.createIntegerSymbol("HardwareLayerCountHint", DriverHintMenu)
HardwareLayerCountHint.setLabel("Hardware Layer Count")
HardwareLayerCountHint.setDefaultValue(1)
HardwareLayerCountHint.setDescription("Indicates the number of hardware layers provided by the display driver.")
HardwareLayerCountHint.setReadOnly(True)
HardwareLayerCountHint.setUseSingleDynamicValue(True)

DisableHardwareLayerCountHint = halComponent.createBooleanSymbol("DisableHardwareLayerCountHint", DriverHintMenu)
DisableHardwareLayerCountHint.setDefaultValue(False)
DisableHardwareLayerCountHint.setVisible(False)
DisableHardwareLayerCountHint.setDependencies(onDisableHint, ["DisableHardwareLayerCountHint"])

PixelClockHint = halComponent.createIntegerSymbol("PixelClockHint", DriverHintMenu)
PixelClockHint.setLabel("Pixel Clock (Hz)")
PixelClockHint.setDefaultValue(50000000)
PixelClockHint.setDescription("Indicates pixel clock frequency.")
PixelClockHint.setReadOnly(True)
PixelClockHint.setUseSingleDynamicValue(True)

DisablePixelClockHint = halComponent.createBooleanSymbol("DisablePixelClockHint", DriverHintMenu)
DisablePixelClockHint.setDefaultValue(False)
DisablePixelClockHint.setVisible(False)
DisablePixelClockHint.setDependencies(onDisableHint, ["DisablePixelClockHint"])
