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
    PLIB_ISC_C = component.createFileSymbol("PLIB_ISC_C", None)
    PLIB_ISC_C.setDestPath("peripheral/isc/")
    PLIB_ISC_C.setOutputName("plib_isc.c")
    PLIB_ISC_C.setProjectPath("config/" + Variables.get("__CONFIGURATION_NAME") + "/peripheral/isc")
    PLIB_ISC_C.setType("SOURCE")
    PLIB_ISC_C.setMarkup(False)
    PLIB_ISC_C.setSourcePath("src/plib_isc.c")
    
    PLIB_ISC_H = component.createFileSymbol("PLIB_ISC_H", None)
    PLIB_ISC_H.setDestPath("peripheral/isc/")
    PLIB_ISC_H.setOutputName("plib_isc.h")
    PLIB_ISC_H.setProjectPath("config/" + Variables.get("__CONFIGURATION_NAME") + "/peripheral/isc")
    PLIB_ISC_H.setType("HEADER")
    PLIB_ISC_H.setMarkup(False)
    PLIB_ISC_H.setSourcePath("inc/plib_isc.h")
    
    DRV_ISC_H = component.createFileSymbol("DRV_ISC_H", None)
    DRV_ISC_H.setDestPath("driver/isc/")
    DRV_ISC_H.setOutputName("drv_isc.h")
    DRV_ISC_H.setProjectPath("config/" + Variables.get("__CONFIGURATION_NAME") + "/driver/isc")
    DRV_ISC_H.setType("HEADER")
    DRV_ISC_H.setMarkup(False)
    DRV_ISC_H.setSourcePath("inc/drv_isc.h")
    
    DRV_ISC_C = component.createFileSymbol("DRV_ISC_C", None)
    DRV_ISC_C.setDestPath("driver/isc/")
    DRV_ISC_C.setOutputName("drv_isc.c")
    DRV_ISC_C.setProjectPath("config/" + Variables.get("__CONFIGURATION_NAME") + "/driver/isc")
    DRV_ISC_C.setType("SOURCE")
    DRV_ISC_C.setMarkup(False)
    DRV_ISC_C.setSourcePath("src/drv_isc.c")
    
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
    
