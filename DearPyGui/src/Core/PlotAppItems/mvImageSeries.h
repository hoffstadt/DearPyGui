#pragma once
#include "mvPlot.h"
#include "mvCore.h"
#include "Core/mvTextureStorage.h"
#include "Core/mvPythonExceptions.h"

namespace Marvel {

	class mvImageSeries : public mvSeries
	{

	public:

		mvImageSeries(const std::string& name, const std::string& value, const ImPlotPoint& boundsMin, const ImPlotPoint& boundsMax,
			mvVec2 uv_min, mvVec2 uv_max, mvColor tintColor)
			: mvSeries(name, boundsMin, boundsMax), m_value(value), m_bounds_min(boundsMin), m_bounds_max(boundsMax), m_uv_min(uv_min), m_uv_max(uv_max),
			m_tintColor(tintColor)
		{
			mvTextureStorage::AddTexture(m_value);
			mvTexture* texture = mvTextureStorage::GetTexture(m_value);
			if (texture == nullptr)
			{
				PyErr_Format(PyExc_Exception,
					"Image %s could not be found for add_image. Check the path to the image "
					"you provided.", value.c_str());
				PyErr_Print();
			}
			m_texture = texture->texture;
		}

		mvSeriesType getSeriesType() override { return mvSeriesType::Image; }

		void draw() override
		{
			if(m_texture)
				ImPlot::PlotImage(m_name.c_str(), m_texture, m_bounds_min, m_bounds_max, m_uv_min, m_uv_max, m_tintColor.toVec4());
		}

		~mvImageSeries()
		{
			mvTextureStorage::DecrementTexture(m_value);
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