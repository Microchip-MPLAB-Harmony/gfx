def validateString(str, list):
	if str is not None and len(str.strip()) > 0 and str not in list:
		return True

	return False

def widgetQueryStringActions(widget, stringList):
	events = widget.getEventList()

	for event in events:
		if event.getEnabled() == False:
			continue

		actionList = event.getActionList()

		for action in actionList:
			if action.getType() != "template":
				continue

			for argData in action.getArgumentValues():
				if argData.type.toString() == "STRING":
					if validateString(argData.value, stringList):
						stringList.append(argData.value)

def getWidgetStringList(widgetList):
	stringList = []

	for widget in widgetList:
		widgetQueryStringActions(widget, stringList)

		if widget.getType() == "BarGraphWidget":
			categoryList = widget.getCategories()

			for i in range(0, len(categoryList)):
				if validateString(categoryList[i].string, stringList):
					stringList.append(categoryList[i].string)

		if widget.getType() == "ButtonWidget":
			if validateString(widget.getStringName(), stringList):
				stringList.append(widget.getStringName())

		if widget.getType() == "CheckBoxWidget":
			if validateString(widget.getStringName(), stringList):
				stringList.append(widget.getStringName())

		if widget.getType() == "CircularGuageWidget":
			if validateString(widget.getTickLabelString(), stringList):
				stringList.append(widget.getTickLabelString())

		if widget.getType() == "GroupBoxWidget":
			if validateString(widget.getStringName(), stringList):
				stringList.append(widget.getStringName())

		if widget.getType() == "LabelWidget":
			if validateString(widget.getStringName(), stringList):
				stringList.append(widget.getStringName())

		if widget.getType() == "KeyPadWidget":
			rows = widget.getCells()

			for row in rows:
				for cell in row:
					if validateString(cell.string, stringList):
						stringList.append(cell.string)

					if validateString(cell.value, stringList):
						stringList.append(cell.value)

		if widget.getType() == "LineGraphWidget":
			categoryList = widget.getCategories()

			for i in range(0, len(categoryList)):
				if validateString(categoryList[i].string, stringList):
					stringList.append(categoryList[i].string)

		if widget.getType() == "ListWidget":
			for item in widget.getItems():
				if validateString(item.text, stringList):
					stringList.append(item.text)

		if widget.getType() == "ListWheelWidget":
			for item in widget.getItems():
				if validateString(item.text, stringList):
					stringList.append(item.text)

		if widget.getType() == "RadioButtonWidget":
			if validateString(widget.getStringName(), stringList):
				stringList.append(widget.getStringName())

		if widget.getType() == "TextFieldWidget":
			if validateString(widget.getStringName(), stringList):
				stringList.append(widget.getStringName())

			if validateString(widget.getHintStringName(), stringList):
				stringList.append(widget.getHintStringName())

		if widget.getType() == "WindowWidget":
			if validateString(widget.getTitleName(), stringList):
				stringList.append(widget.getTitleName())

	return stringList


def getDefaultWidth(type):
	return commonDefaults[type].width


def getDefaultHeight(type):
	return commonDefaults[type].height


def getDefaultBorder(type):
	return commonDefaults[type].border


def getDefaultBackground(type):
	return commonDefaults[type].background


def getDefaultHAlign(type):
	return commonDefaults[type].halign


def getDefaultVAlign(type):
	return commonDefaults[type].valign


def getDefaultLeftMargin(type):
	return commonDefaults[type].left


def getDefaultTopMargin(type):
	return commonDefaults[type].top


def getDefaultRightMargin(type):
	return commonDefaults[type].right


def getDefaultBottomMargin(type):
	return commonDefaults[type].bottom
