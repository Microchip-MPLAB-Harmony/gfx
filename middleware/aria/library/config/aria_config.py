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

libraryOptionsMenu = component.createMenuSymbol("libraryOptionsMenu", None)
libraryOptionsMenu.setLabel("Library Configuration")

useAcceleration = component.createBooleanSymbol("useAcceleration", libraryOptionsMenu)
useAcceleration.setLabel("Enable Display Accleration?")
useAcceleration.setDescription("<html>Enables graphics acceleration pipeline <b>if available<b>.")

useGlobalPalette = component.createBooleanSymbol("useGlobalPalette", libraryOptionsMenu)
useGlobalPalette.setLabel("Enable Global Palette Mode?")
useGlobalPalette.setDescription("Enables global palette mode.")
useGlobalPalette.setDependencies(onUseGlobalPaletteChanged, ["useGlobalPalette"])
useGlobalPalette.setHelp("IDH_HTML_MHGC_UG_Global_Palette")

codeGenerationMenu = component.createMenuSymbol("codeGenerationMenu", None)
codeGenerationMenu.setLabel("Code Generator Options")

genAriaAssets = component.createBooleanSymbol("genAriaAssets", codeGenerationMenu)
genAriaAssets.setLabel("Generate Assets?")
genAriaAssets.setDefaultValue(True)
genAriaAssets.setDescription("Indicates that any configured MHGC assets should be automatically generated and added to the application.")

genAriaDesign = component.createBooleanSymbol("genAriaDesign", codeGenerationMenu)
genAriaDesign.setLabel("Generate Design?")
genAriaDesign.setDefaultValue(True)
genAriaDesign.setDescription("Indicates that any configured MHGC screens should be automatically generated and added to the application.")
genAriaDesign.setDependencies(onGenAriaDesignChanged, ["genAriaDesign"])

genAriaEvents = component.createBooleanSymbol("genAriaEvents", genAriaDesign)
genAriaEvents.setLabel("Generate Event Handlers?")
genAriaEvents.setDefaultValue(True)
genAriaEvents.setDescription("Indicates that any configured MHGC events should be automatically generated and added to the application.")
genAriaEvents.setHelp("IDH_HTML_MHGC_UG_Event_Manager")

genAriaMacros = component.createBooleanSymbol("genAriaMacros", genAriaDesign)
genAriaMacros.setLabel("Generate Macros?")
genAriaMacros.setDefaultValue(True)
genAriaMacros.setDescription("Indicates that any configured MHGC macros should be automatically generated and added to the application.")

genAriaMediaIntf = component.createBooleanSymbol("genAriaMediaIntf", genAriaDesign)
genAriaMediaIntf.setLabel("Generate External Media Interface?")
genAriaMediaIntf.setDefaultValue(True)
genAriaMediaIntf.setDescription("Indicates that the code generator should create the binding code to allow Aria to request external media data.")

enableInput = component.createBooleanSymbol("enableInput", codeGenerationMenu)
enableInput.setLabel("Enable Input Event Interface?")
enableInput.setDefaultValue(False)
enableInput.setDescription("Indicates that Aria should interface with the Input System Service for input events.")
enableInput.setDependencies(onEnableInputChanged, ["enableInput"])

enableAppSupport = component.createBooleanSymbol("enableAppSupport", codeGenerationMenu)
enableAppSupport.setLabel("Include application header in libaria_event.c?")
enableAppSupport.setDescription("Generate libaria_event.c with #include the header file of the application file")
enableAppSupport.setDefaultValue(True)
enableAppSupport.setDependencies(onAppFileEnabled, ["HarmonyCore.ENABLE_APP_FILE","HarmonyCore.GEN_APP_TASK_COUNT"])

genAriaEventAppHeaderName = component.createStringSymbol("genAriaEventAppHeaderName", enableAppSupport)
genAriaEventAppHeaderName.setLabel("Application Header Name")
genAriaEventAppHeaderName.setDefaultValue("app.h")
genAriaEventAppHeaderName.setDependencies(onAppNameChanged, ["HarmonyCore.ENABLE_APP_FILE","HarmonyCore.GEN_APP_TASK_COUNT","HarmonyCore.GEN_APP_TASK_NAME_0"])
genAriaEventAppHeaderName.setUseSingleDynamicValue(True)

widgetMenu = component.createMenuSymbol("widgetMenu", None)
widgetMenu.setLabel("Widget Configuration")
widgetMenu.setHelp("IDH_HTML_MHGC_UG_Widget_Tool_Box_Panel")

enableArcWidget = component.createBooleanSymbol("enableArcWidget", widgetMenu)
enableArcWidget.setLabel("Enable Arc Widget?")
enableArcWidget.setDefaultValue(True)
enableArcWidget.setDescription("Enables the Aria Arc widget.  Disabling this will remove the code for this widget and it will not be available for use.</html>")
enableArcWidget.setHelp("IDH_HTML_MHGC_UG_Widget_Tool_Box_Panel")

enableBarGraphWidget = component.createBooleanSymbol("enableBarGraphWidget", widgetMenu)
enableBarGraphWidget.setLabel("Enable Bar Graph Widget?")
enableBarGraphWidget.setDefaultValue(True)
enableBarGraphWidget.setDescription("Enables the Aria Bar Graph widget.  Disabling this will remove the code for this widget and it will not be available for use.")
enableBarGraphWidget.setHelp("IDH_HTML_MHGC_UG_Widget_Tool_Box_Panel")

enableButtonWidget = component.createBooleanSymbol("enableButtonWidget", widgetMenu)
enableButtonWidget.setLabel("Enable Button Widget?")
enableButtonWidget.setDefaultValue(True)
enableButtonWidget.setDescription("Enables the Aria Button widget.  Disabling this will remove the code for this widget and it will not be available for use.")
enableButtonWidget.setHelp("IDH_HTML_MHGC_UG_Widget_Tool_Box_Panel")

enableCheckBoxWidget = component.createBooleanSymbol("enableCheckBoxWidget", widgetMenu)
enableCheckBoxWidget.setLabel("Enable Check Box Widget?")
enableCheckBoxWidget.setDefaultValue(True)
enableCheckBoxWidget.setDescription("Enables the Aria Check Box widget.  Disabling this will remove the code for this widget and it will not be available for use.")
enableCheckBoxWidget.setHelp("IDH_HTML_MHGC_UG_Widget_Tool_Box_Panel")

enableCircleWidget = component.createBooleanSymbol("enableCircleWidget", widgetMenu)
enableCircleWidget.setLabel("Enable Circle Widget?")
enableCircleWidget.setDefaultValue(True)
enableCircleWidget.setDescription("Enables the Aria Circle widget.  Disabling this will remove the code for this widget and it will not be available for use.")
enableCircleWidget.setHelp("IDH_HTML_MHGC_UG_Widget_Tool_Box_Panel")

enableCircularGuageWidget = component.createBooleanSymbol("enableCircularGuageWidget", widgetMenu)
enableCircularGuageWidget.setLabel("Enable Circular Guage Widget?")
enableCircularGuageWidget.setDefaultValue(True)
enableCircularGuageWidget.setDescription("Enables the Aria Circular Guage widget.  Disabling this will remove the code for this widget and it will not be available for use.")
enableCircularGuageWidget.setHelp("IDH_HTML_MHGC_UG_Widget_Tool_Box_Panel")

enableCircularSliderWidget = component.createBooleanSymbol("enableCircularSliderWidget", widgetMenu)
enableCircularSliderWidget.setLabel("Enable Circular Slider Widget?")
enableCircularSliderWidget.setDefaultValue(True)
enableCircularSliderWidget.setDescription("Enables the Aria Circular Slider widget.  Disabling this will remove the code for this widget and it will not be available for use.")
enableCircularSliderWidget.setHelp("IDH_HTML_MHGC_UG_Widget_Tool_Box_Panel")

enableDrawSurfaceWidget = component.createBooleanSymbol("enableDrawSurfaceWidget", widgetMenu)
enableDrawSurfaceWidget.setLabel("Enable Draw Surface Widget?")
enableDrawSurfaceWidget.setDefaultValue(True)
enableDrawSurfaceWidget.setDescription("Enables the Aria Draw Surface widget.  Disabling this will remove the code for this widget and it will not be available for use.")
enableDrawSurfaceWidget.setHelp("IDH_HTML_MHGC_UG_Widget_Tool_Box_Panel")

enableImageWidget = component.createBooleanSymbol("enableImageWidget", widgetMenu)
enableImageWidget.setLabel("Enable Image Widget?")
enableImageWidget.setDefaultValue(True)
enableImageWidget.setDescription("Enables the Aria Image widget.  Disabling this will remove the code for this widget and it will not be available for use.")
enableImageWidget.setHelp("IDH_HTML_MHGC_UG_Widget_Tool_Box_Panel")

enableImagePlusWidget = component.createBooleanSymbol("enableImagePlusWidget", widgetMenu)
enableImagePlusWidget.setLabel("Enable Image Plus Widget?")
enableImagePlusWidget.setDefaultValue(True)
enableImagePlusWidget.setDescription("Enables the Aria Image Plus widget.  Disabling this will remove the code for this widget and it will not be available for use.")
enableImagePlusWidget.setHelp("IDH_HTML_MHGC_UG_Widget_Tool_Box_Panel")

enableImageSequenceWidget = component.createBooleanSymbol("enableImageSequenceWidget", widgetMenu)
enableImageSequenceWidget.setLabel("Enable Image Sequence Widget?")
enableImageSequenceWidget.setDefaultValue(True)
enableImageSequenceWidget.setDescription("Enables the Aria Image Sequence widget.  Disabling this will remove the code for this widget and it will not be available for use.")
enableImageSequenceWidget.setHelp("IDH_HTML_MHGC_UG_Widget_Tool_Box_Panel")

enableGradientWidget = component.createBooleanSymbol("enableGradientWidget", widgetMenu)
enableGradientWidget.setLabel("Enable Gradient Widget?")
enableGradientWidget.setDefaultValue(True)
enableGradientWidget.setDescription("Enables the Aria Gradient widget.  Disabling this will remove the code for this widget and it will not be available for use.")
enableGradientWidget.setHelp("IDH_HTML_MHGC_UG_Widget_Tool_Box_Panel")

enableGroupBoxWidget = component.createBooleanSymbol("enableGroupBoxWidget", widgetMenu)
enableGroupBoxWidget.setLabel("Enable Group Box Widget?")
enableGroupBoxWidget.setDefaultValue(True)
enableGroupBoxWidget.setDescription("Enables the Aria Group Box widget.  Disabling this will remove the code for this widget and it will not be available for use.")
enableGroupBoxWidget.setHelp("IDH_HTML_MHGC_UG_Widget_Tool_Box_Panel")

enableKeyPadWidget = component.createBooleanSymbol("enableKeyPadWidget", widgetMenu)
enableKeyPadWidget.setLabel("Enable Key Pad Widget?")
enableKeyPadWidget.setDefaultValue(True)
enableKeyPadWidget.setDescription("Enables the Aria Key Pad widget.  Disabling this will remove the code for this widget and it will not be available for use.")
enableKeyPadWidget.setHelp("IDH_HTML_MHGC_UG_Widget_Tool_Box_Panel")

enableLabelWidget = component.createBooleanSymbol("enableLabelWidget", widgetMenu)
enableLabelWidget.setLabel("Enable Label Widget?")
enableLabelWidget.setDefaultValue(True)
enableLabelWidget.setDescription("Enables the Aria Label widget.  Disabling this will remove the code for this widget and it will not be available for use.")
enableLabelWidget.setHelp("IDH_HTML_MHGC_UG_Widget_Tool_Box_Panel")

enableLineWidget = component.createBooleanSymbol("enableLineWidget", widgetMenu)
enableLineWidget.setLabel("Enable Line Widget?")
enableLineWidget.setDefaultValue(True)
enableLineWidget.setDescription("Enables the Aria Line widget.  Disabling this will remove the code for this widget and it will not be available for use.")
enableLineWidget.setHelp("IDH_HTML_MHGC_UG_Widget_Tool_Box_Panel")

enableLineGraphWidget = component.createBooleanSymbol("enableLineGraphWidget", widgetMenu)
enableLineGraphWidget.setLabel("Enable Line Graph Widget?")
enableLineGraphWidget.setDefaultValue(True)
enableLineGraphWidget.setDescription("Enables the Aria Line Graph widget.  Disabling this will remove the code for this widget and it will not be available for use.")
enableLineGraphWidget.setHelp("IDH_HTML_MHGC_UG_Widget_Tool_Box_Panel")

enableListWidget = component.createBooleanSymbol("enableListWidget", widgetMenu)
enableListWidget.setLabel("Enable List Widget?")
enableListWidget.setDefaultValue(True)
enableListWidget.setDescription("Enables the Aria List widget.  Disabling this will remove the code for this widget and it will not be available for use.")
enableListWidget.setHelp("IDH_HTML_MHGC_UG_Widget_Tool_Box_Panel")

enableListWheelWidget = component.createBooleanSymbol("enableListWheelWidget", widgetMenu)
enableListWheelWidget.setLabel("Enable List Wheel Widget?")
enableListWheelWidget.setDefaultValue(True)
enableListWheelWidget.setDescription("Enables the Aria List Wheel widget.  Disabling this will remove the code for this widget and it will not be available for use.")
enableListWheelWidget.setHelp("IDH_HTML_MHGC_UG_Widget_Tool_Box_Panel")

enablePieChartWidget = component.createBooleanSymbol("enablePieChartWidget", widgetMenu)
enablePieChartWidget.setLabel("Enable Pie Chart Widget?")
enablePieChartWidget.setDefaultValue(True)
enablePieChartWidget.setDescription("Enables the Aria Pie Chart widget.  Disabling this will remove the code for this widget and it will not be available for use.")
enablePieChartWidget.setHelp("IDH_HTML_MHGC_UG_Widget_Tool_Box_Panel")

enableProgressBarWidget = component.createBooleanSymbol("enableProgressBarWidget", widgetMenu)
enableProgressBarWidget.setLabel("Enable Progress Bar Widget?")
enableProgressBarWidget.setDefaultValue(True)
enableProgressBarWidget.setDescription("Enables the Aria Progress Bar widget.  Disabling this will remove the code for this widget and it will not be available for use.")
enableProgressBarWidget.setHelp("IDH_HTML_MHGC_UG_Widget_Tool_Box_Panel")

enableRadialMenuWidget = component.createBooleanSymbol("enableRadialMenuWidget", widgetMenu)
enableRadialMenuWidget.setLabel("Enable Radial Menu Widget?")
enableRadialMenuWidget.setDefaultValue(True)
enableRadialMenuWidget.setDescription("Enables the Aria Radial Menu widget.  Disabling this will remove the code for this widget and it will not be available for use.")
enableRadialMenuWidget.setHelp("IDH_HTML_MHGC_UG_Widget_Tool_Box_Panel")

enableRadioButtonWidget = component.createBooleanSymbol("enableRadioButtonWidget", widgetMenu)
enableRadioButtonWidget.setLabel("Enable Radio Button Widget?")
enableRadioButtonWidget.setDefaultValue(True)
enableRadioButtonWidget.setDescription("Enables the Aria Radio Button widget.  Disabling this will remove the code for this widget and it will not be available for use.")
enableRadioButtonWidget.setHelp("IDH_HTML_MHGC_UG_Widget_Tool_Box_Panel")

enableRectangleWidget = component.createBooleanSymbol("enableRectangleWidget", widgetMenu)
enableRectangleWidget.setLabel("Enable Rectangle Widget?")
enableRectangleWidget.setDefaultValue(True)
enableRectangleWidget.setDescription("Enables the Aria Rectangle widget.  Disabling this will remove the code for this widget and it will not be available for use.")
enableRectangleWidget.setHelp("IDH_HTML_MHGC_UG_Widget_Tool_Box_Panel")

enableScrollBarWidget = component.createBooleanSymbol("enableScrollBarWidget", widgetMenu)
enableScrollBarWidget.setLabel("Enable Scroll Bar Widget?")
enableScrollBarWidget.setDefaultValue(True)
enableScrollBarWidget.setDescription("Enables the Aria Scroll Bar widget.  Disabling this will remove the code for this widget and it will not be available for use.")
enableScrollBarWidget.setHelp("IDH_HTML_MHGC_UG_Widget_Tool_Box_Panel")

enableSliderWidget = component.createBooleanSymbol("enableSliderWidget", widgetMenu)
enableSliderWidget.setLabel("Enable Slider Widget?")
enableSliderWidget.setDefaultValue(True)
enableSliderWidget.setDescription("Enables the Aria Slider widget.  Disabling this will remove the code for this widget and it will not be available for use.")
enableSliderWidget.setHelp("IDH_HTML_MHGC_UG_Widget_Tool_Box_Panel")

enableTextFieldWidget = component.createBooleanSymbol("enableTextFieldWidget", widgetMenu)
enableTextFieldWidget.setLabel("Enable Text Field Widget?")
enableTextFieldWidget.setDefaultValue(True)
enableTextFieldWidget.setDescription("Enables the Aria Text Field widget.  Disabling this will remove the code for this widget and it will not be available for use.")
enableTextFieldWidget.setHelp("IDH_HTML_MHGC_UG_Widget_Tool_Box_Panel")

enableTouchTestWidget = component.createBooleanSymbol("enableTouchTestWidget", widgetMenu)
enableTouchTestWidget.setLabel("Enable Touch Test Widget?")
enableTouchTestWidget.setDefaultValue(True)
enableTouchTestWidget.setDescription("Enables the Aria Touch Test widget.  Disabling this will remove the code for this widget and it will not be available for use.")
enableTouchTestWidget.setHelp("IDH_HTML_MHGC_UG_Widget_Tool_Box_Panel")

enableWindowWidget = component.createBooleanSymbol("enableWindowWidget", widgetMenu)
enableWindowWidget.setLabel("Enable Window Widget?")
enableWindowWidget.setDefaultValue(True)
enableWindowWidget.setDescription("Enables the Aria Window widget.  Disabling this will remove the code for this widget and it will not be available for use.")
enableWindowWidget.setHelp("IDH_HTML_MHGC_UG_Widget_Tool_Box_Panel")

global onAriaHeapChanged
def onAriaHeapChanged(sym, event):
	Database.setSymbolValue("core", "XC32_HEAP_SIZE", sym.getValue(), 0)

ariaHeap = component.createIntegerSymbol("ariaHeap", None)
ariaHeap.setLabel("Requested Heap")
ariaHeap.setVisible(False)
ariaHeap.setDependencies(onAriaHeapChanged, ["ariaHeap"])
ariaHeap.setDefaultValue(32768)


