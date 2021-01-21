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

componentsIDTable = ["HarmonyCore", "gfx_hal", "aria_gfx_library", "gfx_disp_atoled1-xpro_128x32", "gfx_driver_ssd1306",
					"drv_spi", "drv_spi_0", "gfx_intf_spi4"]
autoConnectTable = [["gfx_hal", "gfx_display", "gfx_disp_atoled1-xpro_128x32", "gfx_display"],
					["aria_gfx_library", "gfx_hal", "gfx_hal", "gfx_hal"],
					["gfx_hal", "gfx_display_driver", "gfx_driver_ssd1306", "gfx_driver_ssd1306"],
					["gfx_driver_ssd1306", "Display Interface", "gfx_intf_spi4", "gfx_intf_spi4"],
					["gfx_intf_spi4", "DRV_SPI", "drv_spi_0", "drv_spi"]]
deactivateIDTable = ["FreeRTOS"]

execfile(Module.getPath() + "../common/pin_config.py")
execfile(Module.getPath() + "../common/bsp_utils.py")

#Add BSP support
execfile(Module.getPath() + "Support_BSP_SAM_E70_OLED1_Xpro.py")
execfile(Module.getPath() + "Support_BSP_SAM_C21_OLED1_Xpro.py")
execfile(Module.getPath() + "Support_BSP_SAM_E54_OLED1_XPro.py")

def enableSPIPins(bspID, enable):
	SPI4PinConfigs = getBSPSupportNode(bspID, "SPI 4-line").getPinConfig()
	if (SPI4PinConfigs != None):
		resetPins(SPI4PinConfigs)
		if (enable == True):
			configurePins(SPI4PinConfigs)

def enableSPIInterface(bspID, enable):
	componentIDTable = getBSPSupportNode(bspID, "SPI 4-line").getComponentActivateList()
	autoConnectTable = getBSPSupportNode(bspID, "SPI 4-line").getComponentAutoConnectList()
	if (enable == True):
		if (componentIDTable != None):
			res = Database.activateComponents(componentIDTable)
		if (autoConnectTable != None):
			res = Database.connectDependencies(autoConnectTable)
	elif (enable == False):
		if (componentIDTable != None):
			res = Database.deactivateComponents(componentIDTable)
	enableSPIPins(bspID, enable)
	if (getBSPSupportNode(bspID, "SPI 4-line").getEventCallbackFxn() != None):
		getBSPSupportNode(bspID, "SPI 4-line").getEventCallbackFxn()("configure")
	

def configureDisplayInterface(bspID):
	if (bspID == None):
		print("No BSP used, will not configure")
	else:
		enableSPIInterface(bspID, True)

def instantiateComponent(bspComponent):
	global componentsIDTable
	global autoConnectTable
	global supportedBSPsIDList
	
	#Check if a supported BSP is loaded
	bspID = getSupportedBSP()

	res = Database.activateComponents(componentsIDTable)
	res = Database.connectDependencies(autoConnectTable)
	res = Database.deactivateComponents(deactivateIDTable)
	
	#Configure driver
	Database.setSymbolValue("gfx_driver_ssd1306", "DisplayStartLine", 32, 1)
	
	if (bspID != None):
		configureDisplayInterface(bspID)
	else:
		print("No BSP used, only software components are configured. Please add board-specific components.")

