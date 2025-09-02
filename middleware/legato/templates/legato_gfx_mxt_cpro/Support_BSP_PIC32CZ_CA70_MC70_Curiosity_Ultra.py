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

############### ILI9488 Parallel 8080 CONFIG #######################################################
ILI80ActivateList = ["smc", "le_gfx_intf_parallel_smc","le_gfx_driver_external", "twihs0", "drv_i2c", "drv_i2c0", "tc2", "sys_time"]
ILI80AutoConnectList = [["le_gfx_driver_external", "Parallel Display Interface", "le_gfx_intf_parallel_smc", "le_gfx_intf_parallel_smc"],
						["le_gfx_intf_parallel_smc", "SMC_CS", "smc", "smc_cs2"],
						["drv_i2c_0", "drv_i2c_I2C_dependency", "twihs0", "TWIHS0_I2C"],
						["gfx_maxtouch_controller", "i2c", "drv_i2c_0", "drv_i2c"],
						["sys_time", "sys_time_TMR_dependency", "tc2", "TC2_TMR"]]

ILI80PinConfig = [{"pin": 120,  "name": "GFX_DISP_INTF_PIN_RESET", "type": "GPIO", "direction": "Out", "latch": "High", "abcd": "A"}, #PC20
				{"pin": 71,  "name": "BSP_MAXTOUCH_CHG", "type": "GPIO", "direction": "In", "latch": "", "abcd": "A"}, #PD28
				{"pin": 37, "name": "GFX_DISP_INTF_PIN_CS", "type": "GPIO", "direction": "Out", "latch": "High", "abcd": "A"}, #PA22
				{"pin": 91, "name": "TOUCH_SDA", "type": "TWIHS0_TWD0", "direction": "", "latch": "", "abcd": "A"}, #PA3
				{"pin": 77, "name": "TOUCH_SCL", "type": "TWIHS0_TWCK0", "direction": "", "latch": "In", "abcd": "A"}, #PA4
				{"pin": 82, "name": "EBI_NWE_NWR0", "type": "EBI_NWR0/NWE", "direction": "", "latch": "", "abcd": "A"}, #PC8
				{"pin": 94, "name": "EBI_NRD", "type": "EBI_NRD", "direction": "", "latch": "", "abcd": "A"}, #PC11
				{"pin": 106, "name": "EBI_NBS1_NWR1", "type": "EBI_NWR1/NBS1", "direction": "", "latch": "", "abcd": "A"}, #PD15
				{"pin": 117, "name": "GFX_DISP_INTF_PIN_BACKLIGHT", "type": "GPIO", "direction": "Out", "latch": "High", "abcd": "A"}, #PC19*, Backlight
				{"pin": 92, "name": "LCD_DISPEN", "type": "GPIO", "direction": "Out", "latch": "High", "abcd": "D"}, #PD12, LCDDISP
				{"pin": 111, "name": "EBI_A0_NBS0", "type": "EBI_A0/NBS0", "direction": "", "latch": "", "abcd": "A"}, #PC18
				{"pin": 16, "name": "EBI_A11", "type": "EBI_A11", "direction": "", "latch": "", "abcd": "C"}, #PC29
				{"pin": 11, "name": "EBI_D0", "type": "EBI_D0", "direction": "", "latch": "", "abcd": "A"}, #PC0
				{"pin": 38, "name": "EBI_D1", "type": "EBI_D1", "direction": "", "latch": "", "abcd": "A"}, #PC1
				{"pin": 39, "name": "EBI_D2", "type": "EBI_D2", "direction": "", "latch": "", "abcd": "A"}, #PC2
				{"pin": 40, "name": "EBI_D3", "type": "EBI_D3", "direction": "", "latch": "", "abcd": "A"}, #PC3
				{"pin": 58, "name": "EBI_D5", "type": "EBI_D5", "direction": "", "latch": "", "abcd": "A"}, #PC5
				{"pin": 41, "name": "EBI_D4", "type": "EBI_D4", "direction": "", "latch": "", "abcd": "A"}, #PC4
				{"pin": 54, "name": "EBI_D6", "type": "EBI_D6", "direction": "", "latch": "", "abcd": "A"}, #PC6
				{"pin": 48, "name": "EBI_D7", "type": "EBI_D7", "direction": "", "latch": "", "abcd": "A"}, #PC7
				{"pin": 4, "name": "EBI_D8", "type": "EBI_D8", "direction": "", "latch": "", "abcd": "A"}, #PE0
				{"pin": 6, "name": "EBI_D9", "type": "EBI_D9", "direction": "", "latch": "", "abcd": "A"}, #PE1
				{"pin": 7, "name": "EBI_D10", "type": "EBI_D10", "direction": "", "latch": "", "abcd": "A"}, #PE2
				{"pin": 10, "name": "EBI_D11", "type": "EBI_D11", "direction": "", "latch": "", "abcd": "A"}, #PE3
				{"pin": 27, "name": "EBI_D12", "type": "EBI_D12", "direction": "", "latch": "", "abcd": "A"}, #PE4
				{"pin": 28, "name": "EBI_D13", "type": "EBI_D13", "direction": "", "latch": "", "abcd": "A"}, #PE5
				{"pin": 49, "name": "EBI_D14", "type": "EBI_D14", "direction": "", "latch": "", "abcd": "A"}, #PA15
				{"pin": 45, "name": "EBI_D15", "type": "EBI_D15", "direction": "", "latch": "", "abcd": "A"}] #PA16
##################################################################################

def eventHandlerILI80(event):
	ILIParallelList = [["le_gfx_driver_external", "Parallel Display Interface", "le_gfx_intf_parallel_smc", "le_gfx_intf_parallel_smc"],]
	if (event == "configure"):
		print("Configuring for LE EXTERNAL CONTROLLER")
		try:
			Database.setSymbolValue("le_gfx_driver_external", "BaseDriverType", "ILI9488", 1)
			Database.setSymbolValue("le_gfx_driver_external", "DisplayInterfaceType", "Parallel 8080", 1)
			#Database.connectDependencies(ILIParallelList)
			Database.setSymbolValue("le_gfx_intf_parallel_smc", "EBIChipSelectIndex", 2, 1)
			Database.setSymbolValue("le_gfx_intf_parallel_smc", "DataCommandSelectControl", "Peripheral", 1)
			Database.setSymbolValue("le_gfx_intf_parallel_smc", "DCXAddressBit", 11, 1)
			Database.setSymbolValue("le_gfx_intf_parallel_smc", "ReadStrobeControl", "Peripheral", 1)
			Database.setSymbolValue("le_gfx_intf_parallel_smc", "WriteStrobeControl", "Peripheral", 1)
			
		except:
			print("Failed to configure LE EXTERNAL CONTROLLER for ILI9488 Parallel 8080")
		return

bspDisplayInterfaceList = ["Parallel"]

pic32cz_ca70_mc70_curiosity_ultra_ILI80 = bspSupportObj(ILI80PinConfig, ILI80ActivateList, None, ILI80AutoConnectList, eventHandlerILI80)

addBSPSupport("BSP_PIC32CZ_CA70_Curiosity_Ultra", "Parallel", pic32cz_ca70_mc70_curiosity_ultra_ILI80)
addDisplayIntfSupport("BSP_PIC32CZ_CA70_Curiosity_Ultra", bspDisplayInterfaceList)