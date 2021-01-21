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

def loadModule():	
    if ("A5D2" in str(Variables.get("__PROCESSOR")) or "9X60" in str(Variables.get("__PROCESSOR"))):
        print("LCDC module loaded to support " + str(Variables.get("__PROCESSOR")))
        cntlComponent = Module.CreateComponent("le_gfx_lcdc", "LE LCDC Driver", "/Graphics/Driver", "config/lcdc.py")
        cntlComponent.setDisplayType("LE LCDC Display Driver")
        cntlComponent.addCapability("le_gfx_driver_lcdc", "LE Display Driver", False)
        cntlComponent.addDependency("Graphics Display", "Graphics Display", False)
        cntlComponent.addDependency("LCDC", "LCDC", False, True)
    else:
        print("LE LCDC module not loaded.  No support for " + str(Variables.get("__PROCESSOR")))
