#pragma once

#include "Core/AppItems/mvTypeBases.h"

//-----------------------------------------------------------------------------
// Widget Index
//
//     * mvColorEdit3
//     * mvColorEdit4
//     * mvColorPicker3
//     * mvColorPicker4
//
//-----------------------------------------------------------------------------

namespace Marvel {

	//-----------------------------------------------------------------------------
	// mvColorButton
	//-----------------------------------------------------------------------------
	class mvColorButton : public mvAppItem
	{

	public:

		MV_APPITEM_TYPE(mvAppItemType::ColorButton, "add_color_button")

		mvColorButton(const std::string& name, const mvColor& color)
			: mvAppItem(name), m_color(color.toVec4())
		{
		}

		void draw() override
		{

			if (ImGui::ColorButton(m_label.c_str(), m_color, m_flags, ImVec2((float)m_width, (float)m_height)))
			{

				mvApp::GetApp()->runCallback(m_callback, m_name, m_callbackData);

				// Context Menu
				if (!getPopup().empty())
					ImGui::OpenPopup(getPopup().c_str());
			}

			// Regular Tooltip (simple)
			if (!getTip().empty() && ImGui::IsItemHovered())
				ImGui::SetTooltip("%s", getTip().c_str());
		}

		void setExtraConfigDict(PyObject* dict) override
		{
			if (dict == nullptr)
				return;
			mvGlobalIntepreterLock gil;


			if (PyObject* item = PyDict_GetItemString(dict, "color")) m_color = ToColor(item).toVec4();

			// helpers for bit flipping
			auto flagop = [dict](const char* keyword, int flag, int& flags)
			{
				if (PyObject* item = PyDict_GetItemString(dict, keyword)) ToBool(item) ? flags |= flag : flags &= ~flag;
			};
			flagop("no_alpha", ImGuiColorEditFlags_NoAlpha, m_flags);
			flagop("no_border", ImGuiColorEditFlags_NoBorder, m_flags);
		}

		void getExtraConfigDict(PyObject* dict) override
		{
			if (dict == nullptr)
				return;
			mvGlobalIntepreterLock gil;
			PyDict_SetItemString(dict, "color", ToPyColor({(int)m_color.x*255, (int)m_color.y * 255, (int)m_color.z * 255, (int)m_color.w * 255 }));

			// helper to check and set bit
			auto checkbitset = [dict](const char* keyword, int flag, const int& flags)
			{
				PyDict_SetItemString(dict, keyword, ToPyBool(flags & flag));
			};

			checkbitset("no_alpha", ImGuiColorEditFlags_NoAlpha, m_flags);
			checkbitset("no_border", ImGuiColorEditFlags_NoBorder, m_flags);
		}

	private:
		ImGuiColorEditFlags m_flags = ImGuiColorEditFlags_None;
		ImVec4 m_color;

	};

	//-----------------------------------------------------------------------------
	// mvColorEdit3
	//-----------------------------------------------------------------------------
	class mvColorEdit3 : public mvFloat3PtrBase
	{

	public:

		MV_APPITEM_TYPE(mvAppItemType::ColorEdit3, "add_color_edit3")

		mvColorEdit3(const std::string& name, float* color, const std::string& dataSource)
			: mvFloat3PtrBase(name, color, dataSource)
		{}

		void draw() override
		{

			if (ImGui::ColorEdit3(m_label.c_str(), m_value, m_flags))
			{

				mvApp::GetApp()->runCallback(m_callback, m_name, m_callbackData);

				// Context Menu
				if (!getPopup().empty())
					ImGui::OpenPopup(getPopup().c_str());
			}

			// Regular Tooltip (simple)
			if (!getTip().empty() && ImGui::IsItemHovered())
				ImGui::SetTooltip("%s", getTip().c_str());
		}

		void setExtraConfigDict(PyObject* dict) override
		{
			if (dict == nullptr)
				return;
			mvGlobalIntepreterLock gil;

			// helpers for bit flipping
			auto flagop = [dict](const char* keyword, int flag, int& flags)
			{
				if (PyObject* item = PyDict_GetItemString(dict, keyword)) ToBool(item) ? flags |= flag : flags &= ~flag;
			};

			auto conflictingflagop = [dict](const std::vector<std::string>& keywords, std::vector<int> flags, int& mflags)
			{

				for (int i = 0; i < keywords.size(); i++)
				{
					if (PyObject* item = PyDict_GetItemString(dict, keywords[i].c_str()))
					{
						//turning all conflicting flags false
						for (const auto& flag : flags) mflags &= ~flag;
						//writing only the first conflicting flag
						ToBool(item) ? mflags |= flags[i] : mflags &= ~flags[i];
						break;
					}
				}

			};

			flagop("no_alpha", ImGuiColorEditFlags_NoAlpha, m_flags);
			flagop("no_picker", ImGuiColorEditFlags_NoPicker, m_flags);
			flagop("no_options", ImGuiColorEditFlags_NoOptions, m_flags);
			flagop("no_small_preview", ImGuiColorEditFlags_NoSmallPreview, m_flags);
			flagop("no_inputs", ImGuiColorEditFlags_NoInputs, m_flags);
			flagop("no_tooltip", ImGuiColorEditFlags_NoTooltip, m_flags);
			flagop("no_label", ImGuiColorEditFlags_NoLabel, m_flags);
			flagop("no_side_preview", ImGuiColorEditFlags_NoSidePreview, m_flags);
			flagop("no_drag_drop", ImGuiColorEditFlags_NoDragDrop, m_flags);
			flagop("alpha_bar", ImGuiColorEditFlags_AlphaBar, m_flags);
			flagop("display_rgb", ImGuiColorEditFlags_DisplayRGB, m_flags);
			flagop("display_hsv", ImGuiColorEditFlags_DisplayHSV, m_flags);
			flagop("display_hex", ImGuiColorEditFlags_DisplayHex, m_flags);

			static std::vector<std::string> AlphaPreviewKeywords{ "alpha_preview", "alpha_preview_half" };
			static std::vector<int> AlphaPreviewFlags{ ImGuiColorEditFlags_AlphaPreview, ImGuiColorEditFlags_AlphaPreviewHalf };

			static std::vector<std::string> DisplayValueTypeKeywords{ "uint8", "floats" };
			static std::vector<int> DisplayValueTypeFlags{ ImGuiColorEditFlags_Uint8, ImGuiColorEditFlags_Float };


			static std::vector<std::string> IOTypeKeywords{ "input_rgb", "input_hsv" };
			static std::vector<int> IOTypeFlags{ ImGuiColorEditFlags_InputRGB, ImGuiColorEditFlags_InputHSV };

			conflictingflagop(AlphaPreviewKeywords, AlphaPreviewFlags, m_flags);
			conflictingflagop(DisplayValueTypeKeywords, DisplayValueTypeFlags, m_flags);
			conflictingflagop(IOTypeKeywords, IOTypeFlags, m_flags);

		}

		void getExtraConfigDict(PyObject* dict) override
		{
			if (dict == nullptr)
				return;
			mvGlobalIntepreterLock gil;

			// helper to check and set bit
			auto checkbitset = [dict](const char* keyword, int flag, const int& flags)
			{
				PyDict_SetItemString(dict, keyword, ToPyBool(flags & flag));
			};

			checkbitset("no_alpha", ImGuiColorEditFlags_NoAlpha, m_flags);
			checkbitset("no_picker", ImGuiColorEditFlags_NoPicker, m_flags);
			checkbitset("no_options", ImGuiColorEditFlags_NoOptions, m_flags);
			checkbitset("no_small_preview", ImGuiColorEditFlags_NoSmallPreview, m_flags);
			checkbitset("no_inputs", ImGuiColorEditFlags_NoInputs, m_flags);
			checkbitset("no_tooltip", ImGuiColorEditFlags_NoTooltip, m_flags);
			checkbitset("no_label", ImGuiColorEditFlags_NoLabel, m_flags);
			checkbitset("no_drag_drop", ImGuiColorEditFlags_NoDragDrop, m_flags);
			checkbitset("alpha_bar", ImGuiColorEditFlags_AlphaBar, m_flags);
			checkbitset("alpha_preview", ImGuiColorEditFlags_AlphaPreview, m_flags);
			checkbitset("alpha_preview_half", ImGuiColorEditFlags_AlphaPreviewHalf, m_flags);
			checkbitset("display_rgb", ImGuiColorEditFlags_DisplayRGB, m_flags);
			checkbitset("display_hsv", ImGuiColorEditFlags_DisplayHSV, m_flags);
			checkbitset("display_hex", ImGuiColorEditFlags_DisplayHex, m_flags);
			checkbitset("unit8", ImGuiColorEditFlags_Uint8, m_flags);
			checkbitset("floats", ImGuiColorEditFlags_Float, m_flags);
			checkbitset("input_rgb", ImGuiColorEditFlags_InputRGB, m_flags);
			checkbitset("input_hsv", ImGuiColorEditFlags_InputHSV, m_flags);
		}
	private:
		ImGuiColorEditFlags m_flags = ImGuiColorEditFlags_None;

	};

	//-----------------------------------------------------------------------------
	// mvColorEdit4
	//-----------------------------------------------------------------------------
	class mvColorEdit4 : public mvFloat4PtrBase
	{

	public:

		MV_APPITEM_TYPE(mvAppItemType::ColorEdit4, "add_color_edit4")

		mvColorEdit4(const std::string& name, float* color, const std::string& dataSource)
			: mvFloat4PtrBase(name, color, dataSource)
		{}

		void draw() override
		{

			if (ImGui::ColorEdit4(m_label.c_str(), m_value, m_flags))
			{

				mvApp::GetApp()->runCallback(m_callback, m_name, m_callbackData);

				// Context Menu
				if (!getPopup().empty())
					ImGui::OpenPopup(getPopup().c_str());
			}

			// Regular Tooltip (simple)
			if (!getTip().empty() && ImGui::IsItemHovered())
				ImGui::SetTooltip("%s", getTip().c_str());
		}
		void setExtraConfigDict(PyObject* dict) override
		{
			if (dict == nullptr)
				return;
			mvGlobalIntepreterLock gil;

			// helpers for bit flipping
			auto flagop = [dict](const char* keyword, int flag, int& flags)
			{
				if (PyObject* item = PyDict_GetItemString(dict, keyword)) ToBool(item) ? flags |= flag : flags &= ~flag;
			};

			auto conflictingflagop = [dict](const std::vector<std::string>& keywords, std::vector<int> flags, int& mflags)
			{

				for (int i = 0; i < keywords.size(); i++)
				{
					if (PyObject* item = PyDict_GetItemString(dict, keywords[i].c_str()))
					{
						//turning all conflicting flags false
						for (const auto& flag : flags) mflags &= ~flag;
						//writing only the first conflicting flag
						ToBool(item) ? mflags |= flags[i] : mflags &= ~flags[i];
						break;
					}
				}

			};

			flagop("no_alpha", ImGuiColorEditFlags_NoAlpha, m_flags);
			flagop("no_picker", ImGuiColorEditFlags_NoPicker, m_flags);
			flagop("no_options", ImGuiColorEditFlags_NoOptions, m_flags);
			flagop("no_small_preview", ImGuiColorEditFlags_NoSmallPreview, m_flags);
			flagop("no_inputs", ImGuiColorEditFlags_NoInputs, m_flags);
			flagop("no_tooltip", ImGuiColorEditFlags_NoTooltip, m_flags);
			flagop("no_label", ImGuiColorEditFlags_NoLabel, m_flags);
			flagop("no_side_preview", ImGuiColorEditFlags_NoSidePreview, m_flags);
			flagop("no_drag_drop", ImGuiColorEditFlags_NoDragDrop, m_flags);
			flagop("alpha_bar", ImGuiColorEditFlags_AlphaBar, m_flags);
			flagop("display_rgb", ImGuiColorEditFlags_DisplayRGB, m_flags);
			flagop("display_hsv", ImGuiColorEditFlags_DisplayHSV, m_flags);
			flagop("display_hex", ImGuiColorEditFlags_DisplayHex, m_flags);

			static std::vector<std::string> AlphaPreviewKeywords{ "alpha_preview", "alpha_preview_half" };
			static std::vector<int> AlphaPreviewFlags{ ImGuiColorEditFlags_AlphaPreview, ImGuiColorEditFlags_AlphaPreviewHalf };

			static std::vector<std::string> DisplayValueTypeKeywords{ "uint8", "floats" };
			static std::vector<int> DisplayValueTypeFlags{ ImGuiColorEditFlags_Uint8, ImGuiColorEditFlags_Float };


			static std::vector<std::string> IOTypeKeywords{ "input_rgb", "input_hsv" };
			static std::vector<int> IOTypeFlags{ ImGuiColorEditFlags_InputRGB, ImGuiColorEditFlags_InputHSV };

			conflictingflagop(AlphaPreviewKeywords, AlphaPreviewFlags, m_flags);
			conflictingflagop(DisplayValueTypeKeywords, DisplayValueTypeFlags, m_flags);
			conflictingflagop(IOTypeKeywords, IOTypeFlags, m_flags);

		}

		void getExtraConfigDict(PyObject* dict) override
		{
			if (dict == nullptr)
				return;
			mvGlobalIntepreterLock gil;

			// helper to check and set bit
			auto checkbitset = [dict](const char* keyword, int flag, const int& flags)
			{
				PyDict_SetItemString(dict, keyword, ToPyBool(flags & flag));
			};

			checkbitset("no_alpha", ImGuiColorEditFlags_NoAlpha, m_flags);
			checkbitset("no_picker", ImGuiColorEditFlags_NoPicker, m_flags);
			checkbitset("no_options", ImGuiColorEditFlags_NoOptions, m_flags);
			checkbitset("no_small_preview", ImGuiColorEditFlags_NoSmallPreview, m_flags);
			checkbitset("no_inputs", ImGuiColorEditFlags_NoInputs, m_flags);
			checkbitset("no_tooltip", ImGuiColorEditFlags_NoTooltip, m_flags);
			checkbitset("no_label", ImGuiColorEditFlags_NoLabel, m_flags);
			checkbitset("no_drag_drop", ImGuiColorEditFlags_NoDragDrop, m_flags);
			checkbitset("alpha_bar", ImGuiColorEditFlags_AlphaBar, m_flags);
			checkbitset("alpha_preview", ImGuiColorEditFlags_AlphaPreview, m_flags);
			checkbitset("alpha_preview_half", ImGuiColorEditFlags_AlphaPreviewHalf, m_flags);
			checkbitset("display_rgb", ImGuiColorEditFlags_DisplayRGB, m_flags);
			checkbitset("display_hsv", ImGuiColorEditFlags_DisplayHSV, m_flags);
			checkbitset("display_hex", ImGuiColorEditFlags_DisplayHex, m_flags);
			checkbitset("unit8", ImGuiColorEditFlags_Uint8, m_flags);
			checkbitset("floats", ImGuiColorEditFlags_Float, m_flags);
			checkbitset("input_rgb", ImGuiColorEditFlags_InputRGB, m_flags);
			checkbitset("input_hsv", ImGuiColorEditFlags_InputHSV, m_flags);
		}
	private:
		ImGuiColorEditFlags m_flags = ImGuiColorEditFlags_None;

	};

	//-----------------------------------------------------------------------------
	// mvColorPicker3
	//-----------------------------------------------------------------------------
	class mvColorPicker3 : public mvFloat3PtrBase
	{

	public:

		MV_APPITEM_TYPE(mvAppItemType::ColorPicker3, "add_color_picker3")

		mvColorPicker3(const std::string& name, float* color, const std::string& dataSource)
			: mvFloat3PtrBase(name, color, dataSource)
		{}

		void draw() override
		{

			if (ImGui::ColorPicker3(m_label.c_str(), m_value, m_flags))
			{

				mvApp::GetApp()->runCallback(m_callback, m_name, m_callbackData);

				// Context Menu
				if (!getPopup().empty())
					ImGui::OpenPopup(getPopup().c_str());
			}

			// Regular Tooltip (simple)
			if (!getTip().empty() && ImGui::IsItemHovered())
				ImGui::SetTooltip("%s", getTip().c_str());
		}
		void setExtraConfigDict(PyObject* dict) override
		{
			if (dict == nullptr)
				return;
			mvGlobalIntepreterLock gil;

			// helpers for bit flipping
			auto flagop = [dict](const char* keyword, int flag, int& flags)
			{
				if (PyObject* item = PyDict_GetItemString(dict, keyword)) ToBool(item) ? flags |= flag : flags &= ~flag;
			};

			auto conflictingflagop = [dict](const std::vector<std::string>& keywords, std::vector<int> flags, int& mflags)
			{

				for (int i = 0; i < keywords.size(); i++)
				{
					if (PyObject* item = PyDict_GetItemString(dict, keywords[i].c_str()))
					{
						//turning all conflicting flags false
						for (const auto& flag : flags) mflags &= ~flag;
						//writing only the first conflicting flag
						ToBool(item) ? mflags |= flags[i] : mflags &= ~flags[i];
						break;
					}
				}

			};

			flagop("no_alpha", ImGuiColorEditFlags_NoAlpha, m_flags);
			flagop("no_small_preview", ImGuiColorEditFlags_NoSmallPreview, m_flags);
			flagop("no_inputs", ImGuiColorEditFlags_NoInputs, m_flags);
			flagop("no_tooltip", ImGuiColorEditFlags_NoTooltip, m_flags);
			flagop("no_label", ImGuiColorEditFlags_NoLabel, m_flags);
			flagop("no_side_preview", ImGuiColorEditFlags_NoSidePreview, m_flags);
			flagop("alpha_bar", ImGuiColorEditFlags_AlphaBar, m_flags);
			flagop("display_rgb", ImGuiColorEditFlags_DisplayRGB, m_flags);
			flagop("display_hsv", ImGuiColorEditFlags_DisplayHSV, m_flags);
			flagop("display_hex", ImGuiColorEditFlags_DisplayHex, m_flags);

			static std::vector<std::string> AlphaPreviewKeywords{ "alpha_preview", "alpha_preview_half" };
			static std::vector<int> AlphaPreviewFlags{ ImGuiColorEditFlags_AlphaPreview, ImGuiColorEditFlags_AlphaPreviewHalf };
			 
			static std::vector<std::string> DisplayValueTypeKeywords{ "uint8", "floats" };
			static std::vector<int> DisplayValueTypeFlags{ ImGuiColorEditFlags_Uint8, ImGuiColorEditFlags_Float };
			 
			static std::vector<std::string> PickerTypeKeywords{ "picker_hue_bar", "picker_hue_wheel" };
			static std::vector<int> PickerTypeFlags{ ImGuiColorEditFlags_PickerHueBar, ImGuiColorEditFlags_PickerHueWheel };
			 
			static std::vector<std::string> IOTypeKeywords{ "input_rgb", "input_hsv" };
			static std::vector<int> IOTypeFlags{ ImGuiColorEditFlags_InputRGB, ImGuiColorEditFlags_InputHSV };

			conflictingflagop(AlphaPreviewKeywords, AlphaPreviewFlags, m_flags);
			conflictingflagop(DisplayValueTypeKeywords, DisplayValueTypeFlags, m_flags);
			conflictingflagop(PickerTypeKeywords, PickerTypeFlags, m_flags);
			conflictingflagop(IOTypeKeywords, IOTypeFlags, m_flags);

		}

		void getExtraConfigDict(PyObject* dict) override
		{
			if (dict == nullptr)
				return;
			mvGlobalIntepreterLock gil;

			// helper to check and set bit
			auto checkbitset = [dict](const char* keyword, int flag, const int& flags)
			{
				PyDict_SetItemString(dict, keyword, ToPyBool(flags & flag));
			};

			checkbitset("no_alpha", ImGuiColorEditFlags_NoAlpha, m_flags);
			checkbitset("no_small_preview", ImGuiColorEditFlags_NoSmallPreview, m_flags);
			checkbitset("no_inputs", ImGuiColorEditFlags_NoInputs, m_flags);
			checkbitset("no_tooltip", ImGuiColorEditFlags_NoTooltip, m_flags);
			checkbitset("no_label", ImGuiColorEditFlags_NoLabel, m_flags);
			checkbitset("no_side_preview", ImGuiColorEditFlags_NoSidePreview, m_flags);
			checkbitset("alpha_bar", ImGuiColorEditFlags_AlphaBar, m_flags);
			checkbitset("alpha_preview", ImGuiColorEditFlags_AlphaPreview, m_flags);
			checkbitset("alpha_preview_half", ImGuiColorEditFlags_AlphaPreviewHalf, m_flags);
			checkbitset("display_rgb", ImGuiColorEditFlags_DisplayRGB, m_flags);
			checkbitset("display_hsv", ImGuiColorEditFlags_DisplayHSV, m_flags);
			checkbitset("display_hex", ImGuiColorEditFlags_DisplayHex, m_flags);
			checkbitset("unit8", ImGuiColorEditFlags_Uint8, m_flags);
			checkbitset("floats", ImGuiColorEditFlags_Float, m_flags);
			checkbitset("picker_hue_bar", ImGuiColorEditFlags_PickerHueBar, m_flags);
			checkbitset("picker_hue_wheel", ImGuiColorEditFlags_PickerHueWheel, m_flags);
			checkbitset("input_rgb", ImGuiColorEditFlags_InputRGB, m_flags);
			checkbitset("input_hsv", ImGuiColorEditFlags_InputHSV, m_flags);
		}
	private:
		ImGuiColorEditFlags m_flags = ImGuiColorEditFlags_None;

	};

	//-----------------------------------------------------------------------------
	// mvColorPicker4
	//-----------------------------------------------------------------------------
	class mvColorPicker4 : public mvFloat4PtrBase
	{

	public:

		MV_APPITEM_TYPE(mvAppItemType::ColorPicker4, "add_color_picker4")

		mvColorPicker4(const std::string& name, float* color, const std::string& dataSource)
			: mvFloat4PtrBase(name, color, dataSource)
		{}

		void draw() override
		{

			if (ImGui::ColorPicker4(m_label.c_str(), m_value, m_flags))
			{

				mvApp::GetApp()->runCallback(m_callback, m_name, m_callbackData);

				// Context Menu
				if (!getPopup().empty())
					ImGui::OpenPopup(getPopup().c_str());
			}

			// Regular Tooltip (simple)
			if (!getTip().empty() && ImGui::IsItemHovered())
				ImGui::SetTooltip("%s", getTip().c_str());
		}
		void setExtraConfigDict(PyObject* dict) override
		{
			if (dict == nullptr)
				return;
			mvGlobalIntepreterLock gil;

			// helpers for bit flipping
			auto flagop = [dict](const char* keyword, int flag, int& flags)
			{
				if (PyObject* item = PyDict_GetItemString(dict, keyword)) ToBool(item) ? flags |= flag : flags &= ~flag;
			};

			auto conflictingflagop = [dict](const std::vector<std::string>& keywords, std::vector<int> flags, int& mflags)
			{

				for (int i = 0; i < keywords.size(); i++)
				{
					if (PyObject* item = PyDict_GetItemString(dict, keywords[i].c_str()))
					{
						//turning all conflicting flags false
						for (const auto& flag : flags) mflags &= ~flag;
						//writing only the first conflicting flag
						ToBool(item) ? mflags |= flags[i] : mflags &= ~flags[i];
						break;
					}
				}

			};

			flagop("no_alpha", ImGuiColorEditFlags_NoAlpha, m_flags);
			flagop("no_small_preview", ImGuiColorEditFlags_NoSmallPreview, m_flags);
			flagop("no_inputs", ImGuiColorEditFlags_NoInputs, m_flags);
			flagop("no_tooltip", ImGuiColorEditFlags_NoTooltip, m_flags);
			flagop("no_label", ImGuiColorEditFlags_NoLabel, m_flags);
			flagop("no_side_preview", ImGuiColorEditFlags_NoSidePreview, m_flags);
			flagop("alpha_bar", ImGuiColorEditFlags_AlphaBar, m_flags);
			flagop("display_rgb", ImGuiColorEditFlags_DisplayRGB, m_flags);
			flagop("display_hsv", ImGuiColorEditFlags_DisplayHSV, m_flags);
			flagop("display_hex", ImGuiColorEditFlags_DisplayHex, m_flags);

			static std::vector<std::string> AlphaPreviewKeywords{ "alpha_preview", "alpha_preview_half" };
			static std::vector<int> AlphaPreviewFlags{ ImGuiColorEditFlags_AlphaPreview, ImGuiColorEditFlags_AlphaPreviewHalf };
			 
			static std::vector<std::string> DisplayValueTypeKeywords{"uint8", "floats"};
			static std::vector<int> DisplayValueTypeFlags{ImGuiColorEditFlags_Uint8, ImGuiColorEditFlags_Float};
			 
			static std::vector<std::string> PickerTypeKeywords{"picker_hue_bar", "picker_hue_wheel"};
			static std::vector<int> PickerTypeFlags{ImGuiColorEditFlags_PickerHueBar, ImGuiColorEditFlags_PickerHueWheel};
			 
			static std::vector<std::string> IOTypeKeywords{"input_rgb", "input_hsv"};
			static std::vector<int> IOTypeFlags{ImGuiColorEditFlags_InputRGB, ImGuiColorEditFlags_InputHSV};

			conflictingflagop(AlphaPreviewKeywords, AlphaPreviewFlags, m_flags);
			conflictingflagop(DisplayValueTypeKeywords, DisplayValueTypeFlags, m_flags);
			conflictingflagop(PickerTypeKeywords, PickerTypeFlags, m_flags);
			conflictingflagop(IOTypeKeywords, IOTypeFlags, m_flags);

		}

		void getExtraConfigDict(PyObject* dict) override
		{
			if (dict == nullptr)
				return;
			mvGlobalIntepreterLock gil;

			// helper to check and set bit
			auto checkbitset = [dict](const char* keyword, int flag, const int& flags)
			{
				PyDict_SetItemString(dict, keyword, ToPyBool(flags & flag));
			};

			checkbitset("no_alpha", ImGuiColorEditFlags_NoAlpha, m_flags);
			checkbitset("no_small_preview", ImGuiColorEditFlags_NoSmallPreview, m_flags);
			checkbitset("no_inputs", ImGuiColorEditFlags_NoInputs, m_flags);
			checkbitset("no_tooltip", ImGuiColorEditFlags_NoTooltip, m_flags);
			checkbitset("no_label", ImGuiColorEditFlags_NoLabel, m_flags);
			checkbitset("no_side_preview", ImGuiColorEditFlags_NoSidePreview, m_flags);
			checkbitset("alpha_bar", ImGuiColorEditFlags_AlphaBar, m_flags);
			checkbitset("alpha_preview", ImGuiColorEditFlags_AlphaPreview, m_flags);
			checkbitset("alpha_preview_half", ImGuiColorEditFlags_AlphaPreviewHalf, m_flags);
			checkbitset("display_rgb", ImGuiColorEditFlags_DisplayRGB, m_flags);
			checkbitset("display_hsv", ImGuiColorEditFlags_DisplayHSV, m_flags);
			checkbitset("display_hex", ImGuiColorEditFlags_DisplayHex, m_flags);
			checkbitset("unit8", ImGuiColorEditFlags_Uint8, m_flags);
			checkbitset("floats", ImGuiColorEditFlags_Float, m_flags);
			checkbitset("picker_hue_bar", ImGuiColorEditFlags_PickerHueBar, m_flags);
			checkbitset("picker_hue_wheel", ImGuiColorEditFlags_PickerHueWheel, m_flags);
			checkbitset("input_rgb", ImGuiColorEditFlags_InputRGB, m_flags);
			checkbitset("input_hsv", ImGuiColorEditFlags_InputHSV, m_flags);
		}
	private:
		ImGuiColorEditFlags m_flags = ImGuiColorEditFlags_None;

	};

}