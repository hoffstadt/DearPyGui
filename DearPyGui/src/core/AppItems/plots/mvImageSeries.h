#pragma once

#include "mvPlot.h"
#include "mvCore.h"
#include "mvEvents.h"
#include "mvTextureStorage.h"
#include "mvPythonExceptions.h"

namespace Marvel {

	class mvImageSeries : public mvSeries, public mvEventHandler
	{

	public:

		mvImageSeries(const std::string& name, const std::string& value, const ImPlotPoint& boundsMin, const ImPlotPoint& boundsMax,
			mvVec2 uv_min, mvVec2 uv_max, mvColor tintColor, ImPlotYAxis_ axis)
			: 
			mvSeries(name, boundsMin, boundsMax, axis), 
			m_value(value), 
			m_bounds_min(boundsMin), 
			m_bounds_max(boundsMax), 
			m_uv_min(uv_min), 
			m_uv_max(uv_max),
			m_tintColor(tintColor)
		{
			mvApp::GetApp()->getTextureStorage().addTexture(m_value);
			mvTexture* texture = mvApp::GetApp()->getTextureStorage().getTexture(m_value);
			if (texture)
				m_texture = texture->texture;
		}

		bool onEvent(mvEvent& event)
		{
			mvEventDispatcher dispatcher(event);
			dispatcher.dispatch(BIND_EVENT_METH(mvImageSeries::onTextureDeleted), mvEVT_DELETE_TEXTURE);

			return event.handled;
		}

		bool onTextureDeleted(mvEvent& event)
		{
			std::string name = GetEString(event, "NAME");

			if (name == m_value)
			{
				m_texture = nullptr;
				return true;
			}

			return false;
		}

		mvSeriesType getSeriesType() override { return mvSeriesType::Image; }

		void draw() override
		{
			if(m_texture)
				ImPlot::PlotImage(m_name.c_str(), m_texture, m_bounds_min, m_bounds_max, m_uv_min, m_uv_max, m_tintColor.toVec4());
			else
			{
				mvApp::GetApp()->getTextureStorage().addTexture(m_value);
				mvTexture* texture = mvApp::GetApp()->getTextureStorage().getTexture(m_value);
				if (texture)
					m_texture = texture->texture;
			}
		}

		~mvImageSeries()
		{
			mvEventBus::Publish(mvEVT_CATEGORY_TEXTURE, mvEVT_DEC_TEXTURE, { CreateEventArgument("NAME", m_value) });
			mvEventBus::UnSubscribe(this);
		}

	private:

		std::string m_value;
		ImPlotPoint m_bounds_min;
		ImPlotPoint m_bounds_max;
		mvVec2	    m_uv_min = { 0.0f, 0.0f };
		mvVec2	    m_uv_max = { 1.0f, 1.0f };
		mvColor     m_tintColor = { 0, 0, 0, 255, true };
		void*       m_texture = nullptr;

	};

}