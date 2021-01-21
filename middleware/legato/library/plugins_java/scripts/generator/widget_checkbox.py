widget = WidgetDefault()
widget.height = 25
widget.border = "None"
commonDefaults["CheckBoxWidget"] = widget


def generateCheckBoxWidget(file, screen, cbox, parentName):
    name = cbox.getName()

    file.write("    %s = leCheckBoxWidget_New();" % (name))

    generateBaseWidget(file, screen, cbox)

    writeSetStringAssetName(file, name, "String", cbox.getStringName())
    writeSetBoolean(file, name, "Checked", cbox.getChecked(), False)
    writeSetAssetName(file, name, "CheckedImage", cbox.getCheckedImageName())
    writeSetAssetName(file, name, "UncheckedImage", cbox.getUncheckedImageName())
    writeSetLiteralString(file, name, "ImagePosition", getRelativePosition(cbox.getImagePosition().toString()), "LE_RELATIVE_POSITION_LEFTOF")
    writeSetInt(file, name, "ImageMargin", cbox.getImageMargin(), 10)
    writeEvent(file, name, cbox, "CheckedEvent", "CheckedEventCallback", "OnChecked")
    writeEvent(file, name, cbox, "UncheckedEvent", "UncheckedEventCallback", "OnUnchecked")

    file.write("    %s->fn->addChild(%s, (leWidget*)%s);" % (parentName, parentName, name))
    file.writeNewLine()


def generateCheckBoxEvent(screen, widget, event, genActions):
    text = ""

    if event.name == "CheckedEvent":
        text += "void %s_OnChecked(%s)\n" % (widget.getName(), getWidgetVariableName(widget))

    if event.name == "UncheckedEvent":
        text += "void %s_OnUnchecked(%s)\n" % (widget.getName(), getWidgetVariableName(widget))

    text += generateActions(widget, event, genActions, None, None)

    return text

def generateCheckBoxAction(text, variables, owner, event, action):
    name = action.targetName

    if action.actionID == "SetCheckState":
        writeActionFunc(text, action, "setChecked", [
getActionArgumentValue(action, "Checked")])

    elif action.actionID == "SetString":
        writeActionFunc(text, action, "setString", [
getActionArgumentValue(action, "String")])

    elif action.actionID == "SetCheckedImage":
        writeActionFunc(text, action, "setCheckedImage", [
getActionArgumentValue(action, "CheckedImage")])

    elif action.actionID == "SetUncheckedImage":
        writeActionFunc(text, action, "setUncheckedImage", [
getActionArgumentValue(action, "UncheckedImage")])

    elif action.actionID == "SetImageMargin":
        writeActionFunc(text, action, "setImageMargin", [
getActionArgumentValue(action, "ImageMargin")])

    elif action.actionID == "SetImagePosition":
        position = getRelativePosition(
getActionArgumentValue(action, "ImagePosition"))

        writeActionFunc(text, action, "setImagePosition", [position])

    else:
        generateWidgetAction(text, variables, owner, event, action)