#include <utility>
#include "mvImageSeries.h"
#include "mvCore.h"
#include "mvApp.h"
#include "mvItemRegistry.h"
#include "mvImPlotThemeScope.h"
#include "mvTextureStorage.h"

namespace Marvel {

	void mvImageSeries::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{

		parsers->insert({ s_command, mvPythonParser({
			{mvPythonDataType::Optional},
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::String, "value"},
			{mvPythonDataType::FloatList, "bounds_min", "bottom left coordinate"},
			{mvPythonDataType::FloatList, "bounds_max", "top right coordinate"},
			{mvPythonDataType::FloatList, "uv_min", "normalized texture coordinates", "(0.0, 0.0)"},
			{mvPythonDataType::FloatList, "uv_max", "normalized texture coordinates", "(1.0, 1.0)"},
			{mvPythonDataType::IntList, "tint_color", "", "(255, 255, 255, 255)"},
			{mvPythonDataType::String, "label", "Overrides 'name' as label", "''"},
			{mvPythonDataType::String, "parent", "Parent to add this item to. (runtime adding)", "''"},
			{mvPythonDataType::String, "before", "This item will be displayed before the specified item in the parent. (runtime adding)", "''"},
			{mvPythonDataType::Bool, "show", "Attempt to render", "True"},
			{mvPythonDataType::Bool, "contribute_to_bounds", "", "True"},
		}, "Adds a drag point to a plot.", "None", "Plotting") });
	}

	mvImageSeries::mvImageSeries(const std::string& name)
		: mvSeriesBase(name)
	{
	}

	bool mvImageSeries::onEvent(mvEvent& event)
	{
		mvEventDispatcher dispatcher(event);
		dispatcher.dispatch(BIND_EVENT_METH(mvImageSeries::onTextureDeleted), mvEVT_DELETE_TEXTURE);

		return event.handled;
	}

	bool mvImageSeries::onTextureDeleted(mvEvent& event)
	{
		std::string name = GetEString(event, "NAME");

		if (name == m_imagevalue)
		{
			m_texture = nullptr;
			return true;
		}

		return false;
	}

	void mvImageSeries::draw(ImDrawList* drawlist, float x, float y)
	{
		ScopedID id;
		mvImPlotThemeScope scope(this);

		switch (m_axis)
		{
		case ImPlotYAxis_1:
			ImPlot::SetPlotYAxis(ImPlotYAxis_1);
			break;
		case ImPlotYAxis_2:
			ImPlot::SetPlotYAxis(ImPlotYAxis_2);
			break;
		case ImPlotYAxis_3:
			ImPlot::SetPlotYAxis(ImPlotYAxis_3);
			break;
		default:
			break;
		}

		if (m_texture == nullptr)
		{
			mvApp::GetApp()->getTextureStorage().addTexture(m_imagevalue);
			mvTexture* texture = mvApp::GetApp()->getTextureStorage().getTexture(m_imagevalue);
			m_texture = texture->texture;
		}
		if (m_texture)
			ImPlot::PlotImage(m_name.c_str(), m_texture, m_bounds_min, m_bounds_max, m_uv_min, m_uv_max, m_tintColor);

	}

	void mvImageSeries::setExtraConfigDict(PyObject* dict)
	{
		if (dict == nullptr)
			return;

		if (PyObject* item = PyDict_GetItemString(dict, "value")) m_imagevalue = ToString(item);
		if (PyObject* item = PyDict_GetItemString(dict, "uv_min")) m_uv_min = ToVec2(item);
		if (PyObject* item = PyDict_GetItemString(dict, "uv_max")) m_uv_max = ToVec2(item);
		if (PyObject* item = PyDict_GetItemString(dict, "tint_color")) m_tintColor = ToColor(item);
		if (PyObject* item = PyDict_GetItemString(dict, "contribute_to_bounds")) m_contributeToBounds = ToBool(item);
		if (PyObject* item = PyDict_GetItemString(dict, "bounds_min"))
		{
			auto result = ToVec2(item);
			m_bounds_min.x = (double)result.x;
			m_bounds_min.y = (double)result.y;
		}
		if (PyObject* item = PyDict_GetItemString(dict, "bounds_max"))
		{
			auto result = ToVec2(item);
			m_bounds_max.x = (double)result.x;
			m_bounds_max.y = (double)result.y;
		}

	}

	void mvImageSeries::getExtraConfigDict(PyObject* dict)
	{
		if (dict == nullptr)
			return;
	}

}