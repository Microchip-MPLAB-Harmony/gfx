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
	comp.setHelpFile("../../docs/help_harmony_gfx_html_alias.h")
	#comp.setHelp("IDH_HTML_GFX_CMP__2__Display_Component")
	
	Name = comp.createStringSymbol("Name", None)
	Name.setLabel("Name")
	Name.setDescription("The display name.")
	Name.setDefaultValue("Generic Display")
	Name.setReadOnly(True)
	
	Comment = comp.createCommentSymbol("Comment", None)
	Comment.setLabel('Use GFX Core LE Component to configure display parameters')
	Comment.setVisible(True)
	
	DisplayWidth = comp.createIntegerSymbol("DisplayWidth", None)
	DisplayWidth.setLabel("Graphics Display Width")
	DisplayWidth.setDescription("The width of the graphics display in pixels.")
	DisplayWidth.setDefaultValue(480)
	DisplayWidth.setReadOnly(True)
	DisplayWidth.setVisible(False)
	
	DisplayHeight = comp.createIntegerSymbol("DisplayHeight", None)
	DisplayHeight.setLabel("Graphics Display Height")
	DisplayHeight.setDescription("The height of the graphics display in pixels.")
	DisplayHeight.setDefaultValue(272)
	DisplayHeight.setReadOnly(True)
	DisplayHeight.setVisible(False)
	
	TouchWidth = comp.createIntegerSymbol("TouchWidth", None)
	TouchWidth.setLabel("Touch Panel Width")
	TouchWidth.setDescription("The width of the touch panel in pixels.")
	TouchWidth.setDefaultValue(480)
	TouchWidth.setReadOnly(True)
	TouchWidth.setVisible(False)
	
	TouchHeight = comp.createIntegerSymbol("TouchHeight", None)
	TouchHeight.setLabel("Touch Panel Height")
	TouchHeight.setDescription("The height of the touch panel in pixels.")
	TouchHeight.setDefaultValue(272)
	TouchHeight.setReadOnly(True)	
	TouchHeight.setVisible(False)
