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

def pollingFileGen(symbol, event):
    if event["value"] == "Polling":
       symbol.setEnabled(True)
    else:
       symbol.setEnabled(False)

def interruptFileGen(symbol, event):
    if event["value"] == "Interrupt":
       symbol.setEnabled(True)
    else:
       symbol.setEnabled(False)

def setCommonMode(symbol, event):
    rtos_mode = event["value"]

    if rtos_mode != None:
        if rtos_mode == "BareMetal":
            symbol.setValue("Polling")
        else:
            symbol.setValue("Interrupt")


def instantiateComponent(component):

        global TmrInstanceName
        global TouchPanelWidth
        global TouchPanelHeight

	component.setHelpFile("../../../../docs/help_harmony_gfx_html_alias.h")

        rtos_mode = Database.getSymbolValue("HarmonyCore", "SELECT_RTOS")


        ### Start of Touch Panel config
        TouchPanelSettings = component.createMenuSymbol("TouchPanelSettings", None)
        TouchPanelSettings.setLabel("Touch Panel Settings")

        TouchPanelWidth = component.createIntegerSymbol("TOUCH_PANEL_WIDTH", TouchPanelSettings)
        TouchPanelWidth.setLabel("Touch Panel Width")
        TouchPanelWidth.setDescription("The width of the touch panel in pixels.")
        TouchPanelWidth.setDefaultValue(480)

        TouchPanelHeight = component.createIntegerSymbol("TOUCH_PANEL_HEIGHT", TouchPanelSettings)
        TouchPanelHeight.setLabel("Touch Panel Height")
        TouchPanelHeight.setDescription("The height of the touch panel in pixels.")
        TouchPanelHeight.setDefaultValue(272)

        TouchPressThreshold = component.createIntegerSymbol("TOUCH_ADC_PRESS_THRESHOLD", TouchPanelSettings)
        TouchPressThreshold.setLabel("Touch Press Threshold")
        TouchPressThreshold.setDescription("The water mark for sensitivty.")
        TouchPressThreshold.setDefaultValue(256)

        ### Start of Touch Polling config
        PollingSettings = component.createMenuSymbol("PollingSettings", None)
        PollingSettings.setLabel("Touch Polling Settings")

        PollingMode = component.createComboSymbol("PollingMode", PollingSettings, ["Interrupt", "Task"])
        PollingMode.setLabel("Polling Mode")
        PollingMode.setDescription("Mode used to detect a touch.")
        PollingMode.setDefaultValue("Interrupt")
        PollingMode.setDependencies(onPollingModeSet, ["PollingMode"])
        PollingMode.setReadOnly(True)


        TmrInstanceName = component.createStringSymbol("TOUCH_ADC_TMR_INSTANCE", PollingSettings)
        TmrInstanceName.setLabel("Interrupt TMR Instance")
        #tmrInstanceName.setDefaultValue(tmrComponent.getID().upper())
        #Log.writeInfoMessage("Using Interrupt from " + TmrInstanceName.getValue())

        TimerIndexComment = component.createCommentSymbol("TimerIndexComment", PollingSettings)
        TimerIndexComment.setLabel('Please enable and configure selected TMR index')
	
        AnalogInputSettingsMenu = component.createMenuSymbol("AnalogInputSettingsMenu", None)
        AnalogInputSettingsMenu.setLabel("Analog Input Settings")

        ADCModuleSetting = component.createComboSymbol("ADCModuleSetting", AnalogInputSettingsMenu, ["ADCHS_MODULE7"])
        ADCModuleSetting.setLabel("ADC Module Setting")

        ADCModuleSettingComment = component.createCommentSymbol("ADCModuleSettingComment", AnalogInputSettingsMenu)
        ADCModuleSettingComment.setLabel('Please enable and configure ADC Module')

        TouchANInputX = component.createComboSymbol("AN_CHANNEL_X", AnalogInputSettingsMenu, ["ADCHS_CH12", "ADCHS_CH13", "ADCHS_CH14",
        "ADCHS_CH15", "ADCHS_CH16", "ADCHS_CH17", "ADCHS_CH18", "ADCHS_CH19", "ADCHS_CH20",
        "ADCHS_CH21", "ADCHS_CH22", "ADCHS_CH23", "ADCHS_CH24", "ADCHS_CH25", "ADCHS_CH26",
        "ADCHS_CH27", "ADCHS_CH28", "ADCHS_CH29", "ADCHS_CH30", "ADCHS_CH31", "ADCHS_CH32",
        "ADCHS_CH33", "ADCHS_CH34", "ADCHS_CH43", "ADCHS_CH44"])
        TouchANInputX.setLabel("Analog Input Channel X")

        TouchANInputY = component.createComboSymbol("AN_CHANNEL_Y", AnalogInputSettingsMenu, ["ADCHS_CH12", "ADCHS_CH13", "ADCHS_CH14",
        "ADCHS_CH15", "ADCHS_CH16", "ADCHS_CH17", "ADCHS_CH18", "ADCHS_CH19", "ADCHS_CH20",
        "ADCHS_CH21", "ADCHS_CH22", "ADCHS_CH23", "ADCHS_CH24", "ADCHS_CH25", "ADCHS_CH26",
        "ADCHS_CH27", "ADCHS_CH28", "ADCHS_CH29", "ADCHS_CH30", "ADCHS_CH31", "ADCHS_CH32",
        "ADCHS_CH33", "ADCHS_CH34", "ADCHS_CH43", "ADCHS_CH44"])
        TouchANInputY.setLabel("Analog Input Channel Y")

        CalibrationSettingsMenu = component.createMenuSymbol("CalibrationSettingsMenu", None)
        CalibrationSettingsMenu.setLabel("Calibration Settings")

        CalULXSample = component.createIntegerSymbol("CAL_ULX_SAMPLE", CalibrationSettingsMenu)
        CalULXSample.setLabel("Upper Left X Sample (ULX)")
        CalULXSample.setDefaultValue(0x0099)

        CalULYSample = component.createIntegerSymbol("CAL_ULY_SAMPLE", CalibrationSettingsMenu)
        CalULYSample.setLabel("Upper Left Y Sample (ULY) ")
        CalULYSample.setDefaultValue(0x00FF)

        CalURXSample = component.createIntegerSymbol("CAL_URX_SAMPLE", CalibrationSettingsMenu)
        CalURXSample.setLabel("Upper Right X Sample (URX)")
        CalURXSample.setDefaultValue(0x0369)

        CalURYSample = component.createIntegerSymbol("CAL_URY_SAMPLE", CalibrationSettingsMenu)
        CalURYSample.setLabel("Upper Right Y Sample (URY)")
        CalURYSample.setDefaultValue(0x00F1)

        CalLRXSample = component.createIntegerSymbol("CAL_LRX_SAMPLE", CalibrationSettingsMenu)
        CalLRXSample.setLabel("Lower Right X Sample (LRX)")
        CalLRXSample.setDefaultValue(0x0368)

        CalLRYSample = component.createIntegerSymbol("CAL_LRY_SAMPLE", CalibrationSettingsMenu)
        CalLRYSample.setLabel("Lower Right Y Sample (LRY)")
        CalLRYSample.setDefaultValue(0x033C)

        CalLLXSample = component.createIntegerSymbol("CAL_LLX_SAMPLE", CalibrationSettingsMenu)
        CalLLXSample.setLabel("Lower Left X Sample (LLX)")
        CalLLXSample.setDefaultValue(0x00AC)

        CalLLYSample = component.createIntegerSymbol("CAL_LLY_SAMPLE", CalibrationSettingsMenu)
        CalLLYSample.setLabel("Lower Left Y Sample (LLY)")
        CalLLYSample.setDefaultValue(0x032A)

	RTOSMenu = component.createMenuSymbol("RTOSMenu", None)
	RTOSMenu.setLabel("RTOS Settings")
	RTOSMenu.setDescription("RTOS Settings")
	RTOSMenu.setVisible(Database.getSymbolValue("HarmonyCore", "SELECT_RTOS") != "BareMetal")
	RTOSMenu.setDependencies(showTouchRTOSMenu, ["HarmonyCore.SELECT_RTOS"])
	
	RTOSEnabled = component.createBooleanSymbol("RTOSEnabled", RTOSMenu)
	RTOSEnabled.setLabel("RTOS Support Enabled")
	RTOSEnabled.setDefaultValue(Database.getSymbolValue("HarmonyCore", "SELECT_RTOS") != "BareMetal")
	RTOSEnabled.setDependencies(enableTouchRTOSymbol, ["HarmonyCore.SELECT_RTOS"])
	RTOSEnabled.setVisible(False)

	RTOSStackSize = component.createIntegerSymbol("RTOSStackSize", RTOSMenu)
	RTOSStackSize.setLabel("Stack Size")
	RTOSStackSize.setMin(0)
	RTOSStackSize.setDefaultValue(1024)
	
	RTOSTaskPriority = component.createIntegerSymbol("RTOSTaskPriority", RTOSMenu)
	RTOSTaskPriority.setLabel("Task Priority")
	RTOSTaskPriority.setDefaultValue(1)
	
	RTOSUseDelay = component.createBooleanSymbol("RTOSUseDelay", RTOSMenu)
	RTOSUseDelay.setLabel("Use Task Delay?")
	RTOSUseDelay.setDefaultValue(True)
	
	RTOSTaskDelay = component.createIntegerSymbol("RTOSTaskDelay", RTOSMenu)
	RTOSTaskDelay.setLabel("Task Delay")
	RTOSTaskDelay.setDefaultValue(10)
	RTOSTaskDelay.setMin(0)

        projectPath = "config/" + Variables.get("__CONFIGURATION_NAME") + "/driver/input"

        TouchAdc_h = component.createFileSymbol("DRV_TOUCH_ADC_H", None)
        TouchAdc_h.setSourcePath("templates/drv_touch_adc.h.ftl")
        TouchAdc_h.setOutputName("drv_touch_adc.h")
        TouchAdc_h.setDestPath("driver/input/")
        TouchAdc_h.setProjectPath(projectPath)
        TouchAdc_h.setType("HEADER")
        TouchAdc_h.setOverwrite(True)
        TouchAdc_h.setMarkup(True)

        # Interrupt Source Files
        TouchAdc_c = component.createFileSymbol("DRV_TOUCH_ADC_C", None)
        TouchAdc_c.setSourcePath("templates/drv_touch_adc_mz.c.ftl")
        TouchAdc_c.setOutputName("drv_touch_adc.c")
        TouchAdc_c.setDestPath("driver/input/")
        TouchAdc_c.setProjectPath(projectPath)
        TouchAdc_c.setType("SOURCE")
        TouchAdc_c.setOverwrite(True)
        # DRV_TOUCH_ADC_C.setDependencies(pollFileGen, ["DRV_ADC_MODE"])
        TouchAdc_c.setMarkup(True)

        # Polling Source Files
        #touchAdc_c = component.createFileSymbol("DRV_TOUCH_ADC_C", None)
        #touchAdc_c.setSourcePath("templates/drv_touch_adc_mz.c.ftl")
        #touchAdc_c.setOutputName("drv_touch_adc.c")
        #touchAdc_c.setDestPath("driver/input/")
        #touchAdc_c.setProjectPath(projectPath)
        #touchAdc_c.setType("SOURCE")
        #touchAdc_c.setOverwrite(True)

        #touchInitData = component.createFileSymbol("TOUCH_DRV_INIT_DATA", None)
        #touchInitData.setType("STRING")
        #touchInitData.setOutputName("core.LIST_SYSTEM_INIT_C_DRIVER_INITIALIZATION_DATA")
        #touchInitData.setSourcePath("templates/drv_touch_adc_init.c.ftl")
        #touchInitData.setMarkup(True)
	
        TouchObjData = component.createFileSymbol("TOUCH_DRV_OBJ_DATA", None)
        TouchObjData.setType("STRING")
        TouchObjData.setOutputName("core.LIST_SYSTEM_DEFINITIONS_H_OBJECTS")
        TouchObjData.setSourcePath("templates/drv_touch_adc_definitions_obj.h.ftl")
        TouchObjData.setMarkup(True)

        TouchDefinitions = component.createFileSymbol("TOUCH_DRV_DEFINITIONS_H", None)
        TouchDefinitions.setType("STRING")
        TouchDefinitions.setOutputName("core.LIST_SYSTEM_DEFINITIONS_H_INCLUDES")
        TouchDefinitions.setSourcePath("templates/drv_touch_adc_definitions.h.ftl")
        TouchDefinitions.setMarkup(True)
	
        TouchInit = component.createFileSymbol("TOUCH_DRV_INIT_C", None)
        TouchInit.setType("STRING")
        TouchInit.setOutputName("core.LIST_SYSTEM_INIT_C_SYS_INITIALIZE_DRIVERS")
        TouchInit.setSourcePath("templates/drv_touch_adc_initialize.c.ftl")
        TouchInit.setMarkup(True)

        TouchTask = component.createFileSymbol("TOUCH_DRV_TASK_C", None)
        TouchTask.setType("STRING")
        TouchTask.setOutputName("core.LIST_SYSTEM_TASKS_C_CALL_DRIVER_TASKS")
        TouchTask.setSourcePath("templates/drv_touch_adc_tasks.c.ftl")
        TouchTask.setMarkup(True)

        TouchRtosTask = component.createFileSymbol("TOUCH_DRV_RTOS_TASK_C", None)
        TouchRtosTask.setType("STRING")
        TouchRtosTask.setOutputName("core.LIST_SYSTEM_RTOS_TASKS_C_DEFINITIONS")
        TouchRtosTask.setSourcePath("templates/drv_touch_adc_rtos_tasks.c.ftl")
        TouchRtosTask.setMarkup(True)
        TouchRtosTask.setEnabled((Database.getSymbolValue("HarmonyCore", "SELECT_RTOS") != "BareMetal"))
        TouchRtosTask.setDependencies(enableTouchRTOSymbol, ["HarmonyCore.SELECT_RTOS"])
	
def onAttachmentConnected(source, target):
        if source["id"] == "touch_panel":
                TouchPanelWidth.setValue(target["component"].getSymbolValue("DisplayWidth"))
                TouchPanelHeight.setValue(target["component"].getSymbolValue("DisplayHeight"))
                TouchPanelWidth.setReadOnly(True)
                TouchPanelHeight.setReadOnly(True)

        if source["id"] == "tmr_plib":
                TmrInstanceName.setValue(target["component"].getSymbolValue("TMR_INSTANCE_NAME"))
                TmrInstanceName.setReadOnly(True)
                source["component"].getSymbolByID("TimerIndexComment").setVisible(False)


def onAttachmentDisconnected(source, target):
	if source["id"] == "touch_panel":
                TouchPanelWidth.clearValue()
                TouchPanelWidth.clearValue()
                TouchPanelWidth.setReadOnly(False)
                TouchPanelHeight.setReadOnly(False)
	
        if source["id"] == "tmr":
                TmrInstanceName.clearValue()
                TmrInstanceName.setReadOnly(False)
		
def enableTouchRTOSymbol(symbol, event):
    if (event["value"] != "BareMetal"):
        symbol.setEnabled(True)
    else:
        symbol.setEnabled(False)
		
def showTouchRTOSMenu(symbol, event):
	if (event["value"] != "BareMetal"):
		symbol.setVisible(True)
	else:
		symbol.setVisible(False)

def onPollingModeSet(symbol, event):
        symbol.getComponent().getSymbolByID("TOUCH_ADC_TMR_INSTANCE").setVisible(event["value"] == "Interrupt")
        symbol.getComponent().getSymbolByID("TimerIndexComment").setVisible(event["value"] == "Interrupt")
