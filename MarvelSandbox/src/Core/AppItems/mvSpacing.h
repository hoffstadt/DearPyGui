#pragma once

#include "Core/mvAppItem.h"

namespace Marvel {

	class mvSpacing : public mvAppItem
	{

	public:

		mvSpacing(const std::string& parent, int count)
			: mvAppItem(parent, "Spacing")
		{
			m_value = count;
		}

		virtual PyObject* getPyValue() override { return nullptr; }

		virtual mvAppItemType getType() const override { return mvAppItemType::Spacing; }

		virtual void draw() override;

		inline int getValue() const { return m_value; }

	private:

		int m_value;

	};

}