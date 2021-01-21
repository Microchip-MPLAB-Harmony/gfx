widget = WidgetDefault()
widget.width = 200
widget.height = 200
widget.background = "None"
commonDefaults["KeyPadWidget"] = widget


def generateKeyPadCell(file, pad, cell, row, col):
    name = pad.getName()

    enabled = cell.enabled
    string = craftStringAssetName(cell.string)
    action = "LE_KEYPAD_CELL_ACTION_" + cell.action.toString().upper()
    value = craftStringAssetName(cell.value)
    pressIcon = craftAssetName(cell.pressIcon)
    releaseIcon = craftAssetName(cell.releaseIcon)
    pos = getRelativePosition(cell.pos.toString())
    margin = cell.imageMargin
    showBackground = cell.background

    shouldGen = False

    if enabled != True \
        and string != "NULL" \
        and value != "NULL" \
        and action != "LE_KEYPAD_CELL_ACTION_NONE" \
        and pressIcon != "NULL" \
        and releaseIcon != "NULL" \
        and pos != "LE_RELATIVE_POSITION_BEHIND" \
        and margin != 10 \
        and showBackground != True:
        return

    if enabled != True \
        or string != "NULL" \
        or pressIcon != "NULL" \
        or releaseIcon != "NULL" \
        or pos != "LE_RELATIVE_POSITION_BEHIND" \
        or margin != 10 \
        or showBackground != True:
        shouldGen = True

    varName = "%s_cell_%d_%d" % (name, row, col)

    if action != "LE_KEYPAD_CELL_ACTION_NONE":
        file.write("    %s->fn->setKeyAction(%s, %d, %d, %s);" % (name, name, row, col, action))

    if value != "NULL":
        file.write("    %s->fn->setKeyValue(%s, %d, %d, (leString*)%s);" % (name, name, row, col, value))

    if shouldGen == True:
        file.write("    leButtonWidget* %s = %s->fn->getCellButton(%s, %d, %d);" % (varName, name, name, row, col))

        writeSetBoolean(file, varName, "Visible", enabled, True)

        if string != "NULL":
            file.write("    %s->fn->setString(%s, (leString*)%s);" % (varName, varName, string))

        writeSetLiteralString(file, varName, "PressedImage", pressIcon, "NULL")
        writeSetLiteralString(file, varName, "ReleasedImage", releaseIcon, "NULL")
        writeSetLiteralString(file, varName, "ImagePosition", pos, "LE_RELATIVE_POSITION_BEHIND")
        writeSetInt(file, varName, "ImageMargin", margin, 10)

        if showBackground == False:
            file.write("    %s->fn->setBackgroundType(%s, LE_WIDGET_BACKGROUND_NONE);" % (varName, varName))


def generateKeyPadWidget(file, screen, pad, parentName):
    name = pad.getName()

    file.write("    %s = leKeyPadWidget_New(%d, %d);" % (name, pad.getRowCount(), pad.getColCount()))

    generateBaseWidget(file, screen, pad)

    trigMode = pad.getTriggerMode().toString()

    if trigMode == "KeyRelease":
        trigMode = "LE_KEYPAD_TRIGGER_KEYRELEASED"
    else:
        trigMode = "LE_KEYPAD_TRIGGER_KEYPRESSED"

    if trigMode != "LE_KEYPAD_TRIGGER_KEYRELEASED":
        writeSetLiteralString(file, name, "KeyPadActionTrigger", trigMode, "LE_KEYPAD_TRIGGER_KEYRELEASED")

    cells = pad.getCells()

    for row_idx, row in enumerate(cells):
        for col_idx, cell in enumerate(row):
            generateKeyPadCell(file, pad, cell, row_idx, col_idx)

    writeEvent(file, name, pad, "KeyClickEvent", "KeyClickEventCallback", "OnKeyClick")

    file.write("    %s->fn->addChild(%s, (leWidget*)%s);" % (parentName, parentName, name))
    file.writeNewLine()


def generateKeyPadEvent(screen, widget, event, genActions):
    text = ""

    if event.name == "KeyClickEvent":
        text += "void %s_OnKeyClick(%s, leButtonWidget* btn, uint32_t row, uint32_t col)\n" % (widget.getName(), getWidgetVariableName(widget))

    text += generateActions(widget, event, genActions, None, None)

    return text

def generateKeyPadAction(text, variables, owner, event, action):
    name = action.targetName

    if action.actionID == "SetTrigger":
        val = getActionArgumentValue(action, "Trigger")

        if val == "KeyRelease":
            val = "LE_KEYPAD_TRIGGER_KEYRELEASED"
        else:
            val = "LE_KEYPAD_TRIGGER_KEYPRESSED"

        writeActionFunc(text, action, "setKeyPadActionTrigger", [val])

    elif action.actionID == "SetKeyVisible":
        row = getActionArgumentValue(action, "Row")
        col = getActionArgumentValue(action, "Column")
        val = getActionArgumentValue(action, "Visible")

        writeActionFunc(text, action, "setKeyVisible", [row, col, val])

    elif action.actionID == "SetKeyText":
        row = getActionArgumentValue(action, "Row")
        col = getActionArgumentValue(action, "Column")
        val = getActionArgumentValue(action, "String")

        var = "%s_cell_%s_%s" % (name, row, col)
        variables[var] = "leButtonWidget*"

        text.append("    %s = %s->fn->getCellButton(%s, %s, %s);" % (var, action.targetName, action.targetName, row, col))
        writeActionFuncWithName(text, action, "setString", [val], var)

    elif action.actionID == "SetKeyPressedImage":
        row = getActionArgumentValue(action, "Row")
        col = getActionArgumentValue(action, "Column")
        val = getActionArgumentValue(action, "Image")

        var = "%s_cell_%s_%s" % (name, row, col)
        variables[var] = "leButtonWidget*"

        text.append("    %s = %s->fn->getCellButton(%s, %s, %s);" % (var, action.targetName, action.targetName, row, col))
        writeActionFuncWithName(text, action, "setPressedImage", [val], var)

    elif action.actionID == "SetKeyReleasedIamge":
        row = getActionArgumentValue(action, "Row")
        col = getActionArgumentValue(action, "Column")
        val = getActionArgumentValue(action, "Image")

        var = "%s_cell_%s_%s" % (name, row, col)
        variables[var] = "leButtonWidget*"

        text.append("    %s = %s->fn->getCellButton(%s, %s, %s);" % (var, action.targetName, action.targetName, row, col))
        writeActionFuncWithName(text, action, "setReleasedImage", [val], var)

    elif action.actionID == "SetKeyImagePosition":
        row = getActionArgumentValue(action, "Row")
        col = getActionArgumentValue(action, "Column")
        val = getRelativePosition(getActionArgumentValue(action, "Position"))

        var = "%s_cell_%s_%s" % (name, row, col)
        variables[var] = "leButtonWidget*"

        text.append("    %s = %s->fn->getCellButton(%s, %s, %s);" % (var, action.targetName, action.targetName, row, col))
        writeActionFuncWithName(text, action, "setImagePosition", [val], var)

    elif action.actionID == "SetKeyImageMargin":
        row = getActionArgumentValue(action, "Row")
        col = getActionArgumentValue(action, "Column")
        val = getActionArgumentValue(action, "Margin")

        var = "%s_cell_%s_%s" % (name, row, col)
        variables[var] = "leButtonWidget*"

        text.append("    %s = %s->fn->getCellButton(%s, %s, %s);" % (var, action.targetName, action.targetName, row, col))
        writeActionFuncWithName(text, action, "setImageMargin", [val], var)

    elif action.actionID == "SetKeyBackgroundType":
        row = getActionArgumentValue(action, "Row")
        col = getActionArgumentValue(action, "Column")
        val = getBackgroundType(getActionArgumentValue(action, "Type"))

        var = "%s_cell_%s_%s" % (name, row, col)
        variables[var] = "leButtonWidget*"

        text.append("    %s = %s->fn->getCellButton(%s, %s, %s);" % (var, action.targetName, action.targetName, row, col))
        writeActionFunc(text, action, "setBackgroundType", [val])

    elif action.actionID == "SetKeyAction":
        row = getActionArgumentValue(action, "Row")
        col = getActionArgumentValue(action, "Column")
        val = getActionArgumentValue(action, "Action")

        if val == "None":
            val = "LE_KEYPAD_CELL_ACTION_NONE"
        elif val == "Append":
            val = "LE_KEYPAD_CELL_ACTION_APPEND"
        elif val == "Append":
            val = "LE_KEYPAD_CELL_ACTION_SET"
        elif val == "Set":
            val = "LE_KEYPAD_CELL_ACTION_BACKSPACE"
        elif val == "Clear":
            val = "LE_KEYPAD_CELL_ACTION_CLEAR"
        elif val == "Accept":
            val = "LE_KEYPAD_CELL_ACTION_ACCEPT"

        writeActionFunc(text, action, "setKeyAction", [row, col, val])

    elif action.actionID == "SetKeyValue":
        row = getActionArgumentValue(action, "Row")
        col = getActionArgumentValue(action, "Column")
        val = getActionArgumentValue(action, "String")

        writeActionFunc(text, action, "setKeyValue", [row, col, val])

    else:
        generateWidgetAction(text, variables, owner, event, action)