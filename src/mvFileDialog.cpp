#include "mvFileDialog.h"
#include "mvFileExtension.h"
#include "mvItemRegistry.h"
#include "mvPyUtils.h"

static void Panel(const char* vFilter, IGFDUserDatas vUserDatas, bool* vCantContinue)
{
static_cast<mvFileDialog*>(vUserDatas)->drawPanel();
*vCantContinue = static_cast<mvFileDialog*>(vUserDatas)->getContinueValue();
}

mvFileDialog::mvFileDialog(mvUUID uuid)
	: 
	mvAppItem(uuid)
{
	*_value = true;
	config.width = 500;
	config.height = 500;
}

ImGuiFileDialog& mvFileDialog::getDialog()
{
	return _instance;
}

void mvFileDialog::drawPanel()
{
	for (auto& item : childslots[1])
		item->draw(ImGui::GetWindowDrawList(), ImGui::GetCursorPosX(), ImGui::GetCursorPosY());

}

void mvFileDialog::draw(ImDrawList* drawlist, float x, float y)
{
	ScopedID id(uuid);

	if (!config.show)
		return;

	// extensions
	if (_dirtySettings)
	{
		_filters.clear();
		for (auto& item : childslots[0])
		{
			item->draw(drawlist, x, y);
			_filters.append(static_cast<mvFileExtension*>(item.get())->_extension);
			_filters.append(",");
		}

		_dirtySettings = false;
	}

	// remap selectable to FrameBgActive
	ImGuiStyle* style = &ImGui::GetStyle();
	ImGui::PushStyleColor(ImGuiCol_Header, style->Colors[ImGuiCol_FrameBgActive]);

	// without panel
	if (childslots[1].empty())
	{
		if (_modal)
			_instance.OpenModal(info.internalLabel.c_str(), info.internalLabel.c_str(), _directory ? nullptr : _filters.c_str(), _defaultPath, _defaultFilename, _fileCount);
		else
			_instance.OpenDialog(info.internalLabel.c_str(), info.internalLabel.c_str(), _directory ? nullptr : _filters.c_str(), _defaultPath, _defaultFilename, _fileCount);
	}

	// with panel
	else
	{

		if (_modal)
			_instance.OpenModal(info.internalLabel.c_str(), info.internalLabel.c_str(), _directory ? nullptr : _filters.c_str(), _defaultPath, _defaultFilename,
				std::bind(&Panel, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3), 250.0f, _fileCount, IGFDUserDatas(this));
		else
			_instance.OpenDialog(info.internalLabel.c_str(), info.internalLabel.c_str(), _directory ? nullptr : _filters.c_str(), _defaultPath, _defaultFilename,
				std::bind(&Panel, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3), 250.0f, _fileCount, IGFDUserDatas(this));
	}

	{
		//mvFontScope fscope(this);

		if (info.dirtyPos)
		{
			ImGui::SetNextWindowPos(state.pos);
			info.dirtyPos = false;
		}

		if (info.dirty_size)
		{
			ImGui::SetNextWindowSize(ImVec2((float)config.width, (float)config.height));
			info.dirty_size = false;
		}

		// display
		if (_instance.IsOpened())
		{
			state.rectSize = { _instance.windowSizeDPG.x, _instance.windowSizeDPG.y };
		}

		if (_instance.Display(info.internalLabel, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoSavedSettings, _min_size, _max_size))
		{

			// action if OK clicked or if cancel clicked and cancel callback provided
			if (_instance.IsOk() ||  _cancelCallback)
			{
				mvPyObjectStrictPtr callback;
				
				if(_instance.IsOk()) {
					callback = mvPyObjectStrictPtr(shared_from_this(), &config.callback);
				} else {
					callback = mvPyObjectStrictPtr(shared_from_this(), &_cancelCallback);
				}

				mvAddCallbackJob({callback, *this, MV_APP_DATA_FUNC(getInfoDict())}, false);
			}

			// close
			_instance.Close();
			config.show = false;
		}
	}

	ImGui::PopStyleColor();
}

PyObject* mvFileDialog::getPyValue()
{
	return ToPyBool(*_value);
}

void mvFileDialog::setPyValue(PyObject* value)
{
	*_value = ToBool(value);
}

void mvFileDialog::setDataSource(mvUUID dataSource)
{
	if (dataSource == config.source) return;
	config.source = dataSource;

	mvAppItem* item = GetItem((*GContext->itemRegistry), dataSource);
	if (!item)
	{
		mvThrowPythonError(mvErrorCode::mvSourceNotFound, "set_value",
			"Source item not found: " + std::to_string(dataSource), this);
		return;
	}
	if (DearPyGui::GetEntityValueType(item->type) != DearPyGui::GetEntityValueType(type))
	{
		mvThrowPythonError(mvErrorCode::mvSourceNotCompatible, "set_value",
			"Values types do not match: " + std::to_string(dataSource), this);
		return;
	}
	_value = *static_cast<std::shared_ptr<bool>*>(item->getValue());
}

void mvFileDialog::handleSpecificKeywordArgs(PyObject* dict)
{
	if (dict == nullptr)
		return;

	if (PyObject* item = PyDict_GetItemString(dict, "file_count")) _fileCount = ToInt(item);
	if (PyObject* item = PyDict_GetItemString(dict, "default_filename")) _defaultFilename = ToString(item);
	if (PyObject* item = PyDict_GetItemString(dict, "default_path")) _defaultPath = ToString(item);
	if (PyObject* item = PyDict_GetItemString(dict, "modal")) _modal = ToBool(item);
	if (PyObject* item = PyDict_GetItemString(dict, "directory_selector")) _directory = ToBool(item);

	if (PyObject* item = PyDict_GetItemString(dict, "min_size"))
	{
		auto min_size = ToIntVect(item);
		_min_size = { (float)min_size[0], (float)min_size[1] };
	}

	if (PyObject* item = PyDict_GetItemString(dict, "max_size"))
	{
		auto max_size = ToIntVect(item);
		_max_size = { (float)max_size[0], (float)max_size[1] };
	}

	if (PyObject* item = PyDict_GetItemString(dict, "cancel_callback"))
	{
		_cancelCallback = item;
	}

}

void mvFileDialog::getSpecificConfiguration(PyObject* dict)
{
	if (dict == nullptr)
		return;

	PyDict_SetItemString(dict, "file_count", mvPyObject(ToPyInt(_fileCount)));
	PyDict_SetItemString(dict, "default_filename", mvPyObject(ToPyString(_defaultFilename)));
	PyDict_SetItemString(dict, "default_path", mvPyObject(ToPyString(_defaultPath)));
	PyDict_SetItemString(dict, "modal", mvPyObject(ToPyBool(_modal)));
	PyDict_SetItemString(dict, "directory_selector", mvPyObject(ToPyBool(_directory)));
}

PyObject* mvFileDialog::getInfoDict()
{
	PyObject* dict = PyDict_New();

	PyDict_SetItemString(dict, "file_path_name", mvPyObject(ToPyString(_instance.GetFilePathName())));
	PyDict_SetItemString(dict, "file_name", mvPyObject(ToPyString(_instance.GetCurrentFileName())));
	//PyDict_SetItemString(dict, "file_name_buffer", mvPyObject(ToPyString(_instance.FileNameBuffer)));
	PyDict_SetItemString(dict, "current_path", mvPyObject(ToPyString(_instance.GetCurrentPath())));
	PyDict_SetItemString(dict, "current_filter", mvPyObject(ToPyString(_instance.GetCurrentFilter())));
	PyDict_SetItemString(dict, "min_size", mvPyObject(ToPyPair(_min_size.x, _min_size.y)));
	PyDict_SetItemString(dict, "max_size", mvPyObject(ToPyPair(_max_size.x, _max_size.y)));

	auto selections = _instance.GetSelection();

	PyObject* sel = PyDict_New();
	for(auto& item : selections)
		PyDict_SetItemString(sel, item.first.c_str(), mvPyObject(ToPyString(item.second)));
	PyDict_SetItemString(dict, "selections", mvPyObject((sel)));

	return dict;
}
