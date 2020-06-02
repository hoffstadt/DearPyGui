#pragma once

#include "Core/mvAppItem.h"

namespace Marvel {

	class mvChild : public mvAppItem
	{

	public:

		mvChild(const std::string& parent, const std::string& name)
			: mvAppItem(parent, name), m_value(false)
		{
		}

		virtual PyObject* getPyValue() override;

		virtual mvAppItemType getType() const override { return mvAppItemType::Child; }

		virtual void draw() override;

		inline bool getValue() const { return m_value; }

	private:

		bool m_value;

	};

	class mvEndChild : public mvAppItem
	{

	public:

		mvEndChild(const std::string& parent)
			: mvAppItem(parent, "Endgroup")
		{
		}

		virtual PyObject* getPyValue() override { return nullptr; }

		virtual mvAppItemType getType() const override { return mvAppItemType::EndChild; }

		virtual void draw() override;

	};

}