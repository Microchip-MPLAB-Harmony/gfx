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
import re

def instantiateComponent(comp):
	GFX_INTF_EBI_H = comp.createFileSymbol("GFX_INTF_EBI_H", None)
	GFX_INTF_EBI_H.setDestPath("gfx/interface/")
	GFX_INTF_EBI_H.setSourcePath("../drv_gfx_disp_intf.h")
	GFX_INTF_EBI_H.setOutputName("drv_gfx_disp_intf.h")
	GFX_INTF_EBI_H.setProjectPath("config/" + Variables.get("__CONFIGURATION_NAME") + "/gfx/interface")
	GFX_INTF_EBI_H.setType("HEADER")

	GFX_INTF_EBI = comp.createFileSymbol("GFX_INTF_SMC", None)
	GFX_INTF_EBI.setDestPath("gfx/interface/parallel_ebi/")
	GFX_INTF_EBI.setSourcePath("templates/drv_gfx_disp_intf_parallel_ebi.c.ftl")
	GFX_INTF_EBI.setOutputName("drv_gfx_disp_intf_parallel_ebi.c")
	GFX_INTF_EBI.setProjectPath("config/" + Variables.get("__CONFIGURATION_NAME") + "/gfx/interface/parallel_ebi")
	GFX_INTF_EBI.setMarkup(True)
	GFX_INTF_EBI.setType("SOURCE")
	
	### Interface type is required for all interface components. This is queried by the driver to determine
	### the display interface supported by the interface component. Valid values are "SPI 4-line", "Parallel 16-bit",
	### and "Parallel 8-bit"
	InterfaceType = comp.createStringSymbol("InterfaceType", None)
	InterfaceType.setLabel("Interface Type")
	InterfaceType.setDescription("The interface configuration")
	InterfaceType.setDefaultValue("Parallel 16-bit")
	InterfaceType.setVisible(False)

	### Driver settings menu
	DriverSettingsMenu = comp.createMenuSymbol("DriverSettingsMenu", None)
	DriverSettingsMenu.setLabel("Driver Settings")

	UseSyncBarriers = comp.createBooleanSymbol("UseSyncBarriers", DriverSettingsMenu)
	UseSyncBarriers.setLabel("Use Synchronization Barriers")
	UseSyncBarriers.setDescription("Use Synchronization Barriers.")
	UseSyncBarriers.setDefaultValue(True)
	UseSyncBarriers.setVisible(False)
	
	DelayNOPCount = comp.createIntegerSymbol("DelayNOPCount", DriverSettingsMenu)
	DelayNOPCount.setLabel("Number of NOP for delay")
	DelayNOPCount.setDescription("Number of NOP for delay")
	DelayNOPCount.setDefaultValue(12)
	###
	
	### Interface settings menu
	InterfaceSettingsSMCMenu = comp.createMenuSymbol("InterfaceSettingsSMCMenu", None)
	InterfaceSettingsSMCMenu.setLabel("Parallel 8080 Display Interface Settings")
	
	EBIChipSelectIndex = comp.createIntegerSymbol("EBIChipSelectIndex", InterfaceSettingsSMCMenu)
	EBIChipSelectIndex.setLabel("EBI Chip Select Index")
	EBIChipSelectIndex.setDescription("The chip select index")
	EBIChipSelectIndex.setDefaultValue(0)
	EBIChipSelectIndex.setMin(0)
	EBIChipSelectIndex.setMax(4)
	#EBIChipSelectIndex.setVisible(False)

	ControlPinsMenu = comp.createMenuSymbol("ControlPinsMenu", InterfaceSettingsSMCMenu)
	ControlPinsMenu.setLabel("Control Pin Settings")

	ChipSelectControl = comp.createComboSymbol("ChipSelectControl", ControlPinsMenu, ["GPIO", "Peripheral"])
	ChipSelectControl.setLabel("CS# Control")
	ChipSelectControl.setDescription("Chip Select Control")
	ChipSelectControl.setDefaultValue("GPIO")
	ChipSelectControl.setReadOnly(True)

	DataCommandSelectControl = comp.createComboSymbol("DataCommandSelectControl", ControlPinsMenu, ["GPIO", "Peripheral"])
	DataCommandSelectControl.setLabel("D/C# Control")
	DataCommandSelectControl.setDescription("Data Command Select Control")
	DataCommandSelectControl.setDefaultValue("GPIO")
	DataCommandSelectControl.setDependencies(onDataCommandSelectSet, ["DataCommandSelectControl"])

	ReadStrobeControl = comp.createComboSymbol("ReadStrobeControl", ControlPinsMenu, ["GPIO", "Peripheral"])
	ReadStrobeControl.setLabel("RD# Control")
	ReadStrobeControl.setDescription("Read Strobe Control")
	ReadStrobeControl.setDefaultValue("GPIO")

	WriteStrobeControl = comp.createComboSymbol("WriteStrobeControl", ControlPinsMenu, ["GPIO", "Peripheral"])
	WriteStrobeControl.setLabel("WR# Control")
	WriteStrobeControl.setDescription("Write Strobe Control")
	WriteStrobeControl.setDefaultValue("GPIO")

	DCXAddressBit = comp.createIntegerSymbol("DCXAddressBit", DataCommandSelectControl)
	DCXAddressBit.setLabel("DCX Address Bit")
	DCXAddressBit.setDescription("Address bit used for DCX signal.")
	DCXAddressBit.setDefaultValue(1)
	DCXAddressBit.setMin(0)
	DCXAddressBit.setMax(31)
	DCXAddressBit.setVisible(False)
	###
	
def configureEBIComponent(component, ebiComponent, ebiChipSelNum):
	print("Parallel_ebi: Connecting EBI_CS" + str(ebiChipSelNum))
	ebiComponent.setSymbolValue("CFGEBIC_EBIDAT", 2, 1) #2 = 16-bit
	ebiComponent.setSymbolValue("CFGEBIC_EBICSEN" + str(ebiChipSelNum), True, 1)
	ebiComponent.setSymbolValue("EBIMSK" + str(ebiChipSelNum) + "_MEMSIZE", 8, 1) #8 = 8MB
	ebiComponent.setSymbolValue("EBISMT" + str(ebiChipSelNum) + "_TRC", 0, 1)
	ebiComponent.setSymbolValue("EBISMT" + str(ebiChipSelNum) + "_TAS", 3, 1)
	ebiComponent.setSymbolValue("EBISMT" + str(ebiChipSelNum) + "_TWR", 3, 1)
	ebiComponent.setSymbolValue("EBISMT" + str(ebiChipSelNum) + "_TWP", 2, 1)
	ebiComponent.setSymbolValue("EBISMT" + str(ebiChipSelNum) + "_TBTA", 0, 1)
	ebiComponent.setSymbolValue("EBISMCON_SMDWIDTH" + str(ebiChipSelNum), 0, 1)
	if (component.getSymbolByID("WriteStrobeControl").getValue() == "GPIO"):
		print("Clearing EBIWEEN")
		ebiComponent.clearSymbolValue("CFGEBIC_EBIWEEN")
	else:
		print("Setting EBIWEEN")
		ebiComponent.setSymbolValue("CFGEBIC_EBIWEEN", True, 1)
	component.setSymbolValue("EBIChipSelectIndex", ebiChipSelNum, 1)

def resetEBIComponent(component, ebiComponent, ebiChipSelNum):
	print("Parallel_ebi: Disconnecting EBI_CS" + str(ebiChipSelNum))
	ebiComponent.clearSymbolValue("CFGEBIC_EBIDAT")
	ebiComponent.clearSymbolValue("CFGEBIC_EBICSEN" + str(ebiChipSelNum))
	ebiComponent.clearSymbolValue("EBIMSK" + str(ebiChipSelNum) + "_MEMSIZE")
	ebiComponent.clearSymbolValue("EBISMT" + str(ebiChipSelNum) + "_TRC")
	ebiComponent.clearSymbolValue("EBISMT" + str(ebiChipSelNum) + "_TAS")
	ebiComponent.clearSymbolValue("EBISMT" + str(ebiChipSelNum) + "_TWR")
	ebiComponent.clearSymbolValue("EBISMT" + str(ebiChipSelNum) + "_TWP")
	ebiComponent.clearSymbolValue("EBISMT" + str(ebiChipSelNum) + "_TBTA")
	ebiComponent.clearSymbolValue("EBISMCON_SMDWIDTH" + str(ebiChipSelNum))
	ebiComponent.clearSymbolValue("CFGEBIC_EBIWEEN")
	component.clearSymbolValue("EBIChipSelectIndex")

def onDataCommandSelectSet(sourceSymbol, event):
	if (sourceSymbol.getComponent().getSymbolByID("WriteStrobeControl").getValue() == "GPIO"):
		sourceSymbol.getComponent().getSymbolByID("DCXAddressBit").setVisible(True)
	else:
		sourceSymbol.getComponent().getSymbolByID("DCXAddressBit").setVisible(False)

def onInterfaceTypeChanged(sourceSymbol, event):
	print("Interface type changed")
	
def onAttachmentConnected(source, target):
	print("dependency Connected = " + str(target['id']))
	#### test for EBI dependency
	if (source["id"] == "EBI_CS"):
		sub = re.search('ebi_cs(.*)', str(target["id"]))
		if (sub and sub.group(1)):
			configureEBIComponent(source["component"], target["component"], int(sub.group(1)))

def onAttachmentDisconnected(source, target):
	if (source["id"] == "EBI_CS"):
		sub = re.search('ebi_cs(.*)', str(target["id"]))
		if (sub and sub.group(1)):
			resetEBIComponent(source["component"], target["component"], int(sub.group(1)))