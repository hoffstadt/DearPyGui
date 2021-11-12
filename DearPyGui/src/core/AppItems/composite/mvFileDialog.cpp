#include "mvFileDialog.h"
#include "mvFileExtension.h"
#include "mvItemRegistry.h"
#include "mvPythonExceptions.h"
#include "mvLog.h"

static void Panel(const char* vFilter, IGFDUserDatas vUserDatas, bool* vCantContinue)
{
	static_cast<Marvel::mvFileDialog*>(vUserDatas)->drawPanel();
	*vCantContinue = static_cast<Marvel::mvFileDialog*>(vUserDatas)->getContinueValue();
}

namespace Marvel {

	void mvFileDialog::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{
		{
			std::vector<mvPythonDataElement> args;

			AddCommonArgs(args,(CommonParserArgs)(
				MV_PARSER_ARG_ID |
				MV_PARSER_ARG_WIDTH |
				MV_PARSER_ARG_HEIGHT |
				MV_PARSER_ARG_CALLBACK |
				MV_PARSER_ARG_SHOW)
			);

			args.push_back({ mvPyDataType::String, "default_path", mvArgType::KEYWORD_ARG, "''", "Path that the file dialog will default to when opened." });
			args.push_back({ mvPyDataType::String, "default_filename", mvArgType::KEYWORD_ARG, "'.'", "Default name that will show in the file name input." });
			args.push_back({ mvPyDataType::Integer, "file_count", mvArgType::KEYWORD_ARG, "0", "Number of visible files in the dialog." });
			args.push_back({ mvPyDataType::Bool, "modal", mvArgType::KEYWORD_ARG, "False", "Forces user interaction with the file selector." });
			args.push_back({ mvPyDataType::Bool, "directory_selector", mvArgType::KEYWORD_ARG, "False", "Shows only directory/paths as options. Allows selection of directory/paths only." });

			mvPythonParserSetup setup;
			setup.about = "Displays a file or directory selector depending on keywords. Displays a file dialog by default. Callback will be ran when the file or directory picker is closed. The app_data arguemnt will be populated with information related to the file and directory as a dictionary.";
			setup.category = { "Containers", "Widgets", "File Dialog" };
			setup.returnType = mvPyDataType::UUID;
			setup.createContextManager = true;

			mvPythonParser parser = FinalizeParser(setup, args);
			parsers->insert({ s_command, parser });
		}

		{
			std::vector<mvPythonDataElement> args;

			args.push_back({ mvPyDataType::UUID, "file_dialog" });

			mvPythonParserSetup setup;
			setup.about = "Returns information related to the file dialog. Typically used while the file dialog is in use to query data about the state or info related to the file dialog.";
			setup.category = { "Widgets", "File Dialog"};
			setup.returnType = mvPyDataType::Dict;

			mvPythonParser parser = FinalizeParser(setup, args);
			parsers->insert({ "get_file_dialog_info", parser });
		}

	}

	mvFileDialog::mvFileDialog(mvUUID uuid)
		: 
		mvAppItem(uuid)
	{
		*_value = true;
		_width = 500;
		_height = 500;
	}

	ImGuiFileDialog& mvFileDialog::getDialog()
	{
		return _instance;
	}

	void mvFileDialog::drawPanel()
	{
		for (auto& item : _children[1])
			item->draw(ImGui::GetWindowDrawList(), ImGui::GetCursorPosX(), ImGui::GetCursorPosY());

	}

	void mvFileDialog::draw(ImDrawList* drawlist, float x, float y)
	{
		ScopedID id(_uuid);

		if (!_show)
			return;

		// extensions
		if (_dirtySettings)
		{
			_filters.clear();
			for (auto& item : _children[0])
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
		if (_children[1].empty())
		{
			if (_modal)
				_instance.OpenModal(_internalLabel.c_str(), _internalLabel.c_str(), _directory ? nullptr : _filters.c_str(), _defaultPath, _defaultFilename, _fileCount);
			else
				_instance.OpenDialog(_internalLabel.c_str(), _internalLabel.c_str(), _directory ? nullptr : _filters.c_str(), _defaultPath, _defaultFilename, _fileCount);
		}

		// with panel
		else
		{

			if (_modal)
				_instance.OpenModal(_internalLabel.c_str(), _internalLabel.c_str(), _directory ? nullptr : _filters.c_str(), _defaultPath, _defaultFilename,
					std::bind(&Panel, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3), 250.0f, _fileCount, IGFDUserDatas(this));
			else
				_instance.OpenDialog(_internalLabel.c_str(), _internalLabel.c_str(), _directory ? nullptr : _filters.c_str(), _defaultPath, _defaultFilename,
					std::bind(&Panel, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3), 250.0f, _fileCount, IGFDUserDatas(this));
		}

		{
			//mvFontScope fscope(this);

			// display
			if (_instance.Display(_internalLabel, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoSavedSettings, ImVec2(500, 600)))
			{

				// action if OK
				if (_instance.IsOk())
				{
					mvSubmitCallback([&]()
						{
							if(_alias.empty())
								mvRunCallback(_callback, _uuid, getInfoDict(), _user_data);
							else	
								mvRunCallback(_callback, _alias, getInfoDict(), _user_data);
						});

				}

				// close
				_instance.Close();
				_show = false;
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
		if (dataSource == _source) return;
		_source = dataSource;

		mvAppItem* item = GetItem((*GContext->itemRegistry), dataSource);
		if (!item)
		{
			mvThrowPythonError(mvErrorCode::mvSourceNotFound, "set_value",
				"Source item not found: " + std::to_string(dataSource), this);
			return;
		}
		if (item->getValueType() != getValueType())
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

		auto selections = _instance.GetSelection();

		PyObject* sel = PyDict_New();
		for(auto& item : selections)
			PyDict_SetItemString(sel, item.first.c_str(), mvPyObject(ToPyString(item.second)));
		PyDict_SetItemString(dict, "selections", mvPyObject((sel)));

		return dict;
	}

	PyObject* mvFileDialog::get_file_dialog_info(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		PyObject* file_dialog_raw;

		if (!Parse((GetParsers())["get_file_dialog_info"], args, kwargs, __FUNCTION__, &file_dialog_raw))
			return GetPyNone();

		if (!GContext->manualMutexControl) std::lock_guard<std::mutex> lk(GContext->mutex);

		mvUUID file_dialog = GetIDFromPyObject(file_dialog_raw);

		auto aplot = GetItem(*GContext->itemRegistry, file_dialog);
		if (aplot == nullptr)
		{
			mvThrowPythonError(mvErrorCode::mvNone, std::to_string(file_dialog) + " plot does not exist.");
			return GetPyNone();
		}

		if (aplot->getType() != mvAppItemType::mvFileDialog)
		{
			mvThrowPythonError(mvErrorCode::mvNone, std::to_string(file_dialog) + " is not a plot.");
			return GetPyNone();
		}

		mvFileDialog* graph = static_cast<mvFileDialog*>(aplot);

		return graph->getInfoDict();
	}
}