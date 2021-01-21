widget = WidgetDefault()
widget.border = "None"
commonDefaults["GroupBoxWidget"] = widget


def generateGroupBoxWidget(file, screen, box, parentName):
    name = box.getName()

    file.write("    %s = leGroupBoxWidget_New();" % (name))

    generateBaseWidget(file, screen, box)

    writeSetStringAssetName(file, name, "String", box.getStringName())

    file.write("    %s->fn->addChild(%s, (leWidget*)%s);" % (parentName, parentName, name))
    file.writeNewLine()


def generateGroupBoxAction(text, variables, owner, event, action):
    name = action.targetName

    if action.actionID == "SetString":
        val = getActionArgumentValue(action, "String")

        writeActionFunc(text, action, "setString", [val])

    else:
        generateWidgetAction(text, variables, owner, event, action)