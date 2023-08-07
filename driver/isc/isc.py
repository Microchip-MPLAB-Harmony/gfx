# coding: utf-8
##############################################################################
# Copyright (C) 2021 Microchip Technology Inc. and its subsidiaries.
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

def instantiateComponent(component):
    Mipi = component.createBooleanSymbol("Mipi", None)
    Mipi.setLabel("MIPI Mode")
    Mipi.setDefaultValue(False)
    Mipi.setDescription("<html>ISC interfaces with camera sensor through MIPI CSI DPHY. </html>")
    Mipi.setDependencies(OnMipiEnabled, ["Mipi"])

    ContinuousCapture = component.createBooleanSymbol("ContinuousCapture", None)
    ContinuousCapture.setLabel("Continuous Capture")
    ContinuousCapture.setDefaultValue(False)
    ContinuousCapture.setDescription("<html>Enables ISC to continuously update framebuffer with captured image. </html>")

### DMA Configuration Menu
    DMAConfigurationMenu = component.createMenuSymbol("DMAConfigurationMenu", None)
    DMAConfigurationMenu.setLabel("DMA Configuration")

    DMA_CTRLMenu = component.createMenuSymbol("DMA_CTRLMenu", DMAConfigurationMenu)
    DMA_CTRLMenu.setLabel("DMA Control Register")

    DescriptorEnable = component.createBooleanSymbol("DescriptorEnable", DMA_CTRLMenu)
    DescriptorEnable.setLabel("Descriptor Enable")
    DescriptorEnable.setDescription("Enables Descriptor in DCTRL register")
    DescriptorEnable.setDefaultValue(True)

    DescriptorViewPacked = component.createBooleanSymbol("DescriptorViewPacked", DMA_CTRLMenu)
    DescriptorViewPacked.setLabel("Descriptor View")
    DescriptorViewPacked.setDescription("Set Descriptor View to packed")
    DescriptorViewPacked.setDefaultValue(True)

    CMBSIZE = component.createKeyValueSetSymbol("CMBSIZE", DMAConfigurationMenu)
    CMBSIZE.setLabel("DMA Memory Burst Size C channel")
    CMBSIZE.setOutputMode("Value")
    CMBSIZE.setDescription("DMA Memory Burst Size C channel")
    CMBSIZE.addKey("SINGLE", str(0), "SINGLE")
    CMBSIZE.addKey("BEATS4", str(1), "BEATS4")
    CMBSIZE.addKey("BEATS8", str(2), "BEATS8")
    CMBSIZE.addKey("BEATS16", str(3), "BEATS16")
    CMBSIZE.setDefaultValue(3)
    if ("A7G5" in str(Variables.get("__PROCESSOR"))):
        CMBSIZE.addKey("BEATS32", str(4), "BEATS32")

    YMBSIZE = component.createKeyValueSetSymbol("YMBSIZE", DMAConfigurationMenu)
    YMBSIZE.setLabel("DMA Memory Burst Size Y channel")
    YMBSIZE.setOutputMode("Value")
    YMBSIZE.setDescription("DMA Memory Burst Size Y channel")
    YMBSIZE.addKey("SINGLE", str(0), "SINGLE")
    YMBSIZE.addKey("BEATS4", str(1), "BEATS4")
    YMBSIZE.addKey("BEATS8", str(2), "BEATS8")
    YMBSIZE.addKey("BEATS16", str(3), "BEATS16")
    YMBSIZE.setDefaultValue(3)
    if ("A7G5" in str(Variables.get("__PROCESSOR"))):
        YMBSIZE.addKey("BEATS32", str(4), "BEATS32")

    VPol = component.createBooleanSymbol("VPol", None)
    VPol.setLabel("Vertical Synchronization Polarity")
    VPol.setDefaultValue(False)
    VPol.setDescription("<html>Valid pixels are sampled when VSYNC is deasserted and HSYNC is asserted. </html>")

    BPS = component.createKeyValueSetSymbol("BPS", None)
    BPS.setLabel("Bits Per Sample")
    BPS.setOutputMode("Value")
    BPS.setDescription("Bits Per Sample")
    BPS.addKey("TWELVE", str(0), "TWELVE")
    BPS.addKey("ELEVEN", str(1), "ELEVEN")
    BPS.addKey("TEN", str(2), "TEN")
    BPS.addKey("NINE", str(3), "NINE")
    BPS.addKey("EIGHT", str(4), "EIGHT")
    BPS.setDefaultValue(4)
    if ("A7G5" in str(Variables.get("__PROCESSOR"))):
        BPS.addKey("FORTY", str(5), "FORTY")

    GamCorrect = component.createBooleanSymbol("GamCorrect", None)
    GamCorrect.setLabel("Gamma Correction")
    GamCorrect.setDefaultValue(False)
    GamCorrect.setDescription("<html>Apply Gamma Correction. </html>")

    SetCroppingArea = component.createBooleanSymbol("SetCroppingArea", None)
    SetCroppingArea.setLabel("Set Cropping Area")
    SetCroppingArea.setDefaultValue(True)
    SetCroppingArea.setDescription("<html> Enable Cropping Area.</html>")
    SetCroppingArea.setDependencies(onSetCroppingEnabled, ["SetCroppingArea"])

    RowMax = component.createIntegerSymbol("RowMax", SetCroppingArea)
    RowMax.setLabel("Row Maximum Limit")
    RowMax.setDescription("<html>Row Maximum Limit.</html>")
    RowMax.setDefaultValue(480)
    RowMax.setVisible(True)

    ColMax = component.createIntegerSymbol("ColMax", SetCroppingArea)
    ColMax.setLabel("Column Maximum Limit")
    ColMax.setDescription("<html>Column Maximum Limit.</html>")
    ColMax.setDefaultValue(640)
    ColMax.setVisible(True)

    Pattern = component.createKeyValueSetSymbol("Pattern", None)
    Pattern.setLabel("Color Pattern")
    Pattern.setOutputMode("Value")
    Pattern.setDescription("Pixel Pattern for WB and CFA")
    Pattern.addKey("GRGR", str(0), "GRGR")
    Pattern.addKey("RGRG", str(1), "RGRG")
    Pattern.addKey("GBGB", str(2), "GBGB")
    Pattern.addKey("BGBG", str(3), "BGBG")
    Pattern.setDefaultValue(1)

    CBC = component.createBooleanSymbol("CBC", None)
    CBC.setLabel("Contrast And Brightness Control")
    CBC.setDefaultValue(False)
    CBC.setDescription("<html>Enables/disable contrast and brightness control. </html>")

    PLIB_ISC_C = component.createFileSymbol("PLIB_ISC_C", None)
    PLIB_ISC_C.setDestPath("peripheral/isc/")
    PLIB_ISC_C.setOutputName("plib_isc.c")
    PLIB_ISC_C.setProjectPath("config/" + Variables.get("__CONFIGURATION_NAME") + "/peripheral/isc")
    PLIB_ISC_C.setType("SOURCE")
    PLIB_ISC_C.setMarkup(True)
    PLIB_ISC_C.setSourcePath("src/plib_isc.c.ftl")
    
    PLIB_ISC_H = component.createFileSymbol("PLIB_ISC_H", None)
    PLIB_ISC_H.setDestPath("peripheral/isc/")
    PLIB_ISC_H.setOutputName("plib_isc.h")
    PLIB_ISC_H.setProjectPath("config/" + Variables.get("__CONFIGURATION_NAME") + "/peripheral/isc")
    PLIB_ISC_H.setType("HEADER")
    PLIB_ISC_H.setMarkup(True)
    PLIB_ISC_H.setSourcePath("inc/plib_isc.h.ftl")
    
    DRV_ISC_H = component.createFileSymbol("DRV_ISC_H", None)
    DRV_ISC_H.setDestPath("driver/isc/")
    DRV_ISC_H.setOutputName("drv_isc.h")
    DRV_ISC_H.setProjectPath("config/" + Variables.get("__CONFIGURATION_NAME") + "/driver/isc")
    DRV_ISC_H.setType("HEADER")
    DRV_ISC_H.setMarkup(True)
    DRV_ISC_H.setSourcePath("inc/drv_isc.h.ftl")
    
    DRV_ISC_C = component.createFileSymbol("DRV_ISC_C", None)
    DRV_ISC_C.setDestPath("driver/isc/")
    DRV_ISC_C.setOutputName("drv_isc.c")
    DRV_ISC_C.setProjectPath("config/" + Variables.get("__CONFIGURATION_NAME") + "/driver/isc")
    DRV_ISC_C.setType("SOURCE")
    DRV_ISC_C.setMarkup(True)
    DRV_ISC_C.setSourcePath("src/drv_isc.c.ftl")
    
    SYS_DEFINITIONS_H = component.createFileSymbol("SYS_DEFINITIONS_H", None)
    SYS_DEFINITIONS_H.setType("STRING")
    SYS_DEFINITIONS_H.setOutputName("core.LIST_SYSTEM_DEFINITIONS_H_INCLUDES")
    SYS_DEFINITIONS_H.setSourcePath("templates/definitions.h.ftl")
    SYS_DEFINITIONS_H.setMarkup(True)

    SYS_INIT_C = component.createFileSymbol("SYS_INIT_C", None)
    SYS_INIT_C.setType("STRING")
    SYS_INIT_C.setOutputName("core.LIST_SYSTEM_INIT_C_SYS_INITIALIZE_DRIVERS")
    SYS_INIT_C.setSourcePath("templates/init.c.ftl")
    SYS_INIT_C.setMarkup(True)

    PLIB_CSI2DC_C = component.createFileSymbol("PLIB_CSI2DC_C", None)

    PLIB_CSI2DC_H = component.createFileSymbol("PLIB_CSI2DC_H", None)

    PLIB_MIPI_CSI_C = component.createFileSymbol("PLIB_MIPI_CSI_C", None)

    PLIB_MIPI_CSI_H = component.createFileSymbol("PLIB_MIPI_CSI_H", None)

    PLIB_MIPI_CSI_DPHY_C = component.createFileSymbol("PLIB_MIPI_CSI_DPHY_C", None)

    PLIB_MIPI_CSI_DPHY_H = component.createFileSymbol("PLIB_MIPI_CSI_DPHY_H", None)



def OnMipiEnabled(mipiEnabled, event):
    if (mipiEnabled.getComponent().getSymbolValue("Mipi") == True):
        print("Copying MIPI related files")
        csi2dc_c = mipiEnabled.getComponent().getSymbolByID("PLIB_CSI2DC_C")
        csi2dc_c.setDestPath("peripheral/isc/")
        csi2dc_c.setOutputName("plib_csi2dc.c")
        csi2dc_c.setProjectPath("config/" + Variables.get("__CONFIGURATION_NAME") + "/peripheral/isc")
        csi2dc_c.setType("SOURCE")
        csi2dc_c.setMarkup(False)
        csi2dc_c.setSourcePath("src/plib_csi2dc.c")
        csi2dc_h = mipiEnabled.getComponent().getSymbolByID("PLIB_CSI2DC_H")
        csi2dc_h.setDestPath("peripheral/isc/")
        csi2dc_h.setOutputName("plib_csi2dc.h")
        csi2dc_h.setProjectPath("config/" + Variables.get("__CONFIGURATION_NAME") + "/peripheral/isc")
        csi2dc_h.setType("HEADER")
        csi2dc_h.setMarkup(False)
        csi2dc_h.setSourcePath("inc/plib_csi2dc.h")
        mipi_csi_c = mipiEnabled.getComponent().getSymbolByID("PLIB_MIPI_CSI_C")
        mipi_csi_c.setDestPath("peripheral/isc/")
        mipi_csi_c.setOutputName("plib_mipi_csi.c")
        mipi_csi_c.setProjectPath("config/" + Variables.get("__CONFIGURATION_NAME") + "/peripheral/isc")
        mipi_csi_c.setType("SOURCE")
        mipi_csi_c.setMarkup(False)
        mipi_csi_c.setSourcePath("src/plib_mipi_csi.c")
        mipi_csi_h = mipiEnabled.getComponent().getSymbolByID("PLIB_MIPI_CSI_H")
        mipi_csi_h.setDestPath("peripheral/isc/")
        mipi_csi_h.setOutputName("plib_mipi_csi.h")
        mipi_csi_h.setProjectPath("config/" + Variables.get("__CONFIGURATION_NAME") + "/peripheral/isc")
        mipi_csi_h.setType("HEADER")
        mipi_csi_h.setMarkup(False)
        mipi_csi_h.setSourcePath("inc/plib_mipi_csi.h")
        csidphy_c = mipiEnabled.getComponent().getSymbolByID("PLIB_MIPI_CSI_DPHY_C")
        csidphy_c.setDestPath("peripheral/isc/")
        csidphy_c.setOutputName("plib_mipi_csi_dphy.c")
        csidphy_c.setProjectPath("config/" + Variables.get("__CONFIGURATION_NAME") + "/peripheral/isc")
        csidphy_c.setType("SOURCE")
        csidphy_c.setMarkup(False)
        csidphy_c.setSourcePath("src/plib_mipi_csi_dphy.c")
        csidphy_h = mipiEnabled.getComponent().getSymbolByID("PLIB_MIPI_CSI_DPHY_H")
        csidphy_h.setDestPath("peripheral/isc/")
        csidphy_h.setOutputName("plib_mipi_csi_dphy.h")
        csidphy_h.setProjectPath("config/" + Variables.get("__CONFIGURATION_NAME") + "/peripheral/isc")
        csidphy_h.setType("HEADER")
        csidphy_h.setMarkup(False)
        csidphy_h.setSourcePath("inc/plib_mipi_csi_dphy.h")
        

def onSetCroppingEnabled(source, event):
    source.getComponent().getSymbolByID("RowMax").setVisible(event["value"])
    source.getComponent().getSymbolByID("ColMax").setVisible(event["value"])
