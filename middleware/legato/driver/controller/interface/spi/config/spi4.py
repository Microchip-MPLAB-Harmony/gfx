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

def instantiateComponent(comp):
	GFX_INTF_SPI_H = comp.createFileSymbol("GFX_INTF_SPI_H", None)
	GFX_INTF_SPI_H.setDestPath("gfx/interface/")
	GFX_INTF_SPI_H.setSourcePath("../drv_gfx_disp_intf.h")
	GFX_INTF_SPI_H.setOutputName("drv_gfx_disp_intf.h")
	GFX_INTF_SPI_H.setProjectPath("config/" + Variables.get("__CONFIGURATION_NAME") + "/gfx/interface")
	GFX_INTF_SPI_H.setType("HEADER")

	GFX_INTF_SPI = comp.createFileSymbol("GFX_INTF_SPI", None)
	GFX_INTF_SPI.setDestPath("gfx/interface/spi4/")
	GFX_INTF_SPI.setSourcePath("drv_gfx_disp_intf_spi4.c")
	GFX_INTF_SPI.setOutputName("drv_gfx_disp_intf_spi4.c")
	GFX_INTF_SPI.setProjectPath("config/" + Variables.get("__CONFIGURATION_NAME") + "/gfx/interface/spi4")
	GFX_INTF_SPI.setType("SOURCE")
	GFX_INTF_SPI.setMarkup(True)

	### Interface type is required for all interface components. This is queried by the driver to determine
	### the display interface supported by the interface component. Valid values are "SPI 4-line", "Parallel 16-bit",
	### and "Parallel 8-bit"
	InterfaceType = comp.createStringSymbol("InterfaceType", None)
	InterfaceType.setLabel("Interface Type")
	InterfaceType.setDescription("The interface configuration")
	InterfaceType.setDefaultValue("SPI 4-line")
	InterfaceType.setVisible(False)

        InterfaceSpiIndex = comp.createIntegerSymbol("DRV_INTERFACE_SPI_INDEX", None)
	InterfaceSpiIndex.setLabel("SPI Index")
	InterfaceSpiIndex.setDefaultValue(0)
	InterfaceSpiIndex.setMin(0)

	TransferModeSettingsMenu = comp.createMenuSymbol("TransferModeSettingsMenu", None)
	TransferModeSettingsMenu.setLabel("Transfer Mode Settings")
	
	BlockingTransfers = comp.createBooleanSymbol("BlockingTransfers", TransferModeSettingsMenu)
	BlockingTransfers.setLabel("Blocking")
	BlockingTransfers.setDefaultValue(True)
	
def onAttachmentConnected(source, target):
	#print(source["component"].getID() + ": " + dependencyID + " dependent component added ")
	if source["id"] == "DRV_SPI":
		spiIndex = target["component"].getSymbolByID("INDEX").getValue()
		print("Using SPI port " + str(target["component"].getID()))