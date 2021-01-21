import com.microchip.gfx.composer.scheme.SchemeManager as SchemeManager
import com.microchip.gfx.composer.generator.Generator as Generator

def generateSchemeFile():
	file = File("generated/le_gen_scheme.h")
	
	schemeList = SchemeManager.getSchemeNameList()
	
	file.write("#ifndef LEGATO_INIT_SCHEME_H")
	file.write("#define LEGATO_INIT_SCHEME_H")
	file.writeNewLine()
	file.write('#include "gfx/legato/legato.h"')
	file.writeNewLine()
	
	for schemeName in schemeList:
		file.write('extern leScheme %s;' % (schemeName))
		
	file.writeNewLine()
	file.write("// DOM-IGNORE-BEGIN")
	file.write("#ifdef __cplusplus  // Provide C++ Compatibility")
	file.write('extern "C" {')
	file.write("#endif")
	file.write("// DOM-IGNORE-END")
	file.writeNewLine()
	file.write('void legato_initialize_schemes();')
	file.writeNewLine()
	file.write("//DOM-IGNORE-BEGIN")
	file.write("#ifdef __cplusplus")
	file.write("}")
	file.write("#endif")
	file.write("//DOM-IGNORE-END")
	file.writeNewLine()
	file.write('#endif // LEGATO_INIT_SCHEME_H')
	
	file.close()
	
	global fileDict
	fileDict[file.name] = file
	
	file = File("generated/le_gen_scheme.c")
	
	file.write('#include "gfx/legato/generated/le_gen_scheme.h"')
	file.writeNewLine()
	for schemeName in schemeList:
		file.write('leScheme %s;' % (schemeName))
	
	file.writeNewLine()
	file.write("void legato_initialize_schemes(void)")
	file.write("{")
	
	for schemeName in schemeList:
		scheme = SchemeManager.getScheme(schemeName)
		
		colorMode = StateManager.getString("PROJECT_COLOR_MODE")
		
		file.write("    leScheme_Initialize(&%s, LE_COLOR_MODE_%s);" % (schemeName, colorMode))
		file.write("    %s.base = 0x%s;" % (schemeName, scheme.getColor("Base").toHexString(colorMode)))
		file.write("    %s.highlight = 0x%s;" % (schemeName, scheme.getColor("Highlight").toHexString(colorMode)))
		file.write("    %s.highlightLight = 0x%s;" % (schemeName, scheme.getColor("HighlightLight").toHexString(colorMode)))
		file.write("    %s.shadow = 0x%s;" % (schemeName, scheme.getColor("Shadow").toHexString(colorMode)))
		file.write("    %s.shadowDark = 0x%s;" % (schemeName, scheme.getColor("ShadowDark").toHexString(colorMode)))
		file.write("    %s.foreground = 0x%s;" % (schemeName, scheme.getColor("Foreground").toHexString(colorMode)))
		file.write("    %s.foregroundInactive = 0x%s;" % (schemeName, scheme.getColor("ForegroundInactive").toHexString(colorMode)))
		file.write("    %s.foregroundDisabled = 0x%s;" % (schemeName, scheme.getColor("ForegroundDisabled").toHexString(colorMode)))
		file.write("    %s.background = 0x%s;" % (schemeName, scheme.getColor("Background").toHexString(colorMode)))
		file.write("    %s.backgroundInactive = 0x%s;" % (schemeName, scheme.getColor("BackgroundInactive").toHexString(colorMode)))
		file.write("    %s.backgroundDisabled = 0x%s;" % (schemeName, scheme.getColor("BackgroundDisabled").toHexString(colorMode)))
		file.write("    %s.text = 0x%s;" % (schemeName, scheme.getColor("Text").toHexString(colorMode)))
		file.write("    %s.textHighlight = 0x%s;" % (schemeName, scheme.getColor("TextHighlight").toHexString(colorMode)))
		file.write("    %s.textHighlightText = 0x%s;" % (schemeName, scheme.getColor("TextHighlightText").toHexString(colorMode)))
		file.write("    %s.textInactive = 0x%s;" % (schemeName, scheme.getColor("TextInactive").toHexString(colorMode)))
		file.write("    %s.textDisabled = 0x%s;" % (schemeName, scheme.getColor("TextDisabled").toHexString(colorMode)))
		file.writeNewLine()
		
	file.write("}")
	
	file.close()
	
	fileDict[file.name] = file