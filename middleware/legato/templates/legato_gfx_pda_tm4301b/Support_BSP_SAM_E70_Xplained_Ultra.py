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

########### SDRAM PIN CONFIG ####################################################
sdramPinConfig = [{"pin": 12, "name": "EBI_A9/SDA7", "type": "EBI_A9/SDA7", "direction": "", "latch": "", "abcd": "A"}, #PC27
				{"pin": 13, "name": "EBI_A8/SDA6", "type": "EBI_A8/SDA6", "direction": "", "latch": "", "abcd": "A"}, #PC26
				{"pin": 16, "name": "EBI_A11/SDA9", "type": "EBI_A11/SDA9", "direction": "", "latch": "", "abcd": "A"}, #PC29
				{"pin": 18, "name": "EBI_NCS1/SDCS", "type": "EBI_NCS1/SDCS", "direction": "", "latch": "", "abcd": "A"}, #PC15
				{"pin": 22, "name": "EBI_A16/BA0", "type": "EBI_A16/BA0", "direction": "", "latch": "", "abcd": "C"}, #PA20
				{"pin": 57, "name": "EBI_SDCK", "type": "EBI_SDCK", "direction": "", "latch": "", "abcd": "C"}, #PD23
				{"pin": 74, "name": "EBI_CAS", "type": "EBI_CAS", "direction": "", "latch": "", "abcd": "C"}, #PD17
				{"pin": 76, "name": "EBI_A10/SDA8", "type": "EBI_A10/SDA8", "direction": "", "latch": "", "abcd": "A"}, #PC28
				{"pin": 78, "name": "EBI_RAS", "type": "EBI_RAS", "direction": "", "latch": "", "abcd": "C"}, #PD16
				{"pin": 84, "name": "EBI_SDCKE", "type": "EBI_SDCKE", "direction": "", "latch": "", "abcd": "C"}, #PD14
				{"pin": 88, "name": "EBI_SDA10", "type": "EBI_SDA10", "direction": "", "latch": "", "abcd": "C"}, #PD13
				{"pin": 106, "name": "EBI_NWR1/NBS1/DQM1", "type": "EBI_NWR1/NBS1/DQM1", "direction": "", "latch": "", "abcd": "C"}, #PD15
				{"pin": 108, "name": "EBI_SDWE", "type": "EBI_SDWE", "direction": "", "latch": "", "abcd": "C"}, #PD29
				{"pin": 111, "name": "EBI_A0/NBS0/DQM0", "type": "EBI_A0/NBS0/DQM0", "direction": "", "latch": "", "abcd": "A"}, #PC18
				{"pin": 120, "name": "EBI_A2/SDA0", "type": "EBI_A2/SDA0", "direction": "", "latch": "", "abcd": "A"}, #PC20
				{"pin": 122, "name": "EBI_A3/SDA1", "type": "EBI_A3/SDA1", "direction": "", "latch": "", "abcd": "A"}, #PC21
				{"pin": 124, "name": "EBI_A4/SDA2", "type": "EBI_A4/SDA2", "direction": "", "latch": "", "abcd": "A"}, #PC22
				{"pin": 127, "name": "EBI_A5/SDA3", "type": "EBI_A5/SDA3", "direction": "", "latch": "", "abcd": "A"}, #PC23
				{"pin": 130, "name": "EBI_A6/SDA4", "type": "EBI_A6/SDA4", "direction": "", "latch": "", "abcd": "A"}, #PC24
				{"pin": 133, "name": "EBI_A7/SDA5", "type": "EBI_A7/SDA5", "direction": "", "latch": "", "abcd": "A"}] #PC25
##################################################################################

############### LCC CONFIG #######################################################
lccActivateList = ["smc", "le_gfx_driver_lcc", "twihs0", "drv_i2c", "drv_i2c0", "tc0", "sys_time", "tc2"]
lccAutoConnectList = [["le_gfx_driver_lcc", "SMC_CS", "smc", "smc_cs0"],
						["gfx_legato", "gfx_driver", "le_gfx_driver_lcc", "gfx_driver_lcc"],
						["le_gfx_driver_lcc", "Graphics Display", "gfx_disp_pdatm4301b_480x272", "gfx_display"],
						["drv_i2c_0", "drv_i2c_I2C_dependency", "twihs0", "TWIHS0_I2C"],
						["gfx_maxtouch_controller", "i2c", "drv_i2c_0", "drv_i2c"],
						["sys_time", "sys_time_TMR_dependency", "tc0", "TC0_TMR"]]

lccPinConfig = [{"pin": 4, "name": "EBI_D8", "type": "EBI_D8", "direction": "", "latch": "", "abcd": "A"}, #PE0
				{"pin": 6, "name": "EBI_D9", "type": "EBI_D9", "direction": "", "latch": "", "abcd": "A"}, #PE1
				{"pin": 7, "name": "EBI_D10", "type": "EBI_D10", "direction": "", "latch": "", "abcd": "A"}, #PE2
				{"pin": 10, "name": "EBI_D11", "type": "EBI_D11", "direction": "", "latch": "", "abcd": "A"}, #PE3
				{"pin": 11, "name": "EBI_D0", "type": "EBI_D0", "direction": "", "latch": "", "abcd": "A"}, #PC0
				{"pin": 15, "name": "GFX_DISP_INTF_PIN_HSYNC", "type": "GPIO", "direction": "Out", "latch": "High", "abcd": ""}, #PC30
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
				{"pin": 67, "name": "GFX_DISP_INTF_PIN_VSYNC", "type": "GPIO", "direction": "Out", "latch": "High", "abcd": ""}, #PD19
				{"pin": 71, "name": "BSP_MAXTOUCH_CHG", "type": "GPIO", "direction": "In", "latch": "", "abcd": ""}, #PD28
				{"pin": 77, "name": "TWIHS0_TWCK0", "type": "TWIHS0_TWCK0", "direction": "", "latch": "", "abcd": "A"}, #PA4
				{"pin": 82, "name": "EBI_NWR0/NWE", "type": "EBI_NWR0/NWE", "direction": "", "latch": "", "abcd": "A"}, #PC8
				{"pin": 86, "name": "TC2_TIOB7", "type": "TC2_TIOB7", "direction": "", "latch": "", "abcd": "B"}, #PC9, backlight via TC2
				{"pin": 91, "name": "TWIHS0_TWD0", "type": "TWIHS0_TWD0", "direction": "", "latch": "", "abcd": "A"}, #PA8
				{"pin": 94, "name": "GFX_DISP_INTF_PIN_DE", "type": "GPIO", "direction": "Out", "latch": "High", "abcd": ""}] #PC11
##################################################################################

############ SSD1963 CONFIG ######################################################
ssd1963ActivateList = ["smc", "le_gfx_intf_parallel_smc", "le_gfx_driver_ssd1963", "twihs0", "drv_i2c", "drv_i2c0", "tc0", "sys_time"]
ssd1963AutoConnectList = [["le_gfx_driver_ssd1963", "Graphics Display", "gfx_disp_pdatm4301b_480x272", "gfx_display"],
						["le_gfx_driver_ssd1963", "Display Interface", "le_gfx_intf_parallel_smc", "le_gfx_intf_parallel_smc"],
						["le_gfx_intf_parallel_smc", "SMC_CS", "smc", "smc_cs0"],
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

def eventHandlerLCC(event):
	lccBacklightAutoConnectList = [["le_gfx_driver_lcc", "TMR", "tc2", "TC2_TMR"]]
	if (event == "configure"):
		print("Configuring for LCC")
		try:
			Database.setSymbolValue("le_gfx_driver_lcc", "PeripheralControl", "TC", 1)
			Database.connectDependencies(lccBacklightAutoConnectList)
			Database.setSymbolValue("tc2", "TC1_ENABLE", True, 1)
			Database.setSymbolValue("tc2", "TC1_OPERATING_MODE", "COMPARE", 1)
			Database.setSymbolValue("tc2", "TC1_OPERATING_MODE", "COMPARE", 1)
			Database.setSymbolValue("le_gfx_driver_lcc", "TCInstance", 2, 1)
			Database.setSymbolValue("le_gfx_driver_lcc", "TCChannel", 1, 1)
			Database.setSymbolValue("le_gfx_driver_lcc", "TCChannelCompare", "B", 1)
			print("Done confguring backlight")
		except:
			print("Failed to configure backlight")
			return

bspDisplayInterfaceList = ["LCC", "SSD1963"]
#bspDisplayInterfaceList = ["LCC"]

sam_e70_xplained_utra_lcc = bspSupportObj(lccPinConfig, lccActivateList, None, lccAutoConnectList, eventHandlerLCC)
sam_e70_xplained_utra_ssd1963 = bspSupportObj(ssd1963PinConfig, ssd1963ActivateList, None, ssd1963AutoConnectList, eventHandlerSSD1963)

addBSPSupport("BSP_SAM_E70_Xplained_Ultra", "LCC", sam_e70_xplained_utra_lcc)
addBSPSupport("BSP_SAM_E70_Xplained_Ultra", "SSD1963", sam_e70_xplained_utra_ssd1963)
addDisplayIntfSupport("BSP_SAM_E70_Xplained_Ultra", bspDisplayInterfaceList)