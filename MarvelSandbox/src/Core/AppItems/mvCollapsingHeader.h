#pragma once

#include "Core/mvAppItem.h"

namespace Marvel {

	class mvCollapsingHeader : public mvAppItem
	{

	public:

		mvCollapsingHeader(const std::string& parent, const std::string& name)
			: mvAppItem(parent, name)
		{
			m_value = false;
		}

		virtual PyObject* getPyValue() override;

		virtual mvAppItemType getType() const override { return mvAppItemType::CollapsingHeader; }

		virtual void draw() override;

		inline bool getValue() const { return m_value; }

	private:

		bool m_value;

	};

}