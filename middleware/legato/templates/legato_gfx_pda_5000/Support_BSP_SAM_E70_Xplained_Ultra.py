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
############ SSD1963 CONFIG ######################################################
ssd1963ActivateList = ["smc", "le_gfx_intf_parallel_smc", "le_gfx_driver_ssd1963", "twihs0", "drv_i2c", "drv_i2c0", "tc0", "sys_time"]
ssd1963AutoConnectList = [["le_gfx_driver_ssd1963", "Display Interface", "le_gfx_intf_parallel_smc", "le_gfx_intf_parallel_smc"],
						["le_gfx_intf_parallel_smc", "SMC_CS", "smc", "smc_cs0"],
						["le_gfx_driver_ssd1963", "Graphics Display", "gfx_disp_pdatm5000_800x480", "gfx_display"],
						["gfx_legato", "gfx_driver", "le_gfx_driver_ssd1963", "gfx_driver_ssd1963"],
						["drv_i2c_0", "drv_i2c_I2C_dependency", "twihs0", "TWIHS0_I2C"],
						["gfx_maxtouch_controller", "i2c", "drv_i2c_0", "drv_i2c"],
						["sys_time", "sys_time_TMR_dependency", "tc0", "TC0_TMR"]]
				
ssd1963PinConfig = [{"pin": 4, "name": "EBI_D8", "type": "EBI_D8", "direction": "", "latch": "", "abcd": "A"}, #PE0
				{"pin": 6, "name": "EBI_D9", "type": "EBI_D9", "direction": "", "latch": "", "abcd": "A"}, #PE1
				{"pin": 7, "name": "EBI_D10", "type": "EBI_D10", "direction": "", "latch": "", "abcd": "A"}, #PE2
				{"pin": 10, "name": "EBI_D11", "type": "EBI_D11", "direction": "", "latch": "", "abcd": "A"}, #PE3
				{"pin": 11, "name": "EBI_D0", "type": "EBI_D0", "direction": "", "latch": "", "abcd": "A"}, #PC0
				{"pin": 15, "name": "GFX_DISP_INTF_PIN_RSDC", "type": "GPIO", "direction": "Out", "latch": "High", "abcd": ""}, #PC30
				{"pin": 19, "name": "GFX_DISP_INTF_PIN_RESET", "type": "GPIO", "direction": "Out", "latch": "High", "abcd": ""}, #PC13
				{"pin": 27, "name": "EBI_D12", "type": "EBI_D12", "direction": "", "latch": "", "abcd": "A"}, #PE4
				{"pin": 28, "name": "EBI_D13", "type": "EBI_D13", "direction": "", "latch": "", "abcd": "A"}, #PE5
				{"pin": 38, "name": "EBI_D1", "type": "EBI_D1", "direction": "", "latch": "", "abcd": "A"}, #PC1
				{"pin": 39, "name": "EBI_D2", "type": "EBI_D2", "direction": "", "latch": "", "abcd": "A"}, #PC2
				{"pin": 40, "name": "EBI_D3", "type": "EBI_D3", "direction": "", "latch": "", "abcd": "A"}, #PC3
				{"pin": 41, "name": "EBI_D4", "type": "EBI_D4", "direction": "", "latch": "", "abcd": "A"}, #PC4
				{"pin": 45, "name": "EBI_D15", "type": "EBI_D15", "direction": "", "latch": "", "abcd": "A"}, #PA16
				{"pin": 48, "name": "EBI_D7", "type": "EBI_D7", "direction": "", "latch": "", "abcd": "A"}, #PC7
				{"pin": 49, "name": "EBI_D14", "type": "EBI_D14", "direction": "", "latch": "", "abcd": "A"}, #PA15
				{"pin": 54, "name": "EBI_D6", "type": "EBI_D6", "direction": "", "latch": "", "abcd": "A"}, #PC6
				{"pin": 58, "name": "EBI_D5", "type": "EBI_D5", "direction": "", "latch": "", "abcd": "A"}, #PC5
				{"pin": 67, "name": "GFX_DISP_INTF_PIN_CS", "type": "GPIO", "direction": "Out", "latch": "High", "abcd": ""}, #PD19
				{"pin": 71, "name": "BSP_MAXTOUCH_CHG", "type": "GPIO", "direction": "In", "latch": "", "abcd": ""}, #PD28
				{"pin": 77, "name": "TWIHS0_TWCK0", "type": "TWIHS0_TWCK0", "direction": "", "latch": "", "abcd": "A"}, #PA4
				{"pin": 82, "name": "GFX_DISP_INTF_PIN_WR", "type": "GPIO", "direction": "Out", "latch": "High", "abcd": ""}, #PC8
				{"pin": 86, "name": "GFX_DISP_INTF_PIN_BACKLIGHT", "type": "GPIO", "direction": "Out", "latch": "High", "abcd": ""}, #PC9
				{"pin": 91, "name": "TWIHS0_TWD0", "type": "TWIHS0_TWD0", "direction": "", "latch": "", "abcd": "A"}, #PA8
				{"pin": 94, "name": "GFX_DISP_INTF_PIN_RD", "type": "GPIO", "direction": "Out", "latch": "High", "abcd": ""}] #PC11
##################################################################################

def eventHandlerSSD1963(event):
	if (event == "configure"):
		#Enable the MPU, disable caching of SMC memory space
		try:
			Database.setSymbolValue("core", "CoreUseMPU", True, 1)
			Database.setSymbolValue("core", "MPU_Region_0_Enable", True, 1)
			Database.setSymbolValue("core", "MPU_Region_Name0", "EBI_SMC", 1)
			Database.setSymbolValue("core", "MPU_Region_0_Type", 5, 1)
		except:
			return

bspDisplayInterfaceList = ["SSD1963"]

sam_e70_xplained_utra_ssd1963 = bspSupportObj(ssd1963PinConfig, ssd1963ActivateList, None, ssd1963AutoConnectList, eventHandlerSSD1963)

addBSPSupport("BSP_SAM_E70_Xplained_Ultra", "SSD1963", sam_e70_xplained_utra_ssd1963)
addDisplayIntfSupport("BSP_SAM_E70_Xplained_Ultra", bspDisplayInterfaceList)