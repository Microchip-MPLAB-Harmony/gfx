widget = WidgetDefault()
widget.height = 25
widget.halign = "Left"
commonDefaults["TextFieldWidget"] = widget


def generateTextFieldWidget(file, screen, text, parentName):
    name = text.getName()

    file.write("    %s = leTextFieldWidget_New();" % (name))

    generateBaseWidget(file, screen, text)

    writeSetStringAssetName(file, name, "String", text.getStringName())
    writeSetStringAssetName(file, name, "HintString", text.getHintStringName())
    writeSetFontAssetName(file, name, "Font", text.getFontName())
    writeSetBoolean(file, name, "CursorEnabled", text.getCursorEnabled(), True)
    writeSetInt(file, name, "CursorDelay", text.getCursorDelay(), 500)
    writeSetBoolean(file, name, "ClearValueOnFirstEdit", text.getClearOnFirstEdit(), True)

    writeEvent(file, name, text, "TextChangedEvent", "TextChangedEventCallback", "OnTextChanged")
    writeEvent(file, name, text, "FocusChangedEvent", "FocusChangedEventCallback", "OnFocusChanged")

    file.write("    %s->fn->addChild(%s, (leWidget*)%s);" % (parentName, parentName, name))
    file.writeNewLine()


def generateTextFieldEvent(screen, widget, event, genActions):
    text = ""

    if event.name == "TextChangedEvent":
        text += "void %s_OnTextChanged(%s)\n" % (widget.getName(), getWidgetVariableName(widget))

    if event.name == "FocusChangedEvent":
        text += "void %s_OnFocusChanged(%s, leBool focused)\n" % (widget.getName(), getWidgetVariableName(widget))

    text += generateActions(widget, event, genActions, None, None)

    return text


def generateTextFieldAction(text, variables, owner, event, action):
    name = action.targetName

    if action.actionID == "SetValue":
        val = getActionArgumentValue(action, "String")

        writeActionFunc(text, action, "setString", [val])

    elif action.actionID == "SetCursorEnabled":
        val = getActionArgumentValue(action, "Enable")

        writeActionFunc(text, action, "setCursorEnabled", [val])

    elif action.actionID == "SetCursorDelay":
        val = getActionArgumentValue(action, "Delay")

        writeActionFunc(text, action, "setCursorDelay", [val])

    elif action.actionID == "StartEdit":
        writeActionFunc(text, action, "editStart", [])

    elif action.actionID == "ClearText":
        writeActionFunc(text, action, "editClear", [])

    elif action.actionID == "AcceptText":
        writeActionFunc(text, action, "editAccept", [])

    elif action.actionID == "AppendText":
        val = getActionArgumentValue(action, "String")

        writeActionFunc(text, action, "editAppend", [val])

    elif action.actionID == "Backspace":
        writeActionFunc(text, action, "editBackspace", [])

    else:
        generateWidgetAction(text, variables, owner, event, action)