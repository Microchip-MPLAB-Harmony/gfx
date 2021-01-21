# generate code files
projectPath = "config/" + Variables.get("__CONFIGURATION_NAME") + "/gfx/driver/ssd1963"

# common gfx driver header
GFX_DRIVER_H = comp.createFileSymbol("GFX_DRIVER_H", None)
GFX_DRIVER_H.setSourcePath("../../templates/gfx_driver.h.ftl")
GFX_DRIVER_H.setDestPath("gfx/driver/")
GFX_DRIVER_H.setOutputName("gfx_driver.h")
GFX_DRIVER_H.setProjectPath(projectPath)
GFX_DRIVER_H.setType("HEADER")
GFX_DRIVER_H.setMarkup(True)

GFX_DRIVER_C = comp.createFileSymbol("GFX_DRIVER_C", None)
GFX_DRIVER_C.setSourcePath("../../templates/gfx_driver.c.ftl")
GFX_DRIVER_C.setDestPath("gfx/driver/")
GFX_DRIVER_C.setOutputName("gfx_driver.c")
GFX_DRIVER_C.setProjectPath(projectPath)
GFX_DRIVER_C.setType("SOURCE")
GFX_DRIVER_C.setMarkup(True)

GFX_SSD1963_C = comp.createFileSymbol("GFX_SSD1963_C", None)
GFX_SSD1963_C.setDestPath("gfx/driver/controller/ssd1963/")
GFX_SSD1963_C.setSourcePath("templates/drv_gfx_ssd1963.c.ftl")
GFX_SSD1963_C.setOutputName("drv_gfx_ssd1963.c")
GFX_SSD1963_C.setProjectPath(projectPath)
GFX_SSD1963_C.setType("SOURCE")
GFX_SSD1963_C.setMarkup(True)

GFX_SSD1963_H = comp.createFileSymbol("GFX_SSD1963_H", None)
GFX_SSD1963_H.setDestPath("gfx/driver/controller/ssd1963/")
GFX_SSD1963_H.setSourcePath("templates/drv_gfx_ssd1963.h.ftl")
GFX_SSD1963_H.setOutputName("drv_gfx_ssd1963.h")
GFX_SSD1963_H.setProjectPath(projectPath)
GFX_SSD1963_H.setType("HEADER")
GFX_SSD1963_H.setMarkup(True)

GFX_SSD1963_COMMON_H = comp.createFileSymbol("GFX_SSD1963_COMMON_H", None)
GFX_SSD1963_COMMON_H.setDestPath("gfx/driver/controller/ssd1963/")
GFX_SSD1963_COMMON_H.setSourcePath("inc/drv_gfx_ssd1963_common.h")
GFX_SSD1963_COMMON_H.setOutputName("drv_gfx_ssd1963_common.h")
GFX_SSD1963_COMMON_H.setProjectPath(projectPath)
GFX_SSD1963_COMMON_H.setType("HEADER")

GFX_SSD1963_CMD_DEFS_H = comp.createFileSymbol("GFX_SSD1963_CMD_DEFS_H", None)
GFX_SSD1963_CMD_DEFS_H.setDestPath("gfx/driver/controller/ssd1963/")
GFX_SSD1963_CMD_DEFS_H.setSourcePath("inc/drv_gfx_ssd1963_cmd_defs.h")
GFX_SSD1963_CMD_DEFS_H.setOutputName("drv_gfx_ssd1963_cmd_defs.h")
GFX_SSD1963_CMD_DEFS_H.setProjectPath(projectPath)
GFX_SSD1963_CMD_DEFS_H.setType("HEADER")
