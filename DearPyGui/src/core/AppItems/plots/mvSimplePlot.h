#pragma once

#include <utility>
#include "mvTypeBases.h"

namespace Marvel{

	class mvSimplePlot : public mvFloatVectPtrBase
	{

		MV_APPITEM_TYPE(mvAppItemType::SimplePlot, "add_simple_plot")

	public:
		mvSimplePlot(const std::string& name, const std::vector<float>& value)
			: mvFloatVectPtrBase(name, value, name) 
		{

			m_description.ignoreSizeUpdate = true;

			if (!value.empty())
			{
				m_max = (*m_value)[0];
				m_min = (*m_value)[0];

				for (auto& item : *m_value)
				{
					if (item > m_max)
						m_max = item;
					if (item < m_min)
						m_min = item;
				}
			}
		}

		void draw() override
		{
			auto styleManager = m_styleManager.getScopedStyleManager();
			ImGui::PushID(this);
			mvImGuiThemeScope scope(this);

			if(m_histogram)
				ImGui::PlotHistogram(m_label.c_str(), m_value->data(), (int)m_value->size(), 0, m_overlay.c_str(), 
					m_min, m_max, ImVec2((float)m_core_config.width, (float)m_core_config.height));
			else
			ImGui::PlotLines(m_label.c_str(), m_value->data(), (int)m_value->size(), 0, m_overlay.c_str(),
				m_min, m_max, ImVec2((float)m_core_config.width, (float)m_core_config.height));

			ImGui::PopID();
		}

		void setValue(const std::vector<float>& value)
		{ 
			*m_value = value;
		}
		[[nodiscard]] const std::vector<float>& getValue() const { return *m_value; }

		void setExtraConfigDict(PyObject* dict) override
		{
			if (dict == nullptr)
				return;
			 
			if (PyObject* item = PyDict_GetItemString(dict, "overlay")) m_overlay = ToString(item);
			if (PyObject* item = PyDict_GetItemString(dict, "minscale")) m_min = ToFloat(item);
			if (PyObject* item = PyDict_GetItemString(dict, "maxscale")) m_max = ToFloat(item);
			if (PyObject* item = PyDict_GetItemString(dict, "histogram")) m_histogram = ToBool(item);
		}

		void getExtraConfigDict(PyObject* dict) override
		{
			if (dict == nullptr)
				return;
			 
			PyDict_SetItemString(dict, "overlay", ToPyString(m_overlay));
			PyDict_SetItemString(dict, "minscale", ToPyFloat(m_min));
			PyDict_SetItemString(dict, "maxscale", ToPyFloat(m_max));
			PyDict_SetItemString(dict, "histogram", ToPyBool(m_histogram));
		}

	private:

		//std::vector<float> m_value;
		std::string        m_overlay;
		float              m_min = 0.0f;
		float              m_max = 0.0f;
		bool               m_histogram = false;

	};

}