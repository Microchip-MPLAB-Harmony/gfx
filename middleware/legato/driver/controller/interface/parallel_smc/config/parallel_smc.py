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
	GFX_INTF_SMC_H = comp.createFileSymbol("GFX_INTF_SMC_H", None)
	GFX_INTF_SMC_H.setDestPath("gfx/interface/")
	GFX_INTF_SMC_H.setSourcePath("../drv_gfx_disp_intf.h")
	GFX_INTF_SMC_H.setOutputName("drv_gfx_disp_intf.h")
	GFX_INTF_SMC_H.setProjectPath("config/" + Variables.get("__CONFIGURATION_NAME") + "/gfx/interface")
	GFX_INTF_SMC_H.setType("HEADER")

	GFX_INTF_SMC = comp.createFileSymbol("GFX_INTF_SMC", None)
	GFX_INTF_SMC.setDestPath("gfx/interface/parallel_smc/")
	GFX_INTF_SMC.setSourcePath("templates/drv_gfx_disp_intf_parallel_smc.c.ftl")
	GFX_INTF_SMC.setOutputName("drv_gfx_disp_intf_parallel_smc.c")
	GFX_INTF_SMC.setProjectPath("config/" + Variables.get("__CONFIGURATION_NAME") + "/gfx/interface/parallel_smc")
	GFX_INTF_SMC.setMarkup(True)
	GFX_INTF_SMC.setType("SOURCE")
	
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
	DelayNOPCount.setDefaultValue(4)
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
	EBIChipSelectIndex.setVisible(False)

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
	DCXAddressBit.setDefaultValue(12)
	DCXAddressBit.setMin(0)
	DCXAddressBit.setMax(31)
	###
	
def configureSMCComponent(comp, smcComponent):
	print("Configuring SMC")
	smcChipSelNum = comp.getSymbolValue("EBIChipSelectIndex")
	smcComponent.setSymbolValue("SMC_CHIP_SELECT" + str(smcChipSelNum), True, 1)
	smcComponent.setSymbolValue("SMC_MEM_SCRAMBLING_CS" + str(smcChipSelNum), False, 1)
	# SMC Write Timings
	smcComponent.setSymbolValue("SMC_NWE_SETUP_CS" + str(smcChipSelNum), 4, 1)
	smcComponent.setSymbolValue("SMC_NCS_WR_SETUP_CS" + str(smcChipSelNum), 0, 1)
	smcComponent.setSymbolValue("SMC_NWE_PULSE_CS" + str(smcChipSelNum), 4, 1)
	smcComponent.setSymbolValue("SMC_NCS_WR_PULSE_CS" + str(smcChipSelNum), 10, 1)
	smcComponent.setSymbolValue("SMC_DATA_BUS_CS" + str(smcChipSelNum), 1, 1)
	smcComponent.setSymbolValue("SMC_NWE_CYCLE_CS" + str(smcChipSelNum), 3, 1)
	# SMC Read Timings
	smcComponent.setSymbolValue("SMC_NRD_SETUP_CS" + str(smcChipSelNum), 2, 1)
	smcComponent.setSymbolValue("SMC_NCS_RD_SETUP_CS" + str(smcChipSelNum), 0, 1)
	smcComponent.setSymbolValue("SMC_NRD_PULSE_CS" + str(smcChipSelNum), 63, 1)
	smcComponent.setSymbolValue("SMC_NCS_RD_PULSE_CS" + str(smcChipSelNum), 63, 1)
	smcComponent.setSymbolValue("SMC_NRD_CYCLE_CS" + str(smcChipSelNum), 110, 1)
	# SMC Mode Configuration
	smcComponent.setSymbolValue("SMC_DATA_BUS_CS" + str(smcChipSelNum), 1, 1)
	smcComponent.setSymbolValue("SMC_BAT_CS" + str(smcChipSelNum), 0, 1)
	smcComponent.setSymbolValue("SMC_READ_ENABLE_MODE_CS" + str(smcChipSelNum), True, 1)
	smcComponent.setSymbolValue("SMC_WRITE_ENABLE_MODE_CS" + str(smcChipSelNum), False, 1)

	
def onDataCommandSelectSet(sourceSymbol, event):
	if (sourceSymbol.getComponent().getSymbolByID("DataCommandSelectControl").getValue() == "GPIO"):
		sourceSymbol.getComponent().getSymbolByID("DCXAddressBit").setVisible(False)
	else:
		sourceSymbol.getComponent().getSymbolByID("DCXAddressBit").setVisible(True)

def onInterfaceTypeChanged(sourceSymbol, event):
	print("Interface type changed")
	
def onAttachmentConnected(source, target):
	#print(source["component"].getID() + ": " + dependencyID + " dependent component added ")
	if source["id"] == "SMC_CS":
		configureSMCComponent(source["component"], target["component"])