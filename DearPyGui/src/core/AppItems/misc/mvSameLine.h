#pragma once

#include "mvTypeBases.h"

namespace Marvel {

	class mvSameLine : public mvAppItem
	{

	public:

		MV_APPITEM_TYPE(mvAppItemType::SameLine, mvSameLine, "add_same_line")

		MV_START_COLOR_CONSTANTS
		MV_END_COLOR_CONSTANTS

		MV_START_STYLE_CONSTANTS
		MV_END_STYLE_CONSTANTS

		mvSameLine(const std::string& name)
			: mvAppItem(name)
		{
			m_description.duplicatesAllowed = true;
		}

		void draw() override
		{
			ImGui::SameLine(m_xoffset, m_spacing);
		}

#ifndef MV_CPP


		void setExtraConfigDict(PyObject* dict) override
		{
			if (dict == nullptr)
				return;
			 
			if (PyObject* item = PyDict_GetItemString(dict, "xoffset")) m_xoffset = ToFloat(item);
			if (PyObject* item = PyDict_GetItemString(dict, "spacing")) m_spacing = ToFloat(item);

		}

		void getExtraConfigDict(PyObject* dict) override
		{
			if (dict == nullptr)
				return;
			 
			PyDict_SetItemString(dict, "xoffset", ToPyFloat(m_xoffset));
			PyDict_SetItemString(dict, "spacing", ToPyFloat(m_spacing));
		}

#endif

	private:

		float m_xoffset = 0.0f;
		float m_spacing = -1.0f;

	};

}