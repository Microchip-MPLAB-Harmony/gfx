widget = WidgetDefault()
widget.border = "None"
commonDefaults["DrawSurfaceWidget"] = widget


def generateDrawSurfaceWidget(file, screen, surf, parentName):
    name = surf.getName()

    file.write("    %s = leDrawSurfaceWidget_New();" % (name))

    generateBaseWidget(file, screen, surf)

    writeEvent(file, name, surf, "DrawNotificationEvent", "DrawCallback", "OnPaint")
    file.write("    %s->fn->addChild(%s, (leWidget*)%s);" % (parentName, parentName, name))
    file.writeNewLine()


def generateDrawSurfaceEvent(screen, widget, event, genActions):
    text = ""

    if event.name == "DrawNotificationEvent":
        text += "leBool %s_OnPaint(%s, leRect* rect)\n" % (widget.getName(), getWidgetVariableName(widget))

    text += generateActions(widget, event, genActions, None, ["return LE_TRUE; // true indicates that drawing is complete"])

    return text

def generateDrawSurfaceAction(text, variables, owner, event, action):
    name = action.targetName

    generateWidgetAction(text, variables, owner, event, action)