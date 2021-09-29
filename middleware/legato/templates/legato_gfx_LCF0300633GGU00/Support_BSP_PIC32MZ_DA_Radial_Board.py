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

############ GLCD + maXTouch I2C CONFIG ######################################################
mzda_mxt_activateList = ["le_gfx_driver_glcd", "i2c1", "can1", "can2", "usb_device_cdc", "drv_sdmmc", "drv_sdmmc0", "sdhc", "sys_fs", "spi1", "drv_spi", "uart1", "uart4", "sys_console", "sys_console_0", "sys_console", "sys_console_1", "sys_debug", "sys_command", "i2c2", "drv_i2c", "drv_i2c0", "core_timer", "sys_time", "ddr", "le_gfx_driver_2dgpu", "gfx_maxtouch_controller"]
mzda_mxt_connectList = [["gfx_legato", "gfx_driver", "le_gfx_driver_glcd", "gfx_driver_glcd"],
					["le_gfx_driver_glcd", "Graphics Display", "gfx_disp_LCF0300633GGU00_432x432", "gfx_display"],
					["drv_i2c_0", "drv_i2c_I2C_dependency", "i2c2", "I2C2_I2C"],
					["gfx_maxtouch_controller", "touch_panel", "gfx_disp_LCF0300633GGU00_432x432", "touch_panel"],
					["gfx_maxtouch_controller", "i2c", "drv_i2c_0", "drv_i2c"],
					["spi1", "SPI1_SPI", "drv_spi_0", "drv_spi_SPI_dependency"],
					["uart4", "UART4_UART", "sys_console_0", "sys_console_UART_dependency"],
					["uart1", "UART1_UART", "sys_console_1", "sys_console_UART_dependency"],
					["sys_debug", "sys_debug_SYS_CONSOLE_dependency", "sys_console_0", "sys_console"],
					["sys_command", "sys_command_SYS_CONSOLE_dependency", "sys_console_1", "sys_console"],
					["sdhc", "SDHC_SDHC", "drv_sdmmc_0", "drv_sdmmc_SDHC_dependency"],
					["drv_sdmmc_0", "drv_media", "sys_fs", "sys_fs_DRV_MEDIA_dependency"],
					["sys_time", "sys_time_TMR_dependency", "core_timer", "CORE_TIMER_TMR"],
					["gfx_legato", "gpu_driver", "le_gfx_driver_2dgpu", "gfx_driver_2dgpu"]]
mzda_mxt_pinConfig = [{"pin": 42, "name": "BSP_MAXTOUCH_CHG", "type": "GPIO", "direction": "In", "latch": "", "abcd": ""}, #RH14
				{"pin": 79, "name": "BACKLIGHT_TYPE_1", "type": "GPIO", "direction": "Out", "latch": "High", "abcd": ""}, #RD0
				{"pin": 120, "name": "BACKLIGHT_TYPE_2", "type": "GPIO", "direction": "Out", "latch": "", "abcd": ""}, #RD7
				{"pin": 73, "name": "SCK1", "type": "SCK1", "direction": "", "latch": "", "abcd": ""}, #RD1
				{"pin": 167, "name": "SDI1", "type": "SDI1", "direction": "", "latch": "", "abcd": ""}, #RB5
				{"pin": 168, "name": "SDO1", "type": "SDO1", "direction": "", "latch": "", "abcd": ""}, #RB10
				{"pin": 169, "name": "SS1(out)", "type": "SS1 (out)", "direction": "", "latch": "", "abcd": ""}, #RB0
				{"pin": 31, "name": "CAN1RX", "type": "C1RX", "direction": "", "latch": "", "abcd": ""}, #RG7
				{"pin": 32, "name": "CAN1TX", "type": "C1TX", "direction": "", "latch": "", "abcd": ""}, #RG8
				{"pin": 26, "name": "CAN2RX", "type": "C2RX", "direction": "", "latch": "", "abcd": ""}, #RE9
				{"pin": 41, "name": "CAN2TX", "type": "C2TX", "direction": "", "latch": "", "abcd": ""}, #RF2
				{"pin": 126, "name": "SCL1", "type": "SCL1", "direction": "", "latch": "", "abcd": ""}, #RA14
				{"pin": 125, "name": "SDA1", "type": "SDA1", "direction": "", "latch": "", "abcd": ""}, #RA15
				{"pin": 129, "name": "SCL2", "type": "SCL2", "direction": "", "latch": "", "abcd": ""}, #RA2
				{"pin": 128, "name": "SDA2", "type": "SDA2", "direction": "", "latch": "", "abcd": ""}] #RA3

############ GLCD + Generic Touch I2C CONFIG ######################################################
mzda_gt_activateList = ["le_gfx_driver_glcd", "i2c1", "can1", "can2", "usb_device_cdc", "drv_sdmmc", "drv_sdmmc0", "sdhc", "sys_fs", "spi1", "drv_spi", "uart1", "uart4", "sys_console", "sys_console_0", "sys_console", "sys_console_1", "sys_debug", "sys_command", "gfx_hal_le", "i2c2", "drv_i2c", "drv_i2c0", "core_timer", "sys_time", "ddr", "le_gfx_driver_2dgpu", "gfx_generic_touch_controller"]
mzda_gt_connectList = [["gfx_legato", "gfx_driver", "le_gfx_driver_glcd", "gfx_driver_glcd"],
					["le_gfx_driver_glcd", "Graphics Display", "gfx_disp_LCF0300633GGU00_432x432", "gfx_display"],
					["drv_i2c_0", "drv_i2c_I2C_dependency", "i2c2", "I2C2_I2C"],
					["gfx_generic_touch_controller", "touch_panel", "gfx_disp_LCF0300633GGU00_432x432", "touch_panel"],
					["gfx_generic_touch_controller", "i2c", "drv_i2c_0", "drv_i2c"],
					["spi1", "SPI1_SPI", "drv_spi_0", "drv_spi_SPI_dependency"],
					["sdhc", "SDHC_SDHC", "drv_sdmmc_0", "drv_sdmmc_SDHC_dependency"],
					["drv_sdmmc_0", "drv_media", "sys_fs", "sys_fs_DRV_MEDIA_dependency"],
					["uart4", "UART4_UART", "sys_console_0", "sys_console_UART_dependency"],
					["uart1", "UART1_UART", "sys_console_1", "sys_console_UART_dependency"],
					["sys_debug", "sys_debug_SYS_CONSOLE_dependency", "sys_console_0", "sys_console"],
					["sys_command", "sys_command_SYS_CONSOLE_dependency", "sys_console_1", "sys_console"],
					["sys_time", "sys_time_TMR_dependency", "core_timer", "CORE_TIMER_TMR"],
					["gfx_legato", "gpu_driver", "le_gfx_driver_2dgpu", "gfx_driver_2dgpu"]]
mzda_gt_pinConfig = [{"pin": 42, "name": "DRV_TOUCH_PIN_INT", "type": "GPIO", "direction": "In", "latch": "", "abcd": ""}, #RH14
				{"pin": 79, "name": "BACKLIGHT_TYPE_1", "type": "GPIO", "direction": "Out", "latch": "High", "abcd": ""}, #RD0
				{"pin": 120, "name": "BACKLIGHT_TYPE_2", "type": "GPIO", "direction": "Out", "latch": "", "abcd": ""}, #RD7
				{"pin": 73, "name": "SCK1", "type": "SCK1", "direction": "", "latch": "", "abcd": ""}, #RD1
				{"pin": 167, "name": "SDI1", "type": "SDI1", "direction": "", "latch": "", "abcd": ""}, #RB5
				{"pin": 168, "name": "SDO1", "type": "SDO1", "direction": "", "latch": "", "abcd": ""}, #RB10
				{"pin": 169, "name": "SS1(out)", "type": "SS1 (out)", "direction": "", "latch": "", "abcd": ""}, #RB0
				{"pin": 31, "name": "CAN1RX", "type": "C1RX", "direction": "", "latch": "", "abcd": ""}, #RG7
				{"pin": 32, "name": "CAN1TX", "type": "C1TX", "direction": "", "latch": "", "abcd": ""}, #RG8
				{"pin": 26, "name": "CAN2RX", "type": "C2RX", "direction": "", "latch": "", "abcd": ""}, #RE9
				{"pin": 41, "name": "CAN2TX", "type": "C2TX", "direction": "", "latch": "", "abcd": ""}, #RF2
				{"pin": 126, "name": "SCL1", "type": "SCL1", "direction": "", "latch": "", "abcd": ""}, #RA14
				{"pin": 125, "name": "SDA1", "type": "SDA1", "direction": "", "latch": "", "abcd": ""}, #RA15
				{"pin": 129, "name": "SCL2", "type": "SCL2", "direction": "", "latch": "", "abcd": ""}, #RA2
				{"pin": 128, "name": "SDA2", "type": "SDA2", "direction": "", "latch": "", "abcd": ""}] #RA3


##################################################################################

def mzda_mxt_eventHandler(event):
	global pinConfigureFxn
	if (event == "configure"):
		#Override default pin configur function w/ PIC32M specific one
		pinConfigureFxn = configurePinsPIC32M
		try:
			Database.setSymbolValue("le_gfx_driver_glcd", "PixelClockDivider", 11, 1)
			Database.setSymbolValue("le_gfx_driver_glcd", "DoubleBuffer", True, 1)
			Database.setSymbolValue("le_gfx_driver_glcd", "UseGPU", False, 1)
			Database.setSymbolValue("gfx_hal_le", "ColorModeHint", "GFX_COLOR_MODE_RGBA_8888", 1)
			Database.setSymbolValue("core", "CONFIG_FPLLICLK", "PLL_POSC", 1)
			Database.setSymbolValue("core", "CONFIG_FPLLIDIV", "DIV_3", 1)
			Database.setSymbolValue("drv_sdmmc_0", "DRV_SDMMC_BUFFER_OBJECT_NUMBER", 1, 1)
			Database.setSymbolValue("sys_fs", "SYS_FS_MAX_FILES", 2, 1)
			Database.setSymbolValue("sys_fs", "SYS_FS_AUTO_MOUNT", True, 1)
			Database.setSymbolValue("drv_spi_0", "DRV_SPI_TX_RX_DMA", False, 1)
			Database.setSymbolValue("spi1", "SPI_BAUD_RATE", 500000, 1)
			Database.setSymbolValue("usb_device", "CONFIG_USB_DEVICE_EVENT_ENABLE_SOF", True, 1)
			Database.setSymbolValue("usb_device", "CONFIG_USB_DEVICE_PRODUCT_ID_SELECTION_IDX0", "cdc_com_port_single_demo", 1)
			Database.setSymbolValue("uart1", "UART_RX_RING_BUFFER_SIZE", 512, 1)
			Database.setSymbolValue("uart1", "UART_TX_RING_BUFFER_SIZE", 512, 1)
			Database.setSymbolValue("uart4", "UART_TX_RING_BUFFER_SIZE", 2048, 1)
			Database.setSymbolValue("uart4", "UART_STSEL", 0, 1)
			Database.setSymbolValue("sys_console", "SYS_CONSOLE_PRINT_BUFFER_SIZE", 512, 1)
			Database.setSymbolValue("sys_command", "SYS_COMMAND_PRINT_BUFFER_SIZE", 512, 1)
			Database.setSymbolValue("core", "BSP_PIN_79_PU", "True")  #RD0
			Database.setSymbolValue("core", "BSP_PIN_120_PU", "False")  #RD7
		except:
			return

def mzda_gt_eventHandler(event):
	global pinConfigureFxn
	if (event == "configure"):
		#Override default pin configur function w/ PIC32M specific one
		pinConfigureFxn = configurePinsPIC32M
		try:
			Database.setSymbolValue("core", "BSP_PIN_42_PU", "True", 1)
			Database.setSymbolValue("le_gfx_driver_glcd", "PixelClockDivider", 11, 1)
			Database.setSymbolValue("le_gfx_driver_glcd", "DoubleBuffer", True, 1)
			Database.setSymbolValue("le_gfx_driver_glcd", "UseGPU", False, 1)
			Database.setSymbolValue("gfx_hal_le", "ColorModeHint", "GFX_COLOR_MODE_RGBA_8888", 1)
			Database.setSymbolValue("i2c2", "I2C_CLOCK_SPEED", 400000l, 1)
			Database.setSymbolValue("core", "CONFIG_FPLLICLK", "PLL_POSC", 1)
			Database.setSymbolValue("core", "CONFIG_FPLLIDIV", "DIV_3", 1)
			Database.setSymbolValue("drv_sdmmc_0", "DRV_SDMMC_BUFFER_OBJECT_NUMBER", 1, 1)
			Database.setSymbolValue("sys_fs", "SYS_FS_MAX_FILES", 2, 1)
			Database.setSymbolValue("sys_fs", "SYS_FS_AUTO_MOUNT", True, 1)
			Database.setSymbolValue("gfx_generic_touch_controller", "Command0Parm0", 17l, 1)
			Database.setSymbolValue("gfx_generic_touch_controller", "Command0Parm1", 176l, 1)
			Database.setSymbolValue("gfx_generic_touch_controller", "Command0Parm2", 176l, 1)
			Database.setSymbolValue("gfx_generic_touch_controller", "DeviceAddress", 85l, 1)
			Database.setSymbolValue("gfx_generic_touch_controller", "InitCommandsCount", 0, 1)
			Database.setSymbolValue("gfx_generic_touch_controller", "RTOSMenu", False, 1)
			Database.setSymbolValue("gfx_generic_touch_controller", "ResetEnable", False, 1)
			Database.setSymbolValue("gfx_generic_touch_controller", "TouchDataBufferSize", 5, 1)
			Database.setSymbolValue("gfx_generic_touch_controller", "TouchDataReadAddress", 16l, 1)
			Database.setSymbolValue("gfx_generic_touch_controller", "TouchPointEventIndex", 2, 1)
			Database.setSymbolValue("gfx_generic_touch_controller", "TouchPointEventDownValue", 1l, 1)
			Database.setSymbolValue("gfx_generic_touch_controller", "TouchPointEventMask", 128l, 1)
			Database.setSymbolValue("gfx_generic_touch_controller", "TouchPointEventShift", 7, 1)
			Database.setSymbolValue("gfx_generic_touch_controller", "TouchPointPosXHIndex", 2, 1)
			Database.setSymbolValue("gfx_generic_touch_controller", "TouchPointPosXHMask", 112l, 1)
			Database.setSymbolValue("gfx_generic_touch_controller", "TouchPointPosXHShift", 4, 1)
			Database.setSymbolValue("gfx_generic_touch_controller", "TouchPointPosXLIndex", 3, 1)
			Database.setSymbolValue("gfx_generic_touch_controller", "TouchPointPosYHIndex", 2, 1)
			Database.setSymbolValue("gfx_generic_touch_controller", "TouchPointPosYHMask", 7l, 1)
			Database.setSymbolValue("gfx_generic_touch_controller", "TouchPointPosYLIndex", 4, 1)
			Database.setSymbolValue("drv_spi_0", "DRV_SPI_TX_RX_DMA", False, 1)
			Database.setSymbolValue("spi1", "SPI_BAUD_RATE", 500000, 1)
			Database.setSymbolValue("usb_device", "CONFIG_USB_DEVICE_EVENT_ENABLE_SOF", True, 1)
			Database.setSymbolValue("usb_device", "CONFIG_USB_DEVICE_PRODUCT_ID_SELECTION_IDX0", "cdc_com_port_single_demo", 1)
			Database.setSymbolValue("uart1", "UART_RX_RING_BUFFER_SIZE", 512, 1)
			Database.setSymbolValue("uart1", "UART_TX_RING_BUFFER_SIZE", 512, 1)
			Database.setSymbolValue("uart4", "UART_TX_RING_BUFFER_SIZE", 2048, 1)
			Database.setSymbolValue("uart4", "UART_STSEL", 0, 1)
			Database.setSymbolValue("sys_console", "SYS_CONSOLE_PRINT_BUFFER_SIZE", 512, 1)
			Database.setSymbolValue("sys_command", "SYS_COMMAND_PRINT_BUFFER_SIZE", 512, 1)
			Database.setSymbolValue("core", "BSP_PIN_79_PU", "True")  #RD0
			Database.setSymbolValue("core", "BSP_PIN_120_PU", "False")  #RD7
		except:
			return

mzda_mxt_interfaceList = ["Generic Touch"]

#pic32mz_da_radial_board_maxTouch = bspSupportObj(mzda_mxt_pinConfig, mzda_mxt_activateList, None, mzda_mxt_connectList, mzda_mxt_eventHandler)
pic32mz_da_radial_board_GenericTouch = bspSupportObj(mzda_gt_pinConfig, mzda_gt_activateList, None, mzda_gt_connectList, mzda_gt_eventHandler)

addDisplayIntfSupport("BSP_PIC32MZ_DA_Radial_Board", mzda_mxt_interfaceList)
#addBSPSupport("BSP_PIC32MZ_DA_Radial_Board", "maXTouch", pic32mz_da_radial_board_maxTouch)
addBSPSupport("BSP_PIC32MZ_DA_Radial_Board", "Generic Touch", pic32mz_da_radial_board_GenericTouch)

