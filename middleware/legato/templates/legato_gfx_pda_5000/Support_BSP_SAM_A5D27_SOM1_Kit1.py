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
						["le_gfx_lcdc", "Graphics Display", "gfx_disp_pdatm5000_800x480", "gfx_display"],
						["gfx_legato", "gfx_driver", "le_gfx_lcdc", "le_gfx_driver_lcdc"],
						["drv_i2c_0", "drv_i2c_I2C_dependency", "twihs1", "TWIHS1_I2C"],
						["gfx_maxtouch_controller", "i2c", "drv_i2c_0", "drv_i2c"],
						["sys_time", "sys_time_TMR_dependency", "tc0", "TC0_TMR"]]

lcdcPinConfig = [{"pin": 43, "name": "BSP_MAXTOUCH_CHG", "type": "GPIO", "direction": "In", "latch": "", "abcd": ""},
				{"pin": 126, "name": "TWIHS1_TWD1", "type": "TWIHS1_TWD1", "direction": "", "latch": "", "abcd": "A"}, 
				{"pin": 120, "name": "TWIHS1_TWCK1", "type": "TWIHS1_TWCK1", "direction": "", "latch": "", "abcd": "A"},
				#{"pin": 7, "name": "LCDC_LCDDAT0", "type": "LCDC_LCDDAT0", "direction": "", "latch": "", "abcd": "A"},
				#{"pin": 23, "name": "LCDC_LCDDAT1", "type": "LCDC_LCDDAT1", "direction": "", "latch": "", "abcd": "A"},
				{"pin": 39, "name": "LCDC_LCDDAT2", "type": "LCDC_LCDDAT2", "direction": "", "latch": "", "abcd": "A"},
				{"pin": 6, "name": "LCDC_LCDDAT3", "type": "LCDC_LCDDAT3", "direction": "", "latch": "", "abcd": "A"},
				{"pin": 72, "name": "LCDC_LCDDAT4", "type": "LCDC_LCDDAT4", "direction": "", "latch": "", "abcd": "A"},
				{"pin": 22, "name": "LCDC_LCDDAT5", "type": "LCDC_LCDDAT5", "direction": "", "latch": "", "abcd": "A"},
				{"pin": 38, "name": "LCDC_LCDDAT6", "type": "LCDC_LCDDAT6", "direction": "", "latch": "", "abcd": "A"},
				{"pin": 5, "name": "LCDC_LCDDAT7", "type": "LCDC_LCDDAT7", "direction": "", "latch": "", "abcd": "A"},
				#{"pin": 21, "name": "LCDC_LCDDAT8", "type": "LCDC_LCDDAT8", "direction": "", "latch": "", "abcd": "A"},
				#{"pin": 4, "name": "LCDC_LCDDAT9", "type": "LCDC_LCDDAT9", "direction": "", "latch": "", "abcd": "A"},
				{"pin": 54, "name": "LCDC_LCDDAT10", "type": "LCDC_LCDDAT10", "direction": "", "latch": "", "abcd": "A"},
				{"pin": 37, "name": "LCDC_LCDDAT11", "type": "LCDC_LCDDAT11", "direction": "", "latch": "", "abcd": "A"},
				{"pin": 20, "name": "LCDC_LCDDAT12", "type": "LCDC_LCDDAT12", "direction": "", "latch": "", "abcd": "A"},
				{"pin": 70, "name": "LCDC_LCDDAT13", "type": "LCDC_LCDDAT13", "direction": "", "latch": "", "abcd": "A"},
				{"pin": 3, "name": "LCDC_LCDDAT14", "type": "LCDC_LCDDAT14", "direction": "", "latch": "", "abcd": "A"},
				{"pin": 105, "name": "LCDC_LCDDAT15", "type": "LCDC_LCDDAT15", "direction": "", "latch": "", "abcd": "A"},
				#{"pin": 89, "name": "LCDC_LCDDAT16", "type": "LCDC_LCDDAT16", "direction": "", "latch": "", "abcd": "A"},
				#{"pin": 53, "name": "LCDC_LCDDAT17", "type": "LCDC_LCDDAT17", "direction": "", "latch": "", "abcd": "A"},
				{"pin": 110, "name": "LCDC_LCDDAT18", "type": "LCDC_LCDDAT18", "direction": "", "latch": "", "abcd": "A"},
				{"pin": 36, "name": "LCDC_LCDDAT19", "type": "LCDC_LCDDAT19", "direction": "", "latch": "", "abcd": "A"},
				{"pin": 109, "name": "LCDC_LCDDAT20", "type": "LCDC_LCDDAT20", "direction": "", "latch": "", "abcd": "A"},
				{"pin": 214, "name": "LCDC_LCDDAT21", "type": "LCDC_LCDDAT21", "direction": "", "latch": "", "abcd": "A"},
				{"pin": 215, "name": "LCDC_LCDDAT22", "type": "LCDC_LCDDAT22", "direction": "", "latch": "", "abcd": "A"},
				{"pin": 213, "name": "LCDC_LCDDAT23", "type": "LCDC_LCDDAT23", "direction": "", "latch": "", "abcd": "A"},
				{"pin": 197, "name": "LCDC_LCDPWM", "type": "LCDC_LCDPWM", "direction": "", "latch": "", "abcd": "A"},
				{"pin": 219, "name": "LCDC_LCDDISP", "type": "LCDC_LCDDISP", "direction": "", "latch": "", "abcd": "A"},
				{"pin": 203, "name": "LCDC_LCDVSYNC", "type": "LCDC_LCDVSYNC", "direction": "", "latch": "", "abcd": "A"},
				{"pin": 181, "name": "LCDC_LCDHSYNC", "type": "LCDC_LCDHSYNC", "direction": "", "latch": "", "abcd": "A"},
				{"pin": 202, "name": "LCDC_LCDPCK", "type": "LCDC_LCDPCK", "direction": "", "latch": "", "abcd": "A"},
				{"pin": 200, "name": "LCDC_LCDDEN", "type": "LCDC_LCDDEN", "direction": "", "latch": "", "abcd": "A"}]
				
bspDisplayInterfaceList = ["LCDC"]

def eventHandler(event):
	if (event == "configure"):
		### set heap to 32kB for A5D2
		Database.setSymbolValue("core", "IAR_HEAP_SIZE", 32768)
		### Set pixel clock to 27.3MHz
		Database.setSymbolValue("le_gfx_lcdc", "PixelClockDiv", 6)
		### Enable Double Buffering
		#Database.setSymbolValue("le_gfx_lcdc", "DoubleBuffer", True)
		### Backlight Clock Settings
		#Database.setSymbolValue("le_gfx_lcdc", "BacklightPWMClockSource", "CLK_SLOW")
		#Database.setSymbolValue("le_gfx_lcdc", "BacklightPWMClockPrescaler", 0)

##################################################################################

sam_a5d27_wlsom1_kit1_lcdc = bspSupportObj(lcdcPinConfig, lcdcActivateList, lcdcDeactivateList, lcdcAutoConnectList, eventHandler)

addBSPSupport("BSP_SAM_A5D27_SOM1_Kit1", "LCDC", sam_a5d27_wlsom1_kit1_lcdc)
addDisplayIntfSupport("BSP_SAM_A5D27_SOM1_Kit1", bspDisplayInterfaceList)
