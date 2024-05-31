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

############### LCC CONFIG #######################################################
lccActivateList = ["smc", "le_gfx_driver_lcc", "twihs0", "drv_i2c", "drv_i2c0", "tc2", "sys_time", "pwm0"]
lccAutoConnectList = [["le_gfx_driver_lcc", "SMC_CS", "smc", "smc_cs0"],
						["gfx_legato", "gfx_driver", "le_gfx_driver_lcc", "gfx_driver_lcc"],
						["le_gfx_driver_lcc", "Graphics Display", "gfx_disp_pdatm4301b_480x272", "gfx_display"],
						["drv_i2c_0", "drv_i2c_I2C_dependency", "twihs0", "TWIHS0_I2C"],
						["gfx_maxtouch_controller", "i2c", "drv_i2c_0", "drv_i2c"],
						["sys_time", "sys_time_TMR_dependency", "tc2", "TC2_TMR"]]

lccPinConfig = [{"pin": 120,  "name": "GFX_DISP_INTF_PIN_RESET", "type": "GPIO", "direction": "Out", "latch": "High", "abcd": "A"}, #PC20
				{"pin": 71,  "name": "BSP_MAXTOUCH_CHG", "type": "GPIO", "direction": "In", "latch": "", "abcd": "A"}, #PD28
				{"pin": 37, "name": "GFX_DISP_INTF_PIN_VSYNC", "type": "GPIO", "direction": "Out", "latch": "High", "abcd": "A"}, #PA22
				{"pin": 91, "name": "TOUCH_SDA", "type": "TWIHS0_TWD0", "direction": "", "latch": "", "abcd": "A"}, #PA3
				{"pin": 77, "name": "TOUCH_SCL", "type": "TWIHS0_TWCK0", "direction": "", "latch": "In", "abcd": "A"}, #PA4
				{"pin": 82, "name": "EBI_NWE_NWR0", "type": "EBI_NWR0/NWE", "direction": "Out", "latch": "", "abcd": "A"}, #PC8
				{"pin": 94, "name": "GFX_DISP_INTF_PIN_DE", "type": "GPIO", "direction": "Out", "latch": "High", "abcd": "A"}, #PC11
				{"pin": 106, "name": "EBI_NBS1_NWR1", "type": "EBI_NWR1/_NBS1", "direction": "Out", "latch": "", "abcd": "A"}, #PD15
				{"pin": 117, "name": "GFX_DISP_INTF_PIN_BACKLIGHT", "type": "PWM0_PWMH2", "direction": "Out", "latch": "High", "abcd": "A"}, #PC19*, Backlight
				{"pin": 92, "name": "LCD_DISPEN", "type": "GPIO", "direction": "Out", "latch": "High", "abcd": "D"}, #PD12, LCDDISP
				{"pin": 16, "name": "GFX_DISP_INTF_PIN_HSYNC", "type": "GPIO", "direction": "Out", "latch": "High", "abcd": "C"}, #PC29
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

def eventHandlerLCC(event):
	lccBacklightAutoConnectList = [["le_gfx_driver_lcc", "Backlight", "pwm0", "PWM0_PWM"],]
	if (event == "configure"):
		print("Configuring for LCC")
		try:
			Database.setSymbolValue("le_gfx_driver_lcc", "PeripheralControl", "PWM", 1)
			Database.connectDependencies(lccBacklightAutoConnectList)
			Database.setSymbolValue("pwm0", "PWM_CH_2_ENABLE", True, 1)
			Database.setSymbolValue("pwm0", "PWM_CH_2_CPRD", 1500, 1)
			Database.setSymbolValue("pwm0", "PWM_CH_2_CDTY", 750, 1)
			Database.setSymbolValue("le_gfx_driver_lcc", "PWMInstance", 0, 1)
			Database.setSymbolValue("le_gfx_driver_lcc", "PWMChannel", 2, 1)
			print("Done confguring backlight")
		except:
			print("Failed to configure backlight")
		return

bspDisplayInterfaceList = ["LCC"]

pic32cz_ca70_mc70_curiosity_ultra_lcc = bspSupportObj(lccPinConfig, lccActivateList, None, lccAutoConnectList, eventHandlerLCC)

addBSPSupport("BSP_PIC32CZ_CA70_Curiosity_Ultra", "LCC", pic32cz_ca70_mc70_curiosity_ultra_lcc)
addDisplayIntfSupport("BSP_PIC32CZ_CA70_Curiosity_Ultra", bspDisplayInterfaceList)