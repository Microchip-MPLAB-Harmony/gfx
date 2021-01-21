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
e54cult_ssd1963ActivateList = ["le_gfx_intf_parallel_portgroup", "le_gfx_driver_ssd1963", "sercom4", "drv_i2c", "drv_i2c0", "tc0", "sys_time"]
e54cult_ssd1963AutoConnectList = [["le_gfx_driver_ssd1963", "Graphics Display", "gfx_disp_pdatm7000b_800x480", "gfx_display"],
				["le_gfx_driver_ssd1963", "Display Interface", "le_gfx_intf_parallel_portgroup", "le_gfx_intf_parallel_portgroup"],
  				["gfx_legato", "gfx_driver", "le_gfx_driver_ssd1963", "gfx_driver_ssd1963"],
				["drv_i2c_0", "drv_i2c_I2C_dependency", "sercom4", "SERCOM4_I2C"],
				["gfx_maxtouch_controller", "i2c", "drv_i2c_0", "drv_i2c"],
				["sys_time", "sys_time_TMR_dependency", "tc0", "TC0_TMR"]]
e54cult_ssd1963PinConfig = [{"pin": 3, "name": "GPIO_PC00", "type": "GPIO", "direction": "Out", "latch": "Low", "abcd": ""}, #PC00
				{"pin": 4, "name": "GPIO_PC01", "type": "GPIO", "direction": "Out", "latch": "Low", "abcd": ""}, #PC01
				{"pin": 7, "name": "GPIO_PC02", "type": "GPIO", "direction": "Out", "latch": "Low", "abcd": ""}, #PC02
				{"pin": 8, "name": "GPIO_PC03", "type": "GPIO", "direction": "Out", "latch": "Low", "abcd": ""}, #PC03
				{"pin": 11, "name": "GFX_DISP_INTF_PIN_CS", "type": "GPIO", "direction": "Out", "latch": "High", "abcd": ""}, #PB04
				{"pin": 19, "name": "GFX_DISP_INTF_PIN_RD", "type": "GPIO", "direction": "Out", "latch": "High", "abcd": ""}, #PB08
				{"pin": 20, "name": "GFX_DISP_INTF_PIN_WR", "type": "GPIO", "direction": "Out", "latch": "High", "abcd": ""}, #PB09
				{"pin": 27, "name": "GPIO_PC04", "type": "GPIO", "direction": "Out", "latch": "Low", "abcd": ""}, #PC04
				{"pin": 28, "name": "GPIO_PC05", "type": "GPIO", "direction": "Out", "latch": "Low", "abcd": ""}, #PC05
				{"pin": 29, "name": "GPIO_PC06", "type": "GPIO", "direction": "Out", "latch": "Low", "abcd": ""}, #PC06
				{"pin": 30, "name": "GPIO_PC07", "type": "GPIO", "direction": "Out", "latch": "Low", "abcd": ""}, #PC07
				{"pin": 41, "name": "SERCOM4_PAD0", "type": "SERCOM4_PAD0", "direction": "", "latch": "", "abcd": ""}, #PB12
				{"pin": 42, "name": "SERCOM4_PAD1", "type": "SERCOM4_PAD1", "direction": "", "latch": "", "abcd": ""}, #PB13
				{"pin": 70, "name": "BSP_MAXTOUCH_CHG", "type": "GPIO", "direction": "In", "latch": "", "abcd": ""}, #PC16
				{"pin": 72, "name": "GFX_DISP_INTF_PIN_RESET", "type": "GPIO", "direction": "Out", "latch": "High", "abcd": ""}, #PC18
				{"pin": 83, "name": "GFX_DISP_INTF_PIN_RSDC", "type": "GPIO", "direction": "Out", "latch": "High", "abcd": ""}] #PB17
##################################################################################

def e54cult_eventHandlerSSD1963(event):
	if (event == "configure"):
		try:
			#Switch IO Group = 2
			Database.setSymbolValue("le_gfx_intf_parallel_portgroup", "PortGroup", "2", 1)
			#Use 8-bit interface for 
			Database.setSymbolValue("le_gfx_driver_ssd1963", "ParallelInterfaceWidth", "8-bit", 1)
			#Set Sercom/I2C baud to 400kHz
			Database.setSymbolValue("sercom4", "I2C_CLOCK_SPEED", 400, 1)
		except:
			return

e54cult_xbspDisplayInterfaceList = ['SSD1963']

sam_e54_curiosity_ultra_ssd1963 = bspSupportObj(e54cult_ssd1963PinConfig, e54cult_ssd1963ActivateList, None, e54cult_ssd1963AutoConnectList, e54cult_eventHandlerSSD1963)

addBSPSupport("BSP_SAM_E54_Curiosity_Ultra", "SSD1963", sam_e54_curiosity_ultra_ssd1963)
addDisplayIntfSupport("BSP_SAM_E54_Curiosity_Ultra", e54cult_xbspDisplayInterfaceList)
