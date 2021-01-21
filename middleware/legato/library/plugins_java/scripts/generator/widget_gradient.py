widget = WidgetDefault()
widget.border = "None"
commonDefaults["GradientWidget"] = widget


def generateGradientWidget(file, screen, grad, parentName):
    name = grad.getName()

    file.write("    %s = leGradientWidget_New();" % (name))

    generateBaseWidget(file, screen, grad)

    writeSetLiteralString(file, name, "Direction", getDirection(grad.getDirection().toString()), "LE_DIRECTION_RIGHT")

    file.write("    %s->fn->addChild(%s, (leWidget*)%s);" % (parentName, parentName, name))
    file.writeNewLine()

def generateGradientAction(text, variables, owner, event, action):
    name = action.targetName

    if action.actionID == "SetDirection":
        val = getDirection(getActionArgumentValue(action, "Direction"))

        writeActionFunc(text, action, "setDirection", [val])

    else:
        generateWidgetAction(text, variables, owner, event, action)