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
lccActivateList = ["ebi", "le_gfx_driver_lcc", "sercom0", "drv_i2c", "drv_i2c0", "rtc", "sys_time", "tcc9", "tcc0"]
lccAutoConnectList = [["le_gfx_driver_lcc", "EBI_CS", "ebi", "ebi_cs0"],
						["gfx_legato", "gfx_driver", "le_gfx_driver_lcc", "gfx_driver_lcc"],
						["le_gfx_driver_lcc", "Graphics Display", "gfx_disp_pdatm4301b_480x272", "gfx_display"],
						["drv_i2c_0", "drv_i2c_I2C_dependency", "sercom0", "SERCOM0_I2C"],
						["gfx_maxtouch_controller", "i2c", "drv_i2c_0", "drv_i2c"],
						["sys_time", "sys_time_TMR_dependency", "rtc", "RTC_TMR"]]

lccPinConfig = [{"pin": 19,  "name": "GFX_DISP_INTF_PIN_RESET", "type": "GPIO", "direction": "Out", "latch": "High", "abcd": ""}, #PC13
				{"pin": 69,  "name": "PHY_TRISTATE_PULLDOWN", "type": "GPIO", "direction": "Out", "latch": "Low", "abcd": ""}, #PB23
				{"pin": 72,  "name": "BSP_MAXTOUCH_CHG", "type": "GPIO", "direction": "In", "latch": "", "abcd": ""}, #PD28
				{"pin": 105, "name": "GFX_DISP_INTF_PIN_VSYNC", "type": "GPIO", "direction": "Out", "latch": "High", "abcd": ""}, #PD19
				{"pin": 106, "name": "TOUCH_SDA", "type": "SERCOM0_PAD0", "direction": "", "latch": "", "abcd": ""}, #PA4
				{"pin": 107, "name": "TOUCH_SCL", "type": "SERCOM0_PAD1", "direction": "", "latch": "In", "abcd": ""}, #PA8
				{"pin": 110, "name": "EBI_NWE_NWR0", "type": "EBI_NWE_NWR0", "direction": "Out", "latch": "", "abcd": ""}, #PC6
				{"pin": 113, "name": "GFX_DISP_INTF_PIN_DE", "type": "GPIO", "direction": "Out", "latch": "High", "abcd": ""}, #PC11
				{"pin": 114, "name": "EBI_NBS1_NWR1", "type": "EBI_NBS1_NWR1", "direction": "Out", "latch": "", "abcd": ""}, #PC6
				{"pin": 129, "name": "GFX_DISP_INTF_PIN_BACKLIGHT", "type": "GPIO", "direction": "Out", "latch": "High", "abcd": ""}, #PC9, Backlight
				{"pin": 142, "name": "GFX_DISP_INTF_PIN_HSYNC", "type": "GPIO", "direction": "Out", "latch": "High", "abcd": ""}, #PC30
				{"pin": 179, "name": "EBI_D0", "type": "EBI_D0", "direction": "", "latch": "", "abcd": ""}, #PC0
				{"pin": 180, "name": "EBI_D1", "type": "EBI_D1", "direction": "", "latch": "", "abcd": ""}, #PC1
				{"pin": 181, "name": "EBI_D2", "type": "EBI_D2", "direction": "", "latch": "", "abcd": ""}, #PC2
				{"pin": 182, "name": "EBI_D3", "type": "EBI_D3", "direction": "", "latch": "", "abcd": ""}, #PC3
				{"pin": 183, "name": "EBI_D5", "type": "EBI_D5", "direction": "", "latch": "", "abcd": ""}, #PC5
				{"pin": 190, "name": "EBI_D4", "type": "EBI_D4", "direction": "", "latch": "", "abcd": ""}, #PC4
				{"pin": 191, "name": "EBI_D6", "type": "EBI_D6", "direction": "", "latch": "", "abcd": ""}, #PC6
				{"pin": 192, "name": "EBI_D7", "type": "EBI_D7", "direction": "", "latch": "", "abcd": ""}, #PC7
				{"pin": 193, "name": "EBI_D8", "type": "EBI_D8", "direction": "", "latch": "", "abcd": ""}, #PE0
				{"pin": 194, "name": "EBI_D9", "type": "EBI_D9", "direction": "", "latch": "", "abcd": ""}, #PE1
				{"pin": 195, "name": "EBI_D10", "type": "EBI_D10", "direction": "", "latch": "", "abcd": ""}, #PE2
				{"pin": 196, "name": "EBI_D11", "type": "EBI_D11", "direction": "", "latch": "", "abcd": ""}, #PE3
				{"pin": 197, "name": "EBI_D12", "type": "EBI_D12", "direction": "", "latch": "", "abcd": ""}, #PE4
				{"pin": 198, "name": "EBI_D13", "type": "EBI_D13", "direction": "", "latch": "", "abcd": ""}, #PE5
				{"pin": 199, "name": "EBI_D14", "type": "EBI_D14", "direction": "", "latch": "", "abcd": ""}, #PA15
				{"pin": 200, "name": "EBI_D15", "type": "EBI_D15", "direction": "", "latch": "", "abcd": ""}] #PA16
##################################################################################

def eventHandlerLCC(event):
	lccBacklightAutoConnectList = [["le_gfx_driver_lcc", "Backlight", "tcc9", "TCC9_PWM"],
    ["le_gfx_driver_lcc", "TCC", "tcc0", "TCC0_PWM"]]
	if (event == "configure"):
		print("Configuring for LCC")
		try:
			Database.setSymbolValue("le_gfx_driver_lcc", "PeripheralControl", "TCC", 1)
			Database.connectDependencies(lccBacklightAutoConnectList)
			Database.setSymbolValue("tcc0", "TCC_OPERATION_MODE", "PWM", 1)
			Database.setSymbolValue("tcc0", "TCC_PER_PER", long(1), 1)
			Database.setSymbolValue("core", "GCLK_ID_31_GENSEL", 0, 1)
			Database.setSymbolValue("tcc9", "TCC_OPERATION_MODE", "PWM", 1)
			Database.setSymbolValue("tcc9", "TCC_EVCTRL_EVACT1", 0, 1)
			Database.setSymbolValue("le_gfx_driver_lcc", "TCInstance", 9, 1)
			Database.setSymbolValue("le_gfx_driver_lcc", "TCChannel", 1, 1)
			Database.setSymbolValue("le_gfx_driver_lcc", "TCChannelCompare", "B", 1)
			Database.setSymbolValue("ebi", "EBI_SMC_READ_ENABLE_MODE_CS0", 0, 1)
			print("Done confguring backlight")
		except:
			print("Failed to configure backlight")
			return

bspDisplayInterfaceList = ["LCC"]

pic32cz_ca80_curiosity_ultra_lcc = bspSupportObj(lccPinConfig, lccActivateList, None, lccAutoConnectList, eventHandlerLCC)

addBSPSupport("BSP_PIC32Z_CA80_Curiosity_Ultra", "LCC", pic32cz_ca80_curiosity_ultra_lcc)
addDisplayIntfSupport("BSP_PIC32Z_CA80_Curiosity_Ultra", bspDisplayInterfaceList)