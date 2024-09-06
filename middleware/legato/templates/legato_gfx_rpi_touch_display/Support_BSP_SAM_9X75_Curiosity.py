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

############### XLCDC CONFIG ##################################################
sam_9x75_cu_ActivateList = [
    "gfx_bridge_dsi_plib",
    "le_gfx_driver_xlcdc",
    "flexcom6",
    "drv_i2c",
    "drv_i2c0",
    "rtt",
    "sys_time",
    "le_gfx_gfx2d",
    "plib_gfx2d",
]
sam_9x75_cu_DeactivateList = ["pit"]
sam_9x75_cu_AutoConnectList = [
    # Order is not trivial, make sure to connect display to bridge before connecting bridge to xlcdc
    ["gfx_bridge_dsi_plib", "gfx_dsi_display_dep", "gfx_display_comp_rasppi", "gfx_dsi_display_cap"],
    ["le_gfx_driver_xlcdc", "gfx_dpi_bridge_dep", "gfx_bridge_dsi_plib", "gfx_dpi_bridge_cap"],
    ["gfx_legato", "gfx_driver", "le_gfx_driver_xlcdc", "le_gfx_xlcdc_cap"],
    ["drv_i2c_0", "drv_i2c_I2C_dependency", "flexcom6", "FLEXCOM6_I2C"],
    ["gfx_display_comp_rasppi", "gfx_dsi_i2c_dep", "drv_i2c_0", "drv_i2c"],
    ["sys_time", "sys_time_TMR_dependency", "rtt", "RTT_TMR"],
    ["le_gfx_gfx2d", "GFX2D", "plib_gfx2d", "GFX2D"],
    ["gfx_legato", "gpu_driver", "le_gfx_gfx2d", "LE GPU Driver"],
]

sam_9x75_cu_PinConfig = [
    {"pin": 200, "name": "DSI_DISPLAY_SDA", "type": "FLEXCOM6_IO0", "direction": "", "latch": "", "abcd": "C"},
    {"pin": 198, "name": "DSI_DISPLAY_SCL", "type": "FLEXCOM6_IO1", "direction": "", "latch": "", "abcd": "C"},
]

sam_9x75_cu_DisplayInterfaceList = ["XLCDC"]


def sam_9x75_cu_EventHandler(event):
    if event == "configure":
        pass

def sam_9x75_cu_AddFile(component):
    pass

##################################################################################

sam_9x75_cu_bspSupport = bspSupportObj(
    sam_9x75_cu_PinConfig,
    sam_9x75_cu_ActivateList,
    sam_9x75_cu_DeactivateList,
    sam_9x75_cu_AutoConnectList,
    sam_9x75_cu_EventHandler,
)

addBSPSupport("BSP_SAM_9X75_Curiosity", "XLCDC", sam_9x75_cu_bspSupport)
addDisplayIntfSupport("BSP_SAM_9X75_Curiosity", sam_9x75_cu_DisplayInterfaceList)
addFileCallback.update({"BSP_SAM_9X75_Curiosity": sam_9x75_cu_AddFile})
