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


def loadModule():
    # load GFX2D peripheral
    if any(
        device in Variables.get("__PROCESSOR")
        for device in [
            "9X60",
            "SAM9X75",
            "SAM9X72",
        ]
    ):
        print("LE GFX2D module loaded to support " + str(Variables.get("__PROCESSOR")))
        print("GFX: create component: Peripheral GFX2D (ID = 44061)")
        periphComponent = Module.CreateComponent("plib_gfx2d", "GFX2D", "/Peripherals/GFX2D/", "peripheral/plib_gfx2d.py")
        periphComponent.setDisplayType("Peripheral Library")
        periphComponent.addCapability("GFX2D", "GFX2D")
        cntlComponent = Module.CreateComponent("le_gfx_gfx2d", "LE GFX2D", "/Graphics/Processor", "gfx2d.py")
        cntlComponent.setDisplayType("Graphics Processor")
        cntlComponent.addCapability("LE GPU Driver", "LE GPU Driver", False)
        cntlComponent.addDependency("GFX2D", "GFX2D", False, True)
    else:
        print("LE GFX2D module not loaded.  No support for " + str(Variables.get("__PROCESSOR")))
