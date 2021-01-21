widget = WidgetDefault()
widget.border = "None"
commonDefaults["ImageSequenceWidget"] = widget


def generateImageSequenceWidget(file, screen, seq, parentName):
    name = seq.getName()

    file.write("    %s = leImageSequenceWidget_New();" % (name))

    generateBaseWidget(file, screen, seq)

    entries = seq.getEntries()

    if len(entries) > 0:
        writeSetInt(file, name, "ImageCount", len(entries), 0)

        for idx, entry in enumerate(entries):
            if entry.img != "":
                imgName = craftAssetName(entry.img)

                file.write("    %s->fn->setImage(%s, %d, %s);" % (name, name, idx, imgName))

            if entry.delay != 1000:
                file.write("    %s->fn->setImageDelay(%s, %d, %s);" % (name, name, idx, entry.delay))

            if entry.halign.toString() != "Center":
                file.write("    %s->fn->setImageHAlignment(%s, %d, %s);" % (name, name, idx, getHAlign(entry.halign.toString())))

            if entry.valign.toString() != "Middle":
                file.write("    %s->fn->setImageVAlignment(%s, %d, %s);" % (name, name, idx, getVAlign(entry.valign.toString())))

    if seq.getStartingImage() != 0:
        file.write("    %s->fn->showImage(%s, %d);" % (name, name, seq.getStartingImage()))

    if seq.getPlayingByDefault() == True:
        file.write("    %s->fn->play(%s);" % (name, name))

    writeSetBoolean(file, name, "Repeat", seq.getRepeat(), False)

    writeEvent(file, name, seq, "ImageChangedEvent", "ImageChangedEventCallback", "OnImageChanged")

    file.write("    %s->fn->addChild(%s, (leWidget*)%s);" % (parentName, parentName, name))
    file.writeNewLine()


def generateImageSequenceEvent(screen, widget, event, genActions):
    text = ""

    if event.name == "ImageChangedEvent":
        text += "void %s_OnImageChanged(%s)\n" % (widget.getName(), getWidgetVariableName(widget))

    text += generateActions(widget, event, genActions, None, None)

    return text

def generateImageSequenceAction(text, variables, owner, event, action):
    name = action.targetName

    if action.actionID == "SetImageCount":
        val = getActionArgumentValue(action, "ImageCount")

        writeActionFunc(text, action, "setImageCount", [val])

    elif action.actionID == "SetEntryImage":
        idx = getActionArgumentValue(action, "ImageIndex")
        val = getActionArgumentValue(action, "Image")

        writeActionFunc(text, action, "setImage", [idx, val])

    elif action.actionID == "SetEntryDuration":
        idx = getActionArgumentValue(action, "ImageIndex")
        val = getActionArgumentValue(action, "ImageDelay")

        writeActionFunc(text, action, "setImageDelay", [idx, val])

    elif action.actionID == "SetEntryHorizontalAlignment":
        idx = getActionArgumentValue(action, "ImageIndex")
        val = getHAlign(getActionArgumentValue(action, "Value"))

        writeActionFunc(text, action, "setImageHAlignment", [idx, val])

    elif action.actionID == "SetEntryVerticalAlignment":
        idx = getActionArgumentValue(action, "ImageIndex")
        val = getVAlign(getActionArgumentValue(action, "Value"))

        writeActionFunc(text, action, "setImageVAlignment", [idx, val])

    elif action.actionID == "Stop":
        writeActionFunc(text, action, "stop", [])

    elif action.actionID == "Start":
        writeActionFunc(text, action, "play", [])

    elif action.actionID == "Rewind":
        writeActionFunc(text, action, "rewind", [])

    elif action.actionID == "SetRepeat":
        val = getActionArgumentValue(action, "Repeat")

        writeActionFunc(text, action, "setRepeat", [val])

    elif action.actionID == "SetActiveImage":
        val = getActionArgumentValue(action, "ImageIndex")

        writeActionFunc(text, action, "showImage", [val])

    elif action.actionID == "Next":
        writeActionFunc(text, action, "showNextImage", [])

    elif action.actionID == "Previous":
        writeActionFunc(text, action, "showPreviousImage", [])
    else:
        generateWidgetAction(text, variables, owner, event, action)