def generateInitFile():
	file = File("generated/le_gen_init.h")
	
	file.write("#ifndef LEGATO_INIT_H")
	file.write("#define LEGATO_INIT_H")
	file.writeNewLine()
	file.write('#include "gfx/legato/legato.h"')
	file.writeNewLine()
	file.write('#include "gfx/legato/generated/le_gen_scheme.h"')
	file.write('#include "gfx/legato/generated/le_gen_assets.h"')
	file.writeNewLine()
	
	screenList = SceneManager.getScreenList()

	for scr in screenList:
		file.write('#include "gfx/legato/generated/screen/le_gen_screen_%s.h"' % (scr.getName()))
	file.writeNewLine()

	file.write("// DOM-IGNORE-BEGIN")
	file.write("#ifdef __cplusplus  // Provide C++ Compatibility")
	file.write('extern "C" {')
	file.write("#endif")
	file.write("// DOM-IGNORE-END")
	file.writeNewLine()
	file.write("// screen IDs")
	
	idx = 0
	
	for scr in screenList:
		file.write("#define screenID_%s    %d" % (scr.getName(), idx))
		
		idx += 1
	
	file.writeNewLine()
	file.write("void legato_initialize(void);")
	file.writeNewLine()
	file.write("// global screen control functions")
	file.write("uint32_t legato_getCurrentScreen(void);")
	file.write("void legato_showScreen(uint32_t id);")
	file.write("void legato_hideCurrentScreen();")
	file.write("void legato_updateCurrentScreen();")
	file.writeNewLine()
	file.write("//DOM-IGNORE-BEGIN")
	file.write("#ifdef __cplusplus")
	file.write("}")
	file.write("#endif")
	file.write("//DOM-IGNORE-END")
	file.writeNewLine()
	file.write('#endif // LEGATO_INIT_H')
	
	file.close()
	
	global fileDict
	fileDict[file.name] = file
	
	file = File("generated/le_gen_init.c")
	
	file.write('#include "gfx/legato/generated/le_gen_init.h"')
	file.writeNewLine()
	file.write('static uint32_t currentScreen;')
	file.writeNewLine()
	file.write('void legato_initialize(void)')
	file.write('{')
	file.write('    legato_initialize_schemes();')
	file.writeNewLine()

	global stringTableMatrix

	if stringTableMatrix.stringIDList.size() > 0:
		file.write('    leSetStringTable(&stringTable);')
		file.writeNewLine()
	
	file.write('    currentScreen = -1;');

	if len(screenList) > 0:
		file.writeNewLine()
		file.write('    legato_showScreen(screenID_%s);' % (screenList[0].getName()))

	file.write('}')
	file.writeNewLine()
	file.write('uint32_t legato_getCurrentScreen(void)')
	file.write('{')
	file.write('    return currentScreen;')
	file.write('}')
	file.writeNewLine()
	file.write('void legato_hideCurrentScreen()')
	file.write('{')
	file.write('    switch(currentScreen)')
	file.write('    {')
	
	for scr in screenList:
		file.write('        case screenID_%s:' % (scr.getName()))
		file.write('        {')
		file.write('            screenHide_%s();' % (scr.getName()))
		file.write('            currentScreen = 0;')
		file.write('            break;')
		file.write('        }')
		
	file.write('    }')
	file.write('}')
	file.writeNewLine()
	file.write('void legato_showScreen(uint32_t id)')
	file.write('{')
	file.write('    legato_hideCurrentScreen(currentScreen);')
	file.writeNewLine()
	file.write('    switch(id)')
	file.write('    {')
	
	for scr in screenList:
		file.write('        case screenID_%s:' % (scr.getName()))
		file.write('        {')
		file.write('            screenShow_%s();' % (scr.getName()))
		file.write('            currentScreen = id;')
		file.write('            break;')
		file.write('        }')

	file.write('    }')
	file.write('}')
	file.writeNewLine()
	file.write('void legato_updateCurrentScreen(void)')
	file.write('{')
	file.write('    switch(currentScreen)')
	file.write('    {')

	for scr in screenList:
		if scr.getEventByName("UpdateEvent").enabled == True:
			file.write('        case screenID_%s:' % (scr.getName()))
			file.write('        {')
			file.write('            screenUpdate_%s();' % (scr.getName()))
			file.write('            break;')
			file.write('        }')

	file.write('    }')
	file.write('}')
	file.writeNewLine()
	
	file.close()
	
	fileDict[file.name] = file
