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

############ SPI CONFIG ######################################################
pic32cx_sg4x_cult_cpro_spi_ActivateList = ["le_gfx_intf_spi4", "sercom0", "drv_spi", "drv_spi_0","sercom6", "drv_i2c", "drv_i2c0", "tc0", "sys_time"]
pic32cx_sg4x_cult_cpro_spi_ConnectList = [["le_gfx_driver_ili9488", "SPI Display Interface", "le_gfx_intf_spi4", "le_gfx_intf_spi4"],
						["le_gfx_intf_spi4", "DRV_SPI", "drv_spi_0", "drv_spi"],
						["drv_spi_0", "drv_spi_SPI_dependency", "sercom0", "SERCOM0_SPI"],
						["gfx_maxtouch_controller", "i2c", "drv_i2c_0", "drv_i2c"],
						["drv_i2c_0", "drv_i2c_I2C_dependency", "sercom6", "SERCOM6_I2C"],
						["sys_time", "sys_time_TMR_dependency", "tc0", "TC0_TMR"]]
pic32cx_sg4x_cult_cpro_spi_PinConfig = [{"pin": 43, "name": "GFX_DISP_INTF_PIN_BACKLIGHT", "type": "GPIO", "direction": "Out", "latch": "High", "abcd": ""}, #PB14
						{"pin": 47, "name": "SERCOM6_PAD1", "type": "SERCOM6_PAD1", "direction": "", "latch": "", "abcd": ""}, #PD08
						{"pin": 48, "name": "SERCOM6_PAD0", "type": "SERCOM6_PAD0", "direction": "", "latch": "", "abcd": ""}, #PD09
						{"pin": 49, "name": "GFX_DISP_INTF_PIN_RSDC", "type": "GPIO", "direction": "Out", "latch": "High", "abcd": ""}, #PD10
						{"pin": 51, "name": "BSP_MAXTOUCH_CHG", "type": "GPIO", "direction": "In", "latch": "", "abcd": ""}, #PD12
						{"pin": 100, "name": "SERCOM0_PAD0", "type": "SERCOM0_PAD0", "direction": "", "latch": "", "abcd": ""}, #PB24
						{"pin": 101, "name": "SERCOM0_PAD1", "type": "SERCOM0_PAD1", "direction": "", "latch": "", "abcd": ""}, #PB25
						{"pin": 105, "name": "GFX_DISP_INTF_PIN_RESET", "type": "GPIO", "direction": "Out", "latch": "High", "abcd": ""}, #PB29
						{"pin": 108, "name": "GFX_DISP_INTF_PIN_CS", "type": "GPIO", "direction": "Out", "latch": "High", "abcd": ""}, #PC24
						{"pin": 109, "name": "SERCOM0_PAD3", "type": "SERCOM0_PAD3", "direction": "", "latch": "", "abcd": ""}] #PC25
##################################################################################

def pic32cx_sg4x_cult_cpro_spi_eventHandler(event):
	if (event == "configure"):
		#set the SPI clock to 8MHz
		try:
			Database.setSymbolValue("sercom0", "SPI_BAUD_RATE", 8000000, 1)
			Database.setSymbolValue("sercom0", "SPI_DIPO", 3, 1)
		except:
			print("Unable to configure SPI.")
		
		try:
			#Set Sercom/I2C baud to 400kHz
			Database.setSymbolValue("sercom6", "I2C_CLOCK_SPEED", 400, 1)
		except:
			print("Unable to configure I2C.")
			

pic32cx_sg4x_cult_dispintf = ["SPI 4-Line"]

### Use bit-bang w/ 24-bit passthrough GFX interface card
pic32cx_sg4x_cult_cpro_spi = bspSupportObj(pic32cx_sg4x_cult_cpro_spi_PinConfig, pic32cx_sg4x_cult_cpro_spi_ActivateList, None, pic32cx_sg4x_cult_cpro_spi_ConnectList, pic32cx_sg4x_cult_cpro_spi_eventHandler)

addBSPSupport("BSP_PIC32CX_SG41_Curiosity_Ultra", "SPI 4-Line", pic32cx_sg4x_cult_cpro_spi)
addDisplayIntfSupport("BSP_PIC32CX_SG41_Curiosity_Ultra", pic32cx_sg4x_cult_dispintf)

