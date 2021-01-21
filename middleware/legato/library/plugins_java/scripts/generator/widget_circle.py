widget = WidgetDefault()
widget.width = 101
widget.height = 101
widget.border = "None"
widget.background = "None"
commonDefaults["CircleWidget"] = widget

def generateCircleWidget(file, screen, circle, parentName):
    name = circle.getName()

    file.write("    %s = leCircleWidget_New();" % (name))

    generateBaseWidget(file, screen, circle)

    writeSetInt(file, name, "OriginX", circle.getOriginX(), 50)
    writeSetInt(file, name, "OriginY", circle.getOriginY(), 50)
    writeSetInt(file, name, "Radius", circle.getRadius(), 50)
    writeSetInt(file, name, "Thickness", circle.getThickness(), 1)
    writeSetBoolean(file, name, "Filled", circle.getFilled(), False)

    file.write("    %s->fn->addChild(%s, (leWidget*)%s);" % (parentName, parentName, name))
    file.writeNewLine()


def generateCircleAction(text, variables, owner, event, action):
    name = action.targetName

    variables["pnt"] = "lePoint"

    if action.actionID == "SetOrigin":
        text.append("    pnt.x = %s;" % getActionArgumentValue(action, "OriginX"))
        text.append("    pnt.y = %s;" % getActionArgumentValue(action, "OriginY"))
        text.append("    %s->fn->setOrigin(%s, pnt);" % (name, name))

    elif action.actionID == "SetRadius":
        writeActionFunc(text, action, "setRadius", [getActionArgumentValue(action, "Radius")])

    elif action.actionID == "SetThickness":
        writeActionFunc(text, action, "setThickness", [getActionArgumentValue(action, "Thickness")])

    elif action.actionID == "SetFilled":
        writeActionFunc(text, action, "setFilled", [getActionArgumentValue(action, "Filled")])

    else:
        generateWidgetAction(text, variables, owner, event, action)

