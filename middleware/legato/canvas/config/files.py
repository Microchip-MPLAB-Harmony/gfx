# generate code files
projectPath = "config/" + Variables.get("__CONFIGURATION_NAME") + "/gfx/canvas"

# common gfx driver header
GFX_CANVAS_H = comp.createFileSymbol("GFX_CANVAS_H", None)
GFX_CANVAS_H.setSourcePath("templates/gfx_canvas.h")
GFX_CANVAS_H.setDestPath("gfx/canvas/")
GFX_CANVAS_H.setOutputName("gfx_canvas.h")
GFX_CANVAS_H.setProjectPath(projectPath)
GFX_CANVAS_H.setType("HEADER")
GFX_CANVAS_H.setMarkup(True)

GFX_CANVAS_C = comp.createFileSymbol("GFX_CANVAS_C", None)
GFX_CANVAS_C.setSourcePath("templates/gfx_canvas.c")
GFX_CANVAS_C.setDestPath("gfx/canvas/")
GFX_CANVAS_C.setOutputName("gfx_canvas.c")
GFX_CANVAS_C.setProjectPath(projectPath)
GFX_CANVAS_C.setType("SOURCE")
GFX_CANVAS_C.setMarkup(True)

GFX_CANVAS_API_H = comp.createFileSymbol("GFX_CANVAS_API_H", None)
GFX_CANVAS_API_H.setSourcePath("templates/gfx_canvas_api.h")
GFX_CANVAS_API_H.setDestPath("gfx/canvas/")
GFX_CANVAS_API_H.setOutputName("gfx_canvas_api.h")
GFX_CANVAS_API_H.setProjectPath(projectPath)
GFX_CANVAS_API_H.setType("HEADER")

GFX_CANVAS_API_C = comp.createFileSymbol("GFX_CANVAS_API_C", None)
GFX_CANVAS_API_C.setSourcePath("templates/gfx_canvas_api.c")
GFX_CANVAS_API_C.setDestPath("gfx/canvas/")
GFX_CANVAS_API_C.setOutputName("gfx_canvas_api.c")
GFX_CANVAS_API_C.setProjectPath(projectPath)
GFX_CANVAS_API_C.setType("SOURCE")