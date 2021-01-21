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
	GFX_INTF_H = comp.createFileSymbol("GFX_INTF_H", None)
	GFX_INTF_H.setDestPath("gfx/interface/")
	GFX_INTF_H.setSourcePath("../drv_gfx_disp_intf.h")
	GFX_INTF_H.setOutputName("drv_gfx_disp_intf.h")
	GFX_INTF_H.setProjectPath("config/" + Variables.get("__CONFIGURATION_NAME") + "/gfx/interface")
	GFX_INTF_H.setType("HEADER")

	GFX_INTF_SMC = comp.createFileSymbol("GFX_INTF_SMC", None)
	GFX_INTF_SMC.setDestPath("gfx/interface/parallel_smc/")
	GFX_INTF_SMC.setSourcePath("templates/drv_gfx_disp_intf_parallel_portgroup.c.ftl")
	GFX_INTF_SMC.setOutputName("drv_gfx_disp_intf_parallel_portgroup.c")
	GFX_INTF_SMC.setProjectPath("config/" + Variables.get("__CONFIGURATION_NAME") + "/gfx/interface/parallel_portgroup")
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
	InterfaceSettingsMenu = comp.createMenuSymbol("InterfaceSettingsMenu", None)
	InterfaceSettingsMenu.setLabel("Parallel 8080 Display Interface Settings")
	
	PortGroupNum = comp.createComboSymbol("PortGroup", InterfaceSettingsMenu, ["0", "1", "2", "3"])
	PortGroupNum.setLabel("IO Group")
	PortGroupNum.setDescription("The port group")
	PortGroupNum.setDefaultValue("1")

	PortGroupHalf = comp.createComboSymbol("PortGroupHalf", PortGroupNum, ["0", "1"])
	PortGroupHalf.setLabel("Half")
	PortGroupHalf.setDescription("Select the upper or lower half of port group for 16-bit accesses")
	PortGroupHalf.setDefaultValue("0")

	PortGroupQuadrant = comp.createComboSymbol("PortGroupQuadrant", PortGroupNum, ["0", "1"])
	PortGroupQuadrant.setLabel("Quadrant")
	PortGroupQuadrant.setDescription("Select the upper or lower quadrant in a half group for 8-bit accesses")
	PortGroupQuadrant.setDefaultValue("0")

	ControlPinsMenu = comp.createMenuSymbol("ControlPinsMenu", InterfaceSettingsMenu)
	ControlPinsMenu.setLabel("Control Pin Settings")

	ChipSelectControl = comp.createComboSymbol("ChipSelectControl", ControlPinsMenu, ["GPIO"])
	ChipSelectControl.setLabel("CS# Control")
	ChipSelectControl.setDescription("Chip Select Control. In GPIO mode, define GFX_DISP_INTF_PIN_CS pin in Pin Settings.")
	ChipSelectControl.setDefaultValue("GPIO")
	ChipSelectControl.setReadOnly(True)

	DataCommandSelectControl = comp.createComboSymbol("DataCommandSelectControl", ControlPinsMenu, ["GPIO"])
	DataCommandSelectControl.setLabel("D/C# Control")
	DataCommandSelectControl.setDescription("Data Command Select Control. In GPIO mode, define GFX_DISP_INTF_PIN_RSDC pin in Pin Settings")
	DataCommandSelectControl.setDefaultValue("GPIO")
	DataCommandSelectControl.setReadOnly(True)

	ReadStrobeControl = comp.createComboSymbol("ReadStrobeControl", ControlPinsMenu, ["GPIO"])
	ReadStrobeControl.setLabel("RD# Control")
	ReadStrobeControl.setDescription("Read Strobe Control. In GPIO mode, define GFX_DISP_INTF_PIN_RD pin in Pin Settings")
	ReadStrobeControl.setDefaultValue("GPIO")
	ReadStrobeControl.setReadOnly(True)

	WriteStrobeControl = comp.createComboSymbol("WriteStrobeControl", ControlPinsMenu, ["GPIO"])
	WriteStrobeControl.setLabel("WR# Control")
	WriteStrobeControl.setDescription("Write Strobe Control. In GPIO mode, define GFX_DISP_INTF_PIN_WR pin in Pin Settings")
	WriteStrobeControl.setDefaultValue("GPIO")
	WriteStrobeControl.setReadOnly(True)
	###
