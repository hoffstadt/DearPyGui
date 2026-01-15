#include "mvFontItems.h"
#include "mvTextureItems.h"
#include "mvPyUtils.h"
#include "mvToolManager.h"
#include "mvFontManager.h"

mvFontRegistry::mvFontRegistry(mvUUID uuid)
	:
	mvAppItem(uuid)
{
	config.show = true;
}

void mvFontRegistry::resetFont()
{
	for (auto& item : childslots[1])
	{
		static_cast<mvFont*>(item.get())->_default = false;
	}

	mvToolManager::GetFontManager()._resetDefault = true;
}

void mvFontRegistry::draw(ImDrawList* drawlist, float x, float y)
{
	//ImGuiIO& io = ImGui::GetIO();
	//io.Fonts->Clear();
	//io.FontDefault = io.Fonts->AddFontDefault();

	for (auto& item : childslots[1])
	{
		item->draw(drawlist, ImGui::GetCursorPosX(), ImGui::GetCursorPosY());
	}
	config.show = false;
}

void mvCharRemap::handleSpecificRequiredArgs(PyObject* dict)
{
	if (!VerifyRequiredArguments(GetParsers()[GetEntityCommand(type)], dict))
		return;

	_source = ToInt(PyTuple_GetItem(dict, 0));
	_target = ToInt(PyTuple_GetItem(dict, 1));
}

void mvFont::customAction(void* data)
{
	if (!state.ok)
		return;

	ImGuiIO& io = ImGui::GetIO();

	ImFontConfig cfg;
	cfg.PixelSnapH = _pixel_snap_h;
	_fontPtr = io.Fonts->AddFontFromFileTTF(_file.c_str(), _size, &cfg);

	if (_fontPtr == nullptr)
	{
		mvThrowPythonError(mvErrorCode::mvNone, "Font file could not be found");
		io.Fonts->Build();
		return;
	}

	// handled by char remaps
	//for (auto& item : font.charRemaps)
	//	_fontPtr->AddRemapChar(item.first, item.second);

	io.Fonts->Build();

	if (_default)
		io.FontDefault = _fontPtr;

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

void mvFont::draw(ImDrawList* drawlist, float x, float y)
{

	if (!state.ok)
		return;

	//_dirty = true;

	mvToolManager::GetFontManager()._dirty = true;
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

    if (PyObject* item = PyDict_GetItemString(dict, "pixel_snapH")) _pixel_snap_h = ToBool(item);
}

void mvFont::getSpecificConfiguration(PyObject* dict)
{
	if (dict == nullptr)
		return;

	PyDict_SetItemString(dict, "file", ToPyString(_file));
	PyDict_SetItemString(dict, "size", ToPyFloat(_size));
	PyDict_SetItemString(dict, "pixel_snapH", ToPyBool(_pixel_snap_h));
}
