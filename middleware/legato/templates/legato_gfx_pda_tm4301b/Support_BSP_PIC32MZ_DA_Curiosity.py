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

############ GLCD + TOUCH I2C CONFIG ######################################################
mzda_cu_activateList = ["le_gfx_driver_glcd", "gfx_hal_le", "i2c2", "drv_i2c", "drv_i2c0", "core_timer", "sys_time", "ddr", "le_gfx_driver_2dgpu"]
mzda_cu_connectList = [["gfx_legato", "gfx_driver", "le_gfx_driver_glcd", "gfx_driver_glcd"],
					["le_gfx_driver_glcd", "Graphics Display", "gfx_disp_pdatm4301b_480x272", "gfx_display"],
					["drv_i2c_0", "drv_i2c_I2C_dependency", "i2c2", "I2C2_I2C"],
					["gfx_maxtouch_controller", "i2c", "drv_i2c_0", "drv_i2c"],
					["sys_time", "sys_time_TMR_dependency", "core_timer", "CORE_TIMER_TMR"],
					["gfx_legato", "gpu_driver", "le_gfx_driver_2dgpu", "gfx_driver_2dgpu"]]
mzda_cu_pinConfig = [{"pin": 42, "name": "BSP_MAXTOUCH_CHG", "type": "GPIO", "direction": "In", "latch": "", "abcd": ""}, #RH14
				{"pin": 79, "name": "TM4301B_BACKLIGHT", "type": "GPIO", "direction": "Out", "latch": "High", "abcd": ""}, #RD0
				{"pin": 149, "name": "GLCD_HSYNC", "type": "HSYNC", "direction": "", "latch": "", "abcd": "A"}, #RJ5
				{"pin": 150, "name": "GLCD_VSYNC", "type": "VSYNC", "direction": "", "latch": "", "abcd": "A"}, #RJ4
				{"pin": 148, "name": "GLCD_PCK", "type": "GCLK", "direction": "", "latch": "", "abcd": "A"}, #RJ6
				{"pin": 144, "name": "GLCD_D0", "type": "GD0", "direction": "", "latch": "", "abcd": "A"}, #RJ13
				{"pin": 127, "name": "GLCD_D1", "type": "GD1", "direction": "", "latch": "", "abcd": "A"}, #RA4
				{"pin": 76, "name": "GLCD_D2", "type": "GD2", "direction": "", "latch": "", "abcd": "A"}, #RD9
				{"pin": 96, "name": "GLCD_D3", "type": "GD3", "direction": "", "latch": "", "abcd": "A"}, #RG0
				{"pin": 95, "name": "GLCD_D4", "type": "GD4", "direction": "", "latch": "", "abcd": "A"}, #RG1
				{"pin": 90, "name": "GLCD_D5", "type": "GD5", "direction": "", "latch": "", "abcd": "A"}, #RF1
				{"pin": 91, "name": "GLCD_D6", "type": "GD6", "direction": "", "latch": "", "abcd": "A"}, #RF0
				{"pin": 80, "name": "GLCD_D7", "type": "GD7", "direction": "", "latch": "", "abcd": "A"}, #RD12
				{"pin": 143, "name": "GLCD_D8", "type": "GD8", "direction": "", "latch": "", "abcd": "A"}, #RJ14
				{"pin": 145, "name": "GLCD_D9", "type": "GD9", "direction": "", "latch": "", "abcd": "A"}, #RJ12
				{"pin": 74, "name": "GLCD_D10", "type": "GD10", "direction": "", "latch": "", "abcd": "A"}, #RD2
				{"pin": 75, "name": "GLCD_D11", "type": "GD11", "direction": "", "latch": "", "abcd": "A"}, #RD3
				{"pin": 134, "name": "GLCD_D12", "type": "GD12", "direction": "", "latch": "", "abcd": "A"}, #RK3
				{"pin": 133, "name": "GLCD_D13", "type": "GD13", "direction": "", "latch": "", "abcd": "A"}, #RK4
				{"pin": 132, "name": "GLCD_D14", "type": "GD14", "direction": "", "latch": "", "abcd": "A"}, #RK5
				{"pin": 131, "name": "GLCD_D15", "type": "GD15", "direction": "", "latch": "", "abcd": "A"}, #RK6
				{"pin": 89, "name": "GLCD_D16", "type": "GD16", "direction": "", "latch": "", "abcd": "A"}, #RF5
				{"pin": 97, "name": "GLCD_D17", "type": "GD17", "direction": "", "latch": "", "abcd": "A"}, #RF4
				{"pin": 146, "name": "GLCD_D18", "type": "GD18", "direction": "", "latch": "", "abcd": "A"}, #RJ10
				{"pin": 130, "name": "GLCD_D19", "type": "GD19", "direction": "", "latch": "", "abcd": "A"}, #RK7
				{"pin": 151, "name": "GLCD_D20", "type": "GD20", "direction": "", "latch": "", "abcd": "A"}, #RJ3
				{"pin": 92, "name": "GLCD_D21", "type": "GD21", "direction": "", "latch": "", "abcd": "A"}, #RH15
				{"pin": 81, "name": "GLCD_D22", "type": "GD22", "direction": "", "latch": "", "abcd": "A"}, #RD13
				{"pin": 137, "name": "GLCD_D23", "type": "GD23", "direction": "", "latch": "", "abcd": "A"}, #RK0				
				{"pin": 129, "name": "SCL2", "type": "SCL2", "direction": "", "latch": "", "abcd": ""}, #RA2
				{"pin": 128, "name": "SDA2", "type": "SDA2", "direction": "", "latch": "", "abcd": ""}, #RA3
				{"pin": 106, "name": "GLCD_DISP", "type": "GPIO", "direction": "Out", "latch": "High", "abcd": ""}] #RH10

##################################################################################

def mzda_cu_eventHandler(event):
	global pinConfigureFxn
	if (event == "configure"):
		#Override default pin configur function w/ PIC32M specific one
		pinConfigureFxn = configurePinsPIC32M
		try:
			Database.setSymbolValue("le_gfx_driver_glcd", "PixelClockDivider", 10, 1)
			Database.setSymbolValue("gfx_hal_le", "ColorModeHint", "GFX_COLOR_MODE_RGBA_8888", 1)
		except:
			return

mzda_cu_interfaceList = ["GLCD"]

pic32mz_da_curiosity = bspSupportObj(mzda_cu_pinConfig, mzda_cu_activateList, None, mzda_cu_connectList, mzda_cu_eventHandler)

addDisplayIntfSupport("BSP_PIC32MZ_DA_Curiosity", mzda_cu_interfaceList)
addBSPSupport("BSP_PIC32MZ_DA_Curiosity", "GLCD", pic32mz_da_curiosity)

