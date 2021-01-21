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

spi4PinComponentIDList = ["spi0", "drv_spi", "drv_spi_0"]
spi4PinAutoConnectList = [["drv_spi_0", "drv_spi_SPI_dependency", "spi0", "SPI0_SPI"]]
spi4PinConfigs = [{"pin": 31, "name": "GFX_DISP_INTF_PIN_RSDC", "type": "GPIO", "direction": "Out", "latch": "High", "abcd": ""}, #PB3
				{"pin": 52, "name": "GFX_DISP_INTF_PIN_CS", "type": "GPIO", "direction": "Out", "latch": "High", "abcd": ""}, #PD25
				{"pin": 60, "name": "SPI0_SPCK", "type": "SPI0_SPCK", "direction": "", "latch": "", "abcd": "B"}, #PD22
				{"pin": 63, "name": "SPI0_MOSI", "type": "SPI0_MOSI", "direction": "", "latch": "", "abcd": "B"}, #PD21
				{"pin": 65, "name": "SPI0_MISO", "type": "SPI0_MISO", "direction": "", "latch": "", "abcd": "B"}, #PD20
				{"pin": 103, "name": "GFX_DISP_INTF_PIN_RESET", "type": "GPIO", "direction": "Out", "latch": "High", "abcd": ""}] #PC17

def eventHandlerSPI4line(event):
	if (event == "configure"):
		#set the SPI clock to 20MHz
		try:
			Database.setSymbolValue("spi0", "SPI_BAUD_RATE", 20000000, 1)
		except:
			print("Unable to configure SPI baud rate.")

sam_e70_oled1_xpro_SPI = bspSupportObj(spi4PinConfigs, spi4PinComponentIDList, None, spi4PinAutoConnectList, eventHandlerSPI4line)

addBSPSupport("BSP_SAM_E70_Xplained_Ultra", "SPI 4-line", sam_e70_oled1_xpro_SPI)
