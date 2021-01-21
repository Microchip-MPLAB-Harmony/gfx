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

############ LCC + TOUCH I2C CONFIG ######################################################
bsp_pic32mzef_cu_ActivateList_LCC = ["le_gfx_driver_lcc", "i2c2", "drv_i2c", "drv_i2c0", "core_timer", "sys_time", "ebi"]
bsp_pic32mzef_cu_AutoConnectList_LCC = [["gfx_legato", "gfx_driver", "le_gfx_driver_lcc", "gfx_driver_lcc"],
										["le_gfx_driver_lcc", "Graphics Display", "gfx_disp_pdatm4301b_480x272", "gfx_display"],
										["drv_i2c_0", "drv_i2c_I2C_dependency", "i2c2", "I2C2_I2C"],
										["gfx_maxtouch_controller", "i2c", "drv_i2c_0", "drv_i2c"],
										["sys_time", "sys_time_TMR_dependency", "core_timer", "CORE_TIMER_TMR"],
										["le_gfx_driver_lcc", "EBI_CS", "ebi", "ebi_cs0"]]
bsp_pic32mzef_cu_PinConfig_LCC = [{"pin": 104, "name": "BSP_MAXTOUCH_CHG", "type": "GPIO", "direction": "In", "latch": "", "abcd": ""}, #RD0
				{"pin": 53, "name": "GFX_DISP_INTF_PIN_DE", "type": "GPIO", "direction": "Out", "latch": "Low", "abcd": ""}, #RK3
				{"pin": 51, "name": "GFX_DISP_INTF_PIN_HSYNC", "type": "GPIO", "direction": "Out", "latch": "Low", "abcd": ""}, #RK1
				{"pin": 52, "name": "GFX_DISP_INTF_PIN_VSYNC", "type": "GPIO", "direction": "Out", "latch": "Low", "abcd": ""}, #RK2
				{"pin": 37, "name": "GFX_DISP_INTF_PIN_BACKLIGHT", "type": "GPIO", "direction": "Out", "latch": "High", "abcd": ""}, #RB6
				{"pin": 29, "name": "GFX_DISP_INTF_PIN_RESET", "type": "GPIO", "direction": "Out", "latch": "High", "abcd": ""}] #RJ14
##################################################################################

############ SSD1963 + TOUCH I2C CONFIG ######################################################
bsp_pic32mzef_cu_ActivateList_SSD = ["le_gfx_driver_ssd1963", "le_gfx_intf_parallel_ebi", "i2c2", "drv_i2c", "drv_i2c0", "core_timer", "sys_time", "ebi"]
bsp_pic32mzef_cu_AutoConnectList_SSD = [["gfx_legato", "gfx_driver", "le_gfx_driver_ssd1963", "gfx_driver_ssd1963"],
					["le_gfx_driver_ssd1963", "Graphics Display", "gfx_disp_pdatm4301b_480x272", "gfx_display"],
					["drv_i2c_0", "drv_i2c_I2C_dependency", "i2c2", "I2C2_I2C"],
					["gfx_maxtouch_controller", "i2c", "drv_i2c_0", "drv_i2c"],
					["le_gfx_driver_ssd1963", "Display Interface", "le_gfx_intf_parallel_ebi", "le_gfx_intf_parallel_ebi"],
					["le_gfx_intf_parallel_ebi", "EBI_CS", "ebi", "ebi_cs0"],
					["sys_time", "sys_time_TMR_dependency", "core_timer", "CORE_TIMER_TMR"]]
bsp_pic32mzef_cu_PinConfig_SSD = [{"pin": 104, "name": "BSP_MAXTOUCH_CHG", "type": "GPIO", "direction": "In", "latch": "", "abcd": ""}, #RD0
				{"pin": 53, "name": "GFX_DISP_INTF_PIN_RD", "type": "GPIO", "direction": "Out", "latch": "High", "abcd": ""}, #RK3
				{"pin": 51, "name": "GFX_DISP_INTF_PIN_RSDC", "type": "GPIO", "direction": "Out", "latch": "High", "abcd": ""}, #RK1
				{"pin": 52, "name": "GFX_DISP_INTF_PIN_CS", "type": "GPIO", "direction": "Out", "latch": "High", "abcd": ""}, #RK2
				{"pin": 29, "name": "GFX_DISP_INTF_PIN_RESET", "type": "GPIO", "direction": "Out", "latch": "Low", "abcd": ""}, #RJ14
				{"pin": 12, "name": "GFX_DISP_INTF_PIN_WR", "type": "GPIO", "direction": "Out", "latch": "High", "abcd": ""}] #RC3
##########################################################################################

def bsp_pic32mzef_cu_EventHandler(event):
	global pinConfigureFxn
	if (event == "configure"):
		#Override default pin configur function w/ PIC32M specific one
		pinConfigureFxn = configurePinsPIC32M
		try:
			### Slow down I2C2 to 10kHz
			Database.setSymbolValue("i2c2", "I2C_CLOCK_SPEED", 10000L, 1)
		except:
			return

bsp_pic32mzef_cu_DisplayInterfaceList = ["LCC", "SSD1963"]

bsp_pic32mzef_cu_obj_LCC = bspSupportObj(bsp_pic32mzef_cu_PinConfig_LCC,
										bsp_pic32mzef_cu_ActivateList_LCC,
										None,
										bsp_pic32mzef_cu_AutoConnectList_LCC,
										bsp_pic32mzef_cu_EventHandler)

bsp_pic32mzef_cu_obj_SSD = bspSupportObj(bsp_pic32mzef_cu_PinConfig_SSD,
										bsp_pic32mzef_cu_ActivateList_SSD,
										None,
										bsp_pic32mzef_cu_AutoConnectList_SSD,
										bsp_pic32mzef_cu_EventHandler)

addDisplayIntfSupport("BSP_PIC32MZ_EF_Curiosity_2.0", bsp_pic32mzef_cu_DisplayInterfaceList)
addBSPSupport("BSP_PIC32MZ_EF_Curiosity_2.0", "LCC", bsp_pic32mzef_cu_obj_LCC)
addBSPSupport("BSP_PIC32MZ_EF_Curiosity_2.0", "SSD1963", bsp_pic32mzef_cu_obj_SSD)

