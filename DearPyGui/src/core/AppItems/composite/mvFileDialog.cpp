#include "mvFileDialog.h"
#include "mvItemRegistry.h"
#include "mvFontScope.h"

namespace Marvel {

	void mvFileDialog::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{
		{
			mvPythonParser parser(mvPyDataType::String);
			parser.addArg<mvPyDataType::Callable>("callback", mvArgType::KEYWORD_ARG, "None", "function to call on completion");
			parser.finalize();
			parsers->insert({ "select_directory_dialog", parser });
		}

		{
			mvPythonParser parser(mvPyDataType::String);
			parser.addArg<mvPyDataType::Callable>("callback", mvArgType::KEYWORD_ARG, "None", "function to call on completion");
			parser.addArg<mvPyDataType::String>("extensions", mvArgType::KEYWORD_ARG, "''", "filters items with extensions i.e '.*, .py'");
			parser.finalize();
			parsers->insert({ s_command, parser });
		}

	}

	mvFileDialog::mvFileDialog() 
		: 
		mvAppItem("filedialog")
	{
		m_width = 500;
		m_height = 500;
	}

	bool mvFileDialog::prerender2() 
	{ 
		return true; 
	}

	void mvFileDialog::setCallback(PyObject* callback)
	{ 
		m_callback2 = callback; 
	}

	void mvFileDialog::draw(ImDrawList* drawlist, float x, float y)
	{

		if (!m_show)
			return;

		if (m_dirty_size)
		{
			ImGui::SetNextWindowSize(ImVec2((float)m_width, (float)m_height));
			m_dirty_size = false;
		}

		if (m_dirtyPos)
		{
			ImGui::SetNextWindowPos(m_state.getItemPos());
			m_dirtyPos = false;
		}

		if (m_focusNextFrame)
		{
			ImGui::SetNextWindowFocus();
			m_focusNextFrame = false;
		}

		if (!ImGui::Begin(m_label.c_str(), &m_show, m_windowflags))
		{
			ImGui::End();
			return;
		}

		{
			mvFontScope fscope(this);

			// display
			if (ImGuiFileDialog::Instance()->Display("ChooseFileDlgKey", ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoSavedSettings, ImVec2(500, 600)))
			{

				// action if OK
				if (ImGuiFileDialog::Instance()->IsOk())
				{
					mvApp::GetApp()->getCallbackRegistry().submitCallback([&]()
						{
							mvApp::GetApp()->getCallbackRegistry().runCallback(m_callback2, "File Dialog", ToPyList({ ImGuiFileDialog::Instance()->GetCurrentPath(),
								ImGuiFileDialog::Instance()->FileNameBuffer }));

							// action
							if (m_callback2)
								Py_XDECREF(m_callback2);
							m_callback2 = nullptr;
						});

				}
				// close
				ImGuiFileDialog::Instance()->Close();
				m_show = false;
			}
		}

		ImGui::End();
	}

	PyObject* mvFileDialog::select_directory_dialog(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		PyObject* callback = nullptr;

		if (!(mvApp::GetApp()->getParsers())["select_directory_dialog"].parse(args, kwargs, __FUNCTION__, &callback))
			return GetPyNone();

		if (callback)
			Py_XINCREF(callback);

		mvApp::GetApp()->getCallbackRegistry().submit([=]()
			{
				ImGuiFileDialog::Instance()->OpenModal("ChooseFileDlgKey", "Choose Directory", 0, ".");
				auto window = mvApp::GetApp()->getItemRegistry().getItem("filedialog");
				auto dialog = static_cast<mvFileDialog*>(window.get());
				dialog->setCallback(callback);
				window->show();
			});

		return GetPyNone();
	}

	PyObject* mvFileDialog::open_file_dialog(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		PyObject* callback = nullptr;
		const char* extensions = ".*";

		if (!(mvApp::GetApp()->getParsers())["open_file_dialog"].parse(args, kwargs, __FUNCTION__,
			&callback, &extensions))
			return GetPyNone();

		if (callback)
			Py_XINCREF(callback);

		mvApp::GetApp()->getCallbackRegistry().submit([=]()
			{
				ImGuiFileDialog::Instance()->OpenModal("ChooseFileDlgKey", "Choose File", extensions, ".");
				auto window = mvApp::GetApp()->getItemRegistry().getItem("filedialog");
				auto dialog = static_cast<mvFileDialog*>(window.get());

				dialog->setCallback(callback);
				window->show();
			});

		return GetPyNone();
	}

	void mvFileDialog::setWidth(int width)
	{
		m_width = width;
		m_dirty_size = true;
	}

	void mvFileDialog::setHeight(int height)
	{
		m_height = height;
		m_dirty_size = true;
	}

	void mvFileDialog::handleSpecificKeywordArgs(PyObject* dict)
	{
		if (dict == nullptr)
			return;

		// helper for bit flipping
		auto flagop = [dict](const char* keyword, int flag, int& flags)
		{
			if (PyObject* item = PyDict_GetItemString(dict, keyword)) ToBool(item) ? flags |= flag : flags &= ~flag;
		};

		// window flags
		flagop("autosize", ImGuiWindowFlags_AlwaysAutoResize, m_windowflags);
		flagop("no_move", ImGuiWindowFlags_NoMove, m_windowflags);
		flagop("no_resize", ImGuiWindowFlags_NoResize, m_windowflags);
		flagop("no_title_bar", ImGuiWindowFlags_NoTitleBar, m_windowflags);
		flagop("no_scrollbar", ImGuiWindowFlags_NoScrollbar, m_windowflags);
		flagop("no_collapse", ImGuiWindowFlags_NoCollapse, m_windowflags);
		flagop("horizontal_scrollbar", ImGuiWindowFlags_HorizontalScrollbar, m_windowflags);
		flagop("no_focus_on_appearing", ImGuiWindowFlags_NoFocusOnAppearing, m_windowflags);
		flagop("no_bring_to_front_on_focus", ImGuiWindowFlags_NoBringToFrontOnFocus, m_windowflags);
		flagop("menubar", ImGuiWindowFlags_MenuBar, m_windowflags);
		flagop("no_background", ImGuiWindowFlags_NoBackground, m_windowflags);

	}

	void mvFileDialog::addFlag(ImGuiWindowFlags flag)
	{
		m_windowflags |= flag;
	}

	void mvFileDialog::removeFlag(ImGuiWindowFlags flag)
	{
		m_windowflags &= ~flag;
	}

	void mvFileDialog::getSpecificConfiguration(PyObject* dict)
	{
		if (dict == nullptr)
			return;

		// helper to check and set bit
		auto checkbitset = [dict](const char* keyword, int flag, const int& flags)
		{
			PyDict_SetItemString(dict, keyword, ToPyBool(flags & flag));
		};

		// window flags
		checkbitset("autosize", ImGuiWindowFlags_AlwaysAutoResize, m_windowflags);
		checkbitset("no_resize", ImGuiWindowFlags_NoResize, m_windowflags);
		checkbitset("no_title_bar", ImGuiWindowFlags_NoTitleBar, m_windowflags);
		checkbitset("no_move", ImGuiWindowFlags_NoMove, m_windowflags);
		checkbitset("no_scrollbar", ImGuiWindowFlags_NoScrollbar, m_windowflags);
		checkbitset("no_collapse", ImGuiWindowFlags_NoCollapse, m_windowflags);
		checkbitset("horizontal_scrollbar", ImGuiWindowFlags_HorizontalScrollbar, m_windowflags);
		checkbitset("no_focus_on_appearing", ImGuiWindowFlags_NoFocusOnAppearing, m_windowflags);
		checkbitset("no_bring_to_front_on_focus", ImGuiWindowFlags_NoBringToFrontOnFocus, m_windowflags);
		checkbitset("menubar", ImGuiWindowFlags_MenuBar, m_windowflags);
		checkbitset("no_background", ImGuiWindowFlags_NoBackground, m_windowflags);
	}

}