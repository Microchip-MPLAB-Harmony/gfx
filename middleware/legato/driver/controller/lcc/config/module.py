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
		cntlComponent.addDependency("sys_dma", "sys_dma", True)
	else:
	    if ("SAM" in str(Variables.get("__PROCESSOR")) and "7" in str(Variables.get("__PROCESSOR"))):
		   cntlComponent = Module.CreateComponent("le_gfx_driver_lcc", "LE LCC ", "/Graphics/Driver", "config/lcc_controller.py")
		   cntlComponent.setDisplayType("LE LCC Display Driver")
		   cntlComponent.addCapability("gfx_driver_lcc", "LE Display Driver", False)
		   cntlComponent.addDependency("Graphics Display", "Graphics Display", False)
		   cntlComponent.addDependency("SMC_CS", "SMC_CS", False, True)
		   ### TMR dependency for PWM backlight control
		   cntlComponent.addDependency("TMR", "TMR", False, True)
		   cntlComponent.setDependencyEnabled("TMR", False)
	    else:
		   print("LE LCC module not loaded.  No support for " + str(Variables.get("__PROCESSOR")))
