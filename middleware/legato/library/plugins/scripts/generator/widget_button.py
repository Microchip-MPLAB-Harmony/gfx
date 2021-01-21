widget = WidgetDefault()
widget.height = 25
commonDefaults["ButtonWidget"] = widget


def generateButtonWidget(file, screen, button, parentName):
    name = button.getName()

    file.write("    %s = leButtonWidget_New();" % (name))

    generateBaseWidget(file, screen, button)

    writeSetBoolean(file, name, "Toggleable", button.getToggleable(), False)
    writeSetBoolean(file, name, "Pressed", button.getPressed(), False)
    writeSetStringAssetName(file, name, "String", button.getStringName())
    writeSetAssetName(file, name, "PressedImage", button.getPressedImageName())
    writeSetAssetName(file, name, "ReleasedImage", button.getReleasedImageName())
    writeSetLiteralString(file, name, "ImagePosition", getRelativePosition(button.getImagePosition().toString()), "LE_RELATIVE_POSITION_LEFTOF")
    writeSetInt(file, name, "ImageMargin", button.getImageMargin(), 10)
    writeSetInt(file, name, "PressedOffset", button.getPressedOffset(), 1)
    writeEvent(file, name, button, "PressedEvent", "PressedEventCallback", "OnPressed")
    writeEvent(file, name, button, "ReleasedEvent", "ReleasedEventCallback", "OnReleased")

    file.write("    %s->fn->addChild(%s, (leWidget*)%s);" % (parentName, parentName, name))
    file.writeNewLine()


def generateButtonEvent(screen, widget, event, genActions):
    text = ""

    if event.name == "PressedEvent":
        text += "void %s_OnPressed(%s)\n" % (widget.getName(), getWidgetVariableName(widget))

    if event.name == "ReleasedEvent":
        text += "void %s_OnReleased(%s)\n" % (widget.getName(), getWidgetVariableName(widget))

    text += generateActions(widget, event, genActions, None, None)

    return text


def generateButtonAction(text, variables, owner, event, action):
    name = action.targetName

    if action.actionID == "SetToggleable":
        writeActionFunc(text, action, "setToggleable", [getActionArgumentValue(action, "Toggle")])

    elif action.actionID == "SetPressState":
        writeActionFunc(text, action, "setPressed", [getActionArgumentValue(action, "Press")])

    elif action.actionID == "SetString":
        writeActionFunc(text, action, "setString", [getActionArgumentValue(action, "String")])

    elif action.actionID == "SetPressedImage":
        writeActionFunc(text, action, "setPressedImage", [getActionArgumentValue(action, "PressedImage")])

    elif action.actionID == "SetReleasedImage":
        writeActionFunc(text, action, "setReleasedImage", [getActionArgumentValue(action, "ReleasedImage")])

    elif action.actionID == "SetImageMargin":
        writeActionFunc(text, action, "setImageMargin", [getActionArgumentValue(action, "ImageMargin")])

    elif action.actionID == "SetPressedOffset":
        writeActionFunc(text, action, "setPressedOffset", [getActionArgumentValue(action, "PressedOffset")])

    elif action.actionID == "SetImagePosition":
        position = getRelativePosition(getActionArgumentValue(action, "ImagePosition"))

        writeActionFunc(text, action, "setImagePosition", [position])

    else:
        generateWidgetAction(text, variables, owner, event, action)