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
lcdcActivateList = ["lcdc", "le_gfx_lcdc", "twihs1", "drv_i2c", "drv_i2c0", "tc0", "sys_time"]
lcdcDeactivateList = ["pit"]
lcdcAutoConnectList = [["le_gfx_lcdc", "LCDC", "lcdc", "LCDC_LCDC"],
						["le_gfx_lcdc", "Graphics Display", "gfx_disp_pdatm7000b_800x480", "gfx_display"],
						["gfx_legato", "gfx_driver", "le_gfx_lcdc", "le_gfx_driver_lcdc"],
						["drv_i2c_0", "drv_i2c_I2C_dependency", "twihs1", "TWIHS1_I2C"],
						["gfx_maxtouch_controller", "i2c", "drv_i2c_0", "drv_i2c"],
						["sys_time", "sys_time_TMR_dependency", "tc0", "TC0_TMR"]]

lcdcPinConfig = [{"pin": 40, "name": "BSP_MAXTOUCH_CHG", "type": "GPIO", "direction": "In", "latch": "", "abcd": ""}, #PB7, C6
				{"pin": 71, "name": "LCDC_LCDDAT2", "type": "LCDC_LCDDAT2", "direction": "", "latch": "", "abcd": "A"}, #PC10, E3
				{"pin": 70, "name": "LCDC_LCDDAT3", "type": "LCDC_LCDDAT3", "direction": "", "latch": "", "abcd": "A"}, #PC11, E2
				{"pin": 69, "name": "LCDC_LCDDAT4", "type": "LCDC_LCDDAT4", "direction": "", "latch": "", "abcd": "A"}, #PC12, E1
				{"pin": 88, "name": "LCDC_LCDDAT5", "type": "LCDC_LCDDAT5", "direction": "", "latch": "", "abcd": "A"}, #PC13, F3
				{"pin": 90, "name": "LCDC_LCDDAT6", "type": "LCDC_LCDDAT6", "direction": "", "latch": "", "abcd": "A"}, #PC14, F5
				{"pin": 87, "name": "LCDC_LCDDAT7", "type": "LCDC_LCDDAT7", "direction": "", "latch": "", "abcd": "A"}, #PC15, F2
				{"pin": 108, "name": "LCDC_LCDDAT10", "type": "LCDC_LCDDAT10", "direction": "", "latch": "", "abcd": "A"}, #PC16, G6
				{"pin": 86, "name": "LCDC_LCDDAT11", "type": "LCDC_LCDDAT11", "direction": "", "latch": "", "abcd": "A"}, #PC17, F1
				{"pin": 125, "name": "LCDC_LCDDAT12", "type": "LCDC_LCDDAT12", "direction": "", "latch": "", "abcd": "A"}, #PC18, H6
				{"pin": 104, "name": "LCDC_LCDDAT13", "type": "LCDC_LCDDAT13", "direction": "", "latch": "", "abcd": "A"}, #PC19, G2
				{"pin": 105, "name": "LCDC_LCDDAT14", "type": "LCDC_LCDDAT14", "direction": "", "latch": "", "abcd": "A"}, #PC20, G3
				{"pin": 103, "name": "LCDC_LCDDAT15", "type": "LCDC_LCDDAT15", "direction": "", "latch": "", "abcd": "A"}, #PC21, G1
				{"pin": 121, "name": "LCDC_LCDDAT18", "type": "LCDC_LCDDAT18", "direction": "", "latch": "", "abcd": "A"}, #PC22, H2
				{"pin": 107, "name": "LCDC_LCDDAT19", "type": "LCDC_LCDDAT19", "direction": "", "latch": "", "abcd": "A"}, #PC23, G5
				{"pin": 120, "name": "LCDC_LCDDAT20", "type": "LCDC_LCDDAT20", "direction": "", "latch": "", "abcd": "A"}, #PC24, H1
				{"pin": 124, "name": "LCDC_LCDDAT21", "type": "LCDC_LCDDAT21", "direction": "", "latch": "", "abcd": "A"}, #PC25, H5
				{"pin": 145, "name": "LCDC_LCDDAT22", "type": "LCDC_LCDDAT22", "direction": "", "latch": "", "abcd": "A"}, #PC26, J9
				{"pin": 128, "name": "LCDC_LCDDAT23", "type": "LCDC_LCDDAT23", "direction": "", "latch": "", "abcd": "A"}, #PC27, H9
				{"pin": 76, "name": "LCDC_LCDPWM", "type": "LCDC_LCDPWM", "direction": "", "latch": "", "abcd": "A"}, #PC28, E8
				{"pin": 110, "name": "LCDC_LCDDISP", "type": "LCDC_LCDDISP", "direction": "", "latch": "", "abcd": "A"}, #PC29, G8
				{"pin": 93, "name": "LCDC_LCDVSYNC", "type": "LCDC_LCDVSYNC", "direction": "", "latch": "", "abcd": "A"}, #PC30, F8
				{"pin": 59, "name": "LCDC_LCDHSYNC", "type": "LCDC_LCDHSYNC", "direction": "", "latch": "", "abcd": "A"}, #PC31, D8
				{"pin": 112, "name": "LCDC_LCDPCK", "type": "LCDC_LCDPCK", "direction": "", "latch": "", "abcd": "A"}, #PD0, G10
				{"pin": 78, "name": "LCDC_LCDDEN", "type": "LCDC_LCDDEN", "direction": "", "latch": "", "abcd": "A"}, #PD1, E10
				{"pin": 142, "name": "TWIHS1_TWD1", "type": "TWIHS1_TWD1", "direction": "", "latch": "", "abcd": "A"}, #PD4, J6
				{"pin": 140, "name": "TWIHS1_TWCK1", "type": "TWIHS1_TWCK1", "direction": "", "latch": "", "abcd": "A"}] #PD5, J4
				
bspDisplayInterfaceList = ["LCDC"]

def eventHandler(event):
	if (event == "configure"):
		### set heap to 32kB for A5D2
		Database.setSymbolValue("core", "IAR_HEAP_SIZE", 32768, 0)
		### set default color mode to RGBA 8888
		Database.setSymbolValue("gfx_hal", "ColorModeHint", "GFX_COLOR_MODE_RGBA_8888", 0)
		### Set pixel clock to 30MHz
		Database.setSymbolValue("gfx_driver_lcdc", "PixelClock", 30000000, 1)

##################################################################################

sam_e70_xplained_utra_lcdc = bspSupportObj(lcdcPinConfig, lcdcActivateList, lcdcDeactivateList, lcdcAutoConnectList, eventHandler)

addBSPSupport("BSP_SAM_A5D2_Xplained_Ultra", "LCDC", sam_e70_xplained_utra_lcdc)
addDisplayIntfSupport("BSP_SAM_A5D2_Xplained_Ultra", bspDisplayInterfaceList)
