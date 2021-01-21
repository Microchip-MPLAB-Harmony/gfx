widget = WidgetDefault()
widget.width = 101
widget.height = 101
widget.border = "None"
widget.background = "None"
commonDefaults["CircularSliderWidget"] = widget


def generateCircularSliderWidget(file, screen, slider, parentName):
    name = slider.getName()

    file.write("    %s = leCircularSliderWidget_New();" % (name))

    generateBaseWidget(file, screen, slider)

    writeSetInt(file, name, "Radius", slider.getRadius(), 50)

    dir = slider.getDirection().toString()

    dir = getRotationDirection(dir)

    writeSetLiteralString(file, name, "Direction", dir, "LE_COUNTER_CLOCKWISE")
    writeSetInt(file, name, "StartAngle", slider.getStartAngle(), 0)
    writeSetInt(file, name, "StartValue", slider.getStartValue(), 0)
    writeSetInt(file, name, "EndValue", slider.getEndValue(), 100)
    writeSetInt(file, name, "Value", slider.getValue(), 50)
    writeSetBoolean(file, name, "RoundEdges", slider.getRoundEdges(), True)

    # outside circle border arc
    writeSetBool2Arg(file, name, "ArcVisible", slider.getOutsideBorderArc().isVisible(), True, "OUTSIDE_CIRCLE_BORDER")
    writeSetInt2Arg(file, name, "ArcThickness", slider.getOutsideBorderArc().getThickness(), 5, "OUTSIDE_CIRCLE_BORDER")

    # inside circle border arc
    rad = slider.getActiveArc().getRadius() - slider.getActiveArc().getThickness()

    writeSetBool2Arg(file, name, "ArcVisible", slider.getInsideBorderArc().isVisible(), True, "INSIDE_CIRCLE_BORDER")
    writeSetInt2Arg(file, name, "ArcThickness", slider.getInsideBorderArc().getThickness(), 5, "INSIDE_CIRCLE_BORDER")

    # inactive arc
    writeSetBool2Arg(file, name, "ArcVisible", slider.getInactiveArc().isVisible(), True, "INACTIVE_AREA")

    # active arc
    writeSetBool2Arg(file, name, "ArcVisible", slider.getActiveArc().isVisible(), True, "ACTIVE_AREA")
    writeSetInt2Arg(file, name, "ArcThickness", slider.getActiveArc().getThickness(), 30, "ACTIVE_AREA")

    # circle button arc
    scheme = slider.getCircularButtonArc().getSchemeName()

    if scheme != "NULL":
        scheme = craftAssetName(scheme)

    writeSetBool2Arg(file, name, "ArcVisible", slider.getCircularButtonArc().isVisible(), True, "CIRCLE_BUTTON")
    writeSetInt2Arg(file, name, "ArcRadius", slider.getCircularButtonArc().getRadius(), 15, "CIRCLE_BUTTON")
    writeSetInt2Arg(file, name, "ArcThickness", slider.getCircularButtonArc().getThickness(), 3, "CIRCLE_BUTTON")

    if scheme != "NULL":
        file.write("    %s->fn->setArcScheme(%s, CIRCLE_BUTTON, %s);" % (name, name, scheme))

    writeEvent(file, name, slider, "ValueChangedEvent", "ValueChangedEventCallback", "OnValueChanged")
    writeEvent(file, name, slider, "PressedEvent", "PressedEventCallback", "OnPressed")
    writeEvent(file, name, slider, "ReleasedEvent", "ReleasedEventCallback", "OnReleased")

    file.write("    %s->fn->addChild(%s, (leWidget*)%s);" % (parentName, parentName, name))
    file.writeNewLine()


def generateCircularSliderEvent(screen, widget, event, genActions):
    text = ""

    if event.name == "ValueChangedEvent":
        text += "void %s_OnValueChanged(%s, int32_t val)\n" % (widget.getName(), getWidgetVariableName(widget))

    if event.name == "PressedEvent":
        text += "void %s_OnPressed(%s)\n" % (widget.getName(), getWidgetVariableName(widget))

    if event.name == "ReleasedEvent":
        text += "void %s_OnReleased(%s)\n" % (widget.getName(), getWidgetVariableName(widget))

    text += generateActions(widget, event, genActions, None, None)

    return text

def generateCircularSliderAction(text, variables, owner, event, action):
    name = action.targetName

    if action.actionID == "SetRadius":
        writeActionFunc(text, action, "setRadius", [getActionArgumentValue(action, "Radius")])

    elif action.actionID == "SetStartAngle":
        writeActionFunc(text, action, "setStartAngle", [getActionArgumentValue(action, "StartAngle")])

    elif action.actionID == "SetValue":
        writeActionFunc(text, action, "setValue", [getActionArgumentValue(action, "Value")])

    elif action.actionID == "SetStartValue":
        writeActionFunc(text, action, "setStartValue", [getActionArgumentValue(action, "StartValue")])

    elif action.actionID == "SetEndValue":
        writeActionFunc(text, action, "setEndValue", [getActionArgumentValue(action, "EndValue")])

    elif action.actionID == "SetRoundEdges":
        writeActionFunc(text, action, "setRoundEdges", [getActionArgumentValue(action, "RoundEdges")])

    elif action.actionID == "SetStickyButton":
        writeActionFunc(text, action, "setStickyButton", [getActionArgumentValue(action, "Sticky")])

    elif action.actionID == "SetDirection":
        dir = getRotationDirection(getActionArgumentValue(action, "Direction"))

        writeActionFunc(text, action, "setDirection", [dir])

    elif action.actionID == "ShowOutsideBorder":
        val = getActionArgumentValue(action, "Show")

        writeActionFunc(text, action, "setArcVisible", ["OUTSIDE_CIRCLE_BORDER", val])

    elif action.actionID == "SetOutsideBorderThickness":
        val = getActionArgumentValue(action, "Thickness")

        writeActionFunc(text, action, "setArcThickness", ["OUTSIDE_CIRCLE_BORDER", val])

    elif action.actionID == "ShowInsideBorder":
        val = getActionArgumentValue(action, "Show")

        writeActionFunc(text, action, "setArcVisible", ["INSIDE_CIRCLE_BORDER", val])

    elif action.actionID == "SetInsideBorderThickness":
        val = getActionArgumentValue(action, "Thickness")

        writeActionFunc(text, action, "setArcThickness", ["INSIDE_CIRCLE_BORDER", val])

    elif action.actionID == "ShowActiveArcArea":
        val = getActionArgumentValue(action, "Show")

        writeActionFunc(text, action, "setArcVisible", ["ACTIVE_AREA", val])

    elif action.actionID == "SetActiveArcAreaThickness":
        val = getActionArgumentValue(action, "Thickness")

        writeActionFunc(text, action, "setArcThickness", ["ACTIVE_AREA", val])

    elif action.actionID == "ShowInactiveArc":
        val = getActionArgumentValue(action, "Show")

        writeActionFunc(text, action, "setArcVisible", ["INACTIVE_AREA", val])

    elif action.actionID == "ShowCircularButton":
        val = getActionArgumentValue(action, "Show")

        writeActionFunc(text, action, "setArcVisible", ["CIRCLE_BUTTON", val])

    elif action.actionID == "SetCircularButtonRadius":
        val = getActionArgumentValue(action, "Radius")

        writeActionFunc(text, action, "setArcRadius", ["CIRCLE_BUTTON", val])

    elif action.actionID == "SetCircularButtonThickness":
        val = getActionArgumentValue(action, "Thickness")

        writeActionFunc(text, action, "setArcThickness", ["CIRCLE_BUTTON", val])

    else:
        generateWidgetAction(text, variables, owner, event, action)