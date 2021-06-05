#include "mvFileDialog.h"
#include "mvFileExtension.h"
#include "mvItemRegistry.h"
#include "mvFontScope.h"
#include "mvPythonExceptions.h"
#include "mvLog.h"
#include "mvImGuiThemeScope.h"

static void Panel(const char* vFilter, IGFDUserDatas vUserDatas, bool* vCantContinue)
{
	static_cast<Marvel::mvFileDialog*>(vUserDatas)->drawPanel();
	*vCantContinue = static_cast<Marvel::mvFileDialog*>(vUserDatas)->getContinueValue();
}

namespace Marvel {

	void mvFileDialog::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{
		{
			mvPythonParser parser(mvPyDataType::String);
			mvAppItem::AddCommonArgs(parser, (CommonParserArgs)(
				MV_PARSER_ARG_ID |
				MV_PARSER_ARG_WIDTH |
				MV_PARSER_ARG_HEIGHT |
				MV_PARSER_ARG_LABEL |
				MV_PARSER_ARG_CALLBACK |
				MV_PARSER_ARG_SHOW)
			);

			parser.addArg<mvPyDataType::String>("default_path", mvArgType::KEYWORD_ARG, "''");
			parser.addArg<mvPyDataType::String>("default_filename", mvArgType::KEYWORD_ARG, "'.'");
			parser.addArg<mvPyDataType::Integer>("file_count", mvArgType::KEYWORD_ARG, "0");
			parser.addArg<mvPyDataType::Bool>("modal", mvArgType::KEYWORD_ARG, "False");
			parser.addArg<mvPyDataType::Bool>("directory_selector", mvArgType::KEYWORD_ARG, "False");

			parser.finalize();
			parsers->insert({ s_command, parser });
		}

		{
			mvPythonParser parser(mvPyDataType::Dict, "Undocumented function", { "Widgets" });
			parser.addArg<mvPyDataType::String>("file_dialog");
			parser.finalize();
			parsers->insert({ "get_file_dialog_info", parser });
		}

	}

	mvFileDialog::mvFileDialog(const std::string& name)
		: 
		mvBoolPtrBase(name)
	{
		*m_value = true;
		m_width = 500;
		m_height = 500;
	}

	ImGuiFileDialog& mvFileDialog::getDialog()
	{
		return m_instance;
	}

	void mvFileDialog::drawPanel()
	{
		for (auto& item : m_children[1])
		{

			if (!item->preDraw())
				continue;

			item->draw(ImGui::GetWindowDrawList(), ImGui::GetCursorPosX(), ImGui::GetCursorPosY());

			item->postDraw();
		}

	}

	void mvFileDialog::draw(ImDrawList* drawlist, float x, float y)
	{
		ScopedID id;
		mvImGuiThemeScope scope(this);
		mvFontScope fscope(this);
		if (!m_show)
			return;

		// extensions
		if (m_dirtySettings)
		{
			m_filters.clear();
			for (auto& item : m_children[0])
			{
				item->draw(drawlist, x, y);
				m_filters.append(static_cast<mvFileExtension*>(item.get())->getFilter());
				m_filters.append(",");
			}

			m_dirtySettings = false;
		}

		
		// ugly
		if (m_children[1].empty())
		{
			if (m_modal)
				m_instance.OpenModal(m_name.c_str(), m_label.c_str(), m_directory ? nullptr : m_filters.c_str(), m_defaultPath, m_defaultFilename, m_fileCount);
			else
				m_instance.OpenDialog(m_name.c_str(), m_label.c_str(), m_directory ? nullptr : m_filters.c_str(), m_defaultPath, m_defaultFilename, m_fileCount);
		}
		else
		{

			if (m_modal)
				m_instance.OpenModal(m_name.c_str(), m_label.c_str(), m_directory ? nullptr : m_filters.c_str(), m_defaultPath, m_defaultFilename,
					std::bind(&Panel, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3), 250.0f, m_fileCount, IGFDUserDatas(this));
			else
				m_instance.OpenDialog(m_name.c_str(), m_label.c_str(), m_directory ? nullptr : m_filters.c_str(), m_defaultPath, m_defaultFilename,
					std::bind(&Panel, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3), 250.0f, m_fileCount, IGFDUserDatas(this));
		}

		{
			mvFontScope fscope(this);

			// display
			if (m_instance.Display(m_name.c_str(), ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoSavedSettings, ImVec2(500, 600)))
			{

				// action if OK
				if (m_instance.IsOk())
				{
					mvApp::GetApp()->getCallbackRegistry().submitCallback([&]()
						{
							mvApp::GetApp()->getCallbackRegistry().runCallback(m_callback, m_name.c_str(), getInfoDict(), nullptr);
						});

				}

				// close
				m_instance.Close();
				m_show = false;
			}
		}
	}

	void mvFileDialog::handleSpecificKeywordArgs(PyObject* dict)
	{
		if (dict == nullptr)
			return;

		if (PyObject* item = PyDict_GetItemString(dict, "file_count")) m_fileCount = ToInt(item);
		if (PyObject* item = PyDict_GetItemString(dict, "default_filename")) m_defaultFilename = ToString(item);
		if (PyObject* item = PyDict_GetItemString(dict, "default_path")) m_defaultPath = ToString(item);
		if (PyObject* item = PyDict_GetItemString(dict, "modal")) m_modal = ToBool(item);
		if (PyObject* item = PyDict_GetItemString(dict, "directory_selector")) m_directory = ToBool(item);

	}

	void mvFileDialog::getSpecificConfiguration(PyObject* dict)
	{
		if (dict == nullptr)
			return;

		PyDict_SetItemString(dict, "file_count", ToPyInt(m_fileCount));
		PyDict_SetItemString(dict, "default_filename", ToPyString(m_defaultFilename));
		PyDict_SetItemString(dict, "default_path", ToPyString(m_defaultPath));
		PyDict_SetItemString(dict, "modal", ToPyBool(m_modal));
		PyDict_SetItemString(dict, "directory_selector", ToPyBool(m_directory));
	}

	PyObject* mvFileDialog::getInfoDict()
	{
		PyObject* dict = PyDict_New();

		PyDict_SetItemString(dict, "file_path_name", ToPyString(m_instance.GetFilePathName()));
		PyDict_SetItemString(dict, "file_name", ToPyString(m_instance.GetCurrentFileName()));
		PyDict_SetItemString(dict, "file_name_buffer", ToPyString(m_instance.FileNameBuffer));
		PyDict_SetItemString(dict, "current_path", ToPyString(m_instance.GetCurrentPath()));
		PyDict_SetItemString(dict, "current_filter", ToPyString(m_instance.GetCurrentFilter()));

		auto selections = m_instance.GetSelection();

		PyObject* sel = PyDict_New();
		for(auto& item : selections)
			PyDict_SetItemString(sel, item.first.c_str(), ToPyString(item.second));
		PyDict_SetItemString(dict, "selections", sel);

		return dict;
	}

	PyObject* mvFileDialog::get_file_dialog_info(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* file_dialog;

		if (!(mvApp::GetApp()->getParsers())["get_file_dialog_info"].parse(args, kwargs, __FUNCTION__, &file_dialog))
			return GetPyNone();

		std::lock_guard<std::mutex> lk(mvApp::GetApp()->getMutex());
		auto aplot = mvApp::GetApp()->getItemRegistry().getItem(file_dialog);
		if (aplot == nullptr)
		{
			std::string message = file_dialog;
			mvThrowPythonError(1000, message + " plot does not exist.");
			return GetPyNone();
		}

		if (aplot->getType() != mvAppItemType::mvFileDialog)
		{
			std::string message = file_dialog;
			mvThrowPythonError(1000, message + " is not a plot.");
			return GetPyNone();
		}

		mvFileDialog* graph = static_cast<mvFileDialog*>(aplot.get());

		return graph->getInfoDict();
	}
}