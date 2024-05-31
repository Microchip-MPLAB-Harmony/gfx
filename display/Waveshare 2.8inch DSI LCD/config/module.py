# coding: utf-8
##############################################################################
# Copyright (C) 2023 Microchip Technology Inc. and its subsidiaries.
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
    if any(device in Variables.get("__PROCESSOR") for device in ["SAM9X75", "SAMA7D"]):
        component = Module.CreateComponent(
            "gfx_display_comp_waveshare_2_8in_dsi",
            "Waveshare 2.8inch DSI LCD",
            "/Graphics/Displays",
            "config/wavehsare.py",
        )
        component.setDisplayType("480x640 MIPI DSI Touch Display")
        component.addCapability("gfx_dsi_display_cap", "DSI Display", False)
        component.addDependency("gfx_dsi_i2c_dep", "DRV_I2C", False, True)
        component.addDependency("gfx_sys_time_dep", "SYS_TIME", True, True)
        component.addDependency("gfx_sys_input_dep", "Input System Service", True, True)
    else:
        print("Waveshare 2.8inch DSI LCD module not loaded.  No DSI support on " + str(Variables.get("__PROCESSOR")))
