widget = WidgetDefault()
widget.halign = "Left"
commonDefaults["WindowWidget"] = widget


def generateWindowWidget(file, screen, win, parentName):
    name = win.getName()

    file.write("    %s = leWindowWidget_New();" % (name))

    generateBaseWidget(file, screen, win)

    writeSetInt(file, name, "TitleHeight", win.getTitleHeight(), 20)
    writeSetStringAssetName(file, name, "String", win.getTitleName())
    writeSetAssetName(file, name, "Icon", win.getIconName())
    writeSetInt(file, name, "IconMargin", win.getMargin(), 10)

    file.write("    %s->fn->addChild(%s, (leWidget*)%s);" % (parentName, parentName, name))
    file.writeNewLine()


def generateWindowAction(text, variables, owner, event, action):
    name = action.targetName

    if action.actionID == "SetString":
        val = getActionArgumentValue(action, "String")

        writeActionFunc(text, action, "setString", [val])

    elif action.actionID == "SetIcon":
        val = getActionArgumentValue(action, "Icon")

        writeActionFunc(text, action, "setIcon", [val])

    elif action.actionID == "SetIconMargin":
        val = getActionArgumentValue(action, "Margin")

        writeActionFunc(text, action, "setIconMargin", [val])

    else:
        generateWidgetAction(text, variables, owner, event, action)