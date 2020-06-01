#pragma once

#include "mvAppItem.h"

namespace Marvel {

	class mvInputText : public mvAppItem
	{

	public:

		mvInputText(const mvAppItemProps& props, const std::string& hint = "")
			: mvAppItem(props), m_hint(hint)
		{

		}

		virtual mvAppItemType getType() const override { return mvAppItemType::InputText; }

		virtual void draw() override;

		inline const std::string& getValue() const { return m_value; }

	private:

		std::string m_hint;
		std::string m_value;

	};

}