widget = WidgetDefault()
widget.width = 101
widget.height = 101
widget.border = "None"
widget.background = "None"
commonDefaults["PieChartWidget"] = widget


def generatePieChartWidget(file, screen, pie, parentName):
    name = pie.getName()

    file.write("    %s = lePieChartWidget_New();" % (name))

    generateBaseWidget(file, screen, pie)

    writeSetInt(file, name, "StartAngle", pie.getStartAngle(), 0)
    writeSetInt(file, name, "CenterAngle", pie.getCenterAngle(), 180)
    writeSetBoolean(file, name, "LabelsVisible", pie.getLabelsVisible(), True)
    writeSetInt(file, name, "LabelsOffset", pie.getLabelsOffset(), 40)
    writeSetFontAssetName(file, name, "LabelFont", craftAssetName(pie.getLabelFontName()))

    data = pie.getData()

    if len(data) > 0:
        for idx, dataItem in enumerate(data):
            file.write("    %s->fn->addEntry(%s);" % (name, name))

            if dataItem.value != 0:
                file.write("    %s->fn->setEntryValue(%s, %d, %d);" % (name, name, idx, dataItem.value))

            if dataItem.radius != 0:
                file.write("    %s->fn->setEntryRadius(%s, %d, %d);" % (name, name, idx, dataItem.radius))

            if dataItem.offset != 0:
                file.write("    %s->fn->setEntryOffset(%s, %d, %d);" % (name, name, idx, dataItem.offset))

            scheme = craftAssetName(dataItem.scheme)

            if scheme != "NULL":
                file.write("    %s->fn->setEntryScheme(%s, %d, %s);" % (name, name, idx, scheme))

    writeEvent(file, name, pie, "PressedEvent", "PressedEventCallback", "OnPressed")

    file.write("    %s->fn->addChild(%s, (leWidget*)%s);" % (parentName, parentName, name))
    file.writeNewLine()


def generatePieChartEvent(screen, widget, event, genActions):
    text = ""

    if event.name == "PressedEvent":
        text += "void %s_OnPressed(%s)\n" % (widget.getName(), getWidgetVariableName(widget))

    text += generateActions(widget, event, genActions, None, None)

    return text


def generatePieChartAction(text, variables, owner, event, action):
    name = action.targetName

    if action.actionID == "SetStartAngle":
        val = getActionArgumentValue(action, "StartAngle")

        writeActionFunc(text, action, "setStartAngle", [val])

    elif action.actionID == "SetCenterAngle":
        val = getActionArgumentValue(action, "CenterAngle")

        writeActionFunc(text, action, "setCenterAngle", [val])

    elif action.actionID == "ShowLabels":
        val = getActionArgumentValue(action, "Show")

        writeActionFunc(text, action, "setLabelsVisible", [val])

    elif action.actionID == "SetLabelOffset":
        val = getActionArgumentValue(action, "Value")

        writeActionFunc(text, action, "setLabelsOffset", [val])

    elif action.actionID == "SetLabelFont":
        val = getActionArgumentValue(action, "FontAsset")

        writeActionFunc(text, action, "setLabelFont", [val])

    elif action.actionID == "SetEntryValue":
        idx = getActionArgumentValue(action, "EntryIndex")
        val = getActionArgumentValue(action, "EntryValue")

        writeActionFunc(text, action, "setEntryValue", [idx, val])

    elif action.actionID == "SetEntryRadius":
        idx = getActionArgumentValue(action, "EntryIndex")
        val = getActionArgumentValue(action, "EntryRadius")

        writeActionFunc(text, action, "setEntryRadius", [idx, val])

    elif action.actionID == "SetEntryOffset":
        idx = getActionArgumentValue(action, "EntryIndex")
        val = getActionArgumentValue(action, "EntryOffset")

        writeActionFunc(text, action, "setEntryOffset", [idx, val])

    elif action.actionID == "SetEntryScheme":
        idx = getActionArgumentValue(action, "EntryIndex")
        val = getActionArgumentValue(action, "EntryScheme")

        writeActionFunc(text, action, "setEntryScheme", [idx, val])

    else:
        generateWidgetAction(text, variables, owner, event, action)