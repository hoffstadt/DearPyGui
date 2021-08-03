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
			mvPythonParser parser(mvPyDataType::UUID, "Displays a file or directory selector depending on keywords. Displays a file dialog by default.", { "Containers", "Widgets", "File Dialog" }, true);
			mvAppItem::AddCommonArgs(parser, (CommonParserArgs)(
				MV_PARSER_ARG_ID |
				MV_PARSER_ARG_WIDTH |
				MV_PARSER_ARG_HEIGHT |
				MV_PARSER_ARG_CALLBACK |
				MV_PARSER_ARG_SHOW)
			);

			parser.addArg<mvPyDataType::String>("default_path", mvArgType::KEYWORD_ARG, "''", "Path that the file dialog will default to when opened.");
			parser.addArg<mvPyDataType::String>("default_filename", mvArgType::KEYWORD_ARG, "'.'", "Default name that will show in the file name input.");
			parser.addArg<mvPyDataType::Integer>("file_count", mvArgType::KEYWORD_ARG, "0", "Number of visible files in the dialog.");
			parser.addArg<mvPyDataType::Bool>("modal", mvArgType::KEYWORD_ARG, "False", "Forces user interaction with the file selector.");
			parser.addArg<mvPyDataType::Bool>("directory_selector", mvArgType::KEYWORD_ARG, "False", "Shows only directory/paths as options. Allows selection of directory/paths only.");

			parser.finalize();
			parsers->insert({ s_command, parser });
		}

		{
			mvPythonParser parser(mvPyDataType::Dict, "Undocumented function", { "Widgets" });
			parser.addArg<mvPyDataType::UUID>("file_dialog");
			parser.finalize();
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

	void mvFileDialog::setLabel(const std::string& value)
	{
		_specificedlabel = value;
		_label = value + "###" + std::to_string(_uuid);
	}

	void mvFileDialog::drawPanel()
	{
		for (auto& item : _children[1])
		{

			if (!item->preDraw())
				continue;

			item->draw(ImGui::GetWindowDrawList(), ImGui::GetCursorPosX(), ImGui::GetCursorPosY());

			item->postDraw();
		}

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
				_filters.append(static_cast<mvFileExtension*>(item.get())->getFilter());
				_filters.append(",");
			}

			_dirtySettings = false;
		}

		
		// without panel
		if (_children[1].empty())
		{
			if (_modal)
				_instance.OpenModal(_label.c_str(), _label.c_str(), _directory ? nullptr : _filters.c_str(), _defaultPath, _defaultFilename, _fileCount);
			else
				_instance.OpenDialog(_label.c_str(), _label.c_str(), _directory ? nullptr : _filters.c_str(), _defaultPath, _defaultFilename, _fileCount);
		}

		// with panel
		else
		{

			if (_modal)
				_instance.OpenModal(_label.c_str(), _label.c_str(), _directory ? nullptr : _filters.c_str(), _defaultPath, _defaultFilename,
					std::bind(&Panel, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3), 250.0f, _fileCount, IGFDUserDatas(this));
			else
				_instance.OpenDialog(_label.c_str(), _label.c_str(), _directory ? nullptr : _filters.c_str(), _defaultPath, _defaultFilename,
					std::bind(&Panel, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3), 250.0f, _fileCount, IGFDUserDatas(this));
		}

		{
			//mvFontScope fscope(this);

			// display
			if (_instance.Display(_label.c_str(), ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoSavedSettings, ImVec2(500, 600)))
			{

				// action if OK
				if (_instance.IsOk())
				{
					mvApp::GetApp()->getCallbackRegistry().submitCallback([&]()
						{
							mvApp::GetApp()->getCallbackRegistry().runCallback(_callback, _uuid, getInfoDict(), _user_data);
						});

				}

				// close
				_instance.Close();
				_show = false;
			}
		}
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

		mvAppItem* item = mvApp::GetApp()->getItemRegistry().getItem(dataSource);
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
		_value = std::get<std::shared_ptr<bool>>(item->getValue());
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

		PyDict_SetItemString(dict, "file_count", ToPyInt(_fileCount));
		PyDict_SetItemString(dict, "default_filename", ToPyString(_defaultFilename));
		PyDict_SetItemString(dict, "default_path", ToPyString(_defaultPath));
		PyDict_SetItemString(dict, "modal", ToPyBool(_modal));
		PyDict_SetItemString(dict, "directory_selector", ToPyBool(_directory));
	}

	PyObject* mvFileDialog::getInfoDict()
	{
		PyObject* dict = PyDict_New();

		PyDict_SetItemString(dict, "file_path_name", ToPyString(_instance.GetFilePathName()));
		PyDict_SetItemString(dict, "file_name", ToPyString(_instance.GetCurrentFileName()));
		PyDict_SetItemString(dict, "file_name_buffer", ToPyString(_instance.FileNameBuffer));
		PyDict_SetItemString(dict, "current_path", ToPyString(_instance.GetCurrentPath()));
		PyDict_SetItemString(dict, "current_filter", ToPyString(_instance.GetCurrentFilter()));

		auto selections = _instance.GetSelection();

		PyObject* sel = PyDict_New();
		for(auto& item : selections)
			PyDict_SetItemString(sel, item.first.c_str(), ToPyString(item.second));
		PyDict_SetItemString(dict, "selections", sel);

		return dict;
	}

	PyObject* mvFileDialog::get_file_dialog_info(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		mvUUID file_dialog;

		if (!(mvApp::GetApp()->getParsers())["get_file_dialog_info"].parse(args, kwargs, __FUNCTION__, &file_dialog))
			return GetPyNone();

		if (!mvApp::s_manualMutexControl) std::lock_guard<std::mutex> lk(mvApp::s_mutex);
		auto aplot = mvApp::GetApp()->getItemRegistry().getItem(file_dialog);
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