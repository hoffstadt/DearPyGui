#include "mvPyUtils.h"
#pragma hdrstop

#include "mvFontItems.h"

#include "mvTextureItems.h"

void mvFontRegistry::customAction(void* data)
{
	if (!_dirty || !config.show)
		return;

	for (auto& item : childslots[1])
		item->customAction(nullptr);

	_dirty = false;
}

void mvFontRegistry::draw(ImDrawList* drawlist, float x, float y)
{
	for (auto& item : childslots[1])
	{
		item->draw(drawlist, ImGui::GetCursorPosX(), ImGui::GetCursorPosY());
	}
}

void mvCharRemap::handleSpecificRequiredArgs(PyObject* dict)
{
	if (!VerifyRequiredArguments(GetParsers()[GetEntityCommand(type)], dict))
		return;

	_source = ToInt(PyTuple_GetItem(dict, 0));
	_target = ToInt(PyTuple_GetItem(dict, 1));
}

mvFont::~mvFont()
{
	if (_fontPtr)
	{
		mvSubmitTask([=]() {
			ImGuiIO& io = ImGui::GetIO();
			io.Fonts->RemoveFont(_fontPtr);
		}, true);
	}
}

void mvFont::customAction(void* data)
{
	// If add_font arguments are bad, quit.
	if (!state.ok)
		return;
	// If the font has been already loaded, nothing to do here.
	if (_fontPtr)
		return;

	ImGuiIO& io = ImGui::GetIO();

	ImFontConfig cfg;
	cfg.PixelSnapH = _pixelSnapH;
	cfg.PixelSnapV = _pixelSnapV;
	_fontPtr = io.Fonts->AddFontFromFileTTF(_file.c_str(), _size, &cfg);

	if (_fontPtr == nullptr)
	{
		state.ok = false;
		mvThrowPythonError(mvErrorCode::mvNone, "Font file could not be found");
		return;
	}

	// check ranges
	for (const auto& range : childslots[1])
	{

		if (range->type == mvAppItemType::mvCharRemap)
		{
			const auto rangePtr = static_cast<const mvCharRemap*>(range.get());

			_fontPtr->AddRemapChar(rangePtr->getSourceChar(), rangePtr->getTargetChar());
		}

	}
}

void mvFont::handleSpecificRequiredArgs(PyObject* dict)
{
	if (!VerifyRequiredArguments(GetParsers()[GetEntityCommand(type)], dict))
		return;

	_file = ToString(PyTuple_GetItem(dict, 0));
	std::ifstream ifile;
	ifile.open(_file);
	if (ifile)
		ifile.close();
	else
	{
		state.ok = false;
		mvThrowPythonError(mvErrorCode::mvNone, "Font file could not be found");
	}

	_size = ToFloat(PyTuple_GetItem(dict, 1));

}

void mvFont::handleSpecificKeywordArgs(PyObject* dict)
{
	if (dict == nullptr)
		return;

    if (PyObject* item = PyDict_GetItemString(dict, "size")) _size = ToFloat(item);
    if (PyObject* item = PyDict_GetItemString(dict, "pixel_snapH")) _pixelSnapH = ToBool(item);
    if (PyObject* item = PyDict_GetItemString(dict, "pixel_snapV")) _pixelSnapV = ToBool(item);
}

void mvFont::getSpecificConfiguration(PyObject* dict)
{
	if (dict == nullptr)
		return;

	PyDict_SetItemString(dict, "file", mvPyObject(ToPyString(_file)));
	PyDict_SetItemString(dict, "size", mvPyObject(ToPyFloat(_size)));
	PyDict_SetItemString(dict, "pixel_snapH", mvPyObject(ToPyBool(_pixelSnapH)));
	PyDict_SetItemString(dict, "pixel_snapV", mvPyObject(ToPyBool(_pixelSnapV)));
}
