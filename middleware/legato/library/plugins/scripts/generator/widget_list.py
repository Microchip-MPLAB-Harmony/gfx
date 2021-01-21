widget = WidgetDefault()
widget.halign = "Center"
commonDefaults["ListWidget"] = widget


def generateListWidget(file, screen, list, parentName):
    name = list.getName()

    file.write("    %s = leListWidget_New();" % (name))

    generateBaseWidget(file, screen, list)

    mode = list.getSelectionMode().toString()

    if mode != "Single":
        mode = "LE_LIST_WIDGET_SELECTION_MODE_" + mode.upper()

        writeSetLiteralString(file, name, "SelectionMode", mode, "LE_LIST_WIDGET_SELECTION_MODE_SINGLE")

    writeSetBoolean(file, name, "AllowEmptySelection", list.getAllowEmpty(), True)

    pos = getHorzRelativePosition(list.getIconPosition().toString())

    writeSetLiteralString(file, name, "IconPosition", pos, "LE_RELATIVE_POSITION_LEFTOF")
    writeSetInt(file, name, "IconMargin", list.getIconMargin(), 10)

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

    writeEvent(file, name, list, "SelectionChangedEvent", "SelectedItemChangedEventCallback", "OnSelectionChanged")

    file.write("    %s->fn->addChild(%s, (leWidget*)%s);" % (parentName, parentName, name))
    file.writeNewLine()


def generateListEvent(screen, widget, event, genActions):
    text = ""

    if event.name == "SelectionChangedEvent":
        text += "void %s_OnSelectionChanged(%s, uint32_t idx, leBool selected)\n" % (widget.getName(), getWidgetVariableName(widget))

    text += generateActions(widget, event, genActions, None, None)

    return text


def generateListAction(text, variables, owner, event, action):
    name = action.targetName

    if action.actionID == "SetSelectionMode":
        val = getActionArgumentValue(action, "Mode")

        if val == "Single":
            val = "LE_LIST_WIDGET_SELECTION_MODE_SINGLE"
        elif val == "Multiple":
            val = "LE_LIST_WIDGET_SELECTION_MODE_MULTIPLE"
        else:
            val = "LE_LIST_WIDGET_SELECTION_MODE_CONTIGUOUS"

        writeActionFunc(text, action, "setSelectionMode", [val])

    elif action.actionID == "SetAllowEmptySelection":
        val = getActionArgumentValue(action, "AllowEmpty")

        writeActionFunc(text, action, "setAllowEmptySelection", [val])

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

    elif action.actionID == "SetItemSelected":
        idx = getActionArgumentValue(action, "Index")
        sel = getActionArgumentValue(action, "Selected")

        writeActionFunc(text, action, "setItemSelected", [idx, sel])

    elif action.actionID == "ToggleItemSelect":
        idx = getActionArgumentValue(action, "Index")

        writeActionFunc(text, action, "toggleItemSelected", [idx])

    elif action.actionID == "SelectAll":
        writeActionFunc(text, action, "selectAll", [])

    elif action.actionID == "DeselectAll":
        writeActionFunc(text, action, "deselectAll", [])

    elif action.actionID == "SetItemString":
        str = getActionArgumentValue(action, "String")
        idx = getActionArgumentValue(action, "Index")

        writeActionFunc(text, action, "setItemString", [idx, str])

    elif action.actionID == "SetItemIcon":
        icon = getActionArgumentValue(action, "Image")
        idx = getActionArgumentValue(action, "Index")

        writeActionFunc(text, action, "setItemIcon", [idx, icon])

    else:
        generateWidgetAction(text, variables, owner, event, action)