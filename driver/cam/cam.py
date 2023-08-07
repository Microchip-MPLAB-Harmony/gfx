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
    sensor =  component.createKeyValueSetSymbol("CamType", None)
    sensor.setLabel("Camera Sensor")
    sensor.addKey("OV5640" , "0", "Support for OV5640 camera sensor.")
    sensor.addKey("IMX219" , "1", "Support for IMX219 camera sensor.")
    sensor.addKey("Custom" , "2", "Support for Unknown camera sensor.")
    sensor.setDisplayMode("Key")
    sensor.setOutputMode("Key")
    sensor.setDefaultValue(0)
    sensor.setDescription("Add support for the camer sensor being used.")
    sensor.setDependencies(checkSensorType, ["CamType"])

    
    DRV_CAM_H = component.createFileSymbol("DRV_CAM_H", None)
    DRV_CAM_H.setDestPath("driver/cam/")
    DRV_CAM_H.setOutputName("drv_cam.h")
    DRV_CAM_H.setProjectPath("config/" + Variables.get("__CONFIGURATION_NAME") + "/driver/cam")
    DRV_CAM_H.setType("HEADER")
    DRV_CAM_H.setMarkup(False)
    DRV_CAM_H.setSourcePath("inc/drv_cam.h")
    
    DRV_CAM_C = component.createFileSymbol("DRV_CAM_C", None)
    DRV_CAM_C.setDestPath("driver/cam/")
    DRV_CAM_C.setOutputName("drv_cam.c")
    DRV_CAM_C.setProjectPath("config/" + Variables.get("__CONFIGURATION_NAME") + "/driver/cam")
    DRV_CAM_C.setType("SOURCE")
    DRV_CAM_C.setMarkup(False)
    DRV_CAM_C.setSourcePath("src/drv_cam.c")
    
    DRV_CAMERA_REGS_H = component.createFileSymbol("DRV_CAMERA_REGS_H", None)
    DRV_CAMERA_REGS_H.setDestPath("driver/cam/")
    DRV_CAMERA_REGS_H.setOutputName("drv_ov5640_regs.h")
    DRV_CAMERA_REGS_H.setProjectPath("config/" + Variables.get("__CONFIGURATION_NAME") + "/driver/cam")
    DRV_CAMERA_REGS_H.setType("HEADER")
    DRV_CAMERA_REGS_H.setMarkup(False)
    DRV_CAMERA_REGS_H.setSourcePath("inc/drv_ov5640_regs.h")
    
    
    DRV_CAMERA_C = component.createFileSymbol("DRV_CAMERA_C", None)
    DRV_CAMERA_C.setDestPath("driver/cam/")
    DRV_CAMERA_C.setOutputName("drv_ov5640.c")
    DRV_CAMERA_C.setProjectPath("config/" + Variables.get("__CONFIGURATION_NAME") + "/driver/cam")
    DRV_CAMERA_C.setType("SOURCE")
    DRV_CAMERA_C.setMarkup(False)
    DRV_CAMERA_C.setSourcePath("src/drv_ov5640.c")
    
    
def checkSensorType(sensorType, event):
    if (event["value"] == 0):
        print("Copying files for OV5640 camera sensor")
        sensor_symbol_h = sensorType.getComponent().getSymbolByID("DRV_CAMERA_REGS_H")
        sensor_symbol_h.setOutputName("drv_ov5640_regs.h")    
        sensor_symbol_h.setSourcePath("inc/drv_ov5640_regs.h")
        sensor_symbol_c = sensorType.getComponent().getSymbolByID("DRV_CAMERA_C")
        sensor_symbol_c.setOutputName("drv_ov5640.c")    
        sensor_symbol_c.setSourcePath("src/drv_ov5640.c")
    if (event["value"] == 1):
        print("Copying files for IMX219 camera sensor")
        sensor_symbol_h = sensorType.getComponent().getSymbolByID("DRV_CAMERA_REGS_H")
        sensor_symbol_h.setOutputName("drv_imx219_regs.h")    
        sensor_symbol_h.setSourcePath("inc/drv_imx219_regs.h")
        sensor_symbol_c = sensorType.getComponent().getSymbolByID("DRV_CAMERA_C")
        sensor_symbol_c.setOutputName("drv_imx219.c")    
        sensor_symbol_c.setSourcePath("src/drv_imx219.c")
    else:
        print("Copying files for Custom camera sensor")
        sensor_symbol_h = sensorType.getComponent().getSymbolByID("DRV_CAMERA_REGS_H")
        sensor_symbol_h.setOutputName("drv_custom_cam_regs.h")    
        sensor_symbol_h.setSourcePath("inc/drv_custom_cam_regs.h")
        sensor_symbol_c = sensorType.getComponent().getSymbolByID("DRV_CAMERA_C")
        sensor_symbol_c.setOutputName("drv_custom_cam.c")    
        sensor_symbol_c.setSourcePath("src/drv_custom_cam.c")
