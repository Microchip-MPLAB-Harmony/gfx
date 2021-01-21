widget = WidgetDefault()
widget.width = 101
widget.height = 101
widget.border = "None"
widget.background = "None"
commonDefaults["LineGraphWidget"] = widget


def generateLineGraphWidget(file, screen, gph, parentName):
    name = gph.getName()

    file.write("    %s = leLineGraphWidget_New();" % (name))

    generateBaseWidget(file, screen, gph)

    writeSetBoolean(file, name, "Stacked", gph.getStacked(), False)
    writeSetBoolean(file, name, "FillGraphArea", gph.getFillGraphArea(), True)
    writeSetBoolean(file, name, "FillSeriesArea", gph.getFillValueArea(), True)
    writeSetInt(file, name, "TickLength", gph.getTickLength(), 5)

    if gph.getMinValue() != 0:
        file.write("    %s->fn->setMinValue(%s, LINE_GRAPH_AXIS_0, %d);" % (name, name, gph.getMinValue()))

    if gph.getMaxValue() != 100:
        file.write("    %s->fn->setMaxValue(%s, LINE_GRAPH_AXIS_0, %d);" % (name, name, gph.getMaxValue()))

    if gph.getTickInterval() != 10:
        file.write("    %s->fn->setValueAxisTickInterval(%s, LINE_GRAPH_AXIS_0, %d);" % (name, name, gph.getTickInterval()))

    if gph.getSubtickInterval() != 5:
        file.write("    %s->fn->setValueAxisSubtickInterval(%s, LINE_GRAPH_AXIS_0, %d);" % (name, name, gph.getSubtickInterval()))

    if gph.getValueAxisTicksVisible() == False:
        file.write("    %s->fn->setValueAxisTicksVisible(%s, LINE_GRAPH_AXIS_0, LE_FALSE);" % (name, name))

    if gph.getValueGridlinesVisible() == False:
        file.write("    %s->fn->setGridLinesVisible(%s, LINE_GRAPH_AXIS_0, LE_FALSE);" % (name, name))

    position = gph.getValueAxisTicksPosition().toString()

    if position != "Center":
        if position == "Inside":
            position = "LINE_GRAPH_TICK_IN"
        elif position == "Outside":
            position = "LINE_GRAPH_TICK_OUT"

        file.write("    %s->fn->setValueAxisTicksPosition(%s, LINE_GRAPH_AXIS_0, %s);" % (name, name, position))

    if gph.getValueAxisLabelsVisible() == False:
        file.write("    %s->fn->setValueAxisLabelsVisible(%s, LINE_GRAPH_AXIS_0, LE_FALSE);" % (name, name))

    if gph.getValueAxisSubticksVisible() == False:
        file.write("    %s->fn->setValueAxisSubticksVisible(%s, LINE_GRAPH_AXIS_0, LE_FALSE);" % (name, name))

    position = gph.getValueAxisSubticksPosition().toString()

    if position != "Center":
        if position == "Inside":
            position = "LINE_GRAPH_TICK_IN"
        elif position == "Outside":
            position = "LINE_GRAPH_TICK_OUT"

        file.write("    %s->fn->setValueAxisSubticksPosition(%s, LINE_GRAPH_AXIS_0, %s);" % (name, name, position))

    writeSetFontAssetName(file, name, "TicksLabelFont", gph.getLabelFontName())
    writeSetBoolean(file, name, "CategoryAxisTicksVisible", gph.getCategoryAxisTicksVisible(), True)
    writeSetBoolean(file, name, "CategoryAxisLabelsVisible", gph.getCategoryAxisLabelsVisible(), True)

    position = gph.getCategoryAxisTicksPosition().toString()

    if position != "Center":
        if position == "Inside":
            position = "LINE_GRAPH_TICK_IN"
        elif position == "Outside":
            position = "LINE_GRAPH_TICK_OUT"

        file.write("    %s->fn->setCategoryAxisTicksPosition(%s, %s);" % (name, name, position))

    categoryList = gph.getCategories()

    for i in range(0, len(categoryList)):
        file.write("    %s->fn->addCategory(%s);" % (name, name))

        stringName = craftStringAssetName(categoryList[i].string)

        if stringName != "NULL":
            file.write("    %s->fn->setCategoryString(%s, %d, (leString*)%s);" % (name, name, i, stringName))

    seriesList = gph.getDataSeries()

    if len(seriesList) > 0:
        for idx, series in enumerate(seriesList):
            file.write("    %s->fn->addSeries(%s);" % (name, name))

            if testStringValidity(series.scheme):
                file.write("    %s->fn->setSeriesScheme(%s, %d, &%s);" % (name, name, idx, series.scheme))

            for dataVal in series.data:
                file.write("    %s->fn->addDataToSeries(%s, %d, %s);" % (name, name, idx, dataVal))

            pointType = series.pointType

            if pointType == "None":
                pointType = "LINE_GRAPH_DATA_POINT_NONE"
            elif pointType == "Circle":
                pointType = "LINE_GRAPH_DATA_POINT_CIRCLE"
            elif pointType == "Square":
                pointType = "LINE_GRAPH_DATA_POINT_SQUARE"

            if series.drawLines == False:
                file.write("    %s->fn->setSeriesLinesVisible(%s, %d, LE_FALSE);" % (name, name, idx))

            if series.fillPoints == False:
                file.write("    %s->fn->setSeriesFillPoints(%s, %d, LE_FALSE);" % (name, name, idx))

            if series.pointType != "LINE_GRAPH_DATA_POINT_NONE":
                file.write("    %s->fn->setSeriesPointType(%s, %d, %s);" % (name, name, idx, pointType))

            if series.pointSize != 10:
                file.write("    %s->fn->setSeriesPointSize(%s, %d, %d);" % (name, name, idx, series.pointSize))

    file.write("    %s->fn->addChild(%s, (leWidget*)%s);" % (parentName, parentName, name))
    file.writeNewLine()


def generateLineGraphAction(text, variables, owner, event, action):
    name = action.targetName

    if action.actionID == "SetTickLengthValue":
        val = getActionArgumentValue(action, "Length")

        writeActionFunc(text, action, "setTickLength", [val])

    elif action.actionID == "SetFillGraphArea":
        val = getActionArgumentValue(action, "Fill")

        writeActionFunc(text, action, "setFillGraphArea", [val])

    elif action.actionID == "SetFillSeriesArea":
        val = getActionArgumentValue(action, "Fill")

        writeActionFunc(text, action, "setFillSeriesArea", [val])

    elif action.actionID == "SetStacked":
        val = getActionArgumentValue(action, "Stacked")

        writeActionFunc(text, action, "setStacked", [val])

    elif action.actionID == "SetMax":
        val = getActionArgumentValue(action, "Value")

        writeActionFunc(text, action, "setMaxValue", ["LINE_GRAPH_AXIS_0", val])

    elif action.actionID == "SetMin":
        val = getActionArgumentValue(action, "Value")

        writeActionFunc(text, action, "setMinValue", ["LINE_GRAPH_AXIS_0", val])

    elif action.actionID == "SetTickInterval":
        val = getActionArgumentValue(action, "Interval")

        writeActionFunc(text, action, "setValueAxisTickInterval", ["LINE_GRAPH_AXIS_0", val])

    elif action.actionID == "SetSubtickInterval":
        val = getActionArgumentValue(action, "Interval")

        writeActionFunc(text, action, "setValueAxisSubtickInterval", ["LINE_GRAPH_AXIS_0", val])

    elif action.actionID == "ShowValueAxisLabels":
        val = getActionArgumentValue(action, "Show")

        writeActionFunc(text, action, "setValueAxisLabelsVisible", ["LINE_GRAPH_AXIS_0", val])

    elif action.actionID == "ShowValueAxisTicks":
        val = getActionArgumentValue(action, "Show")

        writeActionFunc(text, action, "setValueAxisTicksVisible", ["LINE_GRAPH_AXIS_0", val])

    elif action.actionID == "ShowValueAxisSubticks":
        val = getActionArgumentValue(action, "Show")

        writeActionFunc(text, action, "setValueAxisSubticksVisible", ["LINE_GRAPH_AXIS_0", val])

    elif action.actionID == "ShowValueAxisGridlines":
        val = getActionArgumentValue(action, "Show")

        writeActionFunc(text, action, "setGridLinesVisible", ["LINE_GRAPH_AXIS_0", val])

    elif action.actionID == "SetValueAxisTicksPosition":
        val = getActionArgumentValue(action, "Position")

        if val == "Center":
            val = "LINE_GRAPH_TICK_CENTER"
        elif val == "Inside":
            val = "LINE_GRAPH_TICK_IN"
        elif val == "Outside":
            val = "LINE_GRAPH_TICK_OUT"

        writeActionFunc(text, action, "setValueAxisTicksPosition", ["LINE_GRAPH_AXIS_0", val])

    elif action.actionID == "SetValueAxisSubticksPosition":
        val = getActionArgumentValue(action, "Position")

        if val == "Center":
            val = "LINE_GRAPH_TICK_CENTER"
        elif val == "Inside":
            val = "LINE_GRAPH_TICK_IN"
        elif val == "Outside":
            val = "LINE_GRAPH_TICK_OUT"

        writeActionFunc(text, action, "setValueAxisSubticksPosition", ["LINE_GRAPH_AXIS_0", val])

    elif action.actionID == "ShowCategoryAxisLabels":
        val = getActionArgumentValue(action, "Show")

        writeActionFunc(text, action, "setCategoryAxisLabelsVisible", [val])

    elif action.actionID == "ShowCategoryAxisTicks":
        val = getActionArgumentValue(action, "Show")

        writeActionFunc(text, action, "setCategoryAxisTicksVisible", [val])

    elif action.actionID == "SetCategoryAxisTicksPosition":
        val = getActionArgumentValue(action, "Position")

        if val == "Center":
            val = "LINE_GRAPH_TICK_CENTER"
        elif val == "Inside":
            val = "LINE_GRAPH_TICK_IN"
        elif val == "Outside":
            val = "LINE_GRAPH_TICK_OUT"

        writeActionFunc(text, action, "setCategoryAxisTicksVisible", [val])

    elif action.actionID == "SetLabelFont":
        val = getActionArgumentValue(action, "FontAsset")

        writeActionFunc(text, action, "setTicksLabelFont", [val])

    elif action.actionID == "AddCategory":
        writeActionFunc(text, action, "addCategory", [])

    elif action.actionID == "AddSeries":
        writeActionFunc(text, action, "addSeries", [])

    elif action.actionID == "AddData":
        idx = getActionArgumentValue(action, "SeriesIndex")
        val = getActionArgumentValue(action, "Value")

        writeActionFunc(text, action, "addDataToSeries", [idx, val])

    elif action.actionID == "SetData":
        catIdx = getActionArgumentValue(action, "CategoryIndex")
        seriesIdx = getActionArgumentValue(action, "SeriesIndex")
        val = getActionArgumentValue(action, "Value")

        writeActionFunc(text, action, "setDataInSeries", [seriesIdx, catIdx, val])

    elif action.actionID == "SetSeriesPointType":
        seriesIdx = getActionArgumentValue(action, "SeriesIndex")
        val = getActionArgumentValue(action, "Type")

        if val == "Circle":
            val = "LINE_GRAPH_DATA_POINT_CIRCLE"
        elif val == "Square":
            val = "LINE_GRAPH_DATA_POINT_SQUARE"
        else:
            val = "LINE_GRAPH_DATA_POINT_NONE"

        writeActionFunc(text, action, "setSeriesPointType", [seriesIdx, val])

    elif action.actionID == "SetSeriesPointSize":
        seriesIdx = getActionArgumentValue(action, "SeriesIndex")
        val = getActionArgumentValue(action, "Size")

        writeActionFunc(text, action, "setSeriesPointSize", [seriesIdx, val])

    elif action.actionID == "SetSeriesFillPoints":
        seriesIdx = getActionArgumentValue(action, "SeriesIndex")
        val = getActionArgumentValue(action, "Fill")

        writeActionFunc(text, action, "setSeriesFillPoints", [seriesIdx, val])

    elif action.actionID == "SetSeriesLinesVisible":
        seriesIdx = getActionArgumentValue(action, "SeriesIndex")
        val = getActionArgumentValue(action, "Visible")

        writeActionFunc(text, action, "setSeriesLinesVisible", [seriesIdx, val])

    elif action.actionID == "DeleteData":
        writeActionFunc(text, action, "clear", [])

    else:
        generateWidgetAction(text, variables, owner, event, action)