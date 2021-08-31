/***************************************************************************//*/
Copyright (c) 2021 Dear PyGui, LLC

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

/******************************************************************************/

#pragma once

#include "mvItemRegistry.h"

namespace Marvel {

	MV_REGISTER_WIDGET(mvButton, MV_ITEM_DESC_DEFAULT, StorageValueTypes::None, 1);
	class mvButton : public mvAppItem
	{
	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

		MV_APPLY_WIDGET_REGISTRATION(mvAppItemType::mvButton, add_button)
		MV_NO_COMMANDS
		MV_DEFAULT_PARENTS
		MV_DEFAULT_CHILDREN

		MV_SET_STATES(
			MV_STATE_HOVER |
			MV_STATE_ACTIVE |
			MV_STATE_FOCUSED |
			MV_STATE_CLICKED |
			MV_STATE_VISIBLE |
			MV_STATE_ACTIVATED |
			MV_STATE_DEACTIVATED |
			MV_STATE_RECT_MIN |
			MV_STATE_RECT_MAX |
			MV_STATE_RECT_SIZE |
			MV_STATE_CONT_AVAIL
		);

		MV_CREATE_CONSTANT(mvTool_About, MV_TOOL_ABOUT_UUID);
		MV_CREATE_CONSTANT(mvTool_Debug, MV_TOOL_DEBUG_UUID);
		MV_CREATE_CONSTANT(mvTool_Doc, MV_TOOL_DOC_UUID);
		MV_CREATE_CONSTANT(mvTool_ItemRegistry, MV_TOOL_ITEM_REGISTRY_UUID);
		MV_CREATE_CONSTANT(mvTool_Metrics, MV_TOOL_METRICS_UUID);
		MV_CREATE_CONSTANT(mvTool_Style, MV_TOOL_STYLE_UUID);
		MV_CREATE_CONSTANT(mvTool_Font, MV_TOOL_FONT_UUID);

		MV_CREATE_CONSTANT(mvFontAtlas, MV_ATLAS_UUID);
		MV_CREATE_CONSTANT(mvAppUUID, MV_APP_UUID);
		MV_CREATE_CONSTANT(mvInvalidUUID, MV_INVALID_UUID);

		MV_CREATE_CONSTANT(mvDir_None, ImGuiDir_None);
		MV_CREATE_CONSTANT(mvDir_Left, ImGuiDir_Left);
		MV_CREATE_CONSTANT(mvDir_Right, ImGuiDir_Right);
		MV_CREATE_CONSTANT(mvDir_Up, ImGuiDir_Up);
		MV_CREATE_CONSTANT(mvDir_Down, ImGuiDir_Down);

		MV_START_CONSTANTS
			MV_ADD_CONSTANT(mvTool_About),
			MV_ADD_CONSTANT(mvTool_Debug),
			MV_ADD_CONSTANT(mvTool_Doc),
			MV_ADD_CONSTANT(mvTool_ItemRegistry),
			MV_ADD_CONSTANT(mvTool_Metrics),
			MV_ADD_CONSTANT(mvTool_Style),
			MV_ADD_CONSTANT(mvTool_Font),
			MV_ADD_CONSTANT(mvFontAtlas),
			MV_ADD_CONSTANT(mvAppUUID),
			MV_ADD_CONSTANT(mvInvalidUUID),
			MV_ADD_CONSTANT(mvDir_None),
			MV_ADD_CONSTANT(mvDir_Left),
			MV_ADD_CONSTANT(mvDir_Right),
			MV_ADD_CONSTANT(mvDir_Up),
			MV_ADD_CONSTANT(mvDir_Down),
		MV_END_CONSTANTS

	public:

		explicit mvButton(mvUUID uuid);

		void draw(ImDrawList* drawlist, float x, float y) override;
		void handleSpecificKeywordArgs(PyObject* dict) override;
		void getSpecificConfiguration(PyObject* dict) override;
		void applySpecificTemplate(mvAppItem* item) override;

	private:

		ImGuiDir _direction = ImGuiDir_Up;
		bool     _small_button = false;
		bool     _arrow = false;

	};

}
