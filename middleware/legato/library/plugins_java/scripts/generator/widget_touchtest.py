widget = WidgetDefault()
widget.border = "None"
commonDefaults["TouchTestWidget"] = widget


def generateTouchTestWidget(file, screen, tch, parentName):
    name = tch.getName()

    file.write("    %s = leTouchTestWidget_New();" % (name))

    generateBaseWidget(file, screen, tch)

    writeEvent(file, name, tch, "PointAddedEvent", "PointAddedEventCallback", "OnPointAdded")

    file.write("    %s->fn->addChild(%s, (leWidget*)%s);" % (parentName, parentName, name))
    file.writeNewLine()


def generateTouchTestEvent(screen, widget, event, genActions):
    text = ""

    if event.name == "PointAddedEvent":
        text += "void %s_OnPointAdded(%s, lePoint* pnt)\n" % (widget.getName(), getWidgetVariableName(widget))

    text += generateActions(widget, event, genActions, None, None)

    return text


def generateTouchTestAction(text, variables, owner, event, action):
    generateWidgetAction(text, variables, owner, event, action)