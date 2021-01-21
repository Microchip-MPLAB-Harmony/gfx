widget = WidgetDefault()
widget.height = 25
commonDefaults["ProgressBarWidget"] = widget


def generateProgressBarWidget(file, screen, bar, parentName):
    name = bar.getName()

    file.write("    %s = leProgressBarWidget_New();" % (name))

    generateBaseWidget(file, screen, bar)

    writeSetLiteralString(file, name, "Direction", getDirection(bar.getDirection().toString()), "LE_DIRECTION_RIGHT")
    writeSetInt(file, name, "Value", bar.getValue(), 0)

    writeEvent(file, name, bar, "ValueChangedEvent", "ValueChangedCallback", "OnValueChanged")

    file.write("    %s->fn->addChild(%s, (leWidget*)%s);" % (parentName, parentName, name))
    file.writeNewLine()


def generateProgressBarEvent(screen, widget, event, genActions):
    text = ""

    if event.name == "ValueChangedEvent":
        text += "void %s_OnValueChanged(%s, uint32_t val)\n" % (widget.getName(), getWidgetVariableName(widget))

    text += generateActions(widget, event, genActions, None, None)

    return text


def generateProgressBarAction(text, variables, owner, event, action):
    name = action.targetName

    if action.actionID == "SetDirection":
        val = getDirection(getActionArgumentValue(action, "Direction"))

        writeActionFunc(text, action, "setDirection", [val])

    elif action.actionID == "SetValue":
        val = getActionArgumentValue(action, "Value")

        writeActionFunc(text, action, "setValue", [val])

    else:
        generateWidgetAction(text, variables, owner, event, action)