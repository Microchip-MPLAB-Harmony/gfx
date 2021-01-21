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

############ LCC + TOUCH I2C CONFIG ######################################################
bsp_pic32mzef_sk_meb_ActivateList = ["le_gfx_driver_lcc", "i2c_bb", "tmr2", "drv_i2c", "drv_i2c0", "core_timer", "sys_time", "ebi"]
bsp_pic32mzef_sk_meb_AutoConnectList = [["gfx_legato", "gfx_driver", "le_gfx_driver_lcc", "gfx_driver_lcc"],
										["le_gfx_driver_lcc", "Graphics Display", "gfx_disp_pdatm4301b_480x272", "gfx_display"],
										["drv_i2c_0", "drv_i2c_I2C_dependency", "i2c_bb", "I2C"],
										["i2c_bb", "TMR", "tmr2", "TMR2_TMR"],
										["gfx_maxtouch_controller", "i2c", "drv_i2c_0", "drv_i2c"],
										["sys_time", "sys_time_TMR_dependency", "core_timer", "CORE_TIMER_TMR"],
										["le_gfx_driver_lcc", "EBI_CS", "ebi", "ebi_cs0"]]
bsp_pic32mzef_sk_meb_PinConfig = [{"pin": 23, "name": "BSP_MAXTOUCH_CHG", "type": "GPIO", "direction": "In", "latch": "", "abcd": ""}, #RE8
				{"pin": 26, "name": "GFX_DISP_INTF_PIN_DE", "type": "GPIO", "direction": "Out", "latch": "Low", "abcd": ""}, #RB4
				{"pin": 35, "name": "GFX_DISP_INTF_PIN_HSYNC", "type": "GPIO", "direction": "Out", "latch": "Low", "abcd": ""}, #RB1
				{"pin": 39, "name": "GFX_DISP_INTF_PIN_VSYNC", "type": "GPIO", "direction": "Out", "latch": "Low", "abcd": ""}, #RA9
				{"pin": 57, "name": "GFX_DISP_INTF_PIN_BACKLIGHT", "type": "GPIO", "direction": "Out", "latch": "Low", "abcd": ""}, #RF13
				{"pin": 117, "name": "GFX_DISP_INTF_PIN_RESET", "type": "GPIO", "direction": "Out", "latch": "High", "abcd": ""},
				{"pin": 95, "name": "I2C_BB_SCL", "type": "GPIO", "direction": "In", "latch": "", "abcd": ""}, #RA14 - GPIO input for I2C BB SCL
				{"pin": 96, "name": "I2C_BB_SDA", "type": "GPIO", "direction": "In", "latch": "", "abcd": ""}] #RA15 - GPIO input for I2C BB SDA
##################################################################################

def bsp_pic32mzef_sk_meb_EventHandler(event):
	global pinConfigureFxn
	if (event == "configure"):
		#Override default pin configur function w/ PIC32M specific one
		pinConfigureFxn = configurePinsPIC32M
		try:
			### Configure I2C BB driver
			Database.setSymbolValue("i2c_bb", "I2C_CLOCK_SPEED", 50000, 1)
			Database.setSymbolValue("i2c_bb", "I2CBB_SCL_PIN", 10, 1) #RA14
			Database.setSymbolValue("i2c_bb", "I2CBB_SDA_PIN", 11, 1) #RA15
		except:
			return

bsp_pic32mzef_sk_meb_DisplayInterfaceList = ["LCC"]

bsp_pic32mzef_sk_meb_obj = bspSupportObj(bsp_pic32mzef_sk_meb_PinConfig,
										bsp_pic32mzef_sk_meb_ActivateList,
										None,
										bsp_pic32mzef_sk_meb_AutoConnectList,
										bsp_pic32mzef_sk_meb_EventHandler)

addDisplayIntfSupport("BSP_PIC32MZ_EF_Starter_Kit", bsp_pic32mzef_sk_meb_DisplayInterfaceList)
addBSPSupport("BSP_PIC32MZ_EF_Starter_Kit", "LCC", bsp_pic32mzef_sk_meb_obj)