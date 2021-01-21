widget = WidgetDefault()
commonDefaults["ListWheelWidget"] = widget


def generateListWheelWidget(file, screen, list, parentName):
    name = list.getName()

    file.write("    %s = leListWheelWidget_New();" % (name))

    generateBaseWidget(file, screen, list)

    items = list.getItems()

    if len(items) > 0:
        for idx, item in enumerate(items):
            file.write("    %s->fn->appendItem(%s);" % (name, name))

            text = craftStringAssetName(item.text)

            if text != "NULL":
                file.write("    %s->fn->setItemString(%s, %d, (leString*)%s);" % (name, name, idx, text))

            icon = craftAssetName(item.icon)

            if icon != "NULL":
                file.write("    %s->fn->setItemIcon(%s, %d, %s);" % (name, name, idx, icon))

    writeSetInt(file, name, "VisibleItemCount", list.getVisibleItems(), 5)

    selectedIdx = list.getSelectedIndex()

    if selectedIdx != -1:
        writeSetInt(file, name, "SelectedItem", list.getSelectedIndex(), 0)

    pos = getHorzRelativePosition(list.getIconPosition().toString())
    writeSetLiteralString(file, name, "IconPosition", pos, "LE_RELATIVE_POSITION_LEFTOF")

    writeSetInt(file, name, "IconMargin", list.getIconMargin(), 10)
    writeSetBoolean(file, name, "Shaded", list.getShowIndicators(), True)

    effect = list.getEffects().toString()

    if effect != "None":
        if effect == "Fixed":
            effect = "LE_LISTWHEEL_ZOOM_EFFECT_FIXED_SCALE"
        elif effect == "Vertical":
            effect = "LE_LISTWHEEL_ZOOM_EFFECT_VSCALE"
        else:
            effect = "LE_LISTWHEEL_ZOOM_EFFECT_FULL_SCALE"

        writeSetLiteralString(file, name, "ZoomEffects", effect, "LE_LISTWHEEL_ZOOM_EFFECT_NONE")

    writeSetBoolean(file, name, "AutoHideWheel", list.getAutoHideWheel(), False)
    writeSetBoolean(file, name, "Shaded", list.getShaded(), True)
    writeSetBoolean(file, name, "ShowIndicators", list.getShowIndicators(), True)
    writeSetInt(file, name, "IndicatorArea", list.getIndicatorArea(), 20)

    fillMode = list.getIndicatorFill().toString()

    if fillMode == "None":
        fillMode = "LE_LISTWHEEL_INDICATOR_FILL_NONE"
    elif fillMode == "Solid":
        fillMode = "LE_LISTWHEEL_INDICATOR_FILL_SOLID"
    else:
        fillMode = "LE_LISTWHEEL_INDICATOR_FILL_GRADIENT"

    writeSetLiteralString(file, name, "IndicatorFill", fillMode, "LE_LISTWHEEL_INDICATOR_FILL_SOLID")
    writeSetInt(file, name, "FlickInitSpeed", list.getFlickInitSpeed(), 20)
    writeSetInt(file, name, "MaxMomentum", list.getMaxMomentum(), 100)
    writeSetInt(file, name, "RotationUpdateRate", list.getRotationUpdateRate(), 40)
    writeSetInt(file, name, "MomentumFalloffRate", list.getMomentumFalloff(), 1)

    writeEvent(file, name, list, "SelectedItemChangedEvent", "SelectedItemChangedEventCallback", "OnSelectionChanged")

    file.write("    %s->fn->addChild(%s, (leWidget*)%s);" % (parentName, parentName, name))
    file.writeNewLine()


def generateListWheelEvent(screen, widget, event, genActions):
    text = ""

    if event.name == "SelectedItemChangedEvent":
        text += "void %s_OnSelectionChanged(%s, int32_t idx)\n" % (widget.getName(), getWidgetVariableName(widget))

    text += generateActions(widget, event, genActions, None, None)

    return text


def generateListWheelAction(text, variables, owner, event, action):
    name = action.targetName

    if action.actionID == "SetSelectedIndex":
        val = getActionArgumentValue(action, "Index")

        writeActionFunc(text, action, "setSelectedItem", [val])

    elif action.actionID == "SetIconPosition":
        val = getRelativePosition(getActionArgumentValue(action, "IconPosition"))

        writeActionFunc(text, action, "setIconPosition", [val])

    elif action.actionID == "SetIconMargin":
        val = getActionArgumentValue(action, "IconMargin")

        writeActionFunc(text, action, "setIconMargin", [val])

    elif action.actionID == "AppendItem":
        #str = getActionArgumentValue(action, "String")
        #icon = getActionArgumentValue(action, "Image")

        writeActionFunc(text, action, "appendItem", [])

    elif action.actionID == "InsertItem":
        #var = "%s_listInsertIndex" % name
        #variables[var] = "int32_t"

        #str = getActionArgumentValue(action, "String")
        #icon = getActionArgumentValue(action, "Image")
        idx = getActionArgumentValue(action, "Index")

        writeActionFunc(text, action, "insertItem", [idx])
        #writeActionFunc(text, action, "setItemString", [var, str])
        #writeActionFunc(text, action, "setItemIcon", [var, icon])

    elif action.actionID == "RemoveItem":
        idx = getActionArgumentValue(action, "Index")

        writeActionFunc(text, action, "removeItem", [idx])

    elif action.actionID == "RemoveAllItems":
        writeActionFunc(text, action, "removeAllItems", [])

    elif action.actionID == "SelectPrevious":
        writeActionFunc(text, action, "selectPreviousItem", [])

    elif action.actionID == "SelectNext":
        writeActionFunc(text, action, "selectNextItem", [])

    elif action.actionID == "SetItemString":
        str = getActionArgumentValue(action, "String")
        idx = getActionArgumentValue(action, "Index")

        writeActionFunc(text, action, "setItemString", [idx, str])

    elif action.actionID == "SetItemIcon":
        icon = getActionArgumentValue(action, "Image")
        idx = getActionArgumentValue(action, "Index")

        writeActionFunc(text, action, "setItemIcon", [idx, icon])

    elif action.actionID == "SetIndicatorFill":
        val = getActionArgumentValue(action, "Fill")

        if val == "None":
            val = "LE_LISTWHEEL_INDICATOR_FILL_NONE"
        elif val == "Solid":
            val = "LE_LISTWHEEL_INDICATOR_FILL_SOLID"
        else:
            val = "LE_LISTWHEEL_INDICATOR_FILL_GRADIENT"

        writeActionFunc(text, action, "setIndicatorFill", [val])

    elif action.actionID == "SetZoomEffect":
        text.append("    // not supported yet")
    #    val = getActionArgumentValue(action, "Mode")

    #    writeActionFunc(text, action, "setZoomEffects", [val])

    elif action.actionID == "SetAutoHideWheel":
        val = getActionArgumentValue(action, "AutoHide")

        writeActionFunc(text, action, "setAutoHideWheel", [val])

    else:
        generateWidgetAction(text, variables, owner, event, action)