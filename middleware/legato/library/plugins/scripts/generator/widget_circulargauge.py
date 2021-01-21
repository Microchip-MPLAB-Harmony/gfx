widget = WidgetDefault()
widget.width = 101
widget.height = 101
widget.border = "None"
widget.background = "None"
commonDefaults["CircularGaugeWidget"] = widget


def generateCircularGaugeWidget(file, screen, gauge, parentName):
    name = gauge.getName()

    file.write("    %s = leCircularGaugeWidget_New();" % (name))

    generateBaseWidget(file, screen, gauge)

    writeSetInt(file, name, "Radius", gauge.getRadius(), 50)
    writeSetInt(file, name, "StartAngle", gauge.getStartAngle(), 225)
    writeSetInt(file, name, "CenterAngle", gauge.getCenterAngle(), -270)
    writeSetInt(file, name, "StartValue", gauge.getStartValue(), 0)
    writeSetInt(file, name, "EndValue", gauge.getEndValue(), 100)
    writeSetInt(file, name, "Value", gauge.getValue(), 50)
    writeSetFontAssetName(file, name, "TickLabelFont", gauge.getTickLabelFontName())
    writeSetBoolean(file, name, "TickLabelsVisible", gauge.getTickLabelsVisible(), True)
    writeSetInt(file, name, "TickLength", gauge.getTickLength(), 5)
    writeSetInt(file, name, "TickValue", gauge.getTickValue(), 5)
    writeSetBoolean(file, name, "TicksVisible", gauge.getTicksVisible(), True)
    writeSetBoolean(file, name, "HandVisible", gauge.getHandVisible(), True)
    writeSetInt(file, name, "HandRadius", gauge.getHandRadius(), 40)
    writeSetBoolean(file, name, "CenterCircleVisible", gauge.getCenterCircleVisible(), True)
    writeSetInt(file, name, "CenterCircleRadius", gauge.getCenterCircleRadius(), 5)
    writeSetInt(file, name, "CenterCircleThickness", gauge.getCenterCircleThickness(), 2)

    minorTicks = gauge.getMinorTicks()

    if len(minorTicks) > 0:
        for tick in minorTicks:
            scheme = "NULL"

            if tick.scheme is not None and len(tick.scheme) > 0:
                scheme = "&" + tick.scheme

            file.write("    %s->fn->addMinorTicks(%s, %d, %d, %d, %d, %d, %s);" % (name, name, tick.startValue, tick.endValue, tick.radius, tick.length, tick.interval, scheme))

    minorTickLabels = gauge.getMinorTickLabels()

    if len(minorTickLabels) > 0:
        for lbl in minorTickLabels:
            scheme = "NULL"

            if lbl.scheme is not None and len(lbl.scheme) > 0:
                scheme = "&" + tick.scheme

            pos = ""

            if lbl.position.toString() == "Outside":
                pos = "CIRCULAR_GAUGE_LABEL_OUTSIDE"
            else:
                pos = "CIRCULAR_GAUGE_LABEL_INSIDE"

            file.write("    %s->fn->addMinorTickLabels(%s, %d, %d, %d, %s, %d, %s);" % (name, name, lbl.startValue, lbl.endValue, lbl.radius, pos, lbl.interval, scheme))

    advancedArcList = gauge.getAdvancedArcs()

    if len(advancedArcList) > 0:
        for arc in advancedArcList:
            scheme = arc.scheme

            scheme = craftAssetName(scheme)

            if arc.type == "VALUE":
                file.write("    %s->fn->addValueArc(%s, %d, %d, %d, %d, %s);" % (name, name, arc.start, arc.end, arc.radius, arc.thickness, scheme))
            else:
                file.write("    %s->fn->addAngularArc(%s, %d, %d, %d, %d, %s);" % (name, name, arc.start, arc.end, arc.radius, arc.thickness, scheme))

    writeEvent(file, name, gauge, "ValueChangedEvent", "ValueChangedEventCallback", "OnValueChanged")

    file.write("    %s->fn->addChild(%s, (leWidget*)%s);" % (parentName, parentName, name))
    file.writeNewLine()


def generateCircularGaugeEvent(screen, widget, event, genActions):
    text = ""

    if event.name == "ValueChangedEvent":
        text += "void %s_OnValueChanged(%s)\n" % (widget.getName(), getWidgetVariableName(widget))

    text += generateActions(widget, event, genActions, None, None)

    return text


def generateCircularGaugeAction(text, variables, owner, event, action):
    name = action.targetName

    if action.actionID == "SetRadius":
        writeActionFunc(text, action, "setRadius", [getActionArgumentValue(action, "Radius")])

    elif action.actionID == "SetStartAngle":
        writeActionFunc(text, action, "setStartAngle", [getActionArgumentValue(action, "StartAngle")])

    elif action.actionID == "SetCenterAngle":
        writeActionFunc(text, action, "setCenterAngle", [getActionArgumentValue(action, "CenterAngle")])

    elif action.actionID == "SetValue":
        writeActionFunc(text, action, "setValue", [getActionArgumentValue(action, "Value")])

    elif action.actionID == "SetStartValue":
        writeActionFunc(text, action, "setStartValue", [getActionArgumentValue(action, "StartValue")])

    elif action.actionID == "SetEndValue":
        writeActionFunc(text, action, "setEndValue", [getActionArgumentValue(action, "EndValue")])

    elif action.actionID == "ShowTicks":
        writeActionFunc(text, action, "setTicksVisible", [getActionArgumentValue(action, "TicksVisible")])

    elif action.actionID == "SetTickLength":
        writeActionFunc(text, action, "setTickLength", [getActionArgumentValue(action, "TickLength")])

    elif action.actionID == "SetTickValue":
        writeActionFunc(text, action, "setTickValue", [getActionArgumentValue(action, "TickValue")])

    elif action.actionID == "ShowTickLabels":
        writeActionFunc(text, action, "setTickLabelsVisible", [getActionArgumentValue(action, "Show")])

    elif action.actionID == "SetTickLabelFont":
        writeActionFunc(text, action, "setTickLabelFont", [getActionArgumentValue(action, "FontAsset")])

    elif action.actionID == "ShowHand":
        writeActionFunc(text, action, "setHandVisible", [getActionArgumentValue(action, "Show")])

    elif action.actionID == "SetHandRadius":
        writeActionFunc(text, action, "setHandRadius", [getActionArgumentValue(action, "Radius")])

    elif action.actionID == "ShowCenterCircle":
        writeActionFunc(text, action, "setCenterCircleVisible", [getActionArgumentValue(action, "Show")])

    elif action.actionID == "SetCenterCircleRadius":
        writeActionFunc(text, action, "setCenterCircleRadius", [getActionArgumentValue(action, "Radius")])

    elif action.actionID == "SetCenterCircleThickness":
        writeActionFunc(text, action, "setCenterCircleThickness", [getActionArgumentValue(action, "Thickness")])

    else:
        generateWidgetAction(text, variables, owner, event, action)