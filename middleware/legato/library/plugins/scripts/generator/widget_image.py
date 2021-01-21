widget = WidgetDefault()
widget.border = "None"
commonDefaults["ImageWidget"] = widget


def generateImageWidget(file, screen, image, parentName):
    name = image.getName()

    file.write("    %s = leImageWidget_New();" % (name))

    generateBaseWidget(file, screen, image)

    writeSetAssetName(file, name, "Image", image.getImageName())

    file.write("    %s->fn->addChild(%s, (leWidget*)%s);" % (parentName, parentName, name))
    file.writeNewLine()

def generateImageAction(text, variables, owner, event, action):
    name = action.targetName

    if action.actionID == "SetImage":
        val = getActionArgumentValue(action, "Image")

        writeActionFunc(text, action, "setImage", [val])

    else:
        generateWidgetAction(text, variables, owner, event, action)