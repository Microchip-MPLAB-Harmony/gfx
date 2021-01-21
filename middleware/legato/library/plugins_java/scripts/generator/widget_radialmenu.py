widget = WidgetDefault()
widget.border = "None"
widget.background = "None"
commonDefaults["RadialMenuWidget"] = widget


def generateRadialMenuWidget(file, screen, menu, parentName):
    name = menu.getName()

    file.write("    %s = leRadialMenuWidget_New();" % (name))

    generateBaseWidget(file, screen, menu)

    writeSetInt(file, name, "NumberOfItemsShown", menu.getItemsShown(), 5)
    writeSetBoolean(file, name, "HighlightProminent", menu.getHighlightProminent(), False)
    writeSetInt(file, name, "Theta", menu.getTheta(), 0)

    width = menu.getSize().width
    height = menu.getSize().height

    touchX = menu.getTouchX()
    touchY = menu.getTouchY()
    touchWidth = menu.getTouchWidth()
    touchHeight = menu.getTouchHeight()

    file.write("    %s->fn->setTouchArea(%s, %d, %d, %d, %d);" % (name, name, touchX, touchY, width * touchWidth / 100, height * touchHeight / 100))

    ellipseType = menu.getEllipseType().toString()

    if ellipseType == "Default":
        ellipseType = "LE_RADIAL_MENU_ELLIPSE_TYPE_DEFAULT"
    elif ellipseType == "Orbital":
        ellipseType = "LE_RADIAL_MENU_ELLIPSE_TYPE_OBITAL"
    else:
        ellipseType = "LE_RADIAL_MENU_ELLIPSE_TYPE_ROLLODEX"

    writeSetLiteralString(file, name, "EllipseType", ellipseType, "LE_RADIAL_MENU_ELLIPSE_TYPE_DEFAULT")
    writeSetBoolean(file, name, "DrawEllipse", menu.getEllipseVisible(), True)

    scaleMode = menu.getSizeScale().toString()

    if scaleMode == "Off":
        scaleMode = "LE_RADIAL_MENU_INTERPOLATE_OFF"
    elif scaleMode == "Gradual":
        scaleMode = "LE_RADIAL_MENU_INTERPOLATE_GRADUAL"
    else:
        scaleMode = "LE_RADIAL_MENU_INTERPOLATE_PROMINENT"

    writeSetLiteralString(file, name, "ScaleMode", scaleMode, "LE_RADIAL_MENU_INTERPOLATE_GRADUAL")

    blendMode = menu.getAlphaScale().toString()

    if blendMode == "Off":
        blendMode = "LE_RADIAL_MENU_INTERPOLATE_OFF"
    elif blendMode == "Gradual":
        blendMode = "LE_RADIAL_MENU_INTERPOLATE_GRADUAL"
    else:
        blendMode = "LE_RADIAL_MENU_INTERPOLATE_PROMINENT"

    writeSetLiteralString(file, name, "BlendMode", blendMode, "LE_RADIAL_MENU_INTERPOLATE_GRADUAL")

    min = menu.getMinSizePercent()
    max = menu.getMaxSizePercent()

    if min != 30 or max != 100:
        file.write("    %s->fn->setScaleRange(%s, %d, %d);" % (name, name, min, max))

    min = menu.getMinAlphaAmount()
    max = menu.getMaxAlphaAmount()

    if min != 128 or max != 255:
        file.write("    %s->fn->setBlendRange(%s, %d, %d);" % (name, name, min, max))

    touchX = menu.getTouchX()
    touchY = menu.getTouchY()
    touchWidth = menu.getTouchWidth()
    touchHeight = menu.getTouchHeight()

    if touchX != 0 or touchY != 75 or touchWidth != 100 or touchHeight != 50:
        file.write("    %s->fn->setTouchArea(%s, %d, %d, %d, %d);" % (name, name, touchX, touchY, touchWidth, touchHeight))

    x = menu.getLocation(False).x
    y = menu.getLocation(False).y
    width = menu.getSize().width
    height = menu.getSize().height

    xp = x + width / 2;
    yp = y + height / 2;

    items = menu.getItemList()

    if len(items) > 0:
        for idx, item in enumerate(items):
            varName = "%s_image_%d" % (name, idx)
            file.write("    %s = leImageScaleWidget_New();" % (varName))

            imageName = craftAssetName(item.image)

            if imageName != "NULL":
                file.write("    %s->fn->setImage(%s, %s);" % (varName, varName, imageName))
                file.write("    %s->fn->setTransformWidth(%s, %d);" % (varName, varName, item.currentSize.width))
                file.write("    %s->fn->setTransformHeight(%s, %s);" % (varName, varName, item.currentSize.height))
                file.write("    %s->fn->setStretchEnabled(%s, LE_TRUE);" % (varName, varName))
                file.write("    %s->fn->setPreserveAspectEnabled(%s, LE_TRUE);" % (varName, varName))
            else:
                file.write("    %s->fn->setBackgroundType(%s, LE_WIDGET_BACKGROUND_FILL);" % (varName, varName))
                file.write("    %s->fn->setBorderType(%s, LE_WIDGET_BORDER_LINE);" % (varName, varName))

            if not (item.t == 270 and menu.getItemsShown() < len(items)):
                file.write("    %s->fn->setVisible(%s, LE_FALSE);" % (varName, varName))

            file.write("    %s->fn->setPosition(%s, %d, %d);" % (varName, varName, xp, yp))
            file.write("    %s->fn->setSize(%s, %d, %d);" % (varName, varName, item.originalSize.width, item.originalSize.height))

            if item.originalAlphaAmount != 255:
                file.write("    %s->fn->setAlphaAmount(%s, %d);" % (varName, varName, item.originalAlphaAmount));

            file.write("    %s->fn->addWidget(%s, (leWidget*)%s);" % (name, name, varName))

    writeEvent(file, name, menu, "ItemSelectedEvent", "ItemSelectedEventCallback", "OnItemSelected")
    writeEvent(file, name, menu, "ItemProminenceChangedEvent", "ItemProminenceChangedEvent", "OnItemProminenceChanged")

    file.write("    %s->fn->addChild(%s, (leWidget*)%s);" % (parentName, parentName, name))
    file.writeNewLine()


def generateRadialMenuEvent(screen, widget, event, genActions):
    text = ""

    if event.name == "ItemSelectedEvent":
        text += "void %s_OnItemSelected(%s)\n" % (widget.getName(), getWidgetVariableName(widget))

    if event.name == "ItemProminenceChangedEvent":
        text += "void %s_OnItemProminenceChanged(%s)\n" % (widget.getName(), getWidgetVariableName(widget))

    text += generateActions(widget, event, genActions, None, None)

    return text


def generateRadialMenuAction(text, variables, owner, event, action):
    i = 0