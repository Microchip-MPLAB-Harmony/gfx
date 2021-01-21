widget = WidgetDefault()
widget.height = 25
widget.border = "None"
widget.halign = "Left"
commonDefaults["LabelWidget"] = widget


def generateLabelWidget(file, screen, label, parentName):
    name = label.getName()

    file.write("    %s = leLabelWidget_New();" % (name))

    generateBaseWidget(file, screen, label)

    writeSetStringAssetName(file, name, "String", label.getStringName())

    file.write("    %s->fn->addChild(%s, (leWidget*)%s);" % (parentName, parentName, name))
    file.writeNewLine()

def generateLabelAction(text, variables, owner, event, action):
    name = action.targetName

    if action.actionID == "SetString":
        val = getActionArgumentValue(action, "String")

        writeActionFunc(text, action, "setString", [val])

    else:
        generateWidgetAction(text, variables, owner, event, action)