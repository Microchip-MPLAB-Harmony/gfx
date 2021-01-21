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
sam9x60_ek_ActivateList = ["lcdc", "le_gfx_lcdc", "flexcom0", "drv_i2c", "drv_i2c0", "tc0", "sys_time", "le_gfx_gfx2d", "plib_gfx2d"]
sam9x60_ek_DeactivateList = ["pit"]
sam9x60_ek_AutoConnectList = [["le_gfx_lcdc", "LCDC", "lcdc", "LCDC_LCDC"],
						["le_gfx_lcdc", "Graphics Display", "gfx_disp_pdatm7000b_800x480", "gfx_display"],
						["gfx_legato", "gfx_driver", "le_gfx_lcdc", "le_gfx_driver_lcdc"],
						["drv_i2c_0", "drv_i2c_I2C_dependency", "flexcom0", "FLEXCOM0_I2C"],
						["gfx_maxtouch_controller", "i2c", "drv_i2c_0", "drv_i2c"],
						["sys_time", "sys_time_TMR_dependency", "tc0", "TC0_TMR"],
						["le_gfx_gfx2d", "GFX2D", "plib_gfx2d", "GFX2D"],
						["gfx_legato", "gpu_driver", "le_gfx_gfx2d", "LE GPU Driver"]]

sam9x60_ek_PinConfig = [{"pin": 162, "name": "LCDC_LCDDAT0", "type": "LCDC_LCDDAT0", "direction": "", "latch": "", "abcd": "A"}, #PC0, M4
				{"pin": 187, "name": "LCDC_LCDDAT1", "type": "LCDC_LCDDAT1", "direction": "", "latch": "", "abcd": "A"}, #PC1, P4
				{"pin": 176, "name": "LCDC_LCDDAT2", "type": "LCDC_LCDDAT2", "direction": "", "latch": "", "abcd": "A"}, #PC2, N5
				{"pin": 188, "name": "LCDC_LCDDAT3", "type": "LCDC_LCDDAT3", "direction": "", "latch": "", "abcd": "A"}, #PC3, P5
				{"pin": 147, "name": "LCDC_LCDDAT4", "type": "LCDC_LCDDAT4", "direction": "", "latch": "", "abcd": "A"}, #PC4, L5
				{"pin": 201, "name": "LCDC_LCDDAT5", "type": "LCDC_LCDDAT5", "direction": "", "latch": "", "abcd": "A"}, #PC5, R4
				{"pin": 163, "name": "LCDC_LCDDAT6", "type": "LCDC_LCDDAT6", "direction": "", "latch": "", "abcd": "A"}, #PC6, M6
				{"pin": 215, "name": "LCDC_LCDDAT7", "type": "LCDC_LCDDAT7", "direction": "", "latch": "", "abcd": "A"}, #PC7, T3
				{"pin": 178, "name": "LCDC_LCDDAT8", "type": "LCDC_LCDDAT8", "direction": "", "latch": "", "abcd": "A"}, #PC8, N8
				{"pin": 216, "name": "LCDC_LCDDAT9", "type": "LCDC_LCDDAT9", "direction": "", "latch": "", "abcd": "A"}, #PC9, T4
				{"pin": 189, "name": "LCDC_LCDDAT10", "type": "LCDC_LCDDAT10", "direction": "", "latch": "", "abcd": "A"}, #PC10, P6
				{"pin": 177, "name": "LCDC_LCDDAT11", "type": "LCDC_LCDDAT11", "direction": "", "latch": "", "abcd": "A"}, #PC11, N6
				{"pin": 202, "name": "LCDC_LCDDAT12", "type": "LCDC_LCDDAT12", "direction": "", "latch": "", "abcd": "A"}, #PC12, R5
				{"pin": 149, "name": "LCDC_LCDDAT13", "type": "LCDC_LCDDAT13", "direction": "", "latch": "", "abcd": "A"}, #PC13, L7
				{"pin": 217, "name": "LCDC_LCDDAT14", "type": "LCDC_LCDDAT14", "direction": "", "latch": "", "abcd": "A"}, #PC14, T5
				{"pin": 121, "name": "LCDC_LCDDAT15", "type": "LCDC_LCDDAT15", "direction": "", "latch": "", "abcd": "A"}, #PC15, J7
				{"pin": 203, "name": "LCDC_LCDDAT16", "type": "LCDC_LCDDAT16", "direction": "", "latch": "", "abcd": "A"}, #PC16, R6
				{"pin": 136, "name": "LCDC_LCDDAT17", "type": "LCDC_LCDDAT17", "direction": "", "latch": "", "abcd": "A"}, #PC17, K8
				{"pin": 218, "name": "LCDC_LCDDAT18", "type": "LCDC_LCDDAT18", "direction": "", "latch": "", "abcd": "A"}, #PC18, T6
				{"pin": 150, "name": "LCDC_LCDDAT19", "type": "LCDC_LCDDAT19", "direction": "", "latch": "", "abcd": "A"}, #PC19, L8
				{"pin": 191, "name": "LCDC_LCDDAT20", "type": "LCDC_LCDDAT20", "direction": "", "latch": "", "abcd": "A"}, #PC20, P8
				{"pin": 165, "name": "LCDC_LCDDAT21", "type": "LCDC_LCDDAT21", "direction": "", "latch": "", "abcd": "A"}, #PC21, M8
				{"pin": 204, "name": "LCDC_LCDDAT22", "type": "LCDC_LCDDAT22", "direction": "", "latch": "", "abcd": "A"}, #PC22, R7
				{"pin": 137, "name": "LCDC_LCDDAT23", "type": "LCDC_LCDDAT23", "direction": "", "latch": "", "abcd": "A"}, #PC23, K9
				{"pin": 205, "name": "LCDC_LCDDISP", "type": "LCDC_LCDDISP", "direction": "", "latch": "", "abcd": "A"}, #PC24, R8
				{"pin": 220, "name": "LCDC_LCDPWM", "type": "LCDC_LCDPWM", "direction": "", "latch": "", "abcd": "A"}, #PC26, T8
				{"pin": 166, "name": "LCDC_LCDVSYNC", "type": "LCDC_LCDVSYNC", "direction": "", "latch": "", "abcd": "A"}, #PC27, M9
				{"pin": 179, "name": "LCDC_LCDHSYNC", "type": "LCDC_LCDHSYNC", "direction": "", "latch": "", "abcd": "A"}, #PC28, N9
				{"pin": 152, "name": "LCDC_LCDDEN", "type": "LCDC_LCDDEN", "direction": "", "latch": "", "abcd": "A"}, #PC29, L10
				{"pin": 219, "name": "LCDC_LCDPCK", "type": "LCDC_LCDPCK", "direction": "", "latch": "", "abcd": "A"}, #PC30, T7
				{"pin": 151, "name": "BSP_MAXTOUCH_CHG", "type": "GPIO", "direction": "In", "latch": "", "abcd": ""}, #PC25, L9
				{"pin": 186, "name": "FLEXCOM0_IO0", "type": "FLEXCOM0_IO0", "direction": "", "latch": "", "abcd": "A"}, #PA0, P2
				{"pin": 161, "name": "FLEXCOM0_IO1", "type": "FLEXCOM0_IO1", "direction": "", "latch": "", "abcd": "A"}] #PA1, M3
				
sam9x60_ek_DisplayInterfaceList = ["LCDC"]

def sam9x60_EventHandler(event):
	if (event == "configure"):
		### set heap to 32kB for A5D2
		Database.setSymbolValue("core", "IAR_HEAP_SIZE", 32768, 0)
		### set default color mode to RGBA 8888
		Database.setSymbolValue("gfx_hal", "ColorModeHint", "GFX_COLOR_MODE_RGBA_8888", 0)
		### Set pixel clock to 30MHz
		Database.setSymbolValue("gfx_driver_lcdc", "PixelClock", 25000000, 1)
		### Configure the LCDC GCLK
		Database.setSymbolValue("core", "CLK_LCDC_GCLKEN", True, 1)
		Database.setSymbolValue("core", "CLK_LCDC_GCLKCSS", 3, 1) #MCK

##################################################################################

sam9x60_ek_bspSupport= bspSupportObj(sam9x60_ek_PinConfig, sam9x60_ek_ActivateList, sam9x60_ek_DeactivateList, sam9x60_ek_AutoConnectList, sam9x60_EventHandler)

addBSPSupport("BSP_SAM_9X60_Evaluation_Kit", "LCDC", sam9x60_ek_bspSupport)
addDisplayIntfSupport("BSP_SAM_9X60_Evaluation_Kit", sam9x60_ek_DisplayInterfaceList)
