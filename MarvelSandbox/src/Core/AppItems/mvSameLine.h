#pragma once

#include "Core/mvAppItem.h"

namespace Marvel {

	class mvSameLine : public mvAppItem
	{

	public:

		mvSameLine(const std::string& parent, float xoffset, float spacing)
			: mvAppItem(parent, "SameLine"), m_xoffset(xoffset), m_spacing(spacing)
		{
		}

		virtual PyObject* getPyValue() override { return nullptr; }

		virtual mvAppItemType getType() const override { return mvAppItemType::SameLine; }

		virtual void draw() override;

	private:

		float m_xoffset;
		float m_spacing;

	};

}