# coding: utf-8
##############################################################################
# Copyright (C) 2025 Microchip Technology Inc. and its subsidiaries.
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


ip_name = "GPU2DC"
header_files = ["nano2D.h", "nano2D_base.h", "nano2D_enum.h", "nano2D_hal.h", "nano2D_option.h", "nano2D_types.h", "version.h", "drv_gfx_gpu2dc.h"]
source_files = ["nano2D_hal.c", "drv_gfx_gpu2dc.c"]


def instantiateComponent(component):
    print("Instantiated 2DGPU nano2d driver component")
    dest_path = "gfx/driver/gpu2dc"
    project_path = "config/" + Variables.get("__CONFIGURATION_NAME") + "/" + dest_path

    driver_ver = component.createStringSymbol("Version", None)
    driver_ver.setVisible(True)
    driver_ver.setReadOnly(True)
    driver_ver.setDefaultValue("1.1.0")

    # Utility Symbols (hidden)
    # Driver IP name
    driver_ip_name = component.createStringSymbol("IP", None)
    driver_ip_name.setVisible(False)
    driver_ip_name.setReadOnly(True)
    driver_ip_name.setDefaultValue(ip_name)
    # Define Legato No Cache Attribute
    macro_flags = component.createSettingSymbol("GPU2DC_MACROS", None)
    macro_flags.setCategory("C32")
    macro_flags.setKey("preprocessor-macros")
    macro_flags.setValue('LE_NO_CACHE_ATTR=__attribute__((section(\\".region_nocache\\"),aligned(16)))')
    macro_flags.setAppend(True, ";")

    # Enable Interrupts
    Database.setSymbolValue("core", ip_name + "_INTERRUPT_ENABLE", True)
    Database.setSymbolValue("core", ip_name + "_INTERRUPT_HANDLER", ip_name + "_InterruptHandler")

    libgpu2dc_a = component.createLibrarySymbol("DRV_LIB_GPU2DC", None)
    libgpu2dc_a.setDestPath(dest_path)
    libgpu2dc_a.setOutputName("libgpu2dc.a")
    libgpu2dc_a.setSourcePath("lib/libgpu2dc.a")

    for hdr in header_files:
        hdr_file = component.createFileSymbol("HDR_" + hdr.upper().replace(".", "_"), None)
        hdr_file.setSourcePath("lib/" + hdr)
        hdr_file.setOutputName(hdr)
        hdr_file.setDestPath(dest_path)
        hdr_file.setProjectPath(project_path)
        hdr_file.setType("HEADER")
        hdr_file.setOverwrite(True)

    for src in source_files:
        src_file = component.createFileSymbol("SRC_" + src.upper().replace(".", "_"), None)
        src_file.setSourcePath("lib/" + src)
        src_file.setOutputName(src)
        src_file.setDestPath(dest_path)
        src_file.setProjectPath(project_path)
        src_file.setType("SOURCE")
        src_file.setOverwrite(True)

    include_path = component.createSettingSymbol("GPU2DC_INCLUDE_PATH", None)
    include_path.setCategory("C32")
    include_path.setKey("extra-include-directories")
    include_path.setValue("../src/" + project_path)
    include_path.setAppend(True, ";")

    sys_definitions = component.createFileSymbol("GPU2DC_DEFINITIONS_H", None)
    sys_definitions.setType("STRING")
    sys_definitions.setOutputName("core.LIST_SYSTEM_DEFINITIONS_H_INCLUDES")
    sys_definitions.setSourcePath("templates/definitions.h.ftl")
    sys_definitions.setMarkup(True)

    sys_init = component.createFileSymbol("GPU2DC_INIT_C", None)
    sys_init.setType("STRING")
    sys_init.setOutputName("core.LIST_SYSTEM_INIT_C_INITIALIZE_MIDDLEWARE")
    sys_init.setSourcePath("templates/init.c.ftl")
    sys_init.setMarkup(True)

    driver_interrupts = component.createFileSymbol("INTERRUPTS_C", None)
    driver_interrupts.setType("STRING")
    driver_interrupts.setSourcePath("templates/interrupt.c.ftl")
    driver_interrupts.setOutputName("core.LIST_SYSTEM_INTERRUPT_SHARED_HANDLERS")
    driver_interrupts.setMarkup(True)
