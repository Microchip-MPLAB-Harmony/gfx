widget = WidgetDefault()
widget.width = 20
widget.background = "Fill"
commonDefaults["SliderWidget"] = widget


def generateSliderWidget(file, screen, sld, parentName):
    name = sld.getName()

    file.write("    %s = leSliderWidget_New();" % (name))

    generateBaseWidget(file, screen, sld)

    orientation = getOrientation(sld.getOrientation().toString())

    if orientation != "LE_ORIENTATION_VERTICAL":
        file.write("    %s->fn->setOrientation(%s, LE_ORIENTATION_HORIZONTAL, LE_FALSE);" % (name, name))

    writeSetInt(file, name, "MinimumValue", sld.getMin(), 0)
    writeSetInt(file, name, "MaximumValue", sld.getMax(), 100)
    writeSetInt(file, name, "Value", sld.getValue(), 0)
    writeSetInt(file, name, "GripSize", sld.getGrip(), 10)

    writeEvent(file, name, sld, "ValueChangedEvent", "ValueChangedEventCallback", "OnValueChanged")

    file.write("    %s->fn->addChild(%s, (leWidget*)%s);" % (parentName, parentName, name))
    file.writeNewLine()


def generateSliderEvent(screen, widget, event, genActions):
    text = ""

    if event.name == "ValueChangedEvent":
        text += "void %s_OnValueChanged(%s)\n" % (widget.getName(), getWidgetVariableName(widget))

    text += generateActions(widget, event, genActions, None, None)

    return text


def generateSliderAction(text, variables, owner, event, action):
    name = action.targetName

    if action.actionID == "SetOrientation":
        val = getOrientation(getActionArgumentValue(action, "Orientation"))

        writeActionFunc(text, action, "setOrientation", [val, "LE_TRUE"])

    elif action.actionID == "SetMin":
        val = getActionArgumentValue(action, "Min")

        writeActionFunc(text, action, "setMinimumValue", [val])

    elif action.actionID == "SetMax":
        val = getActionArgumentValue(action, "Max")

        writeActionFunc(text, action, "setMaximumValue", [val])

    elif action.actionID == "SetValue":
        val = getActionArgumentValue(action, "Value")

        writeActionFunc(text, action, "setValue", [val])

    elif action.actionID == "SetValuePercent":
        val = getActionArgumentValue(action, "Value")

        writeActionFunc(text, action, "setPercentage", [val])

    elif action.actionID == "SetGripSize":
        val = getActionArgumentValue(action, "Size")

        writeActionFunc(text, action, "setGripSize", [val])

    elif action.actionID == "Step":
        val = getActionArgumentValue(action, "Amount")

        writeActionFunc(text, action, "step", [val])

    else:
        generateWidgetAction(text, variables, owner, event, action)