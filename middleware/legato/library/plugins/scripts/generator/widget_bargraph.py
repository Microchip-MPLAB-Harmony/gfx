widget = WidgetDefault()
widget.width = 101
widget.height = 101
widget.border = "None"
widget.background = "None"
commonDefaults["BarGraphWidget"] = widget


def generateBarGraphWidget(file, screen, bar, parentName):
    name = bar.getName()

    file.write("    %s = leBarGraphWidget_New();" % (name))

    generateBaseWidget(file, screen, bar)

    writeSetBoolean(file, name, "Stacked", bar.getStacked(), False)
    writeSetBoolean(file, name, "FillGraphArea", bar.getFillGraphArea(), True)
    writeSetInt(file, name, "TickLength", bar.getTickLength(), 5)

    if bar.getMinValue() != 0:
        file.write("    %s->fn->setMinValue(%s, BAR_GRAPH_AXIS_0, %d);" % (name, name, bar.getMinValue()))

    if bar.getMaxValue() != 100:
        file.write("    %s->fn->setMaxValue(%s, BAR_GRAPH_AXIS_0, %d);" % (name, name, bar.getMaxValue()))

    if bar.getTickInterval() != 10:
        file.write("    %s->fn->setValueAxisTicksInterval(%s, BAR_GRAPH_AXIS_0, %d);" % (name, name, bar.getTickInterval()))

    if bar.getSubtickInterval() != 5:
        file.write("    %s->fn->setValueAxisSubticksInterval(%s, BAR_GRAPH_AXIS_0, %d);" % (name, name, bar.getSubtickInterval()))

    if bar.getValueAxisTicksVisible() == False:
        file.write("    %s->fn->setValueAxisTicksVisible(%s, BAR_GRAPH_AXIS_0, LE_FALSE);" % (name, name))

    if bar.getValueGridlinesVisible() == False:
        file.write("    %s->fn->setGridLinesVisible(%s, BAR_GRAPH_AXIS_0, LE_FALSE);" % (name, name))

    position = bar.getValueAxisTicksPosition().toString()

    if position != "Center":
        if position == "Inside":
            position = "BAR_GRAPH_TICK_IN"
        elif position == "Outside":
            position = "BAR_GRAPH_TICK_OUT"
        else:
            position = "BAR_GRAPH_TICK_CENTER"

        file.write("    %s->fn->setValueAxisTicksPosition(%s, BAR_GRAPH_AXIS_0, %s);" % (name, name, position))

    if bar.getValueAxisLabelsVisible() == False:
        file.write("    %s->fn->setValueAxisLabelsVisible(%s, BAR_GRAPH_AXIS_0, LE_FALSE);" % (name, name))

    if bar.getValueAxisSubticksVisible() == False:
        file.write("    %s->fn->setValueAxisSubticksVisible(%s, BAR_GRAPH_AXIS_0, LE_FALSE);" % (name, name))

    position = bar.getValueAxisSubticksPosition().toString()

    if position != "Center":
        if position == "Inside":
            position = "BAR_GRAPH_TICK_IN"
        elif position == "Outside":
            position = "BAR_GRAPH_TICK_OUT"
        else:
            position = "BAR_GRAPH_TICK_CENTER"

        file.write("    %s->fn->setValueAxisSubticksPosition(%s, BAR_GRAPH_AXIS_0, %s);" % (name, name, position))

    writeSetFontAssetName(file, name, "TicksLabelFont", bar.getLabelFontName())
    writeSetBoolean(file, name, "CategoryAxisTicksVisible", bar.getCategoryAxisTicksVisible(), True)
    writeSetBoolean(file, name, "CategoryAxisLabelsVisible", bar.getCategoryAxisLabelsVisible(), True)

    position = bar.getCategoryAxisTicksPosition().toString()

    if position != "Center":
        if position == "Inside":
            position = "BAR_GRAPH_TICK_IN"
        elif position == "Outside":
            position = "BAR_GRAPH_TICK_OUT"
        else:
            position = "BAR_GRAPH_TICK_CENTER"

        file.write("    %s->fn->setCategoryAxisTicksPosition(%s, %s);" % (name, name, position))

    categoryList = bar.getCategories()

    for i in range(0, len(categoryList)):
        file.write("    %s->fn->addCategory(%s, NULL);" % (name, name))

        stringName = craftStringAssetName(categoryList[i].string)

        if stringName != "NULL":
            file.write("    %s->fn->setCategoryString(%s, %d, (leString*)%s);" % (name, name, i, stringName))

    seriesList = bar.getDataSeries()

    if len(seriesList) > 0:
        for idx, series in enumerate(seriesList):
            file.write("    %s->fn->addSeries(%s, NULL);" % (name, name))

            if testStringValidity(series.scheme):
                file.write("    %s->fn->setSeriesScheme(%s, %d, &%s);" % (name, name, idx, series.scheme))

            for dataVal in series.data:
                file.write("    %s->fn->addDataToSeries(%s, %d, %d, NULL);" % (name, name, idx, dataVal))

    file.write("    %s->fn->addChild(%s, (leWidget*)%s);" % (parentName, parentName, name))
    file.writeNewLine()


def generateBarGraphAction(text, variables, owner, event, action):
    name = action.targetName

    #print(action.argumentData)

    if action.actionID == "SetTickLength":
        val = getActionArgumentValue(action, "Length")

        writeActionFunc(text, action, "setTickLength", [val])

    elif action.actionID == "SetFillGraphArea":
        val = getActionArgumentValue(action, "Fill")

        writeActionFunc(text, action, "setFillGraphArea", [val])

    elif action.actionID == "SetStacked":
        val = getActionArgumentValue(action, "Stacked")

        writeActionFunc(text, action, "setStacked", [val])

    elif action.actionID == "SetMax":
        val = getActionArgumentValue(action, "Value")

        writeActionFunc(text, action, "setMaxValue", ["BAR_GRAPH_AXIS_0", val])

    elif action.actionID == "SetMin":
        val = getActionArgumentValue(action, "Value")

        writeActionFunc(text, action, "setMinValue", ["BAR_GRAPH_AXIS_0", val])

    elif action.actionID == "SetTickInterval":
        val = getActionArgumentValue(action, "Interval")

        writeActionFunc(text, action, "setValueAxisTicksInterval", ["BAR_GRAPH_AXIS_0", val])

    elif action.actionID == "SetSubtickInterval":
        val = getActionArgumentValue(action, "Interval")

        writeActionFunc(text, action, "setValueAxisSubticksInterval", ["BAR_GRAPH_AXIS_0", val])

    elif action.actionID == "ShowValueAxisLabels":
        val = getActionArgumentValue(action, "Show")

        writeActionFunc(text, action, "setValueAxisLabelsVisible", ["BAR_GRAPH_AXIS_0", val])

    elif action.actionID == "ShowValueAxisTicks":
        val = getActionArgumentValue(action, "Show")

        writeActionFunc(text, action, "setValueAxisTicksVisible", ["BAR_GRAPH_AXIS_0", val])

    elif action.actionID == "ShowValueAxisSubticks":
        val = getActionArgumentValue(action, "Show")

        writeActionFunc(text, action, "setValueAxisSubticksVisible", ["BAR_GRAPH_AXIS_0", val])

    elif action.actionID == "ShowValueAxisGrid":
        val = getActionArgumentValue(action, "Show")

        writeActionFunc(text, action, "setGridLinesVisible", ["BAR_GRAPH_AXIS_0", val])

    elif action.actionID == "SetValueAxisTickPosition":
        val = getActionArgumentValue(action, "Position")

        if val == "Center":
            val = "BAR_GRAPH_TICK_CENTER"
        elif val == "Inside":
            val = "BAR_GRAPH_TICK_IN"
        elif val == "Outside":
            val = "BAR_GRAPH_TICK_OUT"

        writeActionFunc(text, action, "setValueAxisTicksPosition", ["BAR_GRAPH_AXIS_0", val])

    elif action.actionID == "SetValueAxisSubtickPosition":
        val = getActionArgumentValue(action, "Position")

        if val == "Center":
            val = "BAR_GRAPH_TICK_CENTER"
        elif val == "Inside":
            val = "BAR_GRAPH_TICK_IN"
        elif val == "Outside":
            val = "BAR_GRAPH_TICK_OUT"

        writeActionFunc(text, action, "setValueAxisSubticksPosition", ["BAR_GRAPH_AXIS_0", val])

    elif action.actionID == "ShowCategoryAxisLabels":
        val = getActionArgumentValue(action, "Show")

        writeActionFunc(text, action, "setCategoryAxisLabelsVisible", [val])

    elif action.actionID == "ShowCategoryAxisTicks":
        val = getActionArgumentValue(action, "Show")

        writeActionFunc(text, action, "setCategoryAxisTicksVisible", [val])

    elif action.actionID == "SetCategoryAxisTickPosition":
        val = getActionArgumentValue(action, "Position")

        if val == "Center":
            val = "BAR_GRAPH_TICK_CENTER"
        elif val == "Inside":
            val = "BAR_GRAPH_TICK_IN"
        elif val == "Outside":
            val = "BAR_GRAPH_TICK_OUT"

        writeActionFunc(text, action, "setCategoryAxisTicksPosition", [val])

    elif action.actionID == "SetLabelFont":
        val = getActionArgumentValue(action, "FontAsset")

        writeActionFunc(text, action, "setTicksLabelFont", [val])

    elif action.actionID == "AddCategory":
        val = getActionArgumentValue(action, "StringAsset")

        variables["categoryIdx"] = "uint32_t"

        writeActionFunc(text, action, "addCategory", ["&categoryIdx"])
        writeActionFunc(text, action, "setCategoryString", ["categoryIdx", val])

    elif action.actionID == "AddSeries":
        val = getActionArgumentValue(action, "Scheme")

        variables["seriesIdx"] = "uint32_t"

        writeActionFunc(text, action, "addSeries", ["&seriesIdx"])
        writeActionFunc(text, action, "setSeriesScheme", ["seriesIdx", val])

    elif action.actionID == "AddData":
        seriesIdx = getActionArgumentValue(action, "SeriesIndex")
        val = getActionArgumentValue(action, "Value")

        writeActionFunc(text, action, "addDataToSeries", [seriesIdx, val, "NULL"])

    elif action.actionID == "SetData":
        catIdx = getActionArgumentValue(action, "CategoryIndex")
        seriesIdx = getActionArgumentValue(action, "SeriesIndex")
        val = getActionArgumentValue(action, "Value")

        variables["categoryIdx"] = "uint32_t"
        variables["seriesIdx"] = "uint32_t"

        writeActionFunc(text, action, "setDataInSeries", [seriesIdx, catIdx, val])

    elif action.actionID == "DeleteData":
        writeActionFunc(text, action, "clearData", [])

    else:
        generateWidgetAction(text, variables, owner, event, action)