class WidgetDefault:
    def __init__(self):
        self.width = 100
        self.height = 100
        self.border = "Bevel"
        self.background = "Fill"
        self.halign = "Center"
        self.valign = "Middle"
        self.left = 4
        self.top = 4
        self.right = 4
        self.bottom = 4


commonDefaults = {}


def testStringValidity(string):
    if string is None:
        return False

    if len(string) == 0:
        return False

    return True

def craftAssetName(string):
    if testStringValidity(string) == False:
        return "NULL"

    return "&" + string

def craftStringAssetName(string):
    if testStringValidity(string) == False:
        return "NULL"

    return "&tableString_" + string

def getRelativePosition(pos):
    if pos == "LeftOf":
        return "LE_RELATIVE_POSITION_LEFTOF"
    elif pos == "Above":
        return "LE_RELATIVE_POSITION_ABOVE"
    elif pos == "RightOf":
        return "LE_RELATIVE_POSITION_RIGHTOF"
    elif pos == "Below":
        return "LE_RELATIVE_POSITION_BELOW"
    else:
        return "LE_RELATIVE_POSITION_BEHIND"

def getDirection(dir):
    if dir == "Right":
        return "LE_DIRECTION_RIGHT"
    elif dir == "Up":
        return "LE_DIRECTION_UP"
    elif dir == "Left":
        return "LE_DIRECTION_LEFT"
    else:
        return "LE_DIRECTION_DOWN"


def getHorzRelativePosition(pos):
    if pos == "LeftOf":
        return "LE_RELATIVE_POSITION_LEFTOF"
    else:
        return "LE_RELATIVE_POSITION_RIGHTOF"

def getOrientation(dir):
    if dir == "Vertical":
        return "LE_ORIENTATION_VERTICAL"
    else:
        return "LE_ORIENTATION_HORIZONTAL"


def getHAlign(dir):
    if dir == "Left":
        return "LE_HALIGN_LEFT"
    elif dir == "Center":
        return "LE_HALIGN_CENTER"
    else:
        return "LE_HALIGN_RIGHT"


def getVAlign(dir):
    if dir == "Top":
        return "LE_VALIGN_TOP"
    elif dir == "Middle":
        return "LE_VALIGN_MIDDLE"
    else:
        return "LE_VALIGN_BOTTOM"


def getRotationDirection(dir):
    if dir == "Clockwise":
        return "LE_CLOCKWISE"
    else:
        return "LE_COUNTER_CLOCKWISE"

def getBackgroundType(type):
    if type == "None":
        type = "LE_WIDGET_BACKGROUND_NONE"
    elif type == "Line":
        type = "LE_WIDGET_BACKGROUND_LINE"
    elif type == "Fill":
        type = "LE_WIDGET_BACKGROUND_FILL"

    return type


def getBorderType(type):
    if type == "None":
        type = "LE_WIDGET_BORDER_NONE"
    elif type == "Line":
        type = "LE_WIDGET_BORDER_LINE"
    elif type == "Bevel":
        type = "LE_WIDGET_BORDER_BEVEL"

    return type


def writeSetInt(file, name, func, val, defaultVal):
    if val == defaultVal:
        return

    file.write("    %s->fn->set%s(%s, %d);" % (name, func, name, val))


def writeSetInt2Arg(file, name, func, val, defaultVal, arg):
    if val == defaultVal:
        return

    file.write("    %s->fn->set%s(%s, %s, %d);" % (name, func, name, arg, val))


def writeSetBool2Arg(file, name, func, val, defaultVal, arg):
    if val == defaultVal:
        return

    if val == True:
        val = "LE_TRUE"
    else:
        val = "LE_FALSE"

    file.write("    %s->fn->set%s(%s, %s, %s);" % (name, func, name, arg, val))


def writeSetLiteralString(file, name, func, val, defaultVal):
    if val == defaultVal:
        return

    file.write("    %s->fn->set%s(%s, %s);" % (name, func, name, val))


def writeSetAssetName(file, name, func, val):
    if testStringValidity(val) == False:
        return

    val = craftAssetName(val)

    if val == "&NULL":
        return

    file.write("    %s->fn->set%s(%s, %s);" % (name, func, name, val))

def writeSetFontAssetName(file, name, func, val):
    if testStringValidity(val) == False:
        return

    val = craftAssetName(val)

    if val == "&NULL":
        return

    file.write("    %s->fn->set%s(%s, (leFont*)%s);" % (name, func, name, val))

def writeSetStringAssetName(file, name, func, val):
    if testStringValidity(val) == False:
        return

    val = craftStringAssetName(val)

    if val == "&tableString_NULL":
        return

    file.write("    %s->fn->set%s(%s, (leString*)%s);" % (name, func, name, val))

def writeSetBoolean(file, name, func, val, defaultVal):
    if val == defaultVal:
        return

    if val == True:
        file.write("    %s->fn->set%s(%s, LE_TRUE);" % (name, func, name))
    else:
        file.write("    %s->fn->set%s(%s, LE_FALSE);" % (name, func, name))


def writeEvent(file, name, wgt, evtName, func, cb):
    if wgt.getEventByName(evtName).enabled == False:
        return

    file.write("    %s->fn->set%s(%s, %s_%s);" % (name, func, name, name, cb))



def generateBaseWidget(file, screen, base):
    name = base.getName()
    
    file.write("    %s->fn->setPosition(%s, %d, %d);" % (name, name, base.getX(False), base.getY(False)))

    width = getDefaultWidth(base.getType())
    height = getDefaultHeight(base.getType())

    if width != base.getWidth() or height != base.getHeight():
        file.write("    %s->fn->setSize(%s, %d, %d);" % (name, name, base.getWidth(), base.getHeight()))
    
    if base.getAlphaEnable() == True:
        file.write("    %s->fn->setAlphaEnabled(%s, LE_TRUE);" % (name, name))
        
    alphaAmount = base.getAlphaLevel()
    
    if alphaAmount != 255:
        file.write("    %s->fn->setAlphaAmount(%s, %d);" % (name, name, alphaAmount))
    
    if base.getEnabled() == False:
        file.write("    %s->fn->setEnabled(%s, LE_FALSE);" % (name, name))
    
    if base.getVisible() == False:
        file.write("    %s->fn->setVisible(%s, LE_FALSE);" % (name, name))
        
    scheme = base.getSchemeName()
    
    if testStringValidity(scheme):
        file.write("    %s->fn->setScheme(%s, &%s);" % (name, name, scheme))

    backgroundType = base.getBackgroundType().toString()

    if getDefaultBackground(base.getType()) != backgroundType:
        backgroundType = getBackgroundType(base.getBackgroundType().toString())

        writeSetLiteralString(file, name, "BackgroundType", backgroundType, "LE_WIDGET_BACKGROUND_" + getDefaultBackground(base.getType()).upper())

    borderType = base.getBorderType().toString()

    if getDefaultBorder(base.getType()) != borderType:
        borderType = getBorderType(base.getBorderType().toString())

        writeSetLiteralString(file, name, "BorderType", borderType, "LE_WIDGET_BORDER_" + getDefaultBorder(base.getType()).upper())

    halign = base.getHAlignment().toString()

    if getDefaultHAlign(base.getType()) != halign:
        halign = getHAlign(halign)

        writeSetLiteralString(file, name, "HAlignment", halign, "LE_HALIGN_" + getDefaultHAlign(base.getType()).upper())

    valign = base.getVAlignment().toString()

    if getDefaultVAlign(base.getType()) != valign:
        valign = getVAlign(valign)

        writeSetLiteralString(file, name, "VAlignment", valign, "LE_VALIGN_" + getDefaultVAlign(base.getType()).upper())

    left = getDefaultLeftMargin(base.getType())
    top = getDefaultTopMargin(base.getType())
    right = getDefaultRightMargin(base.getType())
    bottom = getDefaultBottomMargin(base.getType())

    if left != base.getLeftMargin() or \
        top != base.getTopMargin() or \
        right != base.getRightMargin() or \
        bottom != base.getBottomMargin():
        file.write("    %s->fn->setMargins(%s, %d, %d, %d, %d);" % (name, name, base.getLeftMargin(), base.getTopMargin(), base.getRightMargin(), base.getBottomMargin()))


execfile(_SCRIPTPATH + "widget_arc.py")
execfile(_SCRIPTPATH + "widget_bargraph.py")
execfile(_SCRIPTPATH + "widget_button.py")
execfile(_SCRIPTPATH + "widget_checkbox.py")
execfile(_SCRIPTPATH + "widget_circle.py")
execfile(_SCRIPTPATH + "widget_circulargauge.py")
execfile(_SCRIPTPATH + "widget_circularslider.py")
execfile(_SCRIPTPATH + "widget_drawsurface.py")
execfile(_SCRIPTPATH + "widget_gradient.py")
execfile(_SCRIPTPATH + "widget_groupbox.py")
execfile(_SCRIPTPATH + "widget_image.py")
execfile(_SCRIPTPATH + "widget_imagerotate.py")
execfile(_SCRIPTPATH + "widget_imagescale.py")
execfile(_SCRIPTPATH + "widget_imagesequence.py")
execfile(_SCRIPTPATH + "widget_keypad.py")
execfile(_SCRIPTPATH + "widget_label.py")
execfile(_SCRIPTPATH + "widget_line.py")
execfile(_SCRIPTPATH + "widget_linegraph.py")
execfile(_SCRIPTPATH + "widget_list.py")
execfile(_SCRIPTPATH + "widget_listwheel.py")
execfile(_SCRIPTPATH + "widget_panel.py")
execfile(_SCRIPTPATH + "widget_piechart.py")
execfile(_SCRIPTPATH + "widget_progressbar.py")
execfile(_SCRIPTPATH + "widget_radialmenu.py")
execfile(_SCRIPTPATH + "widget_radiobutton.py")
execfile(_SCRIPTPATH + "widget_rectangle.py")
execfile(_SCRIPTPATH + "widget_scrollbar.py")
execfile(_SCRIPTPATH + "widget_slider.py")
execfile(_SCRIPTPATH + "widget_textfield.py")
execfile(_SCRIPTPATH + "widget_touchtest.py")
execfile(_SCRIPTPATH + "widget_window.py")

generateFunctions = {
    "ArcWidget" : generateArcWidget,
    "BarGraphWidget" : generateBarGraphWidget,
    "ButtonWidget": generateButtonWidget,
    "CheckBoxWidget": generateCheckBoxWidget,
    "CircleWidget": generateCircleWidget,
    "CircularGaugeWidget": generateCircularGaugeWidget,
    "CircularSliderWidget": generateCircularSliderWidget,
    "DrawSurfaceWidget": generateDrawSurfaceWidget,
    "GradientWidget": generateGradientWidget,
    "GroupBoxWidget": generateGroupBoxWidget,
    "ImageWidget": generateImageWidget,
    "ImageRotateWidget": generateImageRotateWidget,
    "ImageScaleWidget": generateImageScaleWidget,
    "ImageSequenceWidget": generateImageSequenceWidget,
    "KeyPadWidget": generateKeyPadWidget,
    "LabelWidget": generateLabelWidget,
    "LineWidget": generateLineWidget,
    "LineGraphWidget": generateLineGraphWidget,
    "ListWidget": generateListWidget,
    "ListWheelWidget": generateListWheelWidget,
    "PanelWidget": generatePanelWidget,
    "PieChartWidget": generatePieChartWidget,
    "ProgressBarWidget": generateProgressBarWidget,
    "RadialMenuWidget": generateRadialMenuWidget,
    "RadioButtonWidget": generateRadioButtonWidget,
    "RectangleWidget": generateRectangleWidget,
    "ScrollBarWidget": generateScrollBarWidget,
    "SliderWidget": generateSliderWidget,
    "TextFieldWidget": generateTextFieldWidget,
    "TouchTestWidget": generateTouchTestWidget,
    "WindowWidget": generateWindowWidget,
}

def generateWidget(file, screen, widget, parentName):
    generateFunctions[widget.getType()](file, screen, widget, parentName)

    childList = widget.getChildren()
    
    for child in childList:
        generateWidget(file, screen, child, widget.getName())


def generateWidgetAction(text, variables, owner, event, action):
    name = action.targetName

    if action.actionID == "HAlign":
        val = getHAlign(getActionArgumentValue(action, "HAlign"))

        writeActionFunc(text, action, "setHAlignment", [val])

    elif action.actionID == "VAlign":
        val = getVAlign(getActionArgumentValue(action, "VAlign"))

        writeActionFunc(text, action, "setVAlignment", [val])

    elif action.actionID == "SetPosition":
        x = getActionArgumentValue(action, "X")
        y = getActionArgumentValue(action, "Y")

        writeActionFunc(text, action, "setPosition", [x, y])

    elif action.actionID == "SetX":
        val = getActionArgumentValue(action, "X")

        writeActionFunc(text, action, "setX", [val])

    elif action.actionID == "SetY":
        val = getActionArgumentValue(action, "Y")

        writeActionFunc(text, action, "setY", [val])

    elif action.actionID == "SetWidth":
        val = getActionArgumentValue(action, "Width")

        writeActionFunc(text, action, "setWidth", [val])

    elif action.actionID == "SetHeight":
        val = getActionArgumentValue(action, "Height")

        writeActionFunc(text, action, "setHeight", [val])

    elif action.actionID == "Translate":
        x = getActionArgumentValue(action, "X")
        y = getActionArgumentValue(action, "Y")

        writeActionFunc(text, action, "translate", [x, y])

    elif action.actionID == "SetSize":
        w = getActionArgumentValue(action, "Width")
        h = getActionArgumentValue(action, "Height")

        writeActionFunc(text, action, "setSize", [w, h])

    elif action.actionID == "Resize":
        w = getActionArgumentValue(action, "Width")
        h = getActionArgumentValue(action, "Height")

        writeActionFunc(text, action, "resize", [w, h])

    elif action.actionID == "SetParent":
        val = getActionArgumentValue(action, "Parent")

        writeActionFunc(text, action, "setParent", [val])

    elif action.actionID == "SetEnable":
        val = getActionArgumentValue(action, "Enabled")

        writeActionFunc(text, action, "setEnabled", [val])

    elif action.actionID == "SetVisible":
        val = getActionArgumentValue(action, "Visible")

        writeActionFunc(text, action, "setVisible", [val])

    elif action.actionID == "SetMargins":
        l = getActionArgumentValue(action, "Left")
        t = getActionArgumentValue(action, "Top")
        r = getActionArgumentValue(action, "Right")
        b = getActionArgumentValue(action, "Bottom")

        writeActionFunc(text, action, "setMargins", [l, t, r, b])

    elif action.actionID == "SetBorderType":
        val = getBorderType(getActionArgumentValue(action, "Type"))

        writeActionFunc(text, action, "setBorderType", [val])

    elif action.actionID == "SetBackgroundType":
        val = getBackgroundType(getActionArgumentValue(action, "Type"))

        writeActionFunc(text, action, "setBackgroundType", [val])

    elif action.actionID == "SetScheme":
        val = getActionArgumentValue(action, "Scheme")

        writeActionFunc(text, action, "setScheme", [val])

