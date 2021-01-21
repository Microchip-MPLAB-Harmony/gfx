def generateScreenEvent(screen, event, genActions):
    text = ""

    if event.name == "ShowEvent":
        text += "void %s_OnShow(void)" % screen.getName()

    if event.name == "HideEvent":
        text += "void %s_OnHide(void)" % screen.getName()

    if event.name == "UpdateEvent":
        text += "void %s_OnUpdate(void)" % screen.getName()

    text += generateActions(widget, event, genActions, None, None)

    return text


eventGenerateFunctions = {
    "ButtonWidget": generateButtonEvent,
    "CheckBoxWidget": generateCheckBoxEvent,
    "CircularGaugeWidget": generateCircularGaugeEvent,
    "CircularSliderWidget": generateCircularSliderEvent,
    "DrawSurfaceWidget": generateDrawSurfaceEvent,
    "ImageSequenceWidget": generateImageSequenceEvent,
    "KeyPadWidget": generateKeyPadEvent,
    "ListWidget": generateListEvent,
    "ListWheelWidget": generateListWheelEvent,
    "PieChartWidget": generatePieChartEvent,
    "ProgressBarWidget": generateProgressBarEvent,
    "RadialMenuWidget": generateRadialMenuEvent,
    "RadioButtonWidget": generateRadioButtonEvent,
    "ScrollBarWidget": generateScrollBarEvent,
    "SliderWidget": generateSliderEvent,
    "TextFieldWidget": generateTextFieldEvent,
    "TouchTestWidget": generateTouchTestEvent,
}

actionGenerateFunctions = {
    "Context" : generateContextAction,
    "Screen" : generateScreenAction,
    "ArcWidget" : generateArcAction,
    "BarGraphWidget" : generateBarGraphAction,
    "ButtonWidget": generateButtonAction,
    "CheckBoxWidget": generateCheckBoxAction,
    "CircleWidget" : generateCircleAction,
    "CircularGaugeWidget": generateCircularGaugeAction,
    "CircularSliderWidget": generateCircularSliderAction,
    "DrawSurfaceWidget": generateDrawSurfaceAction,
    "GradientWidget": generateGradientAction,
    "GroupBoxWidget": generateGroupBoxAction,
    "ImageWidget": generateImageAction,
    "ImageRotateWidget": generateImageRotateAction,
    "ImageScaleWidget": generateImageScaleAction,
    "ImageSequenceWidget": generateImageSequenceAction,
    "KeyPadWidget": generateKeyPadAction,
    "LabelWidget": generateLabelAction,
    "LineWidget": generateLineAction,
    "LineGraphWidget": generateLineGraphAction,
    "ListWidget": generateListAction,
    "ListWheelWidget": generateListWheelAction,
    "PanelWidget": generatePanelAction,
    "PieChartWidget": generatePieChartAction,
    "ProgressBarWidget": generateProgressBarAction,
    "RadialMenuWidget": generateRadialMenuAction,
    "RadioButtonWidget": generateRadioButtonAction,
    "RectangleWidget": generateRectangleAction,
    "ScrollBarWidget": generateScrollBarAction,
    "SliderWidget": generateSliderAction,
    "TextFieldWidget": generateTextFieldAction,
    "TouchTestWidget": generateTouchTestAction,
    "WindowWidget": generateWindowAction,
}

def generateActions(owner, event, genActions, nullCheckText, returnText):
    text = []
    body = []
    variables = {}

    if genActions == True:
        text.append("{")

        actionList = event.getActionList()

        for action in actionList:
            generateAction(body, variables, owner, event, action)

        if len(variables) > 0:
            for var in variables:
                text.append("    %s %s;" % (variables[var], var))

            text.append("")

        if nullCheckText != None and len(nullCheckText) > 0:
            for line in nullCheckText:
                text.append("    %s" % line)

            text.append("")

        for line in body:
            text.append(line)

        if returnText != None and len(returnText) > 0:
            for line in returnText:
                text.append("    %s" % line)

        text.append("}")
    else:
        text.append(";")

    finalText = ""

    for line in text:
        finalText += line + "\n"

    return finalText

def generateEvent(screen, widget, event, genActions):
    text = ""

    if widget == None:
        text = generateScreenEvent(screen, event, genActions)
    else:
        text = eventGenerateFunctions[widget.getType()](screen, widget, event, genActions)

    return text


def getWidgetVariableName(widget):
    if widget.getType() == "ButtonWidget":
        return "leButtonWidget* btn"
    elif widget.getType() == "CheckBoxWidget":
        return "leCheckBoxWidget* box"
    elif widget.getType() == "CircularGaugeWidget":
        return "leCircularGaugeWidget* cir"
    elif widget.getType() == "CircularSliderWidget":
        return "leCircularSliderWidget* cir"
    elif widget.getType() == "DrawSurfaceWidget":
        return "leDrawSurfaceWidget* sfc"
    elif widget.getType() == "ImageSequenceWidget":
        return "leImageSequenceWidget* seq"
    elif widget.getType() == "KeyPadWidget":
        return "leKeyPadWidget* pad"
    elif widget.getType() == "ListWidget":
        return "leListWidget* lst"
    elif widget.getType() == "ListWheelWidget":
        return "leListWheelWidget* lst"
    elif widget.getType() == "PieChartWidget":
        return "lePieChartWidget* pie"
    elif widget.getType() == "ProgressBarWidget":
        return "leProgressBarWidget* bar"
    elif widget.getType() == "RadialMenuWidget":
        return "leRadialMenuWidget* rad"
    elif widget.getType() == "RadioButtonWidget":
        return "leRadioButtonWidget* btn"
    elif widget.getType() == "ScrollBarWidget":
        return "leScrollBarWidget* bar"
    elif widget.getType() == "SliderWidget":
        return "leSliderWidget* sld"
    elif widget.getType() == "TextFieldWidget":
        return "leTextFieldWidget* txt"
    elif widget.getType() == "TouchTestWidget":
        return "leTouchTestWidget* tch"