widget = WidgetDefault()
widget.border = "None"
widget.background = "None"
commonDefaults["LineWidget"] = widget


def generateLineWidget(file, screen, line, parentName):
    name = line.getName()

    file.write("    %s = leLineWidget_New();" % (name))

    generateBaseWidget(file, screen, line)

    file.write("    %s->fn->setStartPoint(%s, %d, %d);" % (name, name, line.getP1X(), line.getP1Y()))
    file.write("    %s->fn->setEndPoint(%s, %d, %d);" % (name, name, line.getP2X(), line.getP2Y()))

    file.write("    %s->fn->addChild(%s, (leWidget*)%s);" % (parentName, parentName, name))
    file.writeNewLine()

def generateLineAction(text, variables, owner, event, action):
    name = action.targetName

    if action.actionID == "SetStartPont":
        x = getActionArgumentValue(action, "P1X")
        y = getActionArgumentValue(action, "P1Y")

        writeActionFunc(text, action, "setStartPoint", [x, y])

    elif action.actionID == "SetEndPoint":
        x = getActionArgumentValue(action, "P2X")
        y = getActionArgumentValue(action, "P2Y")

        writeActionFunc(text, action, "setEndPoint", [x, y])

    else:
        generateWidgetAction(text, variables, owner, event, action)