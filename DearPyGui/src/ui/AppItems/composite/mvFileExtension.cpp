#include "mvFileExtension.h"
#include "mvFileDialog.h"
#include "mvItemRegistry.h"
#include "mvPythonExceptions.h"
#include "mvLog.h"

mvFileExtension::mvFileExtension(mvUUID uuid)
	: 
	mvAppItem(uuid)
{
}

void mvFileExtension::applySpecificTemplate(mvAppItem* item)
{
	auto titem = static_cast<mvFileExtension*>(item);
	_customText = titem->_customText;
	_extension = titem->_extension;
}

void mvFileExtension::draw(ImDrawList* drawlist, float x, float y)
{
	IGFD::FileExtentionInfos info;
	info.icon = _customText;
	if (_color.r >= 0)
		info.color = _color.toVec4();
	else
		info.color = { 1.0f, 1.0f, 1.0f, 1.0f };
	static_cast<mvFileDialog*>(this->info.parentPtr)->getDialog().SetExtentionInfos(_extension.c_str(), info);
}

void mvFileExtension::handleSpecificRequiredArgs(PyObject* dict)
{
	if (!VerifyRequiredArguments(GetParsers()[GetEntityCommand(type)], dict))
		return;

	for (int i = 0; i < PyTuple_Size(dict); i++)
	{
		PyObject* item = PyTuple_GetItem(dict, i);
		switch (i)
		{
		case 0:
			_extension = ToString(item);
			break;

		default:
			break;
		}
	}
}

void mvFileExtension::handleSpecificKeywordArgs(PyObject* dict)
{
	if (dict == nullptr)
		return;

	if (PyObject* item = PyDict_GetItemString(dict, "custom_text"))_customText = ToString(item);
	if (PyObject* item = PyDict_GetItemString(dict, "color")) _color = ToColor(item);

	if (info.parentPtr)
		static_cast<mvFileDialog*>(info.parentPtr)->markDirty();
}

void mvFileExtension::getSpecificConfiguration(PyObject* dict)
{
	if (dict == nullptr)
		return;

	PyDict_SetItemString(dict, "custom_text", mvPyObject(ToPyString(_customText)));
	PyDict_SetItemString(dict, "color", mvPyObject(ToPyColor(_color)));
}