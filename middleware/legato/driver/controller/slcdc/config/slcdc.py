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

def instantiateComponent(comp):
	projectPath = "config/" + Variables.get("__CONFIGURATION_NAME") + "/gfx/driver/slcdc"
	
	# these two symbols are read by the HAL for initialization purposes
	# they must match the function names in the actual driver code
	DriverInitName = comp.createStringSymbol("DriverInitName", None)
	DriverInitName.setVisible(False)
	DriverInitName.setReadOnly(True)
	DriverInitName.setDefaultValue("slcdcDisplayDriver")

	BoardSelect = comp.createComboSymbol("BoardSelect", None, ["PIC32CXMTSH_DB", "PIC32CXMTC_DB"])
	BoardSelect.setLabel("PIC32CX Board")
	BoardSelect.setDefaultValue("PIC32CXMTSH_DB")
	BoardSelect.setVisible(True)

	WaveFormMode = comp.createComboSymbol("WaveFormMode", None, ["LOW_POWER", "STANDARD"])
	WaveFormMode.setLabel("WaveForm")
	WaveFormMode.setDefaultValue("SLCDC_LOW_POWER_WAVEFORM_MODE")
	WaveFormMode.setVisible(True)

	SLCDC_Bias = comp.createComboSymbol("SLCDC_Bias", None, ["Static", "1/2", "1/3", "1/4"])
	SLCDC_Bias.setLabel("SLCDC Bias")
	SLCDC_Bias.setDefaultValue("1/3")
	SLCDC_Bias.setDescription("SLCDC Bias")
	SLCDC_Bias.setVisible(True)

	BufferTimeSelect = comp.createComboSymbol("BufferTimeSelect", None, ["Off", "SLCKx2", "SLCKx4", "SLCKx8","SLCKx16","SLCKx32","SLCKx64","SLCKx128","50% of SLCK","10% of SLCK"])
	BufferTimeSelect.setLabel("Buffer On Time")
	BufferTimeSelect.setDefaultValue("SLCKx64")
	BufferTimeSelect.setDescription("Buffer On Time - BUFTIME")
	BufferTimeSelect.setVisible(True)


	CommonSelect = comp.createStringSymbol("CommonSelect", None)
	CommonSelect.setLabel("Number of Commons (1-8)")
	CommonSelect.setDefaultValue("8")
	CommonSelect.setDescription("Number of Commons - COMSEL")

	SegmentSelect = comp.createStringSymbol("SegmentSelect", None)
	SegmentSelect.setLabel("Number of Segments (1-31)")
	SegmentSelect.setDefaultValue("20")
	SegmentSelect.setDescription("Number of Segments - SEGSEL")

	### SLCDC Frame Frequency
	FrameFrequencyMenu = comp.createMenuSymbol("FrameFrequencyMenu", None)
	FrameFrequencyMenu.setLabel("Frame Frequency")

	FramePrescaler = comp.createComboSymbol("FramePrescaler", FrameFrequencyMenu, ["8", "16", "32", "64", "128", "256", "1024"])
	FramePrescaler.setLabel("PRESC")
	FramePrescaler.setDefaultValue("64")
	FramePrescaler.setDescription("SLCDC Prescaler Value (PRESC)")
	FramePrescaler.setVisible(True)

	FrameDivider = comp.createIntegerSymbol("FrameDivider", FrameFrequencyMenu)
	FrameDivider.setLabel("DIV")
	FrameDivider.setDescription("SLCDC Clock divider (DIV)")
	FrameDivider.setDefaultValue(2)
	FrameDivider.setMin(1)
	FrameDivider.setMax(8)
	
	DislayMode = comp.createComboSymbol("DislayMode", None, ["Normal","Force Off","Force On","Blinking","Inverted","Inerted Blink","User Buffer Load","Buffer Swap"])
	DislayMode.setLabel("Display Mode")
	DislayMode.setDefaultValue("Normal")
	DislayMode.setDescription("Display Mode")
	DislayMode.setVisible(True)

	LcdcPowerMode = comp.createComboSymbol("LcdcPowerMode", None, ["Off", "External Votage Supply", "Internal Votage Regulator"])
	LcdcPowerMode.setLabel("LCDC Power Mode")
	LcdcPowerMode.setDefaultValue("Internal Votage Regulator")
	LcdcPowerMode.setDescription("LCDC Power Mode")
	LcdcPowerMode.setVisible(True)


	# generated header files 
	SYS_DEFINITIONS_H = comp.createFileSymbol("SYS_DEFINITIONS_H", None)
	SYS_DEFINITIONS_H.setType("STRING")
	SYS_DEFINITIONS_H.setOutputName("core.LIST_SYSTEM_DEFINITIONS_H_INCLUDES")
	SYS_DEFINITIONS_H.setSourcePath("templates/definitions.h.ftl")
	SYS_DEFINITIONS_H.setMarkup(True)

	SYS_INIT_C = comp.createFileSymbol("SYS_INIT_C", None)
	SYS_INIT_C.setType("STRING")
	SYS_INIT_C.setOutputName("core.LIST_SYSTEM_INIT_C_SYS_INITIALIZE_DRIVERS")
	SYS_INIT_C.setSourcePath("templates/init.c.ftl")
	SYS_INIT_C.setMarkup(True)

	SYS_TASK_C = comp.createFileSymbol("SYS_TASK_C", None)
	SYS_TASK_C.setType("STRING")
	SYS_TASK_C.setOutputName("core.LIST_SYSTEM_TASKS_C_CALL_DRIVER_TASKS")
	SYS_TASK_C.setSourcePath("templates/tasks.c.ftl")
	SYS_TASK_C.setMarkup(True)


	SYS_RTOS_TASK_C = comp.createFileSymbol("SYS_RTOS_TASK_C", None)
	SYS_RTOS_TASK_C.setType("STRING")
	SYS_RTOS_TASK_C.setOutputName("core.LIST_SYSTEM_RTOS_TASKS_C_DEFINITIONS")
	SYS_RTOS_TASK_C.setSourcePath("templates/rtos_tasks.c.ftl")
	SYS_RTOS_TASK_C.setMarkup(True)

	execfile(Module.getPath() + "/config/slcdc_rtos.py")


	# generated code files
	CL010_SEGMAP_H = comp.createFileSymbol("CL010_SEGMAP_H", None)
	CL010_SEGMAP_H.setSourcePath("templates/cl010_segmap.h.ftl")
	CL010_SEGMAP_H.setDestPath("gfx/driver/controller/slcdc/")
	CL010_SEGMAP_H.setOutputName("cl010_segmap.h")
	CL010_SEGMAP_H.setProjectPath(projectPath)
	CL010_SEGMAP_H.setType("HEADER")
	CL010_SEGMAP_H.setMarkup(True)

	GFX_LCDC_C = comp.createFileSymbol("GFX_SLCDC_C", None)
	GFX_LCDC_C.setDestPath("gfx/driver/controller/slcdc/")
	GFX_LCDC_C.setSourcePath("templates/drv_gfx_slcdc.c.ftl")
	GFX_LCDC_C.setOutputName("drv_gfx_slcdc.c")
	GFX_LCDC_C.setProjectPath(projectPath)
	GFX_LCDC_C.setType("SOURCE")
	GFX_LCDC_C.setMarkup(True)
	
	GFX_LCDC_H = comp.createFileSymbol("GFX_SLCDC_H", None)
	GFX_LCDC_H.setSourcePath("templates/drv_gfx_slcdc.h.ftl")
	GFX_LCDC_H.setDestPath("gfx/driver/controller/slcdc/")
	GFX_LCDC_H.setOutputName("drv_gfx_slcdc.h")
	GFX_LCDC_H.setProjectPath(projectPath)
	GFX_LCDC_H.setType("HEADER")
	GFX_LCDC_H.setMarkup(True)

	CL010_H = comp.createFileSymbol("CL010_H", None)
	CL010_H.setSourcePath("templates/cl010.h")
	CL010_H.setDestPath("gfx/driver/controller/slcdc/")
	CL010_H.setOutputName("cl010.h")
	CL010_H.setProjectPath(projectPath)
	CL010_H.setType("HEADER")

	CL010_FONT_H = comp.createFileSymbol("CL010_FONT_H", None)
	CL010_FONT_H.setSourcePath("templates/cl010_font.h")
	CL010_FONT_H.setDestPath("gfx/driver/controller/slcdc/")
	CL010_FONT_H.setOutputName("cl010_font.h")
	CL010_FONT_H.setProjectPath(projectPath)
	CL010_FONT_H.setType("HEADER")

	CL010_SLCDC_C = comp.createFileSymbol("CL010_SLCDC_C", None)
	CL010_SLCDC_C.setSourcePath("templates/cl010_slcdc.c")
	CL010_SLCDC_C.setDestPath("gfx/driver/controller/slcdc/")
	CL010_SLCDC_C.setOutputName("cl010_slcdc.c")
	CL010_SLCDC_C.setProjectPath(projectPath)
	CL010_SLCDC_C.setType("SOURCE")
	

def onAttachmentConnected(source, target):
	print("dependency Connected = " + target["component"].getDisplayName())

def showRTOSMenu(symbol, event):
	symbol.setVisible(event["value"] != "BareMetal")

