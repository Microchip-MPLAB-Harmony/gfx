widget = WidgetDefault()
widget.border = "None"
commonDefaults["PanelWidget"] = widget


def generatePanelWidget(file, screen, panel, parentName):
    name = panel.getName()

    file.write("    %s = leWidget_New();" % (name))

    generateBaseWidget(file, screen, panel)

    file.write("    %s->fn->addChild(%s, %s);" % (parentName, parentName, name))
    file.writeNewLine()

def generatePanelAction(text, variables, owner, event, action):
    generateWidgetAction(text, variables, owner, event, action)