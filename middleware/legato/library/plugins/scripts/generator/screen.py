class RadioButtonGroup:
	def __init__(self, id):
		self.id = id
		self.buttons = []
		self.selectedButton = None

	def __lt__(self, other):
		return self.id < other.id

def findRadioGroup(id):
	for group in radioButtonGroups:
		if group.id == id:
			return group

	return None

radioButtonGroups = []

class RadialMenuItem:
	def __init__(self, menu, item, idx):
		self.name = "%s_image_%d" % (menu.getName(), idx)

class RadialMenu:
	def __init__(self, menu):
		self.menu = menu
		self.items = []

		for idx, item in enumerate(menu.getItemList()):
			menuItem = RadialMenuItem(menu, item, idx)

			self.items.append(menuItem)

radialMenus = []

def generateScreen(scr):
	global radioButtonGroups
	global radialMenus

	radioButtonGroups = []
	radialMenus = []

	name = scr.getName()
	
	screenWidgetList = []
	
	layerList = scr.getLayerList()
	
	for layer in layerList:
		screenWidgetList += layer.getDescendents()

	screenHeaderFile = File("generated/screen/le_gen_screen_%s.h" % name)
	
	screenHeaderFile.write("#ifndef LE_GEN_SCREEN_" + name.upper() + "_H")
	screenHeaderFile.write("#define LE_GEN_SCREEN_" + name.upper() + "_H")
	screenHeaderFile.writeNewLine()
	screenHeaderFile.write('#include "gfx/legato/generated/le_gen_init.h"')
	screenHeaderFile.writeNewLine()
	
	screenHeaderFile.write("// DOM-IGNORE-BEGIN")
	screenHeaderFile.write("#ifdef __cplusplus  // Provide C++ Compatibility")
	screenHeaderFile.write('extern "C" {')
	screenHeaderFile.write("#endif")
	screenHeaderFile.write("// DOM-IGNORE-END")
	screenHeaderFile.writeNewLine()
	
	generateWidgetHeaderDeclarations(screenHeaderFile, scr, screenWidgetList)
		
	screenHeaderFile.writeNewLine()
	screenHeaderFile.write("// screen lifecycle functions")
	screenHeaderFile.write("// DO NOT CALL THESE DIRECTLY")
	screenHeaderFile.write('leResult screenInit_%s(); // called when Legato is initialized' % (name))
	screenHeaderFile.write('leResult screenShow_%s(); // called when screen is shown' % (name))
	screenHeaderFile.write('void screenHide_%s(); // called when screen is hidden' % (name))
	screenHeaderFile.write('void screenDestroy_%s(); // called when Legato is destroyed' % (name))
	screenHeaderFile.write('void screenUpdate_%s(); // called when Legato is updating' % (name))
	screenHeaderFile.writeNewLine()
	screenHeaderFile.write('leWidget* screenGetRoot_%s(uint32_t lyrIdx); // gets a root widget for this screen' % (name))
	screenHeaderFile.writeNewLine()

	# events
	generateEventDeclarations(screenHeaderFile, scr, True)

	# macros
	generateMacroDeclarations(screenHeaderFile, scr)

	screenHeaderFile.write("//DOM-IGNORE-BEGIN")
	screenHeaderFile.write("#ifdef __cplusplus")
	screenHeaderFile.write("}")
	screenHeaderFile.write("#endif")
	screenHeaderFile.write("//DOM-IGNORE-END")
	screenHeaderFile.writeNewLine()
	screenHeaderFile.write('#endif // LE_GEN_SCREEN_' + name.upper() + "_H")
	
	screenHeaderFile.close()
	
	global fileDict
	fileDict[screenHeaderFile.name] = screenHeaderFile
	
	screenSourceFile = File("generated/screen/le_gen_screen_%s.c" % name)
	
	screenSourceFile.write('#include "gfx/legato/generated/screen/le_gen_screen_%s.h"' % (name))
	screenSourceFile.writeNewLine()

	generateWidgetDeclarations(screenSourceFile, scr)

	screenStringList = getWidgetStringList(screenWidgetList)

	if len(screenStringList) > 0:
		screenSourceFile.write("// string list for this screen")

		for stringName in screenStringList:
			screenSourceFile.write('static leTableString tableString_%s;' % (stringName))

	screenSourceFile.writeNewLine()

	generateEventDeclarations(screenSourceFile, scr, False)

	screenSourceFile.write('static leBool showing = LE_FALSE;')
	screenSourceFile.writeNewLine()
	screenSourceFile.write('leResult screenInit_%s()' % (name))
	screenSourceFile.write('{')
	screenSourceFile.write('    return LE_SUCCESS;')
	screenSourceFile.write('}')
	screenSourceFile.writeNewLine()
	screenSourceFile.write('leResult screenShow_%s()' % (name))
	screenSourceFile.write('{')
	generateScreenInitCode(screenSourceFile, scr, screenWidgetList, screenStringList);
	screenSourceFile.writeNewLine()

	if scr.getEventByName("ShowEvent").enabled == True:
		screenSourceFile.write('    %s_OnShow();' % name)
		screenSourceFile.writeNewLine()

	screenSourceFile.write("    return LE_SUCCESS;")

	screenSourceFile.write('}')
	screenSourceFile.writeNewLine()
	screenSourceFile.write('void screenUpdate_%s()' % (name))
	screenSourceFile.write('{')

	if scr.getEventByName("UpdateEvent").enabled == True:
		screenSourceFile.write('    %s_OnUpdate();' % name)

	screenSourceFile.write('}')
	screenSourceFile.writeNewLine()
	screenSourceFile.write('void screenHide_%s()' % (name))
	screenSourceFile.write('{')
	generateScreenCleanupCode(screenSourceFile, scr, screenWidgetList, screenStringList);

	if scr.getEventByName("HideEvent").enabled == True:
		screenSourceFile.writeNewLine()
		screenSourceFile.write('    %s_OnHide();' % name)

	screenSourceFile.write('}')
	screenSourceFile.writeNewLine()
	screenSourceFile.write('void screenDestroy_%s()' % (name))
	screenSourceFile.write('{')
	screenSourceFile.writeNewLine()
	screenSourceFile.write('}')
	screenSourceFile.writeNewLine()
	screenSourceFile.write('leWidget* screenGetRoot_%s(uint32_t lyrIdx)' % (name))
	screenSourceFile.write('{')
	screenSourceFile.write('    if(lyrIdx >= LE_LAYER_COUNT)')
	screenSourceFile.write('        return NULL;')
	screenSourceFile.writeNewLine()
	screenSourceFile.write('    switch(lyrIdx)')
	screenSourceFile.write('    {')
	
	lyrIdx = 0
	
	for layer in layerList:
		screenSourceFile.write('        case %d:' % lyrIdx)
		screenSourceFile.write('        {')
		screenSourceFile.write('            return root%d;' % lyrIdx)
		screenSourceFile.write('        }')
	
	screenSourceFile.write('        default:')
	screenSourceFile.write('        {')
	screenSourceFile.write('            return NULL;')
	screenSourceFile.write('        }')
	screenSourceFile.write('    }')
	screenSourceFile.write('}')
	screenSourceFile.writeNewLine()

	# events
	generateEventFunctions(screenSourceFile, scr)

	# macros
	generateMacroFunctions(screenSourceFile, scr)

	screenSourceFile.close()
	
	fileDict[screenSourceFile.name] = screenSourceFile


def generateWidgetHeaderDeclarations(file, scr, widgets):
	global radioButtonGroups
	global radialMenus

	if len(widgets) == 0:
		return

	file.write("// widget list for this screen")

	layerList = scr.getLayerList()
	layerIdx = 0

	for layer in layerList:

		layerWidgetList = layer.getDescendents()

		if len(layerWidgetList) == 0:
			continue

		file.write("// layer %d" % layerIdx)

		layerIdx += 1

		for widget in layerWidgetList:
			type = widget.getType()

			if type == "PanelWidget":
				type = "Widget"
			elif type == "RadioButtonWidget":
				if widget.getGroup() != -1:
					group = findRadioGroup(widget.getGroup())

					if group == None:
						group = RadioButtonGroup(widget.getGroup())

						radioButtonGroups.append(group)

					if widget.getSelected() == True:
						group.selectedButton = widget
					else:
						group.buttons.append(widget)

				radioButtonGroups.sort()
			elif type == "RadialMenuWidget":
				menu = RadialMenu(widget)

				radialMenus.append(menu)

			file.write('extern le%s* %s;' % (type, widget.getName()))

		file.writeNewLine()

def generateWidgetDeclarations(file, scr):
	global radioButtonGroups
	global radialMenus

	layerList = scr.getLayerList()

	layerIdx = 0

	for layer in layerList:
		widgetList = layer.getDescendents()

		if len(widgetList) == 0:
			continue

		file.write("// widget list for layer %d" % layerIdx)
		file.write('static leWidget* root%d;' % layerIdx)

		layerIdx += 1

		file.writeNewLine()

		for widget in widgetList:
			type = widget.getType()

			if type == "PanelWidget":
				type = "Widget"

			file.write('le%s* %s;' % (type, widget.getName()))

		if len(radioButtonGroups) > 0:
			file.writeNewLine()

			for group in radioButtonGroups:
				file.write('static leRadioButtonGroup* RadioButtonGroup_%d;' % (group.id))

		if len(radialMenus) > 0:
			file.writeNewLine()

			for menu in radialMenus:
				for item in menu.items:
					file.write('static leImageScaleWidget* %s;' % (item.name))


		file.writeNewLine()
	
def generateScreenInitCode(file, screen, screenWidgetList, stringList):
	global radioButtonGroups
	global radialMenus

	file.write('    if(showing == LE_TRUE)')
	file.write('        return LE_FAILURE;')
	file.writeNewLine()

	if len(stringList) > 0:
		file.write("    // initialize static strings")
	
		for stringName in stringList:
			file.write('    leTableString_Constructor(&tableString_%s, string_%s);' % (stringName, stringName))

		file.write("")

	layerList = scr.getLayerList()
	
	layerIdx = 0
	
	for layer in layerList:
		rootName = "root%d" % layerIdx
		
		file.write('    // layer %d' % layerIdx)
		file.write('    %s = leWidget_New();' % (rootName))
		file.write('    %s->fn->setPosition(%s, 0, 0);' % (rootName, rootName))
		file.write('    %s->fn->setSize(%s, %d, %d);' % (rootName, rootName, screen.getWidth(), screen.getHeight()))
		file.write('    %s->fn->setBackgroundType(%s, LE_WIDGET_BACKGROUND_NONE);' % (rootName, rootName))
		file.write('    %s->fn->setMargins(%s, 0, 0, 0, 0);' % (rootName, rootName))
		file.writeNewLine()
		
		childList = layer.getChildren()
		
		for widget in childList:
			generateWidget(file, screen, widget, rootName)

		if len(radioButtonGroups) > 0:
			for group in radioButtonGroups:
				file.write('    leRadioButtonGroup_Create(&RadioButtonGroup_%d);' % (group.id))

				file.write('    leRadioButtonGroup_AddButton(RadioButtonGroup_%d, %s);' % (group.id, group.selectedButton.getName()))

				for btn in group.buttons:
					file.write('    leRadioButtonGroup_AddButton(RadioButtonGroup_%d, %s);' % (group.id, btn.getName()))

			file.writeNewLine()

	file.write("    leAddRootWidget(%s, 0);" % rootName)
	file.writeNewLine()
	file.write("    showing = LE_TRUE;")
		
def generateScreenCleanupCode(file, screen, screenWidgetList, stringList):
	layerIdx = 0
	
	layerList = screen.getLayerList()

	if len(radioButtonGroups) > 0:
		file.writeNewLine()

		for group in radioButtonGroups:
			file.write('    leRadioButtonGroup_Destroy(RadioButtonGroup_%d);' % (group.id))

		file.writeNewLine()

	for layer in layerList:
		rootName = "root%d" % layerIdx

		file.write("    leRemoveRootWidget(%s, 0);" % rootName)
		file.writeNewLine()
		file.write("    leWidget_Delete(%s);" % rootName)
		file.writeNewLine()
		file.write("    %s = NULL;" % rootName)

		descList = layer.getDescendents()

		if len(descList) > 0:
			file.writeNewLine()

			for widget in descList:
				file.write("    %s = NULL;" % (widget.getName()))

	if len(radialMenus) > 0:
		file.writeNewLine()

		for menu in radialMenus:
			for item in menu.items:
				file.write('    %s = NULL;' % (item.name))

		file.writeNewLine()

	if len(stringList) > 0:
		file.writeNewLine()

		for stringName in stringList:
			file.write('    tableString_%s.fn->destructor(&tableString_%s);' % (stringName, stringName))

	file.write("    showing = LE_FALSE;")


def generateEventDeclarations(file, scr, header):
	eventList = [];
	layerList = scr.getLayerList()

	scrEvents = scr.getEventList()

	# determine if there are any events to generate
	if scrEvents is not None:
		for event in scrEvents:
			if event.enabled == False:
				continue

			if header == False and event.mode.toString() == "DECLARE":
				continue

			if header == True and event.mode.toString() != "DECLARE":
				continue

			eventList.append(event)

	layerIdx = 0

	for layer in layerList:
		layerWidgetList = layer.getDescendents()

		for widget in layerWidgetList:
			wgtEvents = widget.getEventList()

			for event in wgtEvents:
				if event.enabled == False:
					continue

				if header == False and event.mode.toString() == "DECLARE":
					continue

				if header == True and event.mode.toString() != "DECLARE":
					continue

				eventList.append(event)

	if len(eventList) > 0:
		file.write("// event handlers")

		list = scr.getEventList()

		for event in list:
			if event.enabled == False:
				continue

			if header == False and event.mode.toString() == "DECLARE":
				continue

			if header == True and event.mode.toString() != "DECLARE":
				continue

			if event.validate() == False:
				print("Failed to validate event: %s" % event.name)

			text = generateEvent(scr, None, event, False);

			if header == False:
				text = "static " + text

			file.write(text)

		layerIdx = 0

		for layer in layerList:
			layerWidgetList = layer.getDescendents()

			for widget in layerWidgetList:
				wgtEvents = widget.getEventList()

				for event in wgtEvents:
					if event.enabled == False:
						continue

					if header == False and event.mode.toString() == "DECLARE":
						continue

					if header == True and event.mode.toString() != "DECLARE":
						continue

					if event.validate() == False:
						print("Failed to validate event: %s" % (event.name))

					text = generateEvent(scr, widget, event, False)

					if header == False:
						text = "static " + text

					file.write(text)

		file.write("")


def generateEventFunctions(file, scr):
	eventList = [];
	layerList = scr.getLayerList()

	scrEvents = scr.getEventList()

	if scrEvents is not None:
		for event in scrEvents:
			if event.enabled == False:
				continue

			if event.mode.toString() == "DECLARE":
				continue

			eventList.append(event)

	layerIdx = 0

	for layer in layerList:
		layerWidgetList = layer.getDescendents()

		for widget in layerWidgetList:
			wgtEvents = widget.getEventList()

			for event in wgtEvents:
				if event.enabled == False:
					continue

				if event.mode.toString() == "DECLARE":
					continue

				eventList.append(event)

	if len(eventList) > 0:
		file.write("// event handlers")

		list = scr.getEventList()

		for event in list:
			if event.enabled == False:
				continue

			if event.mode.toString() == "DECLARE":
				continue

			if event.validate() == False:
				print("Failed to validate event: %s" % event.name)

			text = generateEvent(scr, None, event, True);

			file.write(text)
			file.writeNewLine()

		layerIdx = 0

		for layer in layerList:
			layerWidgetList = layer.getDescendents()

			for widget in layerWidgetList:
				wgtEvents = widget.getEventList()

				for event in wgtEvents:
					if event.enabled == False:
						continue

					if event.mode.toString() == "DECLARE":
						continue

					if event.validate() == False:
						print("Failed to validate event: %s" % event.name)

					text = generateEvent(scr, widget, event, True)

					file.write(text)
					file.writeNewLine()

		file.write("")


def generateMacroDeclarations(file, scr):
	name = scr.getName()

	macroList = scr.getMacroList();

	if len(macroList) > 0:
		file.write("// screen macros")

		for idx, macro in enumerate(macroList):
			desc = macro.description

			if desc == None or len(desc) == 0:
				desc = "Macro '%s' defined for %s" % (macro.name, name)

			file.write('void %s_%s(); // %s' % (name, macro.name, desc))

		file.write("")


def generateMacroFunctions(file, scr):
	name = scr.getName()

	macroList = scr.getMacroList();

	if len(macroList) > 0:
		file.write('')

		for idx, macro in enumerate(macroList):
			if macro.enabled == False:
				continue

			if macro.validate() == False:
				print("Failed to validate macro %s" % macro.name)

				continue

			desc = macro.description

			if desc == None or len(desc) == 0:
				desc = "Macro '%s' - defined for %s" % (macro.name, name)

			file.write("/* %s */" % desc)
			file.write('void %s_%s()' % (name, macro.name))

			nullCheckText = []
			nullCheckText.append("if(root0 == NULL) // make sure this screen exists")
			nullCheckText.append("    return;")

			text = generateActions(scr, macro, True, nullCheckText, None)

			file.write(text);

		file.write('')

def generateScreenAction(text, variables, owner, event, action):
	text.append("    legato_showScreen(screenID_%s);" % action.targetName)