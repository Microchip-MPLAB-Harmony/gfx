# coding: utf-8
##############################################################################
# Copyright (C) 2019 Microchip Technology Inc. and its subsidiaries.
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

import os.path


def loadModule():
    # load 2DGPU module
    if ("PIC32MZ" in str(Variables.get("__PROCESSOR")) and "DA" in str(Variables.get("__PROCESSOR"))):
        print("LE 2DGPU module loaded to support " + str(Variables.get("__PROCESSOR")))
        cntlComponent = Module.CreateComponent("le_gfx_driver_2dgpu", "LE 2DGPU", "/Graphics/Processor", "2dgpu.py")
        cntlComponent.setDisplayType("LE 2D-GPU Graphics Processor")
        cntlComponent.addCapability("gfx_driver_2dgpu", "LE GPU Driver", False)
        #cntlComponent.addDependency("legato_library", "Legato", True, True)
    else:
        print("LE 2DGPU module not loaded.  No support for " + str(Variables.get("__PROCESSOR")))


