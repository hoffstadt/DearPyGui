#pragma once

#include "Core/mvAppItem.h"

namespace Marvel {

	class mvRadioButton : public mvAppItem
	{

	public:

		mvRadioButton(const std::string& parent, const std::string& name, const std::vector<std::string>& itemnames, int default_value)
			: mvAppItem(parent, name)
		{
			m_value = default_value;
			m_itemnames = itemnames;
		}

		virtual PyObject* getPyValue() override;

		virtual mvAppItemType getType() const override { return mvAppItemType::RadioButtons; }

		virtual void draw() override;

		inline int getValue() const { return m_value; }

	private:

		int m_value;
		std::vector<std::string> m_itemnames;

	};

}