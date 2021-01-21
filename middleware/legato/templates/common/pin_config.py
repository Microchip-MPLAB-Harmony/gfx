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

def resetPins(pinConfigs):
	for pinConfig in pinConfigs:
		print("Resetting " + pinConfig["name"])
		Database.clearSymbolValue("core", "PIN_" + str(pinConfig["pin"]) + "_FUNCTION_NAME")
		Database.clearSymbolValue("core", "PIN_" + str(pinConfig["pin"]) + "_FUNCTION_TYPE")
		Database.setSymbolValue("core", "PIN_" + str(pinConfig["pin"]) + "_FUNCTION_TYPE", "Available", 1)
		Database.clearSymbolValue("core", "PIN_" + str(pinConfig["pin"]) + "_DIR")
		Database.clearSymbolValue("core", "PIN_" + str(pinConfig["pin"]) + "_LAT")
		Database.clearSymbolValue("core", "PIN_" + str(pinConfig["pin"]) + "_PERIPHERAL_FUNCTION")

def configurePins(pinConfigs):
	for pinConfig in pinConfigs:
		print("Configuring SAM pin # " + str(pinConfig["pin"]) + " name: " + pinConfig["name"])
		Database.clearSymbolValue("core", "PIN_" + str(pinConfig["pin"]) + "_FUNCTION_NAME")
		Database.clearSymbolValue("core", "PIN_" + str(pinConfig["pin"]) + "_FUNCTION_TYPE")
		Database.clearSymbolValue("core", "PIN_" + str(pinConfig["pin"]) + "_DIR")
		### Comment out clear of Pin Latch, it looks like if latch symbol is cleared it couldn't
		### be changed from low
		#Database.clearSymbolValue("core", "PIN_" + str(pinConfig["pin"]) + "_LAT")
		Database.clearSymbolValue("core", "PIN_" + str(pinConfig["pin"]) + "_PERIPHERAL_FUNCTION")
		Database.setSymbolValue("core", "PIN_" + str(pinConfig["pin"]) + "_FUNCTION_TYPE", pinConfig["type"], 1)

		if (pinConfig["type"] == "GPIO"):
			Database.setSymbolValue("core", "PIN_" + str(pinConfig["pin"]) + "_FUNCTION_NAME", pinConfig["name"], 1)
			Database.setSymbolValue("core", "PIN_" + str(pinConfig["pin"]) + "_DIR", pinConfig["direction"], 1)
			Database.setSymbolValue("core", "PIN_" + str(pinConfig["pin"]) + "_LAT", pinConfig["latch"], 1)
		else:
			Database.setSymbolValue("core", "PIN_" + str(pinConfig["pin"]) + "_PERIPHERAL_FUNCTION", pinConfig["abcd"], 1)

def resetPinsPIC32M(pinConfigs):
	for pinConfig in pinConfigs:
		print("Resetting PIC32M pin # " + str(pinConfig["pin"]) + " name: " + pinConfig["name"])
		Database.clearSymbolValue("core", "BSP_PIN_" + str(pinConfig["pin"]) + "_FUNCTION_NAME")
		Database.clearSymbolValue("core", "BSP_PIN_" + str(pinConfig["pin"]) + "_FUNCTION_TYPE")
		Database.setSymbolValue("core", "BSP_PIN_" + str(pinConfig["pin"]) + "_FUNCTION_TYPE", "Available", 1)
		Database.clearSymbolValue("core", "BSP_PIN_" + str(pinConfig["pin"]) + "_DIR")
		Database.clearSymbolValue("core", "BSP_PIN_" + str(pinConfig["pin"]) + "_LAT")
		Database.clearSymbolValue("core", "BSP_PIN_" + str(pinConfig["pin"]) + "_PERIPHERAL_FUNCTION")

def configurePinsPIC32M(pinConfigs):
	for pinConfig in pinConfigs:
		print("Configuring PIC32M pin # " + str(pinConfig["pin"]) + " name: " + pinConfig["name"])
		Database.clearSymbolValue("core", "BSP_PIN_" + str(pinConfig["pin"]) + "_FUNCTION_NAME")
		Database.clearSymbolValue("core", "BSP_PIN_" + str(pinConfig["pin"]) + "_FUNCTION_TYPE")
		Database.clearSymbolValue("core", "BSP_PIN_" + str(pinConfig["pin"]) + "_DIR")
		### Comment out clear of Pin Latch, it looks like if latch symbol is cleared it couldn't
		### be changed from low
		#Database.clearSymbolValue("core", "BSP_PIN_" + str(pinConfig["pin"]) + "_LAT")
		Database.clearSymbolValue("core", "BSP_PIN_" + str(pinConfig["pin"]) + "_PERIPHERAL_FUNCTION")
		Database.setSymbolValue("core", "BSP_PIN_" + str(pinConfig["pin"]) + "_FUNCTION_TYPE", pinConfig["type"], 1)

		if (pinConfig["type"] == "GPIO"):
			Database.setSymbolValue("core", "BSP_PIN_" + str(pinConfig["pin"]) + "_FUNCTION_NAME", pinConfig["name"], 1)
			Database.setSymbolValue("core", "BSP_PIN_" + str(pinConfig["pin"]) + "_DIR", pinConfig["direction"], 1)
			Database.setSymbolValue("core", "BSP_PIN_" + str(pinConfig["pin"]) + "_LAT", pinConfig["latch"], 1)
		else:
			Database.setSymbolValue("core", "BSP_PIN_" + str(pinConfig["pin"]) + "_PERIPHERAL_FUNCTION", pinConfig["abcd"], 1)
