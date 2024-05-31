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

def loadModule():
	
	if ("PIC32MZ" in str(Variables.get("__PROCESSOR"))):
		cntlComponent = Module.CreateComponent("le_gfx_driver_lcc", "LE LCC ", "/Graphics/Driver", "config/lcc_controller_pic32mz.py")
		cntlComponent.setDisplayType("LE LCC Display Driver")
		cntlComponent.addCapability("gfx_driver_lcc", "LE Display Driver", False)
		cntlComponent.addDependency("EBI_CS", "EBI_CS", False, True)
		cntlComponent.addDependency("Graphics Display", "Graphics Display", False)
	elif ("SAMRH" in str(Variables.get("__PROCESSOR")) and "7" in str(Variables.get("__PROCESSOR"))):
		cntlComponent = Module.CreateComponent("le_gfx_driver_lcc", "LE LCC ", "/Graphics/Driver", "config/lcc_controller.py")
		cntlComponent.setDisplayType("LE LCC Display Driver")
		cntlComponent.addCapability("gfx_driver_lcc", "LE Display Driver", False)
		cntlComponent.addDependency("Graphics Display", "Graphics Display", False)
		cntlComponent.addDependency("HEMC_CS", "HEMC_CS", False, True)
		### TMR dependency for PWM backlight control
		cntlComponent.addDependency("TMR", "TMR", False, True)
		cntlComponent.setDependencyEnabled("TMR", False)
	elif ("SAM" in str(Variables.get("__PROCESSOR")) and "7" in str(Variables.get("__PROCESSOR"))):
		cntlComponent = Module.CreateComponent("le_gfx_driver_lcc", "LE LCC ", "/Graphics/Driver", "config/lcc_controller.py")
		cntlComponent.setDisplayType("LE LCC Display Driver")
		cntlComponent.addCapability("gfx_driver_lcc", "LE Display Driver", False)
		cntlComponent.addDependency("Graphics Display", "Graphics Display", False)
		cntlComponent.addDependency("SMC_CS", "SMC_CS", False, True)
		### TMR dependency for PWM backlight control
		cntlComponent.addDependency("TMR", "TMR", False, True)
		cntlComponent.setDependencyEnabled("TMR", False)
	elif((("PIC32CZ" in str(Variables.get("__PROCESSOR"))) and ("70" in str(Variables.get("__PROCESSOR"))))):
		cntlComponent = Module.CreateComponent("le_gfx_driver_lcc", "LE LCC ", "/Graphics/Driver", "config/lcc_controller_pic32cz_ca70.py")
		cntlComponent.setDisplayType("LE LCC Display Driver")
		cntlComponent.addCapability("gfx_driver_lcc", "LE Display Driver", False)
		cntlComponent.addDependency("Graphics Display", "Graphics Display", False)
		cntlComponent.addDependency("SMC_CS", "SMC_CS", False, True) 
		### PWM dependency for PWM backlight control
		cntlComponent.addDependency("Backlight", "PWM", False, True)
		#cntlComponent.setDependencyEnabled("PWM", False)
	elif (("SAM" in str(Variables.get("__PROCESSOR")) and "5" in str(Variables.get("__PROCESSOR"))) or ("PIC32CX" in str(Variables.get("__PROCESSOR")))):
		cntlComponent = Module.CreateComponent("le_gfx_driver_lcc", "LE LCC ", "/Graphics/Driver", "config/lcc_controller_e5xd5x.py")
		cntlComponent.setDisplayType("LE LCC Display Driver")
		cntlComponent.addCapability("gfx_driver_lcc", "LE Display Driver", False)
		cntlComponent.addDependency("Graphics Display", "Graphics Display", False)
		### TMR dependency for DMA control
		cntlComponent.addDependency("TMR", "TMR", False, True)
	elif ("PIC32CK" in str(Variables.get("__PROCESSOR"))):
		cntlComponent = Module.CreateComponent("le_gfx_driver_lcc", "LE LCC ", "/Graphics/Driver", "config/lcc_controller_pic32ck.py")
		cntlComponent.setDisplayType("LE LCC Display Driver")
		cntlComponent.addCapability("gfx_driver_lcc", "LE Display Driver", False)
		cntlComponent.addDependency("Graphics Display", "Graphics Display", False)
		cntlComponent.addDependency("EBI_CS", "EBI_CS", False, True)
		### TMR dependency for PWM backlight control
		cntlComponent.addDependency("TCC", "PWM", False, True)
		cntlComponent.addDependency("Backlight", "PWM", False) 
	elif ("PIC32CZ" in str(Variables.get("__PROCESSOR")) and ("70" not in str(Variables.get("__PROCESSOR")))):
		cntlComponent = Module.CreateComponent("le_gfx_driver_lcc", "LE LCC ", "/Graphics/Driver", "config/lcc_controller_pic32cz.py")
		cntlComponent.setDisplayType("LE LCC Display Driver")
		cntlComponent.addCapability("gfx_driver_lcc", "LE Display Driver", False)
		cntlComponent.addDependency("Graphics Display", "Graphics Display", False)
		cntlComponent.addDependency("EBI_CS", "EBI_CS", False, True)
		### TMR dependency for PWM backlight control
		cntlComponent.addDependency("TCC", "PWM", False, True)
		cntlComponent.addDependency("Backlight", "PWM", False)
	else:
		print("LE LCC module not loaded.  No support for " + str(Variables.get("__PROCESSOR")))
