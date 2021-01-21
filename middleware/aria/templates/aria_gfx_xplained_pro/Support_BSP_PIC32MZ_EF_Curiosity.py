# coding: utf-8
##############################################################################
# Copyright (C) 2019 Microchip Technology Inc. and its subsidiaries.
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

############ ILI9488 8080 + TOUCH I2C CONFIG ######################################################
bsp_pic32mzef_cu_ActivateList_ILI9488_8080 = ["gfx_driver_ili9488", "gfx_intf_parallel_ebi", "i2c2", "drv_i2c", "drv_i2c0", "core_timer", "sys_time", "ebi"]
bsp_pic32mzef_cu_AutoConnectList_ILI9488_8080 = [["gfx_hal", "gfx_display_driver", "gfx_driver_ili9488", "gfx_driver_ili9488"],
					["drv_i2c_0", "drv_i2c_I2C_dependency", "i2c2", "I2C2_I2C"],
					["gfx_maxtouch_controller", "i2c", "drv_i2c_0", "drv_i2c"],
					["sys_time", "sys_time_TMR_dependency", "core_timer", "CORE_TIMER_TMR"],
					["gfx_driver_ili9488", "Display Interface", "gfx_intf_parallel_ebi", "gfx_intf_parallel_ebi"],
					["gfx_intf_parallel_ebi", "EBI_CS", "ebi", "ebi_cs0"]]
bsp_pic32mzef_cu_PinConfig_ILI9488_8080 = [{"pin": 104, "name": "BSP_MAXTOUCH_CHG", "type": "GPIO", "direction": "In", "latch": "", "abcd": ""}, #RD0
				{"pin": 53, "name": "GFX_DISP_INTF_PIN_RD", "type": "GPIO", "direction": "Out", "latch": "High", "abcd": ""}, #RK3
				{"pin": 51, "name": "GFX_DISP_INTF_PIN_WR", "type": "GPIO", "direction": "Out", "latch": "High", "abcd": ""}, #RK1
				{"pin": 52, "name": "GFX_DISP_INTF_PIN_CS", "type": "GPIO", "direction": "Out", "latch": "High", "abcd": ""}, #RK2
				{"pin": 37, "name": "GFX_DISP_INTF_PIN_BACKLIGHT", "type": "GPIO", "direction": "Out", "latch": "High", "abcd": ""}, #RF13
				{"pin": 29, "name": "GFX_DISP_INTF_PIN_RESET", "type": "GPIO", "direction": "Out", "latch": "Low", "abcd": ""}, #RJ14
				{"pin": 12, "name": "GFX_DISP_INTF_PIN_RSDC", "type": "GPIO", "direction": "Out", "latch": "High", "abcd": ""}] #RC3
##########################################################################################

############ ILI9488 4-LINE SPI + TOUCH I2C CONFIG ######################################################
bsp_pic32mzef_cu_ActivateList_ILI9488_SPI = ["gfx_driver_ili9488", "gfx_intf_spi4", "drv_spi", "drv_spi_0", "spi3", "core_timer", "sys_time"]
bsp_pic32mzef_cu_AutoConnectList_ILI9488_SPI = [["gfx_hal", "gfx_display_driver", "gfx_driver_ili9488", "gfx_driver_ili9488"],
					["drv_spi_0", "drv_spi_SPI_dependency", "spi3", "SPI3_SPI"],
					["sys_time", "sys_time_TMR_dependency", "core_timer", "CORE_TIMER_TMR"],
					["gfx_driver_ili9488", "Display Interface", "gfx_intf_spi4", "gfx_intf_spi4"],
					["gfx_intf_spi4", "DRV_SPI", "drv_spi_0", "drv_spi"]]
bsp_pic32mzef_cu_PinConfig_ILI9488_SPI = [{"pin": 34, "name": "GFX_DISP_INTF_PIN_BACKLIGHT", "type": "GPIO", "direction": "Out", "latch": "Low", "abcd": ""}, #RB2 - Dummy pin
				{"pin": 31, "name": "SDO3", "type": "SDO3", "direction": "", "latch": "", "abcd": ""}, #RB3
				{"pin": 25, "name": "SDI3", "type": "SDI3", "direction": "", "latch": "", "abcd": ""}, #RB5
				{"pin": 61, "name": "SCK3", "type": "SCK3", "direction": "", "latch": "", "abcd": ""}, #RB14
				{"pin": 62, "name": "GFX_DISP_INTF_PIN_CS", "type": "GPIO", "direction": "Out", "latch": "High", "abcd": ""}, #RB15
				{"pin": 92, "name": "GFX_DISP_INTF_PIN_RSDC", "type": "GPIO", "direction": "Out", "latch": "High", "abcd": ""}, #RK4
				{"pin": 94, "name": "GFX_DISP_INTF_PIN_RESET", "type": "GPIO", "direction": "Out", "latch": "Low", "abcd": ""}] #RK6
##########################################################################################

def bsp_pic32mzef_cu_EventHandler(event):
	global pinConfigureFxn
	global pinResetFxn
	#Override default pin configur function w/ PIC32M specific one
	pinConfigureFxn = configurePinsPIC32M
	pinResetFxn = resetPinsPIC32M
	if (event == "configure"):
		pinConfigureFxn = configurePinsPIC32M
		pinResetFxn = resetPinsPIC32M
		try:
			Database.setSymbolValue("gfx_driver_ili9488", "DrawBufferSize", "Pixel", 1)
		except:
			return

def bsp_pic32mzef_cu_EventHandler_SPI(event):
	global pinConfigureFxn
	global pinResetFxn
	#Override default pin configur function w/ PIC32M specific one
	pinConfigureFxn = configurePinsPIC32M
	pinResetFxn = resetPinsPIC32M
	if (event == "configure"):
		try:
			Database.setSymbolValue("gfx_driver_ili9488", "DrawBufferSize", "Pixel", 1)
			Database.setSymbolValue("spi3", "SPI_BAUD_RATE", 35000000, 1)
		except:
			return

bsp_pic32mzef_cu_DisplayInterfaceList = ["Parallel", "SPI 4-line"]

bsp_pic32mzef_cu_obj_ILI9488_8080 = bspSupportObj(bsp_pic32mzef_cu_PinConfig_ILI9488_8080,
										bsp_pic32mzef_cu_ActivateList_ILI9488_8080,
										None,
										bsp_pic32mzef_cu_AutoConnectList_ILI9488_8080,
										bsp_pic32mzef_cu_EventHandler)

bsp_pic32mzef_cu_obj_ILI9488_SPI = bspSupportObj(bsp_pic32mzef_cu_PinConfig_ILI9488_SPI,
										bsp_pic32mzef_cu_ActivateList_ILI9488_SPI,
										None,
										bsp_pic32mzef_cu_AutoConnectList_ILI9488_SPI,
										bsp_pic32mzef_cu_EventHandler_SPI)

addDisplayIntfSupport("BSP_PIC32MZ_EF_Curiosity_2.0", bsp_pic32mzef_cu_DisplayInterfaceList)
addBSPSupport("BSP_PIC32MZ_EF_Curiosity_2.0", "Parallel", bsp_pic32mzef_cu_obj_ILI9488_8080)
addBSPSupport("BSP_PIC32MZ_EF_Curiosity_2.0", "SPI 4-line", bsp_pic32mzef_cu_obj_ILI9488_SPI)

