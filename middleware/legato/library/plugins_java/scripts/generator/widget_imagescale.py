widget = WidgetDefault()
widget.border = "None"
commonDefaults["ImageScaleWidget"] = widget


def generateImageScaleWidget(file, screen, image, parentName):
    name = image.getName()

    file.write("    %s = leImageScaleWidget_New();" % (name))

    generateBaseWidget(file, screen, image)

    writeSetAssetName(file, name, "Image", image.getImageName())
    writeSetInt(file, name, "TransformX", image.getTransX(), 0)
    writeSetInt(file, name, "TransformY", image.getTransY(), 0)
    writeSetInt(file, name, "TransformWidth", image.getTransWidth(), 0)
    writeSetInt(file, name, "TransformHeight", image.getTransHeight(), 0)
    writeSetBoolean(file, name, "StretchEnabled", image.getResize(), False)
    writeSetBoolean(file, name, "PreserveAspectEnabled", image.getPreserveAspect(), False)

    filter = image.getFilter()

    if filter == "Nearest Neighbor":
        filter = "LE_IMAGEFILTER_NEAREST_NEIGHBOR"
    else:
        filter = "LE_IMAGEFILTER_BILINEAR"

    writeSetLiteralString(file, name, "Filter", filter, "LE_IMAGEFILTER_NEAREST_NEIGHBOR")

    file.write("    %s->fn->addChild(%s, (leWidget*)%s);" % (parentName, parentName, name))
    file.writeNewLine()

def generateImageScaleAction(text, variables, owner, event, action):
    name = action.targetName

    if action.actionID == "SetImage":
        val = getActionArgumentValue(action, "Image")

        writeActionFunc(text, action, "setImage", [val])

    else:
        generateWidgetAction(text, variables, owner, event, action)