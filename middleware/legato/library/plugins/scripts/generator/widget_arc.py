widget = WidgetDefault()
widget.width = 101
widget.height = 101
widget.border = "None"
widget.background = "None"
commonDefaults["ArcWidget"] = widget


def generateArcWidget(file, screen, arc, parentName):
    name = arc.getName()

    file.write("    %s = leArcWidget_New();" % (name))

    generateBaseWidget(file, screen, arc)

    writeSetInt(file, name, "Radius", arc.getRadius(), 50)
    writeSetInt(file, name, "StartAngle", arc.getStartAngle(), 0)
    writeSetInt(file, name, "CenterAngle", arc.getCenterAngle(), 135)
    writeSetInt(file, name, "Thickness", arc.getThickness(), 20)
    writeSetBoolean(file, name, "RoundEdge", arc.getUseRoundedEdge(), False)

    file.write("    %s->fn->addChild(%s, (leWidget*)%s);" % (parentName, parentName, name))
    file.writeNewLine()


def generateArcAction(text, variables, owner, event, action):
    if action.actionID == "SetRadius":
        val = getActionArgumentValue(action, "Radius")

        writeActionFunc(text, action, "setRadius", [val])

    elif action.actionID == "SetStartAngle":
        val = getActionArgumentValue(action, "StartAngle")

        writeActionFunc(text, action, "setStartAngle", [val])

    elif action.actionID == "SetCenterAngle":
        val = getActionArgumentValue(action, "CenterAngle")

        writeActionFunc(text, action, "setCenterAngle", [val])

    elif action.actionID == "SetThickness":
        val = getActionArgumentValue(action, "Thickness")

        writeActionFunc(text, action, "setThickness", [val])

    elif action.actionID == "SetRoundEdge":
        val = getActionArgumentValue(action, "RoundEdge")

        writeActionFunc(text, action, "setRoundEdge", [val])

    else:
        generateWidgetAction(text, variables, owner, event, action)