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

############### LCDC CONFIG #######################################################
sama5d29_cu_ActivateList = ["lcdc", "le_gfx_lcdc", "twihs0", "drv_i2c", "drv_i2c0", "tc0", "sys_time", "le_gfx_gfx2d", "plib_gfx2d"]
sama5d29_cu_DeactivateList = ["pit"]
sama5d29_cu_AutoConnectList = [["le_gfx_lcdc", "LCDC", "lcdc", "LCDC_LCDC"],
						["le_gfx_lcdc", "Graphics Display", "gfx_disp_pdatm4301b_480x272", "gfx_display"],
						["gfx_legato", "gfx_driver", "le_gfx_lcdc", "le_gfx_driver_lcdc"],
						["drv_i2c_0", "drv_i2c_I2C_dependency", "twihs0", "TWIHS0_I2C"],
						["gfx_maxtouch_controller", "i2c", "drv_i2c_0", "drv_i2c"],
						["sys_time", "sys_time_TMR_dependency", "tc0", "TC0_TMR"],
						["le_gfx_gfx2d", "GFX2D", "plib_gfx2d", "GFX2D"],
						["gfx_legato", "gpu_driver", "le_gfx_gfx2d", "LE GPU Driver"]]

sama5d29_cu_PinConfig = [
				##{"pin": 22, "name": "LCDC_LCDDAT0", "type": "LCDC_LCDDAT0", "direction": "", "latch": "", "abcd": "A"}, #PB11
				##{"pin": 57, "name": "LCDC_LCDDAT1", "type": "LCDC_LCDDAT1", "direction": "", "latch": "", "abcd": "A"}, #PB12
				{"pin": 71, "name": "LCDC_LCDDAT2", "type": "LCDC_LCDDAT2", "direction": "", "latch": "", "abcd": "A"}, #PC10
				{"pin": 70, "name": "LCDC_LCDDAT3", "type": "LCDC_LCDDAT3", "direction": "", "latch": "", "abcd": "A"}, #PC11
				{"pin": 69, "name": "LCDC_LCDDAT4", "type": "LCDC_LCDDAT4", "direction": "", "latch": "", "abcd": "A"}, #PC12
				{"pin": 88, "name": "LCDC_LCDDAT5", "type": "LCDC_LCDDAT5", "direction": "", "latch": "", "abcd": "A"}, #PC13
				{"pin": 90, "name": "LCDC_LCDDAT6", "type": "LCDC_LCDDAT6", "direction": "", "latch": "", "abcd": "A"}, #PC14
				{"pin": 87, "name": "LCDC_LCDDAT7", "type": "LCDC_LCDDAT7", "direction": "", "latch": "", "abcd": "A"}, #PC15
				##{"pin": 55, "name": "LCDC_LCDDAT8", "type": "LCDC_LCDDAT8", "direction": "", "latch": "", "abcd": "A"}, #PB19
				##{"pin": 20, "name": "LCDC_LCDDAT9", "type": "LCDC_LCDDAT9", "direction": "", "latch": "", "abcd": "A"}, #PB20
				{"pin": 108, "name": "LCDC_LCDDAT10", "type": "LCDC_LCDDAT10", "direction": "", "latch": "", "abcd": "A"}, #PC16
				{"pin": 86, "name": "LCDC_LCDDAT11", "type": "LCDC_LCDDAT11", "direction": "", "latch": "", "abcd": "A"}, #PC17
				{"pin": 125, "name": "LCDC_LCDDAT12", "type": "LCDC_LCDDAT12", "direction": "", "latch": "", "abcd": "A"}, #PC18
				{"pin": 104, "name": "LCDC_LCDDAT13", "type": "LCDC_LCDDAT13", "direction": "", "latch": "", "abcd": "A"}, #PC19
				{"pin": 105, "name": "LCDC_LCDDAT14", "type": "LCDC_LCDDAT14", "direction": "", "latch": "", "abcd": "A"}, #PC20
				{"pin": 103, "name": "LCDC_LCDDAT15", "type": "LCDC_LCDDAT15", "direction": "", "latch": "", "abcd": "A"}, #PC21
				##{"pin": 18, "name": "LCDC_LCDDAT16", "type": "LCDC_LCDDAT16", "direction": "", "latch": "", "abcd": "A"}, #PB27
				##{"pin": 36, "name": "LCDC_LCDDAT17", "type": "LCDC_LCDDAT17", "direction": "", "latch": "", "abcd": "A"}, #PB28
				{"pin": 121, "name": "LCDC_LCDDAT18", "type": "LCDC_LCDDAT18", "direction": "", "latch": "", "abcd": "A"}, #PC22
				{"pin": 107, "name": "LCDC_LCDDAT19", "type": "LCDC_LCDDAT19", "direction": "", "latch": "", "abcd": "A"}, #PC23
				{"pin": 120, "name": "LCDC_LCDDAT20", "type": "LCDC_LCDDAT20", "direction": "", "latch": "", "abcd": "A"}, #PC24
				{"pin": 124, "name": "LCDC_LCDDAT21", "type": "LCDC_LCDDAT21", "direction": "", "latch": "", "abcd": "A"}, #PC25
				{"pin": 145, "name": "LCDC_LCDDAT22", "type": "LCDC_LCDDAT22", "direction": "", "latch": "", "abcd": "A"}, #PC26
				{"pin": 128, "name": "LCDC_LCDDAT23", "type": "LCDC_LCDDAT23", "direction": "", "latch": "", "abcd": "A"}, #PC27
				{"pin": 110, "name": "LCDC_LCDDISP", "type": "LCDC_LCDDISP", "direction": "", "latch": "", "abcd": "A"}, #PC29
				{"pin": 76, "name": "LCDC_LCDPWM", "type": "LCDC_LCDPWM", "direction": "", "latch": "", "abcd": "A"}, #PC28
				{"pin": 93, "name": "LCDC_LCDVSYNC", "type": "LCDC_LCDVSYNC", "direction": "", "latch": "", "abcd": "A"}, #PC30
				{"pin": 59, "name": "LCDC_LCDHSYNC", "type": "LCDC_LCDHSYNC", "direction": "", "latch": "", "abcd": "A"}, #PC31
				{"pin": 78, "name": "LCDC_LCDDEN", "type": "LCDC_LCDDEN", "direction": "", "latch": "", "abcd": "A"}, #PD1
				{"pin": 112, "name": "LCDC_LCDPCK", "type": "LCDC_LCDPCK", "direction": "", "latch": "", "abcd": "A"}, #PD0
				{"pin": 19, "name": "BSP_MAXTOUCH_CHG", "type": "GPIO", "direction": "In", "latch": "", "abcd": ""}, #PB25
				{"pin": 238, "name": "TOUCH_SCL", "type": "TWIHS0_TWCK0", "direction": "", "latch": "", "abcd": "A"}, #PC0
				{"pin": 35, "name": "TOUCH_SDA", "type": "TWIHS0_TWD0", "direction": "", "latch": "", "abcd": "A"}] #PB31
				
sama5d29_cu_DisplayInterfaceList = ["LCDC"]

def sama5d29_EventHandler(event):
	if (event == "configure"):
		### set heap to 32kB for A5D2
		Database.setSymbolValue("core", "IAR_HEAP_SIZE", 32768, 0)
		### set default color mode to RGBA 8888
		Database.setSymbolValue("gfx_hal_le", "ColorModeHint", "GFX_COLOR_MODE_RGBA_8888", 0)
  		### Configure the LCDC GCLK
		Database.setSymbolValue("core", "LCDC_CLOCK_ENABLE", True, 1)
		### Set output color mode to 18 BPP
		Database.setSymbolValue("le_gfx_lcdc", "OutputColorMode", 2, 1)

##################################################################################

sama5d29_cu_bspSupport= bspSupportObj(sama5d29_cu_PinConfig, sama5d29_cu_ActivateList, sama5d29_cu_DeactivateList, sama5d29_cu_AutoConnectList, sama5d29_EventHandler)

addBSPSupport("BSP_SAM_A5D29_Curiosity", "LCDC", sama5d29_cu_bspSupport)
addDisplayIntfSupport("BSP_SAM_A5D29_Curiosity", sama5d29_cu_DisplayInterfaceList)
