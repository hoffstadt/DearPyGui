#include "mvApp.h"
#include "mvCore.h"
#include "mvLogger.h"
#include "AppItems/mvAppItems.h"

namespace Marvel {

	mvApp* mvApp::s_instance = nullptr;

	static void SetStyle(ImGuiStyle& style, mvStyle& mvstyle)
	{
		style.Alpha = mvstyle[ImGuiStyleVar_Alpha].x;
		style.WindowPadding = { mvstyle[ImGuiStyleVar_WindowPadding].x, mvstyle[ImGuiStyleVar_WindowPadding].y };
		style.WindowRounding = mvstyle[ImGuiStyleVar_WindowRounding].x;
		style.WindowBorderSize = mvstyle[ImGuiStyleVar_WindowBorderSize].x;
		style.WindowTitleAlign = { mvstyle[ImGuiStyleVar_WindowTitleAlign].x, mvstyle[ImGuiStyleVar_WindowTitleAlign].y };
		style.ChildRounding = mvstyle[ImGuiStyleVar_ChildRounding].x;
		style.ChildBorderSize = mvstyle[ImGuiStyleVar_ChildBorderSize].x;
		style.PopupRounding = mvstyle[ImGuiStyleVar_PopupRounding].x;
		style.PopupBorderSize = mvstyle[ImGuiStyleVar_PopupBorderSize].x;
		style.FramePadding = { mvstyle[ImGuiStyleVar_FramePadding].x, mvstyle[ImGuiStyleVar_FramePadding].y };
		style.FrameRounding = mvstyle[ImGuiStyleVar_FrameRounding].x;
		style.FrameBorderSize = mvstyle[ImGuiStyleVar_FrameBorderSize].x;
		style.ItemSpacing = { mvstyle[ImGuiStyleVar_ItemSpacing].x, mvstyle[ImGuiStyleVar_ItemSpacing].y };
		style.ItemInnerSpacing = { mvstyle[ImGuiStyleVar_ItemInnerSpacing].x, mvstyle[ImGuiStyleVar_ItemInnerSpacing].y };
		style.TouchExtraPadding = { mvstyle[23].x, mvstyle[23].y };
		style.IndentSpacing = mvstyle[ImGuiStyleVar_IndentSpacing].x;
		style.ScrollbarSize = mvstyle[ImGuiStyleVar_ScrollbarSize].x;
		style.ScrollbarRounding = mvstyle[ImGuiStyleVar_ScrollbarRounding].x;
		style.GrabMinSize = mvstyle[ImGuiStyleVar_GrabMinSize].x;
		style.GrabRounding = mvstyle[ImGuiStyleVar_GrabRounding].x;
		style.TabRounding = mvstyle[ImGuiStyleVar_TabRounding].x;
		style.TabBorderSize = mvstyle[24].x;
		style.ButtonTextAlign = { mvstyle[ImGuiStyleVar_ButtonTextAlign].x, mvstyle[ImGuiStyleVar_ButtonTextAlign].y };
		style.SelectableTextAlign = { mvstyle[ImGuiStyleVar_SelectableTextAlign].x, mvstyle[ImGuiStyleVar_SelectableTextAlign].y };
		//style.DisplayWindowPadding = { mvstyle[25].x, mvstyle[25].y };
		style.DisplaySafeAreaPadding = { mvstyle[25].x, mvstyle[25].y };
		style.AntiAliasedLines = mvstyle[26].x > 0.5f;
		style.AntiAliasedFill = mvstyle[27].x > 0.5f;
		style.CurveTessellationTol = mvstyle[28].x;
		style.CircleSegmentMaxError = mvstyle[29].x;
	}

	mvApp::mvApp()
	{
		m_style = getAppDefaultStyle();
		m_windowflags = ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoSavedSettings
			| ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar;


	}

	static void prepareStandardCallbacks()
	{
		ImGuiIO& io = ImGui::GetIO();

		mvApp* app = mvApp::GetApp();

		for (int i = 0; i < IM_ARRAYSIZE(io.MouseDown); i++)
		{

			if (ImGui::IsMouseClicked(i))
				app->triggerCallback(app->getMouseClickCallback(), std::to_string(i));

			if (io.MouseDownDuration[i] >= 0.0f)
				app->triggerCallback(app->getMouseDownCallback(),
					std::to_string(i), std::to_string(io.MouseDownDuration[i]));

			if (ImGui::IsMouseDoubleClicked(i))
				app->triggerCallback(app->getMouseDoubleClickCallback(), std::to_string(i));

			if (ImGui::IsMouseReleased(i))
				app->triggerCallback(app->getMouseReleaseCallback(), std::to_string(i));
		}

		for (int i = 0; i < IM_ARRAYSIZE(io.KeysDown); i++)
		{
			if (ImGui::IsKeyPressed(i))
				app->triggerCallback(app->getKeyPressCallback(), std::to_string(i));

			if (io.KeysDownDuration[i] >= 0.0f)
				app->triggerCallback(app->getKeyDownCallback(), std::to_string(i), std::to_string(io.KeysDownDuration[i]));

			if (ImGui::IsKeyReleased(i))
				app->triggerCallback(app->getKeyReleaseCallback(), std::to_string(i));
		}
	}

	static bool canItemTypeHaveDuplicates(mvAppItem* item)
	{

		// ugly and quick
		switch (item->getType())
		{
		case mvAppItemType::Spacing: return true;
		case mvAppItemType::SameLine: return true;
		case mvAppItemType::Separator: return true;
		case mvAppItemType::Indent: return true;
		case mvAppItemType::Unindent: return true;
		default: return false;
		}
	}

	void mvApp::showMetricsWindow()
	{
		if (!ImGui::Begin("MarvelSandbox Metrics", &m_showMetrics, ImGuiWindowFlags_NoSavedSettings))
		{
			ImGui::End();
			return;
		}

		ImGuiIO& io = ImGui::GetIO();
		ImGui::Text("MarvelSandbox %s", mvApp::getVersion());
		ImGui::Text("Dear ImGui %s", ImGui::GetVersion());
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
		ImGui::Text("%d vertices, %d indices (%d triangles)", io.MetricsRenderVertices, io.MetricsRenderIndices, io.MetricsRenderIndices / 3);
		ImGui::Text("%d active windows (%d visible)", io.MetricsActiveWindows, io.MetricsRenderWindows);
		ImGui::Text("%d active allocations", io.MetricsActiveAllocations);
		ImGui::End();
	}

	void mvApp::showAboutWindow()
	{
		if (!ImGui::Begin("About MarvelSandbox", &m_showAbout, ImGuiWindowFlags_AlwaysAutoResize))
		{
			ImGui::End();
			return;
		}

		ImGui::Text("MarvelSandbox %s", mvApp::getVersion());
		ImGui::Text("Dear ImGui %s", ImGui::GetVersion());
		ImGui::Separator();
		ImGui::Text("By Jonathan Hoffstadt and Preston Cothren");
		ImGui::Text("MarvelSandbox is licensed under the MIT License, see LICENSE for more information.");
		ImGui::Separator();
		ImGui::Text("By Omar Cornut and all Dear ImGui contributors.");
		ImGui::Text("Dear ImGui is licensed under the MIT License, see LICENSE for more information.");

		static bool show_config_info = false;
		ImGui::Checkbox("Config/Build Information", &show_config_info);
		if (show_config_info)
		{
			ImGuiIO& io = ImGui::GetIO();
			ImGuiStyle& style = ImGui::GetStyle();

			bool copy_to_clipboard = ImGui::Button("Copy to clipboard");
			ImGui::BeginChildFrame(ImGui::GetID("cfginfos"), ImVec2(0, ImGui::GetTextLineHeightWithSpacing() * 18), ImGuiWindowFlags_NoMove);
			if (copy_to_clipboard)
			{
				ImGui::LogToClipboard();
				ImGui::LogText("```\n"); // Back quotes will make the text appears without formatting when pasting to GitHub
			}

			ImGui::Text("Dear ImGui %s (%d)", IMGUI_VERSION, IMGUI_VERSION_NUM);
			ImGui::Separator();
			ImGui::Text("sizeof(size_t): %d, sizeof(ImDrawIdx): %d, sizeof(ImDrawVert): %d", (int)sizeof(size_t), (int)sizeof(ImDrawIdx), (int)sizeof(ImDrawVert));
			ImGui::Text("define: __cplusplus=%d", (int)__cplusplus);
			ImGui::Text("define: _WIN32");
			ImGui::Text("define: _WIN64");
			ImGui::Text("define: _MSC_VER=%d", _MSC_VER);
			ImGui::Separator();
			ImGui::Text("io.BackendPlatformName: %s", io.BackendPlatformName ? io.BackendPlatformName : "NULL");
			ImGui::Text("io.BackendRendererName: %s", io.BackendRendererName ? io.BackendRendererName : "NULL");
			ImGui::Text("io.ConfigFlags: 0x%08X", io.ConfigFlags);
			if (io.ConfigFlags & ImGuiConfigFlags_NavEnableKeyboard)        ImGui::Text(" NavEnableKeyboard");
			if (io.ConfigFlags & ImGuiConfigFlags_NavEnableGamepad)         ImGui::Text(" NavEnableGamepad");
			if (io.ConfigFlags & ImGuiConfigFlags_NavEnableSetMousePos)     ImGui::Text(" NavEnableSetMousePos");
			if (io.ConfigFlags & ImGuiConfigFlags_NavNoCaptureKeyboard)     ImGui::Text(" NavNoCaptureKeyboard");
			if (io.ConfigFlags & ImGuiConfigFlags_NoMouse)                  ImGui::Text(" NoMouse");
			if (io.ConfigFlags & ImGuiConfigFlags_NoMouseCursorChange)      ImGui::Text(" NoMouseCursorChange");
			if (io.MouseDrawCursor)                                         ImGui::Text("io.MouseDrawCursor");
			if (io.ConfigMacOSXBehaviors)                                   ImGui::Text("io.ConfigMacOSXBehaviors");
			if (io.ConfigInputTextCursorBlink)                              ImGui::Text("io.ConfigInputTextCursorBlink");
			if (io.ConfigWindowsResizeFromEdges)                            ImGui::Text("io.ConfigWindowsResizeFromEdges");
			if (io.ConfigWindowsMoveFromTitleBarOnly)                       ImGui::Text("io.ConfigWindowsMoveFromTitleBarOnly");
			if (io.ConfigWindowsMemoryCompactTimer >= 0.0f)                 ImGui::Text("io.ConfigWindowsMemoryCompactTimer = %.1ff", io.ConfigWindowsMemoryCompactTimer);
			ImGui::Text("io.BackendFlags: 0x%08X", io.BackendFlags);
			if (io.BackendFlags & ImGuiBackendFlags_HasGamepad)             ImGui::Text(" HasGamepad");
			if (io.BackendFlags & ImGuiBackendFlags_HasMouseCursors)        ImGui::Text(" HasMouseCursors");
			if (io.BackendFlags & ImGuiBackendFlags_HasSetMousePos)         ImGui::Text(" HasSetMousePos");
			if (io.BackendFlags & ImGuiBackendFlags_RendererHasVtxOffset)   ImGui::Text(" RendererHasVtxOffset");
			ImGui::Separator();
			ImGui::Text("io.Fonts: %d fonts, Flags: 0x%08X, TexSize: %d,%d", io.Fonts->Fonts.Size, io.Fonts->Flags, io.Fonts->TexWidth, io.Fonts->TexHeight);
			ImGui::Text("io.DisplaySize: %.2f,%.2f", io.DisplaySize.x, io.DisplaySize.y);
			ImGui::Text("io.DisplayFramebufferScale: %.2f,%.2f", io.DisplayFramebufferScale.x, io.DisplayFramebufferScale.y);
			ImGui::Separator();
			ImGui::Text("style.WindowPadding: %.2f,%.2f", style.WindowPadding.x, style.WindowPadding.y);
			ImGui::Text("style.WindowBorderSize: %.2f", style.WindowBorderSize);
			ImGui::Text("style.FramePadding: %.2f,%.2f", style.FramePadding.x, style.FramePadding.y);
			ImGui::Text("style.FrameRounding: %.2f", style.FrameRounding);
			ImGui::Text("style.FrameBorderSize: %.2f", style.FrameBorderSize);
			ImGui::Text("style.ItemSpacing: %.2f,%.2f", style.ItemSpacing.x, style.ItemSpacing.y);
			ImGui::Text("style.ItemInnerSpacing: %.2f,%.2f", style.ItemInnerSpacing.x, style.ItemInnerSpacing.y);

			if (copy_to_clipboard)
			{
				ImGui::LogText("\n```\n");
				ImGui::LogFinish();
			}
			ImGui::EndChildFrame();
		}
		ImGui::End();
	}

	mvApp* mvApp::GetApp()
	{
		if (s_instance)
			return s_instance;

		s_instance = new mvApp();
		return s_instance;
	}

	void mvApp::render()
	{
		// set imgui style to mvstyle
		ImGuiStyle& style = ImGui::GetStyle();
		SetStyle(style, m_style);

		// update mouse
		ImVec2 mousePos = ImGui::GetMousePos();
		m_mousePos.x = mousePos.x;
		m_mousePos.y = mousePos.y;

		prepareStandardCallbacks();
			
		ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f));
		ImGui::SetNextWindowSize(ImVec2(m_width, m_height));
		ImGui::Begin("Main Window", (bool*)0, m_windowflags);

		m_parents.push(nullptr);

		// main callback
		triggerCallback(m_callback, "Main Application");

		// standard windows
		if(m_showMetrics)
			showMetricsWindow();
		if(m_showAbout)
			showAboutWindow();

		for (mvAppItem* item : m_items)
		{

			if (item->getParent())
				continue;

			// skip item if it's not shown
			if (!item->isShown())
				continue;

			// set item width
			if(item->getWidth() > 0)
				ImGui::SetNextItemWidth((float)item->getWidth());

			item->draw();

			// Context Menu
			if (item->getPopup() != "")
				ImGui::OpenPopupOnItemClick(item->getPopup().c_str(), getPopupButton(item->getPopup()));

			// Regular Tooltip (simple)
			if (item->getTip() != "" && ImGui::IsItemHovered())
				ImGui::SetTooltip(item->getTip().c_str());

			//item->setHovered(ImGui::IsItemHovered());
			//item->setActive(ImGui::IsItemActive());
			//item->setFocused(ImGui::IsItemFocused());
			//item->setClicked(ImGui::IsItemClicked());
			//item->setVisible(ImGui::IsItemVisible());
			//item->setEdited(ImGui::IsItemEdited());
			//item->setActivated(ImGui::IsItemActivated());
			//item->setDeactivated(ImGui::IsItemDeactivated());
			//item->setDeactivatedAfterEdit(ImGui::IsItemDeactivatedAfterEdit());
			//item->setToggledOpen(ImGui::IsItemToggledOpen());
			//item->setRectMin({ ImGui::GetItemRectMin().x, ImGui::GetItemRectMin().y});
			//item->setRectMax({ ImGui::GetItemRectMax().x, ImGui::GetItemRectMax().y });
			//item->setRectSize({ ImGui::GetItemRectSize().x, ImGui::GetItemRectSize().y });
		}

	}

	bool mvApp::isMouseButtonPressed(int button) const
	{
		ImGuiIO& io = ImGui::GetIO();
		return io.MouseDown[button];
	}

	bool mvApp::isKeyPressed(int keycode) const
	{
		ImGuiIO& io = ImGui::GetIO();
		return io.KeysDown[keycode];
	}

	void mvApp::pushParent(mvAppItem* item)
	{
		m_parents.push(item);
	}

	mvAppItem* mvApp::popParent()
	{
		mvAppItem* item = m_parents.top();
		m_parents.pop();
		return item;
	}

	mvAppItem* mvApp::topParent()
	{
		if(m_parents.size() != 0)
			return m_parents.top();
		return nullptr;
	}

	void mvApp::setItemCallback(const std::string& name, const std::string& callback)
	{
		auto item = getItem(name);
		if (item)
			item->setCallback(callback);
	}

	void mvApp::setItemPopup(const std::string& name, const std::string& popup)
	{
		auto item = getItem(name);
		if (item)
			item->setPopup(popup);
	}

	void mvApp::setItemTip(const std::string& name, const std::string& tip)
	{
		auto item = getItem(name);
		if (item)
			item->setTip(tip);
	}

	void mvApp::setItemWidth(const std::string& name, int width)
	{
		auto item = getItem(name);
		if (item)
			item->setWidth(width);
	}

	mvAppItem* mvApp::getItem(const std::string& name)
	{
		for (mvAppItem* item : m_items)
		{
			if (item->getName() == name)
				return item;
		}

		return nullptr;
	}

	int mvApp::getPopupButton(const std::string& name)
	{
		if (mvAppItem* item = getItem(name))
			return static_cast<mvPopup*>(item)->getButton();
		return -1;
	}

	bool mvApp::isItemHovered(const std::string& item)
	{
		if (mvAppItem* pitem = getItem(item))
			return pitem->isItemHovered();
		return false;
	}

	bool mvApp::isItemActive(const std::string& item)
	{
		if (mvAppItem* pitem = getItem(item))
			return pitem->isItemActive();
		return false;
	}

	bool mvApp::isItemFocused(const std::string& item)
	{
		if (mvAppItem* pitem = getItem(item))
			return pitem->isItemFocused();
		return false;
	}

	bool mvApp::isItemClicked(const std::string& item)
	{
		if (mvAppItem* pitem = getItem(item))
			return pitem->isItemClicked();
		return false;
	}
	
	bool mvApp::isItemVisible(const std::string& item)
	{
		if (mvAppItem* pitem = getItem(item))
			return pitem->isItemVisible();
		return false;
	}
	
	bool mvApp::isItemEdited(const std::string& item)
	{
		if (mvAppItem* pitem = getItem(item))
			return pitem->isItemEdited();
		return false;
	}
	
	bool mvApp::isItemActivated(const std::string& item)
	{
		if (mvAppItem* pitem = getItem(item))
			return pitem->isItemActivated();
		return false;
	}
	
	bool mvApp::isItemDeactivated(const std::string& item)
	{
		if (mvAppItem* pitem = getItem(item))
			return pitem->isItemDeactivated();
		return false;
	}
	
	bool mvApp::isItemDeactivatedAfterEdit(const std::string& item)
	{
		if (mvAppItem* pitem = getItem(item))
			return pitem->isItemDeactivatedAfterEdit();
		return false;
	}
	
	bool mvApp::isItemToogledOpen(const std::string& item)
	{
		if (mvAppItem* pitem = getItem(item))
			return pitem->isItemToogledOpen();
		return false;
	}
	
	mvVec2 mvApp::getItemRectMin(const std::string& item)
	{
		if (mvAppItem* pitem = getItem(item))
			return pitem->getItemRectMin();
		return {0.0f, 0.0f};
	}
	
	mvVec2 mvApp::getItemRectMax(const std::string& item)
	{
		if (mvAppItem* pitem = getItem(item))
			return pitem->getItemRectMax();
		return { 0.0f, 0.0f };
	}
	
	mvVec2 mvApp::getItemRectSize(const std::string& item)
	{
		if (mvAppItem* pitem = getItem(item))
			return pitem->getItemRectSize();
		return { 0.0f, 0.0f };
	}

	void mvApp::triggerCallback(const std::string& name, const std::string& sender)
	{
		if (name == "")
			return;



		PyObject* pHandler = PyDict_GetItemString(m_pDict, name.c_str()); // borrowed reference

		// if callback doesn't exist
		if (pHandler == NULL)
		{
			std::string message(" Callback doesn't exist");
			LogWarning(name + message);
			return;
		}



		// check if handler is callable
		if (PyCallable_Check(pHandler))
		{

			PyErr_Clear();

			PyObject* pArgs = PyTuple_New(1);
			PyTuple_SetItem(pArgs, 0, PyUnicode_FromString(sender.c_str()));

			PyObject* result = PyObject_CallObject(pHandler, pArgs);
			
			// check if call succeded
			if (!result)
			{
				std::string message("Callback failed");
				LogError(name + message);
			}

			Py_XDECREF(pArgs);
			Py_XDECREF(result);

			// check if error occurred
			if(PyErr_Occurred())
				PyErr_Print();

		}

		else
		{
			std::string message(" Callback not callable");
			LogError(name + message);
		}

	}

	void mvApp::triggerCallback(const std::string& name, const std::string& sender, const std::string& data)
	{

		if (name == "")
			return;

		PyErr_Clear();

		PyObject* pHandler = PyDict_GetItemString(m_pDict, name.c_str()); // borrowed reference

		// if callback doesn't exist
		if (pHandler == NULL)
		{
			std::string message(" Callback doesn't exist");
			LogWarning(name + message);
			return;
		}

		// check if handler is callable
		if (PyCallable_Check(pHandler))
		{
			PyObject* pArgs = PyTuple_New(2);
			PyTuple_SetItem(pArgs, 0, PyUnicode_FromString(sender.c_str()));
			PyTuple_SetItem(pArgs, 1, PyUnicode_FromString(data.c_str()));

			PyObject* result = PyObject_CallObject(pHandler, pArgs);

			// check if call succeded
			if (!result)
			{
				std::string message("Callback failed");
				LogError(name + message);
			}

			Py_XDECREF(pArgs);
			Py_XDECREF(result);

			// check if error occurred
			if (PyErr_Occurred())
				PyErr_Print();

		}

		else
		{
			std::string message(" Callback not callable");
			LogError(name + message);
		}

	}

	void mvApp::setAppTheme(const std::string& theme)
	{
		if (theme == "dark")
			ImGui::StyleColorsDark();
		else if (theme == "classic")
			ImGui::StyleColorsClassic();
		else if (theme == "light")
			ImGui::StyleColorsLight();
		else
			ImGui::StyleColorsDark();
	}

	void mvApp::changeThemeItem(long item, mvColor color)
	{
		ImGuiStyle* style = &ImGui::GetStyle();
		style->Colors[item] = ImVec4(color.r, color.g, color.b, color.a);
	}

	void mvApp::changeStyleItem(long item, float x, float y)
	{

		if (m_style.count(item) > 0)
		{
			m_style.at(item).x = x;
			m_style.at(item).y = y;
		}

	}

	void mvApp::addItemManual(mvAppItem* item)
	{
		if (m_started)
		{
			LogWarning("Items can't be added during runtime.");
			return;
		}

		if (!canItemTypeHaveDuplicates(item))
		{
			if (auto otheritem = getItem(item->getName()))
			{
				std::string message = item->getName() + " " + std::to_string(m_items.size());
				LogWarning(message + ": Items of this type must have unique names");
				return;
			}
		}

		m_items.push_back(item);
	}

	void mvApp::addItem(mvAppItem* item)
	{
		if (m_started)
		{
			LogWarning("Items can't be added during runtime.");
			return;
		}

		if (!canItemTypeHaveDuplicates(item))
		{
			if (auto otheritem = getItem(item->getName()))
			{
				std::string message = item->getName() + " " + std::to_string(m_items.size());
				LogWarning(message + ": Items of this type must have unique names");
				return;
			}
		}

		mvAppItem* parentitem = topParent();	
		item->setParent(parentitem);
		m_items.push_back(item);
	}

	void mvApp::closePopup()
	{
		if (!m_started)
		{
			LogWarning("Popups can't be closed until runtime");
			return;
		}

		ImGui::CloseCurrentPopup();
	}

	void mvApp::Log(const std::string& text, const std::string& level)
	{
		if (m_loglevel < 1)
			AppLog::getLogger()->AddLog("[%05d] [%1s]  %2s\n", ImGui::GetFrameCount(), level.c_str(), text.c_str());
	}

	void mvApp::LogDebug(const std::string& text)
	{
		if (m_loglevel < 2)
			AppLog::getLogger()->AddLog("[%05d] [DEBUG]  %1s\n", ImGui::GetFrameCount(), text.c_str());
	}

	void mvApp::LogInfo(const std::string& text)
	{
		if (m_loglevel < 3)
			AppLog::getLogger()->AddLog("[%05d] [INFO]  %1s\n", ImGui::GetFrameCount(), text.c_str());
	}

	void mvApp::LogWarning(const std::string& text)
	{
		if (m_loglevel < 4)
			AppLog::getLogger()->AddLog("[%05d] [WARNING]  %1s\n", ImGui::GetFrameCount(), text.c_str());
	}

	void mvApp::LogError(const std::string& text)
	{
		if (m_loglevel < 5)
			AppLog::getLogger()->AddLog("[%05d] [ERROR]  %1s\n", ImGui::GetFrameCount(), text.c_str());
	}

	void mvApp::ClearLog()
	{
		AppLog::getLogger()->Clear();
	}

}