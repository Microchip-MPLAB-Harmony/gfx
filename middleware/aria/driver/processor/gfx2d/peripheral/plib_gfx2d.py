# coding: utf-8
"""*****************************************************************************
* Copyright (C) 2019 Microchip Technology Inc. and its subsidiaries.
*
* Subject to your compliance with these terms, you may use Microchip software
* and any derivatives exclusively with Microchip products. It is your
* responsibility to comply with third party license terms applicable to your
* use of third party software (including open source software) that may
* accompany Microchip software.
*
* THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
* EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
* WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
* PARTICULAR PURPOSE.
*
* IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
* INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
* WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
* BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
* FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
* ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
* THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
*****************************************************************************"""

###################################################################################################
########################### Component   #################################
###################################################################################################
def instantiateComponent(comp):
    print("Instantiated GFX2D plib component")
    configName = Variables.get("__CONFIGURATION_NAME")

    instanceName = comp.createStringSymbol("GFX2D_INSTANCE_NAME", None)
    instanceName.setReadOnly(True)
    instanceName.setDefaultValue(comp.getID().upper())
    InterruptVector = instanceName.getValue() + "_INTERRUPT_ENABLE"
    InterruptHandler = instanceName.getValue() + "_INTERRUPT_HANDLER"

    #Enable the GFX2D clock
    Database.setSymbolValue("core", "GFX2D_CLOCK_ENABLE", True, 2)

    #Enable the GFX2D Interrupt
    Database.setSymbolValue("core", InterruptVector, True, 2)
    Database.setSymbolValue("core", InterruptHandler, instanceName.getValue() + "_Interrupt_Handler", 2)

    #Generate Output Header
    gfx2dHeaderFile = comp.createFileSymbol("gfx2dHeaderFile", None)
    gfx2dHeaderFile.setSourcePath("peripheral/templates/plib_gfx2d.h.ftl")
    gfx2dHeaderFile.setMarkup(True)
    gfx2dHeaderFile.setOutputName(instanceName.getValue().lower()+".h")
    gfx2dHeaderFile.setOverwrite(True)
    gfx2dHeaderFile.setDestPath("peripheral/gfx2d/")
    gfx2dHeaderFile.setProjectPath("config/" + configName + "/peripheral/gfx2d/")
    gfx2dHeaderFile.setType("HEADER")

    #Generate Output source
    gfx2dSourceFile = comp.createFileSymbol("gfx2dSourceFile", None)
    gfx2dSourceFile.setSourcePath("peripheral/templates/plib_gfx2d.c.ftl")
    gfx2dSourceFile.setMarkup(True)
    gfx2dSourceFile.setOutputName(instanceName.getValue().lower()+".c")
    gfx2dSourceFile.setOverwrite(True)
    gfx2dSourceFile.setDestPath("peripheral/gfx2d/")
    gfx2dSourceFile.setProjectPath("config/" + configName + "/peripheral/gfx2d/")
    gfx2dSourceFile.setType("SOURCE")
  
