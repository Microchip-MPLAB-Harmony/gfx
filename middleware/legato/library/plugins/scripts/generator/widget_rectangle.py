widget = WidgetDefault()
widget.border = "None"
widget.background = "Fill"
commonDefaults["RectangleWidget"] = widget


def generateRectangleWidget(file, screen, rect, parentName):
    name = rect.getName()

    file.write("    %s = leRectangleWidget_New();" % (name))

    generateBaseWidget(file, screen, rect)

    writeSetInt(file, name, "Thickness", rect.getThickness(), 2)

    file.write("    %s->fn->addChild(%s, (leWidget*)%s);" % (parentName, parentName, name))
    file.writeNewLine()


def generateRectangleAction(text, variables, owner, event, action):
    name = action.targetName

    if action.actionID == "SetThickness":
        val = getActionArgumentValue(action, "Thickness")

        writeActionFunc(text, action, "setThickness", [val])

    else:
        generateWidgetAction(text, variables, owner, event, action)