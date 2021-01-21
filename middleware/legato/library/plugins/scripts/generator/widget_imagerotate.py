widget = WidgetDefault()
widget.border = "None"
commonDefaults["ImageRotateWidget"] = widget


def generateImageRotateWidget(file, screen, image, parentName):
    name = image.getName()

    file.write("    %s = leImageRotateWidget_New();" % (name))

    generateBaseWidget(file, screen, image)

    writeSetAssetName(file, name, "Image", image.getImageName())
    writeSetInt(file, name, "OriginX", image.getOrigin().x, 0)
    writeSetInt(file, name, "OriginY", image.getOrigin().y, 0)
    writeSetInt(file, name, "Angle", image.getAngle(), 0)

    filter = image.getFilter()

    if filter == "Nearest Neighbor":
        filter = "LE_IMAGEFILTER_NEAREST_NEIGHBOR"
    else:
        filter = "LE_IMAGEFILTER_BILINEAR"

    writeSetLiteralString(file, name, "Filter", filter, "LE_IMAGEFILTER_NEAREST_NEIGHBOR")

    file.write("    %s->fn->addChild(%s, (leWidget*)%s);" % (parentName, parentName, name))
    file.writeNewLine()

def generateImageRotateAction(text, variables, owner, event, action):
    name = action.targetName

    if action.actionID == "SetImage":
        val = getActionArgumentValue(action, "Image")

        writeActionFunc(text, action, "setImage", [val])

    else:
        generateWidgetAction(text, variables, owner, event, action)