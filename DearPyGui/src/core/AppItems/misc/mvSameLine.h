#pragma once

#include "mvTypeBases.h"

namespace Marvel {

	class mvSameLine : public mvAppItem
	{

	public:

		MV_APPITEM_TYPE(mvAppItemType::SameLine, "add_same_line")

			mvSameLine(const std::string& name)
			: mvAppItem(name)
		{
			m_description.duplicatesAllowed = true;
		}

		void draw() override
		{
			ImGui::SameLine(m_xoffset, m_spacing);
		}

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

	private:

		float m_xoffset = 0.0f;
		float m_spacing = -1.0f;

	};

}