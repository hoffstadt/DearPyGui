#pragma once

#include "mvAppItem.h"

namespace Marvel {

	class mvInputText : public mvAppItem
	{

	public:

		mvInputText(const std::string& parent, const std::string& name, const std::string& hint = "")
			: mvAppItem(parent, name), m_hint(hint)
		{
			m_value = new std::string();
		}

		virtual PyObject* getPyValue() override;

		virtual mvAppItemType getType() const override { return mvAppItemType::InputText; }

		virtual void draw() override;

		inline const std::string& getValue() const { return *m_value; }

	private:

		std::string m_hint;
		std::string* m_value;

	};

}