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

def instantiateComponent(comp):
	comp.setHelpFile("../../../docs/help_harmony_gfx_html_alias.h")
	#comp.setHelp("IDH_HTML_GFXLIB_ILI9488_Display_Controller_Driver_Library")
	
	execfile(Module.getPath() + "/config/ili9488_config.py")
	execfile(Module.getPath() + "/config/ili9488_files.py")

def onHALConnected(halConnected, event):
	halConnected.getComponent().getSymbolByID("HALComment").setVisible(event["value"] == True)
	halConnected.getComponent().getSymbolByID("DisplayWidth").setVisible(event["value"] == False)
	halConnected.getComponent().getSymbolByID("DisplayHeight").setVisible(event["value"] == False)
	halConnected.getComponent().getSymbolByID("PaletteMode").setVisible(event["value"] == False)

def onAttachmentConnected(source, target):
	if source["id"] == "Display Interface":
		print(source["component"].getID() + ": Using " + target["component"].getID() + " interface ")
		source["component"].getSymbolByID("GFX_ILI9488_DBIB_C").setEnabled(False)
		source["component"].getSymbolByID("GFX_ILI9488_SPI").setEnabled(False)
		source["component"].getSymbolByID("ParallelInterfaceWidth").setVisible(False)

		InterfaceType = str(target["component"].getSymbolByID("InterfaceType").getValue())
		source["component"].getSymbolByID("DisplayInterface").setValue(target["id"], 1)
		source["component"].getSymbolByID("DisplayInterfaceType").setValue(InterfaceType, 1)
		if InterfaceType == "SPI 4-line":
			source["component"].getSymbolByID("GFX_ILI9488_SPI").setEnabled(True)
		elif "Parallel" in InterfaceType:
			source["component"].getSymbolByID("GFX_ILI9488_DBIB_C").setEnabled(True)
			source["component"].getSymbolByID("ParallelInterfaceWidth").setVisible(True)
			if InterfaceType == "Parallel 8-bit":
				source["component"].getSymbolByID("ParallelInterfaceWidth").setValue("8-bit", True)
				source["component"].getSymbolByID("ParallelInterfaceWidth").setReadOnly(True)
			else:
				source["component"].getSymbolByID("ParallelInterfaceWidth").setReadOnly(False)
		else:
			print("Interface does not contain 'InterfaceType' capability")

def onDrawBufferSizeSet(drawBufferSizeSet, event):
	if (event["value"] == "Line"):
		drawBufferSizeSet.getComponent().getSymbolByID("DrawBufferPreRead").setVisible(True)
	else:
		drawBufferSizeSet.getComponent().getSymbolByID("DrawBufferPreRead").setVisible(False)

def onDrawBufferPreReadSet(drawBufferPreReadSet, event):
	if (event["value"] == False):
		drawBufferPreReadSet.getComponent().getSymbolByID("DrawBufferPreFillValue").setVisible(True)
	else:
		drawBufferPreReadSet.getComponent().getSymbolByID("DrawBufferPreFillValue").setVisible(False)

