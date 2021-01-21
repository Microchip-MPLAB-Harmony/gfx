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
	projectPath = "config/" + Variables.get("__CONFIGURATION_NAME") + "/gfx/driver/controller/generic"
	
	# some configuration values
	UseStaticBuffer = comp.createBooleanSymbol("UseStaticBuffer", None)
	UseStaticBuffer.setLabel("Use Static Frame Buffer?")
	UseStaticBuffer.setDescription("Configures the driver to use a static flash-based frame buffer.")
	UseStaticBuffer.setDefaultValue(False)

	# the HAL uses these values to populate the driver initialization table
	DriverInfoFunction = comp.createStringSymbol("DriverInfoFunction", None)
	DriverInfoFunction.setLabel("Driver Info Function Name")
	DriverInfoFunction.setReadOnly(True)
	DriverInfoFunction.setDefaultValue("driverGenericInfoGet")
	DriverInfoFunction.setVisible(False)
	
	DriverInitFunction = comp.createStringSymbol("DriverInitFunction", None)
	DriverInitFunction.setLabel("Driver Init Function Name")
	DriverInitFunction.setReadOnly(True)
	DriverInitFunction.setDefaultValue("driverGenericContextInitialize")
	DriverInitFunction.setVisible(False)
	
	# driver code template files
	DRIVER_C = comp.createFileSymbol("DRIVER_C", None)
	DRIVER_C.setSourcePath("drv_gfx_generic.c.ftl")
	DRIVER_C.setDestPath("gfx/driver/controller/generic/")
	DRIVER_C.setOutputName("drv_gfx_generic.c")
	DRIVER_C.setProjectPath(projectPath)
	DRIVER_C.setType("SOURCE")
	DRIVER_C.setMarkup(True)
	
	DRIVER_H = comp.createFileSymbol("DRIVER_H", None)
	DRIVER_H.setSourcePath("drv_gfx_generic.h.ftl")
	DRIVER_H.setDestPath("gfx/driver/controller/generic")
	DRIVER_H.setOutputName("drv_gfx_generic.h")
	DRIVER_H.setProjectPath(projectPath)
	DRIVER_H.setType("HEADER")
	DRIVER_H.setMarkup(True)