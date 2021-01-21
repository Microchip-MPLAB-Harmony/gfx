widget = WidgetDefault()
widget.height = 25
widget.border = "None"
widget.halign = "Left"
commonDefaults["RadioButtonWidget"] = widget


def generateRadioButtonWidget(file, screen, btn, parentName):
    name = btn.getName()

    file.write("    %s = leRadioButtonWidget_New();" % (name))

    generateBaseWidget(file, screen, btn)

    writeSetStringAssetName(file, name, "String", btn.getStringName())
    writeSetAssetName(file, name, "SelectedImage", btn.getSelectedImageName())
    writeSetAssetName(file, name, "UnselectedImage", btn.getUnselectedImageName())
    writeSetLiteralString(file, name, "ImagePosition", getRelativePosition(btn.getImagePosition().toString()), "LE_RELATIVE_POSITION_LEFTOF")
    writeSetInt(file, name, "ImageMargin", btn.getImageMargin(), 10)
    writeSetInt(file, name, "CircleButtonSize", btn.getCircleButtonSize(), 12)

    writeEvent(file, name, btn, "SelectedEvent", "SelectedEventCallback", "OnSelected")
    writeEvent(file, name, btn, "DeselectedEvent", "DeselectedEventCallback", "OnDeselected")

    file.write("    %s->fn->addChild(%s, (leWidget*)%s);" % (parentName, parentName, name))
    file.writeNewLine()


def generateRadioButtonEvent(screen, widget, event, genActions):
    text = ""

    if event.name == "SelectedEvent":
        text += "void %s_OnSelected(%s)\n" % (widget.getName(), getWidgetVariableName(widget))

    if event.name == "DeselectedEvent":
        text += "void %s_OnDeselected(%s)\n" % (widget.getName(), getWidgetVariableName(widget))

    text += generateActions(widget, event, genActions, None, None)

    return text

def generateRadioButtonAction(text, variables, owner, event, action):
    name = action.targetName

    if action.actionID == "Select":
        writeActionFunc(text, action, "select", [])

    elif action.actionID == "SetString":
        val = getActionArgumentValue(action, "String")

        writeActionFunc(text, action, "setString", [val])

    elif action.actionID == "SetSelectedImage":
        val = getActionArgumentValue(action, "Image")

        writeActionFunc(text, action, "setSelectedImage", [val])

    elif action.actionID == "SetUnselectedImage":
        val = getActionArgumentValue(action, "Image")

        writeActionFunc(text, action, "setUnselectedImage", [val])

    elif action.actionID == "SetImageMargin":
        val = getActionArgumentValue(action, "Margin")

        writeActionFunc(text, action, "setImageMargin", [val])

    elif action.actionID == "SetImagePosition":
        val = getRelativePosition(getActionArgumentValue(action, "Position"))

        writeActionFunc(text, action, "setImagePosition", [val])

    elif action.actionID == "SetCircleSize":
        val = getActionArgumentValue(action, "Size")

        writeActionFunc(text, action, "setCircleButtonSize", [val])

    else:
        generateWidgetAction(text, variables, owner, event, action)