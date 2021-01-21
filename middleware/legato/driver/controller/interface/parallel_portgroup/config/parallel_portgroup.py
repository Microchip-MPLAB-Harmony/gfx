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
import re

def instantiateComponent(comp):
	GFX_INTF_H = comp.createFileSymbol("GFX_INTF_H", None)
	GFX_INTF_H.setDestPath("gfx/interface/")
	GFX_INTF_H.setSourcePath("../drv_gfx_disp_intf.h")
	GFX_INTF_H.setOutputName("drv_gfx_disp_intf.h")
	GFX_INTF_H.setProjectPath("config/" + Variables.get("__CONFIGURATION_NAME") + "/gfx/interface")
	GFX_INTF_H.setType("HEADER")

	GFX_INTF_PORTGROUP = comp.createFileSymbol("GFX_INTF_PORTGROUP", None)
	GFX_INTF_PORTGROUP.setDestPath("gfx/interface/parallel_portgroup/")
	GFX_INTF_PORTGROUP.setSourcePath("templates/drv_gfx_disp_intf_parallel_portgroup.c.ftl")
	GFX_INTF_PORTGROUP.setOutputName("drv_gfx_disp_intf_parallel_portgroup.c")
	GFX_INTF_PORTGROUP.setProjectPath("config/" + Variables.get("__CONFIGURATION_NAME") + "/gfx/interface/parallel_portgroup")
	GFX_INTF_PORTGROUP.setMarkup(True)
	GFX_INTF_PORTGROUP.setType("SOURCE")

	GFX_INTF_PORTGROUP_DMA = comp.createFileSymbol("GFX_INTF_PORTGROUP_DMA", None)
	GFX_INTF_PORTGROUP_DMA.setDestPath("gfx/interface/parallel_portgroup/")
	GFX_INTF_PORTGROUP_DMA.setSourcePath("templates/drv_gfx_disp_intf_parallel_portgroup_dma.c.ftl")
	GFX_INTF_PORTGROUP_DMA.setOutputName("drv_gfx_disp_intf_parallel_portgroup_dma.c")
	GFX_INTF_PORTGROUP_DMA.setProjectPath("config/" + Variables.get("__CONFIGURATION_NAME") + "/gfx/interface/parallel_portgroup")
	GFX_INTF_PORTGROUP_DMA.setMarkup(True)
	GFX_INTF_PORTGROUP_DMA.setType("SOURCE")
	GFX_INTF_PORTGROUP_DMA.setEnabled(False)
	
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
	
	TransferMode = comp.createComboSymbol("TransferMode", DriverSettingsMenu, ["Bit-bang", "DMA-CCL"])
	TransferMode.setLabel("Transfer Mode")
	TransferMode.setDescription("Transfer Mode")
	TransferMode.setDependencies(onTransferModeSet, ["TransferMode"])
	
	UseTimerTrigger = comp.createBooleanSymbol("UseTimerTrigger", TransferMode)
	UseTimerTrigger.setLabel("Use Timer Trigger")
	UseTimerTrigger.setDescription("<html>Use Timer Trigger. Enable to set interface write speed.<br> Use with a TC peripheral in 8-bit Counter Mode, and Compare Operating Mode </html>")
	UseTimerTrigger.setVisible(False)
	UseTimerTrigger.setDependencies(onUseTimerTrigger, ["UseTimerTrigger"])
	
	TimerTriggerIndex = comp.createIntegerSymbol("TimerTriggerIndex", UseTimerTrigger)
	TimerTriggerIndex.setLabel("Timer Index")
	TimerTriggerIndex.setDescription("Index of Timer Peripheral")
	TimerTriggerIndex.setDefaultValue(0)

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
	
	TransferModeSettingsMenu = comp.createMenuSymbol("TransferModeSettingsMenu", None)
	TransferModeSettingsMenu.setLabel("Transfer Mode Settings")
	TransferModeSettingsMenu.setVisible(False)

	DMAChannel = comp.createIntegerSymbol("DMAChannel", TransferModeSettingsMenu)
	DMAChannel.setLabel("DMA Channel")
	DMAChannel.setMin(0)
	DMAChannel.setMax(32)
	
	DMATransferSize = comp.createComboSymbol("DMATransferSize", TransferModeSettingsMenu, ["8-bit"]) #, "16-bit"])
	DMATransferSize.setLabel("DMA Transfer Size")
	
	BlockingTransfers = comp.createBooleanSymbol("BlockingTransfers", TransferModeSettingsMenu)
	BlockingTransfers.setLabel("Blocking")
	BlockingTransfers.setDefaultValue(True)
	
	### Interface settings menu
	InterfaceSettingsMenu = comp.createMenuSymbol("InterfaceSettingsMenu", None)
	InterfaceSettingsMenu.setLabel("Parallel 8080 Display Interface Settings")
	
	PortGroupNum = comp.createComboSymbol("PortGroup", InterfaceSettingsMenu, ["0", "1", "2", "3"])
	PortGroupNum.setLabel("IO Group")
	PortGroupNum.setDescription("The port group")
	PortGroupNum.setDefaultValue("1")
	
	PortGroupDMAComment = comp.createCommentSymbol("PortGroupDMAComment", PortGroupNum)
	PortGroupDMAComment.setLabel("DMA-CCL mode only works with IO Group Half = 0, Quadrant = 0")
	PortGroupDMAComment.setVisible(False)

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

	ReadStrobeControl = comp.createComboSymbol("ReadStrobeControl", ControlPinsMenu, ["GPIO", "N/A"])
	ReadStrobeControl.setLabel("RD# Control")
	ReadStrobeControl.setDescription("Read Strobe Control. In GPIO mode, define GFX_DISP_INTF_PIN_RD pin in Pin Settings")
	ReadStrobeControl.setDefaultValue("GPIO")
	ReadStrobeControl.setReadOnly(True)

	WriteStrobeControl = comp.createComboSymbol("WriteStrobeControl", ControlPinsMenu, ["GPIO", "CCL"])
	WriteStrobeControl.setLabel("WR# Control")
	WriteStrobeControl.setDescription("Write Strobe Control. In GPIO mode, define GFX_DISP_INTF_PIN_WR pin in Pin Settings")
	WriteStrobeControl.setDefaultValue("GPIO")
	WriteStrobeControl.setReadOnly(True)
	###

def onTransferModeSet(source, event):
	print("onTransferModeSet " + str(event['value']))
	source.getComponent().getSymbolByID("DelayNOPCount").setVisible(event['value'] == "Bit-bang")
	source.getComponent().getSymbolByID("PortGroupHalf").setVisible(event['value'] == "Bit-bang")
	source.getComponent().getSymbolByID("PortGroupQuadrant").setVisible(event['value'] == "Bit-bang")
	source.getComponent().getSymbolByID("PortGroupDMAComment").setVisible(event['value'] == "DMA-CCL")
	source.getComponent().getSymbolByID("TransferModeSettingsMenu").setVisible(event['value'] == "DMA-CCL")
	source.getComponent().getSymbolByID("UseTimerTrigger").setVisible(event['value'] == "DMA-CCL")
	
	source.getComponent().getSymbolByID("GFX_INTF_PORTGROUP").setEnabled(event['value'] == "Bit-bang")
	source.getComponent().getSymbolByID("GFX_INTF_PORTGROUP_DMA").setEnabled(event['value'] == "DMA-CCL")
	
	if (event['value'] == "DMA-CCL"):
		source.getComponent().setSymbolValue("ReadStrobeControl", "N/A", 0)
		source.getComponent().setSymbolValue("WriteStrobeControl", "CCL", 0)
	else:
		source.getComponent().clearSymbolValue("ReadStrobeControl")
		source.getComponent().clearSymbolValue("WriteStrobeControl")

def onUseTimerTrigger(source, event):
	print("onUseTimerTrigger " + str(event['value']))
	source.getComponent().setDependencyEnabled("TMR", event['value'])

def onAttachmentConnected(source, target):
	print(source["component"].getID() + ": " + target["id"] + " dependent component added ")
	if (source["id"] == "TMR"):
		index = re.search(r'\d+', target["id"]).group(0)
		if (index):
			source["component"].setSymbolValue("TimerTriggerIndex", int(index))
	