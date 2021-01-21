def generateCustomAction(text, variables, action):
    text.append("    // %s : %s - %s" % action.name, action.prettyName, action.description)

    lines = action.code.split('\r?\n')

    if len(lines) > 0:
        for line in lines:
            text.append("    %s" % line)

        text.append("")

def generateTemplateAction(text, variables, owner, event, action):
    text.append("    // " + action.name + " - " + action.actionID + " - " + action.targetName);

    actionGenerateFunctions[action.targetType](text, variables, owner, event, action)

    text.append("")


def generateAction(text, variables, owner, event, action):
    if action.type == "template":
        text = generateTemplateAction(text, variables, owner, event, action)
    else:
        text = generateCustomAction(text, action)


def getActionArgumentValue(action, name):
    type = action.argumentData[name].type.toString()

    if type == "INTEGER":
        return str(action.argumentData[name].value)

    elif type == "BOOLEAN":
        val = action.argumentData[name].value

        if val == True:
            return "LE_TRUE"
        else:
            return "LE_FALSE"

    elif type == "STRING":
        val = action.argumentData[name].value

        if testStringValidity(val) == False:
            return "NULL"

        val = craftStringAssetName(val)

        if val == "&tableString_NULL":
            return "NULL"

        return "(leString*)" + val

    elif type == "IMAGE" or type == "SCHEME":
        val = action.argumentData[name].value

        if testStringValidity(val) == False:
            return "NULL"

        val = craftAssetName(val)

        if val == "&NULL":
            return "NULL"

    elif type == "FONT":
        val = action.argumentData[name].value

        if testStringValidity(val) == False:
            return "NULL"

        val = craftAssetName(val)

        if val == "&NULL":
            return "NULL"

        return "(leFont*)" + val

    elif type == "OBJECT":
        val = action.argumentData[name].value

        if testStringValidity(val) == False:
            return "NULL"

        val = craftAssetName(val)

        if val == "&NULL":
            return "NULL"

        return "(leWidget*)" + val

    else:
        return action.argumentData[name].value


def writeActionFunc(text, action, func, args):
    argString = ""

    for arg in args:
        if arg is not None:
            argString += ", " + arg

    text.append("    %s->fn->%s(%s%s);" % (action.targetName, func, action.targetName, argString))

def writeActionFuncWithName(text, action, func, args, name):
    argString = ""

    for arg in args:
        if arg is not None:
            argString += ", " + arg

    text.append("    %s->fn->%s(%s%s);" % (name, func, name, argString))

def writeActionFuncWithRet(text, action, func, args, ret):
    argString = ""

    for arg in args:
        if arg is not None:
            argString += ", " + arg

    text.append("    %s = %s->fn->%s(%s%s);" % (ret, action.targetName, func, action.targetName, argString))