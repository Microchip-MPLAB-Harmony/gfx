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

spi4PinComponentIDList = ["sercom4", "drv_spi", "drv_spi_0"]
spi4PinAutoConnectList = [["drv_spi_0", "drv_spi_SPI_dependency", "sercom4", "SERCOM4_SPI"]]
spi4PinConfigs = [{"pin": 23, "name": "GFX_DISP_INTF_PIN_RSDC", "type": "GPIO", "direction": "Out", "latch": "High", "abcd": ""}, #PA06
				{"pin": 104, "name": "GFX_DISP_INTF_PIN_CS", "type": "GPIO", "direction": "Out", "latch": "High", "abcd": ""}, #PB28
				{"pin": 103, "name": "SERCOM4_PAD0", "type": "SERCOM4_PAD0", "direction": "", "latch": "", "abcd": "C"}, #PB27
				{"pin": 105, "name": "SERCOM4_PAD3", "type": "SERCOM4_PAD3", "direction": "", "latch": "", "abcd": "C"}, #PB29
				{"pin": 102, "name": "SERCOM4_PAD1", "type": "SERCOM4_PAD1", "direction": "", "latch": "", "abcd": "C"}, #PB26
				{"pin": 113, "name": "GFX_DISP_INTF_PIN_RESET", "type": "GPIO", "direction": "Out", "latch": "High", "abcd": ""}] #PA27

def eventHandlerSPI4line(event):
	if (event == "configure"):
		#set the SPI clock to 8MHz
		try:
			Database.setSymbolValue("sercom4", "SPI_BAUD_RATE", 8000000, 1)
		except:
			print("Unable to configure SPI baud rate.")
		
		try:
			Database.setSymbolValue("sercom4", "SPI_DIPO", 3, 1)
		except:
			print("Unable to configure SERCOM4 DIPO Pad.")
		### set heap to 16kB for C21
		Database.setSymbolValue("core", "XC32_HEAP_SIZE", 16384, 0)

sam_e54_oled1_xpro_SPI = bspSupportObj(spi4PinConfigs, spi4PinComponentIDList, None, spi4PinAutoConnectList, eventHandlerSPI4line)

addBSPSupport("BSP_SAM_E54_Xplained_Pro", "SPI 4-line", sam_e54_oled1_xpro_SPI)