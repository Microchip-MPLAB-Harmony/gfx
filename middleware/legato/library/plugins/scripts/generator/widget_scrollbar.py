widget = WidgetDefault()
widget.width = 20
widget.background = "Fill"
commonDefaults["ScrollBarWidget"] = widget


def generateScrollBarWidget(file, screen, bar, parentName):
    name = bar.getName()

    file.write("    %s = leScrollBarWidget_New();" % (name))

    generateBaseWidget(file, screen, bar)

    orientation = getOrientation(bar.getOrientation().toString())

    if orientation != "LE_ORIENTATION_VERTICAL":
        file.write("    %s->fn->setOrientation(%s, LE_ORIENTATION_HORIZONTAL, LE_FALSE);" % (name, name))

    writeSetInt(file, name, "MaximumValue", bar.getMax(), 100)
    writeSetInt(file, name, "ExtentValue", bar.getExtent(), 10)
    writeSetInt(file, name, "StepSize", bar.getStep(), 1)
    writeSetInt(file, name, "ScrollValue", bar.getValue(), 0)

    writeEvent(file, name, bar, "ValueChangedEvent", "ValueChangedEventCallback", "OnValueChanged")

    file.write("    %s->fn->addChild(%s, (leWidget*)%s);" % (parentName, parentName, name))
    file.writeNewLine()


def generateScrollBarEvent(screen, widget, event, genActions):
    text = ""

    if event.name == "ValueChangedEvent":
        text += "void %s_OnValueChanged(%s)\n" % (widget.getName(), getWidgetVariableName(widget))

    text += generateActions(widget, event, genActions, None, None)

    return text


def generateScrollBarAction(text, variables, owner, event, action):
    name = action.targetName

    if action.actionID == "SetOrientation":
        val = getOrientation(getActionArgumentValue(action, "Orientation"))

        writeActionFunc(text, action, "setOrientation", [val, "LE_TRUE"])

    elif action.actionID == "SetMax":
        val = getActionArgumentValue(action, "Max")

        writeActionFunc(text, action, "setMaximumValue", [val])

    elif action.actionID == "SetExtent":
        val = getActionArgumentValue(action, "Extent")

        writeActionFunc(text, action, "setExtentValue", [val])

    elif action.actionID == "SetValue":
        val = getActionArgumentValue(action, "Value")

        writeActionFunc(text, action, "setScrollValue", [val])

    elif action.actionID == "SetValuePercent":
        val = getActionArgumentValue(action, "Value")

        writeActionFunc(text, action, "setScrollPercentage", [val])

    elif action.actionID == "SetStepSize":
        val = getActionArgumentValue(action, "Size")

        writeActionFunc(text, action, "setStepSize", [val])

    elif action.actionID == "StepBackward":
        writeActionFunc(text, action, "stepBackward", [])

    elif action.actionID == "StepForward":
        writeActionFunc(text, action, "stepForward", [])

    else:
        generateWidgetAction(text, variables, owner, event, action)